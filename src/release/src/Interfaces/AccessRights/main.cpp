/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2009-2011, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2009-2011, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/
#include <qapplication.h>
#include "accessrightsform.h"
#include "debug_level_log.h"
#include "signal.h"
#include "hwsrv.h"
#include "userform.h"

#include "setup_param.h"
#include "blocktimer.h"
#include "const_conf.h"


#include "dkbinput.h"
#include "bcskbinput.h"
#include "mcrkbinput.h"
//-----(+)-----Zuskin-----09/03/2011-----
#include "hwsighandlerscontroller.h"
//-------------------------------------------------

//AccessRights AccR("LinCash"/*dbname*/, "anuta"/*user*/, ""/*password*/, ""/*host*/, ""/*hostaddr*/, "" /*port*/, "./log/access.log"/* log_path*/);

AccessRights AccR(LINCASH_CONF_PATH);
DebugLevelLog pgsql_onscreanlog("dnc_cash", "./log/pgsql_onscreanlog", true);

HWSRV * hw;

KeyboardDevice * kbcode_scaner = NULL;
KeyboardDevice * kbcode_reader = NULL;


UserForm * point_form;
SP sp;
BlockTimer * bt;



int resolution_width;
int resolution_heigth;

//-----(-)-----Zuskin-----09/03/2011-----
/*
void catch_sig(int sig_num)
{   
    printf("catch_sig(%d)\n", sig_num);
    
    if ( sig_num == SIGUSR1 )  {
	sigset(SIGUSR1, SIG_IGN);
	if( hw->SigFromBCS() == 1 ) {
	    puts("------------------------------------SCANER SIGNAL-------------------------------");
	    if ( point_form != NULL ) point_form->ScanerSignal();	     
	}
	if ( hw->SigFromMCR() == 1 ) {
	    puts("----------------------------READER SIGNAL-----------------------------------------");
	    if ( point_form != NULL ) point_form->ReaderSignal();                    
	}
	sigset(SIGUSR1, catch_sig);
    }
}
*/
//-----------------------------------------------------

int main( int argc, char ** argv )
{
    puts("----------------------------");
    puts("Access Rights start");
    puts("----------------------------");
    
    QTextCodec *c = QTextCodec::codecForName("UTF8");
    
  QApplication a( argc, argv );
  
  QDesktopWidget *d = QApplication::desktop();
  resolution_width = d->width();     // returns desktop width
  resolution_heigth = d->height();    // returns desktop height
  //resolution_width = 800;
 // resolution_heigth = 600;
  
  point_form = NULL;
  bt  = NULL;
  
//  AccessRightsForm w;
  
//  if (w.ErrorConnection) {
//      QMessageBox::information(NULL,  c->toUnicode("Настройка приложения"), 
//			       c->toUnicode("Отсутствует подключение к БД!!!"), QMessageBox::Ok  );
//      return 1;   	}
  
//  w.show();
  
  hw = new HWSRV();
  
  int bcs_type = hw->BCS_GetType(); 
  int mcr_type = hw->MCR_GetType();    
  printf("bcs_type = %d \n", bcs_type);
  printf("mcr_type = %d \n", mcr_type);
    
  //-----(-)-----Zuskin-----09/03/2011-----  
/*  if ( bcs_type == 1 || bcs_type == 3 || mcr_type == 1 ) {
	sigset( SIGUSR1, catch_sig );   } 
*/
  //-------------------------------------------------
  
  if ( bcs_type == 2 ) {
      puts("scaner keyboard");
      kbcode_scaner = new KeyboardScaner(&a,hw->BCS_GetPref1(), hw->BCS_GetPref2(), hw->BCS_GetSuf1(), hw->BCS_GetSuf2());
  }
  
  if ( mcr_type == 2 ) {
      puts("reader keyboard");
      kbcode_reader = new KeyboardReader(&a,hw->MCR_GetPref1(), hw->MCR_GetPref2(), hw->MCR_GetSuf1(), hw->MCR_GetSuf2()); 
  }
  //-----------------------------------------------------
  AccessRightsForm w;
  
  if (w.ErrorConnection) {
      QMessageBox::information(NULL,  c->toUnicode("Настройка приложения"), 
			       c->toUnicode("Отсутствует подключение к БД!!!"), QMessageBox::Ok  );
      return 1;   	}
  
  w.show();
  //-----------------------------------------------------
  a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
  int state = a.exec();
  
  if ( kbcode_scaner != NULL ) delete kbcode_scaner;
  if ( kbcode_reader != NULL ) delete kbcode_reader;
  
  delete hw;  
  
  return state;
}
/*------------------------------------------------------------------------------------------------------*/
void quit() 
{
    printf("quit from program in main");
}
/*------------------------------------------------------------------------------------------------------*/


