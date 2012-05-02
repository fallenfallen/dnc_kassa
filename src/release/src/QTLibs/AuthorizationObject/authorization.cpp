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

 #include "authorization.h"

#include "authorizationform.h"
#include "setup_param.h"
#include "blocktimer.h"
#include "hwsrv.h"
#include "const_conf.h"

#include <iostream>

#define NUMRIGHTS 30

extern BlockTimer *bt;
extern SP sp;
extern HWSRV * hw;

static authorizationForm * af;

Authorization Authorization::instance;

//====================PRIVATE============================================//
Authorization::Authorization()
{
    puts("--------AUTHORIZATION-------");
    cur_uid = 0;
    afr_uid = 0;
    
    Acc = new AccessRights(LINCASH_CONF_PATH);
    
    af = NULL;
}

//====================STATIC=============================================//
Authorization * Authorization::GetInstance()
{
    return &instance;
}

//======================================================================//
const char * Authorization::GetUserName() const 
{
    return cur_username.c_str();
}
//======================================================================//
const char * Authorization::GetUserShortcut() const
{
    return cur_shortcut.c_str();
}
//======================================================================//
int Authorization::GetUid() const 
{
    return cur_uid;
}

//======================================================================//
bool Authorization::GetRight(int index) const
{
    if ( index < (int) rights.size() )    return rights.at(index);
    
    return false;
}

//======================================================================//
bool Authorization::Init(int id)
{
    //puts("bool Authorization::Init(int id)");
    GetRights(id);
    return true;
    
}
//======================================================================//
bool Authorization::GetRights(int id)
{
    char * user_name = NULL;
    bool r[NUMRIGHTS];
    
    int ret = Acc->Get_User_Rights(id, user_name, r);
    /*puts("-----------------------------");  
    printf("Acc->Get_User_Rights(%d, %s, r) = %d \n", id, user_name, ret);
    puts("-----------------------------"); */
    
    
    if ( ret == 0 )   {
	cur_uid = id;
	afr_uid = cur_uid;
	
	cur_username = user_name;
	afr_username = cur_username;
	free(user_name);
	
	char * shortcut = NULL;
	Acc->GetSellerName(QString::number(id).ascii(), shortcut);
	cur_shortcut = shortcut;
	if ( shortcut!= NULL) free(shortcut);
	
	rights.assign(r, r + NUMRIGHTS);
	
	return true;
    }	
    
    return false;
}
//======================================================================//
int Authorization::Request(bool block_mode )
{
    //puts("int Authorization::Request(bool block_mode )");
    int ret = 0;
    
    af = new authorizationForm();
    
    af->SetBlockMode(block_mode);
    
    ret = af->InitUserList(NULL, -1);
    
    
    int authres = af->exec();    
    
    if ( (ret == 0) && (authres == QDialog::Accepted) )  {
	bool r[NUMRIGHTS];
	//puts("af->GetNewRights(r);");
	af->GetNewRights(r);
	rights.assign(r, r + NUMRIGHTS);
	
	char *user_name = NULL;
	int id;
	//puts("af->GetNewUser(user_name, id);");
	af->GetNewUser(user_name, id);
	cur_uid = id;
	afr_uid = id;
	cur_username = user_name;
	afr_username = user_name;
	puts(user_name);
	free(user_name);
	
	char * shortcut = NULL;
	Acc->GetSellerName(QString::number(id).ascii(), shortcut);
	cur_shortcut = shortcut;
	if ( shortcut!= NULL) free(shortcut);
	
    }  else {
	ret = 1;
    }
    
    delete(af);
    af = NULL;
    return ret;
}

//======================================================================//
bool Authorization::RequestWithAlliedRights(int * index, int num_rights)
{
    if ( sp.autoblock_flag ) bt->Stop(); // Чтобы выключить автоблокировку....
    bool right = false;
    if ( sp.query_for_authorization )  {
	af = new authorizationForm();
	
	af->InitUserList(index, num_rights);
	
	if ( af->exec() == QDialog::Accepted )    {
	    right = true;
	    
	    char * user_name = NULL;
	    af->GetNewUser(user_name, cur_uid);
	    cur_username = user_name;
	    free(user_name);
	    
	    char * shortcut = NULL;
	    Acc->GetSellerName(QString::number(cur_uid).ascii(), shortcut);
	    cur_shortcut = shortcut;
	    if ( shortcut!= NULL) free(shortcut);
	}
	
	delete(af);
	af = NULL;
    }
    if ( sp.autoblock_flag ) bt->Start();
    return right;
}

//======================================================================//
bool Authorization::RequestWithRight(int index)
{
    if ( sp.autoblock_flag ) bt->Stop(); // Чтобы выключить автоблокировку....
    bool right = false;
    if ( sp.query_for_authorization )  {
	af = new authorizationForm();
	
	af->SetBlockMode(false);
	
	af->InitUserList(&index, 1);
	
	if ( af->exec() == QDialog::Accepted )    {
	    right = true;
	    
	    char * user_name = NULL;
	    af->GetNewUser(user_name, cur_uid);
	    cur_username = user_name;
	    free(user_name);
	     
	    char * shortcut = NULL;
	    Acc->GetSellerName(QString::number(cur_uid).ascii(), shortcut);
	    cur_shortcut = shortcut;
	    if ( shortcut!= NULL) free(shortcut);
	    
	}
	
	delete(af);
	af = NULL;
    }
    if ( sp.autoblock_flag ) bt->Start();
    return right;
}

//======================================================================//
void Authorization::SwapUser()
{
    if ( cur_uid != afr_uid )  {
	cur_uid = afr_uid;
	cur_username = afr_username;
	
	char * shortcut = NULL;
	Acc->GetSellerName(QString::number(cur_uid).ascii(), shortcut);
	cur_shortcut = shortcut;
	if ( shortcut!= NULL) free(shortcut);
    }
}
/*
//======================================================================//
bool AuforizationRequest::Request(int index, bool & auforization_request_flag)
{
puts("bool AuforizationRequest::Request(int index, bool & auforization_request_flag)");
  if(sp.autoblock_flag) bt->Stop(); // Чтобы выключить автоблокировку....
  bool right = false;
  if(sp.query_for_authorization)
  {
    authorizationForm *af = new authorizationForm();
    af->InitUserList(true, index);
    
    releaseKeyboard();
    if(af->exec() == QDialog::Accepted)
    {
      right = true;
      afr_user = (char *) calloc(strlen(user_name) + 1, sizeof(char));
      strcpy(afr_user, user_name);
      free(user_name);
      afr_uid = uid;
      af->GetNewUser(user_name, &uid);
printf("user_name = <%s>\n", user_name);
    // operTextLabel->setText(DbCodec->toUnicode(user_name));
      auforization_request_flag = true;
    }
    grabKeyboard();
    delete(af);
  }
  if(sp.autoblock_flag) bt->Start();
  return right;
}
*/
