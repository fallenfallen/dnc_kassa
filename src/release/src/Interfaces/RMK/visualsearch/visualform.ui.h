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


//#include "../MysqlLib/trans_mysql.h"

#include <qfocusdata.h> 

#include "quantinputform.h"

#include "dkbinput.h"
#include "bcskbinput.h"
#include "mcrkbinput.h"

#include "hwsrv.h"
#include "hwsighandlerscontroller.h"
#include "const_conf.h"

//-----(+)-----Zuskin-----03/08/2011-----
#include "priceinputform.h"
//--------------------------------------------------
//#include "receipt.h"

extern Registration Reg;
//extern Receipt * receipt;
extern SP sp;
extern BlockTimer *bt;
//extern KeyboardDevice * kbcode;
extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;
extern HWSRV * hw;

extern DebugLevelLog regformlog;

int cur_group;
int cur_pos;
bool fill_flag;
//-----(+)-----Zuskin-----14/10/2011-----
bool sQttySet = false;
//-------------------------------------------------

//====================================================================//
void visualForm::setBarcode(QString barcode)
{
    barcodeLineEdit->setText(barcode);
    barcodeLineEdit->setFocus();
    barcodeLineEdit->end(false);
    
    if (visualTable->numRows() > 0)  visualTable->setCurrentCell(0,0);
}
//====================================================================//
void visualForm::setWareTitle(QString ware_title)
{
    //printf("name = %s \n", ware_title.ascii());
    if ( ware_title.isEmpty() ) return;
    titleLineEdit->setText(ware_title);
    titleLineEdit->setFocus();
    titleLineEdit->end(false);
    
    if (visualTable->numRows() > 0) visualTable->setCurrentCell(0,0);
}
//====================================================================//
void visualForm::setCode(QString code)
{
    codeLineEdit->setText(code);
    
    codeLineEdit->setFocus();
    codeLineEdit->end(false);
     
    if (visualTable->numRows() > 0) visualTable->setCurrentCell(0,0);   
}
//====================================================================//
void visualForm::SearchByAll()
{
    messageTextLabel->setText(DefCodec->toUnicode(""));
    
    //-----(*)-----Zuskin-----03/08/2011-----
  //  if ( barcodeLineEdit->text().isEmpty() && codeLineEdit->text().isEmpty() && titleLineEdit->text().isEmpty() ) {
    if ( barcodeLineEdit->text().isEmpty() && codeLineEdit->text().isEmpty() && titleLineEdit->text().isEmpty() && price_filter == -1)     {
    //--------------------------------------------------	  
	ClearVisualTable();
	FillTable("0");
	return;
    }
    
    char *barcode_pattern = (char*) calloc(DbCodec->fromUnicode(barcodeLineEdit->text()).length() + 1, sizeof(char) );
    char *code_pattern = (char*) calloc(DbCodec->fromUnicode(codeLineEdit->text()).length()*2 + 1, sizeof(char) );
    char *title_pattern = (char*) calloc(DbCodec->fromUnicode(titleLineEdit->text()).length()*2 + 1, sizeof(char) );
     
    strcpy(barcode_pattern, DbCodec->fromUnicode(barcodeLineEdit->text()));
    strcpy(code_pattern, DbCodec->fromUnicode(codeLineEdit->text()));
    strcpy(title_pattern, DbCodec->fromUnicode(titleLineEdit->text()));
    //-----(*)-----Zuskin-----03/08/2011-----
//    int res = Reg.VisualSearchByAll( barcode_pattern, code_pattern, title_pattern, !sp.show_absent_goods && check_type != 1 );     
    int res = Reg.VisualSearchByAll( barcode_pattern, code_pattern, title_pattern, !sp.show_absent_goods && check_type != 1, price_filter );     
    //-------------------------------------------------
    free(barcode_pattern);
    free(code_pattern);
    free(title_pattern);
    
    //printf("res = %d \n", res);
    
    if ( res < 0 ) {
	messageTextLabel->setText(DefCodec->toUnicode("<p align=\"center\">Ошибка при запросе к БД</p>"));
	return;
    }
    ClearVisualTable();
    if ( res == 0 ) {
	messageTextLabel->setText(DefCodec->toUnicode("<p align=\"center\">Нет подходящих вариантов!!!</p>"));
	return;
    }
    
    if ( res > sp.max_number_search_result )     {
	messageTextLabel->setText(DefCodec->toUnicode("<p align=\"center\">Слишком много вариантов!!!</p>"));
	return;
    }
    
    FillProd(res);
    if (visualTable->numRows() > 0) visualTable->setCurrentCell(0, 0);
    
}
//====================================================================//
void visualForm::init()
{
    //puts("visualSearch -----  init");
    // //puts("visualForm::init()");
    DefCodec = QTextCodec::codecForName(SYSCODEC);
    DbCodec = QTextCodec::codecForName(DBCODEC);
    //printf("DbCodec = %s\n",DBCODEC);
    //printf("DefCodec = %s\n",SYSCODEC);
    sernumber = 0;
    cur_group = -1;
    cur_pos = 0;
    fill_flag = true;
    q = 1;
    
    check_type = 0;
    //-----(*)-----Zuskin-----03/08/2011-----
    price_filter = -1;
    //-------------------------------------------------
    //-----(+)-----Zuskin-----14/10/2011-----
    vsQttyPreSet = false;
    //-------------------------------------------------
    eraddware = 0;
    
    //puts("init table ----- ");
    InitTable();
    //puts("init table ---- ok");
    
    connect(titleLineEdit, SIGNAL(textChanged(const QString &)), SLOT(SearchByAll()));
    
    connect(barcodeLineEdit, SIGNAL(textChanged(const QString &)), SLOT(SearchByAll()));
    connect(codeLineEdit, SIGNAL(textChanged(const QString &)), SLOT(SearchByAll()));
    
    if ( !sp.block_mouse )  {
	if ( sp.one_click_selection ) connect(visualTable, SIGNAL(clicked( int , int , int , const QPoint &)), SLOT(EnterHandler()));
	connect(visualTable, SIGNAL(doubleClicked( int , int , int , const QPoint &)), SLOT(EnterHandler()));
	connect(visualTable, SIGNAL(clicked( int , int , int , const QPoint &)), SLOT(ClearMessage()));
    }
    
    //if ( !sp.close_after_one_registration )  connect(AddWareController::GetInstance(), SIGNAL(Error(int )), SLOT(AddWareError(int)));
    connect(AddWareController::GetInstance(), SIGNAL(AddWareError(int )), SLOT(AddWareError(int)));
    
    connect(HwSignalHandlersController::GetInstance(), SIGNAL(BarCode(const char *)), SLOT(SetBarCodePattern( const char *)));
    
    //puts("connects -- ok");
    
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence()) );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    
    //printf("SetValidatorProperties \n");
    SetValidatorProperties();
    //printf("SetValidatorProperties --- ok");
    
    if(sp.block_mouse) grabMouse();
    if(sp.autoblock_flag) bt->action_time = QTime::currentTime();
    adjustSize();
    
    HwSignalHandlersController::GetInstance()->SetControlledObject(VISUALSEARCH);
    
    
    
    //-------------------------------------------------------------------------//  
    FILE * tmp = fopen(RMK_CONF_PATH, "r");
    if ( tmp != NULL ) {
	char * str = (char*) calloc(3, sizeof(char));
	fgets(str,2, tmp);
	// b - barcode
	// c - code
	// n - name
	if ( *(str) == 'b' ) barcodeLineEdit->setFocus();
	if ( *(str) == 'c' ) codeLineEdit->setFocus();
	if ( *(str) == 'n' ) titleLineEdit->setFocus();
	free(str);
	fclose(tmp);
    }
    //-------------------------------------------------------------------------//
    
    //releaseKeyboard();
    grabKeyboard();
}

