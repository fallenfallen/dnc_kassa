#ifndef CONNECTOPTIONS_H
#define CONNECTOPTIONS_H

struct ConnectOptions
{
    int portType;
    int port;
    int protocol;
    int deviceType;
    int baudRate;
    char * password;
    char * accessPassword;
    int logicalNumber;
    bool checkPrinter;
};

#endif // CONNECTOPTIONS_H
