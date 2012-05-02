/*====================================================================================
# «ДЭНСИ:Налоговый инспектор» module of «ДЭНСИ: КАССА» complex is an additional utility, wich allows to use FM/ECRP functions.
# © OOO "DNC Trade" 2011, Authors: Romanow Alexey.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:Налоговый инспектор» программного комплекса «ДЭНСИ: КАССА» - дополнительная утилита, предоставляющая доступ к функциям работы с ФП/ЭКЛЗ. 
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Романов Алексей.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "frmodel.h"

#include <QDebug>

#define Z_REPORT 1
#define BRIEF_REPORT_BY_DATE 3
#define FULL_REPORT_BY_DATE 5
#define BRIEF_REPORT_BY_SESSION 4
#define FULL_REPORT_BY_SESSION 6

FRModel::FRModel(QObject * parent) : Model(parent) {}

void FRModel::updateInformation()
{
    hw->hw()->ECR_ReadStatus();
    FRParam * param = new FRParam;
    strcpy(param->serialNumber, hw->hw()->ECR_GetSerialNumber());
    strcpy(param->ROMVersion, hw->hw()->ECR_GetROMVersion());
    strcpy(param->description, hw->hw()->ECR_GetUDescription());
    param->day = hw->hw()->ECR_GetDay();
    param->mounth = hw->hw()->ECR_GetMonth();
    param->year = 2000 + hw->hw()->ECR_GetYear();
    param->hour = hw->hw()->ECR_GetHour();
    param->minute = hw->hw()->ECR_GetMinute();
    param->second = hw->hw()->ECR_GetSecond();
    param->fiscal = hw->hw()->ECR_GetFiscal();
    param->sessionOpened = hw->hw()->ECR_GetSessionOpened();
    param->protocol = protocol();
    param->license = hw->hw()->ECR_GetLicenseIsPresent();
    param->mode = hw->hw()->ECR_GetMode();
    param->advancedMode = hw->hw()->ECR_GetAdvancedMode();
    emit signalUpdateTable(param);
    delete param;
}

int FRModel::initTestMode()
{
    return hw->hw()->ECR_EKLZInitArchive();
}

int FRModel::setDateTime(int day, int month, int year, int hour, int minute, int second)
{   
    int result = hw->hw()->ECR_SetMode(0);
    if (!result)
    {
        result = hw->hw()->ECR_SetDate(day, month, year);
        if (!result)
            result = hw->hw()->ECR_SetTime(hour, minute, second);
    }

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        updateInformation();
        break;

    case 1:
        waitForRelease();
        result = setDateTime(day, month, year, hour, minute, second);
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка синхронизации даты и времени."), result));
    }

    return result;
}

int FRModel::setSerialNumber(const QString &number, QString password)
{
    fillString(password, getPasswordLength(password));

    int result = hw->hw()->ECR_SetMode(0);
    if (result == 0 || result == 157)
    {
        result = hw->hw()->ECR_SetMode(5, codec->fromUnicode(password));
        if (!result)
            result = hw->hw()->ECR_SetSerialNumber(codec->fromUnicode(number));
    }

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        updateInformation();
        break;

    case 1:
        waitForRelease();
        result = setSerialNumber(number, password);
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка установки серийного номера."), result));
    }

    return result;
}

int FRModel::fiscalization(QString registrationNumber, QString INN, QString newPassword, QString password)
{
    fillString(password, getPasswordLength(password));
    fillString(newPassword, getPasswordLength(password));
    fillString(registrationNumber, getRegistrationNumberLength());
    fillString(INN, getINNNumberLength());

    int result = hw->hw()->ECR_SetMode(0);
    if (result == 0 || result == 157)
    {
        result = hw->hw()->ECR_SetMode(5, codec->fromUnicode(password));
        if (!result)
           result = hw->hw()->ECR_Fiscalization(codec->fromUnicode(registrationNumber), codec->fromUnicode(INN), codec->fromUnicode(newPassword));
    }

    result = err->analizeError(result, "fiscalization");
    switch (result)
    {
    case 0:
        updateInformation();
        break;

    case 1:
        waitForRelease();
        result = fiscalization(registrationNumber, INN, newPassword, password);
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка фискализации."), result));
    }

    return result;
}

int FRModel::makeBriefReport(int dayBegin, int monthBegin, int yearBegin, int dayEnd, int monthEnd, int yearEnd, QString password)
{
    fillString(password, getPasswordLength(password));

    int result = hw->hw()->ECR_SetMode(0);
    if (result == 0 || result == 157)
    {
        result = hw->hw()->ECR_SetMode(5, codec->fromUnicode(password));
        if (!result)
            result = hw->hw()->ECR_Report(BRIEF_REPORT_BY_DATE, dayBegin, monthBegin, yearBegin, dayEnd, monthEnd, yearEnd);
    }

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        updateInformation();
        break;

    case 1:
        waitForRelease();
        result = makeBriefReport(dayBegin, monthBegin, yearBegin, dayEnd, monthEnd, yearEnd, password);
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка снятия краткого отчета по датам."), result));
    }

    return result;
}

int FRModel::makeFullReport(int dayBegin, int monthBegin, int yearBegin, int dayEnd, int monthEnd, int yearEnd, QString password)
{
    fillString(password, getPasswordLength(password));

    int result = hw->hw()->ECR_SetMode(0);
    if (result == 0 || result == 157)
    {
        result = hw->hw()->ECR_SetMode(5, codec->fromUnicode(password));
        if (!result)
            result = hw->hw()->ECR_Report(FULL_REPORT_BY_DATE, dayBegin, monthBegin, yearBegin, dayEnd, monthEnd, yearEnd);
    }

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        updateInformation();
        break;

    case 1:
        waitForRelease();
        result = makeFullReport(dayBegin, monthBegin, yearBegin, dayEnd, monthEnd, yearEnd, password);
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка снятия полного отчета по датам."), result));
    }

    return result;
}

int FRModel::makeBriefReport(int sessionBegin, int sessionEnd, QString password)
{
    fillString(password, getPasswordLength(password));

    int result = hw->hw()->ECR_SetMode(0);
    if (result == 0 || result == 157)
    {
        result = hw->hw()->ECR_SetMode(5, codec->fromUnicode(password));
        if (!result)
            result = hw->hw()->ECR_Report(BRIEF_REPORT_BY_SESSION, 0, 0, 0, 0, 0, 0, sessionBegin, sessionEnd);
    }

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        updateInformation();
        break;

    case 1:
        waitForRelease();
        result = makeBriefReport(sessionBegin, sessionEnd, password);
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка снятия краткого отчета по сменам."), result));
    }

    return result;
}

int FRModel::makeFullReport(int sessionBegin, int sessionEnd, QString password)
{
    fillString(password, getPasswordLength(password));

    int result = hw->hw()->ECR_SetMode(0);
    if (result == 0 || result == 157)
    {
        result = hw->hw()->ECR_SetMode(5, codec->fromUnicode(password));
        if (!result)
            result = hw->hw()->ECR_Report(FULL_REPORT_BY_SESSION, 0, 0, 0, 0, 0, 0, sessionBegin, sessionEnd);
    }

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        updateInformation();
        break;

    case 1:
        waitForRelease();
        result = makeFullReport(sessionBegin, sessionEnd, password);
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка снятия полного отчета по сменам."), result));
    }

    return result;
}

int FRModel::testSale(double price)
{
    int result = hw->hw()->ECR_SetMode(0);
    if (!result)
    {
        result = hw->hw()->ECR_SetMode(1);
        if (!result)
        {
            result = hw->hw()->ECR_OpenCheck(0);
            if (!result)
            {
                result = hw->hw()->ECR_Registration(price, 1, "");
                if (!result)
                    result = hw->hw()->ECR_CloseCheck(price);
            }
        }
    }

    result = err->analizeError(result, "check");
    switch (result)
    {
    case 0:
        updateInformation();
        break;

    case 1:
        waitForRelease();
        result = testSale(price);
        break;

    default:
            emit signalFail(generateErrorMessage(conv("Ошибка печати чека."), result));
    }

    return result;
}

int FRModel::report()
{
    int result = hw->hw()->ECR_SetMode(0);
    if (!result)
    {
        result = hw->hw()->ECR_SetMode(3);
        if (!result)
            result = hw->hw()->ECR_Report(Z_REPORT);
    }

    result = err->analizeError(result, "report");
    switch (result)
    {
    case 0:
        updateInformation();
        break;

    case 1:
        waitForRelease();
        result = report();
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка создания z-отчета."), result));
    }

    return result;
}

int FRModel::setLicence(const QString &license)
{
    int result = hw->hw()->ECR_SetMode(0);
    if (!result)
        hw->hw()->ECR_SetLicense(4, codec->fromUnicode(license));

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        updateInformation();
        break;

    case 1:
        waitForRelease();
        result = setLicence(license);
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка установки лицензии."), result));
    }

    return result;
}
