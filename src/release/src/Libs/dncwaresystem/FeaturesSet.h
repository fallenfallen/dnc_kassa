/* 
 * File:   FeaturesSet.h
 * Author: shonda
 *
 * Created on 23 Август 2010 г., 17:59
 */

#ifndef FEATURESSET_H
#define	FEATURESSET_H

#include <string>
#include <vector>
#include <iostream>

#include "Feature.h"
#include "Object.h"


using namespace std;

class FeaturesSet : public Object {
public:
    FeaturesSet();
    FeaturesSet(const string & fsname);
    string getName() const;
    void setName(const string &fname);

    virtual string getCode() const;
    virtual void setCode(const string &code);

    vector<Feature*> getFeatures();

private:
    string name;
    string code;
};

#endif	/* FEATURESSET_H */

