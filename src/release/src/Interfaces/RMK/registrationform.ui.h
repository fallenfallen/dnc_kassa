/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2009-2012, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr aka Zuskin.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2009-2012, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

// parameter mode:
//   'c' - начальный режим
//  ''
//
//#include "kkmerror.h"
//#include "tabitm.h"

//#include <qmovie.h>
#include <qstring.h>
#include <qfont.h>
#include <qpoint.h>
#include <qsignal.h>
#include <qtextcodec.h>

//#include <qtimer.h>
#include <math.h>
//#include <qlocale.h>
//#include <X11/Xlib.h>
//#include <X11/XKBlib.h>
#include <qapplication.h>
#include <qdatetime.h> 
#include <iostream>

//#include <qtable.h>

#include "reg_pgsql.h"
#include "trans_pgsql.h"


#include "serviceform.h"
#include "synchronizationform.h"

#include "canceldiscountform.h"
#include "fixdiscountform.h"
#include "paymentform.h"
#include "striketabitm.h"
#include "checklistform.h"
#include "hwsrv.h"
//#include "time_date.h"
#include "blocktimer.h"
#include "const.h"
#include "setup_param.h"
//#include "insert_transaction.h"
#include "addon_func.h"
#include "encoding.h"
//#include "kkmregistration.h"
#include "cdsp.h"
#include "hwsighandlerscontroller.h"
#include "statestore.h"
#include "promptingform.h"

#include "visualform.h"
#include "authorization.h"
#include "dkbinput.h"
#include "bcskbinput.h"
#include "mcrkbinput.h"
#include "reader_config_load_unload.h"
#include "addwarecontroller.h"
#include "const_conf.h"
#include "printdocumentlist.h"
#include "printreceipt.h"

#include "deletepositionbytimeristrictionform.h"
#include "cashinsectionsform.h"

//#include "Section.h"
#include <vector>

//#include <pthread.h>
//#include <qmovie.h>
//#include <sys/types.h>

//-----(+)-----Zuskin-----15/07/2011-----
#include "stringconvert.h"
//-----(+)-----Zuskin-----15/07/2011-----
#include "bonuscardform.h"
#include <qlineedit.h>
//---------------------------------------
//-----(+)-----Zuskin-----01/08/2011-----
#include "edsform.h"
//-------------------------------------------------
//-----(+)-----Zuskin-----16/11/2011-----
#include "managerlistform.h"
//-------------------------------------------------
//-----(+)-----Zuskin-----07/12/2011-----
#include "const_conf.h"
//-------------------------------------------------
//-----(+)-----Zuskin-----24/01/2012-----
#include "continueprintingform.h"
//-------------------------------------------------

//-----(*)-----Zuskin-----20/12/2011-----
//#define SYNC_CONTROL_TIME 10000
#define SYNC_CONTROL_TIME 60000 // нет смысла делать проверку раз в 10 секунд - раз в минуту вполне достаточно
//-------------------------------------------------
#define PAUSE_TIME 1
#define INPUT_WIDTH 100
#define N_PC_PATH RC_LOADUNLOAD_PATH

using namespace std;

//Section s;
//vector<Section> exist_section;

extern Receipt * receipt;
extern HWSRV *hw;              // Указатель на объект работы с устройствами
extern SP sp;                        // Объект настроек формы
extern Registration Reg;      // Объект работы с базой данных по регистрации товаров
extern Trans Tr;                    // Объект работы с базой данных по работе с транзакциями
extern BlockTimer *bt; 	// Таймер автоблокировки рабочего место по истечению времени.
//extern KeyboardDevice * kbcode;
extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;
/*
// Индексы Прав Доступа
//  индекс   | право доступа                  |  индекс   | право доступа                      |  индекс   | право доступа                     |  индекс   | право доступа                     |
//======|================|======|=================|======|=================|======|=================|
//       0         | рег. по вн. коду                  |       7         | повтор продажи                |       14      | открыть чек возврата      |       21       | синхронизация времени   |
//       1         | рег. по ш.-к. сканером     |       8         | отмена чека                         |       15      | оплата наличными           |       22       | внесение                                |
//       2         | рег. по ш.-к. вручную        |       9         | произв. ск./надб.                  |       16      | оплата типом 2                 |       23       | выплата                                |
//       3         | визуальный подбор          |       10       | фикс. ск./надб.                     |       17      | оплата типом 3                 |       24       | печать копии чека              |
//       4         | сторно                                 |       11       | отмена авто ск./надб.      |      18      | оплата типом 4                 |        25      | открытие ден. ящика        |
//       5         | редактирование кол-ва  |       12       | отмена всех ск./надб.        |      19      | снятие отчета Z                |                   |                                                  |
//       6         | редактирование цены     |       13       | возврат по номеру чека   |       20      | снятие других отчетов   |                   |                                                  |
//======|================|======|=================|======|=================|======|=================|
*/

DebugLevelLog regformlog("dnc_cash", "/tmp/dnc_regform", true);

bool warn_about_sync_need;
int kkm_res;
//char check_mode = 'c';
double nalsum, credsum, contsum;
bool hide_mouse;
static Display* g_d = XOpenDisplay(NULL);
bool clrflg = false;

bool check_closed = false;

QLabel *wait_label = NULL;

RegistrationForm * regform_ref;

int error_id, kkmerror_id, message_id;

bool keyboard_block;

bool check_print_error;


//#################################################--INIT--SECTION--#######################################//
int nsleep(const struct timespec *req, struct timespec *rem){
    struct timespec temp_rem;
    if(nanosleep(req,rem)==-1){
      nsleep(rem,&temp_rem);
    }
    return 1;
};

void waitmsec(unsigned long milisec){
    if (milisec < 1) return;
    long ltime = 0;
    struct timespec R1 = {0};
    struct timespec R2 = {0};
    time_t sec =(int)(milisec/1000);
    milisec = milisec -(sec*1000);
    R1.tv_sec = sec;
    R1.tv_nsec = milisec*1000000L;
    nsleep(&R1,&R2);
};

//======================================================================//
void* thread_function(void* arg)
{    
    int res = regform_ref->PrintCheck();
    //puts("----pthread_exit(NULL)-----");
    
    printf("!-------------------------- res = %d\n", res);
    
    check_print_error = (res == -1);
    
    pthread_exit(arg);
   
}
//======================================================================//
void RegistrationForm::PrintProcessStart()
{
    kkmerror_id = 0;
    error_id = 0;
    message_id = 0;
    
    puts("PrintProcessStart()");
    
    pthread_t mythread;
    pthread_create(&mythread, NULL, thread_function, NULL);
    pthread_detach(mythread);
  
    pthread_join(mythread , NULL);
    
    //printf("kkmerror_id = %d \n error_id = %d \n message_id = %d \n", kkmerror_id, error_id, message_id);
    
}
//======================================================================//
void RegistrationForm::WaitProcessInit()  
{
    QPixmap* pm = new QPixmap();	
    //-----(-)-----Zuskin-----17/05/2011-----
    //if (hw->ECR_GetProtocol() == 6)
    //	pm->load( DefCodec->toUnicode(DefCodec->toUnicode(DbCodec->fromUnicode("/etc/dancy/image/push_fc-1-vv.gif"))));
    //else
    //---------------------------------------
    pm->load( DefCodec->toUnicode(DefCodec->toUnicode(DbCodec->fromUnicode(sp.progres_picture_path))));
    
    int picture_width = pm->width();
    int picture_height = pm->height();
    printf("picture_width = %d, picture_height = %d \n", picture_width, picture_height);
    
    delete pm;	 
    
    QMovie *m;
    if (hw->ECR_GetProtocol() == 6)
	m = new QMovie(DefCodec->toUnicode(DefCodec->toUnicode(DbCodec->fromUnicode("/etc/dancy/image/push_fc-1-vv.gif"))));
    else 
	m = new QMovie(DefCodec->toUnicode(DefCodec->toUnicode(DbCodec->fromUnicode(sp.progres_picture_path))));
    
    
    wait_label = new QLabel(this, "wait_label");
    
    int form_width = this->width();
    int form_height = this->height();
    
    //printf("picture x = %d, y = %d \n", Round(form_width/2) - Round(picture_width/2), Round(form_height/2) - Round(picture_height/2));
    wait_label->setGeometry( Round(form_width/2) - Round(picture_width/2), Round(form_height/2) - Round(picture_height/2), picture_width, picture_height );
    
    wait_label->setBackgroundColor( receiptTable->palette().color(QPalette::Active, QColorGroup::Base) );
    wait_label->setMovie(*m);
    wait_label->hide();
    delete m;
}
//======================================================================//
void RegistrationForm::WaitProcessStart()
{
    //puts("wait start");

    if  ( wait_label != NULL )	wait_label->show();
    
    keyboard_block = true;
}
//======================================================================//
void RegistrationForm::WaitProcessStop()
{
    puts("wait_stop");
    if  ( wait_label != NULL ) wait_label->hide();
    
    keyboard_block = false;
    
   // printf("kkmerror_id = %d \n error_id = %d \n message_id = %d \n", kkmerror_id, error_id, message_id);
}

//======================================================================//
void RegistrationForm::ScanerSignal(const char* barcode)   {	
    printf("RMK Scaner Signal(%s)\n", barcode);
    
    if ( HwSignalHandlersController::GetInstance()->GetControlledObject() != REGISTRATION )  {  return;   }
    if ( ! this->isActiveWindow()  ) return;
    if ( keyboard_block ) return;
    if ( sp.autoblock_flag ) bt->action_time = QTime::currentTime();
    
    barcode_from_scaner = QString(barcode);
}
//======================================================================//
void RegistrationForm::ReaderSignal(const char* cardcode)   {
    if ( HwSignalHandlersController::GetInstance()->GetControlledObject() != REGISTRATION )  {  return;   }
    if ( ! this->isActiveWindow()  ) return;
    if ( keyboard_block ) return;
    emit CardEvent( cardcode );	
}
//======================================================================//
void RegistrationForm::HZSignal(const char*) {
    
     if ( HwSignalHandlersController::GetInstance()->GetControlledObject() != REGISTRATION ) return;
     if ( ! this->isActiveWindow()  ) return; 
     if ( keyboard_block ) return;
     
     if ((hw->Weight) > 0) {
	 emit AddWareController::GetInstance()->SetQuantity(hw->Weight, 1);
     }  else {
	 emit AddWareController::GetInstance()->SetQuantity(1.000, 1);
	 emit quantTextLabel->setText("");
     }
}
//======================================================================//	
void RegistrationForm::init()
{
    //puts("---------Registration ----- init()"); 
       
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
        
    ctrl_v = FALSE;  
    
    regform_ref = this;
    
    keyboard_block = false;
    
    if ((hw->ECR_GetEducationMode() == 1) || (hw->ECR_GetPortNumber() == 0))
	hw->ECR_CreateFileWorkWithoutFR();
	
    // sp.max_change_time = 600;
    // sp.warning_change_time_ending = 580;
	
    init_flag = true; // Флаг инициализации формы. 
    //Некоторые действия не могут выполняться в момент инициализации формы, например вывод сообщения об ошибки в поле ввода.
    cn_good = true; // Признак успешной инициализации
    
    DefCodec = QTextCodec::codecForName(SYSCODEC);
    DbCodec = QTextCodec::codecForName(DBCODEC);
    HwCodec = QTextCodec::codecForName(HWCODEC);
    
    //-----(+)-----Zuskin-----18/11/2011-----
    manager_name = "";
    manager_id = "";
    //--------------------------------------------------
    
    InitStateString(); //Создание и размещение на форме объектов строки сосотояния
    
    //----------------------!!!!!!!!!!!!!!!!!!------------------------
    int group = 0; 
    if  (XkbGetState( g_d, XkbUseCoreKbd, &state ) == Success)
	group = state.locked_group;
    if (group == 0)
	lang_label->setText("ENG");
    else
	lang_label->setText("RUS");
    //----------------------!!!!!!!!!!!!!!!!!!------------------------
	
	
    copy = NULL;                  //Объект будет хранить информацию о копии существующего чека
    
    ins_tr = NULL;                //Объект управляющий записью операций в базу данных
    kkm_reg = NULL;           // Объект управляющий работой с ККМ: формирование и печать чеков
    
    str  = "";                    //textTextLabel->text()
    
    
    kkm_reg = new KkmRegistrations(hw, &regformlog);
    kkm_reg->SetSP(&sp);
    
    // DefaultFontSize = 36;  
    //QFont f = textTextLabel->font();  
    g = textTextLabel->font();
    DefaultFontSize = g.pointSize();
    LastFontSize = DefaultFontSize;
    
    DefaultFSNamePos = nameposTextLabel->font().pointSize();
    
    
    QFont f = codeposTextLabel->font();		
    int font_size = f.pointSize();
    //printf("font_size = %d\n",font_size);
    f.setPointSize(font_size-1);
    codeposTextLabel->setFont(f);    
    //-----(+)-----Zuskin-----03/08/2011-----
    DefLabelFontSize = font_size - 1;
    //-------------------------------------------------
    cancel_flag = false;
    if ( InitDb() == 0 ) // Проверка коннекта к базе данных
    {
	if(InitCash() == 0) // Проверка связи с фискальным регистратором и перевод регистратора в режим регистрации
	{
	    if(InitTimeSync() == 0) // Проверка на рассинхронизацию времени 
	    {
		if(InitChangeTime() == 0)
		{
		    cn_good = true;
		    
		    InitTableSize();
		    QString user_name = Authorization::GetInstance()->GetUserName();
		    if (StateStore::GetInstance()->GetReceiptState() != COPY) receipt->setCashier(Authorization::GetInstance()->GetUserShortcut());
		    
		    operTextLabel->setText( DbCodec->toUnicode( user_name) );
		    statusTextLabel->setText(DefCodec->toUnicode(""));
		    modeTextLabel->setText(DefCodec->toUnicode("готов к работе"));
		    
		    nalsum = 0;
		    credsum = 0;
		    contsum = 0;
		    
		    warn_about_sync_need = false;
		    quantity_flag = false;
		    payment_flag = false;
		   
		    return_flag = false;
		    copy_flag = false;
		    authorization_request_flag = false;
		    notaddbyvisual = true;
		    
		    //mode = 'c';
		    //oldmode = 'c';
		    
		    copy = new CopyInfo;
		    ins_tr = new InsertTransactions(hw, receipt, &Tr, &regformlog);
		    ins_tr->SetSP(&sp);
		    ins_tr->SetUserId( Authorization::GetInstance()->GetUid() );
		    
		    pause_work_timer = new QTimer(this);
		    connect(pause_work_timer, SIGNAL(timeout()), SLOT(Pause_Work()));
		    pause_work_timer->start(1, true); //надо включать только при запуске и после закрытия чека
		    
		    QTimer *timer = new QTimer(this);
		    connect(timer, SIGNAL(timeout()), SLOT(CheckFlag()));
		    timer->start(200, false);
		    
		    synctime_timer = new QTimer(this);
		    connect(synctime_timer, SIGNAL(timeout()), SLOT(DateTimeSync()));
		    synctime_timer->start(SYNC_CONTROL_TIME, true);
		    
		    InitAddWareConnections();
		    
		    GrabKeyboard();
		    
		    hide_mouse = false;  
		    if(sp.block_mouse) 	  {
			QApplication::setOverrideCursor( QCursor(Qt::BlankCursor) );
			hide_mouse = true;
			//grabMouse();
		    }
		    init_flag = false;
		    
		    connect(HwSignalHandlersController::GetInstance(), SIGNAL(BarCode(const char *)), SLOT(ScanerSignal( const char* )));
		    connect(HwSignalHandlersController::GetInstance(), SIGNAL(TrackCode(const char *)), SLOT(ReaderSignal( const char* )) );
		    connect(HwSignalHandlersController::GetInstance(), SIGNAL(HZCode(const char* )), SLOT(HZSignal(const char*)) );
		    
		    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence()) );
		    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
		    
		    StateStore::GetInstance()->SetViewState(GENERAL);
		}
	    }
	}
    }
    
    //puts("!!!!!!!-------------!!!!!!!!");
    //puts("connect(nameposTextLabel, SIGNAL(textChanged(const QString&)), this, SLOT(SelfScalingNameposTaxLabel(const QString&)))");
    //connect(nameposTextLabel, SIGNAL(textChanged(const QString&)), this, SLOT(SelfScalingNameposTaxLabel(const QString&)));
    //ameposTextLabel->setScaledContents(true);
    
    ReturnNumberCheck = -1;
    ReturnPayType = -1;
    
    check_print_error = false;    
    barcode_from_scaner = "";
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    
    //-----(+)-----Zuskin-----07/06/2011----- 
    if ( sp.sections.size() > 0 ) {
        kkm_res = hw->ECR_ReadCashInDrawer();
	sp.deductCashSumInSection(0, sp.getCashSumInSection(0));
	sp.addCashSumInSection(0, hw->ECR_GetSumm());
    }	
    //---------------------------------------


    /*cout << "count of section - " << sp.sections.size() << endl;
    for ( int i = 0; i < sp.sections.size(); i++ ) {
	cout << "section number in rmk - " << sp.sections[i].number << endl;
	cout << "section name in rmk - " << sp.sections[i].name << endl;
    }*/    
    
    
    
    //puts("Registration --=----- init() ------ ok");
    //printf("----------------------------1----------------------END INIT ctrl_v = %d\n",ctrl_v);
}
//======================================================================//
void RegistrationForm::InitAddWareConnections()
{
  //puts("void RegistrationForm::InitAddWareConnections()");
  connect(AddWareController::GetInstance(), SIGNAL(QuantitySet(double , int )), SLOT(ShowQuantitySet(double, int)));
  connect(AddWareController::GetInstance(), SIGNAL(WareAdded(int , bool , bool )), SLOT(ShowAddedWare(int, bool, bool)));
  connect(AddWareController::GetInstance(), SIGNAL(AddWareError(int )), SLOT(AddWareError(int)));
  
  connect(AddWareController::GetInstance(), SIGNAL(AuthRequest()), SLOT(AuthorizationRequested()));
}

//======================================================================//
int RegistrationForm::InitDb()
{
//regformlog.PrintString(DEBLEVEL_REGFORM, INFO, "int RegistrationForm::InitDb()\n");
  if((Reg.er_con != 0) || (Tr.er_con != 0))
  {
    //regformlog.PrintString(DEBLEVEL_REGFORM, INFO,"ERROR DATABASE CONNECTION!!!!\n");
    Error(80);
    cn_good = false;
    return -1;
  }
  return 0;
}

//======================================================================//
int RegistrationForm::InitCash()//corrected by Irina
{
//regformlog.PrintString(DEBLEVEL_REGFORM, INFO, "int RegistrationForm::InitCash()\n");

    kkm_res = hw->ECR_ReadStatus();
    if(kkm_res != 0)
    {
      Kkm_Error(kkm_res);
      cn_good = false;
      return -1;
    }
    
    //-----(+)-----Zuskin-----17/10/2011-----
    // ( hw->ECR_GetCheckState() == 1 ) {
    //cancel_flag = true;
    //}
    int curState = hw->ECR_GetCheckState();
    switch (curState) {
    case 1: { // открыт чек продажи/возврата
	    cancel_flag = true;
	}
    case 2: { // открыт нефискальный (сервисный) документ
	    hw->ECR_CloseNotFiscalDoc();
	}
    case 3: {// ожидание команды продолжения печати
	    hw->ECR_ContinuePrinting();
	}
   }
   
   
      //-------------------------------------------------
      kkm_res = kkm_reg->KkmSetMode(1);
      if(kkm_res != 0)
      {  
	Kkm_Error(kkm_res);
	if(kkm_res == 136)
	{
	  cn_good = false;
      }
	return -1;
      }
  return 0;
}

//======================================================================//
int RegistrationForm::InitTimeSync()//corrected by Irina
{
  //puts("int RegistrationForm::InitTimeSync()");
  int res = DateTimeControl();
  if(res == -1)
  {
    Error(73);
    cn_good = false;
    return -1;
  }
    return 0;
}

//======================================================================//
int RegistrationForm::InitChangeTime()//corrected by Irina
{
    //regformlog.PrintString(DEBLEVEL_REGFORM, INFO, "int RegistrationForm::InitChangeTime()\n");
    //printf("serial_number = %s \n", hw->ECR_GetSerialNumber());

    int res = 0;  
    int kkm_ret = hw->ECR_ReadStatus();
    if ((hw->ECR_GetProtocol() == 4) && (kkm_ret != 0))
	kkm_ret = hw->ECR_ReadStatus();
    if ( kkm_ret != 0 ) {
	if ( init_flag ) Kkm_Error(kkm_ret);
	cn_good = false;
	return -1;
    }
    
    puts("````````````````````````````");
    if ( hw->ECR_GetSessionOpened() ) puts("Session is opened");
    else puts("Session is closed");
    puts("````````````````````````````");
    
    if ( hw->ECR_GetSessionOpened() ) {	 
	// смена открыта
	//puts("----- Открытая смена -------");
	//regformlog.PrintString(DEBLEVEL_REGFORM, INFO, "ОТКРЫТАЯ СМЕНА!!!\n");
	//puts("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
	//puts("InitOpenChangeTime(hw->ECR_GetSerialNumber());");
	res = InitOpenChangeTime(hw->ECR_GetSerialNumber());
	
	if ( res == 0 ) {
	    res = SetOpenSessionTime();
	    if ( res == 0 ) {
		StateStore::GetInstance()->SetSessionState(OPENED);
		closed_session_flag = false;
		//change_time->blink_flag = false;
		change_time->BlinkTime(false);
		InitWarningChangeTimeEnding();
		//change_mode = 'o';
	    }
	}
	if ( res != 0 ) {
	    cn_good = false;
	    return -1;
	}
    } else {
	//regformlog.PrintString(DEBLEVEL_REGFORM, INFO, "СМЕНА ПОКА ЗАКРЫТА\n");
	StateStore::GetInstance()->SetSessionState(CLOSED);
	closed_session_flag = true;
	change_time->StopTime();
	QTime t(0,0,1);	
	t = t.addSecs(sp.max_change_time);
	//cout << sp.max_change_time<<endl;
	//change_time->blink_flag = false;
	change_time->BlinkTime(false);
	change_time->SetTime(t, false);
	// change_mode = 'c';
    }
    return 0;
}

//======================================================================//
int RegistrationForm::InitOpenChangeTime(const char * serial_number)
{
    //regformlog.PrintString(DEBLEVEL_REGFORM, INFO, "int RegistrationForm::InitOpenChangeTime(const char * serial_number)\n");
    char *sdatetime = NULL;
    if (sp.learn_mode && !sp.write_transaction) return 0;
    int res = 0;
    //printf("serial_number = %s \n", serial_number);
    //printf("hw->getserial = %s \n", hw->ECR_GetSerialNumber());
    res = Tr.Get_Chage_Opened(sdatetime, serial_number);
    //printf("Tr.Get_Chage_Opened(sdatetime, serial_number) = %d \n", res);
    if ( res == 0 )  {
	char * sdate = (char *) calloc(11, sizeof(char));
	char * stime = (char *) calloc(9, sizeof(char));
	strncat(sdate, sdatetime, 10);
	strncat(stime, sdatetime + 11, 9);
	QDate sd = QDate::fromString(sdate, Qt::ISODate);
	QTime st = QTime::fromString(stime, Qt::ISODate);
	free(sdate);
	free(stime);
	free(sdatetime);
	change_dt_open.setDate(sd);
	change_dt_open.setTime(st);
    }  else if ( res == 1 )  {	  
	Error(76);		  
	return -1;		  		  						  
    }  else  {
	Error(75);
	return -1;
    }
    return 0;
}

//======================================================================//
void RegistrationForm::InitWarningChangeTimeEnding()
{
    //regformlog.PrintString(DEBLEVEL_REGFORM, INFO, "void RegistrationForm::InitWarningChangeTimeEnding()\n");
    //regformlog.PrintString(DEBLEVEL_REGFORM, INFO, "???????????????\nmax_change_time = %d\nwarning_change_time_ending = %d\nchange_time_remain = %d\ntime_for_warning = %d\n???????????????\n", sp.max_change_time, sp.warning_change_time_ending, sp.max_change_time - change_dt_open.secsTo(QDateTime::currentDateTime()), time_for_warning);
    
    int cct = change_dt_open.secsTo(QDateTime::currentDateTime());
    int time_for_warning = sp.max_change_time - sp.warning_change_time_ending;
    
    if(cct < time_for_warning)
    {
	//cout << "До предупреждения осталось: ";
	//cout << time_for_warning<< endl;
	QTimer::singleShot(time_for_warning * 1000, this, SLOT(Check_Change_Time()));
    }
    else
	QTimer::singleShot(1, this, SLOT(Check_Change_Time()));
}

//======================================================================//
void RegistrationForm::InitPkCurrentTime()
{
    pk_date->SetDate(QDate::currentDate(), QTime::currentTime());
    pk_time->SetTime(QTime::currentTime(), true);
    // pk_time->StopTime();    
    // pk_time->StartTime();    
}

//======================================================================//
void RegistrationForm::InitStateString()
{
    //regformlog.PrintString(DEBLEVEL_REGFORM, INFO, "void RegistrationForm::InitStateString()\n");
    int x = 0;
    int y = 0;
    
    lang_label = new QLabel("RUS",this);
    change_time = new DigitalTime(false, this);
    pk_date = new DigitalDate(this);
    pk_time = new DigitalTime(false, this);
    
    
    learnmode_label = new QLabel("lm_label", this);
    
    //-----(+)-----Zuskin-----18/11/2011-----
    manager_label = new QLabel("", this);
    //-------------------------------------------------
    InitPkCurrentTime();
    
    y = this->height() - 10 - 30;
    
    x = this->width() - 10 - 100;
    pk_time->setGeometry(x, y, 100, 30);
    
    x = x - 150;
    pk_date->setGeometry(x, y, 150, 30);
    
    x = x - 100;
    change_time->setGeometry(x, y, 100, 30);
    
    x = x - 50;
    lang_label->setGeometry(x, y, 50, 30);
    
    //learnmode_label->setGeometry(lang_label->width(), y, 50, 30);
    //  lang_label->setFrameShape(QFrame::StyledPanel);
    //  lang_label->setFrameShadow(QFrame::Plain);
    
    lang_label->setMinimumWidth(50);
    learnmode_label->setMinimumWidth(50);
    
    pk_time->setMinimumWidth(100);
    change_time->setMinimumWidth(100);
    pk_date->setMinimumWidth(120);
    
    statestringLayout = new QHBoxLayout();
    statestringLayout->addWidget(lang_label);    
    statestringLayout->addWidget(learnmode_label);  
    //-----(+)-----Zuskin-----18/11/2011-----
    manager_label->setMinimumWidth(100);
    statestringLayout->addWidget(manager_label);  
    //-------------------------------------------------
    
    statestringLayout->addStretch();
    statestringLayout->addWidget(change_time);
    statestringLayout->addWidget(pk_date);
    statestringLayout->addWidget(pk_time);
    
    //commonLayout->addLayout(statestringLayout);
    ((QVBoxLayout*)this->layout())->addLayout(statestringLayout);
    
    learnmode_label->setText(DefCodec->toUnicode("ОБУЧАЮЩИЙ РЕЖИМ"));  
    if (sp.learn_mode)     learnmode_label->show();
    else     learnmode_label->hide();    
    
    if ( sp.learn_mode ) textTextLabel->setText(DefCodec->toUnicode("ОБУЧАЮЩИЙ РЕЖИМ"));
    
    
}

//======================================================================//
void RegistrationForm::InitTableSize()
{
    //  regformlog.PrintString(DEBLEVEL_REGFORM, INFO, "RegistrationForm::InitTableSize()\n");
    
    receiptTable->setReadOnly(TRUE);
    receiptTable->setColumnStretchable (0, true );
    receiptTable->setColumnWidth( 1, 120 );
    receiptTable->setColumnWidth( 2, 120 );
    receiptTable->setColumnWidth( 3, 120 ); 
    receiptTable->setSelectionMode(QTable::SingleRow);
    
    //  receiptTable->setColumnStretchable (1, false);
    //  receiptTable->setColumnStretchable (2, false);
    //  receiptTable->setColumnStretchable (3, false);
}

//############################################--END--INIT--SECTION--#######################################//

//======================================================================//
void RegistrationForm::destroy()
{
    //regformlog.PrintString(DEBLEVEL_REGFORM, INFO, "void RegistrationForm::destroy()\n");
    ReleaseKeyboard();
    
    if(sp.block_mouse) releaseMouse();
    
    if(copy != NULL) delete(copy);
    
    if(receipt != NULL) delete(receipt);
    
    if(ins_tr != NULL) free(ins_tr);
    
    if(kkm_reg  != NULL) free(kkm_reg);
    
    if (wait_label != NULL) delete(wait_label);
    
    if (learnmode_label != NULL) delete(learnmode_label);
    
}

//##########################--INOUT--INTERFACE--SECTION--#####################################//
//======================================================================//
bool RegistrationForm::GetInitFlag()
{
    //regformlog.PrintString(DEBLEVEL_REGFORM, INFO, "bool RegistrationForm::GetInitFlag()\n");
    return init_flag;
}

//======================================================================//
void RegistrationForm::SetStr(QString s)
{
    // puts("RegistrationForm::SetStr(QString s)");
    str = s;
    //textTextLabel->setText(str);
}
//##########################--END--OUT--INTERFACE--SECTION--##################################//

//############################--DATE--AND--TIME--SECTION--####################################//
//======================================================================//
/*int RegistrationForm::AutoSynchronization()//corrected by Irina
{
    puts("int RegistrationForm::AutoSynchronization()");
    if ( keyboard_block ) return 0;
    int ret = 1;
    kkm_res = hw->ECR_ReadStatus();
    if (kkm_res!=0){
	puts("ECR_ReadStatus() != 0");
	return kkm_res;
    }
    
    if(!hw->ECR_GetSessionOpened()) //Если смена закрыта, то попытаемся синхронизировать автоматически
    {
	ReleaseKeyboard();
	synchronizationForm *sf = new synchronizationForm;
	//sf->SetParams(hw);
	if(sf->kkm_connect)     sf->Synchronize();
	delete(sf);
	GrabKeyboard();
    }
    else ret = 1;
    
    return ret;
}*/

//======================================================================//
void RegistrationForm::SynchronizationWarning(bool wran_need_flag)
{
    if(!init_flag) // Необходимо выдавать предупреждение только при открытой форме
    {
	if(sp.warn_synchro_in_clock_field) // Предупреждение в поле часов
	{
	    //pk_time->blink_flag = wran_need_flag;
	    pk_time->BlinkTime(wran_need_flag);
	}
	if(sp.warn_synchro_after_close_check)
	{
	    warn_about_sync_need = wran_need_flag;
	}
    }
}

