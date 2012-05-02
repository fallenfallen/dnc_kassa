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

#include "kkmregistration.h"

#include "const.h"

#include <malloc.h>
#include <string.h>
#include <qdatetime.h>

#include "setup_param.h"
#include "addon_func.h"

#include <qtextcodec.h>

#include "Section.h"
#include "UsefulFunctions.h"

//-----(+)-----Zuskin-----27/07/2011-----
extern Registration Reg;
//-------------------------------------------------

QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
QTextCodec * DbCodec = QTextCodec::codecForName(DBCODEC);
QTextCodec * HwCodec = QTextCodec::codecForName(HWCODEC);


//======================================================================//

void KkmRegistrations::SetSP(SP* asp)
{
    sp = asp;
}

//======================================================================//
void KkmRegistrations::SetCreditStatus(int cs)
{
    CreditStatus = cs;
}
//======================================================================//
int KkmRegistrations::GetCreditStatus()
{
    return CreditStatus;
}
//======================================================================//
KkmRegistrations::KkmRegistrations()
{
	 hw = NULL;
	 //  receipt = NULL;
	 log = NULL;
	 ctrl_state = false;
	 init_flag = false;
	 CreditStatus = -1;
}
//======================================================================//
void KkmRegistrations::KkmSetCtrlState(bool state)
{
	 ctrl_state = state;
}
//======================================================================//
bool KkmRegistrations::KkmGetCtrlState()
{
	 return ctrl_state;
}
//======================================================================//
KkmRegistrations::KkmRegistrations(HWSRV * hw_prt,/* Receipt * rec_ptr,*/ DebugLevelLog * log_ptr)
{
	 hw = hw_prt;
	 log = log_ptr;
	 hw->ECR_ReadStatus();
	 stringwide = hw->ECR_GetCharLineLength();
	// printf("^^^^^^^^^^^^^^^^^^^^stringwide =  %d\n", stringwide);
	 init_flag = true;
	 ctrl_state = false;
	 CreditStatus = -1;
}

//======================================================================//
KkmRegistrations::~KkmRegistrations()
{
  //log->PrintString(DEBLEVEL_REGFORM, INFO, "KkmRegistrations::~KkmRegistrations()");
}

//======================================================================//
int KkmRegistrations::KkmSetMode(int m)
{
	 if(!init_flag)
		  return -2;
	 int kkm_res = 0;
	 kkm_res = hw->ECR_SetMode(0);
	 if(kkm_res != 0)
		  return kkm_res;
	 kkm_res = hw->ECR_SetMode(m);
	 if(kkm_res != 0)
		  return kkm_res;
	 return 0;
}


//======================================================================//
int KkmRegistrations::KkmRegistration(bool test_mode, double s)
{    
	 if ( ! init_flag )    return -2;
	 //printf("s = %02f \n", s);
	 int kkm_res = hw->ECR_Registration(s, 1, "", 1, 1, 0, 0.00, 0.00, test_mode);
	 printf("Kkm Registation ---- hw->ECR_Registration(s = %f, 1, "", 1, 1, 0, 0.00, 0.00, test_mode) return %d \n", s, kkm_res);	 
	 if ( kkm_res != 0 )    return kkm_res;
	 return 0;	
}


//======================================================================//
int KkmRegistrations::KkmReturn(bool test_mode, double s)
{
	 if ( ! init_flag  )   return -2;
	 int kkm_res = 0;
	 //printf("hw->ECR_Return(s = %03f)\n", s);
	 kkm_res = hw->ECR_Return(s, 1, "", 1, 1, 0, 0.00, 0.00, test_mode);	// регистрация сумм возврата в фр (price, quantity, department, test_mode)
	 if (kkm_res!=0) 
		  return kkm_res;
	 //else
		//  if (test_mode)
		//		kkm_res = KkmPrintString(HwCodec->fromUnicode(DefCodec->toUnicode("ВОЗВРАТ")), true);
	 return 0;  
}
 
//======================================================================//
int KkmRegistrations::KkmPrintTotalSumBySection(Receipt* receipt, double nalsum, double credsum, double contsum) {
    int kkm_res = 0;
    
    kkm_res = PrintCenterString(HwCodec->fromUnicode(DefCodec->toUnicode("ИТОГ ПО ЧЕКАМ")));
    if ( kkm_res < 0 ) return kkm_res;
    
    if ( receipt->GetCheckType() == CHECK_RETURN ) 
	kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ВОЗВРАТ")), QString("=")+QString::number(receipt->GetResultSum(), 'f', 2), " ", 1);
    else 
	kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("К ОПЛАТЕ")), QString("=")+QString::number(receipt->GetResultSum(), 'f', 2), " ", 1);
    
    if ( kkm_res < 0 ) return kkm_res;
    
    kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("НАЛИЧНЫМИ")), QString("=")+QString::number(nalsum, 'f', 2), " ");
    if ( kkm_res < 0 ) return kkm_res;
    
    /*printf("receipt->GetResultSum() = %f \n", receipt->GetResultSum());
    printf("nalsum = %f \n", nalsum);
    printf("credsum = %f \n", credsum);*/
    
    if ( (nalsum+credsum) > receipt->GetResultSum() )  {
	kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("СДАЧА")), QString("=")+QString::number((nalsum+credsum) - receipt->GetResultSum(), 'f', 2), " ");
	if ( kkm_res < 0 ) return kkm_res;    
    }
    
    QString str_separator = "";
    for (int i=0; i < stringwide; i++) 
	str_separator += "-";
    
    kkm_res = KkmPrintString(str_separator.ascii());
    if ( kkm_res < 0 ) return kkm_res;   
    
    return kkm_res;
}

//======================================================================//
int KkmRegistrations::KkmPrintCheckPositions(Receipt * receipt, int section, bool copy_of_return)
{
    printf("int KkmRegistrations::KkmPrintCheckPositions(int section = %d )\n", section);
   
    if ( ! init_flag )    return -2;       
    
    string s;
    QString ss;
    QString qs;
    QString ps;
    int kkm_res = 0;    
    
    if ( section > -1 ) {
	kkm_res = KkmPrintSeveralStrings(sp->sections[section].head_text);
	if ( kkm_res < 0 ) return kkm_res;
    }
    
    QString check_number = HwCodec->fromUnicode(DefCodec->toUnicode("№"));
    check_number += QString::number(receipt->GetCheckNumber());
    
    if ( receipt->GetCheckType() == CHECK_COPY ) {
	kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("Копия чека")), check_number );
	if ( kkm_res < 0 ) return kkm_res;	     
	if ( copy_of_return  ) {
	    kkm_res = KkmPrintWideString(HwCodec->fromUnicode(DefCodec->toUnicode("Возврат")), true);
	    if ( kkm_res != 0 )  {   return kkm_res;  }
	}
    } else {
	kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("Чек")), check_number );
	if ( kkm_res < 0 ) return kkm_res;
	
	if ( receipt->GetCheckType() == CHECK_RETURN ) {
	    kkm_res = KkmPrintWideString(HwCodec->fromUnicode(DefCodec->toUnicode("Возврат")), true);
	    if ( kkm_res != 0 )  {   return kkm_res;  }
	}
    }
	 
    kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("КАССИР")), HwCodec->fromUnicode(DefCodec->toUnicode(receipt->getCashier())));
    if ( kkm_res < 0 ) return kkm_res;
    
    if ( ctrl_state ) {  
	QString sum_string = QString::number(receipt->GetSectionResultSum(section), 'f', 2); 
	kkm_res = KkmPrintString(HwCodec->fromUnicode(DbCodec->toUnicode("Сумма по чеку")), true);
	if ( kkm_res == 0 )  {
	    QString sum_print = "";
	    //printf("stringwide = %d\n",stringwide);
	    //printf("sum_string.length() = %d\n",sum_string.length());
	    for (unsigned int i = 0; i < stringwide - sum_string.length(); i++)    
		sum_print += ' ';
	    sum_print += sum_string;	
	    kkm_res = KkmPrintString(sum_print.ascii(), true);
	}
	ctrl_state = false;
	return kkm_res;
    }
    
    //-----(+)-----Zuskin-----20/01/2012-----
    double CRCSectionSum = 0;
    //--------------------------------------------------
    for ( int i = 0; i < receipt->record_num; i++ )  {       
	if ( receipt->GetPosElemRef(i)->enable_flag )    { 
	    //printf("KkmRegistration PrintCheckPosition receipt->GetPosElemRef(i)->section = %d \n");
	    if ( (section != -1 && receipt->GetPosElemRef(i)->section == section) || section == -1 ) {
		
 	        //-----(+)-----Zuskin-----20/01/2012-----
		CRCSectionSum += receipt->GetPosElemRef(i)->GetPositionResultSum();
		//-------------------------------------------------
		
		QString caption = QString::number(i+1) + ".";
		caption += HwCodec->fromUnicode(DbCodec->toUnicode(receipt->GetPosElemRef(i)->GetShortcut()));
		kkm_res = KkmPrintString(caption.ascii(), true, sp->cut_position_name);
		if ( kkm_res != 0 )        return kkm_res;			  
		
		QString price_quantity_sum_str = "";
		//-----(+)-----Zuskin-----18/08/2011-----
		QString price_quantity_str = "";
		QString sum_str = "";
		//---------------------------------------
		if ( receipt->GetPosElemRef(i)->quantity != 1 ) {
		    price_quantity_sum_str = QString::number(receipt->GetPosElemRef(i)->price, 'f', 2) + " X " + QString::number(receipt->GetPosElemRef(i)->quantity, 'f', 3) + " =" + QString::number(receipt->GetPosElemRef(i)->sum, 'f', 2);
		    //-----(+)-----Zuskin-----18/08/2011-----
		    if (price_quantity_sum_str.length() > stringwide) {
			price_quantity_str = QString::number(receipt->GetPosElemRef(i)->price, 'f', 2) + " X " + QString::number(receipt->GetPosElemRef(i)->quantity, 'f', 3);
			sum_str = " =" + QString::number(receipt->GetPosElemRef(i)->sum, 'f', 2);
		    }
		    //---------------------------------------
		}    
		else 
		    price_quantity_sum_str = "=" + QString::number(receipt->GetPosElemRef(i)->sum, 'f', 2);
		
		QString code_str = "";
		code_str += HwCodec->fromUnicode(DbCodec->toUnicode("Код: "));
		//-----(*)-----Zuskin-----27/01/2012-----
		//code_str += receipt->GetPosElemRef(i)->GetCode();
		// !!! в кодах товаров могут быть русские буквы !!!
		code_str += HwCodec->fromUnicode(DbCodec->toUnicode(receipt->GetPosElemRef(i)->GetCode()));
		//-------------------------------------------------
		
		//-----(*)-----Zuskin-----18/08/2011-----
		//kkm_res = KkmPrintSeparatorString(code_str, price_quantity_sum_str);
		if (sum_str.length() == 0) {
		    kkm_res = KkmPrintSeparatorString(code_str, price_quantity_sum_str);
		}
		else {
		    kkm_res = KkmPrintSeparatorString("", price_quantity_str);
		    kkm_res = KkmPrintSeparatorString(code_str, sum_str);
		}
		//---------------------------------------
		//-----(+)-----Zuskin-----11/11/2011-----
		if ( kkm_res != 0 )        return kkm_res;
		//---------------------------------------
			  
		//Налоги
		if ( sp->print_position_taxes && (receipt->GetPosElemRef(i)->tax_group != 0) )      {
		    kkm_res = KkmPositionTaxes(i, receipt);
		    if ( kkm_res != 0 )          return kkm_res;
		}
	    }	
	}
    }
	 
    kkm_res = KkmPrintCheckReductions(receipt, section);
    //printf("kkm_res of KkmPrintCheckReductions(section - %d) return %d \n", section, kkm_res);
    if ( kkm_res < 0 ) return kkm_res;
    
    if ( section > 0 ) {
	//printf("print itog - \n");
	kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DbCodec->toUnicode("ИТОГ")), QString("=")+QString::number(receipt->GetSectionResultSum(section), 'f', 2), " ", 1);
	if ( kkm_res < 0 ) return kkm_res;
	
	//-----(+)-----Zuskin-----12/12/2011-----
	//добавим строчку с датой/временем печати квитанции
	QString dts = "";
	QString ss = "";
	ss = QString::number(hw->ECR_GetDay());
	if (ss.length() < 2) {
	dts += "0";    
	}
	dts += ss;
	dts += ".";
	ss = QString::number(hw->ECR_GetMonth());
	if (ss.length() < 2) {
	dts += "0";    
	}
	dts += ss;
	dts+= ".";
	ss = QString::number(hw->ECR_GetYear());
	if (ss.length() < 2) {
	dts += "0";    
	}
	dts += ss;
	dts += " ";
	ss = QString::number(hw->ECR_GetHour());
	if (ss.length() < 2) {
	dts += "0";    
	}
	dts += ss;
	dts += ":";
	ss = QString::number(hw->ECR_GetMinute());
	if (ss.length() < 2) {
	dts += "0";    
	}
	dts += ss;
	string s = dts.ascii();
	kkm_res = KkmPrintSeparatorString(s.c_str(), "", " ");
	//-------------------------------------------------
	
	kkm_res = KkmPrintSeveralStrings(sp->sections[section].bottom_text);
	if ( kkm_res < 0 ) return kkm_res;
	
	//отрезка
	kkm_res = KkmPartialCut();
	if (kkm_res != 0) return kkm_res;
    }
    

    if ( section > -1 ) {
	//-----(+)-----Zuskin-----20/01/2012-----
	printf(" Checking sum of section: %2f, %2f\n ", CRCSectionSum, receipt->GetSectionResultSum(section));
	if ( CRCSectionSum != receipt->GetSectionResultSum(section) ) {
	    puts("Check FAILED!!!");
	}
	//--------------------------------------------------
	
	if ( receipt->GetCheckType() == CHECK_SELL ) 
	    sp->addCashSumInSection(section, receipt->GetSectionResultSum(section));
	else if ( receipt->GetCheckType() == CHECK_RETURN )
	    sp->deductCashSumInSection(section, receipt->GetSectionResultSum(section));
    }
    
    printf("end of print check position by section num = %d. kkm_res = %d \n", section, kkm_res);
    
    return kkm_res;
}

