#ifndef CRYPTOENGINE_H
#define CRYPTOENGINE_H

#include <QByteArray>

#include "crypto/rijndael.h"

class CryptoEngine
{
public:
    virtual ~CryptoEngine();

    virtual QByteArray encrypt(const QByteArray &data) = 0;
    virtual QByteArray decrypt(const QByteArray &data) = 0;
    void setKey(const QByteArray &key);
    void generateKey();

protected:
    const unsigned char * key;

};

class AESEngine : public CryptoEngine
{
public:
    AESEngine();
    ~AESEngine();

    virtual QByteArray encrypt(const QByteArray &data);
    virtual QByteArray decrypt(const QByteArray &data);

private:
    Rijndael AES;

};

#endif // CRYPTOENGINE_H
