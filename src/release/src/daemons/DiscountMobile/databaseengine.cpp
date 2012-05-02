#include "databaseengine.h"

#include <QTimerEvent>

DatabaseEngine * DatabaseAdapter::engine = 0;
DatabaseEngine * DatabaseLoader::engine = 0;

void DatabaseAdapter::init()
{
    if (engine == 0)
        engine = new DatabaseEngine;
}

bool DatabaseAdapter::connect(const QString &name, const QString &host, int port,
                              const QString &user, const QString &password)
{
    bool result = true;
    if (!engine || !engine->connect(name, host, port, user, password))
    {
        disconnect();
        result = false;
    }

    return result;
}

void DatabaseAdapter::disconnect()
{
    if (engine != 0)
        engine->disconnect();
}

void DatabaseAdapter::finilize()
{
    if (engine != 0)
    {
        engine->disconnect();
        delete engine;
        engine = 0;
    }
}

DatabaseEngine * DatabaseAdapter::adapter()
{
    return engine;
}

bool DatabaseEngine::connect(const QString &name, const QString &host, int port,
                             const QString &user, const QString &password)
{
    bool result = false;
    QString connectionString = "dbname = '%1' hostaddr = '%2' port = '%3' user = '%4' password = '%5' connect_timeout = '10'";
    connectionString = connectionString.arg(name).arg(host).arg(port).arg(user).arg(password);

    connection = PQconnectdb(connectionString.toAscii());
    if (!connection && PQstatus(connection) != CONNECTION_OK)
    {
        emit information(new Exception(conv("Невозможно подключиться к базе данных %1 по адресу %2:%3.").arg(name).arg(host).arg(port),
                                       conv("Ошибка драйевера базы данных %1.").arg(PQerrorMessage(connection)), 102));
    }
    else
    {
        emit information(new Information(conv("База данных %1, работающая на адресе %2:%3 подключена.").arg(name).arg(host).arg(port)));
        result = true;
    }

    return result;
}

void DatabaseEngine::disconnect()
{
    if (PQstatus(connection) != CONNECTION_OK)
    {
        PQfinish(connection);
        emit information(new Information(conv("База данных отключена.").arg(PQdb(connection)).arg(PQhost(connection)).arg(PQport(connection))));
    }
}

DBRecord DatabaseEngine::fetchResults(const PGresult * result)
{
    DBRecord record;
    int rows = PQntuples(result);
    int columns = PQnfields(result);
    for (int i = 0; i < rows; ++i)
    {
        DBRow row;
        for (int j = 0; j < columns; ++j)
        {
            QString key = PQfname(result, j);
            QVariant field = QVariant(PQgetvalue(result, i, j));
            row.insert(key, field);
        }
        record.append(row);
    }

    return record;
}

DBRecord DatabaseEngine::select(const QString &sql, bool &error)
{
    DBRecord record;
    PGresult * result;
    ExecStatusType status;

    result = PQexec(connection, sql.toAscii());
    if ((status = PQresultStatus(result)) == PGRES_TUPLES_OK)
    {
        error = false;
        record = fetchResults(result);
    }
    else
    {
        error = true;
        emit information(new Exception(conv("Ошибка выполнения запроса на считывание данных."),
                                       conv("Ошибка драйвера базы данных %1: %2.").arg(PQresStatus(status)).arg(PQerrorMessage(connection)), 103));
    }
    PQclear(result);

    return record;
}

char * DatabaseEngine::variable(const QVariant &variable)
{
    char * result;
    switch (variable.type())
    {
    case QVariant::String:
    case QVariant::ByteArray:
        {
           QByteArray array = variable.toByteArray();
           result = new char[array.length()];
           strcpy(result, array.data());
        }
        break;

    case QVariant::Int:
        {
            QByteArray array = QByteArray::number(variable.toInt());
            result = new char[array.length()];
            strcpy(result, array.data());
        }
        break;

    case QVariant::Double:
        {
            QByteArray array = QByteArray::number(variable.toDouble());
            result = new char[array.length()];
            strcpy(result, array.data());
        }
        break;
    }

    return result;
}

int DatabaseEngine::length(const char * str)
{
    return strlen(str);
}

DBRecord DatabaseEngine::execute(const QString &sql, const QList<QVariant> &values, bool &error)
{
    DBRecord record;
    PGresult * result;
    ExecStatusType status;

    int len = values.count();
    char * paramValues[len];
    int paramLength[len];
    int paramFormat[len];

    for (int i = 0; i < len; ++i)
    {
        paramFormat[i] = 0;
        paramValues[i] = variable(values[i]);
        paramLength[i] = length(paramValues[i]);
    }

    result = PQexecParams(connection, sql.toAscii(), len, NULL,
                          paramValues,
                          paramLength,
                          paramFormat, 0);

    if ((status = PQresultStatus(result)) == PGRES_COMMAND_OK || status == PGRES_TUPLES_OK)
    {
        error = false;
        record = fetchResults(result);
    }
    else
    {
        error = true;
        emit information(new Exception(conv("Ошибка выполнения запроса на считывание данных."),
                                       conv("Ошибка драйвера базы данных %1: %2.").arg(PQresStatus(status)).arg(PQerrorMessage(connection)), 103));
    }
    PQclear(result);

    return record;
}

void DatabaseLoader::init()
{
    if (engine == 0)
        engine = DatabaseAdapter::adapter();
}

