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

#include "eklsmodel.h"

EKLSModel::EKLSModel(QObject * parent) : Model(parent) {}

int EKLSModel::activation()
{
    int result = hw->hw()->ECR_SetMode(0);
    if (!result)
    {
        result = hw->hw()->ECR_SetMode(6);
        if (!result)
            result = hw->hw()->ECR_EKLZActivization();
    }

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        break;

    case 1:
        waitForRelease();
        result = activation();
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка активизации ЭКЛЗ."), result));
    }

    return result;
}

int EKLSModel::closeArchive()
{
    int result = hw->hw()->ECR_SetMode(0);
    if (!result)
    {
        result = hw->hw()->ECR_SetMode(6);
        if (!result)
            result = hw->hw()->ECR_EKLZCloseArchive();
    }

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        break;

    case 1:
        waitForRelease();
        result = closeArchive();
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка закрытия архива ЭКЛЗ."), result));
    }

    return result;
}

int EKLSModel::activationResult()
{
    int result = hw->hw()->ECR_SetMode(0);
    if (!result)
    {
        result = hw->hw()->ECR_SetMode(6);
        if (!result)
            result = hw->hw()->ECR_ReportEKLZOutcomeActivization();
    }

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        break;

    case 1:
        waitForRelease();
        result = activationResult();
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка печати отчета об активизации."), result));
    }

    return result;
}

int EKLSModel::printShift(int number)
{
    int result = hw->hw()->ECR_SetMode(0);
    if (!result)
    {
        result = hw->hw()->ECR_SetMode(6);
        if (!result)
            result = hw->hw()->ECR_ReportEKLZCheckBeltForSession(number);
    }

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        break;

    case 1:
        waitForRelease();
        result = printShift(number);
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка печати контрольной ленты за смену ЭКЛЗ."), result));
    }

    return result;
}

int EKLSModel::printShiftFromEKLS(int number)
{
    int result = hw->hw()->ECR_SetMode(0);
    if (!result)
    {
        result = hw->hw()->ECR_SetMode(6);
        if (!result)
            result = hw->hw()->ECR_ReportEKLZOutcomeSessionByNumSession(number);
    }

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        break;

    case 1:
        waitForRelease();
        result = printShiftFromEKLS(number);
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка печати итогов смены из ЭКЛЗ."), result));
    }

    return result;
}

int EKLSModel::printByNumber(QString number)
{
    fillString(number, getDocumentNumberLength());

    int result = hw->hw()->ECR_SetMode(0);
    if (!result)
    {
        result = hw->hw()->ECR_SetMode(6);
        if (!result)
            result = hw->hw()->ECR_EKLZPrintDocByKPK(codec->fromUnicode(number));
    }

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        break;

    case 1:
        waitForRelease();
        result = printByNumber(number);
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка печати документа по криптографическому первичному коду."), result));
    }

    return result;
}

int EKLSModel::makeBriefReport(int dayBegin, int monthBegin, int yearBegin, int dayEnd, int monthEnd, int yearEnd, QString password)
{
    int result = hw->hw()->ECR_SetMode(0);
    if (!result)
    {
        result = hw->hw()->ECR_SetMode(6);
        if (!result)
            result = hw->hw()->ECR_ReportEKLZBySessionInTheRangeOfDates(dayBegin, monthBegin, yearBegin, dayEnd, monthEnd, yearEnd, 0);
    }

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        break;

    case 1:
        waitForRelease();
        result = makeBriefReport(dayBegin, monthBegin, yearBegin, dayEnd, monthEnd, yearEnd, password);
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка снятия краткого отчета ЭКЛЗ по датам."), result));
    }

    return result;
}

int EKLSModel::makeFullReport(int dayBegin, int monthBegin, int yearBegin, int dayEnd, int monthEnd, int yearEnd, QString password)
{
    int result = hw->hw()->ECR_SetMode(0);
    if (!result)
    {
        result = hw->hw()->ECR_SetMode(6);
        if (!result)
            result = hw->hw()->ECR_ReportEKLZBySessionInTheRangeOfDates(dayBegin, monthBegin, yearBegin, dayEnd, monthEnd, yearEnd, 1);
    }

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        break;

    case 1:
        waitForRelease();
        result = makeFullReport(dayBegin, monthBegin, yearBegin, dayEnd, monthEnd, yearEnd, password);
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка снятия полного отчета ЭКЛЗ по датам."), result));
    }

    return result;
}

int EKLSModel::makeBriefReport(int sessionBegin, int sessionEnd, QString password)
{
    int result = hw->hw()->ECR_SetMode(0);
    if (!result)
    {
        result = hw->hw()->ECR_SetMode(6);
        if (!result)
            result = hw->hw()->ECR_ReportEKLZBySessionInTheRangeOfNumSessions(sessionBegin, sessionEnd, 0);
    }

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        break;

    case 1:
        waitForRelease();
        result = makeBriefReport(sessionBegin, sessionEnd, password);
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка снятия краткого отчета ЭКЛЗ по сменам."), result));
    }

    return result;
}

int EKLSModel::makeFullReport(int sessionBegin, int sessionEnd, QString password)
{
    int result = hw->hw()->ECR_SetMode(0);
    if (!result)
    {
        result = hw->hw()->ECR_SetMode(6);
        if (!result)
            result = hw->hw()->ECR_ReportEKLZBySessionInTheRangeOfNumSessions(sessionBegin, sessionEnd, 1);
    }

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        break;

    case 1:
        waitForRelease();
        result = makeFullReport(sessionBegin, sessionEnd, password);
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка снятия полного отчета ЭКЛЗ по сменам."), result));
    }

    return result;
}

int EKLSModel::breakPrint()
{
    int result = hw->hw()->ECR_SetMode(6);
    if (!result)
        result = hw->hw()->ECR_InrettupEKLZReport();

    result = err->analizeError(result);
    switch (result)
    {
    case 0:
        break;

    case 1:
        waitForRelease();
        result = breakPrint();
        break;

    default:
        emit signalFail(generateErrorMessage(conv("Ошибка прерывания печати отчета."), result));
    }

    return result;
}
