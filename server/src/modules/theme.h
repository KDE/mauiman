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
    Q_PROPERTY(bool enableEffects READ enableEffects WRITE setEnableEffects NOTIFY enableEffectsChanged)

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

signals:
    void styleTypeChanged(int styleStype);
    void accentColorChanged(QString accentColor);

    void iconThemeChanged(QString iconTheme);

    void windowControlsThemeChanged(QString windowControlsTheme);

    void enableCSDChanged(bool enableCSD);

    void borderRadiusChanged(uint radius);

    void iconSizeChanged(uint size);

    void enableEffectsChanged(bool enableEffects);

private:
    int m_styleType = MauiMan::ThemeManager::DefaultValues::styleType;
    QString m_accentColor = MauiMan::ThemeManager::DefaultValues::accentColor;
    QString m_iconTheme = MauiMan::ThemeManager::DefaultValues::iconTheme;
    QString m_windowControlsTheme = MauiMan::ThemeManager::DefaultValues::windowControlsTheme;
    bool m_enableCSD = MauiMan::ThemeManager::DefaultValues::enableCSD;
    uint m_borderRadius = MauiMan::ThemeManager::DefaultValues::borderRadius;
    uint m_iconSize = MauiMan::ThemeManager::DefaultValues::iconSize;
    bool m_enableEffects = MauiMan::ThemeManager::DefaultValues::enableEffects;
};
