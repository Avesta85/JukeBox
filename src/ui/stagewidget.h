#ifndef STAGEWIDGET_H
#define STAGEWIDGET_H
#include <QStackedWidget>

class playmusicwindow;
class MovieManagementWindow;

class QWidget;

namespace Ui {
class StageWidget;
}

class StageWidget : public QStackedWidget
{
    Q_OBJECT

public:
    explicit StageWidget(QWidget *parent = nullptr);
    ~StageWidget();

    playmusicwindow* getMusicManagementPage();

public slots:
    void showMusicManagementPage();
    void showMovieManagementPage();
    void showCoverArtPage();
    void showVideoPage();
private:
    Ui::StageWidget *ui;

    playmusicwindow* m_musicManagementPage;
    MovieManagementWindow* m_movieManagementPage;

    QWidget* m_musicPageContainer;
    QWidget* m_moviePageContainer;

};

#endif // STAGEWIDGET_H
