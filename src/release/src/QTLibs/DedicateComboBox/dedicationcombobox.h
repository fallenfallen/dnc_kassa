

#ifndef DEDICATIONCOMBOBOX_H
#define DEDICATIONCOMBOBOX_H

#include <qcombobox.h>

class DedicationComboBox : public QComboBox
{
public :
   DedicationComboBox ( QWidget * parent = 0, const char * name = 0 );    
   void focusInEvent ( QFocusEvent * foc);
   void focusOutEvent ( QFocusEvent * foc);
};


#endif // DEDICATIONCOMBOBOX_H
