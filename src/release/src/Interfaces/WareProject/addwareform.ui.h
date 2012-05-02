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
#include <math.h>
#include <qfocusdata.h>
#include "reference_book_class_pgsql.h"
#include "addon_func.h"
#include "hwsrv.h"
#include "dkbinput.h"
#include "bcskbinput.h"
#include "mcrkbinput.h"
//#include "dedicationcombobox.h"
#include "messages.h"
#include "const.h"
#include "hwsighandlerscontroller.h"


extern PgRef M;
extern SP sp;

DedicationComboBox * WeightQuantity_comboBox2;
DedicationComboBox * Sale_comboBox2;
DedicationComboBox * Return_comboBox2;
DedicationComboBox * NegativeReminder_comboBox2;
DedicationComboBox * WithoutBringingInQuantity_comboBox2;
DedicationComboBox * WritingOffReminder_comboBox2;
DedicationComboBox * EditingPrice_comboBox2;
DedicationComboBox * BringingInQuantityByHand_comboBox2;
DedicationComboBox * PrintInCheck_comboBox2;
DedicationComboBox * GeneralBarcode_comboBox;

extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;

extern HWSRV  * hw;
extern int Group_id;
extern int TaxGroup_id;
extern int  AutoScheme_id;

extern bool addwareform_flag;
extern QString NumberWare;
int WareFormGroupId;
 
void AddWareform::AddComboBoxes()
{
	 //puts("void AddWareform::AddComboBoxes()");
	 WeightQuantity_comboBox2 = new DedicationComboBox(WeightQuantity_comboBox->parentWidget(), "r");
	 QPoint p1 = WeightQuantity_comboBox->pos();
	 ((QGridLayout*)toolBox3->item(1)->layout())->addWidget(WeightQuantity_comboBox2, 0, 1);
	 //layout3->add(WeightQuantity_comboBox2);
	 delete(WeightQuantity_comboBox);
	 WeightQuantity_comboBox2->insertItem(systemCodec->toUnicode("Нет"));
	 WeightQuantity_comboBox2->insertItem(systemCodec->toUnicode("Да"));
	 WeightQuantity_comboBox2->move(p1);
	// puts("--------------------1-----------------------");
	 Sale_comboBox2 = new DedicationComboBox(Sale_comboBox->parentWidget(), "r");
	 QPoint p2 = Sale_comboBox->pos();
	 ((QGridLayout*)toolBox3->item(1)->layout())->addWidget(Sale_comboBox2, 1, 1);
	 //layout4->add(Sale_comboBox2);
	 delete(Sale_comboBox);
	 Sale_comboBox2->insertItem(systemCodec->toUnicode("Нет"));
	 Sale_comboBox2->insertItem(systemCodec->toUnicode("Да"));
	 Sale_comboBox2->move(p2);
	// puts("--------------------2-----------------------"); 
	 Return_comboBox2 = new DedicationComboBox(Return_comboBox->parentWidget(), "r");
	 QPoint p3 = Return_comboBox->pos();
	 ((QGridLayout*)toolBox3->item(1)->layout())->addWidget(Return_comboBox2, 2, 1);
	 //layout5->add(Return_comboBox2);
	 delete(Return_comboBox);
	 Return_comboBox2->insertItem(systemCodec->toUnicode("Нет"));
	 Return_comboBox2->insertItem(systemCodec->toUnicode("Да"));
	 Return_comboBox2->move(p3);
	// puts("--------------------3-----------------------"); 
	 NegativeReminder_comboBox2 = new DedicationComboBox(NegativeReminder_comboBox->parentWidget(), "r");
	 QPoint p4 = NegativeReminder_comboBox->pos();
	 ((QGridLayout*)toolBox3->item(1)->layout())->addWidget(NegativeReminder_comboBox2, 3, 1);
	 //layout6->add(NegativeReminder_comboBox2);
	 delete(NegativeReminder_comboBox);
	 NegativeReminder_comboBox2->insertItem(systemCodec->toUnicode("Нет"));
	 NegativeReminder_comboBox2->insertItem(systemCodec->toUnicode("Да"));
	 NegativeReminder_comboBox2->move(p4);
	// puts("--------------------4-----------------------"); 
	 WithoutBringingInQuantity_comboBox2 = new DedicationComboBox(WithoutBringingInQuantity_comboBox->parentWidget(), "r");
	 QPoint p5 = WithoutBringingInQuantity_comboBox->pos();
	 ((QGridLayout*)toolBox3->item(1)->layout())->addWidget(WithoutBringingInQuantity_comboBox2, 4, 1);
	 //layout7->add(WithoutBringingInQuantity_comboBox2);
	 delete(WithoutBringingInQuantity_comboBox);
	 WithoutBringingInQuantity_comboBox2->insertItem(systemCodec->toUnicode("Нет"));
	 WithoutBringingInQuantity_comboBox2->insertItem(systemCodec->toUnicode("Да"));
	 WithoutBringingInQuantity_comboBox2->move(p5);
//	 puts("--------------------5-----------------------"); 
    	 WritingOffReminder_comboBox2 = new DedicationComboBox(WritingOffReminder_comboBox->parentWidget(), "r");
	 QPoint p6 = WritingOffReminder_comboBox->pos();
	 ((QGridLayout*)toolBox3->item(1)->layout())->addWidget(WritingOffReminder_comboBox2, 5, 1);
	 //layout8->add(WritingOffReminder_comboBox2);
	 delete(WritingOffReminder_comboBox);
	 WritingOffReminder_comboBox2->insertItem(systemCodec->toUnicode("Нет"));
	 WritingOffReminder_comboBox2->insertItem(systemCodec->toUnicode("Да"));
	 WritingOffReminder_comboBox2->move(p6);
	// puts("--------------------6-----------------------"); 
	 EditingPrice_comboBox2 = new DedicationComboBox(EditingPrice_comboBox->parentWidget(), "r");
	 QPoint p7 = EditingPrice_comboBox->pos();
	 ((QGridLayout*)toolBox3->item(1)->layout())->addWidget(EditingPrice_comboBox2, 6, 1);
	 //layout9->add(EditingPrice_comboBox2);
	 delete(EditingPrice_comboBox);
	 EditingPrice_comboBox2->insertItem(systemCodec->toUnicode("Нет"));
	 EditingPrice_comboBox2->insertItem(systemCodec->toUnicode("Да"));
	 EditingPrice_comboBox2->move(p7);
	 //puts("--------------------7-----------------------"); 
	 BringingInQuantityByHand_comboBox2 = new DedicationComboBox(BringingInQuantityByHand_comboBox->parentWidget(), "r");
	 QPoint p8 = BringingInQuantityByHand_comboBox->pos();
	 ((QGridLayout*)toolBox3->item(1)->layout())->addWidget(BringingInQuantityByHand_comboBox2, 7, 1);
	 //layout10->add(BringingInQuantityByHand_comboBox2);
	 delete(BringingInQuantityByHand_comboBox);
	 BringingInQuantityByHand_comboBox2->insertItem(systemCodec->toUnicode("Нет"));
	 BringingInQuantityByHand_comboBox2->insertItem(systemCodec->toUnicode("Да"));
	 BringingInQuantityByHand_comboBox2->move(p8);
	// puts("--------------------8-----------------------");  
	 PrintInCheck_comboBox2 = new DedicationComboBox(PrintInCheck_comboBox->parentWidget(), "r");
	 QPoint p9 = PrintInCheck_comboBox->pos();
	 ((QGridLayout*)toolBox3->item(1)->layout())->addWidget(PrintInCheck_comboBox2, 8, 1);
	 //layout11->add(PrintInCheck_comboBox2);
	 delete(PrintInCheck_comboBox);
	 PrintInCheck_comboBox2->insertItem(systemCodec->toUnicode("Нет"));
	 PrintInCheck_comboBox2->insertItem(systemCodec->toUnicode("Да"));
	 PrintInCheck_comboBox2->move(p9);
}

