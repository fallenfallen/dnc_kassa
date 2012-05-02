/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savrasov Alexandr aka Zuskin.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Саврасов Александр aka Zuskin.
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
#include <qapplication.h>
#include "dkbinput.h"
#include "bcskbinput.h"
#include "mcrkbinput.h"

#include "hwsrv.h"
#include "hwsighandlerscontroller.h"
#include "const_conf.h"
#include "receipt.h"

//extern Registration Reg;
extern Receipt * receipt;
extern SP sp;
extern BlockTimer *bt;
//extern KeyboardDevice * kbcode;
extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;
extern HWSRV * hw;

void EDSForm::init()
{
    n_pc = 1; 
    DefCodec = QTextCodec::codecForName(SYSCODEC);
  //  DbCodec = QTextCodec::codecForName(DBCODEC);
    
 /*   if ( !sp.block_mouse )  {
	if ( sp.one_click_selection ) connect(visualTable, SIGNAL(clicked( int , int , int , const QPoint &)), SLOT(EnterHandler()));
	connect(visualTable, SIGNAL(doubleClicked( int , int , int , const QPoint &)), SLOT(EnterHandler()));
	connect(visualTable, SIGNAL(clicked( int , int , int , const QPoint &)), SLOT(ClearMessage()));
    }*/
            
    //connect(HwSignalHandlersController::GetInstance(), SIGNAL(BarCode(const char *)), SLOT(SetBarCodePattern( const char *)));
    clientsTable->setColumnWidth(0, 470);
    clientsTable->setNumCols(4);
    clientsTable->hideColumn(2);
    clientsTable->hideColumn(3);
    
    connect(HwSignalHandlersController::GetInstance(), SIGNAL(BarCode(const char *)), SLOT(BarcodeSignalHandler( const char *)));
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence()) );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    
    statusLabel->setText("");
    
    if(sp.block_mouse) grabMouse();
    if(sp.autoblock_flag) bt->action_time = QTime::currentTime();

    HwSignalHandlersController::GetInstance()->SetControlledObject(VISUALSEARCH);
     grabKeyboard();
}

void EDSForm::destroy()
{
  if(sp.block_mouse) releaseMouse();
  HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
  //releaseKeyboard();
}

void EDSForm::makeKeyPress(QKeyEvent * k) 
{
    //QWidget* wid = this->focusData()->focusWidget(); 
    updateStatus("");    
    switch(k->key())    {
   
    case Qt::Key_Enter :      {
	    EnterHandler();        
	    break;      }
    case Qt::Key_Return :      {        EnterHandler(); 
	    break;      }
    case Qt::Key_F12 :      {        
	    reject();
	}
    case Qt::Key_E :	{
	    if((k->state() & Qt::ControlButton))  { RequestList(); }
	    break;	}
    case Qt::Key_Up :      {        SetSelectionUp();        break;            }
    case Qt::Key_Down :      {        SetSelectionDown();        break;            }	

//    case Qt::Key_Escape :      {        Clear();        break;            }
/*    case Qt::Key_Left :  {
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
    } */
    default :      { 
	
//	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {    ((QLineEdit*)wid)->insert(k->text());   }
    }
    }
    return;
    
}


void EDSForm::RequestList()
{  
    puts("====================REQUEST=================");
    updateStatus(DefCodec->toUnicode("Пробуем связаться с сервером скидок..."));
    int res = hw->EDS_GetRequestsList(n_pc);
    if (res <0) {
	   string strtxt = string("Ошибка: ") + string(hw->EDS_GetErrDescription(res));
	   QString txt = DefCodec->toUnicode(strtxt.c_str());
	   updateStatus(txt);
    }
    else {  ClearClientsTable();
	QString txt = DefCodec->toUnicode("Получено запросов на скидку: ");
	txt+= QString::number(res).ascii();
	updateStatus(txt);
	for (int tj = 0; tj < res; tj++) {
	    int tres = hw->EDS_GetRequest(tj);
	    if (tres == 0) {
		string ClName = hw->EDS_GetClientName();
		string ShCut = "DscMobile";
		double Dsc = hw->EDS_GetDiscount();
		string DscId = hw->EDS_GetDiscountId();
		string UserId = hw->EDS_GetUserId();
	    
		AddRowInTable(QString(ClName.c_str()), QString::number(Dsc), QString(DscId.c_str()), QString(UserId.c_str()));
	    }
	}
    }
}

