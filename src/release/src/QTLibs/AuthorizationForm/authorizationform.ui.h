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
#define AUTHORIZATION_CARD_TRACK 2

extern SP sp;
extern AccessRights Acc;
extern HWSRV * hw;
//extern KeyboardDevice * kbcode;
extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;


const int max_count_authorization = 3;

HwSignalHandlersController HwSignalHandlersController::instance;

//======================================================================//
void authorizationForm::showEvent(QShowEvent * s)
{
    //puts("----4444----");
    grabKeyboard();
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
}
//======================================================================//
void authorizationForm::ReaderSignal(const char * track) {
    
    //printf("Reader Signal (track = %s)\n", track);
    
    if (strlen(track) < 1) return;
    
    if ( HwSignalHandlersController::GetInstance()->GetControlledObject() != AUTHORIZATION ) return;
    if ( ! this->isActiveWindow() ) return;    
    
    if ( ! sp.card_authorization ) return;
    
    CardcodeAuthorization(track);
}
//======================================================================//
void authorizationForm::ScanerSignal(const char * barcode) {
    
    //printf("ScanerSignal(barcode = %s) \n", barcode);
    
    if (strlen(barcode) < 1) return;
    
    if ( HwSignalHandlersController::GetInstance()->GetControlledObject() != AUTHORIZATION ) return;
    if ( ! this->isActiveWindow() ) return;    
    
    if ( ! sp.barcode_authorization ) return;
    //puts("authorizationForm::ScanerSignal()");
    BarcodeAuthorization(barcode);
}
//======================================================================//
void authorizationForm::init()
{  
  
  //printf("DBCODEC = %s\n",DBCODEC);
  //printf("SYSCODEC = %s\n",SYSCODEC);

    
  DefCodec = QTextCodec::codecForName(SYSCODEC);
  DbCodec = QTextCodec::codecForName(DBCODEC);
  grabKeyboard();
  adjustSize();
  
  //QString true_caption = DefCodec->toUnicode(PROGRAMM_VERSION) + " - " + this->caption();
  //this->setCaption(true_caption);
  
  if (sp.password_authorization) {
      //puts("password authorization anable");
      passwordableTextLabel->setEnabled(true);
      passwordablePixmapLabel->setEnabled(true);
      passwordLineEdit->setEchoMode(QLineEdit::Password);
      //passwordLineEdit->setEnabled(true);
  } 
  
  if (sp.barcode_authorization) {
      //printf("barcode_authorization anable\n");
      barcodeableTextLabel->setEnabled(true);
      barcodeablePixmapLabel->setEnabled(true);  
  }
  
  if (sp.card_authorization) {
      //printf("card authorization anable\n");
      cardableTextLabel->setEnabled(true);
      cardablePixmapLabel->setEnabled(true);
  }

  connect(nameComboBox, SIGNAL(highlighted(int)), this, SLOT(ChangeProfileName(int)));

  userListBox = nameComboBox->listBox();

  one_operation_flag = false;
  
  user_name = NULL;
  user_profile = NULL;
  idents = NULL;
  
  new_user_name = NULL;
  count_authorization = 0;

  connect(HwSignalHandlersController::GetInstance(), SIGNAL(BarCode(const char *)), SLOT(ScanerSignal( const char *)));
  connect(HwSignalHandlersController::GetInstance(), SIGNAL(TrackCode(const char *)), SLOT(ReaderSignal( const char *)));
  
  
  //printf("hw->BCS_GetType() = %d \n", hw->BCS_GetType());
  //printf("hw->MCR_GetType() = %d \n", hw->MCR_GetType());
  
  if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
  if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
  
  
  block_mode = true;
  HwSignalHandlersController::GetInstance()->SetControlledObject(AUTHORIZATION);
  //puts("end of init");
}
//======================================================================//
void authorizationForm::destroy()
{
//puts("****************************void authorizationForm::destroy()************************");
      HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
  if ( new_user_name!=NULL )  free(new_user_name);

  for(int i = 0; i < num_user; i++) 
    if ( *(user_name + i) != NULL ) free(*(user_name + i));
  if ( user_name != NULL ) free(user_name);

  for(int i = 0; i < num_user; i++)
    if ( *(user_profile + i) != NULL ) free(*(user_profile + i));
  if ( user_profile != NULL ) free(user_profile);

  if ( idents != NULL ) free(idents);
  releaseKeyboard();

  
//  puts("****************************destroy ok***************************************");
}
//======================================================================//
// заполняет список пользователей
// используется вне формы
// если num_rights != -1, то в список входят  тольк пользователи, меющие право с индексами rindex
int authorizationForm::InitUserList(int * rindex, int num_rights)
{
  //printf("void authorizationForm::InitUserList(int * rindex, int num_rights )\n");
  
  if(num_rights > 0)
    one_operation_flag = true;
  else
    one_operation_flag = false;  
  if(one_operation_flag)
    num_user = Acc.Get_Users_Info_with_Rights(user_name, user_profile, idents, rindex, num_rights);
  else
    num_user = Acc.Get_Users_Info(user_name, user_profile, idents);
 // printf("num user = %d \n", num_user);
  if(num_user != -1)  
  {
    for(int i = 0; i < num_user; i ++)
    {
      QString s = DbCodec->toUnicode(user_name[i]);
     // printf("user_name[i] = %s\n", user_name[i]);
      nameComboBox->insertItem(s, i);
     // printf("user_profile[%d] = %s\n", i, user_profile[i]);
    }
    profileTextLabel->setText(DbCodec->toUnicode(user_profile[0]));

    return 0;
  }
  else
  {
    Error(3);
    return -1;
  }   
  //puts("end initUserList");
}
//======================================================================//
void authorizationForm::ChangeProfileName(int cur)
{
 // printf("void authorizationForm::ChangeProfileName(int cur = %d )\n", cur);
  profileTextLabel->setText(DbCodec->toUnicode(user_profile[cur]));
}

