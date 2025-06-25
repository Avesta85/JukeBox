#include "choicewindow.h"
#include "ui_choicewindow.h"
#include <QScreen>
#include <QApplication>
#include <QDebug>

Choicewindow::Choicewindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Choicewindow)
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

Choicewindow::~Choicewindow()
{
    delete ui;
}
