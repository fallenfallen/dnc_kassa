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
#include <stdlib.h>
#include "trans_pgsql.h"
#include "reg_pgsql.h"
#include "const_conf.h"
#include "accessrights.h"


//extern SP sp;

//==================================================================================//
Trans::Trans()
{
    //puts("Trans::Trans()");
    Set_Db("Transaction");
    log->PrintString(log_type, INFO, "Trans::Trans() - Connect to db parameters DB name = Transaction \n");
}

//==================================================================================//
Trans::Trans(SP * sparam, const char * conf_path) : PGSQL(conf_path)
{
    //printf("Trans::Trans(SP, conf_path = %s)\n", conf_path);
    log->PrintString(log_type, INFO, "Trans::Trans - Connect to db parameters form path = %s \n", conf_path);
    sp = sparam;
}

//==================================================================================//
Trans::Trans(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, char* log_path) : PGSQL(dbname, user, password, host, hostaddr, port, log_path)
{
    //printf("Trans::Trans(dbname = <%s>, user = <%s>, password = <%s>, host = <%s>, hostaddr = <%s>, port = <%s>, log_path = <%s>)\n", dbname, user, password, host, hostaddr, port, log_path);
    log->PrintString(log_type, INFO, "Trans::Trans - Connect to db with  parameters : dbname = <%s>, user = <%s>, password = <%s>, host = <%s>, hostaddr = <%s>, port = <%s>, log_path = <%s>)\n", dbname, user, password, host, hostaddr, port, log_path);    
}

//==================================================================================//
Trans::~Trans() 
{
//puts("Trans::~Trans()");
  //close(log);
  //Close_Connection();
}


//==================================================================================//
int Trans::Get_Check_List(char *check, char *n_kkm, char**& dt, int& n, char** &n_doc)
// check - номер чека, n_kkm - номер кассы, n_fr - номер ФР
//d - массив для сохранения дат чеков с указанным номером и номером кассы,
//t - массив для сохранения времен чеков с указанным номером и номером кассы, 
//n - количество строк результата запроса
{
//puts("Trans::Get_Check_List(char *check, char *n_kkm, char**& dt, int& n)");
  int res = Query_by_Check_Number(check, n_kkm);
  if(res == 2)
  {
    n = PQntuples(result);
    if(n < 1)
    {
      PQclear(result);
      return 1;
    }
    dt = (char **) malloc(n*sizeof(char *));
    n_doc = ( char**  ) calloc(n, sizeof(char*));
    for(int i = 0; i < n; i ++)    {
	*(dt + i)= (char *) calloc(strlen(PQgetvalue(result, i, 0)) + 1, sizeof(char));
	strcpy(*(dt + i), PQgetvalue(result, i, 0));
	n_doc[i] = (char*) calloc(PQgetlength(result, i, 1) + 1, sizeof(char));
	strcpy(n_doc[i], PQgetvalue(result, i, 1));
    }
    PQclear(result);
//printf("end of Trans::Get_Check_List\n");
    return 0;
  }
  return -1;
}


//==================================================================================//
//returns :
// -1 - не удалось получить актуальные данные
//0 - все нормально
//1 - не полученно ни одной строки 
//2 - возврат одной из позиций чека не возможен, запрет в карточке товара
//6 - этот чек не является продажей  - возврат невозможен
//7 - нет связи с БД
//9 - это чек выплаты или внесения, нельзя напечатать копию
// 108 - содержится товар возврат которого  запрещен
//------------------------
// copy_fl = true: производится разбор для копии чека
//-----(*)-----Zuskin-----13/01/2012-----
//int Trans::Get_Registration_Check_Info(char *receipt, char *n_pc, char *datetime, char* n_doc, Receipt *rec, CopyInfo * copy, bool is_copy)  {
int Trans::Get_Registration_Check_Info(char *receipt, char *n_pc, char *datetime, char* n_doc, Receipt *rec, CopyInfo * copy, bool is_copy, int& manager_id)  {
//---------------------------------------    
    bool copy_fl = false;
    if ( copy != NULL ) copy_fl = true;
    
    int n = 0;
    
    if (copy_fl) {
	//printf("receipt cashier = %s \n", rec->getCashier());
	int state = Query_Get_Check_Cashier(receipt, n_pc, datetime, n_doc);
	//printf("state = %d \n", state);
	
	if (state < 0 ) return state;
	
	n = PQntuples(result);
	//printf("n = %d \n", n);
	
	char * seller_num = NULL;
	if (n == 1) {
	    seller_num = (char*) calloc(PQgetlength(result, 0, 0) + 1, sizeof(char));
	    strcpy(seller_num, PQgetvalue(result, 0, 0));
	    //printf("seller_num = %d \n", seller_num);
	}
	PQclear(result);
	
	char * seller = NULL;
	if ( seller_num != NULL ) {
	    AccessRights acc(LINCASH_CONF_PATH);
	    
	    acc.GetSellerName(seller_num, seller);
	    free(seller_num);
	    
	    if (seller != NULL) {
		rec->setCashier(seller);
		free(seller);
	    }
	}
	
	//printf("receipt cashier = %s \n", rec->getCashier());
    
    }
    
    
    // code value_1 , value_2 , tr_type, check_type, payment_type, card_code, n_doc, code_tax_group, code_tax_rate, n_kkm reduction_type
    //-----(*)-----Zuskin-----13/01/2012-----
    //Query_Info_of_Registration_Check(receipt, n_pc, datetime, n_doc);
    int state = Query_Info_of_Registration_Check(receipt, n_pc, datetime, n_doc);
    if ( state < 0) return -1;
    //---------------------------------------
        
    n = PQntuples(result);
    //printf("n = %d \n", n);
    if ( n < 1 )  {   PQclear(result);   return 1;      }
    
    int er = 0;
    int npos = 0;
    Registration Registr(sp, LINCASH_CONF_PATH);
    
    char *card = (char *) calloc(100, sizeof(char));
    int j = 0;
    
    
    // if (copy_fl) - производится разбор для копии чека
    
    
    if ( !copy_fl && (atoi(PQgetvalue(result, 0, 4)) != 0) )  {    PQclear(result);    return 6;  }
    if ( copy_fl && ( (atoi(PQgetvalue(result, 0, 4)) == 4)  || (atoi(PQgetvalue(result, 0, 4)) == 5)) ) { PQclear(result);    return 9;  }
    
    int taxrate_index = 0;
    TaxRate * tr;
    for ( int i = 0; i < n; i ++ )  {
	//printf("i = %d \n", i);
	switch ( atoi(PQgetvalue(result, i, 3)) ) {   // tr_type
	case 11 : 
	    // зарегистрированная позиция    
	    //Registr.New_Receipt_Record(/*code*/, /*quantity*/, rec, /*code_fl*/, /*return_fl*/, /*copy_fl*/, npos)
	    //printf("new receipt position (%s, %d)\n", PQgetvalue(result, i, 0), atof(PQgetvalue(result, i, 2)));
	    //printf("PQgetvalue(result, i, 12) = %s \n", PQgetvalue(result, i, 12));
	    //-----(*)-----Zuskin-----14/10/2011-----
	    //er = Registr.New_Receipt_Record(PQgetvalue(result, i, 0), atof(PQgetvalue(result, i, 2)), rec, false, !is_copy, is_copy, npos, PQgetvalue(result, i, 12));
	    er = Registr.New_Receipt_Record(PQgetvalue(result, i, 0), atof(PQgetvalue(result, i, 2)), rec, false, !is_copy, is_copy, npos, PQgetvalue(result, i, 12), true);
	    //---------------------------------------
	    printf("er = %d \n", er);
	    
	    if ( er != 0 )     {
		PQclear(result);
		// запрет на возврат в карточке товара
		//-----(*)-----Zuskin-----25/07/2011-----
		//if ( !is_copy && (er == 2) )   return 108;  
		if ( !is_copy && (er == 2) )   return 2;  
		//---------------------------------------
		else   return -1;        
	    }
	    
	    rec->GetPosElemRef(rec->record_num - 1)->price = atof(PQgetvalue(result, i, 1));
	    
	    //Для новой позиции сбрасываем счетчик налоговых ставок, 
	    taxrate_index = 0;
	    rec->Add_Taxes_Info(0, "", 0);
	    break;
	    
	case 15 : // Скидка суммой на позицию
	    switch(atoi(PQgetvalue(result,i,11)))        {
	    case 0 : j = 2; strcpy(card, ""); break;
	    case 1 : j = 1; strcpy(card, PQgetvalue(result, i, 6)); break;
	    case 2 : j = 0; strcpy(card, ""); break;
	    case 3 : j = 3; strcpy(card, ""); break;
	    }
	    rec->GetPosElemRef(npos)->Add_Sum_Discount(j, atof(PQgetvalue(result, i, 1)), PQgetvalue(result, i, 0), "", "", false, 0);
	    break;
	case 16 : // Надбавка суммой на позицию
	    switch(atoi(PQgetvalue(result,i,11)))        {
	    case 0 : j = 2; strcpy(card, ""); break;
	    case 1 : j = 1; strcpy(card, PQgetvalue(result, i, 6)); break;
	    case 2 : j = 0; strcpy(card, ""); break;
	    case 3 : j = 3; strcpy(card, ""); break;
	    }
	    rec->GetPosElemRef(npos)->Add_Sum_Discount(j, -atof(PQgetvalue(result, i, 1)), PQgetvalue(result, i, 0), "", "", false, 0);
	    break;
	case 17 : // Скидка процентная на позицию
	    switch(atoi(PQgetvalue(result,i,11)))        {
	    case 0 : j = 2; strcpy(card, ""); break;
	    case 1 : j = 1; strcpy(card, PQgetvalue(result, i, 6)); break;
	    case 2 : j = 0; strcpy(card, ""); break;
	    case 3 : j = 3; strcpy(card, ""); break;
	    }
	    rec->GetPosElemRef(npos)->Add_Percent_Discount(j, atof(PQgetvalue(result, i, 2)), PQgetvalue(result, i, 0), "", "", false, 0);
	    break;
	case 18 : //  Надбавка процентная на позицию
	    switch(atoi(PQgetvalue(result,i,11)))        {
	    case 0 : j = 2; strcpy(card, ""); break;
	    case 1 : j = 1; strcpy(card, PQgetvalue(result, i, 6)); break;
	    case 2 : j = 0; strcpy(card, ""); break;
	    case 3 : j = 3; strcpy(card, ""); break;
	    }
	    rec->GetPosElemRef(npos)->Add_Percent_Discount(j, -atof(PQgetvalue(result, i, 2)), PQgetvalue(result, i, 0), "", "", false, 0);
	    break;
	case 35 : // Скидка суммой на чек
	    switch(atoi(PQgetvalue(result,i,11)))     {
	    case 0 : j = 2; strcpy(card, ""); break;
	    case 1 : j = 1; strcpy(card, PQgetvalue(result, i, 6)); break;
	    case 2 : j = 0; strcpy(card, ""); break;
	    case 3 : j = 3; strcpy(card, ""); break;
	    }
	    rec->Add_Sum_Discount(j, atof(PQgetvalue(result, i, 1)), PQgetvalue(result, i, 0), "", "", false);
	    break;
	case 36 : // Надбавка суммой на чек
	    switch(atoi(PQgetvalue(result,i,11)))    {
	    case 0 : j = 2; strcpy(card, ""); break;
	    case 1 : j = 1; strcpy(card, PQgetvalue(result, i, 6)); break;
	    case 2 : j = 0; strcpy(card, ""); break;
	    case 3 : j = 3; strcpy(card, ""); break;
	    }
	    rec->Add_Sum_Discount(j, -atof(PQgetvalue(result, i, 1)), PQgetvalue(result, i, 0), "", "", false);
	    break;
	case 37 : // Скидка процентная  на чек
	    switch(atoi(PQgetvalue(result,i,11)))   {
	    case 0 : j = 2; strcpy(card, ""); break;
	    case 1 : j = 1; strcpy(card, PQgetvalue(result, i, 6)); break;
	    case 2 : j = 0; strcpy(card, ""); break;
	    case 3 : j = 3; strcpy(card, ""); break;
	    }
	    rec->Add_Percent_Discount(j, atof(PQgetvalue(result, i, 2)), PQgetvalue(result, i, 0), "", "", false);
	    break;
	case 38 :  // Надбавка процентная на чек
	    switch(atoi(PQgetvalue(result,i,11)))   {
	    case 0 : j = 2; strcpy(card, ""); break;
	    case 1 : j = 1; strcpy(card, PQgetvalue(result, i, 6)); break;
	    case 2 : j = 0; strcpy(card, ""); break;
	    case 3 : j = 3; strcpy(card, ""); break;
	    }
	    rec->Add_Percent_Discount(j, -atof(PQgetvalue(result, i, 2)), PQgetvalue(result, i, 0), "", "", false);
	    break;
	case 14 : 
	    // в модели в результате будет хранится информация о налогах, известная из транзакций
	    //при этом объект Positiontaxes хранит группу налогов из транзакций, а объект Record_Receipt группу налогов из справочника товара	
	    //далее необходим анализ на согласованность данных и получение из справочников более полной информации о налогах.
	    if ( taxrate_index == 0 )	rec->GetTaxElemRef(rec->record_num - 1)->tax_group_id = atoi(PQgetvalue(result, i, 8));
	    
	    rec->GetTaxElemRef(rec->record_num - 1)->num_rates = taxrate_index + 1; // будет меняться с каждой итерацией
	    taxrate_index++;
	    
	    // создадстся объект с идетификатором и значением налога, остальное по умолчанию.
	    tr = new TaxRate(atoi(PQgetvalue(result, i, 9)), atof(PQgetvalue(result, i, 1))); 
	    rec->GetTaxElemRef(rec->record_num - 1)->Add_Tax_Rate(*tr);
	    break;
	case 40 : 
	case 41 :
	    if ( copy_fl )
		switch(atoi(PQgetvalue(result, i, 5)) ) { 
	              case 1 :
			  copy->SetNalSum(atof(PQgetvalue(result, i, 1)));
			  break;
		      case 2 :
			  copy->SetCredSum(atof(PQgetvalue(result, i, 1)));
			  break;
		      case 3 :
			  copy->SetContSum(atof(PQgetvalue(result, i, 1)));
			  break;
		      }
	    break;
	case 55 :
	    if ( copy_fl ) {  
		copy->SetClientSum(atof(PQgetvalue(result, i, 1)));
		
		copy->SetCheckSum(atof(PQgetvalue(result, i, 2)));
	    }
	    //-----(+)-----Zuskin-----13/01/2012-----
	    manager_id = atoi(PQgetvalue(result, i, 13));
	    //---------------------------------------
	    break;
	}
    }    
    free(card);
    
    	//printf("copy->GetCheckSum() = %f \n", copy->GetCheckSum());
	//printf("copy->SetNalSum() = %f \n", copy->GetNalSum());
	//printf("copy->GetCredSum() = %f \n", copy->GetCredSum());
    
    //создадим копию заполненной модели, удалим из нее информацию о налогах и заполним заново из справочника товаров
    //puts("^^^^^^^^^^");
    Receipt * test = new Receipt(*rec);
    test->Clear_Taxes();
    
    for ( int i = 0; i < test->record_num; i++ )  {
	er = Registr.Add_Taxes(test, i);
	if ( er != 0 )    break;  
    }
    
    if ( er == 0 )  {
	er = AnalizeTaxes(rec, test);
	
	if ( er == 0 ) 
	    if ( copy_fl )  copy->SaveInfo(receipt, PQgetvalue(result, 0, 7), PQgetvalue(result, 0, 10), "30", datetime, atoi(PQgetvalue(result, 0, 4)));
    }
    delete(test);
    
    PQclear(result);

    return er;
}

