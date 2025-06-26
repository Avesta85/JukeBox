#include "settingdialog.h"
#include <QVBoxLayout> // برای چیدمان عمودی
#include <QHBoxLayout> // برای چیدمان افقی
#include <QFormLayout> // برای چیدمان فرم (برچسب و فیلد ورودی)
#include <QLabel>
#include <QGroupBox> // برای گروه‌بندی ویجت‌ها

// Include کردن کامل فایل‌های هدر برای کلاس‌هایی که استفاده می‌شن
#include "localuser.h"
#include "accountmanager.h"
#include "src/backend/db/DBM.h"
#include "src/backend/core/Application.h"

// =======================================================
// سازنده (Constructor)
// =======================================================
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
    // این‌ها باید حتماً در زمان ساخت شیء توسط ApplicationManager بهش داده بشن
    // در غیر این صورت، برنامه کرش می‌کنه.
    // می‌توانید Assertions هم اینجا اضافه کنید برای چک کردن null بودن.
    Q_ASSERT(currentUser);
    Q_ASSERT(accountManager);
    Q_ASSERT(dbManager);
    Q_ASSERT(appManager);

    setupUi();           // طراحی رابط کاربری
    loadSettings();      // بارگذاری اطلاعات فعلی
    connectSignalsSlots(); // اتصال سیگنال‌ها به اسلات‌ها
    setWindowTitle("Settings"); // تنظیم عنوان پنجره
}

// =======================================================
// مخرب (Destructor)
// =======================================================
SettingDialog::~SettingDialog()
{
    // نیازی به حذف (delete) اشاره‌گرهای currentUser, accountManager, dbManager, appManager نیست
    // چون این‌ها اشاره‌گر به آبجکت‌هایی هستن که در جای دیگه مدیریت می‌شن (مثلاً سینگلتون‌ها یا در ApplicationManager)
    // و این دیالوگ مالکیت اون‌ها رو نداره.
}

// =======================================================
// متدهای Private
// =======================================================

// طراحی رابط کاربری
void SettingDialog::setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // --- بخش اطلاعات پروفایل ---
    QGroupBox* profileGroup = new QGroupBox("User Profile", this);
    QFormLayout* profileLayout = new QFormLayout(profileGroup);
    nameLineEdit = new QLineEdit(this);
    lastNameLineEdit = new QLineEdit(this);
    emailLineEdit = new QLineEdit(this);

    profileLayout->addRow("Name:", nameLineEdit);
    profileLayout->addRow("Last Name:", lastNameLineEdit);
    profileLayout->addRow("Email:", emailLineEdit);
    mainLayout->addWidget(profileGroup);

    // --- بخش تغییر رمز عبور ---
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

    // --- بخش تنظیمات برنامه ---
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

    // --- دکمه‌های اصلی ---
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    saveButton = new QPushButton("Save Changes", this);
    logoutButton = new QPushButton("Logout", this);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addStretch(); // فاصله انداز برای چپ و راست شدن دکمه‌ها
    buttonLayout->addWidget(logoutButton);
    mainLayout->addLayout(buttonLayout);
}

// بارگذاری اطلاعات کاربر و تنظیمات برنامه در UI
void SettingDialog::loadSettings()
{
    // 1. بارگذاری اطلاعات پروفایل کاربر
    nameLineEdit->setText(currentUser->getName());
    lastNameLineEdit->setText(currentUser->getLastName());
    emailLineEdit->setText(currentUser->getEmail());

    // 2. بارگذاری تنظیمات برنامه از DatabaseManager
    // TODO: پیاده‌سازی متدهای DatabaseManager برای خواندن تنظیمات
    // این قسمت نیاز داره که DatabaseManager متدهایی برای گرفتن تنظیمات داشته باشه.
    // فرض می‌کنیم DatabaseManager یک متد برای گرفتن تنظیمات کل برنامه داره.
    // مثال: QMap<QString, QVariant> appSettings = dbManager->getAppSettings();
    // یا متدهای مجزا برای هر تنظیم:
    QString musicFolderPath = dbManager->getAppSetting("DefaultMusicFolderPath").toString();
    bool visualizerEnabled = dbManager->getAppSetting("VisualizerEnabled").toBool();

    defaultMusicFolderPathLineEdit->setText(musicFolderPath);
    visualizerEnabledCheckBox->setChecked(visualizerEnabled);

    // نکته: فیلدهای رمز عبور هنگام بارگذاری خالی می‌مونن (یا با جایگزین)
    currentPasswordLineEdit->clear();
    newPasswordLineEdit->clear();
    confirmNewPasswordLineEdit->clear();
}

// اتصال سیگنال‌ها به اسلات‌ها
void SettingDialog::connectSignalsSlots()
{
    connect(saveButton, &QPushButton::clicked, this, &SettingDialog::onSaveButtonClicked);
    connect(changePasswordButton, &QPushButton::clicked, this, &SettingDialog::onChangePasswordButtonClicked);
    connect(logoutButton, &QPushButton::clicked, this, &SettingDialog::onLogoutButtonClicked);
    connect(browseMusicFolderButton, &QPushButton::clicked, this, &SettingDialog::onBrowseMusicFolderButtonClicked);
}

