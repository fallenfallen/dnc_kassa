#ifndef TAXGROUP_H
#define TAXGROUP_H

#include <string>
#include "Object.h"

using namespace std;

class TaxGroupInfo {
public:
    int code; // = taxgroup_id
    string long_name;
    string short_name;
    TaxGroupInfo();
};


class TaxGroup : public Object{
public:
    TaxGroup();

    TaxGroupInfo getInfo();
    void setInfo(const TaxGroupInfo & taxgroup_info);
    void copyInfo(Object * obj);
    
private:
    TaxGroupInfo info;
};

#endif	/* TAXGROUP_H */

