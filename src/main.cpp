#include "./src/ui/mainwindow.h"
#include "ui/choicewindow.h"
#include "ui/loginwindow.h"
#include "ui/signupwindow.h"
#include "ui/frogotpasswordwindow.h"
#include "./src/backend/db/DBM.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //MainWindow w;
    //w.show();
    ChoiceWindow choiceWin;
    // tests for checking the dailogs
    //choiceWin.showMaximized();
    //LoginWindow loginWin;
    //loginWin.showMaximized();
    //SignupWindow signupWin;
    //signupWin.showMaximized();
    //FrogotPasswordWindow forgotpasswordWin;
    //forgotpasswordWin.showMaximized();
    QThread* l = QThread::create(&DBM::get_instance);
    l->start();
    MainWindow w;
    w.show();

    return a.exec();
}
