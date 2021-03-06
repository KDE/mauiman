#pragma once

#include <QObject>
#include <QString>
#include "code/modules/backgroundmanager.h"

class Background : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString wallpaperSource READ wallpaperSource WRITE setWallpaperSource NOTIFY wallpaperSourceChanged)
    Q_PROPERTY(bool dimWallpaper READ dimWallpaper WRITE setDimWallpaper NOTIFY dimWallpaperChanged)
    Q_PROPERTY(bool fitWallpaper READ fitWallpaper WRITE setFitWallpaper NOTIFY fitWallpaperChanged)
    Q_PROPERTY(QString solidColor READ solidColor WRITE setSolidColor NOTIFY solidColorChanged)
    Q_PROPERTY(bool showWallpaper READ showWallpaper WRITE setShowWallpaper NOTIFY showWallpaperChanged)

public:
    explicit Background(QObject * parent = nullptr);

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

signals:
    void wallpaperSourceChanged(QString wallpaperSource);

    void dimWallpaperChanged(bool dimWallpaper);

    void fitWallpaperChanged(bool fitWallpaper);

    void solidColorChanged(QString solidColor);

    void showWallpaperChanged(bool showWallpaper);

private:
    QString m_wallpaperSource = MauiMan::BackgroundManager::DefaultValues::wallpaperSource;
    bool m_dimWallpaper = MauiMan::BackgroundManager::DefaultValues::dimWallpaper;
    bool m_fitWallpaper = MauiMan::BackgroundManager::DefaultValues::fitWallpaper;
    QString m_solidColor = MauiMan::BackgroundManager::DefaultValues::solidColor;
    bool m_showWallpaper = MauiMan::BackgroundManager::DefaultValues::showWallpaper;

};
