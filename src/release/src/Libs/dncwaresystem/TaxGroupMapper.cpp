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
#include "TaxGroupMapper.h"

TaxGroupMapper::TaxGroupMapper() {
    to_db_table_name = "tax_group";
    from_db_table_name = "tax_group";
}


string TaxGroupMapper::getIdFieldName() {
    return TAXGROUP_ID_FIELD;
}

Object * TaxGroupMapper::createObjectByDefault() {
    TaxGroup * taxgroup = new TaxGroup();
    return taxgroup;
}

Object* TaxGroupMapper::createObjectFromResultRecord(const ResultRecord& record) {
    TaxGroup * taxgroup = new TaxGroup();
    fromPersistent(taxgroup, record);
    return taxgroup;
}

void TaxGroupMapper::fromPersistent(Object* object, const ResultRecord& record) {

    TaxGroup * taxgroup = (TaxGroup*)object;
    TaxGroupInfo taxgroup_info;

    taxgroup_info.code = fromString<int>(record.getValue( TAXGROUP_ID_FIELD ));
    taxgroup_info.long_name = record.getValue(TAXGROUP_LONGNAME);
    taxgroup_info.short_name = record.getValue(TAXGROUP_SHORTNAME);

    taxgroup->setInfo(taxgroup_info);
}

void TaxGroupMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {

    TaxGroup * taxgroup = (TaxGroup*)object;
    TaxGroupInfo info = taxgroup->getInfo();
        
    rdb_expression.addField(TAXGROUP_ID_FIELD, info.code);
    rdb_expression.addField(TAXGROUP_LONGNAME, info.long_name);
    rdb_expression.addField(TAXGROUP_SHORTNAME, info.short_name);
    
}
