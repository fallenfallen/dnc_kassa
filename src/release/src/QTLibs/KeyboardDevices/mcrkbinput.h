#ifndef READERKEYBOARD_CLASS
#define READERKEYBOARD_CLASS

#include <qobject.h>
#include <qstring.h>
#include "dkbinput.h"
//#include "dncdkbforrussymbol.h"

using namespace std;

class KeyboardReader : public KeyboardDevice
{
  Q_OBJECT
private:
    //DKB_FOR_RUS_SYMBOL* dkbfrs;
    
  public :
    KeyboardReader(QObject * obj, int pref1, int pref2, int suf1, int suf2);
    KeyboardReader();
    ~KeyboardReader();
    
    //static const int max_msec_reader = 1000;
    //virtual int FindCode( QKeyEvent * k );
    QString getCodeWithPrefSuf();
    
    virtual QString getCode(int& num_track);
    QString  getAllTrack();
    
    QString getTrack(int track_num);
    
};

#endif

 
