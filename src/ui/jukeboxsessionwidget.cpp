#include "jukeboxsessionwidget.h"
#include "ui_jukeboxsessionwidget.h"
#include "src/backend/network/sessionmanager.h"
#include "QMessageBox"

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
    emit createSessionClicked();
}

void JukeBoxSessionWidget::on_pushButton_join_session_clicked()
{
    showJoiningState();
}

void JukeBoxSessionWidget::on_pushButton_conect_clicked()
{

    if (m_isConnecting) return; // جلوگیری از کلیک‌های پیاپی

    QString ip = ui->lineEdit_ip_address->text().trimmed();
    if (ip.isEmpty()) {
        QMessageBox::warning(this, "خطا", "لطفاً آدرس IP را وارد کنید.");
        return;
    }

    m_isConnecting = true;
    ui->pushButton_conect->setEnabled(false);
    emit connectToHostClicked(ip);
    showJoiningState();
    ui->label_status->setText("⏳ در حال اتصال به میزبان...");

    // راه‌اندازی تایمر Timeout
    if (!m_connectionTimeoutTimer) {
        m_connectionTimeoutTimer = new QTimer(this);
        m_connectionTimeoutTimer->setSingleShot(true);
        connect(m_connectionTimeoutTimer, &QTimer::timeout, this, [this]() {
            if (m_isConnecting) {
                m_isConnecting = false;
                ui->pushButton_conect->setEnabled(true);
                ui->label_status->setText("⛔️ اتصال برقرار نشد. لطفاً بررسی کنید و دوباره تلاش کنید.");
                showDisconnectedState();
            }
        });
    }
    m_connectionTimeoutTimer->start(10000); // فرصت ۱۰ ثانیه‌ای
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
    showParticipantListState(); // یا showPage_session یا showConnectedState اگه این اسم‌ها رو انتخاب کردی
}

void JukeBoxSessionWidget::onConnectionFailed()
{
    m_isConnecting = false;
    ui->pushButton_conect->setEnabled(true);
    ui->label_status->setText("⛔️ اتصال برقرار نشد. لطفاً بررسی کنید و دوباره تلاش کنید.");
    showDisconnectedState();  // ← این تابع باید صفحه قطع‌شده رو نشون بده
}


