#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include <Qstring>
#include <stdexcept>


class Person : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString m_firstName READ getFirstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString m_lastName READ getLastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString m_userName READ getUserName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString m_IP READ getIP WRITE setIP NOTIFY ipChanged)
    Q_PROPERTY(size_t m_port READ getPort WRITE setPort NOTIFY portChanged)

private :
    QString m_firstName;
    QString m_lastName;
    QString m_userName;
    QString m_IP;
    size_t m_port;

public:
    Person(QObject *parrent = nullptr);

    void setFirstName(QString m_firstName);
    void setLastName(QString m_lastName);
    void setUserName(QString m_userName);
    void setIP(QString m_IP);
    void setPort(size_t m_port);

    QString getFirstName()noexcept;
    QString getLastName()noexcept;
    QString getUserName()noexcept;
    QString getIP()noexcept;
    size_t  getPort()noexcept;

    ~Person();

signals:
    void firstNameChanged();
    void lastNameChanged();
    void userNameChanged();
    void ipChanged();
    void portChanged();
};

#endif // PERSON_H