void EDSForm::keyPressEvent(QKeyEvent *k)
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
	    BarcodeEvent(barcode_str);
	}
    }
    
    
    if ( hw->MCR_GetType() == 2 && state == 0 ) {
	state = kbcode_reader->FindCode(k);
    }
  
    if ( state == 0 )  { this->makeKeyPress(k);	return;   }
  
    if (state == 5 || state == 6)    {  GetKeySequence();  return;    }
  
    //puts("end key press");
    
}

void EDSForm::GetKeySequence()
{
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


void EDSForm::BarcodeEvent( QString barcode_str )
{
    int tres = 0;
    if ( hw->EDS_GetType() == 1) {
	updateStatus(DefCodec->toUnicode("Пробуем связаться с сервером скидок..."));
           	tres = hw->EDS_GetClientByBarcode(n_pc, barcode_str.ascii());
	updateStatus("");	
	printf("in RMK (edsform) result of EDS_GetClientByBarcode = [%d]\n", tres);
	if (tres == 0) {
	    string ClName = hw->EDS_GetClientName();
	    string ShCut = "DscMobile";
	    double Dsc = hw->EDS_GetDiscount();
	    string DscId = hw->EDS_GetDiscountId();
	    string UserId = hw->EDS_GetUserId();
	    
	    AddRowInTable(QString(ClName.c_str()), QString::number(Dsc), QString(DscId.c_str()), QString(UserId.c_str()));
	    return;    
	}
	else {
	   string strtxt = string("Ошибка: ") + string(hw->EDS_GetErrDescription(tres));
	   QString txt = DefCodec->toUnicode(strtxt.c_str());
	   updateStatus(txt);
	}
    }
}

void EDSForm::updateStatus( QString str )
{
//   printf("status = %s\n", str.ascii());
   statusLabel->setText(str);
   qApp->processEvents();
}

void EDSForm::AddRowInTable(QString name, QString discount, QString disc_id, QString cardcode) {
   
  QString s;
  int i = clientsTable->numRows();  
  clientsTable->setNumRows(i + 1);
  
  clientsTable->setText(i, 0, DefCodec->toUnicode(name)); 
  clientsTable->setText(i, 1, DefCodec->toUnicode(discount));
  clientsTable->setText(i, 2, DefCodec->toUnicode(disc_id));
  clientsTable->setText(i, 3, DefCodec->toUnicode(cardcode));
  
  clientsTable->clearSelection(true);
  clientsTable->selectRow(i);
  clientsTable->ensureVisible(0, i);
  

}

void EDSForm::SetSelectionUp()
{    
////puts("visualForm::SetSelectionUp()");

  QTableSelection sel = clientsTable->selection(0);
  int p_sel = sel.topRow();
    
  if((p_sel != 0) && (p_sel != -1))
  {
    clientsTable->clearSelection();
   // sel.init(--p_sel, 0);
   // sel.expandTo(p_sel, 1);
   // clientsTable->addSelection(sel);
    clientsTable->selectRow(--p_sel);
    clientsTable->ensureCellVisible(p_sel,0); 
  }
}

//====================================================================//
void EDSForm::SetSelectionDown()
{
////puts("visualForm::SetSelectionDown()");
    
  QTableSelection sel = clientsTable->selection(0);
  int p_sel = sel.topRow();
    
  if((p_sel != clientsTable->numRows() - 1) && (p_sel != -1))
  {
    clientsTable->clearSelection();
   // sel.init(++p_sel, 0);
   // sel.expandTo(p_sel, 1);
   // clientsTable->addSelection(sel);
    clientsTable->selectRow(++p_sel);
    clientsTable->ensureCellVisible(p_sel,0); 
  } 
}


void EDSForm::EnterHandler()
{
  QTableSelection sel = clientsTable->selection(0);
  int p_sel = sel.topRow();
    
  if(p_sel != -1)
  {
      puts("void EDSForm::EnterHandler()==========================");
      ClientName = clientsTable->text( p_sel, 0);
      Discount = clientsTable->text( p_sel, 1);
      Disc_Id = clientsTable->text( p_sel, 2);
      CardCode = clientsTable->text( p_sel, 3);
  }
   accept();
}


void EDSForm::BarcodeSignalHandler( const char * barcode )
{
    QString bc = QString(barcode);
    BarcodeEvent(bc);
}

void EDSForm::ClearClientsTable() {
    clientsTable->clearSelection();  
    clientsTable->setNumRows(0);
}
