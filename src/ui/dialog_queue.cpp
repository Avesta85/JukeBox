#include "dialog_queue.h"
#include "ui_dialog_queue.h"
#include <QTableWidgetItem>
#include <QCheckBox>

Dialog_Queue::Dialog_Queue(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_Queue)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &Dialog_Queue::on_pushButton_clicked); // search
    connect(ui->pushButton_play, &QPushButton::clicked, this, &Dialog_Queue::on_pushButton_play_clicked);
}

Dialog_Queue::~Dialog_Queue()
{
    delete ui;
}

void Dialog_Queue::show_songs_list(const QList<Song> list)
{
    m_allSongs = list;
    m_filteredSongs = list;
    fill_table(m_filteredSongs);
}

void Dialog_Queue::fill_table(const QList<Song>& list)
{
    ui->tableWidget_songs_of_playlist->clear();
    ui->tableWidget_songs_of_playlist->setRowCount(list.size());
    ui->tableWidget_songs_of_playlist->setColumnCount(2);
    QStringList headers;
    headers << "select" << "name";
    ui->tableWidget_songs_of_playlist->setHorizontalHeaderLabels(headers);
    for (int i = 0; i < list.size(); ++i) {
        QTableWidgetItem* nameItem = new QTableWidgetItem(list[i].getName());
        ui->tableWidget_songs_of_playlist->setItem(i, 1, nameItem);
        QTableWidgetItem* checkItem = new QTableWidgetItem();
        checkItem->setCheckState(Qt::Unchecked);
        ui->tableWidget_songs_of_playlist->setItem(i, 0, checkItem);
    }
    ui->tableWidget_songs_of_playlist->resizeColumnsToContents();
}

void Dialog_Queue::on_pushButton_clicked()
{
    QString searchText = ui->lineEdit_search_song->text().trimmed();
    if (searchText.isEmpty()) {
        m_filteredSongs = m_allSongs;
    } else {
        m_filteredSongs.clear();
        for (const Song& s : m_allSongs) {
            if (s.getName().contains(searchText, Qt::CaseInsensitive))
                m_filteredSongs.append(s);
        }
    }
    fill_table(m_filteredSongs);
}

void Dialog_Queue::on_pushButton_play_clicked()
{
    QList<qint64> checkedIds;
    for (int i = 0; i < ui->tableWidget_songs_of_playlist->rowCount(); ++i) {
        QTableWidgetItem* checkItem = ui->tableWidget_songs_of_playlist->item(i, 0);
        if (checkItem && checkItem->checkState() == Qt::Checked) {
            // find song id from filtered list
            if (i < m_filteredSongs.size())
                checkedIds.append(m_filteredSongs[i].getID());
        }
    }
    emit play_Queue(checkedIds);
}

