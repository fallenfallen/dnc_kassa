#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QFile>
#include <QMutex>
#include <QDateTime>
#include <QTextStream>
#include <QMutexLocker>

#include "translate.h"

#include <QDebug>

class Logger
{
public:
    static void log(const QString &text)
    {
        QMutexLocker locker(mutex);

        QString fileName = "/var/log/dnc/eds.log";
        if (access(fileName.toAscii(), W_OK) == - 1)
            fileName = "/tmp/dnc/eds.log";

        QFile file(fileName);
        if (file.open(QIODevice::Append | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream << text;
        }
        file.close();
    }

public:
    static QMutex * mutex;

};

class Information
{
public:
    Information(const QString &message) : informationMessage(message) {}

    QString &message() { return informationMessage; }

    virtual void print()
    {
        QString text;
        text.append(conv("%1 %2\n").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")).arg(message()));

        QTextStream out(stdout);
        out << text;

        Logger::log(text);
    }

private:
    QString informationMessage;

};

class Exception : public Information
{
public:
    Exception(const QString &message, int number) : Information(message),
            errorNumber(number) {}
    Exception(const QString &message, const QString &description, int number) : Information(message),
            errorDescription(description), errorNumber(number) {}

    QString &description() { return errorDescription; }
    QString number() { return QString::number(errorNumber); }

    virtual void print()
    {
        QString text;
        text.append(conv("%1 Ошибка: %2 Номер ошибки: %3\n").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")).arg(message()).arg(number()));
        if (!description().isEmpty())
            text.append(conv("Описание ошибки: %1\n").arg(description()));

        QTextStream out(stdout);
        out << text;

        Logger::log(text);
    }

private:
    QString errorDescription;
    int errorNumber;

};

class Warning : public Information
{
public:
    Warning(const QString &message) : Information(message) {}
    Warning(const QString &message, const QString &reason) : Information(message), warningReason(reason) {}

    QString &reason() { return warningReason; }

    virtual void print()
    {
        QString text;
        text.append(conv("%1 Внимание: %2\n").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")).arg(message()));
        if (!reason().isEmpty())
            text.append(conv("Действия, вызвавшие ситуацию: %1\n").arg(reason()));

        QTextStream out(stdout);
        out << text;

        Logger::log(text);
    }

private:
    QString warningReason;

};

#endif // EXCEPTION_H
