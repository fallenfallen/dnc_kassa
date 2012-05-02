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
#include "reg_pgsql.h"


//#include <qdatetime.h>
#include <math.h>
#include <stdlib.h>
#include "addon_func.h"
#include <string>
#include "stringconvert.h"

using namespace std;

#define ORDERBYCODE "w.internal_id"    // 0
#define ORDERBYBARCODE "barcode" //1
#define ORDERBYTITLE "w.longtitle"         //2

//==================================================================================//
Registration::Registration()
{
    //puts("Registration:: Registration()");
    Set_Db("LinCash");
}

//==================================================================================//
Registration::Registration(SP * sparam, const char * conf_path) : PGSQL(conf_path)
{
    sp = sparam;
    log->PrintString(log_type, INFO, "Registration::Registration - Connect to db parameters form path = %s \n", conf_path);
}

//==================================================================================//
Registration::Registration(const char *dbname, const char *user, const char *password, const char *host, const char *hostaddr, const char *port, const char* log_path, SP * sparam) : PGSQL(dbname, user, password, host, hostaddr, port, log_path)
{
    log->PrintString(log_type, INFO, "AccessRights::AccessRights - Connect to db with  parameters : dbname = <%s>, user = <%s>, password = <%s>, host = <%s>, hostaddr = <%s>, port = <%s>, log_path = <%s>)\n", dbname, user, password, host, hostaddr, port, log_path);
    //printf("Registration::Registration(dbname = <%s>, user = <%s>, password = <%s>, host = <%s>, hostaddr = <%s>, port = <%s>, log_path = <%s>)\n", dbname, user, password, host, hostaddr, port, log_path);
    sp = sparam;
}

//==================================================================================//
Registration::~Registration() 
{
    //puts("Registration::~Registration()");
    //close(log);
    //Close_Connection();
}

//==================================================================================//
// weit - параметр содержит флаг дробного количества: 0 - штучный товар, 1 - весовой товар
//max_quantity - содержит максимальное количество товара, если требуется контроль количества(не разрешиены отрицательные остатки), либо -1 в противном случае.
//returns:
//0 - все нормально
//4 - не верный код товара
//7 - запрос в БД не был выполнен, смотреть логфайл
int Registration::Get_Max_Quantity(const char * int_code, int & weit, double & max_quantity)
{
//puts("int Registration::Get_Max_Quantity(const char * int_code, int & weit, double & max_quantity)");
  int er = Internal_Get_Product(int_code);
  if(er > 0) return er;
  
  int flags = atoi(PQgetvalue(result, 0, 8));
  
  weit = flags & ALLOWFRACTIONAL;
 
  if((flags & ALLOWNEGATIVEQUANTTITY) == ALLOWNEGATIVEQUANTTITY)
    max_quantity = -1;
  else
    max_quantity = atof(PQgetvalue(result, 0, 3));
  return 0;
}
 
 
//==================================================================================//
//code - код товара (внутренний или штрих-)
//q - количество товара
//rec - указатель на структуру чека
//fl : true - использовать штрих-код товара, false - исрпользовать внутренний код товара // этот параметр не нужен - у товара всегда есть внутренний код
//returns : 
//0 - все нормально
//5 - нет требуемого количества товара
//7 - запрос в БД не был выполнен, смотреть логфайл
//8 - попытка продать штучный товар как весосой    
int Registration::Quantity_Control(const char *code, double q, const char* addition_id)
{
    //puts("Registration::Quantity_Control(const char *code, double q)");
    int er;
    
    if ( atoi(addition_id) > 0 )  {
	er = Get_Product_By_Addition_Id(addition_id);
	if ( er != 0 ) return er;
    } else {  
	er = Internal_Get_Product(code);
	if ( er > 0 ) return er;
    }
    
    er = Check_Product_Atr(q, false);
    if ( er > 0 ) return er;
    
    return 0;
}

//==================================================================================//
int Registration::Get_Pattern_Param(const char *code, double *quantity, char *internal_code)
{
   //puts("int Registration::Get_Pattern_Param(const char *code, double *quantity, char *internal_code)");
   
   char *prefix = (char *) calloc(3, sizeof(char));
   if ( prefix == NULL ) printf("NULL CALLOC!!! parameter prefix\n");
   *prefix = *code;
   *(prefix + 1) = *(code + 1);
   /*puts("***************************************************************");
   printf("prefix = %s \n", prefix);*/
   if ( (*prefix<'0') || (*prefix> '9' ) || (*(prefix+1) <'0') || (*(prefix+1) > '9')  )  {   free(prefix);  return 1;   }
   /*
   puts("***************************************************************");
   printf("prefix = %s \n", prefix);
  puts("***************************************************************");
   */
   
   int res = Query_Pattern(prefix);
   free(prefix);
   
   //printf("res = %d \n", res);
   int ret = 0;
   
   if ( res != 2 ) return 7; 	// Невыполнен запрос в базу, нет коннекта


   int n = PQntuples(result);
   if ( n <= 0 )  return 1; 	//Нет шаблона с таким префиксом
   
   
   int len = PQgetlength(result, 0, 0);
   //Неверная длина весового штрих-кода для префикса штрих-кода
   //if ( len == 26 ) len = 13;
   //-----(*)-----Zuskin-----06/10/2011-----
   // !!! можно отбросить последнюю (контрольную)  цифру штрих-кода !!!
   //if((len != (int) strlen(code)) && ((int)len/2 != (int) strlen(code))) return 2; 
   int clen = strlen(code);
   if ( (len != clen) && ((int)len/2 != clen) && ((len-1) != clen) && ((int)len/2 - 1 != clen) ) return 2; 
   //---------------------------------------

   char * pattern = (char *) calloc(len + 2, sizeof(char));
   if(pattern == NULL) printf("NULL CALLOC!!! parameter pattern\n");
   
   strcpy(pattern, PQgetvalue(result, 0, 0));
   double multiplier = atof(PQgetvalue(result, 0, 1));
   PQclear(result);
   
   char* tmp = (char*) calloc(len+2, sizeof(char));
   if (len == 26) {
	for (int k=0; k<13; k++) *(tmp+k) = *(pattern+2*k+1);	
   } else strcpy(tmp, pattern);
   free(pattern); 

   char * copy = (char *) calloc(strlen(code) + 1, sizeof(char));
   if(copy == NULL) printf("NULL CALLOC!!! parameter copy\n");
   strcpy(copy, code);
   int i = 0;
   int j = 0;
   char ch;
   bool code_fl = false;
   bool quant_fl = false;

   while ( j != -1 )        {
	//printf("while j = %d, i = %d, ch = %d\n", j, i, ch);
	if (i == 0) ch = *(tmp + j);
	if ((j == len - 1) || (ch != *(tmp + j + 1)))  {
		//printf("*(tmp + j + 1) code = %d \n", tmp[j+1]);
		int l = strlen(copy);
		//printf("strlen(copy) = %d\n", l);
		//printf("copy - <%s>\n", copy);
		//printf("ch code = %d \n", ch);
     
		switch(ch)  {
			   //  case 'K' :
			case -102 :   {
			       //printf("case K\n");

			       //printf("2  case K\n");
			       for(int k = 0; k < i + 1; k ++) *(internal_code + k) = *(copy + k);
			       //   strncpy(*internal_code, copy, i + 1);
			       //printf("internal_code after strncpy <%s>, adress [%p]\n", internal_code, internal_code);
			       free(copy);
			       copy = (char*) calloc(l - i + 1, sizeof(char));
			       if(copy == NULL) printf("NULL CALLOC!!! parameter copy\n");
			       for(int k = 0; k < l - i + 1; k++) *(copy + k) = *(code + j + 1 + k);
			       code_fl = true;
			       i = 0;
			       break;
			}
			   //  case 'M' :
		       case -100 :   {
			       //printf("case M\n");

			       char * q_str = (char *) calloc(i + 1, sizeof(char));
			       if(q_str == NULL) printf("NULL CALLOC!!! parameter q_str\n");
			       //strncpy(q_str, copy, i + 2);
			       for(int k = 0; k < i + 1; k++) *(q_str + k) = *(copy + k);
			       //printf("q_str after strncpy <%s>\n", q_str);
			       *quantity = atof(q_str) * multiplier;
			       free(q_str);
			       free(copy);
			       copy = (char*) calloc(l - i, sizeof(char));
			       if(copy == NULL) printf("NULL CALLOC!!! parameter copy\n");
			       for(int k = 0; k < l - i; k++) *(copy + k) = *(code + j  +  1 + k);
			       quant_fl = true;
			       i = 0;
			       break;
			   }
			   // case 'H' :
		       case -99 :   {
			       //printf("case H\n");
			       free(copy);
			       copy = (char*) calloc(l - i, sizeof(char));
			       if(copy == NULL) printf("NULL CALLOC!!! parameter copy\n");
			       //printf("len of copy %d\n", l - i);
			       for(int k = 0; k < l - i; k++) *(copy + k) = *(code + j + 1 + k);
			       i = 0;
			       break;
			}
			default:   {
				free(copy);
				free(tmp);
				return 1;		
			}
		}			
	}   else i++;
	j++;
	if(code_fl && quant_fl) j = -1;
   }
   free(copy);
   free(tmp);
   
   //if(ret == 0) printf("internal_code in the end of Get_Pattern_Param(...) <%s>, adress [%p]\n", internal_code, internal_code);
   
   return ret;
}

//==================================================================================//
//code - код товара (внутренний или штрих-)
//q - количество товара
//rec - указатель на структуру чека
//code_fl : true - использовать штрих-код товара, false - исрпользовать внутренний код товара
//return_fl : true - производится возврат товара
//copy_fl : true - производится подбор для копии чека
//npos : позиция чека, в которую будет добавлен товар
//returns : 
//0 - все нормально
//1 - продажа данного товара запрещена в карточке товара
//2 - возврат данного товара запрещен в карточке товара
//3 - не верный штрих-код товара 
//4 - не верный код товара
//5 - нет требуемого количества товара
//6 - не верное имя товара в БД 
//7 - запрос в БД не был выполнен, смотреть логфайл
//8 - попытка продать штучный товар как весосой
//9 - истек срок годности
//10 - переполнение при умножении
//11 - переполнение при сложении
//12 - не верный весовой штрих-код товара, товар с указанным кодом не найден

//-----(*)-----Zuskin-----13/10/2011-----
//13 - запрещена регистрация товара без ввода количества

