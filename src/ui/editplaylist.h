#ifndef EDITPLAYLIST_H
#define EDITPLAYLIST_H

#include <QDialog>

namespace Ui {
class EditPlayList;
}

class EditPlayList : public QDialog
{
    Q_OBJECT

public:
    explicit EditPlayList(QWidget *parent = nullptr);
    ~EditPlayList();

private:
    Ui::EditPlayList *ui;
};

#endif // EDITPLAYLIST_H