//======================================================================//
int KkmRegistrations::KkmPrintSeveralStrings(const string & str) {
    int kkm_res = 0;
    
    vector<string> printing_strings = parseStringWithSeparator(str, "\n");
    
    for ( int i = 0; i < printing_strings.size(); i++ ) {
	//cout << "string " << i << " - " << printing_strings[i] << endl;
	kkm_res = PrintCenterString(HwCodec->fromUnicode(DbCodec->toUnicode(printing_strings[i].c_str())));
	if ( kkm_res < 0 ) return kkm_res;
    }
    
    return kkm_res;
}
//======================================================================//

int KkmRegistrations::KkmPosRegistration(Receipt * receipt)
{
	 int kkm_res = 0;                          
	 if ( ! init_flag )    return -2;    
	 if ( ctrl_state ) {  
		  QString sum_string = QString::number(receipt->GetResultSum(), 'f', 2); 
		  kkm_res = hw->ECR_Registration(sum_string.toDouble(), 1, HwCodec->fromUnicode(DbCodec->toUnicode("Сумма по чеку")), 1, 1, 0, 0.00, 0.00, false);	
		  ctrl_state = false;
		  return kkm_res;	  
	 }
	 for ( int i = 0; i < receipt->record_num; i++ ){  
		  if ( receipt->GetPosElemRef(i)->enable_flag ){ 
				QString name_ware = HwCodec->fromUnicode(DbCodec->toUnicode(receipt->GetPosElemRef(i)->GetShortcut()));
				double qs = receipt->GetPosElemRef(i)->quantity;
				double ps = receipt->GetPosElemRef(i)->price;
				double charge = (-1)*receipt->GetPosElemRef(i)->dsum;
				//printf("CHARGE BEFORE REGISTRATION  = [[%f]]\n",charge);
				kkm_res = hw->ECR_Registration(ps, qs, name_ware, 1, 1, 0, 0.00, charge, false);	
			//	printf("kkm_res = %d\n",kkm_res);
				// int ECR_Registration(double  Price, double  Quantity, const char * NameWare, int Department = 1, int WareGroup = 1, int NumberCharge = 0, double Percents = 0.00, double Charge = 0.00, bool TestMode = false);
		  }
	 }
	 return kkm_res;
}
//======================================================================//
int KkmRegistrations::KkmPosReturn(Receipt * receipt)
{
	 //puts("<<int KkmRegistrations::KkmPosReturn(Receipt * receipt)>>");
	 int kkm_res = 0;                          
	 if ( ! init_flag )    return -2;    
	 if ( ctrl_state ) {  
		  QString sum_string = QString::number(receipt->GetResultSum(), 'f', 2); 
		  kkm_res = hw->ECR_Return(sum_string.toDouble(), 1, HwCodec->fromUnicode(DbCodec->toUnicode("Сумма по чеку")), 0, 1, 0, 0.00, 0.00, false);	
		  ctrl_state = false;
		  return kkm_res;	  
	 }
	 for ( int i = 0; i < receipt->record_num; i++ ){  
		  if ( receipt->GetPosElemRef(i)->enable_flag ){ 
				QString name_ware = HwCodec->fromUnicode(DbCodec->toUnicode(receipt->GetPosElemRef(i)->GetShortcut()));
				double qs = receipt->GetPosElemRef(i)->quantity;
				double ps = receipt->GetPosElemRef(i)->price;
				double charge = (-1)*receipt->GetPosElemRef(i)->dsum;
				//printf("CHARGE BEFORE RETURN  = [[%f]]\n",charge);
				kkm_res = hw->ECR_Return(ps, qs, name_ware, 1, 1, 0, 0.00, charge, false);	
				// int ECR_Return(double  Price, double  Quantity, const char * NameWare, int Department = 1, int WareGroup = 1, int NumberCharge = 0, double Percents = 0.00, double Charge = 0.00, bool TestMode = false);
		  }
	 }
	 return kkm_res;
}
//======================================================================//
int KkmRegistrations::KkmPrintCheckReductions(Receipt * receipt, int section)
{
    
  int kkm_res = 0;
  
  // receipt->CalcPositionDiscounts() + receipt->dsum - сумма итоговой скидки
  ////puts("----------------Print Sum Reduction ---------------------");
  
  double discount_sum = 0;
  
  if ( section == -1 ) {
      discount_sum = receipt->GetAllDiscountSum();     //- сумма итоговой скидки;
  } else {
      discount_sum = receipt->GetSectionDiscountSum(section);
  }
  
  
  //printf("KkmPrintCheckReductions(section = %d discount sum = %f )\n", section, discount_sum);
  
 //-----(*)-----Zuskin-----27/07/2011-----
//  if ( discount_sum == 0 ) return 0;
  QString s, ds;
 
  if (discount_sum != 0) {
    
      if ( discount_sum > 0 )  s = HwCodec->fromUnicode(DefCodec->toUnicode("Итого скидка : "));
      if ( discount_sum < 0 )  s = HwCodec->fromUnicode(DefCodec->toUnicode("Итого надбавка : "));
      kkm_res = KkmPrintString(s.ascii(), true);
      if(kkm_res != 0)  return kkm_res;

      if ( discount_sum > 0 ) ds = QString::number(discount_sum, 'f', 2);
      if ( discount_sum < 0 ) ds = QString::number(-discount_sum, 'f', 2);
  
      kkm_res = KkmPrintSeparatorString("", ds);
  
  }
  //-----(*)-----Zuskin-----27/07/2011-----
  if ( receipt->GetDiscountElemRef(1)->df == true ) {
	    string ccode = receipt->GetDiscountElemRef(1)->GetCode(); 
	    if (ccode.length() > 0) { // есть какая-то карта, надо понять, не бонусная ли она
		int dtype = Reg.GetTypeByCard(ccode.c_str());
		if (dtype == 4) { // !!! bonuscard !!!
		    QString str_separator = "";
		    for (int i=0; i < stringwide; i++) 
			str_separator += "*";
		    kkm_res = KkmPrintString(str_separator.ascii());
		    
		    s = HwCodec->fromUnicode(DefCodec->toUnicode("Уважаемый покупатель!"));
		    kkm_res = KkmPrintString(s.ascii(), true);
		    s = HwCodec->fromUnicode(DefCodec->toUnicode("По Вашей карте:"));
   		    kkm_res = KkmPrintString(s.ascii(), true);
		    s = HwCodec->fromUnicode(DefCodec->toUnicode(receipt->GetDiscountElemRef(1)->GetName()));
		    kkm_res = KkmPrintString(s.ascii(), true);
		    double dsum = receipt->GetDiscountElemRef(1)->dsum;
		    if (dsum != 0) {
			// бонусы еще не списаны
			//Reg.UpdateBonusCard(ccode.c_str(), cur_dsum - dsum);
			s = HwCodec->fromUnicode(DefCodec->toUnicode("Списано бонусов : "));
//			kkm_res = KkmPrintString(s.ascii(), true);
			ds = QString::number(dsum, 'f', 2);
			kkm_res = KkmPrintSeparatorString(s.ascii(), ds);
		    }
		    double bpercent = Reg.GetPercentByCard(ccode.c_str());
		    double bonus = 0;
		    if (bpercent > 0) {
			bonus = Round((receipt->GetResultSum()) * bpercent)/100;
			s = HwCodec->fromUnicode(DefCodec->toUnicode("Начислено бонусов: "));
			//kkm_res = KkmPrintString(s.ascii(), true);
			ds = QString::number(bonus, 'f', 2);
			kkm_res = KkmPrintSeparatorString(s.ascii(), ds);
		    }
		    double cur_dsum = Reg.GetSummByCard(ccode.c_str());
		    s = HwCodec->fromUnicode(DefCodec->toUnicode("Сумма бонусов : "));
		    ds = QString::number(cur_dsum - dsum + bonus, 'f', 2);
		    kkm_res = KkmPrintSeparatorString(s.ascii(), ds);

    		    kkm_res = KkmPrintString(str_separator.ascii());
		    
		}	
	   }
   }
   //-------------------------------------------------
  
  return kkm_res;
}
/*****************************************************************************/
int KkmRegistrations::PrintCenterString(QString str)
{
	 QString s = "";
	 cout << str << endl;
	 unsigned int lenght = hw->ECR_GetCharLineLength();
	 unsigned int col_probelov = 0;
	 QString ss = str;
	 while ( ss.length()>lenght ){
		  str = ss.mid(0,lenght);
		  if ( !str.isEmpty() ) {
				col_probelov = (int) (lenght/2 - str.length()/2);
				for ( unsigned int i = 0; i < col_probelov; i++)
					 s = s+" ";
				s = s + str;
				int err = hw->ECR_PrintString(s.ascii());  
				if ( err != 0 ) return err;
		  } 
		  ss.remove(0, lenght);
	 }
	 cout << ss << endl;
	 str = ss;
	 s = "";
	 cout << str << endl;
	 if ( !str.isEmpty() ) {
		  col_probelov = (int) (lenght/2 - str.length()/2);
		  for ( unsigned int i = 0; i < col_probelov; i++)
				s = s+" ";
		  s = s + str;
		  int err = hw->ECR_PrintString(s.ascii());  
		  if ( err != 0 ) return err;
	 } 
    return 0;
}
/*****************************************************************************/
int KkmRegistrations::PrintBoldCenterString(QString str)
{
    QString s = "";
    
    int lenght = hw->ECR_GetCharLineLength();
    int col_probelov = 0;
    
    if ( !str.isEmpty() ) {
	col_probelov = (int) (lenght/2 - str.length())/2;
	for ( int i = 0; i < col_probelov; i++ )    s = s+" ";
	s = s + str;
	int err = hw->ECR_PrintWideString(s.ascii());  
	if ( err != 0 ) return err;
    }
    return 0;
}
//======================================================================//
int KkmRegistrations::PrintCreditInfo(bool client_check)
{
    
    //-----(*)-----Mirabell----12/11/2011-----
    //printf("int KkmRegistrations::PrintCreditInfo()\n");
    //printf("int KkmRegistrations::PrintCreditInfo(client_check = %d)\n",client_check);
    //----------------------------------------

  /*  printf("CreditStatus = %d \n", CreditStatus);
    printf("hw->PS_GetSumm() = %f \n", hw->PS_GetSumm());
    printf("operation type = %d \n", hw->PS_GetCodeOper());*/
        
    if ( hw->PS_GetType() <= 0  ) return 0;
    if ( CreditStatus  == -1 ) return 0;   
    if ( hw->PS_GetSumm() <= 0 )  return 0;
    int res = 0;
    if (hw->ECR_GetProtocol() == 3){
	 if (hw->ECR_ReadTypePrintHeadLine() == 0){
        if (hw->ECR_GetTypePrintHeadLine() == 1)
	     {
		  res = hw->ECR_PrintDocHeadLine(); 
		  if ( res != 0 )    
		    return res;
		}
	  }
    }
	 
    CreditStatus = hw->PS_GetTransStatus();	
    //-----(*)-----Mirabell-----14/02/2012-----  
    int TypeSystem = hw->PS_GetTypeSystem();
    // печать inpas квитанции с невыполненой транзакцией
    if ( (TypeSystem!=2) && (hw->PS_GetTransStatus() != 1) && (hw->PS_GetPrintCancelCheck() == false) ) return 0;
    
    /*if ( client_check )    return PrintCenterString(HwCodec->fromUnicode(DefCodec->toUnicode("Квитанция клиенту")));
     else return PrintCenterString(HwCodec->fromUnicode(DefCodec->toUnicode("Квитанция кассиру")));*/
    
    int err = 0;
    switch(TypeSystem){
    case 2:{//СБЕР
	vector <string>mas = hw->PS_InsertMasForPrinting(); 
	int col = mas.size();
	//printf("col = %d\n",col);
	if ( col>0 )  {
	    for ( int g = 0; g < col; g++ )  {
		string s = mas[g];
		//printf("s = |%s|\n",s.c_str());
		res = hw->ECR_PrintString(s.c_str());
		if ( res != 0 ) return res;
	    }
	}
	break;
    }
    default:{
	    //-----(*)-----Mirabell-----11/11/2011-----
	    bool packet_type = hw->PS_GetPacketTypeAnswer();
	    if (packet_type){
		vector <string>mas = hw->PS_InsertMasForPrinting(); 
		int col = mas.size();
		//printf("col = %d\n",col);
		if ( col>0 )  {
		    for ( int g = 0; g < col; g++ )  {
			string s = mas[g];
			//printf("s = |%s|\n",s.c_str());
			vector <string> input_str;
			string tmp_str = "";
			for(unsigned int i = 0; i<s.length();i++){
			    // printf("s[%d] = %d\n",i,s[i]);
			    if ( ((s[i] & 0xFF) == 0x0D) && ((s[i+1] & 0xFF) == 0x0A) ){
				//printf("( s[i] == 0x0D  && (s[i+1] == 0x0A )   i  = %d\n",i);
				input_str.push_back(tmp_str);
				tmp_str.clear();
				i++;
			    }
			    else{
				//printf("i = %d\n",i);
				tmp_str.push_back(s[i]);
			    }
			}
			//printf("input_str.size() = %d\n",input_str.size());
			for (unsigned int i = 0; i<input_str.size();i++){
			    string s1 = input_str[i];
			    //printf("s1 = |%s|\n",s1.c_str());
			    res = hw->ECR_PrintString(s1.c_str());
			    if ( res != 0 ) return res;
			}
		    }
		}
	    }
	    else{    
	
		//--------------------------------------------
		printf("Printing credit receipt...\n");
		vector <string>mas = hw->PS_InsertMasForPrinting(); 
		int col = mas.size();
    
		//------------------------Наименование банка(хоста)------------------------
		QString bank = hw->PS_GetBank();
		if (bank != " "){
		    res = PrintCenterString(bank);
		    if ( res != 0 ) return res;
		}
		//------------------------Название торговой организации-------------------    
		QString trade_name = hw->PS_GetTradeName();
		if (trade_name != " "){
		    res = PrintCenterString(trade_name);
		    if ( res != 0  ) return res;
		}
	 
		//------------------------Город-------------------------------------------------------
		QString town = hw->PS_GetTownTrade();
		if (town != " "){
		    res = PrintCenterString(town);
		    if ( res != 0 ) return res;
		}
	 
		//------------------------Адрес-------------------------------------------------------
		QString address = hw->PS_GetAddressTrade();
		if (address != " "){
		    res = PrintCenterString(address);
		    if ( res != 0 ) return res;
		}
		//-------------------------Идентификатор терминала-----------------------
		QString term = hw->PS_GetTerminalId();
		if ( !term.isEmpty() ) {
		    term = "TID:" + term;
		    res = hw->ECR_PrintString(term);  
		    if ( res != 0 ) return res;
		}
    	 
		//-------------------------Идентификатор торговой точки-----------------------	 
		QString idAss = hw->PS_GetIDAssistant();
		//QString idAss = "25684";
		if ( !idAss.isEmpty() ){
		    idAss = "MID: " + idAss;
		    res = hw->ECR_PrintString(idAss);  
		    if ( res != 0 ) return res;
		}
    
		QString temp;
		//-------------------------Тип операции-----------------------
		int type = hw->PS_GetCodeOper();
		//printf("type = %d \n", type);
		if ( type == -1 ) return -1;
    
		switch ( type )  {
		case 1:
		    temp = HwCodec->fromUnicode(DefCodec->toUnicode("ОПЛАТА ТОВАРОВ"));
		    break;
		case 2:
		    type = hw->PS_GetOriginalCodOper();
		    //printf("код оригинальной операции: %d\n",type);
		    temp = HwCodec->fromUnicode(DefCodec->toUnicode("ОТМЕНА ОПЕРАЦИИ"));
		    break;
		case 3:  
		    temp = HwCodec->fromUnicode(DefCodec->toUnicode("ВОЗВРАТ"));
		    break;	
		default:
		    //printf("ЧЕКИ НА СВЕРКУ ИТОГОВ НЕ ПЕЧАТАЮТСЯ!!!");
		    return 1;	
		}
		 
		PrintBoldCenterString(temp);	 	  
	 
		QString s;
    
		//-------------------------Сумма-----------------------
		double summ = hw->PS_GetSumm();
		if ( summ > 0 ) {
		    res = KkmPrintSeparatorString( HwCodec->fromUnicode(DefCodec->toUnicode("СУММА: ")), QString::number(summ/100, 'f', 2) + " RUR", " ");
		    if ( res != 0 ) return res;
		}
    
		//-------------------------Итоговая сумма-----------------------	     
		res = KkmPrintSeparatorString( HwCodec->fromUnicode(DefCodec->toUnicode("ИТОГО: ")), QString::number(summ/100, 'f', 2) + " RUR", " ");
		if ( res != 0 ) return res;
    
		//--------------------------Вводился ли пин опционально---------
		if ( hw->PS_GetModeCodingPinBlock() != 0 )  {
		    err = PrintCenterString("PIN");
		    if ( err != 0 ) return err; 
		}
    
		//-----------------------Номер карты и срок действия-----------
		QString cardnum = hw->PS_GetCardNumber();   
		//-----------------------Возьмем маску для маскирования номера карты из настроек оборудования
		QString maska = hw->PS_GetCardMask();
		if(!maska.isEmpty()){
		    int i = maska.find("*");
		    while (i != -1){
			cardnum.replace(i,1,"*");
			maska.replace(i,1," ");
			i = maska.find("*");
		    }
		}
		if ( !cardnum.isEmpty() ) {  
	    
		    cardnum.insert(4, " ");
		    cardnum.insert(9, " ");
		    cardnum.insert(14, " ");
	
		    res = PrintCenterString(cardnum);
		    if ( res != 0 ) return res;
		}
    
		//-----------------------------Код авторизации-----------------------
		if ( hw->PS_GetCodAuth() > 0 )  {
		    QString codauth = HwCodec->fromUnicode(DefCodec->toUnicode("КОД АВТОРИЗАЦИИ: "));
		    codauth = codauth + hw->PS_GetCodAuth();
		    res = PrintCenterString(codauth);
		    if ( res != 0 ) return res;	
		}
    
		//-----------------------------Номер ссылки-----------------------
		temp = hw->PS_GetReferNum();
		if ( !temp.isEmpty() )  {
		    temp = "RRN:   " +  temp;
		    res = PrintCenterString(temp);
		    if ( res != 0 ) return res;		
		}	
    
		//-------------------------------Код ответа хоста------------------
		QString codanswer = hw->PS_GetCodAnswer();
		if ( !codanswer.isEmpty() )  {
		    temp  = HwCodec->fromUnicode(DefCodec->toUnicode("КОД ОТВЕТА: "));
		    temp += codanswer;
		    res = PrintCenterString(temp);
		    if ( res != 0 ) return res;	
		}
    
		//-------------------------------Текст с расшифровкой ответа---------
		QString data = hw->PS_GetAddDataAnswer();
		if ( !data.isEmpty() ){
		    res = PrintCenterString(data);
		    if ( res != 0 ) return res;	
		}
		//-------------------------------Дата и время------------------------------------
		//printf("hw->ECR_GetYear() = %d\n",hw->ECR_GetYear());
		QString d = HwCodec->fromUnicode(DefCodec->toUnicode("ДАТА  "));
		d += s.setNum(hw->ECR_GetYear());
		d += "/";
		d +=  s.setNum(hw->ECR_GetMonth());
		d += "/";
		d += s.setNum(hw->ECR_GetDay());   
		d += HwCodec->fromUnicode(DefCodec->toUnicode("  ВРЕМЯ  "));
		d += s.setNum(hw->ECR_GetHour());
		d += ":";
		d +=  s.setNum(hw->ECR_GetMinute());
		d +=  ":";
		d += s.setNum(hw->ECR_GetSecond());
		res = hw->ECR_PrintString(d.ascii());
		if ( res != 0 ) return res; 
		 
		//-------------------------------Данные для печати кроме названия карты-----------
    
		if ( col>0 )  {
		    for ( int g = 0; g < col; g++ )  {
			res = hw->ECR_PrintString( mas[g].c_str());
			if ( res != 0 ) return res;
		    }
		}
    
		//-------------------------------Подпись клиента и кассира--------------------------------
		res = hw->ECR_PrintString(" ");
		if ( res != 0 ) return res;
    
		temp = "";
		for ( unsigned int i = 0; i < hw->ECR_GetCharLineLength(); i++ )  {
		    temp = temp+"_";
		}
		res = hw->ECR_PrintString(temp.ascii());
		if ( res != 0 ) return res;
    
		res = PrintCenterString(HwCodec->fromUnicode(DefCodec->toUnicode("(ПОДПИСЬ КЛИЕНТА)")));
		if ( res != 0 ) return res;
	  
		res = hw->ECR_PrintString(" ");
		if ( res != 0 ) return res;    
    
		res = hw->ECR_PrintString(temp.ascii());
		if ( res != 0 ) return res;
    
		res = PrintCenterString(HwCodec->fromUnicode(DefCodec->toUnicode("(ПОДПИСЬ КАССИРА)"))) ;
		if ( res != 0 ) return res;
    
		if ( hw->ECR_GetProtocol() == 1 ) {// штрих
		    res = PrintCenterString(HwCodec->fromUnicode(DefCodec->toUnicode("СПАСИБО")));
		    res = PrintCenterString(HwCodec->fromUnicode(DefCodec->toUnicode("ЗА ПОКУПКУ!")));
		    if ( res != 0 ) return res;
		} 
    
		res = hw->ECR_PrintString(temp.ascii());
		if ( res != 0 ) return res;	
		mas.clear();
	    }
	    break;
	}
    }
    
    if ((hw->ECR_GetProtocol() == 3)||(hw->ECR_GetProtocol() == 5)){
	res = hw->ECR_CloseNotFiscalDoc();
	if ( res != 0 ) return res;  
    }       
    else{	
	res = hw->ECR_PrintString(" ");
	if ( res != 0 ) return res;    
	res = hw->ECR_PrintString(" ");
	if ( res != 0 ) return res;    
	res = hw->ECR_PrintString(" ");
	if ( res != 0 ) return res;    
	res = hw->ECR_PrintString(" ");
	if ( res != 0 ) return res;    
    }
    if ( hw->ECR_GetProtocol() != 5){
	if ( client_check )    res = hw->ECR_PartialCut();
	else res = hw->ECR_FullCut();
	if ( res != 0 ) return res;     
    }
    
    //------------------------Попытка печати клише чека---------------------------
	 	 
    if (hw->ECR_GetProtocol() == 3){
	if (hw->ECR_ReadTypePrintHeadLine()==0){
	    if (hw->ECR_GetTypePrintHeadLine() == 0)
	    {
		res = hw->ECR_PrintDocHeadLine(); 
		if ( res != 0 )    
		    return res;
	    }
	}
    }
    else
	hw->ECR_PrintDocHeadLine(); 
    //------------------------------------------------------------------------------------------
    return 0;
}

