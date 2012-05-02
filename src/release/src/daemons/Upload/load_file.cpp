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

#include "load_file.h"
#include "const_conf.h"

//===========================================================================//
LoadFile::LoadFile(const char * path)
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "LoadFile::LoadFile(char * path)\n");
  
    er_con = -1;
    
    file_path = NULL;
    cur_str = NULL; 
    prev_comm = NULL;
    cur_param = NULL;
    pg_upload = NULL;
    er_log = NULL;
    fd = NULL ;
    

    fd = NULL;
    prev_comm = NULL;
    
    file_path = (char *) calloc(strlen(path) + 1, sizeof(char));
    strcpy(file_path, path);
    
    cur_str = (char *) calloc(MAX_STR_LEN + 1, sizeof(char));
    
    cur_command = -1;	
    cur_param= (char *) calloc(MAX_PARAM_LEN + 1, sizeof(char));
    er_con = 0;
    
    pg_upload = new PgUpload(LINCASH_CONF_PATH);
    
    er_con = pg_upload->er_con;
    
    er_log = new ErStrLog;
    
    if( er_con != 0)   {
	er_log->AddError(7, 0);
    }
   

    
    ware_params = NULL;
    barcode_params = NULL;
    group_params = NULL;
    user_params = NULL;
    fixreduction_params = NULL;
    autoscheme_params = NULL;
    autoreduction_params = NULL;
    taxrate_params = NULL;
    taxgroup_params = NULL;
    taxrategroup_params = NULL;

    str_number = 0;
    param_number = 0;	

}

//===========================================================================//
LoadFile::LoadFile()
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "LoadFile::LoadFile()\n");
     
  fd = NULL;
        
  file_path = NULL; 
  cur_str = NULL;
  cur_param = NULL;
  cur_command = -1;
  prev_comm = NULL;
  pg_upload = NULL;
  er_log = NULL;

// Инициализируются по необходимости перед использованием, затем удаляются
  ware_params = NULL; 
  barcode_params = NULL;
  group_params = NULL;
  user_params = NULL;
  fixreduction_params = NULL;
  autoscheme_params = NULL;
  autoreduction_params = NULL;
  taxrate_params = NULL;
  taxgroup_params = NULL;
  taxrategroup_params = NULL;
	
  str_number = 0;
  param_number = 0;
  
}

//===========================================================================//
LoadFile::~LoadFile()
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "LoadFile::~LoadFile()\n");

  if(file_path != NULL) free(file_path);
  if(cur_str != NULL) free(cur_str);
 // if(prev_comm != NULL) free(prev_comm);
  if(cur_param != NULL) free(cur_param);
  if(pg_upload != NULL) delete(pg_upload);
  if(er_log != NULL) delete(er_log);
  if(fd != NULL) fclose(fd);
}

//===========================================================================//
int LoadFile::Load()
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- 
//  1 -- файл загрузки был ранее обработан
// -1 -- произошла ошибка при чтении файла
// -2 -- команда не распознана
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::Load()\n");
  if ( OpenFile() == 0 )  {
      int res = CheckHeader();
      if ( res == 1 ) {
	  // файл загрузки был ранее обработан
	  userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Файл загрузки уже был обработан \r\n")) );
	  logfile.PrintString(log_type, INFO, "file of load was processed earlier\n");
	  return 1;          	
      }   else if ( res == 0 )  { 		// пройдена инициализация файла
	  
	   userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Запущен процесс загрузки товаров \r\n")) );
	   logfile.PrintString(log_type, INFO, "Ware load start\n");
	   userlog.SetMode(false);
	   logfile.SetMode(false);
	   
	   int res = GetNextString();
	   
	   if ( res == 0 )	  {
	      if ( strncmp(cur_str, "$$$", 3) ) {
 // После шапки нет команды, а сразу данные, по умолчанию выполняется команда, заданная в DEF_COMMAND (в load_file.h)
		  fseek(fd, 0, SEEK_SET);
		  GetNextString();
		  GetNextString();
		  strcpy(cur_str, DEF_COMMAND);
	      }
        
	      prev_comm = (char *) calloc(strlen(cur_str) + 1, sizeof(char));	
	      strcpy(prev_comm, cur_str);
	      
	      while( true )        {
		  if (res==0)  {
		      free(prev_comm);
		      prev_comm = (char *) calloc(strlen(cur_str) + 1, sizeof(char));	
		      strcpy(prev_comm, cur_str);
		      //printf("prev_comm = %s \n", prev_comm);
		      //printf("cur_str = %s \n", cur_str);
		  }
		  
		  //printf("res = %d \n", res);
		  
		  if(res == 1)  break;  // Достигнут конец файла
		  
		  if(res == -1)  {   // Ошибка чтения файла
		      er_log->AddError(6, 0, str_number);
		      free (prev_comm);
		      return -1;
		  }
		  
		  if(res == -2)   { 
		      // Надо запомнить номер неудачной строки и читать следующую
		      // Команда не поддерживается в текущей версии программы
		      er_log->AddError(3, 0, str_number);
		      strcpy(cur_str, prev_comm);	    
		  }	      
		  
		  if(res == -3)  { 
		      // Надо запомнить номер неудачной строки и читать следующую
		      // Команда не поддерживается в текущей версии программы
		      er_log->AddError(3, 0, str_number);
		      if (GetNextString() != 0)     {  
			  free (prev_comm); 
			  return -2;  
		      }  	    
		  }	    
		  
		  res = ExecuteCommand();
		  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "Execute = %d\n", res);
	      }
	      
	      ChangeLoadFeature();
	      //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "END_OF_LoadFile::Load\n");
	      userlog.SetMode(true);
	      logfile.SetMode(true);	
	      userlog.PrintString(user_log_type, INFO,WinCodec->fromUnicode(SystemCodec->toUnicode( "Загрузка товаров успешно завершена \r\n")) );
	      logfile.PrintString(log_type, INFO, "Ware load successfully complete \n");
	      userlog.SetMode(false);
	      logfile.SetMode(false);		      
	      userlog.PrintString(user_log_type, INFO, WinCodec->fromUnicode(SystemCodec->toUnicode("Всего ошибок : %d\r\n")), er_log->GetErrCount() );
	      logfile.PrintString(log_type, INFO, "Errors : %d \n", er_log->GetErrCount() );    
	      userlog.SetMode(true);	
	      logfile.SetMode(true);	
	      free (prev_comm);
	      return 0;
	  }
      }
  }
  return -1;
}

