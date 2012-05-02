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

#include "upload_pgsql.h"

//==============================================================================//
PgUpload::PgUpload() : PGSQL()
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "Upload()::Upload() : PGSQL()\n");

}

//==============================================================================//
PgUpload::PgUpload(char * dbname, char * user, char * password, char * host, char * hostaddr, char * port, char * log_path) : PGSQL(dbname, user, password, host, hostaddr, port, log_path)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "Upload()::Upload(char* dbname, char *user, char *password, char *host, char * hostaddr, char *port, char* log_path)\n");

}
//==================================================================================//
PgUpload::PgUpload(const char * conf_path) : PGSQL(conf_path)
{
	//puts("PgUpload::PgUpload(const char * conf_path)");
}

//==============================================================================//
PgUpload::~PgUpload()
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "Upload()::~Upload()");
}
//==============================================================================//
int PgUpload::AddWare(const char *internal_code, const char *bar_code, const char *longtitle, const char *shortcut, const char *price, const char * quantity, const char *auto_scheme_id, const char *flags, const char *min_price, const char *best_before,  const char * coefficient, const char *group_code, const char *tax_group_id)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::AddWare(char *internal_code, char *bar_code, char *longtitle, char *shortcut, char *price, char * quantity, char * auto_scheme_id, char * flags, char * min_price, char * best_before, char * group_code, char* tax_group_id)\n");
  
 // printf("bar_code = <%s> \n", bar_code);   
    
  int res;
  //BeginTransaction();
  if((res = Query_By_Internal_Code(internal_code) ) != 2) {// Запрос не выполнен, ошибка
    //RollbackTransaction();
   log->PrintString(log_type, ERROR, "Add ware by internal code error\n");
    return -1;
    //return res;
  }  else  {
    int n = PQntuples(result);
    PQclear(result);
    switch(n)    {
      case 0 : // Нет товара с таким кодом, INSERT
        res = Insert_Ware(internal_code, longtitle, shortcut, price, quantity, auto_scheme_id,  flags, min_price, best_before, group_code, tax_group_id, false);
        break;
      case 1 : // Есть товар с таким кодом, UPDATE
        res = Update_Ware(internal_code, longtitle, shortcut, price, quantity, auto_scheme_id,  flags, min_price, best_before, group_code, tax_group_id, false, true);
        break;
    }
    if(res != 1)   {
      //RollbackTransaction();
     log->PrintString(log_type, ERROR, "Update ware by intrenal code error \n");
      return -1;
      //return res;
    }
  }

  //puts("-----------111111111-------------------");
  if ( Delete_Barcode_By_Ware(internal_code) < 0 ) {
      //RollbackTransaction();
     log->PrintString(log_type, ERROR, "Delete barcode by ware error \n");
      return -1;
  }

  //puts("-----------2-222222222222------------");
  if ( Add_Barcodes(internal_code, bar_code, coefficient) < 0 ) {
      //RollbackTransaction();
     log->PrintString(log_type, ERROR, "Add barcodes by internal code error \n");
      return -1;	
  }

 // puts("----------------3333333333-------------------");
  //CommitTransaction();
  //if ( res != 1 ) printf("res = %d \n", res);
  //if ( res != 1 && res != 2 ) return res;
  //return 0;
  
  return res;
}
//==============================================================================//
int PgUpload::findsep(char* str, char sep, int start) {
   int len = strlen(str);
   if ( start >= len-1 ) return len;
   int i = start;
   while ( (str[i] != sep) && (i<len) ) i++;
   return i;
}
//==============================================================================//
int PgUpload::Add_Barcodes(const char* internal_id, const char* bar_code, const char * coefficient) {
	// INSERT BARCODE
//printf("PgUpload::Add_Barcodes(internal_id = %s, bar_code = %s)\n", internal_id, bar_code);
    if ( strlen(bar_code) < 1 ) return 0;
    
    char * barcode_tmp = (char*) calloc(strlen(bar_code) + 1, sizeof(char));
    strcpy(barcode_tmp, bar_code);
    
  int i = 0;
  int k = 0;
  int tm = 0;
  int res = 0;
  // i - откуда читать штрих-код
  while ( i < strlen(barcode_tmp) ) {
	k = findsep(barcode_tmp, ',', i);
	//printf("k = %d \n", k);
	char* tmp = (char*) calloc(strlen(barcode_tmp)+3, sizeof(char));
	tm = 0;
	for ( int j=i; j<k; j++ ) {
		if ( barcode_tmp[j] != ' ' ) {
			tmp[tm] = barcode_tmp[j];
			tm++;	}
	}
	//printf("tmp = barcode = <%s> \n", tmp);
	//printf("internal_id = <%s>\n", internal_id);
	res = Insert_Barcode(internal_id, tmp, coefficient); 
	
	i = k+1;
	free(tmp);
  }
  free(barcode_tmp);
  return res;
}
//==============================================================================//
#define INSERT_BARCODE "INSERT INTO ware_barcode (internal_id, barcode, coefficient, main_barcode) VALUES ('"
int PgUpload::Insert_Barcode(const char* internal_id, const char* barcode, const char * coefficient) {
  //printf("int PgUpload::Insert_Barcode(const char* internal_id = <%s> , const char* barcode = <%s> , const char* coeficient = <%s>)\n", internal_id, barcode, coefficient);
    
    if ( strlen(barcode) < 1 ) return 0;
    
  int state;
  char* str;
  str = (char *) calloc(strlen(INSERT_BARCODE) + strlen(internal_id) + strlen(barcode) + strlen(coefficient) + 36, sizeof(char));
  strcpy(str,INSERT_BARCODE);

  if ( strlen(internal_id) == 0  )  { free(str); return 2;}
  strcat(str, internal_id);
  strcat(str, "', '");

  if ( strlen(barcode) == 0 ) { free(str); return 2;}
  strcat(str, barcode);
  strcat(str, "', ");
  
  if ( strlen(coefficient) == 0 ) { free(str); return 2;}
  strcat(str,coefficient);
  strcat(str, ", '");
  
  int state_check = CheckMainBarcode(internal_id);
  
  if (state_check <0) {
	log->PrintString(log_type, ERROR, "Error get info main barcode by internal code \n");
	free(str);
	return -1;
  } 
  PQclear(result);
  
  if (state_check == 0) strcat(str, "t"); else strcat(str, "f");
  
  strcat(str, "')");
  //printf("SQL str = %s \n", str);
  state = doSQL(myconnection, str);
  free(str);
  return state;
}
//==============================================================================//
#define CHECK_MAIN_BARCODE "SELECT * FROM ware_barcode WHERE main_barcode = 't' AND internal_id = '"
int PgUpload::CheckMainBarcode(const char* internal_id) {
  if ( strlen(internal_id)==0 ) return 0;
  int state;
  char* str = (char *) calloc(strlen(CHECK_MAIN_BARCODE) + strlen(internal_id) + 3, sizeof(char));
  strcpy(str,CHECK_MAIN_BARCODE);
  strcat(str, internal_id);
  strcat(str, "'");
  state = doSQL(myconnection, str);
  free(str);
  
  if (state != 2) return -1;
  return PQntuples(result);
}

//==============================================================================//
#define DELETE_WARE_BARCODE "DELETE FROM ware_barcode WHERE internal_id  = '"
int PgUpload::Delete_Barcode_By_Ware (const char* internal_id) {
  if ( strlen(internal_id)==0 ) return 0;
  int state;
  char* str;
  str = (char *) calloc(strlen(DELETE_WARE_BARCODE) + strlen(internal_id) + 3, sizeof(char));
  strcpy(str, DELETE_WARE_BARCODE);
  strcat(str, internal_id);
  strcat(str, "'");
  //printf("SQL str = %s \n", str);
  state = doSQL(myconnection, str);
  free(str);
  return state;
}
//==============================================================================//
#define DELETE_BARCODE_BY_GROUP "DELETE FROM ware_barcode WHERE internal_id IN (SELECT internal_id FROM ware WHERE group_id "
int PgUpload::Delete_Barcode_By_Group(const char* group_id) {
  if ( strlen(group_id)==0 ) return 0;
  int state;
  char* str;
  str = (char *) calloc(strlen(DELETE_BARCODE_BY_GROUP) + strlen(group_id) + 2, sizeof(char));
  strcpy(str,DELETE_BARCODE_BY_GROUP);
  strcat(str, group_id);
  strcat(str, ")");
  state = doSQL(myconnection, str);
  free(str);
  return state;
}
//==============================================================================//
#define DELETE_ALL_BARCODE "DELETE FROM ware_barcode "
int PgUpload::Delete_All_Barcodes() {
  int state;
  char* str;
  str = (char *) calloc(strlen(DELETE_ALL_BARCODE) + 2, sizeof(char));
  strcpy(str,DELETE_ALL_BARCODE);
  state = doSQL(myconnection, str);
  free(str);
  return state;
}


#define UPDATE_WARE "UPDATE Ware SET "
#define UPDATE_WARE_SALE "sale_quantity = 0, "
#define UPDATE_WARE_WHERE " WHERE internal_id = '"
//==============================================================================//
//sale_flag = true - обнулять продажи, sale_flag = false не надо обнулять продажи
//add_flag = true - количество должно добавляться, add_flag = false - количество должно заменяться 
int PgUpload::Update_Ware(const char * internal_code, const char * longtitle, const char * shortcut, const char * price, const char * quantity, const char * auto_scheme_id, const char * flags, const char * min_price, const char * best_before, const char * group_code, const char* tax_group_id, bool sale_flag, bool add_flag)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Update_Ware(char * internal_code, char * longtitle, char * shortcut, char * price, char * quantity, char * auto_scheme_id, char * flags, char * min_price, char * best_before, char * group_code, char* tax_group_id, bool sale_flag, bool add_flag)\n");
  int state;
  char * str;

  char * params = NULL;
  PackUpdateWareParameters(params, longtitle, shortcut, price, quantity, auto_scheme_id,  flags, min_price, best_before, group_code, tax_group_id, add_flag);

  str = (char *) calloc(strlen(UPDATE_WARE) + strlen(UPDATE_WARE_SALE) + strlen(params) + strlen(UPDATE_WARE_WHERE) + strlen(internal_code) + 2, sizeof(char));

  strcpy(str, UPDATE_WARE);
  if(sale_flag)
    strcat(str, UPDATE_WARE_SALE);
  strcat(str, params);
  strcat(str, UPDATE_WARE_WHERE);
  strcat(str, internal_code);
  strcat(str, "'");

  free(params);

  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "*******\n%s\n*******\n", str);
  //printf("update ware sql = %s \n", str);
  state = doSQL(myconnection, str);
  free(str);
	
  return state;
}

