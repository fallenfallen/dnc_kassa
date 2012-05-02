/* 
 * File:   Object.h
 * Author: shonda
 *
 * Created on 21 Май 2010 г., 12:52
 */

#ifndef _OBJECT_H
#define	_OBJECT_H

#include <string>
#include "ObjectState.h"

using namespace std;

class ObjectState;

class Object { 
public:

    Object();

    virtual string getClassName();
    void setOID(int id);
    int getOID() const ;

    virtual string getCode() const;

    void setloadedState();
    void save();
    void update();
    void drop();
    void reload();

    virtual void clear();
    virtual void lookParallelObject(Object * obj);

    // необходимо перегрузать в наследниках чтобы работало добавление от CatalogObject
    virtual void copyInfo(Object * obj); // = 0;
    //virtual Object* createCopyObject();

protected:
    string class_name;

private:
    friend class ObjectState;
    void setState(ObjectState * st);

private:
    int oid;
    ObjectState* state;

};

#endif	/* _OBJECT_H */

