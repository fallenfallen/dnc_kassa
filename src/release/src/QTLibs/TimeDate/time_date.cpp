/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2009, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2009, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "time_date.h"

#include <malloc.h>
#include <string.h>


//
// Constructs a DigitalClock widget with a parent and a name.
//
/*---------------------------------------------------*/
// ВАРиАНТ КлАССА С миганием ФОНА ЧАСОВ
DigitalTime::DigitalTime(bool second_visible, QWidget *parent, const char *name)
    : QLCDNumber( parent, name )
{
      setSegmentStyle(QLCDNumber::Flat);
      cur_time = QTime::currentTime();
      if(second_visible)
        num_characters = 8;
      else
        num_characters = 5;
      setNumDigits(num_characters);
      
      color = paletteBackgroundColor();
      color_flag = true;
      blink_flag = false;

      showtimer = new QTimer(this);
      blinktimer = new QTimer(this);
      
      connect(showtimer, SIGNAL(timeout()), SLOT(showTime()));
      connect(blinktimer, SIGNAL(timeout()), SLOT(changeColor()));
}      
/*---------------------------------------------------*/
void DigitalTime::SetTime(QTime t, bool fl)
{
  cur_time = t;
  flag = fl;
  showTime();                                 // display the current time	
}

/*---------------------------------------------------*/

void DigitalTime::StartTime()
{
//puts("void DigitalTime::StartTime()");
  showtimer->start(500);
}
/*---------------------------------------------------*/
void DigitalTime::StopTime()
{
 // puts("void DigitalTime::StopTime()");
//puts("===============================");
  showtimer->stop();
}
/*---------------------------------------------------*/
void DigitalTime::BlinkTime(bool bl_flag)
{
  blink_flag = bl_flag;
  if(blink_flag)
    blinktimer->start(1000);
  else
  {
    blinktimer->stop();
    changeColor();
  }
}
/*---------------------------------------------------*/
void DigitalTime::changeColor()
{
//puts("void DigitalTime::changeColor()");

  if(blink_flag)
  {
    QColor col(126,18, 0);
    if(color_flag)
    {
      setPaletteBackgroundColor(col);
      setSegmentStyle(QLCDNumber::Outline);
    }
    else
    {
      setPaletteBackgroundColor(color);
      setSegmentStyle(QLCDNumber::Flat);
    }
    color_flag = !color_flag;
  }
  else
  {
    setPaletteBackgroundColor(color);
    setSegmentStyle(QLCDNumber::Flat);
  }
}
/*---------------------------------------------------*/
void DigitalTime::showTime()
{
  if(flag) cur_time = cur_time.addMSecs(500);
  else
  {
     cur_time = cur_time.addMSecs(-500);
  }
  QString s = cur_time.toString().left(num_characters);
  display(s);                               // set LCD number/text
}
/*---------------------------------------------------*/
/*
// ВАРИАНТ КЛАССА С МИГАНИЕМ ЦИФР ЧАСОВ
DigitalTime::DigitalTime( QWidget *parent, const char *name)
    : QLCDNumber( parent, name )
{
//    showingColon = FALSE;
  //  setFrameStyle( QFrame::Panel | QFrame::Raised );
 //   setLineWidth( 2 );                          // set frame line width
      setSegmentStyle(QLCDNumber::Flat);
      cur_time = QTime::currentTime();
      setNumDigits(8);
      normalTimer = startTimer(500);            // 1/2 second timer events
      colorTimer = startTimer(1000); 
      
      color = paletteForegroundColor();
      color_flag = true;
      blink_flag = false;
}      

void DigitalTime::SetTime(QTime t, bool fl)
{
  cur_time = t;
  flag = fl;
  showTime();                                 // display the current time	
}


//
// Handles timer events for the digital clock widget.
//
void DigitalTime::timerEvent( QTimerEvent *e )
{
  if(e->timerId() == normalTimer )
    showTime();
  if(e->timerId() == colorTimer )
    changeColor();
}

void DigitalTime::changeColor()
{
  if(blink_flag)
  {
    QColor col(Qt::red);
    if(color_flag)
      setPaletteForegroundColor(col);
    else
      setPaletteForegroundColor(color);
    color_flag = !color_flag;
  }
}

void DigitalTime::showTime()
{
  if(flag) cur_time = cur_time.addMSecs(500);
  else cur_time = cur_time.addMSecs(-500);

  QString s = cur_time.toString().left(8);
  display( s );                               // set LCD number/text
}
*/
/*---------------------------------------------------*/
DigitalDate::DigitalDate( QWidget *parent, const char *name)
    : QLCDNumber( parent, name )
{
//    showingColon = FALSE;
  //  setFrameStyle( QFrame::Panel | QFrame::Raised );
 //   setLineWidth( 2 );                          // set frame line width
      setSegmentStyle(QLCDNumber::Filled);
      setNumDigits(10);
      flag = true;
      normalTimer = startTimer( 500 );         // keep this state for 1/2 secs	
}
/*---------------------------------------------------*/
void DigitalDate::SetDate(QDate d, QTime t)
{
    cur_date = d;
    cur_time = t;
    showDate();
    flag = true;
}
/*---------------------------------------------------*/
void DigitalDate::timerEvent( QTimerEvent *e )
{
  if(e->timerId() == normalTimer )
    showDate();
}

///*---------------------------------------------------*/
// Shows the current date in the internal lcd widget.
void DigitalDate::showDate()
{
  cur_time.addMSecs(500);
  if((cur_time.hour() == 0) && (cur_time.minute() == 0))
  {
    cur_date.addDays(1);
    flag = true;
  }
  if(flag)
  {
    QString s, tmp;
    tmp.setNum(cur_date.day());
    if(cur_date.day() < 10)
      s = "0" + tmp + ".";
    else
      s = tmp + ".";
    tmp.setNum(cur_date.month());
    if(cur_date.month() < 10)
      s = s + "0" + tmp + ".";    
    else
      s = s + tmp + ".";
    tmp.setNum(cur_date.year());
    s = s + tmp;
    display(s);                               // sets the LCD number/text}}
    flag = false;
  }
}
/*---------------------------------------------------*/
