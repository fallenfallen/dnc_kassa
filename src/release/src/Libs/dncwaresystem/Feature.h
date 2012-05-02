/* 
 * File:   FEATURE.h
 * Author: shonda
 *
 * Created on 1 Февраль 2010 г., 18:53
 */

#ifndef _FEATURE_H
#define	_FEATURE_H

#include <string>
#include <vector>

#include "FeatureValue.h"
#include "Object.h"
#include "ObjectCatalog.h"

using namespace std;

class Feature : public Object {
public:
    Feature();
    Feature(const string & fname);

    string getName() const;
    void setName( const string & fname);

    virtual string getCode() const;
    virtual void setCode(const string &code);

    void addParentSet(int set_id);
    bool isParentSet(int set_id);
    vector<int> getParentsId() const;

    void lookParallelObject(Object * obj);
    virtual void clear();
    
    void addPossibleValue(const FeatureValue &pos_value);
    /*void editPossibleValue(int index_value, const char* value );
    void removePossibleValue(int index_value);

    int getCountPossibleValue() const;
    string getPossibleValue(int index) const;

    Value * getValuePoint(const string& value);*/

private:
    string name;
    string code;
    ObjectCatalog possible_values;
    vector<int> parents;

    void insertPossibleValue(const FeatureValue &f_value);
    
};


class FeatureTieWithSet : public Object {
public:
    FeatureTieWithSet() {
        this->class_name = "FeaturesInSet";
    }
    
    int feature_id;
    int set_id;
};

#endif	/* _FEATURE_H */

