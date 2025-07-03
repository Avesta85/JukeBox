#include "dialog_friends.h"
#include "ui_dialog_friends.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QList>
#include <QPushButton>
#include <QLineEdit>
#include <QHeaderView>
#include <QString>
#include <Qt>
#include <QMessageBox>
#include "src/backend/core/UserManager.h"
#include "src/backend/core/person.h"

namespace {
QList<Person> filterPersons(const QList<Person>& list, const QString& search) {
    if (search.trimmed().isEmpty()) return list;
    QList<Person> filtered;
    for (const Person& p : list) {
        if (p.getUserName().contains(search, Qt::CaseInsensitive))
            filtered.append(p);
    }
    return filtered;
}
}

static QList<Person> s_allFriends;

Dialog_Friends::Dialog_Friends(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_Friends)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        QString searchText = ui->lineEdit_search_song->text();
        QList<Person> filtered;
        if (searchText.trimmed().isEmpty()) {
            filtered = m_allFriends;
        } else {
            for (const Person& p : m_allFriends) {
                if (p.getUserName().contains(searchText, Qt::CaseInsensitive))
                    filtered.append(p);
            }
        }
        fill_table(filtered);
    });
    connect(ui->pushButton_2, &QPushButton::clicked, this, [this]() {
        QList<Person> toDelete;
        for (int i = 0; i < ui->tableWidget_songs_of_playlist->rowCount(); ++i) {
            QTableWidgetItem* checkItem = ui->tableWidget_songs_of_playlist->item(i, 0);
            QTableWidgetItem* nameItem = ui->tableWidget_songs_of_playlist->item(i, 1);
            if (checkItem && checkItem->checkState() == Qt::Checked && nameItem) {
                for (const Person& p : m_allFriends) {
                    if (p.getUserName() == nameItem->text()) {
                        toDelete.append(p);
                        break;
                    }
                }
            }
        }
        if (!toDelete.isEmpty())
            emit delete_Friend(toDelete);
    });
    ui->tableWidget_songs_of_playlist->setColumnCount(2);
    QStringList headers; headers << "Select" << "Username";
    ui->tableWidget_songs_of_playlist->setHorizontalHeaderLabels(headers);
    ui->tableWidget_songs_of_playlist->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

Dialog_Friends::~Dialog_Friends()
{
    delete ui;
}

void Dialog_Friends::update_friend_view(const QList<Person> friendList)
{
    m_allFriends = friendList;
    fill_table(m_allFriends);
}

void Dialog_Friends::fill_table(const QList<Person>& list)
{
    ui->tableWidget_songs_of_playlist->clearContents();
    ui->tableWidget_songs_of_playlist->setRowCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        const Person& p = list[i];
        QTableWidgetItem* checkItem = new QTableWidgetItem();
        checkItem->setCheckState(Qt::Unchecked);
        QTableWidgetItem* nameItem = new QTableWidgetItem(p.getUserName());
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget_songs_of_playlist->setItem(i, 0, checkItem);
        ui->tableWidget_songs_of_playlist->setItem(i, 1, nameItem);
    }
    ui->tableWidget_songs_of_playlist->resizeColumnsToContents();
}


void Dialog_Friends::on_pushButton_3_clicked()
{
    QString friendName = ui->lineEdit_get_friend->text();

    if(friendName.isEmpty())
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "fill the your feiend name field", QMessageBox::Ok, this);
        msgBox.setWindowIcon(QIcon(":/icone/warning.png"));
        msgBox.setIconPixmap(QPixmap(":/icone/warning2.png"));
        msgBox.exec();

        return;
    }

    else
    {
        UserManager::getInstance().addFriend(friendName);
        ui->lineEdit_get_friend->clear();
        QMessageBox msgBox(QMessageBox::Information, "information",
                          "friend name added! pleae close friend name and open it again!",
                          QMessageBox::Ok,
                          this);

        msgBox.exec();
    }
}

