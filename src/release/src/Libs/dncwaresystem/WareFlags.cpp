/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "WareFlags.h"
#include <math.h>

WareFlags::WareFlags() {
}

WareFlags::WareFlags(int flags) {
    weight_ware = false;
    sale_ban = false;
    return_ban = false;
    negative_count_able = false;
    must_input_quantity = false;
    decrease_quantity = true;
    edit_price_able = false;
    input_quantity_able = true;
    //-----(+)-----Zuskin-----2011-05-13-----
//    print_in_check = true;
    //---------------------------------------

}

WareFlags::WareFlags(const string& format_string) {
    setFlags(format_string);
}

void WareFlags::setFlags(const string& format_string) {
    int flag_num = 0;
    for (int i = 0; i < format_string.length(); i++) {
        if ( format_string[i] == '0' || format_string[i] == '1' ) {
            setFlag(flag_num, (format_string[i] == '1') );
            flag_num++;
        }
    }
}

void WareFlags::setFlag(int f_num, bool value) {
    if ( f_num == 0 )
        weight_ware = value;
    if ( f_num == 1 )
        sale_ban = !value;
    if ( f_num == 2 )
        return_ban = !value;
    if ( f_num == 3 )
        negative_count_able = value;
    if ( f_num == 4 )
        must_input_quantity = !value;
    if ( f_num == 5 )
        decrease_quantity = value;
    if ( f_num == 6 )
        edit_price_able = value;
    if ( f_num == 7 )
        input_quantity_able = value;
    //-----(+)-----Zuskin-----2011-05-13-----
//    if ( f_num == 8 )
//        print_in_check = value;        
    //---------------------------------------    
}

int WareFlags::toInt() {
//-----(*)-----Zuskin-----2011-05-13-----
/*  int result_int_value = 0;
    if ( input_quantity_able )
        result_int_value += pow(2, 0);
    if ( edit_price_able )
        result_int_value += pow(2, 1);
    if ( decrease_quantity )
        result_int_value += pow(2, 2);
    if ( !must_input_quantity )
        result_int_value += pow(2, 3);
    if ( negative_count_able )
        result_int_value += pow(2, 4);
    if ( !return_ban )
        result_int_value += pow(2, 5);
    if ( !sale_ban )
        result_int_value += pow(2, 6);
    if ( weight_ware )
        result_int_value += pow(2, 7); */
    
    int result_int_value = 0x100; // печать в чеке по умолчанию включена
            
    if ( weight_ware )
        result_int_value += pow(2, 0);        
    if ( !sale_ban )
        result_int_value += pow(2, 1);
    if ( !return_ban )
        result_int_value += pow(2, 2);
    if ( negative_count_able )
        result_int_value += pow(2, 3);
    if ( !must_input_quantity )
        result_int_value += pow(2, 4);        
    if ( decrease_quantity )
        result_int_value += pow(2, 5);        
    if ( edit_price_able )
        result_int_value += pow(2, 6);        
    if ( input_quantity_able )
        result_int_value += pow(2, 7);
//    if ( print_in_check )
//        result_int_value += pow(2, 8);        
//-----------------------------------------        
    return result_int_value;
}

void WareFlags::setFlags(int flags) {
    
    //-----(*)-----Zuskin-----2011-05-13-----
/*    input_quantity_able = flags & 1;
    edit_price_able = flags & 2;
    decrease_quantity = flags & 4;
    must_input_quantity = !(flags & 8);
    negative_count_able = flags & 16;
    return_ban = !(flags & 32);
    sale_ban = !(flags & 64);
    weight_ware = flags & 128; */
    
    weight_ware = flags & 1;
    sale_ban = !(flags & 2);
    return_ban = !(flags & 4);
    negative_count_able = flags & 8;
    must_input_quantity = !(flags & 16);
    decrease_quantity = flags & 32;
    edit_price_able = flags & 64;
    input_quantity_able = flags & 128;
//    print_in_check = flags & 256;
    
    //---------------------------------------
}