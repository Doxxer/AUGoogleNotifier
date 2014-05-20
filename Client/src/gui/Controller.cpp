#include "Controller.hpp"
#include "NetworkManager.hpp"
#include "Unsubscriber.hpp"
#include "Subscriber.hpp"
#include "Authorizer.hpp"
#include "Logouter.hpp"
#include "defs.hpp"

#include <QDesktopServices>
#include <QApplication>
#include <QMessageBox>
#include <QMenu>
#include <QIcon>
#include <QUrl>


QAction *createAction(QMenu *menu, QString const &text,
                      QObject *receiver, char const *slot)
{
    QAction *action = new QAction(text, menu);
    QObject::connect(action, SIGNAL(triggered()), receiver, slot);
    menu->addAction(action);
    return action;
}


QAction *connectProcessor(CommandProcessor *processor, QMenu *menu,
                          QString const &text,
                          QObject *receiver, char const *slot)
{
    QObject::connect(processor, SIGNAL(result(QString const &)),
                     receiver, slot);
    QObject::connect(processor, SIGNAL(error(QString const &)),
                     receiver, SLOT(processError(QString const &)));
    return createAction(menu, text, processor, SLOT(process()));
}


Controller::Controller(QObject *parent):
    QObject(parent)
{
    m_trayIcon = new QSystemTrayIcon(QIcon(":/resources/icon.png"), this);
    QMenu *menu = new QMenu();

    createAction(menu, "Open &recent documents", this, SLOT(openRecent()));
    m_lastChangedAction = createAction(menu, "&Open last changed document",
                                       this, SLOT(openLastChanged()));
    m_lastChangedAction->setDisabled(true);
    menu->addSeparator();

    NetworkManager *networkManager = new NetworkManager(APPSERVER, this);
    Authorizer *authorizer = new Authorizer(networkManager, this);
    m_authorizeAction = connectProcessor(
            authorizer, menu, "&Authorize",
            this, SLOT(processAuth(QString const &)));
    Logouter *logouter = new Logouter(networkManager, this);
    m_logoutAction = connectProcessor(
            logouter, menu, "&Logout",
            this, SLOT(processLogout(QString const &)));
    m_logoutAction->setEnabled(false);
    menu->addSeparator();

    Subscriber *subscriber = new Subscriber(networkManager, this);
    m_subscribeAction = connectProcessor(
            subscriber, menu, "&Subscribe",
            this, SLOT(processSubscribe(QString const &)));
    m_subscribeAction->setEnabled(false);
    Unsubscriber *unsubscriber = new Unsubscriber(networkManager, this);
    m_unsubscribeAction = connectProcessor(
            unsubscriber, menu, "&Unsubscribe",
            this, SLOT(processUnsubscribe(QString const &)));
    m_unsubscribeAction->setEnabled(false);
    menu->addSeparator();

    createAction(menu, "&Exit", qApp, SLOT(quit()));

    m_trayIcon->setContextMenu(menu);
    m_trayIcon->setVisible(true);
}


void Controller::openRecent()
{
    QDesktopServices::openUrl(QUrl(RECENT_URL));
}


void Controller::openLastChanged()
{
    QDesktopServices::openUrl(QUrl(m_lastChanged));
}


void showInfoBox(QString const &msg)
{
    QMessageBox::information(0, "Success", msg);
}


void Controller::processError(QString const &errMsg)
{
    QMessageBox::warning(0, "Failure", errMsg);
}


void Controller::processAuth(QString const &msg)
{
    showInfoBox(msg);
    m_authorizeAction->setEnabled(false);
    m_logoutAction->setEnabled(true);
    m_subscribeAction->setEnabled(true);
}


void Controller::processLogout(QString const &msg)
{
    showInfoBox(msg);
    m_authorizeAction->setEnabled(true);
    m_logoutAction->setEnabled(false);
    m_subscribeAction->setEnabled(false);
}


void Controller::processSubscribe(QString const &msg)
{
    showInfoBox(msg);
    m_subscribeAction->setEnabled(false);
    m_unsubscribeAction->setEnabled(true);
}


void Controller::processUnsubscribe(QString const &msg)
{
    showInfoBox(msg);
    m_subscribeAction->setEnabled(true);
    m_unsubscribeAction->setEnabled(false);
}