//#################################################################################################################//
//===========================================================================//
int LoadFile::ChangeLoadFeature() //Изменяет признак загрузки
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::ChangeLoadFeature()\n");
  fseek(fd, 0, SEEK_SET);
  GetNextString();
  char ch = '@';
//  char ch = '#';
  fwrite(&ch, 1, 1, fd);
  return 0;
}

//===========================================================================//
// 1- файл был загружен давно
// 0 - все ок можно выгружать
// 2 - это не загрузочный файл
// -1 - ошибка при чтении
int LoadFile::CheckHeader() // Просмотр заголовка файла
{
 // pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::CheckHeader()\n");

  int ret = 0;

  fseek(fd, 0, SEEK_SET); // позицианируем курсор на начале файла

  if(GetNextString() == 0) // Читаем первую строку
  {		
    if (!strcmp(cur_str, "##@@&&")) // строка инициализации файла проверена
    {
      if(GetNextString() == 0) // Читаем вторую строку
      {
        if(strlen(cur_str) == 1) // признак загрузки файла
          if(*cur_str == '#') // файл еще не был загружен
            ret = 0;
          else if(*cur_str == '@') // файл уже успешно загружен
            ret = 1;
      }
    }
    else ret = 2; //это не загрузочный файл
  }
  else ret = -1; //файл пустой или произошла ошибка при чтении
    //Не удалось прочитать строку или достигнут конец файла
  return ret;
}

//#################################################################################################################//

//===========================================================================//
int LoadFile::ExecuteCommand()
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, полученна следующая команда
//  1 -- команда выполнена успешно, достигнут конец файла
// -1 -- произошла ошибка при чтении файла
// -2 -- команда не распознана
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::ExecuteCommand()\n");
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "command = %s\n", cur_str);
  if(!strcmp(cur_str, "$$$ADDQUANTITY"))
  {
    cur_command = 1;
    return QuantityCommands(); 
  }
  if(!strcmp(cur_str, "$$$REPLACEQUANTITY")) 
  {
    cur_command = 2;
    return QuantityCommands();
  }
  if (!strcmp(cur_str,"$$$REPLACEQUANTITYWITHOUTSALE")) 
  {
    cur_command = 3;
    return QuantityCommands();
  }
  if (!strcmp(cur_str,"$$$DELETEALLWARES"))
  {
    cur_command = 4;
    return DeleteAllWaresGroups();
  }
  if (!strcmp(cur_str,"$$$DELETEWARESBYWARECODE")) 
  {
    cur_command = 5;
    return DeleteWaresGroupsByCode();
  }
  if (!strcmp(cur_str,"$$$ADDUSERS"))
  {
    cur_command = 6;
    return AddUsers();
  }
  if (!strcmp(cur_str,"$$$DELETEALLUSERS"))
  {
    cur_command = 7;
    return DeleteCommands();
  }
  if (!strcmp(cur_str,"$$$ADDFIXDISCS")) 
  {
    cur_command = 8;
    return AddFixReductions();
  }
  if (!strcmp(cur_str,"$$$DELETEALLFIXDISCS")) 
  {
    cur_command = 9;
    return DeleteCommands();
  }
  if (!strcmp(cur_str,"$$$ADDAUTODISCSCHMS")) 
  {
    cur_command = 10;
    return AddAutoReductionSchemes();
  }
  if (!strcmp(cur_str,"$$$DELETEALLAUTODISCSCHMS")) 
  {
    cur_command = 11;
    return DeleteCommands();
  }
  if (!strcmp(cur_str,"$$$ADDAUTODISCCONDS"))
  {
    cur_command = 12;
    return AddAutoReductions();
  }
  if (!strcmp(cur_str,"$$$DELETEALLAUTODISCCONDS"))
  {
    cur_command = 13;
    return DeleteCommands();
  }
  if (!strcmp(cur_str,"$$$ADDTAXRATES"))
  {
    cur_command = 14;
    return AddTaxRates();
  }
  if (!strcmp(cur_str,"$$$DELETEALLTAXRATES"))
  {
    cur_command = 15;
    return DeleteCommands();
  }
  if (!strcmp(cur_str,"$$$ADDTAXGROUPS"))
  {
    cur_command = 16;
    return AddTaxGroups();
  }
  if (!strcmp(cur_str,"$$$DELETEALLTAXGROUPS"))
  {
    cur_command = 17;
    return DeleteCommands();
  }
  if (!strcmp(cur_str,"$$$ADDTAXGROUPRATES"))
  {
    cur_command = 18;
    return AddTaxRatesGroups();
  }
  if (!strcmp(cur_str,"$$$DELETEALLTAXGROUPRATES"))
  {
    cur_command = 19;
    return DeleteCommands();
  }
  if (!strcmp(cur_str,"$$$ADDBARCODES"))
  {
    cur_command = 20;
    return AddBarcodes();
  }
  if (!strcmp(cur_str,"$$$DELETEALLBARCODES"))
  {
    cur_command = 21;
    return DeleteAllBarcodes();
  }

      userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("При обработке файла загрузки обнаружена неизвестная команда\r\n"))  );
    logfile.PrintString(log_type, ERROR, "command not found\n");
  return -3; 
  // неизвестная команда
}

