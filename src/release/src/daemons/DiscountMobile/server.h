#ifndef SERVER_H
#define SERVER_H

#include <QUuid>
#include <QObject>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

#include "exception.h"

class Socket : public QTcpSocket
{
    Q_OBJECT

public:
    Socket();
    ~Socket();

public slots:
    void send(const QByteArray &data);

};

class ThreadedPrivateServer : public QThread
{
    Q_OBJECT

public:
    ThreadedPrivateServer(QUuid id, int socketDescriptor, QObject * parent = 0);
    ~ThreadedPrivateServer();

protected:
    void run();

signals:
    void information(QUuid id, Information * information);
    void receiveCommnad(QUuid id, const QByteArray &cmd);
    void send(const QByteArray &data);
    void disconnect();
    void disconnect(const QUuid &id);

public slots:
    void setID(const QUuid &serverID, const QUuid &newID);
    void sendReply(QUuid id, const QByteArray &data);

private slots:
    void readyRead();
    void disconnected();

private:
    QUuid id;
    Socket * socket;
    int socketDescriptor;

};

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server();
    ~Server();

signals:
    void information(Information * information);
    void sendReply(QUuid id, const QByteArray &data);
    void receiveCommand(QUuid id, const QByteArray &cmd);
    void setID(const QUuid &serverID, const QUuid &newID);

private slots:
    void disconnected(const QUuid &id);
    void assosiateSession(int rmk, QUuid &id);
    void informationHandle(QUuid id, Information * information);

protected:
    void incomingConnection(int socketDescriptor);

private:
    QMap<QUuid, ThreadedPrivateServer *> connectionList;
    QMap<int, QUuid> rmkList;

};

#endif // SERVER_H
