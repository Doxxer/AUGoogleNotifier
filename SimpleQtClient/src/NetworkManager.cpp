#include "NetworkManager.hpp"

#include <QNetworkCookie>
#include <QUuid>


NetworkManager::NetworkManager(QString const &appserver, QObject *parent):
    QObject(parent),
    m_appserver(appserver),
    m_cookieJar(new CookieJar()),
    m_nam(new QNetworkAccessManager(this))
{
    if (!m_cookieJar->load()) {
        QNetworkCookie cookie("mac_address", // !!! rename
                              QUuid::createUuid().toByteArray());
        m_cookieJar->setCookiesFromUrl(QList<QNetworkCookie>() << cookie,
                                       m_appserver);
    }
    m_nam->setCookieJar(m_cookieJar);

    connect(m_nam, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(processFinish(QNetworkReply *)));
}


NetworkManager::~NetworkManager()
{
    m_cookieJar->save();
}


void NetworkManager::post(QString const &path)
{
    m_nam->post(createRequest(path), QString().toUtf8());
}


void NetworkManager::get(QString const &path)
{
    m_nam->get(createRequest(path));
}


void NetworkManager::processFinish(QNetworkReply *reply)
{
    bool ok;
    QString msg;

    if (reply->error() == QNetworkReply::NoError) {
        ok = true;
        msg = QString::fromUtf8(reply->readAll());
    } else {
        ok = false;
        msg = reply->errorString();
    }
    m_cookieJar->save(); // !!!
    reply->deleteLater();
    emit response(ok, msg);
}


QNetworkRequest const NetworkManager::createRequest(QString const &path) const
{
    QUrl url(m_appserver + path);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");
    return request;
}
