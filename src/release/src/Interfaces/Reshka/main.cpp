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

#include "setup.h"

#include <qapplication.h>
#include <qcursor.h>
#include <qlabel.h>
#include <qpushbutton.h>

#include "mainmenuform.h"
#include "authorization.h"
#include "serviceform.h"


#include "dkbinput.h"
#include "bcskbinput.h"
#include "mcrkbinput.h"

#include "hwsrv.h"

#include "setup_param.h"

#include "addon_func.h"
#include "reg_pgsql.h"
#include "trans_pgsql.h"

#include "path_conf.h"
#include "const_conf.h"
#include "blocktimer.h"

#include "const.h"
#include "messages.h"


mainmenuForm *cfw;
int uid = -1;
KeyboardDevice * kbcode_scaner = NULL;
KeyboardDevice * kbcode_reader = NULL;

HWSRV * hw = NULL;
SP sp;
BlockTimer *bt;


DebugLevelLog regformlog("dnc_cash", "/tmp/dnc_regform", true);

Receipt * receipt;
Registration Reg(&sp, LINCASH_CONF_PATH);
AccessRights Acc(LINCASH_CONF_PATH);
Trans Tr(&sp, TRANSACTION_CONF_PATH);

Setup* setup_w = NULL;
serviceForm * service_w = NULL;

QString UpStr, DownStr;
QTextCodec * DBCodec = QTextCodec::codecForName(DBCODEC); 
QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC); 


void get_setup_params();
void init_keyboard_device(QApplication * a);
void startHW();
int StartSetup();
int StartService();

bool auth_request(bool block_mode = true);



/***********************************************************************/
/*void catch_sig(int sig_num)
{   
    printf("catch_sig(%d)\n", sig_num);
    
    if ( sig_num == SIGUSR1 )  {
	sigset(SIGUSR1, SIG_IGN);
	if( hw->SigFromBCS() == 1 ) {
	    
	    puts("------------------------------------SCANER SIGNAL-------------------------------");
	    if ( setup_w != NULL ) {
		puts("setup form not null !!!");
		setup_w->ScanerSignal();
	    }
	}
	if ( hw->SigFromMCR() == 1 ) {
	    puts("----------------------------READER SIGNAL-----------------------------------------");
	    if ( setup_w != NULL ) setup_w->ReaderSignal();                    
	}
	sigset(SIGUSR1, catch_sig);
    }
}*/
/************************************************************/
void ExecProgram(const char *path, const char *name = NULL, int param = 0)
{
    //printf("void ExecProgram(const char *path = %s, const char *name = %s, int param = %d)\n", path, name, param);
    pid_t pid;
    switch ( pid = fork() )    {
    case (pid_t) -1 :
	cfw->Error(1);
	break;
    case (pid_t) 0 :
	//puts("child process created");
	if ( param == 0  )    execlp(path, NULL);
	else	{
	    //puts("&&&&&&&&&&&&&&&&&&&&&&&&&&&");
	    char * p_str = NULL;
	    p_str = itoa(param);
	    //printf("p_str = <%s>\n", p_str);
	    //printf("path = <%s>\n", path);
	    //puts("&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
	    execlp(path, name, p_str, NULL);
	    free(p_str);
	}
	//perror("execl");
	_exit(0);
	//  exit(errno);
    default :
	//puts("parent process waited");
        int p_status;
	waitpid(pid, &p_status, WUNTRACED);
	//puts("child process ended correctly");
	cfw->exec_flag = false;
    }
}
//======================================================================//
bool auth_request(bool block_mode)
{
    //Вызывается форма авторизации
    bool ret = true;
    

    if ( Authorization::GetInstance()->Request(block_mode) != 0 )     { 
	//puts("--------------");
	return false;     
    }
    
    //В случае удачного выпонения в объекте авторизации ( Authorization::GetInstance() ) сохраняются следующие параметры:
    // * идентификатор пользователя
    // * имя пользователя
    // * вектор прав доступа
    
    
    uid = Authorization::GetInstance()->GetUid();
    //printf("in result uid = %d \n", uid);
    return ret;
}

