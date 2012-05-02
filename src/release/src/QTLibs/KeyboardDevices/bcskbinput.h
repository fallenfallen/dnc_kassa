#ifndef SCANERKEYBOARD_CLASS
#define SCANERKEYBOARD_CLASS

#include <qobject.h>
#include "dkbinput.h"


using namespace std;

class KeyboardScaner : public KeyboardDevice
{      Q_OBJECT
public :
	KeyboardScaner(QObject * obj, int pref1, int pref2, int suf1, int suf2);
        KeyboardScaner();
        ~KeyboardScaner();

      
	QString getCode();
	QString getCodeWithPrefSuf();

};

#endif