void AddWareform::SetTabOrderafterCreateComboBox()
{
   // puts("SetTabOrderafterCreateComboBox()");
    setTabOrder(toolBox3, Barcode_table);
    setTabOrder(Barcode_table,GeneralBarcode_comboBox);
    setTabOrder(GeneralBarcode_comboBox,Code_lineEdit);
    setTabOrder(Code_lineEdit,Name_lineEdit);
    setTabOrder(Name_lineEdit,NameForCheck_lineEdit);
    setTabOrder(NameForCheck_lineEdit, Price_lineEdit);
    setTabOrder(Price_lineEdit,MinPrice_lineEdit);
    setTabOrder(MinPrice_lineEdit, Quantity_lineEdit);
    setTabOrder(Quantity_lineEdit,SaleQuantity_lineEdit);
    setTabOrder(SaleQuantity_lineEdit,BestBefore_dateEdit);
    setTabOrder(BestBefore_dateEdit,AutoScheme_pushButton);
    setTabOrder(AutoScheme_pushButton,WareGroup_pushButton);
    setTabOrder(WareGroup_pushButton,TaxGroup_pushButton);
    setTabOrder(TaxGroup_pushButton,WeightQuantity_comboBox2);
    setTabOrder(WeightQuantity_comboBox2,Sale_comboBox2);
    setTabOrder(Sale_comboBox2,Return_comboBox2);
    setTabOrder(Return_comboBox2,NegativeReminder_comboBox2);
    setTabOrder(NegativeReminder_comboBox2,WithoutBringingInQuantity_comboBox2);
    setTabOrder(WithoutBringingInQuantity_comboBox2,WritingOffReminder_comboBox2);
    setTabOrder(WritingOffReminder_comboBox2,EditingPrice_comboBox2);
    setTabOrder(EditingPrice_comboBox2,BringingInQuantityByHand_comboBox2);
    setTabOrder(BringingInQuantityByHand_comboBox2,PrintInCheck_comboBox2);
    setTabOrder(PrintInCheck_comboBox2,Clear_pushButton);
    setTabOrder(Clear_pushButton,OK);
    setTabOrder(OK,Cansel_pushButton);
}


void AddWareform::InitTable()
{
    //puts("AddWareform::InitTable()");
    Barcode_table->setColumnReadOnly(0, false);
    Barcode_table->setColumnReadOnly(1, false);
    Barcode_table->setColumnWidth(0,150);
    Barcode_table->setColumnWidth(1,100);
}

void AddWareform::Myinit(int par)
{
    printf("NumberWare = %s \n", NumberWare.ascii());
    printf("par = %d \n", par);
    
    Param = par;
    if (Param == 1)
    {
	//Добавление товара
	if(NumberWare == "0") 
	{
		 //без группы
		 Group_lineEdit->setText(systemCodec->toUnicode(WITHOUTGROUP));
	}
	else
	{
		 // в группу
		 Group_lineEdit->setText(dbaseCodec->toUnicode(NumberWare));
	}
	AutoScheme_lineEdit->setText(systemCodec->toUnicode(WITHOUTAUTOSCHEME));
	TaxGroup_lineEdit->setText(systemCodec->toUnicode(WITHOUTGROUP));
	char * values = (char *)calloc(20,sizeof(char));
	M.Get_Flags_Info(values);
	if (strcmp(values,"")!=0)
	    LoadFlags(values);
	free(values);
    }
    else
    {
	//Редактирование товара  и копирование
	
	LoadInfoAboutWare();
    }  
    if ( par == 1 || par == 3 ) {
	Code_lineEdit->setText(QString::number(M.FindNextInternalIdFromWare()+1));	
    } else {
	Code_lineEdit->setText(dbaseCodec->toUnicode(NumberWare));
    }
}

void AddWareform::init()
{
 //    puts("void AddWareform::init()");
	 systemCodec = QTextCodec::codecForName(SYSCODEC);
	 dbaseCodec = QTextCodec::codecForName(DBCODEC);
	 cp1251Codec = QTextCodec::codecForName(HWCODEC);
	 AddComboBoxes();
	 SetTabOrderafterCreateComboBox();
	 InitTable();
	 SetValidatorProperties();
	 addwareform_flag = true;
	 exit_flag = false;
	 BestBefore_dateEdit->setDate(QDate::currentDate());
	 AutoScheme_textLabel->hide();
	 AutoScheme_lineEdit->hide();
	 AutoScheme_pushButton->hide();
    
	 if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence()));
	 if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence()));
    grabKeyboard();
    
	 if ((hw->BCS_GetType() == 1) || (hw->BCS_GetType() == 3)) {
		  connect(HwSignalHandlersController::GetInstance(), SIGNAL(BarCode(const char *)), SLOT(ScanerSignal( const char* )));
		  HwSignalHandlersController::GetInstance()->SetControlledObject(WARE);
	 } 
}

void AddWareform::ScanerSignal(const char* barcode)
{
	 //printf("ScanerSignal (%s) \n", barcode);
	 if ( HwSignalHandlersController::GetInstance()->GetControlledObject() != WARE ) return;
	 if ( ! this->isActiveWindow()  ) return;
	 int col =  0;
	 int row = Barcode_table->numRows() - 1;
	 QString barcode_str(barcode);
	 for (int i = 0; i<Barcode_table->numRows();i++)		{
		  if (Barcode_table->text(i,0) == barcode_str) {
				QString er_str, er_id;
				WareReferenceErrorText(59, er_str, er_id);
				InterfaceErrorWin(er_str, er_id);   
				return;
		  }
	 }
	 Barcode_table->setText( row, col, barcode_str);
	 AddRow();
}

void AddWareform::closeEvent( QCloseEvent* e )
{
	// puts("void AddWareform::closeEvent( QCloseEvent* e )");
	 e->accept();
	 addwareform_flag = false;
	 QString group = dbaseCodec->fromUnicode(Group_lineEdit->text());
	 if (exit_flag == true)
	 {
		  // puts("exit_flag == true");
		  char * s = (char *) calloc(20,sizeof(char));
		  if (Param == 1)
				M.GetGroupId(NumberWare.ascii(),s);
		  else
				M.GetGroupIdById(NumberWare.ascii(),s);
		  WareFormGroupId = atoi(s);
		  free(s);
	 }	  
	 else
	 {
		  //puts("exit_flag == false");
		  QString s = dbaseCodec->fromUnicode(Group_lineEdit->text());
		  char * str = (char *) calloc(20,sizeof(char));
		  M.GetGroupId(s.ascii(),str);
		  WareFormGroupId =atoi(str);
		  free(str);
	 }
	 releaseKeyboard();
	 return;
} 


