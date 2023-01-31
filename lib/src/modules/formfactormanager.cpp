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
        connect(m_interface, SIGNAL(hasKeyboardChanged(bool)), this, SLOT(setHasKeyboard(bool)));
        connect(m_interface, SIGNAL(hasMouseChanged(bool)), this, SLOT(setHasMouse(bool)));
        connect(m_interface, SIGNAL(hasTouchscreenChanged(bool)), this, SLOT(setHasTouchscreen(bool)));
        connect(m_interface, SIGNAL(hasTouchpadChanged(bool)), this, SLOT(setHasTouchpad(bool)));
    }
}

void FormFactorManager::loadSettings()
{
    m_settings->beginModule("FormFactor");

#if !defined Q_OS_ANDROID
    if(m_interface && m_interface->isValid())
    {
        m_preferredMode = m_interface->property("preferredMode").toUInt();
        m_defaultMode = m_interface->property("defaultMode").toUInt();
        m_bestMode = m_interface->property("bestMode").toUInt();

        m_hasKeyboard = m_interface->property("hasKeyboard").toBool();
        m_hasMouse = m_interface->property("hasMouse").toBool();
        m_hasTouchpad = m_interface->property("hasTouchpad").toBool();
        m_hasTouchscreen = m_interface->property("hasTouchscreen").toBool();

        return;
    }
#endif

    m_preferredMode = m_settings->load("PreferredMode", m_preferredMode).toUInt();
    m_defaultMode = m_settings->load("DefaultMode", m_defaultMode).toUInt();
    m_bestMode = m_settings->load("BestMode", m_bestMode).toUInt();

    m_hasKeyboard = m_settings->load("HasKeyboard", m_hasKeyboard).toBool();
    m_hasMouse =  m_settings->load("HasMouse", m_hasMouse).toBool();
    m_hasTouchpad =  m_settings->load("HasTouchpad", m_hasTouchpad).toBool();
    m_hasTouchscreen = m_settings->load("HasTouchscreen", m_hasTouchscreen).toBool();

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

bool FormFactorManager::hasTouchpad() const
{
    return m_hasTouchpad;
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

void FormFactorManager::setHasTouchpad(bool hasTouchpad)
{
    if (m_hasTouchpad == hasTouchpad)
        return;

    m_hasTouchpad = hasTouchpad;
    emit hasTouchpadChanged(m_hasTouchpad);
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

void FormFactorManager::setBestMode(uint mode)
{
    if(m_bestMode == mode)
        return;

    m_bestMode = mode;
    Q_EMIT bestModeChanged(m_bestMode);
}

void FormFactorManager::setDefaultMode(uint mode)
{
    if(m_defaultMode == mode)
        return;

    m_defaultMode = mode;
    Q_EMIT defaultModeChanged(m_defaultMode);
}

