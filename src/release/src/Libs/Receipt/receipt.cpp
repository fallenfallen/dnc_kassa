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

#include "receipt.h"


#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
//#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include "addon_func.h"
#include "Section.h"

double MaxDiscountAndCharge(double s , vector<Discount>&discount);
double Max_Charge(double s , vector<Discount> discount);
double Max_Discount(double s, vector<Discount>  discount);


SP * sp_receipt;


//==================================================================================//
Receipt::Receipt(SP * sp_rec)
{
//puts("Receipt::Receipt(Sp * sp_rec)");
    
    int section_count = 5;
    for ( int i = 0 ; i < section_count; i++ ) {
	ReceiptSection s;
	s.count_position = 0;
	s.discount_sum = 0;
	s.result_section_sum = 0;
    }
    
    record_num = 0;
    receipt_num = -1;
    sum = 0;
    dsum = 0;
    
    //-----(*)-----Zuskin-----27/10/2011-----
    //discount.resize(4);
    discount.resize(DISCVECTORSIZE);
    //---------------------------------------
    
    sp_receipt = sp_rec;
    
    receipt_type = CHECK_FREE;
    receipt_number = 0;
    
    round_sum = 0;
    transaction_discount_sum = 0;
    
    section_num_with_max_sum = 0;
    
    closed = false;
}

//==================================================================================//
/*Receipt::Receipt(const Receipt & r)
{ //   int size;
	  
  closed = r.closed;
  record_num = r.record_num;
  receipt_num = r.receipt_num;
  sum = r.sum;
  dsum = r.dsum;
  date = r.date;
  cashier = r.cashier;
  
  doc_number = r.doc_number;
  kkm_serial = r.kkm_serial;
  n_session = r.n_session;	
  
  receipt_type = CHECK_FREE;
  receipt_number = 0;  
  
  discount.assign(r.discount.begin(), r.discount.end());
  records.assign(r.records.begin(), r.records.end());
  positiontaxes.assign(r.positiontaxes.begin(), r.positiontaxes.end());  
  sections.assign( r.sections.begin(), r.sections.end() );
  
  round_sum = 0;
}*/
//==================================================================================// 
Receipt::~Receipt()
{
    
}
//==================================================================================// 
void Receipt::setDate(const char* d)
{
    date = d;
}
//==================================================================================// 
const char* Receipt::getDate()
{
    return date.c_str();
}
//==================================================================================// 
ReceiptType Receipt::GetCheckType()
{
    return receipt_type;
}
//==================================================================================//    
void Receipt::SetCheckType(ReceiptType rt)
{
    receipt_type = rt;
}
//==================================================================================//    
int Receipt::GetCheckNumber()
{
    return receipt_number;
}
//==================================================================================// 
void Receipt::SetCheckNumber(int check_number)
{
    receipt_number = check_number;
}
//==================================================================================//
Discount * Receipt::GetDiscountElemRef(int index)
{
//puts("Discount * Receipt::GetDiscountElemRef(int index)");
  return &discount.at(index);
}

//==================================================================================// 
Record_Receipt * Receipt::GetPosElemRef(int index)
{
//puts("Record_Receipt * Receipt::GetPosElemRef(int index)");
  return &records.at(index);
}
    
//==================================================================================// 
PositionTaxes * Receipt::GetTaxElemRef(int index)
{
//puts("PositionTaxes * Receipt::GetTaxElemRef(int index)");
   //cout << "positiontaxes.at(index). " << positiontaxes.at(index).tax_group_id << positiontaxes.at(index).tax_group_name << endl;
 return &positiontaxes.at(index);
}

void Receipt::Clear_Receipt()
{
//puts("Receipt::Clear_Receipt()");
  if(!records.empty()) records.clear();
  if(!positiontaxes.empty()) positiontaxes.clear();
  if(!discount.empty()) discount.clear();

  record_num = 0;
  receipt_num = -1;
  sum = 0;
  dsum = 0;
  discount. resize(DISCVECTORSIZE);
  closed = false;
  transaction_discount_sum = 0;
  section_num_with_max_sum = 0;
  //---(+)---Zuskin---24/02/2012---
  round_sum = 0;
  //-------------------------------
}

