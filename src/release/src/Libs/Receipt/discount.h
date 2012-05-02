
#ifndef DISCOUNT
#define DISCOUNT

//---(+)---Zuskin---10/02/2012---
#define DISCVECTORSIZE 4
//-------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Discount
{
  public :
    bool df; //true - начисленна скидка
    double dsum;
    double percent; //будем считать, что это числа от 0 до 100
    
    
    // скидка начисленная на чек пользователем из интерфейса !!!
    // меняется в случае начисления скидки, отмены / замены скидки !!!
    // не меняется при пересчете суммы скидки на чек !!!
    // храниться сумма скидки (процент расчитывается от суммы позиций с учетом скидок на позиции) - Sum() !!!!
    // эта скидка записывается в транзакции 35-38 !!!
    // TestMinPrice делается только от этой суммы, а не от dsum !!! 
    // Иначе сумма скидки будет уменьшаться при каждом запуске алгоритма TestMinPrice
    
    double transaction_percent;
    double transaction_sum;
    
    

    string name;
    string shortcut;
    string code;
    int type;
    
    string internal_code;
    string card_code;

    explicit Discount();
    //Discount(const Discount & d);
    ~Discount();

    void Set_Names(const char * name_str, const char * shortcut_str);
    void Set_Code(const char * code_str);
    void Set_Type(int dis_type) { type = dis_type; };
    void Set_Internal_Code(const char* incode) { internal_code = incode; }
    void Set_Card_Code(const char* cardcode) { card_code = cardcode; }
    

    const char * GetName();
    const char * GetShortcut();
    const char * GetCode();
    const char * Get_Internal_Code() { return internal_code.c_str(); } 
    const char * Get_Card_Code() {  return card_code.c_str(); }
    
    int GetType()  { return type; };

    int GetLengthName();
    int GetLengthShortcut();
    int GetLengthCode();
    
    void Add_Discount(double s, double p);
    void Restore_Discount();
    
    double CalcDiscount(double s);

    void Clear_Discount();
  private :
    double old_p;
    double old_s;
    double old_df;
};

double MaxDiscountAndCharge(double s , vector<Discount>&discount);

double Max_Charge(double s , vector<Discount> discount);

double Max_Discount(double s, vector<Discount>  discount);

double Union_Discount(double s, vector<Discount> & discount);

double Sum_Discount(double s, vector<Discount>& discount);


#endif


