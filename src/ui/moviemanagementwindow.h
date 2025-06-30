#ifndef MOVIEMANAGEMENTWINDOW_H
#define MOVIEMANAGEMENTWINDOW_H

#include <QDialog>

namespace Ui {
class MovieManagementWindow;
}

class MovieManagementWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MovieManagementWindow(QWidget *parent = nullptr);
    ~MovieManagementWindow();

private:
    Ui::MovieManagementWindow *ui;
};

#endif // MOVIEMANAGEMENTWINDOW_H
