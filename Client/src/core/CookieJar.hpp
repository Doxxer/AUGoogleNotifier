#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QNetworkCookieJar>


class CookieJar: public QNetworkCookieJar
{
public:
    CookieJar(QObject *parent = 0);
    bool load();
    bool save() const;
};


#endif // COOKIEJAR_H
