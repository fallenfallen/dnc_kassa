#ifndef BLINKLABEL_H
#define BLINKLABEL_H

#include <QLabel>
#include <QTimerEvent>

class BlinkLabel : public QLabel
{
    Q_OBJECT

public:
    BlinkLabel();
    BlinkLabel(const QString &text);

    void start(int interval, int count);
    void stop();

protected:
    void timerEvent(QTimerEvent * event);

private:
    int blinkCount;
    int blinked;
    int timer;
    QString text;

};

#endif // BLINKLABEL_H
