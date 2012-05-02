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
/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

bool cn_flag;

extern SP sp;
extern BlockTimer *bt;

extern HWSRV * hw;

//======================================================================//
void synchronizationForm::init()
{
    //puts"void synchronizationForm::init()");
    grabKeyboard();
    
    kkm_connect = true;
    
    pk_date = new DigitalDate(this->pkFrame);
    kkm_date = new DigitalDate(this->kkmFrame);
    pk_time = new DigitalTime(true, this->pkFrame);
    kkm_time = new DigitalTime(true, this->kkmFrame);
    
    kkm_date->setGeometry(140, 10, 100, 30);
    kkm_time->setGeometry(140, 50, 100, 30);
    
    kkmdateTextLabel->setGeometry(10, 10, 120, 30);
    kkmtimeTextLabel->setGeometry(10, 10, 120, 30);
    
    pk_date->setGeometry(140, 10, 100, 30);
    pk_time->setGeometry(140, 50, 100, 30); 
    
    //  pkdateTextLabel->setGeometry(10, 10, 120, 30);
    //  pktimeTextLabel->setGeometry(10, 10, 120, 30);
    
    
    pk_date->setMinimumWidth(100);
    pk_date->setMinimumHeight(30);
    pk_date->setMaximumWidth(100);
    pk_date->setMaximumHeight(30);
    
    pk_time->setMinimumWidth(100);
    pk_time->setMinimumHeight(30);
    pk_time->setMaximumWidth(100);
    pk_time->setMaximumHeight(30);
    
    kkm_date->setMinimumWidth(100);
    kkm_date->setMinimumHeight(30);
    kkm_date->setMaximumWidth(100);
    kkm_date->setMaximumHeight(30);
    
    kkm_time->setMinimumWidth(100);
    kkm_time->setMinimumHeight(30);
    kkm_time->setMaximumWidth(100);
    kkm_time->setMaximumHeight(30);
    
    
    pkframeLayout = new QGridLayout(this->pkFrame, 2, 2, 0, 10, "pkframeLayout");
    kkmframeLayout = new QGridLayout(this->kkmFrame, 2, 2, 30, 20, "kkmframeLayout");
    
    QRect framelayoutRect(10, 10, 320, 100);
    kkmframeLayout->setGeometry(framelayoutRect);
    
    pkframeLayout->addWidget(pktimeTextLabel, 0, 0);
    pkframeLayout->addWidget(pkdateTextLabel, 1, 0);  
    pkframeLayout->addWidget(pk_time, 0, 1);
    pkframeLayout->addWidget(pk_date, 1, 1);
    
    kkmframeLayout->addWidget(kkmtimeTextLabel, 0, 0);
    kkmframeLayout->addWidget(kkmdateTextLabel, 1, 0);  
    kkmframeLayout->addWidget(kkm_time, 0, 1);
    kkmframeLayout->addWidget(kkm_date, 1, 1);
    
    if(sp.autoblock_flag) bt->action_time = QTime::currentTime();
    
    
    QTime t_kkm;
    QDate d_kkm;
    int res = hw->ECR_ReadStatus();
    if ( res != 0 )  {
	Kkm_Error(res);
	kkm_connect = false;
    }
    else  {
	// ЗАПОЛНЕНИЕ ВРЕМЕНИ
	res = hw->ECR_ReadStatus();
	t_kkm.setHMS(hw->ECR_GetHour(), hw->ECR_GetMinute(), hw->ECR_GetSecond());
	kkm_time->SetTime(t_kkm);
	kkm_time->StartTime();
	pk_time->SetTime(QTime::currentTime());
	pk_time->StartTime();
	
	//ЗАПОЛНЕНИЕ ДАТЫ
	res = hw->ECR_ReadStatus();
	int y = hw->ECR_GetYear();
	if ( y < 100 ) {
	    y = QDate::currentDate().year() - (QDate::currentDate().year() % 100) + y;
	}
	d_kkm.setYMD(y, hw->ECR_GetMonth(), hw->ECR_GetDay());
	t_kkm.setHMS(hw->ECR_GetHour(), hw->ECR_GetMinute(), hw->ECR_GetSecond());
	kkm_date->SetDate(d_kkm, t_kkm);
	pk_date->SetDate(QDate::currentDate(), QTime::currentTime());
	
    }
    
    connect(syncPushButton, SIGNAL(clicked()), SLOT(Synchronize()));
    
}
//======================================================================//
void synchronizationForm::destroy()
{
//puts"void synchronizationForm::destroy()");
  releaseKeyboard();
}

