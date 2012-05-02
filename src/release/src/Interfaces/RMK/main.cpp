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

#include <qapplication.h>
#include <qobject.h>
#include <signal.h>
////////
/*#include <qstring.h>
#include <qfont.h>
#include <qpoint.h>
#include <qtextcodec.h>
//#include <qtimer.h>
#include <math.h>*/
///////
#include <qtable.h>
#include <qlabel.h>
#include <qcursor.h> 
#include <qdatetime.h>

/*******************************/
/*
//#include <qlocale.h>
//#include <X11/Xlib.h>
//#include <X11/XKBlib.h>

#include "reg_pgsql.h"
#include "trans_pgsql.h"

#include "striketabitm.h"
#include "time_date.h"
#include "insert_transaction.h"
#include "addon_func.h"
#include "encoding.h"
#include "kkmregistration.h"




#include "dkbinput.h"
#include "bcskbinput.h"
#include "mcrkbinput.h"
#include "reader_config_load_unload.h"*/
/*******************************/

#include "receipt.h"
#include "hwsrv.h"
#include "setup_param.h"
#include "messages.h"
#include "blocktimer.h"
#include "registrationform.h"

#include "dkbinput.h"
#include "bcskbinput.h"
#include "mcrkbinput.h"
#include "const_conf.h"

#include "authorization.h"
#include "hwsighandlerscontroller.h"
#include "statestore.h"
#include "addwarecontroller.h"
#include "reg_pgsql.h"
#include "startupform.h"

#include <qmessagebox.h>
#include <qnamespace.h>

//KeyboardDevice * kbcode;
KeyboardDevice * kbcode_scaner = NULL;
KeyboardDevice * kbcode_reader = NULL;

//HWSRV *hw = new HWSRV;   
HWSRV *hw;
startupForm *strt;
RegistrationForm *w;
SP sp;
BlockTimer *bt;
Receipt * receipt;

AccessRights Acc(LINCASH_CONF_PATH);
Registration Reg(&sp, LINCASH_CONF_PATH);
Trans Tr(&sp, TRANSACTION_CONF_PATH);

int user_id;

int resolution_width;
int resolution_heigth;


//======================================================================//
void init_keyboard_device(QApplication * a)
{
  
    int bcs_type = hw->BCS_GetType(); 
    int mcr_type = hw->MCR_GetType();    
    //printf("bcs_type = %d \n", bcs_type);
    //printf("mcr_type = %d \n", mcr_type);
    
    
    if ( bcs_type == 2 ) {
	puts("scaner keyboard");
	printf("%d, %d, %d, %d \n", hw->BCS_GetPref1(), hw->BCS_GetPref2(), hw->BCS_GetSuf1(), hw->BCS_GetSuf2());
	kbcode_scaner = new KeyboardScaner(a,hw->BCS_GetPref1(), hw->BCS_GetPref2(), hw->BCS_GetSuf1(), hw->BCS_GetSuf2());	    
	//-----(+)-----Zuskin-----23/12/2011-----
	kbcode_scaner->TIME_INTERSYMBOL = hw->BCS_GetInterSymbolTimeout();
	//---------------------------------------
	//QObject::connect( kbcode_scaner->timer, SIGNAL(timeout()), w, SLOT(GetKeySequence()) );
    }
    if ( mcr_type == 2 ) {
	//puts("reader keyboard");
	kbcode_reader = new KeyboardReader(a,hw->MCR_GetPref1(), hw->MCR_GetPref2(), hw->MCR_GetSuf1(), hw->MCR_GetSuf2());    
	//QObject::connect( kbcode_reader->timer, SIGNAL(timeout()), w, SLOT(GetKeySequence()) );
    }
  

}
//======================================================================//
void get_setup_params()
{
  int res;
  if(sp.state_connect != 0)
  {
    QString er_id, er_str;
    RegistrationErrorText(78, er_str, er_id);
    InterfaceErrorWin(er_str, er_id);
  }
  else if((res = sp.Get_All_Param()) != 0)
  {
    ////printf("state in main = %d\n", res);
    QString er_id, er_str;
    RegistrationErrorText(79, er_str, er_id);
    InterfaceErrorWin(er_str, er_id);
  }
  
}
//======================================================================//
int main( int argc, char *argv[] )
{
    puts("start of RMK's main function..");     
    
    //QTextCodec DefCodec = QTextCodec::codecForName(SYSCODEC);
    
    QApplication app(argc, argv); 
    QWidget *parent;
    Qt::WFlags f = Qt::WType_TopLevel | Qt::WStyle_Customize | Qt::WStyle_NoBorder;
    strt = new startupForm(parent, 0,true, f);
    strt->show();
    app.processEvents();
    hw = new HWSRV;    
            
    if ( hw->ECR_GetPortNumber() == 0 )          hw->ECR_CreateFileWorkWithoutFR();
    
    int ret = 0;
    
    QDesktopWidget *d = app.desktop();
    resolution_width = d->width();     // returns desktop width
    resolution_heigth = d->height();    // returns desktop height
    printf("resolution_width = %d \n", resolution_width);
    printf("resolution_heigth = %d \n", resolution_heigth);
    //resolution_width = 800;
    // resolution_heigth = 600; 
    
    get_setup_params();	// getAllParam
    
    hw->SetEducationMode(sp.learn_mode);
    
    hw->ECR_SetUseOneRegistration(sp.print_one_registration);
    
    //else puts("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@^^^^^///");
    
    if ( sp.autoblock_flag ) bt = new BlockTimer(sp.autoblock_time);
    
    if ( (argc >= 2) && Authorization::GetInstance()->Init(atoi(argv[1])) )  {
	receipt = new Receipt(&sp);
	
	init_keyboard_device(&app);
	
	w = new RegistrationForm;
	
	//printf("HEY LOOK HERE w->GetInitFlag() = %d\n",w->GetInitFlag());
	if(!w->GetInitFlag())    {
	    //if(get_user_rights(atoi(argv[1])) == 0)
	    if(w->cn_good)      {
		
		if(sp.autoblock_flag)		    bt->Start();
		w->setWindowState(Qt::WindowNoState);
		w->setWindowState(Qt::WindowFullScreen);
		w->setMinimumSize(QSize(resolution_width,resolution_heigth));
		w->setMaximumSize(QSize(resolution_width,resolution_heigth));
		//w->adjustSize();
		//QSize t_s = w->receiptTable->size();
		//w->receiptTable->setMinimumSize(t_s);
		//w->receiptTable->setMaximumSize(t_s);
		//w->adjustSize();
		w->WaitProcessInit();
		w->GetInitFlag();
		
		strt->hide();
		delete strt;
		
		ret = w->exec();
	    }
	}    else   ret = -1;
	
	delete(w);
	
    }    else  {
        
        strt->hide();
	delete strt;
    	QString er_id, er_str;	
	RegistrationErrorText(77, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	ret = -1;
    }
    
   // puts("before delete(hw)");
    delete(hw);
   // puts("after delete(hw)");
    //if(sp.autoblock_flag) delete(bt);
    
    if ( kbcode_scaner != NULL ) delete kbcode_scaner;
    if ( kbcode_reader != NULL ) delete kbcode_reader;
  
   // puts("RMK stopped...");  
    return ret; 		
}
//======================================================================//

