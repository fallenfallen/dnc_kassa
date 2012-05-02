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
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "accessrights.h"
#include "addon_func.h"

#define RIGHT0 "intcode"
#define RIGHT1 "barcode_scaner"
#define RIGHT2 "barcode_manual"
#define RIGHT3 "visual_search"
#define RIGHT4 "storno"
#define RIGHT5 "quant_reduction"
#define RIGHT6 "price_reduction"
#define RIGHT7 "repeate_pos"
#define RIGHT8 "cancel_check"
#define RIGHT9 "red_manual"
#define RIGHT10 "red_fixed"
#define RIGHT11 "red_cancel_auto"
#define RIGHT12 "red_cancel_other"
#define RIGHT13 "return_by_number"
#define RIGHT14 "open_return_check"
#define RIGHT15 "payment_nal"
#define RIGHT16 "payment_type2"
#define RIGHT17 "payment_type3"
#define RIGHT18 "payment_type4"
#define RIGHT19 "service_report_z"
#define RIGHT20 "service_report_x"
#define RIGHT21 "service_sync_time"
#define RIGHT22 "entering"
#define RIGHT23 "getting"
#define RIGHT24 "copy_check"
#define RIGHT25 "open_box"
#define RIGHT26 "setup"
#define RIGHT27 "handle_unload"
#define RIGHT28 "handle_upload"
#define RIGHT29 "ps_reconsiliation"

//==================================================================================//
AccessRights::AccessRights()
{
    //puts("AccessRights:: AccessRights()");
    Set_Db("LinCash");
}

//==================================================================================//
AccessRights::AccessRights(const char * conf_path) : PGSQL(conf_path)
{
    //printf("AccessRights::AccessRights - Connect to db parameters form path = %s \n", conf_path);
    log->PrintString(log_type, INFO, "AccessRights::AccessRights - Connect to db parameters form path = %s \n", conf_path);
}

//==================================================================================//
AccessRights::AccessRights(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, char* log_path) : PGSQL(dbname, user, password, host, hostaddr, port, log_path)
{
    //log->PrintString(log_type, INFO, "AccessRights::AccessRights - Connect to db with  parameters : dbname = <%s>, user = <%s>, password = <%s>, host = <%s>, hostaddr = <%s>, port = <%s>, log_path = <%s>)\n", dbname, user, password, host, hostaddr, port, log_path);  
    printf("AccessRights::AccessRights(dbname = <%s>, user = <%s>, password = <%s>, host = <%s>, hostaddr = <%s>, port = <%s>, log_path = <%s>)\n", dbname, user, password, host, hostaddr, port, log_path);
}

//==================================================================================//
 AccessRights::~AccessRights() 
{
//puts("AccessRights::~AccessRights()");
}
/////////////////////*Access Rights AuthorizationSection*//////////////////////////
 
//==================================================================================//
int AccessRights::Authorize_by_Password(int uid, const char *pass)
{
//puts("int AccessRights::Authorize_by_Password(int user_id, const char *pass)");
  char *user_id = NULL;
  user_id = itoa(uid);
  int ret = Query_User_with_Password(user_id, pass);
  if ( user_id != NULL ) free(user_id);
  if ( ret != 2 ) return -1;
  if ( PQntuples(result) != 1 ) return 1;
  return 0;
} 

//==================================================================================//
int AccessRights::Authorize_by_Barcode(const char *barcode, int &uid)
{
//puts("int AccessRights::Authorize_by_Barcode(const char *barcode, int &uid)");
  if((Query_User_by_Barcode(barcode)) != 2) return -1;
  if(PQntuples(result) != 1) return 1;
  //printf("");   
  uid = atoi(PQgetvalue(result, 0, 0));
  return 0;
} 

//==================================================================================//
int AccessRights::Authorize_by_Cardcode(const char *cardcode, int &uid)
{
//puts("int AccessRights::Authorize_by_Cardcode(const char *cardcode, int &uid)");
  if((Query_User_by_Cardcode(cardcode)) != 2) return -1;
  if(PQntuples(result) != 1) return 1;
  uid = atoi(PQgetvalue(result, 0, 0));
  return 0;
} 

#define QUERY_USER_WITH_PASSWORD "SELECT user_id FROM Users WHERE (user_id = "
#define QUERY_USER_WITH_PASSWORD_PASS ") AND (userpassword "
//==================================================================================//
int AccessRights::Query_User_with_Password(const char * user_id, const char *pass)
{
//puts("int AccessRights::Query_User_with_Password(const char * user_id, const char *pass)");
  char * str;
  int state;   

  str = (char *) calloc(strlen(QUERY_USER_WITH_PASSWORD) + strlen(QUERY_USER_WITH_PASSWORD_PASS) + strlen(user_id) + strlen(pass) + 35, sizeof(char));
if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
 
  strcpy(str, QUERY_USER_WITH_PASSWORD);
  strcat(str, user_id);
  strcat(str, QUERY_USER_WITH_PASSWORD_PASS);
	if (strlen(pass) == 0)	strcat(str, "is null OR userpassword = ''");
	else {  strcat(str, " = '"); strcat(str, pass); strcat(str, "'"); }
  strcat(str, ")");
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}

#define QUERY_USER_WITH_PASSWORD_BAR "SELECT user_id FROM Users WHERE bar_code "
//==================================================================================//
int AccessRights::Query_User_by_Barcode(const char *barcode)
{
//puts("int AccessRights::Query_User_by_Barcode(char *barcode)");
  char * str;
  int state;   

  str = (char *) calloc(strlen(QUERY_USER_WITH_PASSWORD_BAR) + strlen(barcode) + 35, sizeof(char));
if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
 
  strcpy(str, QUERY_USER_WITH_PASSWORD_BAR);
	if (strlen(barcode) == 0)	strcat(str, " is null OR bar_code = ''");
	else {  strcat(str, " = '"); strcat(str, barcode); strcat(str, "'"); }
  
  state = doSQL(myconnection, str);
 
  //printf("str = %s \n", str);
  free(str);
  return state; 
}

#define QUERY_USER_WITH_PASSWORD_CARD "SELECT user_id FROM Users WHERE card_code "
//==================================================================================//
int AccessRights::Query_User_by_Cardcode(const char *cardcode)
{
//puts("int AccessRights::Query_User_by_Cardcode(char *cardcode)");
  char * str;
  int state;   

  str = (char *) calloc(strlen(QUERY_USER_WITH_PASSWORD_CARD) + strlen(cardcode) + 35, sizeof(char));
if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
 
  strcpy(str, QUERY_USER_WITH_PASSWORD_CARD);
		if (strlen(cardcode) == 0)	strcat(str, "is null OR card_code = ''");
		else {  strcat(str, "= '"); strcat(str, cardcode); strcat(str, "'"); }
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}

