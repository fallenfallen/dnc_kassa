/* 
 * File:  ObjectFilters.h
 * Author: shonda
 *
 * Created on 16 Ноябрь 2010 г., 13:15
 */

#ifndef OBJECTFILTERS_H
#define	OBJECTFILTERS_H

#include "ObjectFilter.h"
#include "FeatureValue.h"

using namespace std;

/********************************************************/
class WareFilterByCode : public ObjectFilter {
public:

    WareFilterByCode(const string & wcode);

    bool compareObject(Object * obj) const;
    string getClassName() const;
    string getFilterExpression() const;
private:
    string ware_code;
};

/********************************************************/
class AdditionsInWareFilter : public ObjectFilter {
public:
    AdditionsInWareFilter(int ware_oid);    
    string getClassName() const;
    virtual string getFilterExpression() const;
    bool compareObject(Object* obj) const;
protected:
    int ware_id;
};


class AdditionInWareByCodeFilter : public AdditionsInWareFilter {
public:
    AdditionInWareByCodeFilter(int ware_oid, const string & addit_code);
    string getFilterExpression() const;
private:
    string addition_code;
};


/********************************************************/
class FeaturesInSetFilter : public ObjectFilter {
public:
    FeaturesInSetFilter(int set_id);

    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    string getClassName() const;
    
private:
    int set_id;
};

/********************************************************/
class SelectedValueFilter : public ObjectFilter {
public:
    SelectedValueFilter(int addition_id, int feature_id);

    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    string getClassName() const;

private:
    int addition_id;
    int feature_id;
};

/********************************************************/
class BarcodeFilter : public ObjectFilter {
public:
    virtual bool compareObject(Object* obj) const = 0;
    virtual string getFilterExpression() const = 0;
    string getClassName() const;
};


/********************************************************/
class AdditionBarcodeFilter : public BarcodeFilter {
public:
    AdditionBarcodeFilter(const string &addition_code);

    bool compareObject(Object* obj) const;
    string getFilterExpression() const;

private:
    string addtion_code;
};

/********************************************************/

class WareBarcodeFilter : public BarcodeFilter {
public:
    WareBarcodeFilter(const string &ware_code);

    bool compareObject(Object* obj) const;
    string getFilterExpression() const;

private:
    string ware_code;
};

/********************************************************/
class ExistAdditionBarcodeFilter : public BarcodeFilter {
public:
    ExistAdditionBarcodeFilter(const string &addition_code, const string &barcode_text);

    bool compareObject(Object* obj) const;
    string getFilterExpression() const;

private:
    string addtion_code;
    string barcode;
};

/********************************************************/
class ExistWareBarcodeFilter : public BarcodeFilter {
public:
    ExistWareBarcodeFilter(const string &ware_internal_code, const string &barcode_text);

    bool compareObject(Object* obj) const;
    string getFilterExpression() const;

private:
    string ware_code;
    string barcode;
};

/********************************************************/

class FeatureTieWithSetFilter : public ObjectFilter {
public:
    FeatureTieWithSetFilter(int f_id, int s_id);

    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    string getClassName() const;
    
private:
    int f_id;
    int s_id;
};

class AllFISFilter : public ObjectFilter {
    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    string getClassName() const;
};


/********************************************************/
class SelectedValueByAdditionFilter : public ObjectFilter {
public:
    SelectedValueByAdditionFilter(int addition_id, int value_id);

    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    string getClassName() const;

private:
    int saddition_id;
    int svalue_id;
};

/********************************************************/

class AllWareWithFS : public ObjectFilter {
public:
    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    string getClassName() const;    
};

/********************************************************/

class AllValueInAdditionFilter : public ObjectFilter {
public:
    AllValueInAdditionFilter(int addition_id);
    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    string getClassName() const;    
private:
    int f_addition_id;
};

/********************************************************/

class AllObjectInClass : public ObjectFilter {
    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    virtual string getClassName() const = 0;
};

class AllFeaturesFilter : public AllObjectInClass {
    string getClassName() const;
};

class AllFeaturesSetsFilter : public AllObjectInClass {
    string getClassName() const;
};

class AllWaresFilter : public AllObjectInClass {
    string getClassName() const;
};

class AllWareGroupsFilter : public AllObjectInClass {
    string getClassName() const;
};

/********************************************************/

class PossibleValueInFeature : public ObjectFilter {
public:
    PossibleValueInFeature(int feature_id);
    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    string getClassName() const;
private:
    int feature_id;
};

/********************************************************/

class GroupSubItemFilter : public ObjectFilter {
public:
    GroupSubItemFilter(int parent_group_id);
    virtual bool compareObject(Object* obj) const;
    virtual string getFilterExpression() const;
    virtual string getClassName() const;
protected:
    int parent_id;
};

class GroupSubWareFilter : public GroupSubItemFilter {
public:
    GroupSubWareFilter(int parent_group_id);
    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    string getClassName() const;
};

/********************************************************/

class FiscalSectionFilter : public ObjectFilter {
public:
    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    string getClassName() const;
};


class NotFiscalSectionFilter : public ObjectFilter {
public:
    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    string getClassName() const;
};


class SectionByNumberFilter : public ObjectFilter {
public:
    SectionByNumberFilter(int section_num);
    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    string getClassName() const;
private:
    int section_number;
};

/********************************************************/

class AllProfilesWithoutAdmin : public ObjectFilter {
public:
    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    string getClassName() const;
};

class AllUsersWithoutAdmin : public ObjectFilter {
    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    string getClassName() const;
};

/********************************************************/

//-----(+)-----Zuskin-----15/12/2011-----
class DefaultDiscSchemeFilter : public ObjectFilter {
public:
    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    string getClassName() const;
};

class DiscSchemeByNumberFilter : public ObjectFilter {
public:
    DiscSchemeByNumberFilter(int discscheme_num);
    bool compareObject(Object* obj) const;
    string getFilterExpression() const;
    string getClassName() const;
private:
    int discscheme_number;
};
//---------------------------------------

#endif	/* OBJECTFILTERS_H */

