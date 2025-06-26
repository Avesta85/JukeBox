#include "./src/ui/mainwindow.h"
#include "ui/choicewindow.h"
#include "ui/loginwindow.h"
#include "ui/signupwindow.h"
#include "ui/frogotpasswordwindow.h"
#include "ui/emailverificationwindow.h"
#include "ui/receivesecurewordswindow.h"
#include "ui/changepasswordwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    ChoiceWindow choiceWin;
    //choiceWin.showMaximized();
    LoginWindow loginWin;
    //loginWin.showMaximized();
     SignupWindow signupWin;
    //signupWin.showMaximized();
    FrogotPasswordWindow forgotpasswordWin;
    //forgotpasswordWin.showMaximized();
    EmailVerificationWindow emailverificwin;
    //emailverificwin.showMaximized();
    ReceiveSecureWordsWindow receivesecurwin;
    //receivesecurwin.showMaximized();
    ChangePasswordWindow changepasswing;
    changepasswing.showMaximized();
    return a.exec();
}
