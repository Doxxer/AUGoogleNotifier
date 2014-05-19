#include <QScopedPointer>
#include <QApplication>
#include <QTimer>

#include "Controller.hpp"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QApplication::setOrganizationName("SPbAU");
    QApplication::setOrganizationDomain("mit.spbau.ru");
    QApplication::setApplicationName("AUGoogleNotifier");

    QScopedPointer<Controller> ud(new Controller());

    return app.exec();
}
