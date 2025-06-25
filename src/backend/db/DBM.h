#ifndef DBM_H
#define DBM_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <memory>
#include <QThread>
#include <QDirIterator>
#include <QFuture>
#include <QMap>
#include <QSet>
#include <QObject>


///
/// \brief The DBM class
///
/// All input string about User must be hashed before passed
/// All input directly send to db without hashing
///
///
class DBM final
{


public:

    //C&D
    ~DBM();
    static DBM& get_instance();

    //song sync

    QMap<QString,QString> scanDiskForSongs(const QString& folder_path) const; // read disk and create {path , songs_name } map

    void applySyncChanges(const QMap<QString,QString>& diskSongs); // update songs table from pram: disksongs

    bool verifySongsPath(const QString& path) ;// check path for valid music

    void applyVerifySongsPath(); // delete invalid path from db

    //insert

    bool insertSong(const QString& name , const QString& path);

    bool insertUser(const QString& Username,const QString& Password , const QString& first_name ,
                    const QString& last_name , const QString& email , const QString& secret_Key);

    bool insertPlaylist(const QString& Playlist_Name , const size_t User_id);

    bool insertPlaylistSongs(const size_t Playlist_id , const size_t Song_id);

    bool insertFriend(const size_t owner_id , const QString& friend_username);

    bool insertFavoritSong(const size_t user_id ,const size_t song_id);

    //select



    //ckeckers

    bool isDbOpen();

    bool isUsernameUnique(const QString& Username) ;

private:
    // Static Private Variable
    static std::unique_ptr<DBM> s_instance;

    static const QString s_db_path;

    static const QString s_db_name;



    // Private Member Variable

    QString m_db_songs_folder;

    QSqlDatabase m_db;

    QMutex m_db_mutex;

    // Private Function

    // --Static

    // --Member
    DBM();

    void SetupDb();

    void SetupTable();

    void fineMusicDir();

};

#endif // DBM_H
