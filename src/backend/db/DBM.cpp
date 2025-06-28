#include "./DBM.h"
#include "QtConcurrent/qtconcurrentrun.h"
#include "src/backend/security/SecurityManager.h"
#include <optional>

std::unique_ptr<DBM> DBM::s_instance = nullptr;

const QString DBM::s_db_path =  QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/jukeboxe/data";

const QString DBM::s_db_name = "JuckBox_Data.db";



DBM::~DBM()
{
    ;
}

DBM &DBM::get_instance()
{
    if(!s_instance)
    {

        s_instance.reset(new DBM());

    }
    return *s_instance;
}

DBM::DBM()
{
    main_thread = QThread::currentThread();
    try
    {

        this->fineMusicDir();

        // load from disk
        QFuture<QMap<QString,QString>> future = QtConcurrent::run([this](){
            return this->scanDiskForSongs(this->m_db_songs_folder);
        });

        // setup db
        this->SetupDb();

        future.waitForFinished();

        // apply changes to setuped db

        auto diskSongs = future.result();

        this->applySyncChanges(diskSongs);
    }
    catch(const std::runtime_error& e)
    {
        qDebug()<<e.what();
    }
}

void DBM::SetupDb()
{
    {
    std::scoped_lock<QMutex>lock(m_db_mutex);
    QDir path_dir(s_db_path);
    if(!path_dir.exists())
    {
        qDebug() << "creating dir "<<s_db_path;
        path_dir.mkpath(".");
    }

    const QString full_path = s_db_path +"/"+s_db_name;

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(full_path);

    if(!m_db.open()){
        throw std::runtime_error("can not open DB");
    }

    qDebug() << "data base opened successfully";
    }
    this->SetupTable();
}

void DBM::SetupTable()
{
    std::scoped_lock<QMutex>lock(m_db_mutex);
    QSqlQuery query;

    // user table id , Username , firstname , lastname,password ,email,secret key

    bool success = query.exec(""
                              "CREATE TABLE IF NOT EXISTS Users ( "
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "username TEXT UNIQUE NOT NULL,"
                              "firstname TEXT,"
                              "lastname TEXT,"
                              "password TEXT NOT NULL,"
                              "email TEXT UNIQUE,"
                              "secret_key TEXT "
                              "); ");

    //song table id , name, path


    success &= query.exec(""
                          "CREATE TABLE IF NOT EXISTS Songs ( "
                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "name TEXT NOT NULL, "
                          "path TEXT UNIQUE NOT NULL "
                          "); ");

    //playlist table id , name , id

    success &= query.exec(""
                          "CREATE TABLE IF NOT EXISTS Playlists ( "
                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "name TEXT NOT NULL, "
                          "user_id INTEGER NOT NULL,"
                          "FOREIGN KEY (user_id) REFERENCES Users (id) ON DELETE CASCADE "
                          "); ");

    //playlist songs id id

    success &= query.exec(""
                          "CREATE TABLE IF NOT EXISTS PlaylistSongs ( "
                          "playlist_id INTEGER NOT NULL, "
                          "song_id INTEGER NOT NULL, "
                          "PRIMARY KEY (playlist_id , song_id ),"
                          "FOREIGN KEY (playlist_id) REFERENCES playlists (id) ON DELETE CASCADE, "
                          "FOREIGN KEY (song_id) REFERENCES Songs (id) ON DELETE CASCADE "
                          ");");

    //favorite song id id

    success &= query.exec(""
                          "CREATE TABLE IF NOT EXISTS FavoriteSongs ( "
                          "user_id INTEGER NOT NULL, "
                          "song_id INTEGER NOT NULL, "
                          "PRIMARY KEY (user_id , song_id ),"
                          "FOREIGN KEY (user_id) REFERENCES Users (id) ON DELETE CASCADE, "
                          "FOREIGN KEY (song_id) REFERENCES Songs (id) ON DELETE CASCADE "
                          "); ");

    //friend id , Username, ip , port

    success &= query.exec(""
                          "CREATE TABLE IF NOT EXISTS Friends ( "
                          "owner_id INTEGER NOT NULL, "
                          "friend_username TEXT NOT NULL , "
                          "PRIMARY KEY (owner_id , friend_username),"
                          "FOREIGN KEY (owner_id) REFERENCES Users (id) ON DELETE CASCADE "
                          "); ");

    if(!success)
    {
        throw std::runtime_error("can not create table 1");
    }
    qDebug() << "All table created";


}

