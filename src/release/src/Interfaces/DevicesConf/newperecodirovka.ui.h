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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <signal.h>
#include <qlocale.h> 
#include <qapplication.h>
#include <qstring.h> 
#include <qinputdialog.h> 
#include <qlineedit.h>


#include "hwsrv.h"
#include "messages.h"
#include "configurations.h"

extern Configurations * w;
using namespace std;
extern int p;
int p1;

DedicationComboBox * NamePerecComboBox;
DedicationComboBox * KOD;

void NewPerecodirovka::AddComboBoxes()
{
    NamePerecComboBox = new DedicationComboBox(NamePerecComboBox1->parentWidget(), "r");
    QPoint p1 = NamePerecComboBox1->pos();
    layout114->add(NamePerecComboBox);
    delete(NamePerecComboBox1);
    NamePerecComboBox->move(p1);
    
    KOD = new DedicationComboBox(KOD1->parentWidget(), "r");
    QPoint p2 = KOD1->pos();
    layout57->add(KOD);
    delete(KOD1);
    KOD->insertItem(DefCodec->toUnicode("Десятичные коды"));
    KOD->insertItem(DefCodec->toUnicode("Шестнадцатиричные коды"));
    KOD->move(p2);
}
    
void NewPerecodirovka::SetTabOrderafterCreateComboBox()
{
    setTabOrder(NamePerecComboBox,Letter_01);
    setTabOrder(Letter_01,Letter_02);
    setTabOrder(Letter_02,Letter_03);
    setTabOrder(Letter_03,Letter_04);
    setTabOrder(Letter_04,Letter_05);
    setTabOrder(Letter_05,Letter_06);
    setTabOrder(Letter_06,Letter_07);
    setTabOrder(Letter_07,Letter_08);
    setTabOrder(Letter_08,Letter_09);
    setTabOrder(Letter_09,Letter_10);
    setTabOrder(Letter_10,Letter_11);
    setTabOrder(Letter_11,Letter_12);
    setTabOrder(Letter_12,Letter_13);
    setTabOrder(Letter_13,Letter_14);
    setTabOrder(Letter_14,Letter_15);
    setTabOrder(Letter_15,Letter_16);
    setTabOrder(Letter_16,Letter_17);
    setTabOrder(Letter_17,Letter_18);
    setTabOrder(Letter_18,Letter_19);
    setTabOrder(Letter_19,Letter_20);
    setTabOrder(Letter_20,Letter_21);
    setTabOrder(Letter_21,Letter_22);
    setTabOrder(Letter_22,Letter_23);
    setTabOrder(Letter_23,Letter_24);
    setTabOrder(Letter_24,Letter_25);
    setTabOrder(Letter_25,Letter_26);
    setTabOrder(Letter_26,Letter_27);
    setTabOrder(Letter_27,Letter_28);
    setTabOrder(Letter_28,Letter_29);
    setTabOrder(Letter_29,Letter_30);
    setTabOrder(Letter_30,Letter_31);
    setTabOrder(Letter_31,Letter_32);
    setTabOrder(Letter_32,Letter_33);
    setTabOrder(Letter_33,CopyCodButton);
    setTabOrder(CopyCodButton,Letter_34);
    setTabOrder(Letter_34,Letter_35);
    setTabOrder(Letter_35,Letter_36);
    setTabOrder(Letter_36,Letter_37);
    setTabOrder(Letter_37,Letter_38);
    setTabOrder(Letter_38,Letter_39);
    setTabOrder(Letter_39,Letter_40);
    setTabOrder(Letter_40,Letter_41);
    setTabOrder(Letter_41,Letter_42);
    setTabOrder(Letter_42,Letter_43);
    setTabOrder(Letter_43,Letter_44);
    setTabOrder(Letter_44,Letter_45);
    setTabOrder(Letter_45,Letter_46);
    setTabOrder(Letter_46,Letter_47);
    setTabOrder(Letter_47,Letter_48);
    setTabOrder(Letter_48,Letter_49);
    setTabOrder(Letter_49,Letter_50);
    setTabOrder(Letter_50,Letter_51);
    setTabOrder(Letter_51,Letter_52);
    setTabOrder(Letter_52,Letter_53);
    setTabOrder(Letter_53,Letter_54);
    setTabOrder(Letter_54,Letter_55);
    setTabOrder(Letter_55,Letter_56);
    setTabOrder(Letter_56,Letter_57);
    setTabOrder(Letter_57,Letter_58);
    setTabOrder(Letter_58,Letter_59);
    setTabOrder(Letter_59,Letter_60);
    setTabOrder(Letter_60,Letter_61);
    setTabOrder(Letter_61,Letter_62);
    setTabOrder(Letter_62,Letter_63);
    setTabOrder(Letter_63,Letter_64);
    setTabOrder(Letter_64,Letter_65);
    setTabOrder(Letter_65,Letter_66);
    setTabOrder(Letter_66,KOD);
    setTabOrder(KOD,SavePerecButton);
    setTabOrder(SavePerecButton,NewPerecButton);
    setTabOrder(NewPerecButton,DeletePerecButton);
    setTabOrder(DeletePerecButton,ExitButton);
}

void NewPerecodirovka::init()
{     
   // printf("void NewPerecodirovka::init()\n");
    kodflag = false;
    DefCodec = QTextCodec::codecForName(SYSCODEC);  
    CP1251Codec = QTextCodec::codecForName(HWCODEC);
    AddComboBoxes();
    SetTabOrderafterCreateComboBox();
    GenerateMassiv();
    if (KOD->currentItem()+1==2)
    {
	kodflag = true;
    }
    else
    {
	kodflag = false;
    }
   

    /*    QRegExp rx("^[A-Za-z0-9,./!?*+-)(&^%$#@'']{0,18}$");
    validator = new QRegExpValidator(rx,this);
    NameOfPerec->setValidator(validator);*/
    
    SetValto10();
    char* PName = (char *) calloc (14, sizeof(char));
    strcpy(PName, "PERECODIROVKA");
    int MaxResLen1 = 50;
    char * Val1 = (char *) calloc(MaxResLen1+1, sizeof(char));
    int i = 1;
    while (GetCodesParam(PName, Val1, 50,i)==1)
    {
	NamePerecComboBox -> insertItem(CP1251Codec->toUnicode(Val1),-1); 
	i++;
    }
    LoadPerecChanged(p);
    NamePerecComboBox->setCurrentItem(p);
    NamePerecComboBox-> setEnabled(TRUE);
    free(Val1);
    free(PName);
    if (NamePerecComboBox->count()<1)
    {
	SavePerecButton->setEnabled(FALSE);	
	DeletePerecButton->setEnabled(FALSE);
	KOD->setEnabled(FALSE);
    }
  QObject::connect (NamePerecComboBox, SIGNAL(activated(int)),this, SLOT(LoadPerecChanged(int)));
  QObject::connect (KOD, SIGNAL(activated(int)),this, SLOT(KOD_activated()));
  QObject::connect (CopyCodButton, SIGNAL(clicked()),this, SLOT(CopyCodButton_clicked()));
  QObject::connect (SavePerecButton, SIGNAL(clicked()),this, SLOT(SavePerecButton_clicked()));
  QObject::connect (NewPerecButton, SIGNAL(clicked()),this, SLOT(NewPerecButton_clicked()));
  QObject::connect (DeletePerecButton, SIGNAL(clicked()),this, SLOT(DeletePerecButton_clicked()));
  QObject::connect (ExitButton, SIGNAL(clicked()),this, SLOT(ExitButton_clicked()));
 
}


void NewPerecodirovka::closeEvent( QCloseEvent* e )
{
   // puts("void NewPerecodirovka::closeEvent( QCloseEvent* e )");
    e->accept();
    w->show();
    return;
}

void NewPerecodirovka::destroy()
{
    //puts("void NewPerecodirovka::destroy()");
    if (NamePerecComboBox->currentText()!=NULL)
	p1 = NamePerecComboBox->currentItem();
}