//#################################################################################################################//
//===========================================================================//
int LoadFile::AddBarcodes() 
{
    //puts("int LoadFile::AddBarcodes() ");
    int res;
    while ( true )    {
      res = GetNextString();
      if(res != 0)  return res;  //Конец файла (1) или ошибка чтения(-1)
      else    {
	  if(!strncmp(cur_str, "$$$", 3))  return 0;  // Получена новая команда    
	  AddBarcodesCommands();       }
  }
  return 0;
}
//===========================================================================//
void LoadFile::AddBarcodesCommands()
{
    //puts("void LoadFile::AddBarcodesCommands()");
    barcode_params = new BarcodeParameters;

    int res = GetBarcodeDataParam();
  
    if ( res == 1 )   er_log->AddError(1, 1, str_number, param_number);      // не верный параметр-идентификатор
    else if ( res == -1 )   er_log->AddError(5, 1, str_number);    // не верный формат строки
    else if ( res == 0 )  {
         res = pg_upload->Insert_Barcode(barcode_params->internal_id, barcode_params->barcode, barcode_params->coefficient);
         if ( res <  0 )    er_log->AddError(4, 17, str_number);
     }
    delete(barcode_params);
}
//===========================================================================//
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, получены параметры
//  1 -- не верно заполнен идентификатор  товара
// -1 -- не верный формат строки
int LoadFile::GetBarcodeDataParam()
{
  //puts("int LoadFile::GetBarcodeDataParam()");  
  char * head = (char *) calloc(strlen(cur_str) + 1, sizeof(char));
  strcpy(head, cur_str);
  param_number = 0;
  int ret = 0;
  while(param_number < 4)  {
       switch(param_number)      {
//параместры, значения которых нам необходимы
             case 0 : case 1 : case 3 :
	if((ret = GetNextParam(head, true)) == 0)
	barcode_params->AddBarcodeParameters(param_number, cur_param);
             break;
//пока не нужные для нас параметры
            default :
	int s_ret = GetNextParam(head, false);
       }
       if(barcode_params->error_param) ret = 1; 	// Параметр-Идентификатор заполнен не правильно.
       if(ret != 0) break;
  }
  free(head);
  //printf("param number: %d\n",param_number);  
  return ret;
}
//===========================================================================//
int LoadFile::DeleteAllBarcodes()
{
      //puts("int LoadFile::DeleteAllBarcodes()");
      int res;

      res = pg_upload->Delete_All_Barcodes();
      if(res != 0)
	  er_log->AddError(4, 9, str_number);

      res = GetNextString();
      if(res != 0)  return res;     //Конец файла (1) или ошибка чтения(-1) 
      else      {
	  if(!strncmp(cur_str, "$$$", 3)) return 0;   // Полученна новая команда
	  else  {
	      er_log->AddError(3, 9, str_number);
	      return -3;	  }
      }
}

//===========================================================================//
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, полученна следующая команда
//  1 -- команда выполнена успешно, достигнут конец файла
// -1 -- произошла ошибка при чтении файла
// -2 -- ошибка при определинии группа/товар
int LoadFile::QuantityCommands()
{
    //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::QuantityCommands()\n");
    
    int res;
    while ( true )    {
	res = GetNextString();
	/*puts("+++++++++++++++++++");
	printf("cur_str = %s \n", cur_str);
	puts("+++++++++++++++++++");*/
	
	//Конец файла (1) или ошибка чтения(-1)
	if ( res != 0 )	return res;

	if ( !strncmp(cur_str, "$$$", 3) )  return 0;   // Получена новая команда
	
	bool wg_flag = true;
	int k = GetGroupFlag(&wg_flag);	
//	printf("GetGroupFlag(&wg_flag) return = %d \n", k);
	if ( k != 0 )  return -2;
	
	//printf("k = %d \n", k);
	
	/*if ( wg_flag ) puts("wg_flag == trur");
	else puts("wg_flag == false");  */
	
	if (wg_flag)  GroupQuantityCommands();      // Ведем разбор для группы
	else  WareQuantityCommands();   	         // Ведем разбор для товара

    }
    return 0;
}
//===========================================================================//
void LoadFile::WareQuantityCommands()
{
    //puts("void LoadFile::WareQuantityCommands()");
    ware_params = new WareParameters;
    
   //puts("-*------open-----");
    //puts("GetWareDataParam");
    int res = GetWareDataParam();
    //printf("GetWareDataParam return = %d \n", res);
    //puts("------close -----");
   
    if ( res == 1 ) {
	// не верный параметр-идентификатор
	er_log->AddError(1, 1, str_number, param_number);
	delete (ware_params);
	return;
    }
    if ( res == -1 ) {
	// не верный формат строки
	er_log->AddError(5, 1, str_number);
	delete(ware_params);
	return;
    }
    

    /*printf("internal_id = %s \n", ware_params->internal_code);
    printf("bar_code = %s \n", ware_params->bar_code);
    printf("longtitle = %s \n", ware_params->longtitle);
    printf("shortcut = %s \n",  ware_params->shortcut);
    printf("price = %s  \n", ware_params->price);
    printf("quantity = %s \n", ware_params->quantity);
    printf("auto_scheme_id = %s \n", ware_params->auto_scheme_id);
    printf("flags = %s \n", ware_params->flags);
    printf("min_price = %s \n", ware_params->min_price);
    
    
    printf("cur_command = %d \n", cur_command);*/
    
    
    switch(cur_command)    {
    case 1 :
	res = pg_upload->AddWare(ware_params->internal_code, ware_params->bar_code, ware_params->longtitle, ware_params->shortcut, ware_params->price, ware_params->quantity, ware_params->auto_scheme_id, ware_params->flags, ware_params->min_price, ware_params->best_before, ware_params->coefficient, ware_params->group_code, ware_params->tax_group_id);
	break;
    case 2 :
	res = pg_upload->ReplaceWare(ware_params->internal_code, ware_params->bar_code, ware_params->longtitle, ware_params->shortcut, ware_params->price, ware_params->quantity, ware_params->auto_scheme_id, ware_params->flags, ware_params->min_price, ware_params->best_before, ware_params->coefficient, ware_params->group_code, ware_params->tax_group_id, true);
      break;
    case 3 :
	res = pg_upload->ReplaceWare(ware_params->internal_code, ware_params->bar_code, ware_params->longtitle, ware_params->shortcut, ware_params->price, ware_params->quantity, ware_params->auto_scheme_id, ware_params->flags, ware_params->min_price, ware_params->best_before, ware_params->coefficient, ware_params->group_code, ware_params->tax_group_id, false);
	break;
    }
    
    //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "res=[%d]========\n",res);
    if ( res < 0 )	er_log->AddError(4, 1, str_number);
    
    delete(ware_params);
}

