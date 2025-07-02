#ifndef DIALOG_FRIENDS_H
#define DIALOG_FRIENDS_H

#include "src/backend/core/person.h"
#include <QDialog>

namespace Ui {
class Dialog_Friends;
}

class Dialog_Friends : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Friends(QWidget *parent = nullptr);
    ~Dialog_Friends();

public slots:

    void update_friend_view(const QList<Person> friendList);

signals:

    void delete_Friend(const QList<Person> deletedPerson);
private:
    Ui::Dialog_Friends *ui;
    QList<Person> m_allFriends;
    void fill_table(const QList<Person>& list);
};

#endif // DIALOG_FRIENDS_H
