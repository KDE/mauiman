#include "formfactor.h"
#include "formfactoradaptor.h"
#include <QDBusInterface>
#include <QMap>

#include "settingsstore.h"

FormFactor::FormFactor(QObject *parent) : QObject(parent)
,m_manager(new MauiMan::FormFactorInfo(this))
{
    qDebug( " INIT FORMFACTOR MODULE");
    new FormFactorAdaptor(this);
    if(!QDBusConnection::sessionBus().registerObject(QStringLiteral("/FormFactor"), this))
    {
        qDebug() << "FAILED TO REGISTER FORMFACTOR DBUS OBJECT";
        return;
    }

    //grab default values
    m_defaultMode = m_manager->defaultMode();
    m_bestMode = m_manager->bestMode();
    
    m_hasKeyboard = m_manager->hasKeyboard();
    m_hasMouse = m_manager->hasMouse();
    m_hasTouchpad = m_manager->hasTouchpad();
    m_hasTouchscreen = m_manager->hasTouchscreen();
    
    connect(m_manager, &MauiMan::FormFactorManager::bestModeChanged, this, &FormFactor::setBestMode);
    connect(m_manager, &MauiMan::FormFactorManager::hasKeyboardChanged, this, &FormFactor::setHasKeyboard);
    connect(m_manager, &MauiMan::FormFactorManager::hasMouseChanged, this, &FormFactor::setHasMouse);
    connect(m_manager, &MauiMan::FormFactorManager::hasTouchpadChanged, this, &FormFactor::setHasTouchpad);
    connect(m_manager, &MauiMan::FormFactorManager::hasTouchscreenChanged, this, &FormFactor::setHasTouchscreen);

    //grab user preferences
    MauiMan::SettingsStore settings;
    settings.beginModule("FormFactor");
    m_preferredMode = settings.load("PreferredMode", m_preferredMode).toUInt();
    settings.endModule();
}

uint FormFactor::preferredMode() const
{
    return m_preferredMode;
}

uint FormFactor::bestMode() const
{
    return m_bestMode;
}

uint FormFactor::defaultMode() const
{
    return m_defaultMode;
}

bool FormFactor::hasKeyboard() const
{
    return m_hasKeyboard;
}

bool FormFactor::hasTouchscreen() const
{
    return m_hasTouchscreen;
}

bool FormFactor::hasMouse() const
{
    return m_hasMouse;
}

bool FormFactor::hasTouchpad() const
{
    return m_hasTouchpad;
}

void FormFactor::setPreferredMode(uint preferredMode)
{
    if (m_preferredMode == preferredMode)
        return;

    m_preferredMode = preferredMode;
    Q_EMIT preferredModeChanged(m_preferredMode);
}

void FormFactor::setBestMode(uint bestMode)
{
    if (m_bestMode == bestMode)
        return;

    m_bestMode = bestMode;
    Q_EMIT bestModeChanged(m_bestMode);
}

void FormFactor::setHasKeyboard(bool value)
{
    if (m_hasKeyboard == value)
        return;

    m_hasKeyboard = value;
    Q_EMIT hasKeyboardChanged(m_hasKeyboard);
}

void FormFactor::setHasMouse(bool value)
{
    if (m_hasMouse == value)
        return;

    m_hasMouse = value;
    Q_EMIT hasMouseChanged(m_hasMouse);
}

void FormFactor::setHasTouchpad(bool value)
{
    if (m_hasTouchpad == value)
        return;

    m_hasTouchpad = value;
    Q_EMIT hasTouchpadChanged(m_hasTouchpad);
}

void FormFactor::setHasTouchscreen(bool value)
{
    if (m_hasTouchscreen == value)
        return;

    m_hasTouchscreen = value;
    Q_EMIT hasTouchscreenChanged(m_hasTouchscreen);
}


bool FormFactor::hasTouchScreen() const
{
    return m_hasTouchscreen;
}



