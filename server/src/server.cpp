#include "server.h"
#include <QDBusConnectionInterface>
#include <QDebug>

#include "modules/background.h"
#include "modules/theme.h"
#include "modules/screen.h"
#include "modules/formfactor.h"
#include "modules/accessibility.h"

#include <signal.h>


void sigtermHandler(int signalNumber)
{
    qDebug() << "terminating cask session" << signalNumber;
    if (QCoreApplication::instance()) {
        QCoreApplication::instance()->exit(-1);
        qDebug() << "terminating caks session FINISHED" << signalNumber;

    }
}

void sigHandler(int signalNumber)
{
    qDebug() << "terminating MauiManServer session" << signalNumber;
}

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

    signal(SIGTERM, sigtermHandler);
    signal(SIGINT, sigHandler);

    m_modules << new Background();
    m_modules << new Theme();
    m_modules << new Screen();
    m_modules << new FormFactor();
    m_modules << new Accessibility();
    return true;
}