//==================================================================================//
void Receipt::CloseReceipt()
{
//puts("void Receipt::CloseReceipt()");
  //-----(+)-----Zuskin-----27/10/2011-----
  for (int jj = 0; jj < DISCVECTORSIZE; jj++) 
    discount.at(jj).Clear_Discount();
  //---------------------------------------
  //---(+)---Zuskin---24/02/2012---
  record_num = 0;
  receipt_num = -1;
  sum = 0;
  dsum = 0;
  discount.resize(DISCVECTORSIZE);
  receipt_type = CHECK_FREE;
  receipt_number = 0;
  round_sum = 0;
  transaction_discount_sum = 0;
  section_num_with_max_sum = 0;
  //-------------------------------  
  closed = true;
}

//==================================================================================//
bool Receipt::GetStateofClosing()
{
//puts("bool Receipt::GetStateofClosing()");
  return closed;
}

//==================================================================================//
//flagcode = true - поиск товара по внутреннем коду, flagcode = false - поиск товара по штрих-коду  
int Receipt::FindEquilPosition(const char *code, bool flagcode, const char* addition_id)
{
    int pos = -1;
    int i = 0;
    
    //printf("atoi(addition_id) = %d \n", atoi(addition_id));
    
    if ( atoi(addition_id) > 0 ) {
	string addition_id_str = addition_id;
	while ( i < records.size() && records.at(i).GetAdditionId() != addition_id_str ) i++;
	if ( i < records.size() ) 
	    if ( records[i].enable_flag ) pos = i;
	return pos;
    }
    
    string code_string = code;
    
    i = 0;
    if ( !flagcode ) {
	//puts("find by barcode ");
	while ( i < records.size() ) {
	    if ( (records.at(i).GetCode() == code_string) && 
		 ( (strlen(records.at(i).GetAdditionId()) < 1)  || (strcmp(records.at(i).GetAdditionId(), "-1") == 0) ) ) break;
	    i++;
	}
    } else {
	while ( i < records.size() ) {
	    /*cout << "records.at(i).GetCode() - " << records.at(i).GetCode() << endl;
	    cout << "records.at(i).GetAdditionId() - " << records.at(i).GetAdditionId() << endl;
	    cout << "len - " << strlen( records.at(i).GetAdditionId() ) << endl;*/
	    if ( (records.at(i).GetCode() == code_string) && 
		 ( (strlen(records.at(i).GetAdditionId()) < 1)  || (strcmp(records.at(i).GetAdditionId(), "-1") == 0) ) ) break;
	    i++;
	}
    }
    //cout << "i = " << i << endl;
    if ( i < records.size() ) 
	if (records[i].enable_flag) pos = i;
    
    return pos;
}
//==================================================================================//
//flagcode = true - поиск товара по внутреннем коду, flagcode = false - поиск товара по штрих-коду  
double Receipt::QuantityWareByCode(const char* acode, bool flagcode)
{
  double q = 0;
  string code = acode;
  string str; 

  for(int i = 0; i < (int)records.size(); i++)  {
	if (flagcode) str = records.at(i).GetCode(); else str = records.at(i).GetBarcode();
    	if ( code == str ) q = q + records.at(i).quantity;
    	//printf("-----code = %s, str = %s\n", code.c_str(), str.c_str());
  }

  return q;
}
//==================================================================================//
double Receipt::CalcPositionDiscounts()
{
//puts("Receipt::CalcPositionDiscounts()");
  double redsum = 0;
  for(vector<Record_Receipt>::iterator i = records.begin(); i != records.end(); i++)
  {
    if(i->enable_flag)
      redsum = redsum + i->RecalculationDiscounts();
  }
  //printf("redsum = %f \n", redsum);
  return redsum;
}
//==================================================================================//
double Receipt::RecalculationDiscounts()
{
    //puts("Receipt::RecalculationDiscounts()");
  dsum = 0;
  double s;
  if(sp_receipt->disc_exception_mode)
    s = ExceptionSum();
  else
    s = Sum();         
    
  dsum = 0;  
  switch(sp_receipt->result_discount) // Правило выбора начисления итоговой скидки
  {
    case 0 : // выбирается максимальная скидка
      dsum = MaxDiscountAndCharge(s, discount);
      break;
    case 1 : // выбирается объединение скидок (скидки начисляются последовательно, используя порядок начисления)
      dsum = Union_Discount(s, discount);
      break;
    case 2 : // скидки суммируются, а потом начисляются
      dsum = Sum_Discount(s, discount);	
      break;
  }
  
  
  //double result_check_sum = Sum() - dsum;
  //printf("RecalculationDiscounts() dsum =  %2f \n", dsum);
  
  return dsum;
}

