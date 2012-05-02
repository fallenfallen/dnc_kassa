#include "hwsrv.h"
#include "debug_level_log.h"
#include "receipt.h"
#include "trans_pgsql.h"
#include "setup_param.h"


#ifndef INSERT_TRANSACTION_IN_FORMS
#define INSERT_TRANSACTION_IN_FORMS

class InsertTransactions
{
  private :
    InsertTransactions();
  public :
    InsertTransactions(HWSRV * hw_ptr, Receipt * rec_ptr, Trans * tr_ptr, DebugLevelLog * log_ptr);
    ~InsertTransactions();

  private :
    HWSRV * hw;
    Receipt * receipt;
    Trans * Tr;
    SP * sp;

    DebugLevelLog * log;

    char * cur_receipt_datetime;
		
    bool init_flag;
    int user_id;

  public :
    
    void SetUserId(int auser_id);	
    void SetSP(SP * asp);
    void CreateDateTimeString();
    
    //-----(*)-----Zuskin-----18/11/2011-----
    //int InsertCheckRegistrationTransactions(const char * check_type, const char * card, double nal, double cred, double cont, bool closed_session_flag, bool close_check = true);
    int InsertCheckRegistrationTransactions(const char * check_type, const char * card, double nal, double cred, double cont, bool closed_session_flag, bool close_check = true, string manager_id = "");
    //---------------------------------------

    int InsertEnteringGettingTransaction(int oper_type, const char * sum_str, bool closed_session_flag, int section_num);
	
    int InsertReportTransactions(int rep_type, int session, double cash_sum, double gain_sum, double total_sum);

  private :
    int InsertOpenChangeTransactions(const char * n_rec, const char * n_doc, const char * n_pc, const char * n_kkm, const char * seller, const char * n_session, bool closed_session_flag);

    int InsertRegistrationsTransactions(const char * n_rec, const char * n_doc, const char * n_pc, const char * n_kkm, const char * seller, const char * n_session, const char * check_type);

    int InsertReductionTransactions(const char * n_pc, const char * n_doc, const char * n_kkm, const char * seller, const char * n_session, const char * n_rec, const char * check_type, int reduction_type, int i, int j);

    int InsertTaxTransactions(const char * n_pc, const char * n_kkm, const char * seller, const char * n_session, const char * n_rec, const char * n_doc, const char * check_type, int i);

    int InsertPaymentRegistrationTransactions(const char * n_rec, const char * n_doc, const char * n_pc, const char * n_kkm, const char * seller, const char * n_session,const char * check_type, double nal, double cred, double cont);
    
    //-----(*)-----Zuskin-----18/11/2011-----
    //int InsertCloseCheckTransactions(const char *n_rec, const char * n_doc, const char * n_pc, const char * n_kkm, const char * seller, const char * n_session, const char * check_type, const char * card, double nal, double cred, double cont, bool close_check = true);
    int InsertCloseCheckTransactions(const char *n_rec, const char * n_doc, const char * n_pc, const char * n_kkm, const char * seller, const char * n_session, const char * check_type, const char * card, double nal, double cred, double cont, bool close_check = true, string manager_id = "");
    //---------------------------------------
    
    void CreateDateString(char * cur_receipt_date);
    void CreateTimeString(char * cur_receipt_time);
    
     char * NumberToStr(double Nmb, int Factor);

};

#endif
