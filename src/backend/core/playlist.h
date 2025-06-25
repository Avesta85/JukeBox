#ifndef PLAYLIST_H
#define PLAYLIST_H


#include <QString>
#include <QDebug>
#include <Qvector>

class Playlist {
private:
    QString name;
    size_t playlistId;
    size_t owner_id;

public:
    Playlist(const QString& name);
    Playlist(const QString& name, size_t id , size_t owner_id);
    Playlist(const Playlist& other);

    ~Playlist();

    const QString& getName() const;
    const size_t& getId() const;

    void setName(const QString& NewName);
    void setPlaylistId(const size_t NewID);


};

#endif // PLAYLIST_H