//===========================================================================//
void LoadFile::GroupQuantityCommands()
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "void LoadFile::GroupQuantityCommands()\n");
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "GROUP\n");
  group_params = new GroupParameters;
  
  //puts("-----open-----");
  int res = GetGroupDataParam();
  //puts("------closed----");
  
  if(res == 1) // не верный параметр-идентификатор
    er_log->AddError(1, 1, str_number, param_number);
  else if(res == -1) // не верный формат строки
    er_log->AddError(5, 1, str_number);
  else if(res == 0)
  {  
//      puts("--------------------------------------5555555555555-----------------------------------------------");
//      printf("code = %s \n longtitle = %s \n shortcut = %s \n group_code = %s \n", group_params->code, group_params->longtitle, group_params->shortcut, group_params->group_code);
    switch(cur_command)
    {
      case 1 :
        res = pg_upload->ReplaceGroup(group_params->code, group_params->longtitle, group_params->shortcut, group_params->group_code);
        break;
      case 2 :
        res = pg_upload->ReplaceGroup(group_params->code, group_params->longtitle, group_params->shortcut, group_params->group_code);
        break;
      case 3 :
        res = pg_upload->ReplaceGroup(group_params->code, group_params->longtitle, group_params->shortcut, group_params->group_code);
        break;
    }
    if ( res != 0 )
      er_log->AddError(4, 1, str_number);
  }
  delete(group_params);
}

//===========================================================================//
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, полученна следующая команда
//  1 -- команда выполнена успешно, достигнут конец файла
// -1 -- произошла ошибка при чтении файла
// -3 -- полученна строка не соответветствующая протоколу, ошибка данных
int LoadFile::DeleteAllWaresGroups()
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::DeleteAllWaresGroups()\n");
  int res;

  res = pg_upload->DeleteAllWaresGroups();
  if(res != 0)
    er_log->AddError(4, 9, str_number);

  res = GetNextString();
  if(res != 0) //Конец файла (1) или ошибка чтения(-1)
    return res;
  else
  {
    if(!strncmp(cur_str, "$$$", 3)) // Полученна новая команда
      return 0;
    else
    {
      er_log->AddError(3, 9, str_number);
      return -3;
    }
  }
//  return 0;
}

//===========================================================================//
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, полученна следующая команда
//  1 -- команда выполнена успешно, достигнут конец файла
// -1 -- произошла ошибка при чтении файла
// -3 -- ошибка данных, данные не соответствуют протоколу
int LoadFile::DeleteWaresGroupsByCode()
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::DeleteAllWaresGroupsByCode()\n");
  int res;
  while(1 == 1)
  {
    res = GetNextString();
    if(res != 0) //Конец файла (1) или ошибка чтения(-1)
      return res;
    else
    {
      if(!strncmp(cur_str, "$$$", 3)) // Полученна новая команда
        return 0;
      if(!strcmp(cur_str, "") || !strcmp(cur_str, "0")) 
      {
        er_log->AddError(3, 9, str_number);
        return -3;
      }
      char * code = (char *) calloc(strlen(cur_str), sizeof(char));
      strncpy(code, cur_str, strlen(cur_str) - 1);

      res = pg_upload->DeleteWaresGroupsByCode(code);
      if(res != 0)        er_log->AddError(4, 9, str_number);
	
      free(code);
    }
  }
  return 0;
}

//===========================================================================//
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, полученна следующая команда
//  1 -- команда выполнена успешно, достигнут конец файла
// -1 -- произошла ошибка при чтении файла
int LoadFile::AddUsers()
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::AddUsers()\n");
  int res;
  while(1 == 1)
  {
    res = GetNextString();
    if(res != 0) //Конец файла (1) или ошибка чтения(-1)
      return res;
	 else
    {
      if(!strncmp(cur_str, "$$$", 3)) // Полученна новая команда
        return 0;

      user_params = new	UserParameters;
//pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "%s\n", cur_str);
      int res = GetUserDataParam();
      if(res == 1) // не верный параметр-идентификатор
        er_log->AddError(1, 2, str_number, param_number);
      else if(res == -1) // не верный формат строки
        er_log->AddError(5, 2, str_number);
      else if(res == 0)
      {
        res = pg_upload->AddUser(user_params->code, user_params->longtitle, user_params->shortcut, user_params->profile_code, user_params->password, user_params->card_code);

      if(res != 0)
        er_log->AddError(4, 2, str_number);
      }	
      delete(user_params);
    }
  }
  return 0;
}

//===========================================================================//
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, полученна следующая команда
//  1 -- команда выполнена успешно, достигнут конец файла
// -1 -- произошла ошибка при чтении файла
int LoadFile::AddFixReductions()
{
    
    int res;
    while (true)  {
	res = GetNextString();
	if ( res != 0 )      return res; //Конец файла (1) или ошибка чтения(-1)
	else    {
	    if ( !strncmp(cur_str, "$$$", 3) )         return 0;  // Полученна новая команда
	    
	    fixreduction_params = new  FixReductionParameters;
	    //printf("fixreduction_params->barcode = %s \n", fixreduction_params->barcode);
	    
	    int res = GetFixReductionDataParam();
	    
	    /*printf("fixreduction_params->barcode = %s \n", fixreduction_params->barcode);
	    printf("fixreduction_params->code = %s \n", fixreduction_params->code);
	    printf("fixreduction_params->longtitle = %s \n", fixreduction_params->longtitle);
	    printf("fixreduction_params->shortcut = %s \n", fixreduction_params->shortcut);
	    printf("fixreduction_params->card_pref = %s \n", fixreduction_params->card_prefix);
	    printf("fixreduction_params->type = %s \n", fixreduction_params->type);
	    printf("fixreduction_params->value = %s \n", fixreduction_params->value);
	    printf("fixreduction_params->area = %s \n", fixreduction_params->area);
	    printf("fixreduction_params->show = %s \n", fixreduction_params->show);*/
	    
	    if ( res == 1 )	er_log->AddError(1, 3, str_number, param_number);	// не верный параметр-идентификатор
	    else if ( res == -1 )       er_log->AddError(5, 3, str_number);	 // не верный формат строки
	    else if ( res == 0 )      {
		res = pg_upload->AddFixReduction(fixreduction_params->code, fixreduction_params->longtitle, fixreduction_params->shortcut, fixreduction_params->type, fixreduction_params->value, fixreduction_params->area, fixreduction_params->card_prefix, fixreduction_params->barcode, fixreduction_params->show);
		//printf("res = %d \n", res);
		
		if ( res != 0 )          er_log->AddError(4, 3, str_number);
	    }
	    
	    delete(fixreduction_params);
	}
    }
    return 0;
}

