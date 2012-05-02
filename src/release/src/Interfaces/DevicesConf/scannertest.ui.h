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
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
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

//#include "configurations.h"
#include "const.h"
#include "addon_func.h"
//#include "hwsrv.h"
#include "messages.h"


using namespace std;
extern int klava;
extern HWSRV  *hw;
static Display* g_d = XOpenDisplay(NULL);



void ScannerTest::InsertParam(int TypeBCS, int PortNum, int BaudR, int Pr1, int Pr2, int Su1, int Su2)
{
    //puts("void ScannerTest::InsertParam(PortNum, BaudR, Pr1, Pr2, Su1, Su2)");
    Type = TypeBCS;
    PortNumber = PortNum;
    Pref1 = Pr1;
    Pref2 = Pr2;
    Suf1 = Su1;
    Suf2 = Su2;
    BaudRate = BaudR;
   /*puts("check values param in scannertest");
    printf("PortNumber = %d\n",PortNumber);
    printf("BaudRate = %d\n",BaudRate);
    printf("Type = %d\n",Type);
    printf("Pref1 = %d\n",Pref1);
    printf("Pref2 = %d\n",Pref2);
    printf("Suf1 = %d\n",Suf1);
    printf("Suf2 = %d\n",Suf2);*/
    switch(Type){
	 case 1:
	 case 3:{
		 MyInit(); break;
	     }
	 case 2:{
		 connect(kbcode_scaner.timer, SIGNAL(timeout()), this, SLOT(TimeOut()));
		 if (Pref1+Pref2+Suf1+Suf2){
		     kbcode_scaner.setPrefSuf(Pref1, Pref2, Suf1, Suf2);
		 }
		 break;
	     }
	 default:{
		 break;
	     }
	 }
}


QString  ScannerTest::GetPar(int par_number)
{
    //printf("int ScannerTest::GetPar(int par_number = %d)\n", par_number);
    
    if ( Type == 2 ) {
	if ( par_number == 1 ) {  return QString::number(Pref1);   }	
	if ( par_number == 2 ) {  return QString::number(Pref2);   }
	if ( par_number == 3 ) return QString::number(Suf1);
	if ( par_number == 4 ) return QString::number(Suf2);	
	return QString("0");
    }
    
    QString str  = "",pref1 = "",pref2 = "",suf1 = "",suf2 = "";
    int b_ind = 0;
    int e_ind = 0;
    int kol1,kol2 = 0; 
    int i,k=0;
    bool pref1_fl = false, pref2_fl = false, suf1_fl = false, suf2_fl = false;
    
    QString Err;
	str = BarCodetextLabel->text();
     puts(str);
     kol1 = str.contains('[');
     kol2 = str.contains(']');
     if (kol1 != kol2 )
     { 
	 Err = QString("-3");
	 return Err;
     } 
     if (( kol1 > 4) || (kol2 > 4))
     {
	 Err = QString("-2");
	 return Err; 
     }
     if ((kol1 == 0 ) && (kol2 ==0 ))
     { 
	 Err = QString("0");
	 return Err;
     }
    puts("'[' == ']'");   
     char * s_str = (char*) calloc ((strlen(str))+1,sizeof(char));
     char * s_par = (char*) calloc (6,sizeof(char));
     strcpy(s_str,str);
     
     if ((kol1 == 1) || (kol2 == 1))
     {
   // определен всего один префикс или суффикс
	 b_ind = str.find( '[', e_ind );
	 e_ind = str.find( ']', e_ind ); 
	 for(i = 0; i<(e_ind-b_ind -1);i++ )
	     *(s_par+i) = *(s_str+b_ind+i+1);
	 if ((unsigned)e_ind < (strlen(str)/2))
	 {
	     pref1_fl = true;
	     pref1 = QString(s_par); 
	     pref2_fl = false;
	     suf1_fl = false;
	     suf2_fl = false;
	 }
	 else
	 {
	     suf1_fl = true;
	     suf1 = QString(s_par); 
	     pref2_fl = false;
	     pref1_fl = false;
	     suf2_fl = false;
	 }
     }
     else
     {
	 puts(s_str);
	 while (k < kol1)
	 { 
	     b_ind = str.find( '[', e_ind );
	     e_ind = str.find( ']', e_ind );
	     for(i = 0; i<(e_ind-b_ind -1);i++ )
		 *(s_par+i) = *(s_str+b_ind+i+1);	    
	     if ((unsigned)e_ind < (strlen(str)/2))
	     {
		 pref2_fl = true;
		 pref2 = QString(s_par); 
		 free(s_par);
		 s_par = (char*) calloc (6,sizeof(char));      
		 k++;
		 b_ind = str.find( '[', e_ind+1 );
		 e_ind = str.find( ']', e_ind+1 );
		 for(i = 0; i<(e_ind-b_ind -1);i++ )
		     *(s_par+i) = *(s_str+b_ind+i+1);    
		 if ((unsigned)e_ind < (strlen(str)/2))
		 { 
		     pref1_fl = true;
		     pref1 = pref2;
		     pref2 = QString(s_par);
		     k++;
		 } 
	     }
	     if ((unsigned)e_ind > (strlen(str)/2))
	     {
		 suf1 =   QString(s_par); 
		 suf1_fl = true;
		 free(s_par);
		 s_par = (char*) calloc (6,sizeof(char));	    
		 k++;
		 b_ind = str.find( '[', e_ind+1 );
		 e_ind = str.find( ']', e_ind+1 );
		 if ((b_ind == -1) || (e_ind == -1))
		     break;
		 for(i = 0; i<(e_ind-b_ind - 1);i++ )
		     *(s_par+i) = *(s_str+b_ind+i+1); 
		 suf2 =   QString(s_par); 
		 suf2_fl = true;
		 free(s_par);
		 s_par = (char*) calloc (6,sizeof(char));
		 k++;
	     } 
	 }
	 //puts("after while");
     }
     free(s_str);
     free(s_par);   
    // puts("after free");
     switch (par_number)
       {
         case 1: { if (pref1_fl == false)
	                    { Err = QString("0"); return Err; }
	                 return pref1; }
         case 2: { if (pref2_fl == false)
	                    { Err = QString("0"); return Err; }
			 return pref2; }  
         case 3: { if (suf1_fl == false)
	                    { Err = QString("0"); return Err; }
			 return suf1; }     
         case 4: { if (suf2_fl == false)
	                    { Err = QString("0"); return Err; }
			 return suf2; }
       }  
     Err = QString("0");  
     return Err;
 }


