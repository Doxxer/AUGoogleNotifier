#include "Controller.hpp"
#include "AuthorizationChecker.hpp"
#include "Unsubscriber.hpp"
#include "Subscriber.hpp"
#include "Authorizer.hpp"
#include "Logouter.hpp"
#include "defs.hpp"

#include <QDesktopServices>
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QMenu>
#include <QIcon>
#include <QUrl>


Controller::Controller(QObject *parent):
    QObject(parent),
    m_networkManager(new NetworkManager(APPSERVER, this)),
    m_changesPoller(new ChangesPoller(m_networkManager, this)),
    m_resubscriber(new Resubscriber(m_networkManager, this)),
    m_notifier(new Notifier(this, this))
{
    AuthorizationChecker *checker = new AuthorizationChecker(
            m_networkManager, this);
    connect(checker, SIGNAL(result(bool)), this, SLOT(prepare(bool)));
    connect(checker, SIGNAL(error()), this, SLOT(processCritical()));
    checker->check();

    connect(m_changesPoller, SIGNAL(result(QString const &)),
            m_notifier, SLOT(notify(QString const &)));
}


void Controller::setLastChanged(QString const &url)
{
    m_lastChanged = url;
    m_lastChangedAction->setEnabled(true);
}


void Controller::showMessage(QString const &title, QString const &msg,
                             QSystemTrayIcon::MessageIcon icon)
{
    m_trayIcon->showMessage(title, msg, icon);
}


void Controller::prepare(bool authorized)
{
    createGui();

    m_authorizeAction->setEnabled(!authorized);
    m_logoutAction->setEnabled(authorized);

    if (authorized) {
        m_subscribed = loadSubscribed();
        resetSubscriptionActions();
        if (m_subscribed) {
            m_subscribeAction->trigger();
            subscribe();
        } else {
            m_authorizeAction->trigger();
        }
    } else {
        m_subscribeAction->setEnabled(false);
        m_unsubscribeAction->setEnabled(false);
        m_subscribed = false;
        saveSubscribed();
    }
}


void showMessageBox(QString const &msg,
                    QMessageBox::Icon icon = QMessageBox::Information)
{
    QMessageBox msgBox(icon, APP_NAME, msg, QMessageBox::Ok, 0, Qt::Sheet);
    msgBox.exec();
}


void Controller::processCritical()
{
    showMessageBox("Couldn't connect to server", QMessageBox::Critical);
    qApp->exit();
}


void Controller::openRecent()
{
    QDesktopServices::openUrl(QUrl(RECENT_URL));
}


void Controller::openLastChanged()
{
    QDesktopServices::openUrl(QUrl(m_lastChanged));
}


void Controller::processError(QString const &errMsg)
{
    showMessage("Warning!", errMsg, QSystemTrayIcon::Warning);
}


void Controller::processAuth(QString const &msg)
{
    showMessage("Success", msg);

    m_authorizeAction->setEnabled(false);
    m_logoutAction->setEnabled(true);

    m_subscribed = loadSubscribed();
    resetSubscriptionActions();
}


void Controller::processLogout(QString const &msg)
{
    showMessage("Success", msg);

    unsubscribe();

    m_authorizeAction->setEnabled(true);
    m_logoutAction->setEnabled(false);
    m_subscribeAction->setEnabled(false);
    m_unsubscribeAction->setEnabled(false);

    m_subscribed = false;
    saveSubscribed();
}


void Controller::processSubscribe(QString const &msg)
{
    showMessage("Success", msg);

    m_subscribeAction->setEnabled(false);
    m_unsubscribeAction->setEnabled(true);

    m_subscribed = true;
    saveSubscribed();

    subscribe();
}


void Controller::processUnsubscribe(QString const &msg)
{
    unsubscribe();

    showMessage("Success", msg);

    m_subscribeAction->setEnabled(true);
    m_unsubscribeAction->setEnabled(false);

    m_subscribed = false;
    saveSubscribed();
}


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


void Controller::createGui()
{
    m_trayIcon = new QSystemTrayIcon(QIcon(":/resources/icon.png"), this);
    connect(m_trayIcon, SIGNAL(messageClicked()), this,
            SLOT(openLastChanged()));

    QMenu *menu = new QMenu();

    createAction(menu, "Open &recent documents", this, SLOT(openRecent()));
    m_lastChangedAction = createAction(menu, "&Open last changed document",
                                       this, SLOT(openLastChanged()));
    m_lastChangedAction->setDisabled(true);
    menu->addSeparator();

    Authorizer *authorizer = new Authorizer(m_networkManager, this);
    m_authorizeAction = connectProcessor(
            authorizer, menu, "&Authorize",
            this, SLOT(processAuth(QString const &)));
    Logouter *logouter = new Logouter(m_networkManager, this);
    m_logoutAction = connectProcessor(
            logouter, menu, "&Logout",
            this, SLOT(processLogout(QString const &)));
    menu->addSeparator();

    Subscriber *subscriber = new Subscriber(m_networkManager, this);
    m_subscribeAction = connectProcessor(
            subscriber, menu, "&Subscribe",
            this, SLOT(processSubscribe(QString const &)));
    Unsubscriber *unsubscriber = new Unsubscriber(m_networkManager, this);
    m_unsubscribeAction = connectProcessor(
            unsubscriber, menu, "&Unsubscribe",
            this, SLOT(processUnsubscribe(QString const &)));
    menu->addSeparator();

    createAction(menu, "&Exit", qApp, SLOT(quit()));

    m_trayIcon->setContextMenu(menu);
    m_trayIcon->setVisible(true);
}


bool Controller::loadSubscribed()
{
    QSettings settings;
    return settings.contains(SUBSCRIBED_SETTING)
            && settings.value(SUBSCRIBED_SETTING).toBool();
}


void Controller::saveSubscribed()
{
    QSettings settings;
    if (!settings.isWritable())
        return;
    settings.setValue(SUBSCRIBED_SETTING, m_subscribed);
    settings.sync();
}


void Controller::resetSubscriptionActions()
{
    m_subscribeAction->setEnabled(!m_subscribed);
    m_unsubscribeAction->setEnabled(m_subscribed);
}


void Controller::subscribe()
{
    m_changesPoller->start();
    m_resubscriber->start();
}


void Controller::unsubscribe()
{
    m_changesPoller->stop();
    m_resubscriber->stop();
}
