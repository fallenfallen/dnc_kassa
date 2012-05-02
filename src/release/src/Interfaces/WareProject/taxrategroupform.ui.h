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
#include "mcrkbinput.h"
#include "dedicationcombobox.h"
#include "messages.h"

#include <math.h>
#include <qfocusdata.h>

extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;

extern HWSRV  * hw;
DedicationComboBox * ChangeBase_comboBox;
extern PgRef M;
extern bool taxrategroupform_flag;
//bool taxrategroupform_flag = false;

void TaxRateGroupform::AddComboBoxes()
{
    ChangeBase_comboBox = new DedicationComboBox(groupBox11, "r");
    ChangeBase_comboBox->setGeometry(157,95,106,30);
    ChangeBase_comboBox->insertItem(systemCodec->toUnicode("Да"));
    ChangeBase_comboBox->insertItem(systemCodec->toUnicode("Нет"));
}

void TaxRateGroupform::SetTabOrderafterCreateComboBox()
{
   // puts("SetTabOrderafterCreateComboBox()");
    setTabOrder(LongNameTaxRatelineEdit,ShortNameTaxRatelineEdit);
    setTabOrder(ShortNameTaxRatelineEdit,ValueTaxRatelineEdit);
    setTabOrder(ValueTaxRatelineEdit, ChangeBase_comboBox);
    setTabOrder(ChangeBase_comboBox,ClearAll_Button);
    setTabOrder(ClearAll_Button, OK);
    setTabOrder(OK, Exit);
}


void TaxRateGroupform::init()
{
   // puts("void TaxRateGroupform::init()");
    systemCodec = QTextCodec::codecForName(SYSCODEC);
    dbaseCodec = QTextCodec::codecForName(DBCODEC);
    AddComboBoxes();
    SetTabOrderafterCreateComboBox();
    SetValidatorProperties();
    taxrategroupform_flag = true;
   // grabKeyboard();
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    grabKeyboard();
    releaseKeyboard();
}

void TaxRateGroupform::Myinit(int par, const char * str, const char * str1)
{
   // puts("void TaxRateGroupform::Myinit(CodeScheme)");
    Param = par;
   // printf("Param =%d\n",Param);
    switch(par)
    {
    case 1:
	{
	//    puts("Это создание");
	    Tax_Group_Id = str;
	//    printf("Tax_Group_Id = %s\n",Tax_Group_Id.ascii());
	    break;
	}
    case 2:
	{
	//    puts("Это редактирование");
	    Tax_Rate_Id = str;
	    Tax_Group_Id = str1;
	//    printf("Tax_Rate_Id = %s\n",Tax_Rate_Id.ascii());
	//    printf("Tax_Group_Id = %s\n",Tax_Group_Id.ascii());
	    LoadInfoAboutTaxRate();
	    break;
	}
    }
}

void TaxRateGroupform::LoadInfoAboutTaxRate()
{
  //  puts("void TaxRateGroupform::LoadInfoAboutTaxRate()");
    char * name_c =  (char *)calloc(201, sizeof(char));
    char * shortname_c =  (char *)calloc(201, sizeof(char));
    char * value_c =  (char *)calloc(20, sizeof(char));
    char * base_flag_c =  (char *)calloc(20, sizeof(char));
    char * order_id_c =  (char *)calloc(20, sizeof(char));
    int n = M.Get_Tax_Rate_Info(name_c, shortname_c, value_c, Tax_Rate_Id.ascii()) ;
    int k = M.Get_Base_Flag_Info(base_flag_c,order_id_c, Tax_Rate_Id.ascii());
   if (( n == 1) && (k == 1))
    {
     //  puts("ALL OK!");
       QString name = name_c;
       QString shortname = shortname_c;
       QString value = value_c;
       Order_id = order_id_c;
     //  printf(" Order_id = %s\n", Order_id.ascii());
       LongNameTaxRatelineEdit->setText(dbaseCodec->toUnicode(name));
       ShortNameTaxRatelineEdit->setText(dbaseCodec->toUnicode(shortname));
       ValueTaxRatelineEdit->setText(dbaseCodec->toUnicode(value));
  
       QString base_flag = base_flag_c;
       if (base_flag == "f")
       {
	//   puts("base_flag    =   f");
	   ChangeBase_comboBox->setCurrentItem(1);
	   
       }
       else
       {
	//    puts("base_flag    =   t");
	    ChangeBase_comboBox->setCurrentItem(0);
       }
   }
   free(order_id_c);
   free(name_c);
   free(shortname_c);
   free(value_c);
   free(base_flag_c);
}


