
#include "./src/ui/mainwindow.h"
#include "ui/choicewindow.h"
#include "ui/loginwindow.h"
#include "ui/signupwindow.h"
#include "ui/frogotpasswordwindow.h"
#include "ui/emailverificationwindow.h"
#include "ui/receivesecurewordswindow.h"
#include "ui/changepasswordwindow.h"
#include "ui/settingdialog.h"
#include "ui/musicplayerview.h"
#include "./src/backend/db/DBM.h"
#include "src/backend/core/SendEmail.h"
#include <QApplication>
#include <src/backend/security/SecurityManager.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); 
    MainWindow w;
    //w.show();
    //SecurityManager sm;

    //qDebug()<< sm.securityKey_gen();

    //bool p = sendMail("mohammad1384esf@gmail.com" , "--useremail--" , "1234");
    ChoiceWindow choiceWin;
    choiceWin.showMaximized();
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
    //changepasswing.showMaximized();

    return a.exec();
}
