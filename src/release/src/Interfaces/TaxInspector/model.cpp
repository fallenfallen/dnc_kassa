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

#include "model.h"

#include <QDebug>

Model::Model(QObject * parent) : QObject(parent)
{
    err = new Error(this);
    codec = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
    QObject::connect(err, SIGNAL(requestContinuePrint(int *)), SIGNAL(requestContinuePrint(int *)));

    connect();
}

Model::~Model()
{
    hw->disconnect();
}

void Model::connect()
{
    connectionResult = hw->connect();
}

void Model::connect(const ConnectOptions * options)
{
    connectionResult = hw->connect(options);
}

QString Model::generateErrorMessage(const QString &text, int number)
{
    hw->hw()->ECR_ReadError(number);
    QString result = text + " " + codec->toUnicode(hw->hw()->ECR_GetErrDescription()) + conv(". Код ошибки ") + QString::number(number);

    return result;
}

int Model::protocol()
{
    return hw->protocol();
}

int Model::continuePrinting()
{
    return hw->hw()->ECR_ContinuePrinting();
}

int Model::cancelCheck()
{
    return hw->hw()->ECR_CancelCheck();
}

int Model::getPasswordLength(const QString &str)
{
    switch (protocol())
    {
    case 1: return 8;
    case 2: return 8;
    case 3: return str.length() <= 5 ? str.length() : 5;
    case 4: return 6;
    case 5: return 16;
    }
}

int Model::getRegistrationNumberLength()
{
    switch (protocol())
    {
    case 1: return 10;
    case 2: return 10;
    case 3: return 8;
    case 4: return 12;
    case 5: return 10;
    }
}

int Model::getINNNumberLength()
{
    return 12;
}

int Model::getDocumentNumberLength()
{
    return 8;
}

void Model::fillString(QString &str, int length)
{
    if (str.length() < length)
    {
        while (str.length() < length)
            str.prepend("0");
    }
    else
        str = str.left(length);
}

bool Model::checkConnection()
{
    if (connectionResult == - 1)
        emit signalFail(generateErrorMessage(conv("Невозможно подлючиться к библиотеке HWSRV."), connectionResult));

    return connectionResult == 0;
}

int Model::getStatus(int &mode, int &advancedMode)
{
    int result = hw->hw()->ECR_ReadStatus();
    if (!result)
    {
        mode = hw->hw()->ECR_GetMode();
        advancedMode = hw->hw()->ECR_GetAdvancedMode();
    }

    return result;
}

bool Model::extraCheck(int mode)
{
    if (protocol() == 1)
        return mode == 11 || mode == 12;

    return false;
}

void Model::waitForRelease()
{
    int mode, advancedMode;
    do
    {
        usleep(250);
        getStatus(mode, advancedMode);
    }
    while (advancedMode != 0 || extraCheck(mode));
}
