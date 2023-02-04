#pragma once

#include <QObject>
#include <QString>

#if !defined Q_OS_ANDROID
#include <QDBusInterface>
#endif

#include "mauiman_export.h"
#include "mauimanutils.h"

/**
 * @brief The ThemeManager class
 * Helpfull for third parties to connect to property changes from the Theme module setting changes.
 */
namespace MauiMan
{


class SettingsStore;
class MAUIMAN_EXPORT ThemeManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int styleType READ styleType WRITE setStyleType NOTIFY styleTypeChanged)
    Q_PROPERTY(QString accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged RESET accentColor)
    Q_PROPERTY(QString iconTheme READ iconTheme WRITE setIconTheme NOTIFY iconThemeChanged)
    Q_PROPERTY(QString windowControlsTheme READ windowControlsTheme WRITE setWindowControlsTheme NOTIFY windowControlsThemeChanged)
    Q_PROPERTY(bool enableCSD READ enableCSD WRITE setEnableCSD NOTIFY enableCSDChanged)
    Q_PROPERTY(uint borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged RESET resetBorderRadius)
    Q_PROPERTY(uint iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged RESET resetIconSize)
    Q_PROPERTY(uint paddingSize READ paddingSize WRITE setPaddingSize NOTIFY paddingSizeChanged RESET resetPaddingSize)
    Q_PROPERTY(uint marginSize READ marginSize WRITE setMarginSize NOTIFY marginSizeChanged RESET resetMarginSize)
    Q_PROPERTY(bool enableEffects READ enableEffects WRITE setEnableEffects NOTIFY enableEffectsChanged)

public:

    struct DefaultValues
    {
        static int preferredStyleType()
        {

#ifdef Q_OS_ANDROID
            return 1;
#endif
#ifdef Q_OS_LINUX
            if(!MauiManUtils::isMauiSession())
            {
                return 3; //if it is plasma or other session use the system color scheme by setting the style to 3=auto
            }
#endif
            return 0;
        }

        static inline const int styleType = ThemeManager::DefaultValues::preferredStyleType();
        static inline const QString accentColor = "#26c6da";
        static inline const QString iconTheme = "Luv";
        static inline const QString windowControlsTheme = "Nitrux";
        static inline const bool enableCSD = true;
        static inline const uint borderRadius = 6;
        static inline const uint iconSize = 16;
        static inline const bool enableEffects = true;
        static inline const uint paddingSize = 6;
        static inline const uint marginSize = 8;
    } ;

    explicit ThemeManager(QObject * parent = nullptr);

    int styleType() const;
    void setStyleType(int newStyleType);

    const QString &accentColor() const;
    void setAccentColor(const QString &newAccentColor);
    void resetAccentColor();

    const QString &iconTheme() const;
    void setIconTheme(const QString &newIconTheme);

    const QString &windowControlsTheme() const;
    void setWindowControlsTheme(const QString &newWindowControlsTheme);

    bool enableCSD() const;
    void setEnableCSD(bool enableCSD);

    uint borderRadius() const;
    void setBorderRadius(uint newBorderRadius);
    void resetBorderRadius();

    uint iconSize() const;
    void setIconSize(uint newIconSize);
    void resetIconSize();

    bool enableEffects() const;
    void setEnableEffects(bool enableEffects);

    uint paddingSize() const;
    void setPaddingSize(uint paddingSize);
    void resetPaddingSize();

    uint marginSize() const;
    void setMarginSize(uint marginSize);
    void resetMarginSize();

private slots:
    void onStyleTypeChanged(const int &newStyleType);
    void onAccentColorChanged(const QString &newAccentColor);
    void onWindowControlsThemeChanged(const QString &newWindowControlsTheme);
    void onIconThemeChanged(const QString &newIconTheme);
    void onEnableCSDChanged(const bool &enableCSD);
    void onBorderRadiusChanged(const uint &radius);
    void onIconSizeChanged(const uint &size);
    void onPaddingSizeChanged(const uint &paddingSize);
    void onMarginSizeChanged(const uint &marginSize);
    void onEnableEffectsChanged(bool enableEffects);

signals:

    void styleTypeChanged(int styleType);
    void accentColorChanged(QString accentColor);
    void iconThemeChanged(QString iconTheme);
    void windowControlsThemeChanged(QString windowControlsTheme);
    void enableCSDChanged(bool enableCSD);
    void borderRadiusChanged(uint radius);
    void iconSizeChanged(uint size);
    void enableEffectsChanged(bool enableEffects);
    void paddingSizeChanged(uint paddingSize);
    void marginSizeChanged(uint marginSize);

private:
#if !defined Q_OS_ANDROID
    QDBusInterface *m_interface = nullptr;
#endif

    MauiMan::SettingsStore *m_settings;

    int m_styleType = ThemeManager::DefaultValues::styleType;
    QString m_accentColor = ThemeManager::DefaultValues::accentColor;
    QString m_iconTheme = ThemeManager::DefaultValues::iconTheme;
    QString m_windowControlsTheme = ThemeManager::DefaultValues::windowControlsTheme;
    bool m_enableCSD = ThemeManager::DefaultValues::enableCSD;
    uint m_borderRadius = ThemeManager::DefaultValues::borderRadius;
    uint m_iconSize = ThemeManager::DefaultValues::iconSize;
    uint m_paddingSize = ThemeManager::DefaultValues::paddingSize;
    uint m_marginSize = ThemeManager::DefaultValues::marginSize;
    bool m_enableEffects = ThemeManager::DefaultValues::enableEffects;

    void sync(const QString &key, const QVariant &value);
    void setConnections();
    void loadSettings();
    void updateGtk3Config();

};
}

