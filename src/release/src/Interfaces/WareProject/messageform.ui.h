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
//#include <malloc.h>
extern SP sp;
extern BlockTimer *bt;
 
//======================================================================//
void messageForm::init()
{
puts("void messageForm::init()");
  grabKeyboard();
  connect(closePushButton, SIGNAL(clicked()), SLOT(close()));
  adjustSize();
  if(sp.autoblock_flag) bt->action_time = QTime::currentTime();
}



//======================================================================//
void messageForm::destroy()
{
puts("void messageForm::destroy()");
  releaseKeyboard();
 //DefCodec = QTextCodec::codecForName("UTF8");
}

//======================================================================//
void messageForm::SetText(QString str)
{
puts("void messageForm::SetText(QString str)");
  errorTextLabel->setText(str);
  adjustSize();
}

//======================================================================//
void messageForm::keyPressEvent(QKeyEvent *k)
{
puts("void messageForm::keyPressEvent(QKeyEvent *k)");
  if(sp.autoblock_flag) bt->action_time = QTime::currentTime();
  switch(k->key())
  {
    case Qt::Key_Enter :
      close();
      break;
    case Qt::Key_Return :
      close();
      break;
    case Qt::Key_F12 :
      close();
      break;
  }
}
