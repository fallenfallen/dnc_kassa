#include "datamanager.h"
#include "databaseengine.h"

#include <QRegExp>
#include <QStringList>

DataManager::DataManager() : QObject()
{
    initValuesFromDatabase();
}

DataManager::~DataManager() {}

void DataManager::initValuesFromDatabase()
{
    dinc = DatabaseLoader::memberID();
}

void DataManager::sendError(QUuid id, int rmk, int cmd, int error, const QString &description)
{
    QByteArray reply = createServerReply(rmk, cmd, error, description);
    emit preparedReply(id, reply);
}

void DataManager::parseReply(QUuid id, int rmk, int cmd, const QByteArray &data)
{
    int result;

    QList<User *> list;
    QXmlStreamReader reader(data);
    QString command, description;
    while (!reader.atEnd())
    {
        reader.readNext();

        QStringRef name = reader.name();
        if (!name.compare("cmd", Qt::CaseInsensitive))
        {
            command = reader.readElementText();
            if (!command.compare("init", Qt::CaseInsensitive))
                 list = parseInitReply(reader);
        }
        if (!name.compare("checkdin", Qt::CaseInsensitive) ||
            !name.compare("buy", Qt::CaseInsensitive))
        {
            command = "result";
            parseResult(reader, result, description);
        }
    }

    if (!reader.hasError())
    {
        QByteArray reply;
        if (!command.compare("init", Qt::CaseInsensitive))
            reply = createServerReply(rmk, cmd, list);
        else if (!command.compare("result", Qt::CaseInsensitive))
            reply = createServerReply(rmk, cmd, result, description);

        emit preparedReply(id, reply);
    }
    else
        emit information(new Exception(conv("Ошибка разбора ответа скидочной системы."), conv("Возможно, струтура имеет некорректный формат."), 301));

    while (!list.isEmpty())
        delete list.takeFirst();
}

void DataManager::parseResult(QXmlStreamReader &reader, int &error, QString &description)
{
    while (!reader.atEnd())
    {
        reader.readNext();

        QStringRef name = reader.name();
        if (!name.compare("resultcode", Qt::CaseInsensitive))
            error = reader.readElementText().toInt();
        else if (!name.compare("resultstring", Qt::CaseInsensitive))
            description = reader.readElementText().toUtf8();
    }
}

QList<User *> DataManager::parseInitReply(QXmlStreamReader &reader)
{
    QList<User *> list;

    while (!reader.atEnd())
    {
        reader.readNext();

        QStringRef name = reader.name();
        if (!name.compare("retcode", Qt::CaseInsensitive))
        {
            QString value = reader.readElementText();
            for (int i = 0; i < value.toInt(); ++i)
                list << parseUserData(reader);
        }
    }

    return list;
}

User * DataManager::parseUserData(QXmlStreamReader &reader)
{
    User * user = new User;

    while (!reader.atEnd())
    {
        reader.readNext();

        QStringRef name = reader.name();
        if (!name.compare("dinu", Qt::CaseInsensitive))
            user->ID = reader.readElementText().toInt();
        else if (!name.compare("discount_id", Qt::CaseInsensitive))
            user->discountID = reader.readElementText().toInt();
        else if (!name.compare("fname", Qt::CaseInsensitive))
            user->firstName = reader.readElementText().toUtf8();
        else if (!name.compare("mname", Qt::CaseInsensitive))
            user->middleName = reader.readElementText().toUtf8();
        else if (!name.compare("lname", Qt::CaseInsensitive))
            user->lastName = reader.readElementText().toUtf8();
        else if (!name.compare("proc_dscnt", Qt::CaseInsensitive))
            user->discountPercent = reader.readElementText().toInt();
        else if (!name.compare("bonus_points", Qt::CaseInsensitive))
            user->bonusPoints = reader.readElementText().toInt();
        else if (!name.compare("sum_dscnt", Qt::CaseInsensitive))
            user->discountSum = reader.readElementText().toInt();
        else if (!name.compare("gift_count", Qt::CaseInsensitive))
        {
            int count = reader.readElementText().toInt();
            for (int i = 0; i < count; ++i)
                user->gifts << parseUserGifts(reader);
        }
        else if (!name.compare("user", Qt::CaseInsensitive) && !reader.isStartElement())
            return user;
    }
}

Gift DataManager::parseUserGifts(QXmlStreamReader &reader)
{
    Gift gift;
    while (!reader.atEnd())
    {
        reader.readNext();

        QStringRef name = reader.name();
        if (!name.compare("gift_id", Qt::CaseInsensitive))
            gift.ID = reader.readElementText().toInt();
        else if (!name.compare("gift_type", Qt::CaseInsensitive))
            gift.type = reader.readElementText().toInt();
        else if (!name.compare("gift_name", Qt::CaseInsensitive))
            gift.name = reader.readElementText().toUtf8();
        else if (!name.compare("gift_note", Qt::CaseInsensitive))
            gift.note = reader.readElementText().toUtf8();
        else if (!name.compare("gift_weigth", Qt::CaseInsensitive))
            gift.weight = reader.readElementText().toInt();
        else if (!name.compare("gift", Qt::CaseInsensitive) && !reader.isStartElement())
            return gift;
    }
}