QVariant DatabaseLoader::configRequest(const QString &table, const QString &field)
{
    QString sql = "SELECT %1 FROM %2";
    sql = sql.arg(field).arg(table);
    bool error;
    DBRecord record = engine->select(sql, error);
    QVariant result = record[0].value(field);

    return result;
}

bool DatabaseLoader::configRequest(const QString &table, const QString &field, const QVariant &data)
{
    QString sql = "UPDATE %1 SET %2 = $1";
    sql = sql.arg(table).arg(field);
    QList<QVariant> values;
    values.append(data);
    bool error;
    engine->execute(sql, values, error);

    return !error;
}

QString DatabaseLoader::serverHost()
{
    return configRequest("local_server_config", "server_host").toString();
}

int DatabaseLoader::serverPort()
{
    return configRequest("local_server_config", "server_port").toInt();
}

int DatabaseLoader::serverAttemptCount()
{
    return configRequest("local_server_config", "server_attempt_count").toInt();
}

int DatabaseLoader::serverConnectionTimeout()
{
    return configRequest("local_server_config", "server_attempt_timeout").toInt();
}

int DatabaseLoader::memberID()
{
    return configRequest("discount_mobile_config", "system_member").toInt();
}

QString DatabaseLoader::systemURL()
{
    return QUrl::fromPercentEncoding(configRequest("discount_mobile_config", "system_url").toByteArray());
}

QString DatabaseLoader::systemIPv4Host()
{
    return configRequest("discount_mobile_config", "system_ipv4_host").toString();
}

QString DatabaseLoader::systemIPv6Host()
{
    return configRequest("discount_mobile_config", "system_ipv6_host").toString();
}

int DatabaseLoader::systemPort()
{
    return configRequest("discount_mobile_config", "system_port").toInt();
}

int DatabaseLoader::systemRequestTimeout()
{
    return configRequest("discount_mobile_config", "system_reply_timeout").toInt();
}

int DatabaseLoader::systemRetryCount()
{
    return configRequest("discount_mobile_config", "system_attempt_count").toInt();
}

int DatabaseLoader::systemAttemptTimeout()
{
    return configRequest("discount_mobile_config", "system_attempt_timeout").toInt();
}

QString DatabaseLoader::discountMobileSertify()
{
    return configRequest("discount_mobile_config", "discount_mobile_sertify").toString();
}

bool DatabaseLoader::useProxy()
{
    return configRequest("discount_mobile_config", "use_proxy").toBool();
}

bool DatabaseLoader::proxySettings(int &type, QString &host, int &port, QString &user, QString &password)
{
    bool result = true;
    if (useProxy())
    {
        QString sql = "SELECT proxy_type, proxy_host, proxy_port, proxy_user, proxy_password "
                      "FROM proxy_server INNER JOIN discount_mobile_config ON proxy_server.proxy_id = discount_mobile_config.system_proxy";
        bool error;
        DBRecord record = engine->select(sql, error);
        type = record[0].value("proxy_type").toInt();
        host = record[0].value("proxy_host").toString();
        port = record[0].value("proxy_port").toInt();
        user = record[0].value("proxy_user").toString();
        password = record[0].value("proxy_password").toString();
        result = !error;
    }
    else
    {
        type = 0;
        host = ""; port = 0;
        user = ""; password = "";
    }

    return result;
}

Database::Database() : QObject()
{
    engine = new AESEngine;
    engine->generateKey();
}

Database::~Database()
{
    delete engine;
}

void Database::stop()
{
    killTimer(timer);
}

void Database::start()
{
    int interval = DatabaseLoader::systemAttemptTimeout();
    timer = startTimer(interval * 1000);
    if (!timer)
        emit information(new Exception(conv("Невозможно запустить таймер опроса базы данных."), 1));
}

void Database::addRequestToDatabase(Request * request)
{
    bool error;
    QString sql = "INSERT INTO requests (request_uid, request_rmk, request_command, request_xml, request_url) VALUES($1, $2, $3, $4, $5)";
    QList<QVariant> values;
    values << request->id.toString() << request->rmk << request->cmd
           << engine->encrypt(request->xml) << request->url.toString();

    DatabaseEngine * db = DatabaseAdapter::adapter();
    db->execute(sql, values, error);
}

void Database::timerEvent(QTimerEvent * event)
{
    emit information(new Information(conv("Опрос базы данных по таймеру.")));

    bool error;
    QString sql = "SELECT request_id, request_uid, request_rmk, request_command, request_xml, request_url FROM requests";
    DatabaseEngine * db = DatabaseAdapter::adapter();
    DBRecord record = db->select(sql, error);
    if (!error)
    {
        QString id;
        QList<Request *> list;
        foreach (DBRow row, record)
        {
            id += row.value("request_id").toString() + ", ";
            QUuid uniqID = row.value("request_uid").toString();
            QByteArray data = row.value("request_xml").toByteArray();
            QByteArray xml = engine->decrypt(data);
            QUrl url = row.value("request_url").toString();
            int rmk = row.value("request_rmk").toInt();
            int cmd = row.value("request_command").toInt();
            Request * request = new Request(uniqID, rmk, cmd, xml, url);
            list.append(request);
        }
        id = id.left(id.length() - 2);

        if (!id.isEmpty())
        {
            sql = "DELETE FROM requests WHERE request_id IN (" + id + ")";

            db->select(sql, error);
            if (!error)
                emit loadedData(list);
        }
        else
            emit loadedData(list);
    }
}
