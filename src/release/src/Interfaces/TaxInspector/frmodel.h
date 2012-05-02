#ifndef FRMODEL_H
#define FRMODEL_H

#include "model.h"
#include "frparam.h"
#include "translator.h"

class FRModel : public Model
{
    Q_OBJECT

public:
    FRModel(QObject * parent = 0);

    int initTestMode();
    void updateInformation();
    int setDateTime(int day, int month, int year, int hour, int minute, int second);
    int setSerialNumber(const QString &number, QString password);
    int setLicence(const QString &license);
    int fiscalization(QString registrationNumber, QString INN, QString newPassword, QString password);
    virtual int makeBriefReport(int dayBegin, int monthBegin, int yearBegin,
                        int dayEnd, int monthEnd, int yearEnd, QString password = QString::null);
    virtual int makeBriefReport(int sessionBegin, int sessionEnd, QString password = QString::null);
    virtual int makeFullReport(int dayBegin, int monthBegin, int yearBegin,
                       int dayEnd, int monthEnd, int yearEnd, QString password = QString::null);
    virtual int makeFullReport(int sessionBegin, int sessionEnd, QString password = QString::null);
    int testSale(double price);
    int report();

signals:
    void signalUpdateTable(const FRParam * param);

};

#endif // FRMODEL_H
