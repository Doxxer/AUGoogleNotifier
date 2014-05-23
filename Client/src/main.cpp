#include <QScopedPointer>
#include <QApplication>
#include <QTimer>

#include "Controller.hpp"
#include "defs.hpp"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QApplication::setOrganizationName(ORG_NAME);
    QApplication::setOrganizationDomain(ORG_DOMAIN);
    QApplication::setApplicationName(APP_NAME);

    QScopedPointer<Controller> controller(new Controller());

    return app.exec();
}
