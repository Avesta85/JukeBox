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

class DBM final
{


public:

    //C&D
    ~DBM();
    static DBM& get_instance();

    //song sync

    QMap<QString,QString> scanDiskForSongs(const QString& folder_path) const; // read disk and create {path , songs_name } map

    void applySyncChanges(const QMap<QString,QString>& diskSongs); // update songs table from pram: disksongs


    //bool type

    bool isDbOpen()const;


private:
    // Static Private Variable
    static std::unique_ptr<DBM> s_instance;

    static const QString s_db_path;

    static const QString s_db_name;



    // Member Private Variable

    QString m_db_songs_folder;

    QSqlDatabase m_db;
    // Private Function

    // --Static

    // --Member
    DBM();

    void SetupDb();

    void SetupTable();

    void fineMusicDir();

};

#endif // DBM_H
