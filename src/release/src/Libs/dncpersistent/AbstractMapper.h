/* 
 * File:   AbstractMapper.h
 * Author: shonda
 *
 * Created on 24 Май 2010 г., 16:12
 */

#ifndef _ABSTRACTMAPPER_H
#define	_ABSTRACTMAPPER_H

#include "ObjectFilter.h"
#include "ObjectsCollection.h"

#include <string>

using namespace std;

class AbstractMapper {
public:
    virtual ObjectsCollection * getObjectsCollection(const ObjectFilter &filter) = 0;
    virtual void addNewObject(Object * new_object) = 0;
    virtual void updateObject(Object * object) = 0;
    virtual void reloadObject(Object * object) = 0;

    virtual string getIdFieldName() { 
        return string();
    }

    virtual string getCodeFieldName() {
        return string("code");
    }

    virtual void dropObjects(const ObjectFilter &filter) = 0;

    virtual double getMaximum(const MaximumFilter& filter) {
        return 0;
    }
    
    virtual Object * createObjectByDefault() = 0;
};

#endif	/* _ABSTRACTMAPPER_H */

