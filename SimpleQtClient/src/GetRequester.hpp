#ifndef GETREQUESTER_H
#define GETREQUESTER_H

#include "CommandProcessor.hpp"
#include "NetworkManager.hpp"


class GetRequester: public CommandProcessor
{
    Q_OBJECT

public:
    GetRequester(QString const &path, NetworkManager *networkManager,
                 QObject *parent = 0);
    void process();

private slots:
    void processResponse(bool ok, QString const &msg);

private:
    QString const m_path;
    NetworkManager *m_networkManager;
};


#endif // GETREQUESTER_H
