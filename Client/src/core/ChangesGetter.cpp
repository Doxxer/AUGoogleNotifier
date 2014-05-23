#include "ChangesGetter.hpp"
#include "defs.hpp"


ChangesGetter::ChangesGetter(NetworkManager *networkManager, QObject *parent):
    Requester(Requester::POST, CHANGES_PATH, networkManager, parent)
{
}
