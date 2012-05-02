/* 
 * File:   ObjectFilter.h
 * Author: shonda
 *
 * Created on 13 Ноябрь 2010 г., 16:29
 */

#ifndef OBJECTFILTER_H
#define	OBJECTFILTER_H

#include "Object.h"


class ObjectFilter {
public:
    virtual bool compareObject(Object* obj) const;
    virtual string getFilterExpression() const = 0;
    virtual string getClassName() const = 0;
};



class ObjectFilterById : public ObjectFilter {
public:
    ObjectFilterById(int oid, const string & object_class_name);
    virtual bool compareObject(Object * obj) const;
    virtual string getFilterExpression() const;
    virtual string getClassName() const;
private:
    int compared_id;
    string class_name;
};



// для использования данного фильтра у объекта
// должен быть переопределен
// метод string getCode() const !!! 
class ObjectFilterByCode : public ObjectFilter {
public:
    ObjectFilterByCode(const string &object_internal_code, const string & object_class_name);
    virtual bool compareObject(Object * obj) const;
    virtual string getFilterExpression() const;
    virtual string getClassName() const;
private:
    string compared_code;
    string class_name;
};


class EmptyObjectFilter : public ObjectFilter {
public:
    EmptyObjectFilter(const string & object_class_name);
    virtual bool compareObject(Object * obj) const;
    virtual string getFilterExpression() const;
    virtual string getClassName() const;
private:
    string class_name;
};


class MaximumFilter : public ObjectFilter {
public:
    MaximumFilter(const string & max_field_name, const string& object_class_name);
    virtual string getFilterExpression() const;
    virtual string getClassName() const;
    virtual string getMaxFieldName() const;
private:
    string field_name;
    string class_name;
};

#endif	/* OBJECTFILTER_H */