//==================================================================================//
int Trans::AnalizeTaxes(Receipt * rec, Receipt * test)
{
    //puts("int Trans::AnalizeTaxes(Receipt *rec, Receipt * test)");
    /*
    for (int i =0 ; i < rec->record_num; i++) {
	puts("------- tax from trans --------");
	for (int j = 0; j < rec->GetTaxElemRef(i)->num_rates; j++)	{
	    printf("pos = %d tax = %d tax_rate_id = %d \n", i, j, rec->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->rate_id);
	}
	puts("------- -------------------- --------");
    }
    
    for (int i =0 ; i < test->record_num; i++) {
	puts("------- tax from lincash --------");
	for (int j = 0; j < test->GetTaxElemRef(i)->num_rates; j++)	{
	    printf("pos = %d tax = %d tax_rate_id = %d \n", i, j, test->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->rate_id);
	}
	puts("------- -------------------- --------");
    }
    */
    for ( int i = 0; i < rec->record_num; i++ )  {
	if ( rec->GetTaxElemRef(i)->tax_group_id != test->GetTaxElemRef(i)->tax_group_id ) 	    return -1;
	
	if ( rec->GetTaxElemRef(i)->num_rates != test->GetTaxElemRef(i)->num_rates )    return -1;
	
	for ( int j = 0; j < rec->GetTaxElemRef(i)->num_rates; j ++ )        {
	    if ( rec->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->rate_id != test->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->rate_id ) return -1;
	    else   {
		rec->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->change_base = test->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->change_base;
		rec->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->longtitle = test->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->longtitle;
		rec->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->shortcut = test->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->shortcut;
	    }
	}
    }
    return 0;
}

//==================================================================================//
int Trans::Get_Chage_Opened(char *&dt, const char * serial)
{
//puts("int Trans::Get_ChageOpened(char *dt)");
 int res = Query_ChageOpened_DateTime(serial);
  if(res == 2)  {
    int n = PQntuples(result);
    if(n > 0)    {
      dt = (char *) calloc(PQgetlength(result, 0, 0) + 1, sizeof(char));
      strcpy(dt, PQgetvalue(result, 0, 0));
      PQclear(result);
      return 0;
    }
    return 1;
  }
  return -1;
}

//==================================================================================//
//дата в формате ISO, DMY, то есть 01-12-2005 
#define QUERY_CHECKNUMBER "SELECT DISTINCT tr_date_time, n_doc FROM Transaction WHERE tr_type = 55 AND n_check = "
#define QUERY_CHECKNUMBER_PC " AND n_pc = "
//#define QUERY_CHECKNUMBER_KKM " AND n_kkm = "
int Trans::Query_by_Check_Number(char* check, char * n_pc)
{
//puts("Trans::Query_by_Check_Number(char* check, char * n_pc)");
  int state;
  char *str;
  str = (char *) calloc(strlen(QUERY_CHECKNUMBER) + strlen(QUERY_CHECKNUMBER_PC) /*+ strlen(QUERY_CHECKNUMBER_KKM)*/ + strlen(check) + strlen(n_pc) /* + strlen(n_kkm)*/ + 1, sizeof(char));

  strcpy(str, QUERY_CHECKNUMBER);
  strcat(str, check);
  strcat(str, QUERY_CHECKNUMBER_PC);
  strcat(str, n_pc);
//  strcat(str, QUERY_CHECKNUMBER_KKM);
//  strcat(str, n_kkm);

  state = doSQL(myconnection, str);
//printf("%s\n state %d\n", str, state);

  free(str);
  return state;  
}

//==================================================================================//
// дата в формате ISO, DMY, то есть 01-12-2005 
//                                   price   quantity
//-----(*)-----Zuskin-----13/01/2012-----
//#define QUERY_REG_INFO "SELECT code, value_1, value_2, tr_type, check_type, payment_type, card_code, n_doc, code_tax_group, code_tax_rate, n_kkm, reduction_type, reduction_info FROM Transaction WHERE n_check = "
#define QUERY_REG_INFO "SELECT code, value_1, value_2, tr_type, check_type, payment_type, card_code, n_doc, code_tax_group, code_tax_rate, n_kkm, reduction_type, reduction_info, manager FROM Transaction WHERE n_check = "
//---------------------------------------
#define QUERY_INFO_DATATIME " AND tr_date_time = '" 
#define QUERY_INFO_PC "' AND n_pc = "
#define QUERY_INFO_NDOC " AND n_doc = "
//#define QUERY_INFO_KKM " AND n_kkm = "
int Trans::Query_Info_of_Registration_Check(char *receipt, char *n_pc,/* char *n_kkm,*/ char *datetime, char* n_doc)
{
//puts("Trans::Query_Info_of_Registration_Check(char *receipt, char *n_pc, char *n_kkm, char *datetime)");
  char *str;
  int state;   

  str = (char *) calloc(strlen(QUERY_REG_INFO) + strlen(QUERY_INFO_DATATIME) + strlen(QUERY_INFO_PC) + /*strlen(QUERY_INFO_KKM) +*/ strlen(receipt) + strlen(n_pc) /*+ strlen(n_kkm)*/ + strlen(datetime) + strlen(QUERY_INFO_NDOC) + strlen(n_doc) + 1, sizeof(char));
  
  strcpy(str, QUERY_REG_INFO);
  strcat(str, receipt);
  strcat(str, QUERY_INFO_DATATIME);
  strcat(str, datetime);
  strcat(str, QUERY_INFO_PC);
  strcat(str, n_pc);
  strcat(str, QUERY_INFO_NDOC);
  strcat(str, n_doc);
 // strcat(str, QUERY_INFO_KKM);
//  strcat(str, n_kkm);

  state = doSQL(myconnection, str);
  free(str);
  
  return state;
}
//###############################################__INSERTS__##########################################################//