//int Registration::New_Receipt_Record(const char *code, double q, Receipt *rec, bool code_fl, bool return_fl, bool copy_fl, int &npos, const char * addition_id)
int Registration::New_Receipt_Record(const char *code, double q, Receipt *rec, bool code_fl, bool return_fl, bool copy_fl, int &npos, const char * addition_id, bool qttyset)
//---------------------------------------
{
    //puts("Registration::New_Receipt_Record(char *code, double q, Receipt *rec, bool code_fl, bool return_fl, bool copy_fl, int &npos)");
    printf("New_Receipt_Record (code = %s, q = %f, code_fl = %d, return_fl = %d, copy_fl = %d, addition_id = %s) \n", code, q, code_fl, return_fl, copy_fl, addition_id);
    
    //printf("Add position ( (bar)code = %s, quantity = %f ) \n", code, q);
    //printf("Add New Receipt Position adition_id = %s \n", addition_id);
    int er = 0;
    int pattern_er = 0;
    
    char *internal_code = NULL;    
    char addition_id_str[10];
    strcpy(addition_id_str, "");
    
    //printf("q == %f \n", q);
    if ( rec->GetStateofClosing() ) rec->Clear_Receipt();
    /********************************************///   
    // ищем по коду дополнения товара(товар с указанными значениями дх)
    if ( atoi(addition_id) > 0 ) {
	int res = Get_Product_By_Addition_Id(addition_id);
	if (res == 7 ) return 7;
	if ( res != 0 ) return 4;
	
	internal_code = (char*) calloc(PQgetlength(result, 0, 1)+1, sizeof(char));
	strcpy(internal_code, PQgetvalue(result, 0, 1));
	
	
	strcpy(addition_id_str, addition_id);
    } else if ( code_fl )   {
	// Ищем по штрих-коду
	//printf("barcode = %s \n", code);
	/* определяет шк, код цену количество, срок годности название флаги группы схемы коэффициенты*/		
	er = Bar_Get_Product(code);
	//printf("Bar_Get_Product(code) returns - %d \n", er);
	
	if ( er == 7 ) return 7;
	
	if ( er != 3 ) {
	    //printf("PQgetvalue(result, 0, 1) = %s \n", PQgetvalue(result, 0, 1));
	    internal_code = (char*) calloc(PQgetlength(result, 0, 1)+1, sizeof(char));
	    strcpy(internal_code, PQgetvalue(result, 0, 1));
	    //printf("get by barcode -- addition_id = %s \n", PQgetvalue(result, 0, 13));
	    strcpy(addition_id_str, PQgetvalue(result, 0, 13));
	    //printf("internal_code = %s addition_code = %s \n", internal_code, addition_id_str);
	}
	
	if ( er == 3 )     {
	    // Указанный штрих-код не найден,	
	    // Поищем в весовых штрих-кодах
	    double quantity;
	    internal_code = (char*) calloc(strlen(code) + 1, sizeof(char));
	    pattern_er = Get_Pattern_Param(code, &quantity, internal_code);	// выделяет внутрений код товара и его количество из весового шк 
	    int k = 0;	    
	    
	    int internal_code_by_int = atoi(internal_code); 
	    strcpy(internal_code, "");	    
	    
	    itoa(internal_code_by_int, internal_code);
	    //printf("Get_Pattern_Param(weight code = %s , &quantity = %f, internal_code = %s) = %d\n", code, quantity, internal_code, pattern_er);
	    
	    if ( pattern_er == 0 )  {
		q = quantity;		    
		er = Internal_Get_Product(internal_code);	/* определяет параметры товара по его вн. коду*/
		//if(er == 4) er = 12;
	    }  else if (pattern_er == 7) er = 7;
	    
	}
    } else {
	// Поиск по внутреннему коду
	//printf("code = %s \n", code);
	internal_code = (char *) calloc(strlen(code) + 1, sizeof(char));
	strcpy(internal_code, code);
	er = Internal_Get_Product(internal_code);	/* определяет параметры товара по его вн. коду*/
	
    }
    /********************************************///
    //printf("er must be 0 == %d \n", er);
    if ( er != 0 ) return er;
    
    printf("internal_code = %s addition_code = %s \n", internal_code, addition_id_str);
    Get_Check_Position(internal_code, false, npos, rec, addition_id_str);	
    // определяет номер позиции в чеке куда добавить новую позицию   
    printf("code = %s npos = %d q = %f \n ", internal_code, npos, q);
    
    //er = 0;
    //если не копия чека и не возврат
    //if ( return_fl ) puts("RETURN"); else puts("!!! RETURN");
    //if ( copy_fl ) puts("COPY"); else puts("!!! COPY");
    
    if ( !copy_fl )  {
	// q =  при не объединении строк, нужно посчитать количество товара с таким кодом
	double tmp = 0;
	//if ( (sp->association_positions == NOTASSOCIATION) && !return_fl && !copy_fl ) tmp = rec->QuantityWareByCode(code, true);	
	if (atoi(addition_id_str) > 0) tmp = rec->QuantityWareByCode(addition_id_str, 2);
	else tmp = rec->QuantityWareByCode(internal_code, true);
	
	printf("QuantityWareByCode(internal_code = %s) = %f \n", internal_code, tmp);
	
	double coef = atof(PQgetvalue(result, 0, 12));			
	if ( coef == 0 )     coef = 1;
	
	if (copy_fl || return_fl) {  coef = 1;  } // поскольку из транзакций считывается кол-во уже с учетом коэффициента
	
	//printf("quantity = %f barcode_coeff = %f \n", q, coef);
	/*// проверка на разрешение редактирования количества
	if ( !copy_fl && !return_fl ) {
	    int flags = atoi(PQgetvalue(result, 0, 8));
	    if ( ( (flags & QUANTITYEDIT) == 0 ) && (q != 1) )    return 98;
	}*/
	
	if  ( npos != -1 ) {
	    int quantity_npos = rec->GetPosElemRef(npos)->quantity;
	    //printf("quantity_npos = %f \n", quantity_npos);
	    //printf("Check_Product_Atr(q + quantity_npos = %f)\n", q + quantity_npos);
	    //-----(*)-----Zuskin-----13/10/2011-----
	    //er = Check_Product_Atr(q*coef + quantity_npos, return_fl);	// проверят возможность продажи товара по флагам	
	    er = Check_Product_Atr(q*coef + quantity_npos, return_fl, qttyset);	// проверят возможность продажи товара по флагам	
	    //---------------------------------------
	} else {
	    //printf("Check_Product_Atr(q + tmp = %f)\n", q + tmp);
	    //-----(*)-----Zuskin-----13/10/2011-----
	    //er = Check_Product_Atr(q*coef + tmp, return_fl);		// проверят возможнось продажи товара по флагам
	    er = Check_Product_Atr(q*coef + tmp, return_fl, qttyset);		// проверят возможнось продажи товара по флагам
	    //---------------------------------------
	}
    }
    
    printf("after check attribute er = %d \n", er);
    
    /* добавление позиции в чек */
    if ( er == 0 )    {
	if ( npos == -1 )    {
	    
	    //printf("q = %f \n", q);
	    //printf("1 coeff == %f \n", coef);
	    double coef = atof(PQgetvalue(result, 0, 12));		
	    if ( coef == 0 )     coef = 1;
	    if (copy_fl || return_fl) {coef = 1;} // поскольку из транзакций считывается кол-во уже с учетом коэффициента
	    
	    char barcode[20] ;
	    strcpy(barcode, PQgetvalue(result, 0, 0));
	    
	    //char internal_code[20];
	    //strcpy(internal_code, PQgetvalue(result, 0, 1));
	    
	    char* longtitle = (char*) calloc(PQgetlength(result, 0, 7)+1, sizeof(char));
	    strcpy(longtitle, PQgetvalue(result, 0, 7));
	    char* shortcut = (char*) calloc(PQgetlength(result, 0, 6)+1, sizeof(char));	
	    strcpy(shortcut, PQgetvalue(result, 0, 6));
	    
	    double price = atof(PQgetvalue(result, 0, 2));
	    int flags = atoi(PQgetvalue(result, 0, 8));
	    int auto_scheme = atoi(PQgetvalue(result, 0, 10));
	    int fix_scheme = atoi(PQgetvalue(result, 0, 11));
	    int tax_group = atoi(PQgetvalue(result, 0, 9));
	    double min_price = atof(PQgetvalue(result, 0, 4));
	    
	    int section = atoi(PQgetvalue(result, 0, 14));
	    
	    bool sale_limit_by_time = false;
	    //---(*)---Zuskin---24/02/2012---
	    //if ( strcmp(PQgetvalue(result, 0, 15), "1") == 0 )
	    string slbt = PQgetvalue(result, 0, 15);
	    if (slbt > "0") 
	    //-------------------------------
		sale_limit_by_time = true;
	     	    
	    printf("section of ware = %d \n", section);
	    
	    PQclear(result);
	    
	    //printf("ware make longtitle addition id = %s \n", addition_id_str);
	    if ( atoi(addition_id_str) > 0 ) {
		GetAdditionLongtitle(longtitle, shortcut, addition_id_str);
	    }
	    
	    rec->Add_Record(barcode, internal_code, shortcut, longtitle, q * coef, 
			    price, flags, !code_fl, auto_scheme, fix_scheme, tax_group, min_price, addition_id, section);
	    
	    rec->GetPosElemRef(rec->record_num-1)->sale_limit_by_time = sale_limit_by_time;

	    free(longtitle);
	    free(shortcut);
	    //Add_Taxes(rec);
	    npos = rec->record_num - 1;
	}    else      {
	    double coef = atof(PQgetvalue(result, 0, 12));		
	    if ( coef == 0 )     coef = 1;
	    q = coef * q;
	    PQclear(result);
	    //printf("rec->GetPosElemRef(npos)->quantity = %f \n", rec->GetPosElemRef(npos)->quantity);
	    //printf("q = %f \n", q);
	    if ( (er = Overflow_Control(rec->GetResultSum(), (rec->GetPosElemRef(npos)->quantity + q) * rec->GetPosElemRef(npos)->price, rec->GetPosElemRef(npos)->sum)) == 0 )   {
		rec->GetPosElemRef(npos)->quantity += q;
		//-----(*)-----Zuskin-----14/06/2011-----
		//rec->GetPosElemRef(npos)->sum = rec->GetPosElemRef(npos)->quantity * rec->GetPosElemRef(npos)->price;
		rec->GetPosElemRef(npos)->sum = Round ( (rec->GetPosElemRef(npos)->quantity * rec->GetPosElemRef(npos)->price ) * 100) / 100;
		//---------------------------------------
	    }
	}
    }
	
    if ( internal_code != NULL ) free(internal_code);
    
    /********************************************/
    return er;
}

//==================================================================================//

//---(*)---Zuskin---24/02/2012---
//#define GET_PRODUCT_BY_ADDITION "select b.barcode,  w.internal_id, a.addition_price, a.addition_quantity, w.min_price, now() <= w.best_before, w.shortcut, w.longtitle, w.flags, w.tax_group_id, w.auto_scheme_id, null as fix_scheme_id, b.coefficient, a.addition_code, w.section, w.sale_limit_by_time, slt.sale_access_by_time FROM ware_additions a LEFT JOIN ware w ON a.ware_id = w.ware_id LEFT JOIN ware_barcode b ON b.addition_code = a.addition_code LEFT JOIN sale_limit_by_time_view slt ON true WHERE a.addition_code = '"
#define GET_PRODUCT_BY_ADDITION "select b.barcode,  w.internal_id, a.addition_price, a.addition_quantity, w.min_price, now() <= w.best_before, w.shortcut, w.longtitle, w.flags, w.tax_group_id, w.auto_scheme_id, null as fix_scheme_id, b.coefficient, a.addition_code, w.section, w.sale_limit_by_time, slt.sale_access_by_time FROM ware_additions a LEFT JOIN ware w ON a.ware_id = w.ware_id LEFT JOIN ware_barcode b ON b.addition_code = a.addition_code LEFT JOIN sale_limit_by_time_view slt ON slt.id = w.sale_limit_by_time WHERE a.addition_code = '"
//-------------------------------
int Registration::Get_Product_By_Addition_Id(const char* addition_id) {
    
    char str[strlen(GET_PRODUCT_BY_ADDITION) + strlen(addition_id) + 3];
    int state = 0;
    strcpy(str, GET_PRODUCT_BY_ADDITION);
    strcat(str, addition_id);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    if ( state != 2 ) return 7;
    
    if ( PQntuples(result) == 0 ) {
	PQclear(result);
	return 4;
    }
    if ( PQntuples(result) == 1 ) return 0;
    else return 4;
    
}
//==================================================================================//
#define GET_ADDITIONAL_FEATURES_VALUES "SELECT value_value FROM wa_fv_view WHERE addition_code = '"
int Registration::GetAdditionLongtitle(char * &longtitle, char * &shortcut, const char* addition_id) {
    
    char str[strlen(GET_ADDITIONAL_FEATURES_VALUES) + strlen(addition_id) + 3];
    int state;
    strcpy(str, GET_ADDITIONAL_FEATURES_VALUES);
    strcat(str, addition_id);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    if ( state != 2 ) return -1;
    
    if ( PQntuples(result) < 1 )  return 0;
    
    string result_addition_text;
    result_addition_text += "(";
    result_addition_text += PQgetvalue(result, 0, 0);
    
    for ( int i = 1; i < PQntuples(result); i++ ) {
	result_addition_text += ", ";
	result_addition_text += PQgetvalue(result, i, 0);
    }
    
    result_addition_text += ")";
    
    char temp[202];
    
    strcpy(temp, longtitle);
    free(longtitle);
    longtitle = (char*) calloc(strlen(temp) + result_addition_text.length() + 3, sizeof(char));
    strcat(longtitle, temp);
    strcat(longtitle, " ");
    strcat(longtitle, result_addition_text.c_str());
    
    
    strcpy(temp, shortcut);
    free(shortcut);
    shortcut = (char*) calloc(strlen(temp) + result_addition_text.length() + 3, sizeof(char));
    strcat(shortcut, temp);
    strcat(shortcut, " ");
    strcat(shortcut, result_addition_text.c_str());    
    
    PQclear(result);
    return 0;
    
}
//==================================================================================//
//7 - запрос в БД не был выполнен, смотреть логфайл
int Registration::Add_Taxes(Receipt *rec, int npos)
{

    int ret = 0;
    int n = 0;
    
    if (rec->GetPosElemRef(npos)->tax_group != 0)  {
	char * taxgr_str = NULL;
	taxgr_str = itoa(rec->GetPosElemRef(npos)->tax_group);
	if ( taxgr_str == NULL ) log->PrintString(log_type, ERROR, "Registration::Add_Taxes - Error of convert from integer = %d to char* = NULL \n", rec->GetPosElemRef(npos)->tax_group);

	int res = Query_Taxes(taxgr_str); //trg.tax_rate_id - 0, trg.base_flag - 1, tr.longtitle - 2, tr.shortcut - 3, tr.value - 4
	if(taxgr_str) free(taxgr_str);
	if(res == 2)    {
	    n = PQntuples(result);
	    //printf("n = %d \n", n);
	    if ( n > 0 ) rec->Add_Taxes_Info(rec->GetPosElemRef(npos)->tax_group, PQgetvalue(result, 0, 5),  n);
	    else rec->Add_Taxes_Info(rec->GetPosElemRef(npos)->tax_group, "",  n);
	    //printf("name = PQgetvalue(result, 0, 0) = %s \n", PQgetvalue(result, 0, 5));
	    
	    //else rec->Add_Taxes_Info(rec->GetPosElemRef(npos)->tax_group, PQgetvalue(result, 0, 5),  n);
	    if(n > 0)      {
		bool ch_base;
		for(int i = 0; i < n; i ++)        {
		    if(!strcmp(PQgetvalue(result, i, 1), "t"))             ch_base = true;
		    else            ch_base = false;
		    TaxRate taxrate(atoi(PQgetvalue(result, i, 0)) /*rate_id*/, atof(PQgetvalue(result, i, 4)) /*rate*/, ch_base /*change_base*/,  PQgetvalue(result, i, 3)/*shortcut*/, PQgetvalue(result, i, 2) /*longtitle*/);
		    rec->GetTaxElemRef(npos)->Add_Tax_Rate(taxrate);          
		}
	    }
	    PQclear(result);
	}	else ret = 7;
    }  else
	rec->Add_Taxes_Info(rec->GetPosElemRef(npos)->tax_group, "", n);
    
    return ret;
}

