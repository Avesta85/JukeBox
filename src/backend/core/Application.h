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

class Application : public QObject
{
    Q_OBJECT
public:
    static Application& getInstance();



    ///// window getters

    void show_choiceWindow();
    void show_loginWindow();
    void show_signupWindow();
    void show_changePasswordWindow();
    void show_emailVWindow();
    void show_forgotPassword_window();
    void show_receiveSWWindow();

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


    // function
    void switchWindow(QWidget* nextWindow);

signals:
};

#endif // APPLICATION_H
