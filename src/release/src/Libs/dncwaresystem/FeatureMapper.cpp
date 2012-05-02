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
#include <iostream>

#include "FeatureMapper.h"
#include "Feature.h"

FeatureMapper::FeatureMapper() {
    //table_name = "features_in_set_view";
    from_db_table_name = "features_in_set_view";
    to_db_table_name = "features";
}

string FeatureMapper::getIdFieldName() {
    return FEATURE_ID_FIELD;
}

Object * FeatureMapper::createObjectByDefault() {
    Feature * feature = new Feature;
    return feature;
}

Object * FeatureMapper::createObjectFromResultRecord(const ResultRecord& record) {
    Feature * feature = new Feature;
    fromPersistent(feature, record);
    return feature;
}

void FeatureMapper::fromPersistent(Object* object, const ResultRecord& record) {
    Feature * feature = (Feature*) object;
    feature->setName(record.getValue(FEATURE_NAME));
    feature->setCode(record.getValue(FEATURE_CODE));
    int fset_id = fromString<int>(record.getValue(FEATURE_SET_ID));
    feature->addParentSet(fset_id);
}

void FeatureMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {
    Feature * feature = (Feature*) object;
    rdb_expression.addField(FEATURE_NAME, feature->getName());
    rdb_expression.addField(FEATURE_CODE, feature->getCode());
}


/**********************************************************/
FeatureTieWithSetMapper::FeatureTieWithSetMapper() {
    to_db_table_name = "features_in_sets";
    from_db_table_name = "features_in_sets";
}

string FeatureTieWithSetMapper::getIdFieldName() {
    return FIS_ID_FIELD;
}

Object * FeatureTieWithSetMapper::createObjectByDefault() {
    FeatureTieWithSet * fis = new FeatureTieWithSet();
    return fis;
}

Object * FeatureTieWithSetMapper::createObjectFromResultRecord(const ResultRecord& record) {
    FeatureTieWithSet * fis = new FeatureTieWithSet();
    fromPersistent(fis, record);
    return fis;
}

void FeatureTieWithSetMapper::fromPersistent(Object* object, const ResultRecord& record) {
    FeatureTieWithSet * fis = (FeatureTieWithSet*)object;
    fis->feature_id = fromString<int>(record.getValue(FIS_FEATURE_ID));
    fis->set_id = fromString<int>(record.getValue(FEATURE_SET_ID));
}

void FeatureTieWithSetMapper::toPersistent(Object* object, RDBSetExpression& rdb_expression) {
    FeatureTieWithSet * fis = (FeatureTieWithSet*)object;
    rdb_expression.addField(FIS_FEATURE_ID, fis->feature_id);
    rdb_expression.addField(FIS_SET_ID, fis->set_id);
}

