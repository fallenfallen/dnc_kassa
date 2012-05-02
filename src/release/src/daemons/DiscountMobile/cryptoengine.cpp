#include "cryptoengine.h"

#include <QDebug>
#include <QRegExp>

CryptoEngine::~CryptoEngine()
{
    delete key;
}

void CryptoEngine::setKey(const QByteArray &key)
{
    this->key = reinterpret_cast<const unsigned char *>(key.data());
}

void CryptoEngine::generateKey()
{
    key = reinterpret_cast<const unsigned char *>("0123456789abcde");
}

AESEngine::AESEngine() : CryptoEngine()
{
    AES.SetParameters(128);
}

AESEngine::~AESEngine() {}

QByteArray AESEngine::encrypt(const QByteArray &text)
{
    int size = (text.length() + 15) & ~15;
    unsigned char * data = new unsigned char [size];

    AES.StartEncryption(key);
    AES.Encrypt(reinterpret_cast<const unsigned char *>(text.data()), data, size / 16);

    return QByteArray(reinterpret_cast<const char *>(data), size);
}

QByteArray AESEngine::decrypt(const QByteArray &text)
{
    int size = (text.length() + 15) & ~15;
    unsigned char * data = new unsigned char [size];

    AES.StartDecryption(key);
    AES.Decrypt(reinterpret_cast<const unsigned char*>(text.data()), data, size / 16);

    return QByteArray(reinterpret_cast<const char *>(data), size);
}