//==================================================================================//
int Registration::Overflow_Control(double checksum, double newsum, double oldsum)
{
// puts("int Registration::Overflow_Control(double checksum, double newsum, double oldsum)");
// puts("======================================");
 // printf("sum = %f,\n newsumpos = %f,\n oldsumpos %f\n", checksum, newsum, oldsum);
// puts("======================================");

  if(newsum > 99999999.99) return 10;
  if(checksum - oldsum + newsum > 99999999.99) return 11;
  return 0;
}

//==================================================================================//
//code - штрих-код товара
//returns :
//0 - все нормально
//3 - не верный штрих-код товара
//7 - запрос в БД не был выполнен, смотреть логфайл
int Registration::Bar_Get_Product(const char *code)
{
    //printf("int Registration::Bar_Get_Product(const char * barcode = %s) \n", code);
    int res = Query_by_Bar_Code(code);
    //printf("res ==  %d \n", res);
    if ( res != 2 ) return 7;
    
    int n = PQntuples(result);
    if ( n > 0 ) return 0;
    	
    PQclear(result);      
    
    res = Query_Addition_by_Bar_Code(code);
    if ( res != 2 ) return 7;
    
    n = PQntuples(result);
    if ( n > 0 ) return 0;
    
    return 3;    
}

//==================================================================================//
//code - внутренний код товара
//returns : 
//0 - все нормально
//4 - не верный код товара
//7 - запрос в БД не был выполнен, смотреть логфайл
int Registration::Internal_Get_Product(const char *code)
{
//puts("Registration::Internal_Get_Product(const char *code)");
  int res = Query_by_Internal_Code(code);
  if(res == 2)
  {
    int n = PQntuples(result);
    if(n > 0) return 0;
    else
    {    
      PQclear(result);
      return 4;
    }
  }
  else return 7;
}

//==================================================================================//
//q - необходимое количество товара
//return_fl : true - производится возврат товара
//returns : 
//0 - все нормально
//1 - продажа данного товара запрещена в карточке товара
//2 - возврат данного товара запрещен в карточке товара
//5 - нет требуемого количества товара
//6 - не верное имя товара в БД
//8 - попытка продать штучный товар как весосой
//9 - истек срок годности
// 93 - продажа  товаров запрещена в настройках контроля регистраций
// 94 - возврат данного товара запрещен в настройках контроля регистраций
// 95 - истек срок годности
// 96 - продажа весового товара запрещена в настройках контроля регистраций
// 97 - редактирование цены запрещено в карточке товара
// 98 - запрещено редактирование количества
// 107 - цена товара меньше его минимальной цены
// 108 - ограничение продажи по времени

//-----(*)-----Zuskin-----13/10/2011-----
// 13 - продажа без ввода количества запрещена

//int Registration::Check_Product_Atr(double q, bool return_fl)
int Registration::Check_Product_Atr(double q, bool return_fl, bool qttyset)
//---------------------------------------
{
    printf("Check_Product_Atr(q = %f)\n", q);
    
    //  int res = 0;
    int flags = atoi(PQgetvalue(result, 0, 8));
    
    // Проверка параметров при регистрации товара
    
    //-----(+)-----Zuskin-----13/10/2011-----
    if ( ((flags & ALLOWWITHOUTQTTY) == 0 ) && (!qttyset)) {
	return 115;
    }
    //---------------------------------------
    
    // Проверка флага 'разрешить продажи'
    //printf("sp->control_sale = %d \n", sp->control_sale); // 0- ware card 1- да 2- нет
    if ( !return_fl && sp->control_sale == NO ) return 93;
    if ( !return_fl && (sp->control_sale == WARECARD) && ( (flags & ALLOWREGISTRATION) == 0 ) ) return 1; 
    
    if ( return_fl && sp->control_back == NO ) return 94;
    
    if ( return_fl && sp->control_back == WARECARD && ((flags & ALLOWRETURN) == 0) ) return 2;   
    
    //if ( ( (flags & QUANTITYEDIT) == 0 ) && (q != 1) )    return 98;
    //printf("internal_tmp = %d \n", internal_tmp);
    //if ( ((flags & QUANTITYEDIT) == 0) && internal_tmp != 1 ) return 98;
    
    
    /*if ( !return_fl ) {
	//if ( !return_fl && sp.) {
    }*/
    
    //printf("q = %f \n", q);
    // Проверка соответствия флагу 'разрешить дробное количество'
    if ( sp->control_double == NO && ((q - floor(q)) != 0)  ) return 102;
    if ( sp->control_double == WARECARD && (flags & ALLOWFRACTIONAL) == 0 && ((q - floor(q)) > 0.000001 )  ) return 8;
    
    if ( !return_fl ) {
	bool remains_control = false;
	// списание остатков
	if ( sp->control_remains == YES ) remains_control = true;
	if ( (sp->control_remains == WARECARD) && ((int) (flags & WRITEOFFREMAINS) != 0) ) remains_control =  true; 
	
	// отрицательные остатки
	if ( (sp->control_negative_remains == NO) && (remains_control) && (atof(PQgetvalue(result, 0, 3)) < q) ) return 5;    
	if ( (sp->control_negative_remains == WARECARD) && (remains_control) && ((int) (flags & ALLOWNEGATIVEQUANTTITY) == 0) && (atof(PQgetvalue(result, 0, 3)) < q) )   return 5;    
    }
    
    //Проверка срока годности
    if ( !return_fl ) {    
	if ( sp->control_exparation == 1 && strcmp(PQgetvalue(result, 0, 5), "f") == 0  ) {
	    return 95;
	}
    }
    
    // проверка ограничения продаж по времени
    //---(*)---Zuskin---24/02/2012---
    //if ( strcmp(PQgetvalue(result, 0, 15), "1") == 0 &&  strcmp(PQgetvalue(result, 0, 16), "f") == 0 )
    string slbt = PQgetvalue(result, 0, 15);
    if ( (slbt > "0") &&  strcmp(PQgetvalue(result, 0, 16), "f") == 0 )
    //-------------------------------
	return 108;
    
    return 0;
}

//==================================================================================//
// Проверка ограничения продажи по времени
//---(*)---Zuskin---24/02/2012---
/*#define GET_SALE_ACCESS_BY_TIME "select cast(now()-start_time as TIME) > count_time FROM sale_time_limit "
bool Registration::getSaleAccessByTime() {
    int state = doSQL(myconnection, GET_SALE_ACCESS_BY_TIME);
    if (state != 2) return true;
    
    bool sale_access = true;

    if ( PQntuples(result) == 0 ) 
        return true;
    
    if ( strcmp(PQgetvalue(result, 0, 0), "f") == 0 )
	sale_access = false;
    
    PQclear(result);
    return sale_access;
}*/
#define GET_SALE_ACCESS_BY_TIME "SELECT slt.sale_access_by_time FROM Ware w JOIN sale_limit_by_time_view slt ON slt.id = w.sale_limit_by_time WHERE w.internal_id = '"
bool Registration::getSaleAccessByTime(const char* code) {
    string qstr = "";
    qstr += GET_SALE_ACCESS_BY_TIME;
    qstr += code;
    qstr += "'";
    int state = doSQL(myconnection, qstr.c_str());
    if (state != 2) return true;
    
    bool sale_access = true;

    if ( PQntuples(result) == 0 ) 
        return true;
    
    if ( strcmp(PQgetvalue(result, 0, 0), "f") == 0 )
	sale_access = false;
    
    PQclear(result);
    return sale_access;
}
//---------------------------------

//==================================================================================//
//Определиние позиции чека в зависимости от настройки объединения позиций
//code - код товара (внутренний или штрих-)
//fl : true - использовать штрих-код товара, false - исрпользовать внутренний код товара
//npos : позиция чека, в которую будет добавлен товар
void Registration::Get_Check_Position(const char *code, bool fl, int &npos, Receipt *rec, const char* addition_id)
{
    npos = -1;
    //Не объединяем позиции 
    if ( sp->association_positions == NOTASSOCIATION ) {
	npos = -1;
	return;
    }
    
    //Объединяем все позиции с одинаковыми товарами
    if ( sp->association_positions == ALLASSOCIATION ) {
	npos = rec->FindEquilPosition(code, !fl, addition_id);	
	return;
    }
    
    //Объединяем тольно позиции с штучным товаром
    if(sp->association_positions == NOTFACTORIALASSOCIATION)   {
	if ( (atoi(PQgetvalue(result, 0, 8)) & ALLOWFRACTIONAL) == 0 )  {	
	    npos = rec->FindEquilPosition(code, !fl, addition_id);		
	    return;
	}
    }
    
}

//==================================================================================//
//---(*)---Zuskin---24/02/2012---
//#define QUERY_INT "SELECT wb.barcode, w.internal_id, w.price, w.quantity, w.min_price, now() <= w.best_before, w.shortcut, w.longtitle, w.flags, w.tax_group_id, w.auto_scheme_id, null as fix_scheme_id, wb.coefficient, -1 as addition_id, w.section , w.sale_limit_by_time, slt.sale_access_by_time FROM Ware w LEFT JOIN ware_barcode wb ON wb.internal_id = w.internal_id LEFT JOIN sale_limit_by_time_view slt ON true WHERE w.internal_id = '"
#define QUERY_INT "SELECT wb.barcode, w.internal_id, w.price, w.quantity, w.min_price, now() <= w.best_before, w.shortcut, w.longtitle, w.flags, w.tax_group_id, w.auto_scheme_id, null as fix_scheme_id, wb.coefficient, -1 as addition_id, w.section , w.sale_limit_by_time, slt.sale_access_by_time FROM ware w LEFT JOIN ware_barcode wb ON wb.internal_id = w.internal_id LEFT JOIN sale_limit_by_time_view slt ON slt.id = w.sale_limit_by_time WHERE w.internal_id = '"
//-------------------------------
/*#define QUERY_INT "SELECT barcode, internal_id, price, quantity, min_price, now() <= best_before, shortcut, longtitle, flags, tax_group_id, auto_scheme_id, fix_scheme_id FROM Ware WHERE internal_id = "*/
int Registration::Query_by_Internal_Code(const char *code)
{
//puts("Registration::Query_by_Internal_Code(const char *code)");
  int state;
  char *str; 
    
  //Поиск данных на товар по внутреннему коду
  str = (char *) calloc(strlen(QUERY_INT) + strlen(code) + 3, sizeof(char));
if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_INT);
  strcat(str, code);
  strcat(str, "'");
  //printf("str = %s \n", str);
  state = doSQL(myconnection, str);
  free(str);
  return state;
}