void NewPerecodirovka::DeletePerecButton_clicked()
{
   // puts("void NewPerecodirovka::DeletePerecButton_clicked()");
    QString er_str;
    QString er_id;
    if (!NamePerecComboBox->isEnabled())
    {
	NamePerecComboBox->setEnabled(TRUE);
	NamePerecComboBox->removeItem(NamePerecComboBox->currentItem());
	LoadPerecChanged(NamePerecComboBox->count()-1);
    }
    else
    { 
	if (NamePerecComboBox->count()==1)
	{
	    DeviceConfMessageText(12,er_str,er_id);
	    InterfaceMessageWin(er_str, er_id);	
	}
	else
	{		
	    switch(QMessageBox::information( this, DefCodec->toUnicode("Удаление перекодировки"), DefCodec->toUnicode("Вы точно желаете удалить перекодировку?"), DefCodec->toUnicode("Удалить"), DefCodec->toUnicode("Отмена"),  0, 1))
	    {
	    case 0:{ 
		    int c;
		    QString newstring;
		    char * str; 
		    str =  (char*)calloc(100,sizeof(char)); 
		    char* name = (char*) calloc(100,sizeof(char));
		    strcpy(name,"[PERECODIROVKA]");
		    *(name+strlen(name)) = 34; 
		    strcat(name,  CP1251Codec->fromUnicode(NamePerecComboBox ->currentText()));
		    *(name+strlen(name)) = 34; 
		    strcat(name, "\n"); 
		    NamePerecComboBox->removeItem(NamePerecComboBox->currentItem());
		    if ((fp = fopen(CODES_WAY,"r")) == NULL)
		    {
			//printf("ошибка при открытии файла '/etc/hwsrv/codes.tab'\n");
		    }
		    if ((fd = fopen(TMP_WAY,"w")) == NULL)
		    {
			//printf("ошибка при открытии файла '/etc/hwsrv/tmp.tab'\n");
		    }
		 //  puts("ПОКА НЕ ЗАКОНЧИТСЯ СODES_WAY копируем");
		    while (!feof(fp))
		    {
			if (strcmp(str, name)==0)
			{
			    for (int i = 0;i<66;i++)
			    { 
				fgets(str,50,fp);    
			    }
			}
			else
			{
			    fprintf(fd,str);
			}
			fgets(str,50,fp);
			newstring = DefCodec->toUnicode(CP1251Codec->fromUnicode(str));
			//printf("str = |%s|\n",str);
		    }
		   // puts("СКОПИРОВАЛИ ВСЕ ЧТО НАДО В TMP");
		    //printf("name = |%s|\n",name);
		    fclose(fd);
		    fclose(fp);
		    fp = fopen(CODES_WAY,"w");
		    fd = fopen(TMP_WAY,"r");
		    while ((c=fgetc(fd))!=EOF)
		    {
			fputc(c,fp);	
		    }
		    free(name); 
		    free(str);
		    fclose(fd);
		    fclose(fp); 
		    break;
		}
	    default:{
		  //  puts("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		   // puts("Pushed CANCEL");
		}
	    }
	    NewPerecButton->setEnabled(TRUE);
	    ExitButton->setEnabled(TRUE);
	    NamePerecComboBox->setCurrentItem(NamePerecComboBox->count()-1);
	    LoadPerecChanged(NamePerecComboBox->currentItem());
	}
    }
}


