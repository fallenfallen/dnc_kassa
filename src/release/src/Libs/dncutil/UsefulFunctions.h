/* 
 * File:   UsefulFunction.h
 * Author: shonda
 *
 * Created on 6 Декабрь 2010 г., 13:39
 */

#ifndef USEFULFUNCTION_H
#define	USEFULFUNCTION_H

#include <string>
#include <vector>

using namespace std;

vector<string> parseStringWithSeparator(const string & orig_string, const string &separator);
//-----(+)-----Zuskin-----19/07/2011-----
int convertStringOfZeros(string str);
string convertDate(string str);
//---------------------------------------

#endif	/* USEFULFUNCTION_H */

