#include "NetworkManager.hpp"

#include <QDebug>
#include <QUrl>


NetworkManager::NetworkManager(QString const &appserver,
                               QString const &cookies,
                               QObject *parent):
    QObject(parent),
    m_appserver(appserver),
    m_cookies(cookies),
    m_cookieJar(new CookieJar()),
    m_nam(new QNetworkAccessManager(this))
{
    m_cookieJar->load(m_cookies);
    m_nam->setCookieJar(m_cookieJar);

    connect(m_nam, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(processFinish(QNetworkReply *)));
}


NetworkManager::~NetworkManager()
{
    m_cookieJar->save(m_cookies);
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
        msg = "Network error";
    }
    m_cookieJar->save(m_cookies); // !!!
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
