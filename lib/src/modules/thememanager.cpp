#include "thememanager.h"

#include "settingsstore.h"
#include "mauimanutils.h"

#include <QDebug>

using namespace MauiMan;


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

    m_interface = new QDBusInterface  (QStringLiteral("org.mauiman.Manager"),
                                       QStringLiteral("/Theme"),
                                       QStringLiteral("org.mauiman.Theme"),
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
        connect(m_interface, SIGNAL(spacingSizeChanged(uint)), this, SLOT(onSpacingSizeChanged(uint)));
        connect(m_interface, SIGNAL(enableEffectsChanged(bool)), this, SLOT(onEnableEffectsChanged(bool)));
        connect(m_interface, SIGNAL(defaultFontChanged(QString)), this, SLOT(onDefaultFontChanged(QString)));
        connect(m_interface, SIGNAL(smallFontChanged(QString)), this, SLOT(onSmallFontChanged(QString)));
        connect(m_interface, SIGNAL(monospacedFontChanged(QString)), this, SLOT(onMonospacedFontChanged(QString)));
        connect(m_interface, SIGNAL(customColorSchemeChanged(QString)), this, SLOT(onCustomColorSchemeChanged(QString)));
    }
#endif
}

void ThemeManager::loadSettings()
{
    m_settings->beginModule(QStringLiteral("Theme"));

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
        m_spacingSize = m_interface->property("spacingSize").toUInt();
        m_enableEffects = m_interface->property("enableEffects").toBool();
        m_defaultFont = m_interface->property("defaultFont").toString();
        m_smallFont = m_interface->property("smallFont").toString();
        m_monospacedFont = m_interface->property("monospacedFont").toString();
        m_customColorScheme = m_interface->property("customColorScheme").toString();

        return;
    }
#endif

    m_accentColor = m_settings->load(QStringLiteral("AccentColor"), m_accentColor).toString();
    m_styleType = m_settings->load(QStringLiteral("StyleType"), m_styleType).toInt();
    m_iconTheme = m_settings->load(QStringLiteral("IconTheme"), m_iconTheme).toString();
    m_windowControlsTheme = m_settings->load(QStringLiteral("WindowControlsTheme"), m_windowControlsTheme).toString();
    m_enableCSD = m_settings->load(QStringLiteral("EnableCSD"), m_enableCSD).toBool();
    m_borderRadius = m_settings->load(QStringLiteral("BorderRadius"), m_borderRadius).toUInt();
    m_iconSize = m_settings->load(QStringLiteral("IconSize"), m_iconSize).toUInt();
    m_paddingSize = m_settings->load(QStringLiteral("PaddingSize"), m_paddingSize).toUInt();
    m_marginSize = m_settings->load(QStringLiteral("MarginSize"), m_marginSize).toUInt();
    m_spacingSize = m_settings->load(QStringLiteral("SpacingSize"), m_spacingSize).toUInt();
    m_enableEffects = m_settings->load(QStringLiteral("EnableEffects"), m_enableEffects).toBool();
    m_defaultFont = m_settings->load(QStringLiteral("DefaultFont"), m_defaultFont).toString();
    m_smallFont = m_settings->load(QStringLiteral("SmallFont"), m_smallFont).toString();
    m_monospacedFont = m_settings->load(QStringLiteral("MonospacedFont"), m_monospacedFont).toString();
    m_customColorScheme = m_settings->load(QStringLiteral("CustomColorScheme"), m_customColorScheme).toString();
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
    m_settings->save(QStringLiteral("StyleType"), m_styleType);
    sync(QStringLiteral("setStyleType"), newStyleType);
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
    m_settings->save(QStringLiteral("AccentColor"), m_accentColor);
    sync(QStringLiteral("setAccentColor"), m_accentColor);
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
    m_settings->save(QStringLiteral("IconTheme"), m_iconTheme);
    sync(QStringLiteral("setIconTheme"), m_iconTheme);
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
    m_settings->save(QStringLiteral("WindowControlsTheme"), m_windowControlsTheme);
    sync(QStringLiteral("setWindowControlsTheme"), m_windowControlsTheme);
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
    m_settings->save(QStringLiteral("EnableCSD"), m_enableCSD);
    sync(QStringLiteral("setEnableCSD"), m_enableCSD);
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

void ThemeManager::onSpacingSizeChanged(const uint &spacingSize)
{
    if (m_spacingSize == spacingSize)
        return;

    m_spacingSize = spacingSize;
    Q_EMIT spacingSizeChanged(m_spacingSize);
}

void ThemeManager::onEnableEffectsChanged(bool enableEffects)
{
    qDebug() << "ENABLE EFEFCTS MODIFIED" << enableEffects;
    if (m_enableEffects == enableEffects)
        return;

    m_enableEffects = enableEffects;
    Q_EMIT enableEffectsChanged(m_enableEffects);
}

