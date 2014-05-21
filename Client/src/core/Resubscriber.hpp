#ifndef RESUBSCRIBER_HPP
#define RESUBSCRIBER_HPP

#include <QTimer>

#include "NetworkManager.hpp"
#include "defs.hpp"


class Resubscriber: QObject
{
    Q_OBJECT

public:
    explicit Resubscriber(NetworkManager *networkManager, QObject *parent = 0);
    void start(int msec = RESUBSCRIBE_TIMEOUT);
    void stop();

private:
    QTimer *m_timer;
};


#endif // RESUBSCRIBER_HPP
