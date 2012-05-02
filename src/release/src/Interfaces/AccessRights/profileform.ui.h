/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2009-2011, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2009-2011, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
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
//-----(+)-----Zuskin-----09/03/2011-----
#include "hwsrv.h"
#include "dkbinput.h"
#include "bcskbinput.h"
#include "mcrkbinput.h"
#include "hwsighandlerscontroller.h"

extern HWSRV * hw;
extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;
//--------------------------------------------------

extern AccessRights AccR;
const int count_rights = 30;
//=========================================================================//
void ProfileForm::init()
{
//puts("void ProfileForm::init()");
  rightsCheckListItems = NULL;
  profile_name = NULL;
  
  //-----(+)-----Zuskin-----09/03/2011-----
  grabKeyboard();
  //--------------------------------------------------
  
  defTextCodec = QTextCodec::codecForName( SYSCODEC);
  dbTextCodec = QTextCodec::codecForName( DBCODEC );
  profile_id = -1;
  newflag = false;
  InitProfileForm();
  connect(cancelPushButton, SIGNAL(clicked()), SLOT(Cancel()));
  connect(savePushButton, SIGNAL(clicked()), SLOT(Save()));
  //-----(+)-----Zuskin-----09/03/2011-----
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) {	
	connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    }
   
  if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
//--------------------------------------------------  
  
}

//==========================================================================//
void ProfileForm::destroy()
{
//puts("void ProfileForm::destroy()");
   //-----(+)-----Zuskin-----09/03/2011-----   
  releaseKeyboard();   
 //--------------------------------------------------   
  if(rightsCheckListItems) free(rightsCheckListItems);
  if(profile_name) free(profile_name);
}

