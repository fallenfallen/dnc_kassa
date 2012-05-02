/* 
 * File:   stringconvert.h
 * 
 *
 * Created on 26 Май 2010 г., 19:42
 */

#ifndef _STRINGCONVERT_H
#define _STRINGCONVERT_H
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

template <typename T>
std::string toString(T val){
    std::ostringstream oss;
    //---(+)---Zuskin---29/02/2012---
    oss.precision(2);
    oss.setf(ios::fixed, ios::floatfield);
    //-------------------------------
    oss << val;
    return oss.str();
}

template <typename T>
T fromString(const std::string& s){
    std::istringstream iss(s);
    T res = 0;
    iss >> res;
    return res;
}

#endif /* _STRINGCONVERT_H */

/*
use example

string s = "456.54";
double f = fromString<float>(s);

double ff = 321.54;
string ss = toString<double>(ff);
*/