void DBM::fineMusicDir()
{
    this->m_db_songs_folder = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    qDebug() << "music dir is "<<m_db_songs_folder;
}

QMap<QString, QString> DBM::scanDiskForSongs(const QString &folder_path) const
{
    qDebug()<<"scan disk started in thread : " << QThread::currentThread();
    QMap<QString,QString> disk_songs;
    QDirIterator it(folder_path,{"*.mp3","*.wav"},QDir::Files,QDirIterator::Subdirectories);

    while(it.hasNext())
    {
        QString fullpath = it.next();
        QFileInfo fileInfo(fullpath);
        disk_songs.insert(fullpath,fileInfo.fileName());
    }
    qDebug() << "disk scan finished found <<"<<disk_songs.size() ;
    return disk_songs;
}

void DBM::applySyncChanges(const QMap<QString, QString> &diskSongs)
{
    std::scoped_lock<QMutex>lock(m_db_mutex);
    // load from songs table

    QMap<QString,QString> dbSongs;
    QSqlQuery selectQuery;
    if(selectQuery.exec("SELECT path , name FROM Songs "))
    {
        while ( selectQuery.next())
        {
            dbSongs.insert(selectQuery.value(0).toString(),selectQuery.value(1).toString());
        }
    }
    else
    {
        qDebug()<< "songs Selects Query faild " << selectQuery.lastError();
        throw std::runtime_error("can not load from songs table");

    }

    // compare

    auto tmplist1 = dbSongs.keys();
    const QSet<QString> dbPath(tmplist1.begin(),tmplist1.end());
    auto tmplist = diskSongs.keys();
    const QSet<QString> diskPaths(tmplist.begin(),tmplist.end());

    const auto newSongPaths = diskPaths - dbPath;
    const auto deletedPaths = dbPath - diskPaths;

    if(!m_db.transaction())
    {
        throw std::runtime_error("data base can not set to safe mode");
    }

    if(!newSongPaths.isEmpty())
    {
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO Songs (path, name) VALUES ( ? , ? ) ");
        for(const auto& path : newSongPaths)
        {
            insertQuery.addBindValue(path);
            insertQuery.addBindValue(diskSongs.value(path));
            if(!insertQuery.exec())
            {
                throw std::runtime_error(" can not insert into songs table");
            }
        }
    }

    if(!deletedPaths.isEmpty())
    {
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM Songs WHERE path = ? ");
        for (const auto& path : deletedPaths)
        {
            deleteQuery.addBindValue(path);
            if(!deleteQuery.exec())
            {
                throw std::runtime_error("Can not delete from songs table");
            }
        }
    }


    if(!m_db.commit())
    {
        throw std::runtime_error("db mode can not change");
    }
    else
    {
        qDebug()<<"  DB Changes applied ";
    }
}

// insert
bool DBM::insertSong(const QString &name, const QString &path)
{
    std::scoped_lock<QMutex>lock(m_db_mutex);
    QSqlQuery insert;

    insert.prepare("INSERT INTO Songs (name , path) VALUES ( :name , :path )");
    insert.bindValue(":name",name);
    insert.bindValue(":path",path);

    if(!insert.exec())
    {
        qDebug() << "Failed to insert song:" << insert.lastError().text();
        return false;
    }

    qDebug()<<"Song" << name << "inserted successfully.";
    return true;

}

bool DBM::insertUser(const QString &Username, const QString &Password, const QString &first_name, const QString &last_name, const QString &email, const QString &secret_Key)
{
    std::scoped_lock<QMutex>lock(m_db_mutex);
    QSqlQuery insert ;

    insert.prepare("INSERT INTO Users (username , password , firstname , lastname , email , secret_key) "
                   "VALUES ( :username , :password , :firstname , :lastname , :email , :secret_key )");

    insert.bindValue(":username", Username);
    insert.bindValue(":password", Password);
    insert.bindValue(":firstname", first_name);
    insert.bindValue(":lastname", last_name);
    insert.bindValue(":email", email);
    insert.bindValue(":secret_key", secret_Key);

    if (!insert.exec()) {
        qDebug() << "Failed to insert user:" << insert.lastError().text();
        return false;
    }

    qDebug() << "User" << Username << "inserted successfully.";
    return true;

}