//====================================================================//
void visualForm::showEvent ( QShowEvent * ) 
{
    //puts("void visualForm::showEvent ( QShowEvent * ) ");
    barcodeLineEdit->end(false);
    codeLineEdit->end(false);    
    titleLineEdit->end(false);   
}
//====================================================================//
void visualForm::destroy()
{
    //puts("visualForm::destroy()");
  if(sp.block_mouse) releaseMouse();
  HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
  //grabKeyboard();
}

//===============================================================//
void visualForm::SetBarCodePattern( const char * barcode )
{
    barcodeLineEdit->setText(barcode);
    //editline = barcodeLineEdit;
    //Search(2);
    //GetSearchParameter();
    //CheckValue();
}

//===============================================================//
void visualForm::ClearMessage()
{
////puts("void visualForm::ClearMessage()");
  messageTextLabel->setText("");
}

//===============================================================//
void visualForm::SetValidatorProperties()
{
   // //puts("visualForm::SetValidatorProperties()");
    QRegExp coderx(DefCodec->toUnicode("[0-9A-Za-zА-Яа-я]{0,20}"));
    codevalidator = new QRegExpValidator(coderx, this);
    codeLineEdit->setValidator(codevalidator);
    
    QRegExp barrx("([a-zA-Z]|\\d){0,13}");
    barcodevalidator = new QRegExpValidator(barrx, this);
    barcodeLineEdit->setValidator(barcodevalidator);
    
    QRegExp titlerx("(\\d|\\D){0,20}");
    titlevalidator = new QRegExpValidator(titlerx, this);
    titleLineEdit->setValidator(titlevalidator);
}

