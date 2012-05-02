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

#include "error.h"
#include "model.h"
#include "translator.h"

#include <QDebug>

Error::Error(Model * model) : QObject()
{
    this->model = model;
}

Error::~Error() {}

int Error::analizeError(int result, const QString &funcName)
{
    qDebug() << "Analize error(): " << result;
    if (result != 0)
    {
        if (errorOutOfPaper(result))
        {
            int request = - 1;
            emit requestContinuePrint(&request);

            if (!request)
            {
                switch (model->protocol())
                {
                case 1:
                    result = repeatBreakOperation();
                    if (!result)
                        result = 1;
                    break;

                case 3:
                    if (!funcName.compare("report") || !funcName.compare("check") ||
                        !funcName.compare("fiscalization"))
                        result = repeatBreakOperation();
                    else
                        result = 1;
                    break;

                default:
                    result = 1;
                    break;
                }
            }
            else if (request != - 1)
                result = 0;
        }
        else if (errorCancelCheck(result))
        {
            result = model->cancelCheck();
            if (!result)
                result = 1;
        }
    }

    return result;
}

int Error::repeatBreakOperation()
{
    return model->continuePrinting();
}

bool Error::errorOutOfPaper(int result)
{
    return result == 103 || result == 277;
}

bool Error::errorCancelCheck(int result)
{
    return result == 114;
}