//==========================================================================//
void ProfileForm::InitProfileForm()
{
//puts("void ProfileForm::InitProfileForm()");
  rightsListView->addColumn(defTextCodec->toUnicode(""));
  rightsCheckListItems = (QCheckListItem **) malloc(count_rights * sizeof(QCheckListItem *));
  QCheckListItem *chb;
  QCheckListItem *par  = new QCheckListItem (rightsListView, defTextCodec->toUnicode("Права"), QCheckListItem::CheckBoxController);
  rightsListView->insertItem(par);
  par->setOpen(true);
  
//puts("  //ГРУППЫ");
  QCheckListItem *gr_check  = new QCheckListItem (par, defTextCodec->toUnicode("Чек"), QCheckListItem::CheckBoxController);
  par->insertItem(gr_check);
  QCheckListItem *gr_payment  = new QCheckListItem (par, defTextCodec->toUnicode("Оплата"), QCheckListItem::CheckBoxController);
  par->insertItem(gr_payment);
  QCheckListItem *gr_service  = new QCheckListItem (par, defTextCodec->toUnicode("Сервис"), QCheckListItem::CheckBoxController);
  par->insertItem(gr_service);
  QCheckListItem *gr_operation  = new QCheckListItem (par, defTextCodec->toUnicode("Другие операции"), QCheckListItem::CheckBoxController);
  par->insertItem(gr_operation);
  QCheckListItem *gr_admin  = new QCheckListItem (par, defTextCodec->toUnicode("Администрирование"), QCheckListItem::CheckBoxController);
  par->insertItem(gr_admin);
  
//puts("  //ПОДГРУППЫ 1-ОГО УРОВНЯ");
  QCheckListItem *sub1_check_reg = new QCheckListItem (gr_check, defTextCodec->toUnicode("Регистрация"), QCheckListItem::CheckBoxController);
  gr_check->insertItem(sub1_check_reg);
  QCheckListItem *sub1_check_edit = new QCheckListItem (gr_check, defTextCodec->toUnicode("Редактирование"), QCheckListItem::CheckBoxController);
  gr_check->insertItem(sub1_check_edit);
  QCheckListItem *sub1_check_red = new QCheckListItem (gr_check, defTextCodec->toUnicode("Скидки/надбавки"), QCheckListItem::CheckBoxController);
  gr_check->insertItem(sub1_check_red);
  QCheckListItem *sub1_check_ret = new QCheckListItem (gr_check, defTextCodec->toUnicode("Чек возврата"), QCheckListItem::CheckBoxController);
  gr_check->insertItem(sub1_check_ret);
  
  QCheckListItem *sub1_service_rep = new QCheckListItem (gr_service, defTextCodec->toUnicode("Печать кассовых отчетов"), QCheckListItem::CheckBoxController);
  gr_service->insertItem(sub1_service_rep);
  
//puts("   //ПОДГРУППЫ 2-ОГО УРОВНЯ");
  QCheckListItem *sub2_check_red_add = new QCheckListItem (sub1_check_red, defTextCodec->toUnicode("Начисление скидок/надбавок"), QCheckListItem::CheckBoxController);
  sub1_check_red->insertItem(sub2_check_red_add);
  /*QCheckListItem *sub2_check_red_cancel = new QCheckListItem (sub1_check_red, defTextCodec->toUnicode("Отмена скидок/надбавок"), QCheckListItem::CheckBoxController);
  sub1_check_red->insertItem(sub2_check_red_cancel);*/
  
//puts("   //ЭЛЕМЕНТЫ");
  chb = new QCheckListItem (sub1_check_reg, defTextCodec->toUnicode("По внутреннему коду"), QCheckListItem::CheckBox);
  sub1_check_reg->insertItem(chb);
  *(rightsCheckListItems + 0) = chb;
  chb = new QCheckListItem (sub1_check_reg, defTextCodec->toUnicode("По штрих-коду сканером штрих-кода"), QCheckListItem::CheckBox);
  sub1_check_reg->insertItem(chb);
  *(rightsCheckListItems + 1) = chb;
  chb = new QCheckListItem (sub1_check_reg, defTextCodec->toUnicode("По штрих-коду набранному с клавиатуры"), QCheckListItem::CheckBox);
  sub1_check_reg->insertItem(chb);
  *(rightsCheckListItems + 2) = chb;
  chb = new QCheckListItem (sub1_check_reg, defTextCodec->toUnicode("Через визуальный подбор"), QCheckListItem::CheckBox);
  sub1_check_reg->insertItem(chb);
  *(rightsCheckListItems + 3) = chb;
  chb = new QCheckListItem (sub1_check_edit, defTextCodec->toUnicode("Сторнирование"), QCheckListItem::CheckBox);
  sub1_check_edit->insertItem(chb);
  *(rightsCheckListItems + 4) = chb;
  chb = new QCheckListItem (sub1_check_edit, defTextCodec->toUnicode("Редактирование количества"), QCheckListItem::CheckBox);
  sub1_check_edit->insertItem(chb);
  *(rightsCheckListItems + 5) = chb;
  chb = new QCheckListItem (sub1_check_edit, defTextCodec->toUnicode("Редактирование цены"), QCheckListItem::CheckBox);
  sub1_check_edit->insertItem(chb);
  *(rightsCheckListItems + 6) = chb;
  chb = new QCheckListItem (sub1_check_edit, defTextCodec->toUnicode("Повтор продажи последней позиции"), QCheckListItem::CheckBox);
  sub1_check_edit->insertItem(chb);
  *(rightsCheckListItems + 7) = chb;
  chb = new QCheckListItem (sub1_check_edit, defTextCodec->toUnicode("Отмена открытого чека"), QCheckListItem::CheckBox);
  sub1_check_edit->insertItem(chb);
  *(rightsCheckListItems + 8) = chb;
  chb = new QCheckListItem (sub2_check_red_add, defTextCodec->toUnicode("Начисление произвольных скидок/надбавок"), QCheckListItem::CheckBox);
  sub2_check_red_add->insertItem(chb);
  *(rightsCheckListItems + 9) = chb;
  chb = new QCheckListItem (sub2_check_red_add, defTextCodec->toUnicode("Начисление фиксированных скидок/надбавок"), QCheckListItem::CheckBox);
  sub2_check_red_add->insertItem(chb);
  *(rightsCheckListItems + 10) = chb;
  
  /*chb = new QCheckListItem (sub2_check_red_cancel, defTextCodec->toUnicode("Отмена автоматических скидок/надбавок"), QCheckListItem::CheckBox);
  sub2_check_red_cancel->insertItem(chb);
  *(rightsCheckListItems + 11) = chb;*/
  
  /*chb = new QCheckListItem (sub2_check_red_cancel, defTextCodec->toUnicode("Отмена прочих скидок/надбавок"), QCheckListItem::CheckBox);
  sub2_check_red_cancel->insertItem(chb);
  *(rightsCheckListItems + 12) = chb;*/
  
   /*chb = new QCheckListItem (sub2_check_red_cancel, defTextCodec->toUnicode("Отмена скидок/надбавок"), QCheckListItem::CheckBox);
  sub2_check_red_cancel->insertItem(chb);
  *(rightsCheckListItems + 12) = chb;*/
  
  chb = new QCheckListItem (sub1_check_red, defTextCodec->toUnicode("Отмена скидок/надбавок"), QCheckListItem::CheckBox);
  sub1_check_red->insertItem(chb);
  *(rightsCheckListItems + 12) = chb;
  
  chb = new QCheckListItem (sub1_check_ret, defTextCodec->toUnicode("Возврат по номеру чека"), QCheckListItem::CheckBox);
  sub1_check_ret->insertItem(chb);
  *(rightsCheckListItems + 13) = chb;
  
  chb = new QCheckListItem (sub1_check_ret, defTextCodec->toUnicode("Открытие чека возврата"), QCheckListItem::CheckBox);
  sub1_check_ret->insertItem(chb);
  *(rightsCheckListItems + 14) = chb;
  
  chb = new QCheckListItem (gr_payment, defTextCodec->toUnicode("Наличными"), QCheckListItem::CheckBox);
  gr_payment->insertItem(chb);
  *(rightsCheckListItems + 15) = chb;
  chb = new QCheckListItem (gr_payment, defTextCodec->toUnicode("Кредитом"), QCheckListItem::CheckBox);
  gr_payment->insertItem(chb);
  *(rightsCheckListItems + 16) = chb;
  chb = new QCheckListItem (gr_payment, defTextCodec->toUnicode("Тарой"), QCheckListItem::CheckBox);
  gr_payment->insertItem(chb);
  *(rightsCheckListItems + 17) = chb;
  
// chb = new QCheckListItem (gr_payment, defTextCodec->toUnicode("Типом оплаты 4"), QCheckListItem::CheckBox);
//  gr_payment->insertItem(chb);
//  *(rightsCheckListItems + 18) = chb;
  
  chb = new QCheckListItem (sub1_service_rep, defTextCodec->toUnicode("Снятие отчетов с гашением контрольной ленты"), QCheckListItem::CheckBox);
  chb->setEnabled(false);
  sub1_service_rep->insertItem(chb);
  *(rightsCheckListItems + 19) = chb;
  
  /*chb = new QCheckListItem (sub1_service_rep, defTextCodec->toUnicode("Снятие прочих отчетов"), QCheckListItem::CheckBox);
  sub1_service_rep->insertItem(chb);
  *(rightsCheckListItems + 20) = chb;*/
  
  chb = new QCheckListItem (sub1_service_rep, defTextCodec->toUnicode("Снятие отчетов"), QCheckListItem::CheckBox);
  sub1_service_rep->insertItem(chb);
  *(rightsCheckListItems + 20) = chb;
  
  chb = new QCheckListItem (gr_service, defTextCodec->toUnicode("Синхронизация даты/времени"), QCheckListItem::CheckBox);
  gr_service->insertItem(chb);
  *(rightsCheckListItems + 21) = chb;
  
  chb = new QCheckListItem (gr_operation, defTextCodec->toUnicode("Внесение денежных средств в кассу"), QCheckListItem::CheckBox);
  gr_operation->insertItem(chb);
  *(rightsCheckListItems + 22) = chb;
  chb = new QCheckListItem (gr_operation, defTextCodec->toUnicode("Выплата денежных средств из кассы"), QCheckListItem::CheckBox);
  gr_operation->insertItem(chb);
  *(rightsCheckListItems + 23) = chb;  
  chb = new QCheckListItem (gr_operation, defTextCodec->toUnicode("Печать копии чека"), QCheckListItem::CheckBox);
  gr_operation->insertItem(chb);
  
  *(rightsCheckListItems + 24) = chb;
  chb = new QCheckListItem (gr_operation, defTextCodec->toUnicode("Открыть ящик"), QCheckListItem::CheckBox);
  gr_operation->insertItem(chb);
  
  *(rightsCheckListItems + 25) = chb;
  
  chb = new QCheckListItem (gr_admin, defTextCodec->toUnicode("Настройка программы"), QCheckListItem::CheckBox);
  gr_admin->insertItem(chb);
  *(rightsCheckListItems + 26) = chb;
  
  chb = new QCheckListItem (gr_service, defTextCodec->toUnicode("Выгрузка продаж вручную"), QCheckListItem::CheckBox);
  gr_service->insertItem(chb);
  *(rightsCheckListItems + 27) = chb;
  
  chb = new QCheckListItem (gr_service, defTextCodec->toUnicode("Загрузка справочника товаров"), QCheckListItem::CheckBox);
  gr_service->insertItem(chb);
  *(rightsCheckListItems + 28) = chb;  
  
  chb = new QCheckListItem (gr_service, defTextCodec->toUnicode("Сверка итогов платежной системы"), QCheckListItem::CheckBox);
  gr_service->insertItem(chb);
  *(rightsCheckListItems + 29) = chb;  
}

