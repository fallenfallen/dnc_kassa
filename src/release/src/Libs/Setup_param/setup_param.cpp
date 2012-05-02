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

#include "setup_param.h"
#include "const_conf.h"

#include <iostream>

const int COUNT_PARAM_IN_SP = 91;

//======================================================================//
SP::SP()
{
	//puts("SP::SP()");	
   //puts("constructor SP()");
   // значения по умолчанию 
   
   
/*general setup*/
   //max_check_sum = 100.00;         //XX
   //max_box_sum = 1000.00;            //XX
   
   learn_mode = false;
   write_transaction = false;
   limit_client_sum = true;
   max_money = 5000;

   max_change_time = 1 * 3600;  // время смены 2 часа
   warning_change_time_ending = 15 * 60; //предупреджать за 10 минут
   autoblock_flag = false;
   autoblock_time = 10 * 60;
   block_mouse = false;
   one_click_selection = false;
/*Authorization*/   
   query_for_authorization = true;  //почти
   password_authorization = true;
   card_authorization = false;         //XX //XX
   barcode_authorization = true;   //XX
   
   progres_picture_path = (char*) calloc(1, sizeof(char));
   
   
/*REGISTRATION SECTION*/
	return_count_inc = false;

/*Data output section*/
   clear_window_after_close_check = true;
   show_properties_in_input = true;              
   error_output_window = false;
   association_positions = 0;
/*Visual search section*/
   close_after_one_registration = false;          //почти
   query_quantity = false;           
   show_absent_goods = true;	
   start_vs_manualinput = true;                     
   max_number_search_result = 50;
   order_search_result = 0;                                                                          
/*Control registration section*/ //А НУЖНО ЛИ???
   control_sale = 0;         // продажа      //   
   control_back = 0;         // возвраты   //    
   control_double = 0;        // дробное количество      
   control_price_edit = 0;   //редактирование цен       
   control_exparation = 0;   // срок годности           
   control_min_price = 0;   // минимальная цена          
   control_remains = 0;    //    списание остатков        
   control_negative_remains = 0;     // отрицательные отстатки  
   
   /*Report*/
   round_check_sum = 0;
   round_power = 0.05;
   round_type = 0;
   
  
    /*report*/
   auto_take_report = true;      // автоматическое снятие отчетов      //  "AutoTakeReport"
   report_x = false;            // отчет Х                                             // "ReportX"
   report_section = false;      // отчет по секциям                              //  "ReportSection"
   report_count = false;         // отчет количеств                              //  "ReportCount"
   report_by_hour = false;      //  почасовой отчет                              //  "ReportByHours"
   report_cashier = false;      // покассирный отчет                           //   "ReportCashier"
   report_ware = false;
   report_unload = false;       // выгрузка новых транзакций                   //   "ReportUnload"
   report_psreconsiliation = false;   
   
/*REDUCTIONS SECTION*/
/*Manual reductions section*/
  //указываются в целых процентах от суммы позиции или чека.
   max_position_discount = 50;                         //XX
   max_position_increase = 20;                         //XX
   max_check_discount = 30;                            //XX
   max_check_increase = 10;                            //XX
/*general section*/
   recalc_autored_each_registration = true;     //XX
   disc_exception_mode = false;
   result_discount = 0;
   base_discount_order = (char*) calloc(10, sizeof(char));
   strcpy(base_discount_order, "12");
   
 /*PRINTING CHECK SECTION*/
  /*Printing Check Information*/
   print_position_taxes = true;
   cut_position_name = false;
  
/*Time*/
   control_time_synchronization = true;
   //time_for_block = 5 * 60;
   auto_synchronization = true;
   time_for_synchronization = 1* 60;
   time_for_synchronization_max = 60;
  
   /*warn about need synchro*/
   warn_synchro_in_clock_field = false;      // в поле часов                                                            //   "NotifySynchroTimeField"
   warn_synchro_after_close_check = false;   // сообщение после закрытия чека                                 //   "NotifySynchroMessage"
  
   /*open money box*/
   open_box_by_pay_with_rest = false;   // открывать денежный ящик при оплате со сдачей   //   "OpenBoxByRest"
   open_box_by_close_cheque = false;   // -\\- при закрытии чека                                          //   "OpenBoxByCloseCheque"
   open_box_by_change_money = false;   // -\\- при внесении / выплате денежных средств         //   "OpenBoxByChangeMoney"
   open_box_by_clear_report = false;   // -\\- при снятии отчета с гашением                        //      "OpenBoxByClearReport"
  
   /*Devices section*/
   /*Cusomer Display*/
   cdsp_up_pause_work = (char*) calloc(1, sizeof(char));
   cdsp_up_pause_work_mode_run = 0; 
   cdsp_down_pause_work = (char*) calloc(1, sizeof(char));
   cdsp_down_pause_work_mode_run = 0;  
   //strcpy(cdsp_up_pause_work, ""); 
  
   /*message for not work*/
   cdsp_up_not_work = (char*) calloc(1, sizeof(char));
   cdsp_up_not_work_mode_run = 0;
   cdsp_down_not_work = (char*) calloc(1, sizeof(char));
   cdsp_down_not_work_mode_run = 0;
   //strcpy(cdsp_up_not_work, "");
  
   /*product registration detalization */
   cdsp_detalize_product = false;
   cdsp_detalize_product_name = false;
   cdsp_detalize_product_price = false;
   cdsp_detalize_product_quantity = false;
   cdsp_detalize_product_sum = false;
   cdsp_detalize_product_total = false;
  
   cdsp_detalize_product_name_string = 0;
   cdsp_detalize_product_properties_string = 0;
   cdsp_detalize_product_total_string = 0;
   
   cdsp_up_detalize_product_mode_run = 0; 
   cdsp_down_detalize_product_mode_run = 0;
   /* discount registration detalize */
   cdsp_detalize_discount = false;
   cdsp_detalize_discount_name = false;
   cdsp_detalize_discount_sum = false;
   
   cdsp_detalize_discount_name_string = 0; 
   cdsp_detalize_discount_sum_string = 0;
   
   cdsp_up_detalize_discount_mode_run = 0;  
   cdsp_down_detalize_discount_mode_run = 0;

   db = new GetParam(LINCASH_CONF_PATH);
   
   state_connect = db->er_con;
   try {
	WareSystem::startInstance();
   } catch (const Error & err) {
	catchError(err);
   }
	
   loadSections();
   
   //if (state_connect < 0) printf("the connect to db is apsent!!! \n");
    
}
//======================================================================//
void SP::loadSections() {
	try {
		
		SectionCatalog * section_catalog = WareSystem::getInstance()->getSectionCatalog();
		
		vector<Section*> exist_sections = section_catalog->getAllSections();
		
		for (int i = 0; i < exist_sections.size(); i++) {
			Section s(*exist_sections[i]);
			cout << "s.number - " << s.getNumber() << endl;
			cout << "s.name - " << s.getName() << endl;
			sections.push_back(s.getInfo());
		}
		
	} catch (const Error & err) {
		catchError(err);
	}
}

