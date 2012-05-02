#include "class_pgsql.h"

#ifndef UPLOAD_PGSQL
#define UPLOAD_PGSQL

class PgUpload : public PGSQL 
{
  public :

    PgUpload();
    PgUpload(const char * conf_path);
    PgUpload(char* dbname, char *user, char *password, char *host, char * hostaddr, char *port, char* log_path);
    ~PgUpload();

    int ReplaceWare(const char *internal_code, const char *bar_code, const char *longtitle, const char *shortcut, const char *price, const char * quantity, const char * auto_scheme_id, const char * flags, const char * min_price, const char * best_before,  const char * coefficient, const char * group_code, const char* tax_group_id, bool sale_flag);
    int ReplaceGroup(const char * code, const char * longtitle, const char * shortcut, const char * group_code);
    int AddWare(const char *internal_code, const char *bar_code, const char *longtitle, const char *shortcut, const char *price, const char * quantity, const char *auto_scheme_id, const char *flags, const char *min_price, const char *best_before,  const char * coefficient, const char *group_code, const char *tax_group_id);
    int AddUser(char * code, char * longtitle, char * shortcut, char * profile_code, char * pass, char * card);
    
    int AddFixReduction(const char * code, const char * longtitle, const char * shortcut, const char * type, const char * value, const char * area, const char* card, const char* barcode, const char* show = "'t'");    
    int AddAutoReductionScheme(char * code, char * longtitle, char * shortcut);
    
    int AddAutoReduction(char * scheme_code, char * code, char * longtitle, char * shortcut, char * type, char * value, char * begin_date, char * end_date, char * begin_time, char * end_time, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end);
    int AddTaxRates(char * code, char * longtitle, char * shortcut, char * value);
    int AddTaxGroups(char * code, char * longtitle, char * shortcut);
    int AddTaxRatesGroups(char * code, char * group_code, char * rate_code, char * base_flag);

    int Insert_Barcode(const char* internal_id, const char* barcode, const char * coefficient);
    int Delete_All_Barcodes(); 
    
    int DeleteFromTable(int command_ind);
    int DeleteAllWaresGroups();
    int DeleteWaresGroupsByCode(char *code);
    
    int Add_Barcodes(const char* internal_id, const char* bar_code, const char * coefficient);

  private :
	// INSERTS
    int Insert_Ware(const char *internal_code, const char *longtitle, const char *shortcut, const char *price, const char * quantity, const char * auto_scheme_id, const char * flags, const char * min_price, const char * best_before, const char * group_code, const char* tax_group_id, bool sale_flag);
    int Insert_Group(const char * code, const char *longtitle, const char * shortcut, const char *group_code);
    int Insert_User(char * code, char * longtitle, char * shortcut, char * profile_code, char * pass, char * card);
    int Insert_Fix_Reduction(const char * code, const char * longtitle, const char * shortcut, const char * type, const char * value, const char * area, const char* card, const char* barcode, const char* show);
    int Insert_Auto_Reduction_Scheme(char * code, char * longtitle, char * shortcut);
    int Insert_Auto_Reduction(char * scheme_code, char * code, char * longtitle, char * shortcut, char * type, char * value, char * begin_date, char * end_date, char * begin_time, char * end_time, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end);
    int Insert_Tax_Rate(char * code, char * longtitle, char * shortcut, char * value);
    int Insert_Tax_Group(char * code, char * longtitle, char * shortcut);
    int Insert_Tax_Rate_Group(char * code, char * group_code, char * rate_code, char * base_flag);

    // UPDATES
    int Update_Ware(const char * internal_code, const char * longtitle, const char * shortcut, const char * price, const char * quantity, const char * auto_scheme_id, const char * flags, const char * min_price, const char * best_before, const char * group_code, const char* tax_group_id, bool sale_flag, bool add_flag);
    int Update_Group(const char * code, const char *longtitle,  const char * shortcut, const char *group_code);
    int Update_User(char * code, char * longtitle, char * shortcut, char * profile_code, char * pass, char * card);
    int Update_Fix_Reduction(const char * code, const char * longtitle, const char * shortcut, const char * type, const char * value, const char * area, const char * card, const char* barcode, const char* show);
    int Update_Auto_Reduction_Scheme(char * code, char * longtitle, char * shortcut);
    int Update_Auto_Reduction(char * scheme_code, char * code, char * longtitle, char * shortcut, char * type, char * value, char * begin_date, char * end_date, char * begin_time, char * end_time, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end);
    int Update_Tax_Rate(char * code, char * longtitle, char * shortcut, char * value);
    int Update_Tax_Group(char * code, char * longtitle, char * shortcut);
    int Update_Tax_Rate_Group(char * code, char * group_code, char * rate_code, char * base_flag);

