#include "PollingManager.hpp"


PollingManager::PollingManager(CommandProcessor *processor, int msec,
                               QObject *parent):
    QObject(parent),
    m_processor(processor),
    m_msec(msec),
    m_timer(new QTimer(this))
{
    m_processor->setParent(this);
    connect(m_timer, SIGNAL(timeout()), m_processor, SLOT(process()));
    connect(m_processor, SIGNAL(result(QString const &)),
            this, SLOT(reemitResult(QString const &)));
    connect(m_processor, SIGNAL(error(QString const &)),
            this, SLOT(reemitError(QString const &)));
}


void PollingManager::start(int msec)
{
    m_timer->start(msec);
}


void PollingManager::start()
{
    m_timer->start(m_msec);
}


void PollingManager::stop()
{
    m_timer->stop();
}


void PollingManager::reemitResult(QString const &msg)
{
    emit result(msg);
}


void PollingManager::reemitError(QString const &errMsg)
{
    emit error(errMsg);
}
