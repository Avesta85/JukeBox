#ifndef PLAYLISTCHOICEWINDOW_H
#define PLAYLISTCHOICEWINDOW_H

#include <QDialog>

namespace Ui {
class PlaylistChoiceWindow;
}

class PlaylistChoiceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlaylistChoiceWindow(QWidget *parent = nullptr);
    ~PlaylistChoiceWindow();

private:
    Ui::PlaylistChoiceWindow *ui;
};

#endif // PLAYLISTCHOICEWINDOW_H
