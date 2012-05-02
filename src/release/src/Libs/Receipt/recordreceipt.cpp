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
#include "recordreceipt.h" 
#include "setup_param.h"

extern SP * sp_receipt;


//==================================================================================//
Record_Receipt::Record_Receipt()
{
//puts("Record_Receipt::Record_Receipt()");
  quantity = 0;
  price = 0;
  min_price = 0;
  sum = 0;
  dsum = 0;
  
  code_flag = false;
  enable_flag = true;
  
  flags = 0;
  auto_scheme = 0;
  fix_scheme = 0;
  tax_group = 0;
  
  section = 0;
  section_number = 0;
  
  sale_limit_by_time = false;

  check_discount_sum = 0;
}


//==================================================================================//
Record_Receipt::~Record_Receipt()
{
//puts("Record_Receipt::~Record_Receipt()");

}

//==================================================================================//
Discount * Record_Receipt::GetDiscountElemRef(int index)
{
    
    //puts("Discount * Record_Receipt::GetDiscountElemRef(int index)");
  return &discount.at(index);
}

//==================================================================================//
const char * Record_Receipt::GetBarcode()
{
//puts("const char * Record_Receipt::~GetBarcode()");
  return barcode.c_str();
}

//==================================================================================//
const char * Record_Receipt::GetCode()
{
//puts("const char * Record_Receipt::~GetCode()");
  return code.c_str();
}

//==================================================================================//
const char * Record_Receipt::GetShortcut()
{
//puts("const char * Record_Receipt::~GetShortcut()");
  return shortcut.c_str();
}

//==================================================================================//
const char * Record_Receipt::GetLongtitle()
{
//puts("const char * Record_Receipt::~GetLongtitle()");
  return longtitle.c_str();
}

//==================================================================================//
int Record_Receipt::GetLenghtBarcode()
{
//puts("int Record_Receipt::~GetLenghtBarcode()");
  return barcode.length();
}

//==================================================================================//
int Record_Receipt::GetLenghtCode()
{
//puts("int Record_Receipt::~GetLenghtCode()");
  return code.length();
}

//==================================================================================//
int Record_Receipt::GetLenghtShortcut()
{
//puts("int Record_Receipt::~GetLenghtShortcut()");
  return shortcut.length();
}

//==================================================================================//
int Record_Receipt::GetLenghtLongtitle()
{
//puts("int Record_Receipt::~GetLongtitle()");
  return longtitle.length();
}
//==================================================================================//
int Record_Receipt::GetLenghtAdditionId()
{
    return addition_id.length();
}
//==================================================================================//
const char * Record_Receipt::GetAdditionId() 
{
    return addition_id.c_str();
}
//==================================================================================//
double Record_Receipt::GetPositionResultSum() 
{
    //---(*)---Zuskin---24/02/2012---
    //return sum - dsum - check_discount_sum;
    return Round((sum - dsum - check_discount_sum)*100)/100;
    //-------------------------------
}
//==================================================================================//
void Record_Receipt::Add_Record_Receipt(const char *barcode_str, const char *code_str, const char *shortcut_str, const char *longtitle_str, double quant, double pr, int fls, bool cflag, int a_sch, int f_sch, int t_gr, double m_pr, const char* addition_id_str)
{

  dsum = 0;

  barcode = barcode_str;
  code = code_str;
  shortcut = shortcut_str;
  longtitle = longtitle_str;
 
  addition_id = addition_id_str;
  
  quantity = quant;
  price = pr;
  min_price = m_pr;
  sum = pr * quant;
  
  sum = Round(sum * 100) / 100;

  code_flag = cflag;
  enable_flag = true;

  flags = fls;
  auto_scheme = a_sch;
  fix_scheme = f_sch;
  tax_group = t_gr;
  
  //---(*)---Zuskin---14/02/2012---
  //discount.resize(4);
  discount.resize(DISCVECTORSIZE);
  //-------------------------------

//printf("tax_group %d\n sum %4f\n", tax_group,sum);
//puts("END_OF_Record_Receipt::Add_Record_Receipt(...)");

}

//======================================================================//
int Record_Receipt::Add_Sum_Discount(int i, double s, const char * code, const char *name, const char *shortcut, bool manual_fl, int control_min_price)
// return :
// 1 -- скидка больше максимальной допустимой для произвольной скидки
// 2 -- надбавка больше максимальной допустимой для произвольной надбавки
// 3 -- сумма скидки больше суммы позиции
// 4 -- цена товара со скидкой меньше минимальной цена товара
{
  //puts("int Record_Receipt::Add_Sum_Discount(int i, double s, const char * code, const char *name, const char *shortcut, bool manual_fl)");
    printf("Record_Receipt::Add_Sum_Discount(discount = %f )\n", s);
  discount[i].Add_Discount(s , 0);
  RecalculationDiscounts();
  
  /*printf("discount/charge sum = %f\n", s);
  printf("max position discount in percents = %f\n", sp_receipt->max_position_discount);
  printf("max position charge in percents = %f\n", sp_receipt->max_position_increase);
  printf("max position discount sum = %f\n", Round(sum * sp_receipt->max_position_discount / 100) );
  printf("max position charge sum = %f\n", Round(sum * sp_receipt->max_position_increase / 100) );*/
  
  int ret = 0;
  if(manual_fl && (s > 0) && (Round(sum * sp_receipt->max_position_discount) / 100 < s))   //  проверка максимальной скидки для произвольных скидок
    ret = 1;
  else if(manual_fl && (s < 0) && (Round(sum * sp_receipt->max_position_increase) / 100 < - s)) // проверка максимальной надбавки для произвольных надбавок
    ret = 2;
  else if ( (control_min_price == 1) && (sum - dsum < min_price * quantity) && (!(atoi(addition_id.c_str()) > 0)) ) // котроль минимальной цены товара
    ret = 3;
  else if ( (control_min_price == 2) && (sum - dsum < min_price * quantity) && (!(atoi(addition_id.c_str()) > 0)) ) {
      discount[i].Restore_Discount();
      RecalculationDiscounts();
      
      s = sum - min_price * quantity;
      //printf("new sum discvount = %f \n", s);
      
      discount[i].Add_Discount(s , 0);
      RecalculationDiscounts();
      
  } else if(sum - dsum <= 0) // проверка суммы скидки по сравнению с суммой позиции
    ret = 4;
  

  if(ret != 0) // Востанавливаем предыдущие значения скидки и повторно пересчитываем общую скидку на позицию
  {
    discount[i].Restore_Discount();
    RecalculationDiscounts();
  }
  else
  {
    discount[i].Set_Names(name, shortcut);
    discount[i].Set_Code(code);
  }
  return ret;
}

