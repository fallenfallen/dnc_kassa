

#ifndef CLASS_REGISTRATION
#define CLASS_REGISTRATION

#include "class_pgsql.h"
#include "receipt.h"
#include "setup_param.h"

struct VisualSearchElement {
    char longtitle[401];
    char shortcut[401];
    char price[20];
    char quantity[20];
    char barcode[41];
    char internal_code[41];
    bool is_addition;
    char addition_id[21];
};

class Registration : public PGSQL
{
  private:
    SP * sp;
  public:
    Registration();
    Registration(const char *dbname, const char *user, const char *password, const char *host, const char *hostaddr, const char *port, const char * log_path, SP * sparam);
    Registration(SP * sparam, const char * conf_path);
    ~Registration();    
  private:
    /*Get Product private methods*/
    int Bar_Get_Product(const char *code);
    
    int Internal_Get_Product(const char *code);
    //-----(*)-----Zuskin-----13/10/2011-----
    //int Check_Product_Atr(double q, bool return_fl);
    int Check_Product_Atr(double q, bool return_fl, bool qttyset = false);
    //---------------------------------------
    void Get_Check_Position(const char *code, bool fl, int &npos, Receipt *rec, const char* addition_id);
    int Query_by_Internal_Code(const char *code);
    int Query_by_Bar_Code(const char * code);
    int Query_Addition_by_Bar_Code(const char* code);
    
    int Query_Pattern(const char *prefix);
    int Query_Taxes(const char *taxgroup);
    //int Query_Rates(char *taxrate);
    
    /*Visual Search private methods*/
    int Query_Group_Cnt(const char *gr);
    int Query_Group(const char *gr);
    
    int Query_Goods_Cnt(const char *gr);
    int Query_Goods_Cnt_Available(const char *gr);
    
    int Query_Goods(const char *gr);
    int Query_Parent_Group(const char *gr);
    int Query_by_Internal_Code_Pattern(const char *pattern);
    int Query_by_Bar_Code_Pattern(const char *pattern);
    int Query_by_Shortcut_Pattern(const char *pattern);
    
    int Query_Goods_Available(const char *gr);
    int Query_by_Internal_Code_Pattern_Available(const char *pattern);
    int Query_by_Bar_Code_Pattern_Available(const char *pattern);
    int Query_by_Shortcut_Pattern_Available(const char *pattern);
    
    
    int Query_Cnt_by_IntCode(const char *pattern);
    int Query_Cnt_by_BarCode(const char *pattern);
    int Query_Cnt_by_ShCut(const char *pattern);
    
    int Query_Cnt_by_IntCode_Available(const char *pattern);
    int Query_Cnt_by_BarCode_Available(const char *pattern);
    int Query_Cnt_by_ShCut_Available(const char *pattern);
    
    int GetAdditionLongtitle(char * &longtitle, char * &shortcut, const char* addition_id);
    int Get_Product_By_Addition_Id(const char * addition_id);
    
    int Update_Addition_Quantity(const char* quantity, const char* addition_id);
        
    /*Autoreduction private methods*/
		/*
    int AddCheckReduction(int cpos);
    void Get_Day_of_Week(const char * wd);
    int Insert_Check_Scheme(const char* id, const char* name, char* shortcut, char * checkpos, char *value);
    int Delete_Check_Scheme(char *cpos);
    int Query_Reductions(char *wd, char * ps, char * cs, char *q, char *sch, char * area);
    int Query_Info_Check_Discount();
*/
    
    /*Fixreduction private methods*/
    int Query_List_by_Area(const char *area);
    
    /*Update*/
    int Update_Quantity(const char *quant, const char *code);
    
    int GetFlags(const char* ware_code, int &flags );
    
  public:
      int internal_tmp;
      
    /*Get Product  public methods*/
    int Add_Taxes(Receipt *rec, int npos);
    //-----(*)-----Zuskin-----13/10/2011-----
    //int New_Receipt_Record(const char *code, double q, Receipt *rec, bool code_fl, bool return_fl, bool cancel_fl, int &npos, const char * addition_id = "-1");
    int New_Receipt_Record(const char *code, double q, Receipt *rec, bool code_fl, bool return_fl, bool cancel_fl, int &npos, const char * addition_id = "-1", bool qttyset = false);
    //---------------------------------------
 //   int Update_Quantity(char *code, char *quant, bool cfl, bool sfl);
    int Overflow_Control(double checksum, double newsum, double oldsum);
    int Quantity_Control(const char *code, double q, const char* addition_id);
    int Get_Pattern_Param(const char *code, double *quantity, char *internal_code);
    
