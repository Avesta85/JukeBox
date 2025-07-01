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



public slots:
    void updatePlaybackState(bool isPlaying);
    void updateVolume(int volume);
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void setMuted(bool isMuted);
    void setRepeatIcon(const QIcon& icon);


signals:
    void playPauseClicked();
    void nextClicked();
    void previousClicked();
    void volumeChanged(int volume);
    void seeked(qint64 position);
    void shuffleClicked();
    void repeatModeClicked();
    void muteClicked(bool isMuted);

private slots:
    void handlePlayClicked();
    void handleMuteClicked();

    void on_pushButton_playing_type_clicked();

    //void on_pushButton_mute_clicked();

    void on_horizontalSlider_volume_valueChanged(int value);

    void on_pushButton_next_clicked();
    void on_pushButton_previous_clicked();
    void on_horizontalSlider_timeline_sliderMoved(int position);

private:
    Ui::PlayerControlWidget *ui;

    bool m_isplaying;
    bool m_isMuted;
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