#define INSERT_REG "INSERT INTO Transaction(tr_type, tr_date_time, n_pc, n_kkm, seller, n_session, n_check, n_doc, check_type, code, barcode, value_1, value_2, reduction_info, round_sum, payment_type) VALUES ("
//==================================================================================//
int Trans::Insert_Registration(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * code, const char* barcode, const char * price, const char * quantity, const char* addition_id, const char* result_position_sum, const char* section)
{
    //printf("Insert_Registration(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * code = %s, const char* barcode = %s, const char * price, const char * quantity, const char* addition_id = %s) \n", code , barcode, addition_id);
    
  char * str;
  int state;  

  str = (char *) calloc(strlen(INSERT_REG) + strlen(type) + strlen(datetime) + strlen(n_pc) + strlen(n_kkm) + strlen(seller) + strlen(n_change) + strlen(n_receipt) + strlen(n_doc) + strlen(check_type) + strlen(code) + strlen(barcode) + strlen(price) + strlen(quantity) + strlen(addition_id) + strlen(result_position_sum) + strlen(section) + 63, sizeof(char));
  strcpy(str, INSERT_REG);
  strcat(str, type);
  strcat(str, ", CAST ('");
  strcat(str, datetime);
  strcat(str, "' AS TIMESTAMP), "); // 26
  strcat(str, n_pc);
  strcat(str, ", ");
  strcat(str, n_kkm);
  strcat(str, ", ");
  strcat(str, seller);
  strcat(str, ", ");
  strcat(str, n_change);
  strcat(str, ", ");
  strcat(str, n_receipt);
  strcat(str, ", ");
  strcat(str, n_doc);
  strcat(str, ", ");
  strcat(str, check_type);
  strcat(str, ", '");
  strcat(str, code);
  
  strcat(str, "', '");
  strcat(str, barcode);
  strcat(str, "', ");
  strcat(str, price);
  strcat(str, ", ");
  strcat(str, quantity);
  strcat(str, ", ");
  strcat(str, addition_id);
  strcat(str, ", ");
  strcat(str, result_position_sum);
  strcat(str, ", ");
  strcat(str, section);
  strcat(str, ")");
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}


//ДОБАВИТЬ КОД ТОВАРА!
#define INSERT_TAX "INSERT INTO Transaction(tr_type, tr_date_time, n_pc, n_kkm, seller, n_session, n_check, n_doc, check_type, code, code_tax_group, code_tax_rate, value_1) VALUES ("
//==================================================================================//
int Trans::Insert_Tax(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * w_code, const char * gr_code, const char * rate_code, const char * sum)
{
//puts("Trans::Insert_Tax(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * w_code, const char * gr_code, const char * rate_code, const char * sum)");
  char * str;
  int state;   

  str = (char *) calloc(strlen(INSERT_TAX) + strlen(type) + strlen(datetime) + strlen(n_pc) + strlen(n_kkm) + strlen(seller) + strlen(n_change) + strlen(n_receipt) + strlen(n_doc) + strlen(check_type) + strlen(w_code) + strlen(gr_code) + strlen(rate_code) + strlen(sum) + 50, sizeof(char));
  strcpy(str, INSERT_TAX);
  strcat(str, type);
  strcat(str, ", CAST ('");
  strcat(str, datetime);
  strcat(str, "' AS TIMESTAMP), ");
  strcat(str, n_pc);
  strcat(str, ", ");
  strcat(str, n_kkm);
  strcat(str, ", ");
  strcat(str, seller);
  strcat(str, ", ");
  strcat(str, n_change);
  strcat(str, ", ");
  strcat(str, n_receipt);
  strcat(str, ", ");
  strcat(str, n_doc);
  strcat(str, ", ");
  strcat(str, check_type);
  strcat(str, ", '");
  strcat(str, w_code);
  strcat(str, "', ");
  strcat(str, gr_code);
  strcat(str, ", ");
  strcat(str, rate_code);
  strcat(str, ", ");
  strcat(str, sum);
  strcat(str, ")");

  state = doSQL(myconnection, str);
 
  free(str);
  return state;
}

// round_sum - сумма пересчитанной скидки (recount_sum)
// round_percent - процент пересчитанной скидки (recount_percent)

#define INSERT_REDUCTION "INSERT INTO Transaction(tr_type, tr_date_time, n_pc, n_kkm, seller, n_session, n_check, n_doc, check_type, code, reduction_type, reduction_info, auto_scheme_id, value_1, value_2, round_sum, gain) VALUES ("
//==================================================================================//
int Trans::Insert_Reduction(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * w_code, const char * reduction_type, const char * reduction_info, const char * auto_scheme_id, const char * sum, const char * rate, const char* recount_sum, const char * recount_percent)
{
//puts("Trans::Insert_Reduction(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * w_code, const char * reduction_type, const char * reduction_info, const char * auto_scheme_id, const char * sum, const char * rate)");
  char * str;
  int state;

  str = (char *) calloc(strlen(INSERT_REDUCTION) + strlen(type) + strlen(datetime) + strlen(n_pc) + strlen(n_kkm) + strlen(seller) + strlen(n_change) + strlen(n_receipt) + strlen(n_doc) + strlen(check_type) + strlen(w_code) + strlen(reduction_type) + strlen(reduction_info) + strlen(auto_scheme_id) + strlen(sum) + strlen(rate) + strlen(recount_percent) + strlen(recount_sum) + 66, sizeof(char));
  strcpy(str, INSERT_REDUCTION);
  strcat(str, type);
  strcat(str, ", CAST ('");             ///9
  strcat(str, datetime);
  strcat(str, "' AS TIMESTAMP), ");     ///17
  strcat(str, n_pc);
  strcat(str, ", ");
  strcat(str, n_kkm);
  strcat(str, ", ");
  strcat(str, seller);
  strcat(str, ", ");
  strcat(str, n_change);
  strcat(str, ", ");
  strcat(str, n_receipt);
  strcat(str, ", ");
  strcat(str, n_doc);
  strcat(str, ", ");
  strcat(str, check_type);
  strcat(str, ", '");
  strcat(str, w_code);
  strcat(str, "', ");
  strcat(str, reduction_type);
  strcat(str, ", '");
  strcat(str, reduction_info);
  strcat(str, "', ");
  strcat(str, auto_scheme_id);
  strcat(str, ", ");
  strcat(str, sum);
  strcat(str, ", ");                 ///24
  strcat(str, rate);
  strcat(str, ", ");
  strcat(str, recount_sum);
  strcat(str, ", ");
  strcat(str, recount_percent);
  strcat(str, ")");
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state;
}

#define INSERT_PAYMENT "INSERT INTO Transaction(tr_type, tr_date_time, n_pc, n_kkm, seller, n_session, n_check, n_doc, check_type, payment_type, value_1, value_2) VALUES ("
//==================================================================================//
int Trans::Insert_Payment(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * payment_type, const char * sum, const char * change)
{
//puts("Trans::Insert_Payment(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * payment_type, const char * sum, const char * change)");
  char * str;
  int state;   

  str = (char *) calloc(strlen(INSERT_PAYMENT) + strlen(type) + strlen(datetime) + strlen(n_pc) + strlen(n_kkm) + strlen(seller) + strlen(n_change) + strlen(n_receipt) + strlen(n_doc) + strlen(check_type) + strlen(payment_type) + strlen(sum) + strlen(change) + 46, sizeof(char));
  strcpy(str, INSERT_PAYMENT);
  strcat(str, type);
  strcat(str, ", CAST ('");
  strcat(str, datetime);
  strcat(str, "' AS TIMESTAMP), ");
  strcat(str, n_pc);
  strcat(str, ", ");
  strcat(str, n_kkm);
  strcat(str, ", ");
  strcat(str, seller);
  strcat(str, ", ");
  strcat(str, n_change);
  strcat(str, ", ");
  strcat(str, n_receipt);
  strcat(str, ", ");
  strcat(str, n_doc);
  strcat(str, ", ");
  strcat(str, check_type);
  strcat(str, ", ");
  strcat(str, payment_type);
  strcat(str, ", ");
  strcat(str, sum);
  strcat(str, ", ");
  strcat(str, change);
  strcat(str, ")");

  state = doSQL(myconnection, str);
 
  free(str);
  return state;
}

#define INSERT_INOUTCOME "INSERT INTO Transaction(tr_type, tr_date_time, n_pc, n_kkm, seller, n_session, n_check, n_doc, check_type, value_1, payment_type) VALUES ("
//==================================================================================//
int Trans::Insert_InOutcome(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * sum, const char* section_number)
{
//puts("int Trans::Insert_InOutcome(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * sum)");
  char * str;
  int state;   

  str = (char *) calloc(strlen(INSERT_INOUTCOME) + strlen(type) + strlen(datetime) + strlen(n_pc) + strlen(n_kkm) + strlen(seller) + strlen(n_change) + strlen(n_receipt) + strlen(n_doc) + strlen(check_type) + strlen(sum) + strlen(section_number) + 45, sizeof(char));
  strcpy(str, INSERT_INOUTCOME);
  strcat(str, type);
  strcat(str, ", CAST ('");
  strcat(str, datetime);
  strcat(str, "' AS TIMESTAMP), ");
  strcat(str, n_pc);
  strcat(str, ", ");
  strcat(str, n_kkm);
  strcat(str, ", ");
  strcat(str, seller);
  strcat(str, ", ");
  strcat(str, n_change);
  strcat(str, ", ");
  strcat(str, n_receipt);
  strcat(str, ", ");
  strcat(str, n_doc);
  strcat(str, ", ");
  strcat(str, check_type);
  strcat(str, ", ");
  strcat(str, sum);
  strcat(str, ", ");
  strcat(str, section_number);
  strcat(str, ")");

  state = doSQL(myconnection, str);
 
  free(str);
  return state;
}