//==================================================================================//
int AccessRights::Get_Users_Info_with_Rights(char **& user, char **& profile, int *&ident, int * right_index, int num_allied_rights)
{    
    char **rights = (char **) calloc(num_allied_rights , sizeof(char *));
    for ( int i = 0; i < num_allied_rights; i++ )  {
	*(rights + i) = (char * ) calloc(strlen(Get_Right(*(right_index + i))) + 1, sizeof(char));
	strcat(*(rights + i), Get_Right(*(right_index + i)));
    }

    if ( Query_Users_with_Rights(rights, num_allied_rights) != 2 )  {
	for(int i = 0; i < num_allied_rights; i++)     free(*(rights + i));
	free(rights);
	return -1;
    }
    
    int n = PQntuples(result);
    if ( n == 0 )  {
	PQclear(result);
	
	if( CheckInsertAdminUser() < 0 )    {
	    for(int i = 0; i < num_allied_rights; i++)        free(*(rights + i));
	    free(rights);
	    return -1;
	}
	if ( Query_Users_with_Rights(rights, num_allied_rights) != 2 )    {
	    for(int i = 0; i < num_allied_rights; i++)        free(*(rights + i));
	    free(rights);
	    return -1;
	}
	n = PQntuples(result);
    }
    
    for(int i = 0; i < num_allied_rights; i++)	free(*(rights + i));
    free(rights);
    
    user = (char **) malloc(n * sizeof(char *));
    profile = (char **) malloc(n * sizeof(char *));
    ident = (int *) calloc(n , sizeof(int));
    
    for(int i = 0; i < n; i ++)  {
	*(user + i) = (char *) calloc(PQgetlength(result, i, 0) + 1, sizeof(char));
	strcpy(*(user + i), PQgetvalue(result, i, 0));
	//printf("%d user = %s ",i, user[i]);
	*(profile + i) = (char *) calloc(PQgetlength(result, i, 1) + 1, sizeof(char));
	strcpy(*(profile + i), PQgetvalue(result, i, 1));
	*(ident + i) = atoi(PQgetvalue(result, i, 2));
	//printf(" profile = %s \n", profile);
    }  
    PQclear(result);
    return n;		
}

//==================================================================================//
const char * AccessRights::Get_Right(int i)
{
//printf("const char * AccessRights::Get_Right(int i=%d )\n", i);
  switch(i)
  {
    case 0 : return RIGHT0;
    case 1 : return RIGHT1;
    case 2 : return RIGHT2;
    case 3 : return RIGHT3;
    case 4 : return RIGHT4;
    case 5 : return RIGHT5;
    case 6 : return RIGHT6;
    case 7 : return RIGHT7;
    case 8 : return RIGHT8;
    case 9 : return RIGHT9;
    case 10 : return RIGHT10;
    case 11 : return RIGHT11;
    case 12 : return RIGHT12;
    case 13 : return RIGHT13;
    case 14 : return RIGHT14;
    case 15 : return RIGHT15;
    case 16 : return RIGHT16;
    case 17 : return RIGHT17;
    case 18 : return RIGHT18;
    case 19 : return RIGHT19;
    case 20 : return RIGHT20;
    case 21 : return RIGHT21;
    case 22 : return RIGHT22;
    case 23 : return RIGHT23;
    case 24 : return RIGHT24;
    case 25 : return RIGHT25;
    case 26 : return RIGHT26;	
    case 27 : return RIGHT27;		
    case 28 : return RIGHT28;		
    case 29 : return RIGHT29;
  }
  return "";
}

#define QUERY_USERS_INFO_WITH_RIGHT "SELECT user_name, profile_name, user_id FROM Users as u, Profiles as p WHERE (u.profile_id = p.profile_id) AND (p.profile_id IN (SELECT profile_id FROM Profiles WHERE ("
#define QUERY_USERS_INFO_WITH_RIGHT_VAL " = 't')"
#define QUERY_USERS_INFO_WITH_RIGHT_AND " AND ("
#define QUERY_USERS_INFO_WITH_RIGHT_ORDER ")) ORDER BY u.user_id"
//==================================================================================//
int AccessRights::Query_Users_with_Rights(char **rights, int num_rights)
{
//puts("int AccessRights::Query_Users_with_Rights(char **rights)");
  char * str;
  int state;   

  int len = 0;
  for(int i = 0; i < num_rights; i ++)
    len += strlen(*(rights + i)) + strlen(QUERY_USERS_INFO_WITH_RIGHT_VAL);
  len += (num_rights - 1) * strlen(QUERY_USERS_INFO_WITH_RIGHT_AND);
  
  str = (char *) calloc(strlen(QUERY_USERS_INFO_WITH_RIGHT) + strlen(QUERY_USERS_INFO_WITH_RIGHT_ORDER) + len + 1 , sizeof(char));
if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
 
  strcpy(str, QUERY_USERS_INFO_WITH_RIGHT);

  if(num_rights != 1)
    for(int i = 0; i < num_rights - 1; i ++)
    {
      strcat(str, *(rights + i));
      strcat(str, QUERY_USERS_INFO_WITH_RIGHT_VAL);
      strcat(str, QUERY_USERS_INFO_WITH_RIGHT_AND);
    }
  strcat(str, *(rights + num_rights - 1));
  strcat(str, QUERY_USERS_INFO_WITH_RIGHT_VAL);
  
  strcat(str, QUERY_USERS_INFO_WITH_RIGHT_ORDER);
  //printf("SQL str = %s \n", str);
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}
/////////////////////////////////////////////////////////////////////*Access Rights Registration Section*////////////////////////////////////////////////////////////////////////////////////// 
 
 //==================================================================================//
  // return :
 // 0 - все ОК
 // 1 - нет такого пользователя, запрос не вернул 1 строку
 // -1 - не удалось выполнить запрос в БД
int AccessRights::Get_User_Rights(int uid, char *&user, bool *rights)
{
    
    char *user_id = NULL;
    if ( uid == -1) return 1;
    user_id = NULL; 
    user_id = itoa(uid);
    
    int ret = Query_User_Rights(user_id); 
    if(user_id != NULL ) free(user_id);
    
    if(ret != 2) return -1;
    if(PQntuples(result) != 1) return 1;
    for(int i = 0; i < count_rg; i++)  {
	
	if(!strcmp(PQgetvalue(result, 0, i),"t")) {	
	    *(rights + i) = true;    
	}   else { 
	    *(rights +i) = false; 
	}
    }
    user = (char *) calloc(PQgetlength(result, 0, count_rg) + 1, sizeof(char));
    strcpy(user, PQgetvalue(result, 0, count_rg));
    
    PQclear(result);

    return 0;
}

