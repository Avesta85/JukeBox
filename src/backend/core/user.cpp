#include "user.h"

User::User() {}

User::User(QString firstName, QString lastName, QString userName, size_t ID, QString password, QString email) : Person(userName)
{
    if(firstName.trimmed().isEmpty())
        throw std::invalid_argument("This field cannot be empty!");

    if(lastName.trimmed().isEmpty())
        throw std::invalid_argument("This field cannot be empty!");

    if(email.trimmed().isEmpty())
        throw std::invalid_argument("This field cannot be empty!");

    if(password.trimmed().isEmpty())
        throw std::invalid_argument("This field cannot be empty!");

    m_firstName = firstName;
    m_lastName = lastName;
    m_ID = ID;
    m_password = password;
    m_email = email;
}

void User::setFirstName(const QString firstName)
{
    if(firstName.trimmed().isEmpty())
        throw std::invalid_argument("This field cannot be empty!");

    if(this->m_firstName != firstName)
    {
        this->m_firstName = firstName;

    }
}

void User::setLastName(const QString lastName)
{
    if(lastName.trimmed().isEmpty())
        throw std::invalid_argument("This field cannot be empty!");

    if(this->m_lastName != lastName)
    {
        this->m_lastName = lastName;

    }
}

void User::setID(const size_t ID)
{
    // exeption...
    m_ID = ID;
}

void User::setEmail(const QString &email)
{
    if(email.trimmed().isEmpty())
        throw std::invalid_argument("This field cannot be empty!");

}

void User::setPassword(const QString &password)
{
    if(password.trimmed().isEmpty())
        throw std::invalid_argument("This field cannot be empty!");

    if(this->m_password != password)
    {
        this->m_password = password;

    }
}

//exeption for this methods.....
QString User::getFirstName()const noexcept {return m_firstName;}
QString User::getLastName()const noexcept {return m_lastName;}
size_t User::getID()const noexcept {return m_ID;}
QString User::getEmail()const noexcept {return m_email;}
QString User::getPassword()const noexcept {return m_password;}
User::~User(){}
