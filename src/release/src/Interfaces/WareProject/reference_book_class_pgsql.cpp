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
#include "reference_book_class_pgsql.h"
#include "addon_func.h"
#include "const.h"

#include <qtextcodec.h>
#include <string>
#include <cstdlib>
#include <iostream>


using namespace std;

//==============================================================================//
PgRef::PgRef(const char* str) : PGSQL(str) 
{
   puts("PgRef::PgRef(const char* str)");
}

//==============================================================================//
PgRef::~PgRef()
{
	 puts("PgWare()::~PgWare");
}

///////////////===================SELECT===========================================//////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define GET_QUANTITY_STR_IN_WARE "SELECT count(*) FROM ware"

int PgRef::CheckExistWareTable(int * col_ware)
{
    //puts("int PgRef::CheckExistWareTable(void)");
    int state;
    string str;
    str = GET_QUANTITY_STR_IN_WARE;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	int k = PQntuples(result);
	if (k>0){
		 * col_ware = atoi(PQgetvalue(result,0,0));
	}
	PQclear(result);
    }
    return state; 
}

//===========================Check ExistingWareWithName================================//

#define CHECKEXISTINGWAREWITHNAME  "SELECT longtitle FROM ware where longtitle ='"
int PgRef::CheckExistingWareWithName(const char * str)
{
  //  puts("int PgRef::Check ExistingWareWithName(const char * str)");
    int state;
    string str1;
    str1 = CHECKEXISTINGWAREWITHNAME;
    str1 += checkOnApostrof(str).ascii();
    str1 += "'";
    str1 += ";";
    state = doSQL(myconnection,str1.c_str());
    if (state == 2)
    {
	int k = PQntuples(result);
	PQclear(result);
//	printf("k = %d\n",k);
	return k; 
    }
    return state; 
}

//===========================Check ExistingBarCode================================//

#define CHECKEXISTINGBARCODE  "SELECT * FROM ware_barcode where barcode ='"
int PgRef::CheckExistingBarCode(const char * str)
{
 //   puts("int PgRef::Check ExistingBarCode(const char * str)");
    int state;
    string str1;
    str1 = CHECKEXISTINGBARCODE;
    str1 += str;
    str1 += "'";
    state = doSQL(myconnection,str1.c_str());
    if (state == 2)
    {
	int k = PQntuples(result);
	PQclear(result);
//	printf("k = %d\n",k);
	return k; 
    }
    return state; 	
}

//===========================GetInternalIdWithBarCode================================//
#define GETINTERNALIDWITHBARCODE  "SELECT internal_id  FROM ware_barcode where barcode ='"
int PgRef::GetInternalIdWithBarCode(const char * str, char * id)
{
//    puts("int PgRef::GetInternalIdWithBarCode(const char * str, const char *id)");   
    int state;
    string str1;
    str1 = GETINTERNALIDWITHBARCODE;
    str1 += str;
    str1 += "'";
 //   printf("str1  = %s\n",str1.c_str());
    state = doSQL(myconnection,str1.c_str());
    if (state == 2)
    {
	int k = PQntuples(result);
//	printf("k = %d\n",k);
	if (k>0)
	    strcpy(id,PQgetvalue(result,0,0)); 
	PQclear(result);
	return k; 
    }
    return state; 
}






//===========================GetQuantitySTR=========================================//

//=====================Количество групп налогов========================================//
#define GET_QUANTITY_STR_IN_TAX_GROUP "SELECT * FROM Tax_Group;"
int PgRef::GetQuantityStrInTaxGroup(void)
{
    //puts("int PgRef::GetQuantityStrInTaxGroup(void)");
    int state;
    int k = 0;
    string str;
    str =  GET_QUANTITY_STR_IN_TAX_GROUP;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQntuples(result);
	PQclear(result);
	return k; 
    }
    return state; 
}

//=====================Количество налоговых ставок========================================//
#define GET_QUANTITY_STR_IN_TAX_RATE "SELECT * FROM tax_rate;"
int PgRef::GetQuantityStrInTaxRate(void)
{
    //puts("int PgRef::GetQuantityStrInTaxRate(void)");
    int state;
    int k = 0;
    string str;
    str =  GET_QUANTITY_STR_IN_TAX_RATE;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQntuples(result);
	PQclear(result);
	return k; 
    }
    return state; 
}

//=====================Количество налоговых ставок========================================//
#define GET_QUANTITY_STR_IN_TAX_RATE_GROUP "SELECT * FROM tax_rate_group;"
int PgRef::GetQuantityStrInTaxRateGroup(void)
{
  // puts("int PgRef::GetQuantityStrInTaxRateGroup(void)");
    int state;
    int k = 0;
    string str;
    str =  GET_QUANTITY_STR_IN_TAX_RATE_GROUP;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQntuples(result);
	PQclear(result);
	return k; 
    }
    return state; 
}

//=====================Количество групп товаров========================================//
#define GET_QUANTITY_STR_IN_WARE_GROUP "SELECT * FROM Ware_Group;"
int PgRef::GetQuantityStrInWareGroup(void)
{
    //puts("int PgRef::GetQuantityStrInWareGroup(void)");
    int state;
    int k = 0;
    string str;
    str = GET_QUANTITY_STR_IN_WARE_GROUP;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQntuples(result);
	PQclear(result);
	return k;
    }
    return state; 
}

//=====================Количество строк в Flags========================================//
#define GET_QUANTITY_STR_IN_FLAGS "SELECT * FROM Flags;"
int PgRef::GetQuantityStrInFlags(void)
{
    //puts("int PgRef::GetQuantityStrInFlags(void)");
    int state;
    int k = 0;
    string str;
    str = GET_QUANTITY_STR_IN_FLAGS;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQntuples(result);
	PQclear(result);
	return k;
    }
    return state; 
}
//=====================Количество групп товаров c p_id = NULL========================================//
#define GET_QUANTITY_STR_IN_WARE_GROUP_NULL_PARENT "SELECT * FROM Ware_Group where parent_group_id Is NULL;"
int PgRef::GetQuantityStrInNULLWareGroup(void)
{
    //puts("int PgRef::GetQuantityStrInNULLWareGroup(void)");
    int state;
    int k = 0;
    string str;
    str = GET_QUANTITY_STR_IN_WARE_GROUP_NULL_PARENT;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQntuples(result);
	PQclear(result);
	return k;
    }
    return state; 
}


//=====================Количество групп товаров c p_id != NULL========================================//
#define GET_QUANTITY_STR_IN_WARE_GROUP_NOT_NULL_PARENT "SELECT * FROM Ware_Group where parent_group_id = "
int PgRef::GetQuantityStrInNotNULLWareGroup(const char * s)
{
    //puts("int PgRef::GetQuantityStrInNotNULLWareGroup(void)");
    int state;
    int k = 0;
    string str;
    str = GET_QUANTITY_STR_IN_WARE_GROUP_NOT_NULL_PARENT;
    str +=s;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQntuples(result);
	PQclear(result);
	return k;
    }
    return state; 
}
//=====================Количество товаров============================================//

int PgRef::GetQuantityStrInWare(int * col_ware)
{
    //puts("int PgRef::GetQuantityStrInWare(void)");
    int state;
    int k =0;
    string str;
    str = GET_QUANTITY_STR_IN_WARE;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQntuples(result);
	if (k>0) 
		 * col_ware = atoi(PQgetvalue(result,0,0));
	PQclear(result);
	return k;
    }
    return state; 
}



//==============Количество групп автоматических скидок=====================================//
#define GET_QUANTITY_STR_IN_AUTO_SCHEME_GROUP "SELECT * FROM Auto_Scheme"
int PgRef::GetQuantityStrInAutoSchemeGroup(void)
{
    //puts("int PgRef::GetQuantityStrInAutoSchemeGroup(void)");
    int state;
    int k =0;
    string str;
    str =  GET_QUANTITY_STR_IN_AUTO_SCHEME_GROUP;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQntuples(result);
	PQclear(result);
	return k;
    }
    return state; 
}


//==============Количество  автоматических скидок=====================================//
#define GET_QUANTITY_STR_IN_AUTO_RED "SELECT * FROM Auto_Reduction"
int PgRef::GetQuantityStrInAutoRed(void)
{
   // puts("int PgRef::GetQuantityStrInAutoRed(void)");
    int state;
    int k =0;
    string str;
    str =  GET_QUANTITY_STR_IN_AUTO_RED;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQntuples(result);
	PQclear(result);
	return k;
    }
    return state; 
}


//==============Количество скидок в группе под номером======================================//
#define GET_QUANTITY_STR_IN_AUTO_REDUCTION "SELECT * FROM Auto_Reduction where auto_scheme_id ="
int PgRef::GetQuantityStrInAutoReductionByAutoSchemeId(const char *code)
{
   //puts("int PgRef::GetQuantityStrInAutoReductionByAutoSchemeId(const char *code)");
    int state;
    string str;
    int k = 0;
    str =  GET_QUANTITY_STR_IN_AUTO_REDUCTION;
    str += code;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQntuples(result);
	PQclear(result);	
    }
    return k;
}

//==============Количество налоговых ставок  в группе под номером======================================//
#define GET_QUANTITY_STR_IN_TAX_RATE_BY_TAX_GROUP_ID "SELECT * FROM tax_rate_group where tax_group_id ="
int PgRef::GetQuantityStrInTaxRateByTaxGroupId(const char *code)
{
   //puts("int PgRef::GetQuantityStrInTaxRateByTaxGroupId( const char *code)");
    int state;
    int k = 0;
    string str;
    str =  GET_QUANTITY_STR_IN_TAX_RATE_BY_TAX_GROUP_ID;
    str += code;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQntuples(result);
	PQclear(result);
    }
    return k;
}

#define GET_QUANTITY_FIELD_IN_WARE "SELECT * FROM WARE"
//===========================GetQuantityFIELDS============================================//
int PgRef::GetQuantityFieldsWare(int * col_ware)
{
  //puts("int PgRef::GetQuantityFieldsWare(void)");
    int state;
    int k = 0;
    string str;
    str =  GET_QUANTITY_FIELD_IN_WARE;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQnfields(result);
	//printf("k = %d\n",k);
	if (k>0)
	    *(col_ware) = k;
	PQclear(result);
    }
    return k;  
}

int PgRef::GetQuantityFieldsTaxGroup(void)
{
    //puts("int PgRef::GetQuantityFieldsTaxGroup(void)");
    int state;
    int k = 0;
    string str;
    str =  GET_QUANTITY_STR_IN_TAX_GROUP;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQnfields(result);
	PQclear(result);
    }
    return k;  
}

int PgRef::GetQuantityFieldsTaxRate(void)
{
  // puts("int PgRef::GetQuantityFieldsTaxRate(void)");
    int state;
    int k = 0;
    string str;
    str =  GET_QUANTITY_STR_IN_TAX_RATE;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQnfields(result);
	PQclear(result);
    }
    return k;  
}

int PgRef::GetQuantityFieldsTaxRateGroup(void)
{
 //   puts("int PgRef::GetQuantityFieldsTaxRateGroup(void)");
    
        int state;
    int k = 0;
    string str;
    str =  GET_QUANTITY_STR_IN_TAX_RATE_GROUP;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQnfields(result);
	PQclear(result);
    }
    return k;  
}

int PgRef::GetQuantityFieldsWareGroup(void)
{
//   puts("int PgRef::GetQuantityFieldsWareGroup(void)");
    
        int state;
    int k = 0;
    string str;
    str =  GET_QUANTITY_STR_IN_WARE_GROUP;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQnfields(result);
	PQclear(result);
    }
    return k;  
}

int PgRef::GetQuantityFieldsAutoScheme(void)
{
 //   puts("int PgRef::GetQuantityFieldsAutoScheme(void)");
    
        int state;
    int k = 0;
    string str;
    str =  GET_QUANTITY_STR_IN_AUTO_SCHEME_GROUP;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQnfields(result);
	PQclear(result);
    }
    return k;  
}

int PgRef::GetQuantityFieldsAutoReduction(void)
{
//    puts("int PgRef::GetQuantityFieldsAutoReduction(void)");
    int state;
    int k = 0;
    string str;
    str = GET_QUANTITY_STR_IN_AUTO_RED;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQnfields(result);
	PQclear(result);
    }
    return k;  
}

int PgRef::GetQuantityFieldsWareBarcode(void)
{
//    puts("int PgRef::GetQuantityFieldsWareBarcode(void)");
    int state;
    int k = 0;
    string str;
    str = GET_QUANTITY_STR_IN_AUTO_RED;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	k = PQnfields(result);
	PQclear(result);
    }
    return k;  
}
//====================================END========================================//




//============================Определить название налог.группы  по id=======================//
int PgRef::GetNameTaxGroupById(const char * str, char * value)
{
    string Str = str;
    int n = Get_TaxGroup(Str.c_str());
    if (n == 2)
    {
	if (PQntuples(result)>0)
	{
	    strcpy(value,PQgetvalue(result,0,0));
	}
	PQclear(result);
    }
    else
	strcpy(value, "ERROR");
  //  printf("value = %s\n",value);
    return n;
}




//============================Определить название авт.схемы по id=======================//
#define GET_AUTO_SCHEME "SELECT longtitle FROM auto_scheme WHERE auto_scheme_id = "
int PgRef::Get_AutoScheme(const char * code)
{
//puts("int PgRef::Get_AutoScheme(const char * code)");
  int state;
  string str;
  str = GET_AUTO_SCHEME;
  str +=code;
  state = doSQL(myconnection,str.c_str());
  return state;
}

int PgRef::GetNameAutoSchemeById(const char * str, char * value)
{
    string Str = str;
    int n = Get_AutoScheme(Str.c_str());
    if (n == 2)
    {
	if (PQntuples(result)>0)
	{
	    strcpy(value,PQgetvalue(result,0,0)); 
	}
	PQclear(result);
    }
    else
	strcpy(value, "ERROR");
   // printf("value = %s\n",value);
    return n;
}


//============================Определить название группы товаров по parent_id=======================//
#define WARE_GROUP_BY_P_ID "SELECT longtitle FROM Ware_Group WHERE parent_group_id = "
#define WARE_GROUP_BY_NULL_P_ID "SELECT longtitle FROM Ware_Group WHERE parent_group_id IS NULL "

