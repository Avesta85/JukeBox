#ifndef SECURITYMANAGER_H
#define SECURITYMANAGER_H

#include <QString>


class SecurityManager
{

public:

    SecurityManager();
    ~SecurityManager();
    QString encrypt(const QString& plainText, const QString& baseSecret)const;
    QString decrypt(const QString& encryptedText,  const QString& baseSecret)const;

    QString Hash(const QString& plainText)const;


    QString securityKey_gen()const;


    const QString SendVerification_Code(QString& Email);

private:

    const std::string passphrase;

};


#endif // SECURITYMANAGER_H

