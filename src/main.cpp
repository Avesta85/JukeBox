#include <src/backend/core/application.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Application::getInstance().Run();
    return a.exec();
}