#define QUERY_USER_RIGHTS "SELECT intcode, barcode_scaner, barcode_manual, visual_search, storno, quant_reduction, price_reduction, repeate_pos, cancel_check, red_manual, red_fixed, red_cancel_auto, red_cancel_other, return_by_number, open_return_check, payment_nal, payment_type2, payment_type3, payment_type4, service_report_z, service_report_x, service_sync_time, entering, getting, copy_check, open_box, setup, handle_unload, handle_upload, ps_reconsiliation, user_name, shortcut FROM Profiles as p, Users as u WHERE (p.profile_id = u.profile_id) AND (p.profile_id = (SELECT profile_id FROM Users WHERE user_id = "
#define QUERY_USER_RIGHTS_UNIQ_USER ")) AND (u.user_id = "
//==================================================================================//
int AccessRights::Query_User_Rights(char *user_id) 
{
//puts("int AccessRights::Query_User_Rights(char *user_id)");
  char * str;
  int state;
  str = (char *) calloc(strlen(QUERY_USER_RIGHTS) + strlen(QUERY_USER_RIGHTS_UNIQ_USER) + 2 * strlen(user_id) + 2, sizeof(char));
if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_USER_RIGHTS);
  strcat(str, user_id);
  strcat(str, QUERY_USER_RIGHTS_UNIQ_USER);
  strcat(str, user_id);
  strcat(str, ")");
//printf("<%s>\n",str);
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}

///////////////////////////////////////////////////////////////////////*Access Rights Setup Section*//////////////////////////////////////////////////////////////////////////////////////////////
#define GET_ADMIN_PROFILES_INFO "SELECT profile_id FROM profiles WHERE profile_id = 1 AND profile_name = 'Администратор' AND intcode = 't' AND barcode_scaner = 't' AND barcode_manual = 't' AND visual_search = 't' AND storno = 't' AND quant_reduction = 't' AND price_reduction = 't' AND repeate_pos = 't' AND cancel_check = 't' AND red_manual = 't' AND red_fixed = 't' AND red_cancel_auto = 't' AND red_cancel_other = 't' AND return_by_number = 't' AND open_return_check = 't' AND payment_nal = 't' AND payment_type2 = 't' AND payment_type3 = 't' AND payment_type4 = 't' AND service_report_z = 't' AND service_report_x = 't' AND service_sync_time = 't' AND entering = 't' AND getting = 't' AND copy_check = 't' AND open_box = 't' AND setup = 't' AND handle_unload = 't' AND handle_upload = 't' AND ps_reconsiliation = 't'"
//==================================================================================//
/*-1 - ошибка выполнения запроса
0- такой пользователь есть и он правильный
1 - нужен UPDATE admin
2 - нужен INSERT admin*/
int AccessRights::Get_Admin_Profile_Info()
{
//puts("int AccessRights::Get_Admin_Profile_Info()");
  char * str;
  int state;
  int len = strlen(GET_ADMIN_PROFILES_INFO) + 1;   
  str = (char *) calloc(len, sizeof(char));
  strcat(str, GET_ADMIN_PROFILES_INFO);

  state = doSQL(myconnection, str);
  free(str);
  
  if (state != 2) return -1;
  int n = PQntuples(result);
  PQclear(result);
  if (n == 1) return 0;
  if (n == 0) {
     state = doSQL(myconnection, "SELECT profile_id FROM profiles WHERE profile_id = 1");
     if (state != 2) return -1;
     int n2 = PQntuples(result);
     PQclear(result);
     if (n2 == 1) return 1;
     if (n2 == 0) return 2;
  }
  return -1;   
}

#define GET_ADMIN_USER_INFO "SELECT user_id FROM users WHERE user_id = 1 AND user_name = 'Администратор' AND profile_id = 1"

//==================================================================================//
/*-1 - ошибка выполнения запроса
0- такой пользователь есть и он правильный
1 - нужен UPDATE admin
2 - нужен INSERT admin*/
int AccessRights::Get_Admin_User_Info()
{
//puts("int AccessRights::Get_Admin_User_Info()");
  char * str;
  int state;   
  str = (char *) calloc(strlen(GET_ADMIN_USER_INFO)  + 1, sizeof(char));
  if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcat(str, GET_ADMIN_USER_INFO);
  
  state = doSQL(myconnection, str);
  free(str);
  //printf("state = %d \n", state);
  if (state != 2) return -1;
  int n = PQntuples(result);
  PQclear(result);
  if (n == 1) return 0;
  if (n == 0) {
     state = doSQL(myconnection, "SELECT user_id FROM users WHERE user_id = 1");
     if (state != 2) return -1;
     int n2 = PQntuples(result);
     PQclear(result);
     if (n2 == 1) return 1;
     if (n2 == 0) return 2;
  }
  return -1;   
}

//==================================================================================//

int AccessRights::Get_Profiles_Info(char **& profile, int *&ident)
{
//puts("int AccessRights::Get_Profiles_Info(char ** profile, int *&ident)");

  if(Query_Profiles() != 2) return -1;

  int n = PQntuples(result);
  if(n == 0)
  {
   PQclear(result);
    if ( InsertAdminUser() < 0 ) return -1;
    if ( Query_Profiles() != 2 ) return -1;
    n = PQntuples(result);
  }
  profile = (char **) malloc(n * sizeof(char *));
  ident = (int *) calloc(n , sizeof(int));
  for(int i = 0; i < n; i ++)
  {
    *(profile + i) = (char *) calloc(PQgetlength(result, i, 0) + 1, sizeof(char));
    strcpy(*(profile + i), PQgetvalue(result, i, 0));
    //printf("*(profile + i) <%s>\n", *(profile + i));
    *(ident + i) = atoi(PQgetvalue(result, i, 1));
  }
  PQclear(result);
  return n;
}

//==================================================================================//
int AccessRights::Get_Users_Info(char **& user, char **& profile, int *&ident)
{
//puts("int AccessRights::Get_Users_Info(char ** user, char ** profile, int *&ident)");
  if(Query_Users() != 2) return -1;
  int n = PQntuples(result);
  if(n == 0)
  {
    PQclear(result);
    if(InsertAdminUser() < 0) return -1;
    if(Query_Users() != 2) return -1;
    n = PQntuples(result);
  }
  user = (char **) malloc(n * sizeof(char *));
  profile = (char **) malloc(n * sizeof(char *));
  ident = (int *) calloc(n , sizeof(int));
  for(int i = 0; i < n; i ++)
  {
    *(user + i) = (char *) calloc(2*PQgetlength(result, i, 0) + 1, sizeof(char));
    strcpy(*(user + i), PQgetvalue(result, i, 0));
//printf("*(user + %d) <%s>\n", i, *(user + i));
    *(profile + i) = (char *) calloc(2*PQgetlength(result, i, 1) + 1, sizeof(char));
    strcpy(*(profile + i), PQgetvalue(result, i, 1));
//printf("---------*(profile + %d) <%s>\n", i, *(profile + i));

    *(ident + i) = atoi(PQgetvalue(result, i, 2));
//printf("------------------*(ident+%d) = <%d> \n", i, *(ident + i));
  }
  PQclear(result);
//printf("return = %d \n", n);
  return n;
}

