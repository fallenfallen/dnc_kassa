#include "manager.h"
#include "settings.h"
#include "translate.h"
#include "dbconfigdialog.h"
#include "databaseengine.h"

Manager::Manager() : QObject()
{
    DatabaseAdapter::init();
}

Manager::~Manager()
{
    DatabaseAdapter::finilize();
}

int Manager::manage()
{
    int port;
    QString name, host;
    QString user, password;

    DatabaseEngine * engine = DatabaseAdapter::adapter();
    connect(engine, SIGNAL(information(Information *)), SLOT(informationHaldler(Information *)));

    if (!Settings::readConfig(name, host, port, user, password) ||
        !DatabaseAdapter::connect(name, host, port, user, password))
    {
        databaseDialog = new DBConfigDialog;
        connect(databaseDialog, SIGNAL(information(Information *)), SLOT(informationHaldler(Information *)));
        connect(databaseDialog, SIGNAL(databaseConnected()), SLOT(databaseConnected()));
        databaseDialog->show();
    }
    else
        databaseConnected();
}

void Manager::informationHaldler(Information * information)
{
    information->print();
}

void Manager::databaseConnected()
{
    systemDialog = new SystemConfigDialog;
    connect(systemDialog, SIGNAL(information(Information *)), SLOT(informationHaldler(Information *)));
    systemDialog->show();
}