//==============================================================================//
int PgUpload::ReplaceWare(const char *internal_code, const char *bar_code, const char *longtitle, const char *shortcut, const char *price, const char * quantity, const char * auto_scheme_id, const char * flags, const char * min_price, const char * best_before,  const char * coefficient, const char * group_code, const char* tax_group_id, bool sale_flag)
//sale_flag = true - обнулять продажи, sale_flag = false не надо обнулять продажи
{
    
    //printf("ReplaceWare(%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)\n", internal_code, bar_code, longtitle, shortcut, price, quantity, auto_scheme_id, flags, min_price, best_before, coefficient, group_code, tax_group_id);
    int res;
    //puts("BeginTransaction");
    //printf("bar_code = %s \n", bar_code);
    res = BeginTransaction();
    //printf("begin work retun --- %d \n", res);
    //printf("Query_By_Internal_Code(internal_code = %d) \n", internal_code);
    res = Query_By_Internal_Code(internal_code);
    //printf("Query_By_Internal_Code(internal_code) return = %d \n", res);
    if ( res != 2 )     {
	log->PrintString(log_type, ERROR, "Add ware by internal code error\n");
	//RollbackTransaction();
	return -1;
	//return res;
    }  else  {
	//printf("geting ntuples");
	int n = PQntuples(result);
	//printf("count ware with code = %d \n", n);
	
	//puts("clear result");
	PQclear(result);
	//puts("clear result------ok");
	switch(n)    {
	case 0 : // Нет товара с таким кодом, INSERT
	    //puts("insert ware");
	    res = Insert_Ware(internal_code, longtitle, shortcut, price, quantity, auto_scheme_id,  flags, min_price, best_before, group_code, tax_group_id, sale_flag);
	    break;
	case 1 : // Есть товар с таким кодом, UPDATE
	    //puts("update ware");
	    res = Update_Ware(internal_code, longtitle, shortcut, price, quantity, auto_scheme_id,  flags, min_price, best_before, group_code, tax_group_id, sale_flag, false);
	    break;
	}
	if ( res != 1 )   {
	    log->PrintString(log_type, ERROR, "Update ware by intrenal code error \n");
	    RollbackTransaction();
	    return -1;
	    //return res;
	}
    }
    
    if ( Delete_Barcode_By_Ware(internal_code) < 0 ) {
	log->PrintString(log_type, ERROR, "Delete barcode by ware error \n");
	RollbackTransaction();
	return -1;
    }

    if ( Add_Barcodes(internal_code, bar_code, coefficient) < 0 ) {
	log->PrintString(log_type, ERROR, "Add barcodes by internal code error \n");
	RollbackTransaction();
	return -1;	
    }
    
    CommitTransaction();
    /*if ( res != 1 ) printf("res = %d \n", res);
    if ( res != 1 && res != 2 ) return res;
    return 0;*/
    return res;
}

//==============================================================================//
int PgUpload::ReplaceGroup(const char * code, const char * longtitle, const char * shortcut, const char * group_code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::ReplaceGroup(char * code, char * longtitle, char * shortcut, char * group_code)\n");
  int res;
  //BeginTransaction();
  if((res = Query_Group_By_Code(code)) != 2) // Запрос не выполнен, ошибка
  {
   log->PrintString(log_type, ERROR, "Error get info group by code \n");
    //RollbackTransaction();
    return -1;
    //return res;
  }
  else
  {
    int n = PQntuples(result);
    PQclear(result);
    switch(n)
    {
      case 0 : // Нет группы с таким кодом, INSERT
        res = Insert_Group(code, longtitle, shortcut, group_code);
        break;
      case 1 : // Есть группа с таким кодом, UPDATE
        res = Update_Group(code, longtitle, shortcut, group_code);
        break;
    }
    if(res != 1)
    {
     log->PrintString(log_type, ERROR, "Error replace group by code \n");
      //RollbackTransaction();
      return -1;
      //return res;
    }
  }
  //CommitTransaction();
  return 0;
}
//==============================================================================//
int PgUpload::AddUser(char * code, char * longtitle, char * shortcut, char * profile_code, char * pass, char * card)
{
//pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload	::AddUser(char * code, char * longtitle, char * shortcut, char * profile_code, char * pass, char * card)\n");
  int res;
  BeginTransaction();
  if((res = Query_By_User_Id(code)) != 2)
  {
   log->PrintString(log_type, ERROR, "Error get user info by id\n");
    RollbackTransaction();
    return -1;
    //return res;	
  }
  else
  {
    int n = PQntuples(result);
    PQclear(result);
    switch(n)
    {
      case 0 : // Нет пользователя с таким кодом, INSERT
        res = Insert_User(code, longtitle, shortcut, profile_code, pass, card);
        break;
      case 1 : // Есть пользоатель с таким кодом, UPDATE
        res = Update_User(code, longtitle, shortcut, profile_code, pass, card);
        break;
    }
    if(res != 1)
    {
     log->PrintString(log_type, ERROR, "Error change user info \n");
      RollbackTransaction();
      return -1;
      //return res;
    }
  }
  CommitTransaction();
  return 0;
}

//==============================================================================//
int PgUpload::AddFixReduction(const char * code, const char * longtitle, const char * shortcut, const char * type, const char * value, const char * area, const char* card, const char* barcode, const char* show)
{
    //printf("PgUpload::AddFixReduction(code = %s, longtitle = %s, shortcut = %s, type = %s, value = %s, area = %s, card = %s, barcode = %s, show = %s) \n", code, longtitle, type, value, area, card, barcode, show);
    int res = -1;
    //BeginTransaction();
    res = Query_By_Fix_Reduction_Id(code);
    //printf("res = %d \n", res);
    if ( res != 2 )   {
	log->PrintString(log_type, ERROR, "Error get fix reduction info by id\n");
	//RollbackTransaction();
	return -1;
    }  else  {
	int n = PQntuples(result);
	PQclear(result);
	//printf("count of fix reduction = %d \n", n);
	switch(n)    {
	case 0 : // Нет фиксированной скидки с таким кодом, INSERT
	    res = Insert_Fix_Reduction(code, longtitle, shortcut, type, value, area, card, barcode, show);
	    break;
	case 1 : // Есть фиксированная скидка с таким кодом, UPDATE
	    res = Update_Fix_Reduction(code, longtitle, shortcut, type, value, area, card, barcode, show);
	    break;
	}
	if ( res != 1 )    {
	    log->PrintString(log_type, ERROR, "Error change fix reduction info\n");
	    //RollbackTransaction();
	    return -1;
	    // return res;
	}
    }
    //CommitTransaction();
    return 0;
}

//==============================================================================//
int PgUpload::AddAutoReductionScheme(char * code, char * longtitle, char * shortcut)
{
//pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::AddAutoReductionScheme(char * code, char * longtitle, char * shortcut)\n");
  int res;
  BeginTransaction();
  if((res = Query_By_Auto_Reduction_Scheme_Id(code)) != 2) // Запрос не выполнен, ошибка
  {
   log->PrintString(log_type, ERROR, "Error get autoreduction scheme info\n");
    RollbackTransaction();
    return -1;
    //return res;
  }
  else
  {
    int n = PQntuples(result);
    PQclear(result);
    switch(n)
    {
      case 0 : // Нет фиксированной скидки с таким кодом, INSERT
        res = Insert_Auto_Reduction_Scheme(code, longtitle, shortcut);
        break;
      case 1 : // Есть фиксированная скидка с таким кодом, UPDATE
        res = Update_Auto_Reduction_Scheme(code, longtitle, shortcut);
        break;
    }
    if(res != 1)
    {
     log->PrintString(log_type, ERROR, "Error change autoreduction scheme info \n");
      RollbackTransaction();
      return -1;
      //return res;
    }
  }
  CommitTransaction();
  return 0;
}

//==============================================================================//
int PgUpload::AddAutoReduction(char * scheme_code, char * code, char * longtitle, char * shortcut, char * type, char * value, char * begin_date, char * end_date, char * begin_time, char * end_time, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end)
{
//pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::AddAutoReduction(char * scheme_code, char * code, char * longtitle, char * shortcut, char * type, char * value, char * begin_date, char * end_date, char * begin_time, char * end_time, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end)\n");
  int res;
  BeginTransaction();
  if((res = Query_By_Auto_Reduction_Id(code)) != 2) // Запрос не выполнен, ошибка
  {
   log->PrintString(log_type, ERROR, "Error get autoreduction info\n");
    RollbackTransaction();
    return -1;
    //return res;
  }
  else
  {
    int n = PQntuples(result);
    PQclear(result);
    switch(n)
    {
      case 0 : // Нет автоматической скидки с таким кодом, INSERT
        res = Insert_Auto_Reduction(scheme_code, code, longtitle, shortcut, type, value, begin_date, end_date, begin_time, end_time, weekdays, quantity, position_sum, check_sum, card_prefix_begin, card_prefix_end);
        break;
      case 1 : // Есть автоматической скидка с таким кодом, UPDATE
        res = Update_Auto_Reduction(scheme_code, code, longtitle, shortcut, type, value, begin_date, end_date, begin_time, end_time, weekdays, quantity, position_sum, check_sum, card_prefix_begin, card_prefix_end);
        break;
    }
    if(res != 1)
    {
     log->PrintString(log_type, ERROR, "Error change autoreduction info\n");
      RollbackTransaction();
      return -1;
      //return res;
    }
  }
  CommitTransaction();
  return 0;
}