int RegistrationForm::FrYear(int fr_year)
{
    int year = 0;
    switch (fr_year)
    {
    case 98 :
	year = 1998;
	break;
    case 99 :
	year = 1999;
	break;
    default :
	    year = 2000 + fr_year;
    break;
}
    return year;
}

//======================================================================//
//retrun 1: необходима синхронизация, время рассинхронизированно в допустимых пределах
//retrun -111 : 
//return error_from_fr;
int RegistrationForm::DateTimeControl()//corrected by Irina
{
  // puts("---------RegistrationForm::DateTimeControl()---------");    
    //-----(+)-----Zuskin-----01/12/2011-----
    if (keyboard_block) {
        puts("--- RegistrationForm::DateTimeControl() --- FR is buzy ---");
        return 0;
    }    
    //-------------------------------------------------
    int ret = 0;
    if ( !sp.control_time_synchronization )  {
        //---(*)---Zuskin---15/02/2012---
        // перенесено в CheckFlag()	
        // if(!init_flag) InitPkCurrentTime();
        //-----------------------------------------	
        QDateTime dtpk = QDateTime::currentDateTime();
        if ( keyboard_block ) return ret;
        ret = hw->ECR_ReadStatus();   
        if (ret!=0){
           return ret;
        }
	
        QDate dfr(FrYear(hw->ECR_GetYear()), hw->ECR_GetMonth(), hw->ECR_GetDay());
         QTime tfr(hw->ECR_GetHour(), hw->ECR_GetMinute(), hw->ECR_GetSecond());
         QDateTime dtfr(dfr, tfr);
          int time_difference = abs(dtfr.secsTo(dtpk));
	  
    /* puts("*************************");
     printf("fr time = <%s>\n", dtfr.toString().ascii());
     printf("pk time = <%s>\n", dtpk.toString().ascii());
     printf("time difference = %d\n", time_difference);
     puts("*************************");*/
	    
          if(time_difference >= sp.time_for_synchronization_max)  {
	// Рассинхронизация не допустимая для работы с фискальным регистратором
	//Блокировать рабочее место, если форма регистрации уже запущена
	if(!init_flag){
	    Message(2);
	    CloseRegistrationForm(false);
	}
	ret = -1;
           }  else if ( time_difference >= sp.time_for_synchronization )  { 
	       // Рассинхронизация допустимая для работы с фискальным регистратором
	       if ( sp.auto_synchronization ){   
		   // Попробовать синхронизировать время автоматически
		   //----------last change 17/01/2011-------------
		   //установить нужную дату и время в ФР
		   int res = 0;
		   QDate d = QDate::currentDate();
		   res = hw->ECR_SetDate(d.day(), d.month(), d.year() - 2000);
		   if (res != 0)  return -1;
		   
		   QTime t = QTime::currentTime();
		   res = hw->ECR_SetTime(t.hour(), t.minute(), t.second());
		   if (res != 0)  return -1;
		   //----------last change 17/01/2011-------------
		   DateTimeControl(); 
	       } 
	       else  { 
		   ret = 1; //выдать сообщении о необходимости синхронизации
	       }
	       if ( ret == 1 )          SynchronizationWarning(true);
           }  else  SynchronizationWarning(false);	// Время синхронизированно в допустимых пределах
    }
    return ret;
}

//======================================================================//
void RegistrationForm::Check_Change_Time()
{
    //puts("void RegistrationForm::Check_Change_Time()");
    
    if (sp.learn_mode && !sp.write_transaction) return;
    
    change_time->BlinkTime(true);
    int time_for_stop = sp.max_change_time - change_dt_open.secsTo(QDateTime::currentDateTime());
    //printf("time for stop = %d\n", time_for_stop);
    StateStore::GetInstance()->SetSessionState(ENDING);
    //change_mode = 'w';
    
    if (time_for_stop > 0)
	QTimer::singleShot(time_for_stop * 1000 - 20, this, SLOT(Block_Change()));
    else
	QTimer::singleShot(1, this, SLOT(Block_Change()));
}

//======================================================================//
void RegistrationForm::Block_Change()
{
    //puts("void RegistrationForm::Block_Change()");
    
    
    change_time->StopTime();
    if ( sp.learn_mode && !sp.write_transaction ) return;
    StateStore::GetInstance()->SetSessionState(BLOCKED);
}
//======================================================================//
int RegistrationForm::SetOpenSessionTime()
{
    //puts("int RegistrationForm::SetOpenSessionTime()");
    
    if ( sp.learn_mode && !sp.write_transaction ) return 0;
    
    int time_to_go = sp.max_change_time - change_dt_open.secsTo(QDateTime::currentDateTime());
   // printf("sp.max_change_time =%d\n",sp.max_change_time);
    //printf("change_dt_open.secsTo(QDateTime::currentDateTime()) =%d\n",change_dt_open.secsTo(QDateTime::currentDateTime()));
    if ( time_to_go > 0 )  {
	QTime session_time_to_go(0,0,0);
	session_time_to_go = session_time_to_go.addSecs(time_to_go);
	change_time->StopTime();
	change_time->SetTime(session_time_to_go, false);
	change_time->StartTime();
    }  else  {	
//	puts("Время смены закончилось.... необходимо закрыть смену");
	Error(74); // Время смены закончилось.... необходимо закрыть смену
	return 1;
    }
    return 0;
}
//############################--END--DATE--AND--TIME--SECTION--###############################//

//############################--VISUAL--DATA-ON--FORM--SECTION--##############################//

//======================================================================//
int RegistrationForm::AddRowInTable(int i)
{
    //puts("RegistrationForm::AddRowInTable(int i)");
    QString s;
    if ( receiptTable->numRows() != receipt->record_num )   receiptTable->setNumRows(receipt->record_num);
    //puts("");
    
    receiptTable->setRowHeight( i, 27);
    
    QTableItem *i0= new QTableItem(receiptTable, QTableItem::Never, DbCodec->toUnicode(receipt->GetPosElemRef(i)->GetLongtitle()));  
    receiptTable->setItem(i, 0, i0);
    QTableItem *i1= new QTableItem(receiptTable, QTableItem::Never, s.setNum(receipt->GetPosElemRef(i)->price, 'f', 2));  
    receiptTable->setItem(i, 1, i1);
    QTableItem *i2= new QTableItem(receiptTable, QTableItem::Never, s.setNum(receipt->GetPosElemRef(i)->quantity, 'f', 3));
    receiptTable->setItem(i, 2, i2);
    QTableItem *i3= new QTableItem(receiptTable, QTableItem::Never, s.setNum(receipt->GetPosElemRef(i)->sum, 'f', 2));  
    receiptTable->setItem(i, 3, i3);
    QTableSelection sel = receiptTable->selection(0);
    int p_sel = sel.topRow();
    if ( p_sel != -1 )  {
	sel.init(p_sel, 0);
	sel.expandTo(p_sel, 3);
	receiptTable->removeSelection(sel);
    }
    p_sel = i;
    sel.init(p_sel, 0);
    sel.expandTo(p_sel, 3);
    receiptTable->addSelection(sel); 
    receiptTable->ensureCellVisible(p_sel,0);
    
    return 0;
}

//======================================================================//
void RegistrationForm::PositionPreference(int pos)
{
    //puts("RegistrationForm::PositionPreference(int pos)");
    deliveryTextLabel->setText("");
    QString s;
    if ( receipt->GetPosElemRef(pos)->enable_flag )  {
	//ВЫВОД НАИМЕНОВАНИЯ ТОВАРА ТЕКУЩЕЙ ПОЗИЦИИ
	SelfScalingNameposTaxLabel(DbCodec->toUnicode(receipt->GetPosElemRef(pos)->GetLongtitle()));
	nameposTextLabel->setText(DbCodec->toUnicode(receipt->GetPosElemRef(pos)->GetLongtitle()));
	
	//ВЫВОД ШТРИХКОДА И ВНУТРЕННЕГО КОДА ТЕКУЩЕЙ ПОЗИЦИИ
	QString codes = receipt->GetPosElemRef(pos)->GetBarcode();
	codes += '/';
	codes += receipt->GetPosElemRef(pos)->GetCode();
	codeposTextLabel->setText(DefCodec->toUnicode(codes));
	AdjustLabelFont(codeposTextLabel);
	
	//ВЫВОД КОЛИЧЕСТВА/МАССЫ ТЕКУЩЕЙ ПОЗИЦИИ
	s.setNum(receipt->GetPosElemRef(pos)->quantity, 'f', 3);
	quantposTextLabel->setText(s);
	//ВЫВОД ЦЕНЫ ТЕКУЩЕЙ ПОЗИЦИИ
	s.setNum(receipt->GetPosElemRef(pos)->price, 'f', 2);
	priceposTextLabel->setText(s);
	//ВЫВОД СТОИМОСТИ С УЧЕТОМ СКИДОК И НАДБАВОК ТЕКУЩЕЙ ПОЗИЦИИ.	
	s.setNum(receipt->GetPosElemRef(pos)->sum - receipt->GetPosElemRef(pos)->RecalculationDiscounts(), 'f', 2);
	sumposTextLabel->setText(s);
	//ВЫВОД СУММЫ СКИДОК И НАДБАВОК ТЕКУЩЕЙ ПОЗИЦИИ	
	s.setNum(receipt->GetPosElemRef(pos)->RecalculationDiscounts(), 'f', 2);
	redposTextLabel->setText(s);
	
	//ВЫВОД МЕТКИ ДЛЯ ПОЛЯ КОЛИЧЕСТВО/МАССА
	if ( receipt->GetPosElemRef(pos)->flags & ALLOWFRACTIONAL == 1 )
	    tquantposTextLabel->setText(DefCodec->toUnicode("масса"));
	else
	    tquantposTextLabel->setText(DefCodec->toUnicode("количество"));
	
	//ВЫВОД МЕТКИ ДЛЯ ПОЛЯ СКДИКИ/НАДБАВКИ ПОЗИЦИИ
	if ( receipt->GetPosElemRef(pos)->dsum > 0 )
	    tredposTextLabel->setText(DefCodec->toUnicode("скидка"));
	else if ( receipt->GetPosElemRef(pos)->dsum < 0 )
	    tredposTextLabel->setText(DefCodec->toUnicode("надбавка"));
    }    else
	ClearPositionInfo();
    //ВЫВОД СУММЫ ВСЕХ СКИДОК И НАДБАВОЙ ТЕКУЩЕГО ЧЕКА
    s.setNum(receipt->GetAllDiscountSum(), 'f', 2);
    redallTextLabel->setText(s);
    
    s.setNum(receipt->GetResultSum(), 'f', 2);
    allTextLabel->setText(s);
}

//======================================================================//
void RegistrationForm::SetSelectionUp()
{
    // puts("RegistrationForm::SetSelectionUp()");
    QTableSelection sel = receiptTable -> selection(0);
    int p_sel = sel.topRow();
    
    if ( (p_sel !=0) && (p_sel != -1) )  {
	sel.init(p_sel, 0);
	sel.expandTo(p_sel, 3);
	receiptTable->removeSelection(sel);
	
	sel.init(--p_sel, 0);
	sel.expandTo(p_sel, 3);
	receiptTable->addSelection(sel);
	receiptTable -> ensureCellVisible(p_sel,0);
	// PositionPreference(p_sel);   
    }
}

//======================================================================//
void RegistrationForm::SetSelectionDown()
{
    // puts("RegistrationForm::SetSelectionDown()");
    QTableSelection sel = receiptTable -> selection(0);
    int p_sel = sel.topRow();
    
    if ( (p_sel < receiptTable -> numRows()-1 ) && ( p_sel != -1) )   {  
	sel.init(p_sel, 0);
	sel.expandTo(p_sel, 3);
	receiptTable->removeSelection(sel);
	
	sel.init(++p_sel, 0);
	sel.expandTo(p_sel, 3);
	receiptTable->addSelection(sel);
	receiptTable -> ensureCellVisible(p_sel,0);
	//   PositionPreference(p_sel);
    }
}

//======================================================================//
void RegistrationForm::ClearPositionInfo()
{
    //puts("RegistrationForm::ClearPositionInfo()");
    nameposTextLabel->setText("");
    codeposTextLabel->setText("");
    quantposTextLabel->setText("");
    //tquantposTextLabel->setText(DefCodec->toUnicode("кол-во/масса"));
    priceposTextLabel->setText("");
    sumposTextLabel->setText("");
    //tsumposTextLabel->setText(DefCodec->toUnicode("стоимость"));
    redposTextLabel->setText("");
    //tredposTextLabel->setText(DefCodec->toUnicode("скидка"));
}

//======================================================================//
void RegistrationForm::ClearForNewOperation()
{
    //puts("RegistrationForm::ClearForNewOperation()");
    receiptTable->setNumRows(0);
    
    ClearPositionInfo();
    
    //-----(+)-----Zuskin-----18/10/2011-----
    manager_id = "";
    manager_name = "";
    SetManagerName();
    //--------------------------------------------------
    
    tredallTextLabel->setText(DefCodec->toUnicode("итого скидка"));
    redallTextLabel->setText("");
    
    if ( sp.clear_window_after_close_check   ) {	
	allTextLabel->setText("");
	deliveryTextLabel->setText("");
    }
    
    if ( check_closed ) check_closed = false;
    
    StateStore::GetInstance()->SetViewState(GENERAL);
    
    if(StateStore::GetInstance()->GetSessionState() != BLOCKED)    modeTextLabel->setText(DefCodec->toUnicode("готов к работе"));
    else    modeTextLabel->setText(DefCodec->toUnicode("время смены закончилось"));
    ctrl_v= FALSE;
    
    
    
    //  printf("--------------2---------------IN ClearForNewOperation()   CTRL+V =%d\n",ctrl_v);
    
}
//#########################--END--VISUAL--DATA-ON--FORM--SECTION--############################//

//###################################--KEYBOARD--SECTION--###################################//
//======================================================================//
void RegistrationForm::GetKeySequence()
{
    puts("-----------------Reg --- GetKeySequence()---------------------");
    
    if ( kbcode_scaner != NULL ) {
	int n = kbcode_scaner->GetCountEvent();
	for ( int i = 0; i < n; i++ )   {
	    makeKeyPress( kbcode_scaner->GetEvent(i) );	
	    adjustFont();
	    textTextLabel -> setText(str);
	}
	kbcode_scaner->Clear();
    }
    
    if ( kbcode_reader != NULL ) {
	int n = kbcode_reader->GetCountEvent();
	for ( int i = 0; i < n; i++ )   {
	    makeKeyPress( kbcode_reader->GetEvent(i) );	
	    adjustFont();
	    textTextLabel -> setText(str);
	}
	
	kbcode_reader->Clear();
    }
}
//======================================================================//
void RegistrationForm::keyPressEvent(QKeyEvent *k)
{
    if ( ! isActiveWindow()  ) return;
    
    if ( keyboard_block ) return;
    
    int c_key = k->key(); 
    int c_state = k->state();
    
    
    puts("------reg key press------");
    printf("c_key = %d \n", c_key);
    printf("c_ascii = %d\n", k->ascii());
    printf("c_state = %d \n", c_state);
    printf("c_text = %s \n", k->text().ascii());
    printf("c_text len = %d \n", k->text().length());
    puts("------------"); 
    
    
    if ( sp.autoblock_flag ) bt->action_time = QTime::currentTime();
    if ( StateStore::GetInstance()->GetViewState() == MESS )  {
	str = "";
	//-----(*)-----Zuskin-----30/09/2011-----
	//StateStore::GetInstance()->SetViewState(GENERAL);  
	StateStore::GetInstance()->SetViewState(StateStore::GetInstance()->GetPrevState());
	//-------------------------------------------------
    }
    
    if ( StateStore::GetInstance()->GetViewState() == INS_TEXT )  {   
	str = "";
	//-----(+)-----Zuskin-----16/12/2011-----
	  printf("StateStore::GetInstance()->GetViewState() = %d\n", StateStore::GetInstance()->GetViewState());
	  printf("StateStore::GetInstance()->GetPrevState() = %d\n", StateStore::GetInstance()->GetPrevState());
	StateStore::GetInstance()->SetViewState(StateStore::GetInstance()->GetPrevState());
	//--------------------------------------------------
	StateStore::GetInstance()->SetViewState(INS);  
    }
    
    if (statusTextLabel->text() == DefCodec->toUnicode("сумма в ящике"))  {
	str = "";
	statusTextLabel->setText(DefCodec->toUnicode(""));
    }
    
    //----------------------!!!!!!!!!!!!!!!!!!------------------------
    int group = 0; 
    if  (XkbGetState( g_d, XkbUseCoreKbd, &state ) == Success)      group = state.locked_group;
    if (group == 0)       lang_label->setText("ENG");
    else       lang_label->setText("RUS");	
    //----------------------!!!!!!!!!!!!!!!!!!------------------------
    
    int state = 0;
    
    if ( hw->BCS_GetType() == 2 )  {
	state = kbcode_scaner->FindCode(k);
	//printf("status = %d \n", state);
	if ( state == 2 )  {
	    QString barcode_str = ((KeyboardScaner*) kbcode_scaner)->getCode();
	    char* barcode = (char*) calloc(barcode_str.length()+1, sizeof(char));
	    strcpy(barcode, barcode_str);
	    // printf("-----wtrix kod = <%s> \n", barcode); 
	    
	    //-----(*)-----Zuskin-----21/12/2011-----
	    //BarcodeEvent(barcode_str, false);	
	    barcode_from_scaner = QString(barcode);
	    //-------------------------------------------------
  
	    free(barcode);
	}
    }
    
    if ( hw->MCR_GetType() == 2 && state == 0 ) {
	state = kbcode_reader->FindCode(k);
	//printf("state = %d \n", state);
	if ( state == 2 )  {
	    int num_track;
	    QString code_str = ((KeyboardReader*) kbcode_reader)->getCode(num_track);
	    char* code = (char*) calloc(code_str.length()+1, sizeof(char));
	    strcpy(code, code_str);
	    
	    if (num_track == 2) CardEvent( code );
	    free(code);
	}		
    }	
    
    //printf(" -----------   find code returns state = %d ---------------\n", state);
    
    if ( state == 0 )  { 
	makeKeyPress(k);
	adjustFont();
	textTextLabel -> setText(str);
	return;   
    }
    
    if ( state == 5 )    {  GetKeySequence();  return; }
    if ( state == 7 || state == 6 ) {  Error(1000); return;  }
    
}

//======================================================================//
void RegistrationForm::makeKeyPress(QKeyEvent * k) 
{ 
    /*
    puts("------reg make key press------");
    printf("c_key = %d \n", k->key());
    printf("c_state = %d \n", k->state());
    printf("c_ascii = %d \n", k->ascii());
    printf("c_text = <%s> \n", k->text().ascii());
    printf("c_text len = %d \n", k->text().length());
    puts("------------");    
    
    
    cout << "text coded <" << DefCodec->fromUnicode(k->text()) << ">" << endl;*/
    
    //printf("makeKeyPress k -> key() = %d \n", k->key());
    
    if (clrflg) { str=""; clrflg=false; }
    
    QString all_s;
    if ( StateStore::GetInstance()->GetReceiptState() == FREE )  modeTextLabel->setText(DefCodec->toUnicode("готов к работе"));
    int c_key = k->key(); 
    
    
    if ( (k->state() & Qt::AltButton)  ) {
	if ( (strcmp(DefCodec->fromUnicode(k->text()), "В") == 0) || (strcmp(DefCodec->fromUnicode(k->text()), "в") == 0)) { 
	    SumReduction(false);	  
	    return;
	}
	if ( (strcmp(DefCodec->fromUnicode(k->text()), "Т") == 0) || (strcmp(DefCodec->fromUnicode(k->text()), "т") == 0) ) {
	    SumReduction(true);
	    return;
	}
    }   
    
    
    
    if ( (strcmp(DefCodec->fromUnicode(k->text()), "З") == 0) || (strcmp(DefCodec->fromUnicode(k->text()), "з") == 0 ) ) {
	if ( k->state() & Qt::AltButton ) {	ReductionPrice();  	  return;    }  	    
	if ( k->state() & Qt::ControlButton )   {  	PrintReceipt();   return;  }	
	if (str.length() < INPUT_WIDTH)  { str = str + k->text();    return;   }
	return;
    }
    
    if ( StateStore::GetInstance()->GetReceiptState() == FREE ) {
	allTextLabel->setText("");
	deliveryTextLabel->setText("");
	if ( c_key != Qt::Key_F10 && c_key != Qt::Key_F11 )  textTextLabel->setText("");
    }
    
    //  switch(k->key())
    switch ( c_key )    {
    case Qt::Key_Space :	break;
    case Qt::Key_Backspace : 	{ 
	    //puts("Key backspace press");
	    if ( str.length() > 0 )  str = str.left((str.length()) - 1);
	    if (DefaultFontSize>LastFontSize)    LastFontSize++;   
	    break;    }
    case Qt::Key_Plus : 	{
	    keyboard_block = true;
	    int prob = 0;
	    if ( (k->state() & Qt::ControlButton) )  kkm_reg->KkmSetCtrlState(true);   
	    
	    if ( StateStore::GetInstance()->GetReceiptState() == SELL )   {
		prob = Payment();
		if ( prob == 0 )	DateTimeControl();
		break;
	    }		
	    
	    if ( StateStore::GetInstance()->GetReceiptState() == RETURN )   {
		prob = CloseReturnType(3);	
		if ( prob == 0 )	DateTimeControl();
		break;
	    }
	    
	    if ( StateStore::GetInstance()->GetReceiptState() == COPY )  {
		copy_flag = true;
		if ( prob == 0 )	DateTimeControl();
		break;
	    }
	    
	    kkm_reg->KkmSetCtrlState(false);
	    Error(14);
	    
	    if ( prob == 0 )	DateTimeControl();
	    break;  
	}
    case Qt::Key_Return :	{   	PaymentType(1);   	    break;    	}
    case Qt::Key_Enter:	{	PaymentType(1);   	    break;  	}
    case Qt::Key_Escape :	{
	    str = "";
	    if ( StateStore::GetInstance()->GetReceiptState() == FREE )   allTextLabel->setText("");
	    deliveryTextLabel->setText("");
	    quantity_flag = false;
	    quantTextLabel->setText(DefCodec->toUnicode(""));
	    AddWareController::GetInstance()->EscapeQuantity();
	    break;	}
    case Qt::Key_Asterisk :	{	    SetQuantity();	    break;		}
    case Qt::Key_Insert :	{	    IntermediateResult();	    break;   	}
    case Qt::Key_Tab :	{	    break;   	}
    case Qt::Key_Home :	{
	    if((k->state() & Qt::ControlButton))	SumInBox();
	    //else          DumpBalance();
	    break;	    }
    case Qt::Key_Delete :   	{
	    //сторно, возврат, отмена чека
	    //if((k->state() & Qt::ShiftButton)) 
	    //  Cancellation();
	    // Отменить чек можно и при  уже закончившейся смене.....
	    if((k->state() & Qt::AltButton))  	    {
		CancelCheck();
		DateTimeControl();
		break;
	    }  
	    if ( (k->state() & Qt::ControlButton) )  {
		ReturnCheck();
		break;
	    }
	    Storno();
	    break;		
	}
	//Pg Up
    case Qt::Key_Prior :  	{	AdvancePaper();	    break; 	}
    case Qt::Key_Next :	{    	OpenBox(false);	    break;		}
    case Qt::Key_Up :	{	SetSelectionUp();	    break;      	}
    case Qt::Key_Down :	{	SetSelectionDown();    break;      	}
    case Qt::Key_Left :	break;
    case Qt::Key_Right :	break;
    case Qt::Key_F1:	{
	    //puts("всплывающая подсказка");
	    promptingForm * prompt = new promptingForm;
	    //-----(*)-----Zuskin-----09/08/2011-----
	    //prompt->exec();
	    if ( prompt->exec() == QDialog::Accepted )    {
		// можно вставить вызов выбранной в списке функции
	    }
	    //------------------------------------------------
	    delete(prompt);
	    break;	}
	//Визуальный поиск 
    case Qt::Key_F6 :   {	RegByName(str);	    break;      }
    case Qt::Key_F2 :   {	
	    //-----(*)-----Zuskin-----03/08/2011-----
	    //AddWareByBarcode("", str, true);
	    AddWareByBarcode("", str, true, -1);
	    //-------------------------------------------------
	    break;	
	}
    case Qt::Key_F3 :   {	    BarcodeEvent( str, true );   break;	}
    case Qt::Key_F5 :  {	    PaymentType(0);	    break;     	}
    case Qt::Key_F7 :  {	    CardEvent( str );	break; 	}
    case Qt::Key_F8 : {	    hw->ECR_ContinuePrinting();	    break;     	}	
    case Qt::Key_F9 : {	    PaymentType(2);   break;     	}	
	//внесение денег в кассу
    case Qt::Key_F10 : {	    Entering();	    break;		}
	//выплата денег из кассы	
    case Qt::Key_F11 :  {	    Getting();	    break;   	}
	//выход
    case Qt::Key_F12 :  {	    CloseRegistrationForm(true);    break;	}
    case Qt::Key_P :	{
	    if ( k->state() & Qt::AltButton ) {	ReductionPrice();  	    break;     }  	    
	    if ( k->state() & Qt::ControlButton )   {  	PrintReceipt();   break;  }	
	    if (str.length() < INPUT_WIDTH)  { str = str + k->text();    break;   }	}
    case Qt::Key_Q :	{
	    if((k->state() & Qt::ControlButton))	ReductionQuantity();
	    else  if(str.length() < INPUT_WIDTH) str = str + k->text();
	    break;	}
    case Qt::Key_R :	{
	    if((k->state() & Qt::ControlButton))	RepetitionSale();
	    else  if(str.length() < INPUT_WIDTH) str = str + k->text();
	    break;	}
    case Qt::Key_C :	{
	    if((k->state() & Qt::ControlButton))	CancelReduction();
	    else  if(str.length() < INPUT_WIDTH) str = str + k->text();
	    break;	}
    case Qt::Key_F :	{
	    if((k->state() & Qt::ControlButton))	FixedReduction();
	    else  if(str.length() < INPUT_WIDTH) str = str + k->text();
	    break;	}
    case Qt::Key_D :	{
	    if ( (k->state() & Qt::ControlButton) )  {	PercentReduction(false); break;	}
	    if ( (k->state() & Qt::AltButton) ) {  SumReduction(false);	  break;	}
	    if ( str.length() < INPUT_WIDTH ) {  str = str + k->text();  break;    }
	}
    case Qt::Key_N :	{
	    if ( (k->state() & Qt::ControlButton) ) 	PercentReduction(true);
	    else if((k->state() & Qt::AltButton))	SumReduction(true);
	    else if(str.length() < INPUT_WIDTH)     str = str + k->text();
	    break;	}
    case Qt::Key_V : 	{
	    if ( !ctrl_v )  {
		if ( (k->state() & Qt::ControlButton) )	{
		    //ctrl_v = TRUE;
		    Check();
		}     else  if ( str.length() < INPUT_WIDTH )
		    str = str + k->text();
		break; 
	    } else  {
		Error(36);
		break;
	    }
	}
    case Qt::Key_S :	{
	    if((k->state() & Qt::ControlButton))	Service();
	    else // if(k->state() & Qt::NoButton)
		if(str.length() < INPUT_WIDTH)  str = str + k->text();
	    break;	}
    case Qt::Key_M :	{
	    if ( (k->state() & Qt::ControlButton))	hw->MD_Switch();
	    else // if(k->state() & Qt::NoButton)
		if ( str.length() < INPUT_WIDTH )  str = str + k->text();
	    break;	}
    case Qt::Key_B :	{
	    if((k->state() & Qt::ControlButton))  { Block(false);    }
	    else  if(str.length() < INPUT_WIDTH) str = str + k->text();
	    break;	}
    case Qt::Key_0 :	{
	    if(k->state() & Qt::ControlButton)	    {
		if(str.length() < INPUT_WIDTH - 1)    str = str + "00";	    }
	    else  if(str.length() < INPUT_WIDTH) str = str + k->text();
	    break;	}
    //-----(+)-----Zuskin-----01/08/2011-----
    case Qt::Key_E :	{
	    if((k->state() & Qt::ControlButton))  { ShowEDSForm(); }
	    else  if(str.length() < INPUT_WIDTH) str = str + k->text();
	    break;	}
    case Qt::Key_F4 :   { RegisterByPrice();   break;	 }
   //--------------------------------------------------
   //-----(+)-----Zuskin-----10/10/2011-----
   case Qt::Key_L :	{
	    if((k->state() & Qt::ControlButton))	PutOut();
	    else // if(k->state() & Qt::NoButton)
		if(str.length() < INPUT_WIDTH)  str = str + k->text();
	    break;	}	
   //-------------------------------------------------	
   //-----(+)-----Zuskin-----16/11/2011-----
    case Qt::Key_Z :	{
	    if((k->state() & Qt::ControlButton))  { ShowManagerForm(); }
	    else  if(str.length() < INPUT_WIDTH) str = str + k->text();
	    break;	}
    //---------------------------------------------------	
       
    default :       {  if(str.length() < INPUT_WIDTH)  str = str + k->text();  }
    
    }
    //printf("str.ascii() = %s \n", str.ascii());
}
//======================================================================//
void RegistrationForm::focusInEvent ( QFocusEvent * f )
{
    //puts("focusInEvent");
    //QMessageBox::information( this, DefCodec->toUnicode("Сообщение"),DefCodec->toUnicode("focusInEvent"), "OK",0);
    //puts("RegistrationForm::focusInEvent");
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
}
//======================================================================//
void RegistrationForm::focusOutEvent ( QFocusEvent * f )
{
    //puts("focusOutEvent");
    //QMessageBox::information( this, DefCodec->toUnicode("Сообщение"),DefCodec->toUnicode("focusInEvent"), "OK",0);
    //puts("RegistrationForm::focusInEvent");
    //HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
}
//======================================================================//
void RegistrationForm::showEvent ( QShowEvent * )
{
    //QMessageBox::information( this, DefCodec->toUnicode("Сообщение"),DefCodec->toUnicode("showEvent"), "OK",0);
    puts("RegistrationForm::showEvent");
}
//======================================================================//
void RegistrationForm::paintEvent ( QPaintEvent * )
{
    //QMessageBox::information( this, DefCodec->toUnicode("Вы вошли на территорию рмк"),DefCodec->toUnicode("paintEvent"), "OK",0);
    //Error(152);
    //puts("RegistrationForm::paintEvent");
/*    
    for ( int i = 0 ;i < 100; i++  ) {
	puts(".........");
	sleep(1);
    }
*/
}
//======================================================================//
void RegistrationForm::leaveEvent ( QEvent * e ) 
{
    //QMessageBox::information( this, DefCodec->toUnicode("Сообщение"),DefCodec->toUnicode("wheelEvent "), "OK",0);
    puts("RegistrationForm::leaveEvent ( QEvent * )  ");
}
//======================================================================//
void RegistrationForm::enterEvent ( QEvent * e ) 
{
    //QMessageBox::information( this, DefCodec->toUnicode("Сообщение"),DefCodec->toUnicode("wheelEvent "), "OK",0);
    puts("RegistrationForm::enterEvent ( QEvent * )  ");
}
//======================================================================//
void RegistrationForm::ReleaseKeyboard()
{
    releaseKeyboard();
    active = false;
}

