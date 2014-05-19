#include "Controller.hpp"

#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QIcon>


Controller::Controller(QObject *parent):
    QObject(parent)
{
    createGui();
}


void Controller::createGui()
{
    m_trayIcon = new QSystemTrayIcon(
            QIcon(QApplication::applicationDirPath() + "/icon.png"),
            this);

    QMenu *menu = new QMenu();

    QAction *quitAction = new QAction("&Exit", menu);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    menu->addAction(quitAction);

    QAction *testAction = new QAction("Test", menu);
    connect(testAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    menu->addAction(testAction);

    m_trayIcon->setContextMenu(menu);

    m_trayIcon->show();
}
