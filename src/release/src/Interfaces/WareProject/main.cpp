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
#include <qlabel.h>
#include <qlineedit.h>
#include <fcntl.h>
#include <math.h>
#include <qtextcodec.h>
#include <qmessagebox.h>
#include <signal.h>
#include <qobject.h>
#include <qcursor.h> 
#include <qtable.h> 
#include <qdatetime.h>
#include <qcombobox.h>
#include "hwsrv.h"
#include "reference_book_class_pgsql.h"
#include "addwareform.h"
#include "autoschemeform.h"
#include "mainwareform.h"
#include "taxgroupform.h"
#include "waregroupform.h"
#include "blocktimer.h"

#include "dkbinput.h"
#include "bcskbinput.h"
#include "mcrkbinput.h"
#include "qtextcodec.h"
#include "messages.h"
#include "hwsighandlerscontroller.h"

bool mainform_flag = false;
bool taxgroupform_flag = false;
bool addautoredform_flag = false;
bool addwareform_flag = false;
bool autoschemeform_flag = false;
bool taxrategroupform_flag = false;
bool waregroupform_flag = false;

BlockTimer *bt;
SP sp;

KeyboardDevice * kbcode_scaner = NULL;
KeyboardDevice * kbcode_reader = NULL;


MainWareform * w;
AddWareform * a_w;
HWSRV  * hw;
QString BarCode;
int Scanner;
int resolution_width;
int resolution_heigth;
QTextCodec * systemCodec = QTextCodec::codecForName(SYSCODEC);

PgRef M1("/etc/dancy/LinCash_db.conf");//для main
PgRef M("/etc/dancy/LinCash_db.conf");//для всех остальных


int init_keyboard_device(QApplication * a)
{
    int bcs_type = hw->BCS_GetType(); 
    int mcr_type = hw->MCR_GetType();    
    //printf("bcs_type = %d \n", bcs_type);
    
    if ( bcs_type == 2 ) {
	//puts("scaner keyboard");
	kbcode_scaner = new KeyboardScaner(a,hw->BCS_GetPref1(), hw->BCS_GetPref2(), hw->BCS_GetSuf1(), hw->BCS_GetSuf2());	
    }

    if ( mcr_type == 2 ) {
//	puts("reader keyboard");
	kbcode_reader = new KeyboardReader(a, hw->MCR_GetPref1(), hw->MCR_GetPref2(), hw->MCR_GetSuf1(), hw->MCR_GetSuf2());    
    }
    return 0;
}


void quit()
{
    if (hw->BCS_GetPid() > 0)
	kill(hw->BCS_GetPid(),SIGTERM); 
}

void get_setup_params()
{
  int res;
  if(sp.state_connect != 0)
  {
    QString er_id, er_str;
    WareReferenceErrorText(55, er_str, er_id);
    InterfaceErrorWin(er_str, er_id);
  }
  else if((res = sp.Get_All_Param()) != 0)
  {
    ////printf("state in main = %d\n", res);
    QString er_id, er_str;
    WareReferenceErrorText(56, er_str, er_id);
    InterfaceErrorWin(er_str, er_id);
  }
}

int main( int argc, char ** argv )
{   
    QApplication a (argc, argv);  
    
    	 QDesktopWidget * d = a.desktop();
	 int resolution_width = d->width();     // returns desktop width
	 int resolution_heigth = d->height();    // returns desktop height
	 
	 hw = new HWSRV();    
	 get_setup_params();
	 if (sp.autoblock_flag) bt =new BlockTimer(sp.autoblock_time);
	
	 init_keyboard_device(&a);
	 w = new MainWareform;  
	 
	 a.setMainWidget(w);
	 
	 int wid = w->width(); 
	 int heig = w->height();
	 // printf("wid = %d heig = %d\n", wid, heig); 
	 if ((wid>resolution_width)||(heig>resolution_heigth)){
		  w->setWindowState(Qt::WindowFullScreen);
		  w->setMinimumSize(QSize(resolution_width,resolution_heigth));
		  w->setMaximumSize(QSize(resolution_width,resolution_heigth));
		  w->adjustSize();
	 }
	 
	 w->limit = 20;
	 w->FillTable("0");
	 w->show();
	 
	 int ret = a.exec();
	 //printf("ret = %d\n",ret);
	 if ( kbcode_scaner != NULL ) delete kbcode_scaner;
	 if ( kbcode_reader != NULL ) delete kbcode_reader;
	 
	 return ret;
}   

