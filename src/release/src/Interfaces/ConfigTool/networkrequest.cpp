#include "networkrequest.h"

#include <QFuture>
#include <QPainter>
#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>

ProgressBar::ProgressBar(QWidget * parent) : QWidget(parent)
{
    setWindowFlags(Qt::SplashScreen | Qt::WindowStaysOnTopHint);
    resize(300, 75);

    initWidgets();
}

ProgressBar::ProgressBar(int width, int height, QWidget * parent) : QWidget(parent)
{
    setWindowFlags(Qt::SplashScreen);
    resize(300, 80);

    initWidgets(width, height);
}

void ProgressBar::initWidgets(int w, int h)
{
    count = 0;
    progressItemWidth = w;
    progressItemHeight = h;

    borderRect = QRect(10, height() - 35, width() - 20, progressItemHeight + 4);
    progressRects = generateProgressRects(5);
    gradient = QLinearGradient(borderRect.topLeft(), borderRect.bottomRight());
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, Qt::green);
}

ProgressBar::~ProgressBar()
{
    stop();
}

void ProgressBar::start()
{
    timer = startTimer(100);
}

void ProgressBar::stop()
{
    killTimer(timer);
}

void ProgressBar::setOperationDescription(const QString &desc)
{
    description = desc;
}

QString points(int &count)
{
    count = count % 9 + 1;
    return QString().fill('.', count / 3);
}

QList<QRect> ProgressBar::generateProgressRects(int count)
{
    QList<QRect> rects;

    for (int i = 0; i < count; ++i)
    {
        QRect rect(borderRect.left() + (progressItemWidth + 2) * i + 2, borderRect.top() + 2, progressItemWidth, progressItemHeight);
        rects.append(rect);
    }

    return rects;
}

void ProgressBar::moveProgressRect()
{
    if (progressRects[0].left() > borderRect.right())
    {
        int right = 0;
        for (QList<QRect>::iterator iter = progressRects.end() - 1; iter >= progressRects.begin(); --iter)
        {
            (*iter).moveRight(right);
            right -= (*iter).width() + 2;
        }
    }

    for (QList<QRect>::iterator iter = progressRects.begin(); iter != progressRects.end(); iter++)
    {
        int right = (*iter).right();
        (*iter).moveRight(right + 5);
    }
}

void ProgressBar::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);

    painter.setPen(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawText(10, 16, conv("Выполнение операции") + points(count));
    if (!description.isEmpty())
        painter.drawText(10, 32, description);
    painter.drawRect(borderRect);
    moveProgressRect();

    QPainterPath path;
    foreach (QRect rect, progressRects)
        path.addRoundedRect(rect, 2, 2);

    painter.setClipRect(borderRect, Qt::ReplaceClip);
    painter.setBrush(gradient);
    painter.drawPath(path);
}

void ProgressBar::timerEvent(QTimerEvent * event)
{
    repaint();
}

void ProgressBar::closeEvent(QCloseEvent * event)
{
    stop();
}

NetworkRequest::NetworkRequest()
{
    manager = new QNetworkAccessManager;
}

NetworkRequest::~NetworkRequest()
{
    delete manager;
}

void NetworkRequest::proxyRequest(int type, const QString &host, int port, const QString &user, const QString &password)
{
    QNetworkProxy proxy((QNetworkProxy::ProxyType)type, host, port, user, password);
    QString url = "http://discountmobile.mobi/src/s.php";
    makeRequest(url, proxy);
}

void NetworkRequest::requestFinished()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender());
    emit requestResult(!reply->error());
}

void NetworkRequest::makeRequest(const QString &url, const QNetworkProxy &proxy)
{
    manager->setProxy(proxy);
    QNetworkReply * reply = manager->get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), SLOT(requestFinished()));
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
}

