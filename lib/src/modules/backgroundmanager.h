#pragma once

#include <QObject>
#include <QString>

#include "mauiman_export.h"

class QDBusInterface;
namespace MauiMan
{
class SettingsStore;

/**
 * @brief The BackgroundManager class
 * Helpfull for third parties to connect to property changes from the Background module setting changes.
 */
class MAUIMAN_EXPORT BackgroundManager : public QObject
{
    Q_OBJECT

    /**
     * The image file path to be used as the wallpaper background
     */
    Q_PROPERTY(QString wallpaperSource READ wallpaperSource WRITE setWallpaperSource NOTIFY wallpaperSourceChanged)

    /**
     * Whether the wallpaper background should have a dimmed effect. This could be used to reduce eye strain, or for a night mode effect
     */
    Q_PROPERTY(bool dimWallpaper READ dimWallpaper WRITE setDimWallpaper NOTIFY dimWallpaperChanged)

    /**
     * Whether the wallpaper image should fit the screen area, preserving its aspect ratio, instead of filling it
     */
    Q_PROPERTY(bool fitWallpaper READ fitWallpaper WRITE setFitWallpaper NOTIFY fitWallpaperChanged)

    /**
     * A color to be used in the background. If not wallpaper image is shown then, this is the color visible
     */
    Q_PROPERTY(QString solidColor READ solidColor WRITE setSolidColor NOTIFY solidColorChanged)

    /**
     * Whether to display the wallpaper image
     */
    Q_PROPERTY(bool showWallpaper READ showWallpaper WRITE setShowWallpaper NOTIFY showWallpaperChanged)

    /**
     * The preferred file path to the directory containing the wallpaper images
     */
    Q_PROPERTY(QString wallpaperSourceDir READ wallpaperSourceDir WRITE setWallpaperSourceDir NOTIFY wallpaperSourceDirChanged)

public:
/**
     * @brief The DefaultValues class
     */
    struct DefaultValues
    {
        static inline const QString wallpaperSource = QStringLiteral("qrc:/wallpapers/maui_shell_dev_bg.png");
        static inline const bool dimWallpaper = false;
        static inline const bool fitWallpaper = false; //false is to fill, true to fit
        static inline const QString solidColor = QStringLiteral("#333");
        static inline const bool showWallpaper = true;
        static inline const QString wallpaperSourceDir = QStringLiteral("file:///usr/share/wallpapers/Cask");
    };

    explicit BackgroundManager(QObject * parent = nullptr);

    QString wallpaperSource() const;

    bool dimWallpaper() const;

    bool fitWallpaper() const;

    QString solidColor() const;

    bool showWallpaper() const;

    void setWallpaperSource(QString wallpaperSource);

    void setDimWallpaper(bool dimWallpaper);

    void setFitWallpaper(bool fitWallpaper);

    void setSolidColor(QString solidColor);

    void setShowWallpaper(bool showWallpaper);

    QString wallpaperSourceDir() const;
    void setWallpaperSourceDir(QString wallpaperSourceDir);

private Q_SLOTS:
    void onWallpaperChanged(const QString &wallpaperSource);
    void onSolidColorChanged(const QString &solidColor);
    void onFitWallpaperChanged(const bool &fitWallpaper);
    void onDimWallpaperChanged(const bool &dimWallpaper);
    void onShowWallpaperChanged(const bool &showWallpaper);

Q_SIGNALS:
    void wallpaperSourceChanged(QString wallpaperSource);
    void dimWallpaperChanged(bool dimWallpaper);
    void fitWallpaperChanged(bool fitWallpaper);
    void solidColorChanged(QString solidColor);
    void showWallpaperChanged(bool showWallpaper);
    void wallpaperSourceDirChanged(QString wallpaperSourceDir);

private:
#if !defined Q_OS_ANDROID
    QDBusInterface *m_interface = nullptr;
#endif
    MauiMan::SettingsStore *m_settings;

    QString m_wallpaperSource = MauiMan::BackgroundManager::DefaultValues::wallpaperSource;
    bool m_dimWallpaper = MauiMan::BackgroundManager::DefaultValues::dimWallpaper;
    bool m_fitWallpaper = MauiMan::BackgroundManager::DefaultValues::fitWallpaper; //false is to fill, true to fit
    QString m_solidColor = MauiMan::BackgroundManager::DefaultValues::solidColor;
    bool m_showWallpaper = MauiMan::BackgroundManager::DefaultValues::showWallpaper;

    QString m_wallpaperSourceDir = MauiMan::BackgroundManager::DefaultValues::wallpaperSourceDir;

    void sync(const QString &key, const QVariant &value);
    void setConnections();
    void loadSettings();
};
}

