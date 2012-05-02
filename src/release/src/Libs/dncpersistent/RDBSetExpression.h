/* 
 * File:   RDBSetExpression.h
 * Author: shonda
 *
 * Created on 18 Ноябрь 2010 г., 16:43
 */

#ifndef RDBSETEXPRESSION_H
#define	RDBSETEXPRESSION_H

#include <string>

#include "RDBFilter.h"

using namespace std;

class RDBSetExpression : public RDBFilter {
public:

    void addField(const string & field_name, const string &value);
    void addField(const string & field_name, double value);
    void addField(const string & field_name, int value);
    
protected:
    virtual void concatExpression(const string & field_name, const string &value) = 0;

};

#endif	/* RDBSETEXPRESSION_H */

