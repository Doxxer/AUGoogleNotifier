#include "CookieJar.hpp"

#include <QNetworkCookie>
#include <QDataStream>
#include <QSettings>
#include <QFile>


QString const ARRAY_NAME = "cookies";
QString const ELEM_NAME = "cookie";


CookieJar::CookieJar(QObject *parent):
    QNetworkCookieJar(parent)
{
}


bool CookieJar::load()
{
    QSettings settings;
    int size = settings.beginReadArray(ARRAY_NAME);
    if (!size)
        return false;

    QList<QNetworkCookie> cookies;
    cookies.reserve(size);
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QByteArray raw(settings.value(ELEM_NAME).toByteArray());
        QList<QNetworkCookie> parsed(QNetworkCookie::parseCookies(raw));
        cookies.append(parsed);
    }

    setAllCookies(cookies);
    return settings.status() == QSettings::NoError;
}


bool CookieJar::save() const
{
    QSettings settings;
    if (!settings.isWritable())
        return false;

    QList<QNetworkCookie> const &cookies = allCookies();
    settings.beginWriteArray(ARRAY_NAME);
    for (int i = 0; i < cookies.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue(ELEM_NAME, cookies[i].toRawForm());
    }
    settings.endArray();

    settings.sync();
    return settings.status() == QSettings::NoError;
}
