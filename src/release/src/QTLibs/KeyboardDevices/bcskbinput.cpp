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

#include "bcskbinput.h"

KeyboardScaner::KeyboardScaner(QObject *obj,  int pref1, int pref2, int suf1, int suf2) : 
	KeyboardDevice(obj, pref1, pref2, suf1, suf2)  
{
 //   printf ("pr1 = %d\n",pr1);
 //   printf ("pr2 = %d\n",pr2);
 //   printf ("su1 = %d\n",su1);
 //   printf ("su2 = %d\n",su2);

//-----(*)-----Zuskin-----09/03/2011-----    
//    len_code = 13;
    len_code = 20;
//---------------------------------------    

    type_kbdevice = SCANER_TYPE;
}
/*********************************************************************************/
KeyboardScaner::KeyboardScaner() : KeyboardDevice()  
{    
//-----(*)-----Zuskin-----09/03/2011-----    
//    len_code = 13;
    len_code = 20;
//---------------------------------------    
    
    type_kbdevice = SCANER_TYPE;
}
/*********************************************************************************/
KeyboardScaner::~KeyboardScaner()
{
 //   puts("KeyboardScaner::~KeyboardScaner()");
}

/*********************************************************************************/
QString KeyboardScaner::getCodeWithPrefSuf()
{    
    QString res = MainString;
    //printf("MainString = %s \n", MainString.ascii());
    
//-----(*)-----Zuskin-----09/03/2011-----    
//    if ( MainString.length() > 17 ) return DefCodec->toUnicode("Слишком длинная строка. Возможно задано более двух префиксов или суффиксов");
    if ( MainString.length() > len_code+4 ) return DefCodec->toUnicode("Слишком длинная строка. Возможно задано более двух префиксов или суффиксов");
//---------------------------------------    
    if ( MainString.length() < 3 ) return DefCodec->toUnicode("Неверный штрих-код");
    
    //printf("*(res.mid(0, 1)) = %d , *(res.mid(1, 1)) = %d, *(res.mid(2, 1)) = %d\n", *(res.mid(0, 1)), *(res.mid(1, 1)), *(res.mid(2, 1)) );
    //printf("*(res.mid(-1, 1)) = %d , *(res.mid(-2, 1)) = %d, *(res.mid(-3, 1)) = %d\n", *(res.mid(MainString.length()-1, 1)), *(res.mid(MainString.length()-2, 1)), *(res.mid(MainString.length()-3, 1)) );
    if ( *(res.mid(0, 1)) < 32 && *(res.mid(1, 1)) < 32 && *(res.mid(2, 1)) < 32 ) return DefCodec->toUnicode("Задано более двух префиксов");
    if ( *(res.mid(MainString.length()-1, 1)) < 32 && *(res.mid(MainString.length()-2, 1)) < 32 && *(res.mid(MainString.length()-3, 1)) < 32 ) return DefCodec->toUnicode("Задано более двух суффиксов");
    
    if ( (pr1 + pr2 + su1 + su2) == 0 ) {
	int tmp = *(res.mid(0, 1));
	if ( tmp < 32 ) pr1 = tmp;
	tmp = *(res.mid(1, 1));
	if ( tmp < 32 ) pr2 = tmp;
	tmp = *(res.mid(MainString.length()-1, 1));
	if ( tmp < 32 ) su1 = tmp;
	tmp = *(res.mid(MainString.length()-2, 1));
	if ( tmp < 32 ) {	su2 = su1; su1 = tmp;   }
	
	printf("pr1 = %d pr2 = %d su1 = %d su2 = %d \n", pr1, pr2, su1, su2);
	
	QString s_tmp;
	if ( pr1 != 0 ) {
	    s_tmp = "[";
	    s_tmp += QString::number(pr1);
	    s_tmp += "]";
	    res.replace(0, 1, s_tmp);	    
	}
	
	if ( pr2 != 0 ) {
	    int pos = 0;
	    if ( pr1 != 0 ) pos = s_tmp.length();
	    s_tmp = "[";
	    s_tmp += QString::number(pr2);
	    s_tmp += "]";	    
	    res.replace(pos, 1, s_tmp);
	}
	
	if ( su1 != 0 ) {
	    s_tmp = "[";
	    s_tmp += QString::number(su1);
	    s_tmp += "]";	    
	    if ( su2 != 0 )	res.replace(res.length()-2, 1, s_tmp);
	    else res.replace(res.length()-1, 1, s_tmp);
	}
	if ( su2 != 0 ) {
	    s_tmp = "[";
	    s_tmp += QString::number(su2);
	    s_tmp += "]";	    
	    res.replace(res.length()-1, 1, s_tmp);   
	}
    }	
	
    return res;
}
/*********************************************************************************/
QString KeyboardScaner::getCode()  {
    QString res;
    //  printf("getCode:: status = %d \n", status);
    if ( status == 0 || status == 4 ) res = MainString; else res = "";
    MainString = "";
    return res;
}

/*********************************************************************************/

