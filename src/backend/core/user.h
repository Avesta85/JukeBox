#ifndef USER_H
#define USER_H
#include <Qvector>
#include <QQueue>

#include "person.h"

class User final : public Person
{

private:
    QString m_firstName;
    QString m_lastName;
    size_t  m_ID;
    QString m_password;
    QString m_email;


public:
    User();
    User(QString firstName, QString lastName, QString userName, size_t ID, QString password, QString email, QObject *parrent = nullptr);

    void setFirstName(const QString firstName);
    void setLastName(const QString lastName);
    void setID(const size_t ID);
    void setEmail(const QString &email);
    void setPassword(const QString &password);

    QString getFirstName() const noexcept;
    QString getLastName()const noexcept;
    size_t getID()const noexcept;
    QString getEmail()const noexcept;
    QString getPassword()const noexcept;


    ~User();

signals:
    void firstNameChanged();
    void lastNameChanged();
    void passwordChanged();
    void emailChanged();
};

#endif // USER_H
