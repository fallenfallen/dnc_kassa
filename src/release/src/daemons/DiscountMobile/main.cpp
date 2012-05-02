#include <QCoreApplication>

#include "manager.h"

int main(int argc, char ** argv)
{
//    daemon(0, 0);

    QCoreApplication app(argc, argv);

    Manager * manager = new Manager;
    manager->manage();

    return app.exec();
}

