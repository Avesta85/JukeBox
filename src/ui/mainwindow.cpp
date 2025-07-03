#include "mainwindow.h"
#include "src/ui/jukeboxsessionwidget.h"
#include "ui_mainwindow.h"
#include "src/backend/network/sessionmanager.h"
#include "src/backend/core/playermanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("JukeBox");
    this->setWindowIcon(QIcon(":/icone/musicplayer.png"));

    onlineDock = new QDockWidget("Online", this);
    onlineStack = new QStackedWidget(onlineDock);
    onlineDock->setWidget(onlineStack);
    addDockWidget(Qt::RightDockWidgetArea, onlineDock);

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


QDockWidget &MainWindow::getDock() const
{
    return *onlineDock;
}

QStackedWidget &MainWindow::getStack() const
{
    return *onlineStack;
}

void MainWindow::updateSongInfo(const Song& song)
{

    if (song.getPath() != "") {

        this->setWindowTitle(QString("JukeBox - %1 ").arg(song.getName()));
    } else {
        this->setWindowTitle("JukeBox");
    }
}
