/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
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
#include <qmessagebox.h>
#include <malloc.h>
#include <qfocusdata.h>
#include "addon_func.h"
#include "reader_config_load_unload.h"

#include <sys/stat.h>
#include <stdlib.h>

extern char * fileflag_path;

extern QTextCodec * systemCodec;

//-----(+)-----Zuskin-----23/05/2011-----
int n_protocol = 0;
//---------------------------------------

#define SYSCODEC "UTF8"

/*******************************************************************/
void handleUnload::init()
{
    QDate date =  QDate::currentDate();
    QTime time_c(0,0,0);
    QTime time_b(23,59,59);
    QDateTime d_t(date,time_b);
    QDateTime cur(date,time_c);
    Begin_dateTimeEdit->setDateTime(cur);
    End_dateTimeEdit->setDateTime(d_t);
    //systemCodec = QTextCodec::codecForName(SYSCODEC); 
    connect(DataTimeUnLoadButton, SIGNAL(clicked()), SLOT(DataTimeUnLoadButton_clicked()));
    connect( savelasttransCheckBox, SIGNAL(toggled(bool)), SLOT(SaveHUParam()) );
    
    
    grabKeyboard();
    releaseKeyboard();
    
    DataTimeUnLoadButton->setFocus();
    
    //Begin_dateTimeEdit->releaseKeyboard();
    //End_dateTimeEdit->releaseKeyboard();
    
    ReaderConf* rc = new ReaderConf();
    int len = rc->getLen("[HU_MARK_TRANS]");
    if (len != 0 ) {
	char* tmp = (char*) calloc(len+1, sizeof(char));
	rc->GetHUParam(tmp);
	if ( (strcmp(tmp, " t") == 0) || (strcmp(tmp, "t") == 0) ) savelasttransCheckBox->setChecked(true);
	free(tmp); 
    }
    //-----(+)-----Zuskin-----23/05/2011-----
    len = rc->getLen("[PROTOCOL]");
    if (len != 0 ) {
	char* tmp = (char*) calloc(len+1, sizeof(char));
	rc->GetProtocol(tmp);
	n_protocol = atoi(tmp);
	free(tmp); 
    }
    //----------------------------------------    
    delete rc;
    
    Begin_dateTimeEdit->setAutoAdvance ( true );
    End_dateTimeEdit->setAutoAdvance ( true );
    
}
/*******************************************************************/
void handleUnload::closeEvent( QCloseEvent * ce )
{
    SaveHUParam();
    
    accept(); 
}
/*******************************************************************/
int handleUnload::Contingencies(void)
{
    if (Begin_dateTimeEdit->dateTime() > End_dateTimeEdit->dateTime())
    {
	//releaseKeyboard();
	QMessageBox::critical(this, systemCodec->toUnicode("Ошибка"), systemCodec->toUnicode("Дата и время окончания не может превышать дату и время начала выгрузки отчета!\n"), QMessageBox::Ok, QMessageBox::NoButton);
	//grabKeyboard();
	End_dateTimeEdit->setDateTime(Begin_dateTimeEdit->dateTime());
	return -1;
    }
    return 0;
}
/*******************************************************************/
QString handleUnload::GenerateStr(void)
{
    QDateTime time_begin = Begin_dateTimeEdit->dateTime();
    QDateTime time_end = End_dateTimeEdit->dateTime();
    QDate date_beginn = time_begin.date();
    QTime time_beginn = time_begin.time();
    QDate date_endd = time_end.date();
    QTime time_endd = time_end.time();
	
    QString date_b = date_beginn.toString("dd.MM.yyyy");
    QString time_b = time_beginn.toString(Qt::TextDate);
    QString date_e = date_endd.toString("dd.MM.yyyy");
    QString time_e= time_endd.toString(Qt::TextDate);
    
    //-----(*)-----Zuskin-----23/05/2011-----
    //QString s = "$$$TRANSACTIONSBYDATETIMERANGE\n";
    QString s;
    switch (n_protocol) {
	case 2: { // new DNC protocol
	    s = "!!!SALESOFTIMERANGE\n";
	    break;
	}
	default: {
	    s = "$$$TRANSACTIONSBYDATETIMERANGE\n";
	}
    }
    //----------------------------------------
    s +=date_b;
    s +=" ";
    s +=time_b;
    s +="; ";
    s +=date_e;
    s +=" ";
    s +=time_e;
    //s +="; \n";
    s += ";";
    return s;
}


/*******************************************************************/
// str - строка записи в файл
void handleUnload::CreateUnloadFlag(QString st)
{    
    
    int k = TestPath(fileflag_path);

    
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
    
    FILE * fp = fopen(fileflag_path,"w+");
    if (fp)   {
	fputs(st.ascii(),fp);
	fclose(fp);
	QMessageBox::information( this, systemCodec->toUnicode("Сообщение"),systemCodec->toUnicode("Отчет сформирован!"), "OK",0);
    }
    
}

/*******************************************************************/
void handleUnload::DataTimeUnLoadButton_clicked(void)
{    
    if ( Contingencies() != 0 ) return;
    
    QString Str = GenerateStr();
    
    //releaseKeyboard();
    CreateUnloadFlag(Str);
    //grabKeyboard();
}

/*******************************************************************/
void handleUnload::keyPressEvent(QKeyEvent *k)
{    
    switch(k->key())    {
    case Qt::Key_Up :		
	this->focusNextPrevChild ( false );
	break;
    case Qt::Key_Down :		
	this->focusNextPrevChild ( true );
	break;
    case Qt::Key_F12:	
	accept();
	break;	
    case Qt::Key_Enter:	
	DataTimeUnLoadButton_clicked();
	break;	
    case Qt::Key_Return:		
	DataTimeUnLoadButton_clicked();
	break;	
    }    
    
}
/*******************************************************************/
void handleUnload::SaveHUParam()
{
        
    FILE * f_tmp = fopen("/tmp/unload_config_tmp", "w+");
    if ( f_tmp == NULL ) {  return;   }

    FILE * f_config = fopen(RC_LOADUNLOAD_PATH, "rw+");
    if ( f_config == NULL ) {  	fclose(f_tmp);	return;    }
    
    char* str = (char*) calloc(500, sizeof(char));
    
    bool write_ok = false;
    
    while ( !feof(f_config) ) {
       fgets(str, 500, f_config);
       if ( strncmp(str, "[HU_MARK_TRANS] ", 16) == 0 ) {
	   strcpy(str, "[HU_MARK_TRANS] ");
	   if ( savelasttransCheckBox->isChecked() ) strcat(str, "t");
	   else strcat(str, "f");	
	   write_ok = true;
       }
       fwrite(str, sizeof(char), strlen(str), f_tmp);
       strcpy(str, "");  
    }
    
    if ( ! write_ok ) {	
	if ( savelasttransCheckBox->isChecked() ) fwrite("[HU_MARK_TRANS] t", sizeof(char), 17, f_tmp);
	else fwrite("[HU_MARK_TRANS] f", sizeof(char), 17, f_tmp);
    } else {
	
    }
	
    
    free(str);
    fclose(f_config);
    fclose(f_tmp);
    
      
   
    remove (RC_LOADUNLOAD_PATH);
    rename("/tmp/unload_config_tmp", RC_LOADUNLOAD_PATH);
    remove("/tmp/unload_config_tmp");
    
    chmod(RC_LOADUNLOAD_PATH, 502);
    
}
/*******************************************************************/
