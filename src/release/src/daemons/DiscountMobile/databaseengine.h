#ifndef DATABASEENGINE_H
#define DATABASEENGINE_H

#include <QMap>
#include <QObject>
#include <QVariant>
#include <libpq-fe.h>

#include "request.h"
#include "exception.h"
#include "cryptoengine.h"

typedef QMap<QString, QVariant> DBRow;
typedef QList<DBRow> DBRecord;

class DatabaseEngine : public QObject
{
    Q_OBJECT

public:
    bool connect(const QString &name, const QString &host, int port,
                 const QString &user, const QString &password);
    void disconnect();

    DBRecord select(const QString &sql, bool &error);
    DBRecord execute(const QString &sql, const QList<QVariant> &values, bool &error);

signals:
    void information(Information * information);

private:
    PGconn * connection;

    int length(const char * str);
    char * variable(const QVariant &variable);
    DBRecord fetchResults(const PGresult * result);

};

class DatabaseAdapter
{
public:
    static void init();
    static void finilize();

    static bool connect(const QString &name, const QString &host = "127.0.0.1",
                        int port = 5432, const QString &user = "postgres", const QString &password = "");
    static void disconnect();
    static DatabaseEngine * adapter();

private:
    static DatabaseEngine * engine;
};

class DatabaseLoader
{
public:
    static void init();

    // Local server properties
    static QString serverHost();
    static int serverPort();
    static int serverAttemptCount();
    static int serverConnectionTimeout();
    // Discount server properties
    static int memberID();
    static QString systemURL();
    static QString systemIPv4Host();
    static QString systemIPv6Host();
    static int systemPort();
    static int systemRequestTimeout();
    static int systemRetryCount();
    static int systemAttemptTimeout();
    static QString discountMobileSertify();
    static bool useProxy();
    static bool proxySettings(int &type, QString &host, int &port, QString &user, QString &password);

private:
    static DatabaseEngine * engine;
    static QVariant configRequest(const QString &table, const QString &field);
    static bool configRequest(const QString &table, const QString &field, const QVariant &data);

};

class Database : public QObject
{
    Q_OBJECT

public:
    Database();
    ~Database();

    void start();
    void stop();

protected:
    void timerEvent(QTimerEvent * event);

signals:
    void loadedData(const QList<Request *> req);
    void information(Information * information);

public slots:
    void addRequestToDatabase(Request * request);

private:
    int timer;
    CryptoEngine * engine;

};

#endif // DATABASEENGINE_H
