/* 
 * File:   RDBWhereFilter.h
 * Author: shonda
 *
 * Created on 15 Ноябрь 2010 г., 13:57
 */

#ifndef RDBWHEREFILTER_H
#define	RDBWHEREFILTER_H

#include "RDBFilter.h"

using namespace std;

class RDBWhereFilter : public RDBFilter {
public:
    virtual void addWhereCondition(const string &field_name, const string & logical_operation, const string & value);
    virtual void addWhereCondition(const string &field_name, const string & logical_operation, double value);
    virtual void addWhereCondition(const string &field_name, const string & logical_operation, int value);

    virtual string getWhereExpression() const;
    
private:
    void concatCondition(const string &field_name, const string & logical_operation, const string & value);
    string where_expression;
};

#endif	/* RDBWHEREFILTER_H */

