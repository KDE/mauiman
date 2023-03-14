#pragma once

#include <QObject>
#include <QString>
#include <QFont>

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
    Q_PROPERTY(uint spacingSize READ spacingSize WRITE setSpacingSize NOTIFY spacingSizeChanged RESET resetSPacingSize)
    Q_PROPERTY(bool enableEffects READ enableEffects WRITE setEnableEffects NOTIFY enableEffectsChanged)
    Q_PROPERTY(QString defaultFont READ defaultFont WRITE setDefaultFont NOTIFY defaultFontChanged RESET resetDefaultFont)
    Q_PROPERTY(QString smallFont READ smallFont WRITE setSmallFont NOTIFY smallFontChanged RESET resetSmallFont)
    Q_PROPERTY(QString monospacedFont READ monospacedFont WRITE setMonospacedFont NOTIFY monospacedFontChanged RESET resetMonospacedFont)

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

        static QString getDefaultFont()
        {
            QFont font {"Noto Sans", 10, QFont::Normal};
             font.setStyleHint(QFont::SansSerif);
             font.setStyle(QFont::StyleNormal);
             font.setStyleName("Regular");
             return font.toString();
        }

        static QString getSmallFont()
        {
            QFont font {"Noto Sans", 8, QFont::Normal};
             font.setStyleHint(QFont::SansSerif);
             font.setStyle(QFont::StyleNormal);
             font.setStyleName("Regular");

             return font.toString();
        }

        static QString getMonospacedFont()
        {
            QFont font {"Hack", 10, QFont::Normal};
             font.setStyleHint(QFont::Monospace);
             font.setStyle(QFont::StyleNormal);
             font.setStyleName("Regular");

             return font.toString();
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
        static inline const uint marginSize = 6;
        static inline const uint spacingSize = 6;
        static inline const QString defaultFont = getDefaultFont();
        static inline const QString smallFont = getSmallFont();
        static inline const QString monospacedFont = getMonospacedFont();
    };

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

    uint spacingSize() const;
    void setSpacingSize(uint spacingSize);
    void resetSPacingSize();

    QString defaultFont() const;
    void setDefaultFont(QString defaultFont);
    void resetDefaultFont();

    QString smallFont() const;
    void setSmallFont(QString smallFont);
    void resetSmallFont();

    QString monospacedFont() const;
    void setMonospacedFont(QString monospacedFont);
    void resetMonospacedFont();

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
    void onSpacingSizeChanged(const uint &spacingSize);
    void onEnableEffectsChanged(bool enableEffects);
    void onDefaultFontChanged(const QString &font);
    void onSmallFontChanged(const QString &font);
    void onMonospacedFontChanged(const QString &font);

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
    void spacingSizeChanged(uint spacingSize);

    void defaultFontChanged(QString defaultFont);
    void smallFontChanged(QString smallFont);
    void monospacedFontChanged(QString monospacedFont);

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
    uint m_spacingSize = ThemeManager::DefaultValues::spacingSize;

    bool m_enableEffects = ThemeManager::DefaultValues::enableEffects;

    QString m_defaultFont = MauiMan::ThemeManager::DefaultValues::defaultFont;
    QString m_smallFont = MauiMan::ThemeManager::DefaultValues::smallFont;
    QString m_monospacedFont =  MauiMan::ThemeManager::DefaultValues::monospacedFont;

    void sync(const QString &key, const QVariant &value);
    void setConnections();
    void loadSettings();

};
}

