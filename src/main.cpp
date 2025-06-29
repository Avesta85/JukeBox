#include "src/backend/security/SecurityManager.h"
#include <src/backend/core/application.h>
#include "src/ui/createdialog.h"
#include "src/ui/deleteplaylist.h"
#include "src/ui/playercontrolwidget.h"
#include "src/ui/playlistchoicewindow.h"
#include "src/ui/playmusicwindow.h"
#include "src/ui/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

   // Application::getInstance().Run();

    //CreateDialog d;
    //d.show();

    MainWindow m;
    m.showMaximized();
    //PlayerControlWidget q;
    //q.show();
  //  DeletePlayList k;
   // k.show();

    // SecurityManager sm;
    // qDebug()<< sm.encrypt("Avesta858","Avesta858");
    // qDebug()<<sm.decrypt("qCbw7PvI8M6lOOLbiMRCydGdRutR6AAxrzAj/u2hcpxCbBtpyQ==","Avesta858");
    return a.exec();
}