void TaxRateGroupform::closeEvent( QCloseEvent* e )
{
 //   puts("void TaxRateGroupform::closeEvent( QCloseEvent* e )");
    e->accept();
    taxrategroupform_flag = false;
    grabKeyboard();
    return;
}

void TaxRateGroupform::SetValidatorProperties(void)
{
  //  puts("void TaxRateGroupform::SetValidatorProperties(void)");
    
    QRegExp valuerx("^\\d{1,7}\\.\\d{2}$");
    valuevalidator = new QRegExpValidator(valuerx, this);
    ValueTaxRatelineEdit->setValidator(valuevalidator);
    
    QRegExp titlerx("(\\w|[ ,.%-]){0,99}");
    titlevalidator = new QRegExpValidator(titlerx, this);
    LongNameTaxRatelineEdit->setValidator(titlevalidator);
    ShortNameTaxRatelineEdit->setValidator(titlevalidator);

}



int TaxRateGroupform::Contingencies(void)
{
  //  puts("int TaxRateGroupform::Contingencies(void)");
    QString str;
    QString er_str;
    QString er_id;
    //ограничения на наименование
    str = systemCodec->fromUnicode(LongNameTaxRatelineEdit->text());
     if (str == "")
    {
	 WareReferenceErrorText(11, er_str, er_id);
	 InterfaceErrorWin(er_str, er_id);
	return -1; 
     }
     if (Param == 1)
     {
	 //огр только для вновь создаваемой ставки
	 char * value = (char *) calloc(201,sizeof(char));
	 int i = M.GetTaxRateIdByName(str.ascii(),value);
	 str = value;
	 free(value);
	 if (str =="ERROR")
	 {
	     WareReferenceErrorText(2, er_str, er_id);
	     InterfaceErrorWin(er_str, er_id);
	     return -1; 
	 }
	 if (i>0)
	 {
	     WareReferenceErrorText(12, er_str, er_id);
	     InterfaceErrorWin(er_str, er_id);
	     return -1; 
	 }
     }
    //ограничения на текст для чека
     str = systemCodec->fromUnicode(ShortNameTaxRatelineEdit->text());
     if (str == "")
     {
	 WareReferenceErrorText(13, er_str, er_id);
	 InterfaceErrorWin(er_str, er_id);
	return -1; 
     }
     
    //ограничения на значение
    str = ValueTaxRatelineEdit->text();
    if (str == "")
    {
	WareReferenceErrorText(14, er_str, er_id);
	 InterfaceErrorWin(er_str, er_id);
	return -1; 
    }
    if (str.toInt()>100)
    {
	WareReferenceErrorText(15, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return -1; 
    }
    return 0;
}

void TaxRateGroupform::ClearAll_Button_clicked()
{
 //   puts("void TaxRateGroupform::ClearAll_Button_clicked()");
    LongNameTaxRatelineEdit->setText("");
    ShortNameTaxRatelineEdit->setText("");
    ValueTaxRatelineEdit->setText("");
    ChangeBase_comboBox->setCurrentItem(0);
}


void TaxRateGroupform::OK_clicked()
{
   // puts("void TaxRateGroupform::OK_clicked()");
    if (Contingencies() == 0)
    { 
	QString longtitle = (dbaseCodec->fromUnicode(LongNameTaxRatelineEdit->text()));
	QString shortcut = (dbaseCodec->fromUnicode(ShortNameTaxRatelineEdit->text()));		
	QString values = ValueTaxRatelineEdit->text();
	//printf("values = %s\n",values.ascii());
	//printf("Param = %d\n",Param);
	if (Param == 1)
	{
	    //создание налоговой ставки
	    QString tax_rate_id  = itoa(M.FindNextTaxRateIdFromTaxRate()+1);
	    QString input_string =  tax_rate_id +",'"+ longtitle+"','"+shortcut +"',"+ values;
	//    printf("input_string = %s\n",input_string.ascii());
	    M.Create_Tax_Rate(input_string.ascii());
	
	    //создание связи для налоговой ставки и налоговой группы
	
	    QString id =  itoa(M.FindNextIdFromTaxRateGroup()+1);
	    QString base_flag;
	    if (ChangeBase_comboBox->currentItem() == 0)
		base_flag ="t";
	    else
		base_flag ="f";
	    QString order_id =  itoa(M.FindNextOrderIdInTaxRateGroup()+1);
	 //   printf("!!!!!!!!!!!!!        order_id = %s\n",order_id.ascii());
	    input_string = id+ ",'" +base_flag+ "'," +Tax_Group_Id+ "," +tax_rate_id+ "," + order_id;
	 //   printf("input_string = %s\n", input_string.ascii());

	    int g = M.Create_Tax_Rate_Group(input_string.ascii());
	  //  printf("g = %d\n",g);
	    if (g == 1)
	    {
		QString mes_str;
		QString mes_id;
		WareReferenceMessageText(14,mes_str,mes_id);
		InterfaceMessageWin(mes_str,mes_id);
		close();
	    }
	}
	if (Param == 2)
	{
	//    puts("Param == 2");
	    int i =M.UpdateTaxRate(longtitle.ascii(), shortcut.ascii(), values.ascii(),Tax_Rate_Id.ascii());
	//    printf("-------------------------------->   i = %d\n",i);
	    QString base_flag =  itoa(ChangeBase_comboBox->currentItem());
	    //printf("!!!!!!!!!!!!!!base_flag = |%s|\n",base_flag.ascii());
	    if (base_flag == "1")
		base_flag ="f";
	    else
		base_flag ="t";
	
	    QString order_id;
	    
	    if (Order_id.isEmpty())
		order_id  ="NULL";
	    else
		order_id = Order_id;
	//    printf("order_id = %s\n",order_id.ascii());
	
	    int j =M.UpdateTaxRateGroup(base_flag.ascii(), order_id.ascii(), Tax_Rate_Id.ascii(), Tax_Group_Id.ascii());
	  //  printf("-------------------------------->   j = %d\n",j);
	    if ((i!=-1) && (j!=-1))
	    {
		QString mes_str;
		QString mes_id;
		WareReferenceMessageText(15,mes_str,mes_id);
		InterfaceMessageWin(mes_str,mes_id);
	    }
	    close();  
	}
    }
}



//--------------------------------------------------------- KeyBoard  begin---------------------------------------------------------------------/
void TaxRateGroupform::GetKeySequence()
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


void TaxRateGroupform::keyPressEvent(QKeyEvent *k)
{
    //printf("--------------- press k->key = %d ---------------\n", k->key());
    
        int state = 0;
    
    if ( hw->BCS_GetType() == 2 )  {
	state = kbcode_scaner->FindCode(k);
	//printf("state = %d \n", state);
	if ( state == 2 )  {
	    ;
	}
    }    
    
    if ( hw->MCR_GetType() == 2 && state == 0 ) {
	state = kbcode_reader->FindCode(k);
	//printf("state = %d \n", state);
	if ( state == 2 ) {
	    ;
	}
    }
    
    //printf("state of find scaner code = %d \n", state);
    if ( state == 0 ) {   makeKeyPress(k); return;  }
    if ( state == 5 )     { GetKeySequence();  return;  }
}

void TaxRateGroupform::makeKeyPress(QKeyEvent *k)
{
	QWidget* wid = this->focusData()->focusWidget();  
  // puts("void TaxRateGroupform::makeKeyPress(QKeyEvent *k)");
    switch(k->key())   
    {
    case Qt::Key_F12 :
	{
	//    puts("Qt::Key_F12");
	    close();
	    reject();
	    break;
	}
    case Qt::Key_Enter :
	{
	//    printf("Qt::Key_Enter");
	    if (OK->hasFocus())
			  OK_clicked();
	    if (ClearAll_Button->hasFocus())
			  ClearAll_Button_clicked();
	    if (Exit->hasFocus())
			  close();
	    break;
	}
    case Qt::Key_Return :
	{
	//    printf("Qt::Key_Return");
	    if (OK->hasFocus())
			  OK_clicked();
	    if (ClearAll_Button->hasFocus())
			  ClearAll_Button_clicked();
	    if (Exit->hasFocus())
			  close();
	    break;
	}
    case Qt::Key_Left :
	{
	//    printf("Qt::Key_Left");
	    break;
	}
    case Qt::Key_Right :
	{
	//    printf("Qt::Key_Right");
	    break;	
	}
    case Qt::Key_Tab :
	{
	//    printf("Qt::Key_Tab");
	    break;
	}
    case Qt::Key_Up :
	{
	//    printf("Qt::Key_Up");
		if ( strcmp(wid->className(), "QComboBox" ) == 0 ){
			 int cur_item = ((QComboBox*)wid)->currentItem();
			 if (cur_item!=0)
				((QComboBox*)wid)->setCurrentItem(cur_item-1);
		}
	    break;
	}
    case Qt::Key_Down :
	{
		if ( strcmp(wid->className(), "QComboBox" ) == 0 ){
			int cur_item = ((QComboBox*)wid)->currentItem();
			if (cur_item!=(((QComboBox*)wid)->count()-1))
					((QComboBox*)wid)->setCurrentItem(cur_item+1);
		}
	//    printf("Qt::Key_Down");
	    break; 
	}
    default :
    {	break; 
	//printf("default :");
    }
}		
}
//--------------------------------------------------------- KeyBoard  end---------------------------------------------------------------------//

