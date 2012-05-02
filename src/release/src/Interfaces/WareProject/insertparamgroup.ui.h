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
#include "reference_book_class_pgsql.h"
#include "addon_func.h"
#include "hwsrv.h"
#include "dkbinput.h"
#include "bcskbinput.h"
#include "messages.h"
#include "mcrkbinput.h"
#include <cstdlib>

extern PgRef M;
extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;
extern HWSRV  * hw;
extern int Group_id;

int InsertParamGroupId;

QString newname1; //используется только при редактировании

void InsertParamGroup::init()
{
//    puts("void InsertParamGroup::init()");
    systemCodec = QTextCodec::codecForName(SYSCODEC);
    dbaseCodec = QTextCodec::codecForName(DBCODEC);
 //   printf("TypeForm = %d\n",TypeForm);
    SetValidatorProperties();
    ChangeParent->hide();
    ParentLabel->hide();
    ParentGroupNameEdit->hide();
    grabKeyboard();
    releaseKeyboard();
    this->adjustSize();
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
   // grabKeyboard();
}

void InsertParamGroup::closeEvent( QCloseEvent* e )
{
    grabKeyboard();
//    puts("void InsertParamGroup::closeEvent( QCloseEvent* e )");
    e->accept();
    if (exit_flag == true)
    {
//	puts("exit_flag == true");
	char * s = (char *) calloc(20,sizeof(char));
	char * s1 = (char *) calloc(201,sizeof(char));
//	printf("firstname = |%s|\n",firstname.ascii());
//	printf("newname1 = |%s|\n",newname1.ascii());
	/*if (firstname!="0")
	    puts("%%%%%%%%%%%%%%%");
	else
	    puts("________________________________");*/
	if ((!firstname.isEmpty()) && (!firstname.isNull()) && (firstname!="0"))
	{
	    M.GetGroupId(firstname.ascii(),s);
//	    printf("s = %s\n",s);
	    M.GetParentIdByGroupId(s,s1);
//	    printf("s1 = %s\n",s1);
	    InsertParamGroupId = atoi(s1);
	}
	else
	{
	    InsertParamGroupId = 0;
	}
	free(s);	
	free(s1);	
    }
    else
    {
	//puts("exit_flag == false");
	
	QString s = dbaseCodec->fromUnicode(ParentGroupNameEdit->text());
	if (ParentGroupNameEdit->text()!=systemCodec->toUnicode(WITHOUTGROUP))
	{
	    char * str = (char *) calloc(20,sizeof(char));
	    M.GetGroupId(s.ascii(),str);
	    InsertParamGroupId =atoi(str);
	    free(str);
	}
	else
	    InsertParamGroupId =0;
    }
    return;
}

void InsertParamGroup::SetValidatorProperties(void)
{
  //  puts("void InsertParamGroup::SetValidatorProperties(void)");
    QRegExp titlerx("(\\w|[ ,.-%]){0,99}");
    titlevalidator = new QRegExpValidator(titlerx, this);
    LongtitleEdit->setValidator(titlevalidator);
    ShortcutEdit->setValidator(titlevalidator);
}



