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


void Notifier::show(QString const &title, QString const &userName)
{
#ifdef Q_OS_LINUX
    QProcess::execute(
            "notify-send",
            QStringList() << title
                          << userName
                          << "-i" << "gtk-info");
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

        m_controller->setLastChanged(url);
        show(title, userName);
    }
}
