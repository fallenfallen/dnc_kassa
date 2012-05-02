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
#include "hwsrv.h"
#include "reference_book_class_pgsql.h"
#include "addon_func.h"
#include "dkbinput.h"
#include "bcskbinput.h"
#include "dedicationcombobox.h"
#include "messages.h"
#include "mcrkbinput.h"
#include "hwsighandlerscontroller.h"

#include <wait.h>
#include <math.h>
#include <signal.h>
#include <qfocusdata.h>

DebugLevelLog *dl;

DedicationComboBox * ExistTaxGroupcomboBox;
DedicationComboBox * ExistSchemeAutocomboBox;
DedicationComboBox * WeightQuantity_comboBox;
DedicationComboBox * Sale_comboBox;
DedicationComboBox * Return_comboBox;
DedicationComboBox * NegativeReminder_comboBox;
DedicationComboBox * WithoutBringingInQuantity_comboBox;
DedicationComboBox * WritingOffReminder_comboBox;
DedicationComboBox * EditingPrice_comboBox;
DedicationComboBox * BringingInQuantityByHand_comboBox;
DedicationComboBox * PrintInCheck_comboBox;

extern SP sp;

extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;

extern HWSRV  * hw;
QString  NumberWare;
extern QString newname;
extern QString newname1;
extern QString newnametaxgroup;
extern PgRef M;

extern bool mainform_flag;
extern int WareFormGroupId;
extern int InsertParamGroupId;

void MainWareform::AddComboBoxes()
{
    ExistTaxGroupcomboBox = new DedicationComboBox(Taxes_groupBox, "r");
    //ExistTaxGroupcomboBox->setGeometry(15,93,300,30);
    taxgroupLayout->addWidget(ExistTaxGroupcomboBox, 2, 0);
    
    ExistSchemeAutocomboBox = new DedicationComboBox(AutoSchemes_groupBox, "r");
    ExistSchemeAutocomboBox->setGeometry(15,110,300,30);
        
    WeightQuantity_comboBox = new DedicationComboBox(groupBox20, "r");
    WeightQuantity_comboBox->setGeometry(281,40,107,30);
    WeightQuantity_comboBox->insertItem(systemCodec->toUnicode("Нет"));
    WeightQuantity_comboBox->insertItem(systemCodec->toUnicode("Да"));

    Sale_comboBox = new DedicationComboBox(groupBox20, "r");
    Sale_comboBox->setGeometry(281,75,107,30);
    Sale_comboBox->insertItem(systemCodec->toUnicode("Нет"));
    Sale_comboBox->insertItem(systemCodec->toUnicode("Да"));

    Return_comboBox = new DedicationComboBox(groupBox20, "r");
    Return_comboBox->setGeometry(281,110,107,30);
    Return_comboBox->insertItem(systemCodec->toUnicode("Нет"));
    Return_comboBox->insertItem(systemCodec->toUnicode("Да"));
    
    NegativeReminder_comboBox = new DedicationComboBox(groupBox20, "r");
    NegativeReminder_comboBox->setGeometry(281,145,107,30);
    NegativeReminder_comboBox->insertItem(systemCodec->toUnicode("Нет"));
    NegativeReminder_comboBox->insertItem(systemCodec->toUnicode("Да"));
    
    WithoutBringingInQuantity_comboBox = new DedicationComboBox(groupBox20, "r");
    WithoutBringingInQuantity_comboBox->setGeometry(281,180,107,30);
    WithoutBringingInQuantity_comboBox->insertItem(systemCodec->toUnicode("Нет"));
    WithoutBringingInQuantity_comboBox->insertItem(systemCodec->toUnicode("Да"));
    
    WritingOffReminder_comboBox = new DedicationComboBox(groupBox20, "r");
    WritingOffReminder_comboBox->setGeometry(281,215,107,30);
    WritingOffReminder_comboBox->insertItem(systemCodec->toUnicode("Нет"));
    WritingOffReminder_comboBox->insertItem(systemCodec->toUnicode("Да"));
    
    EditingPrice_comboBox = new DedicationComboBox(groupBox20, "r");
    EditingPrice_comboBox->setGeometry(281,250,107,30);
    EditingPrice_comboBox->insertItem(systemCodec->toUnicode("Нет"));
    EditingPrice_comboBox->insertItem(systemCodec->toUnicode("Да"));
    
    BringingInQuantityByHand_comboBox = new DedicationComboBox(groupBox20, "r");
    BringingInQuantityByHand_comboBox->setGeometry(281,285,107,30);
    BringingInQuantityByHand_comboBox->insertItem(systemCodec->toUnicode("Нет"));
    BringingInQuantityByHand_comboBox->insertItem(systemCodec->toUnicode("Да"));
    
    PrintInCheck_comboBox = new DedicationComboBox(groupBox20, "r");
    PrintInCheck_comboBox->setGeometry(281,320,107,30);
    PrintInCheck_comboBox->insertItem(systemCodec->toUnicode("Нет"));
    PrintInCheck_comboBox->insertItem(systemCodec->toUnicode("Да"));   
}


void MainWareform::SetTabOrderafterCreateComboBox()
{
  //  puts("SetTabOrderafterCreateComboBox()");
    setTabOrder(tabWidget2, WareName_lineEdit);
    setTabOrder(WareName_lineEdit, WareBarCode_lineEdit);
    setTabOrder(WareBarCode_lineEdit, WareCode_lineEdit);
    setTabOrder(WareCode_lineEdit, AddWareGroup);
    setTabOrder(AddWareGroup, DeleteAllWare);
    setTabOrder(DeleteAllWare, AddWare);
    setTabOrder(AddWare, DeleteWare);
    setTabOrder(DeleteWare, ChangeWare);
    setTabOrder(ChangeWare, CopyWare);
    setTabOrder(CopyWare, ExitpushButton);
    setTabOrder(ExitpushButton, ExistTaxGroupcomboBox);
    setTabOrder(ExistTaxGroupcomboBox, CreateTaxGroup);
    setTabOrder(CreateTaxGroup, EditTaxGroup);
    setTabOrder(EditTaxGroup, DeleteTaxGroup);
    setTabOrder(DeleteTaxGroup, ExistTaxlistBox);
    setTabOrder(ExistTaxlistBox, CreateTax);
    setTabOrder(CreateTax, EditTax);
    setTabOrder(EditTax, DeleteTax);
    setTabOrder(DeleteTax,UpButton);
    setTabOrder(UpButton,DownButton);
    setTabOrder(DownButton,CreateAutoSchemeGroup);
    setTabOrder(CreateAutoSchemeGroup,EditAutoSchemeGroup);
    setTabOrder(EditAutoSchemeGroup,ExistSchemeAutocomboBox);
    setTabOrder(ExistSchemeAutocomboBox,CreateAutoSchemeGroup);
    setTabOrder(CreateAutoSchemeGroup,EditAutoSchemeGroup);
    setTabOrder(EditAutoSchemeGroup,DeleteAutoSchemeGroup);
    setTabOrder(DeleteAutoSchemeGroup,ExistAutoSchemelistView);
    setTabOrder(ExistAutoSchemelistView,CreateAutopushButton);
    setTabOrder(CreateAutopushButton,EditAutopushButton);
    setTabOrder(EditAutopushButton,DeleteAutoSchemepushButton);
    setTabOrder(DeleteAutoSchemepushButton,WareGroup_listView);
    setTabOrder(WareGroup_listView,CreateWareGroupButton);
    setTabOrder(CreateWareGroupButton,CreateWareGroupInExistGroupButton);
    setTabOrder(CreateWareGroupInExistGroupButton,DeleteWareGroupButton);
    setTabOrder(DeleteWareGroupButton,WeightQuantity_comboBox);
    setTabOrder(WeightQuantity_comboBox,Sale_comboBox);
    setTabOrder(Sale_comboBox,Return_comboBox);
    setTabOrder(Return_comboBox,NegativeReminder_comboBox);
    setTabOrder(NegativeReminder_comboBox,WithoutBringingInQuantity_comboBox);
    setTabOrder(WithoutBringingInQuantity_comboBox,WritingOffReminder_comboBox);
    setTabOrder(WritingOffReminder_comboBox,EditingPrice_comboBox);
    setTabOrder(EditingPrice_comboBox,BringingInQuantityByHand_comboBox);
    setTabOrder(BringingInQuantityByHand_comboBox,PrintInCheck_comboBox);
    setTabOrder(PrintInCheck_comboBox, SaveFlags); 
}


void MainWareform::init()
{
	//puts("MainWareform::init()");
	 tabWidget2->removePage(tabDiscount);
	 //tabWidget2->page(2)->hide();
	 systemCodec = QTextCodec::codecForName(SYSCODEC);
	 dbaseCodec = QTextCodec::codecForName(DBCODEC);
	 cp1251Codec = QTextCodec::codecForName(HWCODEC);
	 CheckExistWareTable();
	 AddComboBoxes(); 
	 SetTabOrderafterCreateComboBox();
	 group_count = M.GetQuantityStrInWareGroup();
	 //  auto_group_count = M.GetQuantityStrInAutoSchemeGroup(auto_group_count);
	 mainform_flag = true;
	 cur_group = -1;
	 InitTable();
	 sernumber = 0;

	 ///////////--------------------------------------------------------------1-----------------------------------------------------------------------------///////////////
	 //слоты для поиска 
	 connect(WareName_lineEdit, SIGNAL(textChanged(const QString &)), SLOT(CheckValue()));
	 connect(WareBarCode_lineEdit, SIGNAL(textChanged(const QString &)), SLOT(CheckValue()));
	 connect(WareCode_lineEdit, SIGNAL(textChanged(const QString &)), SLOT(CheckValue()));
	 //навигация по таблице
	 if(!sp.block_mouse)
	 {
		  //connect(WareTable, SIGNAL(doubleClicked( int , int , int , const QPoint &)), SLOT());
		  connect(WareTable, SIGNAL(clicked( int , int , int , const QPoint &)), SLOT(ClearMessage()));
	 }
  
	 QRegExp coderx(systemCodec->toUnicode("[A-Za-z0-9А-Яа-я]{0,20}"));
	 codevalidator = new QRegExpValidator(coderx, this);
	 //WareCode_lineEdit->setValidator(codevalidator);    
//-----(*)-----Zuskin-----28/03/2011-----
// 	 QRegExp barrx("([0-9a-zA-Z]){0,13}");
 	 QRegExp barrx("([0-9a-zA-Z]|[-]){0,20}");
//-------------------------------------------------
	 barcodevalidator = new QRegExpValidator(barrx, this);
	 WareBarCode_lineEdit->setValidator(barcodevalidator);
//-----(*)-----Zuskin-----28/03/2011-----
//	 QRegExp titlerx("(\\w|[.,- ]){0,20}");
 	 QRegExp titlerx("(\\w|[., ]|[-]){0,20}");
//-------------------------------------------------
	 titlevalidator = new QRegExpValidator(titlerx, this);
	 WareName_lineEdit->setValidator(titlevalidator);
  
	 //connect(HwSignalHandlersController::GetInstance(), SIGNAL(BarCode(const char *)), SLOT(SetBarCodePattern( const char *)));
	 
	 ///////////--------------------------------------------------------------2-----------------------------------------------------------------------------///////////////
	 //  ExistSchemeAutocomboBox->setAutoResize(TRUE);
	 //заполнение List Box автоматических групп скидок
	 FillAutoReductionListBox();
	 ExistSchemeAutocomboBox->setCurrentItem(0);
	 //слот для загрузки групп скидок в комбо бокс
	 connect(ExistSchemeAutocomboBox, SIGNAL(activated(int)), SLOT(ChangeAutoGroup()));
	 //автоматическое заполнение ListBox для скидок
	 ChangeAutoGroup();
  
	 ///////////--------------------------------------------------------------3-----------------------------------------------------------------------------///////////////
	 // ExistTaxGroupcomboBox->setAutoResize(TRUE);
	 //заполнение List Box  налоговых групп 
	 FillTaxListBox();
	 ExistTaxGroupcomboBox->setCurrentItem(0);
  
	 //слот для загрузки налогов   в комбо бокс
	 connect(ExistTaxGroupcomboBox, SIGNAL(activated(int)), SLOT(ChangeTaxGroup()));
	 //автоматическое заполнение ListBox для налогов
	 ChangeTaxGroup();
  
	 ///////////--------------------------------------------------------------4-----------------------------------------------------------------------------///////////////
	 FillWareGroup_list();
	 WareGroup_listView->setSelected(WareGroup_listView->firstChild(),TRUE);
	 QPixmap pixUp = QPixmap::fromMimeSource("3.png");
	 QPixmap pixDown = QPixmap::fromMimeSource("4.png");
	 //adjustSize(); 
	 ///////////--------------------------------------------------------------5-----------------------------------------------------------------------------///////////////
	 
	     
	 if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
	 if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
	 grabKeyboard();
	 
	 if (( hw->BCS_GetType() == 1 ) ||  ( hw->BCS_GetType() == 3 )) {
	     connect(HwSignalHandlersController::GetInstance(), SIGNAL(BarCode(const char *)), SLOT(ScanerSignal( const char* )));
	     HwSignalHandlersController::GetInstance()->SetControlledObject(WARESEARCH);
	 }
	 
	 LoadFlagsValue();
	 FirstStart(); 
	 //располагаем закладки нужным нам образом при первом запуске
	 
	 //скрываем столбец с идентификаторами групп
	 WareGroup_listView->hideColumn(1);
	 //puts("end MainWareform::init()");
	 // sleep(1);
}

void MainWareform::ScanerSignal(const char* barcode)
{
   // printf("ScanerSignal (%s) \n", barcode);
    if ( HwSignalHandlersController::GetInstance()->GetControlledObject() != WARESEARCH ) return;
    if (tabWidget2->currentPageIndex() != 0) return;
    if ( ! this->isActiveWindow()  ) return;
    SetBarCode(QString(barcode));
}

