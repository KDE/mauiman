#pragma once

#include <QObject>
#include <QString>

#if !defined Q_OS_ANDROID
#include <QDBusInterface>
#endif

#include "mauiman_export.h"


/**
 * @brief The ThemeManager class
 * Helpfull for third parties to connect to property cahnges form the Background module setting changes.
 */
namespace MauiMan
{


class SettingsStore;
class MAUIMAN_EXPORT ThemeManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int styleType READ styleType WRITE setStyleType NOTIFY styleTypeChanged)
    Q_PROPERTY(QString accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)
    Q_PROPERTY(QString iconTheme READ iconTheme WRITE setIconTheme NOTIFY iconThemeChanged)
    Q_PROPERTY(QString windowControlsTheme READ windowControlsTheme WRITE setWindowControlsTheme NOTIFY windowControlsThemeChanged)
    Q_PROPERTY(bool enableCSD READ enableCSD WRITE setEnableCSD NOTIFY enableCSDChanged)
    Q_PROPERTY(uint borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(uint iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)

public:

    struct DefaultValues
    {
        static int preferredStyleType()
        {

#ifdef Q_OS_ANDROID
            return 1;
#endif
#ifdef Q_OS_LINUX
            if(qEnvironmentVariableIsSet("DESKTOP_SESSION"))
            {
                qEnvironmentVariable("DESKTOP_SESSION") == "plasma";
                return 3; //if it is plasma use the system color scheme by setting the style to 3=auto
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

    } ;

    explicit ThemeManager(QObject * parent = nullptr);

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

private slots:
    void onStyleTypeChanged(const int &newStyleType);
    void onAccentColorChanged(const QString &newAccentColor);
    void onWindowControlsThemeChanged(const QString &newWindowControlsTheme);
    void onIconThemeChanged(const QString &newIconTheme);
    void onEnableCSDChanged(const bool &enableCSD);
    void onBorderRadiusChanged(const uint &radius);
    void onIconSizeChanged(const uint &size);

signals:

    void styleTypeChanged(int styleType);

    void accentColorChanged(QString accentColor);

    void iconThemeChanged(QString iconTheme);

    void windowControlsThemeChanged(QString windowControlsTheme);

    void enableCSDChanged(bool enableCSD);

    void borderRadiusChanged(uint radius);

    void iconSizeChanged(uint size);

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

    void sync(const QString &key, const QVariant &value);
    void setConnections();
    void loadSettings();
};
}

