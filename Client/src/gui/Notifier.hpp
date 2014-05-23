#ifndef NOTIFIER_HPP
#define NOTIFIER_HPP

#include <QObject>
#include <QString>


class Controller;

class Notifier: public QObject
{
    Q_OBJECT

public:
    explicit Notifier(Controller *controller, QObject *parent = 0);

public slots:
    void notify(QString const &json);

private:
    void show(QString const &url, QString const &title,
              QString const &userName);

private:
    Controller *m_controller;
};


#endif // NOTIFIER_HPP
