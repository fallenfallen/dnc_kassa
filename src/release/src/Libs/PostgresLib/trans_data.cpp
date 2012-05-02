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

#include "trans_data.h"
#include "addon_func.h"

int type_trans [TYPE_TRANS_MAX]; 
int type_trans_shtrih [TYPE_TRANS_MAX];
int type_trans_dnc[TYPE_TRANS_MAX];


/******************************************************************/
void Transaction::init_type_trans(int n_protocol)
{
    //printf("Transaction::init_type_trans (protocol = %i)\n", n_protocol);
    
    if (n_protocol == 0) {
	type_trans[14] = 1;
	type_trans[55] = 2;
	//-----(+)-----Zuskin-----29/08/2011-----
	type_trans[56] = 2;
	//---------------------------------------
	//-----(+)-----Zuskin-----28/10/2011-----
	type_trans[57] = 2;
	type_trans[58] = 2;
	//---------------------------------------
	type_trans[15] = 3;
	type_trans[16] = 3;
	type_trans[17] = 3;
	type_trans[18] = 3;
	type_trans[35] = 3;
	type_trans[36] = 3;
	type_trans[37] = 3;
	type_trans[38] = 3;
	type_trans[40] = 4;
	type_trans[41] = 4;
	type_trans[50] = 5;
	type_trans[51] = 5;
	type_trans[11] = 7;
	//-----(+)-----Zuskin-----29/08/2011-----
	type_trans[12] = 7;
	//---------------------------------------
	type_trans[60] = 6;
	type_trans[63] = 6;
    } 
    if ( n_protocol == 1 ) {
	type_trans_shtrih[11] = 1;
	type_trans_shtrih[15] = 2;
	type_trans_shtrih[16] = 2;
	type_trans_shtrih[17] = 3;
	type_trans_shtrih[18] = 3;
	type_trans_shtrih[35] = 4;
	type_trans_shtrih[36] = 4;
	type_trans_shtrih[37] = 5;
	type_trans_shtrih[38] = 5;
	type_trans_shtrih[40] = 7;
	type_trans_shtrih[41] = 7;
	type_trans_shtrih[63] = 9;
	type_trans_shtrih[60] = 9;
	type_trans_shtrih[55] = 10;
	type_trans_shtrih[50] = 8;
	type_trans_shtrih[51] = 8;
	type_trans_shtrih[70] = 6;
    }
    if ( n_protocol == 2 ) {
	type_trans_dnc[11] = 1;
	type_trans_dnc[12] = 1;
	type_trans_dnc[14] = 2;
	type_trans_dnc[15] = 3;
	type_trans_dnc[16] = 3;
	type_trans_dnc[17] = 3;
	type_trans_dnc[18] = 3;
	type_trans_dnc[35] = 3;
	type_trans_dnc[36] = 3;
	type_trans_dnc[37] = 3;
	type_trans_dnc[38] = 3;	
	type_trans_dnc[40] = 4;
	type_trans_dnc[41] = 4;
	type_trans_dnc[55] = 5;
	type_trans_dnc[56] = 5;
	//-----(+)-----Zuskin-----28/10/2011-----
	type_trans_dnc[57] = 5;
	type_trans_dnc[58] = 5;
	//---------------------------------------
	type_trans_dnc[50] = 6;
	type_trans_dnc[51] = 6;
	type_trans_dnc[60] = 7;
	type_trans_dnc[61] = 7;
	type_trans_dnc[62] = 7;
	type_trans_dnc[63] = 7;
    }
}
/******************************************************************/
Transaction::Transaction()  {   
//    printf("constructor Transaction ()\n");   
}
/******************************************************************/
Transaction::~Transaction() {  
//    printf("destroy transaction() \n");   
}
/******************************************************************/
void Transaction::DateToFormat(char* str) {
   char * tmp = (char*) calloc(13, sizeof(char));
   
   *(tmp) = *(str + 8);
   *(tmp + 1) = *(str + 9);
   *(tmp + 2) = '.';
   *(tmp + 3) = *(str + 5);
   *(tmp + 4) = *(str + 6);   
   *(tmp + 5) = '.';
   *(tmp + 6) = *(str);   
   *(tmp + 7) = *(str + 1);   
   *(tmp + 8) = *(str + 2);
   *(tmp + 9) = *(str + 3);

   int i = 0;  
   while ( i < 10 ) {*(str + i) = *(tmp + i); i++;}
   *(str + 10) = ';';
   
   free(tmp);
}
/******************************************************************/
void Transaction::add_minus(char* str) {
   if (strcmp(str, "") == 0) return;
   //printf("str = %s len = %d \n", str, strlen(str));
   for ( int i = strlen(str)-1; i >= 0 ; i-- ) {
     *(str + i + 1)  = *(str + i); 
   }
   *(str) = '-';   
}
/******************************************************************/
void Transaction::CreateRecord(char * str)   {

   int num_trans = atoi(data[1]);  // tr type 0 - 100
   
   if ( num_trans == 40 ) add_minus(data[15]);
   
   num_trans = type_trans[num_trans];   // tr type  ot 0 - 7 
   
   DateToFormat(data[2]);
   for (int i=0; i<COUNT_ATTRIBUTE_IN_FORMAT; i++) {  
      if ( mas[num_trans][i] != -1 && mas[num_trans][i] < COUNT_ATTRIBUTE ) {
	  //printf("data[ %d ] = %s \n", mas[num_trans][i] , data[ mas[num_trans][i] ]);
	  strcat(str, data[ mas[num_trans][i] ]); 
      }
      strcat(str, ";");
   }
   
   //printf("str = <%s> \n", str);
}
/******************************************************************/
void Transaction::CreateShtrihRecord(char * str)   {

    int num_trans = atoi(data[1]);  // tr type 0 - 100
    
    bool return_check = false;
    if ( atoi(data[8]) == 1 ) return_check = true;
    
    if ( return_check && num_trans == 11 ) {
	strcpy(data[1], "13");
	add_minus(data[15]);
	add_minus(data[20]);
    }
    
    if ( num_trans == 41 ) strcpy(data[1], "40");
    
    if ( return_check ) {
	if ( num_trans == 55  ) add_minus(data[15]);
	if ( num_trans == 17 || num_trans == 18 || num_trans == 37 || num_trans == 38 || num_trans == 40 || num_trans == 41 ) add_minus(data[14]); 
    }
    
    int num_record = type_trans_shtrih[num_trans];   // tr type  ot 0 - 7 
	
    if (num_trans != 70) DateToFormat(data[2]);
    for (int i=0; i<COUNT_ATTRIBUTE_SHTRIH; i++) {  
	if ( mas_shtrih[num_record][i] != -1 && mas_shtrih[num_record][i] < COUNT_ATTRIBUTE ) {
	    //printf("mas_shtrih[num_record][i] = %d \n", mas_shtrih[num_record][i]);
	    if (return_check && (num_trans == 15 || num_trans == 16 || num_trans == 35 || num_trans == 36) && i == 10) strcat(str, "-");
	    strcat(str, data[ mas_shtrih[num_record][i] ]); 
	}
	strcat(str, ";");
    }
    
    if (num_trans == 15 || num_trans == 16 || num_trans == 17 || num_trans == 18 || num_trans == 35 || num_trans == 36 || num_trans == 37 || num_trans == 38) {
	// детализация скидки
	strcpy(data[1], "70");
	strcat(str, "\r\n");
	CreateShtrihRecord(str);
    }
    
    //printf("str = <%s> \n", str);
}
/******************************************************************/
void Transaction::CreateRecordDnc(char * str)   {
    int num_trans = atoi(data[1]);  // tr type 0 - 100
    //printf("tr_type = %d \n", num_trans);    
   
    if ( num_trans == 40 ) add_minus(data[15]);
   
    num_trans = type_trans_dnc[num_trans];   // tr type  ot 0 - 7 
    //printf("transaction type = %d \n", num_trans);
   
    //DateToFormat(data[2]);
    for (int i = 0; i<COUNT_ATTRIBUTE_DNC; i++) {  
	if ( mas_dnc[num_trans][i] != -1 && mas_dnc[num_trans][i] < COUNT_ATTRIBUTE ) {
	  //printf("data[ %d ] = %s \n", mas[num_trans][i] , data[ mas[num_trans][i] ]);
	  strcat(str, data[ mas_dnc[num_trans][i] ]); 
      }
      strcat(str, ";");
   }
}
/******************************************************************/

