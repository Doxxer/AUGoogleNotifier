#include "Unsubscriber.hpp"
#include "defs.hpp"


Unsubscriber::Unsubscriber(NetworkManager *networkManager, QObject *parent):
    Requester(Requester::POST, UNSUBSCRIBE_PATH, networkManager, parent)
{
}