//======================================================================//
int KkmRegistrations::KkmCloseCheck(double nal, double cred, double cont, Receipt *  receipt)
{
    if ( !init_flag )    return -2;
    double charge = 0.00;
    
    if (!hw->ECR_GetUseOneRegistration()) {
	charge = (-1)*(receipt->dsum + receipt->round_sum);
	//printf("СКИДКА - НАДБАВКА НА ЧЕК = %f\n",charge);
    }
    int kkm_res = hw->ECR_CloseCheck( nal, 0, cred, cont, 0, 0, charge, false);
    printf("KkmRegistration ----- hw->ECR_CloseCheck( nal = %02f, 0, cred = %02f, cont = %02f, 0, 0, charge = %02f, false) return %d\n", nal, cred, cont, charge, kkm_res);
    if ( kkm_res != 0 )    return kkm_res;
    
    return 0;
}
//======================================================================//
// заполняет строку в чеке на ширину чека типа "ИТОГ       120р"
// str 1 - "ИТОГ"
// sep -  " " ( по умолчанию)
// str2 - "120р"
// type - тип : 0 - обычная (по умолчанию)    1 - широкая
//
int KkmRegistrations::KkmPrintSeparatorString(QString str1, QString str2, QString sep, int type)
{
//    printf("int KkmRegistrations::KkmPrintSeparatorString(%s, %s, %s, %d)", str1.ascii(), str2.ascii(), sep.ascii(), type);
        
    QString res = str1;
    double str_len = stringwide;
    if ( type == 1 )
	str_len = stringwide / 2;
    
//    printf("str1.len = [%d], str2.len  = [%d], stringwide = [%d]\n", str1.length(), str2.length(), stringwide);
            
    if ( ((str2.length() + str1.length()) > str_len) && ( str2.length() <= str_len ) && ( str1.length() <= str_len ) ) {
	int res = KkmPrintSeparatorString(str1, "", sep, type);
	if ( res != 0 ) return res;
	res = KkmPrintSeparatorString("", str2, sep, type);
	return res;	
    }
        
    int gg = 0;
    gg  = ( str_len  - str2.length() - str1.length() ) / sep.length() ;
    for ( int i = 0; i < gg; i++ )  res += sep;
    res += str2;
    
    if ( type == 0 )  return KkmPrintString(res.ascii(), false);
    if ( type == 1 ) return KkmPrintWideString(res.ascii(), false);
    
    return 0;
}
//======================================================================//
int KkmRegistrations::KkmPrintCopyCheck(CopyInfo * copy, Receipt * receipt, int section_num)
{
    if ( !init_flag )    return -2;
    
    if ( hw->ECR_GetProtocol() == 4 && hw->ECR_GetEducationMode() ) return 0;
    if ( hw->ECR_GetProtocol() == 5 && hw->ECR_GetEducationMode() ) return 0;
    
    int kkm_res = 0;
    
    string s;
    QString codeds;
    QString ss;
    QString dts;
    
    bool PrintHeadLineAfterDoc = false;// по умолчанию 
    if (hw->ECR_GetProtocol() == 3){
	if (hw->ECR_ReadTypePrintHeadLine()==0){
	    PrintHeadLineAfterDoc = hw->ECR_GetTypePrintHeadLine();
	    if (PrintHeadLineAfterDoc == 1) {
		kkm_res = hw->ECR_PrintDocHeadLine(); 
		if ( kkm_res != 0 )    return kkm_res;
		for (unsigned int i = 0; i<stringwide;i++)
		    ss += "=";
		kkm_res = KkmPrintString( ss.ascii(),false);
		if ( kkm_res != 0 )    return kkm_res;
	    }
	}
    }	
    
	 
    bool MSTAR = false; 
    if ((hw->ECR_GetProtocol() == 1) && (DefCodec->toUnicode(hw->ECR_GetUDescription()) == QString("MSTAR-TK"))) {
	MSTAR = true;
	kkm_res = hw->ECR_PrintDocHeadLine(); 
	if ( kkm_res != 0 )    return kkm_res;
    }
    
    //-----(+)-----Zuskin-----17/10/2011-----
    if (hw->ECR_GetProtocol() == 8){ // ORION-FR01K
	kkm_res = hw->ECR_PrintDocHeadLine(); 
	if ( kkm_res != 0 )    return kkm_res;
   }
    //-------------------------------------------------
    
    /********* позиции ***********/
    kkm_res = KkmPrintCheckPositions(receipt, section_num, copy->GetReceiptType() == 1);    
    if ( kkm_res != 0)    return kkm_res;  
    kkm_res = KkmPrintString("");   
    if ( kkm_res != 0)    return kkm_res;  	 
    
    /********** обучающий режим *********/
    if (hw->ECR_GetEducationMode() == 1)  {
	kkm_res = PrintBoldCenterString(HwCodec->fromUnicode(DefCodec->toUnicode("НЕФИСКАЛЬНЫЙ ЧЕК")));
	if ( kkm_res != 0)    return kkm_res;  
	
	kkm_res = PrintBoldCenterString(HwCodec->fromUnicode(DefCodec->toUnicode("  ")));
	if ( kkm_res != 0)    return kkm_res;  
	
	kkm_res = PrintBoldCenterString(HwCodec->fromUnicode(DefCodec->toUnicode("РЕЖИМ ОБУЧЕНИЯ")));
	if ( kkm_res != 0)    return kkm_res;  
    }
    
    if ( section_num  < 1 ) {
	KkmPrintCopyCheckInfoByProtocol(copy, receipt, section_num);
	if ( kkm_res != 0)    return kkm_res;  	 
	
	kkm_res = KkmPartialCutPrintDocHeadLine(PrintHeadLineAfterDoc);
	if ( kkm_res != 0)    return kkm_res;  
    }
    
    return 0;
}