//=======================================================================//
int ProfileForm::GetNewIdent()
{
//puts("int ProfileForm::GetNewIdent()");
 return profile_id;
}

//======================================================================//
void ProfileForm::GetNewName(char * name)
{
//puts("void ProfileForm::GetNewName(char * name)");
  QString s = dbTextCodec->toUnicode(profile_name);
  s = defTextCodec->fromUnicode(s);
  strcpy(name, s);
}

//=========================================================================//
bool ProfileForm::GetNewFlag()
{
//puts("bool ProfileForm::GetNewFlag()");
  return newflag;
}

//=========================================================================//
void ProfileForm::ShowEditProfile(int id)
{
//puts("void ProfileForm::ShowEditProfile(int id)");
  bool *rights = NULL;
  char * name = NULL;
  AccR.Get_Profile(rights, name, id);
//puts("{{{{{{{");
  for(int i = 0; i < count_rights; i ++)
    if(*(rights + i) && (i != 18) && (i !=11))
      (*(rightsCheckListItems + i))->setState(QCheckListItem::On);
   
   (*(rightsCheckListItems + 19))->setEnabled(*(rights+ 20));
//puts("{{{{{{{1");
  free(rights);
  profilenameLineEdit->setText(dbTextCodec->toUnicode(name));
  free(name);
  profile_id = id;
}

