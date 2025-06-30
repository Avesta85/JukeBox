#ifndef PLAYERCONTROLWIDGET_H
#define PLAYERCONTROLWIDGET_H

#include <QWidget>

namespace Ui {
class PlayerControlWidget;
}

class PlayerControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerControlWidget(QWidget *parent = nullptr);
    ~PlayerControlWidget();

private slots:
    void on_pushButton_play_clicked();

    void on_pushButton_playing_type_clicked();

    void on_pushButton_mute_clicked();

    void on_horizontalSlider_volume_valueChanged(int value);

private:
    Ui::PlayerControlWidget *ui;

    enum PlayOrPause
    {
        play,
        pause
    };
    enum PlatingType
    {
        shuffle,
        repeatOne,
        repeatPlayList
    };

    enum ValumeStatus
    {
        mute,
        volume
    };

    PlayOrPause playStatus = pause;
    PlatingType playingTipe = repeatOne;
    ValumeStatus valumeStatus = volume;
};

#endif // PLAYERCONTROLWIDGET_H
