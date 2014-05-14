#ifndef ONESHOTHTTPSERVER_HPP
#define ONESHOTHTTPSERVER_HPP

#include <QTcpServer>
#include <QTcpSocket>
#include <QString>


class OneShotHttpServer: public QObject
{
    Q_OBJECT

public:
    explicit OneShotHttpServer(int port, QObject *parent = 0);

signals:
    void result(QString const &msg);
    void error(QString const &errMsg);

private slots:
    void processConnection();
    void processError(QAbstractSocket::SocketError);
    void readData();
    void discard();

private:
    QTcpServer *m_tcpServer;
    QTcpSocket *m_tcpSocket;
};


#endif // ONESHOTHTTPSERVER_HPP