//==================================================================================//
int AccessRights::InsertAdminUser()
{
//puts("int AccessRights::InsertAdminUser(char * name, char * profile_name)");
  bool rig[count_rg];
  for (int i = 0; i < count_rg; i++) rig[i] = true;
  if(Insert_Profile("1", "Администратор", rig) != 1) return -1;
  if(Insert_User("1","1","Администратор", "Админ", "", "", "") != 1) return -1;
  return 0;
}
//==================================================================================//
int AccessRights::CheckInsertAdminUser()
{
   int st = Get_Admin_Profile_Info();
   if ( st < 0 ) return -1;
   bool rig[count_rg];
   for (int i = 0; i < count_rg; i++) rig[i] = true;
   if ( st == 1 ) if(Update_Profile("1", "Администратор", rig) != 1) return -1;
   if ( st == 2 ) if(Insert_Profile("1", "Администратор", rig) != 1) return -1;

   st = Get_Admin_User_Info();
   if ( st < 0 ) return -1;
   if ( st == 1 ) if (Update_User("1","1","Администратор", "Админ", "", "", "") != 1) return -1;
   if ( st == 2 ) if (Insert_User("1","1","Администратор", "Админ", "", "", "") != 1) return -1;

   return 0;
}
//==================================================================================//
int AccessRights::Get_Profile(bool *& profile, char*& name, int id)
{
//puts("int AccessRights::Get_Profile(bool *& profile, char*& name, int id)");
  char *id_str = NULL;
  id_str = itoa(id);
  if(id_str == NULL) log->PrintString(log_type, ERROR, "in AccessRights::Get_Profile - Error of conversion from int = %d to char* = NULL \n", id);
      
  int ret = Query_Profile(id_str);
  if(id_str) free(id_str);
  if(ret != 2) return -1;
  
  profile = (bool *) calloc(count_rg, sizeof(bool));
  for(int i =0; i < count_rg; i ++)
  {
    if(!strcmp(PQgetvalue(result, 0, i), "t"))
      profile[i] = true;
    else
      profile[i] = false;
  }
  name = (char *) calloc(PQgetlength(result, 0, count_rg) + 1, sizeof(char));
  strcpy(name, PQgetvalue(result, 0, count_rg));
  PQclear(result);
  return 0;
}

//ВОЗВРАЩАЕТ 0, ЕСЛИ НЕ НАШЕЛ ТАКОГО ИМЕНИ, -1 ЕСЛИ ВОЗНИКЛИ ОШИБКИ ПРИ ЗАПРОСЕ В БД, >0 НАШЕЛ ИМЯ, ВОЗВРАЩАЕТ ID
//==================================================================================//
int AccessRights::Get_Profile_Id_by_Name(char *prof_name)
{
//puts("int AccessRights::Get_Profile_Id_by_Name(char *prof_name)");
  if(Query_Profile_Id_by_Name(prof_name) == 2)
  {
    int n = PQntuples(result);
    if(n != 0)
    {
      int id = atoi(PQgetvalue(result, 0, 0));
      PQclear(result);
      return id;
    }
    else return 0;
  }
  return -1;
}

//ВОЗВРАЩАЕТ 0, ЕСЛИ НЕ НАШЕЛ ТАКОГО ИМЕНИ, -1 ЕСЛИ ВОЗНИКЛИ ОШИБКИ ПРИ ЗАПРОСЕ В БД, >0 НАШЕЛ ИМЯ, ВОЗВРАЩАЕТ ID
//==================================================================================//
int AccessRights::Get_User_Id_by_Name(char *user_name)
{
//puts("int AccessRights::Get_User_Id_by_Name(char *user_name)");
  if(Query_User_Id_by_Name(user_name) == 2)
  {
    int n = PQntuples(result);
    if(n != 0)
    {
      int id = atoi(PQgetvalue(result, 0, 0));
      PQclear(result);
      return id;
    }
    else return 0;
  }
  return -1;
}

//==================================================================================//
int AccessRights::Update_Profile_Info(int id, char *prof_name, bool *rights)
{
    char *id_str = NULL;
    id_str = itoa(id);
    if ( id_str == NULL ) log->PrintString(log_type, ERROR, "in AccessRights::Update_Profile_Info - Error of conversion from int = %d to char* = NULL \n", id);
    int ret = Update_Profile(id_str, prof_name, rights);
    if ( id_str ) free(id_str);
    if( ret != 1 ) return -1;
    return 0;
}

//==================================================================================//
int AccessRights::Update_User_Info(int id, char *user_name, char *shortcut, int profile, const char* bar_code, const char* card_code, const char* upassword)
{
//puts("int AccessRights::Update_User_Info(int id, char *user_name, char *shortcut, int profile, char* bar_code, char* card_code, char* upassword)");
  char *uid_str = NULL;
  uid_str = itoa(id);
  if(uid_str == NULL) log->PrintString(log_type, ERROR, "in AccessRights::Update_User_Info - Error of conversion from int = %d to char* = NULL \n", id);
	
  char *pid_str = NULL;
  pid_str = itoa(profile);
  if(pid_str == NULL) log->PrintString(log_type, ERROR, "in AccessRights::Update_User_Info - Error of conversion from int = %d to char* = NULL \n", profile);
                                  
  int ret = Update_User(uid_str, user_name, shortcut, pid_str, bar_code, card_code, upassword);
  if(uid_str) free(uid_str);
  if(pid_str) free(pid_str);
  if(ret != 1) return -1;
  return 0; 
}

//==================================================================================//
int AccessRights::Add_New_Profile(int &id, char *prof_name, bool *rights)
{
//puts("int AccessRights::Add_New_Profile(int *id, char *prof_name, bool *rights)");
//ВСТАВИТЬ ТРАНЗАКЦИЮ

  if(Query_Max_Profile_Id() != 2) return -1;

  id = atoi(PQgetvalue(result, 0, 0)) + 1;
  PQclear(result);

  char *id_str = NULL;
  id_str = itoa(id);
  if (id_str == NULL) log->PrintString(log_type, ERROR, "in AccessRights::Add_New_Profile - Error of conversion from int = %d to char* = NULL \n", id);

  int ret = Insert_Profile(id_str, prof_name, rights);

  if(id_str) free(id_str);
  if(ret != 1) return -1;
  return 0;
}