//==================================================================================//
double Receipt::CalculationPositionTaxBase(int i)
{
    
    double sum = records.at(i).sum - records.at(i).RecalculationDiscounts();
    double base_sum = sum;
    
    double prestage_percent = 0;
    bool percent_flag = false;
    
    // Посчитаем налоги 
    
    for (vector<TaxRate>::iterator j = positiontaxes.at(i).taxrates.begin(); j != positiontaxes.at(i).taxrates.end(); j ++)  {
	
	if ( !j->change_base ) {
	    j->taxsum = sum *(prestage_percent + j->rate) / 100;
	    base_sum -=  j->taxsum;
	} else {
	    j->taxsum = base_sum *(prestage_percent + j->rate) / 100;
	    base_sum -=  j->taxsum;
	}
    }
    
    return base_sum;
}

//==================================================================================// 
void Receipt::Add_Record(const char *barcode, const char *code, const char *shortcut, const char *longtitle, double quantity, double price, int fls, bool cfl, int a_sch, int f_sch, int t_gr, double m_pr, const char* addition_id, int section)
{
    //printf("Receipt::Add_Record(%s, %s, %s, %f, %f, section - %d)\n", barcode, code, longtitle, quantity, price, section);

  Record_Receipt record;

  record_num++;
  //---(*)---Zuskin---14/02/2012---
  // sum = sum + quantity * price; 
  sum += Round( quantity * price * 100 ) / 100;
  //-------------------------------
  
  record.Add_Record_Receipt(barcode, code, shortcut, longtitle, quantity, price, fls, cfl, a_sch, f_sch, t_gr, m_pr, addition_id);
  records.push_back(record);
  
  /******************* В зависимости от настроек ***********************/
  // если a_section не найден в кодах настроенных секций 
  // section = 0; - номер секции при печати 
  // section_number = a_section - номер аппаратной секции (он же хроаниться в транзакциях) 
    
  records[record_num - 1].section_number = section;
  
  int i = 0;
  while ( (i < sp_receipt->sections.size()) && ( sp_receipt->sections[i].number != section ) )
      i++;
  
  if (i < sp_receipt->sections.size()) 
      records[record_num - 1].section = i;
  else 	
      records[record_num - 1].section = 0;    
  
  //printf("Receipt:: added record section_number = %d print_section = %d \n ", records[record_num - 1].section_number, records[record_num - 1].section);
}

