#include "AuthorizationChecker.hpp"
#include "defs.hpp"

#include <QDesktopServices>


AuthorizationChecker::AuthorizationChecker(NetworkManager *networkManager,
                                           QObject *parent):
    QObject(parent),
    m_networkManager(networkManager)
{
}


bool AuthorizationChecker::isAuthorized(QString const &response)
{
    return response.trimmed().startsWith("authentication successful",
                                         Qt::CaseInsensitive);
}


void AuthorizationChecker::check()
{
    connect(m_networkManager, SIGNAL(response(bool, QString const &)),
            this, SLOT(processResponse(bool, QString const &)));
    m_networkManager->post(LOGIN_PATH);
}


void AuthorizationChecker::processResponse(bool ok, QString const &msg)
{
    disconnect(m_networkManager, SIGNAL(response(bool, QString const &)),
               this, SLOT(processResponse(bool, QString const &)));

    if (!ok) {
        emit error();
        return;
    }
    emit result(isAuthorized(msg));
}
