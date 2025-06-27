#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(QObject *parent = nullptr);

signals:
};

#endif // APPLICATION_H