// اعتبارسنجی ورودی‌های پروفایل
bool SettingDialog::validateProfileInputs()
{
    // مثال: چک کردن خالی نبودن نام، نام خانوادگی و فرمت ایمیل
    if (nameLineEdit->text().isEmpty() || lastNameLineEdit->text().isEmpty() || emailLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Name, Last Name, and Email cannot be empty.");
        return false;
    }
    // TODO: اضافه کردن اعتبارسنجی فرمت ایمیل
    return true;
}

// اعتبارسنجی ورودی‌های تغییر رمز
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
    // TODO: اضافه کردن چک کردن حداقل طول رمز عبور و پیچیدگی (اعداد، حروف بزرگ/کوچک، کاراکترهای خاص)
    return true;
}

// =======================================================
// اسلات‌ها (Private Slots)
// =======================================================

// وقتی دکمه "Save Changes" کلیک میشه
void SettingDialog::onSaveButtonClicked()
{
    if (!validateProfileInputs()) {
        return; // اگر اعتبارسنجی ناموفق بود، ادامه نده
    }

    // 1. به‌روزرسانی اطلاعات پروفایل کاربر در شیء LocalUser
    currentUser->setName(nameLineEdit->text());
    currentUser->setLastName(lastNameLineEdit->text());
    currentUser->setEmail(emailLineEdit->text());

    // 2. به‌روزرسانی اطلاعات کاربر در پایگاه داده (SQLite) از طریق DatabaseManager
    // TODO: این متد در DatabaseManager باید پیاده‌سازی بشه.
    // DatabaseManager مسئول Update کردن ردیف کاربر در جدول User هست.
    if (dbManager->updateUserProfile(currentUser)) { // فرض می‌کنیم این متد true/false برمی‌گردونه
        QMessageBox::information(this, "Success", "Profile information updated successfully.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to update profile information in database.");
    }

    // 3. ذخیره تنظیمات برنامه در پایگاه داده (SQLite) از طریق DatabaseManager
    // TODO: DatabaseManager باید متدهایی برای ذخیره تنظیمات برنامه داشته باشه.
    // این تنظیمات ممکنه در یک جدول جداگانه ذخیره بشن (مثلاً یک جدول Key-Value)
    dbManager->setAppSetting("DefaultMusicFolderPath", defaultMusicFolderPathLineEdit->text());
    dbManager->setAppSetting("VisualizerEnabled", visualizerEnabledCheckBox->isChecked());

    // فرستادن سیگنال که تنظیمات بروز شده
    emit settingsUpdated();
}

// وقتی دکمه "Change Password" کلیک میشه
void SettingDialog::onChangePasswordButtonClicked()
{
    if (!validatePasswordChangeInputs()) {
        return; // اگر اعتبارسنجی ناموفق بود، ادامه نده
    }

    QString currentPass = currentPasswordLineEdit->text();
    QString newPass = newPasswordLineEdit->text();

    // 1. فراخوانی AccountManager برای تغییر رمز عبور
    // AccountManager مسئولیت چک کردن رمز عبور فعلی و هش کردن رمز جدید و به‌روزرسانی در SQLite رو داره.
    // TODO: این متد در AccountManager باید پیاده‌سازی بشه.
    // AccountManager با DatabaseManager برای این کار تعامل می‌کنه.
    if (accountManager->changePassword(currentUser->getUsername(), currentPass, newPass)) {
        QMessageBox::information(this, "Success", "Password changed successfully.");
        // بعد از موفقیت، فیلدها رو خالی کن
        currentPasswordLineEdit->clear();
        newPasswordLineEdit->clear();
        confirmNewPasswordLineEdit->clear();
    } else {
        // پیام خطا می‌تونه دقیق‌تر باشه (مثلاً رمز عبور فعلی اشتباه است)
        QMessageBox::critical(this, "Error", "Failed to change password. Please check your current password.");
    }
}

// وقتی دکمه "Logout" کلیک میشه
void SettingDialog::onLogoutButtonClicked()
{
    // فرستادن سیگنال به ApplicationManager که کاربر درخواست خروج داده
    emit logoutRequested();
    // این دیالوگ رو می‌بندیم، ApplicationManager مسئول ناوبری به پنجره Login/Choice هست.
    this->close();
}

// وقتی دکمه "Browse..." برای انتخاب فولدر موسیقی کلیک میشه
void SettingDialog::onBrowseMusicFolderButtonClicked()
{
    // باز کردن یک دیالوگ برای انتخاب فولدر
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select Music Folder"),
                                                          defaultMusicFolderPathLineEdit->text());
    if (!directory.isEmpty()) {
        defaultMusicFolderPathLineEdit->setText(directory);
    }
}
