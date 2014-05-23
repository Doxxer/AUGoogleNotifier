#ifndef POLLINGMANAGER_HPP
#define POLLINGMANAGER_HPP

#include <QTimer>

#include "CommandProcessor.hpp"


class PollingManager: public QObject
{
    Q_OBJECT

public:
    PollingManager(CommandProcessor *processor, int msec, QObject *parent = 0);
    void start(int msec);
    void start();
    void stop();

signals:
    void result(QString const &msg);
    void error(QString const &errMsg);

private slots:
    void reemitResult(QString const &msg);
    void reemitError(QString const &errMsg);

private:
    CommandProcessor *m_processor;
    int const m_msec;
    QTimer *m_timer;
};


#endif // POLLINGMANAGER_HPP
