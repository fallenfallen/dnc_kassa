#ifndef SETUP_CONST_PARAM
#define SETUP_CONST_PARAM

const int MaxLen_ChequeText = 100;
const int MaxLen_SchemeName = 20;
const int MaxLen_ProcentText = 6;
const int MaxLen_MoneyText = 11; 

const int LEN_MAX = 50; 

const QColor Color_EditSelectItem(85, 255, 255);

  
/*const uint CountDiscountOrder = 4;
const char * DiscountOrder[CountDiscountOrder] =  {"автоматическая", 
						   "произвольная",
						   "фиксированная", 
						   "по дисконтной карте"
						   }; */

const uint CountDiscountOrder = 2;
const char * DiscountOrder[CountDiscountOrder] =  {   "фиксированная", 
						   "поизвольная"}; 


#endif
