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

#include "cdsp.h"
#include "setup_param.h"

#include "const.h"
#include "qtextcodec.h"
#include "qstring.h"

extern SP sp;

QTextCodec * hwcodec = QTextCodec::codecForName(HWCODEC);
QTextCodec * syscodec = QTextCodec::codecForName(SYSCODEC);
QTextCodec * dbcodec = QTextCodec::codecForName(DBCODEC);
	
CDSP::CDSP()
{
  constr = -1;
}

/*PUBLIC CONSTRUCTORS*/
CDSP::CDSP(HWSRV * hw_ref) : hw(hw_ref)
{ // Конструктор для вывода произвольных строк
  constr = 1;
}

//=====================================================================//
CDSP::CDSP(const char * name_str, const char * price_str, const char * quant_str, const char * sum_str, const char * total_str, HWSRV * hw_ref) : hw(hw_ref)
{//Конструктор для вывода информации о позиции
  constr = 2;
  name = name_str;
  price = price_str;
  quant = quant_str;
  sum = sum_str;
  total = total_str;
  
  total_label = hwcodec->fromUnicode(syscodec->toUnicode("ИТОГО:"));
  price_label = hwcodec->fromUnicode(syscodec->toUnicode("Ц."));
  quant_label = hwcodec->fromUnicode(syscodec->toUnicode("К."));
  
  //printf("Cdsp creator 2");
  //cout << "name - " << name << endl;
}


//=====================================================================//
CDSP::CDSP(const char * name_str, const char * sum_str, const char * red_label, HWSRV * hw_ref) : hw(hw_ref)
{// Конструктор для вывода информации о скидках/надбавках
  constr = 3;
  name = name_str;
  sum = sum_str;
  reduct_label = red_label;
  //cout << "name = "<< name << endl;
  //cout << "reduct label = "<< reduct_label << endl;
}


//=====================================================================//
CDSP::CDSP(const char * total_str, HWSRV * hw_ref) : hw(hw_ref)
{
  constr = 4;
  total = total_str;
  total_label = hwcodec->fromUnicode(syscodec->toUnicode("ИТОГО:"));
  text = hwcodec->fromUnicode(syscodec->toUnicode("Позиция отменена"));
  
}

//=====================================================================//
void CDSP::CustomerDisplay(const char * up_str, const char * down_str, int up_run_mode, int down_run_mode)
{    
    if ( constr == 1 ) {
	string empty_str;
	if ( empty_str == up_str) 
	    hw->DSP_Up(" ",up_run_mode);
	else 
	    hw->DSP_Up(up_str, up_run_mode);
	if ( empty_str == down_str)	
	    hw->DSP_Down(" ",down_run_mode);
	else	
	    hw->DSP_Down(down_str, down_run_mode);
    }
}

//=====================================================================//
/*void CDSP::CustomerDisplay(const char * up_str, const char * down_str)
{
  //  puts("void CDSP::CustomerDisplay(const char * up_str, const char * down_str)");
  //  printf("up_str = |%s|\n",up_str);
  //  printf("down_str = |%s|\n",down_str);
    if(constr == 1)
    {
	string empty_str;
	if ( empty_str == up_str)
	    hw->DSP_Up(" ",0);
	else 
	     hw->DSP_Up(up_str,0);
	
	if ( empty_str==down_str)
	    hw->DSP_Down(" ",0);
	else
	    hw->DSP_Down(down_str,0);
    }
}*/
//=====================================================================//
void CDSP::StornoCustomerDisplay()
{
 // puts("void CDSP::StornoCustomerDisplay()");
  if(constr == 4)
  {
    string upstr = "";
    string downstr = "";
    if(sp.cdsp_detalize_product_total_string == 0)
      if(sp.cdsp_detalize_product_total)
      {
        upstr += total_label + ' ' + total;
        downstr += text;
      }
      else upstr += text;
    else
      if(sp.cdsp_detalize_product_total)
      {
        downstr += total_label + ' ' + total;
        upstr += text;
      }
      else upstr += text;    
    hw->DSP_Up(upstr.c_str(), sp.cdsp_up_detalize_product_mode_run);
    hw->DSP_Down(downstr.c_str(), sp.cdsp_down_detalize_product_mode_run);
  }
}