//===========================================================================//
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, полученна следующая команда
//  1 -- команда выполнена успешно, достигнут конец файла
// -1 -- произошла ошибка при чтении файла
int LoadFile::AddAutoReductionSchemes()
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::AddAutoReductionsSchemes()\n");
  int res;
  while(1 == 1)
  {
    res = GetNextString();
    if(res != 0) //Конец файла (1) или ошибка чтения(-1)
      return res;
	 else
    {
      if(!strncmp(cur_str, "$$$", 3)) // Полученна новая команда
        return 0;

      autoscheme_params = new	AutoSchemeParameters;

      int res = GetAutoReductionSchemeDataParam();
      if(res == 1) // не верный параметр-идентификатор
        er_log->AddError(1, 4, str_number, param_number);
      else if(res == -1) // не верный формат строки
        er_log->AddError(5, 4, str_number);
      else if(res == 0)
      {
        res = pg_upload->AddAutoReductionScheme(autoscheme_params->code, autoscheme_params->longtitle, autoscheme_params->shortcut);
        if(res != 0)
          er_log->AddError(4, 4, str_number);
      }
      delete(autoscheme_params);
    }
  }
  return 0;
}

//===========================================================================//
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, полученна следующая команда
//  1 -- команда выполнена успешно, достигнут конец файла
// -1 -- произошла ошибка при чтении файла
int LoadFile::AddAutoReductions()
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::AddAutoReductions()\n");
  int res;
  while(1 == 1)
  {
    res = GetNextString();
    if(res != 0) //Конец файла (1) или ошибка чтения(-1)
      return res;
	 else
    {
      if(!strncmp(cur_str, "$$$", 3)) // Полученна новая команда
        return 0;

      autoreduction_params = new	AutoReductionParameters;

     int res = GetAutoReductionDataParam();
      if(res == 1) // не верный параметр-идентификатор
        er_log->AddError(1, 5, str_number, param_number);
      else if(res == -1) // не верный формат строки
        er_log->AddError(5, 5, str_number);
      else if(res == 0)
      {
        res = pg_upload->AddAutoReduction(autoreduction_params->scheme_code, autoreduction_params->code, autoreduction_params->longtitle, autoreduction_params->shortcut, autoreduction_params->type, autoreduction_params->value, autoreduction_params->begin_date, autoreduction_params->end_date, autoreduction_params->begin_time, autoreduction_params->end_time, autoreduction_params->weekdays, autoreduction_params->quantity, autoreduction_params->position_sum, autoreduction_params->check_sum, autoreduction_params->card_prefix_begin, autoreduction_params->card_prefix_end);
        if(res != 0)
          er_log->AddError(4, 5, str_number);
      }
      delete(autoreduction_params);
    }
  }
  return 0;
}

//===========================================================================//
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, полученна следующая команда
//  1 -- команда выполнена успешно, достигнут конец файла
// -1 -- произошла ошибка при чтении файла
int LoadFile::AddTaxRates()
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::AddTaxRates()\n");
  int res;
  while(1 == 1)
  {
    res = GetNextString();
    if(res != 0) //Конец файла (1) или ошибка чтения(-1)
      return res;
	 else
    {
      if(!strncmp(cur_str, "$$$", 3)) // Полученна новая команда
        return 0;

      taxrate_params = new TaxRateParameters;

      int res = GetTaxRateDataParam();
      if(res == 1) // не верный параметр-идентификатор
        er_log->AddError(1, 6, str_number, param_number);
      if(res == 2) // нет поддержки суммовых налогов
        er_log->AddError(2, 6, str_number, param_number);
      else if(res == -1) // не верный формат строки
        er_log->AddError(5, 6, str_number);
      else if(res == 0)
      {
        res = pg_upload->AddTaxRates(taxrate_params->code, taxrate_params->longtitle, taxrate_params->shortcut, taxrate_params->value);
        if(res != 0)
          er_log->AddError(4, 6, str_number);
      }
      delete(taxrate_params);
    }
  }
  return 0;
}

//===========================================================================//
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, полученна следующая команда
//  1 -- команда выполнена успешно, достигнут конец файла
// -1 -- произошла ошибка при чтении файла
int LoadFile::AddTaxGroups()
{
 // pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::AddTaxGroups()\n");
  int res;
  while(1 == 1)
  {
    res = GetNextString();
    if(res != 0) //Конец файла (1) или ошибка чтения(-1)
      return res;
	 else
    {
      if(!strncmp(cur_str, "$$$", 3)) // Полученна новая команда
        return 0;

      taxgroup_params = new TaxGroupParameters;

      int res = GetTaxGroupDataParam();
      if(res == 1) // не верный параметр-идентификатор
        er_log->AddError(1, 7, str_number, param_number);
      else if(res == -1) // не верный формат строки
        er_log->AddError(5, 7, str_number);
      else if(res == 0)
      {
        res = pg_upload->AddTaxGroups(taxgroup_params->code, taxgroup_params->longtitle, taxgroup_params->shortcut);
        if(res != 0)
          er_log->AddError(4, 7, str_number);
      }
      delete(taxgroup_params);
    }
  }
  return 0;
}

