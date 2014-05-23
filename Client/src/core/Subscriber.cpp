#include "Subscriber.hpp"
#include "defs.hpp"


Subscriber::Subscriber(NetworkManager *networkManager, QObject *parent):
    Requester(Requester::POST, SUBSCRIBE_PATH, networkManager, parent)
{
}