//-----(*)-----Zuskin-----18/11/2011-----
//#define INSERT_CLOSE_CHECK "INSERT INTO Transaction(tr_type, tr_date_time, n_pc, n_kkm, seller, n_session, n_check, n_doc, check_type, card_code, value_1, value_2, round_sum) VALUES ("
#define INSERT_CLOSE_CHECK "INSERT INTO Transaction(tr_type, tr_date_time, n_pc, n_kkm, seller, n_session, n_check, n_doc, check_type, card_code, value_1, value_2, round_sum, manager) VALUES ("
//---------------------------------------
//==================================================================================//
//int Trans::Insert_Close_Check(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * card, const char  * client_sum, const char * check_sum, const char* round_sum)
int Trans::Insert_Close_Check(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * card, const char  * client_sum, const char * check_sum, const char* round_sum, string manager_id)
{
//puts("int Trans::Insert_Close_Check(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * card, const char  * client_sum, const char * check_sum)");
  //-----(*)-----Zuskin-----18/11/2011-----
  /*char * str;
  int state;   

  str = (char *) calloc(strlen(INSERT_CLOSE_CHECK) + strlen(type) + strlen(datetime) + strlen(n_pc) + strlen(n_kkm) + strlen(seller) + strlen(n_change) + strlen(n_receipt) + strlen(n_doc) + strlen(check_type) + strlen(card) + strlen(client_sum) + strlen(check_sum) + strlen(round_sum) + 50, sizeof(char));
  strcpy(str, INSERT_CLOSE_CHECK);
  strcat(str, type);
  strcat(str, ", CAST ('");
  strcat(str, datetime);
  strcat(str, "' AS TIMESTAMP), ");
  strcat(str, n_pc);
  strcat(str, ", ");
  strcat(str, n_kkm);
  strcat(str, ", ");
  strcat(str, seller);
  strcat(str, ", ");
  strcat(str, n_change);
  strcat(str, ", ");
  strcat(str, n_receipt);
  strcat(str, ", ");
  strcat(str, n_doc);
  strcat(str, ", ");
  strcat(str, check_type);
  strcat(str, ", '");
  strcat(str, card);
  strcat(str, "', ");
  strcat(str, client_sum);
  strcat(str, ", ");
  strcat(str, check_sum);
  strcat(str, ", ");
  strcat(str, round_sum);
  strcat(str, ")"); */
  
  string str;
  str += INSERT_CLOSE_CHECK;
  str += type;
  str += ", CAST ('";
  str += datetime;
  str += "' AS TIMESTAMP), ";
  str += n_pc;
  str += ", ";
  str += n_kkm;
  str += ", ";
  str += seller;
  str += ", ";
  str +=  n_change;
  str += ", ";
  str += n_receipt;
  str += ", ";
  str += n_doc;
  str += ", ";
  str += check_type;
  str += ", '";
  str += card;
  str += "', ";
  str += client_sum;
  str += ", ";
  str += check_sum;
  str += ", ";
  str += round_sum;
  str += ", ";
  if (manager_id.length() == 0) {
    str += "null";
  }
  else
    str += manager_id;
  str += ")";
    
  state = doSQL(myconnection, str.c_str());
 
  //free(str);
  return state;
}

#define INSERT_CHANGE_DATA "INSERT INTO Transaction(tr_type, tr_date_time, n_pc, n_kkm, seller, n_session, n_doc, gain, value_1, value_2) VALUES ("
//==================================================================================//
int Trans::Insert_Change_Data(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_doc, const char * gain_sum, const char * cash_sum, const char * total_sum)
{
//puts("int Trans::Insert_Change_Data(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_doc, const char * gain_sum, const char * cash_sum, const char * total_sum)");
  char * str;
  int state;   

  str = (char *) calloc(strlen(INSERT_CHANGE_DATA) + strlen(type) + strlen(datetime) + strlen(n_pc) + strlen(n_kkm) + strlen(seller) + strlen(n_change) + strlen(n_doc)  + strlen(gain_sum) + strlen(cash_sum) + strlen(total_sum) + 42, sizeof(char));
  strcpy(str, INSERT_CHANGE_DATA);
  strcat(str, type);
  strcat(str, ", CAST ('");
  strcat(str, datetime);
  strcat(str, "' AS TIMESTAMP), ");
  strcat(str, n_pc);
  strcat(str, ", ");
  strcat(str, n_kkm);
  strcat(str, ", ");
  strcat(str, seller);
  strcat(str, ", ");
  strcat(str, n_change);
  strcat(str, ", ");
  strcat(str, n_doc);
  strcat(str, ", ");
  strcat(str, gain_sum);
  strcat(str, ", ");
  strcat(str, cash_sum);
  strcat(str, ", ");
  strcat(str, total_sum);
  strcat(str, ")");
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state;
}

#define QUERY_TIME_CHANGE_OPENED "SELECT tr_date_time from Transaction WHERE n_transaction = (SELECT MAX(n_transaction) FROM Transaction WHERE (tr_type = 62) AND (n_kkm = '"
//==================================================================================//
int Trans::Query_ChageOpened_DateTime(const char * serial)
{
//puts("int Trans::Query_ChageOpened_DateTime(const char * serial)");
  int state;
  char *str;
  str = (char *) calloc(strlen(QUERY_TIME_CHANGE_OPENED) + strlen(serial) + 4, sizeof(char));
  strcpy(str, QUERY_TIME_CHANGE_OPENED);
  strcat(str, serial);
  strcat(str, "'))");
  
  
  state = doSQL(myconnection, str);
 
  free(str);
  return state;
}


// возвращает количество закрытых чеков ( по check_type и по коду кассира )
#define QUERY_COUNT_CHECK2 "SELECT tr.n_transaction FROM transaction_by_last_session tr WHERE tr.tr_type = 55"

// возвращает количество чеков выплат и внесений ( check_type = 4 или 5 и по коду кассира )
#define QUERY_COUNT_CHECK1 "SELECT tr.n_transaction FROM transaction_by_last_session tr WHERE true "

// возвращает сумму закрытых чеков ( по check_type и по коду кассира )
#define QUERY_RESULT_SUM2 "SELECT SUM(tr.value_2) FROM transaction_by_last_session tr WHERE tr.tr_type = 55"

// возвращает сумму чеков выплат и внесений ( check_type = 4 или 5 и по коду кассира )
#define QUERY_RESULT_SUM1 "SELECT SUM(tr.value_1) FROM transaction_by_last_session tr WHERE true "

#define CHECK_TYPE " AND tr.check_type = "
#define SELLER_NUM " AND tr.seller = "

//==================================================================================//
int Trans::Get_Session_Info( const char* check_type, const char* seller_num, int &count_check, double & result_sum )
{
    int len = 0;
    char * str = NULL;
    int state = -1;
    
    //на количество чеков
    if ( strcmp(check_type, "4") == 0 || strcmp(check_type, "5") == 0 ) len = strlen(QUERY_COUNT_CHECK1);
    else len = strlen(QUERY_COUNT_CHECK2);
    if ( strlen(check_type) > 0 ) len += strlen(CHECK_TYPE) + strlen(check_type);
    if ( strlen(seller_num) > 0 ) len += strlen( SELLER_NUM ) + strlen(seller_num);
    
    str = (char*) calloc( len + 1, sizeof(char) );
    
    if ( strcmp(check_type, "4") == 0 || strcmp(check_type, "5") == 0 ) strcat(str, QUERY_COUNT_CHECK1);
    else strcat(str, QUERY_COUNT_CHECK2);
    if ( strlen(check_type) > 0 )  {	strcat(str, CHECK_TYPE);  strcat(str, check_type);   }
    if ( strlen(seller_num) > 0 )  {    strcat(str, SELLER_NUM);  strcat(str, seller_num);   }
    
    state = doSQL(myconnection, str);
    
    free(str);
    
    if ( state != 2 ) return -1;
    count_check = PQntuples(result);
    PQclear(result);
    
    // на общую сумму
    if ( strcmp(check_type, "4") == 0 || strcmp(check_type, "5") == 0 )  len = strlen(QUERY_RESULT_SUM1);
    else len = strlen(QUERY_RESULT_SUM2);
    if ( strlen(check_type) > 0 ) len += strlen(CHECK_TYPE) + strlen(check_type);
    if ( strlen(seller_num) > 0 ) len += strlen( SELLER_NUM ) + strlen(seller_num);
    
    str = (char*) calloc( len + 1, sizeof(char) );
    
    if ( strcmp(check_type, "4") == 0 || strcmp(check_type, "5") == 0 ) strcat(str, QUERY_RESULT_SUM1);
    else strcat(str, QUERY_RESULT_SUM2);
    if ( strlen(check_type) > 0 )  {	strcat(str, CHECK_TYPE);  strcat(str, check_type);   }
    if ( strlen(seller_num) > 0 )  {    strcat(str, SELLER_NUM);  strcat(str, seller_num);   }
    
    
    state = doSQL(myconnection, str);
    
    free(str);
    
    if ( state != 2 ) return -1;
    
    if ( PQntuples(result) != 1 ) {
	PQclear(result);
	return -1;
    }
    
    result_sum = atof(PQgetvalue(result, 0, 0));
    PQclear(result);
    
    return 0;
}

// возвращает суммы по типам оплаты
#define QUERY_PAYMENT_SUM "SELECT SUM(tr.value_1 - tr.value_2) FROM transaction_by_last_session tr WHERE (tr.tr_type = 40 OR tr.tr_type = 41) "

#define PAYMENT_TYPE " AND tr.payment_type = "
//==================================================================================//
int Trans::Query_Payment_Sum(const char* check_type, const char* seller_num, const char* payment_type)
{
    int len = 0;
    char * str = NULL;
    int state = -1;
    
    len = strlen(QUERY_PAYMENT_SUM);
    if ( strlen(check_type) > 0 ) len += strlen(CHECK_TYPE) + strlen(check_type);
    if ( strlen(seller_num) > 0 ) len += strlen( SELLER_NUM ) + strlen(seller_num);
    len += strlen(PAYMENT_TYPE) + strlen(payment_type);
    
    str = (char*) calloc( len + 1, sizeof(char) );
    
    strcat(str, QUERY_PAYMENT_SUM);
    if ( strlen(check_type) > 0 )  {	strcat(str, CHECK_TYPE);  strcat(str, check_type);   }
    if ( strlen(seller_num) > 0 )  {    strcat(str, SELLER_NUM);  strcat(str, seller_num);   }
    strcat(str, PAYMENT_TYPE);
    strcat(str, payment_type);
    
    //printf("srt = %s \n", str);
    state = doSQL(myconnection, str);
    free(str);
    
    return state;
}
//==================================================================================//
int Trans::Get_Session_Payment_Info( const char* check_type, const char* seller_num, double &payment_sum1, double &payment_sum2, double &payment_sum3 )
{
    int state = Query_Payment_Sum(check_type, seller_num, "1");
    if ( state != 2 ) return -1;
    if ( PQntuples(result) != 1 ) {	PQclear(result);	return -1;    }
    payment_sum1 = atof(PQgetvalue(result, 0, 0));
    PQclear(result);
    
    state = Query_Payment_Sum(check_type, seller_num, "2");
    if ( state != 2 ) return -1;
    if ( PQntuples(result) != 1 ) {	PQclear(result);	return -1;    }
    payment_sum2 = atof(PQgetvalue(result, 0, 0));
    PQclear(result);
    
    state = Query_Payment_Sum(check_type, seller_num, "3");
    if ( state != 2 ) return -1;
    if ( PQntuples(result) != 1 ) {	PQclear(result);	return -1;    }
    payment_sum3 = atof(PQgetvalue(result, 0, 0));
    PQclear(result);
    
    return 0;
}