//===========================================================================//
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, полученна следующая команда
//  1 -- команда выполнена успешно, достигнут конец файла
// -1 -- произошла ошибка при чтении файла
int LoadFile::AddTaxRatesGroups()
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::AddTaxRatesGroups()\n");
  int res;
  while(1 == 1)
  {
    res = GetNextString();
    if(res != 0) //Конец файла (1) или ошибка чтения(-1)
      return res;
	 else
    {
      if(!strncmp(cur_str, "$$$", 3)) // Полученна новая команда
        return 0;

      taxrategroup_params = new TaxRateGroupParameters;

      int res = GetTaxRateGroupDataParam();
      if(res == 1) // не верный параметр-идентификатор
        er_log->AddError(1, 8, str_number, param_number);
      else if(res == -1) // не верный формат строки
        er_log->AddError(5, 8, str_number);
      else if(res == 0)
      {
        res = pg_upload->AddTaxRatesGroups(taxrategroup_params->code, taxrategroup_params->group_code, taxrategroup_params->rate_code, taxrategroup_params->base_flag);
        if(res != 0)
          er_log->AddError(4, 8, str_number);
      }
      delete(taxrategroup_params);
    }
  }
  return 0;
}

//===========================================================================//
int LoadFile::DeleteCommands()
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, полученна следующая команда
//  1 -- команда выполнена успешно, достигнут конец файла
// -1 -- произошла ошибка при чтении файла
// -3 -- ошибка данных, данные не соответствуют протоколу
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::DeleteCommands()\n");

  int res = 0;
  int com_t = 0;

  switch(cur_command)
  {
    case 7 :
      com_t = 10;
      res = pg_upload->DeleteFromTable(1);
      break;
    case 9 :
      com_t = 11;
      res = pg_upload->DeleteFromTable(2);
      break;
    case 11 :
      com_t = 12;
      res = pg_upload->DeleteFromTable(3);
      break;
    case 13 :
      com_t = 13;
      res = pg_upload->DeleteFromTable(4);
      break;
    case 15 :
      com_t = 14;
      res = pg_upload->DeleteFromTable(5);
      break;
    case 17 :
      com_t = 15;
      res = pg_upload->DeleteFromTable(6);
      break;
    case 19 :
      com_t = 16;
      res = pg_upload->DeleteFromTable(7);
      break;
  }
  if(res != 0)
    er_log->AddError(4, com_t, str_number);

  res = GetNextString();
  if(res != 0) //Конец файла (1) или ошибка чтения(-1)
    return res;
  else
  {
    if(!strncmp(cur_str, "$$$", 3)) // Полученна новая команда
      return 0;
    else
    {
      er_log->AddError(3, 0, str_number);
      return -3;
    }
  }
// return 0;
}

//#################################################################################################################//

//===========================================================================//
int LoadFile::GetWareDataParam()
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, получены параметры
//  1 -- не верно заполнен идентификатор  товара
// -1 -- не верный формат строки
{
    
    //printf("int LoadFile::GetWareDataParam() \n");
    char * head = (char *) calloc(strlen(cur_str) + 1, sizeof(char));
    strcpy(head, cur_str);
    param_number = 0;
    int ret = 0;
    while(param_number < 23)  {
	switch(param_number)    {
	case 0 : case 1 : case 2 : case 3 : case 4 : case 5 : case 6 : case 7 : case 8 : case 9 : case 13 : case 15 : case 22 :
            ret = GetNextParam(head, true);
	    //printf("GetNextParam(head = %s , true) = %d \n", head, ret);
	    //printf("param_number = %d, cur_param = %s \n", param_number, cur_param);
	    if ( ret == 0 )  {
		ware_params->AddWareParameter(param_number, cur_param);
	    }
	    break;
	default :
		int s_ret = GetNextParam(head, false);
        }
	// Параметр-Идентификатор заполнен не правильно.
	if ( ware_params->error_param )      ret = 1;
	if ( ret != 0 )  break;
	
    }
    //printf("before free head = %s \n", head);
    free(head);
    //printf("param number: %d\n",param_number);  
    return ret;
}

//===========================================================================//
int LoadFile::GetGroupDataParam()
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, получены параметры
//  1 -- не верно заполнен идентификатор  товара
// -1 -- не верный формат строки или параметра
{
    //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int GetGroupDataParam()\n");
    char * head = (char *) calloc(strlen(cur_str) + 1, sizeof(char));
    strcpy(head, cur_str);
    
    //printf("head = %s \ncur_str = %s \n", head, cur_str);
    
    param_number = 0;
    int ret = 0;
    while ( param_number < 16 )  {
	
	//printf("param_number = %d \n", param_number);
	
	switch ( param_number )    {
	    //параместры, значения которых нам необходимы
	case 0 : case 2 : case 3 : case 15 :
		    if ( (ret = GetNextParam(head, true)) == 0 )
			group_params->AddGroupParameter(param_number, cur_param);
	    break;
	    //пока не нужные для нас параметры
	default :
		ret = GetNextParam(head, false);
    }
	if ( group_params->error_param ) // Параметр-Идентификатор запонен не правильно.
	    ret = 1;
	if ( ret != 0 ) break;// выходим из цикла, чистим буфер и возвращаем ret = -1 или ret = 1;
    }
    free(head);
    return ret;
}

//===========================================================================//
int LoadFile::GetUserDataParam()
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, получены параметры
//  1 -- не верно заполнен идентификатор  товара
// -1 -- не верный формат строки или параметра
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int GetUserDataParam()\n");
  char * head = (char *) calloc(strlen(cur_str) + 1, sizeof(char));
  strcpy(head, cur_str);
  param_number = 0;
  int ret = 0;
  while(param_number < 6)
  {
    if((ret = GetNextParam(head, true)) == 0) // Изменяет head, param_num и cur_param
      user_params->AddUserParameter(param_number, cur_param);
    else // выходим из цикла, чистим буфер и возвращаем ret = -1;
      break;
    if(user_params->error_param) // Параметр-Идентификатор запонен не правильно.
    {
      ret = 1;
      break;
    }
  }
  free(head);
  return ret;
}

