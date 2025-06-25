#include "./src/ui/mainwindow.h"
#include "ui/choicewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    ChoiceWindow choiceWin;
    //choiceWin.show();
    return a.exec();
}
