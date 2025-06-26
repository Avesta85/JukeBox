#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "src/backend/core/user.h"
#include <memory>
#include <botan>

class UserManager final
{

public:

    UserManager& getInstance();
    ~UserManager();
    UserManager(const UserManager&)=delete;
    UserManager& operator = (const UserManager&)=delete;


    //login

    bool attemptLogin(const QString& username , const QString Password);

private:
    UserManager();

    //static
    static std::unique_ptr<UserManager>s_instance;

    //member
    std::optional<User>m_local_user;

    //function

    //// hash and encript;



};

#endif // USERMANAGER_H
