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

#include "mcrkbinput.h"

#define FIRST_TRACK_HEARD '%'
#define SECOND_TRACK_HEARD ';'
#define THIRD_TRACK_HEARD '_'
#define FIRST_TRACK_TEIL '?'
#define SECOND_TRACK_TEIL '?'
#define THIRD_TRACK_TEIL '?'

const char TRACK_TEIL = '?';

/*********************************************************************************/
KeyboardReader::KeyboardReader(QObject *obj,  int pref1, int pref2, int suf1, int suf2) : 
	KeyboardDevice(obj, pref1, pref2, suf1, suf2) 
{
    //shift = false;
    //dkbfrs = new DKB_FOR_RUS_SYMBOL();
 //   printf ("pr1 = %d\n",pr1);
 //   printf ("pr2 = %d\n",pr2);
 //   printf ("su1 = %d\n",su1);
 //   printf ("su2 = %d\n",su2);
    
    len_code = 200;
    type_kbdevice = READER_TYPE;
	
}
/*********************************************************************************/
KeyboardReader::KeyboardReader() {
    len_code = 200;
    type_kbdevice = READER_TYPE;
}
/*********************************************************************************/
KeyboardReader::~KeyboardReader()
{
    //delete dkbfrs;
}
/*********************************************************************************/
QString KeyboardReader::getCode(int& num_track) {
  //  puts("***********************************************************************************");
    QString s = "";
    num_track = -1;
    if ( status != 0 && status != 4 ) return s;   
    
    int i0 = 0;
    int in = MainString.length() - 1;
    bool cardcode = false;
    
    while ( MainString[i0] == ' ' )  i0++;   
    while ( MainString[in] == ' ' )  in--;
    
    if ( (MainString[i0] == FIRST_TRACK_HEARD) && (MainString[in] == FIRST_TRACK_TEIL) )  {
	num_track = 1;
	cardcode = true;
//	puts("pervaya stroka");
	}
    
    if ( (MainString[i0] == SECOND_TRACK_HEARD) && (MainString[in] == SECOND_TRACK_TEIL) )  {
	num_track = 2;
	cardcode = true;
//	puts("vtoray stroka");    
                }
    
    if ( (MainString[i0] == THIRD_TRACK_HEARD) && (MainString[in] == THIRD_TRACK_TEIL) )  {
	num_track = 3;
	cardcode = true;
//	puts("tretia stroka");    
               }
    
    if ( cardcode )   for (int i=i0+1; i< in; i++) {
	s += MainString[i];    }
    
//    puts("return s");
//    printf("MainString = %s \n", s.ascii());
    return s;
}
 

/*********************************************************************************/
QString KeyboardReader::getAllTrack()
{
    QString res;
    if ( status == 0 || status == 4 ) res = MainString; else res = "";
    MainString = "";
    return res;
}
/*********************************************************************************/
QString KeyboardReader::getTrack(int track_num) {
    QString result = ""; 
    
    int i0 = -1;
    if ( track_num == 1 ) {
	i0 = MainString.find(FIRST_TRACK_HEARD);
    } else if ( track_num == 2 ) {
	i0 = MainString.find(SECOND_TRACK_HEARD);
    } else if ( track_num == 3 ) {
	i0 = MainString.find(THIRD_TRACK_HEARD);
    }
    
    //printf("i0 = %d \n", i0);
    if ( i0 == -1 ) return QString("");
    int i = i0 + 1;
    
    while ( (MainString[i] != TRACK_TEIL) && (i < MainString.length()) ) { 
	//printf("MainString[%d] = %s \n", i, MainString.mid(i, 1).ascii());
	result += MainString[i];
	i++;
    }
    if ( i < MainString.length() ) return result;
    else return QString(""); 
    
}
/*********************************************************************************/
QString KeyboardReader::getCodeWithPrefSuf() {
    QString res = MainString;
    //printf("MainString = %s \n", MainString.ascii());
    
    if ( pr1 + pr2 + su1 + su2 != 0 ) return res;
    
    /*for (int i = 0; i < res.length(); i++) {
	printf("res [%d] = %d\n", i, *(res.mid(i,1) ));
    }*/
    
    QString tmp; 
    if ( (*(res.mid(1,1)) != FIRST_TRACK_HEARD) && (*(res.mid(1,1)) != SECOND_TRACK_HEARD) && (*(res.mid(1,1)) != THIRD_TRACK_HEARD)) {
	    pr2 = *(res.mid(1,1));
	    //printf("pr 2 = %d \n", pr2);
	    res.remove(1,1);
	    tmp += "[";    tmp += QString::number(pr2);    tmp += "]";
	    res.insert(1, tmp);
    }
    
    tmp = "";
    pr1 = *(res.mid(0,1));
    //printf("pr 1 = %d \n", pr1);
    res.remove(0,1);
    tmp += "[";    tmp += QString::number(pr1);    tmp += "]";
    res.insert(0, tmp);
    
    tmp = "";
    //printf("*(res.mid(res.length()-2,1)) = %d \n", *(res.mid(res.length()-2,1)));
    if ( *(res.mid(res.length()-2,1)) != TRACK_TEIL ) {
	su1 = *(res.mid(res.length()-2,1));
	//printf("su1 = %d \n", su1);
	res.remove(res.length() - 2,1);
	tmp += "[";    tmp += QString::number(su1);    tmp += "]";
	res.insert(res.length() - 1, tmp);
    }
    
    tmp = "";
    su2 = *(res.mid(res.length()-1,1));
    //printf("su2 = %d \n", su2);
    res.remove(res.length() - 1,1);
    tmp += "[";    tmp += QString::number(su2);    tmp += "]";
    res.insert(res.length(), tmp);

    return res;
}

/*********************************************************************************/
