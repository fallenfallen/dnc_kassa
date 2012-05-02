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
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/
#include "authorization.h"
#include "messages.h"
#include "reader_config_load_unload.h"
#include "const_conf.h"
#include "addon_func.h"
#include <qmessagebox.h>
#include <qtextcodec.h>
#include <qfocusdata.h>
#include "const.h"
#include "npasreconciliationform.h"


ReaderConf * rc;
extern Trans Tr;
extern SP sp;
extern BlockTimer *bt;
char * fileflag_path = NULL;
char * fileflag_upload = NULL;
char * filedata_upload = NULL;

QTextCodec * systemCodec = QTextCodec::codecForName(SYSCODEC);

extern DebugLevelLog regformlog;

extern HWSRV * hw;

//======================================================================//
void serviceForm::init()
{
    Started = false;
    connect(reportPushButton, SIGNAL(clicked()), SLOT(ReportForm()));
    connect(syncPushButton, SIGNAL(clicked()), SLOT(SynchronizationForm())); 
    connect(transPushButton, SIGNAL(clicked()), SLOT(SaleReports())); 
    connect(waresloadPushButton, SIGNAL(clicked()), SLOT(WaresLoad())); 
    connect(npasreconciliationPushButton, SIGNAL(clicked()), SLOT(Reconsiliation()));
    
    if ( hw->PS_GetType() == 0 )	{
	npasreconciliationPushButton->hide();
    } 
    
    grabKeyboard();
    adjustSize();
    if ( sp.autoblock_flag )    bt->action_time = QTime::currentTime();
    
    if ( !sp.query_for_authorization )  {
	if ( !Authorization::GetInstance()->GetRight(21) )      syncPushButton->setEnabled(false);
	if ( !Authorization::GetInstance()->GetRight(20) )      reportPushButton->setEnabled(false);
	if ( !Authorization::GetInstance()->GetRight(27) )      transPushButton->setEnabled(false);
	if ( !Authorization::GetInstance()->GetRight(28) )      waresloadPushButton->setEnabled(false);
	if ( !Authorization::GetInstance()->GetRight(29) )      npasreconciliationPushButton->setEnabled(false);
    }
    
    rc = new ReaderConf();
    int len = rc->getLen("[UNLOAD_FLAG]");
    if (len != 0 ) {
	fileflag_path = (char*) calloc(len + 1, sizeof(char));
	rc->GetUnloadFlagPath(fileflag_path);
    }
    
    len = rc->getLen("[UPLOAD_FLAG]");
    if (len != 0 ) {
	fileflag_upload = (char*) calloc(len+1, sizeof(char));
	rc->GetUploadFlagPath(fileflag_upload);
    }
    
    len = rc->getLen("[UPLOAD_FILE]");
    if ( len != 0 ) {
	filedata_upload = (char*) calloc(len+1, sizeof(char));
	rc->GetUploadDataPath(filedata_upload);
    }
    
    delete rc;
}

//======================================================================//
void serviceForm::destroy()
{
//puts"void serviceForm::destroy()");
  releaseKeyboard();
}

//======================================================================//
void serviceForm::ReceiveZReport()
{
  emit ZReport();
}

//======================================================================//
void serviceForm::ReceiveSync()
{
  emit Sync();
}
//======================================================================//
void serviceForm::ReportForm()
{
    QWidget *pb = (QWidget *) reportPushButton;
    pb->setFocus();
    
    //запрос авторизации прав доступа
    bool right = Authorization::GetInstance()->GetRight(20);
    if ( !right )    right = sp.query_for_authorization && Authorization::GetInstance()->RequestWithRight(20);
    
    if ( right )  {
	reportForm *rf = new reportForm;
	//rf->SetParams(hw);
	if (rf->kkm_connect && rf->db_connect)    {
	    connect(rf, SIGNAL(ZReport()), this, SLOT(ReceiveZReport()));
	    //-----(*)-----Zuskin-----23/03/2011-----
	    //rf->exec();
	    if (hw->EW_GetType() > 0) {
		hw->EW_Stop();		
		rf->exec();
		hw->EW_Start();
	    }
	    else {
		rf->exec();
	    }
	    //-------------------------------------------------
	    
	}
	delete(rf);
    }
    else Error(72); 
}

//======================================================================//
void serviceForm::SynchronizationForm()
{
//puts"void serviceForm::SynchronizationForm()");
  QWidget *pb = (QWidget *) syncPushButton;
  pb->setFocus();

  //запрос авторизации прав доступа
  bool right = Authorization::GetInstance()->GetRight(21);
  if(!right)
    right = sp.query_for_authorization && Authorization::GetInstance()->RequestWithRight(21);
  
  if(right)
  {
  
    synchronizationForm *sf = new synchronizationForm;

    //sf->SetParams(hw);
    if(sf->kkm_connect)
    {
      connect(sf, SIGNAL(Sync()), this, SLOT(ReceiveSync()));
      sf->exec();
    }
    delete(sf);
  }
  else Error(72);
}
/**************************************************************/
int serviceForm::CheckPath(char* path)
{
    // проверить на правильность входящий путь
    //printf("check path (%s) \n", path);
    char* ptr = strrchr(path,'/');
    if (ptr != NULL) 
    {
	int n = strlen(path) - strlen(ptr);
	char* tmp = (char*) calloc( n + 1, sizeof(char) );
	strncpy(tmp, path, n);
	//printf("проверяемый путь = %s \n", tmp);
	if (access(tmp, 06) != 0) 
	{  
	    //puts"прочитан неверный путь");
	    free(tmp);
	    return -1;
	}
    }
    return 0;
}