//==================================================================================// 
//n - количество налоговых ставок группы
void Receipt::Add_Taxes_Info(int tax_group, const char* tg_name,  int n)
{
    //cout << "add_taxes_ifno (" << tax_group << ", " << tg_name << ", " << n << ")";
    
    PositionTaxes pt;
    
    pt.tax_group_id = tax_group;
    pt.SetName( string(tg_name) );
    pt.num_rates = n;
    
    positiontaxes.push_back( pt);
    
}
//======================================================================//
double Receipt::TestMinPrice(double procent) {
    // проверка мин цены 
    //puts("   test min price");
    //printf("TestMinPrice(double procent = %f) \n", procent);
    
    //printf("sp_receipt->control_min_price = %d \n", sp_receipt->control_min_price);
    //printf("sum of check = %f \n", sum);
    
    double check_sum = 0;
    
    if ( sp_receipt->control_min_price == 1 || sp_receipt->control_min_price == 2 ) {
	// вычислим правильный процент скидки
	double right_discount_sum = 0;
	bool error_min_price = false;
	procent = procent / 100;
        double disc_itog = 0; 
        int max_pos_idx = -1;
	double max_pos_sum = 0;
	for (int i = 0; i < records.size(); i++) {
	    if ( records[i].enable_flag ) {
		if ( !sp_receipt->disc_exception_mode || ( sp_receipt->disc_exception_mode && records[i].dsum == 0) ) {
		    double position_sum = records[i].sum - records[i].dsum;
		    check_sum += position_sum;
		    double position_sum_with_procent = (1-procent)*position_sum;
		    double min_position_sum = 0;
		    if ( !(atoi(records[i].GetAdditionId()) > 0) ) min_position_sum = records[i].min_price * records[i].quantity;
		    position_sum = Round(position_sum*100) / 100;
		    min_position_sum = Round(min_position_sum*100) / 100;
		    position_sum_with_procent = Round(position_sum_with_procent*100) / 100;
		    if ( position_sum_with_procent < min_position_sum ) {
			error_min_price = true;
			right_discount_sum += (position_sum - min_position_sum);
			records[i].check_discount_sum = (position_sum - min_position_sum);
		    } else {
		        if (position_sum > max_pos_sum) {
		    	    max_pos_sum = position_sum;
			    max_pos_idx = i;
		        }
			right_discount_sum += position_sum - position_sum_with_procent;
			
			//---(*)---Zuskin---14/02/2012---
			records[i].check_discount_sum = position_sum - Round(position_sum * (1 - procent) * 100) / 100;
			//-------------------------------
		    }
		    disc_itog += records[i].check_discount_sum; 
		}
	    }
	}
	//-----(*)-----Zuskin-----20/10/2011-----
	if ( !error_min_price ) {
	    procent = procent*100;
	}
	else {
	    //-----(*)-----Zuskin-----21/10/2011-----
	    //if ( sp_receipt->control_min_price == 1 ) return -1;  //  "-1" means charge by 1 percent. Absolutely ordinary.
	    if ( sp_receipt->control_min_price == 1 ) return -9999;
	    //---------------------------------------
	    if ( sp_receipt->control_min_price == 2 ) 
	        procent = (right_discount_sum/check_sum)*100;
	}    
	//---------------------------------------
	//-----(*)-----Zuskin-----22/02/2012-----
	//double disc_diff = Round(Sum() * procent ) / 100 - disc_itog;
	double disc_diff = Sum() - Round(Sum() * (100 - procent) ) / 100 - disc_itog;
	
	if (disc_diff != 0) {
	    if (max_pos_idx == -1) {
		max_pos_idx = 0;
		max_pos_sum = 0;
		for (int i = 0; i < records.size(); i++) {
		    if ( records[i].enable_flag ) {
	    		double pos_sum = records[i].sum - records[i].dsum - records[i].check_discount_sum;
			if ( pos_sum > max_pos_sum ) {
			    max_pos_sum = pos_sum;
			    max_pos_idx = i;
			}
		    }        	
		}
	    }		
	    records[max_pos_idx].check_discount_sum += disc_diff;
	}
	return procent;
	
    } else  {
        //-----(*)-----Zuskin-----20/10/2011-----
        double disc_itog = 0; // ---(+)---
	for (int i = 0; i < records.size(); i++) {
	    if ( records[i].enable_flag ) {
		double position_sum = records[i].sum - records[i].dsum;
		
		//---(*)---Zuskin---17/02/2012---
		//records[i].check_discount_sum = position_sum * procent / 100;
		records[i].check_discount_sum = position_sum - Round(position_sum * (1-procent/100) * 100) / 100;
		//printf("records[%d].check_discount_sum = %2f - %2f\n", i, position_sum, Round(position_sum * (1-procent/100) * 100) / 100);
		//-------------------------------
		disc_itog += records[i].check_discount_sum;
	    }
	}
	//-----(+)-----Zuskin-----22/02/2012-----
	//double disc_diff = Round(Sum() * procent) / 100 - disc_itog;
	double disc_diff = Sum() - Round(Sum() * (100 - procent) ) / 100 - disc_itog;
	//printf("---------------------------Sum=%2f, procent = %2f, disc_itog = %2f, diff = %2f\n", Sum(), procent, disc_itog, disc_diff);
	
	if (disc_diff != 0) {
	    int max_pos_idx = 0;
	    double max_pos_sum = 0;
	    for (int i = 0; i < records.size(); i++) {
	        if ( records[i].enable_flag ) {
	    	    double pos_sum = records[i].sum - records[i].dsum - records[i].check_discount_sum;
		    if ( pos_sum > max_pos_sum ) {
			max_pos_sum = pos_sum;
			max_pos_idx = i;
		    }	
		}	
	    }
	    records[max_pos_idx].check_discount_sum += disc_diff;
	}
	//---(+)-(*)-----------------------------
    }  
    
    //printf("procent = %f \n", procent);
    return procent;
}
//======================================================================//

