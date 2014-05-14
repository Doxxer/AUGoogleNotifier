#ifndef CHANGESGETTER_H
#define CHANGESGETTER_H

#include "CommandProcessor.hpp"
#include "NetworkManager.hpp"


class ChangesGetter: public CommandProcessor
{
    Q_OBJECT

public:
    ChangesGetter(NetworkManager *networkManager, QObject *parent = 0);
    void process();

private slots:
    void processResponse(bool ok, QString const &msg);

private:
    NetworkManager *m_networkManager;
};


#endif // CHANGESGETTER_H