//===========================================================================//
int LoadFile::GetFixReductionDataParam()
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, получены параметры
//  1 -- не верно заполнен идентификатор  товара
// -1 -- не верный формат строки или параметра
{
    char * head = (char *) calloc(strlen(cur_str) + 1, sizeof(char));
    strcpy(head, cur_str);
    param_number = 0;
    int ret = 0;
    while ( param_number < 11 )  {
	ret = GetNextParam(head, true);
	/*printf("get next param return %d \n", ret);
	printf("param_number = %d \n", param_number);
	printf("cur_param = %s \n", cur_param);
	printf("fixreduction_params->error_param = %d \n", fixreduction_params->error_param);*/
	if ( ret == 0 ) 
	    // Изменяет head, param_num и cur_param
	    fixreduction_params->AddFixReductionParameter(param_number, cur_param);
	else 
	    // выходим из цикла, чистим буфер и возвращаем ret = -1;
	    break;	
	if ( fixreduction_params->error_param )    {
	    // Параметр-Идентификатор запонен не правильно.
	    ret = 1;
	    break;
	}
    }
    free(head);
    return ret;
}

//===========================================================================//
int LoadFile::GetAutoReductionSchemeDataParam()
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, получены параметры
//  1 -- не верно заполнен идентификатор  товара
// -1 -- не верный формат строки или параметра
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int GetAutoReductionSchemeDataParam()\n");
  char * head = (char *) calloc(strlen(cur_str) + 1, sizeof(char));
  strcpy(head, cur_str);
  param_number = 0;
  int ret = 0;
  while(param_number < 3)
  {
    ret = GetNextParam(head, true);
    if(ret == 0) // Изменяет head, param_num и cur_param
      autoscheme_params->AddAutoSchemeParameter(param_number, cur_param);
    else // выходим из цикла, чистим буфер и возвращаем ret = -1;
      break;
    if(autoscheme_params->error_param) // Параметр-Идентификатор запонен не правильно.
    {
      ret = 1;
      break;
    }
  }
  free(head);
  return ret;
}
//===========================================================================//
int LoadFile::GetAutoReductionDataParam()
	// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, получены параметры
//  1 -- не верно заполнен идентификатор  товара
// -1 -- не верный формат строки или параметра
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int GetAutoReductionDataParam()\n");
  char * head = (char *) calloc(strlen(cur_str) + 1, sizeof(char));
  strcpy(head, cur_str);
  param_number = 0;
  int ret = 0;
  while(param_number < 26)
  {
    switch(param_number)
    {
//параместры, значения которых нам необходимы
      case 0 : case 1 : case 2 : case 3 : case 4 : case 5 : case 6 : case 7 : case 9 : case 10 : case 12 : case 13 : case 15 : case 18 : case 21 : case 24 : case 25 :
        ret = GetNextParam(head, true);
        if(ret == 0) // Изменяет head, param_num и cur_param
          autoreduction_params->AddAutoReductionParameter(param_number, cur_param);
        break;
//пока не нужные для нас параметры
      default :
        ret = GetNextParam(head, false);
    }
    if(autoreduction_params->error_param) // Параметр-Идентификатор запонен не правильно.
      ret = 1;
    if(ret != 0) break;// выходим из цикла, чистим буфер и возвращаем ret = -1 или ret = 1;
  }
  free(head);
  return ret;
}

//===========================================================================//
int LoadFile::GetTaxRateDataParam()
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, получены параметры
//  1 -- не верно заполнен идентификатор  товара
//  2 -- нет поддержки суммовых скидок
// -1 -- не верный формат строки или параметра
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::GetTaxRateDataParam()\n");
  char * head = (char *) calloc(strlen(cur_str) + 1, sizeof(char));
  strcpy(head, cur_str);
  param_number = 0;
  int ret = 0;
  while(param_number < 5)
  {
    switch(param_number)
    {
//параместры, значения которых нам необходимы
      case 0 : case 1 : case 2 : case 4 :
        ret = GetNextParam(head, true); // Изменяет head, param_num и cur_param
        if(ret == 0) 
          taxrate_params->AddTaxRateParameter(param_number, cur_param);
        break;
      case 3 :
	    if((ret = GetNextParam(head, true)) == 0)
          if(!strcmp(cur_param, "1")) // Тип налога - суммовой
            // Пропускаем такую строку и выдаем ошибку - нет поддержки суммовых налогов
            ret = 2;
          else if(strcmp(cur_param, "0")) // не ноль
            ret = -3;
						
        break;
    }
    if(taxrate_params->error_param) // Параметр-Идентификатор запонен не правильно.
      ret = 1;
    if(ret != 0) break;// выходим из цикла, чистим буфер и возвращаем ret = -1 или ret =1 или ret = 2 или ret = -3;
  }
  free(head);
  return ret;
}	

//===========================================================================//
int LoadFile::GetTaxGroupDataParam()
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, получены параметры
//  1 -- не верно заполнен идентификатор  товара
// -1 -- не верный формат строки или параметра
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::GetTaxGroupDataParam()\n");
  char * head = (char *) calloc(strlen(cur_str) + 1, sizeof(char));
  strcpy(head, cur_str);
  param_number = 0;
  int ret = 0;
  while(param_number < 3)
  {
    if((ret = GetNextParam(head, true)) == 0) // Изменяет head, param_num и cur_param
      taxgroup_params->AddTaxGroupParameter(param_number, cur_param);
    else break; // выходим из цикла, чистим буфер и возвращаем ret = -1;
    if(taxgroup_params->error_param) // Параметр-Идентификатор запонен не правильно.
    {
      ret = 1;
      break;
    }
  }
  free(head);
  return ret;
}