int Receipt::Add_Percent_Discount(int i, double p, const char * code, const char *name, const char *shortcut, bool manual_fl)
{
    //puts("RegistrationForm::Add_Percent_Discount(int i, double p, cosnt char * code, const char *name, const char *shortcut, bool manual_fl)");
    //printf("code of discount = %s \n", code);
    
    //printf("check discount = %f \n", p);
    
    //printf("Receipt :: Add percent check discount - %f \n", p);
    
    discount.at(i).Add_Discount(0, p);
    RecalculationDiscounts();
    //-----(*)-----Zuskin-----21/10/2011-----
    //if ( dsum > transaction_discount_sum ) 
    if ( fabs(dsum) > fabs(transaction_discount_sum) ) 
	transaction_discount_sum = dsum;
    //---------------------------------------	
    //printf("transaction_discount_sum = %f \n", transaction_discount_sum); 
    discount.at(i).Restore_Discount();
    
    discount.at(i).transaction_percent = p;
    discount.at(i).transaction_sum = Sum() * p/100 ;
    
    double right_procent = TestMinPrice(p);
    
    //printf(" result check discount percent = %f \n", right_procent);
    //printf("right_procent = %f \n", right_procent);
    
    //-----(*)-----Zuskin-----30/09/2011-----
    //-----(*)-----Zuskin-----21/10/2011-----
    //if ( right_procent == -1 ) {
    if ( right_procent == -9999 ) {
    //---------------------------------------
	discount.at(i).Restore_Discount();
	transaction_discount_sum = RecalculationDiscounts();
	return 4;
    }
    //---------------------------------------
    
    discount.at(i).Add_Discount(0, right_procent);
    
    int ret = 0;
    double rec_sum = Sum();
    double rec_dsum = RecalculationDiscounts();    
    double s = discount.at(i).dsum;   
    
    //  проверка максимальной скидки
    if ( manual_fl && (s > 0) && (Round(rec_sum * sp_receipt->max_check_discount) / 100 < s) )      ret = 1;
    // проверка максимальной надбавки
    else if ( manual_fl && (s < 0) && (Round(rec_sum * sp_receipt->max_check_increase) / 100 < s) )   ret = 2; 
    else if ( rec_sum - rec_dsum <= 0 )    ret = 3;    
    
    //printf("ret = %d \n", ret);
    
    if ( ret != 0 )  {
	discount.at(i).Restore_Discount();
	transaction_discount_sum = RecalculationDiscounts();
    }  else  {
	discount.at(i).Set_Names(name, shortcut);
	//puts("set code for discount");
	discount.at(i).Set_Code(code);
    }
    return ret;
}

