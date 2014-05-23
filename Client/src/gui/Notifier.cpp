#include "Notifier.hpp"
#include "Controller.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcess>


Notifier::Notifier(Controller *controller, QObject *parent):
    QObject(parent),
    m_controller(controller)
{
}


void Notifier::show(QString const &url, QString const &title,
                    QString const &userName)
{
#ifdef Q_OS_LINUX
    QProcess::execute(
            "notify-send",
            QStringList() << title
                          << userName
                          << "-i" << "gtk-info");
#elif defined Q_OS_OSX
    QProcess::execute(
            "terminal-notifier",
            QStringList() << "-title" << title
                          << "-subtitle" << userName
                          << "-open" << url
                          << "-message" << "Document is modified");
#else
    m_controller->showMessage(title, userName);
#endif
}


void Notifier::notify(QString const &json)
{
    QJsonObject jsonObject(QJsonDocument::fromJson(json.toUtf8()).object());
    QJsonObject::ConstIterator it(jsonObject.constFind("changes_list"));
    if (it == jsonObject.constEnd())
        return;

    QJsonArray changesArray(it.value().toArray());
    if (changesArray.isEmpty())
        return;

    for (int i = 0; i < changesArray.size(); ++i) {
        QJsonObject entry(changesArray[i].toObject());

        QString url(entry.value("URL").toString());
        QString title(entry.value("title").toString());
        QString userName(entry.value("user_name").toString());

        show(url, title, userName);

        if (i == changesArray.size() - 1)
            m_controller->setLastChanged(url);
    }
}
