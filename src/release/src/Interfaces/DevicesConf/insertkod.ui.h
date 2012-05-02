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
******************************************************************************/
#include "const.h"
#include "hwsrv.h"
#include <malloc.h>
#include <qfocusdata.h> 
#include <sys/stat.h>

extern HWSRV *hw;

void InsertKod::init()
{
    vc = new QIntValidator(0,9999,kod1);
    kod1->setValidator(vc);
    kod2->setValidator(vc);
    kod3->setValidator(vc);
    kod4->setValidator(vc);
    DefCodec = QTextCodec::codecForName(SYSCODEC);
    Change->setEnabled(FALSE);
    SetTabOrderKods();
    kod1->setFocus();
    QObject::connect (Change, SIGNAL(clicked()),this, SLOT(Change_clicked()));
    QObject::connect (Cancel, SIGNAL(clicked()),this, SLOT(Cancel_clicked()));
}

void InsertKod::closeEvent(QCloseEvent* e)
{
  e->accept();
  return;  
}


void InsertKod::MyInit(int Param)
{
    //printf("Param = %d\n",Param);
    Par = Param;
	 if (Par == 1)
	 {
		  Insert_textLabel->setText(DefCodec->toUnicode("Введите серийный номер\n продукта:"));
		  char * str = (char *)calloc(17,sizeof(char));
		  strcpy(str,hw->Get_ActiveKod(str));
		  if (strcmp(str,"") == 0)
				Change->setText(DefCodec->toUnicode("Ввести"));
		  free(str);
		  kod4->hide();
	 }
	 else
	 {
		  Insert_textLabel->setText(DefCodec->toUnicode("Введите ключ активации\n продукта:"));
		  Change->setText(DefCodec->toUnicode("Ввести"));
	 }   
}


int InsertKod::Change_clicked()
{
    //puts("Change_clicked()");
    int res = 0;
    FILE * fp;
    if (Par == 1)
    {
	res = hw->FileAccess(SERIAL_WAY);
	if (res == 0)
	{
	    fp = fopen(SERIAL_WAY,"w");
	    char * str = (char *)calloc(13,sizeof(char));
	    strcpy(str,kod1->text());
	    strcat(str,kod2->text());
	    strcat(str,kod3->text());
	    fputs(str,fp);
	    free(str);
	    fclose(fp);
	    close();
	    return 0;
	}
	else
	    return res;
    }
    else
    {
	res = hw->FileAccess(ACTKEY_WAY);
	if (res == 0)
	{
	    fp = fopen(ACTKEY_WAY,"w");
	    char * str = (char *)calloc(17,sizeof(char));
	    strcpy(str,kod1->text());
	    strcat(str,kod2->text());
	    strcat(str,kod3->text());
	    strcat(str,kod4->text());
	    fputs(str,fp);
	    free(str);
	    fclose(fp);
	    close();
	    return 0;
	}
	else 
	    return res;
    }
}


void InsertKod::Cancel_clicked()
{
    //puts("Cancel_clicked()");
    close();
}


void InsertKod::keyPressEvent(QKeyEvent * k)
{	
	// puts("void InsertKod::keyPressEvent(QKeyEvent * k)");
	 switch(k->key())   
	 {
	 case Qt::Key_F12 :{
				//puts("Qt::Key_F12");
				close();
				break;
		  }
	 case Qt::Key_Enter:{
				if (Change->isEnabled())
					 Change_clicked();
				break;
		  }
	 case Qt::Key_Return:{
				if (Change->isEnabled())
					 Change_clicked();
				break;
		  }
	 default :	{	
		  break;
	       }
      }
}

void InsertKod::SetTabOrderKods(void)
{
	setTabOrder(kod1,kod2);
	setTabOrder(kod2,kod3);
	setTabOrder(kod3,kod4);
	setTabOrder(kod4,Change);
	setTabOrder(Change,Cancel);
	setTabOrder(Cancel,kod1);
}


void InsertKod::Kod_textChanged( const QString &str )
{
	 int length1 = kod1->text().length();
	 int length2 = kod2->text().length();
	 int length3 = kod3->text().length();
	 int length4 = kod4->text().length(); 
	 if (Par == 1)
	 {
		  if (length1+length2+length3 == 12)
				Change->setEnabled(TRUE);
		  else
				Change->setEnabled(FALSE);
	 }
	 else
	 {
		  if (length1+length2+length3+length4 == 16)
				Change->setEnabled(TRUE);
		  else
				Change->setEnabled(FALSE);
	 }	 
	 
	 int length = str.length();
	 if ( length == 4){
		  if (kod1->hasFocus()){
				kod2->setFocus();	
				return;
		  }	 
		  if (kod2->hasFocus()){
				kod3->setFocus();				
				return;
		  }	
		  if (Par != 1){
				if (kod3->hasFocus()){
					 kod4->setFocus();				
					 return;
				}			
				if (kod4->hasFocus()){
					 Change->setFocus();				
					 return;
				}	
		  }
		  else{
				if (kod3->hasFocus()){
					 Change->setFocus();				
					 return;
				}
		  }
	 }
}