bool DBM::insertPlaylist(const QString &Playlist_Name, const size_t User_id)
{
    std::scoped_lock<QMutex>lock(m_db_mutex);
    QSqlQuery insert ;

    insert.prepare("INSERT INTO Playlists (name , user_id) VALUES (:name , :user_id )");
    insert.bindValue(":name",Playlist_Name);
    insert.bindValue(":user_id",static_cast<qint64>(User_id));

    if (!insert.exec()) {
        qDebug() << "Failed to insert playlist:" << insert.lastError().text();
        return false;
    }

    qDebug() << "Playlist" << Playlist_Name << "inserted successfully.";
    return true;
}

bool DBM::insertPlaylistSongs(const size_t Playlist_id, const size_t Song_id)
{
    std::scoped_lock<QMutex>lock(m_db_mutex);
    QSqlQuery insert;

    insert.prepare("INSERT INTO PlaylistSongs (playlist_id , song_id ) VALUES ( :playlist_id , :song_id )");
    insert.bindValue(":playlist_id",Playlist_id);
    insert.bindValue(":song_id",Song_id);

    if (!insert.exec()) {
        qDebug() << "Failed to insert playlist songs:" << insert.lastError().text();
        return false;
    }

    qDebug() << "Playlistsong inserted successfully.";
    return true;
}

bool DBM::insertFriend(const size_t owner_id, const QString &friend_username)
{
    std::scoped_lock<QMutex>lock(m_db_mutex);
    QSqlQuery insert;

    insert.prepare("INSERT INTO Friends (owner_id , friend_username ) VALUES ( :owner_id , :friend_username )");
    insert.bindValue(":owner_id",owner_id);
    insert.bindValue(":friend_username",friend_username);


    if (!insert.exec()) {
        qDebug() << "Failed to insert Friend :" << insert.lastError().text();
        return false;
    }

    qDebug() << "Friend inserted successfully.";
    return true;
}

bool DBM::insertFavoritSong(const size_t user_id, const size_t song_id)
{
    std::scoped_lock<QMutex>lock(m_db_mutex);
    QSqlQuery insert;

    insert.prepare("INSERT INTO FavoriteSongs (user_id , song_id ) VALUES ( :user_id , :song_id )");
    insert.bindValue(":user_id",user_id);
    insert.bindValue(":song_id",song_id);


    if (!insert.exec()) {
        qDebug() << "Failed to insert FavoriteSongs :" << insert.lastError().text();
        return false;
    }

    qDebug() << "FavoriteSongs inserted successfully.";
    return true;
}

//select

void DBM::selectUser(std::optional<User>&local_user_holder,const QString &Username, const QString &Password)
{
    std::scoped_lock<QMutex> locker(m_db_mutex);

    QSqlQuery select;

    select.prepare("SELECT id , username , firstname,lastname,email,secret_key FROM Users"
                   " WHERE password = :password ");
    select.bindValue(":password",Password);

    if(!select.exec())
    {
        throw std::runtime_error("cant run user select query");
    }

    while(select.next())
    {
        SecurityManager sm;
        if(sm.decrypt(select.value(1).toString(),Username) == Username){
            local_user_holder.emplace();
            local_user_holder->setID(select.value(0).toLongLong());
            local_user_holder->setUserName(select.value(1).toString());
            local_user_holder->setFirstName(select.value(2).toString());
            local_user_holder->setLastName(select.value(3).toString());
            local_user_holder->setEmail(select.value(4).toString());
            return;
        }
    }
    local_user_holder = std::nullopt;

}

