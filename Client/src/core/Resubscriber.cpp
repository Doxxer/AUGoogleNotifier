#include "Resubscriber.hpp"
#include "Subscriber.hpp"
#include "defs.hpp"


Resubscriber::Resubscriber(NetworkManager *networkManager, QObject *parent):
    PollingManager(new Subscriber(networkManager), RESUBSCRIBE_TIMEOUT, parent)
{
}