// возвращает список кассиров участвующих в текущей смене
#define QUERY_SELLER_BY_SESSION "SELECT DISTINCT tr.seller FROM transaction_by_last_session tr"
//==================================================================================//
int Trans::Get_Cashier_By_Session(char ** & users, int &count_users )
{
    int state = doSQL(myconnection, QUERY_SELLER_BY_SESSION);
    
    if ( state != 2 ) return -1;
    
    count_users = PQntuples(result);
    
    if ( count_users > 0 ) users = ( char** ) calloc(count_users, sizeof(char*));
    
    for( int i = 0; i < count_users; i++ ) {
	users[i] = (char*) calloc(PQgetlength(result, i, 0) + 1, sizeof(char) );
	strcpy(users[i], PQgetvalue(result, i, 0));
    }
        
    PQclear(result);
    return 0;
}


//==================================================================================//
#define QUERY_GET_CHECK_CASHIER "SELECT DISTINCT seller FROM Transaction WHERE n_check = "
int Trans::Query_Get_Check_Cashier(char *receipt, char *n_pc, char *datetime, char* n_doc)
{
    char *str;
    int state;   
    
    str = (char *) calloc(strlen(QUERY_GET_CHECK_CASHIER) + strlen(QUERY_INFO_DATATIME) + strlen(QUERY_INFO_PC) + strlen(receipt) + strlen(n_pc) + strlen(datetime) + strlen(QUERY_INFO_NDOC) + strlen(n_doc) + 1, sizeof(char));
    
    strcpy(str, QUERY_GET_CHECK_CASHIER);
    strcat(str, receipt);
    strcat(str, QUERY_INFO_DATATIME);
    strcat(str, datetime);
    strcat(str, QUERY_INFO_PC);
    strcat(str, n_pc);
    strcat(str, QUERY_INFO_NDOC);
    strcat(str, n_doc);	
    
    
    state = doSQL(myconnection, str);
    free(str);
    return state;
}

//==================================================================================//	
//возвращает сумму скидок количество и сумму скидок (по кассирам)
#define QUERY_GET_POSITION_DISCOUNT "SELECT COUNT(*) as count, SUM(value_1) as sum FROM transaction_with_closed_check_by_last_session tr WHERE tr.check_type = 0 AND (tr.tr_type = 15 OR tr.tr_type = 17) " 

#define QUERY_GET_POSITION_CHARGE "SELECT COUNT(*) as count, SUM(value_1) as sum FROM transaction_with_closed_check_by_last_session tr WHERE tr.check_type = 0 AND (tr.tr_type = 16 OR tr.tr_type = 18) " 

#define QUERY_GET_CHECK_DISCOUNT "SELECT COUNT(*) as count, SUM(value_1) as sum FROM transaction_with_closed_check_by_last_session tr WHERE tr.check_type = 0 AND (tr.tr_type = 35 OR tr.tr_type = 37) " 

#define QUERY_GET_CHECK_CHARGE "SELECT COUNT(*) as count, SUM(value_1) as sum FROM transaction_with_closed_check_by_last_session tr WHERE tr.check_type = 0 AND (tr.tr_type = 36 OR tr.tr_type = 38) " 

int Trans::Get_Cashier_Discount_Info(const char* seller_num, int &pos_discount_count, double &pos_discount_sum, int &pos_charge_count, double &pos_charge_sum, int &check_discount_count, double &check_discount_sum, int &check_charge_count, double &check_charge_sum) 
{
    
    char str[2000];
    
    /*****************************/
    strcpy(str, QUERY_GET_POSITION_DISCOUNT);
    if ( strlen(seller_num) > 0 ) { strcat(str, SELLER_NUM); strcat(str, seller_num); }
    
    int state = doSQL(myconnection, str);
    
    if ( state != 2 ) return -1;
    
    pos_discount_count = atoi(PQgetvalue(result, 0, 0));
    pos_discount_sum = atof(PQgetvalue(result, 0, 1));
    
    PQclear(result);
    
    /*****************************/
    strcpy(str, QUERY_GET_POSITION_CHARGE);
    if ( strlen(seller_num) > 0 ) { strcat(str, SELLER_NUM); strcat(str, seller_num); }
    
    state = doSQL(myconnection, str);
    
    if ( state != 2 ) return -1;
    
    pos_charge_count = atoi(PQgetvalue(result, 0, 0));
    pos_charge_sum = atof(PQgetvalue(result, 0, 1));
    
    PQclear(result);    
    
    /*****************************/
    strcpy(str, QUERY_GET_CHECK_DISCOUNT);
    if ( strlen(seller_num) > 0 ) { strcat(str, SELLER_NUM); strcat(str, seller_num); }
    
    state = doSQL(myconnection, str);
    
    if ( state != 2 ) return -1;
    
    check_discount_count = atoi(PQgetvalue(result, 0, 0));
    check_discount_sum = atof(PQgetvalue(result, 0, 1));
    
    PQclear(result);
    
    /*****************************/    
    strcpy(str, QUERY_GET_CHECK_CHARGE);
    if ( strlen(seller_num) > 0 ) { strcat(str, SELLER_NUM); strcat(str, seller_num); }
    
    state = doSQL(myconnection, str);
    
    if ( state != 2 ) return -1;
    
    check_charge_count = atoi(PQgetvalue(result, 0, 0));
    check_charge_sum = atof(PQgetvalue(result, 0, 1));
    
    PQclear(result);        
    
    return 0;
}
	
//==================================================================================//
// возвращает список кодов товаров проданных и возвращенных в тек. смене
#define GET_WARES "SELECT tr.code FROM transaction_with_closed_check_by_last_session tr WHERE tr.tr_type = 11 GROUP BY code"
int Trans::GetSessionWare(char** & ware_codes, int & ware_count) {
    
    char str[2000];
    
    strcpy(str, GET_WARES);
    int state = doSQL(myconnection, str);
    if ( state != 2 ) return -1;
    
    ware_count = PQntuples(result);
    ware_codes = (char**)calloc(ware_count, sizeof(char*));
    
    for ( int i = 0; i < ware_count; i++ ) {
	ware_codes[i] = (char*) calloc(PQgetlength(result, i, 0) + 1, sizeof(char));
	strcpy(ware_codes[i], PQgetvalue(result, i, 0));
    }
    
    PQclear(result);
    return 0;
}

//==================================================================================//
#define GET_WARE_INFO "SELECT tr.code , SUM(tr.value_2),  SUM(tr.round_sum) FROM transaction_with_closed_check_by_last_session tr WHERE tr.tr_type = 11 AND check_type = 0 AND code = '"
#define GET_WARE_INFO2 "' GROUP BY code"
int Trans::GetWareInfo(const char* wcode, double & ware_quantity, double & ware_sum) {
    
    char str[2000];
    
    strcpy(str, GET_WARE_INFO);
    strcat(str, wcode);
    strcat(str, GET_WARE_INFO2);
    
    //printf("GetWareInfo str = %s \n", str);
    
    int state = doSQL(myconnection, str);
    if ( state != 2 ) return -1;
    
    if ( PQntuples(result) != 1 )  {
	ware_quantity = 0;
	ware_sum = 0;
    } else {
	ware_quantity = atof(PQgetvalue(result, 0, 1));
	ware_sum = atof(PQgetvalue(result, 0, 2));
    }
    
    PQclear(result);
    return 0;    
}	
//==================================================================================//
#define GET_RETURN_WARE_INFO "SELECT tr.code , SUM(tr.value_2), SUM(tr.round_sum) FROM transaction_with_closed_check_by_last_session tr WHERE tr.tr_type = 11 AND check_type = 1 AND code = '"
#define GET_RETURN_WARE_INFO2 "' GROUP BY code"
int Trans::GetReturnWareInfo(const char* wcode, double & ware_quantity, double & ware_sum) {
    
    char str[2000];
    
    strcpy(str, GET_RETURN_WARE_INFO);
    strcat(str, wcode);
    strcat(str, GET_RETURN_WARE_INFO2);
    
    //printf("GetWareInfo str = %s \n", str);
    
    int state = doSQL(myconnection, str);
    if ( state != 2 ) return -1;
    
    if ( PQntuples(result) != 1 ) {
	ware_quantity = 0;
	ware_sum = 0;
    } else {
	ware_quantity = atof(PQgetvalue(result, 0, 1));
	ware_sum = atof(PQgetvalue(result, 0, 2));
    }
    
    PQclear(result);
    return 0;    
}	
//==================================================================================//    
#define GET_SALE_SUM_BY_WARE "SELECT COUNT(*) as count, SUM(tr.round_sum) as sum, tr.check_type FROM  transaction_with_closed_check_by_last_session tr WHERE tr.tr_type = 11 GROUP BY check_type "

int Trans::GetResultSumByWare(int &sale_count, double &sale_sum, int &return_count, double &return_sum) {
    
    puts("int Trans::GetResultSumByWare");
    
    char* str = (char*) calloc(strlen(GET_SALE_SUM_BY_WARE) + 10, sizeof(char));
    
    strcpy(str, GET_SALE_SUM_BY_WARE);
    
    int state = doSQL(myconnection, str);
    free(str);
    if ( state != 2 ) return -1;
    
    sale_count = 0;
    sale_sum = 0;
    
    return_count = 0;
    return_sum = 0;
    
    int n = PQntuples(result);
    
    if ( n == 1 ) {
	if ( atoi(PQgetvalue(result, 0, 2)) == 0 ) {
	    sale_count = atoi(PQgetvalue(result, 0, 0));
	    sale_sum = atof(PQgetvalue(result, 0, 1));    
	} else {
	    return_count = atoi(PQgetvalue(result, 0, 0));
	    return_sum = atof(PQgetvalue(result, 0, 1));
	}
    } else if ( n == 2 ) {
	sale_count = atoi(PQgetvalue(result, 0, 0));
	sale_sum = atof(PQgetvalue(result, 0, 1));
	
	return_count = atoi(PQgetvalue(result, 1, 0));
	return_sum = atof(PQgetvalue(result, 1, 1));
    }
    PQclear(result);
    return 0;
}
//==================================================================================//   
#define GET_POS_DISCOUNT_BY_WARE "SELECT COUNT(*), SUM(value_1) as sum, check_type FROM transaction_with_closed_check_by_last_session tr WHERE  ( tr.tr_type = 15 OR  tr.tr_type = 17 ) GROUP BY check_type "

