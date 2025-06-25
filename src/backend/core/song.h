#ifndef SONG_H
#define SONG_H
#include <iostream>
#include "media.h"

class Song final : public Media
{
private:
    QString artist;
    size_t id;
public:
    Song();
    Song(QString Name , QString Path , size_t Duration , QString Artist , size_t ID);
    Song(const Song& OtherSong);
    ~Song() override;

    void setArtist(const QString& NewArtist);
    void setID(const size_t& NewID);

    const QString& getArtist();
    const size_t& getID();
};

#endif // SONG_H
