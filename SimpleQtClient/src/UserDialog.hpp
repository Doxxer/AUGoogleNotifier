#ifndef USERDIALOG_HPP
#define USERDIALOG_HPP

#include <QTextStream>
#include <QString>
#include <QObject>
#include <QHash>

#include "CommandProcessor.hpp"
#include "NetworkManager.hpp"


class UserDialog: public QObject
{
    Q_OBJECT

public:
    explicit UserDialog(QObject *parent = 0);

signals:
    void quit();

public slots:
    void run();

private slots:
    void readCommand();
    void writeResult(QString const &msg);
    void processError(QString const &errMsg);

private:
    typedef QHash<QString, CommandProcessor *> Dict;

    void showUsage();
    void startReading();
    void addCommandProcessor(CommandProcessor *processor, QString const &cmd);

    QTextStream m_cin;
    QTextStream m_cout;
    Dict m_processors;
    NetworkManager *m_networkManager;
};


#endif // USERDIALOG_HPP