#define GET_POS_CHARGE_BY_WARE "SELECT COUNT(*), SUM(value_1) as sum, check_type FROM transaction_with_closed_check_by_last_session tr WHERE  ( tr.tr_type = 16 OR  tr.tr_type = 18 ) GROUP BY check_type "

#define GET_CHECK_DISCOUNT_BY_WARE "SELECT COUNT(*), SUM(value_1) as sum, check_type FROM transaction_with_closed_check_by_last_session tr WHERE  ( tr.tr_type = 35 OR  tr.tr_type = 37 ) GROUP BY check_type "

#define GET_CHECK_CHANRGE_BY_WARE "SELECT COUNT(*), SUM(value_1) as sum, check_type FROM transaction_with_closed_check_by_last_session tr WHERE  ( tr.tr_type = 36 OR  tr.tr_type = 38 ) GROUP BY check_type "

int Trans::GetDiscountInfoByWare(int &pos_discount_count, int &check_discount_count, double &pos_discount_sum, double &check_discount_sum, double &total_sale_discount_sum){  
    
    pos_discount_count = 0;
    pos_discount_sum = 0;
    
    check_discount_count = 0;
    check_discount_sum = 0;
    
    total_sale_discount_sum = 0;
    
    char* str = (char*) calloc(strlen(GET_POS_DISCOUNT_BY_WARE) + 10, sizeof(char));
    strcpy(str, GET_POS_DISCOUNT_BY_WARE);
    
    int state = doSQL(myconnection, str);
    
    if ( state != 2 ) {  free(str);  return -1;  }
    
    int n = PQntuples(result);
    
    for ( int i = 0; i < n; i++ ) {
	pos_discount_count += atoi(PQgetvalue(result, i, 0));
	if ( atoi(PQgetvalue(result, i, 2)) == 0 ) {
	    total_sale_discount_sum += atof(PQgetvalue(result, i, 1));
	    pos_discount_sum += atof(PQgetvalue(result, i, 1));
	} else pos_discount_sum -= atof(PQgetvalue(result, i, 1)); 
    }
    
    PQclear(result);
    
    /********************************/
    
    strcpy(str, GET_POS_CHARGE_BY_WARE);
    
    state = doSQL(myconnection, str);
    
    if ( state != 2 ) {  free(str);  return -1;  }
    
    n = PQntuples(result);
    
    for ( int i = 0; i < n; i++ ) {
	pos_discount_count += atoi(PQgetvalue(result, i, 0));
	if ( atoi(PQgetvalue(result, i, 2)) == 0 ) {
	    total_sale_discount_sum -= atof(PQgetvalue(result, i, 1));	    
	    pos_discount_sum -= atof(PQgetvalue(result, i, 1));
	} else pos_discount_sum += atof(PQgetvalue(result, i, 1)); 
    }
    
    PQclear(result);
    
    /********************************/
    
    strcpy(str, GET_CHECK_DISCOUNT_BY_WARE);
    
    state = doSQL(myconnection, str);
    
    if ( state != 2 ) {  free(str);  return -1;  }
    
    n = PQntuples(result);
    
    for ( int i = 0; i < n; i++ ) {
	check_discount_count += atoi(PQgetvalue(result, i, 0));
	if ( atoi(PQgetvalue(result, i, 2)) == 0 )  {
	    total_sale_discount_sum += atof(PQgetvalue(result, i, 1));
	    check_discount_sum += atof(PQgetvalue(result, i, 1));
	} else check_discount_sum -= atof(PQgetvalue(result, i, 1)); 
    }
    
    PQclear(result);
    
    /********************************/
    
    strcpy(str, GET_CHECK_CHANRGE_BY_WARE);
    
    state = doSQL(myconnection, str);
    
    if ( state != 2 ) {  free(str);  return -1;  }
    
    n = PQntuples(result);
    
    for ( int i = 0; i < n; i++ ) {
	check_discount_count += atoi(PQgetvalue(result, i, 0));
	if ( atoi(PQgetvalue(result, i, 2)) == 0 ) {
	    total_sale_discount_sum -= atof(PQgetvalue(result, i, 1));
	    check_discount_sum -= atof(PQgetvalue(result, i, 1));
	} else check_discount_sum += atof(PQgetvalue(result, i, 1)); 
    }
    
    PQclear(result);
    free(str);
    return 0;
}
//==================================================================================//    
#define GET_RESULT_ROUND_SUM "SELECT SUM(round_sum), check_type FROM transaction_by_last_session WHERE tr_type = 55 GROUP BY check_type"
int Trans::GetResultRoundSum(double &sale_round_sum, double &return_round_sum) {
    
    sale_round_sum = 0;
    return_round_sum = 0;
    
    
    char* str = (char*) calloc(strlen(GET_RESULT_ROUND_SUM) + 1, sizeof(char));
    strcpy(str, GET_RESULT_ROUND_SUM);
    
    int state = doSQL(myconnection, str);
    
    if ( state != 2 ) {  free(str);  return -1;  }
    
    int n = PQntuples(result);
    
    for ( int i = 0; i < n; i++ ) {
	if ( atoi(PQgetvalue(result, i, 1)) == 0 ) {
	    // чек продажи
	    sale_round_sum += atof(PQgetvalue(result, i, 0));
	} else {
	    // чеки возвратов
	    return_round_sum += atof(PQgetvalue(result, i, 0));	    
	}
    }
    free(str);
    PQclear(result);
    return 0;
}

//==================================================================================// 

//-----(+)-----Zuskin-----03/05/2011-----
#define GET_INCOME_BY_SECTION "SELECT SUM(value_1) FROM transaction_by_last_session WHERE tr_type = 50"
int Trans::GetIncomeSumBySection(const char* section, double &income_sum) {
    
    char* str;
    income_sum = 0;
    
    int query_len = strlen(GET_INCOME_BY_SECTION) + 1;
            
    if (strlen(section)>0) {
	query_len += strlen(" AND payment_type = ") + strlen(section);     
    }
    str = (char*) calloc (query_len, sizeof(char));
    strcpy(str, GET_INCOME_BY_SECTION);
    if (strlen(section)>0) {
	strcat(str, " AND payment_type = ");
	strcat(str, section);     
    }
    
//    printf("QUERY(%s)\n", str);
    
    int state = doSQL(myconnection, str);
    if ( state != 2 ) {  free(str);  return -1;  }
    
    int n = PQntuples(result);
    income_sum = atof(PQgetvalue(result, 0, 0));    
    PQclear(result);
    free(str);
    return 0;

}

#define GET_OUTCOME_BY_SECTION "SELECT SUM(value_1) FROM transaction_by_last_session WHERE tr_type = 51"
int Trans::GetOutcomeSumBySection(const char* section, double &outcome_sum) {
    char* str;
    outcome_sum = 0;
    
    int query_len = strlen(GET_OUTCOME_BY_SECTION) + 1;
            
    if (strlen(section)>0) {
	query_len += strlen(" AND payment_type = ") + strlen(section);     
    }
    str = (char*) calloc (query_len, sizeof(char));
    strcpy(str, GET_OUTCOME_BY_SECTION);
    if (strlen(section)>0) {
	strcat(str, " AND payment_type = ");
	strcat(str, section);     
    }
    
    int state = doSQL(myconnection, str);
    if ( state != 2 ) {  free(str);  return -1;  }
    
    int n = PQntuples(result);
    outcome_sum = atof(PQgetvalue(result, 0, 0));    
    PQclear(result);
    free(str);
    return 0;

}

#define GET_SALES_BY_SECTION "SELECT SUM(round_sum) FROM transaction_by_last_session WHERE tr_type = 11 AND check_type = 0 AND tr_date_time NOT IN (SELECT tr_date_time FROM transaction_by_last_session WHERE tr_type = 56)"
#define GET_SALES_ROUNDS_BY_SECTION "SELECT SUM(tr2.round_sum), tr3.payment_type FROM transaction_by_last_session AS tr2,(SELECT DISTINCT tr1.tr_date_time, MIN(tr1.payment_type) AS payment_type, MAX(tr1.round_sum) AS round_sum FROM transaction_by_last_session AS tr1, (SELECT tr_date_time, MAX(round_sum) AS max_sum FROM transaction_by_last_session WHERE tr_type = 11 AND check_type = 0 GROUP BY tr_date_time) AS tr WHERE tr1.tr_date_time = tr.tr_date_time AND tr1.round_sum = tr.max_sum GROUP BY tr1.tr_date_time) AS tr3 WHERE tr2.tr_type = 55 AND tr2.check_type = 0 AND tr2.tr_date_time = tr3.tr_date_time"
#define SALES_ROUNDS_END " GROUP BY tr3.payment_type"

int Trans::GetSalesSumBySection(const char* section, double &sales_sum) {
    char* str;
    sales_sum = 0;
    
    int query_len = strlen(GET_SALES_BY_SECTION) + 1;
            
    if (strlen(section)>0) {
	query_len += strlen(" AND payment_type = ") + strlen(section);     
    }
    str = (char*) calloc (query_len, sizeof(char));
    strcpy(str, GET_SALES_BY_SECTION);
    if (strlen(section)>0) {
	strcat(str, " AND payment_type = ");
	strcat(str, section);     
    }
    
    int state = doSQL(myconnection, str);
    if ( state != 2 ) {  free(str);  return -1;  }
    
    int n = PQntuples(result);
    sales_sum = atof(PQgetvalue(result, 0, 0));    
    PQclear(result);
    free(str);
    
    double round_sum = 0;
    query_len = strlen(GET_SALES_ROUNDS_BY_SECTION) + 1;
            
    if (strlen(section)>0) {
	query_len += strlen(" AND tr3.payment_type = ") + strlen(section);
    }
    query_len += strlen(SALES_ROUNDS_END);

    str = (char*) calloc (query_len, sizeof(char));
    strcpy(str, GET_SALES_ROUNDS_BY_SECTION);
    if (strlen(section)>0) {
	strcat(str, " AND tr3.payment_type = ");
	strcat(str, section);     
    }
    strcat(str, SALES_ROUNDS_END);

    state = doSQL(myconnection, str);
    if ( state != 2 ) {  free(str);  return -1;  }
    
    n = PQntuples(result);
    if(n < 1)
    {
      PQclear(result);
      free(str);
      return 0;
    }
    round_sum = atof(PQgetvalue(result, 0, 0));    
    PQclear(result);
    sales_sum -= round_sum; 
    free(str);
    return 0;

}

