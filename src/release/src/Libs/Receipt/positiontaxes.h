
#ifndef POSITION_TAXE
#define POSITION_TAXE


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "taxrate.h"

using namespace std;

class PositionTaxes
{
  public :
    int num_rates;
    int tax_group_id;
    string tax_group_name;
    
    vector<TaxRate> taxrates;

    PositionTaxes();
    PositionTaxes(const PositionTaxes & pt);
    ~PositionTaxes();
    
    TaxRate * GetTaxRatesElemRef(int index);
    const char* GetName();
    void SetName(string tg_name);
    
    void Add_Tax_Rate(TaxRate tr);
};

#endif 