//====================================================================//
void visualForm::InitTable()
{
    ////puts("visualForm::InitTable()"); 
    
    visualTable->setColumnReadOnly(0, true);
    visualTable->setColumnReadOnly(1, true);
    visualTable->setColumnReadOnly(2, true);
    visualTable->setColumnReadOnly(3, true);
    visualTable->setColumnReadOnly(4, true);
    visualTable->setColumnReadOnly(5, true);
    visualTable->setColumnReadOnly(6, true);
    
    visualTable->setColumnWidth(0, 15);
    visualTable->setColumnWidth(1, 100);
    visualTable->setColumnWidth(2, 305); 
    visualTable->setColumnWidth(3, 110); 
    visualTable->setColumnWidth(4, 100);
    visualTable->setColumnWidth(5, 100); 
    visualTable->setSelectionMode(QTable::SingleRow);
    visualTable->hideColumn(6);
    visualTable->hideColumn(7);	
    visualTable->hideColumn(8);	
}

//====================================================================//
void visualForm::AddPixmapInTable(bool pixflag, int i)
{
////puts("visualForm::AddPixmapInTable(bool pixflag, int i)");
  QPixmap pix;
  if(pixflag) 
  {
    //pix.load("./images/folder-open.png");
    pix = QPixmap::fromMimeSource("folder-open.png");
    sernumber = pix.serialNumber();
  }
  else 
  {
    //  pix.load("./images/folder-closed.png");
      pix = QPixmap::fromMimeSource("folder-closed.png");
  }
  QTableItem *i0 = new QTableItem(visualTable, QTableItem::Never, "", pix);  
  visualTable->setItem(i, 0, i0);
}

//====================================================================//
void visualForm::AddRowInTable(const char *name, const char *price, const char *quant, const char * code, const char *barcode, const char *group, const char * pgroup, bool grflag, bool pixflag)	{

    /*cfl = 0  - bar code, cfl = 1 - internal code*/
////puts("visualForm::AddRowInTable(const char *name, const char *price, const char *quant, const char * code, const char *barcode, const char *group, const char * pgroup, bool grflag, bool pixflag)");
    
  QString s;
  int i = visualTable->numRows();  
  visualTable->setNumRows(i + 1);
    
  if(grflag) AddPixmapInTable(pixflag, i);
    
  visualTable->setText(i, 1, DbCodec->toUnicode(code)); 
  visualTable->setText(i, 2, DbCodec->toUnicode(name));
  visualTable->setText(i, 3, DbCodec->toUnicode(barcode));
  visualTable->setText(i, 4, DbCodec->toUnicode(price));
  visualTable->setText(i, 5, DbCodec->toUnicode(quant));
  visualTable->setText(i, 6, DbCodec->toUnicode(group));
  visualTable->setText(i, 7, DbCodec->toUnicode(pgroup));

  if(cur_group == atoi(group))
    cur_pos = i;
}

