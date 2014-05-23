#ifndef LOGOUTER_H
#define LOGOUTER_H

#include "Requester.hpp"


class Logouter: public Requester
{
    Q_OBJECT

public:
    Logouter(NetworkManager *networkManager, QObject *parent = 0);
};


#endif // LOGOUTER_H
