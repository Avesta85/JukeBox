#ifndef RECEIVESECUREWORDSWINDOW_H
#define RECEIVESECUREWORDSWINDOW_H

#include <QDialog>

namespace Ui {
class ReceiveSecureWordsWindow;
}

class ReceiveSecureWordsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReceiveSecureWordsWindow(QWidget *parent = nullptr);
    ~ReceiveSecureWordsWindow();

private:
    Ui::ReceiveSecureWordsWindow *ui;
};

#endif // RECEIVESECUREWORDSWINDOW_H