//==================================================================================//
int AccessRights::Add_New_User(int &user_id, char *user_name, char *shortcut, int profile_id,  char *bar_code,  char *card_code, char *upassword)
{
//puts("int AccessRights::Add_New_User(int &user_id, char *user_name, char *shortcut, int profile_id,  char *bar_code,  char *card_code, char *upassword)");
 //ВСТАВИТЬ ТРАНЗАКЦИЮ
  if(Query_Max_User_Id() != 2) return -1;
  user_id = atoi(PQgetvalue(result, 0, 0)) + 1;
  PQclear(result);
  char *uid_str = NULL;
  uid_str = itoa(user_id);
  if(uid_str == NULL) log->PrintString(log_type, ERROR, "in AccessRights::Add_New_User - Error of conversion from int = %d to char* = NULL \n", user_id);

  char *pid_str = NULL;
  pid_str = itoa(profile_id);
  if(pid_str == NULL) log->PrintString(log_type, ERROR, "in AccessRights::Add_New_User - Error of conversion from int = %d to char* = NULL \n", profile_id);
  
  int ret = Insert_User(uid_str, pid_str, user_name, shortcut, bar_code, card_code, upassword);
  if(uid_str) free(uid_str);  
  if(pid_str) free(pid_str);
  if(ret != 1) return -1;
  return 0;
}

//==================================================================================//
int AccessRights::Delete_Profile(int id)
{
//puts("int AccessRights::Delete_Profile(int id)");
  char *id_str = NULL;
  id_str = itoa(id);
  if(id_str == NULL) log->PrintString(log_type, ERROR, "in AccessRights::Delete_Profile - Error of conversion from int = %d to char* = NULL \n", id);
  if(Query_Number_Users_with_Profile(id_str) != 2)
  {
    if(id_str) free(id_str);
    return -1;
  }
  int n = atoi(PQgetvalue(result, 0, 0));
  if(n != 0) return n;
  int ret = Delete_Profile(id_str);
  if(id_str) free(id_str);
  if(ret != 1) return -1;
  return 0;
}

//==================================================================================//
int AccessRights::Delete_User(int id)
{
//puts("int AccessRights::Delete_User(int id)");
  char *id_str = NULL;
  id_str = itoa(id);
  if(id_str == NULL)  log->PrintString(log_type, ERROR, "in AccessRights::Delete_User - Error of conversion from int = %d to char* = NULL \n", id);
  
  int ret = Delete_User(id_str);
  if(id_str) free(id_str);
  if(ret != 1) return -1;
  return 0;
}


//==================================================================================//
int AccessRights::Get_User(int id, char* &pr_name, char *&name, char *&shortcut, char *&barcode, char *&cardcode, char *&userpass)
{
//puts("int AccessRights::Get_User(int id, int * profile_id, char *&name, char *&shortcut, char *&barcode, char *&cardcode, char *&userpass)");
  char *id_str = NULL;
  id_str = itoa(id);
  if(id_str == NULL) log->PrintString(log_type, ERROR, "in AccessRights::Get_User - Error of conversion from int = %d to char* = NULL \n", id);
   
  int ret = Query_User(id_str);
  if(id_str) free(id_str);
  if(ret != 2)  return -1;

  //user_name, shortcut, profile_id, bar_code,  card_code, userpassword
  name = (char *) calloc(PQgetlength(result, 0, 0) + 1, sizeof(char));
  strcpy(name, PQgetvalue(result, 0, 0));
  shortcut = (char *) calloc(PQgetlength(result, 0, 1) + 1, sizeof(char));
  strcpy(shortcut, PQgetvalue(result, 0, 1));
  pr_name = (char *) calloc(PQgetlength(result, 0, 2) + 1, sizeof(char));
  strcpy(pr_name, PQgetvalue(result, 0, 2));
  barcode = (char *) calloc(PQgetlength(result, 0, 3) + 1, sizeof(char));
  strcpy(barcode, PQgetvalue(result, 0, 3));
  cardcode = (char *) calloc(PQgetlength(result, 0, 4) + 1, sizeof(char));
  strcpy(cardcode, PQgetvalue(result, 0, 4));
  userpass = (char *) calloc(PQgetlength(result, 0, 5) + 1, sizeof(char));
  strcpy(userpass, PQgetvalue(result, 0, 5));
  PQclear(result);
  return 0;
}

/*
//==================================================================================//
int AccessRights::Get_User(int id, int *profile_id, char *&name, char *&shortcut, char *&barcode, char *&cardcode, char *&userpass)
{
puts("int AccessRights::Get_User(int id, int * profile_id, char *&name, char *&shortcut, char *&barcode, char *&cardcode, char *&userpass)");
  char *id_str = NULL;
  id_str = itoa(id);
  if(id_str == NULL) printf("NULL CALLOC!!! parameter id_str\n");

  int ret = Query_User(id_str);
  if(id_str) free(id_str);
  if(ret != 2)
    return -1;

  //user_name, shortcut, profile_id, bar_code,  card_code, userpassword
  name = (char *) calloc(PQgetlength(result, 0, 0) + 1, sizeof(char));
  strcpy(name, PQgetvalue(result, 0, 0));
  shortcut = (char *) calloc(PQgetlength(result, 0, 1) + 1, sizeof(char));
  strcpy(shortcut, PQgetvalue(result, 0, 1));
  *profile_id = atoi(PQgetvalue(result, 0, 2));
  barcode = (char *) calloc(PQgetlength(result, 0, 3) + 1, sizeof(char));
  strcpy(barcode, PQgetvalue(result, 0, 3));
  cardcode = (char *) calloc(PQgetlength(result, 0, 4) + 1, sizeof(char));
  strcpy(cardcode, PQgetvalue(result, 0, 4));
  userpass = (char *) calloc(PQgetlength(result, 0, 5) + 1, sizeof(char));
  strcpy(userpass, PQgetvalue(result, 0, 5));
  PQclear(result);
  return 0;
}*/

#define QUERY_MAX_PROFILE_ID "SELECT MAX(profile_id) FROM Profiles" 
//==================================================================================//
int AccessRights::Query_Max_Profile_Id()
{
//puts("int AccessRights::Query_Max_Profile_Id()");

  char * str;
  int state;   

  str = (char *) calloc(strlen(QUERY_MAX_PROFILE_ID) + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_MAX_PROFILE_ID);
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}

#define QUERY_MAX_USER_ID "SELECT MAX(user_id) FROM Users"
//==================================================================================//
int AccessRights::Query_Max_User_Id()
{
//puts("int AccessRights::Query_Max_User_Id()");

  char * str;
  int state;   

  str = (char *) calloc(strlen(QUERY_MAX_USER_ID) + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_MAX_USER_ID);
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}

#define QUERY_PROFILE_ID_BY_NAME "SELECT profile_id FROM Profiles WHERE profile_name LIKE '"
//==================================================================================//
int AccessRights::Query_Profile_Id_by_Name(char * name)
{
//puts("int AccessRights::Query_Profile_Id_by_Name(char * name)");

  char * str;
  int state;   

  str = (char *) calloc(strlen(QUERY_PROFILE_ID_BY_NAME) + strlen(name) + 2, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_PROFILE_ID_BY_NAME);
  strcat(str, name);
  strcat(str, "'");
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}


