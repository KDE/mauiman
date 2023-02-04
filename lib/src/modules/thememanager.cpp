#include "thememanager.h"

#include "settingsstore.h"
#include "mauimanutils.h"

#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QSettings>

using namespace MauiMan;

static QString gtkRc2Path()
{
    return QDir::homePath() + QLatin1String("/.gtkrc-2.0");
}

static QString gtk3SettingsIniPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + QLatin1String("/gtk-3.0/settings.ini");
}

void ThemeManager::updateGtk3Config()
{
    qDebug() << "Request to update GTK3 configs";

    QSettings settings(gtk3SettingsIniPath(), QSettings::IniFormat);
    settings.clear();
    settings.setIniCodec("UTF-8");
    settings.beginGroup("Settings");

    // font
//    settings.setValue("gtk-font-name", QString("%1 %2").arg(systemFont()).arg(systemFontPointSize()));
    // dark mode
    settings.setValue("gtk-application-prefer-dark-theme", m_styleType == 1);
    // icon theme
    settings.setValue("gtk-icon-theme-name", m_iconTheme);
    // other
    settings.setValue("gtk-enable-animations", true);
    // theme
//    settings.setValue("gtk-theme-name", isDarkMode() ? "Cutefish-dark" : "Cutefish-light");
    settings.sync();
}


ThemeManager::ThemeManager(QObject *parent) : QObject(parent)
  ,m_settings(new MauiMan::SettingsStore(this))
{
    qDebug( " INIT THEME MANAGER");

#if !defined Q_OS_ANDROID
    auto server = new MauiManUtils(this);
    if(server->serverRunning())
    {
        this->setConnections();
    }

    connect(server, &MauiManUtils::serverRunningChanged, [this](bool state)
    {
        qDebug() << "THEMEMANAGER MauiMan server running? " << state;

        if(state)
        {
            this->setConnections();
            qDebug() <<"THEMEMANAGER MauiMan server running? " << state << m_interface->isValid();

        }
    });
#endif

    loadSettings();

    connect(this, &ThemeManager::iconThemeChanged, [this](QString)
    {
        qDebug() << "Request to update IconTheme";

       updateGtk3Config();
    });
}


void ThemeManager::sync(const QString &key, const QVariant &value)
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

void ThemeManager::setConnections()
{
#if !defined Q_OS_ANDROID
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
        connect(m_interface, SIGNAL(borderRadiusChanged(uint)), this, SLOT(onBorderRadiusChanged(uint)));
        connect(m_interface, SIGNAL(iconSizeChanged(uint)), this, SLOT(onIconSizeChanged(uint)));
        connect(m_interface, SIGNAL(paddingSizeChanged(uint)), this, SLOT(onPaddingSizeChanged(uint)));
        connect(m_interface, SIGNAL(marginSizeChanged(uint)), this, SLOT(onMarginSizeChanged(uint)));
        connect(m_interface, SIGNAL(enableEffectsChanged(bool)), this, SLOT(onEnableEffectsChanged(bool)));
    }
#endif
}

void ThemeManager::loadSettings()
{
    m_settings->beginModule("Theme");

#if !defined Q_OS_ANDROID
    if(m_interface && m_interface->isValid())
    {
        m_accentColor = m_interface->property("accentColor").toString();
        m_styleType = m_interface->property("styleType").toInt();
        m_iconTheme = m_interface->property("iconTheme").toString();
        m_windowControlsTheme = m_interface->property("windowControlsTheme").toString();
        m_enableCSD = m_interface->property("enableCSD").toBool();
        m_borderRadius = m_interface->property("borderRadius").toUInt();
        m_iconSize = m_interface->property("iconSize").toUInt();
        m_paddingSize = m_interface->property("paddingSize").toUInt();
        m_marginSize = m_interface->property("marginSize").toUInt();
        m_enableEffects = m_interface->property("enableEffects").toBool();
        return;
    }
#endif

    m_accentColor = m_settings->load("AccentColor", m_accentColor).toString();
    m_styleType = m_settings->load("StyleType", m_styleType).toInt();
    m_iconTheme = m_settings->load("IconTheme", m_iconTheme).toString();
    m_windowControlsTheme = m_settings->load("WindowControlsTheme", m_windowControlsTheme).toString();
    m_enableCSD = m_settings->load("EnableCSD", m_enableCSD).toBool();
    m_borderRadius = m_settings->load("BorderRadius", m_borderRadius).toUInt();
    m_iconSize = m_settings->load("IconSize", m_iconSize).toUInt();
    m_paddingSize = m_settings->load("PaddingSize", m_paddingSize).toUInt();
    m_marginSize = m_settings->load("MarginSize", m_marginSize).toUInt();
    m_enableEffects = m_settings->load("EnableEffects", m_enableEffects).toBool();
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
    Q_EMIT styleTypeChanged(m_styleType);
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
    Q_EMIT accentColorChanged(m_accentColor);
}

void ThemeManager::resetAccentColor()
{
    this->setAccentColor(ThemeManager::DefaultValues::accentColor);
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
    Q_EMIT iconThemeChanged(m_iconTheme);
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
    Q_EMIT windowControlsThemeChanged(m_windowControlsTheme);
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
    Q_EMIT enableCSDChanged(m_enableCSD);
}

