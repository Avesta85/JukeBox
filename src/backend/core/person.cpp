#include "person.h"

Person::Person(QObject *parent) : QObject(parent) {}

void Person::setFirstName(QString firstName)
{
    if(firstName.trimmed().isEmpty())
        throw std::invalid_argument("This field cannot be empty!");

    if(this->m_firstName != firstName)
    {
        this->m_firstName = firstName;
        emit firstNameChanged();
    }
}

void Person::setLastName(QString lastName)
{
    if(lastName.trimmed().isEmpty())
        throw std::invalid_argument("This field cannot be empty!");

    if(this->m_lastName != lastName)
    {
        this->m_lastName = lastName;
        emit lastNameChanged();
    }
}

void Person::setUserName(QString userName)
{
    if(userName.trimmed().isEmpty())
        throw std::invalid_argument("This field cannot be empty!");

    if(this->m_userName != userName)
    {
        this->m_userName = userName;
        emit userNameChanged();
    }
}

void Person::setIP(QString IP)
{
    if(IP.trimmed().isEmpty())
        throw std::invalid_argument("This field cannot be empty!");

    if(this->m_IP != IP)
    {
        this->m_IP = IP;
        emit ipChanged();
    }
}

void Person::setPort(size_t port)
{

    if(port >= 65535)
        throw std::out_of_range("This port is invalid!");

    if(this->m_port != port)
    {
        this->  m_port = port;
        emit portChanged();
    }
}


QString Person::getFirstName() noexcept {return m_firstName;}

QString Person::getLastName() noexcept {return m_lastName;}

QString Person::getUserName() noexcept {return m_userName;}

QString Person::getIP() noexcept {return m_IP;}

size_t Person::getPort() noexcept {return m_port;}


Person::~Person() {}