//======================================================================//
void init_keyboard_device(QApplication * a)
{
    int bcs_type = hw->BCS_GetType(); 
    int mcr_type = hw->MCR_GetType();
    
    printf("BCS type = %d \n",  hw->BCS_GetType());
    printf("MCR type = %d \n", hw->MCR_GetType());
    
   if ( bcs_type == 2 ) {
       //printf("Scaner Keyboard ( pref1 = %d, pref2 = %d, suf1 = %d, suf2 = %d )\n", hw->BCS_GetPref1(), hw->BCS_GetPref2(), hw->BCS_GetSuf1(), hw->BCS_GetSuf2());
       kbcode_scaner = new KeyboardScaner(a,hw->BCS_GetPref1(), hw->BCS_GetPref2(), hw->BCS_GetSuf1(), hw->BCS_GetSuf2());	    }

    if ( mcr_type == 2 ) {
	//printf("Reader Keyboard ( pref1 = %d, pref2 = %d, suf1 = %d, suf2 = %d )\n", hw->MCR_GetPref1(), hw->MCR_GetPref2(), hw->MCR_GetSuf1(), hw->MCR_GetSuf2());
	kbcode_reader = new KeyboardReader(a,hw->MCR_GetPref1(), hw->MCR_GetPref2(), hw->MCR_GetSuf1(), hw->MCR_GetSuf2());    }
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
	//printf("state in main = %d\n", res);
	QString er_id, er_str;
	RegistrationErrorText(79, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
    }
    QTextCodec * DSPCodec = QTextCodec::codecForName(HWCODEC);
    
    
    UpStr = DBCodec->toUnicode(sp.cdsp_up_not_work);
    UpStr = DSPCodec->fromUnicode(UpStr);
    DownStr = DBCodec->toUnicode(sp.cdsp_down_not_work);
    DownStr = DSPCodec->fromUnicode(DownStr);  
}

/**********************************/
void applyRights()
{
    //puts("apply rig");
    
    cfw->setupPushButton->setEnabled(Authorization::GetInstance()->GetRight(26));  

    cfw->driversetupPushButton->setEnabled(Authorization::GetInstance()->GetRight(26));  
   
    cfw->exchangesetupPushButton->setEnabled(Authorization::GetInstance()->GetRight(26));  

    cfw->usersetupPushButton->setEnabled(Authorization::GetInstance()->GetRight(26));  

    cfw->warePushButton->setEnabled(Authorization::GetInstance()->GetRight(26)); 
    
    if ( ! sp.query_for_authorization )  {

	cfw->servicePushButton->setEnabled( Authorization::GetInstance()->GetRight(19) || Authorization::GetInstance()->GetRight(20) || Authorization::GetInstance()->GetRight(21) || Authorization::GetInstance()->GetRight(27) || Authorization::GetInstance()->GetRight(28) || Authorization::GetInstance()->GetRight(29) );    
    }

    
}

/**********************************/
void startHW()
{
    hw = new HWSRV();
    /*hw->BCS_ReadConf();
    hw->MCR_ReadConf();
    hw->DSP_ReadConf();
    if (hw->BCS_GetType() == 1)
    {hw->BCS_Start();}
    if (hw->MCR_GetType() == 1) 
    {hw->MCR_Start();}
    if (hw->DSP_GetType()==1)
    {hw->DSP_Start();} */
}