QList<Playlist> DBM::getPlaylistsForUser(qint64 userId)
{
    std::scoped_lock<QMutex> locker(m_db_mutex);

    QSqlQuery select;
    QList<Playlist> holder;
    select.prepare("SELECT id , name, user_id FROM Playlists WHERE user_id = :user_id");
    select.bindValue(":user_id",userId);

    if(!select.exec())
    {
        throw std::runtime_error("cant select playlist from db");
    }

    while(select.next())
    {

        Playlist tmp(select.value(1).toString(),select.value(0).toLongLong(),select.value(2).toLongLong());
        holder.append(tmp);
    }

    return holder;
}

QList<Song> DBM::getSongsInPlaylist(qint64 playlistId)
{
    std::scoped_lock locker(m_db_mutex);
    QList<Song> songs;
    QSqlQuery query;

    query.prepare(
        "SELECT Songs.id, Songs.name, Songs.path FROM Songs "
        "INNER JOIN PlaylistSongs ON Songs.id = PlaylistSongs.song_id "
        "WHERE PlaylistSongs.playlist_id = ?"
        );
    query.addBindValue(playlistId);

    if (!query.exec()) {
        qDebug() << "Failed to get songs for playlist:" << query.lastError();
        return songs;
    }

    while (query.next()) {
        songs.append(Song(
            query.value(1).toString(),   // name
            query.value(2).toString(),     // path
            0,
            "",
            query.value(0).toLongLong() // id
            ));
    }
    return songs;
}

QList<Song> DBM::getFavoriteSongsForUser(qint64 userId)
{
    std::scoped_lock<QMutex> locker(m_db_mutex);

    QSqlQuery select;
    QList<Song> holder;

    select.prepare(
        "SELECT s.id , s.name ,s.path FROM Songs s "
        "INNER JOIN FavoriteSongs fs ON s.id = fs.song_id "
        "WHERE fs.user_id = :user_id"
        );

    select.bindValue(":user_id",userId);

    if(!select.exec())
    {
        throw std::runtime_error("cant select Favorite songs from db");
    }

    while(select.next())
    {
        holder.append(Song(select.value(1).toString(),select.value(2).toString(),0,"",select.value(0).toLongLong()));
    }
    return holder;
}


QList<Person> DBM::getFriendsForUser(qint64 userId)
{
    std::scoped_lock<QMutex> locker(m_db_mutex);

    QSqlQuery select;
    QList<Person> holder;

    select.prepare(
        "SELECT friend_username FROM Friends WHERE owner_id = :owner_id "
        );
    select.bindValue(":owner_id",userId);

    if(!select.exec())
    {
        throw std::runtime_error("cant select friend from db");
    }
    while(select.next())
    {
        holder.append(Person(select.value(0).toString()));
    }

    return holder;

}

QString DBM::getEmailofUser(const QString &username)
{
    std::scoped_lock<QMutex> locker(m_db_mutex);

    QSqlQuery select;

    select.prepare(
        "SELECT username , email FROM Users"
        );

    if(!select.exec())
    {
        throw std::runtime_error("cant select friend from db");
    }
    while(select.next())
    {
        SecurityManager sm;
        if(sm.decrypt(select.value(0).toString(),username) == username)
        {
            return sm.decrypt(select.value(1).toString(),username);
        }
    }

    return "";
}

QString DBM::getSKeyofUser(const QString &username)
{
    std::scoped_lock<QMutex> locker(m_db_mutex);

    QSqlQuery select;

    select.prepare(
        "SELECT username , secret_key FROM Users"
        );

    if(!select.exec())
    {
        throw std::runtime_error("cant select friend from db");
    }
    while(select.next())
    {
        SecurityManager sm;
        if(sm.decrypt(select.value(0).toString(),username) == username)
        {
            return sm.decrypt(select.value(1).toString(),username);
        }
    }

    return "";
}

// delete
bool DBM::deleteUser(const size_t user_id)
{
    std::scoped_lock locker(m_db_mutex);
    QSqlQuery del;
    del.prepare("DELETE FROM Users WHERE id = :id");
    del.bindValue(":id",static_cast<qint64>(user_id));

    if(!del.exec())
    {
        qDebug() << "Failed to delete user:" << del.lastError().text();
        return false;
    }

    return del.numRowsAffected() > 0;
}

