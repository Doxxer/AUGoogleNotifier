#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QNetworkCookieJar>


class CookieJar: public QNetworkCookieJar
{
public:
    CookieJar(QObject *parent = 0);
    bool load(QString const &fileName);
    bool save(QString const &fileName) const;
};


#endif // COOKIEJAR_H
