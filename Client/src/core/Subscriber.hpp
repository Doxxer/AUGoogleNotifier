#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include "Requester.hpp"


class Subscriber: public Requester
{
    Q_OBJECT

public:
    Subscriber(NetworkManager *networkManager, QObject *parent = 0);

protected:
    QString const prepareResult(QString const &msg) const;
};


#endif // SUBSCRIBER_H
