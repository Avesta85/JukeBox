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


    //login ->ok

    bool attemptLogin(const QString& username , const QString Password);
    bool attemptLogout();

    //signup ->ok

    bool attempSignup(const QString& username , const QString& password , const QString& firstname,
                      const QString& lastname,const QString& email ,const QString& securityKey);
    bool attemptDelete();

    // updater -> ok

    bool UpdatePassword(const QString& newPassword);
    bool UpdateEmail(const QString& newEmail);
    bool UpdatePlaylistName(qint64 playlist_id,const QString& newName);

    //adder -> ok

    bool addFriend(const QString& Friend_Username);
    bool addPlaylist(const Playlist& newPlaylist);
    bool addSongToPlaylist(qint64 Song_id , qint64 Playlist_id);
    bool addFavoriteSong(qint64 Song_id);



    //deleter -> ok
    bool deleteFriend(const QString& Friend_username);
    bool deletePlaylist(quint64 playlist_id);
    bool deleteSongfromPlaylist(quint64 playlist_id,quint64 song_id);
    bool deleteFavoriteSong(quint64 song_id);


    // gettets for User -> ok

    QString getUserName() const;
    QString getFirstName()const;
    QString getLastName()const;
    QString getEmail()const;
    Person getPublicDetials() const;
    QList<Playlist> getUserPLaylist()const;
    QList<Song> getUserPlaylistSong(qint64 Playlist_id)const;
    QList<Person> getUserFriend()const;
    QList<Song> getUserFavoriteSongs()const;

private:
    UserManager();

    //static
    static std::unique_ptr<UserManager>s_instance;

    //member
    std::optional<User>m_local_user;
    SecurityManager m_security_manager;
    QString m_decoder_key;
    //function

    //// hash and encript;



};

#endif // USERMANAGER_H