int PgRef::Get_WareGroup_By_ParentId(const char * code)
{
   // puts("int PgRef::Get_WareGroup_By_ParentId(const char * code)");
    int state;
    int n = 0;
    string str;
    string c = code;
    if (c != "NULL")
    {
	str = WARE_GROUP_BY_P_ID;
	str +=code;
    }
    else
    {
	str = WARE_GROUP_BY_NULL_P_ID;
    }
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	n = PQntuples(result);
//	printf("!!!!!!!!!!!!!       n = %d\n",n);
    }
    return n;
}



 int PgRef::GetNameWareGroupByParentId(const char * str, char ** mas)
{
    //puts(" int PgRef::GetNameWareGroupByParentId(const char * str, char ** mas)");
    string Str = str;
    int n = Get_WareGroup_By_ParentId(Str.c_str());  
   if (n >0){
	for (int i = 0; i<n; i++)
	{
	 //   printf("i = %d\n",i);
	    strcpy(mas[i],PQgetvalue(result,i,0));
	   // printf("mas[%d] = %s\n",i,mas[i]);
	}
	PQclear(result);
    }
    return n;
}


 int PgRef::GetNameWareGroupByParentId(const char * str, char ** mas, int i, int j)
{
   // printf(" int PgRef::GetNameWareGroupByParentId(str = |%s|, char ** mas,i = %d, j = %d)\n",str,i,j);
    string Str = str;
    int n = Get_WareGroup_By_ParentId(Str.c_str());
    int l = 0;
  //  printf("n11111 = %d\n",n);
    if (n >0)
    {
	for (int p = i; p < j; p++)
	{
	//    printf("p = %d\n",p);
	    strcpy(mas[p],PQgetvalue(result,l,0));
	 //   printf(" mas[%d] = %s\n",p,mas[p]);
	    l++;
	//    printf("l = %d\n",l);
	}
	PQclear(result);
    }
  //  printf("GetNameWareGroupByParentId  return = %d\n",n);
    return n;
}


//============================Определить название группы товаров по id=======================//
#define WARE_GROUP "SELECT longtitle FROM Ware_Group WHERE group_id = "
int PgRef::Get_WareGroup(const char * code)
{
//puts("int PgRef::Get_WareGroup(char * code)\n");
  int state;
  string str;
  str = WARE_GROUP;
  str +=code;
  state = doSQL(myconnection,str.c_str());
  return state;
}



 int PgRef::GetNameWareGroupById(const char * str, char * value)
{
    string Str = str;
    int state = Get_WareGroup(Str.c_str());
    if (state == 2)
    {
	int n = PQntuples(result);
	if (n>0)
	{
	    strcpy(value,PQgetvalue(result,0,0));
	}
	PQclear(result);
    }
    else
	strcpy(value, "ERROR");
    //printf("value = %s\n",value);
    return state;
}
//============================Определить названия групп товаров===========================//
#define GETEXISTGROUPID "SELECT group_id FROM ware_group;"
int PgRef::GetExistGroupId(char ** massiv)
{
    //puts("int PgRef::GetExistGroupId(char ** massiv)");
    int state;
    string str;
    str= GETEXISTGROUPID;
    state = doSQL(myconnection,str.c_str());
    if (state == 2)
    {
	int n = PQntuples(result);
	if (n > 0 )
	{
	    for (int i = 0;i<n;i++)
	    {
		strcpy(massiv[i],PQgetvalue(result,i,0));
	//	printf("massiv[%d] = %s\n",i,massiv[i]);
	    }
	 
	}
	PQclear(result);
    }
    return state;
}


int PgRef::GetNameGroup(char **mas)
{
   //puts("int PgRef::GetNameGroup(char **mas)\n");
    int i = 0, n = 0;
    int quantity_str  = 0;
    quantity_str = GetQuantityStrInWareGroup();
    char ** massiv;
    massiv = (char**) calloc(quantity_str, sizeof(char*));
    for ( i=0; i<quantity_str; ++i) 
    {
	massiv[i] = (char*) calloc(6, sizeof(char));
    }
    GetExistGroupId(massiv);
    for (i = 0; i < quantity_str; i++)
    {
	//printf("2 massiv[%d] = %s\n",i,massiv[i]);
	if (Get_WareGroup(massiv[i]) != 2)
	{
	    
	    while (--quantity_str=0) 
	    {
	//	printf("%s free \n", massiv[quantity_str]);
		free(massiv[quantity_str]);    
	    }
	    free(massiv);
	    return -1;
	}
	else
	{
	    n = PQntuples(result);
	    if (n>0)
	    { 
		strcpy(mas[i],PQgetvalue(result,0,0));
	    }
	    PQclear(result);
	}
    }
    while (--quantity_str=0) 
    {
	//printf("%s free \n", massiv[quantity_str]);
	free(massiv[quantity_str]);    
    }
    free(massiv);
    return 0;
}



//===============================определить налоговые группы============================//
#define WARE_TAXGROUP "SELECT longtitle FROM Tax_Group WHERE tax_group_id = "

int PgRef::Get_TaxGroup(const char * code)
{
//  puts( "int PgRef::Get_TaxGroup(char * code)\n");
  int state;
  string str;
  str =  WARE_TAXGROUP;
  str += code;
  state = doSQL(myconnection,str.c_str());
  return state;
}



#define GETEXISTTAXGROUPID "SELECT tax_group_id FROM tax_group order by tax_group_id;"
int PgRef::GetExistTaxGroupId(char ** massiv)
{
  //  puts("int PgRef::GetExistGroupId(char ** massiv)");
    int state;
    string str;
    str= GETEXISTTAXGROUPID;
    state = doSQL(myconnection,str.c_str());
    int n = PQntuples(result);
    if (n > 0 )
    {
	for (int i = 0;i<n;i++)
	{
	    strcpy(massiv[i],PQgetvalue(result,i,0));
//	    printf("massiv[%d] = %s\n",i,massiv[i]);
	}
    }
    PQclear(result);
    return state;
}


int PgRef::GetNameTaxGroup(char **mas)
{
   //puts( "int PgRef::GetNameTaxGroup(char **mas)\n");
    int i =0, n = 0;
    int quantity_str  = 0;
    quantity_str = GetQuantityStrInTaxGroup();
  //  printf("quantity_str = %d\n",quantity_str);
    char ** massiv;
    massiv = (char**) calloc(quantity_str, sizeof(char*));
    for ( i=0; i<quantity_str; ++i) 
    {
	massiv[i] = (char*) calloc(6, sizeof(char));
    }
    GetExistTaxGroupId(massiv);
    for (i = 0; i < quantity_str; i++)
    {
	//printf("2 massiv[%d] = %s\n",i,massiv[i]);
	if (Get_TaxGroup(massiv[i]) != 2)
	{
	     while (--quantity_str=0) 
	    {
	//	printf("%s free \n", massiv[quantity_str]);
		free(massiv[quantity_str]);    
	    }
	    free(massiv);
	    return -1;
	}
	else
	{
	    n = PQntuples(result);
	    if (n>0)
	    {
		strcpy(mas[i],PQgetvalue(result,0,0));
	    }   
	    PQclear(result);
	}
    }
    while (--quantity_str=0) 
    {
//	printf("%s free \n", massiv[quantity_str]);
	free(massiv[quantity_str]);    
    }
    free(massiv);
    return 0;
}


//===============определить названия налоговых ставок в налоговых группах ===================//

#define GETTAXLONGTITLEBYID "SELECT longtitle FROM tax_rate WHERE tax_rate_id = "
const char * PgRef::GetTaxLongtitleById(const char * code)
{
//    puts("int PgRef::GetTaxLongtitleById(const char * code)");
    int state;
    string str;
    str = GETTAXLONGTITLEBYID;
    str += code;
    state = doSQL(myconnection,str.c_str());
    int n  = PQntuples(result);
    if (n > 0 )
    {
	str = PQgetvalue(result,0,0);
//	printf("str = %s\n",str.c_str());
    }
    PQclear(result);
    return str.c_str();
}


#define GETTAXRATEBYGROUP "SELECT tax_rate_id from tax_rate_group where tax_group_id  = "
#define INORDERBYORDER " ORDER BY order_id"

int PgRef::Get_TaxRateByGroup(char ** massiv, const char * code)
{
 // puts("int PgRef::Get_TaxRateByGroup(char * massiv, const char * code)");  
  int state;
  string str;
  str = GETTAXRATEBYGROUP;
  str += code;
  str += INORDERBYORDER;
  state = doSQL(myconnection,str.c_str());
  if (state == 2)
  {
      int n = PQntuples(result);
  //    printf("n = %d\n",n);
      char ** mas = (char **)calloc(n, sizeof(char *));
      int i;
      for ( i=0; i<n; ++i) 
      {
	  mas[i] = (char*) calloc(201, sizeof(char));
      }
      for ( i=0; i<n; ++i) 
      {
	  mas[i] = PQgetvalue(result,i,0);
      }
       for ( i=0; i<n; ++i) 
      {
	  strcpy(massiv[i] , GetTaxLongtitleById(mas[i]));
      }

      int k = n;
      while (--k=0) 
      {
	  free(mas[k]);  
      }
      free(mas);
      return n;
  }
  else
      return 0;
}

//===============определить названия автоматических скидок в группах авт. скидок===================//

#define NAME_AUTO_REDUCTION_BY_GROUP "SELECT longtitle FROM Auto_Reduction WHERE auto_scheme_id = "
int PgRef::Get_AutoReductionByGroup(char **mas, const char * code)
{
//    puts("int PgRef::Get_AutoReductionByGroup(char * code)");
    int state;
    string str;
    str= NAME_AUTO_REDUCTION_BY_GROUP;
    str+=code;
    str+=";";
    state = doSQL(myconnection,str.c_str());
    int n  = PQntuples(result);
    if (n > 0 )
    {
	for (int i = 0;i<n;i++)
	{
	    strcpy(mas[i],PQgetvalue(result,i,0));
	//    printf("mas[%d] = %s\n",i,mas[i]);
	}
    }
    PQclear(result);
    return n;
}





//===============================определить схемы автоматических скидок===================//

#define GETEXISTAUTOSCHEMEID "SELECT auto_scheme_id FROM Auto_Scheme order by auto_scheme_id;"
int PgRef::GetExistAutoSchemeId(char ** massiv)
{
 //   puts("int PgRef::GetExistAutoSchemeId(char * massiv)");
    int state;
    string str;
    str= GETEXISTAUTOSCHEMEID;
    state = doSQL(myconnection,str.c_str());
    int n = PQntuples(result);
    if (n > 0 )
    {
	for (int i = 0;i<n;i++)
	{
	    strcpy(massiv[i],PQgetvalue(result,i,0));
//	    printf("massiv[%d] = %s\n",i,massiv[i]);
	} 
    }
    PQclear(result);
    return state;
}


int PgRef::GetNameAutoScheme(char **mas)
{
//puts( "int PgRef::GetNameAutoScheme(char **mas)\n");
    int i =0;
    int quantity_str  = 0;
    quantity_str = GetQuantityStrInAutoSchemeGroup();
  //  printf("quantity_str = %d\n",quantity_str);
    char ** massiv;
     massiv = (char**) calloc(quantity_str, sizeof(char*));
    for ( i=0; i<quantity_str; ++i) 
    {
	massiv[i] = (char*) calloc(6, sizeof(char));
    }
    GetExistAutoSchemeId(massiv);
    
    for (i = 0; i < quantity_str; i++)
    {
	//printf("2 massiv[%d] = %s\n",i,massiv[i]);
	if (Get_AutoScheme(massiv[i]) != 2)
	{
	    while (--quantity_str == 0) 
	    {
	//	printf("%s free \n", massiv[quantity_str]);
		free(massiv[quantity_str]);    
	    }
	    free(massiv);
	    return -1;
	}
	else
	{
	    int n = PQntuples(result);
	    if (n>0)
	    {
		strcpy(mas[i],PQgetvalue(result,0,0));
	    }
	    PQclear(result);
	}	
    }
    while (--quantity_str=0) 
    {
//	printf("%s free \n", massiv[quantity_str]);
	free(massiv[quantity_str]);    
    }
    free(massiv);
    return 0;
}


//=========================Нахождение auto_scheme_id по longtitle============================//
#define SELECTAUTOSCHEMEID "SELECT auto_scheme_id from auto_scheme where longtitle = '"
 int  PgRef::GetAutoSchemeId(const char * longtitle, char * value)
{
   // puts( " int  PgRef::GetAutoSchemeId(const char * longtitle, char * value)");
    int state;
    string str;
    str = SELECTAUTOSCHEMEID;
    str += checkOnApostrof(longtitle).ascii();
    str += "'"; 
    state = doSQL(myconnection,str.c_str());
   // printf("state = %d\n",state);
    int n;
    if ( state == 2 )
    {
	n = PQntuples(result);
	//printf("n = %d\n",n);
	if (n>0)
	    strcpy(value,PQgetvalue(result,0,0));
	//printf("value = %s\n",value);
	PQclear(result);  
	return n;
    }   
    else
	strcpy(value, "ERROR");
    return state;
}

//=========================Нахождение tax_group_id по longtitle============================//
#define SELECTTAXGROUPID "SELECT tax_group_id from tax_group where longtitle = '"

 int PgRef::GetTaxGroupId(const char * str, char * value)
{
    //puts( " int  PgRef::GetTaxGroupId(const char * str, char * value)");
    string str1 = SELECTTAXGROUPID;
    str1 += checkOnApostrof(str).ascii();
    str1 += "'";
    int state = doSQL(myconnection,str1.c_str());
    if ( state == 2 )
    {
	if (PQntuples(result)>0)
	{
	    strcpy(value, PQgetvalue(result,0,0));
	}
	PQclear(result); 
    }
    else
	strcpy(value, "ERROR");
    return state;
}


QString PgRef::checkOnApostrof(const char * s){
	 QTextCodec * systemCodec = QTextCodec::codecForName(SYSCODEC);
	 QTextCodec * hwCodec = QTextCodec::codecForName(HWCODEC);
	 QString ss = s;
	 ss = (hwCodec->fromUnicode(systemCodec->toUnicode(ss)));
	 char c = 39;
	 char * str = (char *)calloc(ss.length()+1,sizeof(char)); 
	 unsigned int j = 0;
	 for(unsigned int i = 0; i<ss.length();i++){ 
		  if (ss[i] == c){
				*(str+j) = c;
				j++;
		  }
		  *(str+j) = QChar(ss[i]);
		  j++;
	 }
	 ss = "";
	 ss.insert(0,str);
	 free(str);
	 QString t = (systemCodec->fromUnicode(hwCodec->toUnicode(ss)));
	 return  t;
 }
 
 
//=========================Нахождение group_id по longtitle================================//

