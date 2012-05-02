/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2009, Authors: Savrasov Alexandr.
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
#include "hwsrv.h"

extern HWSRV  *hw;

void  EWTest::init()
{
	 QObject::connect (FinishButton, SIGNAL(clicked()),this, SLOT(close()));
	 grabKeyboard();  
}
void EWTest::closeEvent( QCloseEvent* e )
{
//  puts("void EWTest::closeEvent( QCloseEvent* e )");
  hw->EW_Stop();  
  releaseKeyboard();
  e->accept();
  return;
}

void EWTest::ShowWeight( void )
{
  //  printf("void EWTest::ShowWeight( )\n");
    WeightLabel->display(hw->Weight);
    textLabel2->setText(QString::number(hw->Weight));
}

void EWTest::StartTest(int Type, int LogNum, int Port, int BaudRate, int Model )
{
   // puts("void EWTest::StartTest( int Port, int BaudRate, int Model )");
    hw->EW_SetConnectOptions(Type, LogNum, Port,BaudRate,Model);
}

void EWTest::keyPressEvent(QKeyEvent *k)
{
  switch(k->key())   
    {
    case Qt::Key_F12 :
	close();
	break;
 	
    default :{}
    }
}
