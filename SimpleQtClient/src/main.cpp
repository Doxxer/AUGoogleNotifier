#include <QScopedPointer>
#include <QApplication>
#include <QTimer>

#include "UserDialog.hpp"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QScopedPointer<UserDialog> ud(new UserDialog());

    QObject::connect(ud.data(), SIGNAL(quit()), &app, SLOT(quit()));
    QTimer::singleShot(0, ud.data(), SLOT(run()));

    return app.exec();
}
