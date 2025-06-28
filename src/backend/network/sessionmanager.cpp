#include "sessionmanager.h"

QString SessionManager::findUsernameByAddress(const QHostAddress &address) const
{
    for(const Participant& p :m_participants)
    {
        if(p.address == address)
        {
            return p.username;
        }
    }
    return "UnKnown User";
}

SessionManager::SessionManager(QObject *parent)
    : QObject{parent}
{}
