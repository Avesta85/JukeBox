#include "media.h"

Media::Media() : path("") , name("") , duration(0) {}

Media::Media(QString Name , QString Path , size_t Duration) : name(Name) , path(Path) , duration(Duration){}

Media::Media(const Media& OtherMedia)
{
    this->name = OtherMedia.name;
    this->path = OtherMedia.path;
    this->duration = OtherMedia.duration;
}

Media::~Media(){}

void Media::setName(const QString& NewName)
{this->name = NewName;}

void Media::setPath(const QString& NewPath)
{this->path = NewPath;}

void Media::setDuration(const size_t& NewDuration)
{this->duration = NewDuration;}

const QString& Media::getPath() const
{return this->path;}

const QString& Media::getName() const
{return this->name;}

const size_t& Media::getDuration() const
{return this->duration;}
