#include "Subscriber.hpp"
#include "defs.hpp"


Subscriber::Subscriber(NetworkManager *networkManager, QObject *parent):
    CommandProcessor(parent),
    m_networkManager(networkManager)
{
}


void Subscriber::process()
{
    connect(m_networkManager, SIGNAL(response(bool, QString const &)),
            this, SLOT(processResponse(bool, QString const &)));
    m_networkManager->post(SUBSCRIBE_PATH);
}


void Subscriber::processResponse(bool ok, QString const &msg)
{
    disconnect(m_networkManager, SIGNAL(response(bool, QString const &)),
               this, SLOT(processResponse(bool, QString const &)));
    if (ok)
        emit result("OK");
    else
        emit error(msg);
}