//====================================================================//
void visualForm::SetSelectionUp()
{    
////puts("visualForm::SetSelectionUp()");

  QTableSelection sel = visualTable -> selection(0);
  int p_sel = sel.topRow();
    
  if((p_sel != 0) && (p_sel != -1))
  {
    visualTable->clearSelection();
    sel.init(--p_sel, 0);
    sel.expandTo(p_sel, 5);
    visualTable->addSelection(sel);
    visualTable -> ensureCellVisible(p_sel,0); 
  }
}

//====================================================================//
void visualForm::SetSelectionDown()
{
////puts("visualForm::SetSelectionDown()");
    
  QTableSelection sel = visualTable -> selection(0);
  int p_sel = sel.topRow();
    
  if((p_sel != visualTable->numRows() - 1) && (p_sel != -1))
  {
    visualTable->clearSelection();
    sel.init(++p_sel, 0);
    sel.expandTo(p_sel, 5);
    visualTable->addSelection(sel);
    visualTable -> ensureCellVisible(p_sel,0); 
  } 
}

//====================================================================//
void visualForm::ClearVisualTable()
{
////puts("visualForm::ClearVisualTable()");
  visualTable->setNumRows(0); //в памяти тоже чистит
}

//====================================================================//
void visualForm::FillGroup(int n, const char *pgroup)
{
////puts("visualForm::FillGroup(int n, const char *pgroup)");
    //printf("FillGroup (n = %d) \n", n);
  char *name, *group_id;
  name = (char *) calloc(201, sizeof(char));
  group_id = (char*) calloc(12, sizeof(char));
  visualTable->hide();
  for(int i = 0; i < n; i++)
  {
    Reg.Get_Group_Info(name, group_id, i, n);
    AddRowInTable(name, "", "", group_id, "", group_id, pgroup, true, false);
  }
  visualTable->show();
  free(name);
  free(group_id);
}

//====================================================================//
void visualForm::FillProd(int n)
{
    //printf("-------FillProd(n - %d) \n", n);
    
    VisualSearchElement product;
    
    visualTable->hide();
    for (int i = 0; i < n; i++) {
	//printf("product i = %d \n", i);
	product = Reg.GetProductInfo(i, n);
	AddRowInTable(product.longtitle, product.price, product.quantity, product.internal_code, product.barcode, "","", false, false);		
	//printf("addition_id = %s \n", product.addition_id);
	if ( product.is_addition ) {
	    //printf("-----fignya------\n");
	    //printf("visualTable->numRows() = %d \n", visualTable->numRows());
	    //printf("visualTable->numCols() = %d \n", visualTable->numCols());
	    visualTable->setText(visualTable->numRows() - 1, 0, DefCodec->toUnicode("⚑")); 
	    visualTable->setText(visualTable->numRows() - 1, 8, product.addition_id);     
	    
	    //QPixmap pix = QPixmap::fromMimeSource("attach.png");
	    //QTableItem *i0 = new QTableItem(visualTable, QTableItem::Never, "", pix);  
	    //visualTable->setItem(visualTable->numRows() - 1, 0, i0);
	}
    }
    visualTable->show();    
}

