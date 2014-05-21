#include "Resubscriber.hpp"
#include "Subscriber.hpp"


Resubscriber::Resubscriber(NetworkManager *networkManager, QObject *parent):
    QObject(parent),
    m_timer(new QTimer(this))
{
    Subscriber *subscriber = new Subscriber(networkManager, this);
    connect(m_timer, SIGNAL(timeout()), subscriber, SLOT(process()));
}


void Resubscriber::start(int msec)
{
    m_timer->start(msec);
}


void Resubscriber::stop()
{
    m_timer->stop();
}
