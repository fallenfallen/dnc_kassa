#include <qvalidator.h>

#ifndef PAYMENTVALIDATOR
#define PAYMENTVALIDATOR

class PaymentSumValidator : private QRegExpValidator
{
  public :
    PaymentSumValidator(QObject * parent, const char * name = 0);
    PaymentSumValidator(const double ms, QObject * parent, const char * name = 0);
    
    ~PaymentSumValidator();
    
    QValidator::State validate ( QString & input, int & pos ) const;
    
    void SetMaxSum(double s);
    
  private :
    void SetRegExp();
	  
    double maxsum;
};

#endif
