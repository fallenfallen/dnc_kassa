#ifndef SERVERCLIENT_H
#define SERVERCLIENT_H

#include <QTcpSocket>

#include "exception.h"

class ServerClient : public QObject
{
    Q_OBJECT

public:
    ServerClient();
    ~ServerClient();

signals:
    void information(Information * information);
    void connectionResult(bool connected);
    void restartResult(int error, const QString &description);
    void parseError();

public slots:
    void checkServerConnection(const QString &host, int port);
    void restartServer();

private slots:
    void readyRead();
    void connected();
    void error(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket * socket;
};

#endif // SERVERCLIENT_H
