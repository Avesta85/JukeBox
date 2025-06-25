#ifndef MOVIE_H
#define MOVIE_H
#include <iostream>
#include "media.h"
#include <QString>

class Movie final : public Media
{
private:
    QString director;
public:
    Movie();
    Movie(QString Name , QString Path , size_t Duration , QString Director);
    Movie(const Movie& OtherMovie);
    ~Movie() override;

    void setDirector(const QString NewDirector);

    const QString& getDirector();
};

#endif // MOVIE_H
