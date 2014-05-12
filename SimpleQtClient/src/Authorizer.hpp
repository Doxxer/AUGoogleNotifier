#ifndef AUTHORIZER_H
#define AUTHORIZER_H

#include "CommandProcessor.hpp"
#include "NetworkManager.hpp"


class Authorizer: public CommandProcessor
{
    Q_OBJECT

public:
    explicit Authorizer(NetworkManager *networkManager, QObject *parent = 0);
    void process();

private slots:
    void processResponse(bool ok, QString const &msg);

private:
    NetworkManager *m_networkManager;
};


#endif // AUTHORIZER_H
