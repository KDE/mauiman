#pragma once

#include <QObject>
#include <QString>

#include "mauiman_export.h"

#if !defined Q_OS_ANDROID
class QDBusInterface;
#endif

/**
 * @brief The BackgroundManager class
 * Helpfull for third parties to connect to property cahnges form the Background module setting changes.
 */
namespace MauiMan
{
class SettingsStore;

class MAUIMAN_EXPORT BackgroundManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString wallpaperSource READ wallpaperSource WRITE setWallpaperSource NOTIFY wallpaperSourceChanged)
    Q_PROPERTY(bool dimWallpaper READ dimWallpaper WRITE setDimWallpaper NOTIFY dimWallpaperChanged)
    Q_PROPERTY(bool fitWallpaper READ fitWallpaper WRITE setFitWallpaper NOTIFY fitWallpaperChanged)
    Q_PROPERTY(QString solidColor READ solidColor WRITE setSolidColor NOTIFY solidColorChanged)
    Q_PROPERTY(bool showWallpaper READ showWallpaper WRITE setShowWallpaper NOTIFY showWallpaperChanged)

    Q_PROPERTY(QString wallpaperSourceDir READ wallpaperSourceDir WRITE setWallpaperSourceDir NOTIFY wallpaperSourceDirChanged)

public:
    struct DefaultValues
    {
        static inline const QString wallpaperSource = QStringLiteral("qrc:/wallpapers/maui_shell_dev_bg.png");
        static inline const bool dimWallpaper = false;
        static inline const bool fitWallpaper = false; //false is to fill, true to fit
        static inline const QString solidColor = QStringLiteral("#333");
        static inline const bool showWallpaper = true;
        static inline const QString wallpaperSourceDir = QStringLiteral("file:///usr/share/wallpapers/Cask");
    } ;

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

