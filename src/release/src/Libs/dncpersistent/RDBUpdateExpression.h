/* 
 * File:   RDBUpdateExpression.h
 * Author: shonda
 *
 * Created on 15 Ноябрь 2010 г., 16:29
 */

#ifndef RDBUPDATEEXPRESSION_H
#define	RDBUPDATEEXPRESSION_H

#include "RDBWhereFilter.h"
#include "RDBSetExpression.h"

using namespace std;

class RDBUpdateExpression : public  RDBSetExpression{
public:

    RDBWhereFilter where_filter;
    
    string getSettingExpression() const;
    string getWhereExpression() const;

private:
    string setting_expression;
    void concatExpression(const string &field_name, const string &value);
};

#endif	/* RDBUPDATEEXPRESSION_H */

