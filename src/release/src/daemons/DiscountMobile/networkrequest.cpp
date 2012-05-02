#include "networkrequest.h"
#include "databaseengine.h"

NetworkRequest::NetworkRequest() : QObject()
{
    manager = new QNetworkAccessManager;

    initValuesFromDatabase();

    connect(manager, SIGNAL(authenticationRequired(QNetworkReply *, QAuthenticator *)),
            SLOT(authenticationRequired(QNetworkReply *, QAuthenticator *)));
    connect(manager, SIGNAL(proxyAuthenticationRequired(QNetworkProxy, QAuthenticator *)),
            SLOT(proxyAuthenticationRequired(QNetworkProxy, QAuthenticator *)));
}

void NetworkRequest::initValuesFromDatabase()
{
    bool useProxy = DatabaseLoader::useProxy();
    if (useProxy)
    {
        int type, port;
        QString host, user, password;
        DatabaseLoader::proxySettings(type, host, port, user, password);
        setProxy(type, host, port, user, password);
    }

    timeout = DatabaseLoader::systemRequestTimeout();
    attemptTimeout = DatabaseLoader::systemAttemptTimeout();
    maxAttempts = DatabaseLoader::systemRetryCount();
    url = QUrl(DatabaseLoader::systemURL());
}

NetworkRequest::~NetworkRequest()
{
    delete manager;
}

void NetworkRequest::stop()
{
    emit information(new Information(conv("Остановка системы отправки сообщений к дисконтной системе.")));

    killTimer(timer);
}

void NetworkRequest::setProxy(int type, const QString &host, int port, const QString &user, const QString &password)
{
    emit information(new Information(conv("Настройка соединения через прокси-сервер на адрес %1 и порт %2.").arg(host).arg(port)));

    QNetworkProxy proxy;
    proxy.setType((QNetworkProxy::ProxyType)type);
    proxy.setHostName(host);
    proxy.setPort(port);
    proxy.setUser(user);
    proxy.setPassword(password);
    manager->setProxy(proxy);
}

void NetworkRequest::start()
{
    timer = startTimer(3000);
    if (!timer)
        emit information(new Exception(conv("Невозможно запустить таймер опроса базы данных."), 401));
}

void NetworkRequest::timerEvent(QTimerEvent * event)
{
    emit information(new Information(conv("Отправка сообщений в дисконтную систему.")));

    foreach (Request * request, requests.values())
    {
        request->newAttempt();
        QNetworkReply * reply = manager->post(QNetworkRequest(request->url), "str=" + request->xml);
        QUuid id = requests.key(request);
        replies.insert(id, reply);

        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(requestError(QNetworkReply::NetworkError)));
        connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
        connect(reply, SIGNAL(finished()), SLOT(requestFinished()));

        reply->waitForReadyRead(timeout);
    }
    updateRequestsQueue();

    emit information(new Information(conv("Размерность очереди к дисконтной системе %1, ответов %2.").arg(requests.count()).arg(replies.count())));
}

void NetworkRequest::authenticationRequired(QNetworkReply * reply, QAuthenticator * authenticator) {}

void NetworkRequest::proxyAuthenticationRequired(QNetworkProxy proxy, QAuthenticator * authenticator)
{
    emit information(new Exception(conv("Ошибка авторизации прокси-сервера."), 104));
    manager->setProxy(QNetworkProxy());
}

void NetworkRequest::loadFromDatabase(QList<Request *> requests)
{
    emit information(new Information(conv("Загрузка %1 необработанных запросов из базы данных.").arg(requests.count())));

    foreach (Request * req, requests)
        newRequests.insert(QUuid::createUuid(), req);
}

void NetworkRequest::requestError(QNetworkReply::NetworkError code)
{
    QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender());
    emit information(new Exception(conv("Невозможно отправить запрос."), conv("Ошибка протокола %1: %2.").arg(code).arg(reply->errorString()), 102));

    QUuid id = replies.key(reply);
    replies.remove(id);
    Request * request = requests.value(id);

    if (request->attemptCount >= maxAttempts)
    {
        requests.remove(id);
        int rmk = request->rmk;
        int cmd = request->cmd;
        id = request->id;
        if (request->cmd == 2)
        {
            emit sendError(id, rmk, cmd, 101, QString("Невозможно выполнить запрос id %1. Данные занесены в базу. Ошибка протокола %2: %3").arg(id).arg(code).arg(reply->errorString()));
            emit information(new Warning(conv("Невозможно отправить запрос id %1. Данные занесены в базу.").arg(id), conv("Ошибка протокола %1: %2.").arg(code).arg(reply->errorString())));
            emit addToDatabase(request);
        }
        else
        {
            emit sendError(id, rmk, cmd, 102, QString("Невозможно выполнить запрос id %1. Запрос отклонен. Ошибка протокола %2: %3").arg(id).arg(code).arg(reply->errorString()));
            emit information(new Warning(conv("Невозможно отправить запрос id %1. Запрос отклонен.").arg(id), conv("Ошибка протокола %1: %2.").arg(code).arg(reply->errorString())));
        }

        delete request;
    }
}

void NetworkRequest::requestFinished()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender());
    QByteArray xml = reply->readAll();
    QUuid id = replies.key(reply);
    if (!id.isNull())
    {
        emit information(new Information(conv("Запрос id %1 выполнен.").arg(id)));

        replies.remove(id);
        Request * request = requests.value(id);
        requests.remove(id);
        int rmk = request->rmk;
        int cmd = request->cmd;
        id = request->id;
        delete request;

        emit receiveReply(id, rmk, cmd, xml);
    }
}

void NetworkRequest::updateRequestsQueue()
{
    requests.unite(newRequests);
    newRequests.clear();
}

void NetworkRequest::addValue(Request * request)
{
    newRequests.insert(QUuid::createUuid(), request);
}

void NetworkRequest::addRequest(QUuid id, int rmk, int cmd, const QByteArray &xml)
{
    Request * request = new Request(id, rmk, cmd, xml, url);
    addValue(request);
}
