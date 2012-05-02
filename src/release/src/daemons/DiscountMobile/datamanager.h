#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QHash>
#include <QUuid>
#include <QXmlStreamReader>

#include "exception.h"

typedef QPair<QString, QString> Pair;

struct Gift
{
    Gift()
    {
        ID = type = weight = 0;
        name = note = "";
    }

    int ID, type;
    int weight;
    QString name, note;
};

struct User
{
    User()
    {
        ID = discountID = 0;
        firstName = middleName = lastName = "";
        discountPercent = discountSum = bonusPoints = 0;
    }

    int ID, discountID;
    QString firstName, middleName, lastName;
    int discountPercent, discountSum;
    int bonusPoints;
    QList<Gift> gifts;
};

class DataManager : public QObject
{
    Q_OBJECT

public:
    DataManager();
    ~DataManager();

    void initValuesFromDatabase();

signals:
    void information(Information * information);

    void restart(QUuid id, int rmk, int cmd);
    void preparedRequest(QUuid id, int rmk, int cmd, const QByteArray &xml);
    void preparedReply(QUuid id, const QByteArray &reply);
    void assosiateSession(int rmk, QUuid &id);

public slots:
    void serverRestarted(QUuid id, int rmk, int cmd, int error, const QString &description);
    void parseCommand(QUuid id, const QByteArray &cmd);
    void parseReply(QUuid id, int rmk, int cmd, const QByteArray &data);
    void sendError(QUuid id, int rmk, int cmd, int error, const QString &description);

private:
    QByteArray createXMLRequest(const QString &command, const QList<Pair> &data);
    QByteArray createServerReply(int rmk, int cmd);
    QByteArray createServerReply(int rmk, int cmd, const QList<User *> &data);
    QByteArray createServerReply(int rmk, int cmd, int error, const QString &description);
    bool parseRequest(const QString &reqeuest, int &length, int &rmk, int &cmd, int &error, QByteArray &data);

    int dinc;
    void parseResult(QXmlStreamReader &reader, int &error, QString &description);
    QList<User *> parseInitReply(QXmlStreamReader &reader);
    User * parseUserData(QXmlStreamReader &reader);
    Gift parseUserGifts(QXmlStreamReader &reader);

};

#endif // DATAMANAGER_H
