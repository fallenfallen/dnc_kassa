/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra, Savrasov Alexander aka Zuskin.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра, Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/
#include "TaxRateMapper.h"

TaxRateMapper::TaxRateMapper() {
    to_db_table_name = "tax_rate";
    from_db_table_name = "tax_rate";
}


string TaxRateMapper::getIdFieldName() {
    return TAXRATE_ID_FIELD;
}

Object * TaxRateMapper::createObjectByDefault() {
    TaxRates * taxrate = new TaxRates();
    return taxrate;
}

Object* TaxRateMapper::createObjectFromResultRecord(const ResultRecord& record) {
    TaxRates * taxrate = new TaxRates();
    fromPersistent(taxrate, record);
    return taxrate;
}

void TaxRateMapper::fromPersistent(Object* object, const ResultRecord& record) {

    TaxRates * taxrate = (TaxRates*)object;
    TaxRateInfo taxrate_info;

    taxrate_info.code = fromString<int>(record.getValue( TAXRATE_ID_FIELD ));
    taxrate_info.long_name = record.getValue(TAXRATE_LONGNAME);
    taxrate_info.short_name = record.getValue(TAXRATE_SHORTNAME);
    taxrate_info.value = fromString<double>(record.getValue( TAXRATE_VALUE ));

    taxrate->setInfo(taxrate_info);
}

void TaxRateMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {

    TaxRates * taxrate = (TaxRates*)object;
    TaxRateInfo info = taxrate->getInfo();
        
    rdb_expression.addField(TAXRATE_ID_FIELD, info.code);
    rdb_expression.addField(TAXRATE_LONGNAME, info.long_name);
    rdb_expression.addField(TAXRATE_SHORTNAME, info.short_name);
    rdb_expression.addField(TAXRATE_VALUE, info.value);
    
}
