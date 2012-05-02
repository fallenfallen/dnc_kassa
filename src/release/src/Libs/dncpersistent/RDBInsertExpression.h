/* 
 * File:   RDBInsertExpression.h
 * Author: shonda
 *
 * Created on 15 Ноябрь 2010 г., 16:04
 */

#ifndef RDBINSERTEXPRESSION_H
#define	RDBINSERTEXPRESSION_H

#include <string>
#include "RDBSetExpression.h"

using namespace std;

class RDBInsertExpression : public RDBSetExpression {
public:

    void addReturningField(const string &field_name);

    string getFieldExpression() const;
    string getValueExpression() const;
    string getReturningExpression() const;

private:
    string fields_expression;
    string values_expression;
    string returning_expression;
    
    void concatExpression(const string & field_name, const string &value);
};

#endif	/* RDBINSERTEXPRESSION_H */