void AddWareform::LoadInfoAboutWare(void)
{
	// puts("void AddWareform::LoadInfoAboutWare(void)");    
	 int i;
	 int * col_fields =(int *)calloc(1,sizeof(int));
	 M.GetQuantityFieldsWare(col_fields);
	 int col_barcodes = M.GetQuantityStrInWareBarcodeById(NumberWare.ascii());
	 char ** mas1 = (char**) calloc(col_barcodes, sizeof(char*));
	 char ** mas_coeff = (char**) calloc(col_barcodes, sizeof(char*));
	 for (i=0; i<col_barcodes; i++) 
	 {
		  mas1[i] = (char*) calloc(21, sizeof(char));
		  mas_coeff [i] = (char*) calloc(11, sizeof(char));
	 }
	 M.LoadBarcodeById(NumberWare.ascii(), mas1, mas_coeff);
	 Barcode_table->setNumRows(col_barcodes+1);
	 for ( i=0; i<col_barcodes;i++) 
	 {
		  Barcode_table->setText(i,0,mas1[i]);
		  Barcode_table->setText(i,1,mas_coeff[i]);	
	 }	
	 InsertBarcodeComboBox();
	 while (--col_barcodes=0) 
	 {
		  free(mas1[col_barcodes]);  
		  free(mas_coeff[col_barcodes]);    
	 }
	 free(mas1); 
	 free(mas_coeff);
	 int fields = *col_fields+1;
	 char **  mas = (char**) calloc(fields, sizeof(char*));
	 for ( i=0; i<(fields); ++i) 
	 {
		  mas[i] = (char*) calloc(201, sizeof(char));
	 }
	 QString str = NumberWare;
	 M.LoadInfo(str.ascii(),mas);  
	 QString w;
	 for (int j = 0;j<GeneralBarcode_comboBox->count();j++)
	 {
		  w = dbaseCodec->fromUnicode(GeneralBarcode_comboBox->text(j));
		  if(w == (QString)mas[1])
		  {
				GeneralBarcode_comboBox->setCurrentItem(j);
		  }
	 }
	 Name_lineEdit->setText(dbaseCodec->toUnicode(QString(mas[2])));
	 NameForCheck_lineEdit->setText(dbaseCodec->toUnicode(QString(mas[3])));
	 Price_lineEdit->setText(dbaseCodec->toUnicode(QString(mas[4])));
	 MinPrice_lineEdit->setText(dbaseCodec->toUnicode(QString(mas[5])));
	 Quantity_lineEdit->setText(dbaseCodec->toUnicode(QString(mas[6])));
	 SaleQuantity_lineEdit->setText(dbaseCodec->toUnicode(QString(mas[7])));
	 //загрузка флагов
	 LoadFlags(mas[8]);
	 char * bbb = (char *) calloc(strlen(mas[9]),sizeof(char));
	 strcpy(bbb,mas[9]);
	 if (strlen(bbb)>0)
	 {
		  char * datefrombd = (char *) calloc(11,sizeof(char));
		  for (int k = 0; k<10; k++)
				*(datefrombd+k) = *(bbb+k);
		  QDate  date; 
		  BestBefore_dateEdit->setDate(date.fromString(datefrombd,ISODate));
		  free(datefrombd);
	 }
	 if (!((QString)mas[10]).isEmpty())
	 {
		  char * value = (char *)calloc(201,sizeof(char));
		  M.GetNameAutoSchemeById(mas[10],value);
		  AutoScheme_lineEdit->setText(dbaseCodec->toUnicode(value));
		  free(value);
	 }
	 else
		  AutoScheme_lineEdit->setText(systemCodec->toUnicode(WITHOUTAUTOSCHEME));
	 if (!((QString)mas[12]).isEmpty())
	 {	
		  char * value = (char *)calloc(201,sizeof(char));
		  M.GetNameWareGroupById(mas[12],value); 
		  Group_lineEdit->setText(dbaseCodec->toUnicode(value));	
		  free(value);
	 }
	 else
		  Group_lineEdit->setText(systemCodec->toUnicode(WITHOUTGROUP));
    
	 if (!((QString)mas[13]).isEmpty())
	 {
		  char * value = (char *)calloc(201,sizeof(char));
		  M.GetNameTaxGroupById(mas[13],value);
		  TaxGroup_lineEdit->setText(dbaseCodec->toUnicode(value));
		  free(value);
	 }
	 else
		  TaxGroup_lineEdit->setText(systemCodec->toUnicode(WITHOUTGROUP));
		  
	 //-----(+)-----Zuskin-----08/07/2011-----
	 if (!((QString)mas[14]).isEmpty())
	 {
		  Section_spinBox->setValue( (dbaseCodec->toUnicode(mas[14])).toInt());
	 }
	 //---------------------------------------	  
		  
	 free(bbb);
	 int c = *col_fields;
	 while (--c=0) 
	 {
		  free(mas[c]);    
	 }
	 free(mas); 
	 free(col_fields);
}


void AddWareform::LoadFlags(char * str)
{
    //puts("void AddWareform::LoadFlags(char * str)");
    unsigned long int k =(unsigned long int)(atoi(str));
    int massiv[9];
    for (int i = 0;i<9;i++)
    {
	massiv[i] = k&1;
	k =k>>1;
	//printf("massiv[%d] = %d\n",i,massiv[i]);
    }
    WeightQuantity_comboBox2->setCurrentItem(massiv[0]);
    Sale_comboBox2->setCurrentItem(massiv[1]);
    Return_comboBox2->setCurrentItem(massiv[2]);
    NegativeReminder_comboBox2->setCurrentItem(massiv[3]);
    WithoutBringingInQuantity_comboBox2->setCurrentItem(massiv[4]);
    WritingOffReminder_comboBox2->setCurrentItem(massiv[5]);
    EditingPrice_comboBox2->setCurrentItem(massiv[6]);
    BringingInQuantityByHand_comboBox2->setCurrentItem(massiv[7]);
    PrintInCheck_comboBox2->setCurrentItem(massiv[8]);
}

void AddWareform::SetValidatorProperties(void)
{
    QRegExp coderx(systemCodec->toUnicode("[A-Za-z0-9А-Яа-я]{0,20}"));
    codevalidator = new QRegExpValidator(coderx, this);
    Code_lineEdit->setValidator(codevalidator);
    
   // puts("AddWareform::SetValidatorProperties()");
    QRegExp pricerx("^\\d{1,8}\\.\\d{2}$");
    pricevalidator = new QRegExpValidator(pricerx, this);
    Price_lineEdit->setValidator(pricevalidator);
    MinPrice_lineEdit->setValidator(pricevalidator);
    
    QRegExp quantityrx("^\\d{1,7}\\.\\d{3}$");
    quantityvalidator = new QRegExpValidator(quantityrx, this);
    Quantity_lineEdit->setValidator(quantityvalidator);
    SaleQuantity_lineEdit->setValidator(quantityvalidator);
	    
    //QRegExp titlerx("(\\w|[.,- ]){0,99}");
    QRegExp titlerx("(\\w|[.,- :"" ' ()$*??]){0,99}");
    titlevalidator = new QRegExpValidator(titlerx, this);
    Name_lineEdit->setValidator(titlevalidator);
    NameForCheck_lineEdit->setValidator(titlevalidator);
    

    QRegExp schemerx("^\\d{0,3}$");
    schemevalidator = new QRegExpValidator(schemerx, this);
    AutoScheme_lineEdit->setValidator(schemevalidator); 
}