//======================================================================//
void RegistrationForm::GrabKeyboard()
{
    grabKeyboard();
    active = true;
}
//======================================================================//

//###################################--END--KEYBOARD--SECTION--##############################//

//##############################--PAYMENT--SECTION--#########################################//

//======================================================================//
int RegistrationForm::PaymentTypeProperties(double nal, double cred, double cont, double check_sum)
{
    //puts("RegistrationForm::PaymentTypeProperties(double nal, double cred, double cont, double check_sum)");
    
    double paymentsum = nal + cont + cred;
    QString s;
    s.setNum(paymentsum, 'f', 2);
    paymentsum = s.toDouble();
    
    if ( paymentsum < check_sum )  {
	Error(29);
	return -1;
    }	
    
    nalsum = nal;
    credsum = cred;
    contsum = cont; 
    
    if ( paymentsum > check_sum )    {
	s.setNum(paymentsum - check_sum, 'f', 2);
	deliveryTextLabel->setText(s);
	str = DefCodec->toUnicode("СДАЧА: ") + s;
    }    else   {
	deliveryTextLabel->setText("0.00");
    }
    
    statusTextLabel->setText(DefCodec->toUnicode("оплата кред/нал/тарой"));
    statusTextLabel->repaint();
    return 0;
}
//======================================================================//
int RegistrationForm::PaymentTypeForm(int type, double client_sum)
{
    
    //printf("typr = %d \n, client_sum = %f \n", type, client_sum);
    
    int ret = 0;
    double check_sum = receipt->GetResultSum();
    double nal = 0, cred = 0, cont = 0;
    int * indexes = NULL, num_rights = 0;
    
    ReleaseKeyboard();
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    //printf("check sum = %f \n", check_sum);
    paymentForm *pf = new paymentForm;
    pf->SetPaymentSum(check_sum);
    pf->SetValidatorProperties();
    
    pf->SetCheckNum(hw->ECR_GetCheckNumber());
    
    // if(str != "") pf->SetNalPaymentSum(s);
    
    //  оплата наличными без сдачи
    if ( type == 0 ) pf->SetNalPaymentSum(check_sum);
    //оплата наличными со сдачей от client_sum
    if ( type == 1 ) {
	//-----(*)-----Zuskin-----30/09/2011-----
	ret = pf->SetNalPaymentSum(client_sum);
	if (ret != 0) {
	    delete(pf);
	    return ret;
	}
	//--------------------------------------------------
    }
    // оплата кредитом
    if ( type == 2 ) pf->SetCredPaymentSum(check_sum);
    
    //    pf->SetNalPaymentSum(s); 
    
    pf->SetReadyFlag(true);
    
    if ( pf->exec() == QDialog::Rejected )    {
	keyboard_block = false;
	pf->RejectedClosePaymentForm();
    } else {
	keyboard_block = true; 
    }
    
    pf->GetSumParameters(nal, cred, cont, check_sum);
    if ( hw->ECR_GetProtocol() == 6 ) {
	int count_payment_type = 0;
	if (nal > 0) count_payment_type++;
	if (cred > 0) count_payment_type++;
	if (cont > 0) count_payment_type++;
	if ( count_payment_type > 1 ) {
	    Error(109);
	    delete(pf);
	    return -1;
	}
    }
    
    //printf("nal = %f \ncred = %f \ncont = %f \ncheck_sum = %f \n");
    
    pf->GetRightsParameters(indexes, num_rights);
    kkm_reg->SetCreditStatus(pf->GetCreditStatus());
    
    
    delete(pf);
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();
    if ( nal != -1 )  {
	if ( (num_rights != 0) && !AuthorizationRequestForAlliedRights(indexes, num_rights) )    {
	    Error(72);
	    ret = -1;
	}    else  {
	    ret = PaymentTypeProperties(nal, cred, cont, check_sum);
	}
	
	if ( authorization_request_flag) SwapToAuthorizedUser();
    }	
    else ret = -1;
    
    if ( indexes != NULL ) free(indexes);
    
    //if ( ret == -1 ) 
    
    return ret;
}

//======================================================================//
int RegistrationForm::PaymentType(int type)
{
    if ( payment_flag ) return 0;
    
    //Kkm_Error(-1);
    
    clrflg=true;
    int ret = -1;
    int r_state = StateStore::GetInstance()->GetReceiptState();
    
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED ) {	    Error(74);  return -1;  }
    
    //if ( r_state == RETURN )  {  Error(52);  return -1;  }
    if ( r_state == RETURN )  { CloseReturnType(type);  return 0;   }
    
    if ( r_state == FREE ) {    Error(14); return -1; }
    if ( r_state == COPY )  {   Error(36);  return -1;  }
    
    if ( (sp.sections.size() > 0) && (type == 2) ) {  Error(112);  return -1;  }
    
    if ( quantity_flag )  {    Error(41);  return -1;   }
    if ( (receipt->Sum() == 0) && (receiptTable->numRows() != 0) )  {    Error(48);   return -1;  } 
    
    //-----(+)-----Zuskin-----15/12/2011-----
    // попробуем рассчитать автоматические скидки
    CheckReduction(true, 0, 0, "-1", "", "", false);
    //-------------------------------------------------
    
    if ( str != "" )      {
	bool nf;
	double s = str.toDouble(&nf);
	s = Round(s * 100) / 100; 
	
	// if(!nf && ((0 <=s) || (s > 99999999.99)))
	//-----(*)-----Zuskin-----30/09/2011-----
//	if ((s <= 0) || (s > 99999999.99))    {      Error(25);  return -1;  }
	if ((s < 0) || (s > 99999999.99))    {      Error(25);  return -1;  }	
	//-------------------------------------------------
	if ( !sp.query_for_authorization && !GetRight(15) )    {      Error(72);    return -1; }
	
	str = "";
	//ret = PaymentTypeForm(s);
	//-----(*)-----Zuskin-----30/09/2011-----
	//ret = PaymentTypeForm(1, s);
	if (s == 0) {
	    ret = PaymentTypeForm(type, allTextLabel->text().toDouble());
	}
	else {
	    ret = PaymentTypeForm(1, s);
	}    
	//-------------------------------------------------
	
    }  else  {
	ret = PaymentTypeForm(type, allTextLabel->text().toDouble());	
    }	  
    
    if  ( ret != -1 )    {
	PaymentCustomerDisplay();
	
	if ( sp.open_box_by_close_cheque )      {
	    if ( sp.open_box_by_pay_with_rest && nalsum > 0 ) OpenBox(true);
	    else if ( !sp.open_box_by_pay_with_rest ) OpenBox(true);
	}
	
	payment_flag = true;  
	
    }
    
    /*if ( kkm_reg->GetCreditStatus() != -1 && ret == -1 ) {
	// послать аварийную транзакцию в нпас
	if ( hw->PS_GetTypeExchange() == 1 ) NPAS_CrushTransaction();
    }*/
    
    return ret;
}

//======================================================================//
void RegistrationForm::NPAS_CrushTransaction()
{   
    hw->PS_SetCodeOper(7);
    
    /*puts("===========================");
    printf("Uniq Number transaction (refer num) : %d \n", NumTrans);
    printf("Check Summ : %f \n", Check_Summ);
    printf("Original code operation : %d \n", OriginalCodOper);
    printf("AdditSumm : %f \n", Addit_Summ);
    printf("RRN : %s \n", ReferNum.ascii());
    puts("===========================");*/
    
   // puts("hw->PS_SendRequest();");
    int k = hw->PS_SendRequest();
  //  printf("k = %d \n", k);
    kkm_reg->SetCreditStatus(-1);
}
//======================================================================//
void RegistrationForm::PaymentCustomerDisplay()
{
    //puts("void RegistrationForm::PaymentCustomerDisplay()");
    QString upstr;
    upstr = HwCodec->fromUnicode(DefCodec->toUnicode("ИТОГО:"));
    upstr += allTextLabel->text();
    QString downstr = HwCodec->fromUnicode(DefCodec->toUnicode("СДАЧА:"));
    downstr +=  deliveryTextLabel->text();
    
    CDSP cdsp(hw);
    cdsp.CustomerDisplay(upstr.ascii(), downstr.ascii());    
}

//======================================================================//
int RegistrationForm::PaymentProperties()
{
    //puts("RegistrationForm::PaymentProperties()");
    QString all_s;
    
    //оплата
    if ( str == "" )   {
	//printf("ОПЛАТА БЕЗ СДАЧИ\n");
	statusTextLabel->setText(DefCodec->toUnicode("оплата нал "));
	statusTextLabel->repaint();
	
	deliveryTextLabel->setText("0.00");
	deliveryTextLabel->repaint(); 
	
	nalsum = allTextLabel->text().toDouble();
	if ( sp.open_box_by_close_cheque )      {
	    OpenBox(true);
	}
	
    }  else  {
	// оплата со сдачей  
	//printf("ОПЛАТА СО СДАЧЕЙ\n");
	bool nf;
	double s = str.toDouble(&nf);
	
	//  if(!nf && ((0 <=s) || (s > 99999999.99)))
	if ( (!nf) || (nf) && ((0 > s) || (s >= 99999999.99)) )    {
	    Error(25);
	    return -1;
	}
	
	//double check_sum = receipt->Sum() - receipt->RecalculationDiscounts();
	double check_sum = receipt->GetResultSum();
	if ( sp.limit_client_sum &&  (s > sp.max_money + check_sum) ) {
	    // сумма клиента превышает значение максимальной купюры + суммы чека
	    Error(100);
	    return -1;
	}
	
	QString chstr;  
	chstr.setNum(check_sum, 'f', 2);
	check_sum = chstr.toDouble();
	
	if ( s < check_sum )        return PaymentTypeForm(1, s);
	else      {
	    statusTextLabel->setText(DefCodec->toUnicode("оплата нал. со сдачей"));
	    all_s.setNum(s - receipt->GetResultSum(), 'f', 2);
	    deliveryTextLabel->setText(all_s);
	    nalsum = s;
	    str = DefCodec->toUnicode("СДАЧА: ") + all_s;
	    if ( sp.open_box_by_close_cheque )      {
		OpenBox(true);
	    }
	}
	
    }
    return 0;
}

//======================================================================//
int RegistrationForm::Payment() /* + */
{
    //  puts("RegistrationForm::Payment()");
    
    clrflg=true;    
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED)  {  Error(74);  return -1;  }      
    
    if ( quantity_flag )  { Error(41);  return -1;   }
    
    if ( (receipt->Sum() == 0) && (receiptTable->numRows() != 0) )  {  Error(48);   return -1;   }  
    
    if ( PaymentProperties() != 0 )  { return -1;   }
    
    if ( !GetRight(15) && !AuthorizationRequest(15))    {  Error(72);     return -1;   }
    
    //-----(+)-----Zuskin-----15/12/2011-----
    // попробуем рассчитать автоматические скидки
    CheckReduction(true, 0, 0, "-1", "", "", false);
    //-------------------------------------------------    
    
    PaymentCustomerDisplay();
    
    payment_flag = true;
    
    if(authorization_request_flag) SwapToAuthorizedUser();
    
    return 0;
}

//#########################--END--PAYMENT--SECTION--########################################//


//########################--WORK--WITH--SUBFORMS--SECTION--##################################//
//======================================================================//
void RegistrationForm::Service() /*CTRL + S*/ //Сервис
{
    //puts("RegistrationForm::Service()");
    bool right_flag = GetRight(19) || GetRight(20) || GetRight(21) || GetRight(27);
    // Даже если прав нет, форма загрузится.... а там уже будет производиться запрос авторизации..
    //  if(sp.query_for_authorization && !right_flag) 
    //    right_flag = true;
    if ( !sp.query_for_authorization && !right_flag )  {
	Error(72);
	return;
    }   
    ReleaseKeyboard();
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
    serviceForm *sf = new serviceForm;
    //sf->SetParams(hw);
    
    connect(sf, SIGNAL(ZReport()), this, SLOT(ZReportSuccessed()));
    connect(sf, SIGNAL(Sync()), this, SLOT(DateTimeSync()));
    
    sf->exec();
    delete(sf);
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();
    
    kkm_res = kkm_reg->KkmSetMode(1);
    if ( kkm_res !=0 )    Kkm_Error(kkm_res);
}

//======================================================================//
void RegistrationForm::ZReportSuccessed()
{
    //puts("void RegistrationForm::ZReportSuccessed()");
    int res = InitChangeTime();
    if(res != 0)
    {
	Message(3);
	CloseRegistrationForm(false); 
    }
}

//======================================================================//
void RegistrationForm::VisualSearch() /*F6*/ //Визуальный поиск
{
    puts("***************VISUAL *** SEARCH **********");
    
    //printf("StateStore::GetInstance()->GetViewState() = %d \n", StateStore::GetInstance()->GetViewState());	
    //printf("StateStore::GetInstance()->GetSessionState() = %d \n", StateStore::GetInstance()->GetSessionState());
    //printf("StateStore::GetInstance()->GetReceiptState() = %d \n", StateStore::GetInstance()->GetReceiptState());
    //printf("StateStore::GetInstance()->GetViewState() == %d \n", StateStore::GetInstance()->GetViewState());
    
    
    
    if ( StateStore::GetInstance()->GetViewState() == FILL )    ClearForNewOperation();
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED ) {   Error(74);  return;  }
    if ( StateStore::GetInstance()->GetReceiptState() == COPY )  {    Error(33);  return;   }
    
    ReleaseKeyboard();
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    //printf("create vs \n");
    visualForm *vs = new visualForm;
    //printf("vs->check_type = %d \n", vs->check_type);
    if ( StateStore::GetInstance()->GetReceiptState() == RETURN ) vs->check_type = 1;
    //printf("vs->check_type = %d \n", vs->check_type);
    vs->FillTable("0");
 
    QDesktopWidget *d = QApplication::desktop();
    if (d->width() <= 800) vs->setWindowState(Qt::WindowFullScreen);
    
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) disconnect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence()) );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) disconnect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    
    notaddbyvisual = false;
    vs->exec();
    notaddbyvisual = true;
    
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence()) );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    
    delete(vs);
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();
    
}
//########################--END--WORK--WITH--SUBFORMS--SECTION##############################//
//======================================================================//
void RegistrationForm::showDeletedPosition(int p_sel) {
    //int num_spaces = receiptTable->columnWidth (0) / 12; 
    QString spaces = "";
    
    QString textStr = DbCodec->toUnicode(receipt->GetPosElemRef(p_sel)->GetLongtitle());
    QFont f;  f.setStrikeOut(TRUE); f.setFamily("Fixed[Misc]"); f.setBold(FALSE); f.setPointSize(12);
    QFontMetrics fm( f );
    int S_W = fm.width(textStr);	 
    int SP_W = fm.width(" ");
    int C_W = receiptTable->columnWidth (0);
    
    int num_spaces = 0;
    if (C_W > S_W) 	num_spaces = (C_W - S_W) / SP_W + 1;
    
    for ( int i = 0; i < num_spaces; i++ )  spaces += " ";		
    
    StrikeOutTableItem *titm = new StrikeOutTableItem(receiptTable, QTableItem::Never, textStr + spaces);
    receiptTable->setItem(p_sel, 0, titm);
    
    textStr =  QString::number(receipt->GetPosElemRef(p_sel)->price,'f',2);   
    S_W = fm.width(textStr);   
    C_W = receiptTable->columnWidth (1);
    num_spaces = 0;
    spaces = "";
    if (C_W > S_W) 	    num_spaces = (C_W - S_W) / SP_W + 1;
    for ( int i = 0; i < num_spaces; i++ ) spaces += " ";		
    
    StrikeOutTableItem *titm1 = new StrikeOutTableItem(receiptTable, QTableItem::Never, spaces + textStr);
    receiptTable->setItem(p_sel, 1, titm1);
    
    textStr =  QString::number(receipt->GetPosElemRef(p_sel)->quantity,'f',3);   
    S_W = fm.width(textStr);   
    C_W = receiptTable->columnWidth (2);
    num_spaces = 0;
    spaces = "";
    if (C_W > S_W) 	num_spaces = (C_W - S_W) / SP_W + 1;
    for(int i = 0; i < num_spaces; i++) spaces += " ";
    
    StrikeOutTableItem *titm2 = new StrikeOutTableItem(receiptTable, QTableItem::Never, spaces + textStr);
    receiptTable->setItem(p_sel, 2, titm2);
    
    textStr =  QString::number(receipt->GetPosElemRef(p_sel)->sum,'f',2);   
    S_W = fm.width(textStr);   
    C_W = receiptTable->columnWidth (3);
    num_spaces = 0;
    spaces = "";
    if (C_W > S_W) 	num_spaces = (C_W - S_W) / SP_W + 1;
    for(int i = 0; i < num_spaces; i++) spaces += " ";
    StrikeOutTableItem *titm3 = new StrikeOutTableItem(receiptTable, QTableItem::Never, spaces + textStr);
    receiptTable->setItem(p_sel, 3, titm3);   
    
    PositionPreference(p_sel);
}
//======================================================================//
void RegistrationForm::Storno() /*DELETE*/
{
    // puts("RegistrationForm::Storno()");
    if ( StateStore::GetInstance()->GetViewState() == FILL )	   ClearForNewOperation();
    
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED )   {   Error(74);    return ;    }
    if ( StateStore::GetInstance()->GetReceiptState() == COPY )  {   Error(34);  return;   }
    if ( StateStore::GetInstance()->GetReceiptState() == FREE )   {    Error(31);     return;    }
    if (StateStore::GetInstance()->GetViewState() == REC)   { Error(45); return; } 
    //-----(+)-----Zuskin-----16/12/2011-----
    if (StateStore::GetInstance()->GetPrevState() == REC)   { Error(45); return; } 
    //--------------------------------------------------
    
    statusTextLabel->setText(DefCodec->toUnicode("сторнирование"));
    
    QTableSelection sel = receiptTable -> selection(0);
    int p_sel = sel.topRow();
    
    if ( p_sel == -1 )  {  Error(30);   return;     }
    
    if ( !GetRight(4) && !AuthorizationRequest(4) ) {   Error(72);  return;   }
    
    if ( receipt->GetPosElemRef(p_sel)->enable_flag != TRUE ) {  
	Error(17);
	if ( authorization_request_flag ) SwapToAuthorizedUser();
	return;
    }
    
    if ( Acknowledgement(DefCodec->toUnicode("Текущая позиция будет сторнирована\n продолжить операцию?\n")) != 0 )          {
	if ( authorization_request_flag ) SwapToAuthorizedUser();
	return;
    }
    
    receipt->GetPosElemRef(p_sel)->enable_flag = false;
    showDeletedPosition(p_sel);
    
    CDSP cdsp(allTextLabel->text().ascii(), hw);
    cdsp.StornoCustomerDisplay();
    
    SelectionChangedHandler();
    
    if ( authorization_request_flag ) SwapToAuthorizedUser();
    
}
//======================================================================//
void RegistrationForm::pause(int Nsec)
{
    struct timespec R1, R2;
    if (Nsec < 1 || Nsec > 9)
	return;
    R1.tv_sec = 0;
    R1.tv_nsec = Nsec * 100000000;
    if (nanosleep(&R1,&R2)!=0)
	nanosleep(&R2,&R1);
    return;
}
//======================================================================//
int RegistrationForm::CancelCheck() /*ALT + DELETE*/ //Отмена чека
{    
    
   // puts("CancelCheck");
    if ( StateStore::GetInstance()->GetViewState() == FILL )	ClearForNewOperation();
    
    QTableSelection sel = receiptTable -> selection(0);
    
    if ( StateStore::GetInstance()->GetReceiptState() == FREE )  {	Error(15);  return -1;    }
    //  else if(StateStore::GetInstance()->GetReceiptState() == COPY) 

    if ( Acknowledgement(DefCodec->toUnicode("Чек будет отменен\n продолжить операцию?\n")) != 0 )  return -1;
	
    if ( !GetRight(8) && !AuthorizationRequest(8) )    {
	Error(72);
	if ( authorization_request_flag ) SwapToAuthorizedUser();
	return -1;
    }
    
    // insert trans cancel check
    //StateStore::GetInstance()->GetReceiptState() == RETURN - тип чека
    FillReceipt( StateStore::GetInstance()->GetReceiptState() == RETURN ? 1 : 0);
    ins_tr->CreateDateTimeString();
    
    //-----(*)-----Zuskin-----10/10/2011-----
    
    if ( StateStore::GetInstance()->GetReceiptState() == RETURN ) 
	//-----(*)-----Zuskin-----18/11/2011-----
	//ins_tr->InsertCheckRegistrationTransactions("1", "", nalsum, credsum, contsum, closed_session_flag, false);
	ins_tr->InsertCheckRegistrationTransactions("1", "", nalsum, credsum, contsum, closed_session_flag, false, string(manager_id.ascii()));
                //-------------------------------------------------
    // else	
    // 	ins_tr->InsertCheckRegistrationTransactions("0", "", nalsum, credsum, contsum, closed_session_flag, false) ;
    if ( StateStore::GetInstance()->GetReceiptState() == SELL )
	//-----(*)-----Zuskin-----18/11/2011-----
    	//ins_tr->InsertCheckRegistrationTransactions("0", "", nalsum, credsum, contsum, closed_session_flag, false) ;
	ins_tr->InsertCheckRegistrationTransactions("0", "", nalsum, credsum, contsum, closed_session_flag, false, string(manager_id.ascii())) ;		//-------------------------------------------------
    // no need to insert transactions for COPY of check	
    //---------------------------------------	
    
    statusTextLabel->setText(DefCodec->toUnicode("отмена чека"));
    
    cancel_flag = true;
	
    QString upstr = HwCodec->fromUnicode(DefCodec->toUnicode("чек отменен"));
    CDSP cdsp(hw);
    cdsp.CustomerDisplay(upstr.ascii() ,"");
    
    pause_work_timer->start(PAUSE_TIME, true);
    pause(2);
    ClearForNewOperation();
    receipt->CloseReceipt();
    StateStore::GetInstance()->SetReceiptState(FREE);
    quantTextLabel->setText("");
    textTextLabel->setText("");
    nalsum = 0;
    credsum = 0;
    contsum = 0;
    
    if ( authorization_request_flag ) SwapToAuthorizedUser();
    //-----(+)-----Zuskin-----30/09/2011-----
    str = "";
    //-------------------------------------------------
    return 0;
}

//###############################--RETURN--SECTION--##########################################//

//======================================================================//
int RegistrationForm::CloseReturnType( int type )
{    
    //puts("-----------------------------------");
    //printf("closereturntype(type = %d)\n", type);
    //printf("ReturnNumberCheck = %d \n", ReturnNumberCheck);
    
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED )  {   Error(74); return -1; }
    if ( StateStore::GetInstance()->GetReceiptState() != RETURN ) {  return -1;  }
    if ( receiptTable->numRows() == 0 ) {    Error(57);  return -1;   }
    
     //-----(+)-----Zuskin-----15/12/2011-----
    // попробуем рассчитать автоматические скидки
    CheckReduction(true, 0, 0, "-1", "", "", false);
    //-------------------------------------------------
    
    double check_sum = receipt->GetResultSum();
    
    ReleaseKeyboard();
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
    paymentForm *pf = new paymentForm;
    
    pf->SetReturnType();
    pf->SetPaymentSum(check_sum);
    
    /*double check_nal_sum = 0;
    
    hw->ECR_ReadCashInDrawer();
    if ( check_sum >= hw->ECR_GetSumm() ) check_nal_sum = hw->ECR_GetSumm();*/
    
    if ( ReturnNumberCheck != -2 ) {
	//printf("copy->GetDateTime() = %s \n", copy->GetDateTime());
	QDateTime check_dt = QDateTime::fromString(copy->GetDateTime(), Qt::ISODate);
	int k = check_dt.daysTo( QDateTime::currentDateTime() );
	// если к <= 1 то делать отмену  иначе возврат
	if ( k < 1 ) pf->SetCreditOperationType(2);
	else pf->SetCreditOperationType(3);
	
/*	printf("copy->GetCheckSum() = %f \n", copy->GetCheckSum());
	printf("copy->SetNalSum() = %f \n", copy->GetNalSum());
	printf("copy->GetCredSum() = %f \n", copy->GetCredSum());*/
	
	double sale_sum = copy->GetCheckSum();
	//printf("sale_sum = %f \n", sale_sum);
	double return_sum = check_sum;
	//printf("return_sum = %f \n", return_sum);
	double sale_cred_sum = copy->GetCredSum();
	//printf("sale_cred_sum = %f \n", sale_cred_sum);
	
	//-----(+)-----Zuskin-----25/07/2011-----
	double sale_cont_sum = copy->GetContSum();
	//-------------------------------------------------
	//-----(*)-----Zuskin-----25/07/2011-----
	//double sale_nal_sum = sale_sum - sale_cred_sum;
	double sale_nal_sum = sale_sum - sale_cred_sum - sale_cont_sum;
	//-------------------------------------------------
	//printf("sale_nal_sum = %f \n", sale_nal_sum);
	
	hw->ECR_ReadCashInDrawer();
	double cash_sum = hw->ECR_GetSumm();
	
	printf("cash_sum in drawer = %f \n", cash_sum);
	
	if ( sp.sections.size() > 0 ) 
	    cash_sum += sp.getNotFiscalCash();	
	
	printf("cash_sum = %f \n", cash_sum);
	
	pf->SetMaxCredSum(sale_cred_sum);
	if (sale_cred_sum > return_sum) pf->SetCredPaymentSum(return_sum);
	else pf->SetCredPaymentSum(sale_cred_sum);
	
        //-----(+)-----Zuskin-----25/07/2011-----
	pf->SetMaxContSum(sale_cont_sum);
	if (sale_cont_sum > return_sum) pf->SetContPaymentSum(return_sum);
	else pf->SetContPaymentSum(sale_cont_sum);
	//-------------------------------------------------

	//printf("sale_sum = %f \n", sale_sum);
	//printf("cash_sum = %f \n", check_sum);
	
	if ( sale_nal_sum > cash_sum ) pf->SetMaxNalSum(cash_sum);
	else pf->SetMaxNalSum(sale_nal_sum);
	
	
	/*printf("pf->GetMaxNalSum() = %f \n", pf->GetMaxNalSum());
	
	printf("pf->GetCredSum() = %f \n", pf->GetCredSum());
	printf("return sum  = %f \n", return_sum);
	printf("check_sum = %f \n", check_sum);

	
	printf("return_sum - pf->GetCredSum() = %f \n", return_sum - pf->GetCredSum());*/
	
	//-----(*)-----Zuskin-----25/07/2011-----
	//if ( Round((return_sum - pf->GetCredSum())*100) > Round(pf->GetMaxNalSum()*100) ){
	if ( Round((return_sum - pf->GetCredSum() - pf->GetContSum())*100) > Round(pf->GetMaxNalSum()*100) ){
	//--------------------------------------------------    
	    Error(50);
	    return -1;
	} else {
	    //-----(*)-----Zuskin-----25/07/2011-----
	    //pf->SetNalPaymentSum( return_sum - pf->GetCredSum() );
	    pf->SetNalPaymentSum( return_sum - pf->GetCredSum() - pf->GetContSum());
	    //-------------------------------------------------
	}
	pf->SetReturnAdditSumm(copy->GetCheckSum());
	
    } else {
	
	hw->ECR_ReadCashInDrawer();
	double cash_sum = hw->ECR_GetSumm();
	printf("cash_sum in drawer = %f \n", cash_sum);
	
	if ( sp.sections.size() > 0 ) 
	    cash_sum += sp.getNotFiscalCash();
	
	printf("cash_sum result = %f \n", cash_sum);
	
	if ( cash_sum < check_sum ) pf->SetMaxNalSum(cash_sum);
	else pf->SetMaxNalSum(check_sum);
	pf->SetMaxCredSum(check_sum);
	//-----(+)-----Zuskin-----25/07/2011-----
	pf->SetMaxContSum(check_sum);
	//-------------------------------------------------
	if (type == 1 || type == 3) {
	    pf->SetNalPaymentSum(pf->GetMaxNalSum());   
	    //pf->SetCredPaymentSum(check_sum - pf->GetMaxNalSum());
	}
	if ( type == 2 ) {
	    pf->SetCredPaymentSum(check_sum);
	    pf->SetNalPaymentSum(0);   
	}
	if (type == 0) {
	    pf->SetNalPaymentSum(0);   
	    pf->SetCredPaymentSum(0);
	}	
	
	pf->SetCreditOperationType(3);
	
    }
    
    // задать параметры для возврата кредитом
    pf->SetValidatorProperties();
    pf->SetCheckNum( ReturnNumberCheck );
    
    // получить информацию о чеке
    /*printf("Check sum  = %f \n", copy->GetCheckSum() );	// сумма чека
    printf("Payment of nal = %f \n", copy->GetNalSum() );	// то что дал клиент
    printf("Payment of cred = %f \n", copy->GetCredSum());	//  было заплачено кредитом
    printf("Payment of cont = %f \n", copy->GetContSum());   // было оплачено тарой
    printf("Payment ClientSum = %f \n", copy->GetClientSum());	//  то что дал клиент + было заплачено кредитом + было оплачено тарой
    printf("Payment of nal sum = %f \n", check_nal_sum);	//    сумма чека, оплаченная наличными
    printf("check date time = %s \n", copy->GetDateTime());
    */
    
    pf->SetReadyFlag(true);
    if ( pf->exec() == QDialog::Rejected ) {
		  keyboard_block = false;
		  return -1;
    }
	 
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();
    
    /******получить результат ********/
    int ret = 0;
    double nal = 0, cred = 0, cont = 0;
    int * indexes = NULL, num_rights = 0;
    
    pf->GetSumParameters(nal, cred, cont, check_sum);
    if ( hw->ECR_GetProtocol() == 6 ) {
	int count_payment_type = 0;
	if (nal > 0) count_payment_type++;
	if (cred > 0) count_payment_type++;
	if (cont > 0) count_payment_type++;
	if ( count_payment_type > 1 ) {
	    Error(109);
	    return -1;
	}
    }
    //printf("nal = %f \ncred = %f \ncont = %f \ncheck_sum = %f \n", nal, cred, cont, check_sum);
    
    // проверка на достаточность налисночти в секциях
    if ( ! testSectionCashEnough() ) {  Error(111); return -1;  }
    
    nalsum = nal;
    credsum = cred;
    //-----(+)-----Zuskin-----25/07/2011-----
    contsum = cont;
    //-------------------------------------------------

    pf->GetRightsParameters(indexes, num_rights);    
    if ( nal != -1 )  {
	if ( (num_rights != 0) && !AuthorizationRequestForAlliedRights(indexes, num_rights) )    {
	    Error(72);
	    ret = -1;
	}
	
	if ( authorization_request_flag) SwapToAuthorizedUser();
    }
    else ret = -1;
    
    if ( indexes != NULL ) free(indexes);
    
    int crst = pf->GetCreditStatus();
    
    delete(pf);
    
    kkm_reg->SetCreditStatus(crst);
    return_flag = true;
    
    return ret; 
    
}
//======================================================================//
bool RegistrationForm::testSectionCashEnough() {
    int section_num_with_max_sum = receipt->GetSectionNumWithMaxSum();
    
    for ( int i = 0; i < sp.sections.size(); i++ ) {
	if ( receipt->GetSectionResultSum(i) > sp.sections[i].cash )
	    return false;
    }
    
    return true;
}