void ScannerTest::init()
{  
    //  puts("void ScannerTest::init()");
    ErrNum = 0;
    DefCodec = QTextCodec::codecForName(SYSCODEC);
    this->setCaption(DefCodec->toUnicode("Тест сканера"));
    connect(closeButton, SIGNAL(clicked()),this , SLOT(close()));
    grabKeyboard();  
    //timer_flag = false;
    //timer = new QTimer(this);
    //connect( timer, SIGNAL(timeout()),this , SLOT(EndTimer()));
}


void ScannerTest::MyInit(void)
{
//    puts("void ScannerTest::MyInit(void)");
    int k = 0;
    ErrNum = 0;
  /*  printf("PortNumber = %d\n",PortNumber);
    printf("BaudRate = %d\n",BaudRate);
    printf("Type = %d\n",Type);
    printf("Pref1 = %d\n",Pref1);
    printf("Pref2 = %d\n",Pref2);
    printf("Suf1 = %d\n",Suf1);
    printf("Suf2 = %d\n",Suf2);*/
    if ( Type != 2 ) {
	k =  hw->BCS_SetConnectOptions(Type,1, PortNumber, BaudRate,   Pref1,  Pref2, Suf1, Suf2);
	if (k == -1 )    {
	    ErrNum = 3; // can't open port
	    QString er_str;
	    QString er_id;
	    DeviceConfErrorText(14,er_str,er_id);
	    InterfaceErrorWin(er_str, er_id);
	}
    }
}

void ScannerTest::closeEvent( QCloseEvent* e )
{
    
 // puts("void ScannerTest::closeEvent( QCloseEvent* e )");
  hw->BCS_Stop();  
  e->accept();
 // w->show();
  return;
}

   

void ScannerTest::makeKeyPress(QKeyEvent * k)
{
    switch (k->key()) {
    case Qt::Key_F12 :
	puts("Qt::Key_F12");
	close();
	reject();
	break;
    case Qt::Key_Enter :
	//printf("Qt::Key_Enter");
	close();
	break;
    case Qt::Key_Return :
	//printf("Qt::Key_Return");
	close();
	break;
    default :
	int code = k->key();
        if ( code != 4129 && code != 4128 && code != 4131 && code != 4132 && code != 4384 && !( code > 4144 && code < 4155 ) )
	    BarCodetextLabel->setText(DefCodec->toUnicode("Получен сигнал от клавиатуры или префиксы и суффиксы определены не верно!"));
    }
}
    


void ScannerTest::TimeOut()
{
    puts("----tim out ----");
    int k = kbcode_scaner.GetCodeLen();    
    if ( (k < 3) ) {
	int n = kbcode_scaner.GetCountEvent();
	for ( int i = 0; i < n; i++ )   {
	    makeKeyPress( kbcode_scaner.GetEvent(i) );
	}
	kbcode_scaner.Clear();
    } else {
	QString s;
	if ( kbcode_scaner.getSumPrefSuf() > 0 ) {
	    s = kbcode_scaner.getCode();
	    BarCodetextLabel->setText(s);
	} else {
	    s = kbcode_scaner.getCodeWithPrefSuf();
	    Pref1 = kbcode_scaner.getPrefix1();
	    Pref2 = kbcode_scaner.getPrefix2();
	    Suf1 = kbcode_scaner.getSuffix1();
	    Suf2 = kbcode_scaner.getSuffix2();
	    BarCodetextLabel->setText(s);
	    kbcode_scaner.clearPrefSuf();
	}
	
	//puts("barcode with prefix and suffix catch");
	//printf("s = <%s> \n", s.ascii());
	kbcode_scaner.Clear();
    }
}

void ScannerTest::keyPressEvent(QKeyEvent *k)
{
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
    
    if (Type != 2) {
	makeKeyPress(k);
	return;
    }
    
    int status = 0;
    
    if ( kbcode_scaner.getSumPrefSuf() > 0 ) {
	status = kbcode_scaner.FindCode(k);
	//printf("status = %d \n", status);
	if (status == 2) TimeOut();
	
    } else {
	status = kbcode_scaner.detectedPrefixSufix(k);
    }
    
    printf("status = %d \n", status);
    
    if ( status == 0 ) {  
	makeKeyPress(k);  
	return;  
    }
    
    if ( status == 5 ) {  
	//printf("kbcode_scaner.GetCodeLen() = %d \n", kbcode_scaner.GetCodeLen());
	BarCodetextLabel->setText(DefCodec->toUnicode("Префиксы и суффиксы определены не верно!"));
	TimeOut();   
	return;   
    }
    
    if ( status == 6 ) {
	BarCodetextLabel->setText(DefCodec->toUnicode("Ошибка! Заданно более чем два префикса"));
	//TimeOut();   
	return;   
    }
    if ( status == 7 ) {
	BarCodetextLabel->setText(DefCodec->toUnicode("Ошибка! Заданно более чем два суффикса"));
	//TimeOut();   
	return;   
    }
}

    
