#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>

#include "exception.h"
#include "dbconfigdialog.h"
#include "systemconfigdialog.h"

class Manager : public QObject
{
    Q_OBJECT

public:
    Manager();
    ~Manager();

    int manage();

private slots:
    void informationHaldler(Information * information);
    void databaseConnected();

private:
    DBConfigDialog * databaseDialog;
    SystemConfigDialog * systemDialog;

};

#endif // MANAGER_H