//======================================================================//
void RegistrationForm::clearCopyInfo() {
	 
    receipt->setDate("");
    copy->Clear_CopyInfo();
    
    QString upstr = HwCodec->fromUnicode(DefCodec->toUnicode("возврат"));
    QString downstr = HwCodec->fromUnicode(DefCodec->toUnicode("ИТОГО: "));
    downstr += allTextLabel->text();
    
    //printf("    CDSP cdsp(hw);");
    //printf("upstr.ascii() - %s, downstr.ascii() - %s \n", upstr.ascii(), downstr.ascii());
    CDSP cdsp(hw);
    cdsp.CustomerDisplay(upstr.ascii(), downstr.ascii());
    
    statusTextLabel->setText(DefCodec->toUnicode("возврат"));
    statusTextLabel->repaint();
    
    statusTextLabel->setText(DefCodec->toUnicode(""));
    
    //printf("hw->ECR_GetCheckNumber = %f \n", hw->ECR_GetCheckNumber());
    
    if (ReturnNumberCheck == -2) ReturnNumberCheck = hw->ECR_GetCheckNumber();
    
}
//======================================================================//

int RegistrationForm::Return() 
{
    //puts("RegistrationForm::Return()");
    
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED )  {   Error(74); return -1; }
    if ( receiptTable->numRows() == 0 ) {    Error(57);  return -1;   }
    
    double sum = 0;
    bool nf;
    sum = allTextLabel->text().toDouble(&nf);
    
    hw->ECR_ReadCashInDrawer();
    if ( hw->ECR_GetSumm() < sum )  {  Error(50);  return -1;   }
    if ( quantity_flag )  {   Error(41);  return -1;   }
    
    QString upstr = HwCodec->fromUnicode(DefCodec->toUnicode("возврат"));
    QString downstr = HwCodec->fromUnicode(DefCodec->toUnicode("ИТОГО: "));
    downstr += allTextLabel->text();
    CDSP cdsp(hw);
    cdsp.CustomerDisplay(upstr.ascii(), downstr.ascii());
    
    statusTextLabel->setText(DefCodec->toUnicode("возврат"));
    statusTextLabel->repaint();
    
    return_flag = true;
    
    statusTextLabel->setText(DefCodec->toUnicode(""));
    
    return 0;
}

//======================================================================//
void RegistrationForm::ReturnCheck() /*CTRL + DELETE*/ //Возврат
{
    //puts("RegistrationForm::ReturnCheck() ");
    if ( StateStore::GetInstance()->GetViewState() == FILL )  {  ClearForNewOperation(); }
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED )  {  Error(74); return;  }
    if ( StateStore::GetInstance()->GetReceiptState() != FREE )   { Error(24);  return; }
    
    if ( sp.control_back == NO ) {  Error(94); return;  }
    
    // Определение прав пользователя на возвраот
    bool right_flag = false;
    if  (  (str != "") && !GetRight(13)  )  right_flag = AuthorizationRequest(13);
    else if ( (str == "") && !GetRight(14) )  right_flag = AuthorizationRequest(14);
    else right_flag = true;
    
    if ( !right_flag ) { Error(72); return; }   
    
    kkm_res = hw->ECR_ReadCashInDrawer();
    if ( kkm_res !=  0 )    Kkm_Error(kkm_res); 
    
    //if ( hw->ECR_GetSumm() <= 0 )      { Error(49);  return; }
    
    //if ( hw->ECR_GetProtocol() == 4 && hw->ECR_GetMode() == 4 )  { Error(49);  return; }
    
    
    // строка ввода номера чека
    if ( str != "" )     {	
	bool nf;
	// заданный номер
	int number = str.toInt(&nf);
	// проверка на правильность
	if ( !nf ) { Error(23);  return;  }
	
	int res = CheckList(number, false, false);	// выбор чека
	if ( res != 0 )  {
	    if ( res == -1 ) Error(88);
	    else if ( res == 1 ) Error(89);
	    else if ( res == -15 ) return;
	    else Error(res);
	    return;
	}
	
    } else ReturnNumberCheck = -2;
    
    // открыть чек возврата 
    StateStore::GetInstance()->SetReceiptState(RETURN);
    
    int check_number = 0;
    int r;
    if (hw->ECR_GetProtocol() == 4) {
	hw->ECR_ReadDocumentNumber(2);
	check_number =  hw->ECR_GetDocumentNumber();
    }    else {
	hw->ECR_ReadCheckNumber();
	check_number = hw->ECR_GetCheckNumber();
    }
    
    /*hw->ECR_ReadCheckNumber();
		  int check_number = hw->ECR_GetCheckNumber();*/
    //printf("check_number  = %d\n",check_number);
    if ( check_number  == -1 ) {  Error(19);  return;  }
    QString ss;
    QString s = "открыт чек возврата №"+ss.setNum(check_number);
    modeTextLabel->setText(DefCodec->toUnicode(s));
    statusTextLabel->setText(DefCodec->toUnicode("чек возврата"));
    receipt->SetCheckType(CHECK_RETURN);
    receipt->SetCheckNumber(check_number);
    
    if ( receipt->GetDiscountElemRef(1)->df || receipt->GetDiscountElemRef(2)->df || receipt->GetDiscountElemRef(3)->df )	    	StateStore::GetInstance()->SetViewState(REC);
    //if(authorization_request_flag) SwapToAuthorizedUser();
    //  str = "";
    
}

//======================================================================//
//number - номер чека введенный с клавиатуры пользователем
//copy_fl = true, если будет осуществлються печать копии чека.
int RegistrationForm::CheckList(int number, bool copy_fl, bool putout_fl) 
{
    deliveryTextLabel->setText("");
    // puts("RegistrationForm::CheckList(int number, bool copy_fl)");
    int er = 0;
    ReleaseKeyboard(); 
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
    // заполним параметры формы
    checklistForm *cf = new checklistForm;
    int n_pc = Read_N_PC();
    //-----(*)-----Zuskin-----24/10/2011-----
    //cf->SetVariables(number, n_pc);
    cf->SetVariables(number, n_pc, putout_fl);
    //-------------------------------------------------
    int k = cf->FillForm();
    
    // проверка на успешное заполнение
    if ( k != 0 )  {
	delete(cf);
	StateStore::GetInstance()->SetReceiptState(FREE);
	modeTextLabel->setText(DefCodec->toUnicode("готов к работе"));
	HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
	GrabKeyboard();
	return 27;
    } 	 
    
  //-----(+)-----Zuskin-----10/10/2011-----
    if (copy_fl) {
	cf->setCaption(DefCodec->toUnicode("Выберите чек для печати копии"));
    }
    else {
	//-----(+)-----Zuskin-----24/10/2011-----	
	//cf->setCaption(DefCodec->toUnicode("Выберите чек для возврата"));
	if (putout_fl) {
	    cf->setCaption(DefCodec->toUnicode("Выберите ранее отложенный чек"));  
	}
	else {
	    cf->setCaption(DefCodec->toUnicode("Выберите чек для возврата"));
                }
	//---------------------------------------------------
    }
    //-------------------------------------------------
    k = cf->exec();
    
    if ( !cf->fixflag || k == QDialog::Rejected ) {
	delete(cf);  
	str = "";
	StateStore::GetInstance()->SetReceiptState(FREE);
	modeTextLabel->setText(DefCodec->toUnicode("готов к работе"));
	
	HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
	GrabKeyboard();	
	//str = "";
	return -15;
    }	
    
    str = "";
    textTextLabel->setText("");
    
    if (putout_fl) { //восстановим отложенный ранее чек
	string npc = cf->GetNPC();
	string nch = cf->GetCheckNum();
	string ndt = cf->GetDateTime();
	string ndn = cf->GetNDoc();
	string nct = cf->GetChType();
	bool ret_fl = false;
	if (nct == "1")  {
	    ret_fl = true;
	}
	//-----(*)-----Zuskin-----13/01/2012-----
	int managerid = 0;
	//er = Tr.Get_PutOut_Check_Info( npc, nch, ndn, ndt, receipt, copy, ret_fl );
	er = Tr.Get_PutOut_Check_Info( npc, nch, ndn, ndt, receipt, copy, managerid, ret_fl );
	//--------------------------------------------------
	if (er == 0) {
	    Tr.Mark_PutOut_Check(npc, nch, ndt);
	    if (nct == "0") { // продажа
		StateStore::GetInstance()->SetReceiptState(SELL);    
	    }
	    else { // возврат
		StateStore::GetInstance()->SetReceiptState(RETURN);      
	    }
	    //-----(+)-----Zuskin-----13/01/2012-----
	    if (managerid != 0) {
		vector <string> manid;
		vector <string> manname;
		int kk = Reg.Query_Managers_List(&manid, &manname);
		if (kk > 0) {
		    for (kk = 0; kk < manid.size(); kk++) {
			if ( atoi(manid[kk].c_str()) == managerid ) {
			    manager_id = QString::number(managerid);
			    manager_name = QString(manname[kk]);
			    SetManagerName();
			    break;
			}
		    }
		}
	    }
	    //--------------------------------------------------
	}
	else {
	    emit(AddWareError(er));
	}
    }
    else {
	char *check = (char *) calloc(5, sizeof(char));
	char *datetime = (char *) calloc(20, sizeof(char));
	char *n_kkm = (char *) calloc(5, sizeof(char));
	char* n_doc = (char * ) calloc(5, sizeof(char));
	strcpy(n_kkm, QString::number(n_pc));
    
    // забрать данные с формы
	cf->GetCheckInfo(check, datetime, n_doc);
    // заполнить объекты copy и receipt, данными из транзакций, в зависимости от быбранных номера чека,номера ккм, даты и тд
    //копия заполняется тут
	//-----(*)-----Zuskin-----13/01/2012-----
	//er = Tr.Get_Registration_Check_Info(check, n_kkm, datetime, n_doc, receipt, copy, copy_fl);
	int managerid = 0;
	er = Tr.Get_Registration_Check_Info(check, n_kkm, datetime, n_doc, receipt, copy, copy_fl, managerid);
	//-----(+)-----Zuskin-----13/01/2012-----
	if (managerid != 0) {
	    vector <string> manid;
	    vector <string> manname;
	    int kk = Reg.Query_Managers_List(&manid, &manname);
	    if (kk > 0) {
		for (kk = 0; kk < manid.size(); kk++) {
		    if ( atoi(manid[kk].c_str()) == managerid ) {
			manager_id = QString::number(managerid);
			manager_name = QString(manname[kk]);
			SetManagerName();
			break;
		    }
		}
	    }
	}
	//--------------------------------------------------
	//--------------------------------------------------
	
	free(datetime);
	free(check);
	free(n_kkm);
	free(n_doc);
    }
    
    receipt->setDate(copy->GetDateTime());
    
    if ( er != 0 )       {
	receipt->CloseReceipt();
	delete(cf);
	StateStore::GetInstance()->SetReceiptState(FREE);
	modeTextLabel->setText(DefCodec->toUnicode("готов к работе"));
	
    }      else      {
	for ( int i = 0; i < receipt->record_num; i++)	  AddRowInTable(i);
	PositionPreference(receipt->record_num - 1); 
	
	QString s;
	s.setNum(receipt->GetResultSum(), 'f', 2);
	allTextLabel->setText(s);
	delete(cf);
	//-----(*)-----Zuskin-----27/10/2011-----
	//ReturnNumberCheck = number;
	 if (putout_fl) { ReturnNumberCheck = -2;
	 }
	 else {
	     ReturnNumberCheck = number;
	 }
	 //-------------------------------------------------
    }
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();

    return er;
}

//###################################--END--RETURN--SECTION--#################################//

//###########################################--ADDWARE--SECTION--####################################//
//======================================================================//
void RegistrationForm::SetQuantity()
{
    //puts("void RegistrationForm::SetQuantity()");
    bool nf;
    double q = str.toDouble(&nf);
    
    if ( !nf ) {   Error(55);  return;  }
    
    AddWareController::GetInstance()->SetQuantity(q, 0);
}

//======================================================================//
void RegistrationForm::ShowQuantitySet(double q, int parental)
	// parental = 1 - с весов
	// parental = 0 - с клавиатуры
	// parental = -1 - с визуального поиска после ошибки надо очистить поле
{
    
    printf("void RegistrationForm::ShowQuantitySet(double q = %f, int parental = %d)\n", q, parental);
    
    if ( StateStore::GetInstance()->GetViewState() == FILL )	ClearForNewOperation();
    
    if ( parental == -1 && q == 1) {  quantTextLabel->setText("");  return;   }
    
    //-----(*)-----Zuskin-----05/10/2011-----
    //str.setNum(q, 'f', 3);
    //quantTextLabel->setText(str);
    QString tstr;
    tstr.setNum(q, 'f', 3);
    quantTextLabel->setText(tstr);
    //---------------------------------------
    
    if ( parental == 1 )  statusTextLabel -> setText(DefCodec->toUnicode("вес"));
    else  statusTextLabel -> setText(DefCodec->toUnicode("ввод количества"));
    
    if ( q != 1 )    quantity_flag = true;
    else quantity_flag = false;
    
    //-----(*)-----Zuskin-----05/10/2011-----
    //str = "";
    if ( parental != 1) {
	str = "";
    }
    //---------------------------------------
}


//======================================================================//
void RegistrationForm::ShowAddedWare(int pos, bool code_type, bool newsellreceipt)
{  
    deliveryTextLabel->setText("");
    if(StateStore::GetInstance()->GetViewState() == MESS)  {
	// Очищение поля ввода для сканера ш-к
	//puts("-------1891--------");
	textTextLabel->setText("");	 
    }
    else if(StateStore::GetInstance()->GetViewState() == FILL)	ClearForNewOperation();
    
    //hw->ECR_ReadStatus();
    //if ( hw->ECR_GetCheckState() == 1 ) hw->ECR_CancelCheck();
    
    if(newsellreceipt){
	int check_number = 0;
	if (hw->ECR_GetProtocol() == 4) {
	    hw->ECR_ReadDocumentNumber(0);
	    check_number =  hw->ECR_GetDocumentNumber();
	}
	else{
	    hw->ECR_ReadCheckNumber();
	    check_number = hw->ECR_GetCheckNumber();
	}
	
	//hw->ECR_ReadCheckNumber();
	//check_number = hw->ECR_GetCheckNumber();
	
	if (check_number == -1) { Error(19); return; }
	QString ss;
	QString s = "открыт чек продажи № "+ss.setNum(check_number);
	//printf("check_number  = %d\n",check_number);
	modeTextLabel->setText(DefCodec->toUnicode(s));
	//printf("CHECK_number = %d \n", check_number);
	//printf("CHECK_SELL = %d \n", CHECK_SELL);
	receipt->SetCheckType(CHECK_SELL);
	receipt->SetCheckNumber(check_number);
    }
    
    if(code_type)    statusTextLabel->setText(DefCodec->toUnicode("штрих-код"));
    else    statusTextLabel->setText(DefCodec->toUnicode("внутренний код"));
    
    AddRowInTable(pos);
    PositionPreference(pos);
    
    quantTextLabel->setText("");
    quantity_flag = false;
    
    if(sp.show_properties_in_input)  {
	str = DbCodec->toUnicode(receipt->GetPosElemRef(pos)->GetLongtitle());
	StateStore::GetInstance()->SetViewState(MESS);
    }  else      str = "";
    adjustFont();
    textTextLabel->setText(str);
    
    if ( authorization_request_flag ) SwapToAuthorizedUser();
    
}

//======================================================================//
void RegistrationForm::AddWareError(int er)
{
    printf("RegistrationForm::AddWareError(int er = %d)\n", er);
    
    if ( er == 5 )  {
	quantTextLabel -> setText ("");
	AddWareController::GetInstance()->EscapeQuantity();
	quantity_flag = false;
    }
    
    if ( !sp.error_output_window || notaddbyvisual )    Error(er);
    if ( authorization_request_flag ) SwapToAuthorizedUser();
}

//=====================================================================//
void RegistrationForm::PositionCustomerDisplay()
{
    
    if ( sp.cdsp_detalize_product )    {
	QTableSelection sel = receiptTable -> selection(0);
	int p_sel = sel.topRow();
	
	QString prod_name =HwCodec->fromUnicode(receiptTable->text(p_sel, 0));
	
	CDSP cdsp(prod_name.ascii(), receiptTable->text(p_sel, 1).ascii(), receiptTable->text(p_sel, 2).ascii(), receiptTable->text(p_sel, 3).ascii(), allTextLabel->text().ascii(), hw);
	cdsp.RegistrationPositionCustomerDisplay();
    }
}

//#########################################--END--ADDWARE--SECTION--#################################//

//======================================================================//
void RegistrationForm::IntermediateResult()
{
    puts("RegistrationForm::IntermediateResult()");
    
    if ( StateStore::GetInstance()->GetViewState() == FILL )    ClearForNewOperation();
    
    if ( StateStore::GetInstance()->GetReceiptState() == FREE )  {  Error(18); return;   }
    if ( StateStore::GetInstance()->GetReceiptState() == COPY ) {   Error(32);   return;   }
    
    //-----(+)-----Zuskin-----15/12/2011-----
    // попробуем рассчитать автоматические скидки
    CheckReduction(true, 0, 0, "-1", "", "", false);
    //-------------------------------------------------
        
    QString all_s;
    all_s.setNum(receipt->GetResultSum(), 'f', 2);
    
    str = all_s;
    adjustFont();
    textTextLabel->setText(str); 
    
    //    str = textTextLabel->text();
    
    QString upstr = HwCodec->fromUnicode(DefCodec->toUnicode("промежуточный итог"));
    QString downstr = textTextLabel->text();
    CDSP cdsp(hw);
    cdsp.CustomerDisplay(upstr.ascii(), downstr.ascii());
    
    //-----(+)-----Zuskin-----15/12/2011-----
    //printf("StateStore::GetInstance()->GetViewState() = %d\n", StateStore::GetInstance()->GetViewState());
    //printf("StateStore::GetInstance()->GetPrevState() = %d\n", StateStore::GetInstance()->GetPrevState());
    
    if ((StateStore::GetInstance()->GetViewState() == MESS) || (StateStore::GetInstance()->GetViewState() == INS)) {
	StateStore::GetInstance()->SetViewState(StateStore::GetInstance()->GetPrevState());	
    }
    //-------------------------------------------------
        
    StateStore::GetInstance()->SetViewState(INS_TEXT);	
         
    statusTextLabel->setText(DefCodec->toUnicode("ПРОМИТОГ"));
    
}

//======================================================================//
void RegistrationForm::SumInBox() 	/*CTRL + HOME*/ //Сумма  в ящике
{
    //  puts("RegistrationForm::SumInBox()");
    //-----(+)-----Zuskin-----2011-05-13-----
    if ( !GetRight(23)  && !AuthorizationRequest(23) )    {
	Error(72);
	DateTimeControl();
	return;
    }
    //---------------------------------------    
    statusTextLabel->setText(DefCodec->toUnicode("сумма в ящике"));
    
    
//-----(*)-----Zuskin-----07/06/2011-----    
    kkm_res = hw->ECR_ReadCashInDrawer();
    if ( kkm_res != 0 )
	Kkm_Error(kkm_res);
    if ( sp.sections.size() == 0 ) {
//	kkm_res = hw->ECR_ReadCashInDrawer();
//	if ( kkm_res != 0 )
//	    Kkm_Error(kkm_res);
//	else     {
	    str.setNum(hw->ECR_GetSumm(),'f', 2);
	    textTextLabel -> setText(str);
//	}
	return;
    }
    
    sp.deductCashSumInSection(0, sp.getCashSumInSection(0));
    sp.addCashSumInSection(0, hw->ECR_GetSumm());
//--------------------------------------------------    
	    
    ReleaseKeyboard();
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
    CashInSectionsForm f;
    if (f.exec() == QDialog::Accepted) {
	double sum_of_select_section = f.GetResultSum();
	str.setNum(sum_of_select_section,'f', 2);
	textTextLabel->setText(str);
    }
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();   
    
}

//======================================================================//
void RegistrationForm::AdvancePaper() /*PAGE UP*/ //Продвинуть бумагу
{
    //  puts("RegistrationForm::AdvancePaper()");
    grabKeyboard();
    //if ( hw->ECR_GetProtocol() != 4 )   hw->ECR_PrintString("  ", false);
    hw->ECR_AdvancePaper();
    
}

//=====================================================================//
// rflag = true - ящик открывается при закрытии чека, внесении, выплате....
void RegistrationForm::OpenBox(bool rflag) /*PAGE DOWN*/ //Открыть ящик
{
    //  puts("RegistrationForm::OpenBox(bool rflag)");
    //printf("GetRight(25) = %d\n", GetRight(25));
    // if(GetRight(25)) puts("TRUE");
    //  else puts("FALSE");
    puts("---------OPEN BOX-------");
    
    bool right = rflag || GetRight(25);
    
    if ( !right )  right = AuthorizationRequest(25);
    
    if ( right )  {
	if  ( (kkm_res = hw->ECR_OpenDrawer()) != 0 )       Kkm_Error(kkm_res);
	if ( authorization_request_flag ) SwapToAuthorizedUser();
    }  else Error(72); //Нет прав для открытия ящика в произвольный момент.
    
}

//======================================================================//
void RegistrationForm::Entering() /*F10*/ //Внесение денег в кассу
{
    if ( StateStore::GetInstance()->GetViewState() == FILL )    ClearForNewOperation();
    
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED ) {   Error(74);   return;   }
    if ( StateStore::GetInstance()->GetReceiptState() != FREE )  {    Error(26);   return;   }
    
    if ( !GetRight(22) &&  !AuthorizationRequest(22) )   { Error(72);    DateTimeControl();   return;    }
    
    statusTextLabel->setText(DefCodec->toUnicode("внесение в кассу"));
    
    //printf("textTextLabel->text() = %s \n", textTextLabel->text().ascii());
    
    double sum = textTextLabel->text().toDouble();
    sum = Round(sum * 100) / 100;
    
    TryGettingEntering(sum, 1);
    
    str = "";
    if ( authorization_request_flag ) SwapToAuthorizedUser();
    DateTimeControl();
}

//======================================================================//
void RegistrationForm::Getting()/*F11*/ //Выплата денег из кассы
{
    
    if ( StateStore::GetInstance()->GetViewState() == FILL )    ClearForNewOperation();
    
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED )  {  Error(74);   return;     }
    if ( StateStore::GetInstance()->GetReceiptState() != FREE )   {    Error(26);    return;   }
    
    if ( !GetRight(23)  && !AuthorizationRequest(23) )    {
	Error(72);
	DateTimeControl();
	return;
    }
    
    statusTextLabel->setText(DefCodec->toUnicode("выплата из кассы"));

    double sum = textTextLabel->text().toDouble();
    sum = Round(sum * 100) / 100;
    
    TryGettingEntering(sum, 0);
    
    str = "";     
    if ( authorization_request_flag ) SwapToAuthorizedUser();
    DateTimeControl(); 
}
//======================================================================//
// oper_type = 0 - getting 1 - entering
int RegistrationForm::TryGettingEntering(double sum, int oper_type) {
    
    puts("int RegistrationForm::TryGettingEntering(double sum, int oper_type) {");
        
    if ( (sum < 0.01) || (sum > 99999999.99) ) {	Error(25);	    return -1;    }    

    int section_num = -1;
    
    if ( sp.sections.size() == 0 ) {
	int kkm_res = kkm_reg->KkmPrintSectionGettingEntering(sum, oper_type, Authorization::GetInstance()->GetUserShortcut(), -1);
	if ( kkm_res != 0 ) {   Kkm_Error(kkm_res);  return -1;    } 	
    } else {
	    ReleaseKeyboard();
	    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
	    
	    CashInSectionsForm f;
	    QString operation_text;
	    if (oper_type == 0) operation_text = "Выберите секцию для выплаты \nСумма выплаты   -   ";
	    else operation_text = operation_text = "Выберите секцию для внесения \nСумма внесения   -   ";
	    operation_text += QString::number(sum, 'f', 2);
	    f.setOperationText(operation_text);
	    int exec_result = f.exec();
	    
	    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
	    GrabKeyboard();   
	    
	    if ( exec_result == QDialog::Accepted) {
		section_num = f.GetSelectedSection();
		
		if ( oper_type == 0 && (sp.sections[section_num].cash < sum ) )  {  Error(110);  return -1;   }
		
		int kkm_res = kkm_reg->KkmPrintSectionGettingEntering(sum, oper_type, Authorization::GetInstance()->GetUserShortcut(), section_num);
		printf("res of KkmPrintSectionGettingEntering - %d \n", kkm_res);
		if ( kkm_res != 0 ) {   Kkm_Error(kkm_res);  return -1;    } 
	    } else {
		return -1;
	    }
    }    
    
    if ( sp.open_box_by_change_money )            OpenBox(true); 
    
    ins_tr->CreateDateTimeString(); 
    // 2 - getting 1 - entering
    int er = 0;
    if ( oper_type == 0 )
    	er = ins_tr->InsertEnteringGettingTransaction(2, textTextLabel->text().ascii(), closed_session_flag, section_num);
    else 
	er = ins_tr->InsertEnteringGettingTransaction(1, textTextLabel->text().ascii(), closed_session_flag, section_num);
    if ( er != 1 )  {   Error(21);   return -1;    }       
    
    return 0;    
    
}

//###########################################--REDUCTION--SECTION--####################################//

//=====================================================================//
void RegistrationForm::ReductionCustomerDisplay(int j, int i)
{
    //puts("RegistrationForm::PositionCustomerDisplay(int j, int i)");
    if(sp.cdsp_detalize_discount)
    {
	QString red_label, red_sum;
	
	Discount * ptr = NULL;
	if(i == -1)
	    ptr = receipt->GetDiscountElemRef(j);
	else
	    ptr = receipt->GetPosElemRef(i)->GetDiscountElemRef(j);
	if(ptr->dsum > 0)
	{
	    red_label = HwCodec->fromUnicode(DefCodec->toUnicode("скидка"));//DbCodec->fromUnicode(DefCodec->toUnicode("Скидка"));
	    red_sum.setNum(ptr->dsum, 'f', 2);
	}
	else
	{
	    red_label = HwCodec->fromUnicode(DefCodec->toUnicode("надбавка"));//DbCodec->fromUnicode(DefCodec->toUnicode("Надбавка"));
	    red_sum.setNum(- ptr->dsum, 'f', 2);
	}
	QString str_kakawka = HwCodec->fromUnicode(DbCodec->toUnicode(ptr->GetShortcut()));
	char* kakawka = (char*) calloc(str_kakawka.length()+1, sizeof(char)); 
	strcpy(kakawka, str_kakawka);
	
	CDSP cdsp(kakawka, red_sum.ascii(), red_label.ascii(), hw);
	
	free(kakawka);
	cdsp.RegistrationReductionCustomerDisplay();
    }
}

//======================================================================//
int RegistrationForm::PositionReduction(bool type_fl, int i, double v, const char * code, const char *name, const char *shortcut, bool manual_fl)
	// type_fl = true -- процентная, false -- суммовая
	//
{
    // printf("RegistrationForm::PositionReduction(bool type_fl, int i, double v, const char * code = <%s>, const char *name = <%s>, const char *shortcut = <%s>, bool manual_fl)\n", code, name, shortcut);
    
    QTableSelection sel = receiptTable -> selection(0);
    int p_sel = sel.topRow(); 
    //printf("!!!!!!!\nname = <%s>\n!!!!!!!!\n", name);
    int res;
    if ( type_fl )     res = receipt->GetPosElemRef(p_sel)->Add_Percent_Discount(i, v, code, name, shortcut, manual_fl, sp.control_min_price);
    else     res = receipt->GetPosElemRef(p_sel)->Add_Sum_Discount(i, v, code, name, shortcut, manual_fl, sp.control_min_price);
    
    if ( res == 1 ) {   Error(83);  return -1;}
    if ( res == 2 )  {  Error(85);  return -1;  }
    if ( res == 3 )  {  Error(46);  return -1;  }
    if ( res == 4 &&  ( StateStore::GetInstance()->GetReceiptState() != RETURN) )   {   Error(82);  return -1;   }

	PositionPreference(p_sel);
	ReductionCustomerDisplay(i, p_sel);
	switch ( i )     {
	case 1 :
	    statusTextLabel->setText(DefCodec->toUnicode("начисление фикс. скидки на позицию"));
	    break;
	case 2 :
	    statusTextLabel->setText(DefCodec->toUnicode("начисление произв. скидки на позицию"));
	    break;
	}
	str = "";
	return 0;
}

