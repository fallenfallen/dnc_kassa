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
#include <qwidgetstack.h> 
#include <math.h>
#include "hwsrv.h"
#include "addon_func.h"
#include "messages.h"
#include "conf_const.h"

extern QTextCodec * DefCodec;
extern HWSRV  *hw;
bool returnPrefSuf = false;
void AutoForm::InsertParam(int TypeBCS, int PortNum, int BaudR, int Pr1, int Pr2, int Su1, int Su2)
{
//   puts("void AutoForm::InsertParam(int PortNum, int BaudR, int Pr1, int Pr2, int Su1, int Su2)");
    ScannerType = TypeBCS;
    PortNumber = PortNum;
    if (ScannerType == 1)
    {
	BaudRate = BaudR;
	Pref1 = Pr1;
	Pref2 = Pr2;
	Suf1 = Su1;
	Suf2 = Su2;
	MyInit();
	if (ErrNum!=0)	    close();
    }
    else
    {
	
	ErrNum = 0;
    }
}

QString  AutoForm::GetPar(int par_number)
{
   // puts("int AutoForm::GetPar(int par_number)");
    QString str,pref1,pref2,suf1,suf2;
    int b_ind = 0;
    int e_ind = 0;
    int kol1,kol2 = 0; 
    int i,k=0;
    bool pref1_fl = false, pref2_fl = false, suf1_fl = false, suf2_fl = false;
    
    QString Err;
    str = BarCodeString;   
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
  //  puts("'[' == ']'");   
    char* s_str;
    char* s_par; 
     s_str = (char*) calloc ((strlen(str))+1,sizeof(char));
     s_par = (char*) calloc (6,sizeof(char));
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
	// puts("after while");
     }
     free(s_str);
     free(s_par);   
   //  puts("after free");
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

void AutoForm::init()
{
    //puts("void AutoForm::init()");
   // DefCodec = QTextCodec::codecForName(DEF_CODEC);
    AutoPr1 = new QIntValidator(0,126,AutoPref1);
    AutoPr2 = new QIntValidator(0,126,AutoPref2);
    AutoSu1 = new QIntValidator(0,126,AutoSuf1);
    AutoSu2 = new QIntValidator(0,126,AutoSuf2);
    AutoPr1 -> setRange (0,126);
    AutoPref1 -> setValidator(AutoPr1); 
    AutoPr2 -> setRange (0,126);
    AutoPref2 -> setValidator(AutoPr2);
    AutoSu1 -> setRange (0,126);
    AutoSuf1 -> setValidator(AutoSu1);
    AutoSu2 -> setRange (0,126);
    AutoSuf2 -> setValidator(AutoSu2);
    Nextbutton->setFocus();
    returnPrefSuf = false;

    QRegExp barrx("([0-9a-zA-Z]){0,13}");
    barcodevalidator = new QRegExpValidator(barrx, this);
    BarCodeFromUserlineEdit->setValidator(barcodevalidator);
    
    QObject::connect (ClearBarCodeLineEdit_Button, SIGNAL(clicked()),this, SLOT(ClearBarCodeLineEdit_Button_clicked()));	 
    QObject::connect (Nextbutton, SIGNAL(clicked()),this, SLOT(Nextbutton_clicked()));	
    QObject::connect (ReturnButton2, SIGNAL(clicked()),this, SLOT(ReturnButton2_clicked()));	
    QObject::connect (CanselButton, SIGNAL(clicked()),this, SLOT(Exit()));	
    QObject::connect (Returnbutton, SIGNAL(clicked()),this, SLOT(Returnbutton_clicked()));	
    QObject::connect (Save_to_main_form, SIGNAL(clicked()),this, SLOT(Save_to_main_form_clicked()));	
    QObject::connect (Nextbutton_2, SIGNAL(clicked()),this, SLOT(Exit()));	
	 
    /*timer_flag = false;
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this , SLOT(EndTimer()));*/
}  
    
void AutoForm::MyInit()
{
 //   puts("void AutoForm::MyInit()");
    QString er_str;
    QString er_id;
    int k = 0;
    ErrNum = 0;
    k =  hw->BCS_SetConnectOptions(1,1, PortNumber, BaudRate, Pref1,  Pref2, Suf1, Suf2 );
    //FIFO[0] = hw->BCS_GetFIFO();
    if (k == -1 )
    {
	ErrNum = 3; // can't open port
	DeviceConfErrorText(14, er_str,er_id);
	InterfaceErrorWin(er_str, er_id);
    }
}


void AutoForm::closeEvent( QCloseEvent* e )
{
//  puts("void ScannerTest::closeEvent( QCloseEvent* e )");
  if (ScannerType == 1)
      Saving();
  hw->BCS_Stop();  
  e->accept();
  //w->show();
  return;
}

/*void AutoForm::EndTimer()
{
  //  puts("!!!!!!!!!!!!!!!!!!!!!! TIMER FINISHED   !!!!!!!!!!!!!!!!!");
    QString str;
    str = BarCodeString;
    BarCodeFromScannerlabel->setText(str);
    AutoPref1->setText(GetPar(1));
    AutoPref2->setText(GetPar(2));
    AutoSuf1->setText(GetPar(3));
    AutoSuf2->setText(GetPar(4));
    timer_flag = false;
}*/

