#include "theme.h"
#include "themeadaptor.h"
#include <QDBusInterface>

#include "settingsstore.h"

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

    settings.beginModule(QStringLiteral("Theme"));

    m_accentColor = settings.load(QStringLiteral("AccentColor"), m_accentColor).toString();
    m_iconTheme = settings.load(QStringLiteral("IconTheme"), m_iconTheme).toString();
    m_windowControlsTheme = settings.load(QStringLiteral("WindowControlsTheme"), m_windowControlsTheme).toString();
    m_styleType = settings.load(QStringLiteral("StyleType"), m_styleType).toInt(&ok);
    m_enableCSD = settings.load(QStringLiteral("EnableCSD"), m_enableCSD).toBool();
    m_borderRadius = settings.load(QStringLiteral("BorderRadius"), m_borderRadius).toUInt();
    m_iconSize = settings.load(QStringLiteral("IconSize"), m_iconSize).toUInt(&ok);
    m_paddingSize = settings.load(QStringLiteral("PaddingSize"), m_paddingSize).toUInt(&ok);
    m_marginSize = settings.load(QStringLiteral("MarginSize"), m_marginSize).toUInt(&ok);
    m_spacingSize = settings.load(QStringLiteral("SpacingSize"), m_spacingSize).toUInt(&ok);
    m_enableEffects = settings.load(QStringLiteral("EnableEffects"), m_enableEffects).toBool();
    m_defaultFont = settings.load(QStringLiteral("DefaultFont"), m_defaultFont).toString();
    m_smallFont = settings.load(QStringLiteral("SmallFont"), m_smallFont).toString();
    m_monospacedFont = settings.load(QStringLiteral("MonospacedFont"), m_monospacedFont).toString();
    m_customColorScheme = settings.load(QStringLiteral("CustomColorScheme"), m_customColorScheme).toString();

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

bool Theme::enableEffects() const
{
    return m_enableEffects;
}

void Theme::setEnableEffects(bool enableEffects)
{
    if (m_enableEffects == enableEffects)
        return;

    m_enableEffects = enableEffects;
    Q_EMIT enableEffectsChanged(m_enableEffects);
}

uint Theme::paddingSize() const
{
    return m_paddingSize;
}

uint Theme::marginSize() const
{
    return m_marginSize;
}

uint Theme::spacingSize() const
{
    return m_spacingSize;
}

void Theme::setSpacingSize(uint spacingSize)
{
    if (m_spacingSize == spacingSize)
        return;

    m_spacingSize = spacingSize;
    Q_EMIT spacingSizeChanged(m_spacingSize);
}

QString Theme::defaultFont() const
{
    return m_defaultFont;
}

QString Theme::smallFont() const
{
    return m_smallFont;
}

QString Theme::monospacedFont() const
{
    return m_monospacedFont;
}

void Theme::setDefaultFont(const QString &defaultFont)
{
    if (m_defaultFont == defaultFont)
        return;

    m_defaultFont = defaultFont;
    emit defaultFontChanged(m_defaultFont);
}

void Theme::setSmallFont(const QString &smallFont)
{
    if (m_smallFont == smallFont)
        return;

    m_smallFont = smallFont;
    Q_EMIT smallFontChanged(m_smallFont);
}

void Theme::setMonospacedFont(const QString &monospacedFont)
{
    if (m_monospacedFont == monospacedFont)
        return;

    m_monospacedFont = monospacedFont;
    Q_EMIT monospacedFontChanged(m_monospacedFont);
}

QString Theme::customColorScheme() const
{
    return m_customColorScheme;
}

void Theme::setCustomColorScheme(const QString &customColorScheme)
{
    if (m_customColorScheme == customColorScheme)
        return;

    m_customColorScheme = customColorScheme;
    Q_EMIT customColorSchemeChanged(m_customColorScheme);
}

void Theme::setPaddingSize(uint paddingSize)
{
    if (m_paddingSize == paddingSize)
        return;

    m_paddingSize = paddingSize;
    Q_EMIT paddingSizeChanged(m_paddingSize);
}

void Theme::setMarginSize(uint marginSize)
{
    if (m_marginSize == marginSize)
        return;

    m_marginSize = marginSize;
    Q_EMIT marginSizeChanged(m_marginSize);
}
