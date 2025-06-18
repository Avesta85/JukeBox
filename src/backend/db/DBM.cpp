#include "./DBM.h"
#include "QtConcurrent/qtconcurrentrun.h"

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

    const QSet<QString> dbPath(dbSongs.begin(),dbSongs.end());
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

bool DBM::isDbOpen()
{

    std::scoped_lock<QMutex>lock(m_db_mutex);
    return m_db.isOpen();
}

bool DBM::isUsernameUnique(const QString& Username)
{
    std::scoped_lock<QMutex>lock(m_db_mutex);

    QSqlQuery search;

    search.prepare("SELECT EXISTS(SELECT 1 FROM Users WHERE username = :username");
    search.bindValue(":username",Username);

    if(!search.exec())
    {
        qDebug()<< "username search faild" << search.lastError().text();
        return false;
    }
    if(search.next())
    {
        bool exi = search.value(0).toBool();

        return !exi;
    }

    return false;
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
