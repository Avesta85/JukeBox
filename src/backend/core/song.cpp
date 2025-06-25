#include "song.h"

Song::Song() : Media() {}
Song::Song(QString Name , QString Path , size_t Duration , QString Artist , size_t ID) : Media(Name , Path , Duration) , artist(Artist) , id(ID) {}
Song::Song(const Song& OtherSong) : Media(OtherSong) , artist(OtherSong.artist) , id(OtherSong.id) {}
Song::~Song(){}

void Song::setArtist(const QString& NewArtist)
{this->artist = NewArtist; }

void Song::setID(const size_t& NewID)
{this->id = NewID; }

const QString& Song::getArtist()
{return this->artist; }

const size_t& Song::getID()
{return this->id; }
