/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2009-2011, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr aka Zuskin.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2009-2011, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/


#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "addon_func.h"
#include "discount.h" 

int base_discount_order[4] = {0, 1, 2, 3}; //Заменить потом на аналог из SP

struct less_discount_for_vector : public binary_function <Discount, Discount, bool>
{
  bool operator () (Discount d1, Discount d2)
  {
    return d1.dsum < d2.dsum;
  }
} less_dsum;

//==================================================================================//
double MaxDiscountAndCharge(double s , vector<Discount>&discount)
{
    printf("MaxDiscountAndCharge(double s = %f , discount) \n", s);
    double sum = 0;
    for ( vector<Discount>::iterator i = discount.begin(); i != discount.end(); i++ )  {
	//cout << i->CalcDiscount(s) << endl; 
	if ( i->df )      i->CalcDiscount(s);
    }
    sum = Max_Discount(s, discount) + Max_Charge(s, discount);
    
    // Round added by A.S. 2008-02-07
    sum = Round(sum * 100) / 100;
    
    return sum;
}

//==================================================================================//
double Max_Charge(double s , vector<Discount> discount)
{
  double max_charge = 0;
  
  vector<Discount>::iterator it = min_element(discount.begin(), discount.end(), less_dsum);
  max_charge = (*it).dsum;

  return max_charge;
}

//==================================================================================//
double Max_Discount(double s, vector<Discount>  discount)
{
  double max_discount =  0;

  vector<Discount>::iterator it = max_element(discount.begin(), discount.end(), less_dsum);
  max_discount = (*it).dsum;
  
  return max_discount; 
}

//==================================================================================//
double Union_Discount(double s, vector<Discount> & discount)
{
    printf("Union_Discount(double s = %f , discount) \n", s);
  double dsum = 0;

  for(int i = 0; i < DISCVECTORSIZE; i++) {
      dsum += discount.at(base_discount_order[i]).CalcDiscount(s - dsum);
      //-----(+)-----Zuskin-----12/01/2012-----
      dsum = Round(dsum * 100) / 100;
      //-------------------------------------------------
}
  
    
  return dsum;
}

//==================================================================================//
double Sum_Discount(double s, vector<Discount>& discount)
{
  printf("Sum_Discount(double s = %f , discount) \n", s);
  double dsum = 0;
  for(vector<Discount>::iterator i = discount.begin(); i != discount.end(); i++) {
    dsum += i->CalcDiscount(s);
    //-----(+)-----Zuskin-----12/01/2012-----
    dsum = Round(dsum * 100) / 100;
    //-------------------------------------------------
}
  return dsum;
}


//###############################DISCOUNT###########################################//
//==================================================================================//
Discount::Discount() : old_p(0), old_s(0), old_df(false)
{
//puts("Discount::Discount()");
  df = false;
  dsum = 0;
  percent = 0;
  
  old_p = 0;
  old_s = 0;
  old_df = 0;
  
  transaction_percent = 0;
  transaction_sum = 0;
}

//==================================================================================//
/*Discount::Discount(const Discount & d)
{
//puts("Discount::Discount(const Discount & d)");
  df = d.df;
  dsum = d.dsum;
  percent = d.percent;
  
  name = d.name;
  shortcut = d.shortcut;
  code = d.code;
  
  internal_code = d.internal_code;
  card_code = d.card_code;
  
  old_p = d.old_p;
  old_s = d.old_s;
  old_df = d.old_df;
}*/

//==================================================================================//
Discount::~Discount()
{
//puts("Discount::~Discount()");
}

//==================================================================================//
void Discount::Clear_Discount()
{
//puts("Discount::Clear_Discount()");
  df = false;
  dsum = 0;
  percent = 0;

  name = "";
  shortcut = "";
  code = "";
}
//==================================================================================//
void Discount::Set_Names(const char * name_str, const char * shortcut_str)
{
//puts("void Discount::Set_Names(const char * name_str, const char * shortcut_str)");
  name = name_str;
  shortcut = shortcut_str;
}

//==================================================================================//
void Discount::Set_Code(const char * code_str)
{
//puts("void Discount::Set_Code(const char * code_str)");
  code = code_str;
}

//==================================================================================//
const char * Discount::GetName()
{
//puts("const char * Discount::GetName()");

 return name.c_str();
}

//==================================================================================//
const char * Discount::GetShortcut()
{
//puts("const char * Discount::GetShortcut()");

 return shortcut.c_str();
}

//==================================================================================//
const char * Discount::GetCode()
{
//puts("const char * Discount::GetCode()");

 return code.c_str();
}

//==================================================================================//
int Discount::GetLengthName()
{
//puts("int Discount::GetLengthName()");
  return name.length();
}

//==================================================================================//
int Discount::GetLengthShortcut()
{
//puts("int Discount::GetLengthShortcut()");
  return shortcut.length();
}

//==================================================================================//
int Discount::GetLengthCode()
{
//puts("int Discount::GetLengthCode()");
return code.length();
}

//==================================================================================//
void Discount::Add_Discount(double s, double p)
{
//puts("void Discount::Add_Discount(double s, double p)");
  old_p = percent;
  old_s = dsum;
  old_df = df;
  
  percent = p;
  dsum = s;
  df = true;
  
}

//==================================================================================//
void Discount::Restore_Discount()
{
//puts("void Discount::Restore_Discount()");

  percent = old_p;
  dsum = old_s;
  df = old_df;

  transaction_percent = 0;
  transaction_sum = 0;
  
}

//==================================================================================//
double Discount::CalcDiscount(double s)
{
  if(df)
  {
     if(percent != 0)
        //---(*)---Zuskin---14/02/2012---
        //dsum = Round(s * percent) / 100; // округление 5% от суммы чека может дать неверный результат. Реальная сумма скидки = сумма чека - ОкрДоКопеек( сумма чека со скидкой )
        dsum = s - Round( s * ( 100 - percent) ) / 100;
        //printf("Discount::CalcDiscount(%2f) = %2f\n", s, dsum);
        //-------------------------------
  }
  return dsum;
}
//==================================================================================//


