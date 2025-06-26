#include "person.h"

Person::Person(){}

Person::Person(QString userName)
{
    if(userName.trimmed().isEmpty())
    {
        throw std::invalid_argument("This field cannot be empty!");
    }

        m_userName = userName;
}

void Person::setUserName(QString userName)
{
    if(userName.trimmed().isEmpty())
        throw std::invalid_argument("This field cannot be empty!");

    if(this->m_userName != userName)
    {
        this->m_userName = userName;
    }
}

QString Person::getUserName() noexcept {return m_userName;}

Person::~Person() {}

