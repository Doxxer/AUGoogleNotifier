#include <QCoreApplication>
#include <QScopedPointer>
#include <QTimer>

#include "UserDialog.hpp"


int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QScopedPointer<UserDialog> ud(new UserDialog());

    QObject::connect(ud.data(), SIGNAL(quit()), &app, SLOT(quit()));
    QTimer::singleShot(0, ud.data(), SLOT(run()));

    return app.exec();
}
