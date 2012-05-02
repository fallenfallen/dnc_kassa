#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class Dialog : public QWidget
{
    Q_OBJECT

public slots:
    int show();

private:
    void initInterface();
    void initConnections();
    void setWindowCenter();

};

#endif // DIALOG_H
