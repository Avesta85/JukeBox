#include "musicplayerview.h"
#include "ui_musicplayerview.h"

MusicPlayerView::MusicPlayerView(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MusicPlayerView)
{
    ui->setupUi(this);
}

MusicPlayerView::~MusicPlayerView()
{
    delete ui;
}
