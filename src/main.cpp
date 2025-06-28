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
#include <QApplication>
#include <src/backend/security/SecurityManager.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); 
    MainWindow w;
    //w.show();

    SecurityManager sm;

    qDebug()<< sm.securityKey_gen();

    return a.exec();
}