void InsertParamGroup::GetTypeForm(int Par, QString Longtitle)
{
//    puts("void InsertParamGroup::  GetTypeForm(void)");  
    TypeForm = Par;
 //   printf("Longtitle = %s\n",Longtitle.ascii());
//    printf("TypeForm = %d\n",TypeForm);
    firstname = Longtitle;
    switch(Par)   
    {
    case 1 :
	{
//	    puts("Тип схемы = создание группы товаров");
	    LongtitleLabel->setText(systemCodec->toUnicode("Наименование группы товаров"));
	    ShortcutLabel->setText(systemCodec->toUnicode("Наименование группы товаров для печати на документах"));
	    p_group_id = Longtitle;
	//    printf("p_group_id  = %s\n", p_group_id.ascii());
	    break;
	}
    case 2:
	{
	//    puts("Тип схемы = редактирование группы товаров");
	    LongtitleLabel->setText(systemCodec->toUnicode("Наименование группы товаров"));
	    ShortcutLabel->setText(systemCodec->toUnicode("Наименование группы товаров для печати на документах"));
	    char * value = (char *)calloc(201,sizeof(char));
	    M.GetWareGroupShortCutByLongtitle(Longtitle.ascii(),value);
	    LongtitleEdit->setText(dbaseCodec->toUnicode(Longtitle));
	    ShortcutEdit->setText(dbaseCodec->toUnicode(value));
	    ChangeParent->show();  
	    ParentLabel->show();
	    ParentGroupNameEdit->show();
	    this->adjustSize();
	    free(value);
	    char * group_id = (char *)calloc(20,sizeof(char));
	    char * parent_group_id = (char *)calloc(20,sizeof(char));
	    char * longtitle = (char *)calloc(201,sizeof(char));
	    M.GetGroupId(Longtitle, group_id);
	 //   printf("group_id = %s\n",group_id);
	    M.GetParentIdByGroupId(group_id, parent_group_id);
	 //   printf("parent_group_id = %s\n",parent_group_id);
	    free(group_id);
	    if (strcmp(parent_group_id,"")==0)
	    {
		ParentGroupNameEdit->setText(systemCodec->toUnicode(WITHOUTGROUP));
		p_group_id = "0";
	    }
	    else
	    {
		p_group_id = parent_group_id;
		M.GetLongtitleByGroupId(parent_group_id, longtitle);
	//	printf("longtitle = %s\n",longtitle);
		ParentGroupNameEdit->setText(dbaseCodec->toUnicode(longtitle));
		free(longtitle);
	    }
	    free(parent_group_id);
	 //   printf("p_group_id = %s\n",p_group_id.ascii());
	    break;
	}
    case 3:
	{
	//    puts("Тип схемы = создание налоговой группы");
	    LongtitleLabel->setText(systemCodec->toUnicode("Наименование налоговой группы"));
	    ShortcutLabel->setText(systemCodec->toUnicode("Наименование налоговой группы для печати на документах"));
	    break; 
	}
    case 4:
	{
	//    puts("Тип схемы = редактирование налоговой группы");
	    LongtitleLabel->setText(systemCodec->toUnicode("Наименование налоговой группы"));
	    ShortcutLabel->setText(systemCodec->toUnicode("Наименование налоговой группы для печати на документах"));
	    char * value = (char *)calloc(201,sizeof(char));
	    M.GetTaxGroupShortCutByLongtitle(Longtitle.ascii(),value);
	    LongtitleEdit->setText(dbaseCodec->toUnicode(Longtitle));
	    ShortcutEdit->setText(dbaseCodec->toUnicode(value));
	    free(value);
	    break;
	}
    case 5:
	{
	//    puts("Тип схемы = создание схемы автоматических скидок");
	    LongtitleLabel->setText(systemCodec->toUnicode("Наименование схемы автоматических скидок"));
	    ShortcutLabel->setText(systemCodec->toUnicode("Наименование схемы автоматических скидок для печати на документах"));
	    break;
	}
    case 6:
	{
	//    puts("Тип схемы = редактирование схемы автоматических скидок");
	    LongtitleLabel->setText(systemCodec->toUnicode("Наименование схемы автоматических скидок"));
	    ShortcutLabel->setText(systemCodec->toUnicode("Наименование схемы автоматических скидок для печати на документах"));
	    char * value = (char *)calloc(201,sizeof(char));
	    M.GetAutoSchemeShortCutByLongtitle( Longtitle.ascii(),value);
	    LongtitleEdit->setText(dbaseCodec->toUnicode(Longtitle));
	    ShortcutEdit->setText(dbaseCodec->toUnicode(value));
	    free(value);
	    break;
	}
    } 
}
  

