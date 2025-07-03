#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QComboBox>
#include "src/ui/mainwindow.h"
#include "src/ui/choicewindow.h"
#include "src/backend/core/Application.h"
#include "src/backend/core/Visualizer.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Application::getInstance().Run();

    return app.exec();
}

