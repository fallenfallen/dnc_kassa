#include <stdio.h>
#include <stdlib.h>

#include <qspinbox.h> 
#include <qlabel.h> 
#include <qlineedit.h>
#include <qvalidator.h>
#include <qstring.h>
#include <qobject.h>


class DoubleSpinBox : public QSpinBox
{
    Q_OBJECT
public: 
    DoubleSpinBox(QWidget *parent, const char *name = 0);
    ~DoubleSpinBox();
    void setGeometry (const QRect & rect);
public slots:
    void editorTextChanged( const QString & );
private:
    QLineEdit* suffixLabel;
protected:
    QString mapValueToText(int value);
    int mapTextToValue(bool *ok);
};

