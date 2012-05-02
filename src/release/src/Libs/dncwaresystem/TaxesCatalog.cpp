/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra, Savrasov Alexander aka Zuskin
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра, Саврасов Александр
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "TaxesCatalog.h"

void TaxesCatalog::addTaxGroup(const TaxGroupInfo& taxgroup_info) {
    TaxGroup* taxgroup = new TaxGroup();
    taxgroup->setInfo(taxgroup_info);
    taxgroup_container.addObject(taxgroup, ObjectFilterById(taxgroup_info.code, "TaxGroup") );
    delete taxgroup;
}

void TaxesCatalog::addTaxRate(const TaxRateInfo& taxrate_info) {
    //puts("void TaxesCatalog::addTaxRate(const TaxRateInfo& taxrate_info) {");
    TaxRates* taxrate = new TaxRates();
    taxrate->setInfo(taxrate_info);
    taxrate_container.addObject(taxrate, ObjectFilterById(taxrate_info.code, "TaxRates") );
    delete taxrate;
    //puts("addTaxRates ends...");
}

void TaxesCatalog::addTaxRateGr(const TaxRateGrInfo& taxrategr_info) {
    TaxRateGr* taxrategr = new TaxRateGr();
    taxrategr->setInfo(taxrategr_info);
    taxrategr_container.addObject(taxrategr, ObjectFilterById(taxrategr_info.code, "TaxRateGr") );
    delete taxrategr;
}

void TaxesCatalog::clearCatalog() {
    
    taxgroup_container.dropObjectByFilter(EmptyObjectFilter("TaxGroup"));
    taxrate_container.dropObjectByFilter(EmptyObjectFilter("TaxRates"));
    taxrategr_container.dropObjectByFilter(EmptyObjectFilter("TaxRateGr"));
    
}