void ThemeManager::onStyleTypeChanged(const int &newStyleType)
{
    if (m_styleType == newStyleType)
        return;

    m_styleType = newStyleType;
    Q_EMIT styleTypeChanged(m_styleType);
}

void ThemeManager::onAccentColorChanged(const QString &newAccentColor)
{
    if (m_accentColor == newAccentColor)
        return;

    m_accentColor = newAccentColor;
    Q_EMIT accentColorChanged(m_accentColor);
}

void ThemeManager::onWindowControlsThemeChanged(const QString &newWindowControlsTheme)
{
    if (m_windowControlsTheme == newWindowControlsTheme)
        return;

    m_windowControlsTheme = newWindowControlsTheme;
    Q_EMIT windowControlsThemeChanged(m_windowControlsTheme);
}

void ThemeManager::onIconThemeChanged(const QString &newIconTheme)
{
    if (m_iconTheme == newIconTheme)
        return;

    m_iconTheme = newIconTheme;
    Q_EMIT iconThemeChanged(m_iconTheme);
}

void ThemeManager::onEnableCSDChanged(const bool &enableCSD)
{
    if (m_enableCSD == enableCSD)
        return;

    m_enableCSD = enableCSD;
    Q_EMIT enableCSDChanged(m_enableCSD);
}

void ThemeManager::onBorderRadiusChanged(const uint &radius)
{
    if (m_borderRadius == radius)
        return;
    m_borderRadius = radius;
    Q_EMIT borderRadiusChanged(m_borderRadius);
}

void ThemeManager::onIconSizeChanged(const uint &size)
{
    if (m_iconSize == size)
        return;
    m_iconSize = size;
    Q_EMIT iconSizeChanged(m_iconSize);
}

void ThemeManager::onPaddingSizeChanged(const uint &paddingSize)
{
    if (m_paddingSize == paddingSize)
        return;

    m_paddingSize = paddingSize;
    Q_EMIT paddingSizeChanged(m_paddingSize);
}

void ThemeManager::onMarginSizeChanged(const uint &marginSize)
{
    if (m_marginSize == marginSize)
        return;

    m_marginSize = marginSize;
    Q_EMIT marginSizeChanged(m_marginSize);
}

void ThemeManager::onEnableEffectsChanged(bool enableEffects)
{
    qDebug() << "ENABLE EFEFCTS MODIFIED" << enableEffects;
    if (m_enableEffects == enableEffects)
        return;

    m_enableEffects = enableEffects;
    Q_EMIT enableEffectsChanged(m_enableEffects);
}

uint ThemeManager::borderRadius() const
{
    return m_borderRadius;
}

void ThemeManager::setBorderRadius(uint newBorderRadius)
{
    if (m_borderRadius == newBorderRadius)
        return;
    m_borderRadius = newBorderRadius;
    m_settings->save("BorderRadius", m_borderRadius);
    sync("setBorderRadius", m_borderRadius);
    Q_EMIT borderRadiusChanged(m_borderRadius);
}

void ThemeManager::resetBorderRadius()
{
    this->setBorderRadius(ThemeManager::DefaultValues::borderRadius);
}

uint ThemeManager::iconSize() const
{
    return m_iconSize;
}

void ThemeManager::setIconSize(uint newIconSize)
{
    if (m_iconSize == newIconSize)
        return;
    m_iconSize = newIconSize;
    m_settings->save("IconSize", m_iconSize);
    sync("setIconSize", m_iconSize);
    Q_EMIT iconSizeChanged(m_iconSize);
}

bool ThemeManager::enableEffects() const
{
    return m_enableEffects;
}

void ThemeManager::setEnableEffects(bool enableEffects)
{
    if (m_enableEffects == enableEffects)
        return;

    m_enableEffects = enableEffects;
    m_settings->save("EnableEffects", m_enableEffects);
    sync("setEnableEffects", m_enableEffects);
    Q_EMIT enableEffectsChanged(m_enableEffects);
}

uint ThemeManager::paddingSize() const
{
    return m_paddingSize;
}

uint ThemeManager::marginSize() const
{
    return m_marginSize;
}

void ThemeManager::setPaddingSize(uint paddingSize)
{
    if (m_paddingSize == paddingSize)
        return;

    m_paddingSize = paddingSize;
    m_settings->save("PaddingSize", m_paddingSize);
    sync("setPaddingSize", m_paddingSize);
    Q_EMIT paddingSizeChanged(m_paddingSize);
}

void ThemeManager::resetPaddingSize()
{
    this->setPaddingSize(ThemeManager::DefaultValues::paddingSize);
}

void ThemeManager::setMarginSize(uint marginSize)
{
    if (m_marginSize == marginSize)
        return;

    m_marginSize = marginSize;
    m_settings->save("MarginSize", m_marginSize);
    sync("setMarginSize", m_marginSize);
    Q_EMIT marginSizeChanged(m_marginSize);
}

void ThemeManager::resetMarginSize()
{
    this->setMarginSize(ThemeManager::DefaultValues::marginSize);
}

void MauiMan::ThemeManager::resetIconSize()
{
    this->setIconSize(ThemeManager::DefaultValues::iconSize);
}
