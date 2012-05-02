#ifndef NETWORKREQUEST_H
#define NETWORKREQUEST_H

#include <QWidget>
#include <QThread>
#include <QTimerEvent>
#include <QPaintEvent>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QLinearGradient>
#include <QNetworkAccessManager>

#include "exception.h"

class ProgressBar : public QWidget
{
    Q_OBJECT

public:
    ProgressBar(QWidget * parent = 0);
    ProgressBar(int width, int height, QWidget * parent = 0);
    ~ProgressBar();

    void start();
    void stop();

public slots:
    void setOperationDescription(const QString &desc);

protected:
    void paintEvent(QPaintEvent * event);
    void timerEvent(QTimerEvent * event);
    void closeEvent(QCloseEvent * event);

private:
    void initWidgets(int w = 25, int h = 21);
    QList<QRect> generateProgressRects(int count);
    void moveProgressRect();

    int timer;
    int count;
    int progressItemWidth;
    int progressItemHeight;
    QString description;
    QRect borderRect;
    QLinearGradient gradient;
    QList<QRect> progressRects;
};

class NetworkRequest : public QObject
{
    Q_OBJECT

public:
    NetworkRequest();
    ~NetworkRequest();

signals:
    void information(Information * information);
    void requestResult(int result);

public slots:
    void proxyRequest(int type, const QString &host, int port,
                      const QString &user, const QString &password);

private slots:
    void requestFinished();

private:
    void makeRequest(const QString &url, const QNetworkProxy &proxy);

    QNetworkAccessManager * manager;

};

#endif // NETWORKREQUEST_H
