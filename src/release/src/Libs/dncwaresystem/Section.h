/* 
 * File:   Section.h
 * Author: shonda
 *
 * Created on 1 Февраль 2011 г., 15:56
 */

#ifndef SECTION_H
#define	SECTION_H

#include <string>
#include "Object.h"
#include "addon_func.h"

using namespace std;

class SectionInfo {
public:
    int number;
    string name;
    string head_text;
    string bottom_text;
    bool is_fiscal;
    double cash;
    bool auto_encashment;
    
    SectionInfo();
};


class Section  : public Object {
public:
    Section();

    SectionInfo getInfo();
    void replaceInfo(const SectionInfo & section_info);

    void copyInfo(Object * obj);

    int getNumber() const;
    void setNumber(int s_number);

    string getName() const;
    void setName(const string & s_name);

    void setHeadText(const string & head_text);
    void setBottomText(const string & bottom_text);

    void setFiscal(bool fiscal);
    bool isFiscal() const;

    void setAutoEncashment(bool auto_encashment);
    bool getAutoEncashment();

    double getCashSum() const;

    void addCashSum(double sum);
    void deductCashSum(double sum);
    void zeroCashSum();

private:
    SectionInfo info;
};

#endif	/* SECTION_H */

