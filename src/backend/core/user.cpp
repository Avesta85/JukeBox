#include "user.h"

User::User() {}

void User::setEmail(const QString &email)
{
    if(email.trimmed().isEmpty())
        throw std::invalid_argument("This field cannot be empty!");

    // more exeption will add

    if(this->m_email != email)
    {
        this->m_email = email;
        emit emailChanged();
    }
}

void User::setPassword(const QString &password)
{
    if(password.trimmed().isEmpty())
        throw std::invalid_argument("This field cannot be empty!");

    if(this->m_password != password)
    {
        this->m_password = password;
        emit passwordChanged();
    }
}

//more exeption for this methods.....
void User::addToPlaylist(const QString &songName)
{
    if(songName.trimmed().isEmpty())
        throw std::invalid_argument("Invalid song name!");

    m_playlist.append(songName);
}

void User::enqueueFavoriteSong(const QString &songName)
{
    if(songName.trimmed().isEmpty())
        throw std::invalid_argument("Invalid song name!");

    m_favoriteSongs.enqueue(songName);
}

void User::addFriend(const QString &friendUserName)
{
    if(friendUserName.trimmed().isEmpty())
        throw std::invalid_argument("Invalid UserName name!");

    m_friends.append(friendUserName);
}

QString User::getEmail()const noexcept {return m_email;}
QString User::getPassword()const noexcept {return m_password;}
QVector<QString> User::getPlaylist()const noexcept {return m_playlist;}
QQueue<QString> User::getFavoriteSongs()const noexcept {return m_favoriteSongs;}
QVector<QString> User::getFriends()const noexcept {return m_friends;}

User::~User(){}
