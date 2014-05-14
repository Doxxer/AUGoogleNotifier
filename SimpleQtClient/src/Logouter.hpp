#ifndef LOGOUTER_H
#define LOGOUTER_H

#include "CommandProcessor.hpp"
#include "NetworkManager.hpp"


class Logouter: public CommandProcessor
{
    Q_OBJECT

public:
    Logouter(NetworkManager *networkManager, QObject *parent = 0);
    void process();

private slots:
    void processResponse(bool ok, QString const &msg);

private:
    NetworkManager *m_networkManager;
};


#endif // LOGOUTER_H