//======================================================================//
int RegistrationForm::CheckReduction(bool type_fl, int i, double v, const char * code, const char *name, const char *shortcut, bool manual_fl)
	// type_fl = true -- процентная, false -- суммовая
{
    //-----(+)-----Zuskin-----13/07/2011-----
    if ( strlen(code) > 0 ) {
        //-----(*)-----Zuskin-----15/12/2011-----
	double CurrentSumm = 0;
	int scheme_id = -1;
	string cc = code;
	if ( cc == "-1" ) { // автоматическая скидка 
	    scheme_id = Reg.GetDefaultSchemeId();
	    if ( scheme_id == -1 ) return 0;
	}
	else { // скидка по карте - может быть схема условий
	    scheme_id = Reg.GetSchemeIdByCode(code);
	    CurrentSumm = Reg.GetSummByCard(code);
	}
	    if (CurrentSumm < 0) {
		CurrentSumm = 0;
	    }
//      printf("============================scheme_id = [%d]=========================\n", scheme_id);    
        if ( scheme_id > 0 ) { // надо проверить все условия схемы скидок с id = scheme_id
    	    int tmp_cnt = Reg.GetListBySchemeId(scheme_id);
    	    if (tmp_cnt > 0) {
//    		printf("+++++++++++++ there are [%d] rules in scheme +++++++++++++++++++\n", tmp_cnt);
    		for (int ti = 0; ti<tmp_cnt; ti++) {
    		    string value, type, title, text, bdate, edate, btime, etime, weekd, sum, accum;
    		    Reg.GetRuleFromList(ti, value, type, title, text, bdate, edate, btime, etime, weekd, sum, accum);
//    		    printf("%s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s\n", value.c_str(), type.c_str(), title.c_str(), text.c_str(), bdate.c_str(), edate.c_str(), btime.c_str(), etime.c_str(), weekd.c_str(), sum.c_str(), accum.c_str());
    		    
    		    bool rule_ok = true; 
    		    QDate CurDate = QDate::currentDate();
    		    if (bdate.length() > 0) { // проверяем по дате начала
    			QDate BDate = QDate::fromString(bdate.c_str(), Qt::ISODate);
    			if ( CurDate  < BDate ) {
    			    rule_ok = false;
    			}
    		    }
    		    if ((edate.length() > 0) && (rule_ok)) { // проверяем по дате окончания
    			QDate EDate = QDate::fromString(edate.c_str(), Qt::ISODate);
    			if ( CurDate  > EDate ) {
    			    rule_ok = false;
    			}
    		    }
    		    if ((btime.length() > 0) && (etime.length() > 0) && (rule_ok)) { // проверяем по времени начала/окончания действия
    			QTime BTime = QTime::fromString(btime.c_str(), Qt::ISODate);
    			QTime ETime = QTime::fromString(etime.c_str(), Qt::ISODate);
    			QTime CurTime = QTime::currentTime();
    			if ( BTime < ETime ) {
    			    if ( (CurTime < BTime) || (CurTime > ETime) ) {
    				rule_ok = false;
    			    }    		    
    			}
    			else {
    			    if ( (CurTime < BTime) && (CurTime > ETime) ) { 
    				rule_ok = false;
    			    }
    			}
    		    }
    		    if ((weekd.length() > 0) && (rule_ok)) { // проверяем по дням недели
    			int wday = (char) CurDate.dayOfWeek();
    			char rwday = (char) atoi(weekd.c_str());
    			char dayflg = ( rwday >> (wday-1) )&0x01;
    			if ( dayflg == 0  ) {
    			    rule_ok = false;
    			}  
    		    }
    		    
    		    if ((sum.length() > 0) && (rule_ok)) { // проверяем по сумме чека
    			double esum = atof(sum.c_str());
    		        //-----(*)-----Zuskin-----29/12/2011-----
		        //if ( receipt->GetResultSum() < esum) {
		        //скидки на чек должны рассчитываться от исходной суммы чека (с учетом скидок на позиции)
		        // т.к. иначе как их комбинировать?	
		        if ( receipt->Sum() < esum) {
		        //------------------------------------------------	    
    		    	    rule_ok = false;
    		        }
    		        
    		    }
    		    if ((accum.length() > 0) && (rule_ok)) { // проверяем по сумме накопления
    			double accsum = atof(accum.c_str());
			
    			if ( CurrentSumm < accsum ) {
    			    rule_ok = false;
    			}
    		    }
    		    if (rule_ok) {
        		    v = atof(value.c_str());
        		    if (ti < tmp_cnt - 1) Reg.ClearRulesList();
        		    break;
        	    }	   
    		} 
    	    }   
        }
        //-----(+)-----Zuskin-----21/07/2011-----
	if (i == 4) { // bonuscard !!!!
	    v = 0;
	    ReleaseKeyboard(); 
	    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
	    // заполним параметры формы
	    BonusCardForm *bcf = new BonusCardForm;
	    bcf->valueLabel->setText(toString<double>(Reg.GetPercentByCard(code)).c_str());
	    bcf->cardCodeLabel->setText(DbCodec->toUnicode(shortcut));
	    
	    double BonusSumm = Reg.GetSummByCard(code);
	    if (BonusSumm < 0) {
		BonusSumm = 0;
	    }
	    double maxPercent = Reg.GetMaxPercentByCard(code);
	    
	    double maxBSumm = receipt->GetResultSum();
	    if (maxPercent < 100) {
		maxBSumm = maxBSumm * maxPercent / 100;
	    }
	    if (BonusSumm < maxBSumm) {
		maxBSumm = BonusSumm;
	    }
	    bcf->InitFields(BonusSumm, maxBSumm);
    	        	    
    	    int k = bcf->exec();
    
	    if ( k == QDialog::Accepted ) {
		v = fromString<double>(bcf->bonusEdit->text().ascii());
	    }
	    delete(bcf);
	    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
	    GrabKeyboard();	
	    i = 1; // номер типа скидки в векторе скидок
    }
    
    //---------------------------------------
    }
    
    //---------------------------------------
    
    //-----(+)-----Zuskin-----15/12/2011-----
    if (v == 0) return 0; // если итоговая скидка = 0
    //--------------------------------------------------
       
    int res;
    if ( type_fl )    res = receipt->Add_Percent_Discount(i, v, code, name, shortcut, manual_fl);
    else    res = receipt->Add_Sum_Discount(i, v, code, name, shortcut, manual_fl);
    
    //Сумма скидки больше максимальной скидки на чек
    if ( res == 1 )  {   Error(84);    return -1;   }
    // Сумма надбавки больше максимальной надбавки на чек"
    if ( res == 2 )   {    Error(86);  return -1;   }	
    //  Сумма скидки превышает сумму чека с учетом всех скидок, начисление скидки отменено!
    if ( res == 3 )   {    Error(47);  return -1;   }	
    // есть товары с ограниченной мин ценой
    if ( res == 4 ) {  Error(106); return -1;  }
    
    QString s;
    s.setNum(receipt->GetAllDiscountSum(), 'f', 2);
    redallTextLabel->setText(s);
    
    //s.setNum(receipt->Sum() - receipt->RecalculationDiscounts(), 'f', 2);
    s.setNum(receipt->GetResultSum(), 'f', 2);
    //puts("CHECK REDUCTION");
    allTextLabel->setText(s);
    
    ReductionCustomerDisplay(i, -1);
    
    str = "";
    //-----(+)-----Zuskin-----29/09/2011-----
    StateStore::GetInstance()->SetViewState(REC);
    //-------------------------------------------------
    switch(i)    {
    case 1 :
	//-----(*)-----Zuskin-----29/09/2011-----
	//statusTextLabel->setText(DefCodec->toUnicode("начисление фикс. скидки на чек"));
	//break;
	{
	    statusTextLabel->setText(DefCodec->toUnicode("начисление фикс. скидки на чек"));
	    str = DefCodec->toUnicode("скидка на чек: ");
	    str += DbCodec->toUnicode(name);
	    str += DefCodec->toUnicode(" код: ");
	    str += DbCodec->toUnicode(code);
	    adjustFont();
                    textTextLabel->setText(str);
	    StateStore::GetInstance()->SetViewState(MESS);
	    break;
	}
	//-----------------------------------------------
    case 2 :
	statusTextLabel->setText(DefCodec->toUnicode("начисление произв. скидки на чек"));
	break;
    }
    return 0;
}

//======================================================================//
void RegistrationForm::FixedReduction() //CTRL + F Фиксированная скидка
{
    //   puts("RegistrationForm::FixedReduction()");
    
    /*printf("StateStore::GetInstance()->GetViewState() = %d \n", StateStore::GetInstance()->GetViewState());
    printf("StateStore::GetInstance()->GetSessionState() = %d \n", StateStore::GetInstance()->GetSessionState());
    printf("StateStore::GetInstance()->GetReceiptState() = %d \n", StateStore::GetInstance()->GetReceiptState());*/
    
    
    QTableSelection sel = receiptTable -> selection(0);
    int p_sel = sel.topRow();
    
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED ) {   Error(74);  return;	  }
    if ( StateStore::GetInstance()->GetReceiptState() == FREE )  {    Error(14);    return;     }        // чек не открыт
    if ( StateStore::GetInstance()->GetReceiptState() == COPY )  {   Error(36);   return;     }                            // копия чека
    
    char area = 'f'; // по умолчанию скидка на позицию 
    if ( (StateStore::GetInstance()->GetViewState() == INS) || (StateStore::GetInstance()->GetViewState() == REC) ) 
	area = 't'; // скидка на чек 
    
    if (p_sel == -1)    {   Error(68);     return;      }
    
    //  else if((mode != 'i') && (mode != 'k') && (!receipt->GetPosElemRef(p_sel)->enable_flag))  // позиция сторнированна   
    
    if ( (area == 'f') && (!receipt->GetPosElemRef(p_sel)->enable_flag) )  {      Error(22);     return;    }  // позиция сторнированна   
    
    if ( !GetRight(10) && !AuthorizationRequest(10) )     {	Error(72);	    return;      }
    
    ReleaseKeyboard();
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
    fixdiscountForm *fd = new fixdiscountForm;
    fd->SetArea(area); 
    
    int res = fd->FillTable();
    if ( res == 0 )      {
	fd->exec();
	QString code, name, shortcut, incode, cardcode;
	double value = 0;
	int type = -1;
	
	fd->GetChoisedPosition(code, name, shortcut, incode, cardcode, &type, &value);
	
	//puts("------------------");
	/*        name = HwCodec->fromUnicode(name);
	shortcut = HwCodec->fromUnicode(shortcut);*/
	//printf("fix_reduction code = <%s> \n", code.ascii() );
	//printf("fix reduction name = <%s> \n", name.ascii() );
	//printf("fix reducxtion code = <%s>, name = <%s>, shortcut = <%s>\n", code.ascii(), name.ascii(), shortcut.ascii() );
	//QString str_hwcodec = HwCodec->fromUnicode(name);
	//QString str_hwcodec_sh = HwCodec->fromUnicode(shortcut);
	//puts("----------------");
	
	
	//printf("fix reduction code = <%s> \n", incode.ascii() );
	//printf("fix red card code = <%s> \n", cardcode.ascii() );
	
	
	
	int state = -1;
	
	if(type != -1)        {
	    //puts("type != -1");
	    if(area == 't')	    {
		if (type == 0 || type == 2)  state =  CheckReduction(true, 1, value, code, name, shortcut, false);
//-----(*)-----Zuskin-----21/07/2011-----		
//		else    state =  CheckReduction(false, 1, value, code, name, shortcut, false);
                else    {
                    if (type == 4 ) { // bonuscard
                	state =  CheckReduction(false, 4, value, code, name, shortcut, false);
                    }
                    else {
            		state =  CheckReduction(false, 1, value, code, name, shortcut, false);
            	    }	
            	}    
//---------------------------------------
	    }    else          {
		if ( type == 0 ||  type == 2 )   state  =  PositionReduction(true, 1, value, code, name, shortcut, false);
		else        state  =   PositionReduction(false, 1, value, code, name, shortcut, false);
	    }
	    //printf("state = %d \n", state);
	    if ( state == 0 ) {
		if (area == 't') {
		    receipt->GetDiscountElemRef(1)->Set_Internal_Code(incode);
		    receipt->GetDiscountElemRef(1)->Set_Card_Code(cardcode); 
		} else {
		    QTableSelection sel = receiptTable -> selection(0);
		    int p_sel = sel.topRow(); 
		    receipt->GetPosElemRef(p_sel)->GetDiscountElemRef(1)->Set_Internal_Code(incode);
		    receipt->GetPosElemRef(p_sel)->GetDiscountElemRef(1)->Set_Card_Code(cardcode); 		    
		}
	    }
	    //printf("Receipt fix discount code = %s cardcode = %s\n", receipt->GetDiscountElemRef(1)->Get_Internal_Code(),  receipt->GetDiscountElemRef(1)->Get_Card_Code());
	    
	    QString s;
	    //s.setNum(receipt->Sum() - receipt->RecalculationDiscounts(), 'f', 2);
	    s.setNum(receipt->GetResultSum(), 'f', 2);
	    //puts("FIXED REDUCTION");
	    allTextLabel->setText(s);
	}
	delete(fd);	
    }      else      {
	delete(fd);
	Error(69);
    }
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();
    
    if(authorization_request_flag) SwapToAuthorizedUser();
    
    /*printf("StateStore::GetInstance()->GetViewState() = %d \n", StateStore::GetInstance()->GetViewState());
    printf("StateStore::GetInstance()->GetSessionState() = %d \n", StateStore::GetInstance()->GetSessionState());
    printf("StateStore::GetInstance()->GetReceiptState() = %d \n", StateStore::GetInstance()->GetReceiptState());*/
    
}

//======================================================================//
void RegistrationForm::PercentReduction(bool flag) //CTRL + D/CTRL + N //Процентная скидка/надбавка
{
    //puts("RegistrationForm::PercentReduction()");
    QTableSelection sel = receiptTable -> selection(0);
    int p_sel = sel.topRow();
    
    if(StateStore::GetInstance()->GetSessionState() == BLOCKED)
	Error(74);
    else if(StateStore::GetInstance()->GetReceiptState() == FREE)
	Error(14);
    else if(StateStore::GetInstance()->GetReceiptState() == COPY)
	Error(36);
    else
    {
	char area = 'f'; // по умолчанию скидка на позицию 
	if((StateStore::GetInstance()->GetViewState() == INS) || (StateStore::GetInstance()->GetViewState() == REC))
	    area = 't'; // скидка на чек 
	
	if(p_sel == -1) Error(68);
	else if((area == 'f') && (!receipt->GetPosElemRef(p_sel)->enable_flag))  // позиция сторнированна   
	    Error(22);
	else if(GetRight(9) || AuthorizationRequest(9))
	{
	    if(str != "")
	    {
		bool f;
		double p = str.toDouble(&f);
		if(!f || (p < 0.01) || (p > 99.99)) Error(42); //Ошибка : не верно заданна скидка
		else
		{
		    if(flag) p = -p;
		    switch(area)
		    {
		    case 'f' :
			PositionReduction(true, 2, p, "", DbCodec->fromUnicode(DefCodec->toUnicode("произвольная % на позицию")), "%", true);
			break;
		    case 't' :
			CheckReduction(true, 2, p, "", DbCodec->fromUnicode(DefCodec->toUnicode("произвольная % на чек")), "%", true);
			break;
		    }
		    QString s;
		    //s.setNum(receipt->Sum() - receipt->RecalculationDiscounts(), 'f', 2);
		    s.setNum(receipt->GetResultSum(), 'f', 2);
		    //puts("PERCENT REDUCION");
		    allTextLabel->setText(s);  
		}
	    }
	    else Error(44); //Ошибка : не заданна скидка
	    
	    if(authorization_request_flag) SwapToAuthorizedUser();
	}
	else Error(72);
    }
}

//======================================================================//
void RegistrationForm::SumReduction(bool flag) //ALT + D/ALT +N //Скидка/надбавка суммой 
{
    //puts("RegistrationForm::SumReduction()");
    QTableSelection sel = receiptTable -> selection(0);
    int p_sel = sel.topRow();
    
    if(StateStore::GetInstance()->GetSessionState() == BLOCKED)
	Error(74);
    else if(StateStore::GetInstance()->GetReceiptState() == FREE)
	Error(14);
    else if(StateStore::GetInstance()->GetReceiptState() == COPY)
	Error(36);
    else
    {
	char area = 'f'; // по умолчанию скидка на позицию 
	if((StateStore::GetInstance()->GetViewState() == INS) || (StateStore::GetInstance()->GetViewState() == REC))
	    area = 't'; // скидка на чек 
	
	if(p_sel == -1) Error(68);
	else if((area == 'f') && (!receipt->GetPosElemRef(p_sel)->enable_flag))  // позиция сторнированна   
	    Error(22);
	else if(GetRight(9) || AuthorizationRequest(9))
	{
	    if(str != "")
	    {
		bool f;
		double s = str.toDouble(&f);
		if(!f || (s < 0.01) || (s > 99999999.99))
		    Error(42);
		else
		{
		    if(flag)
			s = -s;
		    QString codecs;
		    switch(area)
		    {
		    case 'f' : 
			codecs = DbCodec->fromUnicode(DefCodec->toUnicode("произвольная S на позицию"));
			PositionReduction(false, 2, s, "", codecs.ascii(), "S", true);
			break;
		    case 't' :
			codecs = DbCodec->fromUnicode(DefCodec->toUnicode("произвольная S на чек"));
			CheckReduction(false, 2, s, "", codecs.ascii(), "S", true);
			//oldmode = mode;
			//mode = 'k';	      
			break;
		    }
		    QString s;
		    //s.setNum(receipt->Sum() - receipt->RecalculationDiscounts(), 'f', 2);
		    s.setNum(receipt->GetResultSum(), 'f', 2);
		    //puts("SUM REDUCTION");
		    allTextLabel->setText(s);
		}
	    }
	    else Error(44);
	    
	    if(authorization_request_flag) SwapToAuthorizedUser();
	}
	else Error(72);
    }
}

//======================================================================//
void RegistrationForm::CancelReduction() //CTRL + C*/ //Отмена скидки
{
//    puts("RegistrationForm::CancelReduction()");
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED )  {  Error(74);  return;  }
    if ( StateStore::GetInstance()->GetReceiptState() == FREE )   {    Error(14);  return;   }
    if ( StateStore::GetInstance()->GetReceiptState() == COPY )  {    Error(36);  return;   }
    
    int p_sel;
    QTableSelection sel;
    
    char area = 'f'; // по умолчанию скидка на позицию 
    if ( (StateStore::GetInstance()->GetViewState() == INS) || (StateStore::GetInstance()->GetViewState() == REC) )      area = 't'; // скидка на чек 
    
    if ( area == 't' )	     p_sel = -1;
    else    {
	sel = receiptTable->selection(0);
	p_sel = sel.topRow();
    }	
      // позиция сторнированна   
    //-----(*)-----Zuskin-----04/06/2011-----
    //if ( (area == 'f') && (!receipt->GetPosElemRef(p_sel)->enable_flag) )  {   Error(22);  return;   }
    if  (area == 'f') {
	if (p_sel == -1) {
	    Error(67);
	    return;
	}
	if (!receipt->GetPosElemRef(p_sel)->enable_flag) {
	    Error(22);
	    return;
	}
    }
    //-------------------------------------------------
    if ( !GetRight(12) && !AuthorizationRequest(12) ) {   Error(72); return;  }	
    ReleaseKeyboard();
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    canceldiscountForm *cdf = new canceldiscountForm;
    int ret = cdf->GetMaybeDiscounts(receipt, p_sel);    
    
    if ( ret < 0 )       {    
	delete(cdf);	
	Error(67);    
	HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
	GrabKeyboard();
	if ( authorization_request_flag ) SwapToAuthorizedUser();
	return;
    } 
    
    if ( cdf->exec() == QDialog::Rejected ) {
	///puts("-------");
	delete cdf;
	HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
	GrabKeyboard();
	
	if ( authorization_request_flag ) SwapToAuthorizedUser();
	return;
    }    
    
    if ( p_sel == -1 )        {
	
	//printf("что со cкидкой чека? \n"); 
	//printf("флаг %d\n индекс %d\n", cdf->choose[i], i);
	//  if(i == 0) Reg.Delete_All_Check_Scheme();
	//DeleteCheckScheme();
	
	sel = receiptTable -> selection(0);
	p_sel = sel.topRow();
	PositionPreference(p_sel);
	/*fix*/                                                 /*hand*/                                      /*card*/
	if ( receipt->GetDiscountElemRef(1)->df || receipt->GetDiscountElemRef(2)->df || receipt->GetDiscountElemRef(3)->df )
	    StateStore::GetInstance()->SetViewState(REC);
	else
	    StateStore::GetInstance()->SetViewState(GENERAL);
	statusTextLabel->setText(DefCodec->toUnicode("отмена скидок на чек"));
	
    }   else   {
	PositionPreference(p_sel);
	statusTextLabel->setText(DefCodec->toUnicode("отмена скидок на позицию"));       
    }		
    
    QString upstr = HwCodec->fromUnicode(DefCodec->toUnicode("промежуточный итог"));
    QString downstr = allTextLabel->text();
    
    CDSP cdsp(hw);
    cdsp.CustomerDisplay(upstr.ascii(), downstr.ascii());
    
    delete(cdf);
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();
    
    if ( authorization_request_flag ) SwapToAuthorizedUser();
    
}


//######################################--END--REDUCTION--SECTION--####################################//

//======================================================================//
void RegistrationForm::ReductionPrice()    /* */ //редактировать цену
{  
    // puts("RegistrationForm::ReductionPrice()");
    
    QString rrr = str;
    str = "";
    
    if (StateStore::GetInstance()->GetViewState() == FILL)    ClearForNewOperation();
    
    if (StateStore::GetInstance()->GetSessionState() == BLOCKED)    { Error(74); return; }
    if (StateStore::GetInstance()->GetReceiptState() == FREE)    { Error(14); return; } 
    if (StateStore::GetInstance()->GetReceiptState() == COPY)    { Error(37); return; }
    if (StateStore::GetInstance()->GetViewState() == REC)   { Error(45); return; } 
    //-----(+)-----Zuskin-----16/12/2011-----
    if (StateStore::GetInstance()->GetPrevState() == REC)   { Error(45); return; } 
    //printf("StateStore::GetInstance()->GetViewState() = %d\n", StateStore::GetInstance()->GetViewState());
    //printf("StateStore::GetInstance()->GetPrevState() = %d\n", StateStore::GetInstance()->GetPrevState());
    
    //-------------------------------------------------
    
    
    QTableSelection sel = receiptTable -> selection(0);
    int p_sel = sel.topRow();
    if ( p_sel == -1 )   { Error(12); return; }
    
    if ( !receipt->GetPosElemRef(p_sel)->enable_flag ) { Error(22);  return; } 
    
    if ( !GetRight(6) && !AuthorizationRequest(6) )  {  Error(72);  return;   }   // ДЛЯ ВЫПОЛНЕНИЯ ОПЕРАЦИИ НЕДОСТАТОЧНО ПРАВ
    
    bool nf;
    double p = rrr.toDouble(&nf);
    if  ( (rrr == "") || (nf == false) || (p < 0.01) || (p > 99999999.99) )   {
	Error(16);   
	if ( authorization_request_flag ) SwapToAuthorizedUser();   
	return; 
    }
    
    if ( sp.control_price_edit == NO ) {
	Error(97);
	return;
    }
    
    if ( (sp.control_price_edit == WARECARD) && ((receipt->GetPosElemRef(p_sel)->flags & PRICEEDIT) == 0) ) {
	Error(103);
	return;
    } 
    
    if (  StateStore::GetInstance()->GetReceiptState() != RETURN ) {
	
	if (! (atoi(receipt->GetPosElemRef(p_sel)->GetAdditionId()) > 0)  ) {
	    if ( (sp.control_min_price == 1) && (receipt->GetPosElemRef(p_sel)->min_price * receipt->GetPosElemRef(p_sel)->quantity) > (p * receipt->GetPosElemRef(p_sel)->quantity - receipt->GetPosElemRef(p_sel)->RecalculationDiscounts(p * receipt->GetPosElemRef(p_sel)->quantity)) )
	    { 
		Error(53); 
		if ( authorization_request_flag ) SwapToAuthorizedUser(); return;  
	    }
	    if ( (sp.control_min_price == 2) && (receipt->GetPosElemRef(p_sel)->min_price * receipt->GetPosElemRef(p_sel)->quantity) > (p * receipt->GetPosElemRef(p_sel)->quantity - receipt->GetPosElemRef(p_sel)->RecalculationDiscounts(p * receipt->GetPosElemRef(p_sel)->quantity)) )
	    { 
		p = receipt->GetPosElemRef(p_sel)->min_price;
	    }
	}
    }
    
    
    int er;
    //------(*)------Zuskin------14/06/2011------
    double new_sum = Round ( (p * receipt->GetPosElemRef(p_sel)->quantity) * 100 ) / 100;
    //if ( (er =  Reg.Overflow_Control(receipt->Sum(), p * receipt->GetPosElemRef(p_sel)->quantity, receipt->GetPosElemRef(p_sel)->sum)) != 0 ) {
    if ( (er =  Reg.Overflow_Control(receipt->Sum(), new_sum, receipt->GetPosElemRef(p_sel)->sum)) != 0 ) {
	Error(er);
	if ( authorization_request_flag ) SwapToAuthorizedUser();
	return;  
    }
    
    statusTextLabel->setText(DefCodec->toUnicode("редактирование цены"));
    QString s;
    QTableItem *i1 = new QTableItem(receiptTable, QTableItem::Never, s.setNum(p, 'f', 2));
    receiptTable->setItem(p_sel, 1, i1);
    //QTableItem *i3 = new QTableItem(receiptTable, QTableItem::Never, s.setNum(p * receipt->GetPosElemRef(p_sel)->quantity, 'f', 2));
    QTableItem *i3 = new QTableItem(receiptTable, QTableItem::Never, s.setNum(new_sum, 'f', 2));
    //--------------------------------------------
    receiptTable->setItem(p_sel, 3, i3);
    
    //            AutoReduction(p_sel);
    receipt->GetPosElemRef(p_sel)->price = p;
    //s.setNum(receipt->Sum() - receipt->RecalculationDiscounts(), 'f', 2);
    s.setNum(receipt->GetResultSum(), 'f', 2);
    //puts("REDUCTION PRICE");
    allTextLabel->setText(s);
    //ChooseCheckAutoReduction();
    PositionPreference(p_sel);
    PositionCustomerDisplay();
    
    if ( authorization_request_flag ) SwapToAuthorizedUser();
    
}

//======================================================================//
void RegistrationForm::ReductionQuantity() /*CTRL + Q*/ //редактировать количество
{
    //   puts("RegistrationForm::ReductionQuantity()");
    if ( StateStore::GetInstance()->GetViewState() == FILL )	ClearForNewOperation();
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED ) {	Error(74); return;  }
    if ( StateStore::GetInstance()->GetReceiptState() == FREE )   {   Error(14);  return;   }
    if ( StateStore::GetInstance()->GetReceiptState() == COPY )  {    Error(37); return;   }
    if ( StateStore::GetInstance()->GetViewState() == REC )  {   Error(45);    return;   }
    //-----(+)-----Zuskin-----16/12/2011-----
    if (StateStore::GetInstance()->GetPrevState() == REC)   { Error(45); return; } 
    //--------------------------------------------------
    
    
    QTableSelection sel = receiptTable -> selection(0);
    int p_sel = sel.topRow();
    if ( p_sel == -1 )    {
	Error(12);
	return;
    }
    if ( !receipt->GetPosElemRef(p_sel)->enable_flag )   {	Error(22);   return;   }
    if ( !GetRight(5) && !AuthorizationRequest(5) )      {
	Error(72); // ДЛЯ ВЫПОЛНЕНИЯ ОПЕРАЦИИ НЕДОСТАТОЧНО ПРАВА
	return;
    }
    
    /*printf("receipt->GetPosElemRef(p_sel)->flags = %d \n", receipt->GetPosElemRef(p_sel)->flags);
    printf("QUANTITYEDIT = %d \n", QUANTITYEDIT);
    printf("receipt->GetPosElemRef(p_sel)->flags & QUANTITYEDIT = %d \n", receipt->GetPosElemRef(p_sel)->flags & QUANTITYEDIT);*/
    
    if ( (receipt->GetPosElemRef(p_sel)->flags & QUANTITYEDIT) == 0 ) {
	if ( authorization_request_flag ) SwapToAuthorizedUser();
	Error(98);
	return;
    }
    
    bool nf;      
    double q = -1, q1 = -1;
    if ( textTextLabel->text() != "" )         q = str.toDouble(&nf);
    if ( quantTextLabel->text() != "" )       q1 = quantTextLabel->text().toDouble(&nf);
    if ( (q != -1) && (q1 != -1) && (q != q1) ) {
	if ( authorization_request_flag ) SwapToAuthorizedUser();
	Error(51);
	return;
    }
    if ( q1 != -1 ) q = q1;
    if ( (nf == false) || (q < 0.001) || (q > 9999999.999) )  {
	if ( authorization_request_flag ) SwapToAuthorizedUser();
	Error(55);
	return;
    }
    if ( (receipt->GetPosElemRef(p_sel)->flags & ALLOWFRACTIONAL == 0) && ((q - floor(q)) != 0) ) {
	if ( authorization_request_flag ) SwapToAuthorizedUser();
	Error(8);
	return;
    }
    
    int er = 0;
    if ( StateStore::GetInstance()->GetReceiptState() == SELL )     {
	//, !receipt->GetPosElemRef(p_sel)->code_flag);
	er = Reg.Quantity_Control(receipt->GetPosElemRef(p_sel)->GetCode(), q, receipt->GetPosElemRef(p_sel)->GetAdditionId());
	if  ( er !=0 )      Error(er);
    }
    if ( er == 0 )   {
	//-----(*)-----Zuskin-----14/06/2011-----
	double new_sum = Round( (q * receipt->GetPosElemRef(p_sel)->price) * 100 ) / 100;
	er =  Reg.Overflow_Control(receipt->Sum(), new_sum, receipt->GetPosElemRef(p_sel)->sum);
	if (er != 0)  {    if(authorization_request_flag) SwapToAuthorizedUser();     Error(er);    return;     }
	
	statusTextLabel->setText(DefCodec->toUnicode("редактирование кол-ва"));
	QString s;
	QTableItem *i2 = new QTableItem(receiptTable, QTableItem::Never, s.setNum(q, 'f', 3));
	receiptTable->setItem(p_sel, 2, i2);
	//QTableItem *i3 = new QTableItem(receiptTable, QTableItem::Never, s.setNum(q * receipt->GetPosElemRef(p_sel)->price, 'f', 2));
	QTableItem *i3 = new QTableItem(receiptTable, QTableItem::Never, s.setNum(new_sum, 'f', 2));
	//---------------------------------------
	receiptTable->setItem(p_sel, 3, i3);   
	//            AutoReduction(p_sel);
	//ChooseCheckAutoReduction();
	receipt->GetPosElemRef(p_sel)->quantity = q;
	//s.setNum(receipt->Sum() - receipt->RecalculationDiscounts(), 'f', 2);
	s.setNum(receipt->GetResultSum(), 'f', 2);
	allTextLabel->setText(s);
	PositionPreference(p_sel);
	PositionCustomerDisplay();
	str = "";
	AddWareController::GetInstance()->SetQuantity(1,0);
    } 
    
    if(authorization_request_flag) SwapToAuthorizedUser();
    
    quantity_flag = false;	
    quantTextLabel->setText("");
    
}
//======================================================================//
void RegistrationForm::RepetitionSale() /*CTRL + R*/ //Повтор продажи
{
    //puts("RegistrationForm::RepetitionSale()");
    if(StateStore::GetInstance()->GetViewState() == FILL)	ClearForNewOperation();
    
    if(StateStore::GetInstance()->GetSessionState() == BLOCKED)	Error(74);
    else if(StateStore::GetInstance()->GetReceiptState() == FREE) Error(14);  ///УТОЧНИТЬ ВСЕ ОСТАЛЬНЫЕ СОСТОЯНИЯ (mode)
    else if(StateStore::GetInstance()->GetReceiptState() == COPY) Error(35);
    else if(StateStore::GetInstance()->GetViewState() == REC)	Error(45);
    //-----(+)-----Zuskin-----16/12/2011-----
    else if (StateStore::GetInstance()->GetPrevState() == REC)   Error(45);
    //--------------------------------------------------
    //  if((mode == 'r') || (mode == 'e') || (mode =='i')) совсем уже бред
    else
	RepetitionLastPosition();
}

