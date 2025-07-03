#ifndef TOOLBOXWIDGET_H
#define TOOLBOXWIDGET_H

#include <QWidget>

namespace Ui {
class ToolBoxWidget;
}

class ToolBoxWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBoxWidget(QWidget *parent = nullptr);
    ~ToolBoxWidget();


signals:

    void playlistManagementClicked();
    void FavoriteSongsClicked();
    void FriendsListClicked();
    void SongManagementClicked();
    void movieManagementClicked();
    void settingsClicked();
    void aboutUsClicked();
    void videoManagementClicked();
    void QueueClicked();
    void onlineManagmentClicked();
    void visualizerClicked();
    void coverArtClicke();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_favorite_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_Queue_clicked();
    void on_pushButton_online_clicked();


    void on_pushButton_vis_clicked();


private:

    void on_pushButton_playlist_management_clicked();
    void on_pushButton_movie_managment_clicked();
    void on_pushButton_setting_clicked();
    void on_pushButton_about_us_clicked();

    enum PlayerMode
    {
        movie,
        songC,
        songV
    };
private:
    Ui::ToolBoxWidget *ui;
    PlayerMode playerMode = songC;
};

#endif // TOOLBOXWIDGET_H

