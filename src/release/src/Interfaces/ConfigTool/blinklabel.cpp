#include "blinklabel.h"

#include <QDebug>

BlinkLabel::BlinkLabel() : QLabel(), text() {}

BlinkLabel::BlinkLabel(const QString &text) : QLabel(text), text(text) {}

void BlinkLabel::start(int interval, int count)
{
    blinked = 0;
    blinkCount = count;
    timer = startTimer(interval);
}

void BlinkLabel::stop()
{
    QString value = QString("<FONT COLOR=#3C3C3C>%1</FONT>").arg(text);
    setText(value);

    if (timer != 0)
        killTimer(timer);
}

void BlinkLabel::timerEvent(QTimerEvent * event)
{
    blinked++;
    if (blinked < blinkCount)
    {
        QString color = blinked % 2  ? "#FF0000" : "#3C3C3C";
        QString value = QString("<FONT COLOR=%1>%2</FONT>").arg(color).arg(text);
        setText(value);
    }
    else
        stop();
}