//======================================================================//
void RegistrationForm::RepetitionLastPosition()
{    
    puts("RegistrationForm::RepetitionLastPosition()");
    
    QTableSelection sel = receiptTable -> selection(0);
    int p_sel = sel.topRow();
    
    if(p_sel != -1)
    {
	if(!receipt->GetPosElemRef(p_sel)->enable_flag)	    Error(22);
	else if(receipt->GetPosElemRef(p_sel)->flags & ALLOWFRACTIONAL == 0) Error(28);
	else if(GetRight(7) || AuthorizationRequest(7))
	{
	    statusTextLabel->setText(DefCodec->toUnicode("повтор продажи"));
	    
	    //printf("p_sel = %d \n", p_sel);
	    //printf("receipt->GetPosElemRef(p_sel)->GetCode() = %s \n", receipt->GetPosElemRef(p_sel)->GetCode());
	    
	    //-----(*)-----Zuskin-----14/10/2011-----
	    //AddWareController::GetInstance()->SetQuantity(receipt->GetPosElemRef(p_sel)->quantity, 0);
	    if ( quantTextLabel->text().length() == 0 && quantTextLabel->text().length() == 22) {
		AddWareController::GetInstance()->SetQuantity(receipt->GetPosElemRef(p_sel)->quantity, 0);
	   }
	    //-------------------------------------------------
    	    //-----(*)-----Zuskin-----03/08/2011-----
	    //AddWareByBarcode("", receipt->GetPosElemRef(p_sel)->GetCode(), true);
	    AddWareByBarcode("", receipt->GetPosElemRef(p_sel)->GetCode(), true, -1);
	    //-------------------------------------------------
	    	    
	    if(authorization_request_flag) SwapToAuthorizedUser();
	}
	else Error(72); // ДЛЯ ВЫПОЛНЕНИЯ ОПЕРАЦИИ НЕДОСТАТОЧНО ПРАВ
    }
}

//########################--PRINT--COPY--CHECK--SECTION--#####################################//

//======================================================================//
void RegistrationForm::Check() //CTRL + V //Печать Просмотр копии чека
{
    // puts("RegistrationForm::Check()"); 
    /* int check_num = hw->ECR_ReadCheckNumber();
  if (check_num == -1) { Error(19); return;}
  QString ss;*/
    
    QString s;
    
    if ( StateStore::GetInstance()->GetViewState() == FILL )    ClearForNewOperation();
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED )  {  Error(74); return;	}
    if ( (StateStore::GetInstance()->GetReceiptState() == SELL ) || ( StateStore::GetInstance()->GetReceiptState() == RETURN ) ) {   Error(24);  return;    }
    
    if ( !GetRight(24) && !AuthorizationRequest(24) )    {   Error(72);   return;   }
    
    if ( str == "" ) {
	Error(23);
	if ( authorization_request_flag ) SwapToAuthorizedUser();
	return;
    }
    
    s = "печать копии чека";
    statusTextLabel->setText(DefCodec->toUnicode(s));
    int n_check;
    bool nf;
    int n = str.toInt(&nf);
    
    if ( !nf ) {
	Error(23);
	return;
    }    else      {
	n_check = n; 
	int res = CheckList(n, true, false);
	if ( res != 0 )    {
	    if (res == -1) Error(88);
	    else if (res == 1) Error(89);
	    else if ( res == -15 ) return;
	    else Error(res);	    
	}
    }
    
    QTableSelection sel = receiptTable -> selection(0);
    int p_sel = sel.topRow();
    
    if ( p_sel != -1 ) 	      {
	ctrl_v = TRUE;
	StateStore::GetInstance()->SetReceiptState(COPY);
	s = "копия чека";
	modeTextLabel->setText(DefCodec->toUnicode(s));
	receipt->SetCheckType(CHECK_COPY);
	receipt->SetCheckNumber(n_check);
    }
    
    if(authorization_request_flag) SwapToAuthorizedUser();
}

//########################--END--PRINT--COPY--CHECK--SECTION--#####################################//


//#######################--AUTHORIZATION--SECTION--###############################################//

//======================================================================//
bool RegistrationForm::GetRight(int right)
{
    return Authorization::GetInstance()->GetRight(right);
}

//======================================================================//
void RegistrationForm::AuthorizationRequested()
{
    operTextLabel->setText(DbCodec->toUnicode( Authorization::GetInstance()->GetUserName()));
    if (StateStore::GetInstance()->GetReceiptState() != COPY) receipt->setCashier(Authorization::GetInstance()->GetUserShortcut());	
    ins_tr->SetUserId( Authorization::GetInstance()->GetUid() );
    authorization_request_flag = true;
}

//======================================================================//
void RegistrationForm::SwapToAuthorizedUser()
{
    
    Authorization::GetInstance()->SwapUser();
    operTextLabel->setText(DbCodec->toUnicode( Authorization::GetInstance()->GetUserName()));
    if (StateStore::GetInstance()->GetReceiptState() != COPY)  receipt->setCashier(Authorization::GetInstance()->GetUserShortcut());
    ins_tr->SetUserId( Authorization::GetInstance()->GetUid() );
    authorization_request_flag = false;
}
//======================================================================//
bool RegistrationForm::AuthorizationRequestForAlliedRights(int * index, int num_rights)
{
    //puts("bool RegistrationForm::AuthorizationRequestForAlliedRights(int * index, int num_rights)");
    ReleaseKeyboard();
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
    if(Authorization::GetInstance()->RequestWithAlliedRights(index, num_rights))
    {
	operTextLabel->setText( DbCodec->toUnicode( Authorization::GetInstance()->GetUserName() ) );
	if (StateStore::GetInstance()->GetReceiptState() != COPY) receipt->setCashier(Authorization::GetInstance()->GetUserShortcut());
	ins_tr->SetUserId( Authorization::GetInstance()->GetUid() );
	authorization_request_flag = true;
    }
    else
	authorization_request_flag = false;
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();
    
    
    
    return authorization_request_flag;
}

//======================================================================//
bool RegistrationForm::AuthorizationRequest(int index)
{
    //puts("bool RegistrationForm::AuthorizationRequest(int index)");
    ReleaseKeyboard();
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
    if(Authorization::GetInstance()->RequestWithRight(index))
    {
	operTextLabel->setText(DbCodec->toUnicode(Authorization::GetInstance()->GetUserName()));
	if (StateStore::GetInstance()->GetReceiptState() != COPY) receipt->setCashier(Authorization::GetInstance()->GetUserShortcut());
	ins_tr->SetUserId( Authorization::GetInstance()->GetUid() );
	authorization_request_flag = true;
    }
    else
	authorization_request_flag = false;
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();
    
    
    
    return authorization_request_flag;
}

//#######################--END--AUTHORIZATION--SECTION--##########################################//

//======================================================================//
//Если касса заблокировалась по автоблокировке, то auto_flag = true
void RegistrationForm::Block(bool auto_flag) /*CTRL + B*/ //Блокирование 	
{  
    
    if (! this->isActiveWindow() ) return;
    if(sp.autoblock_flag) bt->Stop(); // Чтобы выключить автоблокировку....
    
    if(auto_flag)	Pause_Work(); 
    else	Not_Work();
    
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) disconnect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence()) );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) disconnect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    while ( Authorization::GetInstance()->Request(true) != 0 )	;
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence()) );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    Pause_Work();
    
    operTextLabel->setText("");
    operTextLabel->setText( DbCodec->toUnicode( Authorization::GetInstance()->GetUserName() ) );
    if (StateStore::GetInstance()->GetReceiptState() != COPY) receipt->setCashier(Authorization::GetInstance()->GetUserShortcut());
    if ( sp.autoblock_flag ) bt->Start();
    
    ins_tr->SetUserId( Authorization::GetInstance()->GetUid() );
}

//###################################--MESSAGES--SECTION--###################################//
//======================================================================//
void RegistrationForm::Kkm_Error(int er)
{
    // puts("RegistrationForm::Kkm_Error(int er)");
    //statusTextLabel->setText(DefCodec->toUnicode("Ошибка"));
    
    printf("KKm_Error(%d)\n", er);
    
    QString mes_id, mes_str;
    
    WaitProcessStop();
    
    ReleaseKeyboard();
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
    hw->ECR_ReadError(er);
	 
    mes_id = hw->ECR_GetErrNum();
    mes_str = hw->ECR_GetErrDescription();
    kkm_Error(mes_id, mes_str);
    if ((er == -1) || (er == -2) || (er == 2))
		  cn_good = false;

    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();
    
    if ( (er == 103) || (er == 104 ) || ( er == 105 ) || ( er == 273 ) || ( er == 305 ) || ( er == 304 ) ) return;
 //   if ( hw->ECR_GetCheckState() == 1 ) hw->ECR_CancelCheck();
    //-----(*)-----Zuskin-----11/11/11-----
    //cancel_flag = true;//??????
    //if (er != -1) {
	cancel_flag = true;//??????
    //}
    //---------------------------------------------
}

//======================================================================//
void RegistrationForm::Message(int mes)
{
    //puts("void RegistrationForm::Message(int mes)");
    QString mes_id, mes_str;
    RegistrationMessageText(mes, mes_str, mes_id);
    
    ReleaseKeyboard();
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
    InterfaceErrorWin(mes_str, mes_id);
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();
}

//======================================================================//
void RegistrationForm::Error(int er)
{
    printf("Error(er = %d) \n", er);
    
    WaitProcessStop();
    
    QString er_id, er_str;
    RegistrationErrorText(er, er_str, er_id);    
    
    
    //puts("------3083----------");	
    textTextLabel->setText("");
    deliveryTextLabel->setText("");
    
    str = "";
    //пока окно не открыто ошибки должны выводиться в отдельном окне...  
    if ( sp.error_output_window || init_flag )   {
	//ReleaseKeyboard();
	
	HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
	
	InterfaceErrorWin(er_str, er_id);
	
	//QMessageBox::critical( 0, DefCodec->toUnicode("Ошибка"), er_str, QMessageBox::Yes, 0 );
	//puts("---errorr window");
	
	HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
	
	//GrabKeyboard();
	//puts("--------oopppsss------");
    }  else  {
	str = er_str.replace('\n', " ");
	adjustFont();
	textTextLabel->setText(str);
	StateStore::GetInstance()->SetViewState(MESS);
    }
    statusTextLabel->setText(er_id.replace(':', ""));
	
}
//======================================================================//
int RegistrationForm::Acknowledgement(QString text)
{
    //puts("int RegistrationForm::Acknowledgement(QString text)");
    int ret = 1;
    
    ReleaseKeyboard();
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
    QMessageBox mb( DefCodec->toUnicode("Warning"), text, QMessageBox::NoIcon, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, QMessageBox::NoButton, this);
    
    mb.setButtonText( QMessageBox::Yes, DefCodec->toUnicode("Да") );
    mb.setButtonText( QMessageBox::No, DefCodec->toUnicode("Нет") );
    
    switch( mb.exec() )
    {
    case QMessageBox::Yes :
	//printf("Yes\n");
	ret = 0;
	break;
    case QMessageBox::No :
	//printf("Cancel\n");
	ret = 1;
	break;
    }
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();
    return ret;
}
//##############################--END--MESSAGES--SECTION--###################################//
//======================================================================//
int RegistrationForm::Registration(int section, bool total_sum_was_printed)
{         
    //printf("Print check by section - %d \n", section);
    
    int res = kkm_reg->KkmRegistration(true);   // В test mode - проверка возможности открыть чекa 
    // kkm_res - global param;
    if ( kkm_res != 0 )    { kkmerror_id = kkm_res; return kkm_res;      }
    
    ins_tr->CreateDateTimeString();
    
//-----(*)-----Zuskin-----17/05/2011-----
//  if (!ctrl_v) {	hw->ECR_OpenCheck(0);    }    
    if (!ctrl_v) {
	//-----(*)-----Zuskin-----24/11/2011-----
	if ( sp.sections.size() > 0 ) {
	    if (sp.sections[section].is_fiscal == true) {
		kkm_res = hw->ECR_OpenCheck(0);
		if ( kkm_res != 0 ) {
		    kkmerror_id = kkm_res;
		    return kkm_res;  
		}	
	    }
	}
	else {
	    kkm_res = hw->ECR_OpenCheck(0);
	    if ( kkm_res != 0 ) {
		kkmerror_id = kkm_res;
		return kkm_res;  
	    }
	}
	//------------------------------------------------------
    }
//--------------------

    //-----(*)-----Zuskin-----17/05/2011----- 
    //if ( (!hw->ECR_GetUseOneRegistration()) &&  ((hw->ECR_GetProtocol() == 4) || (hw->ECR_GetProtocol() == 6))&& (!ctrl_v) ) {
    if ( (!hw->ECR_GetUseOneRegistration()) &&  ((hw->ECR_GetProtocol() == 4) || (hw->ECR_GetProtocol() == 6) || (hw->ECR_GetProtocol() == 7))&& (!ctrl_v) ) {
    //---------------------------------------	
	kkm_res = kkm_reg->KkmPosRegistration(receipt);
	if  ( kkm_res != 0)   {    kkmerror_id = kkm_res;	    return kkm_res;  }
	
    }    else  {	
	
	if ((hw->ECR_GetEducationMode() && (hw->ECR_GetProtocol() != 4 && hw->ECR_GetProtocol() != 5))||(!hw->ECR_GetEducationMode())) {
	    
	    if ( !total_sum_was_printed ) {
		kkm_res = kkm_reg->KkmPrintTotalSumBySection(receipt, nalsum, credsum, contsum);
		if (kkm_res!=0){    kkmerror_id = kkm_res;    return kkm_res;}	 
	    }
     //-----(*)-----Zuskin-----17/05/2011-----	    
     //	    if (hw->ECR_GetProtocol() == 6){
	    if ((hw->ECR_GetProtocol() == 6) || (hw->ECR_GetProtocol() == 7)){ //для Касби-02/04
     //---------------------------------------
		kkm_res = kkm_reg->KkmPrintString(HwCodec->fromUnicode(DefCodec->toUnicode("КВИТАНЦИЯ К ЧЕКУ")));
		if (kkm_res!=0){	kkmerror_id = kkm_res;    return kkm_res; }
	    }
	    
	    kkm_res = kkm_reg->KkmPrintCheckPositions(receipt, section);
	    if  ( kkm_res != 0)   {  printf("KkmPrintCheckPositions() returns %d\n", kkm_res); kkmerror_id  = kkm_res;  return kkm_res;   }
	}
	
	if ( section == -1 ) {
	    kkm_res = kkm_reg->KkmRegistration(false, receipt->GetResultSum());
	    if ( kkm_res != 0 )    {    kkmerror_id = kkm_res;   return kkm_res;  }	
	    
	} else if ( section == 0 ) {
	    kkm_res = kkm_reg->KkmRegistration(false, receipt->GetSectionResultSum(0));
	    if ( kkm_res != 0 )    {   kkmerror_id = kkm_res;  return kkm_res;  }		    
	} 
    }
    return 0;
}

//======================================================================//
int RegistrationForm::ReturnRegistration(int section, bool total_sum_was_printed)
{
    //puts("-----------int RegistrationForm::ReturnRegistration()------------");
    if (( hw->ECR_GetProtocol() == 5 )&&( nalsum == 0) && (!hw->ECR_GetEducationMode())){
	ins_tr->CreateDateTimeString();
	kkm_res = kkm_reg->KkmPrintReturnNotNalCheck(copy, receipt, receipt->getCashier());
	return kkm_res;
    }
    
    kkm_res = kkm_reg->KkmReturn(true); 	// регистрация сумм возврата в фр  
    if ( kkm_res != 0 )  {    kkmerror_id = kkm_res;  return kkm_res;      }
    
    ins_tr->CreateDateTimeString();
    
    if ( sp.open_box_by_close_cheque )      {
	if ( sp.open_box_by_pay_with_rest && nalsum > 0 ) OpenBox(true);
	else if ( !sp.open_box_by_pay_with_rest ) OpenBox(true);
    }
    
    //-----(*)-----Zuskin-----17/05/2011-----
    //if (!ctrl_v) {	hw->ECR_OpenCheck(2);    }
    //-----(*)-----Zuskin-----24/11/2011-----
	if ( sp.sections.size() > 0 ) {
	    if (sp.sections[section].is_fiscal == true) {
		kkm_res = hw->ECR_OpenCheck(2);
		if ( kkm_res != 0 ) {
		    kkmerror_id = kkm_res;
		    return kkm_res;  
		}	
	    }
	}
	else {
	    kkm_res = hw->ECR_OpenCheck(2);
	    if ( kkm_res != 0 ) {
		kkmerror_id = kkm_res;
		return kkm_res;  
	    }
	}
	//------------------------------------------------------
    //--------------------------------------- 
    if ( (!hw->ECR_GetUseOneRegistration()) &&  ((hw->ECR_GetProtocol() == 4) || (hw->ECR_GetProtocol() == 6))&& (!ctrl_v) )  {	
	kkm_res = kkm_reg->KkmPosReturn(receipt);
	if  ( kkm_res != 0)   {     kkmerror_id = kkm_res;	    return kkm_res;  }  
	
    } else  {
	
	if ((hw->ECR_GetProtocol() != 5 && hw->ECR_GetEducationMode())||(!hw->ECR_GetEducationMode())) {
	    
	    if ( !total_sum_was_printed ) {
		kkm_res = kkm_reg->KkmPrintTotalSumBySection(receipt, nalsum, credsum, contsum);
		if (kkm_res!=0){    kkmerror_id = kkm_res;    return kkm_res;}	 
	    }
	    //-----(*)-----Zuskin-----17/05/2011-----
	    //if (hw->ECR_GetProtocol() == 6){
	    if ((hw->ECR_GetProtocol() == 6) || (hw->ECR_GetProtocol() == 7))  {
	    //---------------------------------------
		kkm_res = kkm_reg->KkmPrintString(HwCodec->fromUnicode(DefCodec->toUnicode("КВИТАНЦИЯ К ЧЕКУ")));
		if (kkm_res!=0)  {	    kkmerror_id = kkm_res;	return kkm_res;  }
	    }
	    
	    kkm_res = kkm_reg->KkmPrintCheckPositions(receipt, section);  		// печать позиций из чек
	    if  ( kkm_res != 0)   { kkmerror_id = kkm_res;   return kkm_res;  	    }
	}
	
	// регистрация сумм возврата в фр
	if ( section == -1 ) {
	    kkm_res = kkm_reg->KkmReturn(false, receipt->GetResultSum());
	    if ( kkm_res != 0 )    {    kkmerror_id = kkm_res;   return kkm_res;  }	
	} else if ( section == 0 ) {
	    kkm_res = kkm_reg->KkmReturn(false, receipt->GetSectionResultSum(0));
	    if ( kkm_res != 0 )    {   kkmerror_id = kkm_res;  return kkm_res;  }		    
	} 
	
    }
    
    return 0;
}

//======================================================================//
//user_flag = true - выход инициирован пользователем.
void RegistrationForm::CloseRegistrationForm(bool user_flag) /* F12 */ //выход
{
    if ( !user_flag )   {
	//может быть выдавать предупреждение о принудительном окончании работы
	Not_Work();
	close();
	
    }
    if ( StateStore::GetInstance()->GetReceiptState() == FREE ) {
	Not_Work();
	close();
    }
    
    if ( StateStore::GetInstance()->GetReceiptState() != FREE ) {
	if ( CancelCheck() == 0 )  {
	    Not_Work();
	    close();
	}
    }

}

//======================================================================//
void RegistrationForm::Not_Work()
{
    //puts("void RegistrationForm::Not_Work()");
    if(hw->DSP_GetType() != 0)    {
	CDSP cdsp(hw);
	QString upstr = HwCodec->fromUnicode(DbCodec->toUnicode(sp.cdsp_up_not_work));
	QString downstr = HwCodec->fromUnicode(DbCodec->toUnicode(sp.cdsp_down_not_work));
	cdsp.CustomerDisplay(upstr.ascii(), downstr.ascii(), sp.cdsp_up_not_work_mode_run, sp.cdsp_down_not_work_mode_run);
    }  
}

/**********************************TIMER--SLOTS--SECTION*************************************/

//======================================================================//
void RegistrationForm::Pause_Work()
{
    //puts("void RegistrationForm::Pause_Work()");
    if ( StateStore::GetInstance()->GetReceiptState() == FREE )  {
	CDSP cdsp(hw);
	QString upstr = HwCodec->fromUnicode(DbCodec->toUnicode(sp.cdsp_up_pause_work));
	QString downstr = HwCodec->fromUnicode(DbCodec->toUnicode(sp.cdsp_down_pause_work));
	
	cdsp.CustomerDisplay(upstr.ascii(), downstr.ascii(), sp.cdsp_up_pause_work_mode_run, sp.cdsp_down_pause_work_mode_run);
	
	HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    } 
}

//======================================================================//
void RegistrationForm::DateTimeSync()
{
    if( !payment_flag  && !return_flag && !copy_flag)   DateTimeControl();
    synctime_timer->start(SYNC_CONTROL_TIME, true);
}
//======================================================================//
void RegistrationForm::FillReceipt(int check_type)
{
    int r = 0;
    hw->ECR_ReadStatus();
    if (check_type = 1){
	//puts("read document return");
	r = hw->ECR_ReadDocumentNumber(2);
    }   else  {               
	//puts("read document registration");
	r = hw->ECR_ReadDocumentNumber(0);
    }
    if ( r != 0) return;

	 switch( hw->ECR_GetProtocol() ) {
	 case 4:{
		 if (!hw->ECR_GetEducationMode()){
		     r = hw->ECR_ReadStatus();
		     if(r != 0) {
			 kkmerror_id = r;
			 return;
		     }
		     receipt->SetDocumentNumber(hw->ECR_GetDocumentNumber()+1);
		 } else {
		     r = hw->ECR_ReadCheckNumber();
		     if(r != 0) {
			 kkmerror_id = r;
			 return;
		     }
		     receipt->SetDocumentNumber(hw->ECR_GetCheckNumber()-1);
		 }
		 break;
	     }
	 case 5:{
				receipt->SetDocumentNumber(hw->ECR_GetDocumentNumber()+1);
				break;}
	 default:{
				receipt->SetDocumentNumber(hw->ECR_GetCheckNumber() - 1);
		  } 
	 }
	
    receipt->SetSession(hw->ECR_GetSession());
    receipt->SetKkmSerial(hw->ECR_GetSerialNumber());
    
    receipt->setCashier(Authorization::GetInstance()->GetUserShortcut());
}

//======================================================================//

int RegistrationForm::TestKkmCloseCheck(int kkm_res, double fr_sum) {
    
    //получить сменный итог
    //-----(+)-----Zuskin-----11/11/2011-----
    int res = hw->ECR_ReadStatus();
    if ( res != 0 )    {
	CrashPrintSellCheck(kkm_res, true, true);
	return -1;
    } 
    //-------------------------------------------------
    //-----(+)-----Zuskin-----24/01/2012-----
    if ( hw->ECR_GetCheckState() == 1 ) {
	CrashPrintSellCheck(kkm_res, true, true);
	return -1;	
    }
    //--------------------------------------------------
    
    res = hw->ECR_ReadSessionSumm();
    if ( res != 0 )    {
	CrashPrintSellCheck(kkm_res, true, true);
	return -1;
    } 
    
    int check_number = -1;
    if (hw->ECR_GetProtocol() == 4) {
	int r = hw->ECR_ReadDocumentNumber(0);
	if ( r != 0 ) check_number = -1;
	else check_number =  hw->ECR_GetDocumentNumber();
    }	else {
	int r = hw->ECR_ReadCheckNumber();
	if ( r != 0 ) check_number = -1;
	else check_number = hw->ECR_GetCheckNumber();
    }
    
    
    if ( fr_sum == hw->ECR_GetSumm() ) {
	CrashPrintSellCheck(kkm_res, true, true);
	return -1;
    }
    if ( check_number == receipt->GetCheckNumber() ) {
	CrashPrintSellCheck(kkm_res, true, true);
	return -1;
    }
    return 0;
}
//======================================================================//
void RegistrationForm::CrashPrintSellCheck(int kkm_res, bool show_err, bool cancel_check_in_fr) {
    if ( show_err ) kkmerror_id =  kkm_res;
    if (cancel_check_in_fr) cancel_flag = true;
    if ( (kkm_reg->GetCreditStatus() != 1) && (kkm_reg->GetCreditStatus() != 17) ) {
	NPAS_CrushTransaction();
    }
    PrintToLogCloseCheck(-1, kkm_res);
}

