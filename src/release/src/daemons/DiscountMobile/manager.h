#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QHostAddress>

#include "server.h"
#include "exception.h"
#include "networkrequest.h"
#include "datamanager.h"
#include "databaseengine.h"

class Manager : public QObject
{
    Q_OBJECT

public:
    Manager();
    ~Manager();

    void manage();

signals:
    void information(Information * information);
    void serverRestarted(QUuid id, int rmk, int cmd, int error, const QString &description);

private slots:
    void informationHandle(Information * information);
    void restart(QUuid id, int rmk, int cmd);

private:
    bool startServer();
    void initValuesFromDatabase();
    void initProxySettingsFromDatabase();

    Server * server;
    Database * database;
    DataManager * manager;
    NetworkRequest * request;

    bool connected;
    int serverPort;
    int timeout, attemptCount;
    QHostAddress serverHost;

};

#endif // MANAGER_H
