#include "stagewidget.h"
#include "ui_stagewidget.h"

StageWidget::StageWidget(QWidget *parent)
    : QStackedWidget(parent)
    , ui(new Ui::StageWidget)
{
    ui->setupUi(this);
}

StageWidget::~StageWidget()
{
    delete ui;
}