//======================================================================//
int RegistrationForm::PrintCheck()
{      
    
    WaitProcessStart();
              
    // Печать Чека продажи! 
    if ( payment_flag ) {
	 puts("ПЕЧАТЬ ЧЕКА ПРОДАЖИ");
	 payment_flag = false;
	 
	 FillReceipt(0);
	 PrintToLogPosition(0);
	
	 kkm_res = hw->ECR_ReadStatus();
	 if ( kkm_res != 0 ){
	     CrashPrintSellCheck(kkm_res, true, false);
	     return -1;
	 } 	 
	 //-----(+)-----Zuskin-----25/01/2012-----
	 if ( hw->ECR_GetProtocol() == 1 ) {
	     if ((hw->ECR_GetAdvancedMode() == 2) || (hw->ECR_GetAdvancedMode() == 3)) {
		 // ШТРИХ ФР ожидает команду продолдения печати
		 kkm_res = hw->ECR_ContinuePrinting();
		 kkm_res = hw->ECR_ReadStatus();
		 if ( kkm_res != 0 ){
		     CrashPrintSellCheck(kkm_res, true, false);
		     return -1;
		 }
	     }	 
	 }
	 //--------------------------------------------------
	 if ( hw->ECR_GetCheckState() == 1 ) {
	     //если в ФР есть открытый документ, надо его сначала попытаться отменить
	     kkm_res = hw->ECR_CancelCheck();	
	   
	     if ( kkm_res != 0 )    {
    	        CrashPrintSellCheck(kkm_res, true, false);
	        error_id = 40;
	        return -1;
	    }  
	 }
	 //---------------------------------------------------
	  
	 //получить сменный итог до пробития чека, чтобы можно было сравнить со сменным итогом после пробития
	 kkm_res = hw->ECR_ReadSessionSumm();
	 if ( kkm_res != 0 )    {
	     CrashPrintSellCheck(kkm_res, true, false);  
	     return -1;
	 }  
	 double fr_sum = hw->ECR_GetSumm();
		
	if ( sp.sections.size() == 0 ) {
                // INPAS and SBER
	//-----(*)-----Mirabell-----21/02/2012-----
	int type_system = hw->PS_GetTypeSystem();
                switch(type_system){
                   case 2:{
                      kkm_res = kkm_reg->PrintCreditInfo(true);
                      break;
                   }
                   default:{
                      bool packet_answer = hw->PS_GetPacketTypeAnswer();
                      if (!packet_answer){
                         kkm_res = kkm_reg->PrintCreditInfo(false);
                         kkm_res = kkm_reg->PrintCreditInfo(true);
                      } else {
                         kkm_res = kkm_reg->PrintCreditInfo(true);
                      }
                      break;
                   }
                 }
               }
	//-----------------------------------------
	
	if ( sp.sections.size() == 0 ) {
	    int k = Registration(-1, true);
	    if ( k != 0 )   {  
		CrashPrintSellCheck(k, false, true);
		//-----(+)-----Zuskin-----11/11/11-----
		return -1;
		//----------------------------------------------	
	    }
	    //-----(+)-----Zuskin-----25/01/2012-----
	    kkm_res = hw->ECR_ReadStatus();
	    if ( kkm_res != 0 ){
		 CrashPrintSellCheck(kkm_res, true, false);
		 return -1;
	    }
	    //-----(*)-----Zuskin-----27/01/2012-----
	    //---(*)---Zuskin---22/02/2012---
	    //if (hw->ECR_GetProtocol() == 1) { // только для ШТРИХов
	    if ((hw->ECR_GetProtocol() == 1) && (hw->ECR_GetType() > 0)) { // только для ШТРИХов
	    //-------------------------------
		while (hw->ECR_GetCheckState() == 1) {
		    //--------------------------------------------------
		    kkm_res = kkm_reg->KkmCloseCheck(nalsum, credsum, contsum,receipt);
		    printf("Registration ------ KkmCloseCheck(nalsum = %f, credsum = %f, contsum = %f, receipt) return %d \n", nalsum, credsum, contsum, kkm_res);
		    if (kkm_res == 0) {	
			break;
		    }
		    if ((kkm_res == 103) && (hw->ECR_GetProtocol() == 1))  {    
			//-----(+)-----Zuskin-----24/01/2012-----
			if (kkm_res == 103) { // обрыв чековой ленты
			    WaitProcessStop();
			    ContinuePrintingForm *cpf = new ContinuePrintingForm;
			    releaseKeyboard();
			    cpf->show();
			    
			    while ((kkm_res == 103) && (hw->ECR_GetProtocol() == 1)) { 
				//для ШТРИХ в случае обрыва ленты надо (после вставки ленты) послать команду "продолжить печать"			
				waitmsec(500);
				if (cpf->OKpressed) {
				    kkm_res = hw->ECR_ContinuePrinting();
				}
				cpf->OKpressed = false;
			    }
			    delete cpf;	
			    grabKeyboard();
			    WaitProcessStart();
			    kkm_res = hw->ECR_ReadStatus();
			 }
		    } 
		    else {
			break;
		    }
		} // --- while ---
	    }
	    else { // для протоколов кроме ШТРИХ
		kkm_res = kkm_reg->KkmCloseCheck(nalsum, credsum, contsum,receipt);
		printf("Registration ------ KkmCloseCheck(nalsum = %f, credsum = %f, contsum = %f, receipt) return %d \n", nalsum, credsum, contsum, kkm_res);
	    }
	  
	    //-------------------------------------------------
	    if ( kkm_res != 0 ) {
		if ( kkm_res == -1 ) {
		    CrashPrintSellCheck(kkm_res, false, true);
		    kkmerror_id = kkm_res;
		    return -1;   
		}
		if ( TestKkmCloseCheck(kkm_res, fr_sum) < 0 ) return -1;
	    }
	}
	
	int count_printed_not_fiscal_section = 0;
	bool printed_total_sum_info = false;
	//-----(+)-----Zuskin-----24/01/2012-----
	bool fiscal_check_printed = false;
	//-------------------------------------------------
	int section_num_with_max_sum = receipt->GetSectionNumWithMaxSum();
	//printf("section_num_with_max_sum = %d \n", section_num_with_max_sum);
	
	for ( int section_num = 0; section_num < sp.sections.size(); section_num++ ) {
	    printf("Print  section_i - %d count pos = %d \n", section_num, receipt->GetSectionCountPosition(section_num));
	    
	    if ( receipt->GetSectionCountPosition(section_num) > 0 ) {
		int k = Registration(section_num, printed_total_sum_info);
		printed_total_sum_info = true;
		if ( k != 0 )   {  
		    CrashPrintSellCheck(k, false, true);
    		    //-----(*)-----Zuskin-----24/01/2012-----
		    //return -1;
		    if (!fiscal_check_printed) {
			return -1;
		    }
		    else {
			break;
		    }
		    //--------------------------------------------------
		}
		
		if (section_num == 0) {
		     kkm_res = hw->ECR_ReadStatus();
		     if ( kkm_res != 0 ){
			 CrashPrintSellCheck(kkm_res, true, false);
			 return -1;
		     } 
		     // печатаем фискальную секцию
		     //-----(*)-----Zuskin-----27/01/2012-----
		     //---(*)---Zuskin---22/02/2012---
    		     //if (hw->ECR_GetProtocol() == 1) { // только для ШТРИХов
		     if ((hw->ECR_GetProtocol() == 1) && (hw->ECR_GetType() > 0)) { // только для ШТРИХов
		     //-------------------------------
			 while (hw->ECR_GetCheckState() == 1) {
			
			     kkm_res = kkm_reg->KkmCloseCheck(receipt->GetSectionResultSum(0), 0, 0, receipt);				
			     if ( hw->ECR_GetProtocol() == 1 ) {
				 int amode = 5;
				 while (amode == 5) {
				     hw->ECR_ReadStatus();
				     amode = hw->ECR_GetAdvancedMode();	
				 }
			     }
			     if (kkm_res == 0) break;
			
			     if ((kkm_res == 103) && (hw->ECR_GetProtocol() == 1))  {    
				 //-----(+)-----Zuskin-----24/01/2012-----
				 if (kkm_res == 103) { // обрыв чековой ленты
				     WaitProcessStop();
				     ContinuePrintingForm *cpf = new ContinuePrintingForm;
				     releaseKeyboard();
				     cpf->show();
			    
				     while ((kkm_res == 103) && (hw->ECR_GetProtocol() == 1)) { 
					 //для ШТРИХ в случае обрыва ленты надо (после вставки ленты) послать команду "продолжить печать"			
					 waitmsec(500);
					 if (cpf->OKpressed) {
					     kkm_res = hw->ECR_ContinuePrinting();
					 }
					 cpf->OKpressed = false;
				     }
				     delete cpf;	
				     grabKeyboard();
				     WaitProcessStart();
				     kkm_res = hw->ECR_ReadStatus();
				 }
			     }
			     else {
				 break;
			     }
			 }//----- while -----
		     } 
		     else { // для протоколов кроме ШТРИХ
			 kkm_res = kkm_reg->KkmCloseCheck(nalsum, credsum, contsum,receipt);
			 printf("Registration ------ KkmCloseCheck(nalsum = %f, credsum = %f, contsum = %f, receipt) return %d \n", nalsum, credsum, contsum, kkm_res);	     
		     }
		    if (kkm_res != 0) {
			if ( TestKkmCloseCheck(kkm_res, fr_sum) < 0 ) return -1;
		    }
		    // после этого транзакции должны быть записаны обязательно!!!
		    //-----(+)-----Zuskin-----24/01/2012-----
		    fiscal_check_printed = true;
		    //-------------------------------------------------
		} else 
		    count_printed_not_fiscal_section++;
	    } 
	}
	
	if (count_printed_not_fiscal_section > 0) {
	    // принудительная печать клише чека после печати всех нефискальных квитанций
	    kkm_res = kkm_reg->KkmPrintDocHeadLine();
	    if ( kkm_res != 0 ) {
		if ( TestKkmCloseCheck(kkm_res, fr_sum) < 0 ) return -1;
	    }
	}
	
	/////////////////////////////************************//////////////////////////
	
	check_print_error = false;
	
	//printf("receipt->GetKkmSerial() = <%s> \n", receipt->GetKkmSerial());
	
	//-----(*)-----Zuskin-----18/11/2011-----
	//if ( ins_tr->InsertCheckRegistrationTransactions("0", "", nalsum, credsum, contsum, closed_session_flag) != 1 )
	if ( ins_tr->InsertCheckRegistrationTransactions("0", "", nalsum, credsum, contsum, closed_session_flag, true, string(manager_id.ascii())) != 1 )
                //-------------------------------------------------
	{
	    error_id = 21;
	    CrashPrintSellCheck(21, false, true);
	    return -1;
	}
		
	nalsum = 0;
	credsum = 0;
	contsum = 0;
	
	
	Reg.Update_Goods_Quantity(receipt);
	
	//-----(+)-----Zuskin-----22/06/2011-----
	printf("посмотрим, надо ли отправить скидку во внешнюю дисконтную систему...\n");
	if (hw->EDS_GetType() > 0) {
	    printf("внешняя дисконтная система включена!!!\n");
	    // отправляем информацию о предоставленной скидке во внешнюю дисконтную систему
	    if (receipt->GetAllDiscountSum() > 0) { // если вообще были скидки
		printf("скидки были...\n");
	        vector<Discount> dsc = receipt->discount;
		for (int jj = 0; jj < (int) dsc.size(); jj++) {
		    printf("dsc[%d].type = [%d]\n", jj, dsc[jj].GetType());
		    printf("cardcode=[%s]\n", dsc[jj].card_code.c_str());
		    printf("internal_code=[%s]\n", dsc[jj].internal_code.c_str());
		    printf("dsc.df = [%d]\n", (int) dsc[jj].df);
		    
		    if (dsc[jj].card_code > "") { // скидка начислена
		//	if (dsc[jj].type == 3) { // тип = внешняя дисконтная система
			    int n_pc = Read_N_PC();
			    hw->EDS_SendInfoAboutSale(n_pc, dsc[jj].internal_code.c_str(),dsc[jj].card_code.c_str(), receipt->GetResultSum()+receipt->GetAllDiscountSum(), receipt->GetAllDiscountSum());  
			    jj = (int) dsc.size() + 1;
		//	}
		    }	    		
		} 
	    
	    }
	    else {
		printf("скидок не было...\n");
	    }
	}
	//---------------------------------------
	
	//-----(+)-----Zuskin-----26/07/2011-----
	// добавить функцию списания бонусов с карты, если карта есть в чеке
	if ( receipt->GetDiscountElemRef(1)->df == true ) {
	    string ccode = receipt->GetDiscountElemRef(1)->GetCode(); 
	    if (ccode.length() > 0) { // есть какая-то карта, надо понять, не бонусная ли она
		int dtype = Reg.GetTypeByCard(ccode.c_str());
		if (dtype == 4) { // !!! bonuscard !!!
		    double dsum = receipt->GetDiscountElemRef(1)->dsum;
		    if (dsum != 0) {
			double cur_dsum = Reg.GetSummByCard(ccode.c_str());
			// производим списание по карте
			printf("производим списание [%f] бонусов\n", dsum);
			Reg.UpdateBonusCard(ccode.c_str(), cur_dsum - dsum);
		    }
		}
	    }
	}    
	//--------------------------------------------------
	
	check_closed = true; 
	
	receipt->CloseReceipt();
	pause_work_timer->start(PAUSE_TIME, true);
	StateStore::GetInstance()->SetReceiptState(FREE);
	int check_number = 0;
	if (hw->ECR_GetProtocol() == 4) {
	    int r = hw->ECR_ReadDocumentNumber(0);
	    if ( r != 0 ) check_number = -1;
	    else check_number =  hw->ECR_GetDocumentNumber();
	}
	else{
	    //puts("<<<<<<<<<<<int r = hw->ECR_ReadCheckNumber();>>>>>>>>>>>>>>>>>"); 
	    int r = hw->ECR_ReadCheckNumber();
	    //printf("-------------hw->ECR_ReadCheckNumber() = %d\n",r);
	    if ( r != 0 ) check_number = -1;
	    else check_number = hw->ECR_GetCheckNumber();
	}
	
	//printf("check_number = %d \n", check_number);
	
	if (check_number == -1) { 
	    error_id = 19;
	    PrintToLogCloseCheck(-1, error_id);
	    return 1;
	} 
	
	if (hw->ECR_GetProtocol()==4)  { 
	    if (check_number == 1) check_number = 2;
	} else	if (hw->ECR_GetProtocol()!=5) {
	    if (check_number == 1) check_number = 10000;
	}
	
	
	QString ss;
	QString s = "чек № "+ss.setNum(check_number-1);
	s = s +" закрыт";
	modeTextLabel->setText(DefCodec->toUnicode(s));
	PrintToLogCloseCheck(0, 0);
	
	if(closed_session_flag)    {
	    if(InitChangeTime() != 0)      {
		//Message(3);
		message_id = 3;
		//CloseRegistrationForm(false); 
	    }
	}
	
	if(warn_about_sync_need) message_id = 1;   //Message(1);
	kkm_reg->SetCreditStatus(-1);
    }
    
    //============ПЕЧАТЬ ЧЕКА ВОЗВРАТА===========================================
    if ( return_flag )   {
	puts("ПЕЧАТЬ ЧЕКА ВОЗВРАТА");
	return_flag = false;
	
	PrintToLogPosition(1);
	FillReceipt(1);
	
	kkm_res = hw->ECR_ReadStatus();
	if ( kkm_res != 0 )    {  CrashPrintSellCheck(kkm_res, true, false);   return -1;	}  
	 //-----(+)-----Zuskin-----25/01/2012-----
	if ( hw->ECR_GetProtocol() == 1 ) {
	    if ((hw->ECR_GetAdvancedMode() == 2) || (hw->ECR_GetAdvancedMode() == 3)) {
		// ШТРИХ ФР ожидает команду продолдения печати
		kkm_res = hw->ECR_ContinuePrinting();
		kkm_res = hw->ECR_ReadStatus();
		if ( kkm_res != 0 ){
		    CrashPrintSellCheck(kkm_res, true, false);
		    return -1;
		}
	    } 
	}
	//--------------------------------------------------
	if ( hw->ECR_GetCheckState() == 1 ) {
	    //если в ФР есть открытый документ, надо его сначала попытаться отменить
	    kkm_res = hw->ECR_CancelCheck();	
	   
	    if ( kkm_res != 0 )    {
    	        CrashPrintSellCheck(kkm_res, true, false);
	        error_id = 40;
	        return -1;
	    }  
	}
	//---------------------------------------------------	
	//получить сменный итог
	kkm_res = hw->ECR_ReadSessionSumm();
	if ( kkm_res != 0 )    {   CrashPrintSellCheck(kkm_res, true, false);    return -1;  }  
	
	double fr_sum = hw->ECR_GetSumm();

	if ( sp.sections.size() == 0 ) {
                // INPAS and SBER
	//-----(*)-----Mirabell-----21/02/2012-----
	int type_system = hw->PS_GetTypeSystem();
                switch(type_system){
                   case 2:{
                      kkm_res = kkm_reg->PrintCreditInfo(true);
                      break;
                   }
                   default:{
                      bool packet_answer = hw->PS_GetPacketTypeAnswer();
                      if (!packet_answer){
                         kkm_res = kkm_reg->PrintCreditInfo(false);
                         kkm_res = kkm_reg->PrintCreditInfo(true);
                      } else {
                         kkm_res = kkm_reg->PrintCreditInfo(true);
                      }
                      break;
                   }
                 }
               }
	//-----------------------------------------
	
	if ( sp.sections.size() == 0 ) {
	    int k = ReturnRegistration(-1, true);
	    if ( k != 0 )   {  
		CrashPrintSellCheck(k, false, true);
		return -1;
	    }
	     //-----(+)-----Zuskin-----25/01/2012-----
	    kkm_res = hw->ECR_ReadStatus();
	    if ( kkm_res != 0 ){
		 CrashPrintSellCheck(kkm_res, true, false);
		 return -1;
	    }
	     //-----(*)-----Zuskin-----27/01/2012-----
	    if (hw->ECR_GetProtocol() == 1) { // только для ШТРИХов
		while (hw->ECR_GetCheckState() == 1) {
		    printf("in RMK nalsum = [%f], credsum = [%f], contsum = [%f]\n", nalsum, credsum, contsum);
		    kkm_res = kkm_reg->KkmCloseCheck(nalsum, credsum, contsum, receipt);
		    if (kkm_res == 0) break;
		    //--------------------------------------------------
		    if ((kkm_res == 103) && (hw->ECR_GetProtocol() == 1))  {    
			//-----(+)-----Zuskin-----24/01/2012-----
			if (kkm_res == 103) { // обрыв чековой ленты
			    WaitProcessStop();
			    ContinuePrintingForm *cpf = new ContinuePrintingForm;
			    releaseKeyboard();
			    cpf->show();
			    
			    while ((kkm_res == 103) && (hw->ECR_GetProtocol() == 1)) { 
			    //для ШТРИХ в случае обрыва ленты надо (после вставки ленты) послать команду "продолжить печать"			
				waitmsec(500);
				if (cpf->OKpressed) {
				    kkm_res = hw->ECR_ContinuePrinting();
				}
				cpf->OKpressed = false;
			    }
			    delete cpf;	
			    grabKeyboard();
			    WaitProcessStart();
			    kkm_res = hw->ECR_ReadStatus();
			}
		    }
		    else {
			break;
		    }
		} //--- while ---
	    }
	    else {
		printf("in RMK nalsum = [%f], credsum = [%f], contsum = [%f]\n", nalsum, credsum, contsum);
		kkm_res = kkm_reg->KkmCloseCheck(nalsum, credsum, contsum, receipt);
	    }
	    //------------------------------------------------------	      
	    if ( kkm_res != 0 ) {
		if ( TestKkmCloseCheck(kkm_res, fr_sum) < 0 ) return -1;
	    }
	}
	
	int section_num_with_max_sum = receipt->GetSectionNumWithMaxSum();
	int count_printed_not_fiscal_section = 0;
	bool printed_total_sum_info = false;
	//-----(+)-----Zuskin-----24/01/2012-----
	bool fiscal_check_printed = false;
	//-------------------------------------------------
	for ( int section_num = 0; section_num < sp.sections.size(); section_num++ ) {
	    printf("Return print  section_i - %d count pos = %d \n", section_num, receipt->GetSectionCountPosition(section_num));
	    
	    if ( receipt->GetSectionCountPosition(section_num) > 0 ) {
		int k = ReturnRegistration(section_num, printed_total_sum_info);
		printed_total_sum_info = true;
		if ( k != 0 )   {  
		  //-----(*)-----Zuskin-----24/01/2012-----
		    //return -1;
		    if (!fiscal_check_printed) {
			return -1;
		    }
		    else {
			break;
		    }
		    //--------------------------------------------------
		}
		
		if (section_num == 0) {
		    //-----(+)-----Zuskin-----25/01/2012-----
		    kkm_res = hw->ECR_ReadStatus();
		    if ( kkm_res != 0 ){
			CrashPrintSellCheck(kkm_res, true, false);
			return -1;
		    }
		    //-----(*)-----Zuskin-----27/01/2012-----
		    if (hw->ECR_GetProtocol() == 1) { // только для ШТРИХов
			while (hw->ECR_GetCheckState() == 1) {
			 //--------------------------------------------------
			    kkm_res = kkm_reg->KkmCloseCheck(receipt->GetSectionResultSum(0), 0, 0, receipt);
			    if ( hw->ECR_GetProtocol() == 1 ) {
				int amode = 5;
				while (amode == 5) {
				    hw->ECR_ReadStatus();
				    amode = hw->ECR_GetAdvancedMode();	
				}
			    }	
			    if (kkm_res == 0) break;
			    if ((kkm_res == 103) && (hw->ECR_GetProtocol() == 1))  {    
				//-----(+)-----Zuskin-----24/01/2012-----
				if (kkm_res == 103) { // обрыв чековой ленты
				    WaitProcessStop();
				    ContinuePrintingForm *cpf = new ContinuePrintingForm;
				    releaseKeyboard();
				    cpf->show();
			    
				    while ((kkm_res == 103) && (hw->ECR_GetProtocol() == 1)) { 
				    //для ШТРИХ в случае обрыва ленты надо (после вставки ленты) послать команду "продолжить печать"			
					waitmsec(500);
					if (cpf->OKpressed) {
					    kkm_res = hw->ECR_ContinuePrinting();
					}
					cpf->OKpressed = false;
				    }	
				    delete cpf;	
				    grabKeyboard();
				    WaitProcessStart();
				    kkm_res = hw->ECR_ReadStatus();
				}
			    }
			    else {
				break;
			    }
			} //--- while ---
		    }
		    else { // --- для протоколов кроме ШТРИХ
			kkm_res = kkm_reg->KkmCloseCheck(receipt->GetSectionResultSum(0), 0, 0, receipt);			    }
		    if ( kkm_res != 0 ) {
			if ( TestKkmCloseCheck(kkm_res, fr_sum) < 0 ) return -1;
		    }  
		    //-----(+)-----Zuskin-----24/01/2012-----
		    fiscal_check_printed = true;
		    //-------------------------------------------------
		} else 
		    count_printed_not_fiscal_section++;
	    } 
	}
	
	if (count_printed_not_fiscal_section > 0) {
	    // принудительная печать клише чека после печати всех нефискальных квитанций
	    kkm_res = kkm_reg->KkmPrintDocHeadLine();
	    if ( kkm_res != 0 ) {
		if ( TestKkmCloseCheck(kkm_res, fr_sum) < 0 ) return -1;
	    }
	}
	
	/********************************************************************************/
	
	check_print_error = false;
	//-----(*)-----Zuskin-----25/07/2011-----
	//if ( ins_tr->InsertCheckRegistrationTransactions("1", "", nalsum, credsum, 0, closed_session_flag) != 1 )  {
	//-----(*)-----Zuskin-----18/11/2011-----
	//if ( ins_tr->InsertCheckRegistrationTransactions("1", "", nalsum, credsum, contsum, closed_session_flag) != 1 )  {
	if ( ins_tr->InsertCheckRegistrationTransactions("1", "", nalsum, credsum, contsum, closed_session_flag, true, string(manager_id.ascii())) != 1 )  {
	//---------------------------------------
	    error_id = 21;
	    CrashPrintSellCheck(21, false, true);
	    return -1;
	}
	
	if (sp.return_count_inc)  Reg.Update_Goods_Quantity(receipt);
	
	check_closed = true;  
	receipt->CloseReceipt();
	pause_work_timer->start(PAUSE_TIME, true);
	StateStore::GetInstance()->SetReceiptState(FREE);
	
	int check_number = 0;
	switch(hw->ECR_GetProtocol()){	 
	case 3:{
			  int r = hw->ECR_ReadCheckNumber();
			  if ( r != 0 ) check_number = -1;
			  else check_number =  hw->ECR_GetCheckNumber();
			  if (check_number == 1) check_number = 10000;
			  break;
		 }
	case 4:{
			  int r = hw->ECR_ReadDocumentNumber(2);
			  if ( r != 0 ) check_number = -1;
			  else check_number =  hw->ECR_GetDocumentNumber();
			  if (check_number == 1) check_number = 10000;
			  break;
		 }
	case 5:{
			  check_number =  hw->ECR_GetCheckNumber()+1;
			  break;
		 }
	default:{
			  int r = hw->ECR_ReadDocumentNumber();
			  if ( r != 0 ) check_number = -1;
			  else check_number =  hw->ECR_GetDocumentNumber();
			  if (check_number == 1) check_number = 10000;
		 }
	}
	// printf("check_number  = %d\n",check_number);
	
	if ( check_number == -1 ) {  
	    error_id = 19;
	    PrintToLogCloseCheck(-1, 19);
	    clearCopyInfo();
	    return 1;  
	}  

	
	PrintToLogCloseCheck(0, 0);
	
	clearCopyInfo();
	
	QString ss;
	QString s = "чек №"+ss.setNum(check_number-1);
	
	s = s +" закрыт";
	modeTextLabel->setText(DefCodec->toUnicode(s));
	str = "";
	textTextLabel->setText("");
	
	if ( closed_session_flag )    {
	    if ( InitChangeTime() != 0 )      {
		message_id = 3;
	    }
	}	
	if ( warn_about_sync_need ) message_id = 1;
	
	ReturnNumberCheck = -1;
	ReturnPayType = -1;
	kkm_reg->SetCreditStatus(-1);
	
    }
    
    if ( copy_flag ) {
//	puts("PrintCopyCheck()");
	copy_flag = false;
	
	kkm_res = hw->ECR_ReadStatus();
	if ( kkm_res != 0 )    {    kkmerror_id = kkm_res;	    return -1;	}  
	
	if ( hw->ECR_GetCheckState() == 1 ) {
	    kkm_res = hw->ECR_CancelCheck();	
	    if ( kkm_res != 0 )    {	      kkmerror_id = kkm_res;	      return -1;	    }  
                }
	
	/*********************************************************************/
	if ( sp.sections.size() == 0 ) {
	    //?????????
	    if ( check_print_error )  kkm_res = hw->ECR_PrintDocHeadLine();
	    
	    kkm_res = kkm_reg->KkmPrintCopyCheck(copy, receipt);
	    if ( kkm_res != 0 ) {     kkmerror_id = kkm_res;	    return -1;      	}    
	}
	
	if ( sp.sections.size() > 0 ) {
	    kkm_res = kkm_reg->PrintCenterString(HwCodec->fromUnicode(DefCodec->toUnicode("КОПИЯ ЧЕКА №")) + copy->GetCheckNum() );
	    if (kkm_res!=0){    kkmerror_id = kkm_res;    return kkm_res;}	 
	    
	    kkm_res = kkm_reg->KkmPrintTotalSumBySection(receipt, copy->GetNalSum(), copy->GetCredSum(), 0);
	    if (kkm_res!=0){    kkmerror_id = kkm_res;    return kkm_res;}	 
	}
	
	int section_num_with_max_sum = receipt->GetSectionNumWithMaxSum();
	int count_printed_not_fiscal_section = 0;
	for ( int section_num = 0; section_num < sp.sections.size(); section_num++ ) {
	    printf("Print  section_i - %d\n", section_num);
	    int count_section_position = receipt->GetSectionCountPosition(section_num);
	    printf("count_section_position = %d \n", count_section_position);
	    if ( count_section_position > 0 ) {
		kkm_res = kkm_reg->KkmPrintCopyCheck(copy, receipt, section_num);
		if ( kkm_res != 0 ) {     kkmerror_id = kkm_res;	    return -1;      	}    
		
		if ( section_num > 0 ) count_printed_not_fiscal_section++;
	    } 
	}
	
	if (count_printed_not_fiscal_section > 0) {
	    // принудительная печать клише чека после печати всех нефискальных квитанций
	    kkm_res = kkm_reg->KkmPrintDocHeadLine();
	    if ( kkm_res != 0 )    {	      kkmerror_id = kkm_res;	      return -1;	    }  
	}
	
	puts("END OF PRINT COPY CHECK");
	/************************************************************************/
	
	check_closed = true;
	check_print_error = false;
	
	receipt->CloseReceipt();
	
	StateStore::GetInstance()->SetReceiptState(FREE);
	str = "";
	// а тут копия чека чиститься
	copy->Clear_CopyInfo();
	receipt->setCashier(Authorization::GetInstance()->GetUserShortcut());
    }

    WaitProcessStop();
    return 0;
}
//======================================================================//
// true - текущее время входит в диапазон ограничения
bool RegistrationForm::isSaleRistrictionByTime() {
    // сработало врем ограничение и имеются позиции которые надо ограничить
    //---(-)---Zuskin---24/02/2012---
    /*if ( Reg.getSaleAccessByTime() )
	return false; */
    //-----------------------------------------
    
    for (int i = 0; i < receipt->record_num; i++) {
	//---(*)---Zuskin---24/02/2012---
	//if ( receipt->GetPosElemRef(i)->enable_flag && receipt->GetPosElemRef(i)->sale_limit_by_time ) {
	if ( receipt->GetPosElemRef(i)->enable_flag && receipt->GetPosElemRef(i)->sale_limit_by_time && Reg.getSaleAccessByTime(receipt->GetPosElemRef(i)->GetCode()) == false) {
	    return true;
	}
    }  
    return false;
}
//======================================================================//
void RegistrationForm::DeletePositionByTimeRistriction() {
    //printf("DeletePositionByTimeRistriction() \n");
    
    keyboard_block = false;    
    if ( (kkm_reg->GetCreditStatus() != 1) && (kkm_reg->GetCreditStatus() != 17) ) {
	NPAS_CrushTransaction();
    }
    
    ReleaseKeyboard();
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
    DeletePositionByTimeRistrictionForm f;
    f.setReceipt(receipt);
    f.exec();
    
    for (int i = 0; i < receipt->record_num; i++) {
	//---(*)---Zuskin---24/02/2012---
	//if ( receipt->GetPosElemRef(i)->enable_flag && receipt->GetPosElemRef(i)->sale_limit_by_time ) {
	if ( receipt->GetPosElemRef(i)->enable_flag && receipt->GetPosElemRef(i)->sale_limit_by_time && Reg.getSaleAccessByTime(receipt->GetPosElemRef(i)->GetCode()) == false ) {
	//-----------------------------------------
	    receipt->GetPosElemRef(i)->enable_flag = false;
	    showDeletedPosition(i);
	}
    }
    
    //-----(+)-----Zuskin-----24/02/2012-----
    QString s;
    receipt->RecalculationDiscounts();
    s.setNum(receipt->GetAllDiscountSum(), 'f', 2);
    redallTextLabel->setText(s);
    
    s.setNum(receipt->GetResultSum(), 'f', 2);
    allTextLabel->setText(s);
    //-------------------------------------------------   
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();
    
}
//======================================================================//
void RegistrationForm::CheckFlag()
{
   //puts("void RegistrationForm::CheckFlag()");
    //---(+)---Zuskin---15/02/2012---
    InitPkCurrentTime();
    //-----------------------------------------
    // cancel_flag = true, заполняется при неудаче с регистрацией продаж и возвратов, для последующей отмены чека.
    if ( hide_mouse )    {
	grabMouse();
	hide_mouse = false;
    }
    //printf("cancel_flag = %d\n",cancel_flag);
    //printf("cancel_flag = %d, %d\n",cancel_flag,&cancel_flag);
    // выполняется отмена чека, если чек открыт.
    if ( cancel_flag )  {
	puts("ОТМЕНА ЧЕКА");
	nalsum = 0;
	credsum = 0;
	contsum = 0;
	receipt->setCashier(Authorization::GetInstance()->GetUserShortcut());
	int res = hw->ECR_ReadStatus();
	if (res == 0)  {
	    if (hw->ECR_GetCheckState() == 0)
		cancel_flag = false;
	    else{
		kkm_res = hw->ECR_CancelCheck();
		//printf("Kkm_res = %d\n",kkm_res);
	    }
	    // kkm_res=154 - при возникновении ошибки чек начал печататься, но не был открыт
	    if((kkm_res == 0) || (kkm_res == 154)) 	   
		cancel_flag = false;
	}
	//-----(+)-----Zuskin-----11/11/11-----
	if (res == -1) { // связи с ФР нет 
	    cancel_flag = false;
	}
	//----------------------------------------------
	
    }
    
    if ( payment_flag ) {
	if ( isSaleRistrictionByTime() ) {
	    payment_flag = false;
	    DeletePositionByTimeRistriction();
	}
    }
    
    if ( payment_flag || return_flag || copy_flag ) {
	
	//-----(*)-----Zuskin-----11/11/11-----
	//sleep(1);
	    waitmsec(210); // зачем вообще здесь нужна задержка - не совсем понятно
	//-------------------------------------------------
	PrintProcessStart();
    }
    
    if ( ! barcode_from_scaner.isEmpty() )   {
	//-----(+)-----Zuskin-----05/12/2011-----
	if (hw->ECR_IsPortBuzy() == false ) {
	//-------------------------------------------------    
	    QString barcode = barcode_from_scaner;
	    barcode_from_scaner = "";
	    BarcodeEvent( barcode, false );
	}
	else {
	    puts("Идет работа с ФР, подождем пока порт освободится...");
	}
    }
        
    //printf("kkmerror_id = %d \n", kkmerror_id);
    if ( kkmerror_id != 0 ) {  int k = kkmerror_id; kkmerror_id = 0;  Kkm_Error(k);     }
    if ( error_id != 0  ) { int k = error_id; error_id = 0;  Error(k);  }
    if ( message_id != 0 ) { int k = message_id;  message_id = 0;   Message(k);   }
    if ( message_id == 3 ) CloseRegistrationForm(false);
    
    
    if ( check_closed ) {
	ClearForNewOperation();   
	if ( ! sp.clear_window_after_close_check )   StateStore::GetInstance()->SetViewState(FILL);
    }
    
    //printf("kkmerror_id = %d \n error_id = %d \n message_id = %d \n", kkmerror_id, error_id, message_id);
    //применимо так как, если sp.autoblock_flag = false, то дальнейшего вычисления выражения не происходит
    if ( sp.autoblock_flag && bt->block_flag )   {
	bt->block_flag = false;
	Block(true);
    }
}


//#################################--NOT--USE--###############################################//
//======================================================================//
void RegistrationForm::DumpBalance() /*HOME*/ //Ноль
{
    // puts("RegistrationForm::DumpBalance()");
    /*  if(hw->EWType != 0)
  {
    statusTextLabel->setText(DefCodec->toUnicode("Сброс весов в ноль"));
    hw->ew.set_zero();
  }*/
}

//======================================================================//
void RegistrationForm::ClearWeightConteiner() /* END */ //Тара
{
    // puts("RegistrationForm::ClearWeightConteiner()");
    /* if(hw->EWType != 0)
  {
    statusTextLabel->setText(DefCodec->toUnicode("Сброс веса тары"));
    hw->ew.set_tare();
  }*/
}

//======================================================================//
void RegistrationForm::Calculator() /* CTRL + END */ //Калькулятор 
{
    //puts("RegistrationForm::Calculator()");
    /*потом*/
}