//==============================================================================//
int PgUpload::AddTaxRates(char * code, char * longtitle, char * shortcut, char * value)
{
//pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::AddTaxRates(char * code, char * longtitle, char * shortcut, char * value)\n");
  int res;
  BeginTransaction();
  if((res = Query_By_Tax_Rate_Id(code)) != 2) // Запрос не выполнен, ошибка
  {
   log->PrintString(log_type, ERROR, "Error get tax rate by id info\n");
    RollbackTransaction();
    return -1;
    //return res;
  }
  else
  {
    int n = PQntuples(result);
    PQclear(result);
    switch(n)
    {
      case 0 : // Нет налоговой ставки с таким кодом, INSERT
        res = Insert_Tax_Rate(code, longtitle, shortcut, value);
        break;
      case 1 : // Есть налоговой ставки с таким кодом, UPDATE
        res = Update_Tax_Rate(code, longtitle, shortcut, value);
        break;
    }
    if(res != 1)
    {
     log->PrintString(log_type, ERROR, "Error change tax rates info\n");
      RollbackTransaction();
      return -1;
      //return res;
    }
  }
  CommitTransaction();
  return 0;
}

//==============================================================================//
int PgUpload::AddTaxGroups(char * code, char * longtitle, char * shortcut)
{
//pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::AddTaxGroups(char * code, char * longtitle, char * shortcut)\n");
  int res;
  BeginTransaction();
  if((res = Query_By_Tax_Group_Id(code)) != 2) // Запрос не выполнен, ошибка
  {
   log->PrintString(log_type, ERROR, "Error get tax group info\n");
    RollbackTransaction();
    return -1;
    //return res;
  }
  else
  {
    int n = PQntuples(result);
    PQclear(result);
    switch(n)
    {
      case 0 : // Нет налоговой группы с таким кодом, INSERT
        res = Insert_Tax_Group(code, longtitle, shortcut);
        break;
      case 1 : // Есть налоговой группы с таким кодом, UPDATE
        res = Update_Tax_Group(code, longtitle, shortcut);
        break;
    }
    if(res != 1)
    {
     log->PrintString(log_type, ERROR, "Error change tax group info\n");
      RollbackTransaction();
      return -1;
      //return res;
    }
  }
  CommitTransaction();
  return 0;
}

//==============================================================================//
int PgUpload::AddTaxRatesGroups(char * code, char * group_code, char * rate_code, char * base_flag)
{
//pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::AddTaxRatesGroups(char * code, char * group_code, char * rate_code, char * base_flag)\n");
  int res;
  BeginTransaction();
  if((res = Query_By_Tax_Rate_Group_Id(code)) != 2) // Запрос не выполнен, ошибка
  {
   log->PrintString(log_type, ERROR, "Error get tax rate group info by id\n");
    RollbackTransaction();
    return -1;
    //return res;
  }
  else
  {
    int n = PQntuples(result);
    PQclear(result);
    switch(n)
    {
      case 0 : // Нет налоговой ставки группы с таким кодом, INSERT
        res = Insert_Tax_Rate_Group(code, group_code, rate_code, base_flag);
        break;
      case 1 : // Есть налоговой ставки группы с таким кодом, UPDATE
        res = Update_Tax_Rate_Group(code, group_code, rate_code, base_flag);
        break;
    }
    if(res != 1)
    {
     log->PrintString(log_type, ERROR, "Error change tax rates groups info\n");
      RollbackTransaction();
      return -1;
      //return res;
    }
  }
  CommitTransaction();
  return 0;
}

//==============================================================================//
int PgUpload::DeleteAllWaresGroups()
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::DeleteAllWaresGroups()\n");
  int res;
  BeginTransaction();
  
  if( (res = Delete_All_Barcodes()) < 0 )  {
   log->PrintString(log_type, ERROR, "Error delete all barcodes\n");
    RollbackTransaction();
    return -1;
    //return res;
  }
  
  if((res = Delete_All_Groups()) != 1)
  {
   log->PrintString(log_type, ERROR, "Error delete all groups\n");
    RollbackTransaction();
    return -1;
    //return res;
  }
  if((res = Delete_All_Wares()) != 1)
  {
   log->PrintString(log_type, ERROR, "Error delete all wares\n");
    RollbackTransaction();
    return -1;
    //return res;
  }
  CommitTransaction();
  return 0;
}

//==============================================================================//
//
int PgUpload::DeleteWaresGroupsByCode(char *code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::DeleteWaresGroupsByCode(char *code)\n");
  int res;
  BeginTransaction();
  
  if( (res = Delete_Barcode_By_Group(code)) < 0 )  {
   log->PrintString(log_type, ERROR, "Error delete wares group by code\n");
    RollbackTransaction();
    return -1;
    //return res;
  }
//удаляет товары из группы
  if((res = Delete_Wares_By_Code(code)) != 1)
  {
   log->PrintString(log_type, ERROR, "Error delete wares by code\n");
    RollbackTransaction();
    return -1;
    //return res;
  }
// удадяет группу
  if((res = Delete_Groups_By_Code(code)) != 1)
  {
   log->PrintString(log_type, ERROR, "Error delete groups by code\n");
    RollbackTransaction();
    return -1;
    //return res;
  }
  CommitTransaction();
  return 0;
}
//==============================================================================//
int PgUpload::DeleteFromTable(int command_ind)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::DeleteFromTable(int command_ind)\n");
  int res = 0;
  switch(command_ind)
  {
    case 1 :
      res = DeleteFromTable("Users");
      break;
    case 2 :
      res = DeleteFromTable("Fix_Reduction");
      break;
    case 3 :
      res = DeleteFromTable("Auto_Scheme");
      break;
    case 4 :
      res = DeleteFromTable("Auto_Reduction");
      break;
    case 5 :
      res = DeleteFromTable("Tax_Rate");
      break;
    case 6 :
      res = DeleteFromTable("Tax_Group");
      break;
    case 7 :
      res = DeleteFromTable("Tax_Rate_Group");
      break;
  }
  if(res != 1)
    return -1;
  else
    return 0;
}
		 
//###############################################################################################################################//

//+++++++++++++++++++++++++++++++DELETES+++++++++++++++++++++++++++++++++++++++++//
//==============================================================================//
#define DELETE_GROUPS "DELETE FROM Ware_Group"
int PgUpload::Delete_All_Groups()
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Delete_All_Groups()\n");
  int state;
  char * str;
	
  str = (char *) calloc(strlen(DELETE_GROUPS) + 1, sizeof(char));
  strcpy(str, DELETE_GROUPS);

  state = doSQL(myconnection, str);
  free(str);

  return state;
}

#define DELETE_WARES "DELETE FROM Ware"
//==============================================================================//
int PgUpload::Delete_All_Wares()
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Delete_All_Wares()\n");
  int state;
  char * str;
	
  str = (char *) calloc(strlen(DELETE_WARES) + 1, sizeof(char));
  strcpy(str, DELETE_WARES);

  state = doSQL(myconnection, str);
  free(str);

  return state;
}


#define DELETE_GROUPS_WITH_CODE "DELETE FROM Ware_Group WHERE group_id = "
//==============================================================================//
int PgUpload::Delete_Groups_By_Code(char * code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Delete_Groups_By_Code(char * code)\n");
  int state;
  char * str;

  str = (char *) calloc(strlen(DELETE_GROUPS_WITH_CODE) + strlen(code) + 1, sizeof(char));

  strcpy(str, DELETE_GROUPS_WITH_CODE);
  strcat(str, code);

  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "*******\n%s\n*******\n", str);

  state = doSQL(myconnection, str);
  free(str);
	
  return state;
}

#define DELETE_WARES_WITH_CODE "DELETE FROM Ware WHERE internal_id = '"
#define DELETE_WARES_WITH_GROUP_CODE " OR group_id = "
//==============================================================================//
int PgUpload::Delete_Wares_By_Code(char * code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Delete_Wares_By_Code(char * code)\n");
  int state;
  char * str;

  str = (char *) calloc(strlen(DELETE_WARES_WITH_CODE) + strlen(DELETE_WARES_WITH_GROUP_CODE) + 3 * strlen(code) + 1, sizeof(char));

  strcpy(str, DELETE_WARES_WITH_CODE);
  strcat(str, code);
  strcat(str, "'");
  strcat(str, DELETE_WARES_WITH_GROUP_CODE);
  strcat(str, code);

  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "*******\n%s\n*******\n", str);

  state = doSQL(myconnection, str);
  free(str);
	
  return state;
}

#define DELETE_FROM_TABLE "DELETE FROM "
//==============================================================================//
int PgUpload::DeleteFromTable(const char * table)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::DeleteFromTable(char * table)\n");
  int state;
  char * str;
	
  str = (char *) calloc(strlen(DELETE_FROM_TABLE) + strlen(table) + 1, sizeof(char));
  strcpy(str, DELETE_FROM_TABLE);
  strcat(str, table);

  state = doSQL(myconnection, str);
  free(str);

  return state;
}

//+++++++++++++++++++++++++++++++UPDATES+++++++++++++++++++++++++++++++++++++++++//