//======================================================================//
void synchronizationForm::Synchronize()
{
    //puts("void synchronizationForm::Synchronize()");
  int res;
  QDate d = QDate::currentDate();
  res = hw->ECR_SetDate(d.day(), d.month(), d.year() - 2000);
  if(res != 0)  Kkm_Error(res);
  else
  {
  //printf("SYNC RESULT %d\n", res);
    //ПРОВЕРКА НА ПРАВИЛЬНОСТЬ СИНХРОНИЗАЦИИ ВРЕМЕНИ
//    if(sync_date_flag && ((sderror = res) == 0))
//      d_synchronized_flag = true;
  
    QTime t = QTime::currentTime();
    res = hw->ECR_SetTime(t.hour(), t.minute(), t.second());
    if(res != 0)  Kkm_Error(res);
    else
    {
      emit Sync();
  //ПРОВЕРКА НА ПРАВИЛЬНОСТЬ СИНХРОНИЗАЦИИ ВРЕМЕНИ
    //  if(sync_time_flag && ((sterror = res) == 0)) 
//        t_synchronized_flag = true;
  
  //ВЫВОДИМ НОВЫЕ ДАТУ И ВРЕМЯ
      QTime t_kkm;
      QDate d_kkm;
      res = hw->ECR_ReadStatus();
      t_kkm.setHMS(hw->ECR_GetHour(), hw->ECR_GetMinute(), hw->ECR_GetSecond());
      kkm_time->SetTime(t_kkm);
      kkm_time->StartTime();
      pk_time->SetTime(QTime::currentTime());
      pk_time->StartTime();
      
      res = hw->ECR_ReadStatus();
      int y = hw->ECR_GetYear();
      if ( y < 100 ) {
	  y = QDate::currentDate().year() - (QDate::currentDate().year() % 100) + y;
      }
      d_kkm.setYMD(y, hw->ECR_GetMonth(), hw->ECR_GetDay());
      
//printf("***********\n%d.%d.%d***********\n", hw->ECR_GetYear(), hw->ECR_GetMonth(), hw->ECR_GetDay());
      t_kkm.setHMS(hw->ECR_GetHour(), hw->ECR_GetMinute(), hw->ECR_GetSecond());
      kkm_date->SetDate(d_kkm, t_kkm);
    }
  }
}

//======================================================================//
void synchronizationForm::Cancel()
{
//puts"void synchronizationForm::Cancel()");
  if(kkm_time) delete(kkm_time);
  if(kkm_date) delete(kkm_date);
  if(pk_time) delete(pk_time);
  if(pk_date) delete(pk_date);
  close();
}

//======================================================================//
void synchronizationForm::keyPressEvent(QKeyEvent *k)
{
//puts"void synchronizationForm::keyPressEvent(QKeyEvent *k)");
  if(sp.autoblock_flag) bt->action_time = QTime::currentTime();
  switch(k->key())
  {
    case Qt::Key_Enter :
    {
      QWidget *pb;
      pb = (QWidget *) syncPushButton;
      if(pb->hasFocus()) Synchronize();
      else Cancel();
      break;
    }
    case Qt::Key_Return :
    {
      QWidget *pb;
      pb = (QWidget *) syncPushButton;
      if(pb->hasFocus()) Synchronize();
      else Cancel();
      break;
    }
    case Qt::Key_F12 :
    {
      Cancel();
    }
    case Qt::Key_Left :
    {
      SetSelectionLeft();
      break;
    }
    case Qt::Key_Right :
    {
      SetSelectionRight();
      break;
    }
  }
}

//======================================================================//
void synchronizationForm::SetSelectionLeft()
{
//puts"void synchronizationForm::SetSelectionLeft()");
  QWidget *pb;
  pb = (QWidget *) cancelPushButton;
  if(pb->hasFocus())
  {
    pb = (QWidget *) syncPushButton;
    pb->setFocus();
  }
}

//======================================================================//
void synchronizationForm::SetSelectionRight()
{
//puts"void synchronizationForm::SetSelectionRight()");
  QWidget *pb;
  pb = (QWidget *) syncPushButton;
  if(pb->hasFocus())
  {
    pb = (QWidget *) cancelPushButton;
    pb->setFocus();
  }
}

//====================================================================//
void synchronizationForm::Kkm_Error(int er)
{
//puts"synchronizationForm::Kkm_Error(int er)");
    releaseKeyboard();
  
    QString mes_id, mes_str;
    hw->ECR_ReadError(er);
    mes_id = hw->ECR_GetErrNum();
    mes_str = hw->ECR_GetErrDescription();
    kkm_Error(mes_id, mes_str);
    if ((er == -1) || (er == -2) || (er == 2))
      cn_flag = false;
 
    grabKeyboard(); 
}