//====================================================================//
void visualForm::FillTable(const char *group_id)
{
    int n;
    
    // =================SUBGROUPS==================
    n = Reg.Get_Num_Info(group_id, true);
    if ( (n > 0) && (n <= sp.max_number_search_result) )     { 
	if ( Reg.Get_List_By_Parent(group_id, true) > 0 )
	    FillGroup(n, group_id);
    }   else    {
	if ( n > sp.max_number_search_result )
	    messageTextLabel->setText(DefCodec->toUnicode("<p align=\"center\">Слишком много подгрупп...</p>"));
	else
	    messageTextLabel->setText(DefCodec->toUnicode("<p align=\"center\">В группе нет подгрупп...</p>")); 
    }
    // ==================GOODS====================
    if ( sp.show_absent_goods || check_type == 1 )  n = Reg.Get_Num_Info(group_id, false);
    else n = Reg.Get_Num_Info_Available(group_id, false);
   
    if ( (n > 0) && (n <= sp.max_number_search_result) )     {  
	int res = 0;
	if ( sp.show_absent_goods || check_type == 1 ) 
	    res = Reg.Get_List_By_Parent(group_id, false);
	else 
	    res = Reg.Get_List_By_Parent_Available(group_id, false);
	if ( res > 0 )     
	    FillProd(n); 
    }   else     {
	if (n > sp.max_number_search_result)
	    messageTextLabel->setText(DefCodec->toUnicode("<p align=\"center\">Слишком много товаров...</p>"));
	else
	    messageTextLabel->setText(DefCodec->toUnicode("<p align=\"center\">В группе нет товаров...</p>")); 
    }
    QTableSelection sel = visualTable -> selection(0);
    visualTable->clearSelection();
    sel.init(cur_pos, 0);
    sel.expandTo(cur_pos, 5);	
    visualTable->addSelection(sel);
    visualTable -> ensureCellVisible(cur_pos, 0);
}

//====================================================================//
void visualForm::InFolder()
{
////puts("visualForm::InFolder()");
  bool f;
    
  QTableSelection sel = visualTable -> selection(0);
  int p_sel = sel.topRow();
    
  int gr = visualTable->text(p_sel, 6).toInt(&f);
  QTableItem * group = visualTable->item(p_sel, 6);
  QTableItem * pgroup = visualTable->item(p_sel, 7);
  QTableItem * name = visualTable->item(p_sel, 2);
  visualTable->takeItem(group);
  visualTable->takeItem(pgroup);
  visualTable->takeItem(name);
  ClearVisualTable();
  AddRowInTable("", "", "", "", "", "", "", true, true);
  visualTable->setItem(0, 2, name);
  visualTable->setItem(0, 6, group);
  visualTable->setItem(0, 7, pgroup);
  QString gr_qstr = QString::number(gr);
  char * gr_str = (char *) calloc (gr_qstr.length()+1, sizeof(char));
  strcpy(gr_str, gr_qstr);
  cur_pos = 0;
  FillTable(gr_str);    
  free(gr_str);
}

//====================================================================//
void visualForm::OutFolder()
{
////puts("void visualForm::OutFolder()");
  bool f;
    
  QTableSelection sel = visualTable -> selection(0);
  int p_sel = sel.topRow();
    
  int gr = visualTable->text(p_sel, 7).toInt(&f);
  cur_group = visualTable->text(p_sel, 6).toInt(&f);
  //printf("Group:%d\n",gr);
  ClearVisualTable();
  QString gr_qstr = QString::number(gr);
  char * group = (char *) calloc (gr_qstr.length() + 1, sizeof(char));
  strcpy(group, gr_qstr);
  if(gr != 0)
  {
    char * name = (char *) calloc(201, sizeof(char));
    char * pgroup = (char*) calloc(12, sizeof(char));
    Reg.Get_Parent_Info(group, name, pgroup);
    AddRowInTable(name, "", "", "", "", group, pgroup, true, true);
    free(name);
    free(pgroup);
  }
  FillTable(group);
  free(group);
}

//====================================================================//
void visualForm::ChangeFolder()
{
////puts("void visualForm::ChangeFolder()");
  QTableSelection sel = visualTable -> selection(0);
  int p_sel = sel.topRow();

  if(visualTable->pixmap(p_sel, 0).serialNumber() == sernumber)
    OutFolder();
  else
    InFolder();
}

