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

void ToolBoxWidget::on_pushButton_playlist_management_clicked()
{
    emit playlistManagementClicked();
}

void ToolBoxWidget::on_pushButton_movie_managment_clicked()
{
    emit movieManagementClicked();
}

void ToolBoxWidget::on_pushButton_setting_clicked()
{
    emit settingsClicked();
}

void ToolBoxWidget::on_pushButton_about_us_clicked()
{
    emit aboutUsClicked();
}
