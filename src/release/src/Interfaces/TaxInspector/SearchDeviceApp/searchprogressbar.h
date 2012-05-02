#ifndef SEARCHPROGRESSBAR_H
#define SEARCHPROGRESSBAR_H

#include <QWidget>
#include <QFontMetricsF>
#include <QPainter>
#include <QPaintEvent>
#include <QLinearGradient>
#include <QFutureWatcher>
#include <QtConcurrentRun>

class SearchProgressBar : public QWidget
{
    Q_OBJECT

public:
    SearchProgressBar(QWidget * parent = 0);
    ~SearchProgressBar();

public slots:
    void setOperationDescription(const QString &description);
    void setProgressValue(double value);
    void setValueRange(double min, double max);

protected:
    void paintEvent(QPaintEvent * event);

private:
    double min, max;
    double scale;
    double percentage;
    QString description;

};

class SearchProgress : public QObject
{
    Q_OBJECT

public:
    template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
    T run(Class * object, T (Class::*fn)(Param1, Param2, Param3, Param4, Param5), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
    {
        QFutureWatcher<T> * watcher = new QFutureWatcher<T>;

        connect(watcher, SIGNAL(finished()), SIGNAL(finished()));
        connect(watcher, SIGNAL(canceled()), SIGNAL(finished()));

        QFuture<T> future = QtConcurrent::run(object, fn, arg1, arg2, arg3, arg4, arg5);
        watcher->setFuture(future);
    }

signals:
    void finished();

};

#endif // SEARCHPROGRESSBAR_H
