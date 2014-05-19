#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QSystemTrayIcon>


class Controller: public QObject
{
    Q_OBJECT

public:
    Controller(QObject *parent = 0);

private:
    void createGui();

    QSystemTrayIcon *m_trayIcon;
};


#endif // CONTROLLER_HPP
