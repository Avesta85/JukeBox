#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>

class ChangePasswordWindow;
class ChoiceWindow;
class EmailVerificationWindow;
class FrogotPasswordWindow;
class LoginWindow;
class ReceiveSecureWordsWindow;
class SignupWindow;
class ShowKeyWords;

class Application : public QObject
{
    Q_OBJECT
public:
    static Application& getInstance();


    void Run();
    ///// window getters

    void show_choiceWindow();
    void show_loginWindow();
    void show_signupWindow();
    void show_changePasswordWindow(QString Username);
    void show_emailVWindow(QString Email ,QString Username);
    void show_forgotPassword_window();
    void show_receiveSWWindow(QString key , QString username);
    void show_showKeyWindow(QString key);

private:
    explicit Application(QObject *parent = nullptr);


    // static V
    static std::unique_ptr<Application> s_instance;


    //member V
    QWidget* m_currentWindow;
    // member Window

    ChangePasswordWindow* w_change_password_window;
    ChoiceWindow* w_choice_window;
    EmailVerificationWindow* w_email_verification_window;
    FrogotPasswordWindow* w_forgot_password_window;
    LoginWindow* w_login_window;
    ReceiveSecureWordsWindow* w_receive_secureWords_window;
    SignupWindow* w_signUp_window;
    ShowKeyWords* w_showKey_Window;

    // function
    void switchWindow(QWidget* nextWindow);

private slots:

};

#endif // APPLICATION_H