void AutoForm::Returnbutton_clicked()
{
  //  puts("void AutoForm::Returnbutton_clicked()");
    widgetStack1->raiseWidget(0);
    BarCodeFromUserlineEdit->setSelection(0,(BarCodeFromUserlineEdit->text()).length());
    BarCodeFromUserlineEdit ->setDragEnabled(TRUE);
    AutoPref1->setText("");
    AutoPref2->setText("");
    AutoSuf1->setText("");
    AutoSuf2->setText("");
    AutoPref1->setEnabled(FALSE);
    AutoPref2->setEnabled(FALSE);
    AutoSuf1->setEnabled(FALSE);
    AutoSuf2->setEnabled(FALSE);
    releaseKeyboard();
}



void AutoForm::Save_to_main_form_clicked()
{ 
 //   puts("void AutoForm::Save_to_main_form_clicked()");
    if (ScannerType == 1)
    {
	returnPrefSuf = true;
	AutoPref1->setEnabled(FALSE);
	AutoPref2->setEnabled(FALSE);
	AutoSuf1->setEnabled(FALSE);
	AutoSuf2->setEnabled(FALSE);
	close();
    }
    else
    {
	    returnPrefSuf  = true;
    }
}



void AutoForm::Exit()
{
  //  puts("void AutoForm::Exit()");
    close();
    releaseKeyboard();
}

void AutoForm::Saving()
{
   // puts("void AutoForm::Saving()");
    if (ScannerType == 1)
    {
	if (((AutoPref1->isEnabled())||(AutoPref2->isEnabled())||(AutoSuf1->isEnabled())||(AutoSuf2->isEnabled())) &&((strcmp((AutoPref1->text().ascii()),"")!=0) || (strcmp((AutoPref2->text().ascii()),"")!=0) || (strcmp((AutoSuf1->text().ascii()),"")!=0) || (strcmp((AutoSuf2->text().ascii()),"")!=0)))
	{
	    int t =  QMessageBox::information( this, DefCodec->toUnicode("Cообщение"),
					       DefCodec->toUnicode("Определены суффиксы и префиксы\nперенести их значения в настройку оборудования?"),DefCodec->toUnicode("да "),DefCodec->toUnicode("отмена"),0,1);
	    if(t!=1)
	    {
		Save_to_main_form_clicked();
	    } 
	}
    }
    else
    {
	Save_to_main_form_clicked();
    }
}



void AutoForm::Nextbutton_clicked()
{
   // puts("Переходим с первой формы на вторую"); 
    QString er_str;
    QString er_id;
    if (!BarCodeFromUserlineEdit->text().isEmpty())
    {
	if ((strlen(BarCodeFromUserlineEdit->text().ascii())>=8) && (strlen(BarCodeFromUserlineEdit->text().ascii())<20))
	{
	    if (ScannerType == 1)
	    {
		widgetStack1->raiseWidget(1);
	    }
	    else
	    {
		widgetStack1->raiseWidget(2);
		LastPage();
		grabKeyboard(); 
		BarCodeFromScannerlabel->setText( DefCodec->toUnicode("<font color=red><u><b>Просканируйте штрих-код</b></u></font>"));
	    }
	}
	else
	{
	    DeviceConfErrorText(15, er_str, er_id);
	    InterfaceErrorWin(er_str, er_id);
	    return;    
	}
    }
    else
    {
	 DeviceConfErrorText(16, er_str, er_id);
	 InterfaceErrorWin(er_str, er_id);
	return;
    }
}

void AutoForm::LastPage()
{
  //  puts("void AutoForm::LastPage()");
    widgetStack1->raiseWidget(2);
    BarCodeFromScanner->setText(BarCodeFromUserlineEdit->text());
    if (AutoPref1->text()!=NULL)
	AutoPref1->setText("0");
    if (AutoPref2->text()!=NULL)
	AutoPref2->setText("0");
    if (AutoSuf1->text()!=NULL)
	AutoSuf1->setText("0");
    if (AutoSuf2->text()!=NULL)
	AutoSuf2->setText("0");
    if ((!AutoPref1->isEnabled())||(!AutoPref2->isEnabled())||(!AutoSuf1->isEnabled())||(!AutoSuf2->isEnabled()))   {
	AutoPref1->setEnabled(TRUE);
	AutoPref2->setEnabled(TRUE);
	AutoSuf1->setEnabled(TRUE);
	AutoSuf2->setEnabled(TRUE);
    }
}



void AutoForm::keyPressEvent(QKeyEvent *k)
{
    //printf("visibleWidget = %s \n", widgetStack1->visibleWidget()->name());
    
    if ( widgetStack1->visibleWidget() != ResultPage ) {
	makeKeyPress(k);
	return;
    }
    
    if ( ScannerType != 2 ) {
	makeKeyPress(k);
	return;
    }
    
    
    
    
}

void AutoForm::makeKeyPress(QKeyEvent *k)
{
    switch(k->key())   	{
    case Qt::Key_F12 :
	//  puts("Qt::Key_F12");
	close();
	reject();
	break;
    case Qt::Key_Escape :
	//  printf("Qt::Key_Escape");
	close();
	reject();
	break;
    case Qt::Key_Backspace :
	//  printf("Qt::Key_Backspace");
	if(BarCodeFromUserlineEdit->hasFocus())
	    BarCodeFromUserlineEdit->backspace();
	break;
    default :
	    if(BarCodeFromUserlineEdit->hasFocus())
		BarCodeFromUserlineEdit->insert(k->text());
    }
}
	
void AutoForm::ClearBarCodeLineEdit_Button_clicked()
{
   BarCodeFromUserlineEdit->setText(""); 
}


