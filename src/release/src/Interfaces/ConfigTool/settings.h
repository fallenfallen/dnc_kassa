#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QVariant>

class Settings
{
public:
    static bool readConfig(QString &name, QString &host, int &port, QString &user, QString &password);
    static bool writeConfig(const QString &name, const QString &host, int port, const QString &user, const QString &password);

private:
    static QString fileName;

    static bool parseFile(const QString &text, QString &name, QString &host, int &port, QString &user, QString &password);
    static QVariant parseLine(const QString &label, const QString &line);
};

#endif // SETTINGS_H