//==================================================================================//
int Record_Receipt::Add_Percent_Discount(int i, double p, const char * code, const char *name, const char *shortcut, bool manual_fl, int control_min_price)
// 1 -- скидка больше максимальной допустимой для произвольной скидки
// 2 -- надбавка больше максимальной допустимой для произвольной надбавки
// 4 -- цена товара со скидкой меньше минимальной цена товара
{
    //puts("Add_Percent_Discount(int i, double p, const char * code, const char *name, const char *shortcut, bool manual_fl)");  
    
    printf("Record_Receipt::Add_Percent_Discount(discount p = %f )\n",p );
  discount[i].Add_Discount(0, p);
  RecalculationDiscounts();
  
  double s = discount[i].dsum;
 
  /*printf("discount/charge sum = %f\n", s);
  printf("max position discount in percents = %f\n", sp_receipt->max_position_discount);
  printf("max position charge in percents = %f\n", sp_receipt->max_position_increase);
  printf("max position discount sum = %f\n", Round(sum * sp_receipt->max_position_discount) / 100 );
  printf("max position charge sum = %f\n", Round(sum * sp_receipt->max_position_increase) / 100 );*/
  //puts("&&&&&&&&&&&********&&&&&&&&&&&&&&&&");
  //printf("control_min_price = %d \n", control_min_price);
  //if ( control_min_price == YES ) puts("YES");
  //else puts( "NO YES" );
  int ret = 0;
  if(manual_fl && (s > 0) && (Round(sum * sp_receipt->max_position_discount) / 100 < s))   //  проверка максимальной скидки
    ret = 1;
  else if(manual_fl && (s < 0) && (Round(sum * sp_receipt->max_position_increase) / 100 < - s)) // проверка максимальной надбав 
    ret = 2;
  else if ( (control_min_price == 1 ) && (sum - dsum < min_price * quantity) ) {
    //puts("control_min_price TRUE");  
    ret = 4;
  } else if ( control_min_price == 2 && (!(atoi(addition_id.c_str()) > 0)) ) {
      if (sum - dsum < min_price * quantity) {
	  discount[i].Restore_Discount();
	  RecalculationDiscounts();
	  
	  // процент
	  p = 100 - (min_price*quantity/sum)*100;
	  
	  discount[i].Add_Discount(0, p);
	  RecalculationDiscounts();
      }
  }
  
  if(ret != 0)  // Востанавливаем предыдущие значения скидки и повторно пересчитываем общую скидку на позицию
  {
    discount[i].Restore_Discount();
    RecalculationDiscounts();
  }
  else
  {
    discount[i].Set_Names(name, shortcut);
    discount[i].Set_Code(code);
  }
  return ret;
}

//==================================================================================//
double Record_Receipt::RecalculationDiscounts(double s)
{
    /*    printf("double Record_Receipt::RecalculationDiscounts(s = %f) \n", s);
    printf("sp_receipt->result_discount = %d \n", sp_receipt->result_discount);*/
    dsum = 0;
    switch ( sp_receipt->result_discount ) { // Правило выбора начисления итоговой скидки
    case 0 : // выбирается максимальная скидка и максимальная надбавка
	dsum = MaxDiscountAndCharge(s, discount);
	break;
    case 1 : // выбирается объединение скидок (скидки начисляются последовательно, используя порядок начисления)
	//puts("UNION");
	dsum = Union_Discount(s, discount);
	break;
    case 2 : // скидки суммируются, а потом начисляются
	//puts("SUM");
	dsum = Sum_Discount(s, discount);	
	break;
    }
    
    return dsum;
}
//==================================================================================//
double Record_Receipt::RecalculationDiscounts()
{
    //puts("double Record_Receipt::RecalculationDiscounts(double s)");
    
    /*printf("double Record_Receipt::RecalculationDiscounts() \n");
    printf("sp_receipt->result_discount = %d \n", sp_receipt->result_discount);*/
    
    dsum = 0;
    
    switch(sp_receipt->result_discount)  {  // Правило выбора начисления итоговой скидки
    case 0 : // выбирается максимальная скидка
	//puts("MAXIMUM");
	dsum = MaxDiscountAndCharge(sum, discount);
	
	break;
    case 1 : // выбирается объединение скидок (скидки начисляются последовательно, используя порядок начисления)
	//puts("UNION");
	dsum = Union_Discount(sum, discount);
	break;
    case 2 : // скидки суммируются, а потом начисляются
	//puts("SUM");
	dsum = Sum_Discount(sum, discount);	
	break;
    }
    
    return dsum;
}  

//==================================================================================//