/*************************************************************/
void serviceForm::SaleReports()
{  
    //запрос авторизации прав доступа
    bool right = Authorization::GetInstance()->GetRight(27);
    if ( !right )    right = sp.query_for_authorization && Authorization::GetInstance()->RequestWithRight(27);
    
    if ( right )  {
	handleUnload * hu = new handleUnload;
	hu->exec();
	delete(hu); 
    }
    else Error(72);    
    
}
//======================================================================//
void serviceForm::keyPressEvent(QKeyEvent *k)
{
    
    if(sp.autoblock_flag) bt->action_time = QTime::currentTime();
    
    switch(k->key())  {
    case Qt::Key_Enter :
	ExecForm();
	break;
    case Qt::Key_Return :
	ExecForm();
	break;
    case Qt::Key_Space :
	ExecForm();
	break;	   
    case Qt::Key_Up :
	this->focusNextPrevChild ( false );
	break;
    case Qt::Key_Down :
	this->focusNextPrevChild ( true );
	break;
    case Qt::Key_F12 :
	close();
	break;
    case Qt::Key_1 :
	if ( reportPushButton->isEnabled() ) ReportForm();	   
	break;	   
    case Qt::Key_2 :
	if ( syncPushButton->isEnabled() ) SynchronizationForm();	   
	break;	  	   
    case Qt::Key_3 :
	if ( transPushButton->isEnabled() ) SaleReports();	   
	break;	  	   
    case Qt::Key_4 :
	if ( waresloadPushButton->isEnabled() ) WaresLoad();	   
	break;	   
    case Qt::Key_5 :
	Reconsiliation(); 
	break;	   	
    } 
}

//====================================================================//
void serviceForm::ExecForm()
{
  //puts"serviceForm::ExecForm()");
    QWidget * w = this->focusData()->focusWidget();
    
    if ( w == reportPushButton ) ReportForm();	  
    if ( w == syncPushButton ) SynchronizationForm();
    if ( w == transPushButton ) SaleReports();
    if ( w == waresloadPushButton ) WaresLoad();
    if ( w == npasreconciliationPushButton ) Reconsiliation();
}

//======================================================================//
void serviceForm::Error(int er)
{
  //puts"void serviceForm::Error(int er)");
  QString er_id, er_str;
  RegistrationErrorText(er, er_str, er_id);
  
  releaseKeyboard();
  InterfaceErrorWin(er_str, er_id);
  grabKeyboard();
}

//======================================================================//
void serviceForm::WaresLoad()
{
    bool right = Authorization::GetInstance()->GetRight(28);
    if ( !right )    right = sp.query_for_authorization && Authorization::GetInstance()->RequestWithRight(28);
    
    if ( !right ) {  Error(72); return;  }
    
    releaseKeyboard();
    if ( QMessageBox::question( this, systemCodec->toUnicode("Загрузка справочника товаров"), systemCodec->toUnicode("Загрузить справочник товаров?"), QMessageBox::Yes, QMessageBox::Cancel, 0 ) != QMessageBox::Yes ) return;
    
    
    int k = TestPath(filedata_upload);
    
    if ( k != 0 ) {
	QMessageBox::critical( this, systemCodec->toUnicode("Ошибка"),systemCodec->toUnicode("Нет доступа к данным для загрузки товаров"), "OK",0);
	return;
    }
    
    k = TestPath(fileflag_upload);
    
    if ( k == -1 ) {
	QMessageBox::critical( this, systemCodec->toUnicode("Ошибка"),systemCodec->toUnicode("Задан неверный путь для файла флага!"), "OK",0);
	return;
    }
    
    if ( k == 0 ) {
	int message_state = QMessageBox::question( this, systemCodec->toUnicode("Ошибка"),systemCodec->toUnicode("Файл флаг уже существует! Перезаписать?"), QMessageBox::Yes, QMessageBox::Cancel, 0 );
	if ( message_state != QMessageBox::Yes )   return;
    }
    
    if ( k == 3 || k == 2 ) {
	QMessageBox::warning( this, systemCodec->toUnicode("Ошибка"),systemCodec->toUnicode("Создание файла флага не доступно!"), "OK",0);
	return;
    }
    
    FILE * fp = fopen(fileflag_upload,"w+");
    if (fp)   {
	fclose(fp);
	QMessageBox::information( this, systemCodec->toUnicode("Сообщение"),systemCodec->toUnicode("Процесс загрузки справочника запущен!"), "OK",0);
    }
    grabKeyboard();
}
//======================================================================//

//СВЕРКА ИТОГОВ ИНПАС
void serviceForm::Reconsiliation() 
{
    bool right = Authorization::GetInstance()->GetRight(29);
    if ( !right )    right = sp.query_for_authorization && Authorization::GetInstance()->RequestWithRight(29);
    
    if (hw->PS_GetType()!=0)	{
	npasReconciliationForm npasrf;
	npasrf.exec();
    } 
}
//======================================================================//

