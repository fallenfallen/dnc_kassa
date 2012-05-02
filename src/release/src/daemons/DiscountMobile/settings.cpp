#include "settings.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>

QString Settings::fileName = "/etc/dancy/dnc_eds.conf";

bool Settings::readConfig(QString &name, QString &host, int &port, QString &user, QString &password)
{
    bool result = false;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString text = file.readAll();

        if (parseFile(text, name, host, port, user, password))
            result = true;

        file.close();
    }

    return result;
}

bool Settings::writeConfig(const QString &name, const QString &host, int port, const QString &user, const QString &password)
{
    bool result = true;
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << "[DB] " << name << "\n";
        stream << "[HOST] " << host << "\n";
        stream << "[PORT] " << port << "\n";
        stream << "[USER] " << user << "\n";
        stream << "[PASS] " << password << "\n";
        file.close();
    }
    else
        result = false;

    return result;
}

bool Settings::parseFile(const QString &text, QString &name, QString &host, int &port, QString &user, QString &password)
{
    QStringList lines = text.split('\n');
    foreach (QString line, lines)
    {
        if (line.contains("[DB]", Qt::CaseInsensitive))
            name = parseLine("[DB]", line).toString();
        if (line.contains("[HOST]", Qt::CaseInsensitive))
            host = parseLine("[HOST]", line).toString();
        if (line.contains("[PORT]", Qt::CaseInsensitive))
            port = parseLine("[PORT]", line).toInt();
        if (line.contains("[USER]", Qt::CaseInsensitive))
            user = parseLine("[USER]", line).toString();
        if (line.contains("[PASS]", Qt::CaseInsensitive))
            password = parseLine("[PASS]", line).toString();
    }
}

QVariant Settings::parseLine(const QString &label, const QString &line)
{
    return line.right(line.length() - label.length()).trimmed();
}
