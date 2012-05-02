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

#include "unload_pgsql.h"

//==================================================================================//
unload_pgsql::unload_pgsql()
{
    Set_Db("Transactions");
}

//==================================================================================//
//-----(*)-----Zuskin-----20/12/2011-----
//unload_pgsql::unload_pgsql(const char * conf_path) : PGSQL(conf_path)
unload_pgsql::unload_pgsql(const char * conf_path, bool try_home) : PGSQL(conf_path, try_home)
//---------------------------------------
{
//puts("unload_pgsql::unload_pgsql(const char * conf_path)");

}

//==================================================================================//
unload_pgsql::unload_pgsql(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, const char* log_path) : PGSQL(dbname, user, password, host, hostaddr, port, log_path)
{
   
}
//==================================================================================//
unload_pgsql::~unload_pgsql() {

}
//==================================================================================//
int unload_pgsql::DoCommand_GetCount(char* SQLcommand, int *count_transaction) {

   int state = doSQL(myconnection, SQLcommand);
   if ( state != 2)  return state; 
   
   int n = PQntuples(result);
   *(count_transaction) = n;
   //printf("count_transaction = %d \n", n);
   return state;
}
//==================================================================================//
int unload_pgsql::GetResult(Transaction * tr_mas, int n)
{
   int ret = 0;
   if (result == NULL ) {    printf("");    }
   for (int i = 0; i < n; i++) {
      for (int j = 0; j < COUNT_ATTRIBUTE; j++) {
	  //printf("PQgetvalue(result, i = %d , j = %d ) = %s \n", i, j, PQgetvalue(result, i, j));
	  strcpy(tr_mas[i].data[j], PQgetvalue(result, i, j));
      }
   }
   
   PQclear(result);
   return ret;
}
//==================================================================================//

