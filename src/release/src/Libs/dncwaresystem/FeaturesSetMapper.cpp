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

#include "FeaturesSetMapper.h"
#include "FeaturesSet.h"
#include "stringconvert.h"

FeaturesSetMapper::FeaturesSetMapper() {
    //table_name = "features_sets";
    to_db_table_name = "features_sets";
    from_db_table_name = "features_sets";
}

string FeaturesSetMapper::getIdFieldName() {
    return FSET_ID_FIELD;
}

Object * FeaturesSetMapper::createObjectByDefault() {
    FeaturesSet * fset = new FeaturesSet;
    return fset;
}

Object * FeaturesSetMapper::createObjectFromResultRecord(const ResultRecord& record) {
    FeaturesSet * fset = new FeaturesSet;
    fromPersistent(fset, record);
    return fset;
}

void FeaturesSetMapper::fromPersistent(Object* object, const ResultRecord& record) {
    FeaturesSet * fset = (FeaturesSet*) object;
    fset->setName(record.getValue(FSET_NAME));
    fset->setCode(record.getValue(FSET_CODE));
}

void FeaturesSetMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {
    FeaturesSet * fset = (FeaturesSet*) object;
    rdb_expression.addField(FSET_CODE, fset->getCode());
    rdb_expression.addField(FSET_NAME, fset->getName());
}


