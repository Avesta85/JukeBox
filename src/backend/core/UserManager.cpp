#include "src/backend/core/UserManager.h"
#include "src/backend/db/DBM.h"


std::unique_ptr<UserManager> UserManager::s_instance = nullptr;

UserManager &UserManager::getInstance()
{
    if(!s_instance)
    {
        s_instance.reset(new UserManager());
    }

    return *s_instance;
}

UserManager::~UserManager()
{
    ;
}

bool UserManager::attemptLogin(const QString &username, const QString Password)
{
    try{
        DBM::get_instance().selectUser(m_local_user,username,
                                   m_security_manager.Hash(Password));

        this->m_decoder_key = username;
        return true;
    }
    catch(std::exception& e)
    {
        qDebug()<< e.what();
        return false;
    }
}

bool UserManager::attemptLogout()
{
    m_local_user.reset();
}

bool UserManager::attempSignup(const QString &username, const QString &password, const QString &firstname, const QString &lastname, const QString &email, const QString &securityKey)
{
    try{

        if(DBM::get_instance().isUsernameUnique(username)){
        DBM::get_instance().insertUser(
                               m_security_manager.encrypt(username,username),
                               m_security_manager.Hash(password),
                               m_security_manager.encrypt(firstname,username),
                               m_security_manager.encrypt(lastname,username),
                               m_security_manager.encrypt(email,username),
                               m_security_manager.encrypt(securityKey,username)
            );

        return true;
        }
        else{
            return false;
        }
    }
    catch(std::exception& e)
    {
        qDebug()<< e.what();
        return false;
    }
}

bool UserManager::attemptDelete()
{
    try{
        DBM::get_instance().deleteUser(m_local_user->getID());
        this->attemptLogout();
        return true;
    }
    catch(std::exception& e)
    {
        qDebug()<< e.what();
        return false;
    }
}

bool UserManager::UpdatePassword(const QString &newPassword)
{
    try{
        DBM::get_instance().updateUserPassword(m_local_user->getID(),m_security_manager.Hash(newPassword));
        return true;
    }
    catch(std::exception& e)
    {
        qDebug()<< e.what();
        return false;
    }
}

bool UserManager::UpdateEmail(const QString &newEmail)
{

    try{
        DBM::get_instance().updateUserEmail(m_local_user->getID(),m_security_manager.encrypt(newEmail,m_decoder_key));

        return true;
    }
    catch(std::exception& e)
    {
        qDebug()<< e.what();
        return false;
    }
}

bool UserManager::UpdatePlaylistName(qint64 playlist_id, const QString &newName)
{
    try{
        DBM::get_instance().updatePlaylistName(playlist_id,newName);

        return true;
    }
    catch(std::exception& e)
    {
        qDebug()<< e.what();
        return false;
    }
}

bool UserManager::addFriend(const QString &Friend_Username)
{


    try{
        return DBM::get_instance().insertFriend(m_local_user->getID(),Friend_Username);

    }
    catch(std::exception e){
        qDebug()<<e.what();
        return false;
    }
}

bool UserManager::addPlaylist(const Playlist &newPlaylist)
{
    try{
        return DBM::get_instance().insertPlaylist(newPlaylist.getName(),m_local_user->getID());

    }
    catch(std::exception e){
        qDebug()<<e.what();
        return false;
    }
}

bool UserManager::addSongToPlaylist(qint64 Song_id, qint64 Playlist_id)
{
    try{
        return DBM::get_instance().insertPlaylistSongs(Playlist_id,Song_id);

    }
    catch(std::exception e){
        qDebug()<<e.what();
        return false;
    }
}

bool UserManager::addFavoriteSong(qint64 Song_id)
{
    try{
        return DBM::get_instance().insertFavoritSong(m_local_user->getID(),Song_id);

    }
    catch(std::exception e){
        qDebug()<<e.what();
        return false;
    }
}

bool UserManager::deleteFriend(const QString &Friend_username)
{
    try{
        return DBM::get_instance().deleteFriend(m_local_user->getID(),Friend_username);

    }
    catch(std::exception e){
        qDebug()<<e.what();
        return false;
    }
}

bool UserManager::deletePlaylist(quint64 playlist_id)
{
    try{

        return DBM::get_instance().deletePlaylist(playlist_id);
    }
    catch(std::exception e){
        qDebug()<<e.what();
        return false;
    }
}

bool UserManager::deleteSongfromPlaylist(quint64 playlist_id,quint64 song_id)
{
    try{
        return DBM::get_instance().deletePlaylistSong(playlist_id,song_id);
    }
    catch(std::exception e){
        qDebug()<<e.what();
        return false;
    }
}

bool UserManager::deleteFavoriteSong(quint64 song_id)
{
    try{
        return DBM::get_instance().deleteFavoritSong(m_local_user->getID(),song_id);
    }
    catch(std::exception e){
        qDebug()<<e.what();
        return false;
    }
}

UserManager::UserManager()
{
    ;
}



QString UserManager::getUserName() const
{
    return m_security_manager.decrypt(m_local_user->getUserName(),this->m_decoder_key);
}

QString UserManager::getFirstName() const
{
    return m_security_manager.decrypt(m_local_user->getFirstName(),this->m_decoder_key);
}

QString UserManager::getLastName() const
{
    return m_security_manager.decrypt(m_local_user->getLastName(),this->m_decoder_key);
}

QString UserManager::getEmail() const
{
    return m_security_manager.decrypt(m_local_user->getEmail(),this->m_decoder_key);
}

Person UserManager::getPublicDetials() const
{
    Person tmp(m_security_manager.decrypt(m_local_user->getUserName(),this->m_decoder_key));
    return tmp;
}

QList<Playlist> UserManager::getUserPLaylist() const
{
    return DBM::get_instance().getPlaylistsForUser(m_local_user->getID());
}

QList<Song> UserManager::getUserPlaylistSong(qint64 Playlist_id) const
{
    return DBM::get_instance().getSongsInPlaylist(Playlist_id);
}

QList<Person> UserManager::getUserFriend() const
{
    return DBM::get_instance().getFriendsForUser(m_local_user->getID());
}

QList<Song> UserManager::getUserFavoriteSongs() const
{
    return DBM::get_instance().getFavoriteSongsForUser(m_local_user->getID());
}

bool UserManager::is_loggedin()
{
    return m_local_user.has_value();
}
