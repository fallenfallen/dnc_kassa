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

#include "hwserver.h"

HWSRV * HWServer::m_hw = NULL;
int HWServer::m_protocol = 0;

int HWServer::connect()
{
    int result = 0;
    if (hw() == NULL)
    {
        m_hw = new HWSRV();
        result = hw()->ECR_ReadStatus();
        if (!result)
        {
            result = hw()->ECR_GetType();
            if (!result)
            {
                result = - 1;
                m_hw = NULL;
            }
            else
            {
                m_protocol = hw()->ECR_GetProtocol();
                result = 0;
            }
        }
        else
            m_hw = NULL;
    }

    return result;
}

int HWServer::connect(const ConnectOptions * options)
{
    int result = 0;

    if (hw() == NULL)
    {
        m_hw = new HWSRV(1);

        result = hw()->ECR_SetConnectOptions(options->portType,
                                             options->logicalNumber,
                                             options->port,
                                             options->baudRate,
                                             options->protocol,
                                             true,
                                             options->accessPassword,
                                             options->password,
                                             options->checkPrinter);
        if (!result)
        {
            result = hw()->ECR_ReadStatus();
            if (!result)
            {
                result = hw()->ECR_GetType();
                if (!result)
                {
                    result = - 1;
                    m_hw = NULL;
                }
                else
                {
                    m_protocol = hw()->ECR_GetProtocol();
                    result = 0;
                }
            }
            else
                m_hw = NULL;
        }
        else
            m_hw = NULL;

    }

    return result;
}

void HWServer::disconnect()
{
    if (hw() != NULL)
    {
       hw()->ECR_CloseConnection();
       delete m_hw;
       m_hw = NULL;
    }
}

HWSRV * HWServer::hw()
{
    return m_hw;
}

int HWServer::protocol()
{
    return m_protocol;
}