    /*Visual Search public methods*/
    int Get_Max_Quantity(const char * int_code, int & weit, double & max_quantity);
    int Get_Parent_Info(const char *group_id, char *name, char *pgroup);
    int Get_Num_Info(const char *group_id, bool fl);
    
    int Get_Num_Info_Available(const char *group_id, bool fl);
    
    int Get_List_By_Parent(const char *group_id, bool fl);
    int Get_List_By_Parent_Available(const char *group_id, bool fl);
    void Get_Group_Info(char *name, char *group_id, int i, int n);
    void Get_Prod_Info(char *longtitle, char *shortcut, char *price, char *quantity, char *bar_code, char *internal_code, int i, int n);
    VisualSearchElement GetProductInfo(int i, int n);
    
    int Get_Num_Search_Info(const char *pattern, int pattern_type);
    
    int Get_Num_Search_Info_Available(const char *pattern, int pattern_type);
    
    int Get_List_By_Pattern(const char *pattern, int pattern_type);
    int Get_List_By_Pattern_Available(const char *pattern, int pattern_type);
    
    int Get_Price_Control(const char* code);
    int GetQuantityEditInfo(const char* ware_code, double q);
    int GetQuantityEditFlag(const char* ware_code);
    int GetQuantityEditFlag(const char* ware_code, bool type_code);
    
    /*Autoreduction public methods*/
		/*
    int Get_Position_Reductions(double* value, int * type, char* name, char* shortcut, double quantity, double sum, int codescheme);
    int Get_Check_Reductions(double quantity, double sum, int codescheme, int cpos);
    int Get_Max_Check_Discount(double *value, char * name, char *shname);
    int Delete_All_Check_Scheme();
    */
    
    /*Fixreduction public methods*/
    int Get_List(const char *area);
    void Get_List_Position(char *&code, char *&value, char *&name, char *&shortcut, int *type, char * & incode, char* & cardcode, int i, int n);
    int GetDiscountBarcodeInfo(const char* barcode, char*& code, char*& name, char*& shourtcut, double &value, int &type, bool &area, char*& card_code);
    int GetDiscountCardcodeInfo(const char* card_code, char*& code, char*& name, char*& shortcut, double &value, int &type, bool &area, char*& barcode);
    //-----(+)-----Zuskin-----15/07/2011-----
    int GetSchemeIdByCode(const char* code);
    int GetListBySchemeId(int schemeid);
    void GetRuleFromList(int i, string& value, string& type, string& longtitle, string& shortcut, string& begindate, string& enddate, string& begintime, string& endtime, string& weekdays, string& check_sum, string& accsum);
    void ClearRulesList(void);
    //-----(+)-----Zuskin-----14/12/2011-----
    int GetDefaultSchemeId(void);
    //---------------------------------------
    //-----(+)-----Zuskin-----22/07/2011-----
    double GetSummByCard(const char* code);
    int GetTypeByCard(const char* code);
    int UpdateBonusCard(const char* code, double bsumm);
    double GetMaxPercentByCard(const char* code);
    double GetPercentByCard(const char* code);
    //---------------------------------------
    //-----(+)-----Zuskin-----03/08/2011-----
    int Query_Cnt_By_Price(double price);
    int GetListByPrice(double price);
    void GetWareFromList(int i, char*& code, char*& addcode);
    //---------------------------------------
    
    //-----(+)-----Zuskin-----18/11/2011-----
    int Query_Cnt_Managers(void);
    int Query_Managers_List(vector<string>* id_man_list, vector<string>* nam_man_list, string nameptrn = "", string bcodeptrn = "", string cardptrn = "");
    //---------------------------------------
    
    /*Update*/
    int Update_Goods_Quantity(Receipt *rec);
    
    /*added*/
    int GetBarcodeCoefficient(const char* barcode_str, const char* code_str, double & coeff);
    //-----(*)-----Zuskin-----03/08/2011-----
    //int VisualSearchByAll( const char* barcode, const char* code, const char* title, bool hide_absent_goods);
    int VisualSearchByAll( const char* barcode, const char* code, const char* title, bool hide_absent_goods, double price = -1);
    //---------------------------------------
    int GetCountWareByBarcode(const char *barcode, const char * code);
    
    int GetWareShortcut(const char* wcode, char* &w_shorcut);
    
    // возвращает внутренний код товара по штьрих-коду
    int GetWareInternalCodeByBarcode();
    
    // проверка ограничения продажи по времени
    //---(*)---Zuskin---24/02/2012---
    //bool getSaleAccessByTime();
    bool getSaleAccessByTime(const char* code);
    //-------------------------------
};

#endif



















