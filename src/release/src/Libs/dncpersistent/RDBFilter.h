/* 
 * File:   RDBFilter.h
 * Author: shonda
 *
 * Created on 15 Ноябрь 2010 г., 13:58
 */

#ifndef RDBFILTER_H
#define	RDBFILTER_H

#include <string>

using namespace std;

const string EQUAL  = " = ";
const string GREATER  = " > ";
const string NOT_EQUAL = " <> ";

class RDBFilter {
protected :
    string prepareVarchar(const string & value);
    string prepareNULL(int value);
};

#endif	/* RDBFILTER_H */

