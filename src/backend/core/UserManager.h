#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "src/backend/core/playlist.h"
#include "src/backend/core/user.h"
#include <memory>
#include <src/backend/security/SecurityManager.h>

class UserManager final
{

public:

    static UserManager& getInstance();
    ~UserManager();
    UserManager(const UserManager&)=delete;
    UserManager& operator = (const UserManager&)=delete;



    bool attemptLogin(const QString& username , const QString Password);
    bool attemptLogout();


    bool attempSignup(const QString& username , const QString& password , const QString& firstname,
                      const QString& lastname,const QString& email ,const QString& securityKey);
    bool attemptDelete();


    bool UpdatePassword(const QString& newPassword);
    bool UpdateEmail(const QString& newEmail);
    bool UpdatePlaylistName(qint64 playlist_id,const QString& newName);


    bool addFriend(const QString& Friend_Username);
    qint64 addPlaylist(const Playlist& newPlaylist);
    bool addSongToPlaylist(qint64 Song_id , qint64 Playlist_id);
    bool addFavoriteSong(qint64 Song_id);



    bool deleteFriend(const QString& Friend_username);
    bool deletePlaylist(quint64 playlist_id);
    bool deleteSongfromPlaylist(quint64 playlist_id,quint64 song_id);
    bool deleteFavoriteSong(quint64 song_id);



    QString getUserName() const;
    QString getFirstName()const;
    QString getLastName()const;
    QString getEmail()const;
    Person getPublicDetials() const;
    QList<Playlist> getUserPLaylist()const;
    QList<Song> getUserPlaylistSong(qint64 Playlist_id)const;
    QList<Person> getUserFriend()const;
    QList<Song> getUserFavoriteSongs()const;



    bool is_loggedin();

private:
    UserManager();

    static std::unique_ptr<UserManager>s_instance;

    std::optional<User>m_local_user;
    SecurityManager m_security_manager;
    QString m_decoder_key;




};

#endif // USERMANAGER_H

