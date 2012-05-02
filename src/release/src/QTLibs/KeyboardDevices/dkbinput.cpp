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

#include "dkbinput.h"
#include <qapplication.h>
#include <qwidget.h>
#include <qevent.h>

KeyboardDevice::KeyboardDevice(QObject *obj,  int pref1, int pref2, int suf1, int suf2): reciever(obj),
pr1(pref1),pr2(pref2), su1(suf1), su2(suf2)
{
  timer = new QTimer(this);
  //-----(+)-----Zuskin-----23/11/2011-----
  TIME_INTERSYMBOL = 200;
  //---------------------------------------
  //timer->start(10000, true);
  //connect( timer, SIGNAL(timeout()),this , SLOT(EndTimer()));
  
  initMapRusSymbol();
  
  g_d = XOpenDisplay(NULL);
  status = 0;
  DefCodec = QTextCodec::codecForName(SYSCODEC); 
  
  len_code = 0;
  type_kbdevice = NONE_DEVICE;
  
  if ( pr1 == 0 && pr2 != 0 ) { pr1 = pr2; pr2 = 0;  }
  if ( su1 == 0 && su2 != 0 ) { su1 = su2; su2 = 0; }
  
}
/*********************************************************************************/
KeyboardDevice::KeyboardDevice()
{
    timer = new QTimer(this);
    //-----(+)-----Zuskin-----23/11/2011-----
    TIME_INTERSYMBOL = 200;
    //---------------------------------------
    initMapRusSymbol();
    
    g_d = XOpenDisplay(NULL);
    status = 0;
    DefCodec = QTextCodec::codecForName(SYSCODEC); 
    
    len_code = 0;
    type_kbdevice = NONE_DEVICE;
    
    pr1 = 0;
    pr2 = 0;
    su1 = 0;
    su2 = 0;
  
}
/*********************************************************************************/
KeyboardDevice::~KeyboardDevice()
{
    gulp_codes.clear();
    map_rus.clear();
    delete timer;
}
/********************************************************************************/
void KeyboardDevice::setDeviceType(int type)
{
    type_kbdevice = type;
}
/********************************************************************************/
void KeyboardDevice::initMapRusSymbol() {
    map_rus["в"] = Qt::Key_D; 	// [ D ]
    map_rus["В"] = Qt::Key_D; 	// [ D ]
    map_rus[","] = Qt::Key_Question;  	// [ ? ] 
    map_rus ["ю"] = 46; // [ . ]
    map_rus ["Ю"] = 62; // [ > ]
    map_rus ["б"] = 44; // [ , ]
    map_rus ["Б"] = 60; // [ < ] 
    map_rus ["ь"] = Qt::Key_M; // [ M ]
    map_rus ["Ь"] = Qt::Key_M; // [ M ]
    map_rus ["т"] =Qt::Key_N; // [ N ] 
    map_rus ["Т"] = Qt::Key_N; // [ N ]
    map_rus ["и"] = Qt::Key_B; // [ B ]
    map_rus ["И"] = Qt::Key_B; // [ B ]
    map_rus ["м"] = Qt::Key_V; // [ V ]
    map_rus ["М"] = Qt::Key_V; // [ V ]
    map_rus ["с"] = Qt::Key_C; // [ C ]
    map_rus ["С"] = Qt::Key_C; // [ C ]
    map_rus ["ч"] = Qt::Key_X; // [ X ]
    map_rus ["Ч"] = Qt::Key_X; // [ X ]
    map_rus ["я"] = Qt::Key_Z; // [ Z ]
    map_rus ["Я"] = Qt::Key_Z; // [ Z ]
    map_rus ["э"] = 39; // [ ' ]
    map_rus ["Э"] = 34; // [ " ]
    map_rus ["ж"] = 59; // [ ; ]
    map_rus ["Ж"] = 58; // [ : ]    bool shift;
    bool rus_code;
    map_rus ["д"] = Qt::Key_L; // [ L ]
    map_rus ["Д"] = Qt::Key_L; // [ L ]
    map_rus ["л"] = Qt::Key_K; // [ K ]
    map_rus ["Л"] = Qt::Key_K; // [ K ]
    map_rus ["о"] = Qt::Key_J; // [ J ]
    map_rus ["О"] = Qt::Key_J; // [ J ]
    map_rus ["р"] = Qt::Key_H; // [ H ] 
    map_rus ["Р"] = Qt::Key_H; // [ H ]
    map_rus ["п"] = Qt::Key_G; // [ G ]
    map_rus ["П"] = Qt::Key_G; // [ G ]
    map_rus ["а"] = Qt::Key_F; // [ F ]
    map_rus ["А"] = Qt::Key_F; // [ F ]
    map_rus ["в"] = Qt::Key_D; // [ D ]
    map_rus ["В"] = Qt::Key_D; // [ D ]
    map_rus ["ы"] = Qt::Key_S; // [ S ]
    map_rus ["Ы"] = Qt::Key_S; // [ S ]
    map_rus ["ф"] = Qt::Key_A; // [ A ]
    map_rus ["Ф"] = Qt::Key_A; // [ A ]
    map_rus ["ъ"] = 93; // [ ] ]
    map_rus ["Ъ"] = 125; // [ } ]
    map_rus ["х"] = 91; // [ [ ]
    map_rus ["Х"] = 123; // [ { ]
    map_rus ["з"] = Qt::Key_P; // [ L ]
    map_rus ["З"] = Qt::Key_P; // [ L ]
    map_rus ["щ"] = Qt::Key_O; // [ K ]
    map_rus ["Щ"] = Qt::Key_O; // [ K ]
    map_rus ["ш"] = Qt::Key_I; // [ J ]
    map_rus ["Ш"] = Qt::Key_I; // [ J ]
    map_rus ["г"] = Qt::Key_U; // [ H ]
    map_rus ["Г"] = Qt::Key_U; // [ H ]
    map_rus ["н"] = Qt::Key_Y; // [ G ]
    map_rus ["Н"] = Qt::Key_Y; // [ G ]
    map_rus ["е"] = Qt::Key_T; // [ F ]
    map_rus ["Е"] = Qt::Key_T; // [ F ]
    map_rus ["к"] = Qt::Key_R; // [ D ]  
    map_rus ["К"] = Qt::Key_R; // [ D ]
    map_rus ["у"] = Qt::Key_E; // [ S ]
    map_rus ["У"] = Qt::Key_E; // [ S ]
    map_rus ["ц"] = Qt::Key_W; // [ A ]
    map_rus ["Ц"] = Qt::Key_W; // [ A ]
    map_rus ["й"] = Qt::Key_Q; // [ A ]
    map_rus ["Й"] = Qt::Key_Q; // [ A ]
    map_rus ["ё"] = 96; // [ ` ]
    map_rus ["Ё"] = 126; // [ ~ ]
    map_rus ["№"] = 35; // [ # ]
}
/*********************************************************************************/
int KeyboardDevice::getSumPrefSuf()  {
    return pr1+pr2+su1+su2;
}
/*********************************************************************************/
void KeyboardDevice::Clear() {
  status = 0;
  gulp_codes.clear();
  MainString = "";
}
/*********************************************************************************/
QKeyEvent * KeyboardDevice::GetEvent(int index) {
    //printf("gulp_codes.at(index)->key() = %d \n", gulp_codes.at(index).key());
    return  &gulp_codes.at(index);
}
/*********************************************************************************/
int KeyboardDevice::GetCountEvent() {
    return gulp_codes.size();
}
/********************************************************************************/
//returns QKeyEvent::key
int KeyboardDevice::correctCode( QKeyEvent * k ) {
  //  printf("----------------  correctCode( k->code = %d ) --------------------------- \n", k->key());
   
    if (k->key() == 4100)    return 13; 
    if (k->key() == 4101)    return 13; 
    if (k->key() == 4099)    return 8;   
    if (k->key() == 4103)    return 127; 
    if (k->key() == 4097)    return 9;   
    if (k->key() == 4096)    return 27;   
    
  //  puts("--------------------------11111111111111---------------------");
    //----------------------!!!!!!!!!!!!!!!!!!------------------------
    int group = 0; 
    
    g_d = XOpenDisplay(NULL);			     
    if (g_d != NULL) 
       { 
          if  ( XkbGetState( g_d, XkbUseCoreKbd, &state ) == Success )       group = state.locked_group;
	  XCloseDisplay(g_d);
       }
    //----------------------!!!!!!!!!!!!!!!!!!------------------------
//    printf("grpup = %d \n", group);
    if (group == 0) return k->key();
    int res = -1;
    
 //   printf("k->key = %d  \n", k->key()); 
    
    if (k->key() == 47)        return 124; 
    if (k->key() == 46)        return 47; 
    if (k->key() == 63)        return 38; 
    if (k->key() == 58)        return 94; 
    if (k->key() == 59)        return 36; 
    if (k->key() == 34)        return 64; 
    if (k->key() == 44)        return 63; 
    if (k->key() == 8470)      return 35;   

    if ( map_rus[ DefCodec->fromUnicode(k->text()) ]!= 0 )  {    res = map_rus[ DefCodec->fromUnicode(k->text()) ];  return res;   }
    
    return k->key();
}
/*********************************************************************************/
int KeyboardDevice::FindCode( QKeyEvent * k ) {
    //printf("FindCode . status = %d \n", status);
    //printf("FindCode - key = %d  status =%d--\n", k->key(), status);
    
    //printf("pr1 = %d , pr2 = %d su1 = %d su2 = %d len_code = %d \n", pr1, pr2, su1, su2, len_code);
    
    int code = k->key();	
    bool bad_symbol = false;
    //if ( code == 4129 || code == 4128 || code == 4131 || code == 4132 || code == 4384 || ( code > 4144 && code < 4155 ) )  bad_symbol = true; 
    if (k->ascii() == 0 && k->text().length() == 0) bad_symbol = true;
    //if ( bad_symbol && ( status == 0 || type_kbdevice != READER_TYPE  ) ) return 0;
    if ( bad_symbol && ( status == 0 ) ) return 0;
    if ( bad_symbol ) return 1;
    code = correctCode(k);
    if (k->state() & Qt::ControlButton)  code = k->key() - 64;
    
    //printf("FindCode - code = %d \n", code);
    
    //printf("MainString len = %d \n", MainString.length());
    if ( ( status == 2 ) && (code < 32) && MainString.length() < 1) {
	gulp_codes.clear();
	gulp_codes.push_back( *(k) );
	return 6;   
    }
    if ( ( status == 2 ) && (code < 32) && MainString.length() > len_code ) {
	//gulp_codes.push_back( *(k) );
	return 7;
    }
    
    if ( status > 0 ) {
	gulp_codes.push_back( *(k) );
	timer->stop();
	//puts("timer stop");
	if ( bad_symbol && type_kbdevice == READER_TYPE )  {
	      timer->start(TIME_INTERSYMBOL, true); 
	      return 1;
	}
    }
    
    
    //printf("MainString = %s \n", MainString.ascii());
    if ( status == 0 ) {
	if ( code == pr1 ) {
	    status++; 
	    gulp_codes.push_back(*(k)); 
	    if ( pr2 == 0 ) status++;
	    timer->start(TIME_INTERSYMBOL, true);
	    MainString = "";
	    return 1;  }
	else return 0;
    }  
    
    if ( status == 1 ) {
	if ( code == pr2 ) {	    status++;    timer->start(TIME_INTERSYMBOL, true);    return 1;    } 
	else   return 5;      
    }
    
    if ( status == 2 ) {
	//printf("MainString.length() = %d \n",MainString.length() );
	if ( MainString.length() > len_code )  return 5;	
	//printf("code = %d suf1 = %d \n", code, su1);
	if ( code != su1  ) {
	    if ( MainString.length() < len_code && code >= 32 )    MainString += code;  
	    else return 5;
	} else {
	    if ( su2 == 0 ) {
		status = 0;
		gulp_codes.clear();
		return 2;
	    }
	    status++;
	}
	timer->start(TIME_INTERSYMBOL, true);
	return 1;
    }
    
    if ( status == 3 ) {
	if ( code == su2 || su2 == 0 )  {
	    //puts("code == 2");
	    status = 0;
	    gulp_codes.clear();
	    return 2; }
	else return 5;
    }
    return status;
}  
/*********************************************************************************/
void KeyboardDevice::clearPrefSuf()
{
    pr1 = 0;
    pr2 = 0;
    su1 = 0;
    su2 = 0;
}
/*********************************************************************************/
void KeyboardDevice::setPrefSuf(int pref1, int pref2, int suf1, int suf2)
{
    pr1 = pref1;
    pr2 = pref2;
    su1 = suf1;
    su2 = suf2;
    
    if ( pr1 == 0 && pr2 != 0 ) { pr1 = pr2; pr2 = 0;  }
    if ( su1 == 0 && su2 != 0 ) { su1 = su2; su2 = 0; }
}

/*********************************************************************************/
int KeyboardDevice::GetCodeLen() { 
    if ( status == 0 ) return MainString.length();   
    else return 0; 
}
/*********************************************************************************/
int KeyboardDevice::detectedPrefixSufix(QKeyEvent * k)
{
    
    status = 0;
    int code = k->key();	
    bool bad_symbol = false;
    //if ( code == 4129 || code == 4128 || code == 4131 || code == 4132 || code == 4384 || ( code > 4144 && code < 4155 ) )  bad_symbol = true; 
    if (k->ascii() == 0 && k->text().length() == 0) bad_symbol = true;
    if ( bad_symbol ) return 1;
    
    code = correctCode(k);
    if (k->state() & Qt::ControlButton)  code = k->key() - 64;
    
    //printf("detectedPrefixSufix - right code == %d \n", code);
    gulp_codes.push_back(*(k));
    if (!bad_symbol) {
	MainString += code;	
    }
    
    timer->stop();
    timer->start(TIME_INTERSYMBOL, true); 
    
    return 1;
}
/*********************************************************************************/