//======================================================================//
int Receipt::Add_Sum_Discount(int i, double s, const char *code, const char *name, const char *shortcut, bool manual_fl)
{
    //puts("int Receipt::Add_Sum_Discount(int i, double s, const char *code, const char *name, const char *shortcut, bool manual_fl)");
    
    //printf("Receipt :: Add_Sum_Discount(int i = %d s = %f ) \n", i, s);
    
    discount.at(i).Add_Discount(s, 0);
    RecalculationDiscounts();
    if ( dsum > transaction_discount_sum ) 
	transaction_discount_sum = dsum;	
    //printf("transaction_discount_sum = %f \n", transaction_discount_sum); 	
    discount.at(i).Restore_Discount();
    
    
    discount.at(i).transaction_sum = s;
    discount.at(i).transaction_percent = s/Sum()*100;
    
    double rec_sum = Sum(); 
    //printf("Add sum check discount - %f ", s);
    //printf("percent = %f \n", s/rec_sum*100);    
    double right_procent = TestMinPrice(s/rec_sum*100);
    //printf(" result check discount percent = %f", right_procent);
    //printf("right_procent = %f \n", right_procent);
    
    if ( right_procent == -1 ) return 4;
    double right_sum = right_procent*rec_sum/100;
    //printf("right_sum = %f \n", right_sum);
    //printf(" sum = %f \n", right_sum);
    
    discount.at(i).Add_Discount(right_sum, 0);
  
    int ret = 0;

    double rec_dsum = RecalculationDiscounts();
    
    if(manual_fl && (s > 0) && (Round(rec_sum * sp_receipt->max_check_discount) / 100 < s))   //  проверка максимальной скидки
	ret = 1; 
    else if(manual_fl && (s < 0) && (Round(rec_sum * sp_receipt->max_check_increase) / 100 < s)) // проверка максимальной надбавки
	ret = 2;
    else if(rec_sum - rec_dsum <= 0)
	ret = 3;
  
    if ( ret != 0 )  {
	discount.at(i).Restore_Discount();
	transaction_discount_sum = RecalculationDiscounts();
    }  else  {
	discount.at(i).Set_Names(name, shortcut);
	discount.at(i).Set_Code(code);
    }
    
    return ret;
}

//==================================================================================// 
void Receipt::RecalcTransactionDiscount() {
    
    double discount_sum = RecalculationDiscounts();
    //printf("discount_sum = %f \n", discount_sum);
    //printf("transaction_discount_sum = %f \n", transaction_discount_sum);
    
    double check_sum = Sum();
    double procent_tr_discount = transaction_discount_sum / check_sum * 100;
    
    TestMinPrice(procent_tr_discount);
}

//==================================================================================// 
double Receipt::GetTransactionDiscountPercent(int i) {
    //double check_sum = Sum();
    //return transaction_discount_sum / check_sum * 100;   
    return discount.at(i).transaction_percent;
}
//==================================================================================// 
double Receipt::GetTransactionDiscountSum(int i ) {
    return discount.at(i).transaction_sum;
}
//==================================================================================// 
double Receipt::GetRecountTransactionDiscountPercent(int i) {
    double initial_discount_percent = GetTransactionDiscountPercent(i);
    double recount_percent = TestMinPrice(initial_discount_percent);
    return recount_percent;
}
//==================================================================================// 
double Receipt::GetRecountTransactionDiscountSum(int i) {
    double recount_percent = GetRecountTransactionDiscountPercent(i);
    //---(*)---Zuskin---14/02/2012---
    //return (Sum()*recount_percent/100);
    double s = Sum();
    return ( s - Round( s * (100 - recount_percent)) / 100 );
    //-------------------------------
}
//==================================================================================// 
    