void NewPerecodirovka::SavePerecButton_clicked()
{	
   // printf("void NewPerecodirovka::SavePerecButton_clicked()\n");
    QString er_str;
    QString er_id;
    QString NameSavingPerec = DefCodec->fromUnicode(NamePerecComboBox->currentText());
   // cout<<"name = "<<NameSavingPerec<<endl;
    if ((strlen(Letter_01->text())==0)||(strlen(Letter_02->text())==0)
	||(strlen(Letter_03->text())==0)||(strlen(Letter_04->text())==0)
	||(strlen(Letter_05->text())==0)||(strlen(Letter_06->text())==0)
	||(strlen(Letter_07->text())==0)||(strlen(Letter_08->text())==0)
	||(strlen(Letter_09->text())==0)||(strlen(Letter_10->text())==0)
	||(strlen(Letter_11->text())==0)||(strlen(Letter_12->text())==0)
	||(strlen(Letter_13->text())==0)||(strlen(Letter_14->text())==0)
	||(strlen(Letter_15->text())==0)||(strlen(Letter_16->text())==0)
	||(strlen(Letter_17->text())==0)||(strlen(Letter_18->text())==0)
	||(strlen(Letter_19->text())==0)||(strlen(Letter_20->text())==0)
	||(strlen(Letter_21->text())==0)||(strlen(Letter_22->text())==0)
	||(strlen(Letter_23->text())==0)||(strlen(Letter_24->text())==0)
	||(strlen(Letter_25->text())==0)||(strlen(Letter_26->text())==0)
	||(strlen(Letter_27->text())==0)||(strlen(Letter_28->text())==0)
	||(strlen(Letter_29->text())==0)||(strlen(Letter_30->text())==0)
	||(strlen(Letter_31->text())==0)||(strlen(Letter_32->text())==0)
	||(strlen(Letter_33->text())==0)||(strlen(Letter_34->text())==0)
	||(strlen(Letter_35->text())==0)||(strlen(Letter_36->text())==0)
	||(strlen(Letter_37->text())==0)||(strlen(Letter_38->text())==0)
	||(strlen(Letter_39->text())==0)||(strlen(Letter_40->text())==0)
	||(strlen(Letter_41->text())==0)||(strlen(Letter_42->text())==0)
	||(strlen(Letter_43->text())==0)||(strlen(Letter_44->text())==0)
	||(strlen(Letter_45->text())==0)||(strlen(Letter_46->text())==0)
	||(strlen(Letter_47->text())==0)||(strlen(Letter_48->text())==0)
	||(strlen(Letter_49->text())==0)||(strlen(Letter_50->text())==0)
	||(strlen(Letter_51->text())==0)||(strlen(Letter_52->text())==0)
	||(strlen(Letter_53->text())==0)||(strlen(Letter_54->text())==0)
	||(strlen(Letter_55->text())==0)||(strlen(Letter_56->text())==0)
	||(strlen(Letter_57->text())==0)||(strlen(Letter_58->text())==0)
	||(strlen(Letter_59->text())==0)||(strlen(Letter_60->text())==0)
	||(strlen(Letter_61->text())==0)||(strlen(Letter_62->text())==0)
	||(strlen(Letter_63->text())==0)||(strlen(Letter_64->text())==0)
	||(strlen(Letter_65->text())==0)||(strlen(Letter_66->text())==0))
   {
	 DeviceConfErrorText(23, er_str, er_id);
	 InterfaceErrorWin(er_str, er_id);
	 return;
    }
    else
    {
	fp = fopen(CODES_WAY,"r");
	if(!fp)
	{
	  //  puts("WARNING!!! File '/etc/hwsrv/codes.tab' doesn't exist!!!");
	    fp = fopen(CODES_WAY,"a");
	    fclose(fp);
	    fp = fopen(CODES_WAY,"r");
	    QString ss;
	    bool ok;
	    QString text = QInputDialog::getText(DefCodec->toUnicode("Название перекодировки "), DefCodec->toUnicode("Название новой перекодировки:"), QLineEdit::Normal,ss, &ok, this );
	    if ( ok)  //если нажата кнопка ок 
	    {
		QString stroka;
		if (!text.isEmpty())//поле не пустое
		{
		//    printf("если нажата кнопка ок и поле не пустое\n");
		    stroka = DefCodec->fromUnicode(text);
		 //   printf("Новое имя = %s\n",stroka.ascii());
		    if ((stroka[0] =='!')||(stroka[0] =='?')||(stroka[0] =='/')||(stroka[0] ==' ')||(stroka[0] =='|')||
			(stroka[0] =='"')||(stroka[0] =='<')||(stroka[0] =='>')||(stroka[0] =='.')||(stroka[0] ==',')||
			(stroka[0] =='@')||(stroka[0] =='#')||(stroka[0] =='$')||(stroka[0] =='%')||(stroka[0] =='^')||
			(stroka[0] =='&')||(stroka[0] =='*')||(stroka[0] =='(')||(stroka[0] ==')')||(stroka[0] =='~')||
			(stroka[0] =='`')||(stroka[0] =='-')||(stroka[0] =='=')||(stroka[0] =='+'))
		    {
			//printf("ИМЯ ПЕРЕКОДИРОВКИ ВВЕДЕНО  НЕ ПРАВИЛЬНО : %s\n", stroka.ascii()); 
			DeviceConfErrorText(24, er_str, er_id);
			InterfaceErrorWin(er_str, er_id);
		    }
		    else 
		    {
			NamePerecComboBox->insertItem(DefCodec->toUnicode(stroka),-1);
			NamePerecComboBox->setCurrentItem(NamePerecComboBox->count()-1);
		    }
		}
	    }	
	    else
	    {
		//printf("если нажата кнопка ок и поле с именем пустое\n");
		DeviceConfErrorText(25, er_str, er_id);     
		InterfaceErrorWin(er_str, er_id);
		
	    }
	    if (!ok)//если нажата кнопка отмена
	    {
		//printf("нажата кнопка отмена\n");
	    }
	}
	char * str; 
	str =  (char*)calloc(100,sizeof(char)); 
	char* name = (char*) calloc(19 +NamePerecComboBox ->currentText().length() ,sizeof(char));
	strcpy(name,"[PERECODIROVKA]");
	*(name+strlen(name)) = 34; 
	strcat(name,  CP1251Codec->fromUnicode(NamePerecComboBox ->currentText()));
	*(name+strlen(name)) = 34; 
	strcat(name, "\n"); 
    
	fd = fopen(TMP_WAY,"w");
	if(!fd)
	{
	//   puts("WARNING!!! File '/tmp/tmp.tab' doesn't exist!!!");
	    fd = fopen(TMP_WAY,"a");
	    fclose(fd);
	    fd = fopen(TMP_WAY,"w");
	}
	else
	{
	    while (!feof(fp))
	    {
		fgets(str,100,fp); 
		if(strcmp(str, name)==0)
		{
		    for (int i = 0;i<67;i++)
		    {
			fgets(str,100,fp); 
			//printf("str= %s \n",str);
		    }
		}
		else
		{
		    fprintf(fd,str);
		}
	    }
	    fclose(fd);
	    fclose(fp);
	    fp = fopen(CODES_WAY,"w");
	    fd = fopen(TMP_WAY,"r");
	    int c;
	    while ((c=fgetc(fd))!=EOF)
	    {
		fputc(c,fp);	
	    }
	    free(name); 
	    free(str);
	    fclose(fd);
	    fclose(fp); 
	    fd = fopen(TMP_WAY,"w");
	    WriteConf(fd);
	    fclose(fd);
	    fp =   fopen(CODES_WAY,"a");
	    fd =   fopen(TMP_WAY,"r");
	    while ((c=fgetc(fd))!=EOF)
	    {
		fputc(c,fp);	
	    }
	} 	
	fclose(fd);
	fclose(fp);	    
	NamePerecComboBox->setCurrentItem(NamePerecComboBox->count()-1);
	LoadPerecChanged(NamePerecComboBox->currentItem());
	NamePerecComboBox->setEnabled(TRUE);
    }
    NewPerecButton->setEnabled(TRUE);
    ExitButton->setEnabled(TRUE);
    DeletePerecButton->setEnabled(TRUE);
    GetNumberPerec();
    NamePerecComboBox->clear();
    Codes();
    DeleteEmptyString();
    DeviceConfMessageText(14,er_str, er_id);
    InterfaceMessageWin(er_str, er_id);
    for ( int k = 0;k<NamePerecComboBox->count();k++)
    {
	QString a = DefCodec->fromUnicode(NamePerecComboBox->text(k));
	if (NameSavingPerec == a)
	{
	    NamePerecComboBox->setCurrentItem(k);
	    p1 = k;
	}
    }  
}


