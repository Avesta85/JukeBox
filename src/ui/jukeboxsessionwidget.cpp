#include "jukeboxsessionwidget.h"
#include "ui_jukeboxsessionwidget.h"

JukeBoxSessionWidget::JukeBoxSessionWidget(QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::JukeBoxSessionWidget)
{
    ui->setupUi(this);

    ui->tableWidget_list_of_presents->setColumnCount(1);
    ui->tableWidget_list_of_presents->setHorizontalHeaderLabels({"Username"});
    ui->tableWidget_list_of_presents->horizontalHeader()->setStretchLastSection(true);

    showDisconnectedState();
}

JukeBoxSessionWidget::~JukeBoxSessionWidget()
{
    delete ui;
}
void JukeBoxSessionWidget::showDisconnectedState()
{
    ui->stackedWidget->setCurrentWidget(ui->page_disconect);
}
void JukeBoxSessionWidget::showJoiningState()
{

    ui->stackedWidget->setCurrentWidget(ui->page_joining);
}
void JukeBoxSessionWidget::showConnectedState()
{

    ui->stackedWidget->setCurrentWidget(ui->page_conected);
}

void JukeBoxSessionWidget::updateParticipantList(const QStringList& participants)
{

    ui->tableWidget_list_of_presents->setRowCount(0);


    for (const QString& username : participants) {
        int newRow = ui->tableWidget_list_of_presents->rowCount();
        ui->tableWidget_list_of_presents->insertRow(newRow);
        ui->tableWidget_list_of_presents->setItem(newRow, 0, new QTableWidgetItem(username));
    }
}
void JukeBoxSessionWidget::on_pushButton_create_session_clicked()
{

    emit createSessionClicked();
}

void JukeBoxSessionWidget::on_pushButton_join_session_clicked()
{
    showJoiningState();
}

void JukeBoxSessionWidget::on_pushButton_conect_clicked()
{
    QString ipAddress = ui->lineEdit_ip_address->text();
    emit connectToHostClicked(ipAddress);
}

void JukeBoxSessionWidget::on_pushButton_backe_clicked()
{
    showDisconnectedState();
}

void JukeBoxSessionWidget::on_pushButton_leave_clicked()
{

    emit leaveSessionClicked();
}
