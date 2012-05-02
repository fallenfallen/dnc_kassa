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

#include "searchdevice.h"

SearchDevice::SearchDevice(QObject * parent) : QObject(parent)
{
    initPortList();
    initProtocolList();
    initBaudRate();
}

SearchDevice::~SearchDevice() {}

bool SearchDevice::createSystemPorts()
{
    if (system("ls /dev | grep ttyS > /tmp/ports.list") == - 1)
        return false;

    if (system("ls /dev | grep ttyUSB >> /tmp/ports.list") == - 1)
        return false;

    if (system("ls /dev | grep ttyACM >> /tmp/ports.list") == - 1)
        return false;

    if (access("/tmp/ports.list", 4) != 0)
      return false;

    return true;
}

void SearchDevice::initPortList()
{
    m_portList.clear();

    if (createSystemPorts())
    {
        QFile file("/tmp/ports.list");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            int index;
            QString str = file.readAll();
            QStringList list = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            foreach (QString str, list)
                m_portList << setDeviceName(str);
        }
    }
}

QString SearchDevice::setDeviceName(QString deviceType)
{
    QString str;
    if (deviceType.contains("ttyS"))
    {
        int number = deviceType.right(deviceType.length() - 4).toInt() + 1;
        str = "COM" + QString::number(number);
    }
    else if (deviceType.contains("ttyUSB"))
    {
        int number = deviceType.right(deviceType.length() - 6).toInt() + 1;
        str = "USB<->COM" + QString::number(number);
    }
    else if (deviceType.contains("ttyACM"))
    {
        int number = deviceType.right(deviceType.length() - 6).toInt() + 1;
        str = "ACM" + QString::number(number);
    }

    return str;
}

QStringList SearchDevice::portList()
{
    return m_portList;
}

void SearchDevice::initProtocolList()
{
    foreach (int index, m_protocolList.keys())
        m_protocolList.values(index).clear();
    m_protocolList.clear();

    QMap<QString, int> map;
    map.insert(conv("ШТРИХ"), 1);
    map.insert(conv("АТОЛ"), 2);
    map.insert(conv("Меркурий"), 3);
    map.insert(conv("Касби"), 4);
    map.insert(conv("ПРИМ"), 5);
    m_protocolList.insert(1, map);
}

QStringList SearchDevice::protocolList(int type)
{
    QStringList list;
    if (type != 0)
        list.append(m_protocolList.value(type).keys());
    else
    {
        list.append(m_protocolList.value(1).keys());
        list.append(m_protocolList.value(2).keys());
    }

    return list;
}

void SearchDevice::initBaudRate()
{
    m_baudRate.clear();
    m_baudRate.insert("2400", 4);
    m_baudRate.insert("4800", 5);
    m_baudRate.insert("9600", 7);
    m_baudRate.insert("19200", 9);
    m_baudRate.insert("38400", 12);
    m_baudRate.insert("57600", 14);
    m_baudRate.insert("115200", 18);
}

QStringList SearchDevice::baudRate()
{
    return m_baudRate.keys();
}

int SearchDevice::portType(const QString &port)
{
    int result = 0;
    if (port.contains("COM"))
        result = 1;
    else if (port.contains("USB<->COM"))
        result = 3;
    else if (port.contains("ACM"))
        result = 4;
    else if (port.contains("USB"))
        result = 5;

    return result;
}

int SearchDevice::portNumber(int type, const QString &name)
{
    int result = 0;
    switch (type)
    {
    case 1:     // COM
        result = name.right(name.length() - 3).toInt();
        break;

    case 3:     // USB<->COM
        result = name.right(name.length() - 9).toInt();
        break;

    case 4:     // ACM
        result = name.right(name.length() - 3).toInt();
        break;
    }

    return result;
}

int SearchDevice::getDeviceTypeByProtocol(const QString &protocol)
{
    for (int i = 1; i < 3; ++i)
    {
        QMap<QString, int> map = m_protocolList.value(i);
        if (map.find(protocol) != map.end())
            return i;
    }
    return 0;
}

QString SearchDevice::getDeviceTypeByIndex(int index)
{
    QString deviceName;
    switch (index)
    {
    case 1:
        deviceName = conv("Фискальный регистратор");
        break;

    case 2:
        deviceName = conv("Принтер чеков");
        break;

    default:
        deviceName = conv("Неизвестно");
        break;
    }

    return deviceName;
}

QString SearchDevice::getPortName(int type, int port)
{
    QString portName;

    switch (type)
    {
    case 1:
        portName = "COM" + QString::number(port);
        break;

    case 3:
        portName = "USB<->COM" + QString::number(port);
        break;

    case 4:
        portName = "ACM" + QString::number(port);
        break;

    case 5:
        portName = "USB" + QString::number(port);
        break;
    }

    return portName;
}

QString SearchDevice::getProtocolByIndex(int deviceType, int index)
{
    return m_protocolList.value(deviceType).key(index);
}

QString SearchDevice::getBaudRateByIndex(int index)
{
    return m_baudRate.key(index);
}

int SearchDevice::getDeviceTypeByName(const QString &name)
{
    int deviceType = 0;
    if (name == conv("Фискальный регистратор"))
        deviceType = 1;
    else if (name == conv("Принтер чеков"))
        deviceType = 2;

    return deviceType;
}

