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
    this->SetupTable();
}

void DBM::SetupTable()
{
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
                          "Playlist_id INTEGER NOT NULL, "
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
                          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                          "user_id INTEGER NOT NULL, "
                          "name TEXT, "
                          "ip TEXT UNIQUE NOT NULL,"
                          "port INTEGER ,"
                          "FOREIGN KEY (user_id) REFERENCES Users (id) ON DELETE CASCADE "
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

bool DBM::isDbOpen() const
{
    return m_db.isOpen();
}