#define UPDATE_GROUP "UPDATE Ware_Group SET "
#define UPDATE_GROUP_WHERE " WHERE group_id = "
//==============================================================================//
int PgUpload::Update_Group(const char * code, const char *longtitle,  const char * shortcut, const char *group_code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Update_Group(char * code, char *longtitle,  char * shortcut, char *group_code)\n");
  int state;
  char * str;

  char * params = NULL;
  PackUpdateGroupParameters(params, longtitle, shortcut, group_code);

  str = (char *) calloc(strlen(UPDATE_GROUP) + strlen(params) + strlen(UPDATE_GROUP_WHERE) + strlen(code) + 1, sizeof(char));

  strcpy(str, UPDATE_GROUP);
  strcat(str, params);
  strcat(str, UPDATE_GROUP_WHERE);
  strcat(str, code);

  free(params);

  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "*******\n%s\n*******\n", str);

  state = doSQL(myconnection, str);
  free(str);
	
  return state;
}


#define UPDATE_USER "UPDATE Users SET "
#define UPDATE_USER_WHERE "WHERE user_id = "
//==============================================================================//
int PgUpload::Update_User(char * code, char * longtitle, char * shortcut, char * profile_code, char * pass, char * card)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Update_User(char * code, char * longtitle, char * shortcut, char * profile_code, char * pass, char * card)\n");
	
  int state;
  char * str;

  char * params = NULL;
  PackUpdateUserParameters(params, longtitle, shortcut, profile_code, pass, card);

  str = (char *) calloc(strlen(UPDATE_USER) + strlen(params) + strlen(UPDATE_USER_WHERE) + strlen(code) + 1, sizeof(char));

  strcpy(str, UPDATE_USER);
  strcat(str, params);
  strcat(str, UPDATE_USER_WHERE);
  strcat(str, code);
	
  free(params);
 
  state = doSQL(myconnection, str);
 
  free(str);

  return state; 
}

#define UPDATE_FIX_RED "UPDATE Fix_Reduction SET "
#define UPDATE_FIX_RED_WHERE " WHERE external_id = "
//==============================================================================//
int PgUpload::Update_Fix_Reduction(const char * code, const char * longtitle, const char * shortcut, const char * type, const char * value, const char * area, const char * card, const char* barcode, const char* show)
{
  //printf("int PgUpload::Update_Fix_Reduction(%s, %s, %s, %s, %s, %s, %s)\n", code, longtitle, shortcut, type, value, area, barcode);
  int state;
  char * str;
  
  //PackUpdateFixReductionParameters(params, longtitle, shortcut, card_prefix, type, value, begin_date, end_date, area);
  
  str = (char *) calloc(strlen(UPDATE_FIX_RED) + strlen("external_id = , longtitle = , shortcut = , type = , value = , area = , cardcode = ,barcode = , show = ") + 2*strlen(code) + strlen(longtitle) + strlen(shortcut) + strlen(type) + strlen(value) + strlen(area) + strlen(barcode) + strlen(card) + strlen(show) + strlen(UPDATE_FIX_RED_WHERE) + 1, sizeof(char));
  
  strcpy(str, UPDATE_FIX_RED);
  
  strcat(str, "external_id = ");
  strcat(str, code);
  strcat(str, ", longtitle = ");
  strcat(str, longtitle);
  strcat(str, ", shortcut =  ");
  strcat(str, shortcut);
  strcat(str, ", type = ");
  strcat(str, type);
  strcat(str, ", value = ");
  strcat(str, value);
  strcat(str, ", area = ");
  strcat(str, area);
  strcat(str, ", cardcode = ");
  strcat(str, card);
  strcat(str, ", barcode = ");
  strcat(str, barcode);
  strcat(str, ", show = ");
  strcat(str, show);
  strcat(str, UPDATE_FIX_RED_WHERE);
  strcat(str, code);
  
  //printf("str = %s \n", str);
 
  state = doSQL(myconnection, str);
 
  free(str);

  return state; 
}

#define UPDATE_AUTO_SCHEME "UPDATE Auto_Scheme SET "
#define UPDATE_AUTO_SCHEME_WHERE " WHERE auto_scheme_id = "
//==============================================================================//
int PgUpload::Update_Auto_Reduction_Scheme(char * code, char * longtitle, char * shortcut)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Update_Auto_Reduction_Scheme(char * code, char * longtitle, char * shortcut)\n");

  int state;
  char * str;

  char * params = NULL;
  PackUpdateAutoSchemeParameters(params, longtitle, shortcut);

  str = (char *) calloc(strlen(UPDATE_AUTO_SCHEME) + strlen(params) + strlen(UPDATE_AUTO_SCHEME_WHERE) + strlen(code) + 1, sizeof(char));

  strcpy(str, UPDATE_AUTO_SCHEME);
  strcat(str, params);
  strcat(str, UPDATE_AUTO_SCHEME_WHERE);
  strcat(str, code);
	
  free(params);
 
  state = doSQL(myconnection, str);
 
  free(str);

  return state; 
}

#define UPDATE_AUTO_RED "UPDATE Auto_Reduction SET "
#define UPDATE_AUTO_RED_WHERE " WHERE auto_reduction_id = "
//==============================================================================//
int PgUpload::Update_Auto_Reduction(char * scheme_code, char * code, char * longtitle, char * shortcut, char * type, char * value, char * begin_date, char * end_date, char * begin_time, char * end_time, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Update_Auto_Reduction(char * scheme_code, char * code, char * longtitle, char * shortcut, char * type, char * value, char * begin_date, char * end_date, char * begin_time, char * end_time, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end)\n");
  int state;
  char * str;

  char * params = NULL;
  PackUpdateAutoReductionParameters(params, scheme_code, longtitle, shortcut, type, value, begin_date, end_date, begin_time, end_time, weekdays, quantity, position_sum, check_sum, card_prefix_begin, card_prefix_end);

  str = (char *) calloc(strlen(UPDATE_AUTO_RED) + strlen(params) + strlen(UPDATE_AUTO_RED_WHERE) + strlen(code) + 1, sizeof(char));

  strcpy(str, UPDATE_AUTO_RED);
  strcat(str, params);
  strcat(str, UPDATE_AUTO_RED_WHERE);
  strcat(str, code);
	
  free(params);
 
  state = doSQL(myconnection, str);
 
  free(str);

  return state; 
}

#define UPDATE_TAX_RATE "UPDATE Tax_Rate SET "
#define UPDATE_TAX_RATE_WHERE " WHERE tax_rate_id = "
//==============================================================================//
int PgUpload::Update_Tax_Rate(char * code, char * longtitle, char * shortcut, char * value)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Update_Tax_Rate(char * code, char * longtitle, char * shortcut, char * value)\n");
  int state;
  char * str;

  char * params = NULL;
  PackUpdateTaxRateParameters(params, longtitle, shortcut, value);

  str = (char *) calloc(strlen(UPDATE_TAX_RATE) + strlen(params) + strlen(UPDATE_TAX_RATE_WHERE) + strlen(code) + 1, sizeof(char));

  strcpy(str, UPDATE_TAX_RATE);
  strcat(str, params);
  strcat(str, UPDATE_TAX_RATE_WHERE);
  strcat(str, code);
	
  free(params);
 
  state = doSQL(myconnection, str);
 
  free(str);

  return state; 
}

#define UPDATE_TAX_GROUP "UPDATE Tax_Group SET "
#define UPDATE_TAX_GROUP_WHERE " WHERE tax_group_id = "
//==============================================================================//
int PgUpload::Update_Tax_Group(char * code, char * longtitle, char * shortcut)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Update_Tax_Group(char * code, char * longtitle, char * shortcut)\n");
  int state;
  char * str;

  char * params = NULL;
  PackUpdateTaxGroupParameters(params, longtitle, shortcut);

  str = (char *) calloc(strlen(UPDATE_TAX_GROUP) + strlen(params) + strlen(UPDATE_TAX_GROUP_WHERE) + strlen(code) + 1, sizeof(char));

  strcpy(str, UPDATE_TAX_GROUP);
  strcat(str, params);
  strcat(str, UPDATE_TAX_GROUP_WHERE);
  strcat(str, code);
	
  free(params);
 
  state = doSQL(myconnection, str);
 
  free(str);

  return state; 
}

#define UPDATE_TAX_RATE_GROUP "UPDATE Tax_Rate_Group SET "
#define UPDATE_TAX_RATE_GROUP_WHERE " WHERE id = "
//==============================================================================//
int PgUpload::Update_Tax_Rate_Group(char * code, char * group_code, char * rate_code, char * base_flag)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Update_Tax_Rate_Group(char * code, char * group_code, char * rate_code, char * base_flag)\n");
  int state;
  char * str;

  char * params = NULL;
  PackUpdateTaxRateGroupParameters(params, group_code, rate_code, base_flag);

  str = (char *) calloc(strlen(UPDATE_TAX_RATE_GROUP) + strlen(params) + strlen(UPDATE_TAX_RATE_GROUP_WHERE) + strlen(code) + 1, sizeof(char));

  strcpy(str, UPDATE_TAX_RATE_GROUP);
  strcat(str, params);
  strcat(str, UPDATE_TAX_RATE_GROUP_WHERE);
  strcat(str, code);
	
  free(params);
 
  state = doSQL(myconnection, str);
 
  free(str);

  return state; 
}

//+++++++++++++++++++++++++++++++INSERTS+++++++++++++++++++++++++++++++++++++++++//

#define INSERT_GROUP "INSERT INTO Ware_Group(group_id, longtitle, shortcut, parent_group_id) VALUES("
#define INSERT_GROUP_END ")"
//==============================================================================//
int PgUpload::Insert_Group(const char * code, const char *longtitle, const char * shortcut, const char *group_code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Insert_Group(char * code, char *longtitle, char * shortcut, char *group_code)\n");
  int state;
  char * str;

  char * params = NULL;
  PackInsertGroupParameters(params, code, longtitle, shortcut, group_code);

  str = (char *) calloc(strlen(INSERT_GROUP) + strlen(params) + strlen(INSERT_GROUP_END) + 1, sizeof(char));

  strcpy(str, INSERT_GROUP);
  strcat(str, params);
  strcat(str, INSERT_GROUP_END);

  free(params);

  state = doSQL(myconnection, str);
  free(str);
	
  return state;
}

