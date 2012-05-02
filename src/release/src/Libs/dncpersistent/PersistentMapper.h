/* 
 * File:   PersistentMapper.h
 * Author: shonda
 *
 * Created on 26 Май 2010 г., 17:43
 */

#ifndef _PERSISTENTMAPPER_H
#define	_PERSISTENTMAPPER_H

#include "AbstractMapper.h"
#include "ObjectFilter.h"

class PersistentMapper : public AbstractMapper {
public:
    ObjectsCollection * getObjectsCollection(const ObjectFilter &filter);
    void addNewObject(Object* new_object);
    void updateObject(Object* object);
    void reloadObject(Object * object);

    Object * createObjectByDefault() = 0;

protected:
    virtual ObjectsCollection * getObjectsCollectionFromStorage(const ObjectFilter &filter) = 0;
    virtual void addNewObjectToStorage(Object * object) = 0;
    virtual void updateObjectToStorage(Object * object) = 0;
    virtual void reloadObjectFromStorage(Object * object) = 0;

    virtual void dropObjects(const ObjectFilter  & filter) = 0;
};

#endif	/* _PERSISTENTMAPPER_H */