#define QUERY_USER_ID_BY_NAME "SELECT user_id FROM Users WHERE user_name LIKE '"
//==================================================================================//
int AccessRights::Query_User_Id_by_Name(char * name)
{
//puts("int AccessRights::Query_User_Id_by_Name(char * name)");

  char * str;
  int state;   

  str = (char *) calloc(strlen(QUERY_USER_ID_BY_NAME) + strlen(name) + 2, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_USER_ID_BY_NAME);
  strcat(str, name);
  strcat(str, "'");
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}

#define QUERY_USER "SELECT u.user_name, u.shortcut, p.profile_name, u.bar_code,  u.card_code, u.userpassword FROM Users u, Profiles p WHERE u.profile_id = p.profile_id AND u.user_id = "
/*#define QUERY_USER "SELECT user_name, shortcut, profile_id, bar_code,  card_code, userpassword FROM Users WHERE user_id = "*/
//==================================================================================//
int AccessRights::Query_User(char * id)
{
//printf("int AccessRights::Query_User(char * id = %s)\n", id);

  char * str;
  int state;   

  str = (char *) calloc(strlen(QUERY_USER) + strlen(id) + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_USER);
  strcat(str, id);
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}

#define QUERY_PROFILE "SELECT intcode, barcode_scaner, barcode_manual, visual_search, storno, quant_reduction, price_reduction, repeate_pos, cancel_check, red_manual, red_fixed, red_cancel_auto, red_cancel_other, return_by_number, open_return_check, payment_nal, payment_type2, payment_type3, payment_type4, service_report_z, service_report_x, service_sync_time, entering, getting, copy_check, open_box, setup, handle_unload, handle_upload, ps_reconsiliation, profile_name FROM Profiles WHERE profile_id = "
//==================================================================================//
int AccessRights::Query_Profile(char * id)
{
//puts("int AccessRights::Query_Profile(char * id)");

  char * str;
  int state;   

  str = (char *) calloc(strlen(QUERY_PROFILE) + strlen(id) + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_PROFILE);
  strcat(str, id);
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}

#define QUERY_USERS_INFO "SELECT user_name, profile_name, user_id FROM Users as u, Profiles as p WHERE u.profile_id = p.profile_id ORDER BY u.user_id"
//==================================================================================//
int AccessRights::Query_Users()
{
//puts("int AccessRights::Query_Users()");

  char * str;
  int state; 

  str = (char *) calloc(strlen(QUERY_USERS_INFO) + 1, sizeof(char));
  if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_USERS_INFO);

  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}

#define QUERY_PROFILES_INFO "SELECT profile_name, profile_id FROM Profiles ORDER BY profile_id"
//==================================================================================//
int AccessRights::Query_Profiles()
{
//puts("int AccessRights::Query_Profiles()");

  char * str;
  int state;   

  str = (char *) calloc(strlen(QUERY_PROFILES_INFO) + 1, sizeof(char));
  if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_PROFILES_INFO);
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}

#define QUERY_NUM_USERS_WITH_PROFILE "SELECT COUNT(user_id) FROM Users WHERE profile_id = "
//==================================================================================//
int AccessRights::Query_Number_Users_with_Profile(char *pr_id)
{
//puts("int AccessRights::Query_Number_Users_with_Profile(char *pr_id)");

  char * str;
  int state;   

  str = (char *) calloc(strlen(QUERY_NUM_USERS_WITH_PROFILE) + strlen(pr_id) + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_NUM_USERS_WITH_PROFILE);
  strcat(str, pr_id);
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}

#define DELETE_PROFILE "DELETE FROM Profiles WHERE profile_id = "
//==================================================================================//
int AccessRights::Delete_Profile(char * id)
{
//puts("int AccessRights::Delete_Profile(char * id)");

  char * str;
  int state;   

  str = (char *) calloc(strlen(DELETE_PROFILE) + strlen(id) + 1, sizeof(char));
  if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, DELETE_PROFILE);
  strcat(str, id);
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}

#define DELETE_USER "DELETE FROM Users WHERE user_id = "
//==================================================================================//
int AccessRights::Delete_User(char *id)
{
//puts("int AccessRights::Delete_User(char *id)");
  char * str;
  int state;   

  str = (char *) calloc(strlen(DELETE_USER) + strlen(id) + 1, sizeof(char));
  if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, DELETE_USER);
  strcat(str, id);
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}
/*
#define INSERT_ADMIN_USER "INSERT INTO Users(user_id, user_name, shortcut, profile_id, userpassword) VALUES(1, 'Администратор', 'Админ', 1, '30')"
//==================================================================================//
int AccessRights::Insert_Admin_User()
{
puts("int AccessRights::Insert_Admin_User()");

  char * str;
  int state;   

  str = (char *) calloc(strlen(INSERT_ADMIN_USER) + 1, sizeof(char));
  if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, INSERT_ADMIN_USER);
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}
#define INSERT_ADMIN_PROFILE "INSERT INTO Profiles(profile_id, profile_name, intcode, barcode_scaner, barcode_manual, visual_search, storno, quant_reduction, price_reduction, repeate_pos, cancel_check, red_manual, red_fixed, red_cancel_auto, red_cancel_other, return_by_number, open_return_check, payment_nal, payment_type2, payment_type3, payment_type4, service_report_z, service_report_x, service_sync_time, entering, getting, copy_check, open_box, setup) VALUES(1,'Администратор', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't', 't')" 
//==================================================================================//
int AccessRights::Insert_Admin_Profile()
{
puts("int AccessRights::Insert_Admin_Profile()");
puts("------------6666666666666---------------------");
  char * str;
  int state;   

  str = (char *) calloc(strlen(INSERT_ADMIN_PROFILE) + 1, sizeof(char));
  if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, INSERT_ADMIN_PROFILE);
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}*/
//==================================================================================//
#define INSERT_USER "INSERT INTO Users(user_id, user_name, shortcut, profile_id, bar_code, card_code, userpassword) VALUES( "
int AccessRights::Insert_User(const char *uid, const char *pid, const char *name, const char *shortcut, const char *barcode, const char *cardcode, const char *userpass)
{
//puts("int AccessRights::Insert_User(char *uid, char *pid, char *name, char *shortcut, char *barcode, char *cardcode, char *userpass)");
  char * str;
  int state;
  str = (char *) calloc(strlen(INSERT_USER) + strlen(uid) + strlen(pid) + strlen(name) + strlen(shortcut) + strlen(barcode) + strlen(cardcode) + strlen(userpass) + 24, sizeof(char)); 
  if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  
  strcpy(str, INSERT_USER);
  strcat(str, uid);
  strcat(str, ", '");
  strcat(str, name);
  strcat(str, "', '");
  strcat(str, shortcut);
  strcat(str, "', ");
  strcat(str, pid);
  strcat(str, ", '");
  strcat(str, barcode);
  strcat(str, "', '");
  strcat(str, cardcode);
  strcat(str, "', '");
  strcat(str, userpass);
  strcat(str, "')");
  //printf("SQL str = <%s>\n", str);
  state = doSQL(myconnection, str);
  free(str);
  return state;    
}

