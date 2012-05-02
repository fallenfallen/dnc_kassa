#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "get_param_pgsql.h"

#include "WareSystem.h"
#include "Section.h"
#include "Error.h"


/*Registration section*/
#define NOTASSOCIATION 1
#define NOTFACTORIALASSOCIATION 0
#define ALLASSOCIATION 2

/*control registration*/
#define YES 2
#define NO 1
#define WARECARD 0

#define MAXBARLEN 13

#ifndef CLASS_SP
#define CLASS_SP


using namespace std;

class SP
{

private:
  GetParam* db;
  
  
  
  int get_Base_Param();
  int get_DataOutput_Param();
  int get_VisualSearh_Param();
  int get_ControlRegistration_Param();
  int get_FreeDiscount_Param();
  int get_GeneralDiscount_Param();
  int get_PrintCheck_Param();
  int get_KKMSynchronaze_Param();
  int get_OpenBox_Param();
  int get_ProdustRegistrationDetalize_Param() ;
  int get_DiscountRegistrationDetalize_Param();
  
  int get_Authorization_Param();
  int get_BlockAndMouse_Param();
  int get_Report_Param();
  int get_Device_Param();
  
  int get_Round_Param();
  
  void loadSections();

  Section * getSectionByNumber(int section_num);
  void catchError(const Error & err);
  
public :
  SP();
  ~SP();
  int Get_Authorization_Param();
  int Get_BlockAndMouse_Param();
  int Get_Report_Param();
  int Get_Device_Param();
  int Get_All_Param();

  void addCashSumInSection(int section_num, double sum);
  void deductCashSumInSection(int section_num, double sum);
  void zeroCashSumInSections();
  void zeroCashSumInSection(int section_num);

  double getCashSumInSection(int section_num);
  double getNotFiscalCash();
  
  //---(+)---Zuskin---27/02/2012---
  void ReloadSections();
  //-------------------------------
  
  int state_connect;
  int count_error_param;
  
  /*GENERAL SECTION*/
  /*Contingencies*/
  
  bool learn_mode;		//  "LearnMode"
  bool write_transaction;	//  "WriteTransaction"
  bool limit_client_sum;	//  "LimitClientSum"
  double max_money; 		//  "MaxMoney"
    
  int max_change_time; //Время смены в секундах         "DurationSession"
  int warning_change_time_ending; //Начинать предупреждать об окончании смены, в секундах   "WarnAboutEnd"
  
  bool autoblock_flag;            //  "AutoBlock"
  int autoblock_time;            //  "AutoBGlockTime"
  bool block_mouse;            // "BlockMouse"
  bool one_click_selection;        //XX         //      "UseOneClick"
  
  /*Authorization section*/
  bool query_for_authorization; //XX            //      "AuthorLackRights"
  bool password_authorization; //XX            //      "AuthorPassword"
  bool card_authorization;         //XX //XX   //      "AuthorCardReader"
  bool barcode_authorization;   //XX            //      "AuthorBarCode"
  
  char * progres_picture_path;
  
  /*REGISTRATION SECTION*/
  
  /*Return section*/
  bool return_count_inc;		
  
  /*Data output section*/
  bool clear_window_after_close_check;   //XX  = not SaveAfterClose      
  bool show_properties_in_input;         //XX   "ShowProduct" 
  bool error_output_window;       //если true в отдельном окне      //   "ErrorMessage"
  int association_positions;            //   "GroupRecord"
  
  /*VisualSearch section*/
  bool close_after_one_registration;          //XX            "CloseFindAfter1reg"
  bool query_quantity;                        //XX            "FindAskCount"
  bool show_absent_goods;							// 				  "ShowAbsentGoods"
  int order_search_result;                    //              "FindSort"
  int max_number_search_result;               //               "FindRecordCount"
  bool start_vs_manualinput;
  
  /*Control registration section*/
  int control_sale;         // продажа      //   "ControlSale"
  int control_back;         // возвраты   //      "ControlBack"
  int control_double;        // дробное количество        // "ControlDouble"
  int control_price_edit;   //редактирование цен         //   "ControlPriceEdit"
  int control_exparation;   // срок годности               //   "ControlExparation"
  int control_min_price;   // минимальная цена            //   "ControlMinPrice"
  int control_remains;    //    списание остатков         //   "ControlRemains"
  int control_negative_remains;     // отрицательные отстатки   //   "ControlNegativeRemains"
  
  /*Round*/
  bool round_check_sum;
  double round_power;
  int round_type;

  /*report*/
  bool auto_take_report;      // автоматическое снятие отчетов      //  "AutoTakeReport"
  bool report_x;            // отчет Х                                             // "ReportX"
  bool report_section;      // отчет по секциям                              //  "ReportSection"
  bool report_count;         // отчет количеств                              //  "ReportCount"
  bool report_by_hour;      //  почасовой отчет                              //  "ReportByHour"
  bool report_cashier;      // покассирный отчет                           //   "ReportCashier"
  bool report_ware;
  bool report_unload;	    // выгрузка новых транзакций		   //   "ReportUnload"
  bool report_psreconsiliation;   // сверка итогов платежной системы
  
