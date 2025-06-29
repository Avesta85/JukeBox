#include "playercontrolwidget.h"
#include "ui_playercontrolwidget.h"

PlayerControlWidget::PlayerControlWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayerControlWidget)
{
    ui->setupUi(this);
}

PlayerControlWidget::~PlayerControlWidget()
{
    delete ui;
}
