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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <signal.h>
#include <qgroupbox.h> 
#include <qlocale.h> 
//#include "configurations.h"

#include "const.h"
#include "hwsrv.h"
#include "addon_func.h"
//#include "dncdkbforrussymbol.h"
#include "messages.h"

using namespace std;

extern HWSRV  *hw;
//extern DKB_FOR_RUS_SYMBOL * dkb_for_rus_symbol;

static Display* gd = XOpenDisplay(NULL);
//extern Configurations * w;

void MCRTest::init()
{
    //puts("void MCRTest::init()");
    DefCodec = QTextCodec::codecForName(SYSCODEC);
    save_flag = false;
    first_track_flag = false;
    second_track_flag = false;
    third_track_flag = false;
    Type = 0;
    timer_flag = false;
    connect( Save, SIGNAL(clicked()),this , SLOT(SavePrefSuf()));
    connect( closeButton, SIGNAL(clicked()),this , SLOT(close()));
    
   // puts("finish init");
}



void MCRTest::MyInitRS232(void)
{
   // puts("void MCRTest::MyInitRS232(void)");
    QString er_str;
    QString er_id;
    int k = 0;
    ErrNum = 0;
   //printf(" PortNumber =  %d\n", PortNumber);
   // printf(" BaudRate =  %d\n", BaudRate);
    k =  hw->MCR_SetConnectOptions(Type, 1, PortNumber, BaudRate, 0,  0, 0, 0);
    if (k == -1 )
    {
	ErrNum = 3; // can't open port
	DeviceConfErrorText(14, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
    }
}


void MCRTest::MyInitKlavaPort(void)
{
    grabKeyboard(); 
    /*pref1_agree = false;
    pref2_agree = false;
    suf1_agree = false;
    suf2_agree = false;
    col_suf = 0;
    col_pref = 0;*/
    
    printf("Pref1 = %d \n", Pref1);
    printf("Pref2 = %d \n", Pref2);
    printf("Suf1 = %d \n", Suf1);
    printf("Suf2 = %d \n", Suf2);
    
    connect(kbcode_reader.timer, SIGNAL(timeout()), this, SLOT(TimeOut()));
    if (Pref1+Pref2+Suf1+Suf2 > 0) {
	kbcode_reader.setPrefSuf(Pref1, Pref2, Suf1, Suf2);
    }
    
}

void MCRTest::closeEvent( QCloseEvent* e )
{
   // puts("void MCRTest::closeEvent( QCloseEvent* e )");
    hw->MCR_Stop();
    e->accept();
    //w->show();
    return;
}

void MCRTest::makeKeyPress(QKeyEvent * k)
{
    switch (k->key()) {
    case Qt::Key_Enter :
	//printf("Qt::Key_Enter");
	if ( Save->hasFocus() ) SavePrefSuf();
	if ( closeButton->hasFocus() ) { close(); reject(); }
	break;
    case Qt::Key_Return :
	//printf("Qt::Key_Return");
	if ( Save->hasFocus() ) SavePrefSuf();
	if ( closeButton->hasFocus() ) { close(); reject(); }
	break;
    case Qt::Key_Space :
	//printf("Qt::Key_Return");
	if ( Save->hasFocus() ) SavePrefSuf();
	if ( closeButton->hasFocus() ) { close(); reject(); }
	break;	
    default :
	int code = k->key();
        if ( code != 4129 && code != 4128 && code != 4131 && code != 4132 && code != 4384 && !( code > 4144 && code < 4155 ) )
	    MCRInfo_textLabel->setText(DefCodec->toUnicode("Получен сигнал от клавиатуры или префиксы и суффиксы определены не верно!"));
	clearInfo();
    }
}
    
void MCRTest::ReaderSignal() {
    QString s = kbcode_reader.getCodeWithPrefSuf();
    Pref1 = kbcode_reader.getPrefix1();
    Pref2 = kbcode_reader.getPrefix2();
    Suf1 = kbcode_reader.getSuffix1();
    Suf2 = kbcode_reader.getSuffix2();
    MCRInfo_textLabel->setText(s);
    
    MCRCode1textLabel->setText(kbcode_reader.getTrack(1));
    MCRCode2textLabel->setText(kbcode_reader.getTrack(2));
    MCRCode3textLabel->setText(kbcode_reader.getTrack(3));
    MCRPref1->setText(QString::number(Pref1));
    MCRPref2->setText(QString::number(Pref2));
    MCRSuf1->setText(QString::number(Suf1));
    MCRSuf2->setText(QString::number(Suf2));
}

void MCRTest::clearInfo() {
    
}

void MCRTest::TimeOut()
{
    puts("----tim out ----");
    int k = kbcode_reader.GetCodeLen();    
    if ( (k < 3) ) {
	int n = kbcode_reader.GetCountEvent();
	for ( int i = 0; i < n; i++ )   {
	    makeKeyPress( kbcode_reader.GetEvent(i) );
	}
	kbcode_reader.Clear();
    } else {
	QString s;
	if ( kbcode_reader.getSumPrefSuf() > 0 ) {
	    ReaderSignal();
	} else {
	    ReaderSignal();
	    kbcode_reader.clearPrefSuf();
	}
	
	//puts("barcode with prefix and suffix catch");
	//printf("s = <%s> \n", s.ascii());
	kbcode_reader.Clear();
    }
}


void MCRTest::keyPressEvent(QKeyEvent *k)
{
   // puts("void MCRTest::keyPressEvent(QKeyEvent *k)");
    
    puts("------reg key press------");
    printf("c_key = %d \n", k->key());
    printf("c_ascii = %d\n", k->ascii());
    printf("c_state = %d \n", k->state());
    printf("c_text = %s \n", k->text().ascii());
    printf("c_text len = %d \n", k->text().length());
    puts("------------");
    
    if ( k->key() == Qt::Key_F12 ) {
	close();
	reject();
    }
    
    if ( PortNumber != 0 ) {
	makeKeyPress(k);
	return;
    }
    
    int status = 0;
    
    if ( kbcode_reader.getSumPrefSuf() > 0 ) {
	status = kbcode_reader.FindCode(k);
	//printf("status = %d \n", status);
	if (status == 2) TimeOut();
	
    } else {
	status = kbcode_reader.detectedPrefixSufix(k);
    }
    
    printf("status = %d \n", status);
    
    if ( status == 0 ) {  
	makeKeyPress(k);  
	return;  
    }
    
    if ( status == 5 ) {  
	//printf("kbcode_reader.GetCodeLen() = %d \n", kbcode_reader.GetCodeLen());
	MCRInfo_textLabel->setText(DefCodec->toUnicode("Префиксы и суффиксы определены не верно!"));
	clearInfo();
	TimeOut();   
	return;   
    }
    
    if ( status == 6 ) {
	MCRInfo_textLabel->setText(DefCodec->toUnicode("Ошибка! Заданно более чем два префикса"));
	clearInfo();
	//TimeOut();   
	return;   
    }
    if ( status == 7 ) {
	MCRInfo_textLabel->setText(DefCodec->toUnicode("Ошибка! Заданно более чем два суффикса"));
	clearInfo();
	//TimeOut();   
	return;   
    }
    
    /***************************************************************************************************/
    
    /*case Qt::Key_Enter :
	//printf("Qt::Key_Enter");
	if (Save->hasFocus())    SavePrefSuf();
	break;
    case Qt::Key_Return :
	//printf("Qt::Key_Return");
	if (Save->hasFocus()) SavePrefSuf();
	break;*/
}



void MCRTest::SavePrefSuf()
{
  //  puts ("void MCRTest::SavePrefSuf()");  
    QString er_str;
    QString er_id;
    if ((!MCRPref1->text().isEmpty()) || (!MCRPref2->text().isEmpty()) || (!MCRSuf1->text().isEmpty()) || (!MCRSuf2->text().isEmpty()))
    {
        save_flag = true;
        close();
    }
    else
    {
	DeviceConfMessageText(11,er_str,er_id);  
	InterfaceMessageWin(er_str, er_id);
    }
}

