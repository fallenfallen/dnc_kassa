#ifndef RECEIPT
#define RECEIPT

#define ALLOWFRACTIONAL 1
#define ALLOWREGISTRATION 2
#define ALLOWRETURN 4
#define ALLOWNEGATIVEQUANTTITY 8
//-----(+)-----Zuskin-----13/10/2011-----
#define ALLOWWITHOUTQTTY 16
//---------------------------------------
#define WRITEOFFREMAINS 32
#define PRICEEDIT 64
#define QUANTITYEDIT 128

#define ALLOWPRICEREDUCTION 64
#define CONTROLQUANTITY 32


#define MAXLENBARCODE 13 // максимальная длина штрих-кода
#define MAXLENINTCODE 10  // максимальная длина внутреннего кода
#define MAXLENSHORT   20   // максимальная длинна краткого наименования
#define MAXLENLONG    150  // максимальная длина полного наименования

#define MAXLENDISCCODE 20 // максимальная длина кода скидки
#define MAXLENDISCNAME 50 // максимальная длина нименования скидки
#define MAXLENDISCSHORT 20 // максимальная длина краткого наименования

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "discount.h"
#include "recordreceipt.h"
#include "positiontaxes.h"

using namespace std;

struct WareAdditionElement {
    char addition_id[10];
    char addition_text[1000];
    char addition_price[15];
    char addition_quantity[17];
    char addition_validity_date[15];
};


enum RoundType { LESS_ROUND = 0, GREATER_ROUND = 1, BETTER_ROUND = 2 };

enum ReceiptType {CHECK_FREE, CHECK_SELL, CHECK_RETURN, CHECK_COPY};



class ReceiptSection {
public:
    int count_position;
    double discount_sum;
    double result_section_sum;	    
};



class Receipt
{
private :
    //   int size;
    ReceiptType receipt_type;
    int receipt_number;
    bool closed;
  
    vector<ReceiptSection> sections;
    
    // return right discount sum or -1 if error
    double TestMinPrice(double procent); 	    
  
public :
    vector<Discount> discount;
    vector<Record_Receipt> records;
    vector<PositionTaxes> positiontaxes;  
    
    int record_num;
    int receipt_num;
    double sum;
    double dsum;
    string date;
    string cashier;
    int doc_number;
    int n_session;
    string kkm_serial;
    
    double round_sum;
    int section_num_with_max_sum;
    
    
    // скидка начисленная на чек пользователем из интерфейса !!!
    // меняется в случае начисления скидки, отмены / замены скидки !!!
    // не меняется при пересчете суммы скидки на чек !!!
    // храниться сумма скидки (процент расчитывается от суммы позиций с учетом скидок на позиции) - Sum() !!!!
    // эта скидка записывается в транзакции 35-38 !!!
    // TestMinPrice делается только от этой суммы, а не от dsum !!! 
    // Иначе сумма скидки будет уменьшаться при каждом запуске алгоритма TestMinPrice
    
    double transaction_discount_sum;
    void RecalcTransactionDiscount();
    double GetTransactionDiscountPercent(int i);
    double GetTransactionDiscountSum(int i);
    
    double GetRecountTransactionDiscountPercent(int i);
    double GetRecountTransactionDiscountSum(int i);
    
    
    //Конструктор
    Receipt(SP * sp_rec);
    //Receipt(const Receipt & r);
    //Receipt();
    //Receipt(const char *code, const char *shortcut, const char *longtitle, double quantity, double price, short nrec, int fls, bool cfl, int a_sch, int f_sch, int t_gr, double m_pr);
    //Деструктор
    ~Receipt();
    
    double RoundCheckSum(double check_sum);
    
    ReceiptType GetCheckType();
    void SetCheckType(ReceiptType rt);
    void setCashier(const char*  cashier_name) { cashier = cashier_name;  }
    const char* getCashier() {  return cashier.c_str();  }
    void setDate(const char* d);
    const char* getDate();
    
    int GetCheckNumber();
    void SetCheckNumber(int check_number);
    
    int GetDocumentNumber() { return doc_number;   }
    void SetDocumentNumber(int num) { doc_number = num; }
    
    int GetSession() {  return n_session;   }
    void SetSession(int n_s) { n_session = n_s;  } 
    
    const char* GetKkmSerial() {  return kkm_serial.c_str();   }
    void SetKkmSerial(const char* k_s) { kkm_serial = k_s;  }
    

    Discount * GetDiscountElemRef(int index);
    Record_Receipt * GetPosElemRef(int index);
    PositionTaxes * GetTaxElemRef(int index);
    
    void CloseReceipt();
    bool GetStateofClosing();
    
    int FindEquilPosition(const char *code, bool flagcode, const char* addition_id);
    double QuantityWareByCode(const char* code, bool flagcode);
    void Add_Record(const char *barcode, const char *code, const char *shortcut, const char *longtitle, double quantity, double price, int fls, bool cfl, int a_sch, int f_sch, int t_gr, double m_pr, const char* addition_id, int section);
    
   // void Add_Taxes(int tax_group, int n, TaxRate ** tr);
    void Add_Taxes_Info(int tax_group, const char* tg_name, int n);
    int Add_Percent_Discount(int i, double s, const char *code, const char *name, const char *shortcut, bool manual_fl);
    int Add_Sum_Discount(int i, double s, const char *code, const char *name, const char *shortcut, bool manual_fl);
    
    void Clear_Receipt();
    void Clear_Taxes();
    
    double Sum();
    double ExceptionSum();
    double RecalculationDiscounts();
    double CalcPositionDiscounts();
    double CalculationPositionTaxBase(int i);
    
    /*Added by Shonda 02.09.2009*/
    double GetResultSum();		// возвращает сумму чека со скидками, налогами, с учетом удаленных (сторнированных) позиций
    double GetPositionSum();   // сумма позиций (исключая сторнированные) без учета скидок
    double GetAllDiscountSum();	// итоговая сумма всех скидок 
  	/*------------------------*/
    
    int GetSectionCountPosition(int section);
    double GetSectionSum(int section);
    double GetSectionDiscountSum(int section);
    double GetSectionResultSum(int section);
    int GetSectionNumWithMaxSum();
    
};

#endif
