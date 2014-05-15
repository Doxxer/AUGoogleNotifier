#include "Logouter.hpp"
#include "defs.hpp"


Logouter::Logouter(NetworkManager *networkManager, QObject *parent):
    Requester(Requester::GET, LOGOUT_PATH, networkManager, parent)
{
}


QString const Logouter::prepareResult(QString const &) const
{
    return "OK";
}