//======================================================================//
void RegistrationForm::Section() /*F4*/ //Секция
{
    //puts("RegistrationForm::Section()");
}

//======================================================================//
void RegistrationForm::ViewingGoods()/*F9*/ //Просмотр товара
{
    //puts("RegistrationForm::ViewingGoods()");
}

//======================================================================//
void RegistrationForm::Cancellation() /* SHIFT + DELETE */ //Аннулирование
{
    //puts("RegistrationForm::Cancellation()"); 
}

//======================================================================//
void RegistrationForm::SelectionChangedHandler()
{
    QTableSelection sel = receiptTable -> selection(0);
    int p_sel = sel.topRow();
    
    if (p_sel != -1)
    {	
	receiptTable->setCurrentCell(p_sel,0);
	receiptTable->selectRow(p_sel);
	
	PositionPreference(p_sel);   
    }
}


//======================================================================//
int RegistrationForm::Read_N_PC()
{
    //puts("RegistrationForm::Read_N_PC()");
    
    char * pc_nmb = NULL;
    ReaderConf * rc;
    
    rc = new ReaderConf(N_PC_PATH);
    int len = rc->getLen("[N_PC]");
    if (len == 0)
    {
	delete rc; 
	return 1;
    }
    else
    {
	pc_nmb = (char*) calloc(len+1, sizeof(char));
	rc->GetNPC(pc_nmb);
	strrs(pc_nmb);
    }
    delete rc;
    //-----(*)-----Zuskin-----24.01.2012-----
    //return atoi(pc_nmb);
    int jj = atoi(pc_nmb);
    free(pc_nmb);
    return jj;
    //-------------------------------------------------
}

//======================================================================//
void RegistrationForm::adjustFont()
{ 	
    g = textTextLabel->font();    
    QFont f = QFont(g);
    int L_W = textTextLabel->width();
    int F_S = DefaultFontSize;
    f.setPointSize(F_S);
    QFontMetrics fm( f );
    int P_W = fm.width( str );
    double koef1= (double)P_W/(double)(L_W-40);
    if (koef1>1.000)
    {	
	F_S =(int)F_S/koef1;
    }
    
    f.setPointSize(F_S);
    textTextLabel->setFont(f);
}

//======================================================================//
// начисление фиксированной скидки на чек, и  только в состоянии промитога
void RegistrationForm::CardEvent(QString card_code_str )
{
    puts("**********************CARD**EVENT************************");
    
    if ( HwSignalHandlersController::GetInstance()->GetControlledObject() != REGISTRATION ) {
	return;
    }
    
    //-----(*)-----Zuskin-----30/09/2011-----
   /* if  ( StateStore::GetInstance()->GetViewState() != INS_TEXT && StateStore::GetInstance()->GetViewState() != INS && StateStore::GetInstance()->GetViewState() != REC)    {
	Error(91);      
	return;  
    } */
    if ( StateStore::GetInstance()->GetReceiptState() == FREE ) {
	Error(14);
	return;
    }
    if ( StateStore::GetInstance()->GetReceiptState() == COPY ) {
	Error(36);
	return;
    }
        
    // ищем скидку :
    int type;
    char* code;
    double value;
    bool area;
    char* name;
    char* shortcut;
    char* barcode;
    
    //---(+)---Zuskin---17/02/2012---
    // если card_code_str = пустая строка, тогда можно вызвать окно для ввода кода карты (нужно для ридеров Posiflex)
    if (card_code_str == "") {
	
    }
    //-----------------------------------------
    //GetDiscountBarcodeInfo(const char* barcode, int &code, char* name, char* shourtcut, int &value, int &type, bool &area, char* card_code)
    int state =Reg.GetDiscountCardcodeInfo(card_code_str.ascii(), code, name, shortcut, value, type, area,  barcode);	
    //  printf("state of get discount info by barcode = %d \n", state);
    
    if ( state == -1 ) Error(7);		// ошибка бд    
    if ( state == 1 )  Error(92);  	//   скидка отсутсвует
    
    if  ( state == 0 ) { 
	// добавить скидку
	if ( type != -1 )        {
	    if (type == 0 || type == 2)     CheckReduction(true, 1, value, code, name, shortcut, false);
	    //-----(*)-----Zuskin-----26/07/2011-----		
	    //else    state =  CheckReduction(false, 1, value, code, name, shortcut, false);
	    else    {
		if (type == 4 ) { // bonuscard
		    state =  CheckReduction(false, 4, value, code, name, shortcut, false);
		}
		else {
		    state =  CheckReduction(false, 1, value, code, name, shortcut, false);
		}	
	    }    
	    //---------------------------------------	    
	    receipt->GetDiscountElemRef(1)->Set_Internal_Code(code);
	    receipt->GetDiscountElemRef(1)->Set_Card_Code(card_code_str.ascii()); 
	    
	    QString s;
	    //s.setNum(receipt->Sum() - receipt->RecalculationDiscounts(), 'f', 2);
	    s.setNum(receipt->GetResultSum(), 'f', 2);
	    allTextLabel->setText(s);
	    //-----(+)-----Zuskin-----30/09/2011-----
	    //StateStore::GetInstance()->SetViewState(REC);
	    //-------------------------------------------------
	}
	free(code);
	free(name);
	free(shortcut);
	free(barcode);    
    }
}
//======================================================================//
//-----(*)-----Zuskin-----03/08/2011-----
int RegistrationForm::AddWareByBarcode(QString barcode, QString code, bool handle, double price)
//int RegistrationForm::AddWareByBarcode(QString barcode, QString code, bool handle, double price = -1)
//-------------------------------------------------	
{
    //printf("AddWare(QString barcode = %s, QString code = %s, bool handle = %d) \n", barcode.ascii(), code.ascii(), handle);
    
    //printf("code = %s \n", code.ascii());
    QString code_db = DbCodec->fromUnicode(code);
    //printf("DbCodec->fromUnicode(str) = %s \n", code_db.ascii());    
    //-----(+)-----Zuskin-----13/10/2011-----
    bool qttyset = true;
    if ( quantTextLabel->text().length() == 0 || quantTextLabel->text().length() == 22) 
	qttyset = false;
    //---------------------------------------
    if ( handle && !sp.start_vs_manualinput ) {
        //-----(*)-----Zuskin-----13/10/2011-----
	//if ( barcode.isEmpty() )  return AddWareController::GetInstance()->AddWare(code_db.ascii(), false, 0);  
	if ( barcode.isEmpty() )  return AddWareController::GetInstance()->AddWare(code_db.ascii(), false, 0, "", false, qttyset);  
	//---------------------------------------
	else {
	    // выкл настройка, регистрация по шк
	    int count = Reg.GetCountWareByBarcode(barcode.ascii(), code.ascii());
	    //-----(*)-----Zuskin-----13/10/2011-----
	    //if ( count == 1 ) return AddWareController::GetInstance()->AddWare(barcode.ascii(), true, 1); 
	    if ( count == 1 ) return AddWareController::GetInstance()->AddWare(barcode.ascii(), true, 1, "", false, qttyset); 
	    //---------------------------------------
	    if ( count == 0 ) {	
		// попробовать весовой штрих- код 
		//-----(*)-----Zuskin-----13/10/2011-----
		//int result = AddWareController::GetInstance()->AddWare(barcode.ascii(), true, 1);
		int result = AddWareController::GetInstance()->AddWare(barcode.ascii(), true, 1, "", false, qttyset);
		//---------------------------------------
		if ( result == 0 ) return 0;
		else {	return -1;     }
	    }	    
	    if ( count < 0 ) { Error(7); return -1; }
	}
    }
    
    if ( !handle && !barcode.isEmpty() ) {
	int count = Reg.GetCountWareByBarcode(barcode.ascii(), code.ascii());
	//-----(*)-----Zuskin-----13/10/2011-----
	//if ( count == 1 ) return AddWareController::GetInstance()->AddWare(barcode.ascii(), true, 1); 
	if ( count == 1 ) return AddWareController::GetInstance()->AddWare(barcode.ascii(), true, 1, "", qttyset); 
	//---------------------------------------
	if ( count == 0 ) {
	    // попробовать весовой штрих- код 
	    //-----(*)-----Zuskin-----13/10/2011-----
	    //int result = AddWareController::GetInstance()->AddWare(barcode.ascii(), true, 1);
	    int result = AddWareController::GetInstance()->AddWare(barcode.ascii(), true, 1, "", false, qttyset);
	    //---------------------------------------
	    if ( result == 0 ) return 0;
	    else { return -1;     }
	}
	if ( count < 0 ) { Error(7); return -1;  }
    }
    
    //AddWareController::GetInstance()->AddWare(barcode.ascii(), true, 1);
    //если по шк посмотреть сколько таких штук точно совпадающих с шк, если > 1 то выдавать вп список

    
    
    ReleaseKeyboard();
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
    visualForm *vs = new visualForm;
    
    if ( StateStore::GetInstance()->GetReceiptState() == RETURN ) vs->check_type = 1;
    vs->FillTable("0");
    if (barcode.isEmpty()) vs->setCode(code);
    else vs->setBarcode(barcode);
    //-----(+)-----Zuskin-----03/08/2011-----
    if (price != -1) {
        vs->setPrice(price);
    }
    //--------------------------------------------------
    //-----(+)-----Zuskin-----14/10/2011-----
    if ( quantTextLabel->text().length() != 0 && quantTextLabel->text().length() != 22) {
	vs->vsQttyPreSet = true;
    }
    //-------------------------------------------------
    QDesktopWidget *d = QApplication::desktop();
    if (d->width() <= 800) vs->setWindowState(Qt::WindowFullScreen);
    
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) disconnect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence()) );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) disconnect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    
    notaddbyvisual = false;
    vs->exec();
    notaddbyvisual = true;
    
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence()) );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    
    delete(vs);	
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();
    return 0;
    
}
//======================================================================//
void RegistrationForm::BarcodeEvent(QString barcode_str, bool handle)
{
    puts("**********************BARCODE EVENT**************************");
        
    /*if ( StateStore::GetInstance()->GetViewState() != INS_TEXT && StateStore::GetInstance()->GetViewState() != INS && StateStore::GetInstance()->GetViewState() != REC )     {
	//-----(*)-----Zuskin-----30/09/2011-----
	//-----(*)-----Zuskin-----03/08/2011-----
	//AddWareByBarcode(barcode_str, "", handle);
	//AddWareByBarcode(barcode_str, "", handle, -1);
	int tres = AddWareByBarcode(barcode_str, "", handle, -1);
	//--------------------------------------------------
	//AddWareController::GetInstance()->AddWare(barcode_str.ascii(), true, 1);
	//printf("@@@@@@@@@@@@@@@@@@@@@@tres = %d\n", tres);
	if (tres == 0) 
	    return;
	//--------------------------------------------------
    }*/
    
    //puts("prom itog");
    //режим пром итога или начисленной скидки на чек
    // ищем скидку
    
    //-----(*)-----Zuskin-----30/09/2011-----
    if (( StateStore::GetInstance()->GetReceiptState() != FREE ) && (StateStore::GetInstance()->GetReceiptState() != COPY)) {
	int type;
	char* code = NULL;
	double value;
	bool area;
	char* name = NULL;
	char* shortcut = NULL;
	char* card_code = NULL;
      /*puts("in rmk");
    printf("StateStore::GetInstance()->GetViewState() = %d \n", StateStore::GetInstance()->GetViewState());	
    printf("StateStore::GetInstance()->GetSessionState() = %d \n", StateStore::GetInstance()->GetSessionState());
    printf("StateStore::GetInstance()->GetReceiptState() = %d \n", StateStore::GetInstance()->GetReceiptState());*/
    //printf("StateStore::GetInstance()->GetViewState() == %d \n", StateStore::GetInstance()->GetViewState());
	int state =Reg.GetDiscountBarcodeInfo(barcode_str.ascii(), code, name, shortcut, value, type, area, card_code );
    
	if ( state == -1 ) {   Error(7);  return; } 		// ошибка бд    
    
	// добавить скидку
	if ( state == 0 ) {
	    if(type != -1)        {
		if (type == 0 || type == 2)    { 
		    state = CheckReduction(true, 1, value, code, name, shortcut, false);
		}
		//-----(*)-----Zuskin-----26/07/2011-----		
		//else    state =  CheckReduction(false, 1, value, code, name, shortcut, false);
		else    {
		    if (type == 4 ) { // bonuscard
			state =  CheckReduction(false, 4, value, code, name, shortcut, false);
		    }	
		    else {
			state =  CheckReduction(false, 1, value, code, name, shortcut, false);
		    }	
		}
		printf("~~~~~~~~~~state = %d\n", state);
		//---------------------------------------------------
		//назначить внутрений код карты и штрих - код
		if (state == 0) {
		    receipt->GetDiscountElemRef(1)->Set_Internal_Code(code);
		    receipt->GetDiscountElemRef(1)->Set_Card_Code(barcode_str.ascii()); 
	                    QString s;
		    s.setNum(receipt->GetResultSum(), 'f', 2);
		    allTextLabel->setText(s);
		    textTextLabel->setText("");
		}    
	    }
	
	    if ( code != NULL ) free(code);
	    if ( name != NULL ) free(name);
	    if ( shortcut != NULL )  free(shortcut);
	    if ( card_code != NULL ) free(card_code);    
	}
	 int res = 0;
        
	 if ( state != 1 ) return;
	 /*
	 if (( StateStore::GetInstance()->GetViewState() == INS_TEXT ) || (StateStore::GetInstance()->GetViewState() == INS)){
	     //-----(*)-----Zuskin-----03/08/2011-----
	     //res = AddWareByBarcode(barcode_str, "", handle);
	     res = AddWareByBarcode(barcode_str, "", handle, -1);
	     printf("&&&&&&&&&&& res of 2nd AddWareByBarcode() = %d\n", res);
	     //-------------------------------------------------
	     if ( res == -3 ) { Error(45);  return; }
	     if ( res == 3 || res == 12 || res == -1 ) { Error(3); return; }
	     return;
	 } else {
	     Error(101);
	     return;
	 } 
	*/
    }
    
   if ( StateStore::GetInstance()->GetViewState() != INS_TEXT && StateStore::GetInstance()->GetViewState() != INS && StateStore::GetInstance()->GetViewState() != REC )     {
	//-----(*)-----Zuskin-----30/09/2011-----
	//-----(*)-----Zuskin-----03/08/2011-----
	//AddWareByBarcode(barcode_str, "", handle);
	//AddWareByBarcode(barcode_str, "", handle, -1);
	int tres = AddWareByBarcode(barcode_str, "", handle, -1);
	//--------------------------------------------------
	//AddWareController::GetInstance()->AddWare(barcode_str.ascii(), true, 1);
	//printf("@@@@@@@@@@@@@@@@@@@@@@tres = %d\n", tres);
	if (tres == 0) 
	    return;
	//--------------------------------------------------
    }  
    
   return;
   //-----------------30/09/2011------------------- 
}		

//======================================================================//
void RegistrationForm::PrintReceipt() 
{
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED ) {   Error(74);  return;     }
    if ( StateStore::GetInstance()->GetReceiptState() == FREE )  {    Error(14);    return;    }    
    
    PrintDocumentListForm pdl;
    int count_doc = pdl.GetDocumentInfo();
    if ( count_doc <= 0 ) {	Error(105);	return;    }
    if ( count_doc == 1 ) {  PrintReceiptDocument(receipt, pdl.GetPath());   return;   }
    
    if ( pdl.exec() == QDialog::Rejected ) return;
    PrintReceiptDocument(receipt, pdl.GetPath());
}
//======================================================================//
void RegistrationForm::RegByName(QString title)
{
    
    if ( StateStore::GetInstance()->GetViewState() == FILL )    ClearForNewOperation();
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED ) {   Error(74);  return;  }
    if ( StateStore::GetInstance()->GetReceiptState() == COPY )  {    Error(33);  return;   }
    
    //AddWareController::GetInstance()->AddWare(str.ascii(), false, 0);
    
    ReleaseKeyboard();
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    //printf("create vs \n");
    
    visualForm *vs = new visualForm;
    //printf("vs->check_type = %d \n", vs->check_type);
    if ( StateStore::GetInstance()->GetReceiptState() == RETURN ) vs->check_type = 1;
    //printf("vs->check_type = %d \n", vs->check_type);
    vs->FillTable("0");
    vs->setWareTitle(title);
    //-----(+)-----Zuskin-----14/10/2011-----
    if ( quantTextLabel->text().length() != 0 && quantTextLabel->text().length() != 22) {
	vs->vsQttyPreSet = true;
    }
    //-------------------------------------------------
    QDesktopWidget *d = QApplication::desktop();
    if (d->width() <= 800) vs->setWindowState(Qt::WindowFullScreen);
    
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) disconnect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence()) );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) disconnect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    
    notaddbyvisual = false;
    vs->exec();
    notaddbyvisual = true;
    
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence()) );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    
    delete(vs);
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();    

}
//======================================================================//
void RegistrationForm::PrintToLogPosition(int type) {
    
    cout << "PrintToLogPosition()" << endl;
    
    ofstream tmpfile(RMK_GOODS_LOG_PATH, fstream::app);
    
    if ( ! tmpfile.is_open() ) return ;
    
    tmpfile << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") << endl;
    if (type == 0) tmpfile << "открыт чек #" << receipt->GetCheckNumber() << endl;
    else tmpfile << "открыт чек возврата #" << receipt->GetCheckNumber() << endl;
    
    for ( int i = 0; i < receipt->record_num; i++ )  {   
	if ( receipt->GetPosElemRef(i)->enable_flag )    { 
	    tmpfile << receipt->GetPosElemRef(i)->GetCode() << " " << receipt->GetPosElemRef(i)->quantity << endl;
	}
    }
    
    tmpfile.close();   
}
//======================================================================//
void RegistrationForm::PrintToLogCloseCheck(int res, int err_id) {
    
    ofstream tmpfile(RMK_GOODS_LOG_PATH, fstream::app);
    if ( ! tmpfile.is_open() ) return ;
    
    if ( res == 0 ) tmpfile << "чек #" << receipt->GetCheckNumber() << " успешно закрыт" <<endl;
    else tmpfile << "ошибка при печати чека. код ошибки - " << err_id << endl;
    
    tmpfile.close();
}
//======================================================================//
void RegistrationForm::SelfScalingNameposTaxLabel(const QString& name_pos_str) {
    //printf("void RegistrationForm::SelfScalingNameposTaxLabel(const QString& name_pos_str) ");
    
    // вернуть исходный размер штрифта
    QFont f = nameposTextLabel->font();
    int label_width = nameposTextLabel->width();
    //printf("DefaultFSNamePos = %d \n", DefaultFSNamePos);
    f.setPointSize(DefaultFSNamePos);
    
    
    // если название переноситсЯ на две строки уменьшить штрифт на несколько пунктов
    // если (с уже уменьшиным шрифтом) пол строки все равно меньше ширины поля уменьшать штрифт пока не впишется !!!
    
    // померить вписывается ли в одну строку 
    QFontMetrics metric1 (f);
    int name1_width = metric1.width(name_pos_str);
    
    //printf("label_width = %d \n", label_width);
    //printf("name1_width = %d \n", name1_width);
    
    if ( name1_width > label_width )
	f.setPointSize(DefaultFSNamePos - 3);
    
    
    //QFontMetrics half_name_metric2(f);
    //QString half_name_str = name_pos_str.right(((int)(name_pos_str.length()/2)));
    //int half_name_width = half_name_metric2.width( half_name_str );
    int half_name_width = name1_width / 2;
    //printf("half_name_width = %d \n", half_name_width);
    
    double koeff = ((double)half_name_width)/((double)label_width-40.00);
    //cout << "koeff = half_name_w / label_w-40 =  " << koeff << endl;
    
    int font_size = f.pointSize();
    //printf("font_size = %d \n", font_size);
    if ( koeff > 1 )  
	font_size = (int) (font_size/koeff);
    
    f.setPointSize(font_size);
    
    nameposTextLabel->setFont(f);
}
//======================================================================//

void RegistrationForm::initSection()
{

}

//-----(+)-----Zuskin-----01/08/2011-----
void RegistrationForm::ShowEDSForm()
{
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED ) {   Error(74);  return;	  }
    if ( StateStore::GetInstance()->GetReceiptState() == FREE )  {    Error(14);    return;     }        // чек не открыт
    if ( StateStore::GetInstance()->GetReceiptState() == COPY )  {   Error(36);   return;     }                            // копия чека
    
    if (hw->EDS_GetType() != 1) {
	Error(114);
	return;
    }
        
    ReleaseKeyboard(); 
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
    // заполним параметры формы
    EDSForm *eds = new EDSForm;
        	        	    
    int k = eds->exec();
   
    if ( k == QDialog::Accepted ) {
	QString CardCode = eds->CardCode;
	if (CardCode.length() > 0) {
	    QString ClientName = eds->ClientName;
	    QString Discount = eds->Discount;
	    QString Disc_Id = eds->Disc_Id;
	    CheckReduction(true, 3, fromString<double>(Discount.ascii()), Disc_Id.ascii(), ClientName.ascii(), "DiscountMobile", false);
	    //назначить внутрений код карты и штрих - код
	    receipt->GetDiscountElemRef(3)->Set_Internal_Code(Disc_Id.ascii());
	    receipt->GetDiscountElemRef(3)->Set_Card_Code(CardCode.ascii()); 
	    receipt->GetDiscountElemRef(3)->Set_Type(3); 
	    
	    QString s;
	    //s.setNum(receipt->Sum() - receipt->RecalculationDiscounts(), 'f', 2);
	    s.setNum(receipt->GetResultSum(), 'f', 2);
	    allTextLabel->setText(s);
	    textTextLabel->setText(""); 
	    	    
	}
    }
    delete(eds);
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();	
}
//--------------------------------------------------

//-----(+)-----Zuskin-----03/08/2011-----
void RegistrationForm::RegisterByPrice() 
{
    // регистрация товара по введенной цене
    QString rrr = str;
    str = "";    
    bool nf;
    double p = rrr.toDouble(&nf);
    if  ( (rrr == "") || (nf == false) || (p < 0.01) || (p > 99999999.99) )   {
	Error(16);   
	if ( authorization_request_flag ) SwapToAuthorizedUser();   
	return; 
    }
    int count = Reg.Query_Cnt_By_Price(p);
    if ( count == 1 ) {
    // такой товар всего один
	int res = Reg.VisualSearchByAll( "", "", "", false, p ); 
	if (res == 1) {
	    VisualSearchElement product;
	    product = Reg.GetProductInfo(0, 1);
	    if ( product.is_addition ) {
	        printf("before AddWare addition_id = [%s]\n", product.addition_id);
	        //-----(+*)-----Zuskin-----13/10/2011-----
		bool qttyset = true;
		if ( quantTextLabel->text().length() == 0 || quantTextLabel->text().length() == 22) 
		qttyset = false;
    		//AddWareController::GetInstance()->AddWare(product.internal_code, false, 2, product.addition_id);
    		AddWareController::GetInstance()->AddWare(product.internal_code, false, 2, product.addition_id, false, qttyset);
		//---------------------------------------
		return;
	    }
	    else {
		AddWareByBarcode("", product.internal_code, true, p);
		return;
	    }
	} 
	else {
	    Error(113);
	}
    };
    if ( count == 0 ) {	
	Error(113);
	return;
    }	    
    if ( count < 0 ) { Error(7); return; }
     
    // если дошли сюда, то товаров с заданной ценой несколько
    puts("==========================================multiply wares found...");  
    ReleaseKeyboard();
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
    visualForm *vs = new visualForm;
    
    if ( StateStore::GetInstance()->GetReceiptState() == RETURN ) vs->check_type = 1;
    vs->FillTable("0");
    vs->setPrice(p);
    //-----(+)-----Zuskin-----14/10/2011-----
    if ( quantTextLabel->text().length() != 0 && quantTextLabel->text().length() != 22) {
	vs->vsQttyPreSet = true;
    }
    //-------------------------------------------------
    QDesktopWidget *d = QApplication::desktop();
    if (d->width() <= 800) vs->setWindowState(Qt::WindowFullScreen);
    
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) disconnect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence()) );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) disconnect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    
    notaddbyvisual = false;
    vs->exec();
    notaddbyvisual = true;
    
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence()) );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    
    delete(vs);	
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();
    return;
  //--------------------------------------------------------  
}

void RegistrationForm::AdjustLabelFont(QLabel* label)
{ 	
    g = label->font();    
    QFont f = QFont(g);
    int L_W = label->width();
    int F_S = DefLabelFontSize;
    f.setPointSize(F_S);
    QFontMetrics fm( f );
    int P_W = fm.width( label->text() );
    double koef1= (double)P_W/(double)(L_W-10);
    if (koef1>1.000)
    {	
	F_S =(int)F_S/koef1;
    }
    
    f.setPointSize(F_S);
    label->setFont(f);
}

//-----(+)-----Zuskin-----10/10/2011-----
void RegistrationForm::PutOut()
{   
    if (StateStore::GetInstance()->GetReceiptState() == COPY) {
	// нельзя отложить копию чека !!!
	Error(36);
	return;
    }
    //-----(+)-----Zuskin-----21/10/2011----- добавим пока проверку права на отмену чека, потом поменяем на отдельное право откладывать чек
    if ( !GetRight(8) && !AuthorizationRequest(8) )    {
	Error(72);
	if ( authorization_request_flag ) SwapToAuthorizedUser();
	    return;
    }
    //---------------------------------------
    if (StateStore::GetInstance()->GetReceiptState() == FREE) {
	// вывести список ранее отложенных чеков
	int n = -1;
	bool nf;
	
	if ( str != "" ) {
	    n = str.toInt(&nf);
	}
	int res = CheckList(n, false, true);
	if ( res == 27 ) {
	    if (n==-1) {
		Error(116);
	    }
	    else {
		Error(117);
	    }
	    return;
	}
	if ( res != 0) {
 	    return;  
	}
		
	int check_number = 0;
	
	if (hw->ECR_GetProtocol() == 4) {
	    hw->ECR_ReadDocumentNumber(2);
	    check_number =  hw->ECR_GetDocumentNumber();
	}    else {
	    hw->ECR_ReadCheckNumber();
	    check_number = hw->ECR_GetCheckNumber();
	}
    
	if ( check_number  == -1 ) {  Error(19);  return;  }
	QString ss;
	QString s;
	if (StateStore::GetInstance()->GetReceiptState() == SELL) {
	  s = "открыт чек продажи №"+ss.setNum(check_number);  
	  receipt->SetCheckType(CHECK_SELL);
	}
	else {
	  s = "открыт чек возврата №"+ss.setNum(check_number);  
	  receipt->SetCheckType(CHECK_RETURN);  
	}
	
	modeTextLabel->setText(DefCodec->toUnicode(s));
	statusTextLabel->setText(DefCodec->toUnicode("чек восстановлен"));
	receipt->SetCheckNumber(check_number);
	if ( receipt->GetDiscountElemRef(1)->df || receipt->GetDiscountElemRef(2)->df || receipt->GetDiscountElemRef(3)->df )	    	StateStore::GetInstance()->SetViewState(REC);
	return;
    }
    
    if ( receiptTable->numRows() == 0 )  {    Error(118);   return ;  }  // ни одной регистрации в чеке
    if (receipt->Sum() == 0)  {    Error(119);   return;  }   
        
    // чек продажи или возврата - выдать запрос на подтверждение откладывания текущего чека
    if ( Acknowledgement(DefCodec->toUnicode("Чек будет отложен!!!\n продолжить операцию?\n")) != 0 )  return;
    FillReceipt( StateStore::GetInstance()->GetReceiptState() == RETURN ? 1 : 0);
    ins_tr->CreateDateTimeString();
     if ( StateStore::GetInstance()->GetReceiptState() == RETURN ) 
 	//-----(*)-----Zuskin-----18/11/2011-----
	//ins_tr->InsertCheckRegistrationTransactions("98", "", nalsum, credsum, contsum, closed_session_flag, true) ;
	 ins_tr->InsertCheckRegistrationTransactions("98", "", nalsum, credsum, contsum, closed_session_flag, true, string(manager_id.ascii()));
                //---------------------------------------------------
    if ( StateStore::GetInstance()->GetReceiptState() == SELL )
	//-----(*)-----Zuskin-----18/11/2011-----
    	//ins_tr->InsertCheckRegistrationTransactions("99", "", nalsum, credsum, contsum, closed_session_flag, true);
	ins_tr->InsertCheckRegistrationTransactions("99", "", nalsum, credsum, contsum, closed_session_flag, true, string(manager_id.ascii()));
                //----------------------------------
    
    statusTextLabel->setText(DefCodec->toUnicode("чек отложен"));
           
    ClearForNewOperation();
    receipt->CloseReceipt();
    StateStore::GetInstance()->SetReceiptState(FREE);
    quantTextLabel->setText("");
    textTextLabel->setText("");
    nalsum = 0;
    credsum = 0;
    contsum = 0;
    
    if ( authorization_request_flag ) SwapToAuthorizedUser();
    //-----(+)-----Zuskin-----30/09/2011-----
    str = "";
    //-------------------------------------------------
    return;
}
//--------------------------------------------------


void RegistrationForm::ShowManagerForm()
{
    if ( StateStore::GetInstance()->GetSessionState() == BLOCKED ) {   Error(74);  return;	  }
    if ( StateStore::GetInstance()->GetReceiptState() == FREE )  {    Error(14);    return;     }        // чек не открыт
    if ( StateStore::GetInstance()->GetReceiptState() == COPY )  {   Error(36);   return;     }         // копия чека
    
    int n = Reg.Query_Cnt_Managers();
    if (n <= 0) {
	Error(120);
	return;
    }
    
    ReleaseKeyboard(); 
    HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
    
    // заполним параметры формы
    managerListForm *mlf = new managerListForm;
    mlf->fillTable();        	        	    
    int k = mlf->exec();
    
    if (mlf->idx == -2) {
	// отмена выбора менеджера-консультанта
	manager_name = "";
	manager_id = "";
    }
    if (mlf->idx == -1) {
	// закрыли без выбора
    }
    if (mlf->idx >= 0) {
	manager_name = mlf->GetName();
	manager_id = mlf->GetId();
	printf("chosen manager name = %s\n", manager_name.ascii());
    }
    SetManagerName();
    delete(mlf);
    HwSignalHandlersController::GetInstance()->SetControlledObject(REGISTRATION);
    GrabKeyboard();	
    
}

//-----(+)-----Zuskin-----18/11/2011-----
void RegistrationForm::SetManagerName()
{
    manager_label->setText("");
    if (manager_name.isEmpty() == false) {
	QString stext = DefCodec->toUnicode("   Менеджер: ");
	stext += DbCodec->toUnicode( manager_name );
	manager_label->setText( stext );
    }	
}