//====================================================================//
int visualForm::RequestQuantity(const char * code)
{
    ////puts("int visualForm::GetQuantity()");
    //AddWareController::GetInstance()->GetQuantity()
    //-----(+)-----Zuskin-----14/10/2011-----
    sQttySet = false;
    if (vsQttyPreSet) {
	vsQttyPreSet = false;
	sQttySet = true;
    }
    //-------------------------------------------------
    if ( sp.query_quantity )  {	
	
	// проверка на разрешение ввода количества вручную у конкретного товара
	int k = Reg.GetQuantityEditFlag(code);
	//printf("GetQuantityEditFlag(code = %s)  = %d \n", code, k);
	
	 // ошибка  бд
	if (k == -1)  {    Error(4); 	return -1;	}
	// для данного товара запрещено редактирование количества
	if ( k == 1 ) {  Error(5);    return -1;   }
	
	quantinputForm *qi = new quantinputForm;
	
	// можно задать не более остатка
	qi->SetValidatorProperties(code); 
	
	releaseKeyboard();
	int res = qi->exec();
	grabKeyboard();
	
	if ( res == QDialog::Rejected )  res = -1; 
	else {	    
	    res = 0;
	    k = Reg.GetQuantityEditInfo(code, AddWareController::GetInstance()->GetQuantity());
	    if ( k < 0 ) {   Error(4);    return  -1;	}
	    if ( k == 1 )  { Error(2);   return -1;   }
	    //-----(+)-----Zuskin-----14/10/2011-----
	    sQttySet = true;
	    //-------------------------------------------------
	}
	
	//q = qi->GetValue();
	delete(qi);
	
	return res;
    }
    return 0;
}

//====================================================================//
void visualForm::RegistrationGood(QString product_code)
{   
    
    //printf("eraddware  == %d \n", eraddware);
    product_code = DbCodec->fromUnicode(product_code);
    
    //printf("before RequestQuantity ----\n");
    if ( RequestQuantity(product_code.ascii()) == 0 )  {
	
	/*int k =  Reg.GetQuantityEditFlag(code_string.ascii());
	if ( k == -1  ) {   
	    Error(4); 
	    return;  
	}*/
	
	//if ( (k == 1) && (AddWareController::GetInstance()->GetQuantity() != 1) )  { Error(5);  return;  }	
	QTableSelection sel = visualTable -> selection(0);
	int p_sel = sel.topRow();
	double barcode_quantity = 0;
	
	if ( p_sel != -1 ) {
	    if (!visualTable->text(p_sel, 3).isEmpty() ) { 
		//printf("visualTable->text(p_sel, 3) = %s \n", (visualTable->text(p_sel, 3)).ascii());
		QString ware_internal_code = DbCodec->fromUnicode(visualTable->text(p_sel, 1));
		Reg.GetBarcodeCoefficient(visualTable->text(p_sel, 3).ascii(), ware_internal_code.ascii(), barcode_quantity);
		///printf("barcode_quantity = %d \n", barcode_quantity);
		if ( (barcode_quantity) != 1 &&  (barcode_quantity) > 0 )   {
		    AddWareController::GetInstance()->SetQuantity(barcode_quantity * AddWareController::GetInstance()->GetQuantity(), -1);
		}   
		AddWareController::GetInstance()->ware_barcode = visualTable->text(p_sel, 3);
	    }
	}
	
	//printf("visualTable->numCols() = %d \n", visualTable->numCols());
	if ( !visualTable->text(p_sel,8).isEmpty() ) {
	    printf("Registration from VS by addition_code - %s \n", visualTable->text(p_sel,8).ascii());
	    //-----(*)-----Zuskin-----14/10/2011-----
	    //AddWareController::GetInstance()->AddWare(product_code.ascii(), false, 2, visualTable->text(p_sel,8).ascii());
	    AddWareController::GetInstance()->AddWare(product_code.ascii(), false, 2, visualTable->text(p_sel,8).ascii(),false,sQttySet);
	    //-------------------------------------------------
	} else {
	    printf("Registration from VS by internal code - %s \n", product_code.ascii());
	    //-----(*)-----Zuskin-----03/08/2011-----
	    //AddWareController::GetInstance()->AddWare(product_code.ascii(), false, 2, "");
	    if ( price_filter == -1 ) {
		//-----(*)-----Zuskin-----14/10/2011-----
		//AddWareController::GetInstance()->AddWare(product_code.ascii(), false, 2, "");
		AddWareController::GetInstance()->AddWare(product_code.ascii(), false, 2, "", false, sQttySet);
		//-------------------------------------------------
	    }
	    else {
		//-----(*)-----Zuskin-----14/10/2011-----		
		//AddWareController::GetInstance()->AddWare(product_code.ascii(), false, 2, "", true);	
		AddWareController::GetInstance()->AddWare(product_code.ascii(), false, 2, "", true, sQttySet);	
		//-------------------------------------------------
	    }
	    //-------------------------------------------------
	}	
	
	eraddware = AddWareController::GetInstance()->GetResultAddWare();
	
	if ( eraddware != 0 ) AddWareController::GetInstance()->SetQuantity(1.0, -1);
	if ( eraddware == 0 ) messageTextLabel->setText(DefCodec->toUnicode("<p align=\"center\">Товар добавлен</p>"));
	
	if ( sp.close_after_one_registration && (eraddware == 0) )  {  CloseForm();      accept();   }
	
	eraddware = 0;
    }
    
    
}
//====================================================================//
void visualForm::AddWareError(int er)
{
    printf("visualForm::AddWareError(int er = %d) \n", er);
    eraddware = er;
    
    QString er_id, er_str;
    RegistrationErrorText(er, er_str, er_id);
    
    
    releaseKeyboard();
    InterfaceErrorWin(er_str, er_id);
    grabKeyboard();

}

