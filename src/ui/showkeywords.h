#ifndef SHOWKEYWORDS_H
#define SHOWKEYWORDS_H

#include <QDialog>

namespace Ui {
class ShowKeyWords;
}

class ShowKeyWords : public QDialog
{
    Q_OBJECT

public:
    explicit ShowKeyWords(QWidget *parent = nullptr);
    ~ShowKeyWords();
    void set_key(QString key);

private:
    Ui::ShowKeyWords *ui;
    QString key;

signals:
    void signup_successfully();

private slots:
    void on_pushButton_clicked();
};

#endif // SHOWKEYWORDS_H