void MainWareform::CheckExistWareTable(void)
{
   // puts("void MainWareform::CheckExistWareTable(void)");
    int * col_ware = (int * ) calloc (1,sizeof(int));
    int d = M.CheckExistWareTable(col_ware); 
    free(col_ware);
    if (d== -2)
    {
	QString er_str;
	QString er_id;
	WareReferenceErrorText(24, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	close();
	pid_t childpid = getpid();
	kill(childpid,SIGTERM);
	int stat;
	waitpid(childpid,&stat, 0);
    }
   // puts("end CheckExistWareTable()");
}



void MainWareform::SetBarCode( QString str )
{
   //puts("void MainWareform::SetBarCode( QString str )");
    if (WareBarCode_lineEdit->text()!=NULL)
	WareBarCode_lineEdit->clear();
    WareBarCode_lineEdit->setFocus();
    WareBarCode_lineEdit->setText(str);
}


//====================================================================//
void MainWareform::closeEvent( QCloseEvent* e )
{
    puts("void MainWareform::closeEvent( QCloseEvent* e )"); 
    if ((hw->BCS_GetType() == 1) || (hw->BCS_GetType() == 3))
    {
	hw->BCS_Stop();
    }
    e->accept();
    mainform_flag = false;
    puts("before delete hw"); 
    delete(hw);
    puts("after delete hw"); 
    releaseKeyboard();
    return;
}


//====================================================================//
void MainWareform::GetSearchParameter()
{
   // puts("MainWareform::GetSearchParameter()");
    QWidget *le;
    le = (QWidget *) WareCode_lineEdit;
    if(le->hasFocus())
    {
      editline = WareCode_lineEdit;
      v = codevalidator;
      str = WareCode_lineEdit->text();
      M.orderbysearch = 0;
      return;
    }
    le = (QWidget *) WareBarCode_lineEdit;
    if(le->hasFocus())
    {
      editline = WareBarCode_lineEdit;
      v = barcodevalidator;
      str = WareBarCode_lineEdit->text();
      M.orderbysearch = 1;
      return;
    }
    le = (QWidget *) WareName_lineEdit;
    if(le->hasFocus())
    {
      editline =WareName_lineEdit;
      v = titlevalidator;
      str = WareName_lineEdit->text();
      M.orderbysearch = 2;
      return;
    }
}


//==============================================================================================================//
void MainWareform::Search(int pattern_type)
{
    //printf("void MainWareform::Search(int pattern_type = %d)\n", pattern_type);
    
    //printf("editline->text() = %s\n", editline->text().ascii());
    QString s = dbaseCodec->fromUnicode(editline->text());
    //printf("s = %s \n", s.ascii());
    char * pattern = (char *) calloc(s.length()*2 + 1, sizeof(char)); 
    strcpy(pattern, s);
    //printf("PATTERN FOR SEARCH <%s>\n", pattern);
    int n = M.Get_Num_Search_Info(pattern, pattern_type); //кол-во полученных по запросу товаров
    //printf("n = %d \n", n);
    if ((n > 0) && (n <=sp.max_number_search_result))
    {
	if (M.Get_List_By_Pattern(pattern, pattern_type) > 0)
	{
	    messageTextLabel->setText(systemCodec->toUnicode(""));
	    ClearTable();
	    FillProd(n);
	}
	else
	{
	    messageTextLabel->setText(systemCodec->toUnicode("<p align=\"center\">Ошибка при запросе к БД</p>"));
	}
    }
   else if(n>0)
    {	 	 
  //    puts("<p align=\"center\">Слишком много вариантов!!!</p>");
      messageTextLabel->setText(systemCodec->toUnicode("<p align=\"center\">Слишком много вариантов!!!</p>"));
      ClearTable();
    }
    else
    {
//	puts("<p align=\"center\">Нет подходящих вариантов!!!</p>");
      messageTextLabel->setText(systemCodec->toUnicode("<p align=\"center\">Нет подходящих вариантов!!!</p>"));
      ClearTable();
    }
    free(pattern);  
    QTableSelection sel = WareTable -> selection(0);
    WareTable->clearSelection();
    sel.init(cur_pos, 0);
    sel.expandTo(cur_pos, 5);
    WareTable->addSelection(sel);
    WareTable -> ensureCellVisible(cur_pos,0);
}


//==============================================================================================================//
void MainWareform::SearchByPattern()
{
    //puts("MainWareform::SearchByPattern()");
    QWidget *le;
    le = (QWidget *) WareCode_lineEdit;
    if(le->hasFocus())
    {
//	puts("SearchByPattern() Search(1)");
	Search(1);
	return;
    }
    le = (QWidget *) WareBarCode_lineEdit;
    if(le->hasFocus())
    {
//	puts("SearchByPattern() Search(2)");
	Search(2);
	return;
    }
    le = (QWidget *)WareName_lineEdit;
    if(le->hasFocus())
    {
//	puts("SearchByPattern() Search(3)");
	Search(3);
	return;
    }
}
//==============================================================================================================//
void MainWareform::CheckValue()
{
    puts("MainWareform::CheckValue()");
    
    GetSearchParameter();
    
    if ( WareCode_lineEdit->hasFocus() ) {
	//puts("WareCode_lineEdit->hasFocus()");
	Search(1);
	if ( WareCode_lineEdit->text().isEmpty() ) {
	    messageTextLabel->setText(systemCodec->toUnicode(""));
	    ClearTable();
	    FillTable("0");
	}
	return;
    } 
    
        
    bool searchfl = true;
    bool rootfl = false;
    int curpos = editline->cursorPosition();
    QString s = editline->text();
    switch(v->validate(s, curpos))
    {
    case QValidator::Intermediate :
	if ((editline != WareName_lineEdit) && (s.toInt() == 0))
	{
	    searchfl = false;
	    break;
	}
    case QValidator::Invalid :
	{
	    searchfl = false;
	    editline->setText(str);
	    break;
	}
    case QValidator::Acceptable :
	if(s == "")
	{
	    searchfl = false;
	    rootfl = true;
	    break;
	}	
    }
    if(searchfl) 
    {
	SearchByPattern();
    }
    if(rootfl)
    {
	messageTextLabel->setText(systemCodec->toUnicode(""));
	ClearTable();
	FillTable("0");
    }
}

//====================================================================//
void MainWareform::InitTable()
{
    //puts("MainWareform::InitTable()");
    WareTable->setColumnReadOnly(0, true);
    WareTable->setColumnReadOnly(1, true);
    WareTable->setColumnReadOnly(2, true);
    WareTable->setColumnReadOnly(3, true);
    WareTable->setColumnReadOnly(4, true);
    WareTable->setColumnReadOnly(5, true);
    WareTable->setColumnReadOnly(6, true);
    WareTable->setColumnReadOnly(7, true);
    WareTable->setColumnWidth(0, 25);
    WareTable->setColumnWidth(1, 80);//60
    WareTable->setColumnWidth(2, 200);
    WareTable->setColumnWidth(3, 120); 
    WareTable->setColumnWidth(4, 100); //100
    WareTable->setColumnWidth(5, 100);//100
    WareTable->setColumnWidth(6, 50); 
    WareTable->setColumnWidth(7, 50); 
    WareTable->setSelectionMode(QTable::SingleRow);
    WareTable->hideColumn(6);
    WareTable->hideColumn(7);
}

//====================================================================//
void MainWareform::AddPixmapInTable(bool pixflag, int i)
{
  // puts("MainWareform::AddPixmapInTable(bool pixflag, int i)");
    QPixmap pix;
    if(pixflag) 
    {
	pix = QPixmap::fromMimeSource("folder-open.png");
	sernumber = pix.serialNumber();
    }
    else 
	pix = QPixmap::fromMimeSource("folder-closed.png");	
    QTableItem *i0 = new QTableItem(WareTable, QTableItem::Never, "", pix);  
    WareTable->setItem(i, 0, i0);
}

//====================================================================//

void MainWareform::AddRowInTable(char *name, char *price, char *quant, char * code, char *barcode, char *group, char * pgroup, bool grflag, bool pixflag )
{
  //  puts("MainWareform::AddRowInTable(char *name, char *price, char *quant, char * code, char *barcode, char *group, char * pgroup, bool grflag, bool pixflag)");
    QString s;
    int i = WareTable->numRows();  
	 WareTable->insertRows(i);
   // WareTable->setNumRows(i + 1);
    if (grflag)
	AddPixmapInTable(pixflag, i);
    WareTable->setText(i, 1, dbaseCodec->toUnicode(code)); 
    WareTable->setText(i, 2, dbaseCodec->toUnicode(name));
    WareTable->setText(i, 3, dbaseCodec->toUnicode(barcode));
    WareTable->setText(i, 4, dbaseCodec->toUnicode(price));
    WareTable->setText(i, 5, dbaseCodec->toUnicode(quant));
    WareTable->setText(i, 6, dbaseCodec->toUnicode(group));
    WareTable->setText(i, 7,dbaseCodec->toUnicode(pgroup));
    if(cur_group == atoi(group))
	cur_pos = i;
  //  puts("end of AddRowInTable");
}

//====================================================================//
void MainWareform::SetSelectionUp()
{    
 //   puts("MainWareform::SetSelectionUp()");
    QTableSelection sel = WareTable -> selection(0);
    int p_sel = sel.topRow();
    if((p_sel != 0) && (p_sel != -1))
    {
	WareTable->clearSelection();
	sel.init(--p_sel, 0);
	sel.expandTo(p_sel, 5);
	WareTable->addSelection(sel);
	WareTable -> ensureCellVisible(p_sel,0); 
	WareTable->selectRow(p_sel);
    }
}

//====================================================================//
void MainWareform::SetSelectionDown()
{
  //  puts("MainWareform::SetSelectionDown()");
    QTableSelection sel = WareTable -> selection(0);
    int p_sel = sel.topRow();
    // printf("p_sel = %d\n",p_sel);
	 
    string group_id = "";
	 
    //найти идентификатор родителя текущей группы
    QString s = GetNameGroup();
    //   printf("s.ascii = %s\n",s.ascii());
    char * s1 = (char *)calloc(20,sizeof(char));
    if (s == "0")
    {
	strcpy(s1,"0");
    }
    else
    {
	M.GetGroupId(s,s1);
	if (strcmp(s1,"")==0)
	    strcpy(s1,"0");
    }
    // printf("s1 = %s\n",s1);
    group_id = s1;
	 
    bool flag_search = false;
	 
    if ((!WareName_lineEdit->text().isEmpty())||(!WareBarCode_lineEdit->text().isEmpty())||(!WareCode_lineEdit->text().isEmpty()))
	flag_search = true;
	 
    //printf("group_id = %s\n",group_id.c_str());
    if ((p_sel ==(WareTable->numRows() - 1))&&(flag_search == false)){
	// puts("нужно вызывать продолжение для таблицы");
	int L = 1;//это новый limit для подгрузки в текущую таблицу
	//узнаем текущий элемент - группа или товар
	QString s = systemCodec->fromUnicode(WareTable->text(p_sel,1));
	//printf("s = %s\n",s.ascii());
	QString ware_id = "";
	if (s.isEmpty()){//группа
	    QString name_group = systemCodec->fromUnicode(WareTable->text(p_sel,2));
	    //printf("name_group = %s\n",name_group.ascii());
	    int i = M.Get_Group_Info(group_id.c_str(), name_group.ascii(), L);	
	    //printf("Количество групп для отображения = %d\n",i);
	    if (i>0)
		FillGroup(i, (char *)group_id.c_str());
	    else if(i == 0){//групп не нашли, может товар найдем?!
		int order_was = M.orderbysearch;
		M.orderbysearch = 0;//поиск по идентификаторам
		i = M.Get_Goods_Info(group_id.c_str());
		// printf("i = %d\n",i);
		if (i>0){ //нашли
		    
		    //printf("ware_id = %s \n", ware_id.ascii());
		    
		    i = M.Get_Goods_Info(group_id.c_str(), ware_id.ascii(), L); 
		    if (i>0)FillProd(i);
		}
		M.orderbysearch = order_was;
	    }
	}
	else{//товар
	    ware_id = systemCodec->fromUnicode(WareTable->text(p_sel,1));
	    //printf("ware_id = %s \n", ware_id.ascii() );
	    int order_was = M.orderbysearch;
	    M.orderbysearch = 0;//поиск по идентификаторам
	    int i = M.Get_Goods_Info(group_id.c_str());
	    //printf("i = %d\n",i);
	    if (i>0){
		i = M.Get_Goods_Info(group_id.c_str(), ware_id.ascii(), L); 
		if (i>0)FillProd(i);
	    }
	    M.orderbysearch = order_was;  
	}
    } 
    if((p_sel != (WareTable->numRows() - 1)) && (p_sel != -1))
    {
	WareTable->clearSelection();
	sel.init(++p_sel, 0);
	sel.expandTo(p_sel, 5);
	WareTable->addSelection(sel);
	WareTable -> ensureCellVisible(p_sel,0); 
	WareTable->selectRow(p_sel);
    }  
}



//====================================================================//
void MainWareform::ClearTable()
{
   // puts("MainWareform::ClearTable()");
    WareTable->setNumRows(0);
}

//====================================================================//
void MainWareform::FillGroup(int n, char *pgroup)
{
   //printf("MainWareform::FillGroup(%d, %s)\n",n,pgroup);
   QWidget * abc = WareTable;
   abc->hide();
   char *name, *group_id;
   name = (char *) calloc(201, sizeof(char));
   group_id = (char*) calloc(12, sizeof(char));
   char * null_str = (char*) calloc(1, sizeof(char));
   strcpy(null_str,"");
   for(int i = 0; i < n; i++)
   {	
	strcpy(name,"");
	strcpy(group_id,"");
	M.Get_Group_Info(name, group_id, i, n);
	AddRowInTable(name,null_str, null_str, null_str, null_str, group_id, pgroup, true, false);

   }
   free(null_str);
   free(name);
   free(group_id);
  //puts("end FillGroup");
   abc->show();
}

//====================================================================//
void MainWareform::FillProd(int n)
{
	 //printf("MainWareform::FillProd(%d)",n);
	 QWidget * abc = WareTable;
	 abc->hide();
	 char * longtitle = (char *) calloc(201, sizeof(char));
	 char * shortcut = (char*) calloc(201, sizeof(char));
	 char * price = (char *) calloc(15, sizeof(char));
	 char * quantity = (char *) calloc(17, sizeof(char));
	 char  * bar_code = (char *) calloc(21, sizeof(char));
	 char * int_code = (char *) calloc(41, sizeof(char));
	 char * null_str = (char*) calloc(1, sizeof(char));
	 strcpy(null_str,"");
	 for (int i = 0; i<n; i++)
	 {
		  M.Get_Prod_Info(longtitle, shortcut, price, quantity, bar_code, int_code, i, n);
		  //printf("longtitle= %s\n",longtitle);
		  //printf(" int_code = %s\n",int_code);
		  AddRowInTable(longtitle, price, quantity, int_code, bar_code, null_str,null_str, false, false);
	 }
	 free(null_str);
	 free(longtitle);
	 free(shortcut);
	 free(price);
	 free(quantity);
	 free(bar_code);
	 free(int_code);
	 abc->show();
}

//====================================================================//

void MainWareform::FillTable(const char * group_id)
{
    /* puts("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
      puts("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
     puts("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    puts("MainWareform::FillTable(const char * group_id)");*/
    char * g_id = (char *)calloc(12,sizeof(char));
    strcpy(g_id , group_id);
    char * null_str = (char *)calloc(1,sizeof(char)); 
    strcpy(null_str,""); 
    int lim = limit;
    int Col_Group = 0;
    int Col_Ware = 0;
    //--------------------------------------------------------группы
    int n = M.Get_Group_Info(g_id); 
    // printf("!!!!!!!!!!!!!           n = %d\n",n);
    //---------------------------------------------------------
    if (n>0){		  
	Col_Group = M.Get_Group_Info(g_id, null_str, lim);	
	//  printf(	"Количество групп для отображения = %d\n",Col_Group);
	FillGroup(Col_Group, g_id);
    }
    //--------------------------------------------------------товары
	
    if (lim-Col_Group>0){//можно впихнуть еще товары в таблицу  
	lim = lim-Col_Group;
	int order_was = M.orderbysearch;
	M.orderbysearch = 0;//поиск по идентификаторам
	n = M.Get_Goods_Info(g_id);
	//  printf("n = %d\n",n);
	if (n>0){
	    Col_Ware = M.Get_Goods_Info(g_id, "", lim);
	    //	printf("Col_Ware = %d\n",Col_Ware);
	    FillProd(Col_Ware);
	}
	M.orderbysearch = order_was;
    }
    free(null_str);
    free(g_id);
    
    QTableSelection sel = WareTable -> selection(0);
    cur_pos = WareTable->currentRow();
    // printf("cur_pos = %d\n",cur_pos);
    WareTable->clearSelection();
    sel.init(cur_pos, 0);
    sel.expandTo(cur_pos, 5);
    WareTable->addSelection(sel);
    WareTable -> ensureCellVisible(cur_pos,0); 
    WareTable->selectRow(0);
    /*puts(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    puts(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    puts(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");*/
}

//====================================================================//

QString MainWareform::GetNameGroup(void)
{
 // puts("void MainWareform::GetNameGroup(void)");
    QString strings;
    if (WareTable->numRows()==0)
    {
//	puts(" Empty Table");
	strings = "0";
    }
    else
    {
	QTableSelection sel = WareTable -> selection(0);
	int p_sel =0;
	if(WareTable->pixmap(p_sel, 0).serialNumber() == sernumber)
	{
//	    puts("папка открыта");
	    strings = WareTable->text(p_sel,6);
	}
	else
	{
//	    puts("папка закрыта");
	    strings = WareTable->text(p_sel,7);
	}
	if ((strings.isNull())||(strings.isEmpty()))
	{
	    strings = "0";
	}
	if (strings !="0")
	{
	    char * s = (char *)calloc(201,sizeof(char));
	    M.GetLongtitleByGroupId(strings.ascii(),s);
	    strings = s;
	    free(s);
//	    printf("strings = %s\n",strings.ascii());
	}
    }
    return strings;
}
//====================================================================//
void MainWareform::AddWareGroup_clicked()
{
 //   puts("void MainWareform::AddWareGroup_clicked()");
    //необх определить  id  родителя вновь создаваемой группы
    QString s = GetNameGroup();
 //   printf("s.ascii = %s\n",s.ascii());
    char * s1 = (char *)calloc(20,sizeof(char));
    if (s == "0")
    {
	strcpy(s1,"0");
    }
    else
    {
	M.GetGroupId(s,s1);
	if (strcmp(s1,"")==0)
	    strcpy(s1,"0");
    }
 //   printf("s1 = %s\n",s1);
    i_p_g = new InsertParamGroup;
    QSize size(540,150);
    i_p_g->setMinimumSize(size);
    i_p_g->setCaption(systemCodec->toUnicode("Создание группы товаров"));
    i_p_g->GetTypeForm(1,s1);
    releaseKeyboard();
    i_p_g->exec();
    grabKeyboard();
    delete(i_p_g);
//   printf("InsertParamGroup return NAME = %s\n",newname1.ascii());
    ClearTable();
    if (strcmp(s1,"0")==0)
    {
	 FillTable("0");
    }
    else
    {
	char * name = (char *)calloc(201,sizeof(char));
	char * pgroup =(char *) calloc(20,sizeof(char));
	char * null_str = (char*) calloc(1, sizeof(char));
	 strcpy(null_str,"");
	M.Get_Parent_Info(s1, name, pgroup);    
//	printf("name = %s\n",name);
//	printf("pgroup = %s\n",pgroup);

	if (strcmp(s1,"0")!=0)
	{
	    AddRowInTable(name,null_str,null_str, null_str, null_str,s1,pgroup, true, true);
	}
	free(null_str);
	free(name);
	free(pgroup);
	FillTable(s1);
    }
    free(s1);
    FillWareGroup_list(); 
}


//====================================================================//

void MainWareform::DeleteAllWare_clicked()
{
 //   puts("void MainWareform::DeleteAllWare_clicked()");
	 releaseKeyboard();
	 int t =  QMessageBox::information( this, systemCodec->toUnicode("Cообщение"),
systemCodec->toUnicode("Вы действительно хотите очистить справочник?"),systemCodec->toUnicode("Да"),systemCodec->toUnicode("Нет"),0,1);
	 if(t==0)
	 {
    
		  int i = M.DeleteAllWare();
		  i = M.DeleteAllWareGroup();
		  i = M.DeleteAllWareBarcode();
		  ClearTable();
		  FillTable("0");
		  FillWareGroup_list(); 
	 }  
	 grabKeyboard();
}

//====================================================================//

void MainWareform::AddWare_clicked()
{
  //  puts("void MainWareform::AddWare_clicked()");

    NumberWare = GetNameGroup();
 //   printf("NumberWare in Main = %s\n",NumberWare.ascii());
    a_w = new AddWareform;
    a_w->Myinit(1);
    a_w->setCaption(systemCodec->toUnicode("Создание карточки товара"));
    HwSignalHandlersController::GetInstance()->SetControlledObject(WARE);
    a_w->exec();
    HwSignalHandlersController::GetInstance()->SetControlledObject(WARESEARCH);
    delete(a_w);
    ClearTable();
    if (WareFormGroupId ==0)
    {
	FillTable("0");
    }
    else
    {
	char * group_id =(char *) calloc(20,sizeof(char));
	strcpy(group_id,itoa(WareFormGroupId));
	char * name =(char *) calloc(201,sizeof(char));
	char * pgroup =(char *) calloc(20,sizeof(char));
	char * null_str = (char*) calloc(1, sizeof(char));
	 strcpy(null_str,"");
	M.Get_Parent_Info(group_id, name, pgroup);    
	//printf("name = %s\n",name);
	//printf("group_id = %s\n",group_id);
	if (strcmp(pgroup,"")==0)
	    strcpy(pgroup,"0");
	AddRowInTable(name,null_str, null_str, null_str, null_str,group_id,pgroup, true, true);
	FillTable(group_id);
	free(group_id);
	free(null_str);
	free(name);
	free(pgroup);
    }
}


//====================================================================//
void MainWareform::DeleteWare_clicked()
{
   // puts("void MainWareform::DeleteWare_clicked()");
    int k =WareTable->currentRow(); 
  //  printf("k = %d\n",k);
    QString NumberWare = systemCodec->fromUnicode(WareTable->text(k,1));
    if (NumberWare !="")
    {
        //подтверждение получить от пользователя
        int t =  QMessageBox::information( this, systemCodec->toUnicode("Cообщение"),
systemCodec->toUnicode("<font color=red>Вы действительно хотите удалить товар?</font>"),systemCodec->toUnicode("Да"),systemCodec->toUnicode("Нет"),0,1);
        if(t==0)
       {
	    char * value = (char *)calloc(20,sizeof(char));
	    M.GetGroupIdById(NumberWare.ascii(),value);
	    QString group = value;
	    free(value);
	    M.DeleteWare(NumberWare.ascii()); 
	    M.DeleteBarcodeOfWare(NumberWare.ascii());
	    ClearTable();
	    if (group == "0")
	    {
		FillTable("0");
	    }
	    else
	    {
		char * name = (char *)calloc(201,sizeof(char));
		M.GetLongtitleByGroupId(group.ascii(),name);
		char * parent_id = (char *)calloc(20,sizeof(char));
		char * null_str = (char*) calloc(1, sizeof(char));
		strcpy(null_str,"");
		M.GetParentIdByGroupId(group.ascii(),parent_id);
		if ((QString)parent_id!="ERROR")
		    AddRowInTable(name,null_str, null_str, null_str, null_str,(char*) group.ascii(), parent_id, true, true);
		free(null_str);
		free(name);
		free(parent_id);
		FillTable(group.ascii());
	 }
       }
    }
    else
    {
	QString s = dbaseCodec->fromUnicode(WareTable->text(WareTable->currentRow(),2));

	if ((s.isEmpty())|| (s.isNull()))
	{
	    //для удаления товара он должен быть выделен
	    QString er_str;
	    QString er_id;
	    WareReferenceErrorText(25, er_str, er_id);
	    InterfaceErrorWin(er_str, er_id); 
	    return;
	}
	
	//подтверждение получить от пользователя
	int t =  QMessageBox::information( this, systemCodec->toUnicode("Cообщение"),
systemCodec->toUnicode("<font color=red>Вы действительно хотите удалить группу товаров?</font>"),systemCodec->toUnicode("Да"),systemCodec->toUnicode("Нет"),0,1);
	if(t==0)
	{
	    QString group_id = WareTable->text(k,6);
	 //   printf("Текущий  id = %s\n",group_id.ascii());
	    QString p_group_id = WareTable->text(k,7);
	 //   printf("Родительский id = %s\n",p_group_id.ascii());
	    char * name = (char *)calloc(201,sizeof(char));
	    M.GetLongtitleByGroupId(group_id.ascii(),name);
	//    printf("Имя удаляемой группы = %s\n",name);
	    int col_ware = M.GetQuantityWaresInGroup(name);
	//    printf("col_ware = %d\n",col_ware);
	    char ** mas = (char**) calloc(col_ware, sizeof(char*));
	    for (int  i=0; i<col_ware; i++) 
	    {
		mas[i] = (char*) calloc(20, sizeof(char));
	    }
	    M.Get_Id_By_Group_Longtitle(name, mas);
	
	    for (int k = 0; k<col_ware;k++)
	    {
	//	printf("mas[%d] = %s\n",k,mas[k]);
		M.DeleteBarcodeOfWare(mas[k]);
	    }
	    while (--col_ware = 0) 
	    {		
		free(mas[col_ware]);  
	    }	   
	    free(mas); 
	    M.DeleteWareGroup(name);
	    FillWareGroup_list();
	   
	    ClearTable();
	    if (p_group_id == "0")
		FillTable("0");
	    else
	    {
		char * pgroup =(char *) calloc(20,sizeof(char));
		M.Get_Parent_Info((char *)p_group_id.ascii(), name, pgroup);  
		char * null_str = (char*) calloc(1, sizeof(char));
		strcpy(null_str,"");
	//	printf("name = %s\n",name);
	//	printf("pgroup = %s\n",pgroup);
	//	printf("group_id = %s\n",group_id.ascii());
	  
		if (strcmp(pgroup,"")==0)
		    strcpy(pgroup,"0");
		AddRowInTable(name,null_str, null_str, null_str, null_str,(char *)group_id.ascii(),pgroup, true, true);
		FillTable(p_group_id);
		free(pgroup);  
		free(null_str);
	    }
	    free(name);   
	}
    }
}

//====================================================================//

void MainWareform::ChangeWare_clicked()
{
    releaseKeyboard();
    
  //  puts("void MainWareform::ChangeWare_clicked()");
    //проверка выделен ли товар для редактирования
    NumberWare =  dbaseCodec->fromUnicode(WareTable->text(WareTable->currentRow(),1));
    if ((NumberWare != NULL) && (NumberWare != "") && (NumberWare != " "))
    {
//	printf("NumberWare in MainWareform = %s\n",NumberWare.ascii());     
	a_w = new AddWareform;
	a_w->Myinit(2);
	a_w->setCaption(systemCodec->toUnicode("Редактировать карточку товара"));
	a_w->OK->setText(systemCodec->toUnicode("Сохранить изменения"));
	HwSignalHandlersController::GetInstance()->SetControlledObject(WARE);
	a_w->exec();
	HwSignalHandlersController::GetInstance()->SetControlledObject(WARESEARCH);
	delete(a_w);
	ClearTable();
	if (WareFormGroupId ==0)
	{
	     FillTable("0");
	}
	else
	{
	    char * group_id =(char *) calloc(20,sizeof(char));
	    strcpy(group_id,itoa(WareFormGroupId));
	    char * name =(char *) calloc(201,sizeof(char));
	    char * pgroup =(char *) calloc(20,sizeof(char));
	    char * null_str = (char*) calloc(1, sizeof(char));
	    strcpy(null_str,"");
	    M.Get_Parent_Info(group_id, name, pgroup);    
	    if (strcmp(pgroup,"")==0)
	    strcpy(pgroup,"0");
	    AddRowInTable(name,null_str, null_str, null_str, null_str,group_id,pgroup, true, true);         FillTable(group_id);
	    free(group_id);
	    free(null_str);
	    free(name);
	    free(pgroup);
	}
    }
    else
    {  
	QString s = dbaseCodec->fromUnicode(WareTable->text(WareTable->currentRow(),2));
	//printf("s.ascii = %s\n",s.ascii());
	if ((s.isEmpty())|| (s.isNull()))
	{
	    //для редактирования товара он должен быть выделен
	    QString er_str;
	    QString er_id;
	    WareReferenceErrorText(26, er_str, er_id);
	    InterfaceErrorWin(er_str, er_id); 
	    return;
	}

	i_p_g = new InsertParamGroup;
	i_p_g->setCaption(systemCodec->toUnicode("Редактирование группы товаров"));
	i_p_g->GetTypeForm(2,s.ascii());
	i_p_g->exec();
	grabKeyboard();
	delete(i_p_g);
	// printf(" InsertParamGroupId = %d\n", InsertParamGroupId);
	 ClearTable();
	 if (InsertParamGroupId ==0)
	 {
	    FillTable("0");
	 }
	 else
	 {
	     char * group_id =(char *) calloc(20,sizeof(char));
	     strcpy(group_id,itoa(InsertParamGroupId));
	     char * name =(char *) calloc(201,sizeof(char));
	     char * pgroup =(char *) calloc(20,sizeof(char));
	     char * null_str = (char*) calloc(1, sizeof(char));
	     strcpy(null_str,"");
	     M.Get_Parent_Info(group_id, name, pgroup);    
	//     printf("name = %s\n",name);
	//     printf("group_id = %s\n",group_id);
	     if (strcmp(pgroup,"")==0)
		 strcpy(pgroup,"0");
	     AddRowInTable(name,null_str, null_str,null_str, null_str,group_id,pgroup, true, true);
	     FillTable(group_id);
	     free(null_str);
	     free(group_id);
	     free(name);
	     free(pgroup);
	 }
    }
    grabKeyboard();
}
//====================================================================//

void MainWareform::CopyWare_clicked()
{
 //   puts("void MainWareform::CopyWare_clicked()");
    NumberWare =  dbaseCodec->fromUnicode(WareTable->text(WareTable->currentRow(),1));
    if ((NumberWare != NULL) && (NumberWare != "") && (NumberWare != " "))
    {
//	printf("NumberWare in MainWareform = %s\n",NumberWare.ascii());  
	a_w = new AddWareform;
	a_w->setCaption(systemCodec->toUnicode("Копирование карточки товара"));
	a_w->Myinit(3);
	a_w->OK->setText(systemCodec->toUnicode("Сохранить изменения"));
	HwSignalHandlersController::GetInstance()->SetControlledObject(WARE);
	a_w->exec();
	HwSignalHandlersController::GetInstance()->SetControlledObject(WARESEARCH);
	delete(a_w);
	ClearTable();
	if (WareFormGroupId ==0)
	{
	    FillTable("0");
	}
	else
	{
	    char * group_id =(char *) calloc(20,sizeof(char));
	    strcpy(group_id,itoa(WareFormGroupId));
	    char * name =(char *) calloc(201,sizeof(char));
	    char * pgroup =(char *) calloc(20,sizeof(char));
	    char * null_str = (char*) calloc(1, sizeof(char));
	    strcpy(null_str,"");
	    M.Get_Parent_Info(group_id, name, pgroup);    
	//    printf("name = %s\n",name);
	//    printf("group_id = %s\n",group_id);
	    if (strcmp(pgroup,"")==0)
		strcpy(pgroup,"0");
	    AddRowInTable(name,null_str, null_str, null_str, null_str,group_id,pgroup, true, true);
	    FillTable(group_id);
	    free(group_id);
	    free(null_str);
	    free(name);
	    free(pgroup);
	}	
    }
    else
    {  
	QString er_str;
	QString er_id;
	WareReferenceErrorText(45, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
    }
}

//====================================================================//

void MainWareform::ExitpushButton_clicked()
{
//puts("void MainWareform::ExitpushButton_clicked()");
close();
}

//====================================================================//

void MainWareform::ClearMessage()
{
//  puts("void MainWareform::ClearMessage()");
  messageTextLabel->setText("");
}


//====================================================================//
//===========Собственно путешествие по папкам при нажатии ENTER=======//
//====================================================================//

void MainWareform::EnterHandler()
{
 // puts("void MainWareform::EnterHandler()");  
  QTableSelection sel = WareTable -> selection(0);
  int p_sel = sel.topRow();
  if(p_sel != -1)
  {
      if(WareTable->text(p_sel, 4) == "")
      {
	  ChangeFolder();
      }
      else
      {
	  ChangeWare_clicked();
      }
  }
} 

void MainWareform::ChangeFolder()
{
   // puts("void MainWareform::ChangeFolder()");
    QTableSelection sel = WareTable -> selection(0);
    int p_sel = sel.topRow();
    if(WareTable->pixmap(p_sel, 0).serialNumber() == sernumber)
    {
	OutFolder();
    }
    else
    {
	InFolder();
    }
}

void MainWareform::InFolder()
{
   //puts("void MainWareform::InFolder()");
    bool f;
    QTableSelection sel = WareTable -> selection(0);
    int p_sel = sel.topRow();
    int gr = WareTable->text(p_sel, 6).toInt(&f);
    QTableItem * group = WareTable->item(p_sel, 6);
    QTableItem * pgroup = WareTable->item(p_sel, 7);
    QTableItem * name = WareTable->item(p_sel, 2);
    WareTable->takeItem(group);
    WareTable->takeItem(pgroup);
    WareTable->takeItem(name);
    ClearTable();
    char * null_str = (char*) calloc(1, sizeof(char));
    strcpy(null_str,"");
    AddRowInTable(null_str, null_str, null_str,null_str, null_str, null_str, null_str, true, true);
    free(null_str);
    WareTable->setItem(0, 2, name);
    WareTable->setItem(0, 6, group);
    WareTable->setItem(0, 7, pgroup);
    QString gr_qstr;
    gr_qstr = QString::number(gr);
    cur_pos = 0;
    FillTable(gr_qstr.ascii());
}

void MainWareform::OutFolder()
{
 //   puts("void MainWareform::OutFolder()");
    bool f;
    QTableSelection sel = WareTable -> selection(0);
    int p_sel = sel.topRow();
    int gr = WareTable->text(p_sel, 7).toInt(&f);
    cur_group = WareTable->text(p_sel, 6).toInt(&f);
    ClearTable();
    QString gr_qstr = QString::number(gr);
    char * group = (char *) calloc (gr_qstr.length() + 1, sizeof(char));
    strcpy(group, gr_qstr);
    if(gr != 0)
    {
	char *name, *pgroup;
	name = (char *) calloc(201, sizeof(char));
	pgroup = (char*) calloc(12, sizeof(char));
	M.Get_Parent_Info(group, name, pgroup);
	char * null_str = (char*) calloc(1, sizeof(char));
	strcpy(null_str,"");
	AddRowInTable(name, null_str, null_str, null_str, null_str, group, pgroup, true, true);
	free(name);
	free(null_str);
	free(pgroup);
    }
    if (gr == 0)
	FillTable(group);
    else
	FillTable(gr_qstr.ascii());
   
    free(group);
    
    cur_pos = 0;
    WareTable->clearSelection();
    sel.init(cur_pos, 0);
    sel.expandTo(cur_pos, 5);
    WareTable->addSelection(sel);
    WareTable -> ensureCellVisible(cur_pos,0);
}


//=====================================================================//
//------------------------------------------------------------СКИДКИ АВТОМАТИЧЕСКИЕ------------//
//=====================================================================//


//----------------------Заполнить список схем автоматич скидок -----------------------------//
int MainWareform::FillAutoReductionListBox(void)
{
 //   puts("int MainWareform::FillAutoReductionListBox(void)");
    //выделение памяти для mas
    int i;
    ExistSchemeAutocomboBox->clear();
    auto_group_count = M.GetQuantityStrInAutoSchemeGroup();
 //   printf("auto_group_count = %d\n",auto_group_count);
    if (auto_group_count == 0)
    {
	EditAutoSchemeGroup->setEnabled(FALSE);
	DeleteAutoSchemeGroup->setEnabled(FALSE);
	CreateAutopushButton->setEnabled(FALSE);
	EditAutopushButton->setEnabled(FALSE);
	DeleteAutoSchemepushButton->setEnabled(FALSE);
    }
    else
    {
	EditAutoSchemeGroup->setEnabled(TRUE);
	DeleteAutoSchemeGroup->setEnabled(TRUE);
	CreateAutopushButton->setEnabled(TRUE);
	EditAutopushButton->setEnabled(TRUE);
	DeleteAutoSchemepushButton->setEnabled(TRUE);
    }
    
   char ** mas = (char**) calloc(auto_group_count, sizeof(char*));
    for ( i=0; i<auto_group_count; ++i) 
    {
	mas[i] = (char*) calloc(201, sizeof(char));
    }
    M.GetNameAutoScheme(mas);
    for (i = 0;i<auto_group_count;i++)
    {
	if (mas[i]==NULL)
	{
	    return 0;
	}
	else
	{
	    ExistSchemeAutocomboBox->insertItem(dbaseCodec->toUnicode(QString(mas[i])));
	}
    } 
    
    int c = auto_group_count;
    while (--c=0) 
    {
	free(mas[c]);  
    }
    free(mas);
    return i;
}



//-----------Заполнить список автоматических скидок/надбавок ---------------------------//
int MainWareform::ChangeAutoGroup(void)
{
//    puts("int MainWareform::ChangeAutoGroup(void)\n");
   ExistAutoSchemelistView->clear();
    QString namegroup = dbaseCodec->fromUnicode(ExistSchemeAutocomboBox->currentText());
 //   printf("namegroup = |%s|\n",namegroup.ascii());
    if (namegroup != "")
    {
	char * value = (char *)calloc(20,sizeof(char));
	M.GetAutoSchemeId(namegroup.ascii(),value);
	QString s =  value;
	free(value);
	
	auto_reduction_count = M.GetQuantityStrInAutoReductionByAutoSchemeId(s.ascii());
	char ** mas = (char**) calloc(auto_reduction_count, sizeof(char*));
	int i;
	for ( i=0; i<auto_reduction_count; ++i) 
	{
	    mas[i] = (char*) calloc(201, sizeof(char));
	}
	int k = M.Get_AutoReductionByGroup(mas,s.ascii());
	//printf("k = %d\n",k);
	if (k == 0)
	{
	    EditAutopushButton->setEnabled(FALSE);
	    DeleteAutoSchemepushButton->setEnabled(FALSE);
	}
	else
	{
	    EditAutopushButton->setEnabled(TRUE);
	    DeleteAutoSchemepushButton->setEnabled(TRUE);
	}
	for (i = 0;i<k;i++)
	{
	    if (mas[i]==NULL)
	    {
		return 0;
	    }
	    else
	    {
		char * value = (char *)calloc(201,sizeof(char));
		M.GetAutoValueByName(mas[i],value);
		 new QListViewItem(ExistAutoSchemelistView,dbaseCodec->toUnicode(QString(mas[i])),dbaseCodec->toUnicode(value));
		 free(value);
	    }
	} 

	int c =auto_reduction_count;
	while (--c=0) 
	{
	    free(mas[c]);  
	}
	free(mas);
    }
    return 0;
}


//---------------------Удалить Схему автоматических скидок ---------------------------------//
void MainWareform::DeleteAutoSchemeGroup_clicked()
{
  //  puts("void MainWareform::DeleteAutoSchemeGroup_clicked()");
    QString name = dbaseCodec->fromUnicode(ExistSchemeAutocomboBox->currentText());
    if (name != "")
    {	
     //подтверждение получить от пользователя
     int t =  QMessageBox::information( this, systemCodec->toUnicode("Cообщение"),
systemCodec->toUnicode("<font color=red>Вы действительно хотите удалить схему автоматических скидок?</font>"),systemCodec->toUnicode("Да"),systemCodec->toUnicode("Нет"),0,1);
    if(t==0)
    {
	char * value = (char *)calloc(20,sizeof(char));
	M.GetAutoSchemeId(name.ascii(),value);
	QString L =  value;
	free(value);
	//printf("L =%s\n",L.ascii());
	//здесь нужно пересохранить все товары с такой group_id в NULL group_id
	//puts("Здесь нужно все зависимые от группы авт схем товары пересохранить с AutoScheme_id == NULL");
	//puts("Узнаем количество таких товаров");
	int kol_wares = M.GetQuantityStrInWareWithAutoSchemeId(L.ascii());
	//printf("kol_wares = %d\n",kol_wares);
	//puts("Теперь выделим памяти под их кол-во для заполнения internal_id");
	//по умолчанию считаю, что макс число товаров в базе огранич числом 10^32.
	char ** mass= (char** ) calloc(kol_wares,sizeof(char));
	int k;
	for (k=0; k<kol_wares; k++) 
	{
	    mass[k] = (char*) calloc(33, sizeof(char));
	}  
	M.LoadWaresByAutoSchemeId(L.ascii(),mass);
	for (k=0; k<kol_wares; k++) 
	{
	    int kol_field = 13;
	    char ** mas2 = (char**) calloc(kol_field, sizeof(char*));
	    for (int i=0; i<kol_field; ++i) 
	    {
		mas2[i] = (char*) calloc(201, sizeof(char));
	    }
	 //  printf("mass[%d] = %s\n",k,mass[k]);
	    M.LoadInfo(mass[k], mas2);
	    //в этом месте выгрузили информацию о 1 ед товара. 
	    //надо удалить товар из базы с таким id
                  M.DeleteWare(mas2[1]);
	    //удалили
	    
	    //теперь надо создать товар со всеми такими же параметрами, кроме tax_group_id
	    QString date = "CAST('"+(QString)mas2[8] +"' AS DATE ) ";
	    QString tax_group_id = (QString)mas2[12];
	    QString ware_group_id = (QString)mas2[11];
	     if (ware_group_id.isEmpty())
	     {
		// puts("ware_group_id.isEmpty()");
		if (tax_group_id.isEmpty()) 
		 { 
		//    puts("tax_group_id.isEmpty()");
//-----(*)-----Zuskin-----08/07/2011-----
		    //M.AddWareInTable(mas2[1], mas2[2], mas2[3], mas2[4], mas2[5], mas2[6], mas2[7], mas2[8], date.ascii(), "NULL", "NULL", "NULL",);
		    M.AddWareInTable(mas2[1], mas2[2], mas2[3], mas2[4], mas2[5], mas2[6], mas2[7], mas2[8], date.ascii(), "NULL", "NULL", "NULL", "0");
		}
		else
		{
		  //  puts(" !  tax_group_id.isEmpty()");

		    //M.AddWareInTable(mas2[1], mas2[2], mas2[3], mas2[4], mas2[5], mas2[6], mas2[7], mas2[8],  date.ascii(),  "NULL", "NULL", mas2[13]);
		    M.AddWareInTable(mas2[1], mas2[2], mas2[3], mas2[4], mas2[5], mas2[6], mas2[7], mas2[8],  date.ascii(),  "NULL", "NULL", mas2[13], "0");
		}
	     }
	     else
	     {
		// puts("  !   ware_group_id.isEmpty()");
		if (tax_group_id.isEmpty()) 
		 {
		  //  puts("tax_group_id.isEmpty()");
		    //M.AddWareInTable(mas2[1], mas2[2], mas2[3], mas2[4], mas2[5], mas2[6], mas2[7], mas2[8],  date.ascii(), "NULL", mas2[12], "NULL");
		    M.AddWareInTable(mas2[1], mas2[2], mas2[3], mas2[4], mas2[5], mas2[6], mas2[7], mas2[8],  date.ascii(), "NULL", mas2[12], "NULL", "0");
		}
		else
		{
		  //  puts(" !  tax_group_id.isEmpty()");
		    //M.AddWareInTable(mas2[1], mas2[2], mas2[3], mas2[4], mas2[5], mas2[6], mas2[7], mas2[8],   date.ascii(), "NULL", mas2[12], mas2[13]);
		    M.AddWareInTable(mas2[1], mas2[2], mas2[3], mas2[4], mas2[5], mas2[6], mas2[7], mas2[8],   date.ascii(), "NULL", mas2[12], mas2[13], "0");
//----------------------------------
		}
	     }
	   while (--kol_field=0) 
	 {
	     free(mas2[kol_field]); 
	 }
	 free(mas2);  
	}
	while (--kol_wares=0) 
	{
	    free(mass[kol_wares]); 
	}
	free(mass);
	M.DeleteAutoGroupWithAutoSchemeId(L.ascii());
	ExistAutoSchemelistView->clear();
	FillAutoReductionListBox();
	ChangeAutoGroup();
         }
    }
    else
    {
	QString er_str;
	QString er_id;
	WareReferenceErrorText(27, er_str, er_id);
	 InterfaceErrorWin(er_str, er_id);
    } 
}

//-----------------Редактировать Схему автоматических скидок ----------------------------//
void MainWareform::EditAutoSchemeGroup_clicked()
{
  // puts("void MainWareform::EditAutoSchemeGroup_clicked()"); 
    QString name = dbaseCodec->fromUnicode(ExistSchemeAutocomboBox->currentText());
    i_p_g = new InsertParamGroup;
    i_p_g->setCaption(systemCodec->toUnicode("Редактирование схемы автоматических скидок"));
    i_p_g->GetTypeForm(6, name);
    i_p_g->exec();
    grabKeyboard();
    delete(i_p_g);

 //   printf("EditAutoSchemeform return NAME = %s\n",newname1.ascii());
    FillAutoReductionListBox();
    for (int i = 0; i<ExistSchemeAutocomboBox->count();i++)
    {
	QString s = dbaseCodec->fromUnicode(ExistSchemeAutocomboBox->text(i));
	//printf("i = %d, s = %s\n",i,s.ascii());
	if (newname1 == s)
	{
	    ExistSchemeAutocomboBox->setCurrentItem(i);
	}
    }
}


//---------Создать Схему автоматических скидок-----------------------------------------------//
void MainWareform::CreateAutoSchemeGroup_clicked()
{
   // puts("void MainWareform::CreateAutoSchemeGroup_clicked()");
    
    i_p_g = new InsertParamGroup;
    i_p_g->setCaption(systemCodec->toUnicode("Создание схемы автоматических скидок"));
    i_p_g->GetTypeForm(5, "");
    i_p_g->exec();
    grabKeyboard();
    delete(i_p_g);
    ExistSchemeAutocomboBox->clear();
    FillAutoReductionListBox();
    int n = ExistSchemeAutocomboBox->count();
    ExistSchemeAutocomboBox->setCurrentItem(n-1);
    ChangeAutoGroup();
}


//-------Создать автоматическую скидку---------------------------------------------------------//
void MainWareform::CreateAutopushButton_clicked()
{
  // puts("void MainWareform::CreateAutopushButton_clicked()");
    QString name = dbaseCodec->fromUnicode(ExistSchemeAutocomboBox->currentText());
    char * value = (char *)calloc(20,sizeof(char));
    M.GetAutoSchemeId(name.ascii(),value);
    QString L =  value;
    free(value);
    a_a_r = new AddAutoRedform;
    a_a_r->Myinit(1,L.ascii());
    a_a_r->exec();
    delete(a_a_r);
    ChangeAutoGroup();
    
}

//------Редактировать автоматическую скидку/надбавку ---------------------------------//
void MainWareform::EditAutopushButton_clicked()
{
  //  puts("void MainWareform::EditAutopushButton_clicked()");
    QString er_str;
    QString er_id;
    if (ExistAutoSchemelistView->childCount()  == 0)
    {
	WareReferenceErrorText(31, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);	
	return;
    }
    else
    {
	QString name;
	if (ExistAutoSchemelistView->childCount() == 1)
	{
	    name = dbaseCodec->fromUnicode(ExistAutoSchemelistView->firstChild()->text(0));
	}
	else
	{
	    name = dbaseCodec->fromUnicode(ExistAutoSchemelistView->currentItem()->text(0));
	    if (name == "")
	    {
		WareReferenceErrorText(30, er_str, er_id);
		InterfaceErrorWin(er_str, er_id);	
		return;
	    }
	}
	//printf("name = %s\n",name.ascii());
	char * value = (char *)calloc(201, sizeof(char));
	M.GetAutoRedIdByName(name.ascii(),value);
	QString L = value;
	free(value);
	//printf("L = %s\n",L.ascii());
	a_a_r = new AddAutoRedform;
	a_a_r->setCaption(systemCodec->toUnicode("Редактировать автоматическую скидку"));
	a_a_r->OK->setText(systemCodec->toUnicode("Сохранить изменения"));
	a_a_r->Myinit(2, L.ascii());
	a_a_r->exec();
	delete(a_a_r);
    }
    ChangeAutoGroup();
}

//----------------Удалить  скидку/надбавку --------------------------------------------------//
void MainWareform::DeleteAutoSchemepushButton_clicked()
{
   // puts("void MainWareform::DeleteAutoSchemepushButton_clicked()");
    QString Name;
    QString Id;
    QString er_str;
    QString er_id;
    if (ExistAutoSchemelistView->childCount() == 0)
    {
	WareReferenceErrorText(31, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return;
    }
    else
    {
	if (ExistAutoSchemelistView->childCount() ==1)
	{
	    Name = dbaseCodec->fromUnicode(ExistAutoSchemelistView->firstChild()->text(0));
	}
	else
	{
	    Name = dbaseCodec->fromUnicode(ExistAutoSchemelistView->currentItem()->text(0));
	    if (Name == "")
	    {
		WareReferenceErrorText(32, er_str, er_id);
		InterfaceErrorWin(er_str, er_id);
		return;
	    }
	}
	 //подтверждение получить от пользователя
                int t =  QMessageBox::information( this, systemCodec->toUnicode("Cообщение"),
systemCodec->toUnicode("<font color=red>Вы действительно хотите удалить автоматическую скидку?</font>"),systemCodec->toUnicode("Да"),systemCodec->toUnicode("Нет"),0,1);
                if(t==0)
               {
	
	
	//  printf("Name = %s\n",Name.ascii());
	  char* value = (char * )calloc(201,sizeof(char));
	  M.GetAutoRedIdByName(Name.ascii(),value);
	  Id =value;
	  free(value);
	//  printf("Id = %s\n",Id.ascii());
	  int d = M.DeleteAutoRedWithAutoRedId(Id.ascii());
	//  printf("d = %d\n",d);
	  if (d == 1)
	 {
	      WareReferenceMessageText(8,er_str,er_id);
	      InterfaceMessageWin(er_str,er_id);
	      ChangeAutoGroup();
	}
      }
    }
}


//=====================================================================//
//------------------------------------------------------ГРУППЫ ТОВАРОВ-------------------------------//
//=====================================================================//


//------Создать группу товаров без родительской группы ---------------------------------//
void MainWareform::CreateWareGroupButton_clicked()
{
  //  puts("void MainWareform::CreateWareGroupButton_clicked()");
    char * s1 = (char *)calloc(20,sizeof(char));
    strcpy(s1,"0");
    i_p_g = new InsertParamGroup;
    i_p_g->setCaption(systemCodec->toUnicode("Создание группы товаров"));
    i_p_g->GetTypeForm(1,s1);
    i_p_g->exec();
    grabKeyboard();
    delete(i_p_g);
   
    ClearTable();
    FillTable(s1);
    free(s1); 
    FillWareGroup_list();
    LastAddedNameOfNode = dbaseCodec->toUnicode(newname1); 
   // printf("newname1 = %s\n",newname1.ascii());
    OpenNeedPath();
    InsertNeedNode(newname1); 
}


//------------ Создать группу товаров  в текущей группе-----------------------------------//
void MainWareform::CreateWareGroupInExistGroupButton_clicked()
{
  //  puts("void MainWareform::CreateWareGroupInExistGroupButton_clicked()");
    QString name;
    QString id;
    if (WareGroup_listView->childCount() == 1)
    {
	name = dbaseCodec->fromUnicode(WareGroup_listView->firstChild()->text(0));
//	puts("WareGroup_listView->childCount() == 1");
//	printf("name = %s\n",name.ascii());
    }
    else
    {
//	puts("WareGroup_listView->childCount() != 1");
	name = dbaseCodec->fromUnicode(WareGroup_listView->currentItem()->text(0));
//	printf("name = %s\n",name.ascii());
	if (name == "")
	{
	    QString er_str;
	    QString er_id; 
	    WareReferenceErrorText(34, er_str, er_id);
	    InterfaceErrorWin(er_str, er_id);	
	    return;
	}
    }
    
    char * value = (char *)calloc(20,sizeof(char));
    M.GetGroupId(name.ascii(),value);	
   // printf("value = %s\n",value);
    if ((strcmp(value,"ERROR")!=0) && (strcmp(value,"")!=0))
    {
	i_p_g = new InsertParamGroup;
	i_p_g->setCaption(systemCodec->toUnicode("Создание группы товаров"));
	i_p_g->GetTypeForm(1,value);
	i_p_g->exec();
	grabKeyboard();
	delete(i_p_g);
	//printf("InsertParamGroup return NAME = %s\n",newname1.ascii());
	ClearTable();
	FillTable("0");
	FillWareGroup_list();
	
	LastAddedNameOfNode = dbaseCodec->toUnicode(newname1);
	OpenNeedPath();
	InsertNeedNode(newname1); 
    }
      free(value);
}

void MainWareform::Edit_WareGroup_clicked()
{
//    puts("void MainWareform::Edit_WareGroup_clicked()");
    QString s = dbaseCodec->fromUnicode(WareGroup_listView->currentItem()->text(0));
  // printf("s.ascii = %s\n",s.ascii());
    i_p_g = new InsertParamGroup;
    i_p_g->setCaption(systemCodec->toUnicode("Редактирование группы товаров"));
    i_p_g->GetTypeForm(2,s.ascii());
    i_p_g->exec();
    grabKeyboard();
    delete(i_p_g);
    ClearTable();
    FillTable("0");
    FillWareGroup_list();
    LastAddedNameOfNode = dbaseCodec->toUnicode(newname1);
  //  printf("newname1.ascii = %s\n",newname1.ascii());
    OpenNeedPath();
    InsertNeedNode(newname1);
}


//---------------- Раскрыть дерево в нужном месте ----------------------------------------------//
void MainWareform::OpenNeedPath()
{
 //   puts("void MainWareform::OpenNeedPath(void)");
    int i = 0;
    int j = 0;
    int v = 0;
    QListViewItem * startplv = WareGroup_listView ->firstChild();
    QListViewItem * plv;
    QListViewItem ** ItemMas = NULL;
    for (plv= startplv; plv!=0; plv = plv->itemBelow())//открыть все узлы, чтобы посмотреть что в них
    {
	plv->setOpen(TRUE);
    }
    for (plv = startplv ; plv!=0; plv = plv->itemBelow())
    {
	i++;
	if (plv->text(0) == LastAddedNameOfNode)//заполняем массив указателями, ктр. должны быть открыты в списке
	{ 
	    v = plv->depth()+1;
	    ItemMas = (QListViewItem **) calloc (v ,sizeof(QListViewItem *));
	    for (j = 0; j<v;j++)//выделяем память под указатели и заполняем родителями
	    {
		ItemMas[j] = (QListViewItem *) calloc(max_number_group,sizeof(QListViewItem *)); 
		ItemMas[j] =  plv;
		plv= plv->parent(); 
	    }  
	    break;
	}
    }
    for (plv= startplv; plv!=0; plv = plv->itemBelow())//по всем элементам списка закрыть все узлы
    { 
	plv->setOpen(FALSE);
    } 
    for (int k = 0;k<v;k++)
    {
	ItemMas[v-k-1]->setOpen(TRUE);//открыть нужные узлы
	for (plv=ItemMas[v-k-1]->itemBelow(); plv!=0; plv = plv->itemBelow())//по всем элементам списка закрыть все узлы
	{
	    plv->setOpen(FALSE);
	} 
    }
    while (--v = 0) 
    {
	free( ItemMas[v]);  
    }
    free( ItemMas);
    return;
}

//------------ Выделить только что добавленный элемент ----------------------------------//
void MainWareform::InsertNeedNode(QString name)
{
 //   puts("void MainWareform::InsertNeedNode(QString name)"); 
    QListViewItem * plv;
     for (plv=WareGroup_listView ->firstChild(); plv!=0; plv = plv->itemBelow())
    {
	 if ((QString)dbaseCodec->fromUnicode(plv->text(0)) == name)
	 {
	     WareGroup_listView->setSelected(plv,TRUE);
	     return; 
	 }
    } 
}



//-------------- Удалить группу товаров ------------------------------------------------------------//

void MainWareform::DeleteWareGroupButton_clicked()
{
 //   puts("void MainWareform::DeleteWareGroupButton_clicked()");
    QString name;
    if (WareGroup_listView->childCount() == 1)
    {
	name = dbaseCodec->fromUnicode(WareGroup_listView->firstChild()->text(0));

    }
    else
    {
	
	name = dbaseCodec->fromUnicode(WareGroup_listView->currentItem()->text(0));
	
	if (name == "")
	{
	    QString er_str;
	    QString er_id;
	    WareReferenceErrorText(35, er_str, er_id);
	    InterfaceErrorWin(er_str, er_id);
	    return;
	}
    }
    //подтверждение получить от пользователя
    int t =  QMessageBox::information( this, systemCodec->toUnicode("Cообщение"),
systemCodec->toUnicode("<font color=red>Вы действительно хотите удалить группу товаров со всеми входящими в нее товарами?</font>"),systemCodec->toUnicode("Да"),systemCodec->toUnicode("Нет"),0,1);
    if(t==0)
    {
	//необходимо получить все internal_id товаров, ктр удаляются
	
	int col_ware = M.GetQuantityWaresInGroup(name.ascii());
	//printf("col_ware = %d\n",col_ware);
	char ** mas = (char**) calloc(col_ware, sizeof(char*));
	for (int  i=0; i<col_ware; i++) 
	{
	    mas[i] = (char*) calloc(20, sizeof(char));
	}
	M.Get_Id_By_Group_Longtitle(name.ascii(), mas);
	
	for (int k = 0; k<col_ware;k++)
	{
	 //   printf("mas[%d] = %s\n",k,mas[k]);
	    M.DeleteBarcodeOfWare(mas[k]);
	}
	
	while (--col_ware = 0) 
	{		
	    free(mas[col_ware]);  
	}	   
	free(mas); 
	M.DeleteWareGroup(name.ascii());
	FillWareGroup_list();
	ClearTable();
	FillTable("0");
    }
}



//--------------- Заполнить дерево групп ------------------------------------------------------------//
void MainWareform::FillWareGroup_list()
{
   // puts("void MainWareform::FillWareGroup_list()");
    int i = 0;
    WareGroup_listView->clear();
    int null_ware_group_count = M.GetQuantityStrInNULLWareGroup();
    if (null_ware_group_count == 0)
    {
	CreateWareGroupInExistGroupButton->setEnabled(FALSE);
	DeleteWareGroupButton->setEnabled(FALSE);
	Edit_WareGroup->setEnabled(FALSE);
    }
    else
    {
	CreateWareGroupInExistGroupButton->setEnabled(TRUE);
	DeleteWareGroupButton->setEnabled(TRUE);
	Edit_WareGroup->setEnabled(TRUE);
    }
    
     if (null_ware_group_count > 0)
    {
	 char ** mas = (char**) calloc(null_ware_group_count, sizeof(char*));
	 for ( i=0; i<null_ware_group_count; i++) 
	 {
	     mas[i] = (char*) calloc(201, sizeof(char));
	 }
	 M.GetNameWareGroupByParentId("NULL",mas);
	 for (i = 0;i<null_ware_group_count;i++)
	 {
		  char * value = (char *)calloc(20,sizeof(char));
		  M.GetGroupId(mas[i],value);
		  QString s =  value;
		  free(value); 
		  QListViewItem * plvItem = new QListViewItem(WareGroup_listView,dbaseCodec->toUnicode(QString(mas[i])),s); 
		  plvItem->setOpen(true);
		  ShowNextLevel(plvItem);
	 }
	 while (--null_ware_group_count = 0) 
	 {
	     free(mas[null_ware_group_count]);  
	 }
	 free(mas);
	 WareGroup_listView->currentItem()->setSelected(TRUE);
     }   
}


//------------------- Заполнить ветку дерева -------------------------------------------------------//
int MainWareform:: ShowNextLevel(QListViewItem  *plvItem)
{
	// puts("int MainWareform:: ShowNextLevel(QListViewItem  *plvItem)");
	 int i = 0;
	 QString str  = dbaseCodec->fromUnicode(plvItem->text(0));
	 //printf("name of QListViewItem = |%s|\n",str.ascii());
	 QListViewItem * newItem = plvItem;
	 QString group = dbaseCodec->fromUnicode(plvItem->text(1));
	 //printf("group of QListViewItem = |%s|\n",group.ascii());
	 if (group !="ERROR")
	 {
		  //puts("(!s.isEmpty())");
		  int k1 = M.GetQuantityStrInNotNULLWareGroup(group.ascii());
		 // printf("k1 = %d\n",k1);
		  if (k1 >0)
		  {
				char ** mas11 = (char**) calloc(k1, sizeof(char*));
				for (  i=0; i<k1; i++) 
				{
					 mas11[i] = (char*) calloc(201, sizeof(char));
				}
				M.GetNameWareGroupByParentId(group.ascii(),mas11,0, k1);    
				for ( i = 0;i<k1;i++)
				{
					 char * value = (char *)calloc(20,sizeof(char));
					 M.GetGroupId(mas11[i], value, group.ascii());
					 QString s = value;
					// printf("insert qlistviewitem * othersItem  by name = |%s|,group_id = |%s|\n", mas11[i], value);
					 free(value);
					 QListViewItem  * othersItem = new QListViewItem(newItem,dbaseCodec->toUnicode(QString(mas11[i])),s);
					 ShowNextLevel(othersItem);
				}
				while (--k1 = 0) 
				{		
					 free(mas11[k1]);  
				}	   
				free(mas11); 
		  }
		  return k1;
	 }
	 return 0;  
}




//=====================================================================//
//---------------------------------------НАЛОГИ------------------------------------------------------------//
//=====================================================================//


//--------------------------- Заполнение списка налоговых групп --------------------------------//
int MainWareform::FillTaxListBox(void)
{
  //  puts("int MainWareform::FillTaxListBox(void)");
  //  puts("------------------------------------------------------------------------------------------------");
    //выделение памяти для mas
    int i;
    ExistTaxGroupcomboBox->clear();
    tax_group_count = M.GetQuantityStrInTaxGroup();
   // printf("tax_group_count = %d\n",tax_group_count);
    if (tax_group_count == 0)
    {
	EditTaxGroup->setEnabled(FALSE);
	DeleteTaxGroup->setEnabled(FALSE);
	CreateTax->setEnabled(FALSE);
	EditTax->setEnabled(FALSE);
	DeleteTax->setEnabled(FALSE);
	UpButton->setEnabled(FALSE);
	DownButton->setEnabled(FALSE);
    }
    else
    {
	EditTaxGroup->setEnabled(TRUE);
	DeleteTaxGroup->setEnabled(TRUE);
	CreateTax->setEnabled(TRUE);
	EditTax->setEnabled(TRUE);
	DeleteTax->setEnabled(TRUE);
	UpButton->setEnabled(TRUE);
	DownButton->setEnabled(TRUE);
    }
    
    char ** mas = (char**) calloc(tax_group_count, sizeof(char*));
    for ( i=0; i<tax_group_count; ++i) 
    {
	mas[i] = (char*) calloc(201, sizeof(char));
    }
    M.GetNameTaxGroup(mas);
    
    for (i = 0;i<tax_group_count;i++)
    {
	if (mas[i]==NULL)
	{
	    return 0;
	}
	else
	{
	    ExistTaxGroupcomboBox->insertItem(dbaseCodec->toUnicode(QString(mas[i])));
	}
    } 
    int c = tax_group_count;
    while (--c=0) 
    {
	free(mas[c]);  
    }
    free(mas);
    return i;
}


//-------------------------------- Заполнение списка ставок ---------------------------------------//
int MainWareform::ChangeTaxGroup(void)
{
  //  puts("int MainWareform::ChangeTaxGroup(void)");
    ExistTaxlistBox->clear();
    QString taxgroup = dbaseCodec->fromUnicode(ExistTaxGroupcomboBox->currentText());
    if (taxgroup != "")
    {	
	char * value = (char *)calloc(201,sizeof(char));
	M.GetTaxGroupId(taxgroup.ascii(),value);
	QString s =value;
	free(value);
	tax_count = M.GetQuantityStrInTaxRateByTaxGroupId(s.ascii());
	char ** mas = (char**) calloc(tax_count , sizeof(char*));
	int i;
	for ( i=0; i<tax_count; ++i) 
	{
	   mas[i] = (char*) calloc(201, sizeof(char));
	}
	int k = M.Get_TaxRateByGroup(mas, s.ascii());
	if (k == 0)
	{
	    EditTax->setEnabled(FALSE);
	    DeleteTax->setEnabled(FALSE);
	    UpButton->setEnabled(FALSE);
	    DownButton->setEnabled(FALSE);
	}
	else
	{
	    EditTax->setEnabled(TRUE);
	    DeleteTax->setEnabled(TRUE);
	    UpButton->setEnabled(TRUE);
	    DownButton->setEnabled(TRUE);
	}
	
	for (i = 0;i<k;i++)
	{
	    if (strcmp(mas[i],"") == 0)
	    {
		return 0;
	    }
	    else
	    {
		ExistTaxlistBox->insertItem(dbaseCodec->toUnicode(QString(mas[i])));
	    }
	} 
	int c =tax_count;
	while (--c=0) 
	{
	    free(mas[c]);  
	}
	free(mas);
    }
    ExistTaxlistBox->setCurrentItem(0);
    return 0;
}


//----------------------------- Создание налоговой группы-------------------------------------------//

void MainWareform::CreateTaxGroup_clicked()
{
 //  puts("void MainWareform::CreateTaxGroup_clicked()");
    i_p_g = new InsertParamGroup;
    i_p_g->setCaption(systemCodec->toUnicode("Создание налоговой группы"));
    i_p_g->GetTypeForm(3,"");
    i_p_g->exec();
    grabKeyboard();
    delete(i_p_g);
    ExistTaxGroupcomboBox->clear();
    FillTaxListBox(); 
    int n = ExistTaxGroupcomboBox->count();
    ExistTaxGroupcomboBox->setCurrentItem(n-1);
    ChangeTaxGroup();
}

//------------------- Редактирование Налоговой группы-----------------------------------------//
void MainWareform::EditTaxGroup_clicked()
{
  //  puts("void MainWareform::EditTaxGroup_clicked()");
    
    QString name = dbaseCodec->fromUnicode(ExistTaxGroupcomboBox->currentText());
    i_p_g = new InsertParamGroup;
    i_p_g->setCaption(systemCodec->toUnicode("Редактирование налоговой группы"));
    i_p_g->GetTypeForm(4, name);
    i_p_g->exec();
    grabKeyboard();
    delete(i_p_g);

   // printf("EditAutoSchemeform return NAME = %s\n",newname1.ascii());
    FillTaxListBox(); 
    FillAutoReductionListBox();
    for (int i = 0; i<ExistTaxGroupcomboBox->count();i++)
    {
	QString s = dbaseCodec->fromUnicode(ExistTaxGroupcomboBox->text(i));
	//printf("i = %d, s = %s\n",i,s.ascii());
	if (newname1 == s)
	{
	    ExistTaxGroupcomboBox->setCurrentItem(i);
	}
    }
}

//--------------------------------- Удаление  Налоговой группы--------------------------------//
void MainWareform::DeleteTaxGroup_clicked()
{
    //puts("void MainWareform::DeleteTaxGroup_clicked()");
    QString name = dbaseCodec->fromUnicode(ExistTaxGroupcomboBox->currentText()); 
    QString er_str;
    QString er_id;
    if (name != "")
    {
        //подтверждение получить от пользователя
       int t =  QMessageBox::information( this, systemCodec->toUnicode("Cообщение"),
systemCodec->toUnicode("<font color=red>Вы действительно хотите удалить группу налогов?</font>"),systemCodec->toUnicode("Да"),systemCodec->toUnicode("Нет"),0,1); 
       if(t==0)
       {
	    char * value = (char *)calloc(201,sizeof(char));  
	    M.GetTaxGroupId(name.ascii(),value);
	    QString L = value;
	    free(value);
	  //  printf("TaxGroupId удаляемой группы =%s\n",L.ascii());
	
	    int tax_rate_count = M.GetQuantityStrInTaxRateByTaxGroupId(L.ascii());
	    char ** mas = (char**) calloc(tax_rate_count, sizeof(char*));
	    int i;
	    for ( i=0; i<tax_rate_count; ++i) 
	    {
	      mas[i] = (char*) calloc(201, sizeof(char));
	    }  
	    M.Get_TaxRateByGroup(mas,L.ascii());
	 //   puts("Заполнили mas и удаляем ставки налоговые по одной");
	    for ( i=0; i<tax_rate_count; ++i) 
	    {
	   //  printf("mas[%d] = %s\n",i,mas[i]);
	      M.DeleteTaxRateByLongtitle(mas[i]);
	    }  
	 //   puts("Удалили ставки теперь удаляем связи!");
	    M.DeleteTaxRateGroupByTaxGroupId(L.ascii());
	
	
	 //   puts("Здесь нужно все зависимые от группы налогов товары пересохранить с Tax_group_id == NULL");
	 //   puts("Узнаем количество таких товаров");
	    int kol_wares = M.GetQuantityStrInWareWithTaxGroupId(L.ascii());
	 //   printf("kol_wares = %d\n",kol_wares);
	  //  puts("Теперь выделим памяти под их кол-во для заполнения internal_id");
	    char ** mass= (char** ) calloc(kol_wares,sizeof(char));
	    int k;
	    for (k=0; k<kol_wares; k++) 
	    {
	      mass[k] = (char*) calloc(10, sizeof(char));
         }  
	
	    M.LoadWaresByTaxRateId(L.ascii(),mass);
	    for (k=0; k<kol_wares; k++) 
	    {
	      int * kol_field = (int *) calloc(1,sizeof(int));
			
	      M.GetQuantityFieldsWare(kol_field);
		 *kol_field = *kol_field+1;//количество полей о товаре + штрих-код
	      char ** mas2 = (char**) calloc(*kol_field, sizeof(char*));
	      for ( i=0; i<*kol_field; ++i) 
	      {
		   mas2[i] = (char*) calloc(201, sizeof(char));
	      }
	      M.LoadInfo(mass[k], mas2);
	      //в этом месте выгрузили информацию о 1 ед товара. 
	      //надо удалить товар из базы с таким id
           M.DeleteWare(mas2[0]);
	      //удалили
	    
	      //теперь надо создать товар со всеми такими же параметрами, кроме tax_group_id
	      QString date = "CAST('"+(QString)mas2[9] +"' AS DATE ) ";
	      QString autoscheme = (QString)mas2[10];
	      QString ware_group_id = (QString)mas2[12];
		//	printf("date  = %s\n",date.ascii());
		//	printf("autoscheme  = %s\n",autoscheme.ascii());
		//	printf("ware_group_id  = %s\n",ware_group_id.ascii());
			
	       if (ware_group_id.isEmpty())
	       {
		    if (autoscheme.isEmpty()) 
		    { 
		      M.AddWareInTable(mas2[0], mas2[2], mas2[3], mas2[4], mas2[5], mas2[6], mas2[7], mas2[8],   date.ascii(), "NULL", "NULL", "NULL", "0");
		    }
		    else
		    {
		      M.AddWareInTable(mas2[0], mas2[2], mas2[3], mas2[4], mas2[5], mas2[6], mas2[7], mas2[8],  date.ascii(), mas2[10], "NULL", "NULL", "0");
		    }
	       }
	       else
            {
		    if (autoscheme.isEmpty()) 
               {
		      M.AddWareInTable(mas2[0], mas2[2], mas2[3], mas2[4], mas2[5], mas2[6], mas2[7], mas2[8],  date.ascii(), "NULL", mas2[12], "NULL", "0");
		    }
		    else
		    {
		      M.AddWareInTable(mas2[0], mas2[2], mas2[3], mas2[4], mas2[5], mas2[6], mas2[7], mas2[8],  date.ascii(), mas2[10], mas[12], "NULL", "0");
		    }
	       }
	       while (--*kol_field=0) 
	       {
	         free(mas2[*kol_field]); 
	       }
	       free(mas2);  
	       free(kol_field);
	     }
	
	     while (--kol_wares=0) 
	     {
	       free(mass[kol_wares]); 
	     }
	     free(mass);
	
	   //  puts("Теперь удаляем саму группу налоговую!");
	     int g = M.DeleteTaxGroupWhereTaxGroupId(L.ascii());
         // printf("g = %d\n",g);
	    while (--tax_rate_count=0) 
	    {
	      free(mas[tax_rate_count]); 
	    }
	    free(mas);
	    if (g==1)
	    {
	      WareReferenceMessageText(10,er_str,er_id);
	      InterfaceMessageWin(er_str,er_id);
	    }
	    ExistTaxlistBox->clear();
	    FillTaxListBox(); 
	    ChangeTaxGroup();
       }
     }
     else
     {
	  WareReferenceErrorText(27, er_str, er_id);
	  InterfaceErrorWin(er_str, er_id);
     }
}

//-------------------------------- Создание  налоговой ставки--------------------------------------//
void MainWareform::CreateTax_clicked()
{
   // puts("void MainWareform::CreateTax_clicked()");
    QString name = dbaseCodec->fromUnicode(ExistTaxGroupcomboBox->currentText());  
  //  printf("name = %s\n",name.ascii());
    if (name != "")
    {
	char * value = (char *)calloc(201,sizeof(char));
	M.GetTaxGroupId(name.ascii(),value);
	QString L = value;
	free(value);
	//printf("TaxGroupId = %s\n",L.ascii());
	t_r_g = new TaxRateGroupform;
	t_r_g->Myinit(1, L.ascii(),"");
	t_r_g->exec();
	grabKeyboard();
	delete(t_r_g);
	ChangeTaxGroup();
    }
    else
    {   
	QString er_str;
	QString er_id;
	WareReferenceErrorText(37, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
    }
}

//------------------- Редактирование налоговой ставки-----------------------------------------//
void MainWareform::EditTax_clicked()
{
  //  puts("void MainWareform::EditTax_clicked()");
    QString er_str;
    QString er_id;
    if (ExistTaxlistBox->count()  == 0)
    {
	//puts(" 111  ");
	WareReferenceErrorText(38, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);	
	return;
    }
    else
    {
	//puts(" 222  ");
	QString name;
	if (ExistTaxlistBox->count() == 1)
	{
	    name = dbaseCodec->fromUnicode(ExistTaxlistBox->text(0));
	}
	else
	{
	    name = dbaseCodec->fromUnicode(ExistTaxlistBox->currentText());
	    if (name == "")
	    {	
		WareReferenceErrorText(39, er_str, er_id);
		InterfaceErrorWin(er_str, er_id);	
		return;
	    }
	}
	//printf("name = %s\n",name.ascii());
	char * value = (char *)calloc(201,sizeof(char));
	M.GetTaxRateIdByName(name.ascii(),value);
	QString L = value;
	free(value);
	//printf("L = %s\n",L.ascii());
	QString namegroup = dbaseCodec->fromUnicode(ExistTaxGroupcomboBox->currentText());
	char * value1 = (char *)calloc(201,sizeof(char));
	M.GetTaxGroupId(namegroup.ascii(),value1);
	QString LG = value1;
	free(value1);
	//printf("LG = %s\n",LG.ascii());
	t_r_g = new TaxRateGroupform;
	t_r_g->setCaption(systemCodec->toUnicode("Редактировать налоговую ставку"));
	t_r_g->OK->setText(systemCodec->toUnicode("Сохранить"));
	t_r_g->Myinit(2, dbaseCodec->fromUnicode(L),dbaseCodec->fromUnicode(LG));
	t_r_g->exec();
	grabKeyboard();
	delete(t_r_g);
	ChangeTaxGroup();
	return;
    }
}

//------------------------- Удаление  налоговой ставки----------------------------------------//
void MainWareform::DeleteTax_clicked()
{
    //puts("void MainWareform::DeleteTax_clicked()");
    QString er_str;
    QString er_id;
    if (ExistTaxlistBox->count()  == 0)
    {
	WareReferenceErrorText(38, er_str, er_id);
	 InterfaceErrorWin(er_str, er_id);	
	return;
    }
    else
    {
       //подтверждение получить от пользователя
       int t =  QMessageBox::information( this, systemCodec->toUnicode("Cообщение"),
systemCodec->toUnicode("<font color=red>Вы действительно хотите удалить налоговую ставку?</font>"),systemCodec->toUnicode("Да"),systemCodec->toUnicode("Нет"),0,1);
       if(t==0)
       {
	QString name;
	if (ExistTaxlistBox->count() == 1)
	{
	    name = dbaseCodec->fromUnicode(ExistTaxlistBox->text(0));
	}
	else
	{
	    name = dbaseCodec->fromUnicode(ExistTaxlistBox->currentText());
	    if (name == "")
	    {
		WareReferenceErrorText(41, er_str, er_id);
		InterfaceErrorWin(er_str, er_id);	
		return;
	    }
	}
	//printf("name = %s\n",name.ascii());
	char * value = (char *)calloc(201,sizeof(char));
	M.GetTaxRateIdByName(name.ascii(),value);
	QString L = value;
	free(value);
	//printf("L = %s\n",L.ascii());
	int g =M.DeleteTaxRate(L.ascii());
	//printf("g  = %d\n",g);
	if (g>=0)  
	{
	    WareReferenceMessageText(11,er_str,er_id);
	    InterfaceMessageWin(er_str,er_id);
	}
	ChangeTaxGroup();
      }
   }
}


//--------------------------- Работа с изменением порядка начисления скидок---------------//


void MainWareform::ReplaceOrder(const char * tax_rate_id1,const char * order_id1, const char * tax_rate_id2, const char * order_id2)
{
   // puts("void MainWareform::ReplaceOrder(const char * tax_rate_id1,const char * order_id1, const char * tax_rate_id2, const char * order_id2)");
    char * id1 = (char *) calloc(15, sizeof(char));
    char * base_flag1 = (char *) calloc(3, sizeof(char));
    char * tax_group_id1 = (char *) calloc(15, sizeof(char));
    char * id2 = (char *) calloc(15, sizeof(char));
    char * base_flag2 = (char *) calloc(3, sizeof(char));
    char * tax_group_id2 = (char *) calloc(15, sizeof(char));
    M.Get_Tax_Rate_Group_Info( id1, base_flag1,  tax_group_id1, tax_rate_id1, order_id1);
  /*puts("---------------------------------------------------------");
    printf("id1 = %s\n",id1);
    printf("base_flag1 = %s\n",base_flag1);
    printf("tax_group_id1 = %s\n",tax_group_id1);
    printf("tax_rate_id1 = %s\n",tax_rate_id1);
    printf("order_id1 = %s\n",order_id1);
    puts("---------------------------------------------------------");*/
    M.Get_Tax_Rate_Group_Info(id2, base_flag2,  tax_group_id2, tax_rate_id2, order_id2);
  /*puts("---------------------------------------------------------");
   printf("id2 = %s\n",id2);
    printf("base_flag2 = %s\n",base_flag2);
    printf("tax_group_id2 = %s\n",tax_group_id2);
    printf("tax_rate_id2 = %s\n",tax_rate_id2);
    printf("order_id2 = %s\n",order_id2);
    puts("---------------------------------------------------------");*/
    
    //удаление связей
    M.DeleteFromTaxRateGroupById(id1);
    M.DeleteFromTaxRateGroupById(id2);
    //создание новых
    QString code1;
    QString code2;
    code1 = (QString)id1 + ",'"+(QString) base_flag1 +"',"+ (QString)tax_group_id1 +","+(QString)tax_rate_id1 +","+(QString)order_id2;
    M.Create_Tax_Rate_Group(code1);
    code2 = (QString)id2 + ",'"+ (QString)base_flag2 +"',"+(QString)tax_group_id2 +","+(QString)tax_rate_id2 +","+(QString)order_id1;
    M.Create_Tax_Rate_Group(code2);	
    free(id1);
    free(base_flag1);
    free(tax_group_id1);		
    free(id2);
    free(base_flag2);
    free(tax_group_id2);
}



void MainWareform::UpButton_clicked()
{
	 // puts("void MainWareform::UpButton_clicked()");
	 QString mes_str;
	 QString mes_id;
	 //проверка выделен ли элемент вообще, если нет - первый выделить
	 int pos  = ExistTaxlistBox->currentItem();
	 if(pos == -1)
	 {
		  ExistTaxlistBox->setCurrentItem(0);
		  WareReferenceMessageText(12, mes_str, mes_id);
		  InterfaceMessageWin(mes_str,mes_id);
		  return;
	 }
	 else
	 {
		  //проверка на есть ли сверху текущего элемента еще один элемент
		  if (pos ==0)
		  {
				WareReferenceMessageText(12,mes_str,mes_id);
				InterfaceMessageWin(mes_str,mes_id);
				return;
		  }
		  else
		  {
				QString Name1 = dbaseCodec->fromUnicode(ExistTaxlistBox->text(pos));
				//  printf("получено имя 1-й ставки = %s\n",Name1.ascii());
				QString Name2 = dbaseCodec->fromUnicode(ExistTaxlistBox->text(pos-1));
				//  printf("получено имя 2-й ставки = %s\n",Name2.ascii());
	    
				char * value = (char *)calloc(201,sizeof(char));
				M.GetTaxRateIdByName(Name1.ascii(),value); 
				if (strcmp(value,"") == 0){free(value);return;}
				QString TaxRate1 = value;
				free(value);
				//   printf("получен tax_rate_id 1 ставки = %s\n",TaxRate1.ascii());
				char * value1 = (char *)calloc(201,sizeof(char));
				M.GetTaxRateIdByName(Name2.ascii(),value1);
				if (strcmp(value1,"") == 0){free(value1);return;}
				QString TaxRate2 =  value1;
				free(value1);
				//   printf("получен tax_rate_id 2 ставки = %s\n",TaxRate2.ascii());
	   
				QString NameOfTaxGroup = dbaseCodec->fromUnicode(ExistTaxGroupcomboBox->currentText()); 
	   
				char * value2 = (char *)calloc(201,sizeof(char));
				M.GetTaxGroupId(NameOfTaxGroup.ascii(),value2);
				if (strcmp(value2,"") == 0){free(value2);return;}
				QString TaxGroupId =  value2;
				free(value2);
				//   printf("получен tax_group_id  ставок = %s\n",TaxGroupId.ascii());
	 
	   
				char * value3 = (char *)calloc(20,sizeof(char));
				M.GetOrderIdByTaxRateIdAndTaxGroupId(TaxRate1.ascii(),TaxGroupId.ascii(),value3);
				if (strcmp(value3,"") == 0){free(value3);return;}
				QString Order1 = value3;
				free(value3);
				//  printf("получен order_id 1 ставки = %s\n",Order1.ascii());
	   
				char * value4 = (char *)calloc(20,sizeof(char));
				M.GetOrderIdByTaxRateIdAndTaxGroupId(TaxRate2.ascii(),TaxGroupId.ascii(),value4);
				if (strcmp(value4,"") == 0){free(value4);return;}
				QString Order2 = value4;
				free(value4);
				//   printf("получен order_id 2 ставки = %s\n",Order2.ascii());
	   
				ReplaceOrder(TaxRate1.ascii(),Order1.ascii(),TaxRate2.ascii(),Order2.ascii()); 
				ChangeTaxGroup();
				ExistTaxlistBox->setCurrentItem(pos-1);
				ExistTaxlistBox->setSelected(pos-1,"TRUE");
		  }
	 }
}


void MainWareform::DownButton_clicked()
{
	 //puts("void MainWareform::DownButton_clicked()");
	 QString mes_str;
	 QString mes_id;
	 int kol = ExistTaxlistBox->numRows();
	 //printf("kol = %d\n",kol);
	 //проверка выделен ли элемент вообще, если нет - первый выделить
	 int pos  = ExistTaxlistBox->currentItem();
	 if(pos == -1)
	 {
		  ExistTaxlistBox->setCurrentItem(0);
		  pos  = 0;
	 }
	 //проверка на есть ли ниже текущего элемента еще один элемент
    
    
	 //printf("pos = %d\n",pos);
	 if (pos == (kol-1))
	 {
		  WareReferenceMessageText(13,mes_str,mes_id);
		  InterfaceMessageWin(mes_str,mes_id);
		  return;
	 }
	 else
	 {
		  QString Name1 = dbaseCodec->fromUnicode(ExistTaxlistBox->text(pos));
		  //printf("получено имя 1-й ставки = %s\n",Name1.ascii());
		  QString Name2 = dbaseCodec->fromUnicode(ExistTaxlistBox->text(pos+1));
		  //printf("получено имя 2-й ставки = %s\n",Name2.ascii());
	
		  char * value = (char *)calloc(201,sizeof(char));
		  M.GetTaxRateIdByName(Name1.ascii(),value);  
		  if (strcmp(value,"") == 0){
				free(value);
				WareReferenceErrorText(57,mes_str,mes_id);
				InterfaceErrorWin(mes_str,mes_id);
				return;
		  }
		  QString TaxRate1 = value;
		  free(value);
		  //printf("получен tax_rate_id 1 ставки = %s\n",TaxRate1.ascii());
		  char * value1 = (char *)calloc(201,sizeof(char));
		  M.GetTaxRateIdByName(Name2.ascii(),value1); 
		  if (strcmp(value1,"") == 0){
				free(value1);
				WareReferenceErrorText(57,mes_str,mes_id);
				InterfaceErrorWin(mes_str,mes_id);
				return;
		  }
		  QString TaxRate2 = value1;
		  free(value1);
		  //printf("получен tax_rate_id 2 ставки = %s\n",TaxRate2.ascii());
	   
		  QString NameOfTaxGroup = dbaseCodec->fromUnicode(ExistTaxGroupcomboBox->currentText()); 
		  char * value2 = (char *)calloc(201,sizeof(char));   
		  M.GetTaxGroupId(NameOfTaxGroup.ascii(),value2);
		  if (strcmp(value2,"") == 0){
				free(value2);
				WareReferenceErrorText(57,mes_str,mes_id);
				InterfaceErrorWin(mes_str,mes_id);
				return;
		  }
		  QString TaxGroupId =  value2;
		  free(value2);
		  //printf("получен tax_group_id  ставок = %s\n",TaxGroupId.ascii());
	 
	
		  char * value3 = (char *)calloc(20,sizeof(char));
		  M.GetOrderIdByTaxRateIdAndTaxGroupId(TaxRate1.ascii(),TaxGroupId.ascii(),value3);
		  if (strcmp(value3,"") == 0){
				free(value3);
				WareReferenceErrorText(58,mes_str,mes_id);
				InterfaceErrorWin(mes_str,mes_id);
				return;
		  }
		  QString Order1 = value3;
		  free(value3);
		  //printf("получен order_id 1 ставки = %s\n",Order1.ascii());
	   
	
		  char * value4 = (char *)calloc(20,sizeof(char));
		  M.GetOrderIdByTaxRateIdAndTaxGroupId(TaxRate2.ascii(),TaxGroupId.ascii(),value4);
		  if (strcmp(value4,"") == 0){
				free(value4);
				WareReferenceErrorText(58,mes_str,mes_id);
				InterfaceErrorWin(mes_str,mes_id);
				return;
		  }
		  QString Order2 = value4;
		  free(value4);
		  //printf("получен order_id 2 ставки = %s\n",Order2.ascii());
	   
		  ReplaceOrder(TaxRate1.ascii(),Order1.ascii(),TaxRate2.ascii(),Order2.ascii()); 
		  ChangeTaxGroup();
		  ExistTaxlistBox->setCurrentItem(pos+1);
	 }
}


//=====================================================================//
//------------------------------------------ШАБЛОН ТОВАРОВ------------------------------------------//
//=====================================================================//



void MainWareform::LoadFlagsValue(void)
{
   // puts("void MainWareform::LoadFlagsValue(void)");
    char * values = (char *)calloc(20,sizeof(char));
    M.Get_Flags_Info(values);
  //  printf("values = %s\n",values);
    if (strcmp(values,"")!=0)
	LoadFlag(values);
    free(values);
}


void MainWareform::SaveFlags_clicked()
{
  // puts("void MainWareform::SaveFlags_clicked()");  
    char * param = (char *) calloc (18,sizeof(char));
    QString flags = transformationflags(param);  
    free(param);
  //  printf("flags = %s\n",flags.ascii());
    int col = M.GetQuantityStrInFlags();
 //   printf("col = %d\n",col);
    if (col>0)
    {
	
	M.Update_Flags_Value(flags.ascii());
    }
    else
    {
	M.Create_Flags_Table();
	M.Create_Flags_Value(flags.ascii());
	messageLabel->setText(systemCodec->toUnicode("<p align=\"center\">Шаблон флагов для товаров сохранен!</p>"));
    }
}

//преобразование для флагов
QString MainWareform::transformationflags(char * param)
{
      //printf("QString MainWareform::transformationflags()\n");  
      strcpy(param,itoa(WeightQuantity_comboBox->currentItem()));
      strcat(param,",");
  //    printf("param = |%s|\n",param);
      strcat(param,itoa(Sale_comboBox->currentItem()));
      strcat(param,",");
   //   printf("param = |%s|\n",param);
      strcat(param,itoa(Return_comboBox->currentItem()));
      strcat(param,",");
  //    printf("param = |%s|\n",param);
      strcat(param,itoa(NegativeReminder_comboBox->currentItem()));
      strcat(param,",");
 //     printf("param = |%s|\n",param);
      strcat(param,itoa(WithoutBringingInQuantity_comboBox->currentItem()));
      strcat(param,",");
   //   printf("param = |%s|\n",param);
      strcat(param,itoa(WritingOffReminder_comboBox->currentItem()));
      strcat(param,",");
  //    printf("param = |%s|\n",param);
      strcat(param,itoa(EditingPrice_comboBox->currentItem()));
      strcat(param,",");
 //     printf("param = |%s|\n",param);
      strcat(param,itoa(BringingInQuantityByHand_comboBox->currentItem()));
      strcat(param,",");
  //    printf("param = |%s|\n",param);
      strcat(param,itoa(PrintInCheck_comboBox->currentItem()));
  //    printf("итого param =|%s|\n",param);
      QString str =SaveFlag(param); 
  //    printf("End transformationflags str = %s\n",str.ascii());
      return str;
}


QString MainWareform::SaveFlag(char * param)
{
  //puts( "void MainWareform::SaveFlags(char * param)\n");
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
      char *   flags = (char *) calloc(strlen(strfl) + 1, sizeof(char));
      strcpy(flags, strfl);
      strcpy(param,flags);
      free(strfl); 
   //   printf("param = %s\n",param);
      free(flags);
      return (QString)param;
  }
//  puts("End SaveFlags  param = NULL");
  return "";
}


void MainWareform::LoadFlag(char * str)
{
   // puts("void MainWareform::LoadFlag(char * str)");
    unsigned long int k =(unsigned long int)(atoi(str));
    int massiv[9];
    for (int i = 0;i<9;i++)
    {
	massiv[i] = k&1;
	k =k>>1;
	//printf("massiv[%d] = %d\n",i,massiv[i]);
    }
    WeightQuantity_comboBox->setCurrentItem(massiv[0]);
    Sale_comboBox->setCurrentItem(massiv[1]);
    Return_comboBox->setCurrentItem(massiv[2]);
    NegativeReminder_comboBox->setCurrentItem(massiv[3]);
    WithoutBringingInQuantity_comboBox->setCurrentItem(massiv[4]);
    WritingOffReminder_comboBox->setCurrentItem(massiv[5]);
    EditingPrice_comboBox->setCurrentItem(massiv[6]);
    BringingInQuantityByHand_comboBox->setCurrentItem(massiv[7]);
    PrintInCheck_comboBox->setCurrentItem(massiv[8]);
}
//---------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------------------//
void MainWareform::FirstStart(void)
{
    //puts("void MainWarefform::FirstStart()");
    int col = M.GetQuantityStrInFlags();
  //  printf("col = %d\n",col);
    if (col<0)
    {
//	puts("Это дикая ошибка у вас нет таблицы Flags в Базе");
	QString er_str;
	QString er_id;
	WareReferenceErrorText(49, er_str, er_id);
	InterfaceErrorWin(er_str, er_id);
	return;
    }
    if (col==0)
    {
	//tabWidget2->setCurrentPage(4);
	//tabWidget2->showPage( tabWidget2->page(3) );
	tabWidget2->setCurrentPage(3);
	/*QWidget * tab_gr = tabWidget2->page(2);
	QWidget * tab_tax = tabWidget2->page(1);
	QWidget * tab_w = tabWidget2->page(0);
	
	
	tabWidget2->removePage(tab_gr);
	tabWidget2->removePage(tab_tax);
	tabWidget2->removePage(tab_w);
	//tabWidget2->removePage(tab_red);
	
	tabWidget2->insertTab( tab_gr, systemCodec->toUnicode("Группы товаров"));
	tabWidget2->insertTab( tab_tax, systemCodec->toUnicode("Налоги"));
	//tabWidget2->insertTab( tab_red, systemCodec->toUnicode("Скидки"));
	tabWidget2->insertTab( tab_w, systemCodec->toUnicode("Товары"));*/
	messageLabel->setText(systemCodec->toUnicode("<p align=\"center\">Отредактируйте и сохраните шаблон флагов для товаров</p>"));
    }
}



//--------------- KeyBoard  begin---------------------------------------------------------------------//


void MainWareform::GetKeySequence()
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


void MainWareform::keyPressEvent(QKeyEvent *k)
{
    //printf("key press code == %d \n", k->key());
    //printf("key text == <%s> \n", k->text().ascii());
    
    int state = 0;
    
    if ( hw->BCS_GetType() == 2 )  {
	state = kbcode_scaner->FindCode(k);
	//printf("state = %d \n", state);
	if ( state == 2 )  {
	    QString barcode_str = ((KeyboardScaner*) kbcode_scaner)->getCode();
	    char* barcode = (char*) calloc(barcode_str.length()+1, sizeof(char));
	    strcpy(barcode, barcode_str);
	    //printf("wtrix kod = <%s> \n", barcode);
	    SetBarCode((QString)barcode);
	    free(barcode);
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


void MainWareform::makeKeyPress(QKeyEvent *k)
{
    if ( k->key() == 4129 || k->key() == 4128 || k->key() == 4131 || k->key() == 4132 || k->key() == 4384 || ( k->key() > 4144 && k->key() < 4155 ) ) return;
    
    QWidget* wid = this->focusData()->focusWidget();  
  //  printf("wid->className() = %s\n",wid->className() );
    
    //puts("void makeKeyPress(QKeyEvent *k)");
    switch(k->key())   
    {
		  
	case Qt::Key_F1:	{
	    //puts("всплывающая подсказка");
	    promptingForm * prompt = new promptingForm;
	    prompt->exec();
	    delete(prompt);
	    break;	}	  
		  
    case Qt::Key_F12 :
	{
	//    puts("Qt::Key_F12\n");
	    close();
	    reject();
	    break;
	}
    case Qt::Key_Enter :
	{
		//		puts("Qt::Key_Enter");
	    if (strcmp(wid->className(),"QTabBar") == 0){
			 EnterHandler();
		}
		 if (AddWareGroup->hasFocus())
		AddWareGroup_clicked();
	    if (DeleteAllWare->hasFocus())
		DeleteAllWare_clicked();
	     if (AddWare->hasFocus())
		AddWare_clicked();
	     if (DeleteWare->hasFocus())
		DeleteWare_clicked();
	     if (ChangeWare->hasFocus())
		ChangeWare_clicked();
	     if (CopyWare->hasFocus())
		CopyWare_clicked();
	    if (ExitpushButton->hasFocus())
		ExitpushButton_clicked();
	     if (SaveFlags->hasFocus())
		 SaveFlags_clicked();
		if (CreateTaxGroup->hasFocus())
			CreateTaxGroup_clicked();
		if (EditTaxGroup->hasFocus())
			EditTaxGroup_clicked();
		if (DeleteTaxGroup->hasFocus())
			 DeleteTaxGroup_clicked();
		if (CreateTax->hasFocus())
			 CreateTax_clicked();
		if (EditTax->hasFocus())
			 EditTax_clicked();
		if (DeleteTax->hasFocus())
			 DeleteTax_clicked();
		if (UpButton->hasFocus())
			 UpButton_clicked();
		if (DownButton->hasFocus())
			 DownButton_clicked();
		if (CreateAutoSchemeGroup->hasFocus())
			 CreateAutoSchemeGroup_clicked();
		if (EditAutoSchemeGroup->hasFocus())
			 EditAutoSchemeGroup_clicked();
		if (DeleteAutoSchemeGroup->hasFocus())
			 DeleteAutoSchemeGroup_clicked();
		if (CreateAutopushButton->hasFocus())
			 CreateAutopushButton_clicked();
		if (EditAutopushButton->hasFocus())
			 EditAutopushButton_clicked();
		if (DeleteAutoSchemepushButton->hasFocus())
			 DeleteAutoSchemepushButton_clicked();
		if (CreateWareGroupButton->hasFocus())
			 CreateWareGroupButton_clicked();
		if (CreateWareGroupInExistGroupButton->hasFocus())
			 CreateWareGroupInExistGroupButton_clicked();
		if (Edit_WareGroup->hasFocus())
			 Edit_WareGroup_clicked();
		if (DeleteWareGroupButton->hasFocus())
			 DeleteWareGroupButton_clicked();
	      if ( strcmp(wid->className(), "QButton" ) == 0 ) 
		((QButton*)wid)->setAccel( Key_Return );
	    break;
	}
    case Qt::Key_Return :
	{
		//    printf("Qt::Key_Return\n"); 
		if (strcmp(wid->className(),"QTabBar") == 0){
			 EnterHandler();
		}
		if (AddWareGroup->hasFocus())
		AddWareGroup_clicked();
	     if (DeleteAllWare->hasFocus())
		DeleteAllWare_clicked();
	     if (AddWare->hasFocus())
		AddWare_clicked();
	    if (DeleteWare->hasFocus())
		DeleteWare_clicked();
	   if (ChangeWare->hasFocus())
		ChangeWare_clicked();
	     if (CopyWare->hasFocus())
		CopyWare_clicked();
	   if (ExitpushButton->hasFocus())
		ExitpushButton_clicked();
	    if (SaveFlags->hasFocus())
		 SaveFlags_clicked();
		if (CreateTaxGroup->hasFocus())
			CreateTaxGroup_clicked();
		if (EditTaxGroup->hasFocus())
			EditTaxGroup_clicked();
		if (DeleteTaxGroup->hasFocus())
			 DeleteTaxGroup_clicked();
		if (CreateTax->hasFocus())
			 CreateTax_clicked();
		if (EditTax->hasFocus())
			 EditTax_clicked();
		if (DeleteTax->hasFocus())
			 DeleteTax_clicked();
		if (UpButton->hasFocus())
			 UpButton_clicked();
		if (DownButton->hasFocus())
			 DownButton_clicked();
		if (CreateAutoSchemeGroup->hasFocus())
			 CreateAutoSchemeGroup_clicked();
		if (EditAutoSchemeGroup->hasFocus())
			 EditAutoSchemeGroup_clicked();
		if (DeleteAutoSchemeGroup->hasFocus())
			 DeleteAutoSchemeGroup_clicked();
		if (CreateAutopushButton->hasFocus())
			 CreateAutopushButton_clicked();
		if (EditAutopushButton->hasFocus())
			 EditAutopushButton_clicked();
		if (DeleteAutoSchemepushButton->hasFocus())
			 DeleteAutoSchemepushButton_clicked();
		if (CreateWareGroupButton->hasFocus())
			 CreateWareGroupButton_clicked();
		if (CreateWareGroupInExistGroupButton->hasFocus())
			 CreateWareGroupInExistGroupButton_clicked();
		if (Edit_WareGroup->hasFocus())
			 Edit_WareGroup_clicked();
		if (DeleteWareGroupButton->hasFocus())
			 DeleteWareGroupButton_clicked();
	     if ( strcmp(wid->className(), "QButton" ) == 0 ) 
		((QButton*)wid)->setAccel( Key_Return );
	    break;
	}
    
    case Qt::Key_Up :
	{
	  //  printf("Qt::Key_Up\n");
	    if (strcmp(wid->className(),"QTabBar") == 0)
			  SetSelectionUp();
	     if ( strcmp(wid->className(), "QComboBox" ) == 0 ){
			int cur_item = ((QComboBox*)wid)->currentItem();
			if (cur_item!=0)
				 ((QComboBox*)wid)->setCurrentItem(cur_item-1);
		}
	    break;
	}
    case Qt::Key_Down :
	{
	    //printf("Qt::Key_Down\n");
	    if (strcmp(wid->className(),"QTabBar") == 0)
			  SetSelectionDown();
	    if ( strcmp(wid->className(), "QComboBox" ) == 0 ){
			int cur_item = ((QComboBox*)wid)->currentItem();
			if (cur_item!=(((QComboBox*)wid)->count()-1))
				((QComboBox*)wid)->setCurrentItem(cur_item+1);
	    } 
	    break; 
	}
	 case Qt::Key_Left :{
				if (strcmp(wid->className(),"QTabBar") == 0){
					 int cur = tabWidget2->currentPageIndex();
					// printf("QTabWidget tabWidget2->currentPageIndex() = %d\n",tabWidget2->currentPageIndex());
					 if (cur == 0){
						tabWidget2->setCurrentPage(tabWidget2->count()-1);  
					 }
					 else{
					     tabWidget2->setCurrentPage(tabWidget2->currentPageIndex()-1);
					 }
				}
				if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {
					 ((QLineEdit*)wid)->cursorBackward( k->state() == Qt::ShiftButton );	    }
				break;}
	 case Qt::Key_Right :{
				if (strcmp(wid->className(),"QTabBar") == 0){
					 int cur = tabWidget2->currentPageIndex();
					// printf("QTabWidget tabWidget2->currentPageIndex() = %d\n",tabWidget2->currentPageIndex());
					 if (cur == tabWidget2->count()-1){
						tabWidget2->setCurrentPage(0);  
					 }
					 else{
					     tabWidget2->setCurrentPage(tabWidget2->currentPageIndex()+1);
					 }
				}
				if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {	
					 ((QLineEdit*)wid)->cursorForward( k->state() == Qt::ShiftButton );	    }
				break;	}
    case Qt::Key_Tab :
	focusData()->next();
	break;
    case Qt::Key_Delete :	
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {	((QLineEdit*)wid)->del();  }
	break;
    case Qt::Key_Home :
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {	
	    ((QLineEdit*)wid)->home( k->state() == Qt::ShiftButton );	    }
	break;
    case Qt::Key_End :
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {
	    ((QLineEdit*)wid)->end( k->state() == Qt::ShiftButton );	    }
	break;
    case Qt::Key_Backspace :	
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 )   {	((QLineEdit*)wid)->backspace();    }
	break;
    default :				
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {    ((QLineEdit*)wid)->insert(k->text());   }  
     }
}
//--------------------------------------------------------- KeyBoard  end---------------------------------------------------------------------//
    