void NewPerecodirovka::WriteConf(FILE* file)
{   
   // printf("int NewPerecodirovka::WriteConf(FILE* file)\n");
    char* NS = (char*) calloc(99,sizeof(char));
    strcpy(NS,"[PERECODIROVKA]");
    *(NS+strlen(NS)) = 34; 
    strcat(NS, CP1251Codec->fromUnicode(NamePerecComboBox ->currentText()));
    *(NS+strlen(NS)) = 34; 
   // printf("NS = %s\n",NS);
    fprintf(file,"\n\n");
    fprintf(file,NS);
    free(NS);
    bool ok;
    for (int i =0;i<66;i++)
    {
    char *str;
    str = (char*) calloc(8,sizeof(char));
    strcpy(str,"["); 
    strcat(str,int_to_str(i+1));
    strcat(str,"] ");
    *(str+strlen(str)) = 34; 
    if (kodflag == false)
	strcat(str,massiv[i]->text().ascii());
    else
	strcat(str,int_to_str( massiv[i]->text().toInt(&ok,16)));
    *(str+strlen(str)) = 34; 
    fprintf(file,"\n");
    fprintf(file,str);
   // printf("str =%s\n",str);
    free(str);
}
   // printf("WriteConf завершил свою работу!!!\n");
}


void NewPerecodirovka::LoadPerecChanged( int NumberPerec)
{
//    printf("void NewPerecodirovka::LoadPerecChanged( int NumberPerec)\n");
 //   printf("Загружена перекодировка под номером = %d\n",NumberPerec);
    char *MASSIV = (char *) calloc(10,sizeof(char));
     char * c = (char *) calloc(4,sizeof(char));
     for (int j=0;j<66;j++)
     {
	 strcpy(c,int_to_str(j+1));
	 GetStrParamForCodes(NumberPerec+1,c, MASSIV, 10);
	 if (kodflag == false)
	     massiv[j]->setText(MASSIV);
	 else
	     massiv[j]->setText(QString::number(atoi(MASSIV),16));
     }
     free(c);
     free(MASSIV);
 }

