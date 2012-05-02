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
#include "reg_pgsql.h"

extern Registration Reg;
extern Receipt * receipt;
extern SP sp;
extern BlockTimer *bt;
//extern KeyboardDevice * kbcode;
extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;
extern HWSRV * hw;

vector<string> id_man_list;
vector<string> nam_man_list;

void managerListForm::init()
{
    DefCodec = QTextCodec::codecForName(SYSCODEC);
    DbCodec = QTextCodec::codecForName(DBCODEC);
    
    managersTable->setColumnWidth(0, 300);
    managersTable->setNumCols(1);
    
    id_man_list.clear();
    nam_man_list.clear();
    
    searchtype = 0; // by name
    
    connect(searchLine, SIGNAL(textChanged(const QString &)), SLOT(SearchManager()));
    
   connect(HwSignalHandlersController::GetInstance(), SIGNAL(BarCode(const char *)), SLOT(BarcodeSignalHandler( const char *)));
   connect(HwSignalHandlersController::GetInstance(), SIGNAL(TrackCode(const char *)), SLOT(CardcodeSignalHandler( const char *)));
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence()) );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    
    if(sp.block_mouse) grabMouse();
    if(sp.autoblock_flag) bt->action_time = QTime::currentTime();

    HwSignalHandlersController::GetInstance()->SetControlledObject(VISUALSEARCH);
     grabKeyboard();
}

void managerListForm::destroy()
{
  id_man_list.clear();
  nam_man_list.clear(); 
  if(sp.block_mouse) releaseMouse();
  HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
  //releaseKeyboard();
}


