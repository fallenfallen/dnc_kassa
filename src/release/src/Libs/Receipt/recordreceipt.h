
#ifndef RECORD_RECEIPT
#define RECORD_RECEIPT


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "discount.h"

using namespace std;

class Record_Receipt
{
  private:
    string code;
    string barcode;
    string shortcut;
    string longtitle;
    string addition_id;

  public:
      
   vector<Discount> discount;
      
    int auto_scheme;
    int fix_scheme;
    int tax_group;
    int flags; //флаги из БД
  
    double quantity;
    double price;
    double min_price;
    double sum;
    double dsum;
    
    // номера секций управляются в receipt
    
    // номер квитанции на которой печатать данную позицию
    int section;
    
    // номер секции в ктр печатать позицию
    // в случае отсутствия вирт секции печатается в эту аппаратную секцию фр
    // записывается в транзакции
    int section_number; 
    bool sale_limit_by_time;
    
    double check_discount_sum;
    
    // сумма позиции с учетом скидок на позицию и скидок на чек, распределенных на позиции
    double GetPositionResultSum();
 
    bool code_flag; // true - internal code; false - barcode
    bool enable_flag;

    Record_Receipt();
    //Record_Receipt(const Record_Receipt & r);
    ~Record_Receipt();
    
    Discount * GetDiscountElemRef(int index);

    const char * GetBarcode();
    const char * GetCode();
    const char * GetShortcut();
    const char * GetLongtitle();
    const char * GetAdditionId();

    int GetLenghtBarcode();
    int GetLenghtCode();
    int GetLenghtShortcut();
    int GetLenghtLongtitle();
    int GetLenghtAdditionId();

    void Add_Record_Receipt(const char *barcode_str, const char *code_str, const char *shortcut_str, const char *longtitle_str, double quant, double pr, int fls, bool cflag/*code flag = true, if code; false, if bar code*/, int a_sch, int f_sch, int t_gr, double m_pr, const char* addition_id_str);
    
    int Add_Sum_Discount(int i, double s, const char * code, const char *name, const char *shortcut, bool manual_fl, int control_min_price);
    int Add_Percent_Discount(int i, double p, const char * code, const char *name, const char *shortcut, bool manual_fl, int control_min_price); 
    
    double RecalculationDiscounts(double s);
    double RecalculationDiscounts();
    
//  private :
//    double Max_Discount(double sum);
};


#endif
