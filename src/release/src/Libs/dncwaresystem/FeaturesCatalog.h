/* 
 * File:   FeaturesCatalog.h
 * Author: shonda
 *
 * Created on 16 Ноябрь 2010 г., 15:09
 */

#ifndef FEATURESCATALOG_H
#define	FEATURESCATALOG_H

#include "ObjectCatalog.h"
#include "FeaturesSet.h"

using namespace std;

class FeaturesCatalog {
public:
    
    FeaturesSet * getFeaturesSetById(int fset_id);
    FeaturesSet * getFeaturesSetByCode(const string & fset_code);

    vector <Feature *> getFeaturesInSet(int fset_id);

    void addFeaturesSet(const FeaturesSet &new_set);
    void addFeatureInSet(const Feature &feature, const string &fset_code);

    Feature * getFeatureByCode(const string &fcode);

    void clearCatalog();

private:
    ObjectCatalog fsets_catalog;
    ObjectCatalog features_catalog;
    ObjectCatalog fis_catalog;

    void insertFeaturesSet(const FeaturesSet &fset);
    
    void insertFeature(const Feature &feature);
    void insertFeatureToSet(const string &feature_code, const string &fset_code);
    void insertFisObject(int feature_id, int set_id);
};

#endif	/* FEATURESCATALOG_H */