//==================================================================================// 
void Receipt::Clear_Taxes()
{
  if(!positiontaxes.empty())positiontaxes.clear();
}

//==================================================================================// 
double Receipt::ExceptionSum()
{
//puts("Receipt::ExceptionSum()");
  double s = 0;
  
  for(vector<Record_Receipt>::iterator i = records.begin(); i != records.end(); i++)
    if(i->enable_flag)
    {
      i->RecalculationDiscounts();
      if(i->dsum == 0)
      {
        i->sum = i->quantity * i->price;
        //-----(+)-----Zuskin-----20/01/2012-----
        i->sum = Round(i->sum * 100)/100; 
        //-------------------------------------------------	
        s = s + i->sum;
      }
    }
  return Round(s*100)/100;
}

//==================================================================================// 
// сумма чека по позициям с учетом скидок на позиции
double Receipt::Sum()
{
//puts("Receipt::Sum()");
  double s = 0;
  for(vector<Record_Receipt>::iterator i = records.begin(); i != records.end(); i++)
  {
    if(i->enable_flag)
    {
      i->sum = i->quantity * i->price;
      // Round added by A.S. 2008-02-07 
      i->sum = Round(i->sum * 100)/100;
      s = s + i->sum - i->dsum;
    }
 }
// Round added by A.S. 2008-02-07
//s = s - dsum;
  sum = Round(s*100)/100;
  return sum;
}
//==================================================================================// 
// возвращает сумму чека со скидками, налогами, с учетом удаленных (сторнированных) позиций
double Receipt::GetResultSum()
{
    //printf("Receipt::GetResultSum() \n");
   double s = 0;
   for ( vector<Record_Receipt>::iterator i = records.begin(); i != records.end(); i++ )  {
     if ( i->enable_flag )    {
       i->sum = i->quantity * i->price;
       i->sum = Round(i->sum * 100)/100;
       s = s + i->sum - i->dsum;
       //printf("s = %2f\n", s);
     }
   }
   //-----(*)-----Zuskin-----20/10/2011-----
   //s = s - dsum;
   s -= Round(dsum*100)/100;
   //printf("--------------------------- s = %2f, dsum = %2f\n", s, Round(dsum*100)/100);
   //---------------------------------------
   
   sum = RoundCheckSum(s);
   
   //sum = Round(s*100)/100;
   
   //printf("GetResultSum return = %f \n", sum);
   return sum;
}	
//==================================================================================// 
double Receipt::RoundCheckSum(double check_sum) {
    
    //printf("RoundCheckSum(check_sum = %f, round_power = %f, type) \n", check_sum, round_power);
    
    if ( !sp_receipt->round_check_sum ) return check_sum;
    
    double round_power = sp_receipt->round_power;
    
    if ( check_sum <= round_power ) {
	round_sum = 0;
	return check_sum;
    }
    
    //printf("check_sum / round_power = %f \n", check_sum / round_power);
    
    int count = 0;
    if ( sp_receipt->round_type == BETTER_ROUND ) count = Round(check_sum/round_power);
    if ( sp_receipt->round_type == LESS_ROUND )  count = floor(check_sum/round_power);
    if ( sp_receipt->round_type == GREATER_ROUND ) count = ceil(check_sum/round_power);   
    
    //printf("count = %d \n", count);
    //printf("round_power * count = %f \n", round_power * count);
    
    round_sum = check_sum - (round_power * count);
    
    return (round_power * count);
}
//==================================================================================// 
//сумма позиций (исключая сторнированные) без учета скидок
double Receipt::GetPositionSum()
{
   double s = 0;
   for ( vector<Record_Receipt>::iterator i = records.begin(); i != records.end(); i++ )  {
     if ( i->enable_flag )    {
       i->sum = i->quantity * i->price;
       i->sum = Round(i->sum * 100)/100;
       s = s + i->sum;
     }
   }
	sum = Round(s*100)/100;
	return sum;
}
//==================================================================================// 
// итоговая сумма всех скидок
double Receipt::GetAllDiscountSum()
{
   double s = 0;
   for ( vector<Record_Receipt>::iterator i = records.begin(); i != records.end(); i++ )  {
     if ( i->enable_flag )    {
       i->sum = i->quantity * i->price;
       i->sum = Round(i->sum * 100)/100;
       s = s + i->dsum;
     }
   }   
   
   GetResultSum();   
   
   s = s + dsum + round_sum;
   //printf("Disount result sum = %f \n", s);
   //printf("  sum of position discount - %f \n", s);
   //printf("  sum of check reduction - %f \n", dsum);
   //printf("  sum of round_sum - %f \n", round_sum);
   sum = Round(s*100)/100;
   
   return sum;
}	
//==================================================================================// 
int Receipt::GetSectionCountPosition(int section) {
    
    int count_pos = 0;	
    for ( vector<Record_Receipt>::iterator i = records.begin(); i != records.end(); i++ )  {
	if ( i->enable_flag && i->section == section )  
	    count_pos++;
    }  
    return count_pos;
}
    
