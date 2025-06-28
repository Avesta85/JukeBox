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

    choiseW->show();
}
void AplicationManager::showLoginWindow()
{
    if(loginW == nullptr)
        loginW = new LoginWindow;

    loginW->show();
}

void AplicationManager::showChangePassWindow()
{
    if(changePassW == nullptr)
        changePassW = new ChangePasswordWindow;

    changePassW->show();
}

void AplicationManager::showEmailVrifiWindow()
{
    if(emailVrifiW == nullptr)
        emailVrifiW = new EmailVerificationWindow;

    emailVrifiW->show();
}

void AplicationManager::showForgetPassWindow()
{
    if(forgetPassW == nullptr)
        forgetPassW = new FrogotPasswordWindow;

    forgetPassW->show();
}

void AplicationManager::showSecureWordWindow()
{
    if(secureWordW == nullptr)
        secureWordW = new ReceiveSecureWordsWindow;

    secureWordW->show();
}

void AplicationManager::showSignupWindow()
{
    if(signupW == nullptr)
        signupW = new SignupWindow;

    signupW->show();
}

AplicationManager::~AplicationManager()
{
    if(Instance != nullptr)
        delete Instance;
}
