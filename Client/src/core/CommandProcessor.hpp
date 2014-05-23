#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <QString>
#include <QObject>


class CommandProcessor: public QObject
{
    Q_OBJECT

public:
    explicit CommandProcessor(QObject *parent = 0):
        QObject(parent)
    {
    }

public slots:
    virtual void process() = 0;

signals:
    void result(QString const &msg);
    void error(QString const &errMsg);
};


#endif // COMMANDPROCESSOR_H
