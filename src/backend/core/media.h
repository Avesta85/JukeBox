#ifndef MEDIA_H
#define MEDIA_H
#include <iostream>
#include <QString>

class Media
{
private:
    QString path;
    QString name;
    size_t duration;

public:
    Media();
    Media(QString Name , QString Path , size_t Duration);
    Media(const Media& NewMedia);
    virtual ~Media() = 0;

    void setName(const QString& NewName);
    void setPath(const QString& NewPath);
    void setDuration(const size_t& NewDuration);

    const QString& getPath() const;
    const QString& getName() const;
    const size_t& getDuration() const;
};



#endif // MEDIA_H