#define INSERT_PROFILE "INSERT INTO Profiles(profile_id, profile_name, intcode, barcode_scaner, barcode_manual, visual_search, storno, quant_reduction, price_reduction, repeate_pos, cancel_check, red_manual, red_fixed, red_cancel_auto, red_cancel_other, return_by_number, open_return_check, payment_nal, payment_type2, payment_type3, payment_type4, service_report_z, service_report_x, service_sync_time, entering, getting, copy_check, open_box, setup, handle_unload, handle_upload, ps_reconsiliation) VALUES("
//==================================================================================//
int AccessRights::Insert_Profile(const char *id, const char *prof_name, bool *rights)
{
//puts("int AccessRights::Insert_Profile(char *id, char *prof_name, bool *rights)");
  char * str;
  int state;
  str = (char *) calloc(strlen(INSERT_PROFILE) + strlen(id) + strlen(prof_name)  + count_rg * strlen("FALSE, ") + 55 + 50, sizeof(char));
  //str = (char *) calloc(1000, sizeof(char));  
  if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  
  strcpy(str, INSERT_PROFILE);
  strcat(str, id);
  strcat(str, ", '");
  strcat(str, prof_name);
  strcat(str, "'");
  for(int i = 0; i < count_rg; i ++)
  {
    strcat(str, ", ");
    if(rights[i]) 
      strcat(str, "TRUE");
    else
      strcat(str, "FALSE");
  }
  strcat(str, ")");
  //printf("SQL str = %s \n", str);
  //printf("str len = %d \n", strlen(str));
  //printf("str = %s \n", str);
  state = doSQL(myconnection, str);
  free(str);
  return state;
}

#define UPDATE_PROFILE "UPDATE Profiles SET  profile_name = '"
#define UP_IC "', intcode = "
#define UP_BS ", barcode_scaner = "
#define UP_BM ", barcode_manual = "
#define UP_VS ", visual_search = "
#define UP_S ", storno = "
#define UP_QR ", quant_reduction = "
#define UP_PR ", price_reduction = "
#define UP_RP ", repeate_pos = "
#define UP_CC ", cancel_check = "
#define UP_RM ", red_manual = "
#define UP_RF ", red_fixed = "
#define UP_RCA ", red_cancel_auto = "
#define UP_RCO ", red_cancel_other = "
#define UP_RBN ", return_by_number = "
#define UP_ORC ", open_return_check = "
#define UP_PN ", payment_nal = "
#define UP_PT2 ", payment_type2 = "
#define UP_PT3 ", payment_type3 = "
#define UP_PT4 ", payment_type4 = "
#define UP_SRZ ", service_report_z = "
#define UP_SRX ", service_report_x = "
#define UP_SST ", service_sync_time = "
#define UP_ENT ", entering = "
#define UP_GET ", getting = "
#define UP_COP ", copy_check = "
#define UP_OB ", open_box = "
#define UP_ST ", setup = "
#define UP_HU ", handle_unload = "
#define UP_HUP ", handle_upload = "
#define UP_PSR ", ps_reconsiliation = "
#define UP_WHERE " WHERE profile_id = "
//==================================================================================//
int AccessRights::Update_Profile(const char *id, const char *prof_name, bool *rights)
{
//puts("int AccessRights::Update_Profile(char *id, char *prof_name, bool *rights)");
  char * str;
  int state, i = 0;
  str = (char *) calloc(strlen(UPDATE_PROFILE) + strlen(prof_name) + strlen(UP_IC) + strlen(UP_BS) + strlen(UP_BM) + strlen(UP_VS) + strlen(UP_S) + strlen(UP_QR) + strlen(UP_PR) + strlen(UP_RP) + strlen(UP_CC) + strlen(UP_RM) + strlen(UP_RF) + strlen(UP_RCA) + strlen(UP_RCO) 	+ strlen(UP_RBN) + strlen(UP_ORC) + strlen(UP_PN) + strlen(UP_PT2) + strlen(UP_PT3) + strlen(UP_PT4) + strlen(UP_SRZ) + strlen(UP_SRX) + strlen(UP_SST) + strlen(UP_ENT) + strlen(UP_GET) + strlen(UP_COP) + strlen(UP_OB) + strlen(UP_ST) + strlen(UP_HU) + strlen(UP_HUP) + strlen(UP_PSR) + strlen(UP_WHERE) + strlen(id) + count_rg * strlen("FALSE")  + 1, sizeof(char));
  
  
  if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  
  strcpy(str, UPDATE_PROFILE);
  strcat(str, prof_name);
  strcat(str, UP_IC); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_BS); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_BM); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_VS); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_S); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_QR); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_PR); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_RP); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_CC); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_RM); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_RF); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_RCA); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_RCO); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_RBN); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_ORC); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_PN); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_PT2); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_PT3); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_PT4); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_SRZ); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_SRX); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_SST); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_ENT); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_GET); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_COP); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_OB); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_ST); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_HU); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");
  strcat(str, UP_HUP); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");  
  strcat(str, UP_PSR); if(rights[i++]) strcat(str, "TRUE"); else strcat(str, "FALSE");  
  strcat(str, UP_WHERE);
  strcat(str, id);
  
  state = doSQL(myconnection, str);
  
  free(str);
  return state; 
}

