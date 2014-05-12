#ifndef NETWORKMANAGER_HPP
#define NETWORKMANAGER_HPP

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QObject>

#include "CookieJar.hpp"


class NetworkManager: public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = 0);
    ~NetworkManager();

    void post(QString const &path);
    void get(QString const &path);

signals:
    void response(bool ok, QString const &msg);

private slots:
    void processFinish(QNetworkReply *reply);

private:
    QNetworkRequest const createRequest(QString const &path) const;

    static QString const APPSERVER;
    static QString const COOKIES;

    CookieJar *m_cookieJar;
    QNetworkAccessManager *m_nam;
};


#endif // NETWORKMANAGER_HPP
