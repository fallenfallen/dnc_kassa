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

#include "FeaturesCatalog.h"
#include "ObjectFilters.h"
#include "Error.h"
#include "WareCatalog.h"
#include "WareSystem.h"

FeaturesSet * FeaturesCatalog::getFeaturesSetById(int fset_id) {
    return (FeaturesSet*)fsets_catalog.getObject(ObjectFilterById(fset_id, "FeaturesSet"));
}

FeaturesSet * FeaturesCatalog::getFeaturesSetByCode(const string& fset_code) {
    return (FeaturesSet*)fsets_catalog.getObject(ObjectFilterByCode(fset_code, "FeaturesSet"));
}

vector<Feature*> FeaturesCatalog::getFeaturesInSet(int fset_id) {
    vector<Feature*> result_features;
    vector<Object*> features = features_catalog.getObjects(FeaturesInSetFilter(fset_id));
    for (vector<Object*>::iterator it = features.begin(); it != features.end(); it++)
        result_features.push_back( (Feature*)(*it) );
    return  result_features;
}

void FeaturesCatalog::addFeaturesSet(const FeaturesSet& new_set) {
    try {
        ObjectFilterByCode filter(new_set.getCode(), "FeaturesSet");
        FeaturesSet * exist_fset = (FeaturesSet*)fsets_catalog.getObject(filter);
        exist_fset->setName(new_set.getName());
        exist_fset->update();
    } catch (const Error & err) {
        if (err.getErrorCode() == ERROR_OBJECT_NOT_FOUND)
            insertFeaturesSet(new_set);
        else
            throw err;
    }
}

void FeaturesCatalog::insertFeaturesSet(const FeaturesSet& fset) {
    FeaturesSet * new_fset = new FeaturesSet();
    new_fset->setCode(fset.getCode());
    new_fset->setName(fset.getName());
    fsets_catalog.insertObject(new_fset);
}

void FeaturesCatalog::addFeatureInSet(const Feature& feature, const string& fset_code) {
    try {
        ObjectFilterByCode filter(feature.getCode(), "Features");
        Feature * exist_feature = (Feature*)features_catalog.getObject(filter);
        
        exist_feature->setCode(feature.getCode());
        exist_feature->setName(feature.getName());
        exist_feature->update();
    } catch (const Error & err) {
        if (err.getErrorCode() == ERROR_OBJECT_NOT_FOUND)
            insertFeature(feature);
        else
            throw err;
    }
    insertFeatureToSet(feature.getCode(), fset_code);
}

void FeaturesCatalog::insertFeature(const Feature& feature) {
    Feature * new_feature = new Feature();
    new_feature->setCode(feature.getCode());
    new_feature->setName(feature.getName());
    features_catalog.insertObject(new_feature);
}

void FeaturesCatalog::insertFeatureToSet(const string& feature_code, const string& fset_code) {
    ObjectFilterByCode fset_filter(fset_code, "FeaturesSet");
    FeaturesSet * fset = (FeaturesSet*)fsets_catalog.getObject(fset_filter);
    ObjectFilterByCode f_filter(feature_code, "Features");
    Feature * feature = (Feature*) features_catalog.getObject(f_filter);

    int feature_id = feature->getOID();
    int fset_id = fset->getOID();
    
    try {
        FeatureTieWithSetFilter fis_filter(feature_id, fset_id);
        Object * obj = fis_catalog.getObject(fis_filter);
        //cout << "fis object id = " << obj->getOID();
    } catch (const Error &err) {
        if (err.getErrorCode() == ERROR_OBJECT_NOT_FOUND) {
            insertFisObject(feature_id, fset_id);
            feature->addParentSet(fset_id);
        } else
            throw err;
    }
}

void FeaturesCatalog::insertFisObject(int feature_id, int set_id) {
    FeatureTieWithSet* fis_object = new FeatureTieWithSet();
    fis_object->feature_id = feature_id;
    fis_object->set_id = set_id;
    fis_catalog.insertObject(fis_object);
}

Feature * FeaturesCatalog::getFeatureByCode(const string& fcode) {
    return (Feature*) features_catalog.getObject(ObjectFilterByCode(fcode, "Features"));
}

void FeaturesCatalog::clearCatalog() {
    WareCatalog * ware_catalog = WareSystem::getInstance()->getWareCatalog();
    ware_catalog->deleteFSFromAllWares();
    
    fis_catalog.dropObjectByFilter(AllFISFilter());
    features_catalog.dropObjectByFilter(AllFeaturesFilter());
    fsets_catalog.dropObjectByFilter(AllFeaturesSetsFilter());
}

