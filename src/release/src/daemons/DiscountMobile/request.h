#ifndef REQUEST_H
#define REQUEST_H

#include <QUrl>
#include <QUuid>

class Request
{
public:
    Request(QUuid id, int rmk, int cmd, const QByteArray &xml, const QUrl &url) :
            id(id), rmk(rmk), cmd(cmd), xml(xml), url(url), attemptCount(0) {}

    void newAttempt()
    {
        attemptCount++;
    }

    QUrl url;
    QUuid id;
    QByteArray xml;
    int rmk, cmd;
    int attemptCount;

};

#endif // REQUEST_H
