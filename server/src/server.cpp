#include "server.h"
#include <QDBusConnectionInterface>
#include <QDebug>

#include "src/modules/background.h"
#include "src/modules/theme.h"

Server::Server(int &argc, char **argv) : QCoreApplication(argc, argv)
{

}

Server::~Server()
{
    qDeleteAll(m_modules);
}


bool Server::init()
{
    QDBusConnectionInterface *iface = QDBusConnection::sessionBus().interface();

    if(iface->isServiceRegistered("org.mauiman.Manager"))
    {
        qWarning() << "Service is already registered";
        return false;
    }

    auto registration = iface->registerService(QStringLiteral("org.mauiman.Manager"),
                                               QDBusConnectionInterface::ReplaceExistingService,
                                               QDBusConnectionInterface::DontAllowReplacement);

    if (!registration.isValid())
    {
        qWarning("2 Failed to register D-Bus service \"%s\" on session bus: \"%s\"",
                 qPrintable("org.mauiman.Manager"),
                 qPrintable(registration.error().message()));

        return false;
    }

    m_modules << new Background(this);
    m_modules << new Theme(this);
    return true;
}
