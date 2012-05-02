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

#include "setup_sam_pgsql.h"
#include "addon_func.h"

//==================================================================================//
SetupSamplebarcode::SetupSamplebarcode()
{
    //puts("SetupSamplebarcode::SetupSamplebarcode()");
    Set_Db("Setup");
}

//==================================================================================//
SetupSamplebarcode::SetupSamplebarcode(const char * conf_path) : PGSQL(conf_path)
{
    //printf("SetupSamplebarcode::SetupSamplebarcode(conf_path = %s)\n", conf_path);
}


//==================================================================================//
SetupSamplebarcode::SetupSamplebarcode(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, const char* log_path) : PGSQL(dbname, user, password, host, hostaddr, port, log_path)
{
    //printf("SetupSamplebarcode::SetupSamplebarcode(dbname = <%s>, user = <%s>, password = <%s>, host = <%s>, hostaddr = <%s>, port = <%s>, log_path = <%s>)\n", dbname, user, password, host, hostaddr, port, log_path);
}

//==================================================================================//
SetupSamplebarcode::~SetupSamplebarcode()
{
    //puts(" SetupSamplebarcode::~SetupSamplebarcode() ");

}
//==================================================================================//
#define GET_SAMPLES "SELECT sb.prefix, sb.pattern, sb.multiplier FROM schemes s, Weit_Pattern sb WHERE s.scheme_id = sb.scheme_id AND s.scheme_name = '"
int SetupSamplebarcode::GetSamples(const char* scheme_name, TSampleBarcode* sambar, int sam_count)
{
    //printf("SetupSamplebarcode::GetSamples(%s)\n", scheme_name);
    int state;
    char *str;
    
    str = (char *) calloc(strlen(GET_SAMPLES) + strlen(scheme_name) + 2, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, GET_SAMPLES);
    strcat(str, scheme_name);
    strcat(str, "'");
   
    state = doSQL(myconnection, str);
    
    if ( state < 0 ) return state;
    if ( PQntuples(result) != sam_count )  { printf("Error: definition discounts  count!!\n"); return -4;  } 
    //printf("samP_count = %d \n", sam_count);
    int i;
    for ( i = 0; i < sam_count; i++ ) {
	sambar[i].id = i;
	
	// если в бд текст большей длины, томожет быть ошибка, проверить 
	
	strcpy( sambar[i].prefix, PQgetvalue(result, i ,0) );
	strcpy( sambar[i].sample, PQgetvalue(result, i ,1) );
	strcpy( sambar[i].multiplier, PQgetvalue(result, i ,2) );

	//printf("sambar[%d] prefix = <%s>, name = <%s>, multi = <%s>\n", i, sambar[i].prefix, sambar[i].sample, sambar[i].multiplier);
    }
    
    free(str);
    PQclear(result);
    //printf("was get %d discounts \n", i);
    return state;     
    
}
//==================================================================================//
#define GET_COUNT_SAMPLES "SELECT COUNT(sb.prefix) FROM Weit_Pattern sb, schemes s  WHERE sb.scheme_id = s.scheme_id AND s.scheme_name = '"
int SetupSamplebarcode::GetCountSamples(const char * scheme_name, int& sam_count) 
{
    
    int state;
    char * str;
    
    str = (char *) calloc(strlen(GET_COUNT_SAMPLES) + strlen(scheme_name) + 2, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, GET_COUNT_SAMPLES);
    strcat(str, scheme_name);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    free(str);
    
    if ( state < 0 ) {printf("Error: state = %d\n", state); return state;}
    if ( PQntuples(result) != 1) return -4;
    
    sam_count = atoi(PQgetvalue(result, 0, 0) );
    
    PQclear(result);
    //printf("dis count = %d\n", (*dis_count));
    return state; 
}
//==================================================================================//
//#define INIT_NEW_DISCOUNT "SELECT MAX(fix_reduction_id) FROM Fix_reduction"
int SetupSamplebarcode::AddNewSample(const char* scheme_name, TSampleBarcode* sam)
{
    //printf("SetupSamplebarcode::AddNewSample(%s, %s)\n", sam->prefix, sam->sample);
    int state;
    
    state = query_Insert(scheme_name, sam->prefix, sam->sample, sam->multiplier);
    
    if ( state < 0 ) { printf("New sample was not insert\n"); return state; }
    
    return state;
}
//==================================================================================//
#define GET_SCHEME_ID "SELECT scheme_id FROM schemes WHERE scheme_name = '"
#define INSERT_SAMPLE "INSERT INTO Weit_Pattern (scheme_id, prefix, pattern, multiplier) VALUES ( "
int SetupSamplebarcode::query_Insert(const char* scheme_name, char prefix[3], char sample[28], char multiplier[22])
{
    //printf("SetupSamplebarcode::InsertSample(%s)\n", prefix);
    int state;
    char * str;
    char * scheme_id;
    
    str = (char *) calloc(strlen(GET_SCHEME_ID) +strlen(scheme_name) + 3, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, GET_SCHEME_ID);
    strcat(str, scheme_name);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    free(str);
    
    if ( state  < 0 ) return state;  
    if ( PQntuples(result) != 1) { printf("Scheme_name not valid or scheme name was not found!!!\n"); return -4;}
    
    scheme_id = (char*) calloc(PQgetlength(result, 0, 0) + 1, sizeof(char));
    strcpy(scheme_id, PQgetvalue(result, 0, 0));
    
    PQclear(result);
        
    str = (char *) calloc(strlen(INSERT_SAMPLE) + strlen(scheme_id) + 160, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    
    strcat(str, INSERT_SAMPLE);
    strcat(str, scheme_id);
    strcat(str, ", ");
    strcat(str, prefix);
    strcat(str, ", '");
    strcat(str, sample);
    strcat(str, "', ");
    strcat(str, multiplier);
    strcat(str, ")");

    //printf("str = %s\n", str);
    
    state = doSQL(myconnection, str);
    free(str);
    
    return state;   
}
#define DELETE_SAMPLES "DELETE FROM weit_pattern WHERE scheme_id IN (SELECT scheme_id FROM schemes WHERE scheme_name = '"
//==================================================================================//
int  SetupSamplebarcode::query_DeleteSamples(const char*scheme_name)
{
    //printf("SetupSamplebarcode::query_DeleteSamples(const char*scheme_name = %s)\n", scheme_name);
    int state;
    char * str;
    
    str = (char *) calloc(strlen(DELETE_SAMPLES) + strlen(scheme_name) +5, sizeof(char));
    
    strcat(str, DELETE_SAMPLES);
    strcat(str, scheme_name);
    strcat(str, "'");
    strcat(str, ")");
    
    //printf("str = %s\n", str);
    
    state = doSQL(myconnection, str);
    free(str);
    
    return state;   
}

//==================================================================================//
int SetupSamplebarcode::SaveInScheme(const char * scheme_name, TSampleBarcode* sam, int sam_count)
{
    
    //printf("SetupSamplebarcode:: SaveInScheme(%s)\n", scheme_name);
    
    int state  = doSQL(myconnection, "BEGIN WORK");
    if ( state < 0 ) return state;
    
    state = query_DeleteSamples(scheme_name);
    
    if ( state < 0 ) { state  = doSQL(myconnection, "ROLLBACK WORK"); printf("Insert discount was not!!\n"); return -1; }
     
    for (int i = 0; i < sam_count; i++ ) {
	state = AddNewSample(scheme_name, &sam[i]);
	if ( state < 0 )  { state  = doSQL(myconnection, "ROLLBACK WORK"); printf("Update discount was not!!\n"); return -1; }
    }
     
    state = doSQL(myconnection, "COMMIT WORK"); 
    
    return state; 
}
//==================================================================================//
#define UPDATE_SAMPLE "UPDATE Weit_Pattern SET prefix = "
#define UPDATE_SAMPLE2 " WHERE prefix = "
int SetupSamplebarcode::query_Update(TSampleBarcode* sam)
{
    //printf("SetupSamplebarcode::UpdateSamples(%s)\n", sam->prefix);
    int state;
    char * str;
    
    str = (char *) calloc(strlen(UPDATE_SAMPLE) + strlen(UPDATE_SAMPLE2) + 60, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    
    strcat(str, UPDATE_SAMPLE);
    strcat(str, sam->prefix);
    strcat(str, ", pattern = '");
    strcat(str, sam->sample),
    strcat(str, "', multiplier = ");
    strcat(str, sam->multiplier);
    strcat(str, UPDATE_SAMPLE2);
    strcat(str, sam->prefix);

    //printf("str = %s\n", str);
    
    state = doSQL(myconnection, str);
    free(str);
    
    return state;   
}
//==================================================================================//
#define DELETE_SAMPLE "DELETE FROM Weit_Pattern WHERE prefix = "
int SetupSamplebarcode::query_Delete(TSampleBarcode* sam)
{
    //printf("SetupSamplebarcode::DeleteSample(%s)\n", sam->prefix);
    int state;
    char * str;
    
    str = (char *) calloc(strlen(DELETE_SAMPLE) + 5, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    
    strcat(str, DELETE_SAMPLE);
    strcat(str, sam->prefix);
    
    //printf("str = %s\n", str);
    
    state = doSQL(myconnection, str);
    free(str);
    
    return state;   
}
//==================================================================================//

//==================================================================================//
