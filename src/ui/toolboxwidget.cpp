#include "toolboxwidget.h"
#include "ui_toolboxwidget.h"

ToolBoxWidget::ToolBoxWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ToolBoxWidget)
{
    ui->setupUi(this);
}

ToolBoxWidget::~ToolBoxWidget()
{
    delete ui;
}
