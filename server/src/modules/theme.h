#pragma once

#include <QObject>
#include <QString>

#include "modules/thememanager.h"

class Theme : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int styleType READ styleType WRITE setStyleType NOTIFY styleTypeChanged)
    Q_PROPERTY(QString accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)
    Q_PROPERTY(QString iconTheme READ iconTheme WRITE setIconTheme NOTIFY iconThemeChanged)
    Q_PROPERTY(QString windowControlsTheme READ windowControlsTheme WRITE setWindowControlsTheme NOTIFY windowControlsThemeChanged)
    Q_PROPERTY(bool enableCSD READ enableCSD WRITE setEnableCSD NOTIFY enableCSDChanged)
    Q_PROPERTY(uint borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(uint iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
    Q_PROPERTY(uint paddingSize READ paddingSize WRITE setPaddingSize NOTIFY paddingSizeChanged)
    Q_PROPERTY(uint marginSize READ marginSize WRITE setMarginSize NOTIFY marginSizeChanged)
    Q_PROPERTY(uint spacingSize READ spacingSize WRITE setSpacingSize NOTIFY spacingSizeChanged)
    Q_PROPERTY(bool enableEffects READ enableEffects WRITE setEnableEffects NOTIFY enableEffectsChanged)
    Q_PROPERTY(QString defaultFont READ defaultFont WRITE setDefaultFont NOTIFY defaultFontChanged)
    Q_PROPERTY(QString smallFont READ smallFont WRITE setSmallFont NOTIFY smallFontChanged)
    Q_PROPERTY(QString monospacedFont READ monospacedFont WRITE setMonospacedFont NOTIFY monospacedFontChanged)
    Q_PROPERTY(QString customColorScheme READ customColorScheme WRITE setCustomColorScheme NOTIFY customColorSchemeChanged)
    Q_PROPERTY(bool allowCustomStyling READ allowCustomStyling WRITE setAllowCustomStyling NOTIFY allowCustomStylingChanged)

public:
    explicit Theme(QObject * parent = nullptr);

    int styleType() const;
    void setStyleType(int newStyleType);

    const QString &accentColor() const;
    void setAccentColor(const QString &newAccentColor);

    const QString &iconTheme() const;
    void setIconTheme(const QString &newIconTheme);

    const QString &windowControlsTheme() const;
    void setWindowControlsTheme(const QString &newWindowControlsTheme);

    bool enableCSD() const;
    void setEnableCSD(bool enableCSD);

    uint borderRadius() const;
    void setBorderRadius(uint newBorderRadius);

    uint iconSize() const;
    void setIconSize(uint newIconSize);

    bool enableEffects() const;
    void setEnableEffects(bool enableEffects);

    void setPaddingSize(uint paddingSize);
    uint paddingSize() const;

    void setMarginSize(uint marginSize);
    uint marginSize() const;

    uint spacingSize() const;
    void setSpacingSize(uint spacingSize);

    QString defaultFont() const;
    void setDefaultFont(const QString &defaultFont);

    QString smallFont() const;
    void setSmallFont(const QString &smallFont);

    QString monospacedFont() const;
    void setMonospacedFont(const QString &monospacedFont);

    QString customColorScheme() const;
    void setCustomColorScheme(const QString &customColorScheme);

    bool allowCustomStyling() const;
    void setAllowCustomStyling(bool newAllowCustomStyling);

Q_SIGNALS:
    void styleTypeChanged(int styleStype);
    void accentColorChanged(QString accentColor);

    void iconThemeChanged(QString iconTheme);

    void windowControlsThemeChanged(QString windowControlsTheme);

    void enableCSDChanged(bool enableCSD);

    void borderRadiusChanged(uint radius);

    void iconSizeChanged(uint size);

    void enableEffectsChanged(bool enableEffects);

    void paddingSizeChanged(uint paddingSize);

    void marginSizeChanged(uint marginSize);

    void spacingSizeChanged(uint spacingSize);

    void defaultFontChanged(QString defaultFont);

    void smallFontChanged(QString smallFont);

    void monospacedFontChanged(QString monospacedFont);

    void customColorSchemeChanged(QString customColorScheme);

    void allowCustomStylingChanged(bool allowCustomStyling);

private:
    int m_styleType = MauiMan::ThemeManager::DefaultValues::styleType;
    QString m_accentColor = MauiMan::ThemeManager::DefaultValues::accentColor;
    QString m_iconTheme = MauiMan::ThemeManager::DefaultValues::iconTheme;
    QString m_windowControlsTheme = MauiMan::ThemeManager::DefaultValues::windowControlsTheme;
    bool m_enableCSD = MauiMan::ThemeManager::DefaultValues::enableCSD;
    uint m_borderRadius = MauiMan::ThemeManager::DefaultValues::borderRadius;
    uint m_iconSize = MauiMan::ThemeManager::DefaultValues::iconSize;
    bool m_enableEffects = MauiMan::ThemeManager::DefaultValues::enableEffects;
    uint m_paddingSize = MauiMan::ThemeManager::DefaultValues::paddingSize;
    uint m_marginSize = MauiMan::ThemeManager::DefaultValues::marginSize;
    uint m_spacingSize = MauiMan::ThemeManager::DefaultValues::spacingSize;
    QString m_defaultFont = MauiMan::ThemeManager::DefaultValues::defaultFont;
    QString m_smallFont = MauiMan::ThemeManager::DefaultValues::smallFont;
    QString m_monospacedFont = MauiMan::ThemeManager::DefaultValues::monospacedFont;
    QString m_customColorScheme = MauiMan::ThemeManager::DefaultValues::customColorScheme;
    bool m_allowCustomStyling = MauiMan::ThemeManager::DefaultValues::allowCustomStyling;
};
