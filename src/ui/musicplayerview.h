#ifndef MUSICPLAYERVIEW_H
#define MUSICPLAYERVIEW_H

#include <QDialog>

namespace Ui {
class MusicPlayerView;
}

class MusicPlayerView : public QDialog
{
    Q_OBJECT

public:
    explicit MusicPlayerView(QWidget *parent = nullptr);
    ~MusicPlayerView();

private:
    Ui::MusicPlayerView *ui;
};

#endif // MUSICPLAYERVIEW_H
