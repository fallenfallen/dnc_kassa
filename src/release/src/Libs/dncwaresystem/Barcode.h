/* 
 * File:   Barcode.h
 * Author: shonda
 *
 * Created on 16 Сентябрь 2010 г., 17:51
 */

#ifndef BARCODE_H
#define	BARCODE_H

#include <string>
#include "Object.h"

using namespace std;

class Barcode : public Object {
public:
    Barcode();
    Barcode(const string& barcode);
    string getText() const;
    void setText(const string &barcode_text);

    void setWareInternalCode(const string &wcode);
    void setAdditionCode(const string &addition_code);
    void setCoefficient(double coeff);
    void setMainBarcode(bool is_main_barcode);

    string getWareInternalCode() const;
    string getAdditionCode() const;
    bool isMainBarcode() const;
    double getCoefficient() const;

    void copyInfo(Object * obj);
    
private:   
    string text;
    double coefficient;
    bool main_barcode;
    
    string ware_internal_code;
    string addition_code;
};

#endif	/* BARCODE_H */

