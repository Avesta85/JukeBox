#include "playlist.h"



Playlist::Playlist(const QString& name) : name(name), playlistId(-1)
{
    qDebug() << "Playlist created:" << name;
}

Playlist::Playlist(const QString& name, size_t id,size_t ownerId) : name(name), playlistId(id),owner_id(ownerId)
{
    qDebug() << "Playlist created:" << name << "with ID:" << id;
}

Playlist::Playlist(const Playlist& other) : name(other.name), playlistId(other.playlistId)
{
    // these are shallow copy , if you want to have a sprated source must use deep copy wich is not commen

    qDebug() << "Playlist copy constructor called for:" << name;
}

Playlist::~Playlist()
{
    qDebug() << "Playlist destructor called for:" << name;

}

void Playlist::setName(const QString& NewName)
{this->name = NewName;}

void Playlist::setPlaylistId(const size_t NewID)
{this->playlistId = NewID;}


const QString& Playlist::getName() const
{return this->name;}

const size_t& Playlist::getId() const
{return this->playlistId;}
