#include "./src/ui/mainwindow.h"
#include "./src/backend/db/DBM.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QThread* l = QThread::create(&DBM::get_instance);
    l->start();

    MainWindow w;

    w.show();

    return a.exec();
}