#define SELECTGROUPID "SELECT group_id from ware_group where longtitle = '"
 int  PgRef::GetGroupId(const char * longtitle, char * value)
{
   // puts( " int PgRef::GetGroupId(const char * longtitle, char * value)");
    int state = 0;
    QString str = "";
    str += SELECTGROUPID;
    str += checkOnApostrof(longtitle);
    str += "'";
    state = doSQL(myconnection,str.ascii());
    int n;
    if ( state ==2 )
    {
	n = PQntuples(result);
	if (n>0)
	{
	    strcpy(value,PQgetvalue(result,0,0)); 
	}
	PQclear(result);  
    }  
    else
	strcpy(value,"ERROR"); 
    return state;
}
 
 //============Нахождение group_id по longtitle внутри родительской группы==============//

 int  PgRef::GetGroupId(const char * longtitle, char * value, const char * parentgroupid)
{
  //printf( " int PgRef::GetGroupId(const char * longtitle = |%s|, char * value,const char * parentgroupid = |%s|\n)", longtitle, parentgroupid);
	  
  if ((QString(parentgroupid) == "0")||(parentgroupid == NULL))
		return GetGroupId( longtitle, value);

    int state;
    QString str;
    str = SELECTGROUPID;
    str += checkOnApostrof(longtitle);
    str +="' and parent_group_id = "; 
    str += parentgroupid;
   // printf("str = |%s|\n",str.ascii());
    state = doSQL(myconnection,str.ascii());
   // printf("state = %d\n",state);
    int n;
    if ( state ==2 )
    {
	n = PQntuples(result);
	//printf("n = %d\n",n);
	if (n>0)
	{
	    strcpy(value,PQgetvalue(result,0,0)); 
	}
	PQclear(result);  
    }  
    else
	strcpy(value,"ERROR"); 
  //  printf("end GroupId value = |%s|\n",value);
    return state;
}
 
 
//=========================Нахождение longtitle по group_id================================//

#define SELECTLONGTILTLEFROMGROUPID "SELECT longtitle from ware_group where group_id = "

 int  PgRef::GetLongtitleByGroupId(const char * group_id, char * value)
{
   // puts( " int PgRef::GetLongtitleByGroupId(const char * group_id, char * value)");
    int n;
    string str;
    str = SELECTLONGTILTLEFROMGROUPID;
    str +=group_id;
    state = doSQL(myconnection,str.c_str());
  //  printf("state = %d\n",state);
    if (state ==2 )
    {
	n = PQntuples(result);
	//printf("n = %d\n",n);
	if (n>0)
	    strcpy(value, PQgetvalue(result,0,0));
	PQclear(result); 
    } 
    else
	strcpy(value, "ERROR");
    return state;
}

//=========================Нахождение longtitle по group_id================================//

#define SELECTPGROUPIDFROMGROUPID "SELECT parent_group_id from ware_group where group_id = "
 int  PgRef::GetParentIdByGroupId(const char * group_id, char * value)
{
  //  puts( " int  PgRef::GetParentIdByGroupId(const char * group_id, char * value)");
    int n;
    int state;
    string str;
    str = SELECTPGROUPIDFROMGROUPID;
    str +=group_id;
    state = doSQL(myconnection,str.c_str());
    if ( state ==2 )
    {
	n = PQntuples(result);
	//printf("n = %d\n",n);
	if (n>0)
	    strcpy(value, PQgetvalue(result,0,0));
	PQclear(result); 
    }  
    else
	strcpy(value, "ERROR");
    return state;
}


//=========================Нахождение tax_group_id по longtitle=============================//
#define SELECTTAXRATEID "SELECT tax_group_id from tax_group where longtitle = '"

 int PgRef::GetTaxRateId(const char * longtitle, char * value)
{
  //  puts( " int PgRef::GetTaxRateId(const char * longtitle, char * value)");
    int n;
    int state;
    string str = SELECTTAXRATEID;
    str += checkOnApostrof(longtitle).ascii();
    str += "'";
    state = doSQL(myconnection,str.c_str());
   // printf("state = %d\n",state);
    if (state==2)
    {
	n = PQntuples(result);
//	printf("n = %d\n",n);
	if (n>0)
	    strcpy(value,PQgetvalue(result,0,0));
	PQclear(result);  
    }
    else
	strcpy(value, "ERROR");
    return state;
}

//=========================Нахождение shortcut tax_group по longtitle==========================//
#define FINDTAXGROUPSHORTCUTBYLOMGTITLE "SELECT shortcut from tax_group where longtitle = '"
 int  PgRef::GetTaxGroupShortCutByLongtitle(const char * str, char * value)
{
 //   puts( " int  PgRef::GetTaxGroupShortCutByLongtitle(const char * str, char * value)");
    int n; 
    int state;
    string str1 = FINDTAXGROUPSHORTCUTBYLOMGTITLE;
    str1 += checkOnApostrof(str).ascii();
    str1 += "';";
    state = doSQL(myconnection,str1.c_str());
   // printf("state = %d\n",state);
    if ( state ==2 )
    {
	n = PQntuples(result);
	//printf("n = %d\n",n);
	if (n>0)
	    strcpy(value,PQgetvalue(result,0,0));
	PQclear(result);
    }
    else
	strcpy(value, "ERROR");
    return state;
}


//=========================Нахождение shortcut ware_group по longtitle==========================//
#define FINDWAREGROUPSHORTCUTBYLOMGTITLE "SELECT shortcut from ware_group where longtitle = '"
 int  PgRef::GetWareGroupShortCutByLongtitle(const char * str, char * value)
{
  //  puts( " int  PgRef::GetWareGroupShortCutByLongtitle(const char * str, char * value)");
    int n; 
    int state;
    string str1 = FINDWAREGROUPSHORTCUTBYLOMGTITLE;
    str1 += checkOnApostrof(str).ascii();
    str1 += "';";
    state = doSQL(myconnection,str1.c_str());
    //  printf("state = %d\n",state);
    if ( state ==2 )
    {
	n = PQntuples(result);
  //	printf("n = %d\n",n);
	if (n>0)
	    strcpy(value,PQgetvalue(result,0,0));
	PQclear(result);
    }
    else
	strcpy(value, "ERROR");
    return state;
}


//=========================Нахождение shortcut auto_scheme по longtitle=============================//
#define FINDAUTOSCHEMESHORTCUTBYLOMGTITLE "SELECT shortcut from auto_scheme where longtitle = '"

int PgRef::FindAutoSchemeShortCutByLongtitle (const char * code)
{
  //puts( "int PgRef::FindAutoSchemeShortCutByLongtitle (const char * code)");
  int state;
  string str = FINDAUTOSCHEMESHORTCUTBYLOMGTITLE;
  str += checkOnApostrof(code).ascii();
  str += "';";
  state = doSQL(myconnection,str.c_str());
 // printf("state = %d\n",state);
  return state;
}

int  PgRef::GetAutoSchemeShortCutByLongtitle(const char * str, char * value)
{
    //  puts( " int  PgRef::GetAutoSchemeShortCutByLongtitle(const char * str, char * value)");
    int n;
    string str1 = str;
    int state =FindAutoSchemeShortCutByLongtitle (str1.c_str());
    if (state == 2 )
    {
	n = PQntuples(result);
	//printf("n = %d\n",n);
	if (n>0)
	    strcpy(value,PQgetvalue(result,0,0));
	PQclear(result);  
    }
    else
	strcpy(value, "ERROR");
    return state;
}


//================Найти номер скидки по ее имени=====================//

#define GETAUTOREDIDBYNAME "SELECT auto_reduction_id from auto_reduction where longtitle = '"

int PgRef::GetAutoRedIdByName(const char * code, char * value)
{
   //   puts( " int PgRef::GetAutoRedIdByName(const char * code, char * value)");
    int n = 0;
    int state;
    string str;
    str = GETAUTOREDIDBYNAME;
    str += checkOnApostrof(code).ascii();
    str +="'";
    state = doSQL(myconnection, str.c_str());
    if (state == 2)
    {
	n = PQntuples(result);
	//printf("n = %d\n",n);
	if (n>0)
	    strcpy(value, PQgetvalue(result,0,0));  
	PQclear(result);
    }
    else
	strcpy(value, "ERROR");
    return n;
}
//=========Найти порядок действия скидки по номеру группы и номеру скидки=========//

#define GETORDERIDBYTAXRATEIDANDTAXGROUPID "SELECT order_id from tax_rate_group where tax_rate_id = "
#define ANDTAXGROUPID " and tax_group_id ="

int  PgRef::GetOrderIdByTaxRateIdAndTaxGroupId(const char *  tax_rate_id,const char * tax_group_id, char * value)
{
   //   puts( " int  PgRef::GetOrderIdByTaxRateIdAndTaxGroupId(const char *  tax_rate_id,const char * tax_group_id, char * value)");
    int n = 0;
    int state;
    string str;
    str = GETORDERIDBYTAXRATEIDANDTAXGROUPID;
    str += tax_rate_id;
    str +=ANDTAXGROUPID;
    str += tax_group_id;
    state = doSQL(myconnection, str.c_str());
    if (state == 2)
    {
	n = PQntuples(result);
	//printf("n = %d\n",n);
	if (n>0)
	    strcpy(value, PQgetvalue(result,0,0));  
	PQclear(result);		
    }
    else
	strcpy(value, "ERROR");
    return state;
}


//================Найти значение автоматической скидки/надбавки  по ее имени=====================//

#define GETAUTOVALUEBYNAME "SELECT value  from auto_reduction where longtitle = '"

int PgRef::GetAutoValueByName(const char * code, char * value)
{
     // puts( " int PgRef::GetAutoValueByName(const char * code, char * value)");
    int n = 0;
    int state;
    string str;
    str = GETAUTOVALUEBYNAME;
    str += checkOnApostrof(code).ascii();
    str +="'";
    state = doSQL(myconnection, str.c_str());
    if (state == 2)
    {
	n = PQntuples(result);
	//printf("n = %d\n",n);
	if (n>0)
	    strcpy(value,PQgetvalue(result,0,0)); 
	PQclear(result); 
    }
    else
	strcpy(value, "ERROR");
    return state;
}




//================Найти номер ставки по ее имени=====================//

#define GETTAXRATEIDBYNAME "SELECT tax_rate_id  from tax_rate where longtitle = '"

int PgRef::GetTaxRateIdByName(const char * code, char * value)
{
    //  puts( " int PgRef::GetTaxRateIdByName(const char * code, char * value)");
    int n = 0;
    int state;
    string str;
    str = GETTAXRATEIDBYNAME;
    str += checkOnApostrof(code).ascii();
    str +="'";
    state = doSQL(myconnection, str.c_str());
    if (state == 2)
    {
	n = PQntuples(result);
	//printf("n = %d\n",n);
	if (n>0)
	    strcpy(value, PQgetvalue(result,0,0)); 
	PQclear(result);		
    }
    else
	strcpy(value, "ERROR"); 
    return n;
}


//======Узнаем все идентификаторы автоматических скидок по id схемы, которой они принадлежат==========//


#define AUTO_RED "SELECT auto_reduction_id FROM auto_reduction WHERE auto_scheme_id = "
int PgRef::Get_AutoRedWithAutoId(const char * code)
{
  //puts( "int PgRef::Get_AutoRedWithAutoId(const char * code)");

  int state;
  int n = 0;
  string str;
  str = AUTO_RED;
  str += code;
  //  printf("str = %s\n",str.c_str());
  state = doSQL(myconnection, str.c_str());
   // printf("state = %d\n",state);
   // printf("PQntuples(result) = %d\n",PQntuples(result));
  if (state == 2)
  {
      n = PQntuples(result);
      return n;	  
  }
  return 0;
}

int PgRef::GetAutoRedSchemeWithAutoId(const char * str, char **mas)
{
      //puts("int PgRef::GetAutoRedSchemeWithAutoId(const char str, char **mas)");
    int i,n = 0;
    string s = str;
    n = Get_AutoRedWithAutoId(s.c_str());
     // printf("n = %d\n",n);
    if (n>0)
    {
	for (i = 0; i < n; i++)
	{
	    strcpy(mas[i],PQgetvalue(result,i,0));
	    
	}
	PQclear(result);	
    }
    return n; 
}





//===============Находим все id из tax_rate_group где tax_group_id = OLD========================//
#define FINDIDFROMTAXRATEBYTAXGROUPID "SELECT id FROM tax_rate_group WHERE tax_group_id = "

int PgRef::Get_IdFromTaxRateGroupByTaxGroupId(char ** mas,const char * code)
{
  //puts( "int PgRef::Get_TaxRateWithAutoId(const char * code)");

  int state;
  int n = 0;
  string str;
  str = FINDIDFROMTAXRATEBYTAXGROUPID;
  str += code;
  //  printf("str = %s\n",str.c_str());
  state = doSQL(myconnection, str.c_str());
  //  printf("state = %d\n",state);
   // printf("PQntuples(result) = %d\n",PQntuples(result));
  if (state == 2)
  {
      n = PQntuples(result);
      if (n>0)
      {
	  for (int i = 0; i<n; i++)
	  {
	      strcpy(mas[i],PQgetvalue(result,i,0));
  //	      printf("mas[%d] = %s\n",i,mas[i]);
	  }
      }
      PQclear(result); 
  }
  return n;
}

#define FINDTAXRATEINFOBYID "SELECT * FROM tax_rate_group WHERE id = "

int PgRef::Get_Tax_Rate_InfoById(char ** mas,const char * code)
{
    //  puts( "int PgRef::Get_Tax_Rate_InfoById(char ** mas,const char * code)");
    int state;
    int n = 0;
    string str;
    str = FINDTAXRATEINFOBYID;
    str += code;
     // printf("str = %s\n",str.c_str());
    state = doSQL(myconnection, str.c_str());
    //  printf("state = %d\n",state);
    //  printf("PQntuples(result) = %d\n",PQntuples(result));
    if (state == 2)
    {
	n = PQntuples(result);
	if (n>0)
	{
	    for (int k = 0; k<6; k++)
	    {
		strcpy(mas[k],PQgetvalue(result,0,k));
	  //	printf("mas[%d] = %s\n",k,mas[k]);
	    }
	}
	PQclear(result); 
    }
  return state;
}

//=========================Получить количество зависимых от tax_group_id товаров================================//
#define GETQUANTITYWARESBYTAXGROUPID "SELECT internal_id FROM WARE where tax_group_id  = "

int PgRef::GetQuantityStrInWareByTaxGroupId(const char *code)
{
   // puts("int PgRef::GetQuantityStrInWareByTaxGroupId(const char *code)\n");
  int state,n = 0;
  string str = GETQUANTITYWARESBYTAXGROUPID;
  str+=code;
  state = doSQL(myconnection,str.c_str());
  if (state != 2)
  {
      return state;
  }
  else 
  {
      n = PQntuples(result);
      PQclear(result); 
  }
  return n;
}


//=========================Получить количество зависимых от wb_id barcodов================================//
#define GETQUANTITYBARCODESBYID "SELECT barcode, coefficient FROM WARE_BARCODE where internal_id = '"

int PgRef::GetQuantityStrInWareBarcodeById(const char *code)
{
    //puts("int PgRef::GetQuantityStrInWareBarcodeById(const char *code)\n");
  int state,n = 0;
  string str = GETQUANTITYBARCODESBYID;
  str+=code;
  str += "'";
  state = doSQL(myconnection,str.c_str());
  if (state != 2)
  {
      return state;
  }
  else 
  {
      n = PQntuples(result);
      PQclear(result); 
  }
  return n;
}