ConnectOptions * SearchDevice::getConnectOptions(const QString &deviceName, const QString &devicePort, const QString &deviceProtocol,
                                                 const QString &deviceBaudRate, const QString &password, const QString &accessPassword)
{
    ConnectOptions * options = new ConnectOptions;
    options->logicalNumber = 1;
    options->baudRate = m_baudRate.value(deviceBaudRate);
    options->portType = portType(devicePort);
    options->port = portNumber(options->portType, devicePort);
    options->deviceType = getDeviceTypeByName(deviceName);
    options->protocol = m_protocolList.value(options->deviceType).value(deviceProtocol);
    options->checkPrinter = options->deviceType == 2;
    options->accessPassword = new char [16];
    strcpy(options->accessPassword, accessPassword.toAscii().data());
    options->password = new char [16];
    strcpy(options->password, password.toAscii());

    return options;
}

void SearchDevice::searchFRConnectionParam(const QString &deviceName, const QString &itemPort,
                                           const QString &itemBaudRate, const QString &itemProtocol, const Password &passwords)
{
    // Внутрений логический номер
    int logicalNumber = 1;
    // Тип устройства
    int deviceType;
    // Версия протокола
    QList<int> protocol;
    QList<int> protocolDeviceType;
    // Список номеров и типов портов
    QList<Value<int> > port;
    // Скорость передачи данных
    QList<int> baudRate;

    // Тип устройста
    // 0 - Неизвестно
    // 1 - Фискальный регистратор
    // 2 - Принтер чеков
    if (deviceName != conv("Неизвестно"))
    {
        if (deviceName == conv("Фискальный регистратор"))
            deviceType = 1;
        else if (deviceName == conv("Принтер чеков"))
            deviceType = 2;
    }
    else
    {
        // Если тип устройства неизвестен, то пытаемся определить его по протоколу
        if (itemProtocol != conv("Неизвестно"))
            deviceType = getDeviceTypeByProtocol(itemProtocol);
        else
            deviceType = 0;
    }

    // Версия протокола в зависимости от типа устройства
    if (itemProtocol != conv("Неизвестно"))
    {
        QMap<QString, int>::const_iterator iter;
        QMap<QString, int> map = m_protocolList.value(deviceType);
        iter = map.find(itemProtocol);
        if (iter != map.end())
        {
            protocol.append(iter.value());
            protocolDeviceType.append(deviceType);
        }
    }
    else
    {
        // Если был выбран тип устройства, то добавляем протоколы
        // соотвествующего типа, иначе добавляем все
        if (deviceType != 0)
        {
            protocol.append(m_protocolList.value(deviceType).values());
            for (int i = 0; i < protocol.size(); ++i)
                protocolDeviceType.append(deviceType);
        }
        else
        {
            protocol.append(m_protocolList.value(1).values());
            for (int i = 0; i < protocol.size(); ++i)
                protocolDeviceType.append(1);
            int size = protocol.size();
            protocol.append(m_protocolList.value(2).values());
            for (int i = 0; i < protocol.size() - size; ++i)
                protocolDeviceType.append(2);
        }
    }

    // Список портов. Класс Value содержит тип и номер порта
    if (itemPort != conv("Неизвестно"))
    {
        int t = portType(itemPort);
        int p = portNumber(t, itemPort);
        port.append(Value<int>(t, p));
    }
    else
    {
        foreach (QString portName, portList())
        {
            int t = portType(portName);
            int p = portNumber(t, portName);
            port.append(Value<int>(t, p));
        }
    }

    // Скорость передачи данных
    if (itemBaudRate != conv("Неизвестно"))
    {
        QMap<QString, int>::const_iterator iter = m_baudRate.find(itemBaudRate);
        if (iter != m_baudRate.end())
            baudRate.append(iter.value());
    }
    else
    {
        baudRate = m_baudRate.values();
    }

    emit setValueRange(0, protocol.size() * port.size() * baudRate.size());
    emit setProgressValue(0);

    bool found = false;
    int counter = 0;
    for (int i = 0; i < port.size(); ++i)
    {
        found = false;

        for (int j = 0; j < protocol.size() && !found; ++j)
        {
            // TODO: В зависимости от протокола выбираем предпочтительную скорость соединения
            for (int k = 0; k < baudRate.size() && !found; ++k)
            {
                counter += 1;
                HWSRV * hw = new HWSRV(1);
                emit setOperationDescription(conv("Поиск устройств.\nТип устройства: %1\nПротокол: %2\nПорт: %3\nСкорость передачи данных: %4").arg(getDeviceTypeByIndex(protocolDeviceType[j]),
                                                                                                                                                    getProtocolByIndex(protocolDeviceType[j], protocol[j]),
                                                                                                                                                    getPortName(port[i].first(), port[i].second()),
                                                                                                                                                    getBaudRateByIndex(baudRate[k])));

                int result = hw->ECR_SetConnectOptions(port[i].first(), logicalNumber, port[i].second(), baudRate[k],
                                                       protocol[j], true, passwords.accessPassword.toAscii(),
                                                       passwords.password.toAscii(), protocolDeviceType[j] == 2);

                if (result >= 0)
                {
                    result = hw->ECR_ReadStatus();
                    if (result >= 0)
                    {
                        // Устройство найдено
                        found = true;

                        counter = protocol.size() * baudRate.size() * (i + 1);
                        QString deviceName = getDeviceTypeByIndex(protocolDeviceType[j]);
                        QString devicePort = getPortName(port[i].first(), port[i].second());
                        QString deviceProtocol = getProtocolByIndex(protocolDeviceType[j], protocol[j]);
                        QString deviceBaudRate = getBaudRateByIndex(baudRate[k]);

                        emit addFoundDevice(deviceName, devicePort, deviceProtocol, deviceBaudRate);
                    }
                }

                hw->ECR_CloseConnection();
                delete hw;
                emit setProgressValue(counter);
            }
        }
    }
}