//==================================================================================//
//---(*)---Zuskin---01/03/2012---
//#define QUERY_BAR "SELECT wb.barcode, w.internal_id, w.price, w.quantity, w.min_price, now() <= w.best_before, w.shortcut, w.longtitle, w.flags, w.tax_group_id, w.auto_scheme_id, null as fix_scheme_id, wb.coefficient, -1 as addition_code, w.section, w.sale_limit_by_time, slt.sale_access_by_time FROM Ware w, ware_barcode wb LEFT JOIN sale_limit_by_time_view slt ON true WHERE wb.internal_id = w.internal_id AND  wb.barcode = '"
#define QUERY_BAR "SELECT wb.barcode, w.internal_id, w.price, w.quantity, w.min_price, now() <= w.best_before, w.shortcut, w.longtitle, w.flags, w.tax_group_id, w.auto_scheme_id, null as fix_scheme_id, wb.coefficient, -1 as addition_code, w.section, w.sale_limit_by_time, slt.sale_access_by_time FROM ware w LEFT JOIN ware_barcode wb ON wb.internal_id = w.internal_id LEFT JOIN sale_limit_by_time_view slt ON slt.id = w.sale_limit_by_time WHERE wb.barcode = '"
//-------------------------------
/*#define QUERY_BAR "SELECT barcode, internal_id, price, quantity, min_price, now() <= best_before, shortcut, longtitle, flags, tax_group_id, auto_scheme_id, fix_scheme_id FROM Ware WHERE barcode = '"*/
int Registration::Query_by_Bar_Code(const char *code)
{
    //puts("Registration::Query_by_Bar_Code(const char *code)");
    //Поиск данных на товар по штрих коду
    int state;
    char *str;
    
    str = (char *) calloc(strlen(QUERY_BAR) + strlen(code) + 2, sizeof(char));
    strcpy(str, QUERY_BAR);
    
    strcat(str, code);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    free(str);  
    
    return state;
}

//==================================================================================//
//---(*)---Zuskin---24/02/2012---
//#define QUERY_BAR_ADDITION " select b.barcode,  w.internal_id, a.addition_price, a.addition_quantity, w.min_price, now() <= w.best_before, w.shortcut, w.longtitle, w.flags, w.tax_group_id, w.auto_scheme_id, null as fix_scheme_id, b.coefficient, a.addition_code, w.section, w.sale_limit_by_time, slt.sale_access_by_time FROM ware_additions a LEFT JOIN ware w ON a.ware_id = w.ware_id LEFT JOIN ware_barcode b ON b.addition_code = a.addition_code LEFT JOIN sale_limit_by_time_view slt ON true WHERE b.barcode = '" 
#define QUERY_BAR_ADDITION " select b.barcode,  w.internal_id, a.addition_price, a.addition_quantity, w.min_price, now() <= w.best_before, w.shortcut, w.longtitle, w.flags, w.tax_group_id, w.auto_scheme_id, null as fix_scheme_id, b.coefficient, a.addition_code, w.section, w.sale_limit_by_time, slt.sale_access_by_time FROM ware_additions a LEFT JOIN ware w ON a.ware_id = w.ware_id LEFT JOIN ware_barcode b ON b.addition_code = a.addition_code LEFT JOIN sale_limit_by_time_view slt ON slt.id = w.sale_limit_by_time WHERE b.barcode = '" 
//-------------------------------
int Registration::Query_Addition_by_Bar_Code(const char * code) {
    int state;
    char *str;
    
    str = (char *) calloc(strlen(QUERY_BAR_ADDITION) + strlen(code) + 2, sizeof(char));
    strcpy(str, QUERY_BAR_ADDITION);
    
    strcat(str, code);
    strcat(str, "'");
    //printf("str = %s \n", str);
    state = doSQL(myconnection, str);
    free(str);  
    
    return state;
}
//==================================================================================//	
#define QUERY_PATTERN "SELECT pattern, multiplier FROM Weit_Pattern WHERE prefix = '"
int Registration::Query_Pattern(const char *prefix)
{
//puts("Registration::Query_Pattern(const char* prefix)");
   int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_PATTERN) + strlen(prefix) + 2, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_PATTERN);
  strcat(str, prefix);
  strcat(str, "'");
  state = doSQL(myconnection, str);
  free(str);  
  return state;
}

//==================================================================================//
#define QUERY_TAXES "SELECT trg.tax_rate_id, trg.base_flag, tr.longtitle, tr.shortcut, tr.value, tg.shortcut FROM Tax_Rate_Group trg, Tax_Rate tr, Tax_Group tg WHERE (trg.tax_rate_id = tr.tax_rate_id)  AND (trg.tax_group_id = tg.tax_group_id) AND (trg.tax_group_id = "
#define QUERY_TAXES_ORDER " ORDER BY trg.order_id"
int Registration::Query_Taxes(const char *taxgroup)
{
//puts("Registration::Query_Taxes(const char *taxgroup)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_TAXES) + strlen(QUERY_TAXES_ORDER) + strlen(taxgroup) + 2, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_TAXES);
  strcat(str, taxgroup);
  strcat(str, ")");
  strcat(str, QUERY_TAXES_ORDER);

  state = doSQL(myconnection, str);
//puts("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
  free(str);  
  return state;
}

////////////////////////////////////////////////////////////////////////VISUAL SEARCH////////////////////////////////////////////////////////////////////////////
//==================================================================================//
int Registration::Get_Num_Info(const char *group_id, bool fl)
//fl: true - group, false - Registration 
//return :
//0 - нет записей в этом каталоге
//-1 - нет связи с БД
{
//puts("Registration::Get_Num_Info(const char *group_id, bool fl)");
/*  if(fl)
  {
    if(Query_Group(group_id) != 2)
      return -1;
  }
  else if(Query_Goods(group_id) != 2) return -1;
 
  int n = PQntuples(result);
printf("ntuples - %d\n", n);
  if(n == 0) PQclear(result);
  return n;*/
  if (fl) return Query_Group_Cnt(group_id);
  return Query_Goods_Cnt(group_id);
}
//==================================================================================//
int Registration::Get_List_By_Parent(const char *group_id, bool fl)
//fl: true - group, false - Registration 
//return :
//0 - нет записей в этом каталоге
//-1 - нет связи с БД
{
//puts("Registration::Get_List_By_Parent(const char *group_id, bool fl)");
  if(fl)
  {
    if(Query_Group(group_id) != 2)
      return -1;
  }
  else if(Query_Goods(group_id) != 2) return -1;
 
  int n = PQntuples(result);
  if(n == 0) PQclear(result);
  return n;
}

//==================================================================================//
// group_id - идентификатор теаущей группы
// name - имя родительской группы, заполняется внутри функции
// pgroup - идентификатор родительской группы, заполняется внутри функции
int Registration::Get_Parent_Info(const char *group_id, char *name, char *pgroup)
{
//puts("Registration::Get_Parent_Info(const char *group_id, char *name, char *pgroup)");

  if(Query_Parent_Group(group_id) == 2)
  {
    if(PQntuples(result) == 0)
    {
      PQclear(result);
      return 1;
    }
    strcpy(name, PQgetvalue(result, 0, 0));
    strcpy(pgroup, PQgetvalue(result, 0, 1));
    PQclear(result);
    return 0;
  }
  else return -1;
}

//==================================================================================//
void Registration::Get_Group_Info(char *name, char *group_id, int i, int n)
{
//puts("Registration::Get_Group_Info(char *name, char *group_id, int i, int n)");
  strcpy(group_id, PQgetvalue(result, i, 0));
  strcpy(name, PQgetvalue(result, i, 1));
  if(n == i + 1) PQclear(result); 
}

//==================================================================================//
void Registration::Get_Prod_Info(char *longtitle, char *shortcut, char *price, char *quantity, char *bar_code, char *internal_code, int i, int n)
{
  strcpy(longtitle, PQgetvalue(result, i, 0));
  strcpy(shortcut, PQgetvalue(result, i, 1));
  strcpy(price, PQgetvalue(result, i, 2));
  strcpy(quantity, PQgetvalue(result, i, 3));
  strcpy(bar_code, PQgetvalue(result, i, 4));
  strcpy(internal_code, PQgetvalue(result, i, 5));
  if (n == i + 1) PQclear(result); 
//printf("end\n");
}

