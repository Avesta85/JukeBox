#include "user.h"

User::User() {}

User::User(QString firstName, QString lastName, QString userName, size_t ID, QString password, QString email, QObject *parrent) : Person(userName, parrent)
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
        emit firstNameChanged();
    }
}

void User::setLastName(const QString lastName)
{
    if(lastName.trimmed().isEmpty())
        throw std::invalid_argument("This field cannot be empty!");

    if(this->m_lastName != lastName)
    {
        this->m_lastName = lastName;
        emit lastNameChanged();
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

//exeption for this methods.....
void User::addToPlaylist(const std::shared_ptr<Song> &song)
{
    m_playlist.append(song);
}

void User::enqueueFavoriteSong(const std::shared_ptr<Song> &song)
{
    m_favoriteSongs.enqueue(song);
}

void User::addFriend(const std::shared_ptr<Person> &Friend)
{
    m_friends.append(Friend);
}

QString User::getFirstName()const noexcept {return m_firstName;}
QString User::getLastName()const noexcept {return m_lastName;}
size_t User::getID()const noexcept {return m_ID;}
QString User::getEmail()const noexcept {return m_email;}
QString User::getPassword()const noexcept {return m_password;}
QVector<std::shared_ptr<Song>> User::getPlaylist()const noexcept {return m_playlist;}
QQueue<std::shared_ptr<Song>> User::getFavoriteSongs()const noexcept {return m_favoriteSongs;}
QVector<std::shared_ptr<Person>> User::getFriends()const noexcept {return m_friends;}
>>>>>>> user

User::~User(){}
