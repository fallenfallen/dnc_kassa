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
#include "TaxRateGrMapper.h"

TaxRateGrMapper::TaxRateGrMapper() {
    to_db_table_name = "tax_rate_group";
    from_db_table_name = "tax_rate_group";
}


string TaxRateGrMapper::getIdFieldName() {
    return TAXRATEGR_ID_FIELD;
}

Object * TaxRateGrMapper::createObjectByDefault() {
    TaxRateGr * taxrategr = new TaxRateGr();
    return taxrategr;
}

Object* TaxRateGrMapper::createObjectFromResultRecord(const ResultRecord& record) {
    TaxRateGr * taxrategr = new TaxRateGr();
    fromPersistent(taxrategr, record);
    return taxrategr;
}

void TaxRateGrMapper::fromPersistent(Object* object, const ResultRecord& record) {

    TaxRateGr * taxrategr = (TaxRateGr*)object;
    TaxRateGrInfo taxrategr_info;

    taxrategr_info.code = fromString<int>(record.getValue( TAXRATEGR_ID_FIELD ));
    taxrategr_info.rate_id = fromString<int>(record.getValue( TAXRATEGR_RATE_ID ));
    taxrategr_info.group_id = fromString<int>(record.getValue( TAXRATEGR_GROUP_ID ));
    taxrategr_info.switchbase = record.getValue( TAXRATEGR_BASE ) == "t" ? true : false;

    taxrategr->setInfo(taxrategr_info);
}

void TaxRateGrMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {

    TaxRateGr * taxrategr = (TaxRateGr*)object;
    TaxRateGrInfo info = taxrategr->getInfo();
        
    rdb_expression.addField(TAXRATEGR_ID_FIELD, info.code);
    rdb_expression.addField(TAXRATEGR_RATE_ID, info.rate_id);
    rdb_expression.addField(TAXRATEGR_GROUP_ID, info.group_id);
    rdb_expression.addField(TAXRATEGR_BASE, info.switchbase ? "t" : "f");
    
}
