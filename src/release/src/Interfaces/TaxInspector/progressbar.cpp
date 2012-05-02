/*====================================================================================
# «ДЭНСИ:Налоговый инспектор» module of «ДЭНСИ: КАССА» complex is an additional utility, wich allows to use FM/ECRP functions.
# © OOO "DNC Trade" 2011, Authors: Romanow Alexey.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:Налоговый инспектор» программного комплекса «ДЭНСИ: КАССА» - дополнительная утилита, предоставляющая доступ к функциям работы с ФП/ЭКЛЗ. 
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Романов Алексей.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "progressbar.h"

ProgressBar::ProgressBar(QWidget * parent) : QWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));

    setWindowFlags(Qt::SplashScreen | Qt::WindowStaysOnTopHint);
    resize(300, 75);

    init();
}

ProgressBar::ProgressBar(int width, int height, QWidget * parent) : QWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));

    setWindowFlags(Qt::SplashScreen);
    resize(300, 80);

    init(width, height);
}

void ProgressBar::init(int w, int h)
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
    timer->stop();
    delete timer;
}

void ProgressBar::startTimer()
{
    timer->start(100);
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

void ProgressBar::timeout()
{
    repaint();
}

Progress::Progress(QObject * parent)
{
    progress = new ProgressBar;

    connect(this, SIGNAL(setOperationDescription(QString)), progress, SLOT(setOperationDescription(QString)));
}

Progress::~Progress()
{
    delete progress;
}

void Progress::finished()
{
    progress->hide();
}
