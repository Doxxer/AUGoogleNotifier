#include "Authorizer.hpp"

#include <QDebug>


Authorizer::Authorizer(NetworkManager *networkManager, QObject *parent):
    CommandProcessor(parent),
    m_networkManager(networkManager)
{
    connect(networkManager, SIGNAL(response(bool, QString const &)),
            this, SLOT(processResponse(bool, QString const &)));
}


void Authorizer::process()
{
    m_networkManager->post("login");
}


void Authorizer::processResponse(bool ok, QString const &msg)
{
    if (ok)
        emit result(msg);
    else
        emit error(msg);
}
