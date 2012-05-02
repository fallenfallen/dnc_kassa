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
#include "WareAddition.h"
#include <algorithm>
#include "Error.h"
#include "BarcodeCatalog.h"
#include "WareSystem.h"

WareAdditionInfo::WareAdditionInfo() {
    price = 0;
    quantity = 0;
    ware_id = -1;
}


WareAddition::WareAddition() {
    this->class_name = "Additions";
}

string WareAddition::getCode() const {
    return info.code;
}
string WareAddition::getArticle() const {
    return info.article;
}
double WareAddition::getPrice() const {
    return info.price;
}
double WareAddition::getQuantity() const {
    return quantity;
}

WareAdditionInfo WareAddition::getInfo() {
    return info;
}

void WareAddition::replaceInfo(const WareAdditionInfo& wa_info, bool replace_quantity) {
    this->info = wa_info;

    quantity = wa_info.quantity + (replace_quantity ? 0 : quantity);
    info.quantity = this->quantity;
}

void WareAddition::copyInfo(Object* obj) {
    WareAddition * orig_obj = (WareAddition*) obj;
    this->replaceInfo(orig_obj->getInfo());
}

string WareAddition::getMainBarcode() {
    BarcodeCatalog* bcatalog = WareSystem::getInstance()->getBarcodeCatalog();
    return bcatalog->getAdditionMainBarcode(this->getCode());
}

vector<Barcode*> WareAddition::getBarcodes() {
    BarcodeCatalog* bcatalog = WareSystem::getInstance()->getBarcodeCatalog();
    return bcatalog->getAdditionBarcode(this->getCode());
}

string WareAddition::getSelectedValue(Feature* feature) {
    if ( feature == NULL )
        return string();
    SelectedValueFilter filter(this->getOID(), feature->getOID());
    try {
        SelectedValue * svalue = (SelectedValue*) descriptions.getObject(filter);
        return svalue->getMeaning();
    } catch (const Error & err) {
        return string();
    }
}

void WareAddition::setSelectedValueByCode(const string& value_code) {
    FeatureValue * fvalue = (FeatureValue*)possible_value.getObject(ObjectFilterByCode(value_code, "FeatureValue"));
    try {
        SelectedValueByAdditionFilter svalue_exist_filter(this->getOID(), fvalue->getOID());
        descriptions.getObject(svalue_exist_filter);
    } catch (const Error &err) {
        if (err.getErrorCode() == ERROR_OBJECT_NOT_FOUND)
            insertSelectedValue(fvalue);
        else
            throw err;
    }
    
}

void WareAddition::insertSelectedValue(FeatureValue* fvalue) {
    SelectedValue * new_svalue = new SelectedValue(fvalue->getFeatureId());
    FeatureValue * value_to_svalue = new FeatureValue();
    fvalue->getCopy(value_to_svalue);
    
    new_svalue->setValue(value_to_svalue);
    new_svalue->setAdditionId(this->getOID());

    descriptions.insertObject(new_svalue);
}

void WareAddition::clear() {
    // drop selected value
    descriptions.dropObjectByFilter(AllValueInAdditionFilter(this->getOID()));
    
    //drop addition barcode by add_code !!!
    BarcodeCatalog* bcatalog = WareSystem::getInstance()->getBarcodeCatalog();
    bcatalog->dropAdditionBarcode(this->getCode());
}

