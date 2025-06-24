#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include <Qstring>
#include <stdexcept>
#include <memory>
#include "song.h"
#include "movie.h"


class Person : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString m_userName READ getUserName WRITE setUserName NOTIFY userNameChanged)

private :
    QString m_userName;

public:
    Person(QObject *parrent = nullptr);
    Person(QString userName, QObject *parrent = nullptr);

    void setUserName(QString m_userName);

    QString getUserName()noexcept;

    ~Person();

signals:

    void userNameChanged();
};

#endif // PERSON_H
