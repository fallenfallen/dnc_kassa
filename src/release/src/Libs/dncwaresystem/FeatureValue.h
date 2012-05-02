/* 
 * File:   FeatureValue.h
 * Author: shonda
 *
 * Created on 15 Апрель 2010 г., 12:10
 */

#ifndef _VALUE_H
#define	_VALUE_H

#include <string>

#include "Object.h"

using namespace std;

class FeatureValue : public Object{
public:
    FeatureValue();
    FeatureValue(const string & meaning);
    string getMeaning() const;
    void setMeaning(const string &meaning);

    string getCode() const;
    void setCode(const string &code);

    int getFeatureId();
    void setFeatureId(int f_id);

    void getCopy(FeatureValue* copy_value);

private:
    string meaning;
    string code;
    int feature_id;
};


class SelectedValue : public Object {
public:
    SelectedValue();
    SelectedValue(int feature_id);
    ~SelectedValue();
    
    void setValue(FeatureValue* fvalue);
    int getValueId() const;

    string getMeaning() const;
    
    int getFeatureId();
    void setFeatureId(int f_id);

    int getAdditionId() const;
    void setAdditionId(int additionid);

private:
    int feature_id;
    int addition_id;
    
    // создается при создании selected value, в нем же храниться, в нем же удаляется.
    FeatureValue * value;
};

#endif	/* _VALUE_H */

