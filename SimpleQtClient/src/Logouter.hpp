#ifndef LOGOUTER_H
#define LOGOUTER_H

#include "Requester.hpp"


class Logouter: public Requester
{
    Q_OBJECT

public:
    Logouter(NetworkManager *networkManager, QObject *parent = 0);

protected:
    QString const prepareResult(QString const &msg) const;
};


#endif // LOGOUTER_H