//======================================================================//

int KkmRegistrations::KkmPrintCopyCheckInfoByProtocol(CopyInfo * copy, Receipt * receipt, int section_num) {
// ДАЛЕЕ СДЕЛАТЬ ПЕЧАТЬ В ЗАВИСИМОСТИ ОТ  МОДЕЛИ ККМ!!!!!!    
    int kkm_res = 0;
    
    double receipt_result_sum = receipt->GetResultSum();
    double payment_nal_sum = copy->GetNalSum();
    double payment_cred_sum = copy->GetCredSum();
    double payment_cont_sum = copy->GetContSum();
    double payment_client_sum = copy->GetClientSum();
    double payment_change_sum = payment_client_sum - receipt_result_sum;
    
    if ( section_num != -1 )  {
	receipt_result_sum = receipt->GetSectionResultSum(section_num);
	payment_nal_sum = receipt->GetSectionResultSum(section_num);
	payment_cred_sum = 0;
	payment_cont_sum = 0;
	payment_client_sum = receipt->GetSectionResultSum(section_num);
	payment_change_sum = payment_client_sum - receipt_result_sum;
    }
      
    
    string s;
    QString codeds;
    QString ss;
    QString dts;
    
    
    int KKMProt = hw->ECR_GetProtocol();
    switch(KKMProt)
    {	
	 case 1: {
		 // <<<<<<<<<<<<<     SHTRIH    >>>>>>>>>>>>>>>>>
		 if ( hw->ECR_GetCharLineLength() <= 32 ) s = "";
		 else s = "KKM ";
		 s += " ";
		 string tmp = hw->ECR_GetSerialNumber();
		 for ( int i = 0; i < 8 - tmp.length(); i++ )	 s += '0';  
		 s += tmp;
		 s +=' ';		
	
		 hw->ECR_ReadStatus();
		 s += HwCodec->fromUnicode(DefCodec->toUnicode("ИНН "));
		 tmp = hw->ECR_GetINN(); 
		// puts("---------------------->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		// printf("KKMREGISTRATION tmp.length()=[%d]\n", tmp.length());
		// for (int i=0; i < tmp.length(); i++)
		//     printf("tmp[%d]=[%d]\n",i,tmp[i]);
		// puts("---------------------->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		 
		 if (tmp.length()==0)
		     for ( int i = 0; i < 12 - tmp.length(); i++ )	 s += '?'; 
		 else
		     for ( int i = 0; i < 12 - tmp.length(); i++ )	 s += '0'; 
		 s += tmp;
		 tmp = " #";
		 
		 for ( int i = 0; i < 4 - copy->GetLengthCheckNum(); i++ )     tmp += '0';
		 tmp += copy->GetCheckNum();
		 
		 //printf("s = %s tmp = %s \n", s.c_str(), tmp.c_str());
		 kkm_res = KkmPrintSeparatorString(s, tmp, " ");
		 if ( kkm_res != 0 )	  return kkm_res;
		 
		 //_______________________________________________________________________________________
		 dts = copy->GetDateTime();
		 //printf("date time in copy check = %s \n", dts.ascii());
		 dts.remove(16, 3);
		 dts.remove(0,2);
		 dts.insert(0, dts.right(8).left(2));
		 dts.remove(8, 2);
		 dts.insert(8 ,dts.left(4).right(2));
		 dts.remove(2, 2);
		 dts.replace(2, 1, ".");
		 dts.replace(5, 1, ".");
		 
		 s = dts.ascii();
		 //printf("date time in copy check = %s \n", s.c_str());
		 kkm_res = KkmPrintSeparatorString(s.c_str(), HwCodec->fromUnicode(DefCodec->toUnicode("СИСТ.АДМИНИСТРАТОР")), " ");
		 if ( kkm_res != 0 )	  return kkm_res;
		 
		 //_______________________________________________________________________________________
		 s = "N";
		 for ( int i = 0 ; i < (int)(4 - copy->GetLengthDocumentNum()); i ++ ) s += "0"; 
		 s += copy->GetDocumentNum();
		 
		 //-----(*)-----Zuskin-----17/05/2011-----
		 if (copy->GetReceiptType() == 0) {		 
		    kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ПРОДАЖА")), s, " ");
		    if ( kkm_res != 0 )	  return kkm_res;
		 }
		 else {
		    kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ВОЗВРАТ ПРОДАЖИ")), s, " ");
		    if ( kkm_res != 0 )	  return kkm_res;
		 }	    
		 //---------------------------------------	
		 
		 //_______________________________________________________________________________________ 
		 kkm_res = KkmPrintSeparatorString( "", "=" + QString::number(receipt->GetSectionResultSum(section_num), 'f', 2), " " );
		 if ( kkm_res != 0 )	  return kkm_res;	
		 
		 //_______________________________________________________________________________________
		 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ИТОГ")), "=" + QString::number(receipt->GetSectionResultSum(section_num), 'f', 2), " ", 1 );
		 if ( kkm_res != 0)    return kkm_res; 
	
		 //_______________________________________________________________________________________
		 if ( copy->GetReceiptType() == 0 )      {  
		     if ( payment_nal_sum != 0 )   {
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("НАЛИЧНЫМИ")), "=" + QString::number(payment_nal_sum, 'f', 2), " " );
			 if ( kkm_res != 0)    return kkm_res; 		
		     }	
	    
		     if ( payment_cred_sum != 0 )    {
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("КРЕДИТОМ")), "=" + QString::number(payment_cred_sum, 'f', 2), " " );
			 if ( kkm_res != 0)    return kkm_res;		
		     }
	  
		     if ( payment_cont_sum != 0 )  {
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ТАРОЙ")), "=" + QString::number(payment_cont_sum, 'f', 2), " " );
			 if ( kkm_res != 0)    return kkm_res;
		     }
	  
		     if ( payment_client_sum > receipt->sum )	  {
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("СДАЧА")), "=" + QString::number(payment_change_sum, 'f', 2), " " );
			 if ( kkm_res != 0)    return kkm_res;
		     }
		 } // где виды оплаты для возврата  ??????		 
	
		 //_______________________________________________________________________________________
		 codeds = HwCodec->fromUnicode(DefCodec->toUnicode("      "));
		 s = codeds.ascii();
		 kkm_res = KkmPrintString(s.c_str(), false);
		 if ( kkm_res != 0 )    return kkm_res;
		 
		 break;
	     }
	 case 2: {/*<<<<<<<<<<<<<     ATOL       >>>>>>>>>>>>>>>>>*/
		 /********* ИТОГ ************/	
		 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ИТОГ")), "=" + QString::number(receipt_result_sum, 'f', 2), " ", 1 );
		 if ( kkm_res != 0)    return kkm_res; 
		 
		 /************* ПРОДАЖА ***************/
		 if ( copy->GetReceiptType() == 0 )      {
		     int payments = 0;
		     /************* кредитом *****************/
		     if ( payment_cred_sum != 0 )	  {		
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("КРЕДИТОМ")), "=" + QString::number(payment_cred_sum, 'f', 2), " ");
			 if ( kkm_res != 0)    return kkm_res; 
			 payments ++;
		     }
	    
		     /************* тарой *****************/
		     if ( payment_cont_sum != 0 )	  {
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ТАРОЙ")), "=" + QString::number(payment_cont_sum, 'f', 2), " ");
			 if ( kkm_res != 0)    return kkm_res; 
			 payments ++;
		     }	
					 
		     /************* сумма наличными **************/
		     if ( (payments > 0) && (payment_nal_sum != 0) )	  {
			 ss = QString::number(payment_nal_sum, 'f', 2);
			 payments ++;
		     } else if ( (payments == 0) && (payment_client_sum != 0) )
			 ss = QString::number(payment_client_sum, 'f', 2);
		     else if ( payments == 0 )
			 ss = QString::number(receipt_result_sum, 'f', 2);
		     else payments = -1;
	    
		     /*********** наличными *****************/
		     if ( payments != -1 )	  {
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("НАЛИЧНЫМИ")), "=" + ss, " ");
			 if ( kkm_res != 0)    return kkm_res; 
		     }
	    
		     /**************** сумма клиента *******************/  	    
		     if ( (payments > 0) && (payment_client_sum > receipt_result_sum) )  {
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ПОЛУЧЕНО")), "=" + QString::number(payment_client_sum, 'f', 2), " ");
			 if ( kkm_res != 0)    return kkm_res; 	
		     }
		     /************** СДАЧА ******************/
		     if ( payment_client_sum > receipt_result_sum )	  {
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("СДАЧА")), "=" + QString::number(payment_change_sum, 'f', 2), " ");
			 if ( kkm_res != 0)    return kkm_res; 
		     }
		 }
		 
		 //_______________________________________________________________________________________	
		 
		 codeds = HwCodec->fromUnicode(DefCodec->toUnicode("ДОК. "));
		 s = codeds.ascii();
		 for(int i = 0 ; i < (int)(8 - copy->GetLengthDocumentNum()); i ++)	  s += '0';
		 s += copy->GetDocumentNum();
		 kkm_res = KkmPrintString(s.c_str() , false);
		 if(kkm_res != 0)	  return kkm_res;
		 
		 s = '#';
		 for(int i = 0; i < 4 - copy->GetLengthCheckNum(); i++)	  s += '0';
		 s += copy->GetCheckNum();
		 string mys = s;
		 //_______________________________________________________________________________________	
		 dts = copy->GetDateTime();
		 dts.remove(16, 3);
		 dts.remove(0,2);
		 dts.insert(0, dts.right(8).left(2));
		 dts.remove(8, 2);
		 dts.insert(8 ,dts.left(4).right(2));
		 dts.remove(2, 2);
		 dts.replace(2, 1, ".");
		 dts.replace(5, 1, ".");
		 
		 s = "K30 ";
		 s += dts.ascii();
		 int d = stringwide - mys.length() - s.length();
		 while ( d>0 )  {	    mys +=' ';	    d--;	}
		 mys +=s;
		 
		 kkm_res = KkmPrintString(mys.c_str(), false);
		 if(kkm_res != 0)	  return kkm_res;
		 s = "KKM";
		 string sernum = hw->ECR_GetSerialNumber();
		 d = stringwide - s.length() -sernum.length();
		 while ( d > 0 )      {	    s += ' ';	    d--;	}
		 s += sernum;
		 kkm_res = KkmPrintString(s.c_str(), false);
		 if ( kkm_res != 0 )	  return kkm_res;
		 break;
	     } 
	 case 3:{
		 // <<<<<<<<<<<<<     МЕРКУРИЙ   >>>>>>>>>>>>>>>>>
		 // puts("<<<<<<<<<<<<<     МЕРКУРИЙ   >>>>>>>>>>>>>>>>>");
		 int col_reg = 1;//количество регистраций в чеке
		 QString tmp_ss;
		 QString s1;
		 tmp_ss = tmp_ss.setNum(col_reg); 
		 if (col_reg<10){
		     s1 = "0";
		     s1 = s1 + tmp_ss;
		 }
		 else
		     s1 = QString::number(col_reg); 
		 s1 += "-0-0";
		 s1 += " ";
		 double ss = receipt_result_sum;
		 s1 += QString::number(ss, 'f', 2);
		 s1 += "*1.00";
				
		 QString bstr = '='+QString::number(receipt_result_sum, 'f', 2);
		 kkm_res = KkmPrintSeparatorString(s1, bstr, " ");
		 if ( kkm_res != 0 )    return kkm_res;
				
		 if ( copy->GetReceiptType() == 0 ){
		     s1 = HwCodec->fromUnicode(DefCodec->toUnicode("ПРОДАЖА"));
		     tmp_ss = QString::number(receipt_result_sum, 'f', 2);
		 }    else{
		     if (payment_nal_sum>0){
			 if (payment_cred_sum>0){
			     s1 = HwCodec->fromUnicode(DefCodec->toUnicode("ВОЗВР. ПРОД. НАЛ/БН."));
			     if (payment_cont_sum>0){
				 tmp_ss =  QString::number(payment_nal_sum, 'f', 2)+"/";
				 tmp_ss += QString::number(payment_cred_sum+payment_cont_sum, 'f', 2);
			     }
			     else{
				 tmp_ss =  QString::number(payment_nal_sum, 'f', 2)+"/";
				 tmp_ss += QString::number(payment_cred_sum, 'f', 2);  
			     }        
			 }
			 else{
			     s1 = HwCodec->fromUnicode(DefCodec->toUnicode("ИТОГ ВОЗВРАТА ПРОД."));
			     tmp_ss =  QString::number(payment_nal_sum, 'f', 2);
			 }
		     }
		     else{
			 if (payment_cred_sum>0){
			     s1 = HwCodec->fromUnicode(DefCodec->toUnicode("ВОЗВРАТ ПРОД. БН."));
			     if (payment_cont_sum>0){
				 tmp_ss =  QString::number(payment_cred_sum+payment_cont_sum, 'f', 2);
			     }
			     else{
				 tmp_ss =  QString::number(payment_cred_sum, 'f', 2);
			     }
			 }
		     }
		 }	
				
		 kkm_res = KkmPrintSeparatorString(s1, tmp_ss, " ");
		 if ( kkm_res != 0 )    return kkm_res;
				
		 //_______________________________________________________________________________________
		 s1= HwCodec->fromUnicode(DefCodec->toUnicode("ОПЛАТА "));
		 bool flag_change = true;
		 if ( copy->GetReceiptType() == 0 )      {  
		     if (payment_nal_sum>0){
			 if(payment_cred_sum>0){
			     if(payment_cont_sum>0){					    
				 s1 +=HwCodec->fromUnicode(DefCodec->toUnicode("НАЛ/КРЕД/ТИП 3"));
				 tmp_ss = QString::number(payment_nal_sum, 'f', 2)+"/"; 
				 tmp_ss += QString::number(payment_cred_sum, 'f', 2) + "/";	 	
				 tmp_ss += QString::number(payment_cont_sum, 'f', 2); 
			     }
			     else{
				 //-----------
				 s1 +=HwCodec->fromUnicode(DefCodec->toUnicode("НАЛ/КРЕДИТ"));
				 tmp_ss = QString::number(payment_nal_sum, 'f', 2)+"/"; 
				 tmp_ss += QString::number(payment_cred_sum, 'f', 2); 
			     }
			 }
			 else{					
			     if(payment_cont_sum>0){
				 //----------
				 s1 +=HwCodec->fromUnicode(DefCodec->toUnicode("НАЛ/ТИП 3"));
				 tmp_ss = QString::number(payment_nal_sum, 'f', 2)+"/";
				 tmp_ss += QString::number(payment_cont_sum, 'f', 2); 	 
			     }
			     else{
				 s1 +=HwCodec->fromUnicode(DefCodec->toUnicode("НАЛИЧНЫМИ"));
				 tmp_ss = QString::number(payment_nal_sum, 'f', 2);  
			     }
			 }	  
		     }
		     else{
			 if(payment_cred_sum>0){
			     if(payment_cont_sum>0){					    
				 s1 +=HwCodec->fromUnicode(DefCodec->toUnicode("КРЕД/ТИП 3"));
				 tmp_ss = QString::number(payment_cred_sum, 'f', 2)+"/";
				 tmp_ss += QString::number(payment_cont_sum, 'f', 2);
				 flag_change = false;
			     }
			     else{
				 s1 +=HwCodec->fromUnicode(DefCodec->toUnicode("В КРЕДИТ"));
				 tmp_ss = QString::number(payment_cred_sum, 'f', 2);	 
				 flag_change = false;
			     }
			 }	
			 else{					
			     if(payment_cont_sum>0){
				 s1 +=HwCodec->fromUnicode(DefCodec->toUnicode("ТИПОМ 3"));
				 tmp_ss = QString::number(payment_cont_sum, 'f', 2);
				 flag_change = false;
			     }
			 }		  
		     }
		     
		     kkm_res = KkmPrintSeparatorString(s1,tmp_ss, " " );
		     if ( kkm_res != 0)    return kkm_res; 
		 }
		 //_______________________________________________________________________________________
		 if (( copy->GetReceiptType() == 0 )&&(flag_change)){  
		     kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("СДАЧА")), QString::number(payment_change_sum, 'f', 2), " " );
		     if ( kkm_res != 0)    return kkm_res;
		 } 
		 //_______________________________________________________________________________________ 
		 s1 = "";
		 for (unsigned int i = 0; i<stringwide;i++)
		     s1 += "=";
		 kkm_res = KkmPrintString( s1.ascii(),false);
		 if ( kkm_res != 0 )    return kkm_res;
		 //_______________________________________________________________________________________
		 s1 = HwCodec->fromUnicode(DefCodec->toUnicode("ККМ"));
		 s1 += " ";
		 string tmp = hw->ECR_GetSerialNumber();
		 for ( int i = 0; i < 8 - tmp.length(); i++ )	 s += '0';  
		 s1 += tmp;
		 s1 +=' ';		 
		 kkm_res = KkmPrintString( s1.ascii(),false);
		 if ( kkm_res != 0 )    return kkm_res;	
		 //_______________________________________________________________________________________		
		 s1 = HwCodec->fromUnicode(DefCodec->toUnicode("НЕФИСКАЛЬНЫЙ РЕЖИМ"));		
		 kkm_res = KkmPrintString( s1.ascii(),false);
		 if ( kkm_res != 0 )    return kkm_res;		
		 //_______________________________________________________________________________________			
		 QString dts = copy->GetDateTime();
		 //printf("date time in copy check = %s \n", dts.ascii());
		 dts.remove(16, 3);
		 //printf("dts.ascii() 1 = %s \n", dts.ascii());
		 //dts.remove(0,2);
		 dts.insert(0, dts.right(8).left(2));
		 //printf("dts.ascii() 2 = %s \n", dts.ascii());
		 dts.remove(10, 2);
		 //printf("dts.ascii() 3 = %s \n", dts.ascii());
		 dts.insert(10 ,dts.left(6).right(4));
		 //printf("dts.ascii() 4 = %s \n", dts.ascii());
		 dts.remove(2, 4);
		 //printf("dts.ascii() 5 = %s \n", dts.ascii());
		 dts.replace(2, 1, "/");
		 //	printf("dts.ascii() 6 = %s \n", dts.ascii());
		 dts.replace(5, 1, "/");
		 //	printf("dts.ascii() 7 = %s \n", dts.ascii());
		 
		 kkm_res = KkmPrintString( dts.ascii(),false);
		 if ( kkm_res != 0 )    return kkm_res;
		 //_______________________________________________________________________________________			
		 s1 = HwCodec->fromUnicode(DefCodec->toUnicode("НД "));
		 QString tmpp = copy->GetDocumentNum();
		 //printf("copy->GetDocumentNum() = |%s|\n",tmpp.ascii());
		 int len = tmpp.length();
		 while(5-len>0 ){
		     s1 +="0";
		     len++;
		 }	  
		 s1 +=tmpp;
		 kkm_res = KkmPrintString( s1.ascii(),false);
		 if ( kkm_res != 0 )    return kkm_res;	
		 //_______________________________________________________________________________________			
		 s1 = HwCodec->fromUnicode(DefCodec->toUnicode("НЧ "));
		 tmpp = copy->GetCheckNum();
		 //	printf("copy->GetCheckNum() = |%s|\n",tmpp.ascii());
		 len = tmpp.length();
		 while(5-len>0 ){
		     s1 +="0";
		     len++;
		 }	
		 s1 +=tmpp;
		 kkm_res = KkmPrintString( s1.ascii(),false);
		 if ( kkm_res != 0 )    return kkm_res;	
		 //_______________________________________________________________________________________			
		 s1 = HwCodec->fromUnicode(DefCodec->toUnicode("КАССИР 00"));
		 kkm_res = KkmPrintString( s1.ascii(),false);
		 if ( kkm_res != 0 )    return kkm_res;	
		 
		 kkm_res = KkmPrintString(" ",false);	
		 if ( kkm_res != 0 )    return kkm_res;        
		 
		 kkm_res = hw->ECR_CloseNotFiscalDoc();
		 if ( kkm_res != 0 )    return kkm_res;
		 
		 break;
	     }
	 case 4: {
		 // <<<<<<<<<<<<<     CASBY    >>>>>>>>>>>>>>>>>
		 QString bstr = "HM ";
		 bstr = bstr+hw->ECR_GetSerialNumber();
		 
		 QString dts = copy->GetDateTime();
		 //printf("date time in copy check = %s \n", dts.ascii());
		 dts.remove(16, 3);
		 dts.remove(0,2);
		 dts.insert(0, dts.right(8).left(2));
		 dts.remove(8, 2);
		 dts.insert(8 ,dts.left(4).right(2));
		 dts.remove(2, 2);
		 dts.replace(2, 1, ".");
		 dts.replace(5, 1, ".");
		 
		 s = dts.ascii();
		 //printf("date time in copy check = %s \n", s.c_str());
		 kkm_res = KkmPrintSeparatorString(bstr, dts, " ");
		 if ( kkm_res != 0 )    return kkm_res;
		 //_______________________________________________________________________________________
		 
		 bstr = HwCodec->fromUnicode(DefCodec->toUnicode("НОМЕР СМЕНЫ "));
		 
		 int num = hw->ECR_GetSession()+1;
		 QString tmpp =QString::number(num);
		 while(4-tmpp.length()>0 ){
		     tmpp.insert(0,"0");
		 }
		 bstr = bstr +tmpp;
		 
		 dts = HwCodec->fromUnicode(DefCodec->toUnicode("НОМЕР ДОКУМ "));
		 //dts = dts + "";
		 tmpp = copy->GetDocumentNum();
		 while(4-tmpp.length()>0 ){
		     tmpp.insert(0,"0");
		 }
		 dts = dts + tmpp;
		 kkm_res = KkmPrintSeparatorString(bstr, dts, " ");
		 if ( kkm_res != 0 )    return kkm_res;
		 //_______________________________________________________________________________________
		 bstr = HwCodec->fromUnicode(DefCodec->toUnicode("КАССИР 01"));
		 tmpp = copy->GetCheckNum();
		 while(4-tmpp.length()>0 ){
		     tmpp.insert(0,"0");
		 }
		 dts = HwCodec->fromUnicode(DefCodec->toUnicode("ПРОДАЖА N "));
		 dts = dts+tmpp;
		 kkm_res = KkmPrintSeparatorString(bstr, dts, " ");
		 if ( kkm_res != 0 )    return kkm_res;
		 //_______________________________________________________________________________________
		 kkm_res = KkmPrintString( "------------------------------------",false);
		 if ( kkm_res != 0 )	  return kkm_res;	
		 //_______________________________________________________________________________________
		 kkm_res = KkmPrintString( "1.",false);
		 if ( kkm_res != 0 )	  return kkm_res;	
		 //_______________________________________________________________________________________					
		 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ОТДЕЛ 01")), "=" + QString::number(receipt_result_sum, 'f', 2), " ");
		 if ( kkm_res != 0)    return kkm_res; 				
		 //_______________________________________________________________________________________
		 kkm_res = KkmPrintString( "------------------------------------",false);
		 if ( kkm_res != 0 )	  return kkm_res;
		 //_______________________________________________________________________________________			
		 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ИТОГ")), QString::number(receipt_result_sum, 'f', 2), " ");
		 if ( kkm_res != 0)    return kkm_res; 
		 //_______________________________________________________________________________________
		 if ( copy->GetReceiptType() == 0 )      {  
		     if ( payment_nal_sum != 0 )   {
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("НАЛИЧНЫЕ")), QString::number(payment_nal_sum, 'f', 2), " " );
			 if ( kkm_res != 0)    return kkm_res; 		
		     }	
	    
		     if ( payment_cred_sum != 0 )    {
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("Вид оплаты 2")),  QString::number(payment_cred_sum, 'f', 2), " " );
			 if ( kkm_res != 0)    return kkm_res;		
		     }
		     
		     if ( payment_cont_sum != 0 )  {
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("Вид оплаты 3")),  QString::number(payment_cont_sum, 'f', 2), " " );
			 if ( kkm_res != 0)    return kkm_res;
		     }
	  
		     if ( payment_client_sum > receipt_result_sum )	  {
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("СДАЧА")),  QString::number(payment_change_sum, 'f', 2), " " );
			 if ( kkm_res != 0)    return kkm_res;
		     }
		 }
		 //_______________________________________________________________________________________
		 break;
	     }
	 case 5:{
		 /*<<<<<<<<<<<<<     PRIM      >>>>>>>>>>>>>>>>>*/
		 //puts("<<<<<<<<<<     PRIM      >>>>>>>>>>>>");
		 /********* ИТОГ ************/	
		 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ИТОГ")), "=" + QString::number(receipt_result_sum, 'f', 2), " ");
		 if ( kkm_res != 0)    return kkm_res; 
	
		 /************* кредитом *****************/
		 if ( payment_cred_sum != 0 )	  {		
		     kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ТИП ОПЛАТЫ 2")), "=" + QString::number(payment_cred_sum, 'f', 2), " ");
		     if ( kkm_res != 0)    return kkm_res; 
		 }
	    
		 /************* тарой *****************/
		 if ( payment_cont_sum != 0 )	  {
		     kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ТИП ОПЛАТЫ 3")), "=" + QString::number(payment_cont_sum, 'f', 2), " ");
		     if ( kkm_res != 0)    return kkm_res; 
		 }	
					 
		 /************* сумма наличными **************/
		 if (payment_nal_sum != 0)  {
		     kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("НАЛИЧНЫЕ")), "=" + QString::number(payment_nal_sum, 'f', 2), " ");
		     if ( kkm_res != 0)    return kkm_res; 
		 }
				
		 /**************** сумма клиента *******************/  	    
		 if (payment_client_sum > receipt_result_sum)  {
		     kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ПОЛУЧЕНО")), "=" + QString::number(payment_client_sum, 'f', 2), " ");
		     if ( kkm_res != 0)    return kkm_res; 	
		 }
		 /************** СДАЧА ******************/
		 if ( payment_client_sum > receipt_result_sum )	  {
		     kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("СДАЧА")), "=" + QString::number(payment_change_sum, 'f', 2), " ");
		     if ( kkm_res != 0)    return kkm_res; 
		 }
		 /******************************************/
		 kkm_res = hw->ECR_PrintString("          ",false);
		 if ( kkm_res != 0 )	  return kkm_res;	 
				
		 /************* ПРОДАЖА ***************/
		 if ( copy->GetReceiptType() == 0 )      {
		     kkm_res = KkmPrintString(HwCodec->fromUnicode(DefCodec->toUnicode("ПРОДАЖА")),false);
		     if ( kkm_res != 0 )	  return kkm_res;	
		 }
		 else{
		     kkm_res = KkmPrintString(HwCodec->fromUnicode(DefCodec->toUnicode("ВОЗВРАТ")),false);
		     if ( kkm_res != 0 )	  return kkm_res;	 
		 }
		 break;
	     }
	     //-----(+)-----Zuskin-----17/10/2011-----
	      case 8:{ // -------------------------------- ORION-FR01K
		      kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ИТОГО")), QString::number(receipt_result_sum, 'f', 2), " ");
		 if ( kkm_res != 0)    return kkm_res; 
		 if ( copy->GetReceiptType() == 0 )      {  
		     kkm_res = KkmPrintString(HwCodec->fromUnicode(DefCodec->toUnicode("Внесено:")));
		     if ( kkm_res != 0)    return kkm_res;
		 }
		 else {
		      kkm_res = KkmPrintString(HwCodec->fromUnicode(DefCodec->toUnicode("Возврат:")));
		     if ( kkm_res != 0)    return kkm_res;
		 }
		     if ( payment_nal_sum != 0 )   {
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("НАЛИЧНЫМИ")), QString::number(payment_nal_sum, 'f', 2), " " );
			 if ( kkm_res != 0)    return kkm_res; 		
		     }	
	    
		     if ( payment_cred_sum != 0 )    {
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ПЛ. КАРТА1")),  QString::number(payment_cred_sum, 'f', 2), " " );
			 if ( kkm_res != 0)    return kkm_res;		
		     }
		     
		     if ( payment_cont_sum != 0 )  {
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ПЛ. КАРТА2")),  QString::number(payment_cont_sum, 'f', 2), " " );
			 if ( kkm_res != 0)    return kkm_res;
		     }
	  
		     if ( payment_client_sum > receipt_result_sum )	  {
			 kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("Сдача:")),  QString::number(payment_change_sum, 'f', 2), " " );
			 if ( kkm_res != 0)    return kkm_res;
		     }     
		  kkm_res = hw->ECR_CloseNotFiscalDoc();
		  if ( kkm_res != 0 )	  return kkm_res;
	     }
	     //-------------------------------------------------
    }
    return 0;
}