#define INSERT_WARE "INSERT INTO Ware(internal_id, longtitle, shortcut, price, quantity, auto_scheme_id, flags, min_price, best_before, group_id, tax_group_id"
#define INSERT_WARE_SALE ", sale_quantity"
#define INSERT_WARE_VALUES ") VALUES ('"
#define INSERT_WARE_END ")"
//==============================================================================//
//sale_flag = true - обнулять продажи, sale_flag = false не надо обнулять продажи
int PgUpload::Insert_Ware(const char *internal_code, const char *longtitle, const char *shortcut, const char *price, const char * quantity, const char * auto_scheme_id, const char * flags, const char * min_price, const char * best_before, const char * group_code, const char* tax_group_id, bool sale_flag)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Insert_Ware(char *internal_code, char *longtitle, char *shortcut, char *price, char * quantity, char * auto_scheme_id, char * flags, char * min_price, char * best_before, char * group_code, char* tax_group_id, bool sale_flag)\n");

  int state;
  char * str;

  char * params = NULL;
  PackInsertWareParameters(params, internal_code, longtitle, shortcut, price, quantity, auto_scheme_id,  flags, min_price, best_before, group_code, tax_group_id);

  str = (char *) calloc(strlen(INSERT_WARE) + strlen(INSERT_WARE_SALE) + strlen(INSERT_WARE_VALUES) + strlen(params) + strlen(INSERT_WARE_END) + 3 + 1, sizeof(char));

  strcpy(str, INSERT_WARE);
  if(sale_flag)
    strcat(str, INSERT_WARE_SALE);
  strcat(str, INSERT_WARE_VALUES);
  strcat(str, params);
  if(sale_flag)
  strcat(str, ", 0");
  strcat(str, INSERT_WARE_END);
	
  free(params);
  //printf("SQL insert param = %s \n", str); 
  state = doSQL(myconnection, str);
 
  free(str);

  return state; 
  
}

#define INSERT_USER "INSERT INTO Users(user_id, user_name, shortcut, profile_id, userpassword, card_code) VALUES("
#define INSERT_USER_END ")"
//==============================================================================//
int PgUpload::Insert_User(char * code, char * longtitle, char * shortcut, char * profile_code, char * pass, char * card)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Insert_User(char * code, char * longtitle, char * shortcut, char * profile_code, char * pass, char * card)\n");
	
  int state;
  char * str;

  char * params = NULL;
  PackInsertUserParameters(params, code, longtitle, shortcut, profile_code, pass, card);

  str = (char *) calloc(strlen(INSERT_USER) + strlen(params) + strlen(INSERT_USER_END) + 1, sizeof(char));

  strcpy(str, INSERT_USER);
  strcat(str, params);
  strcat(str, INSERT_USER_END);

	
  free(params);
 
  state = doSQL(myconnection, str);
 
  free(str);

  return state; 
}
/*
#define INSERT_FIX_RED "INSERT INTO Fix_Reduction(external_id, longtitle, shortcut, value, type, area, begindate, enddate, card_prefix) VALUES("
#define INSERT_FIX_RED_END ")"
//==============================================================================//
int PgUpload::Insert_Fix_Reduction(char * code, char * longtitle, char * shortcut, char * card_prefix, char * type, char * value, char * begin_date, char * end_date, char * area)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Insert_Fix_Reduction(char * code, char * longtitle, char * shortcut, char * card_prefix, char * type, char * value, char * begin_date, char * end_date, char * area)\n");
  int state;
  char * str;

  char * params = NULL;
  PackInsertFixReductionParameters(params, code, longtitle, shortcut, card_prefix, type, value, begin_date, end_date, area);

  str = (char *) calloc(strlen(INSERT_FIX_RED) + strlen(params) + strlen(INSERT_FIX_RED_END) + 1, sizeof(char));

  strcpy(str, INSERT_FIX_RED);
  strcat(str, params);
  strcat(str, INSERT_FIX_RED_END);
	
  free(params);
 
  state = doSQL(myconnection, str);
 
  free(str);

  return state; 
}
*/



#define INSERT_FIX_RED "INSERT INTO fix_reduction (external_id, scheme_id, longtitle, shortcut, value, type, area, cardcode, barcode, show) SELECT "
#define INSERT_FIX_RED_END " FROM activescheme"
//==============================================================================//
int PgUpload::Insert_Fix_Reduction(const char * code, const char * longtitle, const char * shortcut, const char * type, const char * value, const char * area, const char* card, const char* barcode, const char* show)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Insert_Fix_Reduction(char * code, char * longtitle, char * shortcut, char * card_prefix, char * type, char * value, char * begin_date, char * end_date, char * area)\n");
  int state;
  char * str;
  
  str = (char*) calloc(strlen(INSERT_FIX_RED) + strlen(INSERT_FIX_RED_END) + strlen(code) + strlen(longtitle) + strlen(shortcut) + strlen(type) + strlen(area) + strlen(value) + strlen(barcode) + strlen(card) + strlen(show) + strlen(", activescheme_id, , , , , , , , , , ,") + 1, sizeof(char)) ;  
  
  strcpy(str, INSERT_FIX_RED);
  strcat(str, code);
  strcat(str, ", activescheme_id, ");
  strcat(str, longtitle);
  strcat(str, ", ");
  strcat(str, shortcut);
  strcat(str, ", ");
  strcat(str, value);
  strcat(str, ", ");
  strcat(str, type);
  strcat(str, ", ");
  strcat(str, area);
  strcat(str, ", ");
  strcat(str, card);
  strcat(str, ", ");
  strcat(str, barcode);
  strcat(str, ", ");
  strcat(str, show);
  strcat(str, INSERT_FIX_RED_END);
  
  //printf("str = %s \n", str);
 
  state = doSQL(myconnection, str);
 
  free(str);

  return state; 
}


#define INSERT_AUTO_SCHEME "INSERT INTO Auto_Scheme(auto_scheme_id, longtitle, shortcut) VALUES("
#define INSERT_AUTO_SCHEME_END ")"
//==============================================================================//
int PgUpload::Insert_Auto_Reduction_Scheme(char * code, char * longtitle, char * shortcut)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Insert_Auto_Reduction_Scheme(char * code, char * longtitle, char * shortcut)\n");
  int state;
  char * str;

  char * params = NULL;
  PackInsertAutoSchemeParameters(params, code, longtitle, shortcut);

  str = (char *) calloc(strlen(INSERT_AUTO_SCHEME) + strlen(params) + strlen(INSERT_AUTO_SCHEME_END) + 1, sizeof(char));

  strcpy(str, INSERT_AUTO_SCHEME);
  strcat(str, params);
  strcat(str, INSERT_AUTO_SCHEME_END);
	
  free(params);
 
  state = doSQL(myconnection, str);
 
  free(str);

  return state; 
}

#define INSERT_AUTO_RED "INSERT INTO Auto_Reduction(auto_reduction_id, longtitle, shortcut, value, type, begindate, enddate, begintime, endtime, weekdays, quantity, position_sum, check_sum, auto_scheme_id,  card_prefix_begin, card_prefix_end) VALUES("
#define INSERT_AUTO_RED_END ")"
//==============================================================================//
int PgUpload::Insert_Auto_Reduction(char * scheme_code, char * code, char * longtitle, char * shortcut, char * type, char * value, char * begin_date, char * end_date, char * begin_time, char * end_time, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Insert_Auto_Reduction(char * scheme_code, char * code, char * longtitle, char * shortcut, char * type, char * value, char * begin_date, char * end_date, char * begin_time, char * end_time, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end)\n");

  int state;
  char * str;

  char * params = NULL;
  PackInsertAutoReductionParameters(params, code, scheme_code, longtitle, shortcut, type, value, begin_date, end_date, begin_time, end_time, weekdays, quantity, position_sum, check_sum, card_prefix_begin, card_prefix_end);

  str = (char *) calloc(strlen(INSERT_AUTO_RED) + strlen(params) + strlen(INSERT_AUTO_RED_END) + 1, sizeof(char));

  strcpy(str, INSERT_AUTO_RED);
  strcat(str, params);
  strcat(str, INSERT_AUTO_RED_END);
	
  free(params);
 
  state = doSQL(myconnection, str);
 
  free(str);

  return state; 
}


#define INSERT_TAX_RATE "INSERT INTO Tax_Rate(tax_rate_id, longtitle, shortcut, value) VALUES("
#define INSERT_TAX_RATE_END ")"
//==============================================================================//
int PgUpload::Insert_Tax_Rate(char * code, char * longtitle, char * shortcut, char * value)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Insert_Tax_Rate(char * code, char * longtitle, char * shortcut, char * value)\n");
  int state;
  char * str;
	
  char * params = NULL;
  PackInsertTaxRateParameters(params, code, longtitle, shortcut, value);

  str = (char *) calloc(strlen(INSERT_TAX_RATE) + strlen(params) + strlen(INSERT_TAX_RATE_END) + 1, sizeof(char));

  strcpy(str, INSERT_TAX_RATE);
  strcat(str, params);
  strcat(str, INSERT_TAX_RATE_END);
	
  free(params);
 
  state = doSQL(myconnection, str);
 
  free(str);

  return state; 
}

