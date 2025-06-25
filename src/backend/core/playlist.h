#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "src/backend/core/song.h"
#include <QString>
#include <QDebug>
#include <vector>
#include "media.h"
#include <algorithm>

class Playlist {
private:
    QString name;
    size_t playlistId;
    std::vector<Media*> mediaItems;
    int currentMediaIndex;

public:
    Playlist(const QString& name);
    Playlist(const QString& name, size_t id);
    Playlist(const Playlist& other);

    ~Playlist();

    const QString& getName() const;
    const size_t& getId() const;
    const int& size() const;
    bool isEmpty() const;

    void setName(const QString& NewName);
    void setPlaylistId(const size_t NewID);
    void setCurrentIndex(const int& newCurrIndex);
    void setNewListOfMedia(const std::vector<Media*>& NewMediaList);

    void addMedia(Media* const media);
    bool removeMedia(Media* media); // deleting special media
    bool removeMediaAt(int index); // deleting special index
    void clear(); // clear all items

    Media* getCurrentMedia() const;
    Media* nextMedia(); // goint to next media
    Media* prevMedia();
    void setCurrentMedia(Media* media); // set the current media base on special media
    void setCurrentMediaByIndex(int index);
};

#endif // PLAYLIST_H
