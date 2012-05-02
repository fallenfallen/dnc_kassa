/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include <ostream>

#include "WareSystem.h"
#include "PersistentFacade.h"

#include "MappersInit.h"
#include "Error.h"
#include "SectionCatalog.h"

WareSystem * WareSystem::instance = NULL;
int WareSystem::ref_count = 0;


WareSystem * WareSystem::getInstance() {
    if ( instance == NULL )
        throw Error(216, "Не запущена система работа со справочником товаров", "WareSystem is NULL");
    return instance;
}

void WareSystem::startInstance() {
    if ( instance == NULL )
        instance = new WareSystem();
    ref_count++;
    PersistentFacade::startInstance();
}

void WareSystem::finishInstance() {
    PersistentFacade::finishInstance();
    ref_count--;
    if ( ref_count == 0 ) {
        delete instance;
        instance = NULL;
    }
}

WareSystem::WareSystem() {
    MappersInit::initMappers();
    ware_catalog = NULL;
    fs_catalog = NULL;
    barcode_catalog = NULL;
    section_catalog = NULL;
    discount_catalog = NULL;
    authorization_system = NULL;
    //-----(+)-----Zuskin-----21/11/2011-----
    manager_catalog = NULL;
    //---------------------------------------
    //-----(+)-----Zuskin-----11/01/2012-----
    taxes_catalog = NULL;
    //---------------------------------------
}

WareSystem::~WareSystem() {
    if ( ware_catalog != NULL ) delete ware_catalog;
    if ( fs_catalog != NULL ) delete fs_catalog;
    if ( barcode_catalog != NULL ) delete barcode_catalog;
    if ( section_catalog != NULL ) delete section_catalog;
    if ( discount_catalog != NULL ) delete discount_catalog;
    if ( authorization_system != NULL ) delete authorization_system;
    //-----(+)-----Zuskin-----21/11/2011-----
    if ( manager_catalog != NULL) delete manager_catalog;
    //---------------------------------------
    //-----(+)-----Zuskin-----11/01/2012-----
    if ( taxes_catalog != NULL) delete taxes_catalog;
    //---------------------------------------
}

WareCatalog * WareSystem::getWareCatalog() {
    if ( ware_catalog ==  NULL )
        ware_catalog = new WareCatalog();
    return ware_catalog;
}

FeaturesCatalog * WareSystem::getFeaturesCatalog() {
    if ( fs_catalog ==  NULL )
        fs_catalog = new FeaturesCatalog();
    return fs_catalog;
}

BarcodeCatalog * WareSystem::getBarcodeCatalog() {
    if ( barcode_catalog == NULL )
        barcode_catalog = new BarcodeCatalog();
    return barcode_catalog;
}

SectionCatalog * WareSystem::getSectionCatalog() {
    if ( section_catalog == NULL )
        section_catalog = new SectionCatalog();
    return section_catalog;
}

DiscountCatalog * WareSystem::getDiscountCatalog() {
    if ( discount_catalog == NULL )
        discount_catalog = new DiscountCatalog();
    return discount_catalog;
}

AuthorizationSystem * WareSystem::getAuthorizationSystem() {
    if ( authorization_system == NULL ) {
        authorization_system = new AuthorizationSystem();
        authorization_system->init();
    }
    return authorization_system;
}

void WareSystem::clearWareSystem() {
    WareCatalog * w_catalog = getWareCatalog();
    w_catalog->clearCatalog();

    FeaturesCatalog * f_catalog = getFeaturesCatalog();
    f_catalog->clearCatalog();
}

//-----(+)-----Zuskin-----21/11/2011-----
ManagerCatalog * WareSystem::getManagerCatalog() {
    if ( manager_catalog == NULL )
        manager_catalog = new ManagerCatalog();
    return manager_catalog;
}
//---------------------------------------
//-----(+)-----Zuskin-----22/11/2011-----
TaxesCatalog * WareSystem::getTaxesCatalog() {
    if ( taxes_catalog == NULL )
        taxes_catalog = new TaxesCatalog();
    return taxes_catalog;
}
//---------------------------------------
