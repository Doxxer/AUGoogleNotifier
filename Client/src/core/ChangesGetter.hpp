#ifndef CHANGESGETTER_H
#define CHANGESGETTER_H

#include "Requester.hpp"


class ChangesGetter: public Requester
{
    Q_OBJECT

public:
    ChangesGetter(NetworkManager *networkManager, QObject *parent = 0);
};


#endif // CHANGESGETTER_H