//======================================================================//
int KkmRegistrations::KkmPartialCutPrintDocHeadLine(int PrintHeadLineAfterDoc) {
    int kkm_res = 0;
    int Pr = hw->ECR_GetProtocol();
    //printf("Protocol = %d\n",Pr);
    int UModel;
    switch (Pr)
    {
	 case 1:  {
		 UModel =  hw->ECR_GetUModel();
		 switch(UModel){
		 case 6:{ 
			 kkm_res = KkmPrintString(" ", false);
			 break;
		     }
		 case 9:{
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 hw->ECR_PartialCut();  
			 break;
		     }
		 default:{
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 hw->ECR_PartialCut();  
			 break;  
		     }
		 }
		 
		 if ((hw->ECR_GetProtocol() == 1) && (DefCodec->toUnicode(hw->ECR_GetUDescription()) == QString("MSTAR-TK"))) {
		     kkm_res = hw->ECR_PrintDocHeadLine();
		 }
		 break;
	     }
	 case 3:{
		 hw->ECR_PartialCut();  
		 if (PrintHeadLineAfterDoc == 0)
		 {
		     kkm_res = hw->ECR_PrintDocHeadLine(); 
		     if ( kkm_res != 0 )    return kkm_res;
		 }
		 break;
	     }
	 case 4:{
		 kkm_res = hw->ECR_PrintServiceDoc();
		 break;
	     }
	 case 5:{
		 kkm_res = hw->ECR_CloseNotFiscalDoc();
		 break;
	     }
	 //-----(*)-----Zuskin-----17/10/2011-----
	 //default:{    
	 case 2: {    
	 //-------------------------------------------------  
		 UModel =  hw->ECR_GetUModel();
		 kkm_res = hw->ECR_PrintDocHeadLine();
		 if ((UModel == 35)||(UModel == 32)||(UModel == 30)||(UModel == 47)) {
		     hw->ECR_PartialCut();
		 }
	     }
    }
	 
    return kkm_res;
}

