#include "ChangesGetter.hpp"
#include "defs.hpp"


ChangesGetter::ChangesGetter(NetworkManager *networkManager, QObject *parent):
    CommandProcessor(parent),
    m_networkManager(networkManager)
{
}


void ChangesGetter::process()
{
    connect(m_networkManager, SIGNAL(response(bool, QString const &)),
            this, SLOT(processResponse(bool, QString const &)));
    m_networkManager->post(CHANGES_PATH);
}


void ChangesGetter::processResponse(bool ok, QString const &msg)
{
    disconnect(m_networkManager, SIGNAL(response(bool, QString const &)),
               this, SLOT(processResponse(bool, QString const &)));
    if (ok)
        emit result(msg);
    else
        emit error(msg);
}
