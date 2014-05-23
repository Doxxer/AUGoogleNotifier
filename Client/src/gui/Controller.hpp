#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QSystemTrayIcon>
#include <QAction>

#include "ChangesPoller.hpp"
#include "NetworkManager.hpp"
#include "Resubscriber.hpp"
#include "Notifier.hpp"


class Controller: public QObject
{
    Q_OBJECT

public:
    Controller(QObject *parent = 0);
    void setLastChanged(QString const &url);
    void showMessage(
            QString const &title, QString const &msg,
            QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information);

private slots:
    void prepare(bool authorized);
    void processCritical();
    void openRecent();
    void openLastChanged();
    void processError(QString const &errMsg);
    void processAuth(QString const &msg);
    void processLogout(QString const &msg);
    void processSubscribe(QString const &msg);
    void processUnsubscribe(QString const &msg);

private:
    void createGui();
    bool loadSubscribed();
    void saveSubscribed();
    void resetSubscriptionActions();
    void subscribe();
    void unsubscribe();

    NetworkManager *m_networkManager;
    ChangesPoller *m_changesPoller;
    Resubscriber *m_resubscriber;
    Notifier *m_notifier;

    QSystemTrayIcon *m_trayIcon;
    QAction *m_lastChangedAction;
    QAction *m_authorizeAction;
    QAction *m_logoutAction;
    QAction *m_subscribeAction;
    QAction *m_unsubscribeAction;

    bool m_subscribed;
    QString m_lastChanged;
};


#endif // CONTROLLER_HPP
