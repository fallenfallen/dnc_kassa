#ifndef TAXESCATALOG_H
#define	TAXESCATALOG_H

#include <string>
#include "ObjectCatalog.h"
#include "TaxGroup.h"
#include "TaxRates.h"
#include "TaxRateGr.h"

using  namespace std;

class TaxesCatalog {
public:
    
    void addTaxGroup(const TaxGroupInfo& taxgroup_info);
    void addTaxRate(const TaxRateInfo& taxrate_info);
    void addTaxRateGr(const TaxRateGrInfo& taxrategr_info);
    void clearCatalog();
private:
    ObjectCatalog taxgroup_container;
    ObjectCatalog taxrate_container;
    ObjectCatalog taxrategr_container;
};

#endif	/* TAXESCATALOG_H */

