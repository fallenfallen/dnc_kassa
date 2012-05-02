#include "server.h"

#include <QtNetwork>

Socket::Socket() : QTcpSocket() {}

Socket::~Socket() {}

void Socket::send(const QByteArray &data)
{
    write(data);
}

ThreadedPrivateServer::ThreadedPrivateServer(QUuid id, int socketDescriptor, QObject * parent) :
        id(id), socketDescriptor(socketDescriptor), QThread(parent) {}

ThreadedPrivateServer::~ThreadedPrivateServer() {}

void ThreadedPrivateServer::run()
{
    socket = new Socket;
    if (socket->setSocketDescriptor(socketDescriptor))
    {
        connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
        connect(socket, SIGNAL(disconnected()), SLOT(disconnected()));
        connect(socket, SIGNAL(disconnected()), SLOT(quit()));
        connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
        connect(this, SIGNAL(send(QByteArray)), socket, SLOT(send(QByteArray)));
        exec();
    }
    else
        emit information(id, new Exception(conv("Невозможно создать подключение c клиентом id %1.").arg(id),
                                           conv("Ошибка сокета %1: %2.").arg(QString::number(socket->error()), socket->errorString()), 201));
}

void ThreadedPrivateServer::readyRead()
{
    QByteArray cmd = socket->readAll();
    emit information(id, new Information(conv("Получена команда от id %1 %2.").arg(id).arg(QString(cmd))));
    emit receiveCommnad(id, cmd);
}

void ThreadedPrivateServer::setID(const QUuid &serverID, const QUuid &newID)
{
    if (id == serverID)
    {
        id = newID;

        emit information(id, new Information(conv("Текущее подключение %1 ассоциировано с id %2.").arg(serverID).arg(newID)));
    }
}

void ThreadedPrivateServer::disconnected()
{
    emit information(id, new Warning(conv("Соединение с клиентом id %1 потеряно.").arg(id)));
    emit disconnect(id);
}

void ThreadedPrivateServer::sendReply(QUuid id, const QByteArray &data)
{
    emit information(id, new Information(conv("Отправлены данные от id %1 %2.").arg(id).arg(QString(data))));
    if (this->id == id)
        emit send(data);
}

Server::Server() : QTcpServer()
{
    // Загрузить данные о предыдущих соединениях rmkList
}

Server::~Server()
{
    // Выгружаем rmkList в базу данных
}

void Server::incomingConnection(int socketDescriptor)
{
    QUuid id = QUuid::createUuid();
    ThreadedPrivateServer * server = new ThreadedPrivateServer(id, socketDescriptor);
    // Отправка результата запроса
    connect(this, SIGNAL(sendReply(QUuid, QByteArray)), server, SLOT(sendReply(QUuid, QByteArray)));
    // Получение команды
    connect(server, SIGNAL(receiveCommnad(QUuid, QByteArray)), SIGNAL(receiveCommand(QUuid, QByteArray)));
    // Ошибка и предупреждение
    connect(server, SIGNAL(information(QUuid, Information *)), SLOT(informationHandle(QUuid, Information *)));
    // Сигнал завершения соеднинения сервера
    connect(server, SIGNAL(finished()), server, SLOT(deleteLater()));
    connect(server, SIGNAL(disconnect(QUuid)), SLOT(disconnected(QUuid)));
    connect(this, SIGNAL(setID(QUuid, QUuid)), server, SLOT(setID(QUuid, QUuid)));
    connectionList.insert(id, server);

    server->start();
}

void Server::informationHandle(QUuid id, Information * information)
{
    emit Server::information(information);
}

void Server::disconnected(const QUuid &id)
{
    connectionList.remove(id);
}

void Server::assosiateSession(int rmk, QUuid &id)
{
    if (rmkList.contains(rmk))
    {
        QUuid serverID = id;
        ThreadedPrivateServer * server = connectionList.value(serverID);
        connectionList.remove(serverID);
        id = rmkList.value(rmk);
        connectionList.remove(id);
        connectionList.insert(id, server);
        emit setID(serverID, id);
    }
    else
        rmkList.insert(rmk, id);
}
