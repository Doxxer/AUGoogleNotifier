#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QSystemTrayIcon>
#include <QAction>

#include "NetworkManager.hpp"
#include "Resubscriber.hpp"


class Controller: public QObject
{
    Q_OBJECT

public:
    Controller(QObject *parent = 0);

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

    NetworkManager *m_networkManager;
    Resubscriber *m_resubscriber;

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
