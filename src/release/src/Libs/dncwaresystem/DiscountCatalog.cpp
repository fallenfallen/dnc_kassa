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

#include "DiscountCatalog.h"
//-----(+)-----Zuskin-----15/12/2011-----
#include "ObjectFilters.h"
#include "Error.h"
//---------------------------------------

void DiscountCatalog::addFixedDiscount(const FixedDiscountInfo& fdiscount_info) {
    FixedDiscount* fdiscount = new FixedDiscount();
    fdiscount->setInfo(fdiscount_info);
    discount_container.addObject(fdiscount, ObjectFilterByCode(fdiscount_info.code, "FixedDiscount") );
    delete fdiscount;
}

//-----(+)-----Zuskin-----18/07/2011-----
void DiscountCatalog::addDiscScheme(const DiscSchemeInfo& dsc_info) {
    DiscScheme* dsc = new DiscScheme();
    dsc->setInfo(dsc_info);
    schemes_container.addObject(dsc, ObjectFilterById(dsc_info.code, "DiscScheme") );
    delete dsc;
}

void DiscountCatalog::addDiscRule(const DiscRuleInfo& dsc_info) {
    DiscRule* dsc = new DiscRule();
    dsc->setInfo(dsc_info);
    rules_container.addObject(dsc, ObjectFilterById(dsc_info.code, "DiscRule") );
    delete dsc;
}
//----------------------------------------
void DiscountCatalog::clearCatalog() {
    
    discount_container.dropObjectByFilter(EmptyObjectFilter("FixedDiscount"));
}

//-----(+)-----Zuskin-----19/07/2011-----
void DiscountCatalog::clearDiscSchemes() {
    schemes_container.dropObjectByFilter(EmptyObjectFilter("DiscScheme"));
//    rules_container.dropObjectByFilter(EmptyObjectFilter("DiscRule"));
}
//---------------------------------------

//-----(+)-----Zuskin-----15/12/2011-----
DiscScheme * DiscountCatalog::getDefaultDiscScheme() {
    DiscScheme * default_scheme;
    try {
	default_scheme = (DiscScheme*) schemes_container.getObject( DefaultDiscSchemeFilter() );
    } catch (const Error& err) {
	if ( err.getErrorCode() == ERROR_OBJECT_NOT_FOUND )
	    default_scheme = NULL;
    }
    return default_scheme;
}

/*DiscScheme * DiscountCatalog::getDiscSchemeByNumber(int discscheme_number) {
    return (DiscScheme*) schemes_container.getObject( DiscSchemeByNumberFilter( discscheme_number ) );
}*/

void DiscountCatalog::clearDefaultDiscScheme(void) {
    DiscScheme * current_def_discscheme = getDefaultDiscScheme();
    if (current_def_discscheme != NULL) {
	current_def_discscheme->setIsDefault(false);
	current_def_discscheme->update();
    }
}
