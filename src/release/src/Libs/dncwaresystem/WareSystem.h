/* 
 * File:   WareSystem.h
 * Author: shonda
 *
 * Created on 10 Февраль 2010 г., 13:38
 */

#ifndef _WARESYSTEM_H
#define	_WARESYSTEM_H

#include "WareCatalog.h"
#include "FeaturesCatalog.h"
#include "BarcodeCatalog.h"
#include "SectionCatalog.h"
#include "DiscountCatalog.h"
#include "AuthorizationSystem.h"
//-----(+)-----Zuskin-----21/11/2011-----
#include "ManagerCatalog.h"
//---------------------------------------
//-----(+)-----Zuskin-----22/11/2011-----
#include "TaxesCatalog.h"
//---------------------------------------

using namespace std;

class WareSystem {

public:
    static WareSystem * getInstance();
    static void startInstance();
    static void finishInstance();
    
private:
    static WareSystem * instance;

protected:
    static int ref_count;
    
    WareSystem();
    ~WareSystem();

public:
    WareCatalog * getWareCatalog();
    FeaturesCatalog * getFeaturesCatalog();
    BarcodeCatalog * getBarcodeCatalog();
    SectionCatalog * getSectionCatalog();
    DiscountCatalog * getDiscountCatalog();
    AuthorizationSystem * getAuthorizationSystem();
    //-----(+)-----Zuskin-----21/11/2011-----
    ManagerCatalog * getManagerCatalog();
    //---------------------------------------
    //-----(+)-----Zuskin-----22/11/2011-----
    TaxesCatalog * getTaxesCatalog();
    //---------------------------------------

    void clearWareSystem();

private:
    WareCatalog * ware_catalog;
    FeaturesCatalog * fs_catalog;
    BarcodeCatalog * barcode_catalog;
    SectionCatalog * section_catalog;
    DiscountCatalog * discount_catalog;
    AuthorizationSystem * authorization_system;
    //-----(+)-----Zuskin-----21/11/2011-----
    ManagerCatalog * manager_catalog;
    //---------------------------------------
    //-----(+)-----Zuskin-----22/11/2011-----
    TaxesCatalog * taxes_catalog;
    //---------------------------------------

};

#endif	/* _WARESYSTEM_H */