//=========================================================================//
void ProfileForm::Cancel()
{
//puts("void ProfileForm::Cancel()");
    if(Acknowledgement(defTextCodec->toUnicode("Все изменения будут потеряны\nПродолжить?")) == 0)  { 
	//-----(+)-----Zuskin-----09/03/2011-----  
	releaseKeyboard();
	//-------------------------------------------------  
	reject(); 
    }
}

//=========================================================================//
void ProfileForm::Save()
{
    
    if ( profilenameLineEdit->text() == "" )   { Error(3);  return;   }
   
    bool *rights = (bool *) calloc(count_rights, sizeof(bool));
    QCString s;
    s = dbTextCodec->fromUnicode(profilenameLineEdit->text());
    profile_name = (char *) calloc(s.length() + 1, sizeof(char));
    strcpy(profile_name, s);
    for ( int i = 0; i < count_rights; i++ )   {    
	if ( ( i != 18 ) && ( i != 11 ) && (*(rightsCheckListItems + i))->isOn() )   {  *(rights + i) = true; }
	else     *(rights + i) = false;
    }
    int ret = 0;
    
    //EDIT
    if ( profile_id != -1 )     {
	puts("profile_id = -1");
	ret = AccR.Update_Profile_Info(profile_id, profile_name, rights);     
	free(rights);
	if ( ret == -1 ) Error(2);
	if ( ret != -1 ) accept();  //     else reject();
	return ;
	
    }    
	
    if ( (profile_id = AccR.Get_Profile_Id_by_Name(profile_name)) > 0 )       {
	puts(" by name");
	if ( Acknowledgement(defTextCodec->toUnicode("Профиль с таким наименованием уже существует в базе\nПереписать?")) == 0 )   {
	    newflag = false;
	    ret = AccR.Update_Profile_Info(profile_id, profile_name, rights);
	    //printf("$$$$$$$$$$$$$\n ret =  %d\n$$$$$$$$$$$$$\n", ret);
	    
	    if ( ret == -1 )  Error(2);
	}  else ret = -1;	   
    }   else if(profile_id == 0)       {
	puts("add new p[rofile");
	//NEW
	newflag = true;
	ret = AccR.Add_New_Profile(profile_id, profile_name, rights);
	//printf(" Add_New_Profilec = %d \n", ret);
	//printf("@@@@@@@@@@@@\n%d\n@@@@@@@@@@@@\n", profile_id);
	//puts("Error(1)");
	if(ret == -1) Error(1);	   
    }
   
   free(rights);
   //-----(+)-----Zuskin-----09/03/2011-----  
  releaseKeyboard();
  //-------------------------------------------------  
   if(ret != -1) accept();       else reject();

}

