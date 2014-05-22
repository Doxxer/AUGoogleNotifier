#ifndef NOTIFIER_HPP
#define NOTIFIER_HPP

#include <QObject>
#include <QString>


class Notifier: public QObject
{
    Q_OBJECT

public:
    explicit Notifier(QObject *parent = 0);

public slots:
    void notify(QString const &json);
};


#endif // NOTIFIER_HPP
