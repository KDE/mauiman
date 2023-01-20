#include "formfactormanager.h"

#include "settingsstore.h"
#include "mauimanutils.h"

#include <QDebug>

#if !defined Q_OS_ANDROID
#include <QDBusInterface>
#endif

using namespace MauiMan;

void FormFactorManager::sync(const QString &key, const QVariant &value)
{
    if (m_interface && m_interface->isValid())
    {
        m_interface->call(key, value);
    }
}

void FormFactorManager::setConnections()
{
    if(m_interface)
    {
        m_interface->disconnect();
        m_interface->deleteLater();
        m_interface = nullptr;
    }

    m_interface = new QDBusInterface("org.mauiman.Manager",
                                       "/FormFactor",
                                     "org.mauiman.FormFactor",
                                       QDBusConnection::sessionBus(), this);

    if (m_interface->isValid())
    {
        connect(m_interface, SIGNAL(preferredModeChanged(uint)), this, SLOT(onPreferredModeChanged(uint)));
//        connect(m_interface, SIGNAL(iconThemeChanged(QString)), this, SLOT(onIconThemeChanged(QString)));
//        connect(m_interface, SIGNAL(windowControlsThemeChanged(QString)), this, SLOT(onWindowControlsThemeChanged(QString)));
//        connect(m_interface, SIGNAL(styleTypeChanged(int)), this, SLOT(onStyleTypeChanged(int)));
//        connect(m_interface, SIGNAL(enableCSDChanged(bool)), this, SLOT(onEnableCSDChanged(bool)));
//        connect(m_interface, SIGNAL(borderRadiusChanged(uint)), this, SLOT(onBorderRadiusChanged(uint)));
//        connect(m_interface, SIGNAL(iconSizeChanged(uint)), this, SLOT(onIconSizeChanged(uint)));
//        connect(m_interface, SIGNAL(enableEffectsChanged(bool)), this, SLOT(onEnableEffectsChanged(bool)));
    }
}

void FormFactorManager::loadSettings()
{
    m_settings->beginModule("FormFactor");

#if !defined Q_OS_ANDROID

    if(m_interface && m_interface->isValid())
    {
        m_preferredMode = m_interface->property("preferredMode").toUInt();
        return;
    }
#endif

    m_preferredMode = m_settings->load("PreferredMode", m_preferredMode).toUInt();
}

FormFactorManager::FormFactorManager(QObject *parent) : QObject(parent)
  ,m_settings(new MauiMan::SettingsStore(this))

{
    qDebug( " INIT FORMFACTOR MANAGER");

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

uint FormFactorManager::preferredMode() const
{
    return m_preferredMode;
}

uint FormFactorManager::bestMode() const
{
    return m_bestMode;
}

uint FormFactorManager::defaultMode() const
{
    return m_defaultMode;
}

bool FormFactorManager::hasKeyboard() const
{
    return m_hasKeyboard;
}

bool FormFactorManager::hasTouchscreen() const
{
    return m_hasTouchscreen;
}

bool FormFactorManager::hasMouse() const
{
    return m_hasMouse;
}

void FormFactorManager::setPreferredMode(uint preferredMode)
{
    if (m_preferredMode == preferredMode)
        return;

    m_preferredMode = preferredMode;

    sync("setPreferredMode", m_preferredMode);
    m_settings->save("PreferredMode", m_preferredMode);

    Q_EMIT preferredModeChanged(m_preferredMode);
}

void FormFactorManager::setHasKeyboard(bool hasKeyboard)
{
    if (m_hasKeyboard == hasKeyboard)
        return;

    m_hasKeyboard = hasKeyboard;
    Q_EMIT hasKeyboardChanged(m_hasKeyboard);
}

void FormFactorManager::setHasTouchscreen(bool hasTouchscreen)
{
    if (m_hasTouchscreen == hasTouchscreen)
        return;

    m_hasTouchscreen = hasTouchscreen;
    Q_EMIT hasTouchscreenChanged(m_hasTouchscreen);
}

void FormFactorManager::setHasMouse(bool hasMouse)
{
    if (m_hasMouse == hasMouse)
        return;

    m_hasMouse = hasMouse;
    Q_EMIT hasMouseChanged(m_hasMouse);
}

void FormFactorManager::onPreferredModeChanged(uint preferredMode)
{
    if (m_preferredMode == preferredMode)
        return;

    m_preferredMode = preferredMode;
    Q_EMIT preferredModeChanged(m_preferredMode);
}
