#include "aplicationmanager.h"

AplicationManager::AplicationManager() {}

AplicationManager* AplicationManager::Instance = nullptr;

AplicationManager*& AplicationManager::instance()
{
    if(Instance == nullptr)
        Instance = new AplicationManager;

    return Instance;
}

void AplicationManager::showChoiseWindow()
{
    if(choiseW == nullptr)
        choiseW = new ChoiceWindow;

    choiseW->showMaximized();
}
void AplicationManager::showLoginWindow()
{
    if(loginW == nullptr)
        loginW = new LoginWindow;

    loginW->showMaximized();
}

void AplicationManager::showChangePassWindow()
{
    if(changePassW == nullptr)
        changePassW = new ChangePasswordWindow;

    changePassW->showMaximized();
}

void AplicationManager::showEmailVrifiWindow()
{
    if(emailVrifiW == nullptr)
        emailVrifiW = new EmailVerificationWindow;

    emailVrifiW->showMaximized();
}

void AplicationManager::showForgetPassWindow()
{
    if(forgetPassW == nullptr)
        forgetPassW = new FrogotPasswordWindow;

    forgetPassW->showMaximized();
}

void AplicationManager::showSecureWordWindow()
{
    if(secureWordW == nullptr)
        secureWordW = new ReceiveSecureWordsWindow;

    secureWordW->showMaximized();
}

void AplicationManager::showSignupWindow()
{
    if(signupW == nullptr)
        signupW = new SignupWindow;

    signupW->showMaximized();
}

AplicationManager::~AplicationManager()
{
    if(Instance != nullptr)
        delete Instance;
}
