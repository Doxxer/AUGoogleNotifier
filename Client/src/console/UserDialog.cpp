#include "ChangesGetter.hpp"
#include "Unsubscriber.hpp"
#include "UserDialog.hpp"
#include "Authorizer.hpp"
#include "Subscriber.hpp"
#include "Logouter.hpp"
#include "defs.hpp"

#include <QTimer>


UserDialog::UserDialog(QObject *parent):
    QObject(parent),
    m_cin(stdin),
    m_cout(stdout),
    m_networkManager(new NetworkManager(APPSERVER, this))
{
    addCommandProcessor(new Authorizer(m_networkManager, this), "a");
    addCommandProcessor(new Logouter(m_networkManager, this), "l");
    addCommandProcessor(new Subscriber(m_networkManager, this), "s");
    addCommandProcessor(new Unsubscriber(m_networkManager, this), "u");
    addCommandProcessor(new ChangesGetter(m_networkManager, this), "g");
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
    startReading();
}


void UserDialog::processError(QString const &errMsg)
{
    m_cout << errMsg << endl;
    startReading();
}


void UserDialog::showUsage()
{
    m_cout << "Usage: a | l | s | g | q\n"
              "a -- try to authenticate\n"
              "l -- logout\n"
              "s -- subscribe\n"
              "g -- get changes\n"
              "u -- unsubscribe\n"
              "q -- quit\n";
    m_cout << endl;
}


void UserDialog::startReading()
{
    QTimer::singleShot(0, this, SLOT(readCommand()));
}


void UserDialog::addCommandProcessor(CommandProcessor *processor,
                                     QString const &cmd)
{
    connect(processor, SIGNAL(result(QString const &)),
            this, SLOT(writeResult(QString const &)));
    connect(processor, SIGNAL(error(QString const &)),
            this, SLOT(processError(QString const &)));
    m_processors[cmd] = processor;
}
