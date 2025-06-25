#include "choicewindow.h"
#include "ui_choicewindow.h"
#include <QScreen>
#include <QApplication>
#include <QDebug>

ChoiceWindow::ChoiceWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChoiceWindow)
{
    ui->setupUi(this);
    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        setGeometry(screenGeometry);
        // not bad for test //qDebug() << "Screen Geometry: " << screenGeometry;
    } else {
        qDebug() << "Could not get primary screen information. Dialog might not be full screen.";
    }
}

ChoiceWindow::~ChoiceWindow()
{
    delete ui;
}
