#include "class_pgsql.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>

#ifndef SETUP_DISCOUNTS_CLASS
#define SETUP_DISCOUNTS_CLASS


struct TDiscount {
    int  id;
    char name [200];	// наименование в символьном виде
    char text[200];		// текст чека в символьном виде
    char type[4];		//тип в символьном виде (0, 1, 2, 3)
    char value[28];		// значение скидки в символьном виде (в общем случение дробное число с 2 цифрами после зпт)
    char fixing[4];		// цифра 0 или 1 в символьном виде	//0 - на позицию, 1 - на чек
    char barcode[30];	// штрих код в симвыольном виде
    char cardcode[700];	//код магнитной карты
    char show[4];		// отображение в списке фикс скидок в РМК	   -      `f` или `t`
    char external_id[10];	//внутренний код скидки
};

class SetupDiscount : public PGSQL
{
  public:
    SetupDiscount();
    SetupDiscount(const char * conf_path);
    SetupDiscount(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, const char* log_path);
    ~SetupDiscount();   
   
    int GetDiscounts(const char* scheme_name, TDiscount* dis, int dis_count);
    int GetCountDiscounts(const char* scheme_name, int * dis_count);
    int AddNewDiscount(const char* scheme_name, TDiscount* dis);
    int SaveInScheme(const char * scheme_name, TDiscount* dis, int dis_count);
    
   
  private:
    int CheckBarcode(const char* str);
    int CheckCardcode(const char* cardcode_str);
    int query_DeleteAllDiscounts(const char* scheme_name) ;
    int query_InsertDiscount(const char* scheme_name, char external_id[10], char name[200], char text[40], char type[4], char value[28], char fixing[4], char barcode[30], char cardcode[700], char show[4]);
    int query_UpdateDiscount(TDiscount* dis);
    int query_DeleteDiscount(TDiscount* dis);
    
};

#endif