//======================================================================//	
int KkmRegistrations::KkmPartialCut() {
    int kkm_res = 0;
    int Pr = hw->ECR_GetProtocol();
    //printf("Protocol = %d\n",Pr);
    int UModel;
    switch (Pr)
    {
	 case 1:  {
		 UModel =  hw->ECR_GetUModel();
		 switch(UModel){
		 case 6:{ 
			 kkm_res = KkmPrintString(" ", false);
			 break;
		     }
		 case 9:{
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 hw->ECR_PartialCut();  
			 break;
		     }
		 default:{
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 kkm_res = KkmPrintString(" ", false);
			 hw->ECR_PartialCut();  
			 break;  
		     }
		 }
		 break;
	     }
	 case 3:{
		 hw->ECR_PartialCut();  
		 break;
	     }
	 case 4:{
		 kkm_res = hw->ECR_PrintServiceDoc();
		 break;
	     }
	 case 5:{
		 kkm_res = hw->ECR_CloseNotFiscalDoc();
		 break;
	     }
	 default:{
		 UModel =  hw->ECR_GetUModel();
		 if ((UModel == 35)||(UModel == 32)||(UModel == 30)||(UModel == 47)) {
		     hw->ECR_PartialCut();
		 }
	     }
    }
	 
    return kkm_res;
}
//======================================================================//	
int KkmRegistrations::KkmPrintDocHeadLine() {
        int kkm_res = 0;
    int Pr = hw->ECR_GetProtocol();
    //printf("Protocol = %d\n",Pr);
    int UModel;
    switch (Pr)
    {
	 case 1:  {
		 kkm_res = hw->ECR_PrintDocHeadLine();
		 break;
	     }
	 case 3:{
		 kkm_res = hw->ECR_PrintDocHeadLine(); 
		 if ( kkm_res != 0 )    return kkm_res;
	     }
	 case 4:{
		 kkm_res = hw->ECR_PrintServiceDoc();
		 break;
	     }
	 case 5:{
		 kkm_res = hw->ECR_CloseNotFiscalDoc();
		 break;
	     }
	 default: {
		 kkm_res = hw->ECR_PrintDocHeadLine();
	     }
    }
	 
    return kkm_res;
}