#define GET_RETURNS_BY_SECTION "SELECT SUM(round_sum) FROM transaction_by_last_session WHERE tr_type = 11 AND check_type = 1 AND tr_date_time NOT IN (SELECT tr_date_time FROM transaction_by_last_session WHERE tr_type = 56)"
#define GET_RETURNS_ROUNDS_BY_SECTION "SELECT SUM(tr2.round_sum), tr3.payment_type FROM transaction_by_last_session AS tr2,(SELECT DISTINCT tr1.tr_date_time, MIN(tr1.payment_type) AS payment_type, MAX(tr1.round_sum) AS round_sum FROM transaction_by_last_session AS tr1, (SELECT tr_date_time, MAX(round_sum) AS max_sum FROM transaction_by_last_session WHERE tr_type = 11 AND check_type = 1 GROUP BY tr_date_time) AS tr WHERE tr1.tr_date_time = tr.tr_date_time AND tr1.round_sum = tr.max_sum GROUP BY tr1.tr_date_time) AS tr3 WHERE tr2.tr_type = 55 AND tr2.check_type = 1 AND tr2.tr_date_time = tr3.tr_date_time"
#define RETURNS_ROUNDS_END " GROUP BY tr3.payment_type"

int Trans::GetReturnsSumBySection(const char* section, double &return_sum) {
    char* str;
    return_sum = 0;
    
    int query_len = strlen(GET_RETURNS_BY_SECTION) + 1;
            
    if (strlen(section)>0) {
	query_len += strlen(" AND payment_type = ") + strlen(section);     
    }
    str = (char*) calloc (query_len, sizeof(char));
    strcpy(str, GET_RETURNS_BY_SECTION);
    if (strlen(section)>0) {
	strcat(str, " AND payment_type = ");
	strcat(str, section);     
    }
    
    int state = doSQL(myconnection, str);
    if ( state != 2 ) {  free(str);  return -1;  }
    
    int n = PQntuples(result);
    return_sum = atof(PQgetvalue(result, 0, 0));    
    PQclear(result);
    free(str);
    
    double round_sum = 0;
    query_len = strlen(GET_RETURNS_ROUNDS_BY_SECTION) + 1;
            
    if (strlen(section)>0) {
	query_len += strlen(" AND tr3.payment_type = ") + strlen(section);
    }
    query_len += strlen(RETURNS_ROUNDS_END);

    str = (char*) calloc (query_len, sizeof(char));
    strcpy(str, GET_RETURNS_ROUNDS_BY_SECTION);
    if (strlen(section)>0) {
	strcat(str, " AND tr3.payment_type = ");
	strcat(str, section);     
    }
    strcat(str, RETURNS_ROUNDS_END);

    state = doSQL(myconnection, str);
    if ( state != 2 ) {  free(str);  return -1;  }
    
    n = PQntuples(result);
    if(n < 1)
    {
      PQclear(result);
      free(str);
      return 0;
    }
    round_sum = atof(PQgetvalue(result, 0, 0));    
    PQclear(result);
    return_sum -= round_sum; 
    free(str);
    return 0;   
}


#define GET_CANCELED_BY_SECTION "SELECT SUM(round_sum) FROM transaction_by_last_session WHERE tr_type = 11 AND check_type = 0 AND tr_date_time IN (SELECT tr_date_time FROM transaction_by_last_session WHERE tr_type = 56)"
int Trans::GetCanceledSumBySection(const char* section, double &cancel_sum) {
    char* str;
    cancel_sum = 0;
    
    int query_len = strlen(GET_CANCELED_BY_SECTION) + 1;
            
    if (strlen(section)>0) {
	query_len += strlen(" AND payment_type = ") + strlen(section);     
    }
    str = (char*) calloc (query_len, sizeof(char));
    strcpy(str, GET_CANCELED_BY_SECTION);
    if (strlen(section)>0) {
	strcat(str, " AND payment_type = ");
	strcat(str, section);     
    }
    
    int state = doSQL(myconnection, str);
    if ( state != 2 ) {  free(str);  return -1;  }
    
    int n = PQntuples(result);
    cancel_sum = atof(PQgetvalue(result, 0, 0));    
    PQclear(result);
    free(str);
    return 0;

}

#define GET_STORNO_BY_SECTION "SELECT SUM(round_sum) FROM transaction_by_last_session WHERE tr_type = 12 AND check_type = 0 AND tr_date_time NOT IN (SELECT tr_date_time FROM transaction_by_last_session WHERE tr_type = 56)"
int Trans::GetStornoSumBySection(const char* section, double & storno_sum) {
    char* str;
    storno_sum = 0;
    
    int query_len = strlen(GET_STORNO_BY_SECTION) + 1;
            
    if (strlen(section)>0) {
	query_len += strlen(" AND payment_type = ") + strlen(section);     
    }
    str = (char*) calloc (query_len, sizeof(char));
    strcpy(str, GET_STORNO_BY_SECTION);
    if (strlen(section)>0) {
	strcat(str, " AND payment_type = ");
	strcat(str, section);     
    }
    
    int state = doSQL(myconnection, str);
    if ( state != 2 ) {  free(str);  return -1;  }
    
    int n = PQntuples(result);
    storno_sum = atof(PQgetvalue(result, 0, 0));    
    PQclear(result);
    free(str);
    return 0;

}

//-----(+)-----Zuskin-----06/06/2011-----
#define GET_CANCELED_COUNT_BY_CASHIER "SELECT count(n_transaction) FROM transaction_by_last_session WHERE tr_type = 56 AND check_type = 0"
int Trans::GetCanceledCountByCashier(const char* cashier, double &cancel_count) {
    char* str;
    cancel_count = 0;
    
    int query_len = strlen(GET_CANCELED_COUNT_BY_CASHIER) + 1;
            
    if (strlen(cashier)>0) {
	query_len += strlen(" AND seller = ") + strlen(cashier);     
    }
    str = (char*) calloc (query_len, sizeof(char));
    strcpy(str, GET_CANCELED_COUNT_BY_CASHIER);
    if (strlen(cashier)>0) {
	strcat(str, " AND seller = ");
	strcat(str, cashier);     
    }
    
    int state = doSQL(myconnection, str);
    if ( state != 2 ) {  free(str);  return -1;  }
    
    int n = PQntuples(result);
    cancel_count = atof(PQgetvalue(result, 0, 0));    
    PQclear(result);
    free(str);
    return 0;

}

#define GET_CANCELED_SUM_BY_CASHIER "SELECT SUM(value_2) FROM transaction_by_last_session WHERE tr_type = 56 AND check_type = 0"
int Trans::GetCanceledSumByCashier(const char* cashier, double &cancel_sum) {
    char* str;
    cancel_sum = 0;
    
    int query_len = strlen(GET_CANCELED_SUM_BY_CASHIER) + 1;
            
    if (strlen(cashier)>0) {
	query_len += strlen(" AND seller = ") + strlen(cashier);     
    }
    str = (char*) calloc (query_len, sizeof(char));
    strcpy(str, GET_CANCELED_SUM_BY_CASHIER);
    if (strlen(cashier)>0) {
	strcat(str, " AND seller = ");
	strcat(str, cashier);     
    }
    
    int state = doSQL(myconnection, str);
    if ( state != 2 ) {  free(str);  return -1;  }
    
    int n = PQntuples(result);
    cancel_sum = atof(PQgetvalue(result, 0, 0));    
    PQclear(result);
    free(str);
    return 0;

}

#define GET_STORNO_COUNT_BY_CASHIER "SELECT count(n_transaction) FROM transaction_by_last_session WHERE tr_type = 12 AND check_type = 0 AND tr_date_time NOT IN (SELECT tr_date_time FROM transaction_by_last_session WHERE tr_type = 56)"
int Trans::GetStornoCountByCashier(const char* cashier, double &storno_count) {
    char* str;
    storno_count = 0;
    
    int query_len = strlen(GET_STORNO_COUNT_BY_CASHIER) + 1;
            
    if (strlen(cashier)>0) {
	query_len += strlen(" AND seller = ") + strlen(cashier);     
    }
    str = (char*) calloc (query_len, sizeof(char));
    strcpy(str, GET_STORNO_COUNT_BY_CASHIER);
    if (strlen(cashier)>0) {
	strcat(str, " AND seller = ");
	strcat(str, cashier);     
    }
    
    int state = doSQL(myconnection, str);
    if ( state != 2 ) {  free(str);  return -1;  }
    
    int n = PQntuples(result);
    storno_count = atof(PQgetvalue(result, 0, 0));    
    PQclear(result);
    free(str);
    return 0;

}

#define GET_STORNO_SUM_BY_CASHIER "SELECT SUM(round_sum) FROM transaction_by_last_session WHERE tr_type = 12 AND check_type = 0 AND tr_date_time NOT IN (SELECT tr_date_time FROM transaction_by_last_session WHERE tr_type = 56)"
int Trans::GetStornoSumByCashier(const char* cashier, double &storno_sum) {
    char* str;
    storno_sum = 0;
    
    int query_len = strlen(GET_STORNO_SUM_BY_CASHIER) + 1;
            
    if (strlen(cashier)>0) {
	query_len += strlen(" AND seller = ") + strlen(cashier);     
    }
    str = (char*) calloc (query_len, sizeof(char));
    strcpy(str, GET_STORNO_SUM_BY_CASHIER);
    if (strlen(cashier)>0) {
	strcat(str, " AND seller = ");
	strcat(str, cashier);     
    }
    
    int state = doSQL(myconnection, str);
    if ( state != 2 ) {  free(str);  return -1;  }
    
    int n = PQntuples(result);
    storno_sum = atof(PQgetvalue(result, 0, 0));    
    PQclear(result);
    free(str);
    return 0;

}


//----------------------------------------

//-----(+)-----Zuskin-----24/10/2011-----
#define QUERY_PUTOUT_CHECKS "SELECT DISTINCT n_pc, n_check, tr_date_time, n_doc, check_type, value_2 FROM Transaction WHERE tr_type = 57"
int Trans::Get_PutOut_Checks(vector<string>* npclist, vector<string>* checklist, vector<string>* date_time, vector<string>* ndoc, vector<string>* chtype, vector<string>* summ, string check, string n_pc)
{
  int state;
  string str = QUERY_PUTOUT_CHECKS;
  if (check.length() > 0) {
    str += " AND n_check = ";
    str += check;
  }
  if (n_pc.length() > 0) {
    str += " AND n_pc = ";
    str += n_pc;
  }
  
  str += " ORDER BY tr_date_time DESC";
  
  state = doSQL(myconnection, str.c_str());
  
  if(state == 2)
  {
    int n = PQntuples(result);
    if(n < 1)
    {
      PQclear(result);
      return 0;
    }
    for(int i = 0; i < n; i ++)    {
        npclist->push_back(PQgetvalue(result, i, 0));
	checklist->push_back(PQgetvalue(result, i, 1));
	date_time->push_back(PQgetvalue(result, i, 2));
	ndoc->push_back(PQgetvalue(result, i, 3));
	chtype->push_back(PQgetvalue(result, i, 4));
	summ->push_back(PQgetvalue(result, i, 5));
    }
    PQclear(result);
    return n;
  }
  return -1;
  
}