#define UPDATE_USER "UPDATE Users SET user_name = '"
#define UPDATE_USER_SHORTCUT "', shortcut = '"
#define UPDATE_USER_PROFILE "', profile_id = "
#define UPDATE_USER_BARCODE ", bar_code = '"
#define UPDATE_USER_CARDCODE "', card_code = '"
#define UPDATE_USER_PASSWORD "', userpassword = '"
#define UPDATE_USER_WHERE "' WHERE user_id = "
//==================================================================================//
int AccessRights::Update_User(const char *uid_str, const char *user_name, const char *shortcut, const char *pid_str, const char *bar_code, const char *card_code, const char *upassword)
{
//puts("int AccessRights::Update_User(char *uid_str, char *user_name, char *shortcut, char *pid_str, char *bar_code, char *card_code, char *upassword)");
  char *str;
  int state;
  
  str = (char *) calloc(strlen(UPDATE_USER) + strlen(user_name) + strlen(UPDATE_USER_SHORTCUT) + strlen(shortcut) + strlen(UPDATE_USER_PROFILE) + strlen(pid_str) + strlen(UPDATE_USER_BARCODE) + strlen(bar_code) + strlen(UPDATE_USER_CARDCODE) + strlen(card_code) + strlen(UPDATE_USER_PASSWORD) + strlen(upassword) + strlen(UPDATE_USER_WHERE) + strlen(uid_str) + 1, sizeof(char));
  if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  
  strcpy(str, UPDATE_USER);
  strcat(str, user_name);
  strcat(str, UPDATE_USER_SHORTCUT);
  strcat(str, shortcut);
  strcat(str, UPDATE_USER_PROFILE);
  strcat(str, pid_str);
  strcat(str, UPDATE_USER_BARCODE);
  strcat(str, bar_code);
  strcat(str, UPDATE_USER_CARDCODE);
  strcat(str, card_code);
  strcat(str, UPDATE_USER_PASSWORD);
  strcat(str, upassword);
  strcat(str, UPDATE_USER_WHERE);
  strcat(str, uid_str);

 // printf("<%s>\n", str);
  state = doSQL(myconnection, str);
  free(str);
  return state; 
}
//==================================================================================//
/*
1-штрих код уже используется
0-все нормально
-1--3 - ошибка запроса к БД
*/
int AccessRights::Check_Barcode(int user_id, char* barcode) {
    // puts("int AccessRights::Check_Barcode()");
     
     int res = 1;
     
     int state = Query_Check_Ware_Barcode(barcode);
     if ( state != 2 ) return state;
     
     if ( PQntuples(result) !=0 ) return 1;
     PQclear(result);
     
     state = Query_Check_Barcode(barcode);
     if ( state != 2 ) return state;
     
     int n = PQntuples(result);
     if ( n == 0 ) return 0;
     
     if ( (n == 1) && (user_id != -1) ) {
          if ( atoi(PQgetvalue(result, 0, 0)) == user_id ) return 0;
     }
     
     PQclear(result);
     
     return 1;
}
//==================================================================================//
int AccessRights::Check_Cardcode(int user_id, char* cardcode) {
    // puts("int AccessRights::Check_Barcode()");
     
        int state = Query_Check_Cardcode(cardcode);
     
     if ( state != 2 ) return state;
         
     int n = PQntuples(result);
     
     if ( n == 0 ) return 0;
     
     if ( (n == 1) && (user_id != 0) ) {
          if ( atoi(PQgetvalue(result, 0, 0)) == user_id ) return 0;
     }
     PQclear(result);
     return 1;
}

#define CHECK_WARE_BARCODE "SELECT barcode FROM ware_barcode WHERE barcode LIKE '"
//==================================================================================//
int AccessRights::Query_Check_Ware_Barcode(char* barcode){
  //puts("int AccessRights::Query_Check_Ware_Barcode(char* barcode)"); 
  char *str;
  int state;
     
  str = (char*) calloc(strlen(CHECK_WARE_BARCODE) + strlen(barcode) + 3, sizeof(char));
     
  strcpy(str, CHECK_WARE_BARCODE);     
  strcat(str, barcode);
  strcat(str, "'");

  state = doSQL(myconnection, str);
  free(str);
  return state;
}

#define CHECK_CARDCODE "SELECT user_id FROM users WHERE card_code LIKE '"
//==================================================================================//
int AccessRights::Query_Check_Cardcode(char* cardcode){
  //puts("int AccessRights::Query_Check_Cardcode(char* cardcode)"); 
  char *str;
  int state;
     
  str = (char*) calloc(strlen(CHECK_CARDCODE) + strlen(cardcode) + 3, sizeof(char));
     
  strcpy(str, CHECK_CARDCODE);     
  strcat(str, cardcode);
  strcat(str, "'");

  state = doSQL(myconnection, str);
  free(str);
  return state; 
}

#define CHECK_BARCODE "SELECT user_id FROM users WHERE bar_code LIKE '"
//==================================================================================//
int AccessRights::Query_Check_Barcode(char* barcode) {
 // puts("int AccessRights::Query_Check_Barcode(char* barcode)"); 
  char *str;
  int state;
     
  str = (char*) calloc(strlen(CHECK_BARCODE) + strlen(barcode) + 3, sizeof(char));
     
  strcpy(str, CHECK_BARCODE);     
  strcat(str, barcode);
  strcat(str, "'");

  state = doSQL(myconnection, str);
  free(str);
  return state; 
     
}
//==================================================================================//
int AccessRights::GetUsers(int &count, char ** &users_shortcut)
{
    int state = doSQL(myconnection, "SELECT shortcut FROM users");
    if ( state != 2 ) return -1;
    
    count = PQntuples(result);
    if ( count ==  0 ) {
	PQclear(result);
	return 0;    
    }
    
    users_shortcut = (char **) calloc(count, sizeof(char*));
    for ( int i = 0; i < count; i++ ) {
	users_shortcut[i] = (char*) calloc( PQgetlength(result, i, 0) + 1, sizeof(char) );
	strcpy(users_shortcut[i], PQgetvalue(result, i, 0));
    }
    
    PQclear(result);
    return 0;    
}

#define QUERY_GET_SELLER_BY_NUM "SELECT shortcut FROM users WHERE user_id = "
//==================================================================================//
int AccessRights::GetSellerName(const char * seller_num, char* &seller_name)
{	
    char * str = (char*) calloc(strlen(QUERY_GET_SELLER_BY_NUM) + strlen(seller_num) + 1, sizeof(char) );
    
    strcat(str, QUERY_GET_SELLER_BY_NUM);
    strcat(str, seller_num);
    
    int state = doSQL(myconnection, str);
    
    free(str);
    
    if ( state != 2 ) return -1;  
    
    if ( PQntuples(result) != 1 ) { PQclear(result);  return 1;   }
    
    seller_name = (char* ) calloc(PQgetlength(result, 0, 0) + 1, sizeof(char));
    strcpy(seller_name, PQgetvalue(result, 0, 0));
    
    
    PQclear(result);

}   


#define QUERY_GET_USER_BY_NUM "SELECT user_name FROM users WHERE user_id = "
//==================================================================================//
int AccessRights::GetUserName(const char * seller_num, char* &seller_name)
{	
    char * str = (char*) calloc(strlen(QUERY_GET_USER_BY_NUM) + strlen(seller_num) + 1, sizeof(char) );
    
    strcat(str, QUERY_GET_USER_BY_NUM);
    strcat(str, seller_num);
    
    int state = doSQL(myconnection, str);
    
    free(str);
    
    if ( state != 2 ) return -1;  
    
    if ( PQntuples(result) != 1 ) { PQclear(result);  return 1;   }
    
    seller_name = (char* ) calloc(PQgetlength(result, 0, 0) + 1, sizeof(char));
    strcpy(seller_name, PQgetvalue(result, 0, 0));
    
    
    PQclear(result);

}   
