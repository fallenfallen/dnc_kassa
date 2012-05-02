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
#include "positiontaxes.h" 


//==================================================================================// 
PositionTaxes::PositionTaxes()
{
//puts("PositionTaxes::PositionTaxes()");
  num_rates = 0;
  tax_group_id = 0;
  
//  taxrates.reserve(5);
}

//==================================================================================// 
PositionTaxes::PositionTaxes(const PositionTaxes & pt)
{
//puts("PositionTaxes::PositionTaxes(const PositionTaxes & pt)");
  num_rates = pt.num_rates;
  tax_group_id = pt.tax_group_id;
  taxrates = pt.taxrates;
  tax_group_name = pt.tax_group_name;
  
}

//==================================================================================// 
PositionTaxes::~PositionTaxes()
{
//puts("PositionTaxes::~PositionTaxes()");
}

//==================================================================================// 
TaxRate * PositionTaxes::GetTaxRatesElemRef(int index)
{
  //puts("TaxRate * PositionTaxes::GetTaxRatesElemRef(int index)");
  return &taxrates[index];
}

//==================================================================================// 
void PositionTaxes::Add_Tax_Rate(TaxRate tr)
{
//puts("void PositionTaxes::Add_Tax_Rate(TaxRate tr)");
  taxrates.push_back(tr);
}
//==================================================================================// 
const char* PositionTaxes::GetName()
{
	return tax_group_name.c_str();
}
//==================================================================================// 
void PositionTaxes::SetName(string tg_name)
{
    tax_group_name = tg_name;
}
//==================================================================================// 

