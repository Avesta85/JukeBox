#include "settingdialog.h"
#include <QVBoxLayout> // برای چیدمان عمودی
#include <QHBoxLayout> // برای چیدمان افقی
#include <QFormLayout> // برای چیدمان فرم (برچسب و فیلد ورودی)
#include <QLabel>
#include <QGroupBox> // برای گروه‌بندی ویجت‌ها

#include "localuser.h"
#include "accountmanager.h"
#include "src/backend/db/DBM.h"
#include "src/backend/core/Application.h"

SettingDialog::SettingDialog(LocalUser* currentUser,
                             AccountManager* accountManager,
                             DatabaseManager* dbManager,
                             ApplicationManager* appManager,
                             QWidget* parent)
    : QDialog(parent),
    currentUser(currentUser),
    accountManager(accountManager),
    dbManager(dbManager),
    appManager(appManager)
{
    Q_ASSERT(currentUser);
    Q_ASSERT(accountManager);
    Q_ASSERT(dbManager);
    Q_ASSERT(appManager);

    setupUi();           // طراحی رابط کاربری
    loadSettings();      // بارگذاری اطلاعات فعلی
    connectSignalsSlots(); // اتصال سیگنال‌ها به اسلات‌ها
    setWindowTitle("Settings"); // تنظیم عنوان پنجره
}

SettingDialog::~SettingDialog()
{
}


void SettingDialog::setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QGroupBox* profileGroup = new QGroupBox("User Profile", this);
    QFormLayout* profileLayout = new QFormLayout(profileGroup);
    nameLineEdit = new QLineEdit(this);
    lastNameLineEdit = new QLineEdit(this);
    emailLineEdit = new QLineEdit(this);

    profileLayout->addRow("Name:", nameLineEdit);
    profileLayout->addRow("Last Name:", lastNameLineEdit);
    profileLayout->addRow("Email:", emailLineEdit);
    mainLayout->addWidget(profileGroup);

    QGroupBox* passwordGroup = new QGroupBox("Change Password", this);
    QFormLayout* passwordLayout = new QFormLayout(passwordGroup);
    currentPasswordLineEdit = new QLineEdit(this);
    currentPasswordLineEdit->setEchoMode(QLineEdit::Password); // برای مخفی کردن رمز
    newPasswordLineEdit = new QLineEdit(this);
    newPasswordLineEdit->setEchoMode(QLineEdit::Password);
    confirmNewPasswordLineEdit = new QLineEdit(this);
    confirmNewPasswordLineEdit->setEchoMode(QLineEdit::Password);

    passwordLayout->addRow("Current Password:", currentPasswordLineEdit);
    passwordLayout->addRow("New Password:", newPasswordLineEdit);
    passwordLayout->addRow("Confirm New Password:", confirmNewPasswordLineEdit);
    changePasswordButton = new QPushButton("Change Password", this);
    passwordLayout->addRow(changePasswordButton);
    mainLayout->addWidget(passwordGroup);

    QGroupBox* appSettingsGroup = new QGroupBox("Application Settings", this);
    QFormLayout* appSettingsLayout = new QFormLayout(appSettingsGroup);
    defaultMusicFolderPathLineEdit = new QLineEdit(this);
    browseMusicFolderButton = new QPushButton("Browse...", this);
    QHBoxLayout* folderLayout = new QHBoxLayout();
    folderLayout->addWidget(defaultMusicFolderPathLineEdit);
    folderLayout->addWidget(browseMusicFolderButton);
    appSettingsLayout->addRow("Default Music Folder:", folderLayout);

    visualizerEnabledCheckBox = new QCheckBox("Enable Visualizer", this);
    appSettingsLayout->addRow(visualizerEnabledCheckBox);
    mainLayout->addWidget(appSettingsGroup);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    saveButton = new QPushButton("Save Changes", this);
    logoutButton = new QPushButton("Logout", this);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addStretch(); // فاصله انداز برای چپ و راست شدن دکمه‌ها
    buttonLayout->addWidget(logoutButton);
    mainLayout->addLayout(buttonLayout);
}

