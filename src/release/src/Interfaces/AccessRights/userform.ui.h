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
** If you wpassant to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
//-----(+)-----Zuskin-----09/03/2011-----
#include "dkbinput.h"
#include "bcskbinput.h"
#include "mcrkbinput.h"
#include "hwsighandlerscontroller.h"
//--------------------------------------------------

extern AccessRights AccR;
extern HWSRV * hw;
extern UserForm * point_form;
extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;

//==================================================================================//
//-----(-)-----Zuskin-----09/03/2011-----
/*void UserForm::ScanerSignal() {
    //puts("UserForm::ScanerSignal()");
    //printf("barcode = %s \n", hw->BCS_GetBarCode());
    barcodeLineEdit->setText(hw->BCS_GetBarCode());
}
*/
//-------------------------------------------------
//==================================================================================//
//-----(-)-----Zuskin-----09/03/2011-----
/*void UserForm::ReaderSignal() {
    //puts("UserForm::ReaderSignal()");
    cardcodeLineEdit->setText( hw->MCR_GetSecondTrack() );
}
*/
//------------------------------------------------
//==================================================================================//
void UserForm::init()
{
//puts("void UserForm::init()");

  user_name = NULL;
  profile_name = NULL;
  profile_idents = NULL;
  grabKeyboard();			// ----------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!----------------------//
  defTextCodec = QTextCodec::codecForName(SYSCODEC);
  dbTextCodec = QTextCodec::codecForName(DBCODEC);
  //edit_flag = false;
  user_id = -1;
  InitUserForm();
  connect(cancelPushButton, SIGNAL(clicked()), SLOT(Cancel()));
  connect(savePushButton, SIGNAL(clicked()), SLOT(Save()));

  connect(nameLineEdit, SIGNAL(textChanged(const QString&)), SLOT(ChangeFlag()));
  connect(shortcutLineEdit, SIGNAL(textChanged(const QString&)), SLOT(ChangeFlag()));
  connect(profileComboBox, SIGNAL(textChanged ( const QString &) ), SLOT(ChangeFlag()));
  connect(barcodeLineEdit, SIGNAL(textChanged(const QString&)), SLOT(ChangeFlag()));
  connect(cardcodeLineEdit, SIGNAL(textChanged(const QString&)), SLOT(ChangeFlag()));
  connect(passwordLineEdit, SIGNAL(textChanged(const QString&)), SLOT(ChangePassword()));
    
  
  if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
  if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
  
point_form = this;

//-----(+)-----Zuskin-----25/03/2011-----    
//   grabKeyboard();

if ((hw->BCS_GetType() > 0) && (hw->BCS_GetType() != 2)) {
     connect(HwSignalHandlersController::GetInstance(), SIGNAL(BarCode(const char *)), SLOT(ScanerSignal( const char* )));
 }
 if ((hw->MCR_GetType()>0)  && (hw->MCR_GetType() !=2)) {
     connect(HwSignalHandlersController::GetInstance(), SIGNAL(TrackCode(const char *)), SLOT(ReaderSignal( const char* )) );
}		  
HwSignalHandlersController::GetInstance()->SetControlledObject(ACCESSRIGHTS);
 //---------------------------------------------------
  
}

//-----(+)-----Zuskin-----09/03/2011-----
void UserForm::ScanerSignal(const char* barcode)
{
    puts("void UserForm::ScanerSignal(const char* barcode)");
	 if ( HwSignalHandlersController::GetInstance()->GetControlledObject() != ACCESSRIGHTS ) return;
	 if ( ! this->isActiveWindow()  ) return;
	 QString barcode_str(barcode);
	 barcodeLineEdit->setText(barcode_str);
}
//=====================================================//
void UserForm::ReaderSignal(const char* cardcode)   {
      puts("void UserForm::ReaderSignal(const char* cardcode)");
    if ( HwSignalHandlersController::GetInstance()->GetControlledObject() != ACCESSRIGHTS ) return;   
    if ( ! this->isActiveWindow()  ) return;
    QString cardcode_str(cardcode);
    cardcodeLineEdit->setText( cardcode_str );
}

//---------------------------------------------------------