void ThemeManager::onDefaultFontChanged(const QString &font)
{
    if (m_defaultFont == font)
        return;

    m_defaultFont = font;
    Q_EMIT defaultFontChanged(m_defaultFont);
}

void ThemeManager::onSmallFontChanged(const QString &font)
{
    if (m_smallFont == font)
        return;

    m_smallFont = font;
    Q_EMIT smallFontChanged(m_smallFont);
}

void ThemeManager::onMonospacedFontChanged(const QString &font)
{
    if (m_monospacedFont == font)
        return;

    m_monospacedFont = font;
    Q_EMIT monospacedFontChanged(m_monospacedFont);
}

void ThemeManager::onCustomColorSchemeChanged(const QString &scheme)
{
    if (m_customColorScheme == scheme)
        return;

    m_customColorScheme = scheme;
    Q_EMIT customColorSchemeChanged(m_customColorScheme);
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
    m_settings->save(QStringLiteral("BorderRadius"), m_borderRadius);
    sync(QStringLiteral("setBorderRadius"), m_borderRadius);
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
    m_settings->save(QStringLiteral("IconSize"), m_iconSize);
    sync(QStringLiteral("setIconSize"), m_iconSize);
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
    m_settings->save(QStringLiteral("EnableEffects"), m_enableEffects);
    sync(QStringLiteral("setEnableEffects"), m_enableEffects);
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
    m_settings->save(QStringLiteral("PaddingSize"), m_paddingSize);
    sync(QStringLiteral("setPaddingSize"), m_paddingSize);
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
    m_settings->save(QStringLiteral("MarginSize"), m_marginSize);
    sync(QStringLiteral("setMarginSize"), m_marginSize);
    Q_EMIT marginSizeChanged(m_marginSize);
}

void ThemeManager::resetMarginSize()
{
    this->setMarginSize(ThemeManager::DefaultValues::marginSize);
}

uint ThemeManager::spacingSize() const
{
    return m_spacingSize;
}

void ThemeManager::setSpacingSize(uint spacingSize)
{
    if (m_spacingSize == spacingSize)
        return;

    m_spacingSize = spacingSize;
    m_settings->save(QStringLiteral("SpacingSize"), m_spacingSize);
    sync(QStringLiteral("setSpacingSize"), m_spacingSize);
    Q_EMIT spacingSizeChanged(m_spacingSize);
}

void ThemeManager::resetSPacingSize()
{
    this->setSpacingSize(ThemeManager::DefaultValues::spacingSize);
}

QString ThemeManager::defaultFont() const
{
    return m_defaultFont;
}

QString ThemeManager::smallFont() const
{
    return m_smallFont;
}

QString ThemeManager::monospacedFont() const
{
    return m_monospacedFont;
}

void ThemeManager::setDefaultFont(const QString &defaultFont)
{
    if (m_defaultFont == defaultFont)
        return;

    m_defaultFont = defaultFont;
    m_settings->save(QStringLiteral("DefaultFont"), m_defaultFont);
    sync(QStringLiteral("setDefaultFont"), m_defaultFont);
    Q_EMIT defaultFontChanged(m_defaultFont);
}

void ThemeManager::resetDefaultFont()
{
    setDefaultFont(ThemeManager::DefaultValues::defaultFont);
}

void ThemeManager::setSmallFont(const QString &smallFont)
{
    if (m_smallFont == smallFont)
        return;

    m_smallFont = smallFont;
    m_settings->save(QStringLiteral("SmallFont"), m_smallFont);
    sync(QStringLiteral("setSmallFont"), m_smallFont);
    Q_EMIT smallFontChanged(m_smallFont);
}

void ThemeManager::resetSmallFont()
{
    setSmallFont(ThemeManager::DefaultValues::smallFont);
}

void ThemeManager::setMonospacedFont(const QString &monospacedFont)
{
    if (m_monospacedFont == monospacedFont)
        return;

    m_monospacedFont = monospacedFont;
    m_settings->save(QStringLiteral("MonospacedFont"), m_monospacedFont);
    sync(QStringLiteral("setMonospacedFont"), m_monospacedFont);
    Q_EMIT monospacedFontChanged(m_monospacedFont);
}

void ThemeManager::resetMonospacedFont()
{
    setMonospacedFont(ThemeManager::DefaultValues::monospacedFont);
}

QString ThemeManager::customColorScheme() const
{
    return m_customColorScheme;
}

void ThemeManager::setCustomColorScheme(const QString &customColorScheme)
{
    if (m_customColorScheme == customColorScheme)
        return;

    m_customColorScheme = customColorScheme;
    m_settings->save(QStringLiteral("CustomColorScheme"), m_customColorScheme);
    sync(QStringLiteral("setCustomColorScheme"), m_customColorScheme);
    Q_EMIT customColorSchemeChanged(m_customColorScheme);
}

void MauiMan::ThemeManager::resetIconSize()
{
    this->setIconSize(ThemeManager::DefaultValues::iconSize);
}
