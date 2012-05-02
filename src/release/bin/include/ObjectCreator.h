/* 
 * File:   ObjectCreator.h
 * Author: shonda
 *
 * Created on 25 Март 2011 г., 22:41
 */

#ifndef OBJECTCREATOR_H
#define	OBJECTCREATOR_H

#include <string>

#include "Object.h"

using namespace std;

class ObjectFactory {
public:
    virtual Object * createDomainObject(const string & object_class_name);
};

class ObjectCreator {
public:
    static Object* createObject(const string & object_class_name);
    static ObjectFactory* object_factory;
};

#endif	/* OBJECTCREATOR_H */

