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
#include "const_conf.h"
#include <malloc.h>
#include <stdlib.h>


const char* CONFIG_PATH = RC_LOADUNLOAD_PATH;

const int  PARAM_COUNT = 6;
//const int MAX_LEN = 1000;

const char* PARAM_MAS[PARAM_COUNT] = {"[UPLOAD_FLAG]",
			  "[UPLOAD_FILE]",
			  "[UNLOAD_FLAG]",	     
			  "[UNLOAD_FILE]",
		          "[N_PC]",
			  "[PROTOCOL]"};

QWidget*  param_mas[PARAM_COUNT];
QTextCodec* codec_sys = QTextCodec::codecForName("UTF8");
QString folder_path("");
bool hu_mark_trans;
/************************************************************************/
void MainForm::InitMasParam()	 {
    //printf("mas init \n");
    param_mas[0] = uploadflagLineEdit;
    param_mas[1] = uploadfileLineEdit;
    param_mas[2] = unloadflagLineEdit;
    param_mas[3] = unloadfileLineEdit;
    param_mas[4] = n_pcSpinBox;
    param_mas[5] = protocolComboBox;
    
    //if(loadflagPushButton->autoDefault()) puts("Default Button!!!");
}
/************************************************************************/
void MainForm::ReadConfig() 	{

   ReaderConf* rc = new ReaderConf(CONFIG_PATH);
   
   int len = rc->getLen(PARAM_MAS[0]);
   char* upload_flag = (char*) calloc(len+2, sizeof(char));
   rc->GetUploadFlagPath(upload_flag);
   
   len = rc->getLen(PARAM_MAS[1]);
   char* upload_file = (char*) calloc(len+2, sizeof(char));
   rc->GetUploadDataPath(upload_file);
   
   len = rc->getLen(PARAM_MAS[2]);
   char* unload_flag = (char*) calloc(len+2, sizeof(char));
   rc->GetUnloadFlagPath(unload_flag);
   
   len = rc->getLen(PARAM_MAS[3]);
   char* unload_file = (char*) calloc(len+2, sizeof(char));
   rc->GetUnloadDataPath(unload_file);
   
   len = rc->getLen(PARAM_MAS[4]);
   char* n_pc = (char*) calloc(len + 2, sizeof(char));
   rc->GetNPC(n_pc);
   int npc = atoi(n_pc);
   
   len = rc->getLen(PARAM_MAS[5]);
   char* n_protocol = (char*) calloc(len + 2, sizeof(char));
   rc->GetProtocol(n_protocol);
   printf("n_protocol = %s \n", n_protocol);
   int n_prot = atoi(n_protocol);
   
   hu_mark_trans = false;
   
   len = rc->getLen("[HU_MARK_TRANS]");
   if (len != 0 ) {
       char* tmp = (char*) calloc(len+1, sizeof(char));
       rc->GetHUParam(tmp);
       if ( strcmp(tmp, " t") == 0 || strcmp(tmp, "t") == 0 ) hu_mark_trans = true;
       else hu_mark_trans = false;
       free(tmp); 
   }	

   //printf("[UPLOAD PARAM] flag = %s \n file = %s \n", upload_flag, upload_file);
   //printf("\n[UNLOAD PARAM] flag = %s \n file = %s \n", unload_flag, unload_file);
   
   delete rc;
   
   //puts("------------4---------------");
   
   ( (QLineEdit*) param_mas[0] )->setText(codec_sys->toUnicode(upload_flag));
   ( (QLineEdit*) param_mas[1] )->setText(codec_sys->toUnicode(upload_file));
   ( (QLineEdit*) param_mas[2] )->setText(codec_sys->toUnicode(unload_flag));
   ( (QLineEdit*) param_mas[3] )->setText(codec_sys->toUnicode(unload_file));
   ( (QSpinBox*) param_mas[4] )->setValue(npc);
   ( (QComboBox*) param_mas[5] )->setCurrentItem(n_prot);
   
   //puts("-------------5-------------");
   
   free(unload_flag);
   free(unload_file);
   
   free(upload_flag);
   free(upload_file);
   free(n_pc);
   //puts("end of init");
}
/************************************************************************/
int MainForm::init() 	{
    printf("init \n");
    InitMasParam();

    ReadConfig();
    //this->grabKeyboard();
    printf("end init \n");
    return 0;
}
/************************************************************************/
void MainForm::cancelPushButton_clicked()		{	reject();		}
/************************************************************************/
void MainForm::okPushButton_clicked()	{
    if (   QMessageBox::information(this, codec_sys->toUnicode("Сохрание изменений"), 
				    codec_sys->toUnicode("Сохранить параметры?"),
				    QMessageBox::Ok,
				    QMessageBox::Cancel ) != QMessageBox::Ok ) return;
    //puts("save");	
    
    
    
    FILE * f_config = fopen(CONFIG_PATH, "w+");
    if (f_config == NULL) {	printf("could not open config file \n");
	QMessageBox::information(this, codec_sys->toUnicode("Ошибка"), 
				 codec_sys->toUnicode("Файл config не может быть создан"),
				 QMessageBox::Ok ) ;
	return;        }	
    
    //puts("config open");
    //char * tmp = (char*) calloc(  2*MAX_LEN, sizeof(char)  );
    for (int i = 0; i < PARAM_COUNT;  i++ ) {
	//printf("in for i = %d \n", i);
	QString s = "";
	if ( strcmp(param_mas[i]->className(),"QLineEdit") == 0  ) 
	    s = codec_sys->fromUnicode( ( (QLineEdit*) param_mas[i] )->text() );
	if (	 strcmp(param_mas[i]->className(),"QSpinBox") == 0  ) 
	    s = ( (QSpinBox*)param_mas[i] )->text() ;
	if (	 strcmp(param_mas[i]->className(),"QComboBox") == 0  ) 
	    s = QString::number(( (QComboBox*)param_mas[i] )->currentItem()) ;
	//printf("s.length() = %d \n", s.length());
	//printf("s = %s \n", s.ascii());
	char* tmp = (char*) calloc(s.length()+strlen(PARAM_MAS[i])+3, sizeof(char));
	strcat(tmp, PARAM_MAS[i]);
	strcat(tmp, s);
	strcat(tmp, "\n");
	fwrite(tmp, sizeof(char), strlen(tmp), f_config);    	    
	//puts("was write");
	free(tmp);	
	//puts("tmp is free");
    }
    
    if ( hu_mark_trans ) {
	fwrite("[HU_MARK_TRANS] t", sizeof(char), strlen("[HU_MARK_TRANS] t") , f_config);
    } else {
	fwrite("[HU_MARK_TRANS] f", sizeof(char), strlen("[HU_MARK_TRANS] f") , f_config);
    }
    
   /* if (str_hu_mark_trans.ascii() != NULL){
	//puts("fwrite(str_hu_mark_trans.ascii(), sizeof(char), str_hu_mark_trans.length()+1, f_config);   ");
	fwrite("\n", sizeof(char), 5, f_config); 
	fwrite(str_hu_mark_trans.ascii(), sizeof(char), str_hu_mark_trans.length()+1, f_config);   
    } else {
	//puts("fwrite(\n[HU_MARK_TRANS] f, sizeof(char), str_hu_mark_trans.length()+1, f_config);   ");
	fwrite("\n[HU_MARK_TRANS]f", sizeof(char), strlen("\n[HU_MARK_TRANS]f") , f_config);   
    }*/

    fclose(f_config);
    puts("accept");
    accept();
}
/**************************************************************************/
void MainForm::ChooseFile( int n )  {
    QString str ;
    QString s  = codec_sys->fromUnicode( ( (QLineEdit*) param_mas[n] )->text());
    if ( (access(s.ascii(), 06)) != 0)  	
	str = QFileDialog::getSaveFileName( folder_path, "All files (*.*)", this, "file dialog", "Choose a file" );
    else str = QFileDialog::getSaveFileName( ( (QLineEdit*) param_mas[n])->text() ,  "All files (*.*)",  this, "file dialog", 
					     "Choose a file" );
    if ( ! str.isEmpty() )  {   
	( (QLineEdit*) param_mas[n] )->setText(str);    
	folder_path = str;
    }
    //param_mas[n]->setText(str); 
   /* else   { QMessageBox::information(this, codec_sys->toUnicode("error :("), 
				       codec_sys->toUnicode("не допустимое имя файла"),
				       QMessageBox::Ok ) ;    }*/
}
/**************************************************************************/
void MainForm::loadflagPushButton_clicked()	{   ChooseFile(0);		}
/***************************************************************************/
void MainForm::loadfilePushButton_clicked()	{   ChooseFile(1);		}
/***************************************************************************/
void MainForm::unloadflagPushButton_clicked() 	{   ChooseFile(2);		}
/***************************************************************************/
void MainForm::unloadfilePushButton_clicked()	{    ChooseFile(3);		}
/***************************************************************************/
int destroy()   {
    return 0;
}
/***************************************************************************/
void MainForm::keyPressEvent( QKeyEvent * k ) {
    switch(k->key())    {
       case Qt::Key_F12 : //ВЫХОД
	   close();
	   break;
    }	       
}
/***************************************************************************/

