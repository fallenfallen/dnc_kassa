#ifndef EKLSMODEL_H
#define EKLSMODEL_H

#include "model.h"
#include "translator.h"

class EKLSModel : public Model
{
    Q_OBJECT

public:
    EKLSModel(QObject * parent = 0);

    int activation();
    int closeArchive();
    int activationResult();
    int printShift(int number);
    int printShiftFromEKLS(int number);
    int printByNumber(QString number);
    virtual int makeBriefReport(int dayBegin, int monthBegin, int yearBegin,
                        int dayEnd, int monthEnd, int yearEnd, QString password = QString::null);
    virtual int makeBriefReport(int sessionBegin, int sessionEnd, QString password = QString::null);
    virtual int makeFullReport(int dayBegin, int monthBegin, int yearBegin,
                       int dayEnd, int monthEnd, int yearEnd, QString password = QString::null);
    virtual int makeFullReport(int sessionBegin, int sessionEnd, QString password = QString::null);
    int breakPrint();

};

#endif // EKLSMODEL_H
