#include <QApplication>

#include "manager.h"

#include <QNetworkProxy>
#include <QNetworkProxyQuery>
#include <QNetworkProxyFactory>

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    Manager * manager = new Manager;
    manager->manage();

    return app.exec();
}