void managerListForm::keyPressEvent( QKeyEvent * k )
{ 
    if ( ! isActiveWindow()  ) return;
    
    if(sp.autoblock_flag) bt->action_time = QTime::currentTime();
    
    int state = 0;   
        
    if ( hw->BCS_GetType() == 2 )  {
	state = kbcode_scaner->FindCode(k);
	int n = kbcode_scaner->GetCountEvent();
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


}


void managerListForm::makeKeyPress( QKeyEvent * k )
{
    switch(k->key())    {
   
    case Qt::Key_Enter :      {
	    EnterHandler();        
	    break;      }
    case Qt::Key_Return :      {        
	    EnterHandler(); 
	    break;      }
    case Qt::Key_F12 :      {        
	    Reject();
	    break;
	}
    case Qt::Key_Up :      {        SetSelectionUp();        break;            }
    case Qt::Key_Down :      {        SetSelectionDown();        break;            }	
    case Qt::Key_Escape :      {        searchLine->clear();        break;            }
    case Qt::Key_Left :  {
	    searchLine->cursorBackward( k->state() == Qt::ShiftButton );
	break;	    }
    case Qt::Key_Right : {
	    searchLine->cursorForward( k->state() == Qt::ShiftButton );	 
	break;	}
    case Qt::Key_Delete :  {
		searchLine->del(); 
	break;    }
    case Qt::Key_Home :  {
	   searchLine->home( k->state() == Qt::ShiftButton );
	break;	}
    case Qt::Key_End : {
	 searchLine->end( k->state() == Qt::ShiftButton );
	break;     }
    case Qt::Key_Backspace :	{
	    //puts("Qt::Key_Backspace");
	searchLine->backspace();
	break; }	
   case Qt::Key_F3 :      {
	    if ( searchLine->text().isEmpty() == false ) {
		searchtype = 1;
		fillTable();
	    }
	    break;      }	
   case Qt::Key_F7 :      {
	    if ( searchLine->text().isEmpty() == false ) {
		searchtype = 2;
		fillTable();
	    }
	    break;      }       
	
    default :      { 
	searchLine->insert(k->text());   
    }
    }
    return;
}


void managerListForm::GetKeySequence()
{
     if ( kbcode_scaner != NULL ) {
	int n = kbcode_scaner->GetCountEvent();
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


void managerListForm::SetSelectionUp()
{
  QTableSelection sel = managersTable->selection(0);
  int p_sel = sel.topRow();
    
  if((p_sel != 0) && (p_sel != -1))
  {
    managersTable->clearSelection();
    managersTable->selectRow(--p_sel);
    managersTable->ensureCellVisible(p_sel,0); 
  }
}


void managerListForm::SetSelectionDown()
{
  QTableSelection sel = managersTable->selection(0);
  int p_sel = sel.topRow();
    
  if((p_sel != managersTable->numRows() - 1) && (p_sel != -1))
  {
    managersTable->clearSelection();
    managersTable->selectRow(++p_sel);
    managersTable->ensureCellVisible(p_sel,0); 
  } 
}

void managerListForm::BarcodeEvent( QString barcode_str )
{
    // поиск менеджера по штрих-коду
   printf("void managerListForm::BarcodeEvent( %s )\n", barcode_str.ascii());
   searchtype = 1;
   searchLine->setText(DefCodec->toUnicode(barcode_str));
}

int managerListForm::fillTable()
{ 
    // searchtype = 
    // 0 - by name
    // 1 - by barcode
    // 2 - by cardcode
    
    id_man_list.clear();
    nam_man_list.clear();
    string nameptr = "";
    string bcptr = "";
    string ccptr = "";
    tsearchLabel->setText(DefCodec->toUnicode("Поиск (имя):"));
    if ( searchLine->text().isEmpty() == false ) {
	QString ptrn = DbCodec->fromUnicode(searchLine->text());
	switch (searchtype) {
	case 1: {
		tsearchLabel->setText(DefCodec->toUnicode("Поиск (ШК):"));
		bcptr = string(ptrn.ascii());
		break;
	}
	case 2: {
		tsearchLabel->setText(DefCodec->toUnicode("Поиск (карта):"));
		ccptr = string(ptrn.ascii());
		break;
	}
	default: {
		nameptr = string(ptrn.ascii());
		break;		
	}	
                }
    }
    searchtype = 0;
    
    //printf("name = [%s], bcode = [%s], card = [%s]\n", nameptr.c_str(), bcptr.c_str(), ccptr.c_str());
    
    int n = Reg.Query_Managers_List(&id_man_list, &nam_man_list, nameptr, bcptr, ccptr);
    managersTable->setNumRows(0);
    if (n > 0) {
	for (int jj = 0; jj < n; jj++) {
	    int i = managersTable->numRows();  
	    managersTable->setNumRows(i + 1);
	    managersTable->setText(i, 0, DefCodec->toUnicode(nam_man_list[jj].c_str())); 
	}
	managersTable->clearSelection(true);
	managersTable->selectRow(0);
	managersTable->ensureVisible(0, 0);
    }	
    
    return n;
}


void managerListForm::SearchManager()
{
    fillTable();
}


void managerListForm::Reject()
{
  idx = -2;
  reject();
}


void managerListForm::Close()
{
  idx = -1;
  close();
}


void managerListForm::EnterHandler()
{ 
  QTableSelection sel = managersTable->selection(0);
  int p_sel = sel.topRow();
    
  if(p_sel != -1)
  {
      idx = p_sel;
  }
  else {
      return;
  }
   accept();
}


QString managerListForm::GetName()
{
    if (idx >=0 ) return QString(nam_man_list[idx].c_str());
    return "";
}


QString managerListForm::GetId()
{
    if (idx >= 0) return QString(id_man_list[idx].c_str());
    return "";
}


void managerListForm::BarcodeSignalHandler( const char * barcode )
{
    QString bc = QString(barcode);
    BarcodeEvent(bc);
}


void managerListForm::CardcodeSignalHandler( const char * cardcode)
{
    QString cc = QString(cardcode);
    CardcodeEvent(cc);
}


void managerListForm::CardcodeEvent( QString cardcode_str )
{
    // поиск менеджера по коду магнитной карты
   printf("void managerListForm::CardcodeEvent( %s )\n", cardcode_str.ascii());
   searchtype = 2;
   searchLine->setText(DefCodec->toUnicode(cardcode_str));
}