bool DataManager::parseRequest(const QString &reqeuest, int &length, int &rmk, int &cmd, int &error, QByteArray &data)
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

void DataManager::parseCommand(QUuid id, const QByteArray &cmd)
{
    QByteArray data;
    int length, rmk, command, error;
    if (parseRequest(cmd, length, rmk, command, error, data))
    {
        switch (command)
        {
        case 0:
            {
                QByteArray reply = createServerReply(rmk, command);
                if (rmk != 0)
                    emit assosiateSession(rmk, id);
                emit preparedReply(id, reply);
            }
            break;

        case 1:
            {
                QList<Pair> map;

                data = data.left(data.length() - 1);
                map.append(Pair("dinc", QString::number(dinc)));
                map.append(Pair("rqcode", data));
                QByteArray xml = createXMLRequest("init", map);
                emit preparedRequest(id, rmk, command, xml);
            }
            break;

        case 2:
            {
                QByteArray reply = createServerReply(rmk, command);
                emit preparedReply(id, reply);

                QList<Pair> map;
                QStringList list = QString(data).split(';');
                map.append(Pair("discount_id", list[0]));
                map.append(Pair("dinc",QString::number(dinc)));
                map.append(Pair("dinu", list[1]));
                map.append(Pair("dink", list[2]));
                map.append(Pair("buy_summ", list[3]));
                map.append(Pair("summ_dscnt", list[4]));
                map.append(Pair("bonus", list[5]));
                map.append(Pair("payment", list[6]));
                map.append(Pair("gift_count", list[7]));
                if (list[7] != "0")
                {
                    map.append(Pair("start", "gift"));
                    for (int i = 8; i < list.count(); i += 2)
                    {
                        map.append(Pair("gift_id", list[i]));
                        map.append(Pair("gift_weigth", list[i + 1]));
                    }
                    map.append(Pair("end", "gift"));
                }
                QByteArray xml = createXMLRequest("buy", map);

                emit preparedRequest(id, rmk, command, xml);
            }
            break;

        case 100:
            {
                QByteArray reply = createServerReply(rmk, 101);
                emit preparedReply(id, reply);
                emit restart(id, rmk, 102);

            }
            break;

        default:
            emit information(new Warning(conv("Неизвестная команда.")));
        }
    }
    else
        emit information(new Warning(conv("Неизвестная структура команды.")));
}

void DataManager::serverRestarted(QUuid id, int rmk, int cmd, int error, const QString &description)
{
    QByteArray reply = createServerReply(rmk, cmd, error, description);
    emit preparedReply(id, reply);
}

QByteArray DataManager::createServerReply(int rmk, int cmd)
{
    QByteArray result;

    result.append(QString(";%1;%2;0;").arg(rmk).arg(cmd));
    int length = result.length() - 1;
    result.prepend(QByteArray::number(length));

    return result;
}

QByteArray DataManager::createServerReply(int rmk, int cmd, int error, const QString &description)
{
    QByteArray result;

    result.append(QString(";%1;%2;%3;").arg(rmk).arg(cmd).arg(error));
    if (error != 0)
        result.append(description + ";");
    int length = result.length() - 1;
    result.prepend(QByteArray::number(length));

    return result;
}

QByteArray DataManager::createServerReply(int rmk, int cmd, const QList<User *> &data)
{
    QByteArray result;

    result.append(QString(";%1;%2;0;%3;").arg(rmk).arg(cmd).arg(data.length()));
    foreach (User * user, data)
    {
        result.append(QString("%1;%2;%3;%4;%5;%6;%7;%8;%9;").arg(user->ID).arg(user->discountID).arg(user->firstName).arg(user->middleName).arg(user->lastName).arg(
                user->discountPercent).arg(user->bonusPoints).arg(user->discountSum).arg(user->gifts.count()));
        if (!user->gifts.isEmpty())
        {
            foreach (Gift gift, user->gifts)
                result.append(QString("%1;%2;%3;%4;%5;").arg(gift.ID).arg(gift.type).arg(gift.name).arg(gift.note).arg(gift.weight));
        }
    }

    int length = result.length() - 1;
    result.prepend(QByteArray::number(length));

    return result;
}

QByteArray DataManager::createXMLRequest(const QString &command, const QList<Pair> &data)
{
    QByteArray xml;
    QXmlStreamWriter writer(&xml);
    writer.writeStartDocument("1.0");
    writer.writeStartElement("dataset");
    writer.writeTextElement("cmd", command);
    writer.writeStartElement("session");

    for (QList<Pair>::const_iterator iter = data.begin(); iter != data.end(); iter++)
    {
        if (iter->first == "start")
            writer.writeStartElement(iter->second);
        else if (iter->first == "end")
            writer.writeEndElement();
        else
            writer.writeTextElement(iter->first, iter->second);
    }

    writer.writeEndElement();
    writer.writeEndElement();
    writer.writeEndDocument();

    return xml;
}
