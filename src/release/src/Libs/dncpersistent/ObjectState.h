/* 
 * File:   ObjectState.h
 * Author: shonda
 *
 * Created on 1 Июнь 2010 г., 16:39
 */

#ifndef _OBJECTSTATE_H
#define	_OBJECTSTATE_H

#include "Object.h"

class Object;

class ObjectState {
public:
    virtual void load(Object * obj);
    virtual void save(Object * obj);
    virtual void update(Object * obj);
    virtual void drop(Object * obj);
    virtual void reload(Object * obj);
protected:
    void changeState(Object* obj, ObjectState* state);
};

class NewState : public ObjectState {
public:
    static ObjectState * instance;

    virtual void load(Object * obj);
    virtual void save(Object * obj);
    virtual void drop(Object * obj);
};

class LegacyState : public ObjectState {
public:
    static ObjectState * instance;
    
    virtual void update(Object * obj);
    virtual void drop(Object * obj);
    virtual void reload(Object * obj);
};

class DeletedState : public ObjectState {
public:
    static ObjectState * instance;
};



#endif	/* _OBJECTSTATE_H */

