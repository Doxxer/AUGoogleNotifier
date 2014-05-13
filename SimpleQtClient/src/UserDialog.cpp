#include "UserDialog.hpp"
#include "Authorizer.hpp"
#include "defs.hpp"

#include <QTimer>


UserDialog::UserDialog(QObject *parent):
    QObject(parent),
    m_cin(stdin),
    m_cout(stdout),
    m_networkManager(new NetworkManager(APPSERVER, COOKIES, this))
{
    Authorizer *authorizer = new Authorizer(m_networkManager, this);
    connect(authorizer, SIGNAL(result(QString const &)),
            this, SLOT(writeResult(QString const &)));
    connect(authorizer, SIGNAL(error(QString const &)),
            this, SLOT(processError(QString const &)));
    m_processors["a"] = authorizer;
}


void UserDialog::run()
{
    showUsage();
    startReading();
}


void UserDialog::readCommand()
{
    m_cout << ">> " << flush;
    QString cmd;
    m_cin >> cmd;
    cmd = cmd.trimmed().toLower();

    if (cmd == "q") {
        emit quit();
        return;
    }

    Dict::const_iterator it(m_processors.find(cmd));
    if (it != m_processors.end()) {
        it.value()->process();
    } else {
        showUsage();
        startReading();
    }
}


void UserDialog::writeResult(QString const &msg)
{
    m_cout << msg << endl;
    //startReading();
}


void UserDialog::processError(QString const &errMsg)
{
    m_cout << errMsg << endl;
    emit quit();
}


void UserDialog::showUsage()
{
    m_cout << "Usage: a | l | s | g | q\n"
              "a -- try to authenticate\n"
              "l -- logout\n"
              "s -- subscribe\n"
              "g -- get changes\n"
              "q -- quit\n";
    m_cout << endl;
}


void UserDialog::startReading()
{
    QTimer::singleShot(0, this, SLOT(readCommand()));
}
