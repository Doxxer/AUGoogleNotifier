#ifndef UNSUBSCRIBER_H
#define UNSUBSCRIBER_H

#include "Requester.hpp"


class Unsubscriber: public Requester
{
    Q_OBJECT

public:
    Unsubscriber(NetworkManager *networkManager, QObject *parent = 0);
};


#endif // UNSUBSCRIBER_H
