#include "src/backend/security/SecurityManager.h"
#include <src/backend/core/application.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Application::getInstance().Run();

    // SecurityManager sm;
    // qDebug()<< sm.encrypt("Avesta858","Avesta858");
    // qDebug()<<sm.decrypt("qCbw7PvI8M6lOOLbiMRCydGdRutR6AAxrzAj/u2hcpxCbBtpyQ==","Avesta858");
    return a.exec();
}