#define INSERT_TAX_GROUP "INSERT INTO Tax_Group(tax_group_id, longtitle, shortcut) VALUES("
#define INSERT_TAX_GROUP_END ")"
//==============================================================================//
int PgUpload::Insert_Tax_Group(char * code, char * longtitle, char * shortcut)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Insert_Tax_Group(char * code, char * longtitle, char * shortcut)\n");
  int state;
  char * str;
  
  char * params = NULL;
  PackInsertTaxGroupParameters(params, code, longtitle, shortcut);

  str = (char *) calloc(strlen(INSERT_TAX_GROUP) + strlen(params) + strlen(INSERT_TAX_GROUP_END) + 1, sizeof(char));

  strcpy(str, INSERT_TAX_GROUP);
  strcat(str, params);
  
  strcat(str, INSERT_TAX_GROUP_END);
	
  free(params);
 
  state = doSQL(myconnection, str);
 
  free(str);

  return state; 
}
#define GET_MAX_ORDER_NUM "SELECT MAX(order_id)+1 FROM tax_rate_group WHERE tax_group_id = "
#define INSERT_TAX_RATE_GROUP "INSERT INTO Tax_Rate_Group(id, tax_group_id, tax_rate_id, base_flag, order_id) VALUES("
#define INSERT_TAX_RATE_GROUP_END ")"
//==============================================================================//
int PgUpload::Insert_Tax_Rate_Group(char * code, char * group_code, char * rate_code, char * base_flag)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Insert_Tax_Rate_Group(char * code, char * group_code, char * rate_code, char * base_flag)\n");
  int state;
  
  char* order_id;
  char * str = (char*) calloc(strlen(GET_MAX_ORDER_NUM) + strlen(group_code) + 1, sizeof(char));
  strcat(str, GET_MAX_ORDER_NUM);
  strcat(str, group_code);
  state = doSQL(myconnection, str);
  if ( state != 2 ) return -1;
  order_id = (char*) calloc(PQgetlength(result, 0, 0) + 2 , sizeof(char));
  strcpy(order_id, PQgetvalue(result, 0, 0));
  PQclear(result);
  free(str);
  if ( strlen(order_id) < 1 ) strcpy(order_id, "0");
  
  
  char * params = NULL;
  PackInsertTaxRateGroupParameters(params, code, group_code, rate_code, base_flag);

  str = (char *) calloc(strlen(INSERT_TAX_RATE_GROUP) + strlen(params) + strlen(INSERT_TAX_RATE_GROUP_END) + strlen(order_id)  + 5, sizeof(char));

  strcpy(str, INSERT_TAX_RATE_GROUP);
  strcat(str, params);
  
  strcat(str, ", ");
  strcat(str, order_id);
  free(order_id);
  
  strcat(str, INSERT_TAX_RATE_GROUP_END);
	
  free(params);
 
  state = doSQL(myconnection, str);
 
  free(str);

  return state; 
}

//+++++++++++++++++++++++++++++++QUERIES+++++++++++++++++++++++++++++++++++++++++//

#define QUERY_GROUP "SELECT group_id FROM Ware_Group WHERE group_id = "
int PgUpload::Query_Group_By_Code(const char * code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Query_Group_By_Code(char* code)\n");

  int state;
  char *str;
 
  str = (char *) calloc(strlen(QUERY_GROUP) + strlen(code) + 1, sizeof(char));
  strcpy(str, QUERY_GROUP);
  strcat(str, code);

  state = doSQL(myconnection,str);
  free(str);

  return state;  
}

#define QUERY_INTERNAL "SELECT internal_id FROM Ware WHERE internal_id = '"
//==============================================================================//
int PgUpload::Query_By_Internal_Code(const char* internal_code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Query_By_Internal_Code(char* internal_code)\n");

  int state;
  char *str;
  
  str = (char *) calloc(strlen(QUERY_INTERNAL) + strlen(internal_code) + 3, sizeof(char));
  
  strcpy(str,QUERY_INTERNAL);
  strcat(str, internal_code);    
  strcat(str, "'");
  
  state = doSQL(myconnection, str);
  free(str);  
  return state;
}
//==============================================================================//
#define QUERY_USER "SELECT user_id FROM Users WHERE user_id = "
int PgUpload::Query_By_User_Id(char * code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Query_By_User_Id(char * code)\n");
  int state;
  char* str;
  
  str = (char *) calloc(strlen(QUERY_USER) + strlen(code) +1, sizeof(char));
  
  strcpy(str, QUERY_USER);
  strcat(str, code);
  
  state = doSQL(myconnection, str);

  free(str);  
  return state; 
}

#define QUERY_FIX_REDUCTION "SELECT external_id FROM Fix_Reduction WHERE external_id = "
//==============================================================================//
int PgUpload::Query_By_Fix_Reduction_Id(const char * code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Query_By_Fix_Reduction_Id(char * code)\n");
  
  int state;
  char* str;
  
  //printf("strlen(QUERY_FIX_REDUCTION) + strlen(code) = %d \n", strlen(QUERY_FIX_REDUCTION) + strlen(code));
  str = (char *) calloc(strlen(QUERY_FIX_REDUCTION) + strlen(code) +1, sizeof(char));
  
  strcpy(str, QUERY_FIX_REDUCTION);
  strcat(str, code);  
  
  
  //printf("str = %s \n", str);
  state = doSQL(myconnection, str);
  
  //printf("str before free = %s \n", str);  
  
  free(str);  
  
  //printf("state in  Query_By_Fix_Reduction_Id = %d \n", state);
  
  return state;  
}

#define QUERY_AUTO_REDUCTION_SCHEME "SELECT auto_scheme_id FROM Auto_Scheme WHERE auto_scheme_id = "
//==============================================================================//
int PgUpload::Query_By_Auto_Reduction_Scheme_Id(char *code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Query_By_Auto_Reduction_Scheme_Id(char *code)\n");
  
  int state;
  char* str;
  
  str = (char *) calloc(strlen(QUERY_AUTO_REDUCTION_SCHEME) + strlen(code) +1, sizeof(char));
  
  strcpy(str, QUERY_AUTO_REDUCTION_SCHEME);
  strcat(str, code);
  
  state = doSQL(myconnection, str);

  free(str);  
  return state; 
}

#define QUERY_AUTO_REDUCTION "SELECT auto_reduction_id FROM Auto_Reduction WHERE auto_reduction_id = "
//==============================================================================//
int PgUpload::Query_By_Auto_Reduction_Id(char * code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Query_By_Auto_Reduction_Id(char * code)\n");
  
  int state;
  char * str;
  
  str = (char *) calloc(strlen(QUERY_AUTO_REDUCTION) + strlen(code) +1, sizeof(char));
  
  strcpy(str, QUERY_AUTO_REDUCTION);
  strcat(str, code);
  
  state = doSQL(myconnection, str);

  free(str);  
  return state; 
}
//==============================================================================//
#define QUERY_TAX_RATE "SELECT tax_rate_id FROM Tax_Rate WHERE tax_rate_id = "
int PgUpload::Query_By_Tax_Rate_Id(char * code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Query_By_Tax_Rate_Id(char * code)\n");
  int state;
  char * str;
  
  str = (char *) calloc(strlen(QUERY_TAX_RATE) + strlen(code) +1, sizeof(char));
  
  strcpy(str, QUERY_TAX_RATE);
  strcat(str, code);
  
  state = doSQL(myconnection, str);

  free(str);  
  return state; 
}
//==============================================================================//
#define QUERY_TAX_GROUP "SELECT tax_group_id FROM Tax_Group WHERE tax_group_id = "
int PgUpload::Query_By_Tax_Group_Id(char * code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Query_By_Tax_Group_Id(char * code)\n");
  int state;
  char * str;
  
  str = (char *) calloc(strlen(QUERY_TAX_GROUP) + strlen(code) +1, sizeof(char));
  
  strcpy(str, QUERY_TAX_GROUP);
  strcat(str, code);
  
  state = doSQL(myconnection, str);

  free(str);  
  return state; 
}
//==============================================================================//
#define QUERY_TAX_RATE_GROUP "SELECT id FROM Tax_Rate_Group WHERE id = "
int PgUpload::Query_By_Tax_Rate_Group_Id(char * code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "int PgUpload::Query_By_Tax_Rate_Group_Id(char * code)\n");
  int state;
  char* str;
  
  str = (char *) calloc(strlen(QUERY_TAX_RATE_GROUP) + strlen(code) +1, sizeof(char));
  
  strcpy(str, QUERY_TAX_RATE_GROUP);
  strcat(str, code);
  
  state = doSQL(myconnection, str);

  free(str);  
  return state; 
}

//#####################################################################################################################//
#define PACK_DELIM ", "

//==============================================================================//
void PgUpload::PackUpdateGroupParameters(char *& pack_param, const char *longtitle,  const char * shortcut, const char *group_code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackUpdateGroupParameters(char *& pack_param, char *longtitle,  char * shortcut, char *group_code)\n");
	
 pack_param = (char *) calloc(strlen(longtitle) + strlen(shortcut) + strlen(group_code) + strlen("longtitle = ") + strlen("shortcut = ") + strlen("parent_group_id = ") + 2 * strlen(PACK_DELIM) + 1, sizeof(char));

  strcat(pack_param, "longtitle = ");
  strcat(pack_param, longtitle);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, "shortcut = ");
  strcat(pack_param, shortcut);

  if(strcmp(group_code, "null"))
  {
    strcat(pack_param, PACK_DELIM);
    strcat(pack_param, "parent_group_id = ");
    strcat(pack_param, group_code);
  }

}

