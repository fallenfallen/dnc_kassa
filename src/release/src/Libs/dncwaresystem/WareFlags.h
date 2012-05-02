/* 
 * File:   WareFlags.h
 * Author: shonda
 *
 * Created on 19 Ноябрь 2010 г., 11:54
 */

#ifndef WAREFLAGS_H
#define	WAREFLAGS_H

#include <string>

using namespace std;

class WareFlags {
public:
    WareFlags();
    WareFlags(int flags);
    WareFlags(const string & format_string);

    void setFlags(const string & format_string);
    void setFlags(int flags);

    int toInt();
    
private:
    bool weight_ware;
    bool sale_ban;
    bool return_ban;
    bool negative_count_able;
    bool must_input_quantity;
    bool decrease_quantity;
    bool edit_price_able;
    bool input_quantity_able;
    //-----(+)-----Zuskin-----2011-05-13-----
//    bool print_in_check;
    //---------------------------------------

    void setFlag(int f_num, bool value);

};

#endif	/* WAREFLAGS_H */

