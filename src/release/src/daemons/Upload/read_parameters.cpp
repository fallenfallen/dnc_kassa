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

#include "read_parameters.h"

//===========================================================================//
BarcodeParameters::BarcodeParameters()
{
    barcode = NULL;
    internal_id = NULL;
    coefficient = NULL;
    error_param = false;
    DbCodec = QTextCodec::codecForName(DBCODEC);
    CP1251Codec = QTextCodec::codecForName("CP1251");
}
//===========================================================================//
BarcodeParameters::~BarcodeParameters()
{
    FreeBarcodeParameters();
}
//===========================================================================//
void BarcodeParameters::AddBarcodeParameters(int param_num, char * param)
{
  
    switch(param_num)  {
    case 2 :
	if(strcmp(param, "") && strcmp(param, "0"))      {
	    internal_id = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(internal_id, param);
	} else {
	    internal_id  = (char *) calloc(5, sizeof(char));
	    strcpy(internal_id, "null");      }
	break;
    case 1 : 
	barcode = (char *) calloc(strlen(param) + 3, sizeof(char));
	strcpy(barcode, param);
	break;
    case 4 :
	if(strcmp(param, "") && strcmp(param, "0"))      {
	    coefficient = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(coefficient, param); 
	} else {    
	    coefficient = (char *) calloc(2, sizeof(char));
	    strcpy(coefficient, "1");      }
	break;
    }
}

//===========================================================================//
void BarcodeParameters::FreeBarcodeParameters()
{
    if ( internal_id != NULL ) free(internal_id);
    if ( barcode != NULL ) free(barcode);
    if ( coefficient != NULL ) free(coefficient);
}
//#################################################################################################################//

//===========================================================================//

WareParameters::WareParameters()
{
    internal_code = NULL;
    bar_code = NULL;
    longtitle = NULL;
    shortcut = NULL;
    price = NULL;
    quantity = NULL;
    auto_scheme_id = NULL;
    flags = NULL;
    min_price = NULL;
    best_before = NULL;
    coefficient = NULL;
    group_code = NULL;
    tax_group_id = NULL;
    
    error_param = false;
    DbCodec = QTextCodec::codecForName(DBCODEC);
    CP1251Codec = QTextCodec::codecForName("CP1251");
}

//===========================================================================//
WareParameters::~WareParameters()
{
    FreeWareParameters();
}


