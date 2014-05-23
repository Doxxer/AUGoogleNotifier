#ifndef AUTHORIZATIONCHECKER_HPP
#define AUTHORIZATIONCHECKER_HPP

#include "NetworkManager.hpp"


class AuthorizationChecker: public QObject
{
    Q_OBJECT

public:
    explicit AuthorizationChecker(NetworkManager *networkManager,
                                  QObject *parent = 0);

    static bool isAuthorized(QString const &response);

signals:
    void result(bool authorized);
    void error();

public slots:
    void check();

private slots:
    void processResponse(bool ok, QString const &msg);

private:
    NetworkManager *m_networkManager;
};


#endif // AUTHORIZATIONCHECKER_HPP
