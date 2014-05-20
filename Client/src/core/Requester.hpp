#ifndef REQUESTER_H
#define REQUESTER_H

#include "CommandProcessor.hpp"
#include "NetworkManager.hpp"


class Requester: public CommandProcessor
{
    Q_OBJECT

public:
    enum RequestType { GET, POST };

    Requester(RequestType type, QString const &path,
              NetworkManager *networkManager, QObject *parent = 0);

public slots:
    void process();

protected:
    virtual QString const prepareResult(QString const &msg) const;
    virtual QString const prepareError(QString const &errMsg) const;

private slots:
    void processResponse(bool ok, QString const &msg);

private:
    RequestType const m_type;
    QString const m_path;
    NetworkManager *m_networkManager;
};


#endif // REQUESTER_H
