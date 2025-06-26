#include "./src/ui/mainwindow.h"
#include "./src/backend/db/DBM.h"
#include <QApplication>
#include <src/backend/security/SecurityManager.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SecurityManager sm;

    QString test = "Avestagame858";
    auto hashed = sm.Hash(test);
    auto encrypted = sm.encrypt(test,"Avesta");
    auto decrypted = sm.decrypt(test,"Avesta");
    qDebug()<< hashed;
    qDebug()<< encrypted;
    qDebug()<< decrypted ;
    return a.exec();
}
