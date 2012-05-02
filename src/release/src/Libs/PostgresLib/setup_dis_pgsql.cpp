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

#include "setup_dis_pgsql.h"
#include "addon_func.h"

//==================================================================================//
SetupDiscount::SetupDiscount()
{
    //puts("SetupDiscount::SetupDiscount()");
    Set_Db("LinCash");
    log->PrintString(log_type, INFO, "SetupDiscount::SetupDiscount - Connect to db parameters DB name = Setup \n");
}

//==================================================================================//
SetupDiscount::SetupDiscount(const char * conf_path) : PGSQL(conf_path)
{
    log->PrintString(log_type, INFO, "SetupDiscount::SetupDiscount - Connect to db parameters form path = %s \n", conf_path);
    //printf("SetupDiscount::SetupDiscount(conf_path = %s)\n", conf_path);
}
//==================================================================================//
SetupDiscount::SetupDiscount(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, const char* log_path) : PGSQL(dbname, user, password, host, hostaddr, port, log_path)
{
    log->PrintString(log_type, INFO, "SetupDiscount::SetupDiscount - Connect to db with  parameters : dbname = <%s>, user = <%s>, password = <%s>, host = <%s>, hostaddr = <%s>, port = <%s>, log_path = <%s>)\n", dbname, user, password, host, hostaddr, port, log_path);
    
    //printf("SetupDiscount::SetupDiscount(dbname = <%s>, user = <%s>, password = <%s>, host = <%s>, hostaddr = <%s>, port = <%s>, log_path = <%s>)\n", dbname, user, password, host, hostaddr, port, log_path);
}

