#include "class_pgsql.h"

#ifndef TRANS_CLASS
#define TRANS_CLASS

#include <malloc.h>
#include <string.h>
#include "receipt.h"
#include "setup_param.h"

#include "copyinfo.h"

class Trans : public PGSQL
{
  private :
    SP * sp;
  public:
    Trans();
	Trans(SP * sparam, const char * conf_path);
    Trans(char *dbname, char *user, char *password, char *host, char *hostaddr, char *port, char* log_path);
    ~Trans();
    
  public:
    int Insert_Registration(const char * type, const char *datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * code, const char* barcode, const char * price, const char * quantity, const char* addition_id, const char* result_position_sum, const char* section);
    
    
    
    int Insert_Tax(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * w_code, const char * gr_code, const char * rate_code, const char * sum);
    
    int Insert_Reduction(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * w_code, const char * reduction_type, const char * reduction_info, const char * auto_scheme_id, const char * sum, const char * rate, const char* recount_sum, const char * recount_percent);
    
    int Insert_Payment(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * payment_type, const char * sum, const char * change);
    int Insert_InOutcome(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char *sum, const char* section_number);

//-----(*)-----Zuskin-----18/11/2011-----
  //int Insert_Close_Check(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * card, const char * client_sum, const char * check_sum, const char* round_sum);
  int Insert_Close_Check(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_receipt, const char * n_doc, const char * check_type, const char * card, const char * client_sum, const char * check_sum, const char* round_sum, string manager_id);
//---------------------------------------

    int Insert_Change_Data(const char * type, const char * datetime, const char * n_pc, const char * n_kkm, const char * seller, const char * n_change, const char * n_doc, const char * gain_sum, const char * cash_sum, const char * total_sum);

    int Get_Check_List(char *check, char *n_kkm/*, char *n_fr*/, char**& dt, int& n, char** &n_doc);
//    int Get_Check_Info(char *check, char *cash, char *date, char *time, Receipt *rec, bool fl, int *type);
//    int Get_Check_List(char *check, char *cash, char** &d, char** &t, int& n);
//    int Get_Check_Info(char *check, char *cash, char *date, char *time, Receipt *rec, bool fl, int *type);
//-----(*)-----Zuskin-----13/01/2012-----
    //int Get_Registration_Check_Info(char *receipt, char *n_pc, char *datetime, char* n_doc, Receipt *rec, CopyInfo * copy, bool is_copy);
    int Get_Registration_Check_Info(char *receipt, char *n_pc, char *datetime, char* n_doc, Receipt *rec, CopyInfo * copy, bool is_copy, int& manager_id);
//---------------------------------------    
    
    int Get_Chage_Opened(char *&dt, const char * serial);
    
    
    /*Added by Shonda*/
    int Get_Session_Info( const char* check_type, const char* seller_num, int &count_check, double & result_sum );
    int Get_Session_Payment_Info( const char* check_type, const char* seller_num, double &payment_sum1, double &payment_sum2, double &payment_sum3 ); 
    int Get_Cashier_By_Session(char ** & users, int &count_users );
    
    int Get_Cashier_Discount_Info(const char* seller_num, int &pos_discount_count, double &pos_discount_sum, int &pos_charge_count, double &pos_charge_sum, int &check_discount_count, double &check_discount_sum, int &check_charge_count, double &check_charge_sum);
    
    int GetSessionWare(char** & ware_codes, int & ware_count);
    int GetWareInfo(const char* wcode, double & ware_quantity, double & ware_sum);
    int GetReturnWareInfo(const char* wcode, double & ware_quantity, double & ware_sum);
    int GetResultSumByWare(int &sale_count, double& sale_sum, int& return_count, double& return_sum);
    int GetDiscountInfoByWare(int &pos_discount_count, int &check_discount_count, double &pos_discount_sum, double &check_discount_sum, double &total_sale_discount_sum);
    
    int GetResultRoundSum(double &sale_round_sum, double &return_round_sum);
    
//-----(+)-----Zuskin-----03/05/2011-----
    int GetIncomeSumBySection(const char* section, double &income_sum);
    int GetOutcomeSumBySection (const char* section, double &outcome_sum);
    int GetSalesSumBySection(const char* section, double &sales_sum);
    int GetReturnsSumBySection(const char* section, double &return_sum);
    int GetCanceledSumBySection(const char* section, double &cancel_sum);
    int GetStornoSumBySection(const char* section, double &storno_sum);
    int GetCanceledCountByCashier(const char* cashier, double &cancel_count);
    int GetCanceledSumByCashier(const char* cashier, double &cancel_sum);
    int GetStornoCountByCashier(const char* cashier, double &storno_count);
    int GetStornoSumByCashier(const char* cashier, double &storno_sum);
    
//---------------------------------------    
//-----(+)-----Zuskin-----24/10/2011-----    
    int Get_PutOut_Checks( vector<string>* npclist, vector<string>* checklist, vector<string>* date_time, vector<string>* ndoc, vector<string>* chtype, vector<string>* summ, string check="", string n_pc="");

//-----(*)-----Zuskin-----13/01/2012-----
//    int Get_PutOut_Check_Info(string n_pc, string receipt, string n_doc, string datetime, Receipt *rec, CopyInfo * copy, bool return_fl = false); 
    int Get_PutOut_Check_Info(string n_pc, string receipt, string n_doc, string datetime, Receipt *rec, CopyInfo * copy, int& manager_id, bool return_fl = false); 
//---------------------------------------    

    int Mark_PutOut_Check( string n_pc, string ncheck, string datetime);
//---------------------------------------
//-----(+)-----Zuskin-----13/01/2012-----
    int GetPutOutCheckNumber(void);
//---------------------------------------    

  private:
    int AnalizeTaxes(Receipt * rec, Receipt * test);
    
    int Query_Get_Check_Cashier(char *receipt, char *n_pc, char *datetime, char* n_doc);
	    
    int Query_by_Check_Number(char* check, char * n_pc/*, char *n_kkm*/);
    int Query_Info_of_Registration_Check(char *receipt, char *n_pc,/* char *n_kkm,*/ char *datetime, char* n_doc);
//    int Query_Info_of_Check(char *Nrec, char *Ncash, char *date, char *time);
    
    int Query_ChageOpened_DateTime(const char * serial);
    
    int Query_Payment_Sum(const char* check_type, const char* seller_num, const char* payment_type);
};
#endif
