#ifndef CALENDAR_H
#define CALENDAR_H

#include <QCalendarWidget>
#include <QMouseEvent>
#include <QKeyEvent>

class Calendar : public QCalendarWidget
{
    Q_OBJECT

public:
    Calendar(QWidget * parent = 0);

};

#endif // CALENDAR_H