//===========================================================================//
void WareParameters::AddWareParameter(int param_num, char * param)
{
    //printf("void WareParameters::AddWareParameter(int param_num = %d , char * param = %s )\n", param_num, param);
    
   switch(param_num)  {
   case 1 :
       //printf(" internal_id = %s \n", param);
       if(strcmp(param, "") && strcmp(param, "0"))      {
	       internal_code = (char *) calloc(strlen(param)*2 + 1, sizeof(char));
	       strcpy(internal_code, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	       break;
       }      
       internal_code = (char *) calloc(5, sizeof(char));
       strcpy(internal_code, "null");
       error_param = true;
       break;	
   case 2 : // barcode     
       if ( strcmp(param, "")  && strcmp(param, "0") )      {
	   bar_code = (char *) calloc(strlen(param) + 1, sizeof(char));
	   strcpy(bar_code, param);
       }  else  {
	   bar_code = (char *) calloc (5,sizeof(char));
	   strcpy(bar_code, "");
       }
       
       //SaveBarcode(param);
       break;
   case 3 : // longtitle
       if ( strcmp(param, "")  && strcmp(param, "0") )      {       
	   longtitle = (char *) calloc((strlen(param) + 2+1)*2, sizeof(char));
	   //printf("longtitle = %s \n", param);
	   
	   QString longtitle_str = DbCodec->fromUnicode(CP1251Codec->toUnicode(param));
	   strcpy(longtitle, "'");
	   strcat(longtitle, longtitle_str);
	   strcat(longtitle, "'");
       } else {
	   longtitle = (char* ) calloc(5, sizeof(char));
	   strcpy(longtitle, "null");
       }
       //   str_cp1251_to_koi8r(longtitle);
       break;
   case 4 : // shortcut
       if ( strcmp(param, "")  && strcmp(param, "0") )      {       
	   shortcut = (char *) calloc((strlen(param) + 2 + 1)*2, sizeof(char));
	   QString shortcut_str = DbCodec->fromUnicode(CP1251Codec->toUnicode(param));
	   strcpy(shortcut, "'");	   
	   strcat(shortcut, shortcut_str);
	   strcat(shortcut, "'");
	   //   str_cp1251_to_koi8r(shortcut);
       } else {
	   shortcut = (char* ) calloc(5, sizeof(char));
	   strcpy(shortcut, "null");
       }
       
       break;
   case 5 : // price
       SaveDoubleValue(5, param);
       break;
   case 6 : // quantity
       //SaveDoubleValue(6, param);
       if ( strcmp(param, "") != 0 )      {
	   quantity = (char *) calloc(strlen(param) + 1, sizeof(char));
	   strcpy(quantity, param);
       }  else  {
	   quantity = (char *) calloc(2, sizeof(char));
	   strcpy(quantity, "0");
       }	
       break;
   case 7 :
       if ( strcmp(param, "") && strcmp(param, "0") )      {
	   auto_scheme_id = (char *) calloc(strlen(param) + 1, sizeof(char));
	   strcpy(auto_scheme_id, param);
       }      else      {
	   auto_scheme_id = (char *) calloc(5, sizeof(char));
	   strcpy(auto_scheme_id, "null");
       }	
       break;
   case 8 : // flags
       SaveFlags(param);
       break;
   case 9 : // min_price
       SaveDoubleValue(9, param);
       break;
   case 10 : // best_before
       SaveDateTimeValue(best_before, param);
       break;
   case 14 :   //coefficient
       if ( strcmp(param, "") && strcmp(param, "0") )    {
	   coefficient = (char *) calloc(strlen(param) + 1, sizeof(char));
	   strcpy(coefficient, param);
       }  else    {
	   coefficient = (char *) calloc(2, sizeof(char));
	   strcpy(coefficient, "1");      }
       break;
   case 16 :  //group id
       if ( strcmp(param, "") && strcmp(param, "0") )      {
    	    if (IsInteger(param))	{
		group_code = (char *) calloc(strlen(param) + 1, sizeof(char));
		strcpy(group_code, param);
		break;
	    }	
       } 
       group_code = (char *) calloc(5, sizeof(char));
       strcpy(group_code, "null");
       break;
   case 23 :
       if ( strcmp(param, "") && strcmp(param, "0") )      {
    	    if (IsInteger(param))	{
		tax_group_id = (char *) calloc(strlen(param) + 1, sizeof(char));
		strcpy(tax_group_id, param);
		break;
	    }	
       }	
       tax_group_id = (char *) calloc(5, sizeof(char));
       strcpy(tax_group_id, "null");
       break;
   }	
}

//===========================================================================//
void WareParameters::FreeWareParameters()
{
  if ( internal_code != NULL ) free(internal_code);
  if ( bar_code != NULL ) free(bar_code);
  if ( longtitle != NULL ) free(longtitle);
  if ( shortcut != NULL ) free(shortcut);
  if ( price != NULL ) free(price);
  if ( quantity != NULL ) free(quantity);
  if ( auto_scheme_id != NULL ) free(auto_scheme_id);
  if ( flags != NULL ) free(flags);
  if ( min_price != NULL ) free(min_price);
  if ( best_before != NULL ) free(best_before);
  if ( coefficient != NULL ) free(coefficient);
  if ( group_code != NULL ) free(group_code);
  if ( tax_group_id != NULL ) free(tax_group_id);
}

//#################################################################################################################//

//===========================================================================//
void WareParameters::SaveBarcode(char * param)
{
  /*char * tail, * head;
  head = param;	

  tail = strchr(head, ',');
  if(tail != NULL) // only first barcode if count of them > 1
  {
    int tok_len = strlen(head) - strlen(tail);
    bar_code = (char *) calloc(tok_len + 2 + 1, sizeof(char));
    strcpy(bar_code, "'");
    strncpy(bar_code + 1, head, tok_len);
    strcat(bar_code, "'");
  }
    else
  {
	if (strlen(param) > 0) // if there's only one barcode
	{
	 bar_code = (char *) calloc(strlen(param) + 2 + 1, sizeof(char));
    strcpy(bar_code, "'");
    strncpy(bar_code + 1, param, strlen(param));
    strcat(bar_code, "'");
	}
	else // if there's no barcode at all
	{
    bar_code = (char *) calloc(5, sizeof(char));
    strcpy(bar_code, "null"); 
                }
  }     */

    if ( strlen(param) > 0 )  {
	bar_code = (char *) calloc(strlen(param) + 1, sizeof(char));
	strcpy(bar_code, param);
    }  else  {
	bar_code = (char *) calloc (1,sizeof(char));
    }
    
}
//===========================================================================//
// 0 -- не верный формат параметра
void WareParameters::SaveFlags(char * param)
{
    bool wrong_par_flag = false;
    //Прочитаем все параметры-флаги
    int fls[FLAG_NUM];
    int k = 0;
    
    fls[0] = 0;
    for (int i = 1; i < FLAG_NUM; i ++)  {          fls[i]=1;	  }
    
    for(unsigned int i = 0; i < strlen(param); i+=2)  {
	if ( ( ( *(param + i) == '0' ) || ( *(param + i) == '1' ) ) && ( ( *(param + i + 1) == ',' ) || ( *(param + i + 1) == '\0' ) ) )    {
	    fls[k] = (int) ( *(param+i) - 48 );
	    k++;
	}	
	//else
	//{
	//  wrong_par_flag = true;
	//  break;
	// }
    }
  
  if ( wrong_par_flag )  {
      flags = (char *) calloc(2, sizeof(char));
      strcpy(flags, "0");
  }  else  {
      // Побитово закодируем флаги
      int bitfl = 0;
      for ( int i = 0; i < 9; i ++ )     
	  if ( fls[i] != 0 )  bitfl = bitfl | (int)pow(2, i);
      
      // Полученное число преобразуем в строку
      char *strfl = itoa(bitfl); // Не забыть очистить память
      flags = (char *) calloc(strlen(strfl) + 1, sizeof(char));
      strcpy(flags, strfl);
      
      free(strfl);                    // Очиcтка памяти
  }
}

//===========================================================================//
//Функция временная, в реализации не нужна!!!! Проверка числовых параметров некорректна!
void WareParameters::SaveDoubleValue(int num, char * param)
{
    char * str;
    if ( strcmp(param, "") )  {
	int len = strlen(param);
	str = (char *) calloc(len + 1, sizeof(char));
	int k = 0;
	int i = 0;
	for ( i = 0; i < len; i++ )	{
	    if ( ( *(param + i) >= 48 ) && ( *(param + i) <= 57 ) || ( *(param + i) == 46 ) )      {
		*(str + k) = *(param + i);
		k++;
	    }
	    if ( *(param + i) == 44 )	{
		*(str + k) = 46; // заменяем [,] на [.]
		k++;
	    }
	}	
    }  else  {
	str = (char *) calloc(2, sizeof(char));
	strcpy(str, "0");
    }
    switch(num)  {
    case 5 :
	price = (char *) calloc(strlen(str) + 1, sizeof(char));
	strcpy(price, str);
	break;
    case 6 :
	quantity = (char *) calloc(strlen(str) + 1, sizeof(char));
	strcpy(quantity, str);	
	break;
    case 9 :
	min_price = (char *) calloc(strlen(str) + 1, sizeof(char));
	strcpy(min_price, str);
	break;
    }
    free(str);
}

//===========================================================================//
//НУЖНО БУДЕТ РАЗОБРАТЬСЯ, ЧТО ПЕРЕДАЕТСЯ ПО ПРОТОКОЛУ: ТОЛЬКО ДАТА ИЛИ ДАТА И ВРЕМЯ
//БУДЕМ СЧИТАТЬ, ЧТО ТАМ ВРЕМЯ И ДАТА
void WareParameters::SaveDateTimeValue(char *& datetimevalue, char * param)
{
    
    if ( strcmp(param, "") && strcmp(param, "0") )  {   // не пустое значение
	char * date_str;
	datetimevalue = (char *) calloc(strlen(param) + 2 + 1, sizeof(char));
	if ( (date_str = strchr(param, ' ')) != NULL )  { // "Время Дата"
	    strcpy(datetimevalue, "'");
	    date_str = date_str + 1;
	    strcat(datetimevalue, date_str);
	    strcat(datetimevalue, " ");
	    strncat(datetimevalue, param, strlen(param) - strlen(date_str) - 1);
	    strcat(datetimevalue, "'");
	}    else    {
	    strcpy(datetimevalue, "'");
	    strcat(datetimevalue, param);
	    strcat(datetimevalue, "'");
	}
    }  else  {
	datetimevalue = (char *) calloc(5, sizeof(char));
	strcpy(datetimevalue, "null");
    }
}




//#################################################################################################################//

//===========================================================================//
GroupParameters::GroupParameters()
{
    code = NULL;
    longtitle = NULL;
    shortcut = NULL;	
    group_code = NULL;
    
    error_param = false;
    DbCodec = QTextCodec::codecForName(DBCODEC);
    CP1251Codec = QTextCodec::codecForName("CP1251");
}

//===========================================================================//
GroupParameters::~GroupParameters()
{
    FreeGroupParameters();
}
//===========================================================================//
void GroupParameters::AddGroupParameter(int param_num, char * param)
{

    switch(param_num) {
    case 1 :	// code
	if ( strcmp(param, "0") == 0 &&  strlen(param) > 7 ) {
	    code = (char *) calloc(7, sizeof(char));
	    strcpy(code, "null");
	    error_param = true;
	    break;
	}
	
	if ( strcmp(param, "") && strcmp(param, "0") )    {
	    if ( IsInteger(param) )	{
		code = (char *) calloc(strlen(param) + 1, sizeof(char));
		strcpy(code, param);
		break;
	    }	
	}   
	code = (char *) calloc(6, sizeof(char));
	strcpy(code, "null");
	error_param = true;
	break;
    case 3 : // longtitle
	longtitle = (char *) calloc((strlen(param) + 2 +  1)*2, sizeof(char));
	strcpy(longtitle, "'");
	strcat(longtitle, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	strcat(longtitle, "'");  
	//  str_cp1251_to_koi8r(longtitle);
	break;
    case 4 : // shortcut
	shortcut = (char *) calloc((strlen(param) + 2 + 1)*2, sizeof(char));
	strcpy(shortcut, "'");
	strcat(shortcut, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	strcat(shortcut, "'");
	
	// str_cp1251_to_koi8r(shortcut);
	break;
    case 16 :
	if ( strcmp(param, "") && strcmp(param, "0") )      {
	    if ( IsInteger(param) )	{
	    group_code = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(group_code, param);
	    break;
	    }
	}      
        group_code = (char *) calloc(5, sizeof(char));
        strcpy(group_code, "null");
	break;
    }	
}

//===========================================================================//
void GroupParameters::FreeGroupParameters()
{
    if ( code != NULL ) free(code);
    if ( longtitle != NULL ) free(longtitle);
    if ( shortcut != NULL ) free(shortcut);
    if ( group_code != NULL ) free(group_code);
}

//#################################################################################################################//

//===========================================================================//
UserParameters::UserParameters()
{
  
    code = NULL;
    longtitle = NULL;
    shortcut = NULL;
    profile_code = NULL;
    password = NULL;
    card_code = NULL;
    
    error_param = false;
    DbCodec = QTextCodec::codecForName(DBCODEC);
    CP1251Codec = QTextCodec::codecForName("CP1251");
}

//===========================================================================//
UserParameters::~UserParameters()
{
    FreeUserParameters();
}
//===========================================================================//
void UserParameters::FreeUserParameters()
{
    if ( code != NULL ) free(code);
    if ( longtitle != NULL ) free(longtitle);
    if ( shortcut != NULL ) free(shortcut);
    if ( profile_code != NULL ) free(profile_code);
    if ( password != NULL ) free(password);
    if ( card_code != NULL ) free(card_code);
    
    code = NULL;
    longtitle = NULL;
    shortcut = NULL;
    profile_code = NULL;
    password = NULL;
    card_code = NULL;
}

//===========================================================================//
void UserParameters::AddUserParameter(int param_num, char * param)
{
    switch(param_num)  {
    case 1 :
	if ( strcmp(param, "") && strcmp(param, "0") )     {
	    code = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(code, param);	
	}      else      {
	    code = (char *) calloc(5, sizeof(char));
	    strcpy(code, "null");
	    error_param = true;
	}
	break;
    case 2 : // longtitle
	longtitle = (char *) calloc((strlen(param) + 2 +  1)*2, sizeof(char));
	strcpy(longtitle, "'");
	strcat(longtitle, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	strcat(longtitle, "'");
	// str_cp1251_to_koi8r(longtitle);
	break;
    case 3 : // shortcut
	shortcut = (char *) calloc((strlen(param) + 2 + 1)*2, sizeof(char));
	strcpy(shortcut, "'");
	strcat(shortcut, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	strcat(shortcut, "'");
	//  str_cp1251_to_koi8r(shortcut);
	break;
    case 4 :
	if ( strcmp(param, "") && strcmp(param, "0") )      {
	    profile_code = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(profile_code, param);
	}      else      {
	    profile_code =	 (char *) calloc(5, sizeof(char));
	    strcpy(profile_code, "null");
	}
	break;
    case 5 :
	password = (char *) calloc((strlen(param) + 2 + 1)*2, sizeof(char));
	strcpy(password, "'");
	strcat(password, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	strcat(password, "'");
	//str_cp1251_to_koi8r(password);
	break;
    case 6 :
	if(strcmp(param, ""))      {
	    card_code = (char *) calloc((strlen(param) + 1)*2, sizeof(char));
	    strcpy(card_code, "'");
	    strcat(card_code, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	    strcat(card_code, "'");
	    //  str_cp1251_to_koi8r(card_code);
	}      else      {
	    card_code = (char *) calloc(5, sizeof(char));
	    strcpy(card_code, "null");
	}
	break;
    }
}

//#################################################################################################################//

//===========================================================================//
FixReductionParameters::FixReductionParameters()
{
  
    code = NULL;
    longtitle = NULL;
    shortcut = NULL;	
    card_prefix = NULL;
    type = NULL;	
    value = NULL;
    area = NULL;
    barcode = NULL;
    show = NULL;
    
    barcode = (char *) calloc(5, sizeof(char));
    strcpy(barcode, "null");
    
    error_param = false;
    
    DbCodec = QTextCodec::codecForName(DBCODEC);
    CP1251Codec = QTextCodec::codecForName("CP1251");
}
//===========================================================================//
FixReductionParameters::~FixReductionParameters()
{
    FreeFixReductionParameters();
}
//===========================================================================//
void FixReductionParameters::FreeFixReductionParameters()
{
    //printf("code = %s \n", code);
    
    if ( code != NULL ) free(code);
    if ( longtitle != NULL ) free(longtitle);
    if ( shortcut != NULL ) free(shortcut);
    if ( card_prefix != NULL ) free(card_prefix);	
    if ( type != NULL ) free(type);
    if ( value != NULL ) free(value);
    if ( area != NULL ) free(area);	
    if ( barcode  != NULL ) free(barcode);	
    if ( show != NULL ) free(show);
    
    //printf("FixReductionParameters::FreeFixReductionParameters() is ok");
}

//===========================================================================//
void FixReductionParameters::AddFixReductionParameter(int param_num, char * param)
{
    //printf("param_num = %d, param = %s ", param_num, param);
    //printf("strlen(param) = %d \n", strlen(param));
    switch(param_num)  {
    case 1 :
	if ( strcmp(param, "") && strcmp(param, "0") )      {
	    code = (char *) calloc((strlen(param) + 1), sizeof(char));
	    strcpy(code, param);
	}      else      {
	    code = (char *) calloc(5, sizeof(char));
	    strcpy(code, "null");
	    error_param = true;
	}
	break;
    case 2 : // longtitle
	longtitle = (char *) calloc(2*strlen(param) + 3, sizeof(char));
	strcpy(longtitle, "'");
	strcat(longtitle, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	strcat(longtitle, "'");
	// str_cp1251_to_koi8r(longtitle);
	break;
    case 3 : // shortcut
	shortcut = (char *) calloc(2*strlen(param) +3, sizeof(char));
	strcpy(shortcut, "'");
	strcat(shortcut, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	strcat(shortcut, "'");
	//str_cp1251_to_koi8r(shortcut);
	break;
    case 4 :
	if ( strcmp(param, "") )      {
	    card_prefix = (char *) calloc(strlen(param) + 3, sizeof(char));
	    strcpy(card_prefix, "'");
	    strcat(card_prefix, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	    strcat(card_prefix, "'");
	    //str_cp1251_to_koi8r(card_prefix);
	}      else      {
	    card_prefix = (char *) calloc(5, sizeof(char));
	    strcpy(card_prefix, "null");
	}
	//printf("card_prefix = %s \n", card_prefix);
	break;
    case 5 :
	if ( !strcmp(param, "0") || !strcmp(param, "1") || !strcmp(param, "2") || !strcmp(param, "3") )      {
	    type = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(type, param);
	}      else      {
	    type = (char *) calloc(9, sizeof(char));
	    strcpy(type, "null");
	}
	break;
    case 6 :
	if ( strcmp(param, "") )      {
	    value = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(value, param);
	}      else      {
	    value = (char *) calloc(5, sizeof(char));
	    strcpy(value, "0");
	}
	break;
    case 9 :
	area = (char*) calloc(5, sizeof(char) );
	if ( strcmp(param, "1") )       strcpy(area, "'f'");	else strcpy(area, "'t'");
	break;
    case 10 :
	//printf("param = %s \n", param);
	free(barcode);
	if ( strcmp(param, "") )      {
	    barcode = (char *) calloc(strlen(param) + 4, sizeof(char));
	    strcpy(barcode, "'");
	    strcat(barcode, param);
	    strcat(barcode, "'");
	}     else      {
	    barcode = (char *) calloc(5, sizeof(char));
	    strcpy(barcode, "null");
	}
	break;      
    case 11 :
	//printf("param = %s \n", param);
	show = (char*) calloc(7, sizeof(char));
	if ( strcmp(param, "0") == 0 ) strcpy(show, "'f'"); else strcpy(show, "'t'");
	break;      	
    }
    //printf("param_num = %d param = %s succesful load \n", param_num, param);
    
}

//#################################################################################################################//

//===========================================================================//
AutoSchemeParameters::AutoSchemeParameters()
{
    code = NULL;
    longtitle = NULL;
    shortcut = NULL;
    
    error_param = false;
    DbCodec = QTextCodec::codecForName(DBCODEC);
    CP1251Codec = QTextCodec::codecForName("CP1251");
}

//===========================================================================//
AutoSchemeParameters::~AutoSchemeParameters()
{
    FreeAutoSchemeParameters();
}

//===========================================================================//
void AutoSchemeParameters::FreeAutoSchemeParameters()
{
    if ( code != NULL ) free(code) ;
    if ( longtitle != NULL ) free(longtitle);
    if ( shortcut != NULL ) free(shortcut);
}
//===========================================================================//
void AutoSchemeParameters::AddAutoSchemeParameter(int param_num, char * param)
{
    switch(param_num)  {
    case 1 :
	if ( strcmp(param, "") && strcmp(param, "0") )      {
	    code = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(code, param);
	}      else      {
	    code = (char *) calloc(5, sizeof(char));
	    strcpy(code, "null");
	    error_param = true;
	}
	break;
    case 2 :
	longtitle = (char *) calloc((strlen(param) + 2 + 1)*2, sizeof(char));
	strcpy(longtitle, "'");
	strcat(longtitle, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	strcat(longtitle, "'");
	//str_cp1251_to_koi8r(longtitle);
	break;
    case 3 :
	shortcut = (char *) calloc((strlen(param) + 2 + 1)*2, sizeof(char));
	strcpy(shortcut, "'");
	strcat(shortcut, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	strcat(shortcut, "'");
	//  str_cp1251_to_koi8r(shortcut);
	break;
    }
}

//#################################################################################################################//

//===========================================================================//
AutoReductionParameters::AutoReductionParameters()
{
    code = NULL;
    longtitle = NULL;
    shortcut = NULL;
    scheme_code = NULL;
    type = NULL;
    value = NULL;
    begin_date = NULL;	
    end_date = NULL;
    begin_time = NULL;
    end_time = NULL;
    weekdays = NULL;
    quantity = NULL;
    position_sum = NULL;
    check_sum = NULL;
    card_prefix_begin = NULL;
    card_prefix_end = NULL;
    
    error_param = false;
    
    day_beg = -1;
    day_end = -1;
    
    DbCodec = QTextCodec::codecForName(DBCODEC);
    CP1251Codec = QTextCodec::codecForName("CP1251");
}

//===========================================================================//
AutoReductionParameters::~AutoReductionParameters()
{
    FreeAutoReductionParameters();
}
//===========================================================================//
void AutoReductionParameters::FreeAutoReductionParameters()
{
    if ( code != NULL ) free(code);
    if ( longtitle != NULL ) free(longtitle);
    if ( shortcut != NULL ) free(shortcut);
    if ( scheme_code != NULL ) free(scheme_code);	
    if ( type != NULL )  free(type);
    if ( value != NULL ) free(value);
    if ( begin_date != NULL ) free(begin_date);
    if ( end_date != NULL ) free(end_date);
    if ( begin_time != NULL ) free(begin_time);
    if ( end_time != NULL ) free(end_time);
    if ( weekdays != NULL )  free(weekdays);
    if ( quantity != NULL ) free(quantity);
    if ( position_sum != NULL ) free(position_sum);
    if ( check_sum != NULL ) free(check_sum);
    if ( card_prefix_begin != NULL ) free(card_prefix_begin);
    if ( card_prefix_end != NULL ) free(card_prefix_end);
}
//===========================================================================//
void AutoReductionParameters::AddAutoReductionParameter(int param_num, char * param)
{
    switch(param_num)  {
    case 1 :
	if ( strcmp(param, "") && strcmp(param, "0") )      {
	    scheme_code = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(scheme_code, param);
	}      else      {
	    scheme_code = (char *) calloc(4, sizeof(char));
	    strcpy(scheme_code, "null");
	}
	break;
    case 2 :
	if ( strcmp(param, "") && strcmp(param, "0") )      {
	    code = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(code, param);
	}      else      {
	    code = (char *) calloc(5, sizeof(char));
	    strcpy(code, "null");
	    error_param = true;
	}
	break;
    case 3 : // longtitle
	longtitle = (char *) calloc((strlen(param) + 2 +  1)*2, sizeof(char));
	strcpy(longtitle, "'");
	strcat(longtitle, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));	
	strcat(longtitle, "'");	
	//str_cp1251_to_koi8r(longtitle);
	break;
    case 4 : // shortcut
	shortcut = (char *) calloc((strlen(param) + 2 + 1)*2, sizeof(char));
	strcpy(shortcut, "'");
	strcat(shortcut, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	strcat(shortcut, "'");
	// str_cp1251_to_koi8r(shortcut);
	break;
    case 5 :
	if ( !strcmp(param, "0") || !strcmp(param, "1") || !strcmp(param, "2") || !strcmp(param, "3") )      {
	    type = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(type, param);
	}	else      {
	    type = (char *) calloc(5, sizeof(char));
	    strcpy(type, "null");
	}
	break;
    case 6 :
	if ( strcmp(param, "") && strcmp(param, "0") && (*param != '=') )      {
	    value = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(value, param);
	}      else      {
	    value = (char *) calloc(2, sizeof(char));
	    strcpy(value, "0");
	}
	break;
    case 7 :
	if ( strcmp(param, "") )      {
	    begin_date = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(begin_date, "'");
	    strcat(begin_date, param);
	    strcat(begin_date, "'");
	}      else      {
	    begin_date = (char *) calloc(5, sizeof(char));
	    strcpy(begin_date, "null");
	}
	break;
    case 8 :
	if ( strcmp(param, "") )      {
	    end_date = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(end_date, "'");
	    strcat(end_date, param);
	    strcat(end_date, "'");
	}      else      {
	    end_date = (char *) calloc(5, sizeof(char));
	    strcpy(end_date, "null");
	}
	break;
    case 10 :
	if  ( strcmp(param, "") )	{
	    begin_time = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(begin_time, "'");
	    strcat(begin_time, param);
	    strcat(begin_time, "'");
	}      else      {
	    begin_time = (char *) calloc(5, sizeof(char));
	    strcpy(begin_time, "null");
	}
	break;
    case 11 :
	if ( strcmp(param, "") )      {
	    end_time = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(end_time, "'");
	    strcat(end_time, param);
	    strcat(end_time, "'");
	}      else      {
	    end_time = (char *) calloc(5, sizeof(char));
	    strcpy(end_time, "null");
	}
	break;
    case 13 :
	SaveWeekdays(param_num, param);
	break;	
    case 14 :
	SaveWeekdays(param_num, param);
	break;
    case 16 :
	if ( strcmp(param, "") )     {
	    quantity = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(quantity, param);
	}      else      {
	    quantity = (char *) calloc(5, sizeof(char));
	    strcpy(quantity, "null");
	}
	break;
    case 19 :
	if ( strcmp(param, "") )      {
	    position_sum = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(position_sum, param);
	}      else      {
	    position_sum = (char *) calloc(5, sizeof(char));
	    strcpy(position_sum, "null");
	}
	break;
    case 22 :
	if ( strcmp(param, "") )      {
	    check_sum = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(check_sum, param);
	}      else      {
	    check_sum = (char *) calloc(5, sizeof(char));
	    strcpy(check_sum, "null");
	}
	break;
    case 25 :
	if ( strcmp(param, "") )      {
	    card_prefix_begin = (char *) calloc((strlen(param) + 1)*2, sizeof(char));
	    strcpy(card_prefix_begin, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	    // str_cp1251_to_koi8r(card_prefix_begin);
	}      else      {
	    card_prefix_begin = (char *) calloc(5, sizeof(char));
	    strcpy(card_prefix_begin, "null");    
	}
	break;
    case 26 :
	if ( strcmp(param, "") )     {
	    card_prefix_end = (char *) calloc((strlen(param) + 1)*2, sizeof(char));
	    strcpy(card_prefix_end, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	    // str_cp1251_to_koi8r(card_prefix_end);
	}      else      {
	    card_prefix_end = (char *) calloc(5, sizeof(char));
	    strcpy(card_prefix_end, "null");
	}
	break;
    }
}	
//===========================================================================//
void AutoReductionParameters::SaveWeekdays(int param_num, char * param)
{
    switch(param_num)  {
    case 13 :
	if ( (*param >= 48) && (*param <= 53 ) && (*(param + 1) =='\0') )       {
	    day_beg = (int) ( *param - 48 );
	}
	break;
    case 14 :
	if ( day_beg != -1 )      {
	    if ( (*param >= 48) && (*param <= 53 ) && (*(param + 1) =='\0') )        {
		day_end = (int) (*param - 48);
		if ( day_beg < day_end )        {
		    int wds = 0;
		    // За Понедельник отвечает нулевой бит и так далее  до 6-ого
		    for ( int i = day_beg; i <=day_end; i++)     wds = wds | (int)pow(2, i); 
		    char * wds_str = itoa(wds); // не забывать очищать эту память
		    weekdays = (char *) calloc(strlen(wds_str) + 1, sizeof(char));
		    strcpy(weekdays, wds_str);
		    free(wds_str);
		    break;
		}
	    }
	}
	//Сюда приходим только в случае любого не верного заполнения двух полей, отвечающих за дни недели
	weekdays = (char *) calloc(4, sizeof(char));	
	strcpy(weekdays, "null");
	break;
    }
}

//#################################################################################################################//

//===========================================================================//
TaxRateParameters::TaxRateParameters()
{
    code = NULL;
    longtitle = NULL;
    shortcut = NULL;
    value = NULL;
    
    error_param = false;
    
    DbCodec = QTextCodec::codecForName(DBCODEC);
    CP1251Codec = QTextCodec::codecForName("CP1251");
}

//===========================================================================//
TaxRateParameters::~TaxRateParameters()
{
    FreeTaxRateParameters();
}

//===========================================================================//
void TaxRateParameters::FreeTaxRateParameters()
{
    if ( code != NULL ) free(code);
    if ( longtitle != NULL ) free(longtitle);
    if ( shortcut != NULL ) free(shortcut);
    if ( value != NULL ) free(value);
}

//===========================================================================//
void TaxRateParameters::AddTaxRateParameter(int param_num, char * param)
{
    switch(param_num)  {
    case 1 :
	if ( strcmp(param, "") && strcmp(param, "0") )      {
	    code = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(code, param);
	}      else      {
	    code = (char *) calloc(5, sizeof(char));
	    strcpy(code, "null");
	    error_param = true;
	}
	break;
    case 2 :
	longtitle = (char *) calloc((strlen(param) + 2 +  1)*2, sizeof(char));
	strcpy(longtitle, "'");
	strcat(longtitle, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	strcat(longtitle, "'");
	//str_cp1251_to_koi8r(longtitle);
	break;
    case 3 :
	shortcut = (char *) calloc((strlen(param) + 2 + 1)*2, sizeof(char));
	strcpy(shortcut, "'");	
	strcat(shortcut, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	strcat(shortcut, "'");
	//str_cp1251_to_koi8r(shortcut);
	break;
    case 5 :
	if ( strcmp(param, "") )      {
	    value = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(value, param);
	}      else      {
	    value = (char *) calloc(2, sizeof(char));
	    strcpy(value, "0");
	}
	break;
    }
}

//#################################################################################################################//

//===========================================================================//
TaxGroupParameters::TaxGroupParameters()
{
    code = NULL;
    longtitle = NULL;
    shortcut = NULL;
    
    error_param = false;
    
    DbCodec = QTextCodec::codecForName(DBCODEC);
    CP1251Codec = QTextCodec::codecForName("CP1251");
}

//===========================================================================//
TaxGroupParameters::~TaxGroupParameters()
{
    FreeTaxGroupParameters();
}

//===========================================================================//
void TaxGroupParameters::FreeTaxGroupParameters()
{
    if ( code != NULL ) free(code);
    if ( longtitle != NULL ) free(longtitle);
    if ( shortcut != NULL ) free(shortcut);
}

//===========================================================================//
void TaxGroupParameters::AddTaxGroupParameter(int param_num, char * param)
{
    switch(param_num)    {
    case 1 :
	if ( strcmp(param, "") && strcmp(param, "0") )      {
	    code = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(code, param);
	}      else      {
	    code = (char *) calloc(5, sizeof(char));
	    strcpy(code, "null");
	    error_param = true;
	}
	break;
    case 2 :
	longtitle = (char *) calloc((strlen(param) + 2 +  1)*2, sizeof(char));
	strcpy(longtitle, "'");
	strcat(longtitle, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	strcat(longtitle, "'");
	// str_cp1251_to_koi8r(longtitle);
	break;
    case 3 :
	shortcut = (char *) calloc((strlen(param) + 2 + 1)*2, sizeof(char));
	strcpy(shortcut, "'");
	strcat(shortcut, DbCodec->fromUnicode(CP1251Codec->toUnicode(param)));
	strcat(shortcut, "'");
	//   str_cp1251_to_koi8r(shortcut);
	break;
    }
}

//#################################################################################################################//

//===========================================================================//
TaxRateGroupParameters::TaxRateGroupParameters()
{
    code = NULL;
    group_code = NULL;
    rate_code = NULL;
    base_flag = NULL;
    
    error_param = false;
}

//===========================================================================//
TaxRateGroupParameters::~TaxRateGroupParameters()
{
    FreeTaxRateGroupParameters();
}
//===========================================================================//
void TaxRateGroupParameters::FreeTaxRateGroupParameters()
{
    if ( code != NULL ) free(code);
    if ( group_code != NULL ) free(group_code);
    if ( rate_code != NULL ) free(rate_code);
    if ( base_flag != NULL ) free(base_flag);
}

//===========================================================================//
void TaxRateGroupParameters::AddTaxRateGroupParameter(int param_num, char * param)
{
    switch(param_num)  {
    case 1 :
	if ( strcmp(param, "") && strcmp(param, "0") )      {
	    code = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(code, param);
	}      else      {
	    code = (char *) calloc(5, sizeof(char));
	    strcpy(code, "null");
	    error_param = true;
	}
	break;
    case 2 :
	if ( strcmp(param, "") && strcmp(param, "0") )      {
	    group_code = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(group_code, param);
	}      else      {
	    group_code = (char *) calloc(5, sizeof(char));
	    strcpy(group_code, "null");
	}
	break;
    case 3 :
	if ( strcmp(param, "") && strcmp(param, "0") )      {
	    rate_code = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(rate_code, param);
	}      else      {
	    rate_code = (char *) calloc(5, sizeof(char));
	    strcpy(rate_code, "null");
	}
	break;
    case 4 :
	if ( !strcmp(param, "1") || !strcmp(param, "0") )      {
	    base_flag = (char *) calloc(strlen(param) + 1, sizeof(char));
	    strcpy(base_flag, "'");
	    strcat(base_flag, param);
	    strcat(base_flag, "'");
	}	else	{
	    base_flag = (char *) calloc(5, sizeof(char));
	    strcpy(base_flag, "null");
	}
	break;
    }	
}

/*************************************************************************/
bool IsInteger(char* str) {

    if (strlen(str) == 0) return false;
    int i=0;
    bool res = true;
    for (i = 0; i < strlen(str); i++) {
	if (( *(str + i) < 48 ) || ( *(str + i) > 57 )) {
    	    res = false;
	    break;
	}    
    }
    
    return res;
}
/*************************************************************************/
//#################################################################################################################//

