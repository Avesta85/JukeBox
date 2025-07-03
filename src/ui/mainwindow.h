#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qstackedwidget.h"
#include "src/backend/core/song.h"
#include "src/ui/playmusicwindow.h"
#include <QMainWindow>



class PlayerControlWidget;
class ToolBoxWidget;
class StageWidget;
class JukeBoxSessionWidget;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    PlayerControlWidget* getPlayerControls() const;
    ToolBoxWidget* getToolBox() const;
    StageWidget* getStage() const;
    JukeBoxSessionWidget* getJukeBoxSession() const;

    QDockWidget& getDock()const;
    QStackedWidget& getStack()const;

public slots:
    void updateSongInfo(const Song& song);

private:
    Ui::MainWindow *ui;


    QDockWidget* onlineDock;
    QStackedWidget* onlineStack;
};
#endif // MAINWINDOW_H
