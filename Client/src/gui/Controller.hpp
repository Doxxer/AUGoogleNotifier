#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QSystemTrayIcon>


class Controller: public QObject
{
    Q_OBJECT

public:
    Controller(QObject *parent = 0);

public slots:
    void run();

signals:
    void quit();

private slots:
    void emitQuit();

private:
    void createGui();

    QSystemTrayIcon *m_trayIcon;
};


#endif // CONTROLLER_HPP
