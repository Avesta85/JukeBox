#ifndef USER_H
#define USER_H
#include <vector>
#include <QQueue>

#include "person.h"

class User final : public Person
{
    Q_OBJECT
    Q_PROPERTY(QString m_password READ getPassword WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString m_email READ getEmail WRITE setEmail NOTIFY emailChanged)

public:
    User();

    void setEmail(const QString &email);
    void setPassword(const QString &password);
    void addToPlaylist(const QString &songName);
    void enqueueFavoriteSong(const QString &songName);
    void addFriend(const QString &friendUserName);

    QString getEmail()const noexcept;
    QString getPassword()const noexcept;
    QVector<QString> getPlaylist()const noexcept;
    QQueue<QString> getFavoriteSongs()const noexcept;
    QVector<QString> getFriends()const noexcept;

    ~User();

private:
    QString m_password;
    QString m_email;
    QVector<QString> m_playlist;
    QQueue<QString> m_favoriteSongs ;
    QVector<QString> m_friends;

signals:
    void passwordChanged();
    void emailChanged();
};

#endif // USER_H
