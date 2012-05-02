#include <stdio.h>
#include <stdlib.h>

#include <qlineedit.h> 
#include <qevent.h>
#include <qpushbutton.h>
/*#include <qlabel.h> 
#include <qvalidator.h>*/
#include <qstring.h>
#include <qobject.h>

class UpDownLineEdit: public QLineEdit
{
    Q_OBJECT
public: 
    static const int EDIT_WIDTH = 25;
    static const int BUTTON_HEIGHT = 15;
    static const int EDIT_HEIGHT = 21;
    UpDownLineEdit(QWidget *parent, const char *name = 0, QString possible_value = "");
    ~UpDownLineEdit();
    void focusInEvent(QFocusEvent * fe);
    void focusOutEvent(QFocusEvent * fe);
    void keyPressEvent(QKeyEvent * ke);
    void setPosition(int x, int y);
    void setEnabled(bool b);
    
    void setPossibleValue(QString str);
    void setValue(int num);
    int getValue();
    int setText(QChar c);
    int getIndex(QChar c);
    
    
public slots:
    void GoUp();
    void GoDown();
    
private:
    uint index;
    QString possible_val;
    QPushButton * buttonUp;
    QPushButton * buttonDown;

};

 
