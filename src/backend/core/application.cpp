#include "application.h"
#include "src/backend/core/UserManager.h"
#include "src/backend/db/DBM.h"
#include "src/ui/changepasswordwindow.h"
#include "src/ui/choicewindow.h"
#include "src/ui/loginwindow.h"
#include "src/ui/showkeywords.h"
#include "src/ui/signupwindow.h"
#include "src/ui/emailverificationwindow.h"
#include "src/ui/frogotpasswordwindow.h"
#include "src/ui/receivesecurewordswindow.h"

#include <QThread>


std::unique_ptr<Application> Application::s_instance = nullptr;

Application &Application::getInstance()
{
    if(!s_instance)
    {
        s_instance.reset(new Application());
    }

    return *s_instance;
}

void Application::Run()
{

    DBM::get_instance();
    this->show_choiceWindow();
    UserManager::getInstance();
}
//ok
void Application::show_choiceWindow()
{
    if (!w_choice_window) {
        w_choice_window = new ChoiceWindow();

        // signals
        connect(w_choice_window,&ChoiceWindow::LoginRequest,this,&Application::show_loginWindow);
        connect(w_choice_window,&ChoiceWindow::SignupRequest,this,&Application::show_signupWindow);
    }
    switchWindow(w_choice_window);
}
//ok
void Application::show_loginWindow()
{
    if (!w_login_window) {
        w_login_window = new LoginWindow();

        // signals
        connect(w_login_window,&LoginWindow::CancelOperation,this,&Application::show_choiceWindow);
        connect(w_login_window,&LoginWindow::ForgotPassword,this,&Application::show_forgotPassword_window);
    }
    switchWindow(w_login_window);
}

void Application::show_signupWindow()
{
    if(!w_signUp_window){
        w_signUp_window = new SignupWindow();

        connect(w_signUp_window,&SignupWindow::backToChoiseWindow,this,&Application::show_choiceWindow);
        connect(w_signUp_window,&SignupWindow::showkey,this,&Application::show_showKeyWindow);
    }
    switchWindow(w_signUp_window);
}
//ok
void Application::show_changePasswordWindow(QString Username)
{
    if(!w_change_password_window){
        w_change_password_window = new ChangePasswordWindow();
        connect(w_change_password_window,&ChangePasswordWindow::goToLoginWindow,this,&Application::show_loginWindow);
    }
    w_change_password_window->set_username(Username);
    switchWindow(w_change_password_window);
}

void Application::show_emailVWindow(QString Email, QString Username)
{
    if(!w_email_verification_window){
        w_email_verification_window = new EmailVerificationWindow();

        connect(w_email_verification_window,&EmailVerificationWindow::backToForgetPassWindow,this,&Application::show_forgotPassword_window);
        connect(w_email_verification_window,&EmailVerificationWindow::passVerification_gotoChangepassword,this,&Application::show_changePasswordWindow);
    }
    w_email_verification_window->setResiverUsername(Username);
    w_email_verification_window->Emailsender(Email);
    switchWindow(w_email_verification_window);
}
//ok
void Application::show_forgotPassword_window()
{
    if(!w_forgot_password_window){
        w_forgot_password_window = new FrogotPasswordWindow();

        connect(w_forgot_password_window ,&FrogotPasswordWindow::backToLoginWindow,this,&Application::show_loginWindow);
        connect(w_forgot_password_window ,&FrogotPasswordWindow::EmailVerification,this,&Application::show_emailVWindow);
        connect(w_forgot_password_window ,&FrogotPasswordWindow::SecurityVerification,this,&Application::show_receiveSWWindow);
    }
    switchWindow(w_forgot_password_window);
}

void Application::show_receiveSWWindow(QString key, QString username)
{
    if(!w_receive_secureWords_window){
        w_receive_secureWords_window = new ReceiveSecureWordsWindow();
        connect(w_receive_secureWords_window ,&ReceiveSecureWordsWindow::backToForgetPassWindow,this,&Application::show_forgotPassword_window);
        connect(w_receive_secureWords_window ,&ReceiveSecureWordsWindow::passed,this,&Application::show_changePasswordWindow);

    }
    w_receive_secureWords_window->setkey(key);
    w_receive_secureWords_window->setusername(username);
    switchWindow(w_receive_secureWords_window);
}

void Application::show_showKeyWindow(QString key)
{
    if(!w_showKey_Window){
        w_showKey_Window = new ShowKeyWords();
        connect(w_showKey_Window ,&ShowKeyWords::signup_successfully,this,&Application::show_loginWindow);
    }
    w_showKey_Window->set_key(key);
    switchWindow(w_showKey_Window);
}

Application::Application(QObject *parent)
    :QObject(parent),m_currentWindow(nullptr),w_change_password_window(nullptr)
    ,w_choice_window(nullptr),w_email_verification_window(nullptr),w_forgot_password_window(nullptr)
    ,w_login_window(nullptr),w_receive_secureWords_window(nullptr),w_signUp_window(nullptr),w_showKey_Window(nullptr)
{
    ;
}

void Application::switchWindow(QWidget *nextWindow)
{
    if(!nextWindow) return;
    nextWindow->show();
    if(m_currentWindow)
    {
        m_currentWindow->hide();
    }
    m_currentWindow = nextWindow;
}
