#ifndef USER_H
#define USER_H
#include <QVector>
#include <QQueue>

#include "person.h"

class User final : public Person , public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString m_firstName READ getFirstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString m_lastName READ getLastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString m_password READ getPassword WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString m_email READ getEmail WRITE setEmail NOTIFY emailChanged)

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
