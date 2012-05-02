#ifndef SEARCHDEVICE_H
#define SEARCHDEVICE_H

#include <cstdlib>
#include <QFile>
#include <QMap>
#include <QStringList>

#include "translator.h"
#include "connectoptions.h"
#include "hwsrv.h"

struct Password
{
    QString password;
    QString accessPassword;
};

template <class T>
class Value
{
public:
    Value(T first, T second) : m_first(first), m_second(second) {}

    T first()  { return m_first; }
    T second() { return m_second; }

private:
    T m_first;
    T m_second;
};

class SearchDevice : public QObject
{
    Q_OBJECT

public:
    SearchDevice(QObject * parent = 0);
    ~SearchDevice();

    QStringList portList();
    QStringList baudRate();
    QStringList protocolList(int type);

    void searchFRConnectionParam(const QString &deviceName, const QString &itemPort,
                                 const QString &itemBaudRate, const QString &itemProtocol, const Password &passwords);

    ConnectOptions * getConnectOptions(const QString &deviceName, const QString &devicePort, const QString &deviceProtocol,
                                       const QString &deviceBaudRate, const QString &password, const QString &accessPassword);

signals:
    void setOperationDescription(const QString &description);
    void setValueRange(double min, double max);
    void setProgressValue(double value);
    void addFoundDevice(const QString &deviceName, const QString &port,
                        const QString &protocol, const QString &baudRate);

private:
    QStringList m_portList;
    QMap<int, QMap<QString, int> > m_protocolList;
    QMap<QString, int> m_baudRate;

    void initPortList();
    void initProtocolList();
    void initBaudRate();
    bool createSystemPorts();
    QString setDeviceName(QString deviceType);
    int getDeviceTypeByProtocol(const QString &protocol);
    int getDeviceTypeByName(const QString &name);
    int portType(const QString &port);
    int portNumber(int type, const QString &name);

    QString getDeviceTypeByIndex(int index);
    QString getPortName(int type, int port);
    QString getProtocolByIndex(int deviceType, int index);
    QString getBaudRateByIndex(int index);

};

#endif // SEARCHDEVICE_H
