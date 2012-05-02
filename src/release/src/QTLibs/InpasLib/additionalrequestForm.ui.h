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
#include "const.h"
#include "hwsrv.h"
#include <qtextcodec.h>
#include <signal.h>
#include <sys/wait.h>
#include <malloc.h>
#include <cstdlib>

extern HWSRV  * hw;

extern QTextCodec * systemCodec;
extern QTextCodec * hwCodec;

/************************************************************************************/
int additionalrequestForm::GetQuantStr(const char * str){
	 char * ss = (char *)calloc(strlen(str),sizeof(char));
	 strcpy(ss,str);
	 int count = 0;
	 for (unsigned int i = 0;i<strlen(str);i++){
		  if (*(ss+i) == 0x0A){
				count++;
		  }
	 }
	 free(ss);
	 return count+1;
}

void additionalrequestForm::init(){
    
    Mode = hw->PS_GetMode();
    printf("Mode = %d\n",Mode);
    
    Ask = hw->PS_GetAskAssistant();
    printf("Ask = |%s|\n",Ask.ascii());
    
    VariantAnswer = hw->PS_GetAnswerAssistant();
    printf("VariantAnswer = |%s|\n",VariantAnswer.ascii());
    
    int MaxAnswer = GetQuantStr(VariantAnswer.ascii());
    printf("MaxAnswer = |%d|\n",MaxAnswer);
    
    switch ( Mode ) {
    case 1:  {	
	AskAssistant->setText(systemCodec->toUnicode(Ask.ascii()));
	VariantAnswerAssistant->setText(systemCodec->toUnicode(VariantAnswer.ascii()));
	QString s;
	QString ss = "[1-"+s.setNum(MaxAnswer);
	ss = ss+"]";
	QRegExp re(ss);
	QValidator * valid = new QRegExpValidator(re,this);
	AnswerAssistant->setValidator(valid);			
	break;
    }
    case 2:  {
	AskAssistant->setText(systemCodec->toUnicode(Ask.ascii()));
	AnswerAssistant->setInputMask(VariantAnswer);
	VariantAnswerAssistant_textLabel->hide();
	VariantAnswerAssistant->hide();
	AnswerAssistant->setCursorPosition(0);
	break;	
    }	
    case 3:  {
	    AskAssistant->setText(systemCodec->toUnicode(Ask.ascii()));
	    VariantAnswerAssistant->setText(systemCodec->toUnicode(VariantAnswer.ascii()));
	    if ( MaxAnswer < 0 )  {	puts("error");   return;  }
	    switch ( MaxAnswer ) {
	    case 1:  {
		QString ss = "[1-1]";
		QRegExp re(ss);
		QValidator * valid = new QRegExpValidator(re,this);
		AnswerAssistant->setValidator(valid);
		break;
	    }
	    default:  {
		    QString ss;
		    QString tmp;
		    for ( int h = 1; h < (MaxAnswer); h++ )  {
			tmp.setNum(h);
			ss = ss+"[";
			ss = ss+tmp;
			ss = ss+"-";
			tmp.setNum(h+1);
			ss = ss+tmp;
			ss = ss+"]";
		    }
		    ss = ss+"[";
		    ss = ss+tmp.setNum(MaxAnswer);
		    ss = ss+"]";
		    
		    printf("ss = |%s|\n",ss.ascii());
		    
		    QRegExp re(ss);
		    QValidator * valid = new QRegExpValidator(re,this);
		    AnswerAssistant->setValidator(valid);
		    break;
		}
	    }	
	    
	    break;
	}
    case 5:  {
	    AskAssistant_textLabel->hide();
	    AskAssistant->hide();
	    VariantAnswerAssistant_textLabel->hide();
	    VariantAnswerAssistant->hide();
	    AnswerAssistant_textLabel->hide();
	    AnswerAssistant->hide();
	    
	    //создать массив здесь и заполнить его в hwsrv
	    vector <string> mas = hw->PS_InsertMas(Ask.ascii());
	    unsigned int Length  = mas.size();
	    if (Mode == 4) {
		//нужно вывести строки на печать принтеру 
		int Err = 0;
		int res = hw->ECR_GetType();
		printf("ECR_GetType = %d\n",res);
		if (res != 0) {
		    for (unsigned int f = 0; f<Length; f++){
			printf("mas[f] = |%s|\n",mas[f].c_str());
			if ( hw->ECR_PrintString(hwCodec->fromUnicode(systemCodec->toUnicode(mas[f].c_str())))!=0 )  Err++;
		    }
		} else {
		    puts("а принтер вообще выключен))");
		    Err++;
		}
		if ( Err > 0 )  {
		    puts("Невозможно напечатать строки переданные на печать сервером!!!");
		    //Message->setText(systemCodec->toUnicode("Невозможно напечатать строки переданные на печать сервером!!!"));
		    hw->PS_SetStatus(1);
		}  else  {
		    puts("Печать строк успешно завершена!!!");
		    hw->PS_SetStatus(0);
		}
		OK_clicked();
	    }  else  {
		if ( hw->DSP_GetType() != 0 ) {
		    for (unsigned int f = 0; f<Length; f++) {
			hw->DSP_Up(hwCodec->fromUnicode(systemCodec->toUnicode(mas[f].c_str())));
			sleep(1);
		    }
		    puts("Строки успешно выведены на экран!!!");
		    hw->PS_SetStatus(0);  
		}  else  {
		    puts("а дисплей вообще выключен))");
		    hw->PS_SetStatus(1);
		    //Message->setText(systemCodec->toUnicode("Невозможно вывести строки на экран!!!"));
		} 
		OK_clicked(); 
	    }	 
	    close();
	    break;
	}
    default:    puts("error");
    }
}
/************************************************************************************/
void additionalrequestForm::WaitAnswer(void){					
    //включаем таймаут2 и если он закончился посылаем ждать серверу
    ChildPid = fork();
    if (ChildPid == 0){
	hw->PS_WaitForTimeout2AnswerFromKash();
	_exit (EXIT_SUCCESS);
    }
}	
/************************************************************************************/
void additionalrequestForm::OK_clicked()
{
    puts("void additionalrequestForm::OK_clicked()");
    int res = hw->PS_GetType();
    if (res== 0){
	puts("ОТКЛЮЧЕНА ПЛАТЕЖНАЯ СИСТЕМА");
	return;
    } 
    hw->PS_SetCodeOper(5);
    if ((Mode > 0) && (Mode < 4)){
	Answer = AnswerAssistant->text();
	hw->PS_SetAnswerAssistant(Answer.ascii());
	hw->PS_SetStatus(1);
    } 
    close(); 
}	
/************************************************************************************/
void additionalrequestForm::Cancel_clicked()
{
    puts("void additionalrequestForm::Cancel_clicked()");
    int res = hw->PS_GetType();
    if (res== 0){
	puts("ОТКЛЮЧЕНА ПЛАТЕЖНАЯ СИСТЕМА");
	return;
    }
    hw->PS_SetCodeOper(5);
    if ((Mode > 0) && (Mode < 4))   hw->PS_SetStatus(0);
    close();
}
/************************************************************************************/
void additionalrequestForm::closeEvent(QCloseEvent * e){
    int stat;
    if (ChildPid>0) {
	kill(ChildPid,SIGTERM);
	waitpid(ChildPid, &stat, 0);
    }
    e->accept();
    return;
}	
/************************************************************************************/

