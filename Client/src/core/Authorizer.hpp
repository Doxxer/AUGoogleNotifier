#ifndef AUTHORIZER_HPP
#define AUTHORIZER_HPP

#include "CommandProcessor.hpp"
#include "NetworkManager.hpp"


class Authorizer: public CommandProcessor
{
    Q_OBJECT

public:
    explicit Authorizer(NetworkManager *networkManager, QObject *parent = 0);

public slots:
    void process();

private slots:
    void processLoginResponse(bool ok, QString const &msg);
    void processOAuthResponse(bool ok, QString const &msg);
    void processServerResult(QString const &msg);
    void processServerError(QString const &errMsg);

private:
    NetworkManager *m_networkManager;
};


#endif // AUTHORIZER_HPP
