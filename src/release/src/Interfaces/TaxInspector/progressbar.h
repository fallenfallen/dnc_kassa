#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QWidget>
#include <QTimer>
#include <QThread>
#include <QPainter>
#include <QPaintEvent>
#include <QLinearGradient>

#include <QFutureWatcher>
#include <QtConcurrentRun>

#include "translator.h"

class ProgressBar : public QWidget
{
    Q_OBJECT

public:
    ProgressBar(QWidget * parent = 0);
    ProgressBar(int width, int height, QWidget * parent = 0);
    ~ProgressBar();

public slots:
    void startTimer();
    void setOperationDescription(const QString &desc);

private slots:
    void timeout();

protected:
    void paintEvent(QPaintEvent * event);

private:
    void init(int w = 25, int h = 21);
    QList<QRect> generateProgressRects(int count);
    void moveProgressRect();

    int count;
    int progressItemWidth;
    int progressItemHeight;
    QString description;
    QRect borderRect;
    QLinearGradient gradient;
    QList<QRect> progressRects;
    QTimer * timer;
};

class Progress : public QObject
{
    Q_OBJECT

public:
    Progress(QObject * parent = 0);
    ~Progress();

    template <typename T, typename Class, typename Param1, typename Arg1>
    T run(Class * object, T (Class::*fn)(Param1), const Arg1 &arg1)
    {
        QFutureWatcher<T> * watcher = new QFutureWatcher<T>;
        QObject * obj = (QObject *)object;
        connect(obj, SIGNAL(cancel()), watcher, SLOT(cancel()));
        connect(watcher, SIGNAL(finished()), this, SLOT(finished()));
        connect(watcher, SIGNAL(finished()), this, SIGNAL(signalFinished()));

        progress->startTimer();
        progress->show();
        QFuture<T> future = QtConcurrent::run(object, fn, arg1);
        watcher->setFuture(future);
    }

signals:
    void setOperationDescription(const QString &desc);
    void signalFinished();

private slots:
    void finished();

private:
    ProgressBar * progress;

};

#endif // PROGRESSBAR_H