//==============================================================================//
void PgUpload::PackUpdateWareParameters(char *& pack_param, const char *longtitle, const char *shortcut, const char *price, const char * quantity, const char * auto_scheme_id, const char * flags, const char * min_price, const char * best_before, const char * group_code, const char* tax_group_id, bool add_flag)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackUpdateWareParameters(char *& pack_param, char *internal_code, char *longtitle, char *shortcut, char *price, char * quantity, char * auto_scheme_id, char * flags, char * min_price, char * best_before, char * group_code, char* tax_group_id, bool add_flag)\n");
	
  pack_param = (char *) calloc( strlen(longtitle) + strlen(shortcut) + strlen(price) + strlen(quantity) + strlen(auto_scheme_id) + strlen(flags) + strlen(min_price) + strlen(best_before) + strlen(group_code) + strlen(tax_group_id) + strlen("price = ") + strlen("quantity = quantity + ") + strlen("longtitle = ") + strlen("shortcut = ") + strlen("auto_scheme_id = ") + strlen("flags = ") + strlen("min_price = ") + strlen("best_before = ") + strlen("group_id = ") + strlen("tax_group_id = ") + 10 * strlen(PACK_DELIM) + 1, sizeof(char));

  strcat(pack_param, "longtitle = ");
  strcat(pack_param, longtitle);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, "shortcut = ");
  strcat(pack_param, shortcut);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, "price = ");
  strcat(pack_param, price);
  strcat(pack_param, PACK_DELIM);

  if(add_flag)
    strcat(pack_param, "quantity = quantity + ");
  else 
    strcat(pack_param, "quantity = ");
  strcat(pack_param, quantity);
  strcat(pack_param, PACK_DELIM);
  
  if(strcmp(auto_scheme_id, "null") !=0  && strcmp(auto_scheme_id, "0") != 0)
  {
    strcat(pack_param, "auto_scheme_id = ");
    strcat(pack_param, auto_scheme_id);
    strcat(pack_param, PACK_DELIM);
  }

  strcat(pack_param, "flags = ");
  strcat(pack_param, flags);
  strcat(pack_param, PACK_DELIM);

  if(strcmp(best_before, "null"))
  {
    strcat(pack_param, "best_before = ");
    strcat(pack_param, best_before);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(group_code, "null"))
  {
    strcat(pack_param, "group_id = ");
    strcat(pack_param, group_code);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(tax_group_id, "null"))
  {
    strcat(pack_param, "tax_group_id = ");
    strcat(pack_param, tax_group_id);
    strcat(pack_param, PACK_DELIM);
  }

// По принципу сохранения не могут быть null
  strcat(pack_param, "min_price = ");
  strcat(pack_param, min_price);
}

//==============================================================================//
void PgUpload::PackUpdateUserParameters(char *& pack_param, char * longtitle, char * shortcut, char * profile_code, char * pass, char * card)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackUpdateUserParameters(char *& pack_param, char * longtitle, char * shortcut, char * profile_code, char * pass, char * card)\n");

  pack_param = (char *) calloc(strlen("longtitle = ") + strlen("shortcut = ") + strlen("profile_id = ") + strlen("userpassword = ") + strlen("card_code = ") + strlen(longtitle) + strlen(shortcut) + strlen(profile_code) + strlen(pass) + strlen(card) + 4 * strlen(PACK_DELIM) + 1, sizeof(char));

  if(strcmp(profile_code, "null"))
  {
    strcat(pack_param, "profile_id = ");
    strcat(pack_param, profile_code); 
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(pass, "null"))
  {
    strcat(pack_param, "userpassword = ");
    strcat(pack_param, pass);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(card, "null"))
  {
    strcat(pack_param, "card_code = ");
    strcat(pack_param, card);
    strcat(pack_param, PACK_DELIM);
  }

// По принципу сохранения не могут быть null
  strcat(pack_param, "user_name = ");
  strcat(pack_param, longtitle);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, "shortcut = ");
  strcat(pack_param, shortcut);
}

//==============================================================================//
void PgUpload::PackUpdateFixReductionParameters(char *& pack_param, char * longtitle, char * shortcut, char * card_prefix, char * type, char * value, char * begin_date, char * end_date, char * area)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackUpdateFixReductionParameters(char *& pack_param, char * longtitle, char * shortcut, char * card_prefix, char * type, char * value, char * begin_date, char * end_date, char * area)\n");

  pack_param = (char *) calloc(strlen("longtitle = ") + strlen("shortcut = ") + strlen("card_prefix = ") + strlen("type = ") + strlen("value = ") + strlen("begindate = ") + strlen("enddate = ") + strlen("area = ") + strlen(longtitle) + strlen(shortcut) + strlen(card_prefix) + strlen(type) + strlen(value) + strlen(begin_date) + strlen(end_date) + strlen(area) + 7 * strlen(PACK_DELIM) + 1, sizeof(char));

  if(strcmp(card_prefix, "null"))
  {
    strcat(pack_param, "card_prefix = ");
    strcat(pack_param, card_prefix);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(type, "null"))
  {
    strcat(pack_param, "type = ");
    strcat(pack_param, type);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(area, "null"))
  {
    strcat(pack_param, "area = ");
    strcat(pack_param, area);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(begin_date, "null"))
  {
    strcat(pack_param, "begindate = ");
    strcat(pack_param, begin_date);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(end_date, "null"))
  {
    strcat(pack_param, "enddate = ");
    strcat(pack_param, end_date);
    strcat(pack_param, PACK_DELIM);
  }

// По принципу сохранения не могут быть null
  strcat(pack_param, "longtitle = ");
  strcat(pack_param, longtitle);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, "shortcut = ");
  strcat(pack_param, shortcut);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, "value = ");
  strcat(pack_param, value);
	
}

//==============================================================================//
void PgUpload::PackUpdateAutoSchemeParameters(char *& pack_param, char * longtitle, char * shortcut)
{
  //gsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackUpdateAutoSchemeParameters(char *& pack_param, char * longtitle, char * shortcut)\n");

  pack_param = (char *) calloc(strlen("longtitle = ") + strlen("shortcut = ") + strlen(longtitle) + strlen(shortcut) + strlen(PACK_DELIM) + 1, sizeof(char));

// По принципу сохранения не могут быть null
  strcat(pack_param, "longtitle = ");
  strcat(pack_param, longtitle);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, "shortcut = ");
  strcat(pack_param, shortcut);

}

//==============================================================================//
void PgUpload::PackUpdateAutoReductionParameters(char *& pack_param, char * scheme_code, char * longtitle, char * shortcut, char * type, char * value, char * begin_date, char * end_date, char * begin_time, char * end_time, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end)
{
//pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackUpdateAutoReductionParameters(char *& pack_param, char * scheme_code, char * longtitle, char * shortcut, char * type, char * value, char * begin_date, char * end_date, char * begin_time, char * end_time, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end)\n");

  pack_param = (char *) calloc(strlen("auto_scheme_id = ") + strlen("longtitle = ") + strlen("shortcut = ") + strlen("type = ") + strlen("value = ") + strlen("begindate = ") + strlen("enddate = ") + strlen("begintime = ") + strlen("endtime = ") + strlen("weekdays = ") + strlen("quantity = ") + strlen("position_sum = ") + strlen("check_sum = ") + strlen("card_prefix_begin = ") + strlen("card_prefix_end = ") + strlen(scheme_code) + strlen(longtitle) + strlen(shortcut) + strlen(type) + strlen(value) + strlen(begin_date) + strlen(end_date) + strlen(begin_time) + strlen(end_time) + strlen(weekdays) + strlen(quantity) + strlen(position_sum) + strlen(check_sum) + strlen(card_prefix_begin) + strlen(card_prefix_end) + 14 * strlen(PACK_DELIM) + 1, sizeof(char));

  if(strcmp(scheme_code, "null"))
  {
    strcat(pack_param, "auto_scheme_id = ");
    strcat(pack_param, scheme_code);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(type, "null"))
  {
    strcat(pack_param, "type = ");
    strcat(pack_param, type);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(begin_date, "null"))
  {
    strcat(pack_param, "begindate = ");
    strcat(pack_param, begin_date);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(end_date, "null"))
  {
    strcat(pack_param, "enddate = ");
    strcat(pack_param, end_date);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(begin_time, "null"))
  {
    strcat(pack_param, "begintime = ");
    strcat(pack_param, begin_time);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(end_time, "null"))
  {
    strcat(pack_param, "endtime = ");
    strcat(pack_param, end_time);
    strcat(pack_param, PACK_DELIM);
  }
	
  if(strcmp(weekdays, "null"))
  {
    strcat(pack_param, "weekdays = ");
    strcat(pack_param, weekdays);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(quantity, "null"))
  {
    strcat(pack_param, "quantity = ");
    strcat(pack_param, quantity);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(position_sum, "null"))
  {
    strcat(pack_param, "position_sum = ");
    strcat(pack_param, position_sum);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(check_sum, "null"))
  {
    strcat(pack_param, "check_sum = ");
    strcat(pack_param, check_sum);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(card_prefix_begin, "null"))
  {
    strcat(pack_param, "card_prefix_begin = ");
    strcat(pack_param, card_prefix_begin);
    strcat(pack_param, PACK_DELIM);
  }

  if(strcmp(card_prefix_end, "null"))
  {
    strcat(pack_param, "card_prefix_end = ");
    strcat(pack_param, card_prefix_end);
    strcat(pack_param, PACK_DELIM);
  }

// По принципу сохранения не могут быть null
  strcat(pack_param, "longtitle = ");
  strcat(pack_param, longtitle);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, "shortcut = ");
  strcat(pack_param, shortcut);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, "value = ");
  strcat(pack_param, value);	
}

//==============================================================================//
void PgUpload::PackUpdateTaxRateParameters(char *& pack_param, char * longtitle, char * shortcut, char * value)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackUpdateTaxRateParameters(char *& pack_param, char * longtitle, char * shortcut, char * value)\n");

  pack_param = (char *) calloc(strlen("longtitle = ") + strlen("shortcut = ") + strlen("value = ") + strlen(longtitle) + strlen(shortcut) + strlen(value) + 2 * strlen(PACK_DELIM) + 1, sizeof(char));
	
  strcat(pack_param, "longtitle = ");
  strcat(pack_param, longtitle);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, "shortcut = ");
  strcat(pack_param, shortcut);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, "value = ");
  strcat(pack_param, value);
}

