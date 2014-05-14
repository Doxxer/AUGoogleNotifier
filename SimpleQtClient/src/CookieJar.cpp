#include "CookieJar.hpp"

#include <QNetworkCookie>
#include <QDataStream>
#include <QFile>


void writeCookies(QDataStream &out, QList<QNetworkCookie> const &list)
{
    out << static_cast<quint32>(list.size());
    for (int i = 0; i < list.size(); ++i) {
        out << list[i].toRawForm();
    }
}


bool readCookies(QDataStream &in, QList<QNetworkCookie> &list)
{
    list.clear();

    quint32 count;
    in >> count;
    bool ok = true;

    for (quint32 i = 0; i < count; ++i) {
        QByteArray raw;
        in >> raw;
        QList<QNetworkCookie> cookies = QNetworkCookie::parseCookies(raw);
        ok = ok && !(cookies.size() == 0 && raw.size() != 0);
        for (int j = 0; j < cookies.size(); ++j) {
            list.append(cookies[j]);
        }
        if (in.atEnd())
            break;
    }

    return ok;
}


CookieJar::CookieJar(QObject *parent):
    QNetworkCookieJar(parent)
{
}


bool CookieJar::load(QString const &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    QDataStream in(&file);

    QList<QNetworkCookie> cookies;
    if (!readCookies(in, cookies))
        return false;
    setAllCookies(cookies);

    return true;
}


bool CookieJar::save(QString const &fileName) const
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
        return false;
    QDataStream out(&file);

    QList<QNetworkCookie> cookies;
    writeCookies(out, allCookies());

    return true;
}
