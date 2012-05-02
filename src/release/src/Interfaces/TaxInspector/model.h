#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QTextCodec>

#include "error.h"
#include "hwserver.h"
#include "translator.h"

class Model : public QObject
{
    Q_OBJECT

public:
    Model(QObject * parent = 0);
    ~Model();

    void connect();
    void connect(const ConnectOptions * options);
    bool checkConnection();
    virtual int makeBriefReport(int dayBegin, int monthBegin, int yearBegin,
                                int dayEnd, int monthEnd, int yearEnd, QString password = QString::null) = 0;
    virtual int makeBriefReport(int sessionBegin, int sessionEnd, QString password = QString::null) = 0;
    virtual int makeFullReport(int dayBegin, int monthBegin, int yearBegin,
                               int dayEnd, int monthEnd, int yearEnd, QString password = QString::null) = 0;
    virtual int makeFullReport(int sessionBegin, int sessionEnd, QString password = QString::null) = 0;

    int cancelCheck();
    bool extraCheck(int mode);
    int continuePrinting();
    void waitForRelease();
    int protocol();

signals:
    void signalFail(const QString &text);
    void requestContinuePrint(int * request);

protected:
    int getStatus(int &mode, int &advancedMode);
    void fillString(QString &str, int length);
    QString generateErrorMessage(const QString &text, int number);
    int getPasswordLength(const QString &str);
    int getRegistrationNumberLength();
    int getINNNumberLength();
    int getDocumentNumberLength();

    Error * err;
    HWServer * hw;
    int connectionResult;

    QTextCodec * codec;
};

#endif // MODEL_H
