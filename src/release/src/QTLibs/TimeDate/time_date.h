#ifndef DCLOCK_AND_DATE_H
#define DCLOCK_AND_DATE_H

#include <qlcdnumber.h>
#include <qdatetime.h>
#include <qpainter.h>
#include <qtimer.h>

/**********************************************************************/
class DigitalTime : public QLCDNumber          // digital clock widget
{
    Q_OBJECT
private :                                        // internal data
    
    bool flag;
   // int normalTimer;
    // int colorTimer;

    bool color_flag;
    QColor color;

    int num_characters;
    
    QTime cur_time;
    //bool blink_flag;
    
    QTimer * showtimer;
    QTimer * blinktimer;
    
public :
    bool blink_flag;
    //DigitalTime(QWidget *parent=0, const char *name=0);
    DigitalTime(bool second_visible = false, QWidget *parent=0, const char *name=0);
  //  SetDate(QDate d);
    void SetTime(QTime t, bool fl = true); //fl = false - обратный отсчет времени
    void StartTime();
    void StopTime();

    void BlinkTime(bool bl_flag);
protected :                                      // event handlers    
    //void  timerEvent( QTimerEvent *);
    //void drawContents (QPainter *p);
private slots :                                  // internal slots
    void showTime();
    void changeColor();


};
/**********************************************************************/
class DigitalDate : public QLCDNumber          // digital clock widget
{
    Q_OBJECT
public:
    DigitalDate(QWidget *parent=0, const char *name=0);
    void SetDate(QDate d, QTime t);

protected:                                      // event handlers
    void        timerEvent( QTimerEvent * );

private slots:                                  // internal slots

private:                                        // internal data
    void        showDate();
    
    bool        showingColon;
    int         normalTimer;
    //int         showDateTimer; 
    bool flag; // true - обновить вывод даты
    QDate cur_date;
    QTime cur_time;
};
/**********************************************************************/
#endif

