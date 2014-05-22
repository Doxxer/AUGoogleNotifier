#ifndef RESUBSCRIBER_HPP
#define RESUBSCRIBER_HPP

#include <QTimer>

#include "NetworkManager.hpp"
#include "PollingManager.hpp"


class Resubscriber: public PollingManager
{
    Q_OBJECT

public:
    explicit Resubscriber(NetworkManager *networkManager, QObject *parent = 0);
};


#endif // RESUBSCRIBER_HPP