bool DBM::deleteSong(const size_t Song_id)
{
    std::scoped_lock locker(m_db_mutex);
    QSqlQuery del;
    del.prepare("DELETE FROM Songs WHERE id = :id");
    del.bindValue(":id",static_cast<qint64>(Song_id));

    if(!del.exec())
    {
        qDebug() << "Failed to delete song:" << del.lastError().text();
        return false;
    }

    return del.numRowsAffected() > 0;
}



bool DBM::deletePlaylist(const size_t playlist_id)
{
    std::scoped_lock locker(m_db_mutex);
    QSqlQuery del;
    del.prepare("DELETE FROM Playlists WHERE id = :id");
    del.bindValue(":id",static_cast<qint64>(playlist_id));

    if(!del.exec())
    {
        qDebug() << "Failed to delete playlist:" << del.lastError().text();
        return false;
    }

    return del.numRowsAffected() > 0;
}

bool DBM::deletePlaylistSong(const size_t playlist_id, const size_t song_id)
{
    std::scoped_lock locker(m_db_mutex);
    QSqlQuery del;
    del.prepare("DELETE FROM PlaylistSongs WHERE playlist_id = :playlist_id AND song_id = :song_id ");
    del.bindValue(":playlist_id",static_cast<qint64>(playlist_id));
    del.bindValue(":song_id",static_cast<qint64>(song_id));
    if(!del.exec())
    {
        qDebug() << "Failed to delete song from playlist:" << del.lastError().text();
        return false;
    }

    return del.numRowsAffected() > 0;
}

bool DBM::deleteFriend(const size_t user_id, const QString &friend_username)
{
    std::scoped_lock locker(m_db_mutex);
    QSqlQuery del;
    del.prepare("DELETE FROM Friends WHERE owner_id = :owner_id AND friend_username = :friend_username ");
    del.bindValue(":owner_id",static_cast<qint64>(user_id));
    del.bindValue(":friend_username",friend_username);
    if(!del.exec())
    {
        qDebug() << "Failed to delete friend:" << del.lastError().text();
        return false;
    }

    return del.numRowsAffected() > 0;
}

bool DBM::deleteFavoritSong(const size_t user_id, const size_t song_id)
{
    std::scoped_lock locker(m_db_mutex);
    QSqlQuery del;
    del.prepare("DELETE FROM FavoriteSongs WHERE user_id = :user_id AND song_id = :song_id ");
    del.bindValue(":user_id",static_cast<qint64>(user_id));
    del.bindValue(":song_id",static_cast<qint64>(song_id));
    if(!del.exec())
    {
        qDebug() << "Failed to delete favorite song:" << del.lastError().text();
        return false;
    }

    return del.numRowsAffected() > 0;
}

bool DBM::updateUserPassword(qint64 user_id, const QString &newPassword)
{

    std::scoped_lock locker(m_db_mutex);

    QSqlQuery query;
    query.prepare("UPDATE Users SET password = :password WHERE id = :id");

    query.bindValue(":password", newPassword);
    query.bindValue(":id", user_id);

    if (!query.exec()) {
        qDebug() << "Failed to update user password:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() > 0) {
        qDebug() << "Password for user id" << user_id << "updated successfully.";
        return true;
    } else {
        qDebug() << "Update password failed: User with id" << user_id << "not found.";
        return false;
    }
}

