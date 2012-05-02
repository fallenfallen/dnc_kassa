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

#include "Ware.h"
#include "Object.h"
#include "Error.h"

#include "WareSystem.h"
#include "FeaturesCatalog.h"


WareInfo::WareInfo() {
    min_price = 0;
    price = 0;
    quantity = 0;
    section_num = 0;

    features_set_id = -1;
    group_id = -1;
}

/******************************************/

Ware::Ware() {
    init();
}

/*Ware::Ware(const string& internal_code)  {
    this->info.internal_code = internal_code;
    init();
}*/

void Ware::init() {
    this->class_name = "Ware";
    features_set = NULL;
    group = NULL;
}

string Ware::getCode() const {
    return info.internal_code;
}
void Ware::setCode(const string& code) {
    info.internal_code = code;
}

WareInfo Ware::getInfo() {
    info.features_set_id = getFeaturesSetId();
    info.group_id = getGroupId();
    return info;
}

void Ware::replaceInfo(const WareInfo& ware_info, bool replace_quantity) {
    info = ware_info;
    
    base_quantity = ware_info.quantity + (replace_quantity ? 0 : base_quantity);
    info.quantity = base_quantity;

    if ( ware_info.features_set_id > 0 )
        assignFeaturesSet(ware_info.features_set_id);
    else
        assignFeaturesSet(ware_info.features_set_code);
    
    assignGroup(ware_info.group_id);
    //assignTaxGr(ware_info.taxgr_id);
}

void Ware::copyInfo(Object* obj) {
    Ware * orig_obj = (Ware*)obj;
    this->replaceInfo(orig_obj->getInfo());
}

double Ware::getUnallotedQuantity() {
    vector<WareAddition*> additions_vector = getAdditions();

    double addition_quantity = 0;
    for ( vector<WareAddition*>::iterator it = additions_vector.begin(); it != additions_vector.end(); it++ ) {
        addition_quantity += (*it)->getQuantity();
    }
    return base_quantity - addition_quantity;
}

double Ware::getAllRestQuantity() {
    return base_quantity;
}

void Ware::addQuantity(double ware_quantity) {
    base_quantity += ware_quantity;
}

string Ware::getMainBarcode() {
    BarcodeCatalog* bcatalog = WareSystem::getInstance()->getBarcodeCatalog();
    return bcatalog->getWareMainBarcode(this->getCode());
}

bool Ware::isUseFeaturesSet() {
    return (features_set != NULL);
}

void Ware::setFeaturesSet(FeaturesSet* fset) {
    features_set = fset;
}

FeaturesSet * Ware::getFeaturesSet() {
    return features_set;
}

int Ware::getFeaturesSetId() {
    if ( features_set != NULL )
        return features_set->getOID();
    else
        return  -1;
}

void Ware::assignFeaturesSet(const string& fset_code) {
    if ( !fset_code.empty() )  {
        FeaturesCatalog * fs_catalog = WareSystem::getInstance()->getFeaturesCatalog();
        this->features_set = fs_catalog->getFeaturesSetByCode(fset_code);
    }
}

void Ware::assignFeaturesSet(int fset_id) {
    FeaturesCatalog * fs_catalog = WareSystem::getInstance()->getFeaturesCatalog();
    this->features_set = fs_catalog->getFeaturesSetById(fset_id);
}

vector<Feature*> Ware::getFeatures() {
    if (features_set == NULL)
        throw Error(212, "У данного товара отключена настройка дополнительных характеристик");
    return features_set->getFeatures();
}

void Ware::setWareGroup(WareGroup* ware_group) {
    group = ware_group;
}

WareGroup * Ware::getWareGroup() {
    return group;
}

int Ware::getGroupId() {
    if ( group == NULL )
        return -1;
    else
        return group->getOID();
}

void Ware::assignGroup(int group_id) {
    if ( group_id > 0 ) {
        WareCatalog * ware_catalog = WareSystem::getInstance()->getWareCatalog();
        group = ware_catalog->getWareGroupById(group_id);
    }
}

int Ware::getTaxGroupId() {
    return -1;
}

vector<WareAddition*> Ware::getAdditions()  {
    vector<Object*> objects = additions.getAllObjects(AdditionsInWareFilter(this->getOID()));
    vector<WareAddition*> result_additions;
    for (vector<Object*>::iterator it = objects.begin(); it != objects.end(); it++) {
        result_additions.push_back( (WareAddition* ) (*it) );
    }
    return result_additions;
}

WareAddition * Ware::getAdditionByCode(const string& code) {
    return (WareAddition*)additions.getObject(AdditionInWareByCodeFilter(this->getOID(), code) );
}

void Ware::addAddition(WareAddition* addition, bool replace_quantity) {
    try {
        replaceExistAddition(addition, replace_quantity);
    } catch (const Error & err) {
         if (err.getErrorCode() == ERROR_OBJECT_NOT_FOUND) 
             addNewAddition(addition);
         else
            throw err;
    }
}

void Ware::replaceExistAddition(WareAddition* addition, bool replace_quantity) {
    ObjectFilterByCode filter(addition->getCode(), "Additions");
    WareAddition * exist_addition = (WareAddition*) additions.getObject(filter);
    
    WareAdditionInfo new_addition_info = addition->getInfo();
    new_addition_info.ware_id = this->getOID();

    double addition_old_quantity = exist_addition->getQuantity();
    double addition_new_quantity = new_addition_info.quantity;
    
    exist_addition->replaceInfo(new_addition_info, replace_quantity);
    exist_addition->update();

    this->addQuantity( replace_quantity ? addition_new_quantity - addition_old_quantity : addition_new_quantity );
    this->update();
}

void Ware::addNewAddition(WareAddition* addition) {
    ObjectFilterByCode filter(addition->getCode(), "Additions");
    WareAdditionInfo new_addition_info = addition->getInfo();
    new_addition_info.ware_id = this->getOID();
    WareAddition new_addition;
    new_addition.replaceInfo(new_addition_info);
    additions.addObject(&new_addition, filter);
    this->addQuantity(addition->getQuantity());
    this->update();
}

void Ware::clear() {
    clearAdditions();
    
    BarcodeCatalog * b_catalog = WareSystem::getInstance()->getBarcodeCatalog();
    b_catalog->dropWareBarcode(this->getCode());
}

void Ware::clearAdditions() {
    additions.dropObjectByFilter(AdditionsInWareFilter(this->getOID()));
}

