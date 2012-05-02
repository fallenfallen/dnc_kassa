 /* 
 * File:   PersistentFacade.h
 * Author: shonda
 *
 * Created on 21 Май 2010 г., 12:45
 */

#ifndef _PERSISTENTFACADE_H
#define	_PERSISTENTFACADE_H

#include <string>
#include <map>

#include "Object.h"
#include "AbstractMapper.h"
#include "ObjectFilter.h"
#include "ObjectsCollection.h"

using namespace std;

class PersistentFacade {
public:
    static PersistentFacade * getInstance();

    static void startInstance();
    static void finishInstance();

private:
    static PersistentFacade * instance;

protected:
    static int ref_count;
    PersistentFacade();

public:
    ObjectsCollection * getObjectsCollection(const ObjectFilter & filter);
    Object* getObject(const ObjectFilter & filter);
    
    string getIdFieldName(const string & class_name);
    string getCodeFieldName(const string & class_name);

    void insertObject(Object * new_object);
    void updateObject(Object * object);
    void reloadObject(Object * object);
    void dropObjectsByFilter(const ObjectFilter & filter);
    double getMaximum(const MaximumFilter & filter);

    Object* createObjectByDefault(const string & class_name);

private:
    map <string, AbstractMapper*> mappers;
    void initMappers();

    void findMapper(const string & class_name);
};

#endif	/* _PERSISTENTFACADE_H */