//==================================================================================//
//returns :
// -1 - не удалось получить актуальные данные
//0 - все нормально
//1 - не получено ни одной строки 
//7 - нет связи с БД

//------------------------
//-----(*)-----Zuskin-----13/01/2012-----
//int Trans::Get_PutOut_Check_Info(string n_pc, string receipt, string n_doc, string datetime, Receipt *rec, CopyInfo * copy, bool return_fl)  {
int Trans::Get_PutOut_Check_Info(string n_pc, string receipt, string n_doc, string datetime, Receipt *rec, CopyInfo * copy, int& manager_id, bool return_fl)  {
//---------------------------------------
    
    printf("Get_PutOut_Check_Info(n_pc = %s, ncheck = %s, n_doc = %s, datetime = %s)\n", n_pc.c_str(), receipt.c_str(), n_doc.c_str(), datetime.c_str());
   
    //-----(*)-----Zuskin-----13/01/2012-----
    //Query_Info_of_Registration_Check((char *) receipt.c_str(), (char *) n_pc.c_str(), (char *) datetime.c_str(), (char *) n_doc.c_str());
    int n = Query_Info_of_Registration_Check((char *) receipt.c_str(), (char *) n_pc.c_str(), (char *) datetime.c_str(), (char *) n_doc.c_str());
    if (n < 0) return 88;
    //---------------------------------------    
    
    n = PQntuples(result);
    if ( n < 1 )  {   PQclear(result);   return 1;      }
    
    int er = 0;
    int npos = 0;
    Registration Registr(sp, LINCASH_CONF_PATH);
    
    //char *card = (char *) calloc(100, sizeof(char));
    string card = "";

    int j = 0;
    
    // if (copy_fl) - произдводится разбор для копии чека
    
    int taxrate_index = 0;
    TaxRate * tr;
    for ( int i = 0; i < n; i ++ )  {
	//printf("i = %d \n", i);
	card = "";
	switch ( atoi(PQgetvalue(result, i, 3)) ) {   // tr_type
	case 11 : 
	    // зарегистрированная позиция    
	    printf("PQgetvalue(result, i, 12) = %s \n", PQgetvalue(result, i, 12));
	    er = Registr.New_Receipt_Record(PQgetvalue(result, i, 0), atof(PQgetvalue(result, i, 2)), rec, false, return_fl, false, npos, PQgetvalue(result, i, 12), true);
	    printf("er = %d \n", er);
	    
	    if ( er != 0 )     {
		PQclear(result);
		return er;        
	    }
	    
	    rec->GetPosElemRef(rec->record_num - 1)->price = atof(PQgetvalue(result, i, 1));
	    
	    //Для новой позиции сбрасываем счетчик налоговых ставок, 
	    taxrate_index = 0;
	    rec->Add_Taxes_Info(0, "", 0);
	    break;
	    
	case 15 : // Скидка суммой на позицию
	    switch(atoi(PQgetvalue(result,i,11)))        {
	     case 0 : j = 2; break;
	     case 1 : j = 1; card = PQgetvalue(result, i, 6); break;
	     case 2 : j = 0; break;
	     case 3 : j = 3; break;
	    }
	    rec->GetPosElemRef(npos)->Add_Sum_Discount(j, atof(PQgetvalue(result, i, 1)), PQgetvalue(result, i, 0), "", "", false, 0);
	    break;
	case 16 : // Надбавка суммой на позицию
	    switch(atoi(PQgetvalue(result,i,11)))        {
	     case 0 : j = 2; break;
	     case 1 : j = 1; card = PQgetvalue(result, i, 6); break;
	     case 2 : j = 0; break;
	     case 3 : j = 3; break;
	    }
	    rec->GetPosElemRef(npos)->Add_Sum_Discount(j, -atof(PQgetvalue(result, i, 1)), PQgetvalue(result, i, 0), "", "", false, 0);
	    break;
	case 17 : // Скидка процентная на позицию
	    switch(atoi(PQgetvalue(result,i,11)))        {
	    case 0 : j = 2; break;
	    case 1 : j = 1; card = PQgetvalue(result, i, 6); break;
	    case 2 : j = 0; break;
	    case 3 : j = 3; break;
	    }
	    rec->GetPosElemRef(npos)->Add_Percent_Discount(j, atof(PQgetvalue(result, i, 2)), PQgetvalue(result, i, 0), "", "", false, 0);
	    break;
	case 18 : //  Надбавка процентная на позицию
	    switch(atoi(PQgetvalue(result,i,11)))        {
	    case 0 : j = 2; break;
	    case 1 : j = 1; card = PQgetvalue(result, i, 6); break;
	    case 2 : j = 0; break;
	    case 3 : j = 3; break;
	    }
	    rec->GetPosElemRef(npos)->Add_Percent_Discount(j, -atof(PQgetvalue(result, i, 2)), PQgetvalue(result, i, 0), "", "", false, 0);
	    break;
	case 35 : // Скидка суммой на чек
	    switch(atoi(PQgetvalue(result,i,11)))     {
	    case 0 : j = 2; break;
	    case 1 : j = 1; card = PQgetvalue(result, i, 6); break;
	    case 2 : j = 0; break;
	    case 3 : j = 3; break;
	    }
	    rec->Add_Sum_Discount(j, atof(PQgetvalue(result, i, 1)), PQgetvalue(result, i, 0), "", "", false);
	    break;
	case 36 : // Надбавка суммой на чек
	    switch(atoi(PQgetvalue(result,i,11)))    {
	    case 0 : j = 2; break;
	    case 1 : j = 1; card = PQgetvalue(result, i, 6); break;
	    case 2 : j = 0; break;
	    case 3 : j = 3; break;
	    }
	    rec->Add_Sum_Discount(j, -atof(PQgetvalue(result, i, 1)), PQgetvalue(result, i, 0), "", "", false);
	    break;
	case 37 : // Скидка процентная  на чек
	    switch(atoi(PQgetvalue(result,i,11)))   {
	    case 0 : j = 2; break;
	    case 1 : j = 1; card = PQgetvalue(result, i, 6); break;
	    case 2 : j = 0; break;
	    case 3 : j = 3; break;
	    }
	    rec->Add_Percent_Discount(j, atof(PQgetvalue(result, i, 2)), PQgetvalue(result, i, 0), "", "", false);
	    break;
	case 38 :  // Надбавка процентная на чек
	    switch(atoi(PQgetvalue(result,i,11)))   {
	    case 0 : j = 2; break;
	    case 1 : j = 1; card = PQgetvalue(result, i, 6); break;
	    case 2 : j = 0; break;
	    case 3 : j = 3; break;
	    }
	    rec->Add_Percent_Discount(j, -atof(PQgetvalue(result, i, 2)), PQgetvalue(result, i, 0), "", "", false);
	    break;
	case 14 : 
	    // в модели в результате будет хранится информация о налогах, известная из транзакций
	    //при этом объект Positiontaxes хранит группу налогов из транзакций, а объект Record_Receipt группу налогов из справочника товара	
	    //далее необходим анализ на согласованность данных и получение из справочников более полной информации о налогах.
	    if ( taxrate_index == 0 )	rec->GetTaxElemRef(rec->record_num - 1)->tax_group_id = atoi(PQgetvalue(result, i, 8));
	    
	    rec->GetTaxElemRef(rec->record_num - 1)->num_rates = taxrate_index + 1; // будет меняться с каждой итерацией
	    taxrate_index++;
	    
	    // создастся объект с идетификатором и значением налога, остальное по умолчанию.
	    tr = new TaxRate(atoi(PQgetvalue(result, i, 9)), atof(PQgetvalue(result, i, 1))); 
	    rec->GetTaxElemRef(rec->record_num - 1)->Add_Tax_Rate(*tr);
	    break;
	case 40 : 
	case 41 :
	    break;
	case 55 :
	    break;
	//-----(+)-----Zuskin-----13/01/2012-----
	case 57 : // отложенный чек
	    manager_id = atoi(PQgetvalue(result, i, 13));
	    //printf("manager_id = %d\n", manager_id);
	    break;
	//---------------------------------------    
	}
    }    
         
    //создадим копию заполненной модели, удалим из нее информацию о налогах и заполним заново из справочника товаров
    //Receipt * test = new Receipt(*rec);
    //test->Clear_Taxes();
    
    //for ( int i = 0; i < test->record_num; i++ )  {
    //	er = Registr.Add_Taxes(test, i);
    //	if ( er != 0 )    break;  
    //}
    
    //delete(test);
    
    PQclear(result);

    return er;
}

#define MARK_PUTOUT_CHECK "UPDATE transaction SET tr_type = 58 WHERE tr_type = 57"

int Trans::Mark_PutOut_Check(string n_pc, string ncheck, string datetime)  {
    
    printf("Mark_PutOut_Check(npc = %s, ncheck = %s, datetime = %s)\n", n_pc.c_str(), ncheck.c_str(), datetime.c_str());
    
    int state;
    string str = MARK_PUTOUT_CHECK;
    str += " AND n_pc = ";
    str += n_pc;
    str += " AND n_check = ";
    str += ncheck;
    str += " AND tr_date_time = '";
    str += datetime;
    str += "'";
  
    state = doSQL(myconnection, str.c_str());
    return state;
}

//-----(+)-----Zuskin-----13/01/2012-----
#define GET_PUTOUT_CHECK_NUMBER "SELECT MAX(n_check) FROM transaction WHERE (tr_type = 57 OR tr_type = 58) AND tr_date_time >= (SELECT ((EXTRACT('year' FROM current_date))||'-01-01')::timestamp)"
int Trans::GetPutOutCheckNumber() {
    int state;
    string str = GET_PUTOUT_CHECK_NUMBER;
    state = doSQL(myconnection, str.c_str());
    if (state < 0) return state;
    int n = PQntuples(result);
    if (n < 1) {
	n = 1;
    }
    else {
	n = atoi(PQgetvalue(result,0,0)) + 1;
    }	
    
    PQclear(result);
    return n;
}
//----------------------------------------
