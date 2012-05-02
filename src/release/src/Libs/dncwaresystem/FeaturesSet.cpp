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

#include "FeaturesSet.h"
#include "FeaturesCatalog.h"
#include "WareSystem.h"

FeaturesSet::FeaturesSet() {
    this->class_name = "FeaturesSet";
}

FeaturesSet::FeaturesSet(const string& fsname) {
    this->class_name = "FeaturesSet";
    name = fsname;
}

string FeaturesSet::getName() const {
    return name;
}
void FeaturesSet::setName(const string& fname) {
    name = fname;
}

string FeaturesSet::getCode() const {
    return code;
}
void FeaturesSet::setCode(const string& code) {
    this->code = code;
}

vector<Feature *> FeaturesSet::getFeatures() {
    FeaturesCatalog* fcatalog = WareSystem::getInstance()->getFeaturesCatalog();
    return fcatalog->getFeaturesInSet(this->getOID());
}