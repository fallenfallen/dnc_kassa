/* 
 * File:   FixedDiscount.h
 * Author: shonda
 *
 * Created on 4 Март 2011 г., 12:33
 */

#ifndef FIXEDDISCOUNT_H
#define	FIXEDDISCOUNT_H

#include <string>
#include "Object.h"


using namespace std;

enum DiscountRange {CHECK_DISCOUNT = 1, POSITION_DISCOUNT = 0};
enum DiscountVariation { REDUCTION, CHARGE };
enum DiscountFactor { SUM_DISCOUNT, PERCENT_DISCOUNT };
enum FixedDiscountType { PERCENT_REDUCTION = 0, SUM_REDUCTION = 1, PERCENT_CHARGE = 2, SUM_CHARGE = 3 };

class FixedDiscountInfo {
public:
    string code;
    string title;
    string text;
    string card_code;
    DiscountRange range;
    FixedDiscountType type;
    double rate;
    string barcode;
    bool visible;
    //-----(+)-----Zuskin-----
    double summ;
    int scheme_id;
    double maxbonuspercent;
    //------------------------

    FixedDiscountInfo();
};


class FixedDiscount : public Object {
public:
    FixedDiscount();

    void setInfo(const FixedDiscountInfo& discount_info);
    FixedDiscountInfo getInfo();

    void copyInfo(Object * obj);
    Object* createCopyObject();

    string getCode() const;
    
private:
    FixedDiscountInfo info;
};

#endif	/* FIXEDDISCOUNT_H */