//======================================================================//	
int KkmRegistrations::KkmPrintReturnNotNalCheck(CopyInfo * copy, Receipt * receipt, const char* seller){
    if ( !init_flag )    return -2;
    
    if ( hw->ECR_GetEducationMode() ) return 0;
    
    if (hw->ECR_GetProtocol()!=5) return 356;
    
    int kkm_res = 0;
    
    string s;
    QString codeds;
    QString ss;
    QString dts;
    
    /*********** номер чека ***********/	
    ss = itoa(hw->ECR_GetCheckNumber()); 
    int len = ss.length();
    s = HwCodec->fromUnicode(DefCodec->toUnicode("ЧЕК="));
    while((5-len)>0)  { s +="0";  len++;    }
    s += ss.ascii();
    s +="=";
    kkm_res = KkmPrintSeparatorString(s.c_str(), " ", " ");
    if ( kkm_res != 0 )    return kkm_res;
    
    /*********** возврат ***********/	 
    kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ВОЗВРАТ")), " ", " ");
    if ( kkm_res != 0 )    return kkm_res;
    
    /*********** кассир ***********/	 
    kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("КАССИР")), HwCodec->fromUnicode(DefCodec->toUnicode(seller)), " ");
    if ( kkm_res != 0 )    return kkm_res;
	 
    /********* позиции ***********/
    kkm_res = KkmPrintCheckPositions(receipt);    
    if ( kkm_res != 0)    return kkm_res;  
    
    /******** итог ***********/
    double itogo = receipt->GetResultSum();
    kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("ИТОГ")), "=" + QString::number(itogo, 'f', 2), " ");
    if ( kkm_res != 0)    return kkm_res; 
	
    /************* кредитом *****************/
    if ( copy->GetCredSum() != 0 )	  {		
	kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("КРЕДИТ")), "=" + QString::number(copy->GetCredSum(), 'f', 2), " ");
	if ( kkm_res != 0)    return kkm_res; 
    } else{
	kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("КРЕДИТ")), "=" + QString::number(itogo, 'f', 2), " ");
	if ( kkm_res != 0)    return kkm_res; 	 	 
    }
	
    kkm_res = hw->ECR_CloseNotFiscalDoc();
    return kkm_res;	
    
}

//#######################################--KKM--CREATE--CHECK--SECTION--###############################//
//======================================================================//
int KkmRegistrations::KkmPositionTaxes(int i, Receipt * receipt)
{
    
    int kkm_res = 0;
    
    double taxbase = receipt->CalculationPositionTaxBase(i);

    if ( taxbase == 1 ) return 0;
    
    char * str;
    QString s;
    int n = 0;
    
    double taxessum = 0;    
    for ( int j = 0; j < receipt->GetTaxElemRef(i)->num_rates; j++ )    {
	taxessum += receipt->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->taxsum;
    }
    
    if ( Round(taxessum * 100) / 100 == 0)    return 0;
    
    //ПЕЧАТАЕМ СПИСОК НАЛОГОВ ТЕКУЩЕЙ ПОЗИЦИИ
    for ( int j = 0; j < receipt->GetTaxElemRef(i)->num_rates; j++ )      {
	str = (char *) calloc(stringwide + 1, sizeof(char));
	strcat(str, HwCodec->fromUnicode(DefCodec->toUnicode("в т.ч. налог ")));
	strcat(str, HwCodec->fromUnicode(DbCodec->toUnicode(receipt->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->GetShortcut())));
	s.setNum(receipt->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->taxsum, 'f', 2);
	n = stringwide - strlen(str) - s.length() - 1;
	for(int k = 0; k < n; k ++)          strcat(str, " ");
	strcat(str, "=");
	strcat(str, s);
	kkm_res = KkmPrintString(str, true);
	free(str);
	if(kkm_res != 0)          return kkm_res;
    }
	
    //ПЕЧАТАЕМ ИТОГОВУЮ СУММУ НАЛОГОВ ТЕКУЩЕЙ ПОЗИЦИИ
    str = (char *) calloc(stringwide + 1, sizeof(char));
    strcpy(str, HwCodec->fromUnicode(DefCodec->toUnicode("Всего налогов")));
    kkm_res = KkmPrintString(str, true);
    free(str);
    if(kkm_res != 0)	return kkm_res;
    
    str = (char *) calloc(stringwide + 1, sizeof(char));
    s.setNum(taxessum, 'f', 2);
    n = stringwide - s.length() - 1;
    for ( int k = 0; k < n; k ++ )        strcat(str, " ");
    strcat(str, "=");
    strcat(str, s);
    kkm_res = KkmPrintString(str, true);
    free(str);
    if(kkm_res != 0)       return kkm_res;
    return 0;
}

//======================================================================//
int KkmRegistrations::KkmSumDiscount(int i, int d, Receipt * receipt)
{
  //log->PrintString(DEBLEVEL_REGFORM, INFO, "KkmRegistration::KkmSumDiscount(int i, int d, Receipt * receipt)\n");
  int kkm_res = 0;

  double discount_sum;
  if(d == 1)
     discount_sum = receipt->GetPosElemRef(i)->dsum;
  else
    discount_sum = receipt->dsum;
  
  QString s, ds;
  
  s = HwCodec->fromUnicode(DefCodec->toUnicode("Скидка"));
  kkm_res = KkmPrintString(s.ascii(), true);
    if(kkm_res != 0)
      return kkm_res;

  s = "";
  ds = QString::number(discount_sum, 'f', 2);
  for(int i = 0; i < stringwide - ds.length(); i++)
    s += ' ';
  s += ds;
  
  kkm_res = KkmPrintString(s.ascii(), true);
    if(kkm_res != 0)
      return kkm_res;

  return 0;
}

//======================================================================//
int KkmRegistrations::KkmSumCharge(int i, int d, Receipt * receipt)
{
  //log->PrintString(DEBLEVEL_REGFORM, INFO, "KkmRegistrations::KkmSumCharge(int d, Receipt * receipt)\n");
  int kkm_res = 0;

  double charge_sum;
  if(d == 1)
     charge_sum = receipt->GetPosElemRef(i)->dsum;
  else
    charge_sum = receipt->dsum;
  
  QString s, ds;
  
  s = HwCodec->fromUnicode(DefCodec->toUnicode("Надбавка"));
  kkm_res = KkmPrintString(s.ascii(), true);
  
    if(kkm_res != 0)
      return kkm_res;
  
  s = "";
  ds = QString::number(charge_sum, 'f', 2);
  for(int i = 0; i < stringwide - ds.length(); i++)
    s += ' ';
  s += ds;

  kkm_res = KkmPrintString(s.ascii(), true);
    if(kkm_res != 0)
      return kkm_res;

  return 0;
}