//==================================================================================//
void UserForm::ChangePassword(){
   //puts("Change Password");
   reppasswordLineEdit->setText(""); 
   reppasswordLineEdit->setEnabled(true);
   ChangeFlag();
}
//==================================================================================//
void UserForm::destroy()
{
//puts("void UserForm::destroy()");
  releaseKeyboard();
  if(user_name) free(user_name);
  if(profile_name) free(profile_name);
  if(profile_idents) free(profile_idents);
  point_form = NULL;
  
  
  //puts("-----------2--------------");
}

//==================================================================================//
void UserForm::InitKeyAccel()
{
//puts("void AccessRightsForm::InitKeyAccel()");
//  QAccel *a = new QAccel(this);
//  a->insertItem(Key_F12, 200);
//  a->connectItem(200, this, SLOT(Cancel())); 
}

//==================================================================================//
void UserForm::ChangeFlag()
{
//puts("void UserForm::ChangeFlag()");
  //edit_flag = true  ;
}

//==================================================================================//
void UserForm::InitUserForm()
{
//puts("void UserForm::InitUserForm()");
  char **p = NULL;
  int n = AccR.Get_Profiles_Info(p, profile_idents);
  QString s;
  profileComboBox->insertItem("  ");
  for(int i = 0; i < n; i ++)
  {
    s = dbTextCodec->toUnicode(*(p + i));
    profileComboBox->insertItem(s);
    free(*(p + i));
  }
  free(p);
  passwordLineEdit->setEchoMode(QLineEdit::Password);
  reppasswordLineEdit->setEchoMode(QLineEdit::Password);
  new_flag = true;
}

//==================================================================================//
bool UserForm::GetNewFlag()
{
//puts("bool UserForm::GetNewFlag()");
  return new_flag;
}

//==================================================================================//
int UserForm::GetNewIdent()
{
//puts("int UserForm::GetNewIdent()");
  return user_id;
}
//==================================================================================//
void UserForm::GetNewName(char *name)
{
//puts("void UserForm::GetNewName(char *name)");
  QString s = dbTextCodec->toUnicode(user_name);
  s = defTextCodec->fromUnicode(s);
  strcpy(name, s);
}

//==================================================================================//
void UserForm::GetNewProfile(char *name)
{
//puts("void UserForm::GetNewProfile(char *name)");
  QString s = dbTextCodec->toUnicode(profile_name);
  s = defTextCodec->fromUnicode(s);
  strcpy(name, s);
}

//==================================================================================//
void UserForm::ShowEditUser(int id)
{
//puts("void UserForm::ShowEditUser(int id)");
  //int pr_id = 0;
  char *name, *shortcut, *barcode, *cardcode, *userpass, *pr_name;
  if(AccR.Get_User(id, pr_name, name, shortcut, barcode, cardcode, userpass) != -1)
  {
      //printf("profile name = %s \n", pr_name);
      
    nameLineEdit->setText(dbTextCodec->toUnicode(name));
    shortcutLineEdit->setText(dbTextCodec->toUnicode(shortcut));
    barcodeLineEdit->setText(dbTextCodec->toUnicode(barcode));
    cardcodeLineEdit->setText(dbTextCodec->toUnicode(cardcode));
    passwordLineEdit->setText(dbTextCodec->toUnicode(userpass));
    reppasswordLineEdit->setText(dbTextCodec->toUnicode(userpass));
    reppasswordLineEdit->setEnabled(false);
    profileComboBox->setCurrentText(dbTextCodec->toUnicode(pr_name));
    //profileComboBox->setCurrentItem(pr_id);
    user_id = id;
 //   profile_id = pr_id; //new 27.06.06
    free(name);
    free(shortcut);
    free(barcode);
    free(cardcode);
    free(userpass);
    new_flag= false;
  }
}

//==================================================================================//
void UserForm::DisableNoEdit()
{
//puts("void UserForm::DisableNoEdit()");
  nameLineEdit->setEnabled(false);
  profileComboBox->setEnabled(false);
//  if(Acknowledgement(defTextCodec->toUnicode("Все изменения будут потеряны\nПродолжить?")) == 0) reject();
}

//==================================================================================//
void UserForm::Cancel()
{
//puts("void UserForm::Cancel()");
  if(Acknowledgement(defTextCodec->toUnicode("Все изменения будут потеряны\nПродолжить?")) == 0) reject();
}