//==================================================================================//
SetupDiscount::~SetupDiscount()
{
    //puts(" SetupDiscount::~SetupDiscount() ");
    
}
//==================================================================================//
//-----(*)-----Zuskin-----28/07/2011-----
//#define GET_DISCOUNTS "SELECT d.fix_reduction_id, d.longtitle, d.shortcut, d.type, d.value, d.area, d.barcode, d.cardcode, d.show, d.external_id FROM schemes s, Fix_reduction d WHERE s.scheme_id = d.scheme_id AND s.scheme_name = '"
#define GET_DISCOUNTS "SELECT d.fix_reduction_id, d.longtitle, d.shortcut, d.type, d.value, d.area, d.barcode, d.cardcode, d.show, d.external_id FROM Fix_reduction d"
//---------------------------------------
int SetupDiscount::GetDiscounts(const char* scheme_name, TDiscount* dis, int dis_count)
{
    //printf("SetupDiscount::GetDiscounts(%s)\n", scheme_name);
    int state;
    //-----(*)-----Zuskin-----28/07/2011-----
    string str;
    str = GET_DISCOUNTS;
    state = doSQL(myconnection, str.c_str());
    //----------------------------------------
    if ( state < 0 ) return state;
    if ( PQntuples(result) != dis_count )  { 
	//printf("Error: definition discounts  count!!\n"); 
	log->PrintString(log_type, ERROR, "SetupDiscount::GetDiscounts - Error of count discount \n");
	return -4; } 
    
    int i;
    for ( i = 0; i < dis_count; i++ ) {
	
	dis[i].id = atoi(PQgetvalue(result, i, 0));
	strcpy( dis[i].name, PQgetvalue(result, i ,1) );
	strcpy( dis[i].text, PQgetvalue(result, i, 2) );
	strcpy( dis[i].type, PQgetvalue(result, i ,3) );
	strcpy( dis[i].value, PQgetvalue(result, i ,4) );
	strcpy( dis[i].fixing, PQgetvalue(result, i ,5) );
	strcpy( dis[i].barcode, PQgetvalue(result, i, 6) );
	strcpy( dis[i].cardcode, PQgetvalue(result, i, 7) );
	strcpy( dis[i].show, PQgetvalue(result, i, 8) );
	strcpy( dis[i].external_id, PQgetvalue(result, i, 9) );
	//printf("i = %d. discount %s was get \n", i, dis[i].name);
	//printf("dis_id  = %d \n", dis[i].id);
    }
    PQclear(result);
    //printf("was get %d discounts \n", i);
    return state;     
    
}
//==================================================================================//
//-----(*)-----Zuskin-----28/07/2011-----
//#define GET_COUNT_DISCOUNTS "SELECT COUNT(d.fix_reduction_id) FROM Fix_reduction d, schemes s  WHERE d.scheme_id = s.scheme_id AND s.scheme_name = '"
#define GET_COUNT_DISCOUNTS "SELECT COUNT(d.fix_reduction_id) FROM Fix_reduction d"
//---------------------------------------
int SetupDiscount::GetCountDiscounts(const char * scheme_name, int * dis_count) 
{
    //printf("SetupDiscount::GetCountDiscounts(%s)\n", scheme_name);
    
    int state;
    //-----(*)-----Zuskin-----28/07/2011-----
    string str = GET_COUNT_DISCOUNTS;
    state = doSQL(myconnection, str.c_str());
    //----------------------------------------
    
    if ( state < 0 ) {
	log->PrintString(log_type, ERROR, "SetupDiscount::GetCountDiscounts - Error of SQL request \n");
	return state;	}
    if ( PQntuples(result) != 1) return -4;
    
    (*dis_count) = atoi(PQgetvalue(result, 0, 0) );
    
    PQclear(result);
    //printf("dis count = %d\n", (*dis_count));
    return state; 
}
//==================================================================================//
#define INIT_NEW_DISCOUNT "SELECT MAX(fix_reduction_id) FROM Fix_reduction"
int SetupDiscount::AddNewDiscount(const char* scheme_name, TDiscount* dis)
{
    //printf("SetupDiscount::AddNewDiscount(%s)\n", dis->name);
    
    
    int state;
    
    int st_exist = CheckBarcode(dis->barcode);
    //if ( st_exist < 0 ) return st_exist;
    if ( st_exist != 0 ) return 8;
    
    st_exist = CheckCardcode(dis->cardcode);
    //if ( st_exist < 0 ) return st_exist;
    if ( st_exist != 0 ) return 9;
    
    state = query_InsertDiscount(scheme_name, dis->external_id, dis->name, dis->text, dis->type, dis->value, dis->fixing, dis->barcode, dis->cardcode, dis->show);
    
    if ( state < 0 ) { 
	log->PrintString(log_type, ERROR, "SetupDiscount::AddNewDiscoun - Error of SQL request. New discount was not insert \n");
	//printf("New discount was not insert\n"); 
	return state; 
    } 
    
    char* str;
    str = (char *)  calloc( strlen(INIT_NEW_DISCOUNT) + 1 , sizeof(char)); 
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, INIT_NEW_DISCOUNT);
    
    state = doSQL(myconnection, str);
    free(str);
    
    if ( state < 0 ) { 
	log->PrintString(log_type, ERROR, "SetupDiscount::AddNewDiscoun - Error of SQL request. New discount was not init \n");
	return state; }
    
    if (PQntuples(result) != 1)  { 
	log->PrintString(log_type, ERROR, "SetupDiscount::AddNewDiscoun - Error of SQL request. New discount was not init \n");
	return -4; 
    }
    
    dis->id = atoi(PQgetvalue(result, 0, 0));
    
    //printf( "new discount id = %d\n", dis->id);
    
    PQclear(result);
    
    return state;
}

//==================================================================================//