int PgRef::LoadBarcodeById(const char * code, char **mas, char ** mas_coeff)
{
 int state;
 int n = 0;
 string str;
 str = GETQUANTITYBARCODESBYID;
 str += code;
 str += "'";
 state = doSQL(myconnection,str.c_str());
 if (state!=2)
     return n;
 else
 {
     n = PQntuples(result);
     if (n>0)
     {
  //	 printf("n = %d\n",n);
	 for (int i = 0; i<n; i++)
	 {
	     strcpy(mas[i],PQgetvalue(result,i,0));
	     strcpy(mas_coeff[i],PQgetvalue(result,i,1));
  //	     printf("mas[%d] = %s\n",i,mas[i]);
	 }  
     }
     PQclear(result);
 }
 return n;
}






//=========================Получить количество зависимых от группы автоматических скидок товаров ================================//
#define GETQUANTITYWAREWITHAUTOSHEMEID "SELECT * FROM WARE where auto_scheme_id = "

int PgRef::GetQuantityStrInWareWithAutoSchemeId(const char *code)
{
  //  puts("int PgRef::GetQuantityStrInWareWithAutoSchemeId(const char *code)\n");
  int state,n = 0;
  string str = GETQUANTITYWAREWITHAUTOSHEMEID;
  str+=code;
  state = doSQL(myconnection,str.c_str());
  if (state != 2)
  {
      return -1;
  }
  else 
  {
      n = PQntuples(result);
      PQclear(result); 
  }
  return n;
}
//=========================Выгрузить инфо о товарах зависимых от группы автом схем скидок из БД ================================//

#define SELECTINTERNALIDBYAUTOSCHEMEID "SELECT internal_id FROM WARE where auto_scheme_id ="

int PgRef::LoadWaresByAutoSchemeId(const char * code, char **mas)
{
 int state;
 int n = 0;
 string str;
 str = SELECTINTERNALIDBYAUTOSCHEMEID;
 str += code;
 state = doSQL(myconnection,str.c_str());
 if (state!=2)
     return n;
 else
 {
     n = PQntuples(result);
     if (n>0)
     {
	 for (int i = 0; i<n; i++)
	 {
	     strcpy(mas[i],PQgetvalue(result,i,0));
	  //     printf("mas[%d] = %s\n",i,mas[i]);
	 } 
     }
     PQclear(result);
 }
 return n;
}



//=========================Получить количество зависимых от налоговой группы товаров ================================//
#define GETQUANTITYWAREWITHTAXGROUPID "SELECT * FROM WARE where tax_group_id = "

int PgRef::GetQuantityStrInWareWithTaxGroupId(const char *code)
{
  //  puts("int PgRef::GetQuantityStrInWareWithTaxGroupId(void)\n");
  int state,n = 0;
  string str = GETQUANTITYWAREWITHTAXGROUPID;
  str+=code;
  state = doSQL(myconnection,str.c_str());
  if (state != 2)
  {
      return -1;
  }
  else 
  {
      n = PQntuples(result);
      PQclear(result); 
  }
  return n;
}
//=========================Выгрузить инфо о товарах зависимых от налоговой группы из БД ================================//

#define SELECTINTERNALIDBYTAXGROUPID "SELECT internal_id FROM WARE where tax_group_id ="

int PgRef::LoadWaresByTaxRateId(const char * code, char **mas)
{
 int state;
 int n = 0;
 string str;
 str = SELECTINTERNALIDBYTAXGROUPID;
 str += code;
 state = doSQL(myconnection,str.c_str());
 if (state!=2)
     return n;
 else
 {
     n = PQntuples(result);
     if (n>0)
     {
	 for (int i = 0; i<n; i++)
	    {
	     strcpy(mas[i],PQgetvalue(result,i,0));
	     //printf("internalID зависимых товаров => mas[%d] = %s\n",i,mas[i]);
	 } 
     }
    PQclear(result); 
 }
 return n;
}






//=========================Выгрузить инфо о товаре из БД================================//

//-----(*)-----Zuskin-----08/07/2011-----
//#define WARE_INFO "SELECT w.internal_id,  wb.barcode, w.longtitle, w.shortcut, w.price, w.min_price, w.quantity, w.sale_quantity, w.flags, w.best_before, w.auto_scheme_id, 0 as fix_scheme_id, w.group_id, w.tax_group_id FROM WARE w LEFT JOIN ware_barcode wb ON w.internal_id = wb.internal_id  where   w.internal_id  = '"
#define WARE_INFO "SELECT w.internal_id,  wb.barcode, w.longtitle, w.shortcut, w.price, w.min_price, w.quantity, w.sale_quantity, w.flags, w.best_before, w.auto_scheme_id, 0 as fix_scheme_id, w.group_id, w.tax_group_id, w.section FROM WARE w LEFT JOIN ware_barcode wb ON w.internal_id = wb.internal_id  where   w.internal_id  = '"
//---- read of section number added ----
//---------------------------------------
#define MAIN_BARCODE " AND (wb.main_barcode = 't' OR wb.main_barcode IS NULL) "

int PgRef::Get_Ware_Info(const char * code)
{
  //puts("int PgRef::Get_Ware_Info(char * code)\n");
  int state;
  string str;
  str = WARE_INFO;
  str += code;
  str += "'";
  if (GetQuantityStrInWareBarcodeById(code) > 0)
      str +=MAIN_BARCODE;
  //printf("str.c_str() = |%s|\n",str.c_str());
  state = doSQL(myconnection,str.c_str());
  //printf("state = %d\n",state);
  return state;
}



int  PgRef::LoadInfo(const char *str, char **mas)
{
 //   puts("char *  PgRef::LoadInfo(char *str, char **mas)\n");
    int n;
    string str1 = str;
    if ( Get_Ware_Info (str1.c_str())!=2 )
    {
	return -1;
    }
    else
    {
	n = PQntuples(result);
	//printf("n =%d\n",n);
	if (n>0)
	{
//-----(*)-----Zuskin-----08/07/2011-----	
//	    for (int k = 0; k<14; k++)
	    for (int k = 0; k<15; k++)
//---------------------------------------	    
	    {
		strcpy(mas[k],PQgetvalue(result,0,k));
//		printf("mas[%d] = %s\n",k,mas[k]);
	    }
	}
	PQclear(result);  
	return 0; 
    }
}


//==============Выгрузить инфо об автоматических скидках с auto_scheme_id из БД=====================//
#define AUTO_REDUCTION_INFO "SELECT auto_scheme_id, longtitle, shortcut,value, type,begindate, enddate, begintime, endtime, weekdays, quantity, position_sum, check_sum, card_prefix_begin, card_prefix_end FROM auto_reduction where auto_reduction_id ='"

int PgRef::Get_Auto_Reduction_Info(const char * code)
{
  //puts("int PgRef::Get_Auto_Reduction_Info(const char * code)");
  int state;
  string str;
  str = AUTO_REDUCTION_INFO;
  str += code;
  str +="'";
  state = doSQL(myconnection,str.c_str());
//  printf("code = %s\n",code);
  return state;
}


int  PgRef::LoadAutoRedInfoWithAutoSchemeId(const char *str, char **mas)
{
    //  puts("int  PgRef::LoadAutoRedInfoWithAutoSchemeId(const char *str, char **mas)");
    int n;
    string str1 = str;
    if ( Get_Auto_Reduction_Info(str1.c_str())!=2 )
    {
	return -1;
    }
    else
    {
	n = PQntuples(result);
	//printf("n = %d\n",n);
	if (n>0)
	{
	    for (int k = 0; k<15; k++)
	    {
		strcpy(mas[k],PQgetvalue(result,0,k));
		//printf("mas[%d] = %s\n",k,mas[k]);
	    }
	}
	PQclear(result);  
	return 0; 
    }
}


//=========================Получить group_id по longtitle ware=============================//

#define SELECT_GROUP_ID_USE_LONGTITLE_OF_WARE "SELECT group_id  FROM WARE where longtitle ='"
int  PgRef::GetGroupIdInWares(const char * longtitle, char * value)
{
    //  puts( "int PgRef::GetGroupIdInWares(const char * longtitle, char * value);");
    int state;
    string str;
    str = SELECT_GROUP_ID_USE_LONGTITLE_OF_WARE;
    str +=checkOnApostrof(longtitle).ascii();
    str +="'"; 
    state = doSQL(myconnection,str.c_str());
//   printf("state = %d\n",state);
    int n;
    if ( state ==2 )
    {
	n = PQntuples(result);
	  //printf("n = %d\n",n);
	if (n>0)
	    strcpy(value,PQgetvalue(result,0,0));
	PQclear(result);  
    }   
    else
	strcpy(value, "ERROR");
    return state;
}

//=========================Получить group_id по internal_id===============================//
#define SELECT_GROUP_ID "SELECT group_id  FROM WARE where internal_id = '"
int  PgRef::GetGroupIdById(const char * internal_id, char * value)
{
    //  puts("int PgRef::GetGroupIdById(const char * internal_id, char * value)");
    int n;
    string str;
    str =  SELECT_GROUP_ID;
    str +=internal_id;
    str += "'";
    state = doSQL(myconnection,str.c_str());
     // printf("state = %d\n",state);
    if ( state==2 )
    {
	
	n = PQntuples(result);
	if (n>0)
	{
	    strcpy(value,PQgetvalue(result,0,0));
	    if (strcmp(value,"")==0)
		strcpy(value,"0");
	}
	PQclear(result);
    }
    else
    {
	strcpy(value, "ERROR");
    }
    return state;
}


//============Получить имя и родительскую группу группы товаров с идентификатором===============//
#define QUERY_PARENT_GROUP "SELECT longtitle, parent_group_id FROM Ware_Group WHERE group_id = "
int PgRef::Query_Parent_Group(const char *gr)
{
   //   puts("int PgRef::Query_Parent_Group(const char *gr)");
    int state;
    string str;
    str =  QUERY_PARENT_GROUP;
    str += gr;
    state = doSQL(myconnection, str.c_str());  
    //  printf("str = %s\n",str.c_str());
    return state;
}

int PgRef::Get_Parent_Info(char *group_id,  char *name,  char *pgroup)
{
    //  puts("int PgRef::Get_Parent_Info(char *group_id,const  char *name, const char *pgroup)");
    string gr_id = group_id;
    if(Query_Parent_Group(gr_id.c_str()) == 2)
    {
	if(PQntuples(result) == 0)
	{
	    PQclear(result);
	    return 1;
	}
	else
	{
	    strcpy(name, PQgetvalue(result, 0, 0));
	    strcpy(pgroup, PQgetvalue(result, 0, 1));
	    PQclear(result);
	    return 0;
	}
    }
    else return -1;
}

#define GET_MAS_INTERNAL_ID_BY_GROUP_LONGTITLE "select internal_id from ware where group_id = (select group_id from ware_group where longtitle = '"

int PgRef::GetQuantityWaresInGroup(const char * code)
{
    //  puts("int PgRef::GetQuantityWaresInGroup(const char * code)");
    string str = GET_MAS_INTERNAL_ID_BY_GROUP_LONGTITLE;
    str += checkOnApostrof(code).ascii();
    str += "')";
    int n = 0;
    int state = doSQL(myconnection, str.c_str());  
    if (state == 2)
    {
	n = PQntuples(result);
	PQclear(result);
    }
    return n;
}





int PgRef:: Get_Id_By_Group_Longtitle(const char * code, char ** mas)
{
  //    puts("int PgRef:: Get_Id_By_Group_Longtitle(const char * code)");
    string str = GET_MAS_INTERNAL_ID_BY_GROUP_LONGTITLE;
    str += checkOnApostrof(code).ascii();
    str += "')";
    int n = 0;
    int state = doSQL(myconnection, str.c_str());  
    if (state == 2)
    {
	n = PQntuples(result);
	for (int i = 0; i<n;i++)
	{
	    strcpy(mas[i],PQgetvalue(result, i, 0));
	}
	PQclear(result);
    }
    return n;
}




///////////////===================UPDATE========================================//////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//====================UPDATE  TAXGROUP===================
#define UPDATEGROUP1 "UPDATE TAX_GROUP SET LONGTITLE ='"
#define UPDATEGROUP2 "', SHORTCUT ='"
#define UPDATEGROUP3 "' where tax_group_id = "

int PgRef::UpdateTaxGroup(const char * longtitle, const char * shortcut, const char * code)
{
  //    puts("int PgRef::UpdateTaxGroup(const char * longtitle, const char * shortcut, const char * code)");
    int n;
    string str;
    str = UPDATEGROUP1;
    str += checkOnApostrof(longtitle).ascii();
    str += UPDATEGROUP2;
    str += checkOnApostrof(shortcut).ascii();
    str += UPDATEGROUP3;
    str += code;
    int state;
    state = doSQL(myconnection,str.c_str());
    //  printf("state = %d\n",state);
    if (state <0)
    {
	return -1;
    }
    else
    {
	n = 0;
  //	printf("n = %d\n",n);
    }
    return n; 
}

//====================UPDATE  TAXRATE===================
#define UPDATETAXRATE1 "UPDATE TAX_RATE SET LONGTITLE ='"
#define UPDATETAXRATE2 "' , SHORTCUT = '"
#define UPDATETAXRATE3 "' , VALUE = "
#define UPDATEBYTAXRATEID "where tax_rate_id = "


int PgRef::UpdateTaxRate(const char * longtitle,const char * shortcut, const char * value, const char * code)
{
   //   puts("int PgRef::UpdateTaxRate(const char * longtitle,const char * shortcut, const char * value, const char * code)");
    int n;
    string str;
    str = UPDATETAXRATE1;
    str += checkOnApostrof(longtitle).ascii();
    str += UPDATETAXRATE2;
    str += checkOnApostrof(shortcut).ascii();
    str += UPDATETAXRATE3;
    str += value;
    str += UPDATEBYTAXRATEID;
    str += code;
    int state;
    state = doSQL(myconnection,str.c_str());
     // printf("state = %d\n",state);
    if (state <0)
    {
	return -1;
    }
    else
    {
	n = 0;
	  //printf("n = %d\n",n);	
    }
    return n; 
}


//====================UPDATE  TAXRATEGROUP===================
#define UPDATETAXRATEGROUP1 "UPDATE TAX_RATE_GROUP SET BASE_FLAG ='"
#define UPDATETAXRATEGROUP2 "',ORDER_ID = "
#define UPDATETAXRATEGROUP3 " where tax_rate_id = "
#define UPDATETAXRATEGROUP4 " AND tax_group_id = "

int PgRef::UpdateTaxRateGroup(const char * base_flag, const char * order_id, const char * tax_rate_id, const char * tax_group_id)
{
    //  puts("int PgRef::UpdateBaseFlagTaxRateGroupById(const char * base_flag, const char * tax_rate_id, const char * tax_group_id)");
    int n;
    string str;
    str = UPDATETAXRATEGROUP1;
    str += base_flag;
    str += UPDATETAXRATEGROUP2;
    str += order_id;
    str += UPDATETAXRATEGROUP3;
    str += tax_rate_id;
    str += UPDATETAXRATEGROUP4;
    str += tax_group_id;
    int state;
    state = doSQL(myconnection,str.c_str());
   //   printf("state = %d\n",state);
    if (state <0)
    {
	return -1;
    }
    else
    {
	n = 0;
  //	printf("n = %d\n",n);
    }
    return n; 
}


