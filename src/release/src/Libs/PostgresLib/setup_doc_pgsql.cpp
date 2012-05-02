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

#include "setup_doc_pgsql.h"
#include "addon_func.h"

//==================================================================================//
SetupDocument::SetupDocument()
{
    //puts("SetupDocument::SetupDocument()");
    Set_Db("Setup");
}

//==================================================================================//
SetupDocument::SetupDocument(const char * conf_path) : PGSQL(conf_path)
{
    //printf("SetupDocument::SetupDocument(conf_path = %s)\n", conf_path);
}


//==================================================================================//
SetupDocument::SetupDocument(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, const char* log_path) : PGSQL(dbname, user, password, host, hostaddr, port, log_path)
{
    //printf("SetupDocument::SetupDocument(dbname = <%s>, user = <%s>, password = <%s>, host = <%s>, hostaddr = <%s>, port = <%s>, log_path = <%s>)\n", dbname, user, password, host, hostaddr, port, log_path);
}

//==================================================================================//
SetupDocument::~SetupDocument()
{
    //puts(" SetupDocument::~SetupDocument() ");

}
//==================================================================================//
#define GET_DOCUMENTS "SELECT d.document_id, d.document_name, d.document_path FROM schemes s, Documents d WHERE s.scheme_id = d.scheme_id AND s.scheme_name = '"
int SetupDocument::GetDocuments(const char* scheme_name, TDocument*& docs, int & count)
{
    
    int state;
    char *str;
    
    str = (char *) calloc(strlen(GET_DOCUMENTS) + strlen(scheme_name) + 2, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, GET_DOCUMENTS);
    strcat(str, scheme_name);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    free(str);
    
    if ( state < 0 ) return -1;
    
    count = PQntuples(result);
    
    if ( count == 0 ) {
	    PQclear(result);
	    return 0;
    }
    
    docs = (TDocument *) calloc(count, sizeof(TDocument));
    
    for ( int i = 0; i < count; i++ ) {
	docs[i].id = atoi(PQgetvalue(result, i ,0));
	strcpy( docs[i].name, PQgetvalue(result, i ,1) );
	strcpy( docs[i].path, PQgetvalue(result, i ,2) );
    }
    
    PQclear(result);
    return 0;     
    
}
//==================================================================================//
#define GET_SCHEME_ID "SELECT scheme_id FROM schemes WHERE scheme_name = '"
#define ADD_DOCUMENT "INSERT INTO Documents(scheme_id, document_name, document_path) VALUES("
int SetupDocument::AddDocument(const char* scheme_name, const char* name, const char* path)
{
    int state;
    char * str;
    char * scheme_id;
    
    str = (char *) calloc(strlen(GET_SCHEME_ID) + 2*strlen(scheme_name) + 3, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, GET_SCHEME_ID);
    strcat(str, scheme_name);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    free(str);
    
    if ( state  < 0 ) return -1;  
    if ( PQntuples(result) != 1) { printf("Scheme_name not valid or scheme name was not found!!!\n"); return -4;}
    
    scheme_id = (char*) calloc(PQgetlength(result, 0, 0) + 1, sizeof(char));
    strcpy(scheme_id, PQgetvalue(result, 0, 0));
    
    PQclear(result);
        
    str = (char *) calloc(strlen(ADD_DOCUMENT) + 2*strlen(scheme_id) + 2*strlen(name) + 2*strlen(path) + 10, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    
    strcat(str, ADD_DOCUMENT);
    strcat(str, scheme_id);
    strcat(str, ", '");
    strcat(str, name);
    strcat(str, "', '");
    strcat(str, path);
    strcat(str, "')");

    //printf("str = %s\n", str);
    
    state = doSQL(myconnection, str);
    free(str);
    
    
    return state > 0 ? 0 : -1;   
}

#define DELETE_DOCUMENTS "DELETE FROM Documents WHERE scheme_id IN (SELECT scheme_id FROM schemes WHERE scheme_name = '"
//==================================================================================//
int  SetupDocument::DeleteDocuments(const char*scheme_name)
{
    
    int state;
    char * str;
    
    str = (char *) calloc(strlen(DELETE_DOCUMENTS) + 2*strlen(scheme_name) +5, sizeof(char));
    
    strcat(str, DELETE_DOCUMENTS);
    strcat(str, scheme_name);
    strcat(str, "'");
    strcat(str, ")");
    
    //printf("str = %s\n", str);
    
    state = doSQL(myconnection, str);
    free(str);
    
    return state > 0 ? 0 : -1;   
}


//==================================================================================//
#define GET_ACTIVE_DOCUMENTS "SELECT d.document_id, d.document_name, d.document_path FROM activescheme s, Documents d WHERE d.scheme_id = s.activescheme_id "
int SetupDocument::GetActiveDocuments(TDocument*& docs, int & count) 
{
    int state;
    char *str;
    
    str = (char *) calloc(strlen(GET_ACTIVE_DOCUMENTS) + 2, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcat(str, GET_ACTIVE_DOCUMENTS);
    
    state = doSQL(myconnection, str);
    free(str);
    
    if ( state < 0 ) return -1;
    
    count = PQntuples(result);
    
    if ( count == 0 ) {
	    PQclear(result);
	    return 0;
    }
    
    docs = (TDocument *) calloc(count, sizeof(TDocument));
    
    for ( int i = 0; i < count; i++ ) {
	docs[i].id = atoi(PQgetvalue(result, i ,0));
	strcpy( docs[i].name, PQgetvalue(result, i ,1) );
	strcpy( docs[i].path, PQgetvalue(result, i ,2) );
    }
    
    PQclear(result);
    return 0;     
}

//==================================================================================//




