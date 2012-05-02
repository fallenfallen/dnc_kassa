#ifndef TAXRATEGR_H
#define TAXRATEGR_H

#include <string>
#include "Object.h"

using namespace std;

class TaxRateGrInfo {
public:
    int code; // = taxrate_id
    int rate_id;
    int group_id;
    bool switchbase;
    TaxRateGrInfo();
};


class TaxRateGr : public Object{
public:
    TaxRateGr();

    TaxRateGrInfo getInfo();
    void setInfo(const TaxRateGrInfo & taxrategr_info);
    void copyInfo(Object * obj);
    
private:
    TaxRateGrInfo info;
};

#endif	/* TAXRATEGR_H */