/***********************************************************************/
int StartSetup()
{    
    puts("-----Start Setup----");
    setup_w = new Setup();    
	
    if (setup_w->Error_Connection) {
	QMessageBox::information(NULL,  DefCodec->toUnicode("Настройка приложения"), 
				 DefCodec->toUnicode("Отсутствует подключение к БД!!!"), QMessageBox::Ok  );
	return 1;  
    }	
    
    int k = setup_w->exec();

    delete setup_w;
    return k;
    
}
/***********************************************************************/
int StartService()
{
    puts("----Start Service-----");
    
    hw->SetEducationMode(sp.learn_mode);
    if ( sp.autoblock_flag ) bt = new BlockTimer(sp.autoblock_time); 
    service_w = new serviceForm;
    
    int k = service_w->exec();
    delete	service_w;
    if ( bt != NULL ) free(bt);
    return k;
}
/**********************************/
int main( int argc, char ** argv )
{
    
    QApplication a( argc, argv );
    
    puts("`````````````````````````````````````````````````");
    printf("RESHKA MAIN PROCESS ID - %d\n", getpid());
    printf("VERSION: %s\n", PROGRAMM_VERSION);
    puts("`````````````````````````````````````````````````");
    
    startHW();
    
    get_setup_params();
    
    hw->DSP_Up(UpStr);
    hw->DSP_Down(DownStr);   
    
    init_keyboard_device(&a);     
    
    bool flag = auth_request(false);
    if ( flag )    {	
	cfw = new mainmenuForm;
	cfw->setWindowState(Qt::WindowNoState);
	cfw->setWindowState(Qt::WindowFullScreen);
	//printf("Authorization::GetInstance()->GetUserName() = %s \n", Authorization::GetInstance()->GetUserName());
	cfw->textLabel2->setText(DBCodec->toUnicode((Authorization::GetInstance()->GetUserName())));
	applyRights();
	
	while ( true )	{
	    puts("------Main Menu------");	    
	    cfw->setWindowState(Qt::WindowNoState);
      	    cfw->setWindowState(Qt::WindowFullScreen);
	    cfw->exec();
	    //printf("Selected point  = %d \n", cfw->focus_num);
	    cfw->releaseKeyboard();
	    if  ( cfw->exec_flag )   {
		switch(cfw->focus_num)		{
		case 0 :
		    printf("--------lincash-------\n");
		    delete (hw);
		    ExecProgram(REG_PATH, REG_NAME, uid);//, REG_NAME);	///-------------------------23/10 16.41---------------
		    cfw->grabKeyboard();
		    //---(+)---Zuskin---27/02/2012---
		    sp.ReloadSections();
		    //-------------------------------
		    startHW();
		    hw->DSP_Up(UpStr);
		    hw->DSP_Down(DownStr); 
		    break;
		case 1 :
		    printf("-------service-----\n");
		    //delete (hw);
		    //QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
		    //ExecProgram(SERVICE_PATH, SERVICE_NAME, uid);//, SERVICE_NAME);
		    StartService();
		    cfw->exec_flag = false;
   		    cfw->grabKeyboard();
		    //QApplication::restoreOverrideCursor(); 
		    //startHW();
		    hw->DSP_Up(UpStr);
		    hw->DSP_Down(DownStr);      
		    break;
		case 2 :
		    printf("-------accessrights-------\n");
		    delete (hw);
		    //printf("RIGHTS_PATH = %s \n", RIGHTS_PATH);
		    ExecProgram(RIGHTS_PATH);//, RIGHTS_NAME);
		    Authorization::GetInstance()->GetRights(Authorization::GetInstance()->GetUid());
		    applyRights();
   		    cfw->grabKeyboard();
		    startHW();
		    hw->DSP_Up(UpStr);
		    hw->DSP_Down(DownStr); 
		    break;
		case 3 :
		    printf("--------ware------\n");
		    delete (hw);
		    ExecProgram(WARE_PATH);//, SETUP_NAME);
		    cfw->grabKeyboard();	
		    startHW();
		    hw->DSP_Up(UpStr);
		    hw->DSP_Down(DownStr);
		    break;			    
		case 4 :
		    printf("-------programsetup-----\n");
		    StartSetup();
		    cfw->grabKeyboard();	
		    get_setup_params();
		    cfw->exec_flag = false;
		    hw->DSP_Up(UpStr);
		    hw->DSP_Down(DownStr);
		    break;
		case 5 :
		    printf("--------devicesetup-------\n");		    
		    if ( kbcode_scaner != NULL ) {   delete (kbcode_scaner);    kbcode_scaner = NULL;    }
		    if ( kbcode_reader != NULL ) {   delete (kbcode_reader);    kbcode_reader = NULL;    }
		    
		    delete (hw);
		    ExecProgram(DCONF_PATH);//, DCONF_NAME);
    		    cfw->grabKeyboard();
		    startHW();
		    init_keyboard_device(&a); 
		    hw->DSP_Up(UpStr);
		    hw->DSP_Down(DownStr); 
		    break;
		case 6 :
		    printf("--------setuploadunload------\n");
		    delete (hw);
		    ExecProgram(EXCH_PATH);//, EXCH_NAME);
    		    cfw->grabKeyboard();
		    startHW();
		    hw->DSP_Up(UpStr);
		    hw->DSP_Down(DownStr);	      
		    break;
		case 7 :
		    printf("-------changeuser------\n");
		    bool auth = auth_request(true);
		    while ( !auth ) auth = auth_request(true);
    		    cfw->grabKeyboard();
		    cfw->exec_flag = false;
		    cfw->textLabel2->setText(DBCodec->toUnicode((Authorization::GetInstance()->GetUserName())));
		    applyRights();
		    break;	    
		}
		cfw->exec_flag = false;
		
	    }
	    else break;   
	}
	delete(cfw);
    }       
    if ( kbcode_scaner != NULL ) delete (kbcode_scaner);
    if ( kbcode_reader != NULL ) delete (kbcode_reader);
    if ( hw != NULL )  delete (hw);
    puts("END OF RESHKA");
    
    return 0;
}
/**********************************/

