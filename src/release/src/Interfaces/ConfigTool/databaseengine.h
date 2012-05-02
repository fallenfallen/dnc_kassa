#ifndef DATABASEENGINE_H
#define DATABASEENGINE_H

#include <QMap>
#include <QObject>
#include <QVariant>
#include <libpq-fe.h>

#include "exception.h"

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
                        int port = 5432, const QString &user = "postgres", const QString &password = QString());
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
    static bool setServerHost(const QString &host);
    static int serverPort();
    static bool setServerPort(int port);
    static int serverAttemptCount();
    static bool setServerAttemptCount(int attemptCount);
    static int serverConnectionTimeout();
    static bool setServerConnectionTimeout(int timeout);
    // Discount server properties
    static int memberID();
    static bool setMemberID(int member);
    static QString systemURL();
    static bool setURL(const QString &url);
    static QString systemIPv4Host();
    static bool setSystemIPv4Host(const QString &host);
    static QString systemIPv6Host();
    static bool setSystemIPv6Host(const QString &host);
    static int systemPort();
    static bool setSystemPort(int port);
    static int systemRequestTimeout();
    static bool setSystemRequestTimeout(int timeout);
    static int systemRetryCount();
    static bool setSystemRetryCount(int retryCount);
    static int systemAttemptTimeout();
    static bool setSystemAttemptTimeout(int timeout);
    static QString discountMobileSertify();
    static bool setDiscountMobileSertify(const QString &sertify);
    static bool useProxy();
    static bool proxySettings(int &type, QString &host, int &port, QString &user, QString &password);
    static bool setProxySettings(bool use, int type, const QString &host, int port,
                                 const QString &user = QString(), const QString &password = QString());

private:
    static DatabaseEngine * engine;
    static QVariant configRequest(const QString &table, const QString &field);
    static bool configRequest(const QString &table, const QString &field, const QVariant &data);
};

#endif // DATABASEENGINE_H
