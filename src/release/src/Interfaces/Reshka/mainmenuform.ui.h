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

#include  <qfocusdata.h>

#include "aboutform.h"

extern int ret; 

//======================================================================//
void mainmenuForm::init()
{
    //puts("void mainmenuForm::init()");
    DefCodec = QTextCodec::codecForName(SYSCODEC);  
    
    QString true_caption = DefCodec->toUnicode(PROGRAMM_VERSION);
    programmversionTextLabel->setText(true_caption);
    
    exec_flag = false;  
    
    button_collection[0] = (QWidget*) registrationPushButton;
    button_collection[1] = (QWidget*) servicePushButton;
    button_collection[2] = (QWidget*) usersetupPushButton;
    button_collection[3] = (QWidget*) warePushButton;
    button_collection[4] = (QWidget*) setupPushButton;
    button_collection[5] = (QWidget*) driversetupPushButton;
    button_collection[6] = (QWidget*) exchangesetupPushButton;
    button_collection[7] = (QWidget*) changeuserPushButton;
    
    cur_button_num = 0;
    lost_button = 1;
    focus_num = 0;
    
    connect(button_collection[0], SIGNAL(clicked()), SLOT(SetCurButton0()));
    connect(button_collection[1], SIGNAL(clicked()), SLOT(SetCurButton1()));
    connect(button_collection[2], SIGNAL(clicked()), SLOT(SetCurButton2()));
    connect(button_collection[3], SIGNAL(clicked()), SLOT(SetCurButton3()));
    connect(button_collection[4], SIGNAL(clicked()), SLOT(SetCurButton4()));
    connect(button_collection[5], SIGNAL(clicked()), SLOT(SetCurButton5()));
    connect(button_collection[6], SIGNAL(clicked()), SLOT(SetCurButton6()));
    connect(button_collection[7], SIGNAL(clicked()), SLOT(SetCurButton7()));	
    
    
    
    for ( int i = 0; i< buttons_qtty; i++ )  {
	connect(button_collection[i], SIGNAL(clicked()), SLOT(ButtonClick()) );
    }
    
    grabKeyboard();
    
    button_collection[cur_button_num]->setFocus();
    setSelection(button_collection[cur_button_num]);
    lost_button = cur_button_num;
    
}	
//======================================================================//
void mainmenuForm::paintEvent(QPaintEvent * pe)
{
    //printf("this->focusData()->focusWidget().name() = %s \n", this->focusData()->focusWidget()->name());
    //printf("cur_button_num = %d \n", cur_button_num);
    unsetSelection(button_collection[lost_button]);
    setSelection(button_collection[cur_button_num]);
    lost_button = cur_button_num;
}
//======================================================================//
void mainmenuForm::SetCurButton0() {  cur_button_num = 0;  }
void mainmenuForm::SetCurButton1() {  cur_button_num = 1;  }
void mainmenuForm::SetCurButton2() {  cur_button_num = 2;  }
void mainmenuForm::SetCurButton3() {  cur_button_num = 3;  }
void mainmenuForm::SetCurButton4() {  cur_button_num = 4;  }
void mainmenuForm::SetCurButton5() {  cur_button_num = 5;  }
void mainmenuForm::SetCurButton6() {  cur_button_num = 6;  }
void mainmenuForm::SetCurButton7() {  cur_button_num = 7;  }
//======================================================================//
void mainmenuForm::ButtonClick()
{
    //printf("void mainmenuForm::ButtonClick()\n");
    //printf("cur_button_num = %d \n", cur_button_num);      
    
    if ( ! button_collection[cur_button_num]->isEnabled() ) return;
    focus_num = cur_button_num;
    exec_flag = true;
    releaseKeyboard();
    //puts("accept");
    close();
}
	
//======================================================================//
void mainmenuForm::setSelection(QWidget *k)
{
    k->setFocus();
    QFont f = k->font();
    f.setPointSize(16);
    k->setFont(f);
}
//======================================================================//
void mainmenuForm::unsetSelection(QWidget *k)
{
    QFont f = k->font();
    f.setPointSize(10);
    k->setFont(f);
}
//====================================================================//
void mainmenuForm::Error(int er)
{
//puts("mainmenuForm::Error(in er)");
  releaseKeyboard();
  QMessageBox erBox("Error", DefCodec->toUnicode("блаблабла"), QMessageBox::Warning, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
  
  erBox.setButtonText( QMessageBox::Ok, DefCodec->toUnicode("Закрыть"));
  
  switch(er)
  {
    case 1 : 
    { 
      erBox.setText(DefCodec->toUnicode("Ошибка 1 :\nНе удалось запустить подпрограмму\n"));
      break;
    }
  }
  erBox.exec();
  grabKeyboard();
}
//======================================================================//
void mainmenuForm::ExitFromProgramm()
{
    releaseKeyboard();
    exitForm * exitf = new exitForm;
    exitf->exec();
    delete(exitf);
    if (ret == 1)  {
	focus_num = -1;
	close();
    }
    grabKeyboard();
}
//======================================================================//
void mainmenuForm::NextFocus()
{
    //printf("NextFocusw() cur_button_num = %d \n", cur_button_num);
    
    unsetSelection(button_collection[lost_button]);
    cur_button_num++;
    if ( cur_button_num ==  buttons_qtty ) cur_button_num = 0;
    while ( ! button_collection[cur_button_num]->isEnabled() ) {
	cur_button_num++;
    }
    setSelection(button_collection[cur_button_num]);
    lost_button = cur_button_num;
    //focusNextPrevChild(false);
}
//======================================================================//
void mainmenuForm::PrevFocus()
{
    //printf("PrevFocus ()  cur_button_numq == %d \n", cur_button_num);
    
    unsetSelection(button_collection[lost_button]);
    cur_button_num--;
    if ( cur_button_num < 0 ) cur_button_num = buttons_qtty -1;
    while ( ! button_collection[cur_button_num]->isEnabled() ) cur_button_num--;
    setSelection(button_collection[cur_button_num]);
    lost_button = cur_button_num;
    //focusNextPrevChild(true);
}
//======================================================================//
void mainmenuForm::keyPressEvent(QKeyEvent *k)
{
    switch ( k->key() )  {
    case Qt::Key_Enter :    
	ButtonClick();
	break;
    case Qt::Key_Return :
	ButtonClick();
	break;
    case Qt::Key_F1 :
	AboutProgramm();
	break;	
    case Qt::Key_Up :
	PrevFocus();
	break;
    case Qt::Key_Down :
	NextFocus();
	break;
    case Qt::Key_Left :
	PrevFocus();
	break;
    case Qt::Key_Right :
	NextFocus();
	break;
    case Qt::Key_F12 : 
	ExitFromProgramm();
	break;	
    }
    
    if ( k->key() < Qt::Key_1 || k->key() > (Qt::Key_1 + buttons_qtty - 1) ) {
	return;
    } else {
	cur_button_num = k->key() - Qt::Key_1;
	ButtonClick();
    }
    
}

//====================================================================//
void mainmenuForm::AboutProgramm()
{
    releaseKeyboard();
    AboutForm * af = new AboutForm();
    af->exec();
    delete af;
    grabKeyboard();
}
//====================================================================//