//==================================================================================// 
double Receipt::GetSectionSum(int section) {
    
    double sum = 0;
    
    for ( vector<Record_Receipt>::iterator i = records.begin(); i != records.end(); i++ )  {
	if ( i->enable_flag && i->section == section )  
	    sum += i->sum;
	    //-----(+)-----Zuskin-----20/10/2011-----
	    sum = Round(sum*100)/100;
	    //---------------------------------------
    }  
    //printf("GetSectionSum(%d) = %2f\n", section, sum);
    return Round(sum*100)/100;   
}
//==================================================================================// 
double Receipt::GetSectionDiscountSum(int section) {
    
    //printf("Receipt::GetSectionDiscountSum(int section - %d ) \n", section);
    
    RecalcTransactionDiscount();
    
    double section_discount_sum = 0;
    
    for ( vector<Record_Receipt>::iterator i = records.begin(); i != records.end(); i++ )  {
        printf("record : section = %d, check_discount = %2f, dsum = %2f\n", i->section, i->check_discount_sum, i->dsum);
	if ( i->enable_flag && i->section == section )  
	    section_discount_sum += Round(i->check_discount_sum*100)/100 + Round(i->dsum*100)/100;
    }  	
    
    //---(*)---Zuskin---24/02/2012---
    if (round_sum !=0) {
	printf("---------------------------------------round_sum != 0\n");
	if (section == section_num_with_max_sum) 
	    section_discount_sum += round_sum;
    }	
    printf("GetSectionDiscountSum(%d) = %f\n", section, section_discount_sum);
    return Round(section_discount_sum*100)/100;
}
//==================================================================================// 
double Receipt::GetSectionResultSum(int section) {
    if ( section == -1 ) 
	return GetResultSum();
    return (GetSectionSum(section) - GetSectionDiscountSum(section));
}
//==================================================================================// 
int Receipt::GetSectionNumWithMaxSum() {
    
    //-----(+)-----Zuskin-----2011-06-02-----
    if (sp_receipt->sections.size() == 0) return -1;
    //---------------------------------------
    
    vector<double>sections_sums (sp_receipt->sections.size(), 0);
    
    for ( vector<Record_Receipt>::iterator i = records.begin(); i != records.end(); i++ )  {
	if ( i->enable_flag  )  {
	    sections_sums[i->section] += i->sum -  i->check_discount_sum - i->dsum;
	}
    }  	
    
    section_num_with_max_sum = 0;
    
    for ( int section_num = 0; section_num < sp_receipt->sections.size(); section_num++ ) {
	//printf("sum of section %d = %f \n", section_num, sections_sums[section_num]);
	if ( sections_sums[section_num] > sections_sums[section_num_with_max_sum] )
	    section_num_with_max_sum = section_num;
    }
    
    //printf("section_num_with_max_sum = %d \n", section_num_with_max_sum);
    return section_num_with_max_sum;
}

//==================================================================================// 




