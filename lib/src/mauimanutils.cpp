#include "mauimanutils.h"

#if !defined Q_OS_ANDROID
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusServiceWatcher>
#endif

#include <QDebug>
#include <QStringList>

#include <QProcess>

static const QString mauimanInterface(QStringLiteral("org.mauiman.Manager"));

MauiManUtils::MauiManUtils(QObject *parent)
    : QObject{parent}
{
#if !defined Q_OS_ANDROID
    const QDBusConnection bus = QDBusConnection::sessionBus();
    const auto registeredServices = bus.interface()->registeredServiceNames();

    if (registeredServices.isValid())
    {
        m_serverRunning = registeredServices.value().contains(mauimanInterface);
    }

    auto watcher = new QDBusServiceWatcher(mauimanInterface, QDBusConnection::sessionBus(), QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration, this);

    connect(watcher, &QDBusServiceWatcher::serviceRegistered, [=](const QString &name) {
        qDebug() << "Connected to MauiMan server" << name;
        m_serverRunning = true;
        emit serverRunningChanged(m_serverRunning);
    });

    connect(watcher, &QDBusServiceWatcher::serviceUnregistered, [=](const QString &name) {
        qDebug() << "Disconnected to MauiMan server" << name;
        m_serverRunning = false;
        emit serverRunningChanged(m_serverRunning);
    });
#endif
}

bool MauiManUtils::serverRunning() const
{
    return m_serverRunning;
}

void MauiManUtils::startServer()
{
#if !defined Q_OS_ANDROID
    QProcess::startDetached("MauiManServer", QStringList());
#endif
}

void MauiManUtils::invokeManager(const QString &module)
{
    QProcess::startDetached("MauiSettings", QStringList() << "-m" << module);
}

QString MauiManUtils::currentDesktopSession()
{
    if(qEnvironmentVariableIsSet("XDG_CURRENT_DESKTOP"))
    {
        const auto names = qEnvironmentVariable("XDG_CURRENT_DESKTOP").split(";");
        return names.first();
    }

    return QString();
}

bool MauiManUtils::isMauiSession()
{
  return currentDesktopSession() == "CASK";
}

bool MauiManUtils::isPlasmaSession()
{
    return currentDesktopSession() == "KDE";
}

bool MauiManUtils::isGnomeSession()
{
    return currentDesktopSession() == "Gnome";
}
