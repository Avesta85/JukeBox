#ifndef SHOWKEYWORDS_H
#define SHOWKEYWORDS_H

#include <QDialog>
#include <QString>
#include <QMessageBox>

namespace Ui {
class ShowKeyWords;
}

class ShowKeyWords : public QDialog
{
    Q_OBJECT

public:
    explicit ShowKeyWords(QWidget *parent = nullptr);
    ~ShowKeyWords();

private:
    Ui::ShowKeyWords *ui;
};

#endif // SHOWKEYWORDS_H
