#include "screenmanager.h"

#include "code/settingsstore.h"
#include "code/mauimanutils.h"

#include <QDebug>

#if !defined Q_OS_ANDROID
#include <QDBusInterface>
#endif

using namespace MauiMan;

ScreenManager::ScreenManager(QObject * parent ) : QObject(parent)
  ,m_settings(new MauiMan::SettingsStore(this))
{
    qDebug( " INIT SCREEN MANAGER");

#if !defined Q_OS_ANDROID
    auto server = new MauiManUtils(this);
    if(server->serverRunning())
    {
        this->setConnections();
    }

    connect(server, &MauiManUtils::serverRunningChanged, [this](bool state)
    {
        if(state)
        {
            this->setConnections();
        }
    });
#endif

    loadSettings();
}

double ScreenManager::scaleFactor() const
{
    return m_scaleFactor;
}

void ScreenManager::setScaleFactor(double scaleFactor)
{
    if (m_scaleFactor == scaleFactor)
        return;

    m_scaleFactor = scaleFactor;
    sync("setScaleFactor", m_scaleFactor);
    m_settings->save("ScaleFactor", m_scaleFactor);
    Q_EMIT scaleFactorChanged(m_scaleFactor);
}

void ScreenManager::onScaleFactorChanged(double scale)
{
    if (m_scaleFactor == scale)
        return;

    m_scaleFactor = scale;
    Q_EMIT scaleFactorChanged(m_scaleFactor);
}

void ScreenManager::sync(const QString &key, const QVariant &value)
{
#if !defined Q_OS_ANDROID
    if (m_interface && m_interface->isValid())
    {
        m_interface->call(key, value);
    }
#else
    Q_UNUSED(key)
    Q_UNUSED(value)
#endif
}

void ScreenManager::setConnections()
{
#if !defined Q_OS_ANDROID

    if(m_interface)
    {
        m_interface->disconnect();
        m_interface->deleteLater();
        m_interface = nullptr;
    }

    m_interface = new QDBusInterface ("org.mauiman.Manager",
                                      "/Screen",
                                      "org.mauiman.Screen",
                                      QDBusConnection::sessionBus(), this);
    if (m_interface->isValid())
    {
        connect(m_interface, SIGNAL(scaleFactorCChanged(double)), this, SLOT(onScaleFactorChanged(double)));

    }
#endif
}

void ScreenManager::loadSettings()
{
    m_settings->beginModule("Screen");

#if !defined Q_OS_ANDROID

    if(m_interface && m_interface->isValid())
    {
        m_scaleFactor = m_interface->property("scaleFactor").toDouble();
        return;
    }
#endif

    m_scaleFactor = m_settings->load("ScaleFactor", m_scaleFactor).toDouble();
}