//==================================================================================//
void UserForm::Get_User_Info(char *&shortcut, char *&bar_code, char *&card_code, char *&upassword)
{
//puts("void UserForm::Get_User_Info(char *shortcut, char *bar_code, char *card_code, char *upassword)");
    QCString s;
    s = dbTextCodec->fromUnicode(nameLineEdit->text());
    user_name = (char *) calloc(s.length() + 1, sizeof(char));
    strcpy(user_name, s);
    s = dbTextCodec->fromUnicode(profileComboBox->currentText());
    profile_name = (char *) calloc(s.length() + 1, sizeof(char));
    strcpy(profile_name, s);
    s = dbTextCodec->fromUnicode(shortcutLineEdit->text());
    shortcut = (char *) calloc(s.length() + 1, sizeof(char));
    strcpy(shortcut, s);
    s = dbTextCodec->fromUnicode(barcodeLineEdit->text());
    bar_code = (char *) calloc(s.length() + 1, sizeof(char));
    strcpy(bar_code, s);
    s = dbTextCodec->fromUnicode(cardcodeLineEdit->text());
    card_code = (char *) calloc(s.length() + 1, sizeof(char));
    strcpy(card_code, s);
    s = dbTextCodec->fromUnicode(passwordLineEdit->text());
    upassword = (char *) calloc(s.length() + 1, sizeof(char));
    strcpy(upassword, s);
}

//==================================================================================//
void UserForm::Get_Info_Extra() {
    QCString s;
    s = dbTextCodec->fromUnicode(nameLineEdit->text());
    user_name = (char *) calloc(s.length() + 1, sizeof(char));
    strcpy(user_name, s);
    s = dbTextCodec->fromUnicode(profileComboBox->currentText());
    profile_name = (char *) calloc(s.length() + 1, sizeof(char));
    strcpy(profile_name, s);
}
//==================================================================================//
void UserForm::Get_Info(char*& param, QLineEdit* le) {
    QCString s;
    s = dbTextCodec->fromUnicode(le->text());
    param = (char *) calloc(s.length() + 1, sizeof(char));
    strcpy(param, s);
}
//==================================================================================//
void UserForm::Save()
{
//puts("void UserForm::Save()");

  char* bar_code = NULL;
  char* card_code = NULL;
  
  Get_Info(bar_code, barcodeLineEdit);
  Get_Info(card_code, cardcodeLineEdit);
  
  // Не введено имя
  if(nameLineEdit->text() == "")   Error(1);
  //Не введен профиль пользователя
  else if(profileComboBox->currentItem() == 0) 	    Error(2);
  // пароль не совпадает с подтверждением
  else if(passwordLineEdit->text() != reppasswordLineEdit->text())	 Error(5);
  // данный штрих код не пустой уже существует
  else if ( (barcodeLineEdit->text() != "")  && (AccR.Check_Barcode(user_id, bar_code) == 1)  )    Error(6);
  // карт коде уже используется
  else if ( (cardcodeLineEdit->text() != "") && (AccR.Check_Cardcode(user_id, card_code) == 1)  )    Error(7);  
  else   {
/*    char *shortcut = NULL;
    char *bar_code = NULL;
    char *card_code = NULL;
    char *upassword = NULL;
    Get_User_Info(shortcut, bar_code, card_code, upassword);*/
    
    Get_Info_Extra();
    char * shortcut = NULL;  
    char* upassword = NULL; 
    Get_Info(shortcut, shortcutLineEdit);
    Get_Info(upassword, passwordLineEdit);
    
    int ret = 0;
  //  printf("???????????????????????????????????\nuser_id = %d\n????????????????????????????????????\n", user_id);
    
    if(user_id != -1) //EDIT
    {
      ret = AccR.Update_User_Info(user_id, user_name, shortcut, profile_idents[profileComboBox->currentItem() - 1],  bar_code,  card_code, upassword);
      if(ret == -1) Error(2); 
    }
    else 
    {

      if((user_id = AccR.Get_User_Id_by_Name(user_name)) > 0) 
      {
        if(Acknowledgement(defTextCodec->toUnicode("Пользователь с таким именем уже существует в базе\nПереписать?")) == 0)
        {
          new_flag = false;
          ret = AccR.Update_User_Info(user_id, user_name, shortcut, profile_idents[profileComboBox->currentItem() - 1],  bar_code,  card_code, upassword);
          if(ret == -1) Error(4);
        }
        else ret = -1;
      }
      else if(user_id == 0) //Новый пользователь
      {
        new_flag = true;
//printf("@@@@@@@@@@@@\nprofile_id = %d\n@@@@@@@@@@@@\n", profileComboBox->currentItem() - 1);
        ret = AccR.Add_New_User(user_id, user_name, shortcut, profile_idents[profileComboBox->currentItem() - 1],  bar_code,  card_code, upassword);
//printf("@@@@@@@@@@@@\n%d\n@@@@@@@@@@@@\n", profile_id);
        if(ret == -1) Error(3);
      }

    }
    free(shortcut);
    free(upassword);    
    if(ret != -1) accept();
    else reject();
  }
  free(bar_code);
  free(card_code);
}

