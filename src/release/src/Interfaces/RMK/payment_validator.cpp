/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2009, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2009, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "payment_validator.h"
/*****************************************************************************/
PaymentSumValidator::PaymentSumValidator(QObject * parent, const char * name) : QRegExpValidator(parent, name)
{
    maxsum = 99999999.99;
    SetRegExp();
}
/*****************************************************************************/
PaymentSumValidator::PaymentSumValidator(const double ms, QObject * parent, const char * name) : QRegExpValidator(parent, name)
{
    maxsum = ms;
    SetRegExp();
}

/*****************************************************************************/
PaymentSumValidator::~PaymentSumValidator()
{
    
}	
/*****************************************************************************/
QValidator::State PaymentSumValidator::validate (QString & input, int & pos ) const
{
    bool nf;
    double sum = input.toDouble(&nf);
    QValidator::State state = QValidator::Invalid;
    if ( input.find("+") != -1 )  {
	state = QValidator::Acceptable;
	return state;
    }
    
    switch ( QRegExpValidator::validate(input, pos) )  {
    case QValidator::Intermediate :
	state = QValidator::Intermediate;
	break;
    case QValidator::Invalid :
	break;
    case QValidator::Acceptable :{
	if ( input.find(QRegExp("^0?([\\.\\,])?0?$"), 0) != -1 )    state = QValidator::Intermediate;
	//else if ( (sum <= maxsum) && (sum >= 0.01) )        state = QValidator::Acceptable;
	//puts("----fff----");
	//printf("maxsum = %f \n", maxsum);
	//printf("sum = %f \n", sum);
	//printf("maxsum - sum = %f \n", maxsum - sum);
	if ( (maxsum - sum >= -0.000001) && (sum < 100000000) )   state = QValidator::Acceptable;
	//else if ( ((maxsum - sum) > -0.01) && (sum >= 0.01) )        state = QValidator::Acceptable;
	break;   }
    }
    return state;
}
/*****************************************************************************/
void PaymentSumValidator::SetMaxSum(double s)
{
    maxsum = s;
}

/**************************PRIVATE*SECTION************************/
/*****************************************************************************/
void PaymentSumValidator::SetRegExp()
{
    //QRegExp rx("^0?\\.[0-9]{0,2}$|^[1-9][0-9]*(\\.)?[0-9]{0,2}$|^0$");
    QRegExp rx("^0?[\\.\\,]{1}[0-9]{0,2}$|^[1-9][0-9]*([\\.\\,]{1})?[0-9]{0,2}$|^0$");
    //QValidator dx = new QDoubleValidator(0,99999999.99, 2, this);
    setRegExp(rx);
}
/*****************************************************************************/

