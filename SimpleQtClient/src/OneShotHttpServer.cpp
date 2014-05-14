#include "OneShotHttpServer.hpp"

#include <QStringList>
#include <QRegExp>


OneShotHttpServer::OneShotHttpServer(int port, QObject *parent):
    QObject(parent),
    m_tcpServer(new QTcpServer(this)),
    m_tcpSocket(0)
{
    connect(m_tcpServer, SIGNAL(newConnection()),
            this, SLOT(processConnection()));
    connect(m_tcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this, SLOT(processError(QAbstractSocket::SocketError)));

    if (!m_tcpServer->listen(QHostAddress::LocalHost, port))
        emit error("HTTP listening error");
}


void OneShotHttpServer::processConnection()
{
    m_tcpSocket = m_tcpServer->nextPendingConnection();
    if (!m_tcpSocket) {
        emit error("HTTP error");
        return;
    }
    m_tcpServer->close();

    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(discard()));
}


void OneShotHttpServer::processError(QAbstractSocket::SocketError)
{
    emit error(m_tcpServer->errorString());
}


void OneShotHttpServer::readData()
{
    if (!m_tcpSocket->canReadLine())
        return;

    QString query(m_tcpSocket->readLine());
    QStringList tokens(query.split(QRegExp("[ \n\r]+")));
    bool ok = tokens.size() >= 2 && tokens[0] == "GET";

    QTextStream out(m_tcpSocket);
    out << "HTTP/1.1 200 OK\n"
           "Content-Type: text/html; charset=\"utf-8\"\n"
           "\n";
    if (ok) {
        out << "<html><script>window.close();</script>"
               "<body><p>Ok. Please, close the window</p></body></html>";
    } else {
        out << "<html><body><p>Authorization failed</p></body></html>";
    }
    out << flush;

    m_tcpSocket->waitForBytesWritten();
    disconnect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(discard()));
    m_tcpSocket->close();
    m_tcpServer->close();

    if (ok)
        emit result(tokens[1]);
    else
        emit error("Incorrect HTTP request");
}


void OneShotHttpServer::discard()
{
    m_tcpServer->close();
    emit error("HTTP discard");
}