//======================================================================//
int ProfileForm::Acknowledgement(QString text)
{
//puts("int ProfileForm::Acknowledgement(QString text)");
  QMessageBox mb( defTextCodec->toUnicode("ВНИМАНИЕ!"), text, QMessageBox::NoIcon, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, QMessageBox::NoButton, this);
  mb.setButtonText( QMessageBox::Yes, defTextCodec->toUnicode("Да") );
  mb.setButtonText( QMessageBox::No, defTextCodec->toUnicode("Нет") );
  releaseKeyboard();
  switch(mb.exec())
  {
    case QMessageBox::Yes :
//printf("Yes\n");
      grabKeyboard();	
      return 0;
      break;
    case QMessageBox::No :
//printf("Cancel\n");
      grabKeyboard();	
      return 1;
      break;
  }
  grabKeyboard();
  return 1;
}
//===========================================================================//
void ProfileForm::keyPressEvent ( QKeyEvent* k ) {
    
    int state = 0;    
    if ( hw->BCS_GetType() == 2 )  {
	state = kbcode_scaner->FindCode(k);
	//printf("state = %d \n", state);
	if ( state == 2 )  {
	    QString barcode_str = ((KeyboardScaner*) kbcode_scaner)->getCode();
	    char* barcode = (char*) calloc(barcode_str.length()+1, sizeof(char));
	    strcpy(barcode, barcode_str);
	    printf("barcode = <%s> \n", barcode);
	    //barcodeLineEdit->setText(barcode);
	    free(barcode);
	}
    }        
    //printf("state of find scaner code = %d \n", state);
    if ( state == 0 ) {   makeKeyPress(k); return;  }
    if ( state == 5 )     { GetKeySequence();  return;  }
    
    
   /* switch(k->key())    {
       case Qt::Key_Enter :
          Save();
          break;
       case Qt::Key_Return :
          Save();
          break;
       case Qt::Key_F12 :
          close();
          break;
       case Qt::Key_Escape :
          Cancel();
          break;	
      } */
}
//==========================================================================//
void ProfileForm::Error(int er)
{
  //puts("void ProfileForm::Error(int er)");
  er = er + 7;
  QString er_str;
  QString er_id;

  AccessRightsMessageText(er, er_str, er_id);
  
  //releaseKeyboard();
  InterfaceErrorWin(er_str, er_id);
  //grabKeyboard();
}

