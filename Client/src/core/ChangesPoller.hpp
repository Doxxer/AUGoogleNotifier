#ifndef CHANGESPOLLER_HPP
#define CHANGESPOLLER_HPP

#include <QTimer>

#include "NetworkManager.hpp"
#include "PollingManager.hpp"


class ChangesPoller: public PollingManager
{
    Q_OBJECT

public:
    explicit ChangesPoller(NetworkManager *networkManager,
                               QObject *parent = 0);
};


#endif // CHANGESPOLLER_HPP
