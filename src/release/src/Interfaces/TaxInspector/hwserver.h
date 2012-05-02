#ifndef HWSERVER_H
#define HWSERVER_H

#include "hwsrv.h"
#include "SearchDeviceApp/connectoptions.h"

class HWServer
{
public:
    static int connect();
    static int connect(const ConnectOptions * options);
    static void disconnect();
    static HWSRV * hw();
    static int protocol();

private:
    static int m_protocol;
    static HWSRV * m_hw;
};

#endif // HWSERVER_H