//==============================================================================//
void PgUpload::PackUpdateTaxGroupParameters(char *& pack_param, char * longtitle, char * shortcut)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackUpdateTaxGroupParameters(char *& pack_param, char * longtitle, char * shortcut)\n");

  pack_param = (char *) calloc(strlen("longtitle = ") + strlen("shortcut = ") + strlen(longtitle) + strlen(shortcut) + strlen(PACK_DELIM) + 1, sizeof(char));

  strcat(pack_param, "longtitle = ");
  strcat(pack_param, longtitle);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, "shortcut = ");
  strcat(pack_param, shortcut);
}

//==============================================================================//
void PgUpload::PackUpdateTaxRateGroupParameters(char *& pack_param, char * group_code, char * rate_code, char * base_flag)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackUpdateTaxRateGroupParameters(char *& pack_param, char * group_code, char * rate_code, char * base_flag)\n");

  pack_param = (char *) calloc(strlen("tax_group_id = ") + strlen("tax_rate_id = ") + strlen("base_flag = ") + strlen(group_code) + strlen(rate_code) + strlen(base_flag) + 2 * strlen(PACK_DELIM) + 1, sizeof(char));

  if(strcmp(group_code, "null"))
  {
    strcat(pack_param, "tax_group_id = ");
    strcat(pack_param, group_code);
    strcat(pack_param, PACK_DELIM);
  }
  if(strcmp(rate_code, "null"))
  {					
    strcat(pack_param, "tax_rate_id = ");
    strcat(pack_param, rate_code);
    strcat(pack_param, PACK_DELIM);
  }
  if(strcmp(base_flag, "null"))
  {				
    strcat(pack_param, "base_flag = ");
    strcat(pack_param, base_flag);
  }
  if(*(pack_param + strlen(pack_param) - 1) == ' ')
    //Если на конце оказался разделитель PACK_DELIM = ", ", то его надо удалять
    *(pack_param + strlen(pack_param) - 2) = '\0';
}

//==============================================================================//
void PgUpload::PackInsertGroupParameters(char *& pack_param, const char *code, const char *longtitle, const char * shortcut, const char *group_code)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackInsertGroupParameters(char *& pack_param, char *code, char *longtitle,  char * shortcut, char *group_code)\n");

  pack_param = (char *) calloc(strlen(code) + strlen(longtitle) + strlen(shortcut) + strlen(group_code) + 3 * strlen(PACK_DELIM) + 1, sizeof(char));

  strcat(pack_param, code);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, longtitle);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, shortcut);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, group_code);
}

//==============================================================================//
void PgUpload::PackInsertWareParameters(char *& pack_param, const char *internal_code, const char *longtitle, const char *shortcut, const char *price, const char * quantity, const char * auto_scheme_id, const char * flags, const char * min_price, const char * best_before, const char * group_code, const char* tax_group_id)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackInsertWareParameters(char *& pack_param, char *internal_code, char *longtitle, char *shortcut, char *price, char * quantity, char * auto_scheme_id, char * flags, char * min_price, char * best_before, char * group_code, char* tax_group_id)\n");

    
  pack_param = (char *) calloc(strlen(internal_code) + strlen(longtitle) + strlen(shortcut) + strlen(price) + strlen(quantity) + strlen(auto_scheme_id) + strlen(flags) + strlen(min_price) + strlen(best_before) + strlen(group_code) + strlen(tax_group_id) + 11 * strlen(PACK_DELIM)  + 4 + 4, sizeof(char));

  
  strcat(pack_param, internal_code);
  strcat(pack_param, "'");
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, longtitle);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, shortcut);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, price);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, quantity);
  strcat(pack_param, PACK_DELIM);

  if ( strcmp(auto_scheme_id, "0") == 0 ) strcat(pack_param, "null");
  else strcat(pack_param, auto_scheme_id);
  
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, flags);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, min_price);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, best_before);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, group_code);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, tax_group_id);
}

//==============================================================================//
void PgUpload::PackInsertUserParameters(char *& pack_param, char * code, char * longtitle, char * shortcut, char * profile_code, char * pass, char * card)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackInsertUserParameters(char *& pack_param, char * code, char * longtitle, char * shortcut, char * profile_code, char * pass, char * card)\n");

  pack_param = (char *) calloc(strlen(code) + strlen(longtitle) + strlen(shortcut) + strlen(profile_code) + strlen(pass) + strlen(card) + 5 * strlen(PACK_DELIM) + 1, sizeof(char));

  strcat(pack_param, code);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, longtitle);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, shortcut);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, profile_code); 
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, pass);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, card);
}
//external_id, longtitle, shortcut, value, type, area, begindate, enddate, card_prefix
//==============================================================================//
void PgUpload::PackInsertFixReductionParameters(char *& pack_param, char * code, char * longtitle, char * shortcut, char * card_prefix, char * type, char * value, char * begin_date, char * end_date, char * area)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackInsertFixReductionParameters(char *& pack_param, char * code, char * longtitle, char * shortcut, char * card_prefix, char * type, char * value, char * begin_date, char * end_date, char * area)\n");

  pack_param = (char *) calloc(strlen(code) + strlen(longtitle) + strlen(shortcut) + strlen(card_prefix) + strlen(type) + strlen(value) + strlen(begin_date) + strlen(end_date) + strlen(area) + 8 * strlen(PACK_DELIM) + 1, sizeof(char));

  strcat(pack_param, code);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, longtitle);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, shortcut);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, value);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, type);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, area);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, begin_date);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, end_date);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, card_prefix);
}

//==============================================================================//
void PgUpload::PackInsertAutoSchemeParameters(char *& pack_param, char * code, char * longtitle, char * shortcut)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackInsertAutoSchemeParameters(char *& pack_param, char * code, char * longtitle, char * shortcut)\n");

  pack_param = (char *) calloc(strlen(code) + strlen(longtitle) + strlen(shortcut) + 2 * strlen(PACK_DELIM) + 1, sizeof(char));
  strcat(pack_param, code);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, longtitle);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, shortcut);

}

//auto_reduction_id, longtitle, shortcut, value, type, begindate, enddate, begintime, endtime, weekdays, quantity, position_sum, check_sum, auto_scheme_id,  card_prefix_begin, card_prefix_end
//==============================================================================//
void PgUpload::PackInsertAutoReductionParameters(char *& pack_param, char *code, char * scheme_code, char * longtitle, char * shortcut, char * type, char * value, char * begin_date, char * end_date, char * begin_time, char * end_time, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end)
{
//pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackInsertAutoReductionParameters(char *& pack_param, char *code, char * scheme_code, char * longtitle, char * shortcut, char * type, char * value, char * begin_date, char * end_date, char * begin_time, char * end_time, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end)\n");

  pack_param = (char *) calloc(strlen(code) + strlen(scheme_code) + strlen(longtitle) + strlen(shortcut) + strlen(type) + strlen(value) + strlen(begin_date) + strlen(end_date) + strlen(begin_time) + strlen(end_time) + strlen(weekdays) + strlen(quantity) + strlen(position_sum) + strlen(check_sum) + strlen(card_prefix_begin) + strlen(card_prefix_end) + 15 * strlen(PACK_DELIM) + 1, sizeof(char));

  strcat(pack_param, code);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, longtitle);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, shortcut);
  strcat(pack_param, PACK_DELIM);
	
  strcat(pack_param, value);
  strcat(pack_param, PACK_DELIM);	

  strcat(pack_param, type);
  strcat(pack_param, PACK_DELIM);
	
  strcat(pack_param, begin_date);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, end_date);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, begin_time);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, end_time);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, weekdays);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, quantity);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, position_sum);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, check_sum);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, scheme_code);
  strcat(pack_param, PACK_DELIM);
	
  strcat(pack_param, card_prefix_begin);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, card_prefix_end);
}

//tax_rate_id, longtitle, shortcut, value
//==============================================================================//
void PgUpload::PackInsertTaxRateParameters(char *& pack_param, char * code, char * longtitle, char * shortcut, char * value)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackInsertTaxRateParameters(char *& pack_param, char * code, char * longtitle, char * shortcut, char * value)\n");
	
  pack_param = (char *) calloc(strlen(code) + strlen(longtitle) + strlen(shortcut) + strlen(value) + 3 * strlen(PACK_DELIM) + 1, sizeof(char));

  strcat(pack_param, code);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, longtitle);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, shortcut);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, value);

}

//==============================================================================//
void PgUpload::PackInsertTaxGroupParameters(char *& pack_param, char * code, char * longtitle, char * shortcut)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackInsertTaxGroupParameters(char *& pack_param, char * code, char * longtitle, char * shortcut)\n");

  pack_param = (char *) calloc(strlen(code) + strlen(longtitle) + strlen(shortcut) + 2 * strlen(PACK_DELIM) + 1, sizeof(char));

  strcat(pack_param, code);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, longtitle);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, shortcut);
}

//==============================================================================//
void PgUpload::PackInsertTaxRateGroupParameters(char *& pack_param, char * code, char * group_code, char * rate_code, char * base_flag)
{
  //pgsql_onscreanlog.PrintString(PGSQL_ONSCREAN, INFO, "void PgUpload::PackInsertTaxRateGroupParameters(char *& pack_param, char * code, char * group_code, char * rate_code, char * base_flag)\n");

  pack_param = (char *) calloc(strlen(code) + strlen(group_code) + strlen(rate_code) + strlen(base_flag) + 3 * strlen(PACK_DELIM) + 1, sizeof(char));

  strcat(pack_param, code);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, group_code);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, rate_code);
  strcat(pack_param, PACK_DELIM);

  strcat(pack_param, base_flag);
}
//==============================================================================//