#define UPDATEBYINTERNAL_ID " where internal_id = '"


#define UPDATEFLAGS "Update  Flags SET flags  = "

int PgRef::Update_Flags_Value( const char * code)	
{
    string str;
    str = UPDATEFLAGS;
    str += code;
    int state;
    state = doSQL(myconnection,str.c_str());
    return state;
}

int PgRef::Create_Flags_Table(void)
{
    string str = "create table flags(flags integer)"; 
    int state = doSQL(myconnection,str.c_str());
    return state;  
}


//====================UPDATE  WARE====================================

#define UPDATEINTERNALID "UPDATE WARE SET internal_id ='"
#define UPDATELONGTITLEWARE "', LONGTITLE = '"
#define UPDATESHORTCUTWARE "', SHORTCUT ='"
#define UPDATEPRICEWARE "',  PRICE ="
#define UPDATEMINPRICEWARE ", MIN_PRICE  "
#define UPDATEQUANTITYWARE ",  QUANTITY  "
#define UPDATESALEQUANTITYWARE ",  SALE_QUANTITY = "
#define UPDATEFLAGSWARE ",  FLAGS ="
#define UPDATEBESTBEFOREWARE ", BEST_BEFORE  "
#define UPDATEAUTOSCHEMEIDWARE ",  AUTO_SCHEME_ID "
#define UPDATEGROUPIDWARE ", GROUP_ID "
#define UPDATETAXGROUPIDWARE ",  TAX_GROUP_ID "
//-----(+)-----Zuskin-----08/07/2011-----
#define UPDATESECTION ", SECTION = "
//---------------------------------------

#define ISNULL "= NULL"

//-----(*)-----Zuskin-----08/07/2011-----
//int PgRef::UpdateWare(const char* internal_id, const char * new_internal_id, const char * longtitle, const char * shortcut, const char * price, const char * min_price, const char * quantity, const char * sale_quantity, const char * flags, const char * best_before, const char * auto_scheme_id, const char * group_id, const char * tax_group_id)
int PgRef::UpdateWare(const char* internal_id, const char * new_internal_id, const char * longtitle, const char * shortcut, const char * price, const char * min_price, const char * quantity, const char * sale_quantity, const char * flags, const char * best_before, const char * auto_scheme_id, const char * group_id, const char * tax_group_id, const char * section)
//---------------------------------------
{
    //  puts("int PgRef::UpdateWare(const char * internal_id, const char * longtitle, const char * shortcut, const char * price, const char * min_price, const char * quantity, const char * sale_quantity, const char * flags, const char * best_before, const char * auto_scheme_id, const char * group_id, const char * tax_group_id)");
    
  /*  printf("%s\n",internal_id);
    printf("%s\n",longtitle);
    printf("%s\n",shortcut);
    printf("%s\n",price);
    printf("%s\n",min_price);
    printf("%s\n",quantity);
    printf("%s\n",sale_quantity);
    printf("%s\n",flags);
    printf("%s\n",best_before);
    printf("%s\n",auto_scheme_id);
    printf("%s\n",group_id);
    printf("%s\n",tax_group_id);*/
    
    int n;
    string minprice = min_price;
    string quant = quantity;
    string bestbefore = best_before;
    string auto_scheme = auto_scheme_id; 
    string group = group_id;
    string tax_group = tax_group_id;
    //-----(+)-----Zuskin-----08/07/2011-----
    string sect = section;
    //---------------------------------------
    string str;
    str += UPDATEINTERNALID;
    str += new_internal_id;
    str += UPDATELONGTITLEWARE;
    str += checkOnApostrof(longtitle).ascii();
    str += UPDATESHORTCUTWARE;
    str += checkOnApostrof(shortcut).ascii();
    str += UPDATEPRICEWARE;
    str += price;
    str += UPDATEMINPRICEWARE;
    if (minprice == "o")
    {
	str+=ISNULL;
    }
    else
    {
	str+="=";
	str+=min_price;
    }
    str += UPDATEQUANTITYWARE;
    if (quant == "o")
    {
	str+=ISNULL;
    }
    else
    {
	str+="=";
	str+=quantity;
    }
    str += UPDATESALEQUANTITYWARE;
    str += sale_quantity;
    str += UPDATEFLAGSWARE;
    str += flags;
    str += UPDATEBESTBEFOREWARE;
    if (bestbefore == "o")
    {
	str += ISNULL;
    }
    else
    {
	str += "=";
	str += best_before;
    }
    str += UPDATEAUTOSCHEMEIDWARE;
    if (auto_scheme == "o")
    {
	str += ISNULL;
    }
    else
    {
	str += "=";
	str += auto_scheme_id;
    }
    
    str += UPDATEGROUPIDWARE;
        if (group == "o")
    {
	str += ISNULL;
    }
    else
    {
	str += "=";
	str += group_id;
    }
    str += UPDATETAXGROUPIDWARE;
        if (tax_group == "o")
    {
	str += ISNULL;
    }
    else
    {
	str += "=";
	str += tax_group_id;
    }
    //-----(+)-----Zuskin-----08/07/2011-----
    str += UPDATESECTION;
    str += sect;
    //---------------------------------------
    str += UPDATEBYINTERNAL_ID;
    str += internal_id;
    str += "'";
    
    int state;
     // printf("в запрос строку %s\n",str.c_str());
    state = doSQL(myconnection,str.c_str());
    //  printf("state = %d\n",state);
    if (state <0)
	return -1;
    else
    {
	n = 0;
  //	printf("n = %d\n",n);
    }
    return n; 
}

//==================== UPDATE  AUTO SCHEME ====================================
#define UPDATEAUTOSCHEME "UPDATE auto_scheme SET LONGTITLE ='"
#define UPDATEAUTOSCHEME1 ", SHORTCUT ='"
#define UPDATEAUTOSCHEME2 "WHERE AUTO_SCHEME_ID =  "

int PgRef::UpdateAutoScheme(const char * auto_scheme_id, const char * longtitle, const char * shortcut)
{
     // puts("int PgRef::UpdateAutoScheme(const char * auto_scheme_id, const char * longtitle, const char * shortcut)");
     // printf("%s\n",auto_scheme_id);
   //   printf("%s\n",longtitle);
    //  printf("%s\n",shortcut);
    int n;
    string str;
    str = UPDATEAUTOSCHEME;
    str += checkOnApostrof(longtitle).ascii();
    str += "'";
    str += UPDATEAUTOSCHEME1;
    str += checkOnApostrof(shortcut).ascii();
    str += "'";
    str += UPDATEAUTOSCHEME2;
    str += auto_scheme_id;
     int state;
     // printf("в запрос строку %s\n",str.c_str());
    state = doSQL(myconnection,str.c_str());
    //  printf("state = %d\n",state);
    if (state <0)
	return -1;
    else
    {
	n = 0;
	  //printf("n = %d\n",n);
    }
    return n; 
}

//==================== UPDATE  AUTO REDUCTION ====================================

#define UPDATEAUTORED1 "UPDATE AUTO_REDUCTION SET AUTO_SCHEME_ID = "
#define UPDATEAUTORED2 ", LONGTITLE = '"
#define UPDATEAUTORED3 "', SHORTCUT = '"
#define UPDATEAUTORED4  "', VALUE ="
#define UPDATEAUTORED5  ",TYPE ="
#define UPDATEAUTORED6  ", BEGINDATE = "
#define UPDATEAUTORED7  ", ENDDATE = "
#define UPDATEAUTORED8  ", BEGINTIME = "
#define UPDATEAUTORED9  ", ENDTIME = "
#define UPDATEAUTORED10  ",WEEKDAYS = "
#define UPDATEAUTORED11  ", QUANTITY = "
#define UPDATEAUTORED12  ", POSITION_SUM = "
#define UPDATEAUTORED13 ", CHECK_SUM = "
#define UPDATEAUTORED14  ", CARD_PREFIX_BEGIN = "
#define UPDATEAUTORED15  ", CARD_PREFIX_END ="
#define UPDATEAUTORED16  " WHERE AUTO_REDUCTION_ID = "



int PgRef::UpdateAutoReduction(const char * auto_reduction_id, const char * auto_scheme_id, const char * longtitle, const char * shortcut, const char * value, const char * type, const char * begindate, const char * enddate, const char * begintime, const char * endtime, const char * weekdays, const char * quantity, const char * position_sum, const char * check_sum, const char * card_prefix_begin, const char * card_prefix_end)
{
 /*puts("int PgRef::UpdateAutoReduction(const char * auto_reduction_id, const char * auto_scheme_id, const char * longtitle, const char * shortcut, const char * value, const char * type, const char * begindate, const char * enddate, const char * begintime, const char * endtime, const char * weekdays, const char * quantity, const char * position_sum, const char * check_sum, const char * card_prefix_begin, const char * card_prefix_end)");
 printf("%s\n",auto_reduction_id);
 printf("%s\n",auto_scheme_id);
 printf("%s\n",longtitle);
 printf("%s\n",shortcut);  
 printf("%s\n",value); 
 printf("%s\n",type); 
 printf("%s\n",begindate);
 printf("%s\n",enddate);
 printf("%s\n",begintime);
 printf("%s\n",endtime);
 printf("%s\n",weekdays);
 printf("%s\n",quantity);
 printf("%s\n",position_sum);
 printf("%s\n",check_sum);
 printf("%s\n",card_prefix_begin);
 printf("%s\n",card_prefix_end);	   
     */
 int n;
 string str;
 str = UPDATEAUTORED1;
 str +=  auto_scheme_id;
 str += UPDATEAUTORED2;
 str +=  checkOnApostrof(longtitle).ascii();
 str += UPDATEAUTORED3;
 str += checkOnApostrof(shortcut).ascii();
 str += UPDATEAUTORED4;
 str += value;
 str += UPDATEAUTORED5;
  str += type;
 str += UPDATEAUTORED6;
  str += begindate;
 str += UPDATEAUTORED7;
  str += enddate;
 str += UPDATEAUTORED8;
  str += begintime;
 str += UPDATEAUTORED9;
  str += endtime;
 str += UPDATEAUTORED10;
  str += weekdays;
 str += UPDATEAUTORED11;
  str += quantity;
 str += UPDATEAUTORED12;
  str += position_sum;
 str += UPDATEAUTORED13;
  str += check_sum;
 str += UPDATEAUTORED14;
  str += card_prefix_begin;
 str += UPDATEAUTORED15;
  str += card_prefix_end;
 str += UPDATEAUTORED16;
  str += auto_reduction_id;
 int state;
   //printf("в запрос строку %s\n",str.c_str());
 state = doSQL(myconnection,str.c_str());
  // printf("state = %d\n",state);
 if (state <0)
     return -1;
 else
 {
     n = 0;
      // printf("n = %d\n",n);
 }
 return n;    
}

//====================UPDATE  WAREGROUP===================
#define UPDATEWAREGROUP1 "UPDATE WARE_GROUP SET LONGTITLE ='"
#define UPDATEWAREGROUP2 "', SHORTCUT ='"
#define UPDATEWAREGROUP3 "', PARENT_GROUP_ID = "
#define UPDATEWAREGROUP4 " where group_id = "

int PgRef::UpdateWareGroup(const char * longtitle, const char * shortcut, const char * p_id, const char * code)
{
     // puts("int PgRef::UpdateWareGroup(const char * longtitle, const char * shortcut,const char * p_id, const char * code)");
    int n;
    string str;
    str = UPDATEWAREGROUP1;
    str += checkOnApostrof(longtitle).ascii();
    str += UPDATEWAREGROUP2;
    str += checkOnApostrof(shortcut).ascii();
    str += UPDATEWAREGROUP3;
    str += p_id;
    str += UPDATEWAREGROUP4;
    str += code;
    int state;
    state = doSQL(myconnection,str.c_str());
     // printf("state = %d\n",state);
    if (state <0)
    {
	return -1;
    }
    else
    {
	n = 0;
	  //printf("n = %d\n",n);
    }
    return n; 
}


///////////////===================INSERT INTO========================================//////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//=========================Добавление товара в БД=====================================//

//-----(*)-----Zuskin-----08/07/2011-----
//#define ADD_WARE "INSERT INTO ware (internal_id, longtitle, shortcut,price, min_price, quantity, sale_quantity, flags, best_before, auto_scheme_id,  group_id, tax_group_id) values ('"	
#define ADD_WARE "INSERT INTO ware (internal_id, longtitle, shortcut,price, min_price, quantity, sale_quantity, flags, best_before, auto_scheme_id,  group_id, tax_group_id, section) values ('"	
//---------------------------------------
int PgRef::Add_Ware(const char * code)
{
  //puts( "int PgRef::Add_Ware(const char * code)");
 string str;
 str = ADD_WARE;
 
 str +=code;
  int state;
  state = doSQL(myconnection,str.c_str());
  return state;
}

//-----(*)-----Zuskin-----08/07/2011-----
//int PgRef::AddWareInTable(const char  * internal_id, const char  * longtitle, const char  * shortcut, const char *  price, const char *  min_price, const char *  quantity, const char *  sale_quantity, const char  * flags, const char *  best_before, const char  * auto_scheme_id,  const char *  group_id, const char *  tax_group_id)
int PgRef::AddWareInTable(const char  * internal_id, const char  * longtitle, const char  * shortcut, const char *  price, const char *  min_price, const char *  quantity, const char *  sale_quantity, const char  * flags, const char *  best_before, const char  * auto_scheme_id,  const char *  group_id, const char *  tax_group_id, const char * section)
//---------------------------------------
{
      //puts( "int PgRef::AddWareInTable(const char  internal_id,  const char longtitle, const char shortcut, const char price, const char min_price, const char quantity, const char sale_quantity, const char flags, const char best_before, const char auto_scheme_id,  const char group_id, const char tax_group_id)\n");
  string   intermediate_stroka = "";  
  string group = group_id;
  string auto_scheme = auto_scheme_id; 
  string tax_group = tax_group_id;
  string minprice = min_price;
  string bestbefore = best_before;
  string quant = quantity;
  //-----(+)-----Zuskin-----08/07/2011-----
  string sect = section;
  //---------------------------------------
/* printf("group = %s\n",group.c_str());
  printf("auto_scheme = %s\n",auto_scheme.c_str());
  printf("tax_group = %s\n",tax_group.c_str());
  printf("minprice = %s\n",minprice.c_str());
  printf("bestbefore = %s\n",bestbefore.c_str());
  printf("quant = %s\n",quant.c_str());*/
  
  if (minprice == "o")
      minprice = "NULL";
  if (quant == "o")
      quant = "NULL";
  if (bestbefore == "o")
      bestbefore = "NULL";
  if (tax_group == "o")
      tax_group = "NULL";
  if (auto_scheme == "o")
      auto_scheme = "NULL";
  if (group == "o")
      group = "NULL";
    
  intermediate_stroka += internal_id;
  intermediate_stroka +="','" ;
  intermediate_stroka +=checkOnApostrof(longtitle).ascii();
  intermediate_stroka +="','";
  intermediate_stroka +=checkOnApostrof(shortcut).ascii();
  intermediate_stroka +="'," ;
  intermediate_stroka +=price;
  intermediate_stroka +="," ;
  intermediate_stroka +=minprice;
  intermediate_stroka +="," ;
  intermediate_stroka += quant;
  intermediate_stroka +="," ;
  intermediate_stroka +=sale_quantity;
  intermediate_stroka +="," ;
  intermediate_stroka +=flags;
  intermediate_stroka +="," ;
  intermediate_stroka +=bestbefore;
  intermediate_stroka +="," ;
  intermediate_stroka +=auto_scheme;
  intermediate_stroka +="," ;
  intermediate_stroka +=group;
  intermediate_stroka +="," ;
  intermediate_stroka +=tax_group;
  //-----(+)-----Zuskin-----08/07/2011-----
  intermediate_stroka +="," ;
  intermediate_stroka +=sect;
  //---------------------------------------
  intermediate_stroka +=");";
  int state = Add_Ware(intermediate_stroka.c_str());
  return state; 
}