//======================================================================//
void authorizationForm::ChangeFocus()
{
//puts("void authorizationForm::ChangeFocus()");
  if(nameComboBox->hasFocus()) {
    passwordLineEdit->setFocus();
    userListBox->close();
  } else
    nameComboBox->setFocus();
}

//======================================================================//
void authorizationForm::ChangeComboItemUp()
{
//puts("void authorizationForm::ChangeComboItemUp()");
  //if(nameComboBox->hasFocus())
    nameComboBox->popup();
  if(userListBox->hasFocus())
  {
    int cur = userListBox->currentItem();
    if(cur != 0) userListBox->setCurrentItem(--cur);
  }
}

//======================================================================//
void authorizationForm::ChangeComboItemDown()
{
//puts("void authorizationForm::ChangeComboItemDown()");
 // if(nameComboBox->hasFocus())
    nameComboBox->popup();
  if(userListBox->hasFocus())
  {
    int cur = userListBox->currentItem();
    if(cur != (num_user - 1)) userListBox->setCurrentItem(++cur);
  }
}

//======================================================================//
void authorizationForm::UserSelected()
{
//puts("void authorizationForm::UserSelected()");
  int cur = userListBox->currentItem();
  //wid = (QWidget *) userListBox;
  //wid->close();
  userListBox->close();

  nameComboBox->setFocus();
  if ( nameComboBox->currentItem() != cur ) count_authorization = 0;
  nameComboBox->setCurrentItem(cur);
}

