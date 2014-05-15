#include "Requester.hpp"


Requester::Requester(RequestType type, QString const &path,
                     NetworkManager *networkManager, QObject *parent):
    CommandProcessor(parent),
    m_type(type),
    m_path(path),
    m_networkManager(networkManager)
{
}


void Requester::process()
{
    connect(m_networkManager, SIGNAL(response(bool, QString const &)),
            this, SLOT(processResponse(bool, QString const &)));
    if (m_type == GET)
        m_networkManager->get(m_path);
    else
        m_networkManager->post(m_path);
}


QString const Requester::prepareResult(QString const &msg) const
{
    return msg;
}


QString const Requester::prepareError(QString const &errMsg) const
{
    return errMsg;
}


void Requester::processResponse(bool ok, QString const &msg)
{
    disconnect(m_networkManager, SIGNAL(response(bool, QString const &)),
               this, SLOT(processResponse(bool, QString const &)));
    if (ok) {
        emit result(prepareResult(msg));
    } else {
        emit error(prepareError(msg));
    }
}
