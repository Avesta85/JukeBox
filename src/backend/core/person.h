#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include <Qstring>
#include <stdexcept>
#include <memory>
#include "song.h"
#include "movie.h"



class Person
{


private :
    QString m_userName;

public:
    Person();
    Person(QString userName);

    void setUserName(QString m_userName);

    QString getUserName()noexcept;

    ~Person();
};

#endif // PERSON_H
