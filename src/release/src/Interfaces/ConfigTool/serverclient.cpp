#include "serverclient.h"

#include <QRegExp>
#include <QStringList>

ServerClient::ServerClient()
{
    socket = new QTcpSocket;
    connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(error(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(connected()), SLOT(connected()));
}

ServerClient::~ServerClient()
{
    if (socket->isOpen())
        socket->close();
    delete socket;
}

void ServerClient::checkServerConnection(const QString &host, int port)
{
    socket->close();
    socket->connectToHost(host, port);
}

void ServerClient::connected()
{
    QByteArray data = "6;0;0;0;";
    socket->write(data);
}

void ServerClient::restartServer()
{
    if (socket->isOpen())
    {
        QByteArray data = "8;0;100;0;";
        socket->write(data);
    }
    else
        emit information(new Exception(conv("Ошибка при запросе к серверу"), conv("Ошибка протокола %1: %2").arg(socket->error()).arg(socket->errorString()), 301));
}

bool parseRequest(const QString &reqeuest, int &length, int &rmk, int &cmd, int &error, QByteArray &data)
{
    QRegExp expr("^(\\d*);(\\d*);(\\d*);(\\d*);(.*)$");
    bool result = expr.indexIn(reqeuest) != - 1;
    if (result)
    {
        QStringList list = expr.capturedTexts();
        length = list[1].toInt();
        rmk = list[2].toInt();
        cmd = list[3].toInt();
        error = list[4].toInt();
        data = list[5].toAscii();
        if (list[2].length() + list[3].length() +
            list[4].length() + list[5].length() + 3 != length)
            result = false;
    }

    return result;
}

void ServerClient::readyRead()
{
    int length, rmk, command, error;
    QByteArray data = socket->readAll();
    if (parseRequest(data, length, rmk, command, error, data))
    {
        switch (command)
        {
        case 0:
            if (!error)
                emit connectionResult(true);
            break;

        case 101:
            emit information(new Information(conv("Перезапуск сервера")));
            break;

        case 102:
            emit restartResult(error, data);
            break;
        }
    }
    else
    {
        emit information(new Exception(conv("Ошибка разбора ответа от сервера"), 301));
        emit parseError();
    }
}

void ServerClient::error(QAbstractSocket::SocketError error)
{
    emit information(new Exception(conv("Ошибка при запросе к серверу"), conv("Ошибка протокола %1: %2").arg(socket->error()).arg(socket->errorString()), 301));
    emit connectionResult(false);
}
