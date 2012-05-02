#ifndef KEYBOARDDEVICE_CLASS
#define KEYBOARDDEVICE_CLASS

#include <qobject.h>
//#include <qevent.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <qstring.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <qtextcodec.h>
#include "const.h"


#include <vector>
#include <string>
#include <map>

using namespace std;

class KeyboardDevice : public QObject
{  Q_OBJECT 
protected :
    QObject * reciever;
    int pr1;
    int pr2;
    int su1;
    int su2;
    int status;
    int len_code;
    int type_kbdevice;
    
    Display* g_d;
    XkbStateRec state;
    QTextCodec* DefCodec;
    
    QString MainString; 
    
    map<QString, int> map_rus;
    
    void initMapRusSymbol();

public :	
      QTimer * timer;	
      vector <QKeyEvent> gulp_codes;
      KeyboardDevice(QObject * obj, int pref1, int pref2, int suf1, int suf2);
      KeyboardDevice();
      ~KeyboardDevice();
      
      // 0 - клавиатурный ввод, 
      // 1 - возможный ввод с клавиатуры, 
      // 2 - получен сигнал от клавиатурного устройства, 
      // 5 - накопленная последовательность не является параметром сигнала от клавиатурного устройства ( нужно отправить ее в интерфейс )
      // 6 - количество префиксов больше чем 2
      // 7 - количество суффиксов больше чем 2
      int FindCode(QKeyEvent * k); 
      
      int correctCode( QKeyEvent * k );      
      
      //-----(*)-----Zuskin-----23/11/2011-----
      //static const int TIME_INTERSYMBOL = 200;
      int TIME_INTERSYMBOL;
      //---------------------------------------
      static const int SCANER_TYPE = 1;
      static const int READER_TYPE = 2;
      static const int NONE_DEVICE = 0;
      
      QKeyEvent * GetEvent(int index);
      int GetCountEvent();
      void setDeviceType(int type);
      void Clear();
      
      int getSumPrefSuf();
      
      int GetCodeLen();
      int detectedPrefixSufix(QKeyEvent * k);
      void clearPrefSuf();
      	
      int getPrefix1() { return pr1; }
      int getPrefix2() { return pr2; }
      int getSuffix1() { return su1; }
      int getSuffix2() { return su2; }
      
      void setPrefSuf(int pref1, int pref2, int suf1, int suf2);
      
};
#endif