int AddWareform::Contingencies(void)
{
  //puts("int AddWareform::Contingencies(void)\n");
    QString str;
    QString er_str;
    QString er_id;
    
    //проверка на существование внутреннего кода товара
    QString codeofware = dbaseCodec->fromUnicode(Code_lineEdit->text());
    
    if ( codeofware.isEmpty() ) {
	WareReferenceErrorText(61, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return -1;
    }
    
    int n = M.GetCountWare(codeofware.ascii());
    if ( n < 0 ) {
	// ошибка при запросе к базе данных
	WareReferenceErrorText(2, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return -1;
    }
    if ( (Param == 1 || Param == 3 || (Param == 2 && codeofware != NumberWare) )  && n != 0 ) {
	//товар с таким внутренним кодом уже существует
	WareReferenceErrorText(60, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return -1;
    }
    
    
    //ограничения на наименование товара
     str = dbaseCodec->fromUnicode(Name_lineEdit->text());
    if (str.length() == 0)
    {
	WareReferenceErrorText(16, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return -1;
    }
    if ((Param == 1)||(Param == 3))
    {
	if (M.CheckExistingWareWithName(str.ascii())>0)
	{
	    WareReferenceErrorText(17, er_str, er_id);
	    InterfaceErrorWin(er_str, er_id);
	    return -1;
	}
    }
    //ограничения на цену товара
    str = Price_lineEdit->text();
    if (str.isEmpty())
    {
	WareReferenceErrorText(18, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return -1;
    }
    if (str == "0")
    {
	WareReferenceErrorText(19, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return -1;
    }
    double a,b;
    a = atof(Price_lineEdit->text());
    b = atof(MinPrice_lineEdit->text());
    if (b >a)
    {
	WareReferenceErrorText(21, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);   
	return -1;
    }
    if (Param == 1)
    {
	a = atof(Quantity_lineEdit->text());
	b = atof(SaleQuantity_lineEdit->text());
	if (b > a)
	{
	    WareReferenceErrorText(22, er_str, er_id);
	    InterfaceErrorWin(er_str, er_id);
	    return -1;
	}
    }
    for ( int i = 0; i<GeneralBarcode_comboBox->count(); i++)
    {
	
	QString string = dbaseCodec->fromUnicode(Barcode_table->text(i,0));
	if (!string.isEmpty())
	{
	    unsigned  int size = string.length();
	    char * str = (char *)calloc(size+1,sizeof(char));
	    QString koef = dbaseCodec->fromUnicode(Barcode_table->text(i,1));
	    strcpy(str,dbaseCodec->fromUnicode(Barcode_table->text(i,0)));
	    double d = koef.toDouble();
	    d -= floor(d);
	    if ( d > 0.00001 && WeightQuantity_comboBox2->currentItem() == 0 ) 
	    {
		WareReferenceErrorText(48, er_str, er_id);
		InterfaceErrorWin(er_str, er_id);  
		free(str);
		return -1;
	    }
	    for (unsigned int i= 0; i<size; i++)
	    {
		if (((str[i]<47) && (str[i]>58)) || ((str[i]<64) && (str[i]>91)) || ((str[i]<96) && (str[i]>123)))
		{
		  WareReferenceErrorText(44, er_str, er_id);
		  InterfaceErrorWin(er_str, er_id);  
		  free(str);
		  return -1;
		}
	    } 
	    
	    free(str);
	}

	if (string.length()>20)
	{
	    WareReferenceErrorText(20, er_str, er_id);
	    InterfaceErrorWin(er_str, er_id);
	    return -1;
	}
	QString coef = dbaseCodec->fromUnicode(Barcode_table->text(i,1));
	if (coef.toDouble()>9999999.999)
	{
	    WareReferenceErrorText(43, er_str, er_id);
	    InterfaceErrorWin(er_str, er_id);
	    return -1;
	}
	else if (coef.isEmpty()){
		 Barcode_table->setText(i,1,"1");
	}
    }
    return 0;
}
	

//преобразование для флагов
QString AddWareform::transformationflags(char * param)
{
     // printf("char * AddWareform::flags()\n");  
      strcpy(param,itoa(WeightQuantity_comboBox2->currentItem()));
      strcat(param,",");
      strcat(param,itoa(Sale_comboBox2->currentItem()));
      strcat(param,",");
      strcat(param,itoa(Return_comboBox2->currentItem()));
      strcat(param,",");
      strcat(param,itoa(NegativeReminder_comboBox2->currentItem()));
      strcat(param,",");
      strcat(param,itoa(WithoutBringingInQuantity_comboBox2->currentItem()));
      strcat(param,",");
      strcat(param,itoa(WritingOffReminder_comboBox2->currentItem()));
      strcat(param,",");
      strcat(param,itoa(EditingPrice_comboBox2->currentItem()));
      strcat(param,",");
      strcat(param,itoa(BringingInQuantityByHand_comboBox2->currentItem()));
      strcat(param,",");
      strcat(param,itoa(PrintInCheck_comboBox2->currentItem()));
     // printf("param = |%s|\n",param);
      QString str = SaveFlags(param); 
      //printf("str  = |%s|\n",str.ascii());
      return str;
}


QString AddWareform::SaveFlags(char * param)
{
  //puts( "QString AddWareform::SaveFlags(char * param)\n");
  bool wrong_par_flag = false;
  int fls[9];
  int k = 0;
  for(unsigned int i = 0; i < strlen(param); i+=2)
  {
    if( ( ( *(param + i) == '0' ) || ( *(param + i) == '1' ) ) && ( ( *(param + i + 1) == ',' ) || ( *(param + i + 1) == '\0' ) ) )
    {
       fls[k] = (int) ( *(param+i) - 48 );
       k++;
    }
    else
    {
      wrong_par_flag = true;
      break;
    }
  }
  if (!wrong_par_flag)
  {
    int bitfl = 0;
    for (int i = 0; i < 9; i ++)
	if(fls[i] != 0)
        bitfl = bitfl | (int)pow(2, i);
    char *strfl = itoa(bitfl); 
    char * flags = (char *) calloc(strlen(strfl) + 1, sizeof(char));
    strcpy(flags, strfl);
   strcpy(param,flags);
    free(strfl);                
    free(flags);
    return (QString)param;
  }
  return "";
}

void AddWareform::OK_clicked(void)
{
    QString codeofware = dbaseCodec->fromUnicode(Code_lineEdit->text());
    //puts("void AddWareform::OK_clicked(void)---------------");
    if (Contingencies() == 0)
    {
		  QString ware_code;
		  QString  bare_code;
		  QString name;
		  QString  nameforcheck;
		  QString  price;
		  QString minprice;
		  QString  quantity;
		  QString  salequantity;
		  QString  transformationflag;
		  QString best_before_str;
		  QString autoscheme;
		  //-----(+)-----Zuskin-----08/07/2011-----
		  QString section;
		  //---------------------------------------
		  name = dbaseCodec->fromUnicode(Name_lineEdit->text());
		  nameforcheck = dbaseCodec->fromUnicode(NameForCheck_lineEdit->text());
		  price = Price_lineEdit->text();
		  if(!MinPrice_lineEdit->text().isEmpty())
				minprice = MinPrice_lineEdit->text();
		  else
				minprice = "o";
		  if (!Quantity_lineEdit->text().isEmpty())
				quantity = Quantity_lineEdit->text();
		  else
				quantity = "o";
		  if (!SaleQuantity_lineEdit->text().isEmpty())
		  {
				salequantity = SaleQuantity_lineEdit->text();
		  }
		  else
		  {
				if (WeightQuantity_comboBox2->currentItem() == 1)//весовой
					 salequantity = "0.001";
				else					       //штучный
					 salequantity = "1";
		  }
		  char * param = (char *) calloc (18,sizeof(char));
		  transformationflag = transformationflags(param);
		  free(param);
		  if (!((BestBefore_dateEdit->date()).isNull()))
				best_before_str = best_before();
		  else
				best_before_str ="o";
		  autoscheme =dbaseCodec->fromUnicode(AutoScheme_lineEdit->text());
		  if (AutoScheme_lineEdit->text().isEmpty())
		  {
				autoscheme ="o";
		  }
		  else
		  {
				if (autoscheme!=(QString)dbaseCodec->fromUnicode(systemCodec->toUnicode(WITHOUTAUTOSCHEME)))
				{
					 char * value = (char *)calloc(20,sizeof(char));
					 M.GetAutoSchemeId(autoscheme.ascii(),value);
					 autoscheme =  value;
					 free(value);
				}
				else
				{
					 autoscheme ="o";
				}
		  }
		  QString group = dbaseCodec->fromUnicode(Group_lineEdit->text());	
		  if(Group_lineEdit->text().isEmpty())
		  {
				group ="o";
		  }
		  else
		  {
				if (group != (QString)dbaseCodec->fromUnicode(systemCodec->toUnicode(WITHOUTGROUP)))
				{
					 char * value = (char *)calloc(20,sizeof(char));
					 M.GetGroupId(group.ascii(),value);
					 group =  value;
					 free(value);
				}
				else
				{
					 group ="o";
				}
		  }
	
		  QString taxgroup =dbaseCodec->fromUnicode(TaxGroup_lineEdit->text());
		  if (TaxGroup_lineEdit->text().isEmpty())
		  {
				taxgroup="o";   
		  }
		  else
		  {
				if (taxgroup!= (QString) dbaseCodec->fromUnicode(systemCodec->toUnicode(WITHOUTGROUP)))
				{
					 char * value = (char *) calloc(20,sizeof(char));
					 M.GetTaxRateId(taxgroup.ascii(),value);
					 taxgroup =  value;
					 free(value);
				}
				else
				{
					 taxgroup="o";
				}
		  }
		  int k = -1;
		  int col = GeneralBarcode_comboBox->count();
	
		  if ((Param == 1) || (Param == 3))
		  {
				//для копирования и создания товара необх проверять штрих-коды
				//puts("Устраиваем проверку на существующие штрих-коды");
				for (int l = 0; l<GeneralBarcode_comboBox->count();l++)
				{
					 QString s1 = GeneralBarcode_comboBox->text(l);
					 QTableSelection sel = Barcode_table->selection(0);
					 Barcode_table->clearSelection();
					 sel.init(l, 0);
					 sel.expandTo(l, 2);
					 Barcode_table->addSelection(sel);
					 Barcode_table -> ensureCellVisible(l,0);
					 GeneralBarcode_comboBox->setCurrentItem(l);
				}
				
				//ware_code = itoa(M.FindNextInternalIdFromWare()+1);//созд идент для товара
		  }
		  else
		  {
				//ware_code = NumberWare;
				M.DeleteBarcodeOfWare(NumberWare.ascii());
		  }

		  char **  mas = (char**) calloc(col, sizeof(char*));
		  char **  mas_coef = (char**) calloc(col, sizeof(char*));
		  for ( int i=0; i<col; ++i) 
		  {
				mas[i] = (char*) calloc(21, sizeof(char));
				mas_coef[i] = (char*) calloc(11, sizeof(char));
				QString bar_code = dbaseCodec->fromUnicode(Barcode_table->text(i,0));
				QString koef = dbaseCodec->fromUnicode(Barcode_table->text(i,1));
				if ((bar_code.length()<21)&&(koef.length()<11)){
					 strcpy(mas[i],bar_code);
					 strcpy(mas_coef[i],koef);
				}
				else{
					 if (bar_code.length()>20)
						   strncpy(mas[i],bar_code,20);
					 if (koef.length()>10)
						  strncpy(mas_coef[i],koef,10);
					 
				}
		  }
		
		  int current =GeneralBarcode_comboBox->currentItem();
		  QString flag = "f";
		  for (int j = 0; j<col;j++)
		  {
				if (j == current)
					 flag = "t";
				else
					 flag = "f";
				//k = M.CreateBarCode(ware_code.ascii(), mas[j], mas_coef[j], flag.ascii()); 
				k = M.CreateBarCode(codeofware.ascii(), mas[j], mas_coef[j], flag.ascii()); 
		  }	
		  int c =col;
		  while (--c=0) 
		  {
				free(mas[c]);    
				free(mas_coef[c]); 
		  }
		  free(mas); 
		  free(mas_coef); 
	
		  //-----(+)-----Zuskin-----08/07/2011-----
		  section = QString::number(Section_spinBox->value());
		  //---------------------------------------
	
		  QString mes_str;
		  QString mes_id;
		  int g = -1;
		  switch(Param)
		  {
		  case 1:
				{
					 //создание товара
					 //-----(*)-----Zuskin-----08/07/2011-----					 
					 //int g = M.AddWareInTable(codeofware.ascii(), name.ascii(), nameforcheck.ascii(), price.ascii(), minprice.ascii(), quantity.ascii(), salequantity.ascii(), transformationflag.ascii(),  best_before_str.ascii(), autoscheme.ascii(), group.ascii(), taxgroup.ascii());
					 int g = M.AddWareInTable(codeofware.ascii(), name.ascii(), nameforcheck.ascii(), price.ascii(), minprice.ascii(), quantity.ascii(), salequantity.ascii(), transformationflag.ascii(),  best_before_str.ascii(), autoscheme.ascii(), group.ascii(), taxgroup.ascii(), section.ascii());
					 //---------------------------------------
					 if (g == 1)
					 { 
						  QString mes_str;
						  QString mes_id;
						  WareReferenceMessageText(2,mes_str,mes_id);
						  InterfaceMessageWin(mes_str,mes_id);
						  close();
					 } 
					 break;
				}
		  case 2:
				{
			
					 //редактирование товара
					 //-----(*)-----Zuskin-----08/07/2011-----					 
					 //g =M.UpdateWare(NumberWare.ascii(), codeofware.ascii(), name.ascii(), nameforcheck.ascii(),  price.ascii(), minprice.ascii(), quantity.ascii(), salequantity.ascii(), transformationflag.ascii(), best_before_str.ascii(), autoscheme.ascii(), group.ascii(),  taxgroup.ascii());
					 g =M.UpdateWare(NumberWare.ascii(), codeofware.ascii(), name.ascii(), nameforcheck.ascii(),  price.ascii(), minprice.ascii(), quantity.ascii(), salequantity.ascii(), transformationflag.ascii(), best_before_str.ascii(), autoscheme.ascii(), group.ascii(),  taxgroup.ascii(), section.ascii());
					 //---------------------------------------
					 if (g == 0)
					 {
						  WareReferenceMessageText(6,mes_str,mes_id);
						  InterfaceMessageWin(mes_str,mes_id);
						  close();
					 } 
					 break;
				}
		  case 3:
				{
					 //копирование товара
					 //-----(*)-----Zuskin-----08/07/2011-----					 
					 //g = M.AddWareInTable(codeofware.ascii(), name.ascii(), nameforcheck.ascii(), price.ascii(), minprice.ascii(), quantity.ascii(), salequantity.ascii(), transformationflag.ascii(),  best_before_str.ascii(), autoscheme.ascii(), group.ascii(), taxgroup.ascii());
					 g = M.AddWareInTable(codeofware.ascii(), name.ascii(), nameforcheck.ascii(), price.ascii(), minprice.ascii(), quantity.ascii(), salequantity.ascii(), transformationflag.ascii(),  best_before_str.ascii(), autoscheme.ascii(), group.ascii(), taxgroup.ascii(), section.ascii());
					 //---------------------------------------
					 if (g == 1)
					 { 
						  WareReferenceMessageText(2,mes_str,mes_id);
						  InterfaceMessageWin(mes_str,mes_id);
						  close();
					 }    
					 break;
				}
		  }
	 }
}



QString AddWareform::best_before(void)
{
	 //  printf("void AddWareform::best_before(QString str)\n");
	 QString str = "CAST('" + BestBefore_dateEdit->date().toString("dd.MM.yyyy")+"' AS DATE )";
	 return str;	    
}

void AddWareform::AutoScheme_pushButton_clicked()
{
	 //  puts("void AddWareform::AutoScheme_pushButton_clicked()"); 
	 releaseKeyboard();
	 a_f = new Autoschemeform;
	 if (AutoScheme_lineEdit->text() == systemCodec->toUnicode(WITHOUTAUTOSCHEME)) {
		  a_f->Number_Group = 0;
	 }
	 else
	 {
		  QString s = dbaseCodec->fromUnicode(AutoScheme_lineEdit->text());
		  char* id = (char *) calloc(20,sizeof(char));
		  M.GetAutoSchemeId(s.ascii(),id);
		  if((QString)id!="ERROR")
				a_f->Number_Group =atoi(id);
		  free(id);  
	 }
    
	 a_f->exec();
	 delete(a_f);
	 if (AutoScheme_id !=0)
	 {
		  QString str = itoa(AutoScheme_id);
		  char * value = (char *) calloc(201,sizeof(char));
		  M.GetNameAutoSchemeById(str.ascii(),value);
		  if (strcmp(value,"ERROR")!=0)
				AutoScheme_lineEdit->setText(dbaseCodec->toUnicode(value));
		  free(value);
	 }
	 else
		  AutoScheme_lineEdit->setText(systemCodec->toUnicode(WITHOUTAUTOSCHEME));
    grabKeyboard();
}



void AddWareform::WareGroup_pushButton_clicked(void)
{ 
 //  puts("void AddWareform::WareGroup_pushButton_clicked(void)");
	 releaseKeyboard();
	 w_g = new WareGroupform;	 
	 if (Group_lineEdit->text() == systemCodec->toUnicode(WITHOUTGROUP))
	 {
		  w_g->Number_Group = 0;
	 }
	 else
	 {
		  QString name_group = dbaseCodec->fromUnicode(Group_lineEdit->text());
		  char * s = (char *)calloc(20,sizeof(char));
		  M.GetGroupId(name_group,s);
		  w_g->Number_Group = atoi(s);
		  free(s);
	 }  
	 w_g->exec();
	 delete(w_g);
	 if (Group_id !=0)
	 {
		  QString str = itoa(Group_id);
		  char * value = (char *) calloc(201,sizeof(char));
		  M.GetNameWareGroupById(str.ascii(),value);
		  if (strcmp(value,"ERROR")!=0)
				Group_lineEdit->setText(dbaseCodec->toUnicode(value)); 
		  free(value);
	 }
	 else
		  Group_lineEdit->setText(systemCodec->toUnicode(WITHOUTGROUP));
	 grabKeyboard();
}

void AddWareform::TaxGroup_pushButton_clicked()
{
 //  puts("void AddWareform::TaxGroup_pushButton_clicked()");
   releaseKeyboard();
   t_g = new TaxGroupform;
   QString s = dbaseCodec->fromUnicode(TaxGroup_lineEdit->text());
   char* id = (char *) calloc(20,sizeof(char));
   M.GetTaxGroupId(s.ascii(),id);
   if((QString)id!="ERROR")
       t_g->Number_Group =atoi(id);
   free(id);
   t_g->exec();
   delete(t_g);
   if (TaxGroup_id !=0)
   {
       QString str = itoa(TaxGroup_id);
       char * value = (char *) calloc(201,sizeof(char));
       M.GetNameTaxGroupById(str.ascii(),value);
       if (strcmp(value,"ERROR")!=0)
	   TaxGroup_lineEdit->setText(dbaseCodec->toUnicode(value));
       free(value);
   }
   else
       TaxGroup_lineEdit->setText(systemCodec->toUnicode(WITHOUTGROUP));
   grabKeyboard();
}


void AddWareform::Clear_pushButton_clicked()
{
//    puts("void AddWareform::Clear_pushButton_clicked()");
    Barcode_table->setNumRows(1);
    Barcode_table->setText(1,0,"");
    GeneralBarcode_comboBox->clear();
    Name_lineEdit->setText("");
    NameForCheck_lineEdit->setText("");
    Price_lineEdit->setText("");
    MinPrice_lineEdit->setText("");
    Quantity_lineEdit->setText("");
    SaleQuantity_lineEdit->setText("");
    BestBefore_dateEdit->setDate(QDate(0000,00,00));
    AutoScheme_lineEdit->setText(systemCodec->toUnicode(WITHOUTAUTOSCHEME));
    Group_lineEdit->setText(systemCodec->toUnicode(WITHOUTGROUP));
    TaxGroup_lineEdit->setText(systemCodec->toUnicode(WITHOUTGROUP));
    
    WeightQuantity_comboBox2->setCurrentItem(0);
    Sale_comboBox2->setCurrentItem(0);
    Return_comboBox2->setCurrentItem(0);
    NegativeReminder_comboBox2->setCurrentItem(0);
    WithoutBringingInQuantity_comboBox2->setCurrentItem(0);
    WritingOffReminder_comboBox2->setCurrentItem(0);
    EditingPrice_comboBox2->setCurrentItem(0);
    BringingInQuantityByHand_comboBox2->setCurrentItem(0);
    PrintInCheck_comboBox2->setCurrentItem(0);
}


void AddWareform::Cansel_pushButton_clicked()
{
    exit_flag = true;
    close();
}




//--------------------------------------------------------- KeyBoard  begin---------------------------------------------------------------------//
void AddWareform::GetKeySequence()
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


void AddWareform::keyPressEvent(QKeyEvent *k)
{
	// puts("---keyPress---");
	 int state = 0;
    
	 if ( hw->BCS_GetType() == 2 )  {
		  state = kbcode_scaner->FindCode(k);
		  if ( state == 2 )  {
				QString barcode_str = ((KeyboardScaner*) kbcode_scaner)->getCode();
				int col =  0;
				int row = Barcode_table->numRows() - 1;
			//	printf("barcode_str = %s\n",barcode_str.ascii());
				for (int i = 0; i<Barcode_table->numRows();i++)		{
					 if (Barcode_table->text(i,0) == barcode_str) {
						  QString er_str, er_id;
						  WareReferenceErrorText(59, er_str, er_id);
						  InterfaceErrorWin(er_str, er_id);
						  return;
					 }
				}
				Barcode_table->setText( row, col, barcode_str);
				AddRow();
		  }
	 }
    
	 if ( hw->MCR_GetType() == 2 && state == 0 ) {
		  state = kbcode_reader->FindCode(k);
		  if ( state == 2 ) {
				return;
		  }
	 }
    
	// printf("state of find scaner code = %d \n", state);
	 if ( state == 0 ) {   makeKeyPress(k); return;  }
	 if ( state == 5 )     { GetKeySequence();  return;  }
}

void AddWareform::makeKeyPress(QKeyEvent *k)
{		
	// printf("k->key() =%d\n",k->key());
    
	 if ( k->key() == 4129 || k->key() == 4128 || k->key() == 4131 || k->key() == 4132 || k->key() == 4384 || ( k->key() > 4145 && k->key() < 4155 ) ) 
		  return;
	 
	 QWidget* wid = this->focusData()->focusWidget();  
	 switch(k->key())   
	 {
	 case Qt::Key_F12 :
		  {
				//puts("Qt::Key_F12");
				Cansel_pushButton_clicked();
				reject();
				break;
		  }
	 case Qt::Key_Enter:
		  {
				//puts("Qt::Key_Enter");
				if ((Barcode_table->hasFocus())||(Barcode_table->currentSelection()!=-1)){
					// puts("table has focus");
					 int row = Barcode_table->currentRow();
					 int col = Barcode_table->currentColumn(); 
					 Barcode_table_valueChanged(row,	col);
				}
				if (AutoScheme_pushButton->hasFocus())
				    AutoScheme_pushButton_clicked();
				if (WareGroup_pushButton->hasFocus())
				    WareGroup_pushButton_clicked();
				if (TaxGroup_pushButton->hasFocus())
				    TaxGroup_pushButton_clicked();
				if (Clear_pushButton->hasFocus())
				    Clear_pushButton_clicked();
				if (OK->hasFocus())
				    OK_clicked();
				if (Cansel_pushButton->hasFocus())
				    Cansel_pushButton_clicked();
				break;
		  }
	 case Qt::Key_Return:
		  {
				//puts("Qt::Key_Return");			
				if (Barcode_table->hasFocus()){
				//	 puts("table has focus");
					 int row = Barcode_table->currentRow();
					 int col = Barcode_table->currentColumn(); 
					 Barcode_table_valueChanged(row,	col);
				}
				if (AutoScheme_pushButton->hasFocus())
				    AutoScheme_pushButton_clicked();
				if (WareGroup_pushButton->hasFocus())
				    WareGroup_pushButton_clicked();
				if (TaxGroup_pushButton->hasFocus())
				    TaxGroup_pushButton_clicked();
				if (Clear_pushButton->hasFocus())
				    Clear_pushButton_clicked();
				if (OK->hasFocus())
				    OK_clicked();
				if (Cansel_pushButton->hasFocus())
				    Cansel_pushButton_clicked();
				break;
		  }
	 case Qt::Key_Left :
		  if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {
				((QLineEdit*)wid)->cursorBackward( k->state() == Qt::ShiftButton );	   
		  }
		  if (Barcode_table->hasFocus()){
				//puts("barcode_table has focus");
				int col = Barcode_table->currentColumn(); 
				if(col!=0){
					 Barcode_table->editCell(Barcode_table->currentRow(),col-1);
					 Barcode_table->setCurrentCell(Barcode_table->currentRow(),col-1);
					 
				}
				
		  }
		  break;
	 case Qt::Key_Right :
		  if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {	
				((QLineEdit*)wid)->cursorForward( k->state() == Qt::ShiftButton );	   
		  }
		  if (Barcode_table->hasFocus()){
				//puts("barcode_table has focus");
				int col = Barcode_table->currentColumn(); 
				if(col!=Barcode_table->numCols()){
					 Barcode_table->editCell(Barcode_table->currentRow(),col+1);
					 Barcode_table->setCurrentCell(Barcode_table->currentRow(),col+1);
				}
		  }
		  break;	
	 case Qt::Key_Tab :
		  focusData()->next();
		  break;
	 case Qt::Key_Up :
		  {
				if (Barcode_table->hasFocus()){
					// puts("barcode_table has focus");
					 int row = Barcode_table->currentRow(); 
					 if(row!=0)
					 Barcode_table->setCurrentCell(row-1,0);
					 Barcode_table->selectRow(row-1);
				}
				if (toolBox3->hasFocus()){
					 int index = toolBox3->currentIndex();
					 if(index == 1)
						  toolBox3->setCurrentItem(toolBox3->item(0));
				}
				if ( strcmp(wid->className(), "QComboBox" ) == 0 ){
					 int cur_item = ((QComboBox*)wid)->currentItem();
					 if (cur_item!=0)
						  ((QComboBox*)wid)->setCurrentItem(cur_item-1);
				}
				
				break;
		  }
	 case Qt::Key_Down :
		  {
				if (Barcode_table->hasFocus()){
					// puts("barcode_table has focus");
					 int row = Barcode_table->currentRow(); 
					 if(row!=Barcode_table->numRows())
					 Barcode_table->setCurrentCell(row+1,0);
					 Barcode_table->selectRow(row+1);
				}
				if (toolBox3->hasFocus()){
					 int index = toolBox3->currentIndex();
					 if(index == 0)
						  toolBox3->setCurrentItem(toolBox3->item(1));
				}
				if ( strcmp(wid->className(), "QComboBox" ) == 0 ){
					 int cur_item = ((QComboBox*)wid)->currentItem();
					 if (cur_item!=(((QComboBox*)wid)->count()-1))
						  ((QComboBox*)wid)->setCurrentItem(cur_item+1);
				}
				break; 
		  }
	 case Qt::Key_Delete :
		  {
				//    puts("Qt::Key_Delete");
				if (Barcode_table->hasFocus())
				{
					 int row = Barcode_table->currentRow();
					 if (row!=Barcode_table->numRows()-1)
						  Barcode_table->removeRow(row);
					 InsertBarcodeComboBox();
					 break; 
				}
				if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {	
					 ((QLineEdit*)wid)->del();
				}
				break;
		  }
	 case Qt::Key_Home :
		  if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {	
				((QLineEdit*)wid)->home( k->state() == Qt::ShiftButton );	
		  }
		  break;
	 case Qt::Key_End :
		  if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {
				((QLineEdit*)wid)->end( k->state() == Qt::ShiftButton );
		  }
		  break;
	 case Qt::Key_Backspace :	
		  if ( strcmp(wid->className(), "QLineEdit" ) == 0 )   {
				((QLineEdit*)wid)->backspace();    
		  }
		  break;
	 default :
				if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {    
		  ((QLineEdit*)wid)->insert(k->text());   
	 }
	 break; 
}	
}	
//--------------------------------------------------------- KeyBoard  end---------------------------------------------------------------------//

void AddWareform::AddRowInTable()
{
	 //  puts("void AddWareform::AddRowInTable()");
	 int cur_rows = Barcode_table->currentRow();
	 QString BarCode = Barcode_table->text(cur_rows,0);
	 QString Koeffic = Barcode_table->text(cur_rows,1);
	 if (!BarCode.isEmpty())
	 {
		  int checkExisting = CheckExistingBarCode(BarCode,Koeffic);
		  if (checkExisting!=0)
		  {
				Barcode_table->setText(cur_rows,0,""); 
				Barcode_table->setText(cur_rows,1,""); 
				if (Barcode_table->numRows()>1){
					 Barcode_table->removeRow(cur_rows);
				}
				Barcode_table->setCurrentCell(cur_rows,0);
		  } 
		  AddRow();
	 }
}




int  AddWareform::CheckExistingBarCode(QString BarCode, QString Koef)
{
	 //puts("такого ш-к еще нет в базе");
	 int check_sym_koef = check_sym_koef_into_barcode(dbaseCodec->fromUnicode(BarCode),dbaseCodec->fromUnicode(Koef));
	 if ( check_sym_koef == 0)
	 {
		  int cur_row = Barcode_table->currentRow();
		  for (int i = 0; i<GeneralBarcode_comboBox->count();i++)
		  {
				if ((GeneralBarcode_comboBox->text(i) == BarCode ) && (i!=cur_row))
				{
					 //puts("В общем такой штрих-код совпал с уже введенным выше");
					 QString er_str;
					 QString er_id;
					 WareReferenceErrorText(52, er_str, er_id);
					 InterfaceErrorWin(er_str, er_id);
					 return -1;
				}
		  }
		  return 0;
	 }
	 else 
	 {
		  //    puts("с внутренностями ш-к или коэфф чего-то не так");
		  return -1;
	 }
}



void AddWareform::AddRow()
{
//   puts("void AddWareform::AddRow()");
	 int cur_rows = Barcode_table->currentRow();
	 QString newBarCode = Barcode_table->text(cur_rows,0);
	 QString coef = Barcode_table->text(cur_rows,1);
	 int num_rows = Barcode_table->numRows();
	 if (!Barcode_table->text(num_rows-1,0).isEmpty())
	 {
		  Barcode_table->setNumRows(num_rows+1);
		  Barcode_table->setCurrentCell(cur_rows+1,0);
		  Barcode_table->selectRow(cur_rows+1);
		  cur_rows = Barcode_table->currentRow();
		  if (Barcode_table->text(num_rows-1,1).isEmpty())
		  {
				Barcode_table->setText(num_rows-1,1,"1");
		  }	
	 }
	 InsertBarcodeComboBox();
	 Barcode_table->setCurrentCell(num_rows-1,0);
}



void AddWareform::InsertBarcodeComboBox()
{
	// puts("void AddWareform::InsertBarcodeComboBox()");
	 GeneralBarcode_comboBox->clear();
	 unsigned int num_rows = Barcode_table->numRows();
	 QString barcode;
	 for(unsigned int i = 0; i<(num_rows-1);i++)
	 {
		  barcode = Barcode_table->text(i,0);
		  GeneralBarcode_comboBox->insertItem(barcode,-1);
	 }
}


void AddWareform::Barcode_table_valueChanged(int row, int col)
{
	// printf("void AddWareform:: Barcode_table_valueChanged(%d, %d)\n",row,col);
	 if (col == 0)
	 {
		  AddRowInTable();
	 }
	 else
	 {
		  if (Barcode_table->text(row,0).isEmpty())
		  {
				Barcode_table->setText(row,1,"");  
		  }
		  else
				AddRowInTable();
	 }
}


int AddWareform::check_sym_koef_into_barcode(QString sym, QString koef)
{
    QString er_str;
    QString er_id;
    bool ok;
    ok = true;
    int len = sym.length(); 
    if (len>20)
    {
	WareReferenceErrorText(20, er_str, er_id);//штрих-код содержит недопустимые символы
	InterfaceErrorWin(er_str, er_id); 
	return -1;
    }
    int len_koef = koef.length();
    double max_value_koef = 9999999.999;
    double koeff = koef.toDouble();
    if (koeff>max_value_koef)
    {
	WareReferenceErrorText(51, er_str, er_id);//коэффициент больше максимального
	InterfaceErrorWin(er_str, er_id); 
	return -1;
    }
    char * buf = (char *)calloc(len+1, sizeof(char));
    strcpy(buf,sym.ascii());
    for (int i = 0; i<len;i++)
    {
	if(ok)
	{
	   if (buf[i]<0)
	   {
	       ok = false;
			 //     puts("(buf[i]<0)");
			 //    puts("Это русский символ < 0");
	   }
	   else
	   {
	       if ((buf[i]>47) && (buf[i]<58))
	       {
				  //	puts("Это цифры");
	       }
	       else
	       {
		   if ((buf[i]>64) && (buf[i]<91))
		   {
				 //	       puts("Это маленький английский символ");
		   }
		   else
		   {
		       if ((buf[i]>96) && (buf[i]<123))
		       {
					  //	   puts("Это большой английский символ");
		       }
		       else
		       {
					  //	   puts("то, что не подо что нас устраивающее не подходит");
					  ok = false;    
		       }
		   }
	       }
	   }
	}
	else
	    break;
    }
    if (ok == false)
	 {
		  WareReferenceErrorText(44, er_str, er_id);//штрих-код содержит недопустимые символы
		  InterfaceErrorWin(er_str, er_id); 
		  free(buf);
		  return -1;
	 }
    
	 free(buf);
	 char * buf_koef = (char *)calloc(len_koef+1, sizeof(char));
	 strcpy(buf_koef,koef.ascii());
	 for (int j = 0; j<len_koef;j++)
	 {
		  if(ok)
		  {
				if (buf_koef[j]<0)
				{
					 ok = false;
					 //	puts("Это русский символ");
				}
				else
				{
					 if (buf_koef[j]==46)
					 {
						  //    puts("Это устраивающая нас точка");   
					 }
					 else
					 {
						  if ((buf_koef[j]>47) && (buf_koef[j]<58))
						  {
								//	puts("Это цифра");
						  }
						  else
						  {
								ok = false;
								//	puts("Это что-то нас не устраивающее...");
						  }
					 }
				}
		  }
		  else
				break;
	 }
	 if (ok == false)
	 {
		  free(buf_koef);
		  //коэффициент содержит недопустимые символы
		  WareReferenceErrorText(50, er_str, er_id);
		  InterfaceErrorWin(er_str, er_id); 
		  return -1;
	 }  
	 free(buf_koef);
	 return 0;
}


void AddWareform::ShowError(){
     QString mes_str;
     QString mes_id;
     WareReferenceErrorText(23,mes_str,mes_id);
     InterfaceErrorWin(mes_str,mes_id);
}