//==================================================================================//
VisualSearchElement Registration::GetProductInfo(int i, int n) {
    
    //printf("GetProductInfo(int i = %d, int n = %d ) \n", i, n);
    //printf("PQntuples = %d \n", PQntuples(result));
    VisualSearchElement product;
    
    product.is_addition = false;
    strcpy(product.longtitle, PQgetvalue(result, i, 0));
    strcpy(product.shortcut, PQgetvalue(result, i, 1));
    strcpy(product.price, PQgetvalue(result, i, 2));
    strcpy(product.quantity, PQgetvalue(result, i, 3));
    strcpy(product.barcode, PQgetvalue(result, i, 4));
    strcpy(product.internal_code, PQgetvalue(result, i, 5));
    if ( PQgetlength(result, i, 6) > 0 ) {
	product.is_addition = true;
	strcpy(product.addition_id, PQgetvalue(result, i, 6));
    }
    if (n == i + 1) PQclear(result);    
    return product;
}
//==================================================================================//
int Registration::Get_Num_Search_Info(const char *pattern, int pattern_type)
// return :
// 0 - нет записей соответствующих указанному шаблону
// -1 - нет связи с БД
// n - кол-во записей в запросе	
{
//puts("Registration::Get_Num_Search_Info(const char *group_id, int pattern_type)");
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
//==================================================================================//
int Registration::Get_List_By_Pattern(const char *pattern, int pattern_type)
//return :
// 0 - нет записей соответствующих указанному шаблону
// -1 - нет связи с БД
// n - кол-во записей в запросе
{
//puts("Registration::Get_List_By_Pattern(const char *group_id, int pattern_type)");
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
//==================================================================================//
#define QUERY_GROUP_CNT "SELECT count(group_id) FROM Ware_Group WHERE parent_group_id "
#define QUERY_GROUP_W_CNT "IS NULL"
int Registration::Query_Group_Cnt(const char *gr)
{
//puts("int Registration::Query_Group(const char *gr)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_GROUP_CNT) + strlen(QUERY_GROUP_W_CNT ) + strlen(gr) + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GROUP_CNT);
  if(!strcmp(gr, "0"))
    strcat(str, QUERY_GROUP_W_CNT);
  else
  {
    strcat(str, "= ");
    strcat(str, gr);
  }
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

//==================================================================================//
#define QUERY_GROUP "SELECT group_id, longtitle FROM Ware_Group WHERE parent_group_id "
#define QUERY_GROUP_ORDER " ORDER BY "
#define QUERY_GROUP_W "IS NULL"
int Registration::Query_Group(const char *gr)
{
//puts("int Registration::Query_Group(const char *gr)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_GROUP) + strlen(QUERY_GROUP_ORDER) + strlen(QUERY_GROUP_W ) + strlen(gr) + 30, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GROUP);
  if(!strcmp(gr, "0"))
    strcat(str, QUERY_GROUP_W);
  else
  {
    strcat(str, "= ");
    strcat(str, gr);
  }
  
  switch(sp->order_search_result)
  {
    case 0 :
      strcat(str, QUERY_GROUP_ORDER);
      strcat(str, "group_id");
      break;
    case 2 :
      strcat(str, QUERY_GROUP_ORDER);	
      strcat(str, "longtitle");
      break;
  }
  state = doSQL(myconnection, str);
  free(str);  
  return state;
}
//==================================================================================//
#define QUERY_GOODS_CNT "SELECT count(internal_id) FROM Ware WHERE group_id "
#define QUERY_GOODS_W_CNT "IS NULL"
int Registration::Query_Goods_Cnt(const char *gr)
{
//puts("int Registration::Query_Goods_Cnt(const char *gr)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_GOODS_CNT) + strlen(QUERY_GOODS_W_CNT ) + strlen(gr) + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GOODS_CNT);
  if(!strcmp(gr, "0"))
    strcat(str, QUERY_GOODS_W_CNT);
  else
  {
    strcat(str, "= ");
    strcat(str, gr);
  }
  state = doSQL(myconnection, str);
  //printf("state = <%d>\n",state);
  free(str);
  if (state !=2)
       return -1;     
  int n = PQntuples(result);
  if (n == 0) {PQclear(result); return 0;}
  n = atoi(PQgetvalue(result, 0, 0));    
  PQclear(result);
  return n;
}
//==================================================================================//
#define QUERY_GOODS "SELECT w.longtitle, w.shortcut, w.price, w.quantity, (select barcode from ware_barcode wb WHERE wb.internal_id = w.internal_id ORDER BY main_barcode DESC LIMIT 1)  as barcode, w.internal_id, null as addition_code FROM Ware w WHERE w.group_id "
/*#define QUERY_GOODS "SELECT longtitle, shortcut, price, quantity, barcode, internal_id FROM Ware WHERE group_id "*/
#define QUERY_GOODS_ORDER " ORDER BY  "
#define QUERY_GOODS_W "IS NULL"
int Registration::Query_Goods(const char *gr)
{
//puts("int Registration::Query_Goods(const char *gr)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_GOODS) + strlen(QUERY_GOODS_ORDER) + strlen(QUERY_GOODS_W) + strlen(gr) + strlen(ORDERBYCODE) + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GOODS);
  if(!strcmp(gr, "0"))
    strcat(str, QUERY_GOODS_W);
  else
  {
    strcat(str, "= ");
    strcat(str, gr);
  }
  strcat(str, QUERY_GOODS_ORDER);
  switch(sp->order_search_result)
  {
    case 0 :
      strcat(str, ORDERBYCODE);
      break;
    case 1 :
      strcat(str, ORDERBYBARCODE);
      break;
    case 2 :
      strcat(str, ORDERBYTITLE);
      break;
  }

  state = doSQL(myconnection, str); 
  free(str);  
  return state;
}

//==================================================================================//
#define QUERY_PARENT_GROUP "SELECT longtitle, parent_group_id FROM Ware_Group WHERE group_id = "
int Registration::Query_Parent_Group(const char *gr)
{
//puts("int Registration::Query_Parent_Group(const char *gr)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_PARENT_GROUP) + strlen(gr) + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_PARENT_GROUP);
  strcat(str, gr);
  state = doSQL(myconnection, str); 
  free(str);  
  return state;
}
//==================================================================================//
#define QUERY_GOODS_INT_CNT "SELECT COUNT(*) FROM visual_search_view WHERE internal_id ~~* '"

int Registration::Query_Cnt_by_IntCode(const char *pattern)
{
//puts("int Registration::Query_Cnt_by_IntCode(const char *pattern)");
  int state;
  char *str;
  
  str = (char *) calloc(strlen(QUERY_GOODS_INT_CNT) + strlen(pattern) + 3 + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GOODS_INT_CNT);
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

//==================================================================================//
#define QUERY_GOODS_INT "SELECT longtitle, shortcut, price, quantity, barcode, internal_id, addition_code FROM visual_search_view WHERE internal_id ~~* '"
int Registration::Query_by_Internal_Code_Pattern(const char *pattern)
{
//puts("int Registration::Query_by_Internal_Code_Pattern(const char *pattern)");
  int state;
  char *str;
  
  str = (char *) calloc(strlen(QUERY_GOODS_INT) + strlen(QUERY_GOODS_ORDER) + strlen(pattern) + strlen(ORDERBYCODE) + 3 + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GOODS_INT);
  strcat(str, pattern);
  strcat(str, "%' ");
  strcat(str, QUERY_GOODS_ORDER);
  switch(sp->order_search_result)
  {
    case 0 :
      strcat(str, ORDERBYCODE);
      break;
    case 1 :
      strcat(str, ORDERBYBARCODE);
      break;
    case 2 :
      strcat(str, ORDERBYTITLE);
      break;
  }
  //printf("vs by code str = %s \n", str);
  state = doSQL(myconnection, str); 
  free(str);  
  return state;
}
//==================================================================================//
#define QUERY_GOODS_BAR_CNT "SELECT COUNT(*) FROM visual_search_view WHERE barcode ~~* '%"
int Registration::Query_Cnt_by_BarCode(const char *pattern)
{
//puts("int Registration::Query_Cnt_by_BarCode(const char *pattern)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_GOODS_BAR_CNT) + strlen(pattern) + 3 + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
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
//==================================================================================//
#define QUERY_GOODS_BAR "SELECT longtitle, shortcut, price, quantity, barcode, internal_id, addition_code FROM visual_search_view WHERE barcode ~~* '%"
int Registration::Query_by_Bar_Code_Pattern(const char *pattern)
{
//puts("int Registration::Query_by_Bar_Code_Pattern(const char *pattern)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_GOODS_BAR) + strlen(QUERY_GOODS_ORDER) + strlen(pattern) + strlen(ORDERBYCODE) + 3 + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GOODS_BAR);
  strcat(str, pattern);
  strcat(str, "%' ");
  strcat(str, QUERY_GOODS_ORDER);
  switch(sp->order_search_result)
  {
    case 0 :
      strcat(str, ORDERBYCODE);
      break;
    case 1 :
      strcat(str, ORDERBYBARCODE);
      break;
    case 2 :
      strcat(str, ORDERBYTITLE);
      break;
  }
  //printf("vs by barcode = %s \n ", str);
  state = doSQL(myconnection, str); 
  free(str);  
  return state;
}
//==================================================================================//
#define QUERY_GOODS_SHCUT_CNT "SELECT COUNT(*) FROM visual_search_view WHERE upper(longtitle) ~~*  upper('%"
int Registration::Query_Cnt_by_ShCut(const char *pattern)
{
//puts("int Registration::Query_Cnt_by_ShCut(const char *pattern)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_GOODS_SHCUT_CNT) + strlen(pattern) + 3 + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GOODS_SHCUT_CNT);
  strcat(str, pattern);
  strcat(str, "%')");
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
//==================================================================================//
#define QUERY_GOODS_SHORTCUT "SELECT longtitle, shortcut, price, quantity, barcode, internal_id, addition_code FROM visual_search_view WHERE upper(longtitle) ~~*  upper('%"
int Registration::Query_by_Shortcut_Pattern(const char *pattern)
{
//puts("int Registration::Query_by_Shortcut_Pattern(const char *pattern)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_GOODS_SHORTCUT) + strlen(QUERY_GOODS_ORDER) + strlen(pattern) + strlen(ORDERBYCODE) + 3 + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GOODS_SHORTCUT);
  strcat(str, pattern);
  strcat(str, "%')");
  strcat(str, QUERY_GOODS_ORDER);
  switch(sp->order_search_result)
  {
    case 0 :
      strcat(str, ORDERBYCODE);
      break;
    case 1 :
      strcat(str, ORDERBYBARCODE);
      break;
    case 2 :
      strcat(str, ORDERBYTITLE);
      break;
  }
  //printf("vs by longtittle str = %s \n", str);
  state = doSQL(myconnection, str); 
  free(str);  
  return state;
}


//==================================================================================//
int Registration::Get_List(const char *area)
{
//puts("Registration::Get_List(const char *area)");
  if(Query_List_by_Area(area) != 2) return -1;
  
  int n = PQntuples(result);
  if(n == 0) PQclear(result);  
  return n;
}

//==================================================================================//
void Registration::Get_List_Position(char *&code, char *&value, char *&name, char *&shortcut, int *type, char * & incode, char* & cardcode, int i, int n)
{
//puts("Registration::Get_List_Position(char *&code = %s, char *&value = %s, char *&name = %s, char *&shortcut = %s, int *type = %d, int i = %d, int n = d)", code, value, name, shortcut, type, i, n);

  if(i < n)  {
    code = (char *) calloc(PQgetlength(result, i , 0) + 1, sizeof(char));
    value = (char *) calloc(PQgetlength(result, i , 1) + 2, sizeof(char));
    name = (char *) calloc(PQgetlength(result, i , 3) + 1, sizeof(char));
    shortcut = (char *) calloc(PQgetlength(result, i , 4) + 1, sizeof(char));
    incode = (char* ) calloc(PQgetlength(result, i , 5) + 1, sizeof(char));
    cardcode = (char* ) calloc(PQgetlength(result, i , 6) + 1, sizeof(char));
    strcpy(code, PQgetvalue(result, i, 0));
    strcpy(value, PQgetvalue(result, i, 1));
    strcpy(name, PQgetvalue(result, i, 3));
    strcpy(shortcut, PQgetvalue(result, i, 4));
    strcpy(incode, PQgetvalue(result, i, 5));
    strcpy(cardcode, PQgetvalue(result, i, 6));
    *type = atoi(PQgetvalue(result, i, 2));
  }
  if(n == i + 1) PQclear(result);
}

//-----(*)-----Zuskin-----15/07/2011-----
//#define QUERY_LIST_AREA "SELECT external_id, value, type, longtitle, shortcut, external_id, cardcode FROM Fix_Reduction, activescheme WHERE area = '"
//#define QUERY_LIST_AREA_ORDER "' AND scheme_id = activescheme_id AND show = 't'  ORDER BY external_id"
#define QUERY_LIST_AREA "SELECT external_id, value, type, longtitle, shortcut, external_id, cardcode FROM Fix_Reduction WHERE area = '"
#define QUERY_LIST_AREA_ORDER "' AND show = 't'  ORDER BY external_id"
//---------------------------------------
//==================================================================================//
int Registration::Query_List_by_Area(const char *area)
{
//puts("Registration::Query_List_by_Area(const char *area)");
  char * str;
  int state;   

  str = (char *) calloc(strlen(QUERY_LIST_AREA) + strlen(QUERY_LIST_AREA_ORDER) + strlen(area) + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_LIST_AREA);
  strcat(str, area);
  strcat(str, QUERY_LIST_AREA_ORDER);

  state = doSQL(myconnection, str);
 
  free(str);
  return state; 
}

//////////////////////////////////////////////////////////////////////////////UPDATE/////////////////////////////////////////////////////////////////////////////////

//==================================================================================//
// 1 - списание остатков отключено в настройках 
// 2 - списание остатков запрещено в карточке товара
// 
int Registration::Update_Goods_Quantity(Receipt *rec)
{

    char *q_str = NULL;
    for(int i = 0; i < rec->record_num; i ++)  {
	if ( rec->GetPosElemRef(i)->enable_flag ) {	    
	    //printf("prodano v kol-ve - %f \n", rec->GetPosElemRef(i)->quantity);
	    q_str = (char*) calloc(strlen(ftoa(rec->GetPosElemRef(i)->quantity, 3)) + 1, sizeof(char));
	    strcpy(q_str,ftoa(rec->GetPosElemRef(i)->quantity, 3));
	    //printf("prodano v stroke - q_str = %s\n", q_str);
	    
	    char* ssdfsdrfg = (char*) calloc(strlen(q_str) + 8, sizeof(char));
	    if ( rec->GetCheckType() == CHECK_RETURN ) strcat(ssdfsdrfg, "-");
	    strcat(ssdfsdrfg, q_str);
	    free(q_str);
	    if ( atoi(rec->GetPosElemRef(i)->GetAdditionId()) > 0 ) 
		Update_Addition_Quantity(ssdfsdrfg, rec->GetPosElemRef(i)->GetAdditionId());
	    Update_Quantity(ssdfsdrfg, rec->GetPosElemRef(i)->GetCode());    
	    free(ssdfsdrfg);
	}
    }	
}


//==================================================================================//
#define UPDATE_ADDITION_QUANTITY "UPDATE ware_additions SET addition_quantity = addition_quantity - "
#define UPDATE_ADDITION_QUANTITY_WHERE " WHERE addition_code = '"
int Registration::Update_Addition_Quantity(const char* quantity, const char* addition_id)
{
    //printf("Update_Addition_Quantity(const char* quantity = %s, const char* addition_id = %s) \n", quantity, addition_id);
    char str[strlen(UPDATE_ADDITION_QUANTITY) + strlen(UPDATE_ADDITION_QUANTITY_WHERE) + strlen(quantity) + strlen(addition_id) + 1];
    strcpy(str, UPDATE_ADDITION_QUANTITY);
    strcat(str, quantity);
    strcat(str, UPDATE_ADDITION_QUANTITY_WHERE);
    strcat(str, addition_id);
    strcat(str, "'");
    
    //printf("str - %s \n", str);
    return doSQL(myconnection, str);
}


#define SELECT_FLAGS "SELECT flags FROM ware WHERE internal_id = '"
//==================================================================================//
int Registration::GetFlags(const char* ware_code, int &flags )
{
  char * str;
  int state;   

  str = (char *) calloc(strlen(SELECT_FLAGS) + strlen(ware_code) + 2, sizeof(char));
  if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, SELECT_FLAGS);
  strcat(str, ware_code);
  strcat(str, "'");
  state = doSQL(myconnection, str);
  
  free(str);
  
  if (state == 2) flags = atoi(PQgetvalue(result, 0, 0));
  PQclear(result);
  
  return state; 
}


#define UPDATE_QUANTITY "UPDATE Ware SET quantity = quantity - "
#define UPDATE_QUANTITY_WHERE " WHERE internal_id = '"
//==================================================================================//
int Registration::Update_Quantity(const char *quant, const char *code)
{
    //puts("Registration::Update_Quantity(const char *quant, const char *code)");
    
    //puts("Registration::Update_Goods_Quantity(Receipt *rec)");
    
    if ( sp->control_remains == NO )  return 1;	
    
    if ( sp->control_remains == WARECARD ) {
	int flags = -1;
	int state = GetFlags(code, flags);	
	//printf("flags = %d \n", flags);
	if ( flags == -1 ) return state;
	if ( (int) (flags & WRITEOFFREMAINS) == 0 ) return 2;
    }
    
    
    char * str;
    int state;   

    str = (char *) calloc(strlen(UPDATE_QUANTITY) + strlen(UPDATE_QUANTITY_WHERE) + strlen(quant) + strlen(code) + 3, sizeof(char));
    if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
    strcpy(str, UPDATE_QUANTITY);
    strcat(str, quant);
    strcat(str, UPDATE_QUANTITY_WHERE);
    strcat(str, code);
    strcat(str, "'");
    state = doSQL(myconnection, str);
    
    free(str);
    return state; 
}
//==================================================================================//
// 1 - discount = 0
// -1 - request error
#define GET_DISCOUNT_BARCODE_INFO "SELECT external_id, longtitle, shortcut, value, type, area, cardcode FROM fix_reduction WHERE barcode = '"
int Registration::GetDiscountBarcodeInfo(const char* barcode, char*& code, char*& name, char*& shortcut, double &value, int &type, bool &area, char*& card_code)
{
  
  if ( strlen(barcode) == 0 ) return 1;
  char * str;
  int state;   

  str = (char *) calloc(strlen(GET_DISCOUNT_BARCODE_INFO) + strlen(barcode) + 4, sizeof(char));
  if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, GET_DISCOUNT_BARCODE_INFO);
  strcat(str, barcode);
  strcat(str, "'");

  state = doSQL(myconnection, str);
  
  if ( state != 2 ) return -1;
  int n = PQntuples(result);
  if ( n != 1 ) {	
	PQclear(result);
	return 1;
  }

//  printf("n = %d \n", n);

  code = (char*) calloc(PQgetlength(result, 0, 0) + 2, sizeof(char));
  strcpy(code, PQgetvalue(result, 0, 0)); 


  name = (char*) calloc(PQgetlength(result, 0, 1) + 2, sizeof(char));
  //printf("PQgetvalue(result, 0, 1) = %s \n", PQgetvalue(result, 0, 1));
  strcpy(name, PQgetvalue(result, 0, 1)); 
  //printf("-----------name = %s ----------- \n", name);

  shortcut = (char*) calloc(PQgetlength(result, 0, 2) + 2, sizeof(char));
  //printf("PQgetlength(result, 0, 2) = %s \n", PQgetvalue(result, 0, 2));
  strcpy(shortcut, PQgetvalue(result, 0, 2)); 
  //printf("----------- shortcut = %s ----------- \n", shortcut);
  
  value = atoi(PQgetvalue(result, 0, 3));
  type = atoi(PQgetvalue(result, 0, 4));
  area = ( strcmp(PQgetvalue(result, 0, 5), "t") == 0 );
  
  card_code = (char*) calloc(PQgetlength(result, 0, 6) + 2, sizeof(char));
  //printf("PQgetvalue(result, 0, 6) = %s \n", PQgetvalue(result, 0, 6));
  strcpy(card_code, PQgetvalue(result, 0, 6)); 
  //printf("----------- card_code = %s ----------- \n", card_code);

  PQclear(result);
  
  free(str);
  return 0; 
	
}
//==================================================================================//
#define GET_DISCOUNT_CARDCODE_INFO "SELECT external_id, longtitle, shortcut, value, type, area, barcode FROM fix_reduction WHERE cardcode = '"
int Registration::GetDiscountCardcodeInfo(const char* card_code, char*& code, char*& name, char*& shortcut, double &value, int &type, bool &area, char*& barcode)
{
  
  if ( strlen(card_code) == 0 ) return 1;
  char * str;
  int state;   

  str = (char *) calloc(strlen(GET_DISCOUNT_CARDCODE_INFO) + strlen(card_code) + 4, sizeof(char));
  if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, GET_DISCOUNT_CARDCODE_INFO);
  strcat(str, card_code);
  strcat(str, "'");

  state = doSQL(myconnection, str);
  free(str);

  if ( state != 2 ) return -1;
  int n = PQntuples(result);
  if ( n != 1 ) {	
	PQclear(result);
	return 1;
  }

//  printf("n = %d \n", n);

  code = (char*) calloc(PQgetlength(result, 0, 0) + 2, sizeof(char));
  strcpy(code, PQgetvalue(result, 0, 0)); 

  name = (char*) calloc(PQgetlength(result, 0, 1) + 2, sizeof(char));
  //printf("PQgetvalue(result, 0, 1) = %s \n", PQgetvalue(result, 0, 1));
  strcpy(name, PQgetvalue(result, 0, 1)); 
  //printf("-----------name = %s ----------- \n", name);

  shortcut = (char*) calloc(PQgetlength(result, 0, 2) + 2, sizeof(char));
  //printf("PQgetlength(result, 0, 2) = %s \n", PQgetvalue(result, 0, 2));
  strcpy(shortcut, PQgetvalue(result, 0, 2)); 
  //printf("----------- shortcut = %s ----------- \n", shortcut);
  
  value = atoi(PQgetvalue(result, 0, 3));
  type = atoi(PQgetvalue(result, 0, 4));
  area = ( strcmp(PQgetvalue(result, 0, 5), "t") == 0 );
  
  barcode = (char*) calloc(PQgetlength(result, 0, 6) + 2, sizeof(char));
  //printf("PQgetvalue(result, 0, 6) = %s \n", PQgetvalue(result, 0, 6));
  strcpy(barcode, PQgetvalue(result, 0, 6)); 
  //printf("----------- card_code = %s ----------- \n", card_code);

  PQclear(result);
  
  
  return 0; 	
}

#define GET_QUANTEDIT_INFO "SELECT flags FROM ware WHERE internal_id = '"
#define GET_QUANTITY_INFO "SELECT quantity FROM ware WHERE internal_id = '"
//==================================================================================//
//return 0 - можно продавать;
// 1 - нельзя продавать;
// -1 - error;
int Registration::GetQuantityEditInfo(const char* ware_code, double q)
{
    int state = 0;
    
    char* str = (char*) calloc( strlen(GET_QUANTEDIT_INFO) + strlen(ware_code) + 2, sizeof(char) );
    
    strcat(str, GET_QUANTEDIT_INFO);
    strcat(str, ware_code);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    free(str);
    
    if (state != 2) return -1;
    if ( PQntuples(result) != 1 ) {
	PQclear(result);
	return -1;    
    }
    
    int flags = atoi(PQgetvalue(result, 0, 0));
    
    PQclear(result);
    
    bool remains_control = false;
    // списание остатков
    if ( sp->control_remains == YES ) remains_control = true;    
    if ( (sp->control_remains == WARECARD) && ((int) (flags & WRITEOFFREMAINS) != 0) ) remains_control =  true; 
    
    bool control_negative_remains = true;
    // отрицательные остатки
    if ( sp->control_negative_remains == NO ) control_negative_remains = false;
    if ( (sp->control_negative_remains == WARECARD) && ( (int) (flags & ALLOWNEGATIVEQUANTTITY) != 0) ) control_negative_remains = false;
        
    
    state = 0;
    
    str = (char*) calloc( strlen(GET_QUANTITY_INFO) + strlen(ware_code) + 2, sizeof(char) );
    
    strcat(str, GET_QUANTITY_INFO);
    strcat(str, ware_code);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    free(str);
    
    if (state != 2) return -1;
    if ( PQntuples(result) != 1 )    {	
	PQclear(result);
	return -1;    
    }
    
    double quantity = atof(PQgetvalue(result, 0, 0));
    
    PQclear(result);
    
    //printf("quantity = %f \n", quantity);
    //printf("q = %f \n", q);
    
    if ( (remains_control) && (!control_negative_remains) && (quantity - q < 0)  ) return 1;
    
    return 0;
    
}

#define GET_QUNTEDITFLAGS "SELECT flags FROM ware WHERE internal_id = '"
//==================================================================================//
int Registration::GetQuantityEditFlag(const char* ware_code)
{
    int state = 0;
    
    char* str = (char*) calloc( strlen(GET_QUNTEDITFLAGS) + strlen(ware_code) + 2, sizeof(char) );
    
    strcat(str, GET_QUNTEDITFLAGS);
    strcat(str, ware_code);
    strcat(str, "'");
    
    state = doSQL(myconnection, str);
    free(str);
    
    if (state != 2) return -1;
    if ( PQntuples(result) != 1 ) {
	PQclear(result);
	return -1;    
    }
    
    int flags = atoi(PQgetvalue(result, 0, 0));
    
    PQclear(result);
    
    if ( (int) (flags & QUANTITYEDIT) == 0 ) return 1;
    return 0;
    
}
//==================================================================================//

#define GET_QUNTEDITFLAGS_BY_CODE "SELECT flags FROM ware WHERE internal_id = '"
#define GET_QUNTEDITFLAGS_BY_BARCODE "SELECT flags FROM ware WHERE internal_id IN (SELECT internal_id FROM ware_barcode WHERE barcode = '"
//==================================================================================//
int Registration::GetQuantityEditFlag(const char* ware_code, bool type_code)
{
    int state = 0;
    
    char* str;
    
    if ( type_code ) {
	// barcode
	str = (char*) calloc( strlen(GET_QUNTEDITFLAGS_BY_BARCODE) + strlen(ware_code) + 3, sizeof(char) );
	strcat(str, GET_QUNTEDITFLAGS_BY_BARCODE);
	strcat(str, ware_code);
	strcat(str, "')");	
    } else {
	// code
	str = (char*) calloc( strlen(GET_QUNTEDITFLAGS_BY_CODE) + strlen(ware_code) + 2, sizeof(char) );
	strcat(str, GET_QUNTEDITFLAGS_BY_CODE);
	strcat(str, ware_code);
	strcat(str, "'");
    }
    
    state = doSQL(myconnection, str);
    free(str);

    if (state != 2) return -1;
    
    if ( PQntuples(result) != 1 ) {
	PQclear(result);
	return -1;    
    }
    
    int flags = atoi(PQgetvalue(result, 0, 0));
    
    PQclear(result);
    
    if ( (int) (flags & QUANTITYEDIT) == 0 ) return 1;	// запрещен
    return 0;
    
}

//==================================================================================//
int Registration::Get_Num_Info_Available(const char *group_id, bool fl)
{
    if (fl) return Query_Group_Cnt(group_id);
    return Query_Goods_Cnt_Available(group_id);
}
//==================================================================================//
#define QUERY_GOODS_CNT_A "SELECT count(internal_id) FROM Ware WHERE quantity > 0 AND group_id "
#define QUERY_GOODS_W_CNT_A "IS NULL"
int Registration::Query_Goods_Cnt_Available(const char *gr)
{
//puts("int Registration::Query_Goods_Cnt(const char *gr)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_GOODS_CNT_A) + strlen(QUERY_GOODS_W_CNT_A ) + strlen(gr) + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GOODS_CNT_A);
  if(!strcmp(gr, "0"))
    strcat(str, QUERY_GOODS_W_CNT_A);
  else
  {
    strcat(str, "= ");
    strcat(str, gr);
  }
  state = doSQL(myconnection, str);
  //printf("state = <%d>\n",state);
  free(str);
  if (state !=2)
       return -1;     
  int n = PQntuples(result);
  if (n == 0) {PQclear(result); return 0;}
  n = atoi(PQgetvalue(result, 0, 0));    
  PQclear(result);
  return n;

}
//==================================================================================//
int Registration::Get_Num_Search_Info_Available(const char *pattern, int pattern_type)
{
    if ( pattern_type == 1 )  {
	return Query_Cnt_by_IntCode_Available(pattern);
    }
    if (pattern_type == 2)  {
	return Query_Cnt_by_BarCode_Available(pattern);
    }
    if (pattern_type == 3)  {
	return Query_Cnt_by_ShCut_Available(pattern);
    }
    return 0;
}
//==================================================================================//
#define QUERY_GOODS_INT_CNT_A "SELECT COUNT(*) FROM visual_search_view WHERE quantity > 0 AND internal_id ~~* '"
int Registration::Query_Cnt_by_IntCode_Available(const char *pattern)
{
    //puts("int Registration::Query_Cnt_by_IntCode(const char *pattern)");
  int state;
  char *str;
  
  str = (char *) calloc(strlen(QUERY_GOODS_INT_CNT_A) + strlen(pattern) + 3 + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GOODS_INT_CNT_A);
  strcat(str, pattern);
  strcat(str, "%' ");
/*  strcat(str, QUERY_GOODS_ORDER);
  switch(sp->order_search_result)
  {
    case 0 :
      strcat(str, ORDERBYCODE);
      break;
    case 1 :
      strcat(str, ORDERBYBARCODE);
      break;
    case 2 :
      strcat(str, ORDERBYTITLE);
      break;
  }*/
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
//==================================================================================//
#define QUERY_GOODS_BAR_CNT_A "SELECT COUNT(*) FROM visual_search_view WHERE quantity > 0 AND barcode ~~* '%"
/*#define QUERY_GOODS_BAR "SELECT longtitle, shortcut, price, quantity, barcode, internal_id FROM Ware WHERE barcode ~~* '"*/
int Registration::Query_Cnt_by_BarCode_Available(const char *pattern)
{
    //puts("int Registration::Query_Cnt_by_BarCode(const char *pattern)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_GOODS_BAR_CNT_A) + strlen(pattern) + 3 + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GOODS_BAR_CNT_A);
  strcat(str, pattern);
  strcat(str, "%' ");
/*  strcat(str, QUERY_GOODS_ORDER);
  switch(sp->order_search_result)
  {
    case 0 :
      strcat(str, ORDERBYCODE);
      break;
    case 1 :
      strcat(str, ORDERBYBARCODE);
      break;
    case 2 :
      strcat(str, ORDERBYTITLE);
      break;
  }*/
  
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
//==================================================================================//
#define QUERY_GOODS_SHCUT_CNT_A "SELECT COUNT(*) FROM visual_search_view WHERE quantity > 0 AND upper(longtitle) ~~*  upper('%"
int Registration::Query_Cnt_by_ShCut_Available(const char *pattern)
{
    //puts("int Registration::Query_Cnt_by_ShCut(const char *pattern)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_GOODS_SHCUT_CNT_A) + strlen(pattern) + 3 + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GOODS_SHCUT_CNT_A);
  strcat(str, pattern);
  strcat(str, "%')");
/*  strcat(str, QUERY_GOODS_ORDER);
  switch(sp->order_search_result)
  {
    case 0 :
      strcat(str, ORDERBYCODE);
      break;
    case 1 :
      strcat(str, ORDERBYBARCODE);
      break;
    case 2 :
      strcat(str, ORDERBYTITLE);
      break;
  }*/
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

//==================================================================================//
int Registration::Get_List_By_Pattern_Available(const char *pattern, int pattern_type)
//return :
// 0 - нет записей соответствующих указанному шаблону
// -1 - нет связи с БД
// n - кол-во записей в запросе
{
//puts("Registration::Get_List_By_Pattern(const char *group_id, int pattern_type)");
  if(pattern_type == 1)
  {
    if(Query_by_Internal_Code_Pattern_Available(pattern) != 2)
      return -1;
  }
  else if(pattern_type == 2)
  {
    if(Query_by_Bar_Code_Pattern_Available(pattern) != 2)
      return -1;
  }
  else if(pattern_type == 3)
  {
    if(Query_by_Shortcut_Pattern_Available(pattern) != 2)
      return -1;
  }
  int n = PQntuples(result);
  if(n == 0) PQclear(result);
  return n;
}
//==================================================================================//
#define QUERY_GOODS_INT_A "SELECT longtitle, shortcut, price, quantity, barcode, internal_id, addition_code FROM visual_search_view WHERE quantity > 0 AND internal_id ~~* '"
/*#define QUERY_GOODS_INT "SELECT longtitle, shortcut, price, quantity, barcode, internal_id FROM Ware WHERE CAST(internal_id AS VARCHAR(20)) ~~* '" */
int Registration::Query_by_Internal_Code_Pattern_Available(const char *pattern)
{
//puts("int Registration::Query_by_Internal_Code_Pattern(const char *pattern)");
  int state;
  char *str;
  
  str = (char *) calloc(strlen(QUERY_GOODS_INT_A) + strlen(QUERY_GOODS_ORDER) + strlen(pattern) + strlen(ORDERBYCODE) + 3 + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GOODS_INT_A);
  strcat(str, pattern);
  strcat(str, "%' ");
  strcat(str, QUERY_GOODS_ORDER);
  switch(sp->order_search_result)
  {
    case 0 :
      strcat(str, ORDERBYCODE);
      break;
    case 1 :
      strcat(str, ORDERBYBARCODE);
      break;
    case 2 :
      strcat(str, ORDERBYTITLE);
      break;
  }
  puts(str);
  state = doSQL(myconnection, str); 
  free(str);  
  return state;
}

//==================================================================================//

#define QUERY_GOODS_BAR_A "SELECT longtitle, shortcut, price, quantity, barcode, internal_id, addition_code FROM visual_search_view WHERE quantity > 0 AND barcode ~~* '%"
int Registration::Query_by_Bar_Code_Pattern_Available(const char *pattern)
{
//puts("int Registration::Query_by_Bar_Code_Pattern(const char *pattern)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_GOODS_BAR_A) + strlen(QUERY_GOODS_ORDER) + strlen(pattern) + strlen(ORDERBYCODE) + 3 + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GOODS_BAR_A);
  strcat(str, pattern);
  strcat(str, "%' ");
  strcat(str, QUERY_GOODS_ORDER);
  switch(sp->order_search_result)
  {
    case 0 :
      strcat(str, ORDERBYCODE);
      break;
    case 1 :
      strcat(str, ORDERBYBARCODE);
      break;
    case 2 :
      strcat(str, ORDERBYTITLE);
      break;
  }
  //printf("Query_by_Bar_Code_Pattern_Available() str - %s \n", str);
  state = doSQL(myconnection, str); 
  free(str);  
  return state;
}
//==================================================================================//

#define QUERY_GOODS_SHORTCUT_A "SELECT longtitle, shortcut, price, quantity, barcode, internal_id, addition_code FROM visual_search_view WHERE quantity > 0 AND upper(longtitle) ~~*  upper('%"
int Registration::Query_by_Shortcut_Pattern_Available(const char *pattern)
{
//puts("int Registration::Query_by_Shortcut_Pattern(const char *pattern)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_GOODS_SHORTCUT_A) + strlen(QUERY_GOODS_ORDER) + strlen(pattern) + strlen(ORDERBYCODE) + 3 + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GOODS_SHORTCUT_A);
  strcat(str, pattern);
  strcat(str, "%')");
  strcat(str, QUERY_GOODS_ORDER);
  switch(sp->order_search_result)
  {
    case 0 :
      strcat(str, ORDERBYCODE);
      break;
    case 1 :
      strcat(str, ORDERBYBARCODE);
      break;
    case 2 :
      strcat(str, ORDERBYTITLE);
      break;
  }
  state = doSQL(myconnection, str); 
  free(str);  
  return state;
}


//==================================================================================//

int Registration::Get_List_By_Parent_Available(const char *group_id, bool fl)
//fl: true - group, false - Registration 
//return :
//0 - нет записей в этом каталоге
//-1 - нет связи с БД
{
//puts("Registration::Get_List_By_Parent(const char *group_id, bool fl)");
  if(fl)
  {
    if(Query_Group(group_id) != 2)
      return -1;
  }
  else if(Query_Goods_Available(group_id) != 2) return -1;
 
  int n = PQntuples(result);
  if(n == 0) PQclear(result);
  return n;
}
//==================================================================================//
#define QUERY_GOODS_A "SELECT w.longtitle, w.shortcut, w.price, w.quantity,  (select barcode from ware_barcode wb WHERE wb.internal_id = w.internal_id ORDER BY main_barcode DESC LIMIT 1) as barcode, w.internal_id, null as addition_code FROM Ware w WHERE w.quantity > 0 AND w.group_id "
#define QUERY_GOODS_W_A "IS NULL"
int Registration::Query_Goods_Available(const char *gr)
{
//puts("int Registration::Query_Goods(const char *gr)");
  int state;
  char *str;

  str = (char *) calloc(strlen(QUERY_GOODS_A) + strlen(QUERY_GOODS_ORDER) + strlen(QUERY_GOODS_W_A) + strlen(gr) + strlen(ORDERBYCODE) + 1, sizeof(char));
 if(str == NULL) printf("NULL CALLOC!!! parameter str\n");
  strcpy(str, QUERY_GOODS_A);
  if(!strcmp(gr, "0"))
    strcat(str, QUERY_GOODS_W_A);
  else
  {
    strcat(str, "= ");
    strcat(str, gr);
  }
  strcat(str, QUERY_GOODS_ORDER);
  switch(sp->order_search_result)
  {
    case 0 :
      strcat(str, ORDERBYCODE);
      break;
    case 1 :
      strcat(str, ORDERBYBARCODE);
      break;
    case 2 :
      strcat(str, ORDERBYTITLE);
      break;
  }

  state = doSQL(myconnection, str); 
  free(str);  
  return state;
}

//==================================================================================//
#define GET_COEFFICIENT "SELECT coefficient FROM ware_barcode WHERE barcode = '"
#define GET_COEFFICIENT_AND_INTERNAL_CODE "' AND internal_id = '"
int Registration::GetBarcodeCoefficient(const char* barcode_str, const char* code_str, double & coeff)
{
    //printf("int Registration::GetBarcodeCoefficient(const char* code_str = %s, const char* barcode_str = %s, double & coeff) \n", code_str, barcode_str);
    
    coeff = 1;
    char * str = (char*) calloc(strlen(GET_COEFFICIENT) + strlen(barcode_str) + strlen(GET_COEFFICIENT_AND_INTERNAL_CODE) + strlen(code_str) + 3, sizeof(char));
    
    strcat(str, GET_COEFFICIENT);
    strcat(str, barcode_str);
    strcat(str, GET_COEFFICIENT_AND_INTERNAL_CODE);
    strcat(str, code_str);
    strcat(str, "'");
    
    
    int state = doSQL(myconnection, str);
    
    free(str);
    
    if ( state < 0 ) return -1;
    
    if ( PQntuples(result) != 1 ) return -1;
    
    coeff = atof(PQgetvalue(result, 0, 0));
    //printf("coeff = %d \n", coeff);
    
    return 0;
}
	
//==================================================================================//
#define GET_WARE_BY_FILTER "SELECT longtitle, shortcut, price, quantity, barcode, internal_id, addition_code FROM visual_search_view WHERE "
#define GET_WARE_CODE_FILTER " internal_id ~~* '%"
#define GET_WARE_BARCODE_FILTER " barcode ~~* '%"
#define GET_WARE_TITLE_FILTER " upper(longtitle) ~~*  upper('%"
#define GET_WARE_ABSENT_FILTER " quantity > 0"
#define GET_WARE_ORDER_BY_CODE " ORDER BY internal_id " 
#define GET_WARE_ORDER_BY_BARCODE " ORDER BY barcode " 
#define GET_WARE_ORDER_BY_TITLE " ORDER BY longtitle " 
//-----(+)-----Zuskin-----03/08/2011-----
#define GET_WARE_PRICE_FILTER " price = "
//---------------------------------------
//-----(*)-----Zuskin-----03/08/2011-----
//int Registration::VisualSearchByAll( const char* barcode, const char* code, const char* title, bool hide_absent_goods)
int Registration::VisualSearchByAll( const char* barcode, const char* code, const char* title, bool hide_absent_goods, double price)
//---------------------------------------
{
    /*printf("barcode = %s \n", barcode);
    printf("code = %s \n", code );
    printf("title = %s \n", title);
    
    printf("strlen(bar) = %d \n", strlen(barcode));
    printf("strlen(code) = %d \n", strlen(code));
    printf("strlen(tit) = %d \n", strlen(title));*/
    
    int count_exp = 0;
    int len = strlen(GET_WARE_BY_FILTER);
    if (strlen(barcode) > 0 ) { len += 2*strlen(barcode) + strlen(GET_WARE_BARCODE_FILTER) + strlen("%' ") + 1; count_exp++;  }
    if (strlen(code) > 0 )  {   len += 2*strlen(code) + strlen(GET_WARE_CODE_FILTER) + strlen("%' ") + 1;   count_exp++;  }

//-----(*)-----Zuskin-----24/03/2011-----
    //if (strlen(title) > 0 ) {   len += 2*strlen(title) + strlen(GET_WARE_TITLE_FILTER) + strlen("%') ") + 1;  count_exp++;   }
    int nw = str_count_words((char *) title);
    if (strlen(title) > 0 ) {   len += 2*strlen(title) + nw * strlen(GET_WARE_TITLE_FILTER) + nw * strlen("%') ") + (nw-1) * strlen("AND ") + 1;  count_exp++;   }
//--------------------------------------------------------------------
//-----(+)-----Zuskin-----03/08/2011-----
    if (price >= 0) {
        string pricestr = toString<double>(price);
	len+= 2*strlen(pricestr.c_str()) + strlen(GET_WARE_PRICE_FILTER) + 1;
	count_exp++;
    }
//---------------------------------------

    if ( hide_absent_goods )  {  len += strlen(GET_WARE_ABSENT_FILTER) + 1;   count_exp++;    }
    
    if (sp->order_search_result == 0 ) len += strlen(GET_WARE_ORDER_BY_CODE);
    if (sp->order_search_result == 1 ) len += strlen(GET_WARE_ORDER_BY_BARCODE);
    if (sp->order_search_result == 2 ) len += strlen(GET_WARE_ORDER_BY_TITLE);
    
    if ( count_exp > 1 )  len += (count_exp-1) * strlen(" AND ") ;
    //printf("len = %d \n", len);
	
    
    char* str = (char*) calloc(len +  5, sizeof(char));
    
    strcat(str, GET_WARE_BY_FILTER);
    if (strlen(barcode) > 0 )    { 	
	strcat(str, GET_WARE_BARCODE_FILTER);
	strcat(str, barcode);
	strcat(str, "%' ");
	count_exp--;
	if (count_exp > 0) strcat(str, " AND ");
    }
    
    if (strlen(code) > 0 ) {
	strcat(str, GET_WARE_CODE_FILTER);
	strcat(str, code);
	strcat(str, "%' ");
	count_exp--;
	if (count_exp > 0) strcat(str, " AND ");
    }
    
    if (strlen(title) > 0 ) {

//-----(*)-----Zuskin-----24/03/2011-----    
	//strcat(str, GET_WARE_TITLE_FILTER);
    	//strcat(str, wrd);
	//strcat(str, "%') ");
    	//count_exp--;
	//if (count_exp > 0) strcat(str, " AND ");
	for (int j = 0; j < nw; j++) { // по количеству слов в строке шаблона поиска по наименованию
	    char * wrd = str_get_word((char*) title, j+1);	    //получаем слово с номером j	
	    if (j == nw-1) { 		//последнее слово в строке
		strcat(str, GET_WARE_TITLE_FILTER);
    		strcat(str, wrd);
		strcat(str, "%') ");
    		count_exp--;
		if (count_exp > 0) strcat(str, " AND ");
	    }
	    else { 			//есть еще слова
		strcat(str, GET_WARE_TITLE_FILTER);
		strcat(str, wrd);
		strcat(str, "%') AND ");
	    }
	    free(wrd);    
	}
    }	
//----------------------------------------
//-----(+)-----Zuskin-----03/08/2011-----	
    if (price >= 0 ) {
        string pricestr = toString<double>(price);
	strcat(str, GET_WARE_PRICE_FILTER);
	strcat(str, pricestr.c_str());
	count_exp--;
	if (count_exp > 0) strcat(str, " AND ");
    }

//---------------------------------------
    if ( hide_absent_goods ) {
	strcat(str, GET_WARE_ABSENT_FILTER);
    }
    
    if (sp->order_search_result == 0 ) strcat(str, GET_WARE_ORDER_BY_CODE);
    if (sp->order_search_result == 1 ) strcat(str, GET_WARE_ORDER_BY_BARCODE);
    if (sp->order_search_result == 2 ) strcat(str, GET_WARE_ORDER_BY_TITLE);
    
    //printf("visual search str = %s \n", str);
    
    int state = doSQL(myconnection, str);
    
    free(str);
    
    if ( state < 0 ) return state;
    
    int n = PQntuples(result);
    
    return n;
    
}
//==================================================================================//
#define GET_COUNT_WARE_BY_BARCODE "SELECT internal_id FROM visual_search_view WHERE barcode = '" 
#define GET_COUNT_WARE_BY_CODE "SELECT internal_id FROM visual_search_view WHERE  internal_id ~~* '%" 
int Registration::GetCountWareByBarcode(const char *barcode, const char * code)
{	
    if ( (strlen(barcode) + strlen(code)) < 1) return 0;
    
    int state = -1;
    
    if (strlen(barcode) > 0 ) {
	char* str = (char *) calloc(strlen(GET_COUNT_WARE_BY_BARCODE) + strlen(barcode) + 2, sizeof(char));
	
	strcpy(str, GET_COUNT_WARE_BY_BARCODE);
	strcat(str, barcode);
	strcat(str, "'");
	state = doSQL(myconnection, str);
	free(str);
    }
    
    if ( strlen(code) > 0 ) {
	char* str = (char *) calloc(strlen(GET_COUNT_WARE_BY_CODE) + strlen(code) + 3, sizeof(char));
	
	strcpy(str, GET_COUNT_WARE_BY_CODE);
	strcat(str, code);
	strcat(str, "%'");
	//printf("str = %s \n", str);
	state = doSQL(myconnection, str);
	free(str);
    }
    
    if ( state < 0 ) return state;
    int n = PQntuples(result);
    PQclear(result);
    return n;
    
}
//==================================================================================//
#define GET_WARE_SHORTCUT "SELECT shortcut FROM ware WHERE internal_id = '"
int Registration::GetWareShortcut(const char* wcode, char* & w_shorcut) {
    
    char str[2000];
    
    //printf("code of ware = %s \n", wcode);
    strcpy(str, GET_WARE_SHORTCUT);
    strcat(str, wcode);
    strcat(str, "'");
    //printf("GetWareShortcut request = %s \n", str);
    
    int state = doSQL(myconnection, str);
    if ( state != 2 ) return -1;
    if ( PQntuples(result) != 1 ) {
	PQclear(result);
	return -1;
    }
    
    //printf("PQgetvalue(result, 0, 0) = %s \n", PQgetvalue(result, 0, 0));
    
    w_shorcut = (char*) calloc(PQgetlength(result, 0,0) + 1, sizeof(char*));
    strcpy(w_shorcut, PQgetvalue(result, 0, 0));
    
    PQclear(result);
    return 0;
}
//==================================================================================//

//-----(+)-----Zuskin-----15/07/2011------    
#define GET_SCHEME_ID "SELECT scheme_id FROM fix_reduction WHERE external_id = "
int Registration::GetSchemeIdByCode(const char* code) {
    
    string str = GET_SCHEME_ID;
    str += code;
    
    int state = doSQL(myconnection, str.c_str());
    if ( state != 2 ) return -1;
    if ( PQntuples(result) != 1 ) {
	PQclear(result);
	return -1;
    }
    
    int res = atoi(PQgetvalue(result, 0, 0));
    PQclear(result);
    return res;
}

#define GET_LIST_BY_SCHEME "SELECT value, type, longtitle, shortcut, begindate, enddate, begintime, endtime, weekdays, check_sum, accumulation FROM auto_reduction WHERE auto_scheme_id = "
#define ORDER_LIST_BY_SCHEME " ORDER BY auto_reduction_id"
int Registration::GetListBySchemeId(int schemeid)
{
  string str = GET_LIST_BY_SCHEME;
  str+= toString(schemeid);
  str+= ORDER_LIST_BY_SCHEME;

  state = doSQL(myconnection, str.c_str());
  if (state != 2) return -1;
  
  int n = PQntuples(result);
  if(n == 0) PQclear(result);  
  return n;
   
}

void Registration::GetRuleFromList(int i, string& value, string& type, string& longtitle, string& shortcut, string& begindate, string& enddate, string& begintime, string& endtime, string& weekdays, string& check_sum, string& acc_sum)
{
 int n = PQntuples(result);
 if (i < n) {
    value = PQgetvalue(result, i, 0);
    type = PQgetvalue(result, i, 1);
    longtitle = PQgetvalue(result, i, 2);
    shortcut = PQgetvalue(result, i, 3);
    begindate = PQgetvalue(result, i, 4);
    enddate = PQgetvalue(result, i, 5);
    begintime = PQgetvalue(result, i, 6);
    endtime = PQgetvalue(result, i, 7);
    weekdays = PQgetvalue(result, i, 8);
    check_sum = PQgetvalue(result, i, 9);
    acc_sum = PQgetvalue(result, i, 10);
 }
 if ( n == i + 1) PQclear(result);
}

void Registration::ClearRulesList(void) {
    PQclear(result);
}

//------------------------------------------

//-----(+)-----Zuskin-----22/07/2011------    
#define GET_CARD_SUMM "SELECT summ FROM fix_reduction WHERE external_id = "
double Registration::GetSummByCard(const char* code) {
    
    string str = GET_CARD_SUMM;
    str += code;
    
    int state = doSQL(myconnection, str.c_str());
    if ( state != 2 ) return -1;
    if ( PQntuples(result) != 1 ) {
	PQclear(result);
	return -1;
    }
    
    double res = atof(PQgetvalue(result, 0, 0));
    PQclear(result);
    return res;
}
//--------------------------------------------

//-----(+)-----Zuskin-----26/07/2011------    
#define GET_TYPE_CARD "SELECT type FROM fix_reduction WHERE external_id = "
int Registration::GetTypeByCard(const char* code) {
    
    string str = GET_TYPE_CARD;
    str += code;
    
//    puts(str.c_str());
    
    int state = doSQL(myconnection, str.c_str());
    if ( state != 2 ) return -1;
    if ( PQntuples(result) != 1 ) {
	PQclear(result);
	return -1;
    }
    
    int res = atoi(PQgetvalue(result, 0, 0));
    PQclear(result);
    return res;
}

#define UPDATE_BCARD "UPDATE fix_reduction SET summ = "
#define UPDATE_BCARD_WHERE " WHERE external_id = "
int Registration::UpdateBonusCard(const char* code, double bsumm) {
    
    string str = UPDATE_BCARD;
    str += toString<double>(bsumm);
    str += UPDATE_BCARD_WHERE;
    str += code;
    
    int state = doSQL(myconnection, str.c_str());
    return state;
}

#define GET_MAX_PERCENT "SELECT maxbonuspercent FROM fix_reduction WHERE external_id = "
double Registration::GetMaxPercentByCard(const char* code) {
    
    string str = GET_MAX_PERCENT;
    str += code;
    
    int state = doSQL(myconnection, str.c_str());
    if ( state != 2 ) return -1;
    if ( PQntuples(result) != 1 ) {
	PQclear(result);
	return -1;
    }
    
    double res = atof(PQgetvalue(result, 0, 0));
    PQclear(result);
    return res;
}

#define GET_CUR_PERCENT "SELECT value FROM fix_reduction WHERE external_id = "
double Registration::GetPercentByCard(const char* code) {
    
    string str = GET_CUR_PERCENT;
    str += code;
    
    int state = doSQL(myconnection, str.c_str());
    if ( state != 2 ) return -1;
    if ( PQntuples(result) != 1 ) {
	PQclear(result);
	return -1;
    }
    
    double res = atof(PQgetvalue(result, 0, 0));
    PQclear(result);
    return res;
}
//--------------------------------------------
//-----(+)-----Zuskin-----03/08/2011-----
#define QUERY_GOODS_PRICE_CNT "SELECT COUNT(*) FROM visual_search_view WHERE price = "
int Registration::Query_Cnt_By_Price(double price) {
    string str = QUERY_GOODS_PRICE_CNT;
    str+= toString<double>(price);
    int state = doSQL(myconnection, str.c_str());
    if (state !=2)
       return -1;     
    int n = PQntuples(result);
    if (n == 0) {PQclear(result);return 0;}
    n = atoi(PQgetvalue(result, 0, 0)); 
    PQclear(result);
    return n;
}
//---------------------------------------

//-----(+)-----Zuskin-----18/11/2011-----
#define QUERY_MANAGERS_CNT "SELECT COUNT(*) FROM managers;"
int Registration::Query_Cnt_Managers(void) {
    string str = QUERY_MANAGERS_CNT;
    int state = doSQL(myconnection, str.c_str());
    if (state !=2)
       return -1;     
    int n = PQntuples(result);
    if (n == 0) {PQclear(result);return 0;}
    n = atoi(PQgetvalue(result, 0, 0)); 
    PQclear(result);
    return n;
}

#define QUERY_MANAGERS_LIST "SELECT manager_id, manager_name FROM managers"
int Registration::Query_Managers_List(vector<string>* id_man_list, vector<string>* nam_man_list, string nameptrn, string barcodeptrn, string cardptrn) {
    
  int state;
  string str = QUERY_MANAGERS_LIST;
  
  bool haveptrn = false;
    
  if (nameptrn > "") {
    str += " WHERE UPPER(manager_name) ~~* UPPER('%";
    str += nameptrn;
    str += "%')";
    haveptrn = true;
  }
  if (barcodeptrn > "") {
    if (haveptrn) {
	str += " AND ";
    }
    else {
	str += " WHERE ";
    }
    str += " barcode ~~* '%";
    str += barcodeptrn;
    str += "%'";
    haveptrn = true;
  }
  if (cardptrn > "") {
    if (haveptrn) {
	str += " AND ";
    }
    else {
	str += " WHERE ";
    }
    str += " cardcode ~~* '%";
    str += cardptrn;
    str += "%'";
  }
  
  str += " ORDER BY manager_id";
  
//  printf("%s\n", str.c_str());
  
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
        id_man_list->push_back(PQgetvalue(result, i, 0));
	nam_man_list->push_back(PQgetvalue(result, i, 1));
    }
    PQclear(result);
    return n;
  }
  return -1;

}

//---------------------------------------

//-----(+)-----Zuskin-----14/12/2011------    
#define GET_DEFAULT_SCHEME_ID "SELECT auto_scheme_id FROM auto_scheme WHERE is_default = true"
int Registration::GetDefaultSchemeId(void) {
    
    string str = GET_DEFAULT_SCHEME_ID;
        
    int state = doSQL(myconnection, str.c_str());
    if ( state != 2 ) return -1;
    if ( PQntuples(result) != 1 ) {
	PQclear(result);
	return -1;
    }
    
    int res = atoi(PQgetvalue(result, 0, 0));
    PQclear(result);
    return res;
}
//----------------------------------------