    // QUERIES
    int Query_By_Internal_Code(const char * internal_code);
    int Query_Group_By_Code(const char * code);
    int Query_By_User_Id(char * code);
    int Query_By_Fix_Reduction_Id(const char * code);
    int Query_By_Auto_Reduction_Scheme_Id(char *code);
    int Query_By_Auto_Reduction_Id(char * code);
    int Query_By_Tax_Rate_Id(char * code);
    int Query_By_Tax_Group_Id(char * code);
    int Query_By_Tax_Rate_Group_Id(char * code);

    // DELETES
    int DeleteFromTable(const char * table);
    int Delete_All_Groups();
    int Delete_All_Wares();

    int Delete_Groups_By_Code(char * code);
    int Delete_Wares_By_Code(char * code); 
    
    int Delete_Barcode_By_Ware (const char* internal_id); 
    int Delete_Barcode_By_Group(const char* group_id);
    
    int findsep(char* str, char sep, int start = 0);

    
    int CheckMainBarcode(const char * internal_id); // return ntuples , if state < 2 then  = -1

    //int Delete_All_Fix_Reductions();
   // int Delete_All_Auto_Reductions();
   // int Delete_All_Auto_Schemes();

    // PACK PARAMETERS TO STRING
    void PackInsertWareParameters(char *& pack_param, const char *internal_code, const char *longtitle, const char *shortcut, const char *price, const char * quantity, const char * auto_scheme_id, const char * flags, const char * min_price, const char * best_before, const char * group_code, const char* tax_group_id);
    void PackUpdateWareParameters(char *& pack_param, const char *longtitle, const char *shortcut, const char *price, const char * quantity, const char * auto_scheme_id, const char * flags, const char * min_price, const char * best_before, const char * group_code, const char* tax_group_id, bool add_flag);
		
    void PackInsertGroupParameters(char *& pack_param, const char *code, const char *longtitle, const char * shortcut, const char *group_code);
    void PackUpdateGroupParameters(char *& pack_param, const char *longtitle,  const char * shortcut, const char *group_code);

    void PackInsertUserParameters(char *& pack_param, char * code, char * longtitle, char * shortcut, char * profile_code, char * pass, char * card);
    void PackUpdateUserParameters(char *& pack_param, char * longtitle, char * shortcut, char * profile_code, char * pass, char * card);
	
    void PackInsertFixReductionParameters(char *& pack_param, char * code, char * longtitle, char * shortcut, char * card_prefix, char * type, char * value, char * begin_date, char * end_date, char * area);
    void PackUpdateFixReductionParameters(char *& pack_param, char * longtitle, char * shortcut, char * card_prefix, char * type, char * value, char * begin_date, char * end_date, char * area);
		
    void PackInsertAutoSchemeParameters(char *& pack_param, char * code, char * longtitle, char * shortcut);
    void PackUpdateAutoSchemeParameters(char *& pack_param, char * longtitle, char * shortcut);
		
    void PackInsertAutoReductionParameters(char *& pack_param, char *code, char * scheme_code, char * longtitle, char * shortcut, char * type, char * value, char * begin_date, char * end_date, char * begin_time, char * end_time, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end);
    void PackUpdateAutoReductionParameters(char *& pack_param, char * scheme_code, char * longtitle, char * shortcut, char * type, char * value, char * begin_date, char * end_date, char * begin_time, char * end_time, char * weekdays, char * quantity, char * position_sum, char * check_sum, char * card_prefix_begin, char * card_prefix_end);

    void PackInsertTaxRateParameters(char *& pack_param, char * code, char * longtitle, char * shortcut, char * value);
    void PackUpdateTaxRateParameters(char *& pack_param, char * longtitle, char * shortcut, char * value);

    void PackInsertTaxGroupParameters(char *& pack_param, char * code, char * longtitle, char * shortcut);
    void PackUpdateTaxGroupParameters(char *& pack_param, char * longtitle, char * shortcut);

    void PackInsertTaxRateGroupParameters(char *& pack_param, char * code, char * group_code, char * rate_code, char * base_flag);
    void PackUpdateTaxRateGroupParameters(char *& pack_param, char * group_code, char * rate_code, char * base_flag);
    
//  protected :
    
};

#endif