void NewPerecodirovka::CopyCodButton_clicked()
{
   // printf("void NewPerecodirovka::CopyCodButton_clicked()\n");

for (int i =0;i<33;i++)
    {
     massiv[i+33]->setText(massiv[i]->text().ascii());
    }
}

void NewPerecodirovka::KOD_activated()
{
  //  printf("Изменили систему счисления\n");
    if ((KOD->currentItem()+1==2) && (kodflag == false))
    {
//	printf("kodflag = true\n");
	kodflag = true;
	SetValto16();
	
	for (int i =0;i<66;i++)
	{
	    if (!massiv[i]->text().isEmpty())
	    {
		massiv[i]->setText(QString::number(atoi(massiv[i]->text().ascii()),16));
	    }
	    else
		massiv[i]->setText("");
	}
    }
   if ((KOD->currentItem()+1==1) && (kodflag == true))
    {
	kodflag = false;
	//printf("kodflag = false\n");	
	SetValto10();
	bool ok;     
	for (int i =0;i<66;i++)
	{
	    massiv[i]->setText(int_to_str(massiv[i]->text().toInt(&ok,16)));
	}
    }
}


void NewPerecodirovka::ExitButton_clicked()
{
  //  printf("void NewPerecodirovka::ExitButton_clicked()\n");
    p1=NamePerecComboBox->currentItem();
   // printf("p1 = %d\n",p1);
    close();
}