//======================================================================//
void authorizationForm::CardcodeAuthorization(const char* cardcode) 
{
    printf("CardAuthorization(code = %s) \n", cardcode);
    
    int uid = -1;
    int res = Acc.Authorize_by_Cardcode(cardcode, uid);
    
    if ( res < 0 ) { Authorization(res, -1); return; }
    if ( res > 0 ) { 
	Error(6); 
	Authorization(1, -1); 
	return; 
    }
    
    
    //printf("k = %d \n", uid);
    int i = 0;
    while ( i < num_user && idents[i] != uid ) i++;

    //printf("i = %d \n", i);
    if ( i == num_user )  {  
	//puts("error 5");  
	Error(5); 
	Authorization(1, -1); 
	return; 
    }
    
    author_user = i;
    if ( !one_operation_flag ) {
	Message(0);
    } 
    Authorization(0, uid);
}
//======================================================================//
void authorizationForm::BarcodeAuthorization(const char* barcode) 
{
    printf("BarcodeAuthorization(barcode = %s)\n", barcode);
    
    int uid = -1;
    int res = Acc.Authorize_by_Barcode(barcode, uid);
    if ( res < 0 ) { Authorization(res, -1); return; }
    if ( res > 0 ) { 
	//puts("error 6"); 
	Error(6); Authorization(1, -1); return; }
   // printf("k = %d \n", uid);
    int i = 0;
    while ( i<num_user && idents[i] != uid ) { 
	//printf("idents[%d] = %d \n", i, idents[i]); 
	i++;} 
    //printf("i = %d \n", i);
    if ( i == num_user )  { 
	//puts("error 5");  
	Error(5);  Authorization(1, -1); return; }
    author_user = i;
    if (!one_operation_flag ) {
	//QString name_user = DefCodec->toUnicode("Вы авторизовались как ");
	//name_user += user_name[i] ;
	Message(0);
    }
    Authorization(0, uid);
    
    
}
//======================================================================//
void authorizationForm::PasswordAuthorization()
{
    puts("PasswordAuthorization()");
    
    int uid = idents[nameComboBox->currentItem()];

    QString qspass;

    qspass = DbCodec->fromUnicode(passwordLineEdit->text());
    char *pass = (char *) calloc(qspass.length() + 1, sizeof(char));
    strcpy(pass, qspass);
    int res =  Acc.Authorize_by_Password(uid, pass); 
    free(pass);
    if ( res > 0 ) {    Error(4);  passwordLineEdit->setText("");    }
   //printf("-------------------------------- res > 0");
    Authorization(res, uid);
}

//======================================================================//
void authorizationForm::Authorization( int res, int uid ) 
{
        
    if ( res == 0 )  {    
	author_user = uid;  
	if ( SetNewRights(uid) != 0 ) {
	    reject();
	    puts("Authorization ----- cancel");
	} else {
	    new_uid = uid;
	    puts("Authorization ----- ok");
	    accept();   
	}
    } 
    if (res < 0) {  
	Error(3);     
	puts("Authorization ----- cancel");
	reject();   
    }	
   
   if (res > 0) {
       count_authorization++;
       if ( count_authorization  >= max_count_authorization && !block_mode ) {  
	   Error(1);  
	   puts("Authorization ----- cancel");
	   reject();       
       }
   }
}
//======================================================================//
int authorizationForm::SetNewRights(int uid)
{
//printf("void authorizationForm::SetNewRights(%d) \n", uid);
  int ret = Acc.Get_User_Rights(uid, new_user_name, new_rights);
  //printf("ret = %d \n", ret);
  if ( ret != 0 ) { 
      Error(2); 
      return -1;
  } else {
      return 0;
  }
  
}
//======================================================================//
void authorizationForm::GetNewRights(bool *rights)
{
//puts("void authorizationForm::GetNewRights(bool *rights)");
  for(int i = 0; i < Acc.count_rg; i++)    rights[i] = new_rights[i];
}