bool DBM::updateUserPassword(const QString &userName, const QString &newPassword)
{
    std::scoped_lock locker(m_db_mutex);
    qint64 user_id =-1;
    SecurityManager sm;
    QSqlQuery find_id ;
    find_id.prepare("SELECT id, username FROM Users");

    if (!find_id.exec()) {
        qDebug() << "Failed to find  user id :" << find_id.lastError().text();
        return false;
    }
    while(find_id.next())
    {
        if(sm.decrypt(find_id.value(1).toString(),userName) == userName){
            user_id = find_id.value(0).toLongLong();
        }
    }


    if(user_id == -1){
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE Users SET password = :password WHERE id = :id");

    query.bindValue(":password", newPassword);
    query.bindValue(":id", user_id);

    if (!query.exec()) {
        qDebug() << "Failed to update user password:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() > 0) {
        qDebug() << "Password for user id" << user_id << "updated successfully.";
        return true;
    } else {
        qDebug() << "Update password failed: User with id" << user_id << "not found.";
        return false;
    }
}

bool DBM::updateUserEmail(qint64 user_id, const QString &newEmail)
{

    std::scoped_lock locker(m_db_mutex);

    QSqlQuery query;
    query.prepare("UPDATE Users SET email = :email WHERE id = :id");

    query.bindValue(":email", newEmail);
    query.bindValue(":id", user_id);

    if (!query.exec()) {
        qDebug() << "Failed to update user email:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() > 0) {
        qDebug() << "Email for user id" << user_id << "updated successfully.";
        return true;
    } else {
        qDebug() << "Update email failed: User with id" << user_id << "not found.";
        return false;
    }
}

// update
bool DBM::updatePlaylistName(const size_t playlist_id, const QString &newPlaylist_Name)
{
    std::scoped_lock locker(m_db_mutex);
    QSqlQuery update;
    update.prepare("UPDATE Playlists SET name = :new_name WHERE id = :id");
    update.bindValue(":new_name",newPlaylist_Name);
    update.bindValue(":id",static_cast<qint64>(playlist_id));

    if(!update.exec())
    {
        qDebug() << "Failed to update playlist name:" << update.lastError().text();
        return false;
    }
    if (update.numRowsAffected() > 0) {
        qDebug() << "Playlist with id" << playlist_id << "was successfully updated to" << newPlaylist_Name;
        return true;
    } else {
        qDebug() << "Tried to update playlist with id" << playlist_id << "but it was not found.";
        return false;
    }
}

bool DBM::isDbOpen()
{

    std::scoped_lock<QMutex>lock(m_db_mutex);
    return m_db.isOpen();
}

bool DBM::isUsernameUnique(const QString& Username)
{
    std::scoped_lock<QMutex>lock(m_db_mutex);

    QSqlQuery search;
    SecurityManager sm;
    search.prepare("SELECT username FROM Users");
    search.bindValue(":username",Username);

    if(!search.exec())
    {
        qDebug()<< "username search faild" << search.lastError().text();
        return false;
    }
    if(search.next())
    {
        if(sm.decrypt(search.value(0).toString(),Username) == Username){
            return false;
        }
    }

    return true;
}

bool DBM::verifySongsPath(const QString& path)
{

    QFileInfo fileInfo(path);

    if (!fileInfo.exists() || !fileInfo.isFile()) {
        return false;
    }

    QString suffix = fileInfo.suffix().toLower();
    if (suffix == "mp3" || suffix == "wav") {
        return true;
    }

    return false;
}

void DBM::applyVerifySongsPath()
{

    std::scoped_lock<QMutex>lock(m_db_mutex);
    // Select all songs

    QMap<qint64,QString> allsongs;
    QSqlQuery selectall;
    if(!selectall.exec("SELECT id , path FROM Songs"))
    {
        qDebug() << "can not select songs from db";
        throw std::runtime_error("can not select songs from db");
    }

    while(selectall.next())
    {
        allsongs.insert(selectall.value(0).toLongLong(),selectall.value(1).toString());
    }

    QList<qint64> idsToDelete;
    for(auto it = allsongs.begin();it != allsongs.end();it++)
    {
        if(!verifySongsPath(it.value()))
        {
            qDebug()<<"delete invalid path "<<it.value() ;
            idsToDelete.append(it.key());
        }
    }

    if(idsToDelete.isEmpty())
    {
        qDebug()<<"all paths is valid";
        return;
    }

    qDebug() << " delete paths : "<<idsToDelete.size();



    if (!m_db.transaction()) {
        qDebug() << "Failed to start transaction for deletion:" << m_db.lastError().text();
        return;
    }

    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM Songs WHERE id = ?");
    for (const qint64 id : idsToDelete) {
        deleteQuery.addBindValue(id);
        if (!deleteQuery.exec()) {
            qDebug() << "Failed to delete song with id" << id << ":" << deleteQuery.lastError().text();
            m_db.rollback();
            return;
        }
    }

    if (m_db.commit()) {
        qDebug() << "Successfully deleted all invalid entries";
    } else {
        qDebug() << "Failed to commit deletion transaction : " << m_db.lastError().text();
        m_db.rollback();
    }



}