//==================Cоздание строк в ware_barcode==================================//
#define CREATEBARCODES "INSERT INTO WARE_BARCODE (internal_id, barcode,coefficient, main_barcode) values ('"  

int PgRef::CreateBarCode(const char * id, const char * code, const char * coefficient, const char * flag)
{
   //   puts( "int PgRef::CreateBarCode(const char * id, const char * code, const char * flag)");
    //  printf("id = %s\n", id);
     // printf("code = %s\n", code);
    //  printf("flag = %s\n", flag);
    string str;
    str = CREATEBARCODES;
    str += id;
    str +="','";
    str += code;
    str +="',";
    str += coefficient;
    str +=",'";
    str += flag;
    str +="')";
    int state;
    state = doSQL(myconnection,str.c_str());
    return state;
}

//==================Cоздание группы автоматических скидок=============================//
#define CREATEAUTOREDUCTIONGROUP "INSERT INTO AUTO_SCHEME (auto_scheme_id,longtitle, shortcut) values ("  
#define INTERMEDIATE "','"
int PgRef::Create_Auto_Reduction_Group(const char * name,const char * cutname)	
{
    string str;
    str = CREATEAUTOREDUCTIONGROUP;  
    str +=itoa( FindNextAutoSchemeIdFromAutoScheme()+1);
    str +=	 ",'";   
    str += checkOnApostrof(name).ascii();
    str +=INTERMEDIATE;
    str += checkOnApostrof(cutname).ascii();
    str +="')";
    int state;
    state = doSQL(myconnection,str.c_str());
    return state;
}

//==================Cоздание группы автоматических скидок c известным id====================//

#define CREATEAUTOREDGROUPWITHSCHEMEID "INSERT INTO AUTO_SCHEME (auto_scheme_id, longtitle, shortcut) values ("  
#define INTERMEDIATE "','"
int PgRef::Create_Auto_Red_Group_With_SchemeId(const char * auto_scheme_id, const char * name,const char * cutname)	
{
    string str;
    str = CREATEAUTOREDGROUPWITHSCHEMEID;  
    str +=auto_scheme_id;
    str +=",'";
    str += checkOnApostrof(name).ascii();
    str +=INTERMEDIATE;
    str += checkOnApostrof(cutname).ascii();
    str +="')";
    int state;
    state = doSQL(myconnection,str.c_str());
    return state;
}

//==================Cоздание автоматической скидки======================================//
#define CREATEAUTOREDUCTION "INSERT INTO Auto_Reduction ( auto_reduction_id , auto_scheme_id, longtitle, shortcut, value, type, begindate, enddate, begintime, endtime, weekdays , quantity, position_sum, check_sum, card_prefix_begin, card_prefix_end) VALUES ("

int PgRef::Create_Auto_Reduction( const char * code)	
{
    string str;
    str = CREATEAUTOREDUCTION;
    str += code;
    str +=")";
    int state;
    state = doSQL(myconnection,str.c_str());
    return state;
}

//==================Cоздание флагов======================================//
#define CREATEFLAGS "INSERT INTO Flags ( flags) VALUES ("

int PgRef::Create_Flags_Value( const char * code)	
{
    string str;
    str = CREATEFLAGS;
    str += code;
    str +=")";
    int state;
    state = doSQL(myconnection,str.c_str());
    return state;
}

//=============Cоздание связи м/у налоговой ставкой и группой============================//

#define CREAT_TAX_RATE_GROUP "INSERT INTO Tax_Rate_Group  (id, base_flag, tax_group_id, tax_rate_id, order_id) VALUES ("

int PgRef::Create_Tax_Rate_Group( const char * code)	
{
    string str;
    str = CREAT_TAX_RATE_GROUP;
    str += code;
    str +=")";
    int state;
    state = doSQL(myconnection,str.c_str());
    return state;
}
//==================Cоздание налоговой ставки======================================//

#define CREAT_TAXRATE "INSERT INTO Tax_Rate  (tax_rate_id, longtitle, shortcut, value) VALUES ("

int PgRef::Create_Tax_Rate( const char * code)	
{
    //  puts("int PgRef::Create_Tax_Rate( const char * code)");
    string str;
    str = CREAT_TAXRATE;
    str += code;
    str +=")";
    int state;
    state = doSQL(myconnection,str.c_str());
    return state;
}

//==================Cоздание группы товаров======================================//
#define CREATEWAREGROUP "INSERT INTO Ware_group ( group_id, longtitle, shortcut, parent_group_id) VALUES ("

int  PgRef::CreateNewGroupOfWare(const char * LongtitleStroka,const char * ShortCutStroka,const char * p_id)
{
    //  puts("int  PgRef::CreateNewGroupOfWare( const char * LongtitleStroka,const char * ShortCutStroka,const char * p_id)");
    string str;
    str = CREATEWAREGROUP;
    str +=itoa(FindNextWareGroupIdFromWare()+1);
    str += ",'";
    str += checkOnApostrof(LongtitleStroka).ascii();
    str += "','";
    str += checkOnApostrof(ShortCutStroka).ascii();
    str += "',";
    str += p_id;
    str +=")";
    int state;
    state = doSQL(myconnection,str.c_str());
    return state;
}

//==================Cоздание налоговой группы ======================================//
#define CREATETAXGROUP "INSERT INTO Tax_group (tax_group_id, longtitle, shortcut) VALUES ("

int  PgRef::CreateNewTaxGroup(const char * LongtitleStroka,const char * ShortCutStroka)
{
    //  puts("int  PgRef::CreateNewTaxGroup(const char * LongtitleStroka,const char * ShortCutStroka)");
    string str;
    str = CREATETAXGROUP;
    str +=itoa(FindNextTaxGroupIdFromTaxGroup()+1);
    str += ",'";
    str += checkOnApostrof(LongtitleStroka).ascii();
    str += "','";
    str += checkOnApostrof(ShortCutStroka).ascii();
    str += "')";
    int state;
    state = doSQL(myconnection,str.c_str());
    return state;
}

//////////////////////////////////////////////////////////////////////////SEARCH//////////////////////////////////////////////////////

int PgRef::Get_Num_Search_Info(const char *pattern, int pattern_type)
{
  //puts("PgRef::Get_Num_Search_Info(const char *group_id, int pattern_type)");
  if(pattern_type == 1)
  {
    return Query_Cnt_by_IntCode(pattern);
  }
  if (pattern_type == 2)
  {
    return Query_Cnt_by_BarCode(pattern);
  }
  if (pattern_type == 3)
  {
    return Query_Cnt_by_ShCut(pattern);
  }
  return 0;
 }

#define QUERY_GOODS_INT_CNT "SELECT COUNT(w.internal_id) FROM Ware w LEFT JOIN ware_barcode wb ON wb.internal_id = w.internal_id WHERE w.internal_id ~~* '%"

int PgRef::Query_Cnt_by_IntCode(const char *pattern)
{
  //puts("int PgRef::Query_Cnt_by_IntCode(const char *pattern)");
  int state;
  char *str;
  
  str = (char *) calloc(strlen(QUERY_GOODS_INT_CNT) + strlen(pattern) + 5 + 1, sizeof(char));
  strcpy(str, QUERY_GOODS_INT_CNT);
  strcat(str, pattern);
  strcat(str, "%'");
  //printf("str = %s \n", str);
  state = doSQL(myconnection, str);
  free(str);
  if (state !=2)
       return -1;     
  int n = PQntuples(result);
  if (n == 0) {PQclear(result);return 0;}
  n = atoi(PQgetvalue(result, 0, 0)); 
  PQclear(result);
  return n;
}

#define QUERY_GOODS_BAR_CNT "SELECT COUNT(w.internal_id) FROM Ware w, ware_barcode wb WHERE wb.internal_id = w.internal_id AND wb.barcode ~~* '"
int PgRef::Query_Cnt_by_BarCode(const char *pattern)
{
  //puts("int PgRef::Query_Cnt_by_BarCode(const char *pattern)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_GOODS_BAR_CNT) + strlen(pattern) + 3 + 1, sizeof(char));
 /*if(str == NULL) printf("NULL CALLOC!!! parameter str\n");*/
  strcpy(str, QUERY_GOODS_BAR_CNT);
  strcat(str, pattern);
  strcat(str, "%' ");
  state = doSQL(myconnection, str);
  free(str);
  if (state !=2)
       return -1;     
  int n = PQntuples(result);
  if (n == 0) {PQclear(result);return 0;}
  n = atoi(PQgetvalue(result, 0, 0)); 
  PQclear(result);
  return n;
}

#define QUERY_GOODS_SHCUT_CNT "SELECT COUNT(w.internal_id) FROM Ware w LEFT JOIN ware_barcode wb ON wb.internal_id = w.internal_id WHERE upper(w.longtitle) ~~*  upper('%"
int PgRef::Query_Cnt_by_ShCut(const char *pattern)
{
  //puts("int PgRef::Query_Cnt_by_ShCut(const char *pattern)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_GOODS_SHCUT_CNT) + strlen(pattern) + 3 + 1, sizeof(char));
/* if(str == NULL) printf("NULL CALLOC!!! parameter str\n");*/
  strcpy(str, QUERY_GOODS_SHCUT_CNT);
  strcat(str, pattern);
  strcat(str, "%')");
  //printf("str = %s \n", str);
   state = doSQL(myconnection, str);
  free(str);
  if (state !=2)
       return -1;     
  int n = PQntuples(result);
  if (n == 0) {PQclear(result);return 0;}
  n = atoi(PQgetvalue(result, 0, 0)); 
  PQclear(result);
  return n;
}

int PgRef::Get_List_By_Pattern(const char *pattern, int pattern_type)
{
 // printf("PgRef::Get_List_By_Pattern(const char *patern = %s , int pattern_type = %d )\n", pattern, pattern_type);
  if(pattern_type == 1)
  {
    if(Query_by_Internal_Code_Pattern(pattern) != 2)
      return -1;
  }
  else if(pattern_type == 2)
  {
    if(Query_by_Bar_Code_Pattern(pattern) != 2)
      return -1;
  }
  else if(pattern_type == 3)
  {
    if(Query_by_Shortcut_Pattern(pattern) != 2)
      return -1;
  }
  int n = PQntuples(result);
  if(n == 0) PQclear(result);
  return n;
}