  /*REDUCTIONS SECTION*/
  /*Manual reductions section*/
  //указываются в целых процентах от суммы позиции или чека.
  double max_position_discount;                         //XX                           //   "MaxStandDiscount"
  double max_position_increase;                         //XX                           //   "MaxStandIncrease"
  double max_check_discount;                            //XX                           //   "MaxChequeDiscount"
  double max_check_increase;                            //XX                           //  "MaxChequeIncrease"
  
  /*general section*/
  bool recalc_autored_each_registration;     //XX                               //      "AutoRecountDiscount" 
  bool disc_exception_mode;                                                      //      "WithoutStandDiscount"
  int result_discount;   //   итоговая скидка  //объединяется, суммируется, максимальная   //  "ResultDiscount"
  char* base_discount_order;  // порядок объединения(строка цифер - номеров скидок  по очереди ) 
                 // 1 - фиксированная    2 - произвольная                                                   //   "BaseDiscountOrder"
  
  /*PRINTING CHECK SECTION*/
  bool print_position_taxes;                        // "PrintTax"
  bool cut_position_name;                           // "WordWrap"
  bool print_one_registration;  							// "PrintOneReg"

  /*Time*/  /*KKM*/
  bool control_time_synchronization; //  not не производить контроль времени на кассе   //   "Synchro"
  bool auto_synchronization;      //автоматически синхронизовать время                   //   "AutoSynchro"
  //int time_for_block;                                                
  int time_for_synchronization;    // нижняя граница диапозона синхронизации          //   "SynchroFrom"
  int time_for_synchronization_max;  // верхняя граница диапозона синхронизации      //   "SynchroTo"   
  /*warn about need synchro*/
  bool warn_synchro_in_clock_field;      // в поле часов                                                            //   "NotifySynchroTimeField"
  bool warn_synchro_after_close_check;   // сообщение после закрытия чека                                 //   "NotifySynchroMessage"
  
  /*open money box*/
  bool open_box_by_pay_with_rest;   // открывать денежный ящик при оплате со сдачей   //   "OpenBoxByRest"
  bool open_box_by_close_cheque;   // -\\- при закрытии чека                                          //   "OpenBoxByCloseCheque"
  bool open_box_by_change_money;   // -\\- при внесении / выплате денежных средств         //   "OpenBoxByChangeMoney"
  bool open_box_by_clear_report;   // -\\- при снятии отчета с гашением                        //      "OpenBoxByClearReport"
  
/*Devices section*/
  /*Cusomer Display*/ /*message for pause work*/ 
  char * cdsp_up_pause_work;      // текст верней строки при простое    "StandUpString"
  int cdsp_up_pause_work_mode_run;   // способ вывода верхней строки при простоях "StandUpStringRun"
  char * cdsp_down_pause_work;   //  текст нижней строки                "StandDownString"
  int cdsp_down_pause_work_mode_run;  // способ вывода нижней строки при простоях "StandDownStringRun"
  /*message for not work*/
  char * cdsp_up_not_work;      // текст верней строки в не рабочем состоянии  "OffUpString"
  int cdsp_up_not_work_mode_run;   // способ вывода верней строки в не раб состоянии    "OffUpStringRun"
  char * cdsp_down_not_work;   // текст нижней строки                   "OffDownString"
  int cdsp_down_not_work_mode_run;   // способ вывода нижней строки             "OffDownStringRun"
  
  /*product registration detalization */
  bool cdsp_detalize_product;                // детализация при регистрации общая для товара      // "DisplayProduct"
  bool cdsp_detalize_product_name;           // детализация товара - наименование                       // "DisplayProductName"
  bool cdsp_detalize_product_price;         // цена                                                                     // "DisplayProductPrice"
  bool cdsp_detalize_product_quantity;      // количеаство                                                         // "DisplayProductQuantity"
  bool cdsp_detalize_product_sum;            // сумма                                                                   //"DisplayProductSum"
  bool cdsp_detalize_product_total;         // промитог                                                               //  "DisplayProductTotal"
  
  int cdsp_detalize_product_name_string;   // в какой строке печатать верхней или нижней         // "DisplayProductNameString"
  int cdsp_detalize_product_properties_string; // в какой строке печатать цену, кол-во, сумму               // "DisplayProductPriceString"
  int cdsp_detalize_product_total_string;                                                                                  // "DisplayProductTotalString"
       
  int cdsp_up_detalize_product_mode_run;   // способ вывода вернхней строки                              // "DisplayProductUpStringRun"
  int cdsp_down_detalize_product_mode_run;   // способ вывода нижней строки                                 //  "DisplayProductDownStringRun"
  
  /* discount registration detalize */
  bool cdsp_detalize_discount;             // детализация при регистрации для скидок               //  "DisplayDiscount"
  bool cdsp_detalize_discount_name;      //   название                                                              //  "DisplayDiscountName"         
  bool cdsp_detalize_discount_sum;      // сумма                                                                      //  "DisplayDiscountSum"  
  
  int cdsp_detalize_discount_name_string;   // в каккой строке выводить                                     //  "DisplayDiscountNameString"  
  int cdsp_detalize_discount_sum_string;                                                                                  //  "DisplayDiscountSumString"        
  
  int cdsp_up_detalize_discount_mode_run;   // способ вывода верхней строки                               //  "DisplayDiscountUpStringRun"  
  int cdsp_down_detalize_discount_mode_run;   // способ вывода нижней строки                               //  "DisplayDiscountDownStringRun"     
  
  
  
  vector<SectionInfo> sections;
  
};
#endif




