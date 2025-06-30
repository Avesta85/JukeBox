#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("JukeBox");
    this->setWindowIcon(QIcon(":/icone/musicplayer.png"));

    ui->dockWidget_juckbox_session_placeholder->setWindowTitle("JukeBox Session");
}

MainWindow::~MainWindow()
{
    delete ui;
}

PlayerControlWidget *MainWindow::getPlayerControls() const
{
   return  ui->widget_playercontrol_placeholder;
}

ToolBoxWidget* MainWindow::getToolBox() const
{
    return ui->widget_toolbox_placeholder;
}
StageWidget* MainWindow::getStage() const
{
    return ui->stackedWidget_video_placholder;
}

JukeBoxSessionWidget* MainWindow::getJukeBoxSession() const
{
    return ui->dockWidget_juckbox_session_placeholder;
}

void MainWindow::updateSongInfo(const Song& song)
{
    if (song.getPath() != "") {

        this->setWindowTitle(QString("JukeBox - %1 ").arg(song.getName()));
    } else {
        this->setWindowTitle("JukeBox");
    }
}
