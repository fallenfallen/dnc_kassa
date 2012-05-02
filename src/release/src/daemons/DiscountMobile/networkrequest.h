#ifndef NETWORKREQUEST_H
#define NETWORKREQUEST_H

#include <QUuid>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QAuthenticator>
#include <QNetworkAccessManager>

#include "request.h"
#include "exception.h"

class NetworkRequest : public QObject
{
    Q_OBJECT

public:
    NetworkRequest();
    ~NetworkRequest();

    void initValuesFromDatabase();
    void start();
    void stop();

signals:
    void information(Information * information);
    void sendError(QUuid id, int rmk, int cmd, int error, const QString &description);
    void receiveReply(QUuid id, int rmk, int cmd, const QByteArray &xml);
    void addToDatabase(Request * request);

public slots:
    void addRequest(QUuid id, int rmk, int cmd, const QByteArray &xml);
    void loadFromDatabase(QList<Request *> requests);

private slots:
    void requestFinished();
    void requestError(QNetworkReply::NetworkError code);
    void authenticationRequired(QNetworkReply * reply, QAuthenticator * authenticator);
    void proxyAuthenticationRequired(QNetworkProxy proxy, QAuthenticator * authenticator);

protected:
    void timerEvent(QTimerEvent * event);

private:
    void addValue(Request * request);
    void updateRequestsQueue();
    bool errorAnalize(QNetworkReply::NetworkError code);
    void setProxy(int type, const QString &host, int port,
                  const QString &user = QString(), const QString &password = QString());

    int timer;
    QUrl url;
    QMap<QUuid, Request *> requests, newRequests;
    QMap<QUuid, QNetworkReply *> replies;
    QNetworkAccessManager * manager;
    int maxAttempts, timeout, attemptTimeout;

};

#endif // NETWORKREQUEST_H
