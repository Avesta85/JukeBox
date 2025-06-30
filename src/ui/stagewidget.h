#ifndef STAGEWIDGET_H
#define STAGEWIDGET_H
#include <QStackedWidget>

class playmusicwindow;
class MovieManagementWindow;

namespace Ui {
class StageWidget;
}

class StageWidget : public QStackedWidget
{
    Q_OBJECT

public:
    explicit StageWidget(QWidget *parent = nullptr);
    ~StageWidget();


public slots:
    void showMusicManagementPage();
    void showMovieManagementPage();
    void showCoverArtPage();
    void showVideoPage();
private:
    Ui::StageWidget *ui;

    playmusicwindow* m_musicManagementPage;
    MovieManagementWindow* m_movieManagementPage;
};

#endif // STAGEWIDGET_H
