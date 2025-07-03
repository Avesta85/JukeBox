#include "jukeboxsessionwidget.h"
#include "ui_jukeboxsessionwidget.h"
#include "src/backend/network/sessionmanager.h"
#include "QMessageBox"
#include <QNetworkInterface>

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

void JukeBoxSessionWidget::showParticipantListState()
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

void JukeBoxSessionWidget::enableConnectButten()
{
    ui->pushButton_conect->setEnabled(false);
}

void JukeBoxSessionWidget::on_pushButton_create_session_clicked()
{
    // Show user's IP
    QString ip;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (const QHostAddress &address : ipAddressesList) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress::LocalHost) {
            ip = address.toString();
            break;
        }
    }
    if (ip.isEmpty())
        ip = QHostAddress(QHostAddress::LocalHost).toString();
    QMessageBox::information(this, "Your IP Address", "Your IP for sharing: " + ip);
    emit createSessionClicked();
}

void JukeBoxSessionWidget::on_pushButton_join_session_clicked()
{
    showJoiningState();
}

void JukeBoxSessionWidget::on_pushButton_conect_clicked()
{
    if (m_isConnecting) return;
    QString ip = ui->lineEdit_ip_address->text().trimmed();
    if (ip.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter the IP address.");
        return;
    }
    m_isConnecting = true;
    ui->pushButton_conect->setEnabled(false);
    emit connectToHostClicked(ip);
    showJoiningState();
    ui->label_status->setText("⏳ Connecting to host...");
    if (!m_connectionTimeoutTimer) {
        m_connectionTimeoutTimer = new QTimer(this);
        m_connectionTimeoutTimer->setSingleShot(true);
        connect(m_connectionTimeoutTimer, &QTimer::timeout, this, [this]() {
            if (m_isConnecting) {
                m_isConnecting = false;
                ui->pushButton_conect->setEnabled(true);
                ui->label_status->setText("⛔️ Connection failed. Please check and try again.");
                showDisconnectedState();
            }
        });
    }
    m_connectionTimeoutTimer->start(10000);
}

void JukeBoxSessionWidget::resetJoinForm()
{
    if (m_connectionTimeoutTimer)
        m_connectionTimeoutTimer->stop();

    m_isConnecting = false;
    ui->pushButton_conect->setEnabled(true);
    ui->lineEdit_ip_address->clear();
    ui->label_status->clear();
    showDisconnectedState();
}


void JukeBoxSessionWidget::on_pushButton_backe_clicked()
{
    resetJoinForm();
}

void JukeBoxSessionWidget::on_pushButton_leave_clicked()
{
    emit leaveSessionClicked();
}

bool& JukeBoxSessionWidget::getConnecting()
{
    return m_isConnecting;
}

void JukeBoxSessionWidget::onConnectionSuccess(const QList<Person>& persons)
{
    QStringList list;
    for (const Person& p : persons)
        list << p.getUserName();
    updateParticipantList(list);
    ui->pushButton_conect->setEnabled(true);
    showParticipantListState();
}

void JukeBoxSessionWidget::onConnectionFailed()
{
    m_isConnecting = false;
    ui->pushButton_conect->setEnabled(true);
    ui->label_status->setText("⛔️ Connection failed. Please check and try again.");
    showDisconnectedState();
}


