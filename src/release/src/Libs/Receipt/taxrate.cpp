
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


#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "addon_func.h"
#include "taxrate.h" 

//==================================================================================// 
TaxRate::TaxRate(int r_id, double r)
{
//puts("TaxRate::TaxRate(int r_id, double r)");

  taxsum = 0;
  change_base = false;
  
  rate_id = r_id;
  rate = r;
}

//==================================================================================// 
TaxRate::TaxRate(int r_id, double r, bool ch_base, const char * sh_str, const char * ln_str)
{
//puts("TaxRate::TaxRate(int r_id, double r, bool ch_base, const char * sh_str, const char * ln_str)");

  taxsum = 0;
  change_base = ch_base;
//  order = ord;
  rate_id = r_id;
  rate = r;

  shortcut = sh_str;
  longtitle = ln_str;
}

//==================================================================================// 
TaxRate::TaxRate(const TaxRate & tr)
{
//puts("TaxRate::TaxRate(const TaxRate & tr)");
  change_base = tr.change_base;

  rate_id = tr.rate_id;
  taxsum = tr.taxsum;
  rate = tr.rate;
 
  shortcut = tr.shortcut;
  longtitle = tr.longtitle;
}

//==================================================================================// 
TaxRate::~TaxRate()
{
//puts("TaxRate::~TaxRate()");
 
}

//==================================================================================// 
int TaxRate::GetLenghtShortcut()
{
//puts("int TaxRate::GetLenghtShortcut()");
  return shortcut.length();
}

//==================================================================================// 
int TaxRate::GetLenghtLongtitle()
{
//puts("int TaxRate::GetLenghtLongtitle()");
  return longtitle.length();
}

//==================================================================================// 
const char * TaxRate::GetShortcut()
{
//puts("const char * TaxRate::GetShortcut()");
  return shortcut.c_str();
}

//==================================================================================// 
const char * TaxRate::GetLongtitle()
{
//puts("const char * TaxRate::GetLongtitle()");
  return longtitle.c_str();
}

//==================================================================================// 
void TaxRate::Clear_Tax_Rate()
{
//puts("TaxRate::Clear_Tax_Rate()");
 // order = 0;
  rate_id = 0;
  rate = 0;
  shortcut = "";
  longtitle = "";
}

//==================================================================================// 

