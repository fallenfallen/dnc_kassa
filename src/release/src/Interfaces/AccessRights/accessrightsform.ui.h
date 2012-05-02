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
#include "dkbinput.h"
#include "bcskbinput.h"
#include "mcrkbinput.h"
#include "hwsighandlerscontroller.h"

extern HWSRV * hw;
extern KeyboardDevice * kbcode_scaner;
extern KeyboardDevice * kbcode_reader;
//--------------------------------------------------

extern AccessRights AccR;
extern UserForm * point_form;
const int count_rights = 26;

//==================================================================================//
void AccessRightsForm::init()
{
puts("void AccessRightsForm::init()");    
  
  defTextCodec = QTextCodec::codecForName(SYSCODEC);
  dbTextCodec =  QTextCodec::codecForName(DBCODEC);
  profile_idents = NULL;
  user_idents = NULL;
  
  //-----(+)-----Zuskin-----09/03/2011-----
  grabKeyboard();
  //--------------------------------------------------
/*  QTimer *timer_init = new QTimer( this );
  connect(timer_init, SIGNAL(timeout()), SLOT(InitAccessRightsForm()));
  timer_init->start(10, true);
*/  
  InitConnections();
  //InitKeyAccel();
  InitWidget();
  
  userTable->setCurrentCell( 0, 0 );
  
  ErrorConnection = ( AccR.er_con != 0 ) ;
  if (ErrorConnection) return;
  
  focus_wid = (QWidget *) profileListBox;
  focus_wid->setFocus();
  focus_ind = 0;
//-----(+)-----Zuskin-----09/03/2011-----
    if (  hw->BCS_GetType() == 2 && kbcode_scaner != NULL ) {	
	connect( kbcode_scaner->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
    }
   
  if (  hw->MCR_GetType() == 2 && kbcode_reader != NULL) connect( kbcode_reader->timer, SIGNAL(timeout()), this, SLOT(GetKeySequence())  );
//--------------------------------------------------  
}

//==================================================================================//
void AccessRightsForm::InitConnections()
{
//puts("void AccessRightsForm::InitConnections()");

  connect(profileListBox, SIGNAL(doubleClicked(QListBoxItem *)), SLOT(EditProfile()));//SLOT(ProfileDoubleCliked()));
  connect(newprofilePushButton, SIGNAL(clicked()), SLOT(AddProfile()));
  connect(editprofilePushButton, SIGNAL(clicked()), SLOT(EditProfile()));
  connect(deleteprofilePushButton, SIGNAL(clicked()), SLOT(DelProfile()));
  

  connect(userTable, SIGNAL(doubleClicked(int , int , int, const QPoint&)), SLOT(EditUser()));//SLOT(UserDoubleCliked()));  
  connect(newuserPushButton, SIGNAL(clicked()), SLOT(AddUser()));
  connect(edituserPushButton, SIGNAL(clicked()), SLOT(EditUser()));
  connect(deleteuserPushButton, SIGNAL(clicked()), SLOT(DelUser()));
}

//==================================================================================//
void AccessRightsForm::InitKeyAccel()
{
//puts("void AccessRightsForm::InitKeyAccel()");

  /*QAccel *a = new QAccel(this);
  a->insertItem(Key_Left, 200);
  a->connectItem(200, this, SLOT(LeftChangeFocus())); 
  a->insertItem(Key_Right, 201);
  a->connectItem(201, this, SLOT(RightChangeFocus()));
  a->insertItem(Key_Up, 202);
  a->connectItem(202, this, SLOT(UpChangeSelection())); 
  a->insertItem(Key_Down, 203);
  a->connectItem(203, this, SLOT(DownChangeSelection()));
  a->insertItem(Key_F12, 204);
  a->connectItem(204, this, SLOT(CloseAccessRightsForm()));
  a->insertItem(Key_Enter, 205);
  a->connectItem(205, this, SLOT(EnterHandler()));
  a->insertItem(Key_Return, 206);
  a->connectItem(206, this, SLOT(EnterHandler()));*/
}
//==================================================================================//
void AccessRightsForm::InitWidget()
{
//puts("void AccessRightsForm::InitWidget()");

  profileListBox->setSelectionMode(QListBox::Single);
  userTable->setColumnWidth(0, 190);
  userTable->setColumnWidth(1, 190);
  userTable->setSelectionMode(QTable::SingleRow);
  
  /*AccR.setAdminProfileName(dbTextCodec->fromUnicode(defTextCodec->toUnicode("Администратор")));
  AccR.setAdminName(dbTextCodec->fromUnicode(defTextCodec->toUnicode("Администратор")));
  AccR.setAdminShortcut(dbTextCodec->fromUnicode(defTextCodec->toUnicode("Админ")));*/
  
  /*заполнение списка профилей*/
  char **p;
  p = NULL;
  int n = AccR.Get_Profiles_Info(p, profile_idents);
  if (n < 0) { Error(8); return; }
    
  for(int i = 0; i < n; i ++)    {
      profileListBox->insertItem(dbTextCodec->toUnicode(*(p + i)));
      free(*(p + i));    }
  free(p);
   puts("----------------------------------");
   puts("profile is load");
   puts("----------------------------------");

  /*заполнение списка пользователей*/
  char **u, **up; 
  u = NULL;
  up = NULL;
  
  n = AccR.Get_Users_Info(u, up, user_idents);
 // printf("get users info = %d \n", n);
  //printf("%d\n", n);
  if (n < 0) { Error(8); return; }
  
  userTable->setNumRows(n);
  for(int i = 0; i < n; i ++)
  {
      QTableItem *i0= new QTableItem(userTable, QTableItem::Never, dbTextCodec->toUnicode(*(u + i)));
      userTable->setItem(i, 0, i0);
      QTableItem *i1= new QTableItem(userTable, QTableItem::Never, dbTextCodec->toUnicode(*(up + i)));
      userTable->setItem(i, 1, i1);
      free(*(u + i));
      free(*(up + i));
  }
  free(u);
  free(up);
    
  userTable->selectRow(0);
  table_sel_ind = 0;
  tsel = userTable->selection(userTable->currentSelection());
  userTable->removeSelection(tsel);
  profileListBox->setSelected(0, true);
  list_sel_ind = 0;
}

/*
//==================================================================================//
void AccessRightsForm::InitAccessRightsForm()
{
puts("void AccessRightsForm::InitAccessRightsForm()");
  InitConnections();
  InitKeyAccel();
  InitWidget();
}*/

//==================================================================================//
void AccessRightsForm::LeftChangeFocus()
{
//puts("void AccessRightsForm::LeftChangeFocus()");
/*  switch(focus_ind)
  {
    case 0 :  // profileListBox 
      focus_wid = (QWidget *) deleteuserPushButton;
      focus_wid->setFocus();
      profileListBox->setSelected(list_sel_ind, false);
      userTable->addSelection(tsel);
//      userTable->selectRow(table_sel_ind);
      focus_ind = 7;
      break;
    case 1 : // newprofilePushButton :
      focus_wid = (QWidget *) profileListBox;
      focus_wid->setFocus();
      focus_ind = 0;
      break;
    case 2 : // editprofilePushButton :
      focus_wid = (QWidget *) newprofilePushButton;
      focus_wid->setFocus();
      focus_ind = 1;
      break;
    case 3 : // deleteprofilePushButton :
      focus_wid = (QWidget *) editprofilePushButton;
      focus_wid->setFocus();
      focus_ind = 2;
      break;
    case 4 : // userTable :
      focus_wid = (QWidget *) deleteprofilePushButton;
      focus_wid->setFocus();
      userTable->removeSelection(tsel);
      profileListBox->setSelected(list_sel_ind, true);
      focus_ind = 3;
      break;
    case 5 : // newuserPushButton :
      focus_wid = (QWidget *) userTable;
      focus_wid->setFocus();
      focus_ind = 4;
      
      break;
    case 6 : // edituserPushButton :
      focus_wid = (QWidget *) newuserPushButton;
      focus_wid->setFocus();
      focus_ind = 5;
      break;
    case 7 : // deleteuserPushButton :
      focus_wid = (QWidget *) edituserPushButton;
      focus_wid->setFocus();
      focus_ind = 6;
      break;
  }*/
}

//==================================================================================//
void AccessRightsForm::RightChangeFocus()
{
//puts("void AccessRightsForm::RightChangeFocus()");
/* switch(focus_ind)
  {
    case 0 : // profileListBox :
      focus_wid = (QWidget *) newprofilePushButton;
      focus_wid->setFocus();
      focus_ind = 1;
      break;
    case 1 : // newprofilePushButton :
      focus_wid = (QWidget *) editprofilePushButton;
      focus_wid->setFocus();
      focus_ind = 2;
      break;
    case 2 : // editprofilePushButton :
      focus_wid = (QWidget *) deleteprofilePushButton;
      focus_wid->setFocus();
      focus_ind = 3;
      break;
    case 3 : // deleteprofilePushButton :
      focus_wid = (QWidget *) userTable;
      focus_wid->setFocus();
      profileListBox->setSelected(list_sel_ind, false);
      userTable->addSelection(tsel);
//      userTable->selectRow(table_sel_ind);
      focus_ind = 4;
      break;
    case 4 : // userTable :
      focus_wid = (QWidget *) newuserPushButton;
      focus_wid->setFocus();
      focus_ind = 5;
      break;
    case 5 : // newuserPushButton :
      focus_wid = (QWidget *) edituserPushButton;
      focus_wid->setFocus();
      focus_ind = 6;
      break;
    case 6 : // edituserPushButton :
      focus_wid = (QWidget *) deleteuserPushButton;
      focus_wid->setFocus();
      focus_ind = 7;
      break;
    case 7 : // deleteuserPushButton :
      focus_wid = (QWidget *) profileListBox;
      focus_wid->setFocus();
      userTable->removeSelection(tsel);
      profileListBox->setSelected(list_sel_ind, true);
      focus_ind = 0;
      break;
  }*/
}

//==================================================================================//
void AccessRightsForm::UpChangeSelection()
{
//puts("void AccessRightsForm::UpChangeSelection()");

   QWidget* wid = this->focusData()->focusWidget();
   
   if ( strcmp(wid->className(), "QTable") == 0 ) {
       QTable* qtab = (QTable*) wid;
       if ( qtab->currentRow() > 0 )  qtab->setCurrentCell( qtab->currentRow()-1, 0);
   }

   
   if  ( strcmp(wid->className(), "QListBox") == 0 ) {
       QListBox* qlb = (QListBox*) wid;
       qlb->setCurrentItem(qlb->currentItem()-1);
   }
   
//printf("focus_ind = %d \n", focus_ind);
/*
  switch(focus_ind)
  {
    case 0 :
      if(list_sel_ind != 0)
      {
        profileListBox->setSelected(list_sel_ind, false);
        profileListBox->setSelected(-- list_sel_ind, true);
      }
      break;
    case 4 :
      puts("user table is focus");
      printf("table_set_ind b= %d \n", table_sel_ind);
      if(table_sel_ind != 0)
      {
        userTable->selectRow(-- table_sel_ind);
        tsel = userTable->selection(userTable->currentSelection());
      }
      break;
  }*/
}

//==================================================================================//
void AccessRightsForm::DownChangeSelection()
{
//puts("void AccessRightsForm::DownChangeSelection()");

   QWidget* wid = this->focusData()->focusWidget();
   //printf("class name = ");
   if ( strcmp(wid->className(), "QTable") == 0 ) {
       QTable* qtab = (QTable*) wid;
       if ( qtab->currentRow() < qtab->numRows() )  qtab->setCurrentCell( qtab->currentRow()+1, 0 );
   }

   if  ( strcmp(wid->className(), "QListBox") == 0 ) {
       QListBox* qlb = (QListBox*) wid;
       qlb->setCurrentItem(qlb->currentItem()+1);
   }
   
  /*switch(focus_ind)
  {
    case 0 :
      if(list_sel_ind != (profileListBox->numRows() - 1))
      {
        profileListBox->setSelected(list_sel_ind, false);
        profileListBox->setSelected(++ list_sel_ind, true);
      }
      break;
    case 4 :
      if(table_sel_ind != (userTable->numRows() - 1))
      {
        userTable->selectRow(++table_sel_ind);
        tsel = userTable->selection(userTable->currentSelection());
      }
      break;
  }*/
}

//==================================================================================//
void AccessRightsForm::EnterHandler()
{
//puts("void AccessRightsForm::EnterHandler()");
   if ( this->focusData()->focusWidget() == userTable ) EditUser();
   if ( this->focusData()->focusWidget() == profileListBox ) EditProfile();
   if ( this->focusData()->focusWidget() == newprofilePushButton ) AddProfile();
   if ( this->focusData()->focusWidget() == editprofilePushButton ) EditProfile() ;
   if ( this->focusData()->focusWidget() == deleteprofilePushButton ) DelProfile() ;
   if ( this->focusData()->focusWidget() == newuserPushButton ) AddUser() ;
   if ( this->focusData()->focusWidget() == edituserPushButton ) EditUser() ;
   if ( this->focusData()->focusWidget() == deleteuserPushButton ) DelUser() ;
}

//==================================================================================//
void AccessRightsForm::ProfileDoubleCliked()
{
//puts("void AccessRightsForm::ProfileDoubleCliked()");
   EditProfile();
}

//==================================================================================//
void AccessRightsForm::UserDoubleCliked()
{
//puts("void AccessRightsForm::UserDoubleCliked()");
   EditUser();
}


//==================================================================================//
void AccessRightsForm::AddProfile()
{
//puts("void AccessRightsForm::AddProfile()");
//  focus_wid = (QWidget *) newprofilePushButton;
//  focus_wid->setFocus();
  if(focus_ind >= 4)
  {
    userTable->removeSelection(tsel);
    profileListBox->setSelected(list_sel_ind, true);
  }
  focus_ind = 1;
  
  ProfileForm *pf = new ProfileForm;
  releaseKeyboard();
  int ret = pf->exec();
  grabKeyboard();
  if(ret == QDialog::Accepted)
  {
    int id = pf->GetNewIdent();
    if(pf->GetNewFlag()) //ДЕЙСТВИТЕЛЬНО НОВАЯ СТРОКА
   {
      int n = profileListBox->numRows();
      profile_idents = (int *) realloc(profile_idents, (n + 1) * sizeof(int));
      *(profile_idents + n) = id;
// printf("^^^^^^^^\nid = %d*(profile_idents + n - 1) = %d\n^^^^^^^^\n", id , *(profile_idents + n));
      char * name = (char *) calloc(150, sizeof(char));
      pf->GetNewName(name);
      profileListBox->insertItem(defTextCodec->toUnicode(name));
      free(name);
      profileListBox->setCurrentItem( profileListBox->count() -1 );
    }
    else //ПРОФИЛЬ С ЭТИМ ИДЕНТИФИКАТОРОМ УЖЕ СУЩЕСТВУЕТ
    {
      int index = 0;
      while(profile_idents[index] != id) index++;
      char *name = (char *) calloc(150, sizeof(char));
      pf->GetNewName(name);
      profileListBox->changeItem(defTextCodec->toUnicode(name), index);
      free(name);
      profileListBox->setCurrentItem(index);
    }
  }
  delete(pf);
}

//==================================================================================//
void AccessRightsForm::AddUser()
{
//puts("void AccessRightsForm::AddUser()");
//  focus_wid = (QWidget *) newuserPushButton;
//  focus_wid->setFocus();
  if(focus_ind < 4)
  {
  //  profileListBox->setSelected(list_sel_ind, false);
  //  userTable->addSelection(tsel);
  }
  focus_ind = 5;

  UserForm *uf = new UserForm;
  releaseKeyboard();
  int ret = uf->exec();
  grabKeyboard();
  if(ret == QDialog::Accepted)
  {
    int id = uf->GetNewIdent();
    if(uf->GetNewFlag()) //ДЕЙСТВИТЕЛЬНО НОВАЯ СТРОКА
    {
      int n = userTable->numRows();
      user_idents = (int *) realloc(user_idents, (n + 1) * sizeof(int));
      *(user_idents + n) = id;
// printf("^^^^^^^^\nid = %d*(user_idents + n) = %d\n^^^^^^^^\n", id , *(user_idents + n));

       userTable->setNumRows(n + 1);
      
      char* name = (char *) calloc(150, sizeof(char));
      uf->GetNewName(name);
      QTableItem *i0= new QTableItem(userTable, QTableItem::Never, defTextCodec->toUnicode(name));
      userTable->setItem(n, 0, i0);
      free(name);

      char * profile_name = (char *) calloc(150, sizeof(char));
      uf->GetNewProfile(profile_name);
//printf("^^^^^^**********\nprofile_name <%s>\n^^^^^^**********\n", profile_name);
//printf("^^^^^^********** name <%s> ^^^^^^**********\n", name);
      QTableItem *i1= new QTableItem(userTable, QTableItem::Never, defTextCodec->toUnicode(profile_name));
//printf("^^^^^^**********\nprofile_name <%s>\n^^^^^^**********\n", profile_name);      
      userTable->setItem(n, 1, i1);
      free(profile_name);
    }
    else // ПОЛЬЗОВАТЕЛЬ С ТАКИМ ИМЕНЕМ УЖЕ СУЩЕСТВУЕТ
    {
      int index = 0;
      while(user_idents[index] != id) index++;
      char* name = (char *) calloc(150, sizeof(char));
      uf->GetNewName(name);
      QTableItem *i0= new QTableItem(userTable, QTableItem::Never, defTextCodec->toUnicode(name));
      userTable->setItem(index, 0, i0);
      free(name);
      
      char * profile_name = (char *) calloc(150, sizeof(char));
      uf->GetNewProfile(profile_name);	
      QTableItem *i1= new QTableItem(userTable, QTableItem::Never, defTextCodec->toUnicode(name));
      userTable->setItem(index, 1, i1);
      free(profile_name);
    }
  }
  delete(uf);
}

//==================================================================================//
void AccessRightsForm::EditProfile()
{
//puts("void AccessRightsForm::EditProfile()");

  int index = profileListBox->index(profileListBox->selectedItem());
  if(index == -1) Error(5);
  else
  {
    if(profile_idents[index] == 1) //АДМИНИСТРАТОР
      Error(1);
    else
    {
      focus_ind = 2;

      ProfileForm *pf = new ProfileForm;
      pf->ShowEditProfile(profile_idents[index]);
      releaseKeyboard();
      int ret = pf->exec();
      grabKeyboard();
      if(ret == QDialog::Accepted)
      {
        char* name = (char *) calloc(150, sizeof(char));
        pf->GetNewName(name);
        profileListBox->changeItem(defTextCodec->toUnicode(name), index);
        free(name);
        profileListBox->clear();
        InitWidget();
        profileListBox->setCurrentItem(index);	
      }
      delete(pf);
    }
}
}

//==================================================================================//
void AccessRightsForm::EditUser()
{
//puts("void AccessRightsForm::EditUser()");  
  int index = userTable->currentRow();
//printf("***********\nindex %d\n***********\n", index);
  if(index == -1) Error(5);
  else
  {
//    focus_wid = (QWidget *) edituserPushButton;
 //   focus_wid->setFocus();
/*    if(focus_ind < 4)
    {
      profileListBox->setSelected(list_sel_ind, false);
      userTable->addSelection(tsel);
    }*/
    focus_ind = 6;

    UserForm *uf = new UserForm;
    uf->ShowEditUser(user_idents[index]);
    if(user_idents[index] == 1) //АДМИНИСТРАТОР
      uf->DisableNoEdit();
    releaseKeyboard();
    int ret = uf->exec();
    grabKeyboard();
    if(ret == QDialog::Accepted)
    {
//printf("accept\n");
      char* name = (char *) calloc(150, sizeof(char));
      uf->GetNewName(name);
      QTableItem *i0= new QTableItem(userTable, QTableItem::Never, defTextCodec->toUnicode(name));
      userTable->setItem(index, 0, i0);
      free(name);

      char * profile_name = (char *) calloc(150, sizeof(char));
      uf->GetNewProfile(profile_name);	
      QTableItem *i1= new QTableItem(userTable, QTableItem::Never, defTextCodec->toUnicode(profile_name));
      userTable->setItem(index, 1, i1);
      free(profile_name);
    }
    delete(uf);
  }
}

//==================================================================================//
void AccessRightsForm::DelProfile()
{
//puts("void AccessRightsForm::DelProfile()");
   /*
  if ( QMessageBox::question( this,  defTextCodec->toUnicode("Подтверждение удаления"), defTextCodec->toUnicode("Вы действительно желаете удалить выделенный профиль??"), defTextCodec->toUnicode("Да") , defTextCodec->toUnicode("Нет")) != QMessageBox::Yes ) return;
  */
  QMessageBox mb( defTextCodec->toUnicode("Подтверждение удаления"), defTextCodec->toUnicode("Вы действительно желаете удалить выделенный профиль?"), QMessageBox::Question, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, QMessageBox::NoButton, this);
  mb.setButtonText( QMessageBox::Yes, defTextCodec->toUnicode("Да") );
  mb.setButtonText( QMessageBox::No, defTextCodec->toUnicode("Нет") );
  
  releaseKeyboard();
  if (mb.exec() != QMessageBox::Yes ) {   grabKeyboard(); return; }
  grabKeyboard();
  
  int index = profileListBox->index(profileListBox->selectedItem());
  int n = profileListBox->numRows();
  if(index == -1) Error(6);
  else  {
    if(profile_idents[index] == 1) //АДМИНИСТРАТОР
    Error(1);
    else
    {
     // focus_wid = (QWidget *) deleteprofilePushButton;
     // focus_wid->setFocus();
      /*if(focus_ind >= 4)
      {
        userTable->removeSelection(tsel);
        profileListBox->setSelected(list_sel_ind, true);
      }*/
      focus_ind = 3;
      
      int res;
      if((res = AccR.Delete_Profile(profile_idents[index])) == 0)
      {
        for(int i = index; i < n; i ++)
          profile_idents[i] = profile_idents[i + 1] ;
        profile_idents[n -1] = 0;
        if((profile_idents = (int *) realloc(profile_idents, (n - 1) * sizeof(int))) == NULL) puts("NULL REALLOC PARAMETER profile_idents");

        profileListBox->setSelected(list_sel_ind, false);
        profileListBox->removeItem(index);
        if(list_sel_ind == n - 1) list_sel_ind--; 
        profileListBox->setSelected(list_sel_ind, true);
      }
      else if(res == -1) Error(3);
      else Error(4);
    }
  }
}

//==================================================================================//
void AccessRightsForm::DelUser()
{
//puts("void AccessRightsForm::DeleteUser()");

  QMessageBox mb( defTextCodec->toUnicode("Подтверждение удаления"), defTextCodec->toUnicode("Вы действительно желаете удалить выделенного пользователя?"), QMessageBox::Question, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, QMessageBox::NoButton, this);
  mb.setButtonText( QMessageBox::Yes, defTextCodec->toUnicode("Да") );
  mb.setButtonText( QMessageBox::No, defTextCodec->toUnicode("Нет") );
  
  releaseKeyboard();
  if (mb.exec() != QMessageBox::Yes) return;
  grabKeyboard();
  
  int index = userTable->currentRow();
  int n = userTable->numRows();
  if(index == -1) Error(6);
  else
  {
    if(user_idents[index] == 1) //АДМИНИСТРАТОР
    Error(2);
    else
    {
   //   focus_wid = (QWidget *) deleteuserPushButton;
   //   focus_wid->setFocus();
      if(focus_ind < 4)
     {
       profileListBox->setSelected(list_sel_ind, false);
       userTable->addSelection(tsel);
     }
     focus_ind = 7;
     
      if(AccR.Delete_User(user_idents[index]) == 0)
      {
        for(int i = index; i < n; i ++)
          user_idents[i] = user_idents[i + 1] ;
        user_idents[n -1] = 0;
        if((user_idents = (int *) realloc(user_idents, (n - 1) * sizeof(int))) == NULL) puts("NULL REALLOC PARAMETER profile_idents");

        userTable->removeSelection(tsel);
        userTable->removeRow(index);
        if(table_sel_ind == n - 1)
        {
          table_sel_ind--;
          userTable->selectRow(table_sel_ind);
          tsel = userTable->selection(userTable->currentSelection());
          userTable->addSelection(tsel);
        }
      }
      else Error(7);
    }
  }
}

//==================================================================================//
void AccessRightsForm::CloseAccessRightsForm()
{
//puts("void AccessRightsForm::CloseAccessRightsForm()");
    releaseKeyboard();
  close();
}

//==================================================================================//
int AccessRightsForm::Acknowledgement(QString text)
{
//puts("int AccessRightsForm::Acknowledgement(QString text)");
QMessageBox mb( defTextCodec->toUnicode("Warning"), text, QMessageBox::Question, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, QMessageBox::NoButton, this);
  mb.setButtonText( QMessageBox::Yes, defTextCodec->toUnicode("Да") );
  mb.setButtonText( QMessageBox::No, defTextCodec->toUnicode("Нет") );
  releaseKeyboard();
  switch(mb.exec())
  {
    case QMessageBox::Yes :
printf("Yes\n");
      grabKeyboard();
      return 0;
      break;
    case QMessageBox::No :
printf("Cancel\n");
      grabKeyboard();
      return 1;
      break;
  }
  return 1;
}

//==================================================================================//
void AccessRightsForm::Error(int er)
{
 // printf("void AccessRightsForm::Error(%d \n)", er);
  er = er +10;
  QString er_str;
  QString er_id;

  AccessRightsMessageText(er, er_str, er_id);
  
  releaseKeyboard();
  InterfaceErrorWin(er_str, er_id);
  grabKeyboard();
}

//==================================================================================//

//-----(+)-----Zuskin-----09/03/2011-----
void AccessRightsForm::GetKeySequence()
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
void AccessRightsForm::makeKeyPress(QKeyEvent * k) 
{
    QWidget* wid = this->focusData()->focusWidget();  
//    printf("wid named class = %s \n", wid->className());  
    
    if ( k->key() == 4129 || k->key() == 4128 || k->key() == 4131 || k->key() == 4132 || k->key() == 4384 || ( k->key() > 4144 && k->key() < 4155 ) ) return;
    
    
    switch (k->key())    {
    case Qt::Key_Enter :
	if ( strcmp(wid->className(), "QPushButton" ) == 0 ) {
	    ((QButton*)wid)->animateClick();
	}
	if ( strcmp(wid->className(), "QListBox" ) == 0 ) {
	    EditProfile();
	}
	if ( strcmp(wid->className(), "QTable" ) == 0 ) {
	    EditUser();
	}   
	break;
    case Qt::Key_Return :
	if ( strcmp(wid->className(), "QPushButton" ) == 0 ) {
	    ((QButton*)wid)->animateClick();
	}
	if ( strcmp(wid->className(), "QListBox" ) == 0 ) {
	    EditProfile();
	}
	if ( strcmp(wid->className(), "QTable" ) == 0 ) {
	    EditUser();
	}   
	break;
    case Qt::Key_Left :
	break;
    case Qt::Key_Right :
	break;	
    case Qt::Key_Tab :
	focusData()->next();
	break;
    case Qt::Key_Up :
	UpChangeSelection();
	break;
    case Qt::Key_Down :
	DownChangeSelection();
	break;     
    case Qt::Key_F12 :
	close();
	break;
    case Qt::Key_Escape :
	close();
	break;
    case Qt::Key_Delete :
	if ( strcmp(wid->className(), "QListBox" ) == 0 ) {
	    DelProfile();
	}
	if ( strcmp(wid->className(), "QTable" ) == 0 ) {
	    DelUser();
	}
	break;
    case Qt::Key_Insert :
	if ( strcmp(wid->className(), "QListBox" ) == 0 ) {
	    AddProfile();
	}
	if ( strcmp(wid->className(), "QTable" ) == 0 ) {
	    AddUser();
	}   	
	break;	
    default :				
	if ( strcmp(wid->className(), "QLineEdit" ) == 0 ) { //   ((QLineEdit*)wid)->insert(k->text());   
                }  
     }	
}	

    
void AccessRightsForm::keyPressEvent(QKeyEvent *k)
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
	    printf("wtrix kod = <%s> \n", barcode);
	    //barcodeLineEdit->setText(barcode);
	    free(barcode);
	}
    }
    
    
    //printf("state of find scaner code = %d \n", state);
    if ( state == 0 ) {   makeKeyPress(k); return;  }
    if ( state == 5 )     { GetKeySequence();  return;  }
    
}
    
