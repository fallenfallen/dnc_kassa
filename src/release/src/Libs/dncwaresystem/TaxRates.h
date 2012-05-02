#ifndef TAXRATES_H
#define TAXRATES_H

#include <string>
#include "Object.h"

using namespace std;

class TaxRateInfo {
public:
    int code; // = taxrate_id
    string long_name;
    string short_name;
    double value;
    TaxRateInfo();
};


class TaxRates : public Object{
public:
    TaxRates();

    TaxRateInfo getInfo();
    void setInfo(const TaxRateInfo & taxrate_info);
    void copyInfo(Object * obj);
    
private:
    TaxRateInfo info;
};

#endif	/* TAXRATES_H */

