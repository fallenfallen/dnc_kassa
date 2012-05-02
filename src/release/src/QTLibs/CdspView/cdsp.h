#ifndef CUSTOMER_DISPLAY_VIEW
#define CUSTOMER_DISPLAY_VIEW


#include <iostream>
#include <string>

#include "hwsrv.h"

using namespace std;

class CDSP
{
  public :
    CDSP(HWSRV * hw_ref);       //constr = 1
	  
    CDSP(const char * name_str, const char * price_str, const char * quant_str, const char * sum_str, const char * total_str, HWSRV * hw_ref); //constr = 2
  
    CDSP(const char * name_str, const char * sum_str, const char * red_label, HWSRV * hw_ref); //constr = 3
    
    CDSP(const char * total_str, HWSRV * hw_ref); //constr = 4

    
    void StornoCustomerDisplay();
    void RegistrationPositionCustomerDisplay();
    void RegistrationReductionCustomerDisplay();
    void CustomerDisplay(const char * up_str, const char * down_str, int up_run_mode = 0, int down_run_mode = 0);
    //void CustomerDisplay(const char * up_str, const char * down_str);

  private :
    CDSP();
  
    string RegistrationPositionUpString();
    string RegistrationPositionDownString();
    
    string RegistrationReductionUpString();
    string RegistrationReductionDownString();
  
    string name;
    string price;
    string quant;
    string sum;
    string total;
    string text;
    
    string price_label;
    string quant_label;
    string total_label;
    string reduct_label;
    
    HWSRV * hw;
    
    int constr;
};

#endif
