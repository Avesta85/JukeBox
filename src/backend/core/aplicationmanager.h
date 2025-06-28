#ifndef APLICATIONMANAGER_H
#define APLICATIONMANAGER_H

#include <QObject>
#include <QPointer>
#include "ui/choicewindow.h"
#include "ui/loginwindow.h"
#include "ui/changepasswordwindow.h"
#include "ui/emailverificationwindow.h"
#include "ui/frogotpasswordwindow.h"
#include "ui/receivesecurewordswindow.h"
#include "ui/signupwindow.h"

class AplicationManager
{
public:
    static AplicationManager*& instance();

    void showChoiseWindow();
    void showLoginWindow();
    void showChangePassWindow();
    void showEmailVrifiWindow();
    void showForgetPassWindow();
    void showSecureWordWindow();
    void showSignupWindow();

    ~AplicationManager();

private:
    explicit AplicationManager();
    AplicationManager(const AplicationManager&) = delete;
    AplicationManager& operator=(const AplicationManager&) = delete;

    static AplicationManager* Instance;

    QPointer<ChoiceWindow> choiseW = nullptr;
    QPointer<LoginWindow> loginW = nullptr;
    QPointer<ChangePasswordWindow> changePassW = nullptr;
    QPointer<EmailVerificationWindow> emailVrifiW = nullptr;
    QPointer<FrogotPasswordWindow> forgetPassW = nullptr;
    QPointer<ReceiveSecureWordsWindow> secureWordW = nullptr;
    QPointer<SignupWindow> signupW = nullptr;

};

#endif // APLICATIONMANAGER_H