void SP::ReloadSections() {
    try {
	WareSystem::finishInstance();
    } 
    catch(const Error & err) {
	catchError(err);
    }
    try {
	WareSystem::startInstance();
    } 
    catch (const Error & err) {
	catchError(err);
    }


    try {
	sections.clear();	
	SectionCatalog * section_catalog = WareSystem::getInstance()->getSectionCatalog();
		
	vector<Section*> exist_sections = section_catalog->getAllSections();
		
	for (int i = 0; i < exist_sections.size(); i++) {
		Section s(*exist_sections[i]);
		cout << "s.number - " << s.getNumber() << endl;
		cout << "s.name - " << s.getName() << endl;
		sections.push_back(s.getInfo());
	}
		
    } 
    catch (const Error & err) {
		catchError(err);
    }
}

//======================================================================//

void SP::catchError(const Error & err) {
	cout << "error # " << err.getErrorCode();
	cout << " " << err.getErrorLogText() << endl;
}

Section * SP::getSectionByNumber(int section_num) {
	try {
		SectionCatalog * section_catalog = WareSystem::getInstance()->getSectionCatalog();	
		Section * s = section_catalog->getSectionByNumber(sections[section_num].number);		
	} catch (const Error & err) {
		catchError(err);
	}
}

void SP::addCashSumInSection(int section_num, double sum) {
	if ( section_num > sections.size() ) 
		return;
	Section * s = getSectionByNumber(section_num);	
	s->addCashSum(sum);
	sections[section_num] = s->getInfo();
	//---(+)---Zuskin---28/02/2012---
	//double cashInSection = db->GetSectionCash(section_num);
	//printf("Section = %d, cash in SP= %2f, in DB= %2f\n", section_num, sections[section_num].cash, cashInSection);
	//if ( sections[section_num].cash != cashInSection )
	//    puts("Cash check FAILED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	//-------------------------------
	
}