void SettingDialog::loadSettings()
{
    nameLineEdit->setText(currentUser->getName());
    lastNameLineEdit->setText(currentUser->getLastName());
    emailLineEdit->setText(currentUser->getEmail());

    QString musicFolderPath = dbManager->getAppSetting("DefaultMusicFolderPath").toString();
    bool visualizerEnabled = dbManager->getAppSetting("VisualizerEnabled").toBool();

    defaultMusicFolderPathLineEdit->setText(musicFolderPath);
    visualizerEnabledCheckBox->setChecked(visualizerEnabled);

    currentPasswordLineEdit->clear();
    newPasswordLineEdit->clear();
    confirmNewPasswordLineEdit->clear();
}

void SettingDialog::connectSignalsSlots()
{
    connect(saveButton, &QPushButton::clicked, this, &SettingDialog::onSaveButtonClicked);
    connect(changePasswordButton, &QPushButton::clicked, this, &SettingDialog::onChangePasswordButtonClicked);
    connect(logoutButton, &QPushButton::clicked, this, &SettingDialog::onLogoutButtonClicked);
    connect(browseMusicFolderButton, &QPushButton::clicked, this, &SettingDialog::onBrowseMusicFolderButtonClicked);
}

bool SettingDialog::validateProfileInputs()
{
    if (nameLineEdit->text().isEmpty() || lastNameLineEdit->text().isEmpty() || emailLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Name, Last Name, and Email cannot be empty.");
        return false;
    }
    return true;
}

bool SettingDialog::validatePasswordChangeInputs()
{
    QString currentPass = currentPasswordLineEdit->text();
    QString newPass = newPasswordLineEdit->text();
    QString confirmNewPass = confirmNewPasswordLineEdit->text();

    if (currentPass.isEmpty() || newPass.isEmpty() || confirmNewPass.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All password fields must be filled.");
        return false;
    }
    if (newPass != confirmNewPass) {
        QMessageBox::warning(this, "Input Error", "New password and confirmation do not match.");
        return false;
    }
    return true;
}


void SettingDialog::onSaveButtonClicked()
{
    if (!validateProfileInputs()) {
        return; // اگر اعتبارسنجی ناموفق بود، ادامه نده
    }

    currentUser->setName(nameLineEdit->text());
    currentUser->setLastName(lastNameLineEdit->text());
    currentUser->setEmail(emailLineEdit->text());

    if (dbManager->updateUserProfile(currentUser)) { // فرض می‌کنیم این متد true/false برمی‌گردونه
        QMessageBox::information(this, "Success", "Profile information updated successfully.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to update profile information in database.");
    }

    dbManager->setAppSetting("DefaultMusicFolderPath", defaultMusicFolderPathLineEdit->text());
    dbManager->setAppSetting("VisualizerEnabled", visualizerEnabledCheckBox->isChecked());

    emit settingsUpdated();
}

void SettingDialog::onChangePasswordButtonClicked()
{
    if (!validatePasswordChangeInputs()) {
        return; // اگر اعتبارسنجی ناموفق بود، ادامه نده
    }

    QString currentPass = currentPasswordLineEdit->text();
    QString newPass = newPasswordLineEdit->text();

    if (accountManager->changePassword(currentUser->getUsername(), currentPass, newPass)) {
        QMessageBox::information(this, "Success", "Password changed successfully.");
        currentPasswordLineEdit->clear();
        newPasswordLineEdit->clear();
        confirmNewPasswordLineEdit->clear();
    } else {
        QMessageBox::critical(this, "Error", "Failed to change password. Please check your current password.");
    }
}

void SettingDialog::onLogoutButtonClicked()
{
    emit logoutRequested();
    this->close();
}

void SettingDialog::onBrowseMusicFolderButtonClicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select Music Folder"),
                                                          defaultMusicFolderPathLineEdit->text());
    if (!directory.isEmpty()) {
        defaultMusicFolderPathLineEdit->setText(directory);
    }
}

