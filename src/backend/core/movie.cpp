#include "movie.h"

Movie::Movie() : Media() {}
Movie::Movie(QString Name , QString Path , size_t Duration , QString Director) : Media(Name , Path , Duration) , director(Director) {}
Movie::Movie(const Movie& OtherMovie) : Media(OtherMovie) , director(OtherMovie.director) {}

Movie::~Movie() {}

void Movie::setDirector(const QString NewDirector)
{this->director = NewDirector;}

const QString& Movie::getDirector()
{return this->director;}
