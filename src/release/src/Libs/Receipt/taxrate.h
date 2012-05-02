#ifndef TAXRATE
#define TAXRATE

#include "setup_param.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class TaxRate
{
  public :
    bool change_base;
//    int order;
    int rate_id;
    double taxsum;
    double rate;
    string shortcut;
    string longtitle;
  
    TaxRate(int r_id, double r, bool ch_base, const char * sh_str, const char * ln_str);
    TaxRate(int r_id, double r);
    TaxRate(const TaxRate & tr);
    ~TaxRate();

    int GetLenghtShortcut();
    int GetLenghtLongtitle();
    
    const char * GetShortcut();
    const char * GetLongtitle();
    
    void Clear_Tax_Rate();
    //void Add_New_Rate(int ord, int r_id, double r, bool ch_base, char * sh_str, char * ln_str);
};

#endif //TAXRATE
