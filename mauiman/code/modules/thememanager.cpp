#include "thememanager.h"

#include "code/settingsstore.h"
#include "code/mauimanutils.h"

#include <QDebug>

using namespace MauiMan;
ThemeManager::ThemeManager(QObject *parent) : QObject(parent)
  ,m_settings(new MauiMan::SettingsStore(this))
{
    qDebug( " INIT THEME MANAGER");
    m_settings->beginModule("Theme");

    auto server = new MauiManUtils(this);
    if(server->serverRunning())
    {
        this->setConnections();
    }

    loadSettings();

    connect(server, &MauiManUtils::serverRunningChanged, [this](bool state)
    {
        qDebug() << "THEMEMANAGER MauiMan server running? " << state;

        if(state)
        {
            this->setConnections();
            qDebug() <<"THEMEMANAGER MauiMan server running? " << state << m_interface->isValid();

        }
    });
}


void ThemeManager::sync(const QString &key, const QVariant &value)
{
    if (m_interface && m_interface->isValid())
    {
        m_interface->call(key, value);
    }
}

void ThemeManager::setConnections()
{
    if(m_interface)
    {
        m_interface->disconnect();
        m_interface->deleteLater();
        m_interface = nullptr;
    }

    m_interface = new QDBusInterface  ("org.mauiman.Manager",
                                       "/Theme",
                                       "org.mauiman.Theme",
                                       QDBusConnection::sessionBus(), this);

    if (m_interface->isValid())
    {
        connect(m_interface, SIGNAL(accentColorChanged(QString)), this, SLOT(onAccentColorChanged(QString)));
        connect(m_interface, SIGNAL(iconThemeChanged(QString)), this, SLOT(onIconThemeChanged(QString)));
        connect(m_interface, SIGNAL(windowControlsThemeChanged(QString)), this, SLOT(onWindowControlsThemeChanged(QString)));
        connect(m_interface, SIGNAL(styleTypeChanged(int)), this, SLOT(onStyleTypeChanged(int)));
        connect(m_interface, SIGNAL(enableCSDChanged(bool)), this, SLOT(onEnableCSDChanged(bool)));
    }
}

void ThemeManager::loadSettings()
{
    if(m_interface && m_interface->isValid())
    {
        m_accentColor = m_interface->property("accentColor").toString();
        m_styleType = m_interface->property("styleType").toInt();
        m_iconTheme = m_interface->property("iconTheme").toString();
        m_windowControlsTheme = m_interface->property("windowControlsTheme").toString();
        m_enableCSD = m_interface->property("enableCSD").toBool();
        return;
    }

    m_accentColor = m_settings->load("AccentColor", m_accentColor).toString();
    m_styleType = m_settings->load("StyleType", m_styleType).toInt();
    m_iconTheme = m_settings->load("IconTheme", m_iconTheme).toString();
    m_windowControlsTheme = m_settings->load("WindowControlsTheme", m_windowControlsTheme).toString();
    m_enableCSD = m_settings->load("EnableCSD", m_enableCSD).toBool();
}

int ThemeManager::styleType() const
{
    return m_styleType;
}

void ThemeManager::setStyleType(int newStyleType)
{
    if (m_styleType == newStyleType)
        return;

    m_styleType = newStyleType;
    m_settings->save("StyleType", m_styleType);
    sync("setStyleType", newStyleType);
    emit styleTypeChanged(m_styleType);

}

const QString &ThemeManager::accentColor() const
{
    return m_accentColor;
}

void ThemeManager::setAccentColor(const QString &newAccentColor)
{
    if (m_accentColor == newAccentColor)
        return;

    qDebug() << "Setting accent color" << m_accentColor;

    m_accentColor = newAccentColor;
    m_settings->save("AccentColor", m_accentColor);
    sync("setAccentColor", m_accentColor);
    emit accentColorChanged(m_accentColor);
}

const QString &ThemeManager::iconTheme() const
{
    return m_iconTheme;
}

void ThemeManager::setIconTheme(const QString &newIconTheme)
{
    if (m_iconTheme == newIconTheme)
        return;

    m_iconTheme = newIconTheme;
    m_settings->save("IconTheme", m_iconTheme);
    sync("setIconTheme", m_iconTheme);
    emit iconThemeChanged(m_iconTheme);
}

const QString &ThemeManager::windowControlsTheme() const
{
    return m_windowControlsTheme;
}

void ThemeManager::setWindowControlsTheme(const QString &newWindowControlsTheme)
{
    if (m_windowControlsTheme == newWindowControlsTheme)
        return;

    m_windowControlsTheme = newWindowControlsTheme;
    m_settings->save("WindowControlsTheme", m_windowControlsTheme);
    sync("setWindowControlsTheme", m_windowControlsTheme);
    emit windowControlsThemeChanged(m_windowControlsTheme);
}

bool ThemeManager::enableCSD() const
{
    return m_enableCSD;
}

void ThemeManager::setEnableCSD(bool enableCSD)
{
    if (m_enableCSD == enableCSD)
        return;

    m_enableCSD = enableCSD;
    m_settings->save("EnableCSD", m_enableCSD);
    sync("setEnableCSD", m_enableCSD);
    emit enableCSDChanged(m_enableCSD);
}

void ThemeManager::onStyleTypeChanged(const int &newStyleType)
{
    if (m_styleType == newStyleType)
        return;

    m_styleType = newStyleType;
    emit styleTypeChanged(m_styleType);
}

void ThemeManager::onAccentColorChanged(const QString &newAccentColor)
{
    if (m_accentColor == newAccentColor)
        return;

    m_accentColor = newAccentColor;
    emit accentColorChanged(m_accentColor);
}

void ThemeManager::onWindowControlsThemeChanged(const QString &newWindowControlsTheme)
{
    if (m_windowControlsTheme == newWindowControlsTheme)
        return;

    m_windowControlsTheme = newWindowControlsTheme;
    emit windowControlsThemeChanged(m_windowControlsTheme);
}

void ThemeManager::onIconThemeChanged(const QString &newIconTheme)
{
    if (m_iconTheme == newIconTheme)
        return;

    m_iconTheme = newIconTheme;
    emit iconThemeChanged(m_iconTheme);
}

void ThemeManager::onEnableCSDChanged(const bool &enableCSD)
{
    if (m_enableCSD == enableCSD)
        return;

    m_enableCSD = enableCSD;
    emit enableCSDChanged(m_enableCSD);
}
