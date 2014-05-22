#include "Notifier.hpp"

#include <QDebug>


Notifier::Notifier(QObject *parent):
    QObject(parent)
{
}


void Notifier::notify(QString const &json)
{
    qDebug() << json;
}
