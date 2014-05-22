#include "ChangesPoller.hpp"
#include "ChangesGetter.hpp"
#include "defs.hpp"


ChangesPoller::ChangesPoller(NetworkManager *networkManager,
                             QObject *parent):
    PollingManager(new ChangesGetter(networkManager), CHANGES_TIMEOUT, parent)
{
}