//======================================================================//
int UserForm::Acknowledgement(QString text)	
{
//puts("int UserForm::Acknowledgement(QString text)");
  releaseKeyboard();
  QMessageBox mb( defTextCodec->toUnicode("Warning"), text, QMessageBox::NoIcon, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, QMessageBox::NoButton, this);
  mb.setButtonText( QMessageBox::Yes, defTextCodec->toUnicode("Да") );
  mb.setButtonText( QMessageBox::No, defTextCodec->toUnicode("Нет") );
  
  switch(mb.exec())
  {
    case QMessageBox::Yes :
printf("Yes\n");
      return 0;
      break;
    case QMessageBox::No :
printf("Cancel\n");
      return 1;
      break;
  }
  grabKeyboard();
  return 1;
}

//==================================================================================//
void UserForm::makeKeyPress(QKeyEvent * k) 
{
    QWidget* wid = this->focusData()->focusWidget();  
    //printf("wid named class = %s \n", wid->className());  
    
    if ( k->key() == 4129 || k->key() == 4128 || k->key() == 4131 || k->key() == 4132 || k->key() == 4384 || ( k->key() > 4144 && k->key() < 4155 ) ) return;
    
    
    switch (k->key())    {
    case Qt::Key_Enter :
	Save();
	break;
    case Qt::Key_Return :
	Save();
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
    case Qt::Key_Up :
	if ( strcmp(wid->className(), "QComboBox" ) == 0 )  {  
	    ((QComboBox*)wid)->setCurrentItem( ((QComboBox*)wid)->currentItem() - 1 );  };
	break;
    case Qt::Key_Down :
	if ( strcmp(wid->className(), "QComboBox" ) == 0 )  {  
	    ((QComboBox*)wid)->setCurrentItem( ((QComboBox*)wid)->currentItem() + 1 );   };
	break;     
    case Qt::Key_F12 :
	close();
	break;
    case Qt::Key_Escape :
	Cancel();
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
//==================================================================================//
void UserForm::GetKeySequence()
{
    //puts("-----------------GetKeySequence()---------------------");
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
//==================================================================================//
void UserForm::keyPressEvent(QKeyEvent *k)
{
    //puts("void UserForm::keyPressEvent(QKeyEvent *k)");
    
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
	    barcodeLineEdit->setText(barcode);
	    free(barcode);
	}
    }
    
    
    
    if ( hw->MCR_GetType() == 2 && state == 0 ) {
	state = kbcode_reader->FindCode(k);
	//printf("state = %d \n", state);
	if ( state == 2 ) {
	    int num_track;
	    QString cardcode_str = ((KeyboardReader*) kbcode_reader)->getCode(num_track);
	    char* cardcode = (char*) calloc(cardcode_str.length()+1, sizeof(char));
	    strcpy(cardcode, cardcode_str);
	    //printf("card code = <%s> \n", cardcode);
	    //printf("num_track = %d \n", num_track);
	    if (num_track == 2) cardcodeLineEdit->setText(cardcode);
	    free(cardcode);
	}
    }
    
    //printf("state of find scaner code = %d \n", state);
    if ( state == 0 ) {   makeKeyPress(k); return;  }
    if ( state == 5 )     { GetKeySequence();  return;  }
    
}
//==================================================================================//    
void UserForm::Error(int er)
{
  //puts("void UserForm::Error(int er)");
  QString er_str;
  QString er_id;

  AccessRightsMessageText(er, er_str, er_id);
  
  releaseKeyboard();
  InterfaceErrorWin(er_str, er_id);
  grabKeyboard();
}
//==================================================================================//