//-----(*)-----Zuskin-----28/07/2011-----
//#define GET_SCHEME_ID "SELECT scheme_id FROM schemes WHERE scheme_name = '"
#define INSERT_DISCOUNT "INSERT INTO Fix_reduction (external_id, scheme_id, longtitle, shortcut, type, value, area, barcode, cardcode, show) VALUES ( "
//---------------------------------------
int SetupDiscount::query_InsertDiscount(const char* scheme_name, char external_id[10], char name[200], char text[40], char type[4], char value[28], char fixing[4], char barcode[30], char cardcode[700], char show[4] )
{
    //printf("SetupDiscount::InsertDiscount(%s)\n", name);
    int state;
    
    //-----(*)-----Zuskin-----28/07/2011-----
    string str = INSERT_DISCOUNT;
    if ( strlen(external_id) > 0 )  str+= external_id;
    else str += "0";
    str += ", ";
    str += "0"; // auto_reduction scheme id 
    str += ", '";
    str += name;
    str += "', '";
    str += text;
    str += "', '";
    str+= type;
    str += "', '";
    str += value;
    str += "', '";
    str += fixing;
    str += "', '";
    str += barcode;
    str += "', '";
    str += cardcode;
    str += "', '";
    if (strlen(show) == 0) strcpy(show, "f");
    str += show;
    str += "')";
    state = doSQL(myconnection, str.c_str());
    //-------------------------------------    
    return state;   
}
//==================================================================================//
//-----(*)-----Zuskin-----28/07/2011-----
//#define DELETE_ALL_DISCOUNT "DELETE FROM fix_reduction WHERE scheme_id IN (SELECT scheme_id FROM schemes WHERE scheme_name = '"
#define DELETE_ALL_DISCOUNT "DELETE FROM fix_reduction"
//---------------------------------------
int SetupDiscount::query_DeleteAllDiscounts(const char* scheme_name)
{
    //printf("int SetupDiscount::query_DeleteAllDiscounts(const char* scheme_name)");
    int state;
    //-----(*)-----Zuskin-----28/07/2011-----
    string str = DELETE_ALL_DISCOUNT;
    state = doSQL(myconnection, str.c_str());
    //----------------------------------------
    return state;   
}
//==================================================================================//
int SetupDiscount::SaveInScheme(const char * scheme_name, TDiscount* dis, int dis_count)
{
    //printf("SetupDiscount:: SaveInScheme(%s)\n", scheme_name);
    int state1 = query_DeleteAllDiscounts(scheme_name);
    
    if ( state1 < 0 ) { state1  = doSQL(myconnection, "ROLLBACK WORK");
    	log->PrintString(log_type, ERROR, "SetupDiscount::SaveInScheme - Error of SQL request. insert discount was not \n");
    	return -1; }

    int state  = doSQL(myconnection, "BEGIN WORK");
    if ( state < 0 ) return state;
    
    for (int i = 0; i < dis_count; i++ ) {
	
	//-----(*)-----Zuskin-----28/07/2011-----
	//if (dis[i].id == -1) {
	    state = AddNewDiscount(scheme_name, &dis[i]);
	    if ( state == 8 )  { 
		state  = doSQL(myconnection, "ROLLBACK WORK"); 
		log->PrintString(log_type, ERROR, "SetupDiscount::SaveInScheme - Error of SQL request. Update discount was not.  \n");
		return 8; 
	    }
	    if ( state == 9 )  { 
		state  = doSQL(myconnection, "ROLLBACK WORK"); 
		log->PrintString(log_type, ERROR, "SetupDiscount::SaveInScheme - Error of SQL request. Update discount was not \n");
		return 9; 
	    }
	    if ( state < 0 )  { 
		log->PrintString(log_type, ERROR, "SetupDiscount::SaveInScheme - Error of SQL request. Update discount was not \n");
		state  = doSQL(myconnection, "ROLLBACK WORK"); 
		return -1; 
	    } 
	
	//}
	//else {
	//  state = query_UpdateDiscount(&dis[i]);
	//    if ( state < 0 )  { 
	//	log->PrintString(log_type, ERROR, "SetupDiscount::SaveInScheme - Error of SQL request. Update discount was not \n");
	//	state  = doSQL(myconnection, "ROLLBACK WORK"); 
	//	return -1; 
	//    }
	//---------------------------------------
	//}
    }	
    state = doSQL(myconnection, "COMMIT WORK"); 
    //-----(-)-----Zuskin-----28/07/2011-----
    //if (state >= 0) state  = doSQL(myconnection, "COMMIT WORK"); 
    //---------------------------------------
    return state; 
    
}
//==================================================================================//
#define UPDATE_DISCOUNT "UPDATE Fix_reduction SET longtitle = '"
#define UPDATE_DISCOUNT2 "' WHERE fix_reduction_id = "
int SetupDiscount::query_UpdateDiscount(TDiscount* dis)
{
    //printf("SetupDiscount::UpdateDiscount(%s)\n", dis->name);
    int state;
    //-----(*)-----Zuskin-----28/07/2011-----
    string str = UPDATE_DISCOUNT;
    str += dis->name;
    str += "', external_id = ";
    str += dis->external_id;
    str += ", shortcut = '";
    str += dis->text;
    str += "', type = '";
    str += dis->type;
    str += "', value = '";
    str += dis->value;
    str += "', area = '";
    str +=  dis->fixing;
    str += "', barcode = '";
    str += dis->barcode;
    str += "', cardcode = '";
    str += dis->cardcode;
    str += "', show = '";
    str += dis->show;
    
    str += UPDATE_DISCOUNT2;
    str += itoa(dis->id);
    
    state = doSQL(myconnection, str.c_str());
    
    return state;   
}
//==================================================================================//
#define DELETE_DISCOUNT "DELETE FROM Fix_reduction WHERE fix_reduction_id = "
int SetupDiscount::query_DeleteDiscount(TDiscount* dis)
{
    //printf("SetupDiscount::DeleteDiscount(%s)\n", dis->name);
    int state;
    char * str;
    char * tmp = itoa(dis->id);
    
    str = (char *) calloc(strlen(DELETE_DISCOUNT) + strlen(tmp) + 2, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    
    strcat(str, DELETE_DISCOUNT);
    strcat(str, tmp);
    
    //printf("str = %s\n", str);
    
    state = doSQL(myconnection, str);
    free(str);
    
    return state;   
}
//==================================================================================//
#define CHECK_WARE_BARCODE "SELECT wb_id FROM ware_barcode WHERE barcode <> '' AND barcode = '"
#define CHECK_USER "SELECT user_id FROM users WHERE bar_code <> '' AND bar_code = '"
#define CHECK_REDUCTION "SELECT fix_reduction_id FROM fix_reduction WHERE barcode <> '' AND barcode = '"
int SetupDiscount::CheckBarcode(const char* barcode_str)
{
    if ( strlen(barcode_str) == 0 ) return 0;
    
    char* str = (char*) calloc(strlen(CHECK_WARE_BARCODE) + strlen(barcode_str) + 3, sizeof(char));
    strcat(str, CHECK_WARE_BARCODE);
    strcat(str, barcode_str);
    strcat(str, "'");
    
    int state = doSQL(myconnection, str);
    
    if ( state != 2 ) { free(str); return state; }
    if ( PQntuples(result) > 0) { 
	log->PrintString(log_type, ERROR, " SetupDiscount::CheckBarcode - Barcode used by ware \n"); 
	free(str);
	PQclear(result);
	return -1;
    }
    
    free(str);
    PQclear(result);
    
    str = (char*) calloc( strlen(CHECK_USER) + strlen(barcode_str) + 3, sizeof(char) );
    
    strcat(str, CHECK_USER);
    strcat(str, barcode_str);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    
    if ( state != 2 ) { free(str); return state; }
    if ( PQntuples(result) > 0) { 
	log->PrintString(log_type, ERROR, " SetupDiscount::CheckBarcode - Barcode used by user \n");
	free(str);
	PQclear(result);
	return -1;
    }
    
    free(str);
    PQclear(result);
    
    str = (char*) calloc( strlen(CHECK_REDUCTION) + strlen(barcode_str) + 3, sizeof(char) );
    
    strcat(str, CHECK_REDUCTION);
    strcat(str, barcode_str);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    
    if ( state != 2 ) { free(str); return state; }
    if ( PQntuples(result) > 0) { 
	log->PrintString(log_type, ERROR, " SetupDiscount::CheckBarcode - Barcode used by reduction \n");
	free(str);
	PQclear(result);
	return -1;
    }
    
    free(str);
    PQclear(result);
    
    return 0;
}
//==================================================================================//
#define CHECK_CARDCODE_USER "SELECT user_id FROM users WHERE card_code <> '' AND card_code = '"
#define CHECK_CARDCODE "SELECT fix_reduction_id FROM fix_reduction WHERE cardcode <> '' AND cardcode = '"
int SetupDiscount::CheckCardcode(const char* cardcode_str)
{
    if ( strlen(cardcode_str) == 0 ) return 0;
    
    char* str = (char*) calloc(strlen(CHECK_CARDCODE_USER) + strlen(cardcode_str) + 3, sizeof(char));
    strcat(str, CHECK_CARDCODE_USER);
    strcat(str, cardcode_str);
    strcat(str, "'");
    
    int state = doSQL(myconnection, str);
    
    if ( state < 0 ) { free(str); return state; }
    if ( PQntuples(result) > 0 ) {
	log->PrintString(log_type, ERROR, " SetupDiscount::CheckBarcode - Cardcode used by user \n");
	free(str);
	PQclear(result);
	return -1;
    }
    
    PQclear(result);
    free(str);
    
    str = (char*) calloc(strlen(CHECK_CARDCODE) + strlen(cardcode_str) + 3, sizeof(char) );
    strcat(str, CHECK_CARDCODE);
    strcat(str, cardcode_str);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    
    if ( state < 0 ) { free(str);  return state; }
    if ( PQntuples(result) > 0 ) {
	log->PrintString(log_type, ERROR, " SetupDiscount::CheckBarcode - Cardcode used by discount \n");
	free(str);
	PQclear(result);
	return -1;
    }
    
    PQclear(result);
    free(str);
    return 0;
}
//==================================================================================//