void InsertParamGroup::OK_clicked()
{
  //  puts("void InsertParamGroup::OK_clicked()");
    switch (TypeForm)
    {
    case 1:
	{
	    FormCreateWareGroup();
	    break;
	}
    case 2:
	{
	    FormEditWareGroup();
	    break;
	}
    case 3:
	{
	    FormCreateTaxGroup();
	    break;
	}
    case 4:
	{
	    FormEditTaxGroup();
	    break;
	}
    case 5:
	{
	    FormCreateAutoRed();
	    break;
	}
    case 6:
	{
	    FormEditAutoRed();
	    break;
                }	
    }
}
void InsertParamGroup::FormCreateWareGroup(void)
{
  //  puts("void InsertParamGroup::FormCreateWareGroup(void)");
    QString er_str;
    QString er_id;
    QString name = dbaseCodec->fromUnicode(LongtitleEdit->text());
    QString shortname = dbaseCodec->fromUnicode(ShortcutEdit->text());
    QString NULLSTR = "NULL";
    if (name.isEmpty())
    {
	WareReferenceErrorText(40, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return ;
    }
    if (shortname.isEmpty())
    {
	WareReferenceErrorText(42, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return ;
    }  
    char * value = (char *)calloc(20,sizeof(char));
    M.GetGroupId(name.ascii(),value);
    QString str =  value;
    free(value);
  //  printf("функция   _GetGroupId_   вернула значение = %s\n",str.ascii());
    if (str.isEmpty())
    {
//	puts("такого имени для группы товаров еще не было");	
	int g = -4;
//	printf("p_group_id = %s\n",p_group_id.ascii());
	if (p_group_id != "0")
	{
//	    puts("p_group_id != 0");
	    g = M.CreateNewGroupOfWare(name.ascii(), shortname.ascii(), p_group_id.ascii());
	}
	else
	{
//	    puts("p_group_id == 0");
	    g = M.CreateNewGroupOfWare(name.ascii(), shortname.ascii(),NULLSTR.ascii());
	}
	newname1 = name;
//	printf("newname1 = %s\n",newname1.ascii());
	if (g ==1)
	{
//	    puts("В ОБЩЕМ-ТО ВСЕ В ПОРЯДКЕ. СОЗДАЛИ ГРУППУ ТОВАРОВ!!!!");
	    WareReferenceMessageText(16,er_str,er_id);		
	    InterfaceMessageWin(er_str,er_id);
	    close();
	    return;
	}
    }
    else
    {
	WareReferenceErrorText(46, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
    }  
}

void InsertParamGroup::FormEditWareGroup(void)
{
 //   puts("void InsertParamGroup::FormEditWareGroup(void)");
    exit_flag = false;
    char * group_id = (char *)calloc(20,sizeof(char));
    M.GetGroupId(firstname.ascii(), group_id);
  //  printf("group_id = %s\n",group_id);
    
    QString longtitle = dbaseCodec->fromUnicode(LongtitleEdit->text());
    QString shortcut = dbaseCodec->fromUnicode(ShortcutEdit->text());
    QString name_p_group_id = dbaseCodec->fromUnicode(ParentGroupNameEdit->text());
    QString er_str;
    QString er_id;
    if ((name_p_group_id == longtitle) || (name_p_group_id == firstname))
    {
	free(group_id);
//	puts("Имя группы не может совпадать с именем родительской группы");
	WareReferenceErrorText(47, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	close();
    }
    else
    {
//	puts("@@@@@@@@@@@@@@@@@@@@");
	char * parent_group_id = (char *)calloc(20,sizeof(char));
	M.GetGroupId(name_p_group_id, parent_group_id);
//	printf("parent_group_id = %s\n",parent_group_id);
	if ((strcmp(parent_group_id,"0")==0)||(strcmp(parent_group_id,"")==0))
	    strcpy(parent_group_id,"NULL");
	
//	printf("group_id = %s\n",group_id);
	int g = M.UpdateWareGroup(longtitle.ascii(), shortcut.ascii(), parent_group_id, group_id);
//	printf("g = %d\n",g);
	free(group_id);
	free(parent_group_id);
	
	if (g == 0)
	{
	    WareReferenceMessageText(17,er_str,er_id);		
	    InterfaceMessageWin(er_str,er_id);
	    newname1 = longtitle;
	    close();
	}
	else
	    closeForm();
    }
}



void InsertParamGroup::ChangeParent_clicked()
{
  //  puts("void InsertParamGroup::ChangeParent_clicked()");
    w_g = new WareGroupform;	 
    w_g->Number_Group = p_group_id.toInt();
    w_g->exec();
    delete(w_g);
  //  printf("Group_id = %d\n",Group_id);
    if (Group_id !=0)
    {
	QString str = itoa(Group_id);
//	printf("str = |%s|\n",str.ascii());
	char * value = (char *) calloc(201,sizeof(char));
	M.GetNameWareGroupById(str.ascii(),value);
//	printf("value = |%s|\n",value);
	if (strcmp(value,"ERROR")!=0)
	    ParentGroupNameEdit->setText(dbaseCodec->toUnicode(value)); 
	free(value);
    }
    else
	ParentGroupNameEdit->setText(systemCodec->toUnicode(WITHOUTGROUP));
}



void InsertParamGroup::FormCreateTaxGroup(void)
{
//    puts("void InsertParamGroup::FormCreateTaxGroup(void)");
    QString er_str;
    QString er_id;
    QString name = dbaseCodec->fromUnicode(LongtitleEdit->text());
    QString shortname = dbaseCodec->fromUnicode(ShortcutEdit->text());
    if (name.isEmpty())
    {
	WareReferenceErrorText(9, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return ;
    }
    if (shortname.isEmpty())
    {
	WareReferenceErrorText(10, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return ;
    }   
    char * value = (char *)calloc(20,sizeof(char));
    M.GetTaxGroupId(name.ascii(),value);
    QString str =  value;
    free(value);
//    printf("функция   _GetTaxGroupId_   вернула значение = %s\n",str.ascii());
    if (str.isEmpty())
    {
//	puts("такого имени для налоговой группы скидок еще не было ");
	int res = M.CreateNewTaxGroup(name.ascii(),shortname.ascii());
//	printf("RES = %d\n",res);
	if (res == 1)
	{
	    WareReferenceMessageText(9,er_str,er_id);		
	    InterfaceMessageWin(er_str,er_id);
//	    puts("создали новую группу авт.скидок");
	    close();
	}
	else
	{
	    WareReferenceErrorText(10, er_str, er_id);
	    InterfaceErrorWin(er_str, er_id);
	    return ;
	}
    }
    else
    {
	WareReferenceErrorText(36, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return;	
    }
}

void InsertParamGroup::FormEditTaxGroup(void)
{
 //   puts("void InsertParamGroup::FormEditTaxGroup(void)");
    QString er_str;
    QString er_id;
    QString name = dbaseCodec->fromUnicode(LongtitleEdit->text());
    QString shortname = dbaseCodec->fromUnicode(ShortcutEdit->text());
    if (name.isEmpty())
    {
	WareReferenceErrorText(9, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return;
    }
    if (shortname.isEmpty())
    {
	WareReferenceErrorText(10, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return;
    }
    
 //   printf("firstname = %s\n",firstname.ascii());
    char * value = (char *)calloc(201,sizeof(char));
    M.GetTaxGroupId(firstname.ascii(),value);
    QString L = value;
    free(value);
    int i =M.UpdateTaxGroup(name.ascii(), shortname.ascii(), L.ascii());
    if (i!=-1)
    {
	WareReferenceMessageText(5,er_str,er_id);
	InterfaceMessageWin(er_str,er_id);
	newname1 = name;
	close();
    }
    else
	closeForm();
}

void InsertParamGroup::FormCreateAutoRed(void)
{
 //   puts("void InsertParamGroup::FormCreateAutoRed(void)");
    QString er_str;
    QString er_id;
    QString name = dbaseCodec->fromUnicode(LongtitleEdit->text());
    QString shortname = dbaseCodec->fromUnicode(ShortcutEdit->text());
    if (name.isEmpty())
    {
	WareReferenceErrorText(7, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return ;
    }
    if (shortname.isEmpty())
    {
	WareReferenceErrorText(8, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return ;
    }   
    char * value = (char *)calloc(20,sizeof(char));
    M.GetAutoSchemeId(name.ascii(),value);
    QString str =  value;
    free(value);
  //  printf("функция   _GetAutoSchemeId_   вернула значение = %s\n",str.ascii());
    if (str.isEmpty())
    {
//	puts("такого имени для группы автоматических скидок еще не было ");
	int res = M.Create_Auto_Reduction_Group(name.ascii(),shortname.ascii());
//	printf("RES = %d\n",res);
	if (res == 1)
	{
	    WareReferenceMessageText(7,er_str,er_id);		
	    InterfaceMessageWin(er_str,er_id);
//	    puts("создали новую группу авт.скидок");
	    close();
	}
	else
	{
	    WareReferenceErrorText(2, er_str, er_id);
	    InterfaceErrorWin(er_str, er_id);
	    return ;
	}
    }
    else
    {
	WareReferenceErrorText(33, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return;	
    }
}

void InsertParamGroup::FormEditAutoRed(void)
{
  //  puts("void InsertParam::FormEditAutoRed(void)");
    QString er_str;
    QString er_id;
    QString name = dbaseCodec->fromUnicode(LongtitleEdit->text());
    QString shortname = dbaseCodec->fromUnicode(ShortcutEdit->text());
    if (LongtitleEdit->text().isEmpty())
    {
	WareReferenceErrorText(7, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return;
    }
    if (ShortcutEdit->text().isEmpty())
    {
	WareReferenceErrorText(8, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return;
    }
//    printf("firstname = %s\n",firstname.ascii());
    char * value = (char *)calloc(20,sizeof(char));
    M.GetAutoSchemeId(firstname.ascii(),value);
    int g = M.UpdateAutoScheme(value , name.ascii(), shortname.ascii());
//    printf("g = %d\n",g);
    if (g == 0)
    {
	QString mes_str;
	QString mes_id;
	WareReferenceMessageText(4,mes_str,mes_id);
	InterfaceMessageWin(mes_str,mes_id);
    } 
    free(value);
    newname1 = name;
    close();
}


void InsertParamGroup::closeForm()
{
//    puts("void InsertParamGroup::closeForm()");
    exit_flag = true;
    newname1 = firstname;
    close();
}

//--------------------------------------------------------- KeyBoard  begin---------------------------------------------------------------------/
void InsertParamGroup::GetKeySequence()
{

    if ( kbcode_scaner != NULL ) {
	int n = kbcode_scaner->GetCountEvent();
	for ( int i = 0; i < n; i++ )   {
	    makeKeyPress( kbcode_scaner->GetEvent(i) );	}
	kbcode_scaner->Clear();
    }
    
    if ( kbcode_reader != NULL ) {
	int n = kbcode_reader->GetCountEvent();
	for ( int i = 0; i < n; i++ )   {
	    makeKeyPress( kbcode_reader->GetEvent(i) );	}
	kbcode_reader->Clear();
    }
}


void InsertParamGroup::keyPressEvent(QKeyEvent *k)
{
    int state = 0;
    if ( hw->BCS_GetType() == 2 )  {
	state = kbcode_scaner->FindCode(k);
	if ( state == 2 )  {
	    ;
	}
    }    
    
    if ( hw->MCR_GetType() == 2 && state == 0 ) {
	state = kbcode_reader->FindCode(k);
	if ( state == 2 ) {
	    ;
	}
    }
    
    if ( state == 0 ) {   makeKeyPress(k); return;  }
    if ( state == 5 )     { GetKeySequence();  return;  }
}

void InsertParamGroup::makeKeyPress(QKeyEvent *k)
{
  //  puts("makeKeyPress");
    switch(k->key())   
    {
    case Qt::Key_F12 :
	{
	//    puts("Qt::Key_F12");
	    closeForm();
	    reject();
	    break;
	}
    case Qt::Key_Enter :
	{
	//    printf("Qt::Key_Enter");
	    if (ChangeParent->hasFocus())
		 ChangeParent_clicked();
	    if(OK->hasFocus())
		 OK_clicked();
	    if (Exit->hasFocus())
		 closeForm();
	    break;
	}
    case Qt::Key_Return :
	{
	//    printf("Qt::Key_Return");
	    if (ChangeParent->hasFocus())
		 ChangeParent_clicked();
	    if(OK->hasFocus())
		 OK_clicked();
	    if (Exit->hasFocus())
		 closeForm();
	    break;
	}
    case Qt::Key_Left :
	{
	  //  printf("Qt::Key_Left");
	    break;
	}
    case Qt::Key_Right :
	{
	 //   printf("Qt::Key_Right");
	    break;	
	}
    case Qt::Key_Tab :
	{
	//    printf("Qt::Key_Tab");
	    break;
	}
    case Qt::Key_Up :
	{
	  //  printf("Qt::Key_Up");
	    break;
	}
    case Qt::Key_Down :
	{
	 //   printf("Qt::Key_Down");
	    break; 
	}
    default :
    { 
	break; 
	 /*   if (LongtitleEdit->hasFocus())
		LongtitleEdit->insert(k->text());
	    if (ShortcutEdit->hasFocus())	
		ShortcutEdit->insert(k->text());
	    if(ParentGroupNameEdit->hasFocus())
		ParentGroupNameEdit->insert(k->text());*/
    }
   }		
}
//--------------------------------------------------------- KeyBoard  end---------------------------------------------------------------------//