//======================================================================//
int KkmRegistrations::KkmPrintString(const char *str, bool print_control, bool wrap_flag)
// На вход должна подаваться строка в кодировке CP1251
{    
    /*printf("int KkmRegistrations::KkmPrintString(%s)\n",str);
    printf(" stringwide = (%d)\n", stringwide);
    printf(" strlen(str) = (%d)\n", strlen(str));*/
    
    int kkm_res = 0;
    char * caption = (char *) calloc(stringwide + 1, sizeof(char));
    if ( (int)strlen(str) <= stringwide )  {
	 for(int f = 0; f < stringwide; f++)      *(caption + f) = ' ';
	 for(int j = 0; j < (int) strlen(str); j++)      *(caption + j) = *(str + j);
	
	 kkm_res = hw->ECR_PrintString(caption, print_control);
	
	 if ( kkm_res != 0 )    {
	     free(caption);
	     return kkm_res;
	 }
    }  else  {
	int j, k;
	for(j = 0; j < stringwide; j++)      *(caption + j) = *(str + j);
	
	kkm_res = hw->ECR_PrintString(caption, print_control);
	
	if ( kkm_res != 0 )    {
	    free(caption);
	    return kkm_res;
	}
	
	if ( wrap_flag )    {
	    for(int f = 0; f < stringwide; f++)        *(caption + f) = ' ';
	    
	    for(k = 0; k < stringwide; k++)        *(caption + k) = *(str+ k + j);
	    
	    kkm_res = hw->ECR_PrintString(caption, print_control);
	    
	    if ( kkm_res != 0 )      {
		free(caption);
		return kkm_res;
	    }
	}
    }
    free(caption);
    return 0;
    
}

//======================================================================//
int KkmRegistrations::KkmPrintWideString(const char *str, bool print_control)
{
    //log->PrintString(DEBLEVEL_REGFORM, INFO, "KkmRegistrations::KkmPrintWideString(const char *str = %s, bool print_control)\n", str);
    int kkm_res = 0;
    int strw = (int) (stringwide / 2);
    char * caption = (char *) calloc(strw + 1, sizeof(char));
    ////printf("strw/2 = %d\n",(int)strw);
    ////printf("strlen(str) = %d\n",(int)strlen(str));
    
    if((int)strlen(str) <= strw)  {
	//puts("----------------------1--------------------");
	for(int f = 0; f < strw; f++)      *(caption + f) = ' ';	
	
	for(int j = 0; j < strw; j++)      *(caption + j) = *(str + j);
	////printf("caption = |%s|\n",caption);
	kkm_res = hw->ECR_PrintWideString(caption, print_control);
	if(kkm_res  != 0)    {
	    free(caption);
	    return kkm_res;
	}
    }  else  {
	//puts("----------------------2--------------------");
	int j, k;
	for(j = 0; j < strw; j++)      *(caption + j) = *(str + j);
	
	////printf("caption = |%s|\n",caption);
	kkm_res = hw->ECR_PrintWideString(caption, print_control);
	if(kkm_res != 0)    {
	    free(caption);
	    return kkm_res;
	}
	for(int f = 0; f < strw; f++)       *(caption + f) = ' ';
	
	for(k = 0; k < strw; k++)      *(caption + k) = *(str + k + j);
	
	kkm_res = hw->ECR_PrintWideString(caption, print_control);
	if(kkm_res != 0)    {
	    free(caption);
	    return kkm_res;
	}
    }
    free(caption);
    return 0;
}

//======================================================================//
int KkmRegistrations::KkmPrintHighString(const char *str, bool print_control)
{
  ////log->PrintString(DEBLEVEL_REGFORM, INFO, "KkmRegistrations::KkmPrintHighString(const char *str = %s, bool print_control)\n", str);
  int kkm_res = 0;
  int strw = (int) (stringwide / 2);
  char * caption = (char *) calloc(strw + 1, sizeof(char));

  if((int)strlen(str) <= strw)
  {
    for(int f = 0; f < strw / 2; f++)
      *(caption + f) = ' ';

    for(int j = 0; j < strw / 2; j++)
      *(caption + j) = *(str + j);
    kkm_res = hw->ECR_PrintHighString(caption, print_control);
    if(kkm_res  != 0)
    {
      free(caption);
      return kkm_res;
    }
  }
  else
  {
    int j, k;
    for(j = 0; j < strw; j++)
      *(caption + j) = *(str + j);
    kkm_res = hw->ECR_PrintHighString(caption, print_control);
    if(kkm_res != 0)
    {
      free(caption);
      return kkm_res;
    }
    for(int f = 0; f < strw; f++) 
      *(caption + f) = ' ';
    for(k = 0; k < strw; k++)
      *(caption + k) = *(str + k + j);
    kkm_res = hw->ECR_PrintHighString(caption, print_control);
    if(kkm_res != 0)
    {
      free(caption);
      return kkm_res;
    }
  }
  free(caption);
  return 0;
}
//======================================================================//
/*-------Added by Shonda------*/
int KkmRegistrations::KkmPrintDocumentInfo()
{
   // puts("int KkmRegistrations::KkmPrintDocumentInfo()");
    //печать всяких номеров
    int kkm_res = 0;
    QString s = "";
    
    //=====(+)====31/03/2011=================================
    if ((hw->ECR_GetProtocol() == 1) && (DefCodec->toUnicode(hw->ECR_GetUDescription()) == QString("MSTAR-TK"))) {
	     kkm_res = hw->ECR_PrintDocHeadLine(); 
	     if ( kkm_res != 0 )    return kkm_res;
	 }
    //===================================================
    
//-----(*)-----Zuskin-----17/10/2011-----
    if (hw->ECR_GetProtocol() == 8) {
	kkm_res = hw->ECR_PrintDocHeadLine();
    }
    else {
//---------------------------------------    
    QString tmp = hw->ECR_GetSerialNumber();
    int length = tmp.length();
    //-----(+)-----Zuskin-----25/07/2011-----
    if (length < 8) {
    //---------------------------------------
	 while((8-length)>0){
		  s += '0';  
		  length++;
	 }
    //-----(+)-----Zuskin-----25/07/2011-----	 
    }	 
    //---------------------------------------
    s += tmp;
    s += " ";
    s += HwCodec->fromUnicode(DefCodec->toUnicode("ИНН "));
	 
    tmp = hw->ECR_GetINN(); 
    if ( tmp.length() == 0 ) 
		  s += "????????????";
    else {
          //-----(+)-----Zuskin-----25/07/2011-----
	  if (tmp.length() < 12) {
	  //---------------------------------------
		  for ( int i = 0; i < 12 - tmp.length(); i++ )	
				s += '0';
	  //-----(+)-----Zuskin-----25/07/2011-----			
	  }			
	  //---------------------------------------
	  s += tmp;
    }
    s += " #";
   // printf("s = |%s|\n",s.ascii());
    kkm_res = hw->ECR_ReadDocumentNumber();
    if ( kkm_res != 0 ) return kkm_res;  
	 
    tmp = QString::number(hw->ECR_GetDocumentNumber());
    //-----(+)-----Zuskin-----25/07/2011-----
    if (tmp.length() < 4) {
    //---------------------------------------
	for ( int i = 0; i < 4 - tmp.length(); i++ )   s += '0';
    //-----(+)-----Zuskin-----25/07/2011-----	
    }		
    //---------------------------------------
    
    s += tmp;
    kkm_res = KkmPrintString( s.ascii() );
    if ( kkm_res != 0 ) return kkm_res;  
    
    //дата время, пользователь
    s = "";
    QDate d( hw->ECR_GetYear(), hw->ECR_GetMonth(), hw->ECR_GetDay() );
    s += d.toString("dd.MM.yy");
    //printf("s = %s \n", s.ascii());
    s += " ";
    QTime t( hw->ECR_GetHour(), hw->ECR_GetMinute() );
    s += t.toString("hh:mm");
    
    kkm_res = KkmPrintSeparatorString(s.ascii(), HwCodec->fromUnicode(DefCodec->toUnicode("СИСТ.АДМИН.")), " ");
    if ( kkm_res != 0 )	  return kkm_res;
//-----(+)-----Zuskin-----17/10/2011-----    
    }
//---------------------------------------    
    return kkm_res;
}
//======================================================================//	
int KkmRegistrations::KkmPrintPositionInfo(const char* ware_shortcut, double quantity, double price, bool wrap_flag) {
    
    //printf("KkmPrintPositionInfo(const char* ware_shortcut = %s double quantity = %f, double price = %f) \n", ware_shortcut, quantity, price);
    
    string s;
    QString ss;
    QString qs;
    QString ps;
    
    int kkm_res = KkmPrintString(HwCodec->fromUnicode(DbCodec->toUnicode(ware_shortcut)), true, wrap_flag);
    if ( kkm_res != 0 )        return kkm_res;    

    s = "";
    qs = QString::number(quantity, 'f', 3);
    ps = QString::number(price, 'f', 2);
    for ( unsigned int j = 0; j < stringwide - qs.length() - ps.length() - 3; j++ )	 s += ' ';
    s += qs.ascii();
    s += " X ";
    s += ps.ascii();
    kkm_res = KkmPrintString(s.c_str(), false); 
    if ( kkm_res != 0 )          return kkm_res;
    
    s = "";
    ss = QString::number(quantity * price, 'f', 2);
    for ( unsigned int j = 0; j < stringwide - ss.length() - 1; j ++ )        s += ' ';
    s += '=';	
    s += ss.ascii();
    kkm_res = KkmPrintString(s.c_str(), false);
    if ( kkm_res != 0 )        return kkm_res;
    
    return kkm_res;
}	
//======================================================================//	
// oper_type = 0 - getting 1 - entering
int KkmRegistrations::KkmPrintSectionGettingEntering(double sum, int oper_type, const char* seller, int section_num) {
    int kkm_res = 0;
    
    if ( section_num == 0 ) {
	kkm_res = KkmPrintSeveralStrings(sp->sections[0].head_text);
	if ( kkm_res != 0 ) return kkm_res;
    }
    
    if ( section_num < 1 ) {
	kkm_res = hw->ECR_ReadStatus();
	if ( kkm_res != 0 ) return kkm_res;
	
	if (oper_type == 0) kkm_res = hw->ECR_CashOutcome(sum, false);
	else kkm_res = hw->ECR_CashIncome(sum, false);
	if ( kkm_res != 0 ) return kkm_res;
	
	if ( section_num == 0 ) {
	    if ( oper_type == 0 ) 
		sp->deductCashSumInSection(section_num, sum);
	    else 
		sp->addCashSumInSection(section_num, sum);
	}
	
    } else  {
	//  try { ...  } catch (const Error & err) {  MessageError(err.getErrorText()); return;  }  
	if ( oper_type == 0 ) 
	    sp->deductCashSumInSection(section_num, sum);
	else 
	    sp->addCashSumInSection(section_num, sum);
	
	kkm_res = KkmPrintSeveralStrings(sp->sections[section_num].head_text);
	if ( kkm_res != 0 ) return kkm_res;
	
	QString s = "";
	QDate d( hw->ECR_GetYear(), hw->ECR_GetMonth(), hw->ECR_GetDay() );
	s += d.toString("dd.MM.yy") + " ";
	QTime t( hw->ECR_GetHour(), hw->ECR_GetMinute() );
	s += t.toString("hh:mm");
	kkm_res = KkmPrintString(s.ascii());
	if ( kkm_res != 0 )	  return kkm_res;
	
	kkm_res = KkmPrintSeparatorString(HwCodec->fromUnicode(DefCodec->toUnicode("КАССИР")), HwCodec->fromUnicode(DefCodec->toUnicode(seller)));
	 if ( kkm_res < 0 ) return kkm_res;
	
	 if ( oper_type == 0 ) 
	     s = HwCodec->fromUnicode(DefCodec->toUnicode("ВЫПЛАТА"));
	 else 
	     s = HwCodec->fromUnicode(DefCodec->toUnicode("ВНЕСЕНИЕ"));
	 
	 kkm_res = KkmPrintSeparatorString(s, QString::number(sum, 'f', 2));
	 if ( kkm_res != 0 ) return kkm_res;
	 
	 kkm_res = KkmPrintSeveralStrings(sp->sections[section_num].bottom_text);
	 if ( kkm_res != 0 ) return kkm_res;
	 
	 kkm_res = KkmPartialCut();
	 if (kkm_res != 0) return kkm_res;
    }
    return kkm_res;
}
//======================================================================//


