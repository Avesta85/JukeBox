#include "application.h"
#include "src/ui/changepasswordwindow.h"
#include "src/ui/choicewindow.h"
#include "src/ui/loginwindow.h"
#include "src/ui/signupwindow.h"
#include "src/ui/emailverificationwindow.h"
#include "src/ui/frogotpasswordwindow.h"
#include "src/ui/receivesecurewordswindow.h"


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
    this->show_choiceWindow();
}

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

    }
    switchWindow(w_signUp_window);
}

void Application::show_changePasswordWindow()
{
    if(!w_change_password_window){
        w_change_password_window = new ChangePasswordWindow();

    }
    switchWindow(w_change_password_window);
}

void Application::show_emailVWindow()
{
    if(!w_email_verification_window){
        w_email_verification_window = new EmailVerificationWindow();

    }
    switchWindow(w_email_verification_window);
}

void Application::show_forgotPassword_window()
{
    if(!w_forgot_password_window){
        w_forgot_password_window = new FrogotPasswordWindow();

    }
    switchWindow(w_forgot_password_window);
}

void Application::show_receiveSWWindow()
{
    if(!w_receive_secureWords_window){
        w_receive_secureWords_window = new ReceiveSecureWordsWindow();

    }
    switchWindow(w_receive_secureWords_window);
}

Application::Application(QObject *parent)
    :QObject(parent),m_currentWindow(nullptr),w_change_password_window(nullptr)
    ,w_choice_window(nullptr),w_email_verification_window(nullptr),w_forgot_password_window(nullptr)
    ,w_login_window(nullptr),w_receive_secureWords_window(nullptr),w_signUp_window(nullptr)
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
