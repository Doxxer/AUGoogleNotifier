#include "Logouter.hpp"
#include "defs.hpp"


Logouter::Logouter(NetworkManager *networkManager, QObject *parent):
    Requester(Requester::POST, LOGOUT_PATH, networkManager, parent)
{
}