//====================================================================//

void visualForm::EnterHandler()
{ 
    
    QTableSelection sel = visualTable -> selection(0);
    int p_sel = sel.topRow();
    //printf("visualTable->currentRow() = %d \n", p_sel);
    
    if ( p_sel == -1 ) return;
   
    if ( visualTable->text(p_sel, 4).isEmpty() ) ChangeFolder();
    else 	RegistrationGood(visualTable->text(p_sel, 1));
    
} 

//====================================================================//
void visualForm::CloseForm()
{
 if(sp.block_mouse) releaseMouse();

    FILE * tmp = fopen(RMK_CONF_PATH, "w");
    if ( tmp != NULL ) {
	if ( barcodeLineEdit == focusWidget() ) fwrite("b", sizeof(char), 1, tmp);
	if ( codeLineEdit == focusWidget() ) fwrite("c", sizeof(char), 1, tmp);
	if ( titleLineEdit == focusWidget() ) fwrite("t", sizeof(char), 1, tmp);
	
	fclose(tmp);
    } 

}

//======================================================================//
void visualForm::GetKeySequence()
{
    //puts("-----------------GetKeySequence()---------------------");
    if ( kbcode_scaner != NULL ) {
	int n = kbcode_scaner->GetCountEvent();
	//printf("n = %d \n", n);
	for ( int i = 0; i < n; i++ )   {
	    emit makeKeyPress( kbcode_scaner->GetEvent(i) );	}
	kbcode_scaner->Clear();
    }
    
    if ( kbcode_reader != NULL ) {
	int n = kbcode_reader->GetCountEvent();
	for ( int i = 0; i < n; i++ )   {
	    emit makeKeyPress( kbcode_reader->GetEvent(i) );	}
	kbcode_reader->Clear();
    }
}
//======================================================================//
void visualForm::makeKeyPress(QKeyEvent * k) 
{
    
    //puts("makeKeyPress");
    
    if(!sp.close_after_one_registration) ClearMessage();
    
    QWidget* wid = this->focusData()->focusWidget(); 
    //printf("wid class name = %s \n", wid->className());
    
    
    
    switch(k->key())    {
    case Qt::Key_F2 :      {       codeLineEdit->setFocus();        break;      }
    case Qt::Key_F3 :      {       barcodeLineEdit->setFocus();        break;      }
    case Qt::Key_F6 :      {       titleLineEdit->setFocus() ;        break;      }
    //-----(+)-----Zuskin-----03/08/2011-----
    case Qt::Key_F4 :      {       RequestPrice() ;        break;      }	
    //-------------------------------------------------	
    case Qt::Key_Enter :      {        EnterHandler();        break;      }
    case Qt::Key_Return :      {        EnterHandler();        break;      }
    case Qt::Key_F12 :      {        
	    CloseForm(); 
	    reject();
	}
    case Qt::Key_Up :      {        SetSelectionUp();        break;            }
    case Qt::Key_Down :      {        SetSelectionDown();        break;            }	
    case Qt::Key_Escape :      {        Clear();        break;            }
    case Qt::Key_Left :  {
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {
	    ((QLineEdit*)wid)->cursorBackward( k->state() == Qt::ShiftButton );	    }
	break;	}
    case Qt::Key_Right : {
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {	
	    ((QLineEdit*)wid)->cursorForward( k->state() == Qt::ShiftButton );	    }
	break;	}
    case Qt::Key_Delete :  {
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {	((QLineEdit*)wid)->del();  }
	break;    }
    case Qt::Key_Home :  {
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {	
	    ((QLineEdit*)wid)->home( k->state() == Qt::ShiftButton );	    }
	break;	}
    case Qt::Key_End : {
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {
	    ((QLineEdit*)wid)->end( k->state() == Qt::ShiftButton );	    }
	break; 
    }
    case Qt::Key_Backspace :	{
	    //puts("Qt::Key_Backspace");
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 )   {	((QLineEdit*)wid)->backspace();    }
	break;
    }
    default :      {  	   
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {    ((QLineEdit*)wid)->insert(k->text());   }
    }
    }
    return;
    
}
//====================================================================//
void visualForm::Clear()
{
    barcodeLineEdit->setText("");
    codeLineEdit->setText("");
    titleLineEdit->setText("");
    //-----(+)-----Zuskin-----03/08/2011-----
    setPrice(-1);
    //-------------------------------------------------
}
//====================================================================//
void visualForm::keyPressEvent(QKeyEvent *k)
{
    
    if ( ! isActiveWindow()  ) return;
    
    if(sp.autoblock_flag) bt->action_time = QTime::currentTime();
    
    int state = 0;   
        
    if ( hw->BCS_GetType() == 2 )  {
	state = kbcode_scaner->FindCode(k);
	//printf("state = %d \n", state);
	int n = kbcode_scaner->GetCountEvent();
	//printf("n = %d \n", n);
	if ( state == 2 )  {
	    QString barcode_str = ((KeyboardScaner*) kbcode_scaner)->getCode();
	    char* barcode = (char*) calloc(barcode_str.length()+1, sizeof(char));
	    strcpy(barcode, barcode_str);
	    SetBarCodePattern(barcode);
	    free(barcode);
	}
    }
    
    
    if ( hw->MCR_GetType() == 2 && state == 0 ) {
	state = kbcode_reader->FindCode(k);
    }
  
    if ( state == 0 )  { this->makeKeyPress(k);	return;   }
  
    if (state == 5 || state == 6)    {  GetKeySequence();  return;    }
  
    //puts("end key press");
    
}	
//====================================================================//
void visualForm::Error(int er)
{
  printf("Error(%d) \n", er);
  QString er_str;
  QString er_id;
  VisualsearchErrorText(er, er_str, er_id);
  
  releaseKeyboard();
  InterfaceErrorWin(er_str, er_id);
  grabKeyboard();
  
}
//====================================================================//

//-----(+)-----Zuskin-----03/08/2011-----
void visualForm::setPrice( double price )
{
    price_filter = price;
    if (price != -1) {
	priceLineEdit->setText(QString::number(price, 'f' , 2)); 
    }
    else {
	priceLineEdit->setText(""); 
    }
    SearchByAll();
}

void visualForm::RequestPrice() {
    priceinputform *pi = new priceinputform;
    pi->SetValidatorProperties(); 
    releaseKeyboard();
    int res = pi->exec();
    grabKeyboard();

    if ( res == QDialog::Rejected ) {
	setPrice(-1);
    }
    else {	    
	setPrice(pi->price);
    }
    delete(pi);

}
//--------------------------------------------------
