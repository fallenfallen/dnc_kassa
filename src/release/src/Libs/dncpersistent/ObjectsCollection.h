/* 
 * File:   ObjectsCollection.h
 * Author: shonda
 *
 * Created on 17 Август 2010 г., 17:39
 */

#ifndef OBJECTSCOLLECTION_H
#define	OBJECTSCOLLECTION_H

#include <map>
#include <vector>

#include "Object.h"

using namespace std;

class ObjectsCollection {
public:
    ObjectsCollection();
    ~ObjectsCollection();
    
    void addObject(Object * obj);

    Object * seizeFirstObject();    
    bool isEmpty();

    vector<Object*> seizeObjects();

    void setLoadedState();

private:
    map<int, Object*> objects;
};

#endif	/* OBJECTSCOLLECTION_H */