void NewPerecodirovka::NewPerecButton_clicked()
{ 
	 QString er_str;
	 QString er_id;
	 SetValto10();
	 KOD->setCurrentItem(0);
	 int c;
	 bool saveflag = false;
	 QString stroka;
    
	 NewSchemePerec * nschemeperec = new NewSchemePerec();
	 char * sname = (char *)calloc(100, sizeof(char));
	 strcpy(sname,"");
	 nschemeperec->setNewSchemeName(sname);
	 if (nschemeperec->exec() != QDialog::Accepted)
	 { 
		  //puts("Пользователь нажал отмену!!!");
		  return;
	 }
	 else
	 {
		  QString scheme_name = nschemeperec->getSchemeName();
		  //printf("scheme_name = |%s|\n",scheme_name.ascii());
		  strcpy(sname, scheme_name.ascii());
		  nschemeperec->close();
		  delete nschemeperec;	
	 }
	 stroka = sname;
	 for (int h = 0;h<(NamePerecComboBox->count());h++)
	 {
		//  printf("Зашли в проверку\n");
		  QString str = DefCodec->fromUnicode(NamePerecComboBox->text(h));
		  if  (stroka == str)		   
		  {
				// printf("Строки равны\n"); 
				DeviceConfErrorText(13, er_str, er_id);
				InterfaceErrorWin(er_str, er_id);
				saveflag = true;
				return;
		  }     
	 } 
    
	 int i = FileAcces(CODES_WAY);
	// printf("i = %d\n",i);
	 switch(i){  
	 case (-14):{
				DeviceConfErrorText(32, er_str, er_id);
				InterfaceErrorWin(er_str, er_id);
				saveflag = true;
				return;
		  }
	 case (-11):{
				DeviceConfErrorText(31, er_str, er_id);
				InterfaceErrorWin(er_str, er_id);
				saveflag = true;
				return;
		  }
	 case 0:{ 
				FILE * fp  = fopen(CODES_WAY,"r");
				char* name = (char*) calloc(100,sizeof(char));
				strcpy(name,"[PERECODIROVKA]");
				*(name+strlen(name)) = 34; 
				strcat(name, CP1251Codec->fromUnicode(stroka));
				*(name+strlen(name)) = 34; 
				strcat(name, "\n"); 
				char *str = (char *)calloc(100,sizeof(char));
				while (!feof(fp))
				{
					 fgets(str,50,fp);
					 if(strcmp(str, name)==0)
					 {
						  DeviceConfMessageText(13, er_str, er_id);
						  InterfaceMessageWin(er_str, er_id);
						  saveflag = true;
						  break;
					 }  
				}
				free(str);
				free(name);
				break;
		  }
	 default:{
				DeviceConfErrorText(33, er_str, er_id);
				InterfaceErrorWin(er_str, er_id);
				saveflag = true;
				return;	
		  }
	 }

	 
     if (saveflag == false)
     {
	 NamePerecComboBox->insertItem(DefCodec->toUnicode(stroka),-1);
	// printf("NamePerecComboBox->count() = %d\n",NamePerecComboBox->count());
	 NamePerecComboBox->setCurrentItem(NamePerecComboBox->count()-1);
	 LoadPerecChanged(NamePerecComboBox->currentItem());
	 fd = fopen(TMP_WAY,"w");
	 clearLetters();
	 fclose(fd);
	 fd = fopen(TMP_WAY,"r");
	 fp = fopen(CODES_WAY,"a");
	 while (( c=fgetc(fd))!=EOF)
	 {
	     fputc(c,fp);	
	 }
	 fclose(fd);
	 fclose(fp);
	 NewPerecButton->setEnabled(FALSE);
	 ExitButton->setEnabled(FALSE);
	 SavePerecButton->setEnabled(TRUE);
	 NamePerecComboBox->setEnabled(FALSE);
	 KOD->setEnabled(TRUE);
     }
}    

void NewPerecodirovka::clearLetters()
{
    //printf("void NewPerecodirovka::clearLetters()\n");
 
    for (int i =0;i<66;i++)
    {
	massiv[i]->setText("");
    }
}

void NewPerecodirovka::SetValto16(void)
{
    //puts("void NewPerecodirovka::SetValto16(void)");
    QRegExp rq("^[A-Za-z0-9]{0,3}$");
    val1 = new QRegExpValidator(rq,this);
    for (int i =0;i<66;i++)
    {
	massiv[i]->setValidator(val1);
    }
}	

void NewPerecodirovka::SetValto10(void)
{
    //puts("void NewPerecodirovka::SetValto10(void)");
    val = new QIntValidator(0, 255, this);
    for (int i =0;i<66;i++)
    {
	massiv[i]->setValidator(val);
    }
}
int  NewPerecodirovka::GetNumberPerec(void)
{
    int t= 0;
    for (int i = 0; i<(NamePerecComboBox->count());i++)
    {
	t = NamePerecComboBox->currentItem();
    }
    return t;
}


