#ifndef SECURITYMANAGER_H
#define SECURITYMANAGER_H

#include <QString>


class SecurityManager
{

public:

    SecurityManager();
    ~SecurityManager();
    // encrypt and decrypt
    QString encrypt(const QString& plainText, const QString& baseSecret)const;
    QString decrypt(const QString& plainText,  const QString& baseSecret)const;

    //hash
    QString Hash(const QString& plainText)const;

    //security_key gen

private:

    const std::string passphrase;

};


#endif // SECURITYMANAGER_H
