#include "GetRequester.hpp"
#include "defs.hpp"


GetRequester::GetRequester(QString const &path, NetworkManager *networkManager,
                           QObject *parent):
    CommandProcessor(parent),
    m_path(path),
    m_networkManager(networkManager)
{
}


void GetRequester::process()
{
    connect(m_networkManager, SIGNAL(response(bool, QString const &)),
            this, SLOT(processResponse(bool, QString const &)));
    m_networkManager->get(m_path);
}


void GetRequester::processResponse(bool ok, QString const &msg)
{
    disconnect(m_networkManager, SIGNAL(response(bool, QString const &)),
               this, SLOT(processResponse(bool, QString const &)));
    if (ok)
        emit result(msg);
    else
        emit error(msg);
}