void NewPerecodirovka::GenerateMassiv()
{
massiv = (QLineEdit **)calloc(67,sizeof(QLineEdit*));
massiv[0]=Letter_01;
massiv[1]=Letter_02;
massiv[2]=Letter_03;
massiv[3]=Letter_04;
massiv[4]=Letter_05;
massiv[5]=Letter_06;
massiv[6]=Letter_07;
massiv[7]=Letter_08;
massiv[8]=Letter_09;
massiv[9]=Letter_10;
massiv[10]=Letter_11;
massiv[11]=Letter_12;
massiv[12]=Letter_13;
massiv[13]=Letter_14;
massiv[14]=Letter_15;
massiv[15]=Letter_16;
massiv[16]=Letter_17;
massiv[17]=Letter_18;
massiv[18]=Letter_19;
massiv[19]=Letter_20;
massiv[20]=Letter_21;
massiv[21]=Letter_22;
massiv[22]=Letter_23;
massiv[23]=Letter_24;
massiv[24]=Letter_25;
massiv[25]=Letter_26;
massiv[26]=Letter_27;
massiv[27]=Letter_28;
massiv[28]=Letter_29;
massiv[29]=Letter_30;
massiv[30]=Letter_31;
massiv[31]=Letter_32;
massiv[32]=Letter_33;
massiv[33]=Letter_34;
massiv[34]=Letter_35;
massiv[35]=Letter_36;
massiv[36]=Letter_37;
massiv[37]=Letter_38;
massiv[38]=Letter_39;
massiv[39]=Letter_40;
massiv[40]=Letter_41;
massiv[41]=Letter_42;
massiv[42]=Letter_43;
massiv[43]=Letter_44;
massiv[44]=Letter_45;
massiv[45]=Letter_46;
massiv[46]=Letter_47;
massiv[47]=Letter_48;
massiv[48]=Letter_49;
massiv[49]=Letter_50;
massiv[50]=Letter_51;
massiv[51]=Letter_52;
massiv[52]=Letter_53;
massiv[53]=Letter_54;
massiv[54]=Letter_55;
massiv[55]=Letter_56;
massiv[56]=Letter_57;
massiv[57]=Letter_58;
massiv[58]=Letter_59;
massiv[59]=Letter_60;
massiv[60]=Letter_61;
massiv[61]=Letter_62;
massiv[62]=Letter_63;
massiv[63]=Letter_64;
massiv[64]=Letter_65;
massiv[65]=Letter_66;
}


void NewPerecodirovka::Codes()
{
    FILE *cod;
    cod= fopen(CODES_WAY,"r");
    if ((access(CODES_WAY,4))==-1)
    {
	//printf("file couldn't be read  CODES_WAY\n");
	return;
    }
    char* PName = (char *) calloc (14, sizeof(char));
    strcpy(PName, "PERECODIROVKA");
    int MaxLen1 = 13;
    int MaxResLen1 = 274;
    char * Buf1 = (char *) calloc (MaxLen1+1, sizeof(char));  
    char * Val1 = (char *) calloc(MaxResLen1+1, sizeof(char));
    int i = 1;
    while (GetCodesParam(PName, Val1, 274,i)==1)
    {
	NamePerecComboBox -> insertItem(CP1251Codec->toUnicode(Val1),-1); 
	i++;
    }
   
    NamePerecComboBox->currentItem();
    free(Buf1);
    free(Val1);
    free(PName);
    fclose(cod);
}


void NewPerecodirovka::keyPressEvent(QKeyEvent * k)
{	
    //puts("void NewPerecodirovka::keyPressEvent(QKeyEvent * k)");
    switch(k->key())   
    {
    case Qt::Key_F12 :
	//puts("Qt::Key_F12");
	close();
	reject();
	break;
    case Qt::Key_F2 :
	   //puts("Qt::Key_F2"); 
	   SavePerecButton_clicked();
	   break;
    case Qt::Key_F7 :
	   //puts("Qt::Key_F7"); 
	   NewPerecButton_clicked();
	   break;
    case Qt::Key_F8 :
	   //puts("Qt::Key_F8"); 
	   DeletePerecButton_clicked();
	   break;
       default :{}
    }
}



void NewPerecodirovka::DeleteEmptyString()
{
    //printf("void NewPerecodirovka::DeleteEmptyString()\n");
    char * str =(char *) calloc(100,sizeof(char));
    char * strbeforestr =(char *) calloc(100,sizeof(char));
   FILE * fk;
   FILE * ft;
   fk = fopen(CODES_WAY,"r");
   ft = fopen(TMP_WAY,"w");
   int probel = 0;
   while (fgets(str,100,fk)!=NULL)
   {
       if ((strcmp(str,"\n")==0) && (strcmp(strbeforestr,"\n")==0))
       {
	probel++;   
       }
         if ((strcmp(str,"\n") !=0) && (strcmp(strbeforestr,"\n")==0))
       {
	probel=0;   
       }
       if (probel<3)
       {
	   fprintf(ft,str);
       }
	   strcpy(strbeforestr,str);
   }
   fclose(ft);
   fclose(fk);
   strcpy(str,"");
   fk = fopen(CODES_WAY,"w");
   ft = fopen(TMP_WAY,"r");
    while (fgets(str,100,ft)!=NULL)
   {
	   fprintf(fk,str);
    }
   fclose(ft);
   fclose(fk);
   free(str);
   free(strbeforestr);
}

