#include "./src/ui/mainwindow.h"
#include "./src/backend/db/DBM.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DBM::get_instance();

    MainWindow w;

    w.show();

    return a.exec();
}