//===========================================================================//
int LoadFile::GetTaxRateGroupDataParam()
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, получены параметры
//  1 -- не верно заполнен идентификатор  товара
// -1 -- не верный формат строки или параметра
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::GetTaxRateGroupDataParam()\n");
  char * head = (char *) calloc(strlen(cur_str) + 1, sizeof(char));
  strcpy(head, cur_str);
  param_number = 0;
  int ret = 0;
  while(param_number < 4)
  {
    if((ret = GetNextParam(head, true)) == 0) // Изменяет head, param_num и cur_param
      taxrategroup_params->AddTaxRateGroupParameter(param_number, cur_param);
    else break; // выходим из цикла, чистим буфер и возвращаем ret = -1;
    if(taxrategroup_params->error_param) // Параметр-Идентификатор запонен не правильно.
    {
      ret = 1;
      break;
    }
  }
  free(head);
  return ret;
}

//#################################################################################################################//

//===========================================================================//
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, получен флаг группа/товар
// -1 -- не найден очередной символ ' ; ', не верный формат строки
// -3 -- ошибка данных (??? нужно ли делать проверку?)
// -------------------------------------ЗНАЧЕНИЕ ПАРАМЕТРОВ:-------------------------------------------
// wg_flag = true -- строка содержит информацию о группе товаров
//wg_flag = false -- строка содержит информацию о товаре
int LoadFile::GetGroupFlag(bool *wg_flag)
{
    //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::GetGroupFlag(bool *gr_flag)\n");
    char * tail, * head;
    head = cur_str;
    
    
/*    puts("===================");
    printf("cur_str = %d\n", cur_str);
    puts("===================");*/
 
    // позиционируемся на 17 параметре
    for ( int i = 0; i < 16; i ++ )   {
	tail = strchr(head, ';');
	if ( tail == NULL ) return -1;
	head = tail + 1;
    }
    
/*    puts("===================");
    printf("head = %d\n", head);
    puts("===================");*/
    
    if ( *head == '0' ) { *wg_flag = true;  return 0;  }
    if ( *head == '1' ) { *wg_flag = false;  return 0;  }   
    
    return -3;
    
}

//===========================================================================//
// ------------------------ВАРИАНТЫ ВОЗВРАЩАЕМЫХ ЗНАЧЕНИЙ:------------------------------
//  0 -- команда выполнена успешно, получен очередной параметр
// -1 -- не найден очередной символ ' ; ', не верный формат с троки
int LoadFile::GetNextParam(char *head, bool fl)
{
    //printf("GetNextParam(head = %s )\n", head);
    char *token, *tail;
    int plen; // Длина строки содержащей параметер
    
    *cur_param = '\0'; // Очищаем данные о старом параметре
    
    tail = strchr(head, ';');
    param_number ++;
    if ( tail != NULL )  {
	//    param_number ++;
	if ( fl )    {
	    plen = strlen(head) - strlen(tail);
	    
	    token = (char *) calloc(plen + 1, sizeof(char));
	    strncpy(token, head, plen);
	    int k = 0;
	    	
	    for ( int i = 0; i < plen; i++ )      {
		if (k > MAX_PARAM_LEN - 3 )  {
		    //слишком большая длина параметра
		    logfile.PrintString(log_type, ERROR, "Too large parameters length. str_number = %d \n", str_number);
		    userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Слишком большая длина параметра. Ошибка в строке - %d\r\n")), str_number);
		    //er_log->AddError(1,1,str_number);
		    free(token);
		    return -1;
		}
		
		if(*(token + i) == 92) 		{
		    // встретился символ ' \'   - маскируем его символом ' \'
		    *(cur_param + k) = '/'; //попробуем просто заменить на обычный слеш
		}  else if ( *(token + i) == 39 )        {
		     // встретился символ ' ' '  - маскируем его символом ' \' 
		    *(cur_param + k) = 39;
		    *(cur_param + k + 1) = 39;
		    k += 2;
		}  else if ( *(token + i) == -92 )         {
		    // символ A4h, который заменяет ';', обратно заменяется на символ ';'
		    *(cur_param + k) = 59;
		    k++;
		}        else        {
		    *(cur_param + k) = *(token + i);
		    k++;
		}
	    }
	    *(cur_param + k) = '\0';
	    free(token);
	}
	tail = tail + 1;
	strcpy(head, tail);
	return 0;
    }
    *cur_param = '\0';
    return 0;
    //  return -1;
}

//===========================================================================//
int LoadFile::GetNextString()
//  0 -- команда выполнена успешно, полученна следующая строка
//  1 -- команда выполнена успешно, достигнут конец файла
// -1 -- произошла ошибка при чтении файла
{
    //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::GetNextString()\n");
    
    *cur_str = '\0'; // сбрасываем предыдущую строку
    char * ptr_str = fgets(cur_str, MAX_STR_LEN, fd);
    if ( ptr_str != NULL )  {
	str_number ++;
	int len = strlen(cur_str);
	char ch = *(cur_str + len - 1);
	if((ch == CR) || (ch == LF)) *(cur_str + len - 1) = '\0';
	ch = *(cur_str + len - 2);
	if((ch == CR) || (ch == LF)) *(cur_str + len - 2) = '\0';
	//pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "cur_str = <%s>\n", cur_str);
    }  else  {
	if ( feof(fd) != 0 )    {
	    //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "end_of_file\n");
	    return 1;
	}  else if(ferror(fd) != 0)    {
	    //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, ERROR, "Write error\n");
	    userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Ошибка чтения файла загрузки\r\n")) );
	    logfile.PrintString(log_type, ERROR, "error open of file upload...\n");
	    return -1;
	}
    }
    return 0;
}

//===========================================================================//
int LoadFile::OpenFile()
{
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "int LoadFile::OpenFile()\n");   
  //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, INFO, "file_path : <%s>\n", file_path);
  fd =  fopen(file_path,"r+t");
  if(fd == NULL)  {
    //pgsql_onscreanlog.PrintString(ONSCREAN_DEBUG_LEVEL, ERROR, "ошибка при открытии файла загрузки\n");
    userlog.PrintString(user_log_type, ERROR, WinCodec->fromUnicode(SystemCodec->toUnicode("Ошибка открытия файла загрузки\r\n")) );
    logfile.PrintString(log_type, ERROR, "error open of file upload...\n");
    return -1;
  }
  return 0;
}
//===========================================================================//


