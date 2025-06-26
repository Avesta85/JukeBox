#include "./src/ui/mainwindow.h"
#include "./src/backend/db/DBM.h"
#include <QApplication>
#include <src/backend/security/SecurityManager.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SecurityManager sm;

    qDebug()<< sm.securityKey_gen();

    return a.exec();
}
