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

#include "blocktimer.h"


/*******************************************************************/
BlockTimer::BlockTimer(int abt) 
{
//    puts("BlockTimer::BlockTimer(abt = %d)\n", abt);
  autoblock_time = abt;
  block_flag = false;
    
  block_timer = new QTimer(this, "BlockTimer");
  connect(block_timer, SIGNAL(timeout()), SLOT(CheckForBlock()));
}
/*******************************************************************/
BlockTimer::~BlockTimer()
{
//puts("BlockTimer::~BlockTimer()");
  block_timer->stop();
  disconnect(block_timer, SIGNAL(timeout()));
}
/*******************************************************************/
void BlockTimer::CheckForBlock()
{
//puts("void BlockTimer::CheckForBlock()");
  int time_for_check = autoblock_time - action_time.secsTo(QTime::currentTime());
//printf("%d\n%d\n", action_time.secsTo(QTime::currentTime()), time_for_check);
  if(time_for_check > 0)
    block_timer->start(time_for_check * 1000, true);
  else
  {
      block_timer->stop();
      block_flag = true;
  }
}
/*******************************************************************/
void BlockTimer::Start()
{
//puts("void BlockTimer::Start()");
  block_timer->start(autoblock_time * 1000, true);
  action_time = QTime::currentTime();
}
/*******************************************************************/
void BlockTimer::Stop()
{
//puts("void BlockTimer::Stop()");
  block_timer->stop();
  block_flag = false;
}
/*******************************************************************/