//=========================================================================//
void ProfileForm::rightsListView_pressed( QListViewItem * qlvi)
{

    (*(rightsCheckListItems + 19))->setEnabled( (*(rightsCheckListItems + 20))->isOn() );
    if ( !(*(rightsCheckListItems + 20))->isOn() ) (*(rightsCheckListItems + 19))->setState(QCheckListItem::Off);
       
    
}
//=======================================================================//
//-----(+)-----Zuskin-----09/03/2011-----
void ProfileForm::GetKeySequence()
{
puts("-----------------GetKeySequence()---------------------");
    if ( kbcode_scaner != NULL ) {
	int n = kbcode_scaner->GetCountEvent();
	for ( int i = 0; i < n; i++ )   {
	    makeKeyPress( kbcode_scaner->GetEvent(i) );	
	}
	   
	kbcode_scaner->Clear();
    }
    
    if ( kbcode_reader != NULL ) {
	int n = kbcode_reader->GetCountEvent();
	for ( int i = 0; i < n; i++ )   {
	    makeKeyPress( kbcode_reader->GetEvent(i) );	
	}
	kbcode_reader->Clear();
    }
    
}
//----------------------------------------------------
void ProfileForm::makeKeyPress(QKeyEvent * k) 
{
    QWidget* wid = this->focusData()->focusWidget();  
 //   printf("wid named class = %s \n", wid->className());  
    
    if ( k->key() == 4129 || k->key() == 4128 || k->key() == 4131 || k->key() == 4132 || k->key() == 4384 || ( k->key() > 4144 && k->key() < 4155 ) ) return;
    
    
    switch (k->key())    {
    case Qt::Key_Enter :
	if ( strcmp(wid->className(), "QPushButton" ) == 0 ) {
	    ((QButton*)wid)->animateClick();
	}
	else {
	    Save();
	}    
	break;
    case Qt::Key_Return :
	if ( strcmp(wid->className(), "QPushButton" ) == 0 ) {
	    ((QButton*)wid)->animateClick();
	}
	else {
	    Save();
	}    
	break;
    case Qt::Key_Left :
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {
	    ((QLineEdit*)wid)->cursorBackward( k->state() == Qt::ShiftButton );	    }
	break;
    case Qt::Key_Right :
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {	
	    ((QLineEdit*)wid)->cursorForward( k->state() == Qt::ShiftButton );	    }
	break;	
    case Qt::Key_Tab :
	focusData()->next();
	break;
    case Qt::Key_Plus :
	if ( strcmp(wid->className(), "QListView" ) == 0 ) {
	    ((QListView*)wid)->currentItem()->setOpen(true);	    }
	break;	
    case Qt::Key_Asterisk :	
    case Qt::Key_Space :
	if ( strcmp(wid->className(), "QListView" ) == 0 ) {
	    QCheckListItem* curItm;
	    curItm =  ((QCheckListItem*)((QListView*)wid)->currentItem());
	    if (curItm->state() == QCheckListItem::On) {
		curItm->setState(QCheckListItem::Off); 	
	    }
	    else {
		curItm->setState(QCheckListItem::On); 	
	    }
	
	}
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) { ((QLineEdit*)wid)->insert(QString(" "));   
                }  
	break;	    	
    case Qt::Key_Minus :
	if ( strcmp(wid->className(), "QListView" ) == 0 ) {
	    ((QListView*)wid)->currentItem()->setOpen(false);	    }
	break;		
    case Qt::Key_Up :
	if ( strcmp(wid->className(), "QListView" ) == 0 ) {
	    ((QListView*)wid)->setCurrentItem(((QListView*)wid)->currentItem()->itemAbove());	    }
	break;
    case Qt::Key_Down :
	if ( strcmp(wid->className(), "QListView" ) == 0 ) {
	    ((QListView*)wid)->setCurrentItem(((QListView*)wid)->currentItem()->itemBelow());	    }
	break;     
    case Qt::Key_F12 :
	close();
	break;
    case Qt::Key_Escape :
	close();
	break;
    case Qt::Key_Delete :
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {	((QLineEdit*)wid)->del();  }
	break;    
    case Qt::Key_Backspace :	
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 )   {	((QLineEdit*)wid)->backspace();    }
	break;	
   case Qt::Key_Home :
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {	
	    ((QLineEdit*)wid)->home( k->state() == Qt::ShiftButton );	    }
	break;
    case Qt::Key_End :
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) {
	    ((QLineEdit*)wid)->end( k->state() == Qt::ShiftButton );	    }
	break;
    default :				
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) { ((QLineEdit*)wid)->insert(k->text());   
                }  
     }	
}	
