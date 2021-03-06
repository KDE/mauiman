#include "theme.h"
#include "themeadaptor.h"
#include <QDBusInterface>

#include "code/settingsstore.h"

#include <QDebug>

Theme::Theme(QObject *parent) : QObject(parent)
{
    qDebug( " INIT THEME SERVER");
    new ThemeAdaptor(this);
    if(!QDBusConnection::sessionBus().registerObject(QStringLiteral("/Theme"), this))
    {
        qDebug() << "FAILED TO REGISTER THEME DBUS OBJECT";
        return;
    }

    bool ok = false;
    MauiMan::SettingsStore settings;
    settings.beginModule("Theme");
    m_accentColor = settings.load("AccentColor", m_accentColor).toString();
    m_iconTheme = settings.load("IconTheme", m_iconTheme).toString();
    m_windowControlsTheme = settings.load("WindowControlsTheme", m_windowControlsTheme).toString();
    m_styleType = settings.load("StyleType", m_styleType).toInt(&ok);
    m_enableCSD = settings.load("EnableCSD", m_enableCSD).toBool();
    m_borderRadius = settings.load("BorderRadius", m_borderRadius).toUInt();
    m_iconSize = settings.load("IconSize", m_iconSize).toUInt(&ok);
    settings.endModule();
}

int Theme::styleType() const
{
    return m_styleType;
}

void Theme::setStyleType(int newStyleType)
{
    if (m_styleType == newStyleType)
        return;
    m_styleType = newStyleType;
    Q_EMIT styleTypeChanged(m_styleType);
}

const QString &Theme::accentColor() const
{
    return m_accentColor;
}

void Theme::setAccentColor(const QString &newAccentColor)
{
    if (m_accentColor == newAccentColor)
        return;
    m_accentColor = newAccentColor;
    Q_EMIT accentColorChanged(m_accentColor);
}

const QString &Theme::iconTheme() const
{
    return m_iconTheme;
}

void Theme::setIconTheme(const QString &newIconTheme)
{
    if (m_iconTheme == newIconTheme)
        return;
    m_iconTheme = newIconTheme;
    Q_EMIT iconThemeChanged(m_iconTheme);
}

const QString &Theme::windowControlsTheme() const
{
    return m_windowControlsTheme;
}

void Theme::setWindowControlsTheme(const QString &newWindowControlsTheme)
{
    if (m_windowControlsTheme == newWindowControlsTheme)
        return;
    m_windowControlsTheme = newWindowControlsTheme;
    Q_EMIT windowControlsThemeChanged(m_windowControlsTheme);
}

bool Theme::enableCSD() const
{
    return m_enableCSD;
}

void Theme::setEnableCSD(bool enableCSD)
{
    if (m_enableCSD == enableCSD)
        return;

    m_enableCSD = enableCSD;
    Q_EMIT enableCSDChanged(m_enableCSD);
}

uint Theme::borderRadius() const
{
    return m_borderRadius;
}

void Theme::setBorderRadius(uint newBorderRadius)
{
    if (m_borderRadius == newBorderRadius)
        return;
    m_borderRadius = newBorderRadius;
    Q_EMIT borderRadiusChanged(m_borderRadius);
}

uint Theme::iconSize() const
{
    return m_iconSize;
}

void Theme::setIconSize(uint newIconSize)
{
    if (m_iconSize == newIconSize)
        return;
    m_iconSize = newIconSize;
    Q_EMIT iconSizeChanged(m_iconSize);
}
