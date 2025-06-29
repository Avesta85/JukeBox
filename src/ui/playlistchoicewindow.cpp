#include "playlistchoicewindow.h"
#include "ui_playlistchoicewindow.h"

PlaylistChoiceWindow::PlaylistChoiceWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlaylistChoiceWindow)
{
    ui->setupUi(this);
}

PlaylistChoiceWindow::~PlaylistChoiceWindow()
{
    delete ui;
}