//======================================================================//
void authorizationForm::GetNewUser(char *& user, int &uid)
{
//puts("void authorizationForm::GetNewUser(char *& user, int &uid)");
  user = (char *) calloc(strlen(new_user_name) + 1, sizeof(char));
  strcpy(user, new_user_name);
  
  uid = new_uid;
//printf("user_id = %d, user_name = <%s>\n", uid, user);
}
//======================================================================//
void authorizationForm::makeKeyPress(QKeyEvent * k) 
{
    //printf("----aut form. makeKeyPress k -> key() = %d \n", k->key());
    switch(k->key())    {
    case Qt::Key_Enter :
	if(userListBox->hasFocus())	    UserSelected();
	else {
	    if(sp.password_authorization)   PasswordAuthorization();
	    else Error(9);
	}
	break;
    case Qt::Key_Return :
	if(userListBox->hasFocus())  UserSelected();
	else {
	    if(sp.password_authorization)  PasswordAuthorization();
	    else Error(9);
	}
	break;
    case Qt::Key_Left :
	//  if(passwordLineEdit->hasFocus())
	passwordLineEdit->cursorBackward(false);
	break;
    case Qt::Key_Right :
	//   if(passwordLineEdit->hasFocus())
	passwordLineEdit->cursorForward(false);
	break;	
    case Qt::Key_Tab :
	ChangeFocus();
	break;
    case Qt::Key_Up :
	ChangeComboItemUp();
	break;
    case Qt::Key_Down :
	ChangeComboItemDown();
	break;     
    case Qt::Key_F12 :
	//puts("REJECT");
	HwSignalHandlersController::GetInstance()->SetControlledObject(NOCONTROLLED);
	reject();
	puts("Authorization ----- cancel");
	close();
	break;
    case Qt::Key_Escape :
	//  if(passwordLineEdit->hasFocus())
	passwordLineEdit->clear();
	break;
    case Qt::Key_Backspace :
	// if(passwordLineEdit->hasFocus())
	passwordLineEdit->backspace();
	break;
    case Qt::Key_Delete :
	passwordLineEdit->del();
	break;
    case Qt::Key_F7 :
	ReaderSignal(passwordLineEdit->text());
	break;
    case Qt::Key_F3 :	
	ScanerSignal(passwordLineEdit->text());
	break;
    default :
	    //puts("===2===");
	    //if(passwordLineEdit->hasFocus())	
	    passwordLineEdit->insert(k->text());  }
}
//======================================================================//
void authorizationForm::GetKeySequence()
{
   // puts("-----------------GetKeySequence()---------------------");
    
    /*if ( kbcode != NULL ) {
	int n = kbcode->GetCountEvent();
	for ( int i = 0; i < n; i++ )   {
	  //  printf( "kbcode->GetEvent(i)->key() = %d  \n", kbcode->GetEvent(i)->key() );
	    makeKeyPress( kbcode->GetEvent(i) );	}
	kbcode->Clear();
    }
    */
    
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
//======================================================================//
void authorizationForm::keyPressEvent(QKeyEvent * k)
{
    //puts("------authorizationForm----- keyPress");
    //printf("k->key() = %d\n", k->key());
    //printf("k->text() = %s\n", k->text().ascii());
    
    int state = 0;
    
    if ( hw->BCS_GetType() == 2 )  {
	state = kbcode_scaner->FindCode(k);
	//printf("state = %d \n", state);
	if ( state == 2 )  {
	    if ( sp.barcode_authorization ) {
		QString barcode_str = ((KeyboardScaner*) kbcode_scaner)->getCode();
		char* barcode = (char*) calloc(barcode_str.length()+1, sizeof(char));
		strcpy(barcode, barcode_str);
		// printf("wtrix kod = <%s> \n", barcode);
		BarcodeAuthorization(barcode);
		free(barcode);
	    } else Error(7);
	}
    }
    
    if ( hw->MCR_GetType() == 2 && state == 0 ) {
	state = kbcode_reader->FindCode(k);
	//printf("state = %d \n", state);
	if ( state == 2 ) {
	    if ( sp.card_authorization ) {
		int num_track;
		QString cardcode_str = ((KeyboardReader*) kbcode_reader)->getCode(num_track);
		char* cardcode = (char*) calloc(cardcode_str.length()+1, sizeof(char));
		strcpy(cardcode, cardcode_str);
		// printf("-----------------------card code = <%s> \n", cardcode);
		//printf("num_track = %d \n", num_track);
		if (num_track == AUTHORIZATION_CARD_TRACK) CardcodeAuthorization(cardcode);	
		free(cardcode);
	    } else Error(8);
	} 
    }
    
    //printf("state of find code = %d \n", state);
    //if ( state == 6 ) return;
    if ( state == 0 ) 	{  makeKeyPress(k);   return;  }
    if ( state == 5 || state == 6 )   {  GetKeySequence();	}
}

//======================================================================//
void authorizationForm::Error(int er)
{
//  puts("void authorizationForm::Error(int er)");
    
    QString er_str;
    QString er_id;
    
    AuthorizationErrorText(er, er_str, er_id);
    
    releaseKeyboard();
    InterfaceErrorWin(er_str, er_id);
    grabKeyboard();  
}
//======================================================================//
void authorizationForm::Message(int m)
{
  //printf("void authorizationForm::Error(int er = %d) \n", m);
  
  QString er_str;
  QString er_id;
  //printf("user_name[uid] = %s \n", user_name[author_user]);    
  
  if (m == 0) {
      er_str = DbCodec->toUnicode(user_name[author_user]);
  }
  AuthorizationErrorText(m, er_str, er_id);
  
  releaseKeyboard();
  InterfaceMessageWin(er_str, er_id);
  grabKeyboard();
  
}
//======================================================================//
void authorizationForm::SetBlockMode(bool ablock_mode)
{
    block_mode = ablock_mode;
}
//======================================================================//
bool authorizationForm::GetBlockMode()
{	
    return block_mode;
}
//======================================================================//



