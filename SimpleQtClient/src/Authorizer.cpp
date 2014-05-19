#include "Authorizer.hpp"
#include "OneShotHttpServer.hpp"
#include "defs.hpp"

#include <QDesktopServices>


Authorizer::Authorizer(NetworkManager *networkManager, QObject *parent):
    CommandProcessor(parent),
    m_networkManager(networkManager)
{
}


void Authorizer::process()
{
    connect(m_networkManager, SIGNAL(response(bool, QString const &)),
            this, SLOT(processLoginResponse(bool, QString const &)));
    m_networkManager->post(LOGIN_PATH);
}


void Authorizer::processLoginResponse(bool ok, QString const &msg)
{
    disconnect(m_networkManager, SIGNAL(response(bool, QString const &)),
               this, SLOT(processLoginResponse(bool, QString const &)));

    if (!ok) {
        emit error(msg);
        return;
    }
    if (msg.trimmed().startsWith("authentication successful",
                                 Qt::CaseInsensitive)) {
        emit result(msg);
        return;
    }

    OneShotHttpServer *server = new OneShotHttpServer(HTTP_PORT);
    connect(server, SIGNAL(result(QString const &)),
            this, SLOT(processServerResult(QString const &)));
    connect(server, SIGNAL(error(QString const &)),
            this, SLOT(processServerError(QString const &)));

    QDesktopServices::openUrl(QUrl(msg));
}


void Authorizer::processOAuthResponse(bool ok, QString const &msg)
{
    disconnect(m_networkManager, SIGNAL(response(bool, QString const &)),
               this, SLOT(processOAuthResponse(bool, QString const &)));

    if (ok)
        emit result(msg);
    else
        emit error(msg);
}


void Authorizer::processServerResult(QString const &msg)
{
    sender()->deleteLater();

    QUrl url(msg);
    url.setPath(AUTH_CALLBACK_PATH);

    connect(m_networkManager, SIGNAL(response(bool, QString const &)),
            this, SLOT(processOAuthResponse(bool, QString const &)));

    m_networkManager->get(url.toString());
}


void Authorizer::processServerError(QString const &errMsg)
{
    emit error(errMsg);
}
