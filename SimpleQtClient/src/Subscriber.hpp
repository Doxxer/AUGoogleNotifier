#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include "CommandProcessor.hpp"
#include "NetworkManager.hpp"


class Subscriber: public CommandProcessor
{
    Q_OBJECT

public:
    Subscriber(NetworkManager *networkManager, QObject *parent = 0);
    void process();

private slots:
    void processResponse(bool ok, QString const &msg);

private:
    NetworkManager *m_networkManager;
};


#endif // SUBSCRIBER_H