void SP::deductCashSumInSection(int section_num, double sum) {
	if ( section_num > sections.size() ) 
		return;
	Section * s = getSectionByNumber(section_num);	
	s->deductCashSum(sum);
	sections[section_num] = s->getInfo();
}

void SP::zeroCashSumInSections() {
	for ( int i = 0; i < sections.size(); i++ ) {
		zeroCashSumInSection(i);
	}
}

void SP::zeroCashSumInSection(int section_num) {
	if ( section_num > sections.size() ) 
		return;
	if ( sections[section_num].auto_encashment == false ) 
		return;
	Section * s = getSectionByNumber(section_num);	
	s->zeroCashSum();
	sections[section_num] = s->getInfo();
}

double SP::getCashSumInSection(int section_num) {
	if ( section_num > sections.size() ) 
		return 0;
	Section * s = getSectionByNumber(section_num);	
	return s->getCashSum();
}

double SP::getNotFiscalCash() {
	double res = 0;
	for (int i = 1; i < sections.size(); i++) {
		res += Round(sections[i].cash*100)/100;
	}
	return Round(res*100)/100;
}

//======================================================================//

SP::~SP()   {
   //puts("SP::~SP()");
   try {
      WareSystem::finishInstance();
   } catch(const Error & err) {
      catchError(err);
   }

   delete db;
    
   free(cdsp_up_pause_work);
   free(cdsp_down_pause_work);
   free(cdsp_up_not_work);
   free(cdsp_down_not_work);
   free(base_discount_order);
   free(progres_picture_path);
   
   //puts("SP clear");
}
//======================================================================//
int SP::Get_All_Param() {
   count_error_param = 0;   
   if (state_connect != 0) {  count_error_param = COUNT_PARAM_IN_SP; return -1;  }
   
   int state = 0;
   
   if ( get_Base_Param() != 0 ) {puts("Error load base parameters!!!");state = -1;}
   if ( get_Authorization_Param() != 0) {puts("Error load authorization parameters!!!");state = -1;}
   if ( get_Round_Param() != 0 ) { puts("Error load round parameters!!!");state = -1; }
   if ( get_BlockAndMouse_Param() != 0) {puts("Error load block and mouse parameters!!!");state = -1;}
   if ( get_Device_Param() != 0) {puts("Error load device parameters!!!");state = -1;}
   if ( get_Report_Param() != 0) {puts("Error load report parameters!!!");state = -1;}
   if ( get_DataOutput_Param() != 0 ) {puts("Error load data output parameters!!!");state = -1;}
   if ( get_VisualSearh_Param() != 0 ) {puts("Error load visual search parameters!!!");state = -1;}
   if ( get_ControlRegistration_Param() != 0 ) {puts("Error load control registration parameters!!!");state = -1;}
   if ( get_FreeDiscount_Param() != 0 ) {puts("Error load free discount parameters!!!");state = -1;}
   if ( get_GeneralDiscount_Param() != 0 ) {puts("Error load general discount parameters!!!");state = -1;}
   if ( get_PrintCheck_Param() != 0 ) {puts("Error load print check parameters!!!");state = -1;}
   if ( get_KKMSynchronaze_Param() != 0 ) {puts("Error load kkm synchronization parameters!!!");state = -1;}
   if ( get_OpenBox_Param() != 0 ) {puts("Error load open box parameters!!!");state = -1;}
   if ( get_ProdustRegistrationDetalize_Param() != 0 ) {puts("Error load proucts registraion detalize parameters!!!");state = -1;}
   if ( get_DiscountRegistrationDetalize_Param() != 0 ) {puts("Error load discount registration detalize parameters!!!");state = -1;}
   
   puts("``````````````````````````");
   printf("SYSTEM SETUP: was init %d parameters \n", COUNT_PARAM_IN_SP - count_error_param);
   puts("``````````````````````````");
      
   return state;
}
//======================================================================//
int SP::Get_Authorization_Param() {
   count_error_param = 0;
   if (state_connect != 0) {  
      count_error_param = 4; return -1;  }
   else 
      return get_Authorization_Param();
}
//======================================================================//
int SP::Get_BlockAndMouse_Param() {
   count_error_param = 0;
   if (state_connect != 0) {  
      count_error_param = 4; return -1;  }
   else 
      return get_BlockAndMouse_Param();
}
//======================================================================//
int SP::Get_Device_Param()   {
   count_error_param = 0;
   if (state_connect != 0) {  
      count_error_param = 8; return -1;  }
   else 
      return get_Device_Param();
}
//======================================================================//
int SP::Get_Report_Param() {
   count_error_param = 0;   
   if (state_connect != 0) {  count_error_param = 22; return -1;  }
   
   int state = 0;
   
   if (get_Authorization_Param() != 0) state = -1;
   if (get_BlockAndMouse_Param() != 0) state = -1;
   if (get_Device_Param() != 0) state = -1;
   if (get_Report_Param() != 0) state = -1;
   
   return state;
   
}
//======================================================================//
int SP::get_Round_Param() {
   
   if ( db->TakeParam("RoundCheckSum", &round_check_sum) < 0 ) count_error_param++ ; 
   if ( db->TakeParam("RoundPower", &round_power) < 0 ) count_error_param++;
   if ( db->TakeParam("RoundType", &round_type) < 0 ) count_error_param++;

   if ( count_error_param != 0 ) return -1; else return 0;
}
//======================================================================//
int SP::get_Authorization_Param() {
//puts("=================");
//   puts("get_Authorization_Param()");
//puts("=================");
   
   if ( db->TakeParam("AuthorLackRights", &query_for_authorization) < 0 ) count_error_param++ ; 
   if ( db->TakeParam("AuthorPassword", &password_authorization) < 0 ) count_error_param++;
   if ( db->TakeParam("AuthorCardReader", &card_authorization) < 0 ) count_error_param++;
   if ( db->TakeParam("AuthorBarCode", &barcode_authorization) < 0 )  count_error_param++;
   
	/*if (query_for_authorization) printf("запрос авторищации при недостатке прав = true\n"); else printf("запрос при недостатке прав = false\n");
	if (password_authorization) printf("авторизация по паролю = true\n"); else printf("авторизация по паролю = false\n");
	if (card_authorization) printf("авторизация  по магенитным картам = true\n"); else printf("авторизация по магнитным картам = false\n");
	if (barcode_authorization) printf("авторизация  по шттрих кодам = true\n"); else printf("авторизация по штрих кодам = false\n");*/


   if ( count_error_param != 0 ) return -1; else return 0;
}
//======================================================================//
int SP::get_BlockAndMouse_Param() {
//puts("=================");
//   puts("get_BlockAndMouse_Param()");
//puts("=================");
   
   if ( db->TakeParam("AutoBlock", &autoblock_flag) > 0 ) count_error_param++;
   int n = 10;
   if ( db->TakeParam("AutoBlockTime", &n) < 0 ) count_error_param++;
   autoblock_time = n * 60;
   if ( db->TakeParam("BlockMouse", &block_mouse) < 0 ) count_error_param++;
   if ( db->TakeParam("UseOneClick", &one_click_selection) < 0 ) count_error_param++;
   
   if ( count_error_param != 0 ) return -1; else return 0;
}
//======================================================================//
int SP::get_Device_Param()   {
//puts("=================");
//   puts("get_Device_Param()");
//puts("=================");
   
   if ( db->TakeParam("StandUpString", cdsp_up_pause_work) < 0 ) count_error_param++; 
   if ( db->TakeParam("StandUpStringRun", &cdsp_up_pause_work_mode_run) < 0 ) count_error_param++;
   if ( db->TakeParam("StandDownString", cdsp_down_pause_work) < 0 ) count_error_param++;
   if ( db->TakeParam("StandDownStringRun", &cdsp_down_pause_work_mode_run) < 0 ) count_error_param++;
   if ( db->TakeParam("OffUpString", cdsp_up_not_work) < 0 ) count_error_param++;
   if ( db->TakeParam("OffUpStringRun", &cdsp_up_not_work_mode_run) < 0 ) count_error_param++;
   if ( db->TakeParam("OffDownString", cdsp_down_not_work) < 0 ) count_error_param++;
   if ( db->TakeParam("OffDownStringRun", &cdsp_down_not_work_mode_run) < 0 ) count_error_param++;
 /*  
printf("cdsp_up_pause_work = %s \n", cdsp_up_pause_work);
printf("cdsp_down_pause_work = %s \n", cdsp_down_pause_work);
printf("cdsp_up_not_work = %s \n", cdsp_up_not_work);
printf("cdsp_down_not_work = %s \n", cdsp_down_not_work);*/

   if ( count_error_param != 0 ) return -1; else return 0;
}
//======================================================================//
int SP::get_Report_Param() {
//puts("=================");
//   puts("get_Report_Param()");
//puts("=================");
   
   if ( db->TakeParam("AutoTakeReport", &auto_take_report) < 0) count_error_param++;
   if ( db->TakeParam("ReportX", &report_x) < 0) count_error_param++;
   if ( db->TakeParam("ReportSection", &report_section) < 0) count_error_param++;
   if ( db->TakeParam("ReportCount", &report_count) < 0) count_error_param++;
   if ( db->TakeParam("ReportByHour", &report_by_hour) < 0) count_error_param++;
   if ( db->TakeParam("ReportCashier", &report_cashier) < 0) count_error_param++;
	if ( db->TakeParam("ReportWare", &report_ware) < 0) count_error_param++;

   if ( db->TakeParam("ReportUnload", &report_unload) < 0) count_error_param++;
   if ( db->TakeParam("ReportPSReconsiliation", &report_psreconsiliation) < 0) count_error_param++;

   if ( count_error_param != 0 ) return -1; else return 0;
}
//======================================================================//
int SP::get_Base_Param() {
//puts("=================");
//   puts("get_Base_Param()");
//puts("=================");
   
   //if ( db->TakeParam("MaxClientSum", &max_check_sum) < 0 ) count_error_param++;
   //if ( db->TakeParam("MaxMoneyBox", &max_box_sum) < 0 ) count_error_param++;
   
   if ( db->TakeParam("LearnMode", &learn_mode) < 0 ) count_error_param++;
   if ( db->TakeParam("WriteTransaction", &write_transaction) < 0 ) count_error_param++;
   if ( db->TakeParam("LimitClientSum", &limit_client_sum) < 0 ) count_error_param++;
   if ( db->TakeParam("MaxMoney", &max_money) < 0 ) count_error_param++;

   int n = 1;
   if ( db->TakeParam("DurationSession", &n) < 0 ) count_error_param++; 
   max_change_time = n * 3600;
   n = 15;
   if ( db->TakeParam("WarnAboutEnd", &n) < 0 ) count_error_param++;
   warning_change_time_ending = n * 60;
   
   if ( db->TakeParam("PicturePath", progres_picture_path) < 0 ) count_error_param++;
   
   if ( count_error_param != 0 ) return -1; else return 0;
}
//======================================================================//
int SP::get_DataOutput_Param()   {
//puts("=================");
//   puts("get_DataOutput_Param()");
//puts("=================");
	
	if ( db->TakeParam("ReturnCountInc", &return_count_inc) < 0 ) count_error_param++;
	
   if ( db->TakeParam("SaveAfterClose", &clear_window_after_close_check) < 0 ) count_error_param++;
   clear_window_after_close_check = ! clear_window_after_close_check;
   if ( db->TakeParam("ShowProduct", &show_properties_in_input) < 0 ) count_error_param++;
   int n = 1;
   if ( db->TakeParam("ErrorMessage", &n) < 0 ) count_error_param++;
   if (n == 0) error_output_window = true; else error_output_window = false;
   if ( db->TakeParam("GroupRecord", &association_positions) < 0 ) count_error_param++;
   
   if ( count_error_param != 0 ) return -1; else return 0;
}  
//======================================================================//
int SP::get_VisualSearh_Param()    {
//puts("=================");
//   puts("get_VisualSearh_Param()");
//puts("=================");
   
   if ( db->TakeParam("CloseFindAfter1reg", &close_after_one_registration) < 0) count_error_param++;
   if ( db->TakeParam("FindAskCount", &query_quantity) < 0) count_error_param++;
   if ( db->TakeParam("ShowAbsentGoods", &show_absent_goods) < 0) count_error_param++;
   if ( db->TakeParam("FindSort", &order_search_result) < 0) count_error_param++;
   if ( db->TakeParam("FindRecordCount", &max_number_search_result) < 0) count_error_param++;
   if ( db->TakeParam("VSManualInput", &start_vs_manualinput) < 0) count_error_param++;
   
   if ( count_error_param != 0 ) return -1; else return 0;
}
//======================================================================//
int SP::get_ControlRegistration_Param() {
//puts("=================");
//   puts("get_ControlRegistration_Param()");
//puts("=================");
   
   if ( db->TakeParam("ControlSale", &control_sale) < 0) count_error_param++;
   if ( db->TakeParam("ControlBack", &control_back) < 0) count_error_param++;
   if ( db->TakeParam("ControlDouble", &control_double) < 0) count_error_param++;
   if ( db->TakeParam("ControlPriceEdit", &control_price_edit) < 0) count_error_param++;
   if ( db->TakeParam("ControlExparation", &control_exparation) < 0) count_error_param++;
   if ( db->TakeParam("ControlMinPrice", &control_min_price) < 0) count_error_param++;
   if ( db->TakeParam("ControlRemains", &control_remains) < 0) count_error_param++;
   if ( db->TakeParam("ControlNegativeRemains", &control_negative_remains) < 0) count_error_param++;  
   
   if ( count_error_param != 0 ) return -1; else return 0;
}
//======================================================================//
int SP::get_FreeDiscount_Param()   {
//puts("=================");
//   puts("get_FreeDiscount_Param()");
//puts("=================");
   
   if ( db->TakeParam("MaxStandDiscount", &max_position_discount) < 0) count_error_param++;
   if ( db->TakeParam("MaxStandIncrease", &max_position_increase) < 0) count_error_param++;
   if ( db->TakeParam("MaxChequeDiscount", &max_check_discount) < 0) count_error_param++;
   if ( db->TakeParam("MaxChequeIncrease", &max_check_increase) < 0) count_error_param++;
   
   if ( count_error_param != 0 ) return -1; else return 0;
}
//======================================================================//
int SP::get_GeneralDiscount_Param()   {
//puts("=================");
//   puts("get_GeneralDiscount_Param()");
//puts("=================");
   
   if ( db->TakeParam("AutoRecountDiscount", &recalc_autored_each_registration) < 0) count_error_param++;
   if ( db->TakeParam("WithoutStandDiscount", &disc_exception_mode) < 0) count_error_param++;
   if ( db->TakeParam("ResultDiscount", &result_discount) < 0) count_error_param++;
   if ( db->TakeParam("BaseDiscountOrder", base_discount_order) < 0) count_error_param++;
   
   if ( count_error_param != 0 ) return -1; else return 0;
}
//======================================================================//
int SP::get_PrintCheck_Param()  {  
//puts("=================");
//   puts("get_PrintCheck_Param()");
//puts("=================");
   if ( db->TakeParam("PrintTax", &print_position_taxes) < 0) count_error_param++;
   if ( db->TakeParam("WordWrap", &cut_position_name) < 0) count_error_param++;
   if ( db->TakeParam("PrintOneReg", &print_one_registration) < 0) count_error_param++;
   
   if ( count_error_param != 0 ) return -1; else return 0;
}
//======================================================================//
int SP::get_KKMSynchronaze_Param()   {  
//puts("=================");
//   puts("get_KKMSynchronaze_Param()");
//puts("=================");
   if ( db->TakeParam("Synchro", &control_time_synchronization) < 0) count_error_param++;
   if ( db->TakeParam("AutoSynchro", &auto_synchronization) < 0) count_error_param++;
   int n = 1;
   if ( db->TakeParam("SynchroFrom", &n) < 0) count_error_param++;
   time_for_synchronization = 60 * n;
   n = 1;
   if ( db->TakeParam("SynchroTo", &n) < 0) count_error_param++;
   time_for_synchronization_max = 60 * n;
   if ( db->TakeParam("NotifySynchroTimeField", &warn_synchro_in_clock_field) < 0) count_error_param++;
   if ( db->TakeParam("NotifySynchroMessage", &warn_synchro_after_close_check) < 0) count_error_param++;
   
   if ( count_error_param != 0 ) return -1; else return 0;
}
//======================================================================//
int SP::get_OpenBox_Param()   {
//puts("=================");
//   puts("get_OpenBox_Param()");
//puts("=================");
   if ( db->TakeParam("OpenBoxByRest", &open_box_by_pay_with_rest) < 0) count_error_param++;
   if ( db->TakeParam("OpenBoxByCloseCheque", &open_box_by_close_cheque) < 0) count_error_param++;
   if ( db->TakeParam("OpenBoxByChangeMoney", &open_box_by_change_money) < 0) count_error_param++;
   if ( db->TakeParam("OpenBoxByClearReport", &open_box_by_clear_report) < 0) count_error_param++;
   
   if ( count_error_param != 0 ) return -1; else return 0;
}
//======================================================================//
int SP::get_ProdustRegistrationDetalize_Param()   {
//puts("=================");
//   puts("get_ProdustRegistrationDetalize_Param()");
//puts("=================");
   if ( db->TakeParam("DisplayProduct", &cdsp_detalize_product) < 0) count_error_param++;
   if ( db->TakeParam("DisplayProductName", &cdsp_detalize_product_name) < 0) count_error_param++;
   if ( db->TakeParam("DisplayProductPrice", &cdsp_detalize_product_price) < 0) count_error_param++;
   if ( db->TakeParam("DisplayProductQuantity", &cdsp_detalize_product_quantity) < 0) count_error_param++;
   if ( db->TakeParam("DisplayProductSum", &cdsp_detalize_product_sum) < 0) count_error_param++;
   if ( db->TakeParam("DisplayProductTotal", &cdsp_detalize_product_total) < 0) count_error_param++;
   if ( db->TakeParam("DisplayProductNameString", &cdsp_detalize_product_name_string) < 0) count_error_param++;
   if ( db->TakeParam("DisplayProductPriceString", &cdsp_detalize_product_properties_string) < 0) count_error_param++;
   if ( db->TakeParam("DisplayProductTotalString", &cdsp_detalize_product_total_string) < 0) count_error_param++;
   if ( db->TakeParam("DisplayProductUpStringRun", &cdsp_up_detalize_product_mode_run) < 0) count_error_param++;
   if ( db->TakeParam("DisplayProductDownStringRun", &cdsp_down_detalize_product_mode_run) < 0) count_error_param++;
   
   if ( count_error_param != 0 ) return -1; else return 0;
}
//======================================================================//
int SP::get_DiscountRegistrationDetalize_Param()   { 
//puts("=================");
//   puts("get_DiscountRegistrationDetalize_Param() ");
//puts("=================");
   if ( db->TakeParam("DisplayDiscount", &cdsp_detalize_discount) < 0) count_error_param++;
   if ( db->TakeParam("DisplayDiscountName", &cdsp_detalize_discount_name) < 0) count_error_param++;
   if ( db->TakeParam("DisplayDiscountSum", &cdsp_detalize_discount_sum) < 0) count_error_param++;
   if ( db->TakeParam("DisplayDiscountNameString", &cdsp_detalize_discount_name_string) < 0) count_error_param++;
   if ( db->TakeParam("DisplayDiscountSumString", &cdsp_detalize_discount_sum_string) < 0) count_error_param++;
   if ( db->TakeParam("DisplayDiscountUpStringRun", &cdsp_up_detalize_discount_mode_run) < 0) count_error_param++;
   if ( db->TakeParam("DisplayDiscountDownStringRun", &cdsp_down_detalize_discount_mode_run) < 0) count_error_param++;

   if ( count_error_param != 0 ) return -1; else return 0;
}
//======================================================================//