//=====================================================================//
void CDSP::RegistrationPositionCustomerDisplay()
{
    //puts("void CDSP::RegistrationPositionCustomerDisplay()");
    //cout << "constr - " << constr << endl;
    if(constr == 2)  {
	string upstr = RegistrationPositionUpString();
	string downstr = RegistrationPositionDownString();
	
	//cout << "upstring to display  - " << upstr << endl;
	//cout << "downstring to display - " << downstr << endl;
	//puts("--------------RegistrationPositionCustomerDisplay()--------------");
    
	//printf("hw->DSP_Up(upstr.c_str() = %s, sp.cdsp_up_detalize_product_mode_run = %d) \n", upstr.c_str(), sp.cdsp_up_detalize_product_mode_run);
	//printf("hw->DSP_Down(downstr.c_str() = %s, sp.cdsp_down_detalize_product_mode_run = %d)\n", downstr.c_str(), sp.cdsp_down_detalize_product_mode_run);
    
	hw->DSP_Up(upstr.c_str(), sp.cdsp_up_detalize_product_mode_run);
	hw->DSP_Down(downstr.c_str(), sp.cdsp_down_detalize_product_mode_run);
    }
}

//=====================================================================//
void CDSP::RegistrationReductionCustomerDisplay()
{
//  puts("void CDSP::RegistrationReductionCustomerDisplay()");
  if(constr == 3)
 {
    string upstr = RegistrationReductionUpString();
    string downstr = RegistrationReductionDownString();
  
    /*cout << upstr << endl;
    cout << downstr << endl;*/
  
    hw->DSP_Up(upstr.c_str(), sp.cdsp_up_detalize_discount_mode_run);
    hw->DSP_Down(downstr.c_str(), sp.cdsp_down_detalize_discount_mode_run);
  
  }
}

//=====================================================================//
string CDSP::RegistrationPositionUpString()
{
    
    string upstr = "";
    
    if ( sp.cdsp_detalize_product_name && sp.cdsp_detalize_product_name_string == 0 )    upstr += name + ' ';
  
    if ( sp.cdsp_detalize_product_properties_string == 0 )  {
	if ( sp.cdsp_detalize_product_price && sp.cdsp_detalize_product_quantity )      upstr  += price + 'X' + quant + ' ';
	else if ( sp.cdsp_detalize_product_price )      upstr += price_label + ' '+ price + ' ';
	else if ( sp.cdsp_detalize_product_quantity )      upstr += quant_label + ' ' + quant + ' ';
	if ( sp.cdsp_detalize_product_sum )      upstr += '=' + sum + ' ';
    }
  
  if(sp.cdsp_detalize_product_total_string == 0)
    if(sp.cdsp_detalize_product_total)
      upstr += total_label + ' ' + total;
  
  return upstr;
}

//=====================================================================//
string CDSP::RegistrationPositionDownString()
{
 // puts("string CDSP::RegistrationPositionDownString()");
  string downstr = "";

  if(sp.cdsp_detalize_product_name && sp.cdsp_detalize_product_name_string == 1)
    downstr += name + ' ';
  
  if(sp.cdsp_detalize_product_properties_string == 1)
  {
    if(sp.cdsp_detalize_product_price && sp.cdsp_detalize_product_quantity)
      downstr  += price + 'X' + quant + ' ';
    else if(sp.cdsp_detalize_product_price)
      downstr += price_label + ' ' + price + ' ';
    else if(sp.cdsp_detalize_product_quantity)
      downstr += quant_label + ' ' + quant + ' ';
    if(sp.cdsp_detalize_product_sum)
      downstr += '=' + sum + ' ';
  }
  
  if(sp.cdsp_detalize_product_total_string == 1)
    if(sp.cdsp_detalize_product_total)
      downstr += total_label + ' ' + total;
  
  return downstr;
}

//=====================================================================//
string CDSP::RegistrationReductionUpString()
{
 // puts("string CDSP::RegistrationReductionUpString()");
  string upstr = "";
  if(sp.cdsp_detalize_discount_name_string == 0)
  {
    upstr += reduct_label + ' ';

    if(sp.cdsp_detalize_discount_name)
      upstr += name + ' ';
  }

  if(sp.cdsp_detalize_discount_sum_string == 0)
    if(sp.cdsp_detalize_discount_sum)
      upstr += "=" + sum + "p";

  return upstr;
}

//=====================================================================//
string CDSP::RegistrationReductionDownString()
{
//  puts("string CDSP::RegistrationReductionDownString()");
  string downstr = "";
  
  if(sp.cdsp_detalize_discount_name_string == 1)
  {
    downstr += reduct_label + ' ';
    if(sp.cdsp_detalize_discount_name)
      downstr += name + ' ';
  }
  if(sp.cdsp_detalize_discount_sum_string == 1)
    if(sp.cdsp_detalize_discount_sum)
      downstr += sum;
  //cout << downstr << endl;
  return downstr;
}