//==============================================================================//
int PgRef::GetCountWare(const char* internal_id) {
    
    string str = "SELECT internal_id FROM ware WHERE internal_id = '";
    str += internal_id;
    str += "'";
    
    int state = doSQL(myconnection, str.c_str()); 
    if ( state != 2 ) return -1;
    int n = PQntuples(result);
    PQclear(result);
    return n;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//==============================================================================//
#define ORDERBYCODE "w.internal_id"    
#define ORDERBYBARCODE "wb.barcode" 
#define ORDERBYTITLE "w.longtitle"  

#define QUERY_GOODS_ORDER " ORDER BY  "

#define QUERY_GOODS_SHORTCUT "SELECT w.longtitle, w.shortcut, w.price, w.quantity, wb.barcode, w.internal_id FROM Ware w LEFT JOIN ware_barcode wb ON wb.internal_id = w.internal_id WHERE upper(w.longtitle) ~~*  upper('%"

int PgRef::Query_by_Shortcut_Pattern(const char *pattern)
{
  // puts("int PgRef::Query_by_Shortcut_Pattern(char *pattern)");
  int state;
  char  * str = (char *) calloc(strlen(QUERY_GOODS_SHORTCUT) + strlen(QUERY_GOODS_ORDER) + strlen(pattern) + strlen(ORDERBYCODE) + 3 + 1, sizeof(char));
 /* if(str == NULL) 
      printf("NULL CALLOC!!! parameter str\n");*/
  strcpy(str, QUERY_GOODS_SHORTCUT);
  strcat(str, pattern);
  strcat(str, "%')");
  strcat(str, QUERY_GOODS_ORDER);
  switch(orderbysearch)
  {
  case 0 :
      {
	  strcat(str, ORDERBYCODE);
	  break;
      }
  case 1 :
      {
	  strcat(str, ORDERBYBARCODE);
	  break;
      }
  case 2 :
      {
	  strcat(str, ORDERBYTITLE);
	  break;
      }
  }
   //printf("str = %s\n",str);
  state = doSQL(myconnection, str); 
  free(str);  
    //printf("state = %d\n",state);
  return state;
}



#define QUERY_GOODS_INT "SELECT w.longtitle, w.shortcut, w.price, w.quantity, wb.barcode, w.internal_id   FROM Ware w LEFT JOIN  ware_barcode wb ON   wb.internal_id = w.internal_id where w.internal_id ~~* '%"

int PgRef::Query_by_Internal_Code_Pattern(const char *pattern)
{
  //puts("int PgRef::Query_by_Internal_Code_Pattern(char *pattern)");
  int state;
  string str;
  str =QUERY_GOODS_INT;
  str+= pattern;
  str+= "%'";
  str+= QUERY_GOODS_ORDER;
  switch(orderbysearch)
  {
    case 0 :
      str+= ORDERBYCODE;
      break;
    case 1 :
      str+=ORDERBYBARCODE;
      break;
    case 2 :
      str+= ORDERBYTITLE;
      break;
  }
  //printf("str = %s \n", str.c_str());
  state = doSQL(myconnection, str.c_str()); 
  return state;
}


//==============================================================================//
#define QUERY_GOODS_BAR "SELECT w.longtitle, w.shortcut, w.price, w.quantity, wb.barcode,  w.internal_id  FROM Ware w LEFT  JOIN ware_barcode wb on wb.internal_id = w.internal_id WHERE wb.barcode ~~* '"

int PgRef::Query_by_Bar_Code_Pattern(const char *pattern)
{
  //puts("int PgRef::Query_by_Bar_Code_Pattern(char *pattern)");
  int state;
  string str;
  str =  QUERY_GOODS_BAR;
  str+=pattern;
  str+="%' ";
  str+= QUERY_GOODS_ORDER;
  switch(orderbysearch)
  {
    case 0 :
      str+= ORDERBYCODE;
      break;
    case 1 :
      str+= ORDERBYBARCODE;
      break;
    case 2 :
      str+= ORDERBYTITLE;
      break;
  }
  state = doSQL(myconnection, str.c_str()); 
   // printf("Query_by_Bar_Code_Pattern return = %d\n",state);
  return state;
}

//========================GET BASE_FLAG INFO======================================//

#define GETBASEFLAGINFOBYTAXRATEID "SELECT base_flag, order_id  FROM Tax_Rate_Group WHERE tax_rate_id = "
int PgRef::Get_Base_Flag_Info(char * base_flag, char * order_id, const char *group_id)
{
  //    puts("int PgRef::Get_Base_Flag_Info(char * base_flag, const char *group_id)");
    int state;
    string str = GETBASEFLAGINFOBYTAXRATEID;
    str += group_id;
    state = doSQL(myconnection, str.c_str()); 
    int n = 0;
    if (state == 2)
    {
	n = PQntuples(result);
	if (n >0)
	{
	    strcpy(base_flag, PQgetvalue(result, 0,0));
	    strcpy(order_id, PQgetvalue(result, 0,1));
	}
	  //printf("order_id = %s\n",order_id);
	  //printf("base_flag = %s\n",base_flag);
	PQclear(result); 
    }
    return n;
}

//========================GET ALL AUTO REDUCTION INFO=================================//

#define GETALLAUTOREDINFO "SELECT auto_scheme_id, longtitle, shortcut, value, type, begindate, enddate, begintime, endtime, weekdays, quantity, position_sum, check_sum, card_prefix_begin, card_prefix_end  from auto_reduction WHERE auto_reduction_id = "

int PgRef::Get_All_Auto_Red_Info( char * auto_reduction_id, char * auto_scheme_id, char * longtitle, char * shortcut, char * value, char * type, char * begindate, char * enddate, char * begintime, char * endtime, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end)
{
   //   puts("Get_All_Auto_Red_Info( char * auto_reduction_id, char * auto_scheme_id, char * longtitle, char * shortcut, char * value, char * type, char * begindate, char * enddate, char * begintime, char * endtime, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end)");
    int state;
    string str = GETALLAUTOREDINFO;
    str += auto_reduction_id;
    state = doSQL(myconnection, str.c_str()); 
    int n = state;
    if (n == 2)
    {
	n = PQntuples(result);
	if (n>0)
	{
	    strcpy(auto_scheme_id, PQgetvalue(result, 0,0));
	    strcpy(longtitle, PQgetvalue(result, 0,1));
	    strcpy(shortcut, PQgetvalue(result, 0,2));
	    strcpy(value, PQgetvalue(result, 0,3));
	    strcpy(type, PQgetvalue(result, 0,4));
	    strcpy(begindate, PQgetvalue(result, 0,5));
	    strcpy(enddate, PQgetvalue(result, 0,6));
	    strcpy(begintime, PQgetvalue(result, 0,7));
	    strcpy(endtime, PQgetvalue(result, 0,8));
	    strcpy(weekdays, PQgetvalue(result, 0,9));
	    strcpy(quantity, PQgetvalue(result, 0,10));
	    strcpy(position_sum, PQgetvalue(result, 0,11));
	    strcpy(check_sum, PQgetvalue(result, 0,12));
	    strcpy(card_prefix_begin, PQgetvalue(result, 0,13));
	    strcpy(card_prefix_end, PQgetvalue(result, 0,14));
	}
	PQclear(result); 
	/*printf("auto_scheme_id = <%s>\n",auto_scheme_id);
	printf("longtitle = <%s>\n",longtitle);
	printf("shortcut = <%s>\n",shortcut);
	printf("value = <%s>\n",value);
	printf("type = <%s>\n",type);
	printf("begindate(PQgetvalue(result, 0,5)) = <%s>\n",PQgetvalue(result, 0,5));
	printf("enddate(PQgetvalue(result, 0,6)) = <%s>\n",PQgetvalue(result, 0,6));
	printf("begintime = <%s>\n",begintime);
	printf("endtime = <%s>\n",endtime);
	printf("weekdays = <%s>\n",weekdays);
	printf("quantity = <%s>\n",quantity);
	printf("position_sum = <%s>\n",position_sum);
	printf("check_sum = <%s>\n",check_sum);
	printf("card_prefix_begin = <%s>\n",card_prefix_begin);
	printf("card_prefix_end = <%s>\n",card_prefix_end);*/
	
    }
    return state;
}


//========================GET TAX RATE GROUP INFO==================================//

#define GETTAXRATEGROUPINFOBYTAXRATEID "SELECT  id,base_flag, tax_group_id FROM Tax_Rate_Group WHERE tax_rate_id = "
#define GETBYORDERID " and order_id ="
int PgRef::Get_Tax_Rate_Group_Info( char * id,char * base_flag,  char * tax_group_id, const char * tax_rate_id, const char * order_id)
{
     // puts("int PgRef::Get_Tax_Rate_Group_Info(char * id,char * base_flag,  char * tax_group_id, const char * tax_rate_id, const char * order_id)");
    int state;
    string str = GETTAXRATEGROUPINFOBYTAXRATEID;
    str += tax_rate_id;
    str += GETBYORDERID;
    str += order_id;
    state = doSQL(myconnection, str.c_str()); 
    int n = 0;
    if (state == 2)
    {
	n = PQntuples(result);
	if (n>0)
	{
	    strcpy(id, PQgetvalue(result, 0,0));
	    strcpy(base_flag, PQgetvalue(result, 0,1));
	    strcpy(tax_group_id, PQgetvalue(result, 0,2)); 
	}
	PQclear(result); 
    }
    return n;
}

//========================GET ALL TAX RATE GROUP INFO==============================//

#define GETTAXRATEGROUPINFO "SELECT  id, base_flag, tax_rate_id, order_id  FROM Tax_Rate_Group WHERE tax_rate_id = "

int PgRef::Get_All_Tax_Rate_Group_Info( char * id, char * base_flag,  char * tax_group_id, const char * tax_rate_id, char * order_id)
{
   //   puts("int PgRef::Get_All_Tax_Rate_Group_Info( char * id, char * base_flag,  char * tax_group_id, const char * tax_rate_id, char * order_id)");
    int state;
    string str = GETTAXRATEGROUPINFO;
    str += tax_rate_id;
    state = doSQL(myconnection, str.c_str()); 
    int n = 0;
    if (state == 2)
    {
	n = PQntuples(result);
	if (n >0)
	{
	    strcpy(id, PQgetvalue(result, 0,0));
	    strcpy(base_flag, PQgetvalue(result, 0,1));
	    strcpy(tax_group_id, PQgetvalue(result, 0,2));
	    strcpy(order_id, PQgetvalue(result, 0,3)); 
	}
	PQclear(result); 
    }
    return state;
}
//========================GET TAX RATE INFO=========================================//
#define GETTAXRATEINFOBYTAXRATEID "SELECT longtitle, shortcut,value FROM Tax_Rate WHERE tax_rate_id = "
int PgRef::Get_Tax_Rate_Info( char *name, char *shortname,char * value, const char *group_id)
{
     // puts("int PgRef::Get_Tax_Rate_Info( char *name, char *shortname,char * value, const char *group_id)");
    int state;
    string str = GETTAXRATEINFOBYTAXRATEID;
    str += group_id;
    state = doSQL(myconnection, str.c_str()); 
    int n = 0;
    if (state == 2)
    {
	n = PQntuples(result);
	if (n >0)
	{
	    strcpy(name, PQgetvalue(result, 0,0));
	    strcpy(shortname, PQgetvalue(result, 0,1));
	    strcpy(value, PQgetvalue(result, 0,2));
	}
	PQclear(result); 
    }
    else
	strcpy(value, "ERROR");
    return n;
}

//==============================================================================//
void PgRef::Get_Group_Info( char * name,  char * group_id, int i, int n)
{
   // puts("PgRef::Get_Group_Info(char *name, char *group_id, int i, int n)");
    strcpy(group_id,  PQgetvalue(result, i, 0));
   // printf("group_id = %s ",group_id);
    strcpy(name, PQgetvalue(result, i, 1));
    //printf("name = %s\n",name);
    if(n == i + 1) PQclear(result); 
}


void PgRef::Get_Prod_Info( char * longtitle,  char * shortcut, char * price, char * quantity,  char * bar_code, char * internal_code, int i, int n)
{
    //  puts("PgRef::Get_Prod_Info(char *longtitle, char *shortcut, char *price, char *quantity, char *bar_code, char *internal_code, int i, int n)");
    strcpy(longtitle, PQgetvalue(result, i, 0));
    strcpy(shortcut, PQgetvalue(result, i, 1));
    strcpy(price, PQgetvalue(result, i, 2));
    strcpy(quantity, PQgetvalue(result, i, 3));
    strcpy(bar_code, PQgetvalue(result, i, 4));
    strcpy(internal_code, PQgetvalue(result, i, 5));
    if(n == (i + 1)) PQclear(result); 
   //   printf("end\n");
}




int PgRef::Get_Group_Info(const char *group_id)
{
   //   puts("PgRef::Get_Group_Info(char *group_id)");
    string str = group_id; 
    if (Query_Group(str.c_str()) != 2)
	return -1;	
    int n = PQntuples(result);
     // printf("PQntuples - %d\n", n);
    if(n == 0) PQclear(result);
    return n;
}



int PgRef::Get_Group_Info(const char *group_id, const char * name_group, int limit)
{
	// printf("PgRef::Get_Group_Info(%s, %s, %d)\n",group_id, name_group, limit);
	 string str = group_id; 
	 string name_str = name_group;
	 if (Query_Group(str.c_str(),name_str.c_str(), limit) != 2)
		  return -1;	
	 int n = PQntuples(result);
	 // printf("PQntuples - %d\n", n);
	 if(n == 0) PQclear(result);
	 return n;
}



#define GET_FLAGS_INFO "SELECT flags FROM Flags"    

int PgRef::Get_Flags_Info(char * values)
{
   //   puts("int PgRef::Get_Flags_Info(void)");
    string str = GET_FLAGS_INFO;
    int n = 0;
    int state = doSQL(myconnection, str.c_str()); 
    if (state == 2)
    {
	n = PQntuples(result);
	if (n >0)
	{
	    strcpy(values, PQgetvalue(result, 0,0)); 
	}
	PQclear(result); 
    }
    return n;
}


int PgRef::Get_Goods_Info(const char *group_id)
{
   //   puts("PgRef::Get_Goods_Info(char *group_id)");
    string str =group_id;    
    if (Query_Goods(str.c_str()) != 2)
	return -1;
    int n = PQntuples(result);
    //  printf("PQntuples - %d\n", n);
    if(n == 0) PQclear(result);
    return n;
}

 
int PgRef::Get_Goods_Info(const char *group_id, const char * id_ware,int limit)
{
	 //printf("PgRef::Get_Goods_Info(group_id = %s, id_ware = %s,limit = %d)\n", group_id, id_ware, limit);
	 string str = group_id;  
	 string id_w = id_ware;
	 if (Query_Goods( str.c_str(), id_w.c_str(), limit) != 2)
		  return -1;
	 int n = PQntuples(result);
	// printf("PQntuples - %d\n", n);
	 if(n == 0) PQclear(result);
	 return n;
}
 
 
 
 

#define QUERY_GROUP "SELECT group_id, longtitle FROM Ware_Group WHERE parent_group_id "
#define QUERY_GROUP_ORDER " ORDER BY longtitle"
#define QUERY_GROUP_W "IS NULL"

int PgRef::Query_Group(const char *gr)
{
    //  puts("int PgRef::Query_Group(char *gr)");
   //   printf("group_id in Query_Group = |%s|\n",gr);
    int state;
    string s = gr;
    string str;
    str = QUERY_GROUP;
    if (s == "0")
    {
	//puts("s == 0");
	str += QUERY_GROUP_W;
    }
    else
    {
	str +=  "= ";
	str +=  gr;
    }
    str += QUERY_GROUP_ORDER;
    //printf("str = %s\n",str.c_str());
    state = doSQL(myconnection, str.c_str());
    return state;
}


int PgRef::Query_Group(const char *gr, const char * name_begin_gr, int limit)
{
	 //  puts("int PgRef::Query_Group(const char *gr, const char * name_begin_gr, int limit)");
	 //   printf("group_id in Query_Group = |%s|\n",gr);
	 int state;
	 string s = gr;
	 string name_begin = name_begin_gr;
	 string str;
	 str = QUERY_GROUP;
	 if (s == "0")
	 {
		//  puts("s == 0");
		  str += QUERY_GROUP_W;
	 }
	 else
	 {
		  str +=  "= ";
		  str +=  gr;
	 }
	 str += " AND longtitle > '";
	 str += checkOnApostrof(name_begin.c_str()).ascii();
	 str += "' ";
	 str += QUERY_GROUP_ORDER;
	 str += " LIMIT ";
	 str += itoa(limit);
	// printf("str = %s\n",str.c_str());
	 state = doSQL(myconnection, str.c_str());
	 return state;
}






//==============================================================================//
 int PgRef::QuantityOfGroup(const char * str,char ** mas,char ** mas1)
 {
     //  puts(" int PgRef::QuantityOfGroup(const char * str,char ** mas)");
     int state;
     string s1 = str;
     string s;
     s = QUERY_GROUP;
     s +=  "= ";
     s +=  s1;
     s += QUERY_GROUP_ORDER;
     state = doSQL(myconnection, s.c_str());
    int n,i;
     n = PQntuples(result);
    //   printf("n = %d\n",n);
     if (n>0)
     { 
	 for (i = 0;i<n;i++)
	 {
	     strcpy(mas[i],PQgetvalue(result,i,0));
	     strcpy(mas1[i],PQgetvalue(result,i,1));
	  //     printf("mas[%d] = %s\n",i,mas[i]);
	  //     printf("mas1[%d] = %s\n",i,mas1[i]);
	 } 
     }
     PQclear(result);
     return n; 
 }
//==============================================================================//
#define QUERY_GOODS "SELECT w.longtitle, w.shortcut, w.price, w.quantity,  wb.barcode, w.internal_id  FROM Ware w Left Join ware_barcode wb  on wb.internal_id = w.internal_id WHERE (wb.main_barcode = 't' OR wb.main_barcode IS NULL )  AND group_id "

int PgRef::Query_Goods(const char *gr)
{
 //   puts("int PgRef::Query_Goods(char *gr)");
    int state;
    string str;
    string strforgr = gr;
    str =  QUERY_GOODS;
 //   printf("gr = %s\n",gr);
    if ((strforgr == "0") || (strforgr == ""))
	str += QUERY_GROUP_W;
    else
    {
	str += "= ";
	str += gr;
    }
    str += QUERY_GOODS_ORDER;
    switch(orderbysearch)
    {
    case 0 :
	str += ORDERBYCODE;
	break;
    case 1 :	
	
	str += ORDERBYBARCODE;
	break;
    case 2 :
	str +=ORDERBYTITLE;
	break;
    }
	 //printf("str = %s\n",str.c_str());
    state = doSQL(myconnection, str.c_str()); 
    return state;
}

//==============================================================================//

int  PgRef::Query_Goods(const char *gr, const char * id_ware, int limit)
{
	 
	// printf("PgRef::Query_Goods(%s, %s,  %d)\n",gr, id_ware, limit); 
	 int state;
	 string str;
	 string strforgr = gr;
	 string stridware = id_ware;
	 string lim  = itoa(limit);
	 str =  "SELECT w.longtitle, w.shortcut, w.price, w.quantity,  wb.barcode, w.internal_id  FROM Ware w Left Join ware_barcode wb  on wb.internal_id = w.internal_id WHERE (wb.main_barcode = 't' OR wb.main_barcode IS NULL ) ";
	 if ( stridware == "" ) {
	     str += " AND w.ware_id > 0 ";
	 } else {
	     str += " AND w.ware_id > (SELECT ware_id FROM ware WHERE internal_id = '";
	     str+=stridware;
	     str += "')";
	 }
	 str += " AND group_id ";
	// printf("gr = %s\n",gr);
	 if ((strforgr == "0") || (strforgr == ""))
		  str += QUERY_GROUP_W;
	 else
	 {
		  str += "= ";
		  str += gr;
	 }
	 str += " ORDER BY w.ware_id "; 
	 str += "LIMIT ";
	 str += lim;
	 //printf("str = %s\n",str.c_str());
	 state = doSQL(myconnection, str.c_str()); 
	 return state;
}



///////////////==========================DELETE====================================//////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DELETETAXRATEGROUPBYTAXGROUPID " Delete from tax_rate_group where tax_group_id =  "

int PgRef::DeleteTaxRateGroupByTaxGroupId(const char * str)
{
      //puts("int PgRef::DeleteTaxRateGroupByTaxGroupId(const char * str)");
    int state;
    string str1 = DELETETAXRATEGROUPBYTAXGROUPID;
    str1 += str;
    str1 += ";";
    state = doSQL(myconnection, str1.c_str()); 
    return state;
}

#define DELETETAXRATEGROUPBYTAXRATEID " Delete from tax_rate_group where tax_rate_id =  "

int PgRef::DeleteTaxRateGroupByTaxRateId(const char * str)
{
  //    puts("int PgRef::DeleteTaxRateGroupByTaxRateId(const char * str)");
    int state;
    string str1 = DELETETAXRATEGROUPBYTAXRATEID;
    str1 += str;
    str1 += ";";
    state = doSQL(myconnection, str1.c_str()); 
    return state;
}

#define DELETEWARE " Delete from ware where internal_id =  '"

int PgRef::DeleteWare(const char * str)
{
   // printf("int PgRef::DeleteWare(%s)\n",str);
    int state;
    string str1;
    str1 = DELETEWARE;
    str1 += str;
    str1 += "';";
   // printf("str1 = %s\n",str1.c_str());
    state = doSQL(myconnection, str1.c_str()); 
   // printf("state = %d\n",state);
    return state;
}

#define DELETEBARCODESOFWARE " Delete from ware_barcode where internal_id =  '"

int PgRef::DeleteBarcodeOfWare(const char * str)
{
     // puts("int PgRef::DeleteBarcodeOfWare(const char * str)");
    int state;
    string str1;
    str1 = DELETEBARCODESOFWARE;
    str1 += str;
    str1 += "';";
    state = doSQL(myconnection, str1.c_str()); 
    return state;
}


#define DELETEAUTOGROUPWITHAUTOSCHEMEID " Delete from auto_scheme where auto_scheme_id =  "

int PgRef::DeleteAutoGroupWithAutoSchemeId(const char * str)
{
     // puts("int PgRef::DeleteAutoGroupWithAutoSchemeId(const char * str)");
    int state;
    string str1;
    str1 = DELETEAUTOGROUPWITHAUTOSCHEMEID;
    str1 += str;
    str1 += ";";
    state = doSQL(myconnection, str1.c_str()); 
    return state;
}


#define DELETEFROMTAXRATEGROUPBYID " Delete from tax_rate_group where id =  "

int PgRef::DeleteFromTaxRateGroupById(const char * str)
{
    // puts("int PgRef::DeleteFromTaxRateGroupById(const char * str)");
    int state;
    string str1;
    str1 = DELETEFROMTAXRATEGROUPBYID;
    str1 += str;
    str1 += ";";
    state = doSQL(myconnection, str1.c_str()); 
    return state;
}


#define DELETETAXGROUPWHERETAXGROUPID " Delete from tax_group where tax_group_id =  "

int PgRef::DeleteTaxGroupWhereTaxGroupId(const char * str)
{
    //  puts("int PgRef::DeleteTaxGroupWhereTaxGroupId(const char * str)");
    int state;
    string str1;
    str1 = DELETETAXGROUPWHERETAXGROUPID;
    str1 += str;
    str1 += ";";
    state = doSQL(myconnection, str1.c_str()); 
    return state;
}


#define DELETEAUTOREDWITHAUTOREDID "Delete from auto_reduction where auto_reduction_id = "
int PgRef::DeleteAutoRedWithAutoRedId(const char * str)
{
     // puts("int PgRef::DeleteAutoRedWithAutoRedId(const char * str)");
    int state;
    string str1;
    str1 = DELETEAUTOREDWITHAUTOREDID;
    str1 +=str;
    state = doSQL(myconnection, str1.c_str()); 
     // printf("state = %d\n",state);
    return state;
}


#define DELETEWAREGROUP " Delete  from ware_group where longtitle = '"

int PgRef::DeleteWareGroup(const char * str)
{
   //   puts("int PgRef::DeleteWareGroup(const char * str)");
    int state;
    string str1;
    str1 = DELETEWAREGROUP;
    str1 += checkOnApostrof(str).ascii();
    str1 += "';";
    state = doSQL(myconnection, str1.c_str()); 
    return state;
}


#define DELETETAXRATE " Delete  from tax_rate where tax_rate_id =  "

int PgRef::DeleteTaxRate(const char * str)
{
    //  puts("int PgRef::DeleteTaxRate(const char * str)");
    int state;
    string str1;
    str1 = DELETETAXRATE ;
    str1 += str;
    str1 += ";";
    state = doSQL(myconnection, str1.c_str()); 
    return state;
}

#define DELETETAXRATEBYLONGTITLE " Delete  from tax_rate where longtitle = '"

int PgRef::DeleteTaxRateByLongtitle(const char * str)
{
    //  puts("int PgRef::DeleteTaxRateByLongtitle(const char * str)");
    int state;
    string str1;
    str1 = DELETETAXRATEBYLONGTITLE ;
    str1 += checkOnApostrof(str).ascii();
    str1 += "';";
    state = doSQL(myconnection, str1.c_str()); 
    return state;
}

#define DELETEALLWARES "Delete from ware"
int PgRef::DeleteAllWare(void)
{
    //  puts("int PgRef::DeleteAllWare(void)");
    int state;
    string str = DELETEALLWARES;
    state = doSQL(myconnection, str.c_str());
    return state;
}

#define DELETEALLWAREGROUP "Delete from ware_group"
int PgRef::DeleteAllWareGroup(void)
{
   //   puts("int PgRef::DeleteAllWareGroup(void)");
    int state;
    string str = DELETEALLWAREGROUP;
    state = doSQL(myconnection, str.c_str());
    return state;
}
#define DELETEALLWAREBARCODE "Delete from ware_barcode"
int PgRef::DeleteAllWareBarcode(void)
{
    //  puts("int PgRef::DeleteAllWareBarcode(void)");
    int state;
    string str = DELETEALLWAREBARCODE;
    state = doSQL(myconnection, str.c_str());
    return state;
}
///////////////==========================FIND MAX====================================///////////

////////////////////////////////////////////////////////////////////////AUTO_SCHEME_ID////////////////////////////////////////////////////////////////////////////////
#define FINDNEXTAUTOSCHEMEIDFROMAUTOSCHEME "Select max(auto_scheme_id) from auto_scheme";

int PgRef::FindNextAutoSchemeIdFromAutoScheme(void)
{
     // puts("int PgRef::FindNextAutoSchemeIdFromAutoScheme(void)");
    int state;
    string str;
    int n = 0;
    str = FINDNEXTAUTOSCHEMEIDFROMAUTOSCHEME;
    state = doSQL(myconnection,str.c_str());
    if (state ==2)
    {
	n = PQntuples(result);
	  //printf("n = %d\n",n);
	if (n > 0 )
	{
	    str = PQgetvalue(result,0,0);
	    n = atoi(str.c_str());
	}
	PQclear(result);
    }
    return n;
}

////////////////////////////////////////////////////////////////////////AUTO_REDUCTION_ID//////////////////////////////////////////////////////////////////////

#define FINDNEXTAUTOREDIDFROMAUTORED "Select max(auto_reduction_id) from auto_reduction";

int PgRef::FindNextAutoRedIdFromAutoRed(void)
{
    // puts("int PgRef::FindNextAutoRedIdFromAutoRed(void)");
    int state;
    string str;
    int n = 0;
    str = FINDNEXTAUTOREDIDFROMAUTORED;
    state = doSQL(myconnection,str.c_str());
    if (state ==2)
    {
	n = PQntuples(result);
	  //printf("n = %d\n",n);
	if (n > 0 )
	{
	    str = PQgetvalue(result,0,0);
	    n = atoi(str.c_str()); 
	}
	PQclear(result);
    }
    return n;
}

////////////////////////////////////////////////////////////////////////TAX_RATE_ID////////////////////////////////////////////////////////////////////////////////

#define FINDNEXTTAXRATEIDFROMTAXRATE "Select max(tax_rate_id) from tax_rate";

int PgRef::FindNextTaxRateIdFromTaxRate(void)
{
   //   puts("int PgRef::FindNextTaxRateIdFromTaxRate(void)");
    int state;
    string str;
    int n = 0;
    str = FINDNEXTTAXRATEIDFROMTAXRATE;
    state = doSQL(myconnection,str.c_str());
    if (state ==2)
    {
	n = PQntuples(result);
	  //printf("n = %d\n",n);
	if (n > 0 )
	{
	    str = PQgetvalue(result,0,0);
	    n = atoi(str.c_str());
	}
	PQclear(result);
    }
    return n;
}

////////////////////////////////////////////////////////////////////////ORDER_ID////////////////////////////////////////////////////////////////////////////////

#define FINDNEXTORDERID "Select max(order_id) from tax_rate_group";

int PgRef::FindNextOrderIdInTaxRateGroup(void)
{
     // puts("int PgRef::FindNextOrderIdInTaxRateGroup(void)");
    int state;
    string str;
    int n = 0;
    str = FINDNEXTORDERID;
    state = doSQL(myconnection,str.c_str());
    if (state ==2)
    {
	n = PQntuples(result);
	  //printf("n = %d\n",n);
	if (n > 0 )
	{
	    str = PQgetvalue(result,0,0);
	    n = atoi(str.c_str()); 
	}
	PQclear(result);
    }
    return n;
}

////////////////////////////////////////////////////////////////////////TAX_GROUP_RATE_ID/////////////////////////////////////////////////////////////////////////

#define FINDNEXTIDFROMTAXRATEGROUP "Select max(id) from tax_rate_group";

int PgRef::FindNextIdFromTaxRateGroup(void)
{
     // puts("int PgRef::FindNextIdFromTaxRateGroup(void)");
    int state;
    string str;
    int n = 0;
    str = FINDNEXTIDFROMTAXRATEGROUP;
    state = doSQL(myconnection,str.c_str());
    if (state ==2)
    {
	n = PQntuples(result);
  //	printf("n = %d\n",n);
	if (n > 0 )
	{
	    str = PQgetvalue(result,0,0);
	    n = atoi(str.c_str()); 
	}
	PQclear(result);
    }
    return n;
}

////////////////////////////////////////////////////////////////////////TAX_GROUP_ID////////////////////////////////////////////////////////////////////////////////

#define FINDNEXTTAXGROUPIDFROMTAXGROUP "Select max(tax_group_id) from tax_group";

int PgRef::FindNextTaxGroupIdFromTaxGroup(void)
{
    //  puts("int PgRef::FindNextTaxGroupIdFromTaxGroup(void)");
    int state;
    string str;
    int n = 0;
    str = FINDNEXTTAXGROUPIDFROMTAXGROUP;
    state = doSQL(myconnection,str.c_str());
    if (state ==2)
    {
	n = PQntuples(result);
	  //printf("n = %d\n",n);
	if (n > 0 )
	{
	    str = PQgetvalue(result,0,0);
	    n = atoi(str.c_str());
	}
	PQclear(result);
    }
    return n;
}

////////////////////////////////////////////////////////////////////////WARE_GROUP_ID////////////////////////////////////////////////////////////////////////////////

#define FINDNEXTWAREGROUPIDFROMWAREGROUP "Select max(group_id) from ware_group";

int PgRef::FindNextWareGroupIdFromWare(void)
{
      //puts("int PgRef::FindNextWareGroupIdFromWare(void)");
   int state;
    string str;
    int n = 0;
    str = FINDNEXTWAREGROUPIDFROMWAREGROUP;
    state = doSQL(myconnection,str.c_str());
    if (state ==2)
    {
	n = PQntuples(result);
	  //printf("n = %d\n",n);
	if (n > 0 )
	{
	    str = PQgetvalue(result,0,0);
	    n = atoi(str.c_str()); 
	}
	PQclear(result);
    }
    return n;
}

////////////////////////////////////////////////////////////////////////INTERNAL_ID////////////////////////////////////////////////////////////////////////////////

#define FINDNEXTINTERNALIDFROMWARE "Select max(ware_id) from ware";

int PgRef::FindNextInternalIdFromWare(void)
{
      //puts("int PgRef::FindNextInternalIdFromWare(void)");
   int state;
    string str;
    int n = 0;
    str = FINDNEXTINTERNALIDFROMWARE;
    state = doSQL(myconnection,str.c_str());
    if (state ==2)
    {
	n = PQntuples(result);
	  //printf("n in my lib= %d\n",n);
	if (n > 0 )
	{
	    str = PQgetvalue(result,0,0);
	    n = atoi(str.c_str());
	    
	}
	PQclear(result);
    }
    return n;
}




























