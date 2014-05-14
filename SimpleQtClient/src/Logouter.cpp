#include "Logouter.hpp"
#include "defs.hpp"


Logouter::Logouter(NetworkManager *networkManager, QObject *parent):
    CommandProcessor(parent),
    m_networkManager(networkManager)
{
}


void Logouter::process()
{
    connect(m_networkManager, SIGNAL(response(bool, QString const &)),
            this, SLOT(processResponse(bool, QString const &)));
    m_networkManager->get(LOGOUT_PATH);
}


void Logouter::processResponse(bool ok, QString const &msg)
{
    disconnect(m_networkManager, SIGNAL(response(bool, QString const &)),
               this, SLOT(processResponse(bool, QString const &)));
    if (ok)
        emit result("OK");
    else
        emit error(msg);
}
