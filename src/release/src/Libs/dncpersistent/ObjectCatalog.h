/* 
 * File:   ObjectCatalog.h
 * Author: shonda
 *
 * Created on 13 Ноябрь 2010 г., 16:55
 */

#ifndef OBJECTCATALOG_H
#define	OBJECTCATALOG_H

#include "Object.h"
#include "ObjectFilter.h"
#include "ObjectsCollection.h"

#include <vector>

using namespace std;

class ObjectCatalog {
public:
    ~ObjectCatalog();

    Object * getObject(const ObjectFilter & filter);
    vector<Object*> getAllObjects(const ObjectFilter & filter);
    vector<Object*> getObjects(const ObjectFilter & filter);
    void insertObject(Object * obj);

    Object * addObject(Object * obj, const ObjectFilter & filter);
    void dropObjectByFilter(const ObjectFilter & filter);

private:
    vector<Object *> objects;

    void deleteObjects();
    
    Object * findExistObject(const ObjectFilter & filter);
    Object * loadObject(const ObjectFilter & filter);

    void addObjectToCatalog(Object * object);
    
    void loadObjectCollectionToBuffer(const ObjectFilter & filter);
    vector<Object*>getExistObjects(const ObjectFilter & filter);

    vector<Object *> buffer_collection;

    Object * addNewObject(Object * obj);
    
};

#endif	/* OBJECTCATALOG_H */

