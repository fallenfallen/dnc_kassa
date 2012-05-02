#ifndef DISCOUNTCATALOG_H
#define	DISCOUNTCATALOG_H


#include <string>
#include "ObjectCatalog.h"
#include "FixedDiscount.h"
//-----(+)-----Zuskin-----18/07/2011-----
#include "DiscScheme.h"
#include "DiscRule.h"
//---------------------------------------

using  namespace std;

class DiscountCatalog {
public:
    
    void addFixedDiscount(const FixedDiscountInfo& fdiscount_info);
    void clearCatalog();
    //-----(+)-----Zuskin-----18/07/2011-----
    void addDiscScheme(const DiscSchemeInfo& dsc_info);
    void addDiscRule(const DiscRuleInfo& dsc_info);
    void clearDiscSchemes(void);
    //---------------------------------------
    //-----(+)-----Zuskin-----15/12/2011-----
    DiscScheme* getDefaultDiscScheme(void);
    //DiscScheme* getDiscSchemeByNumber(int discscheme_number);    
    void clearDefaultDiscScheme(void);
    
    //---------------------------------------
private:
    ObjectCatalog discount_container;
    //-----(+)-----Zuskin-----18/07/2011-----
    ObjectCatalog schemes_container;
    ObjectCatalog rules_container;
    //---------------------------------------
    
};

#endif	/* DISCOUNTCATALOG_H */

