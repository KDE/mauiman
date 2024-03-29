#include "backgroundmanager.h"

#include "settingsstore.h"
#include "mauimanutils.h"

#include <QDebug>

#if !defined Q_OS_ANDROID
#include <QDBusInterface>
#endif

using namespace MauiMan;
BackgroundManager::BackgroundManager(QObject *parent) : QObject(parent)
  ,m_settings(new MauiMan::SettingsStore(this))
{
    qDebug( " INIT BACKGORUND MANAGER");
#if !defined Q_OS_ANDROID
    auto server = new MauiManUtils(this);
    if(server->serverRunning())
    {
        this->setConnections();

    }

    connect(server, &MauiManUtils::serverRunningChanged, [this](bool state)
    {
        if(state)
        {
            this->setConnections();
        }
    });
#endif

    loadSettings();

}

QString BackgroundManager::wallpaperSource() const
{
    return m_wallpaperSource;
}

bool BackgroundManager::dimWallpaper() const
{
    return m_dimWallpaper;
}

bool BackgroundManager::fitWallpaper() const
{
    return m_fitWallpaper;
}

QString BackgroundManager::solidColor() const
{
    return m_solidColor;
}

bool BackgroundManager::showWallpaper() const
{
    return m_showWallpaper;
}

void BackgroundManager::setWallpaperSource(QString wallpaperSource)
{
    if (m_wallpaperSource == wallpaperSource)
        return;

    m_wallpaperSource = wallpaperSource;
    m_settings->save(QStringLiteral("Wallpaper"), m_wallpaperSource);
    sync(QStringLiteral("setWallpaperSource"), m_wallpaperSource);
    Q_EMIT wallpaperSourceChanged(m_wallpaperSource);
}

void BackgroundManager::setDimWallpaper(bool dimWallpaper)
{
    if (m_dimWallpaper == dimWallpaper)
        return;

    m_dimWallpaper = dimWallpaper;
    m_settings->save(QStringLiteral("DimWallpaper"), m_dimWallpaper);
    sync(QStringLiteral("setDimWallpaper"), m_dimWallpaper);
    Q_EMIT dimWallpaperChanged(m_dimWallpaper);
}

void BackgroundManager::setFitWallpaper(bool fitWallpaper)
{
    if (m_fitWallpaper == fitWallpaper)
        return;

    m_fitWallpaper = fitWallpaper;
    m_settings->save(QStringLiteral("FitWallpaper"), m_fitWallpaper);
    sync(QStringLiteral("setFitWallpaper"), m_fitWallpaper);
    Q_EMIT fitWallpaperChanged(m_fitWallpaper);
}

void BackgroundManager::setSolidColor(QString solidColor)
{
    if (m_solidColor == solidColor)
        return;

    m_solidColor = solidColor;
    m_settings->save(QStringLiteral("SolidColor"), m_solidColor);
    sync(QStringLiteral("setSolidColor"), m_solidColor);
    Q_EMIT solidColorChanged(m_solidColor);
}

void BackgroundManager::setShowWallpaper(bool showWallpaper)
{
    if (m_showWallpaper == showWallpaper)
        return;

    m_showWallpaper = showWallpaper;
    sync(QStringLiteral("setShowWallpaper"), m_showWallpaper);
    m_settings->save(QStringLiteral("ShowWallpaper"), m_showWallpaper);
    Q_EMIT showWallpaperChanged(m_showWallpaper);
}

QString BackgroundManager::wallpaperSourceDir() const
{
    return m_wallpaperSourceDir;
}

void BackgroundManager::setWallpaperSourceDir(QString wallpaperSourceDir)
{
    if (m_wallpaperSourceDir == wallpaperSourceDir)
        return;

    m_wallpaperSourceDir = wallpaperSourceDir;
    Q_EMIT wallpaperSourceDirChanged(m_wallpaperSourceDir);
}

void BackgroundManager::onWallpaperChanged(const QString &wallpaperSource)
{
    if (m_wallpaperSource == wallpaperSource)
        return;

    m_wallpaperSource = wallpaperSource;
    Q_EMIT wallpaperSourceChanged(m_wallpaperSource);
}

void BackgroundManager::onSolidColorChanged(const QString &solidColor)
{
    if (m_solidColor == solidColor)
        return;

    m_solidColor = solidColor;
    Q_EMIT solidColorChanged(m_solidColor);
}

void BackgroundManager::onFitWallpaperChanged(const bool &fitWallpaper)
{
    if (m_fitWallpaper == fitWallpaper)
        return;

    m_fitWallpaper = fitWallpaper;
    Q_EMIT fitWallpaperChanged(m_fitWallpaper);
}

void BackgroundManager::onDimWallpaperChanged(const bool &dimWallpaper)
{
    if (m_dimWallpaper == dimWallpaper)
        return;

    m_dimWallpaper = dimWallpaper;
    Q_EMIT dimWallpaperChanged(m_dimWallpaper);
}

void BackgroundManager::onShowWallpaperChanged(const bool &showWallpaper)
{
    if (m_showWallpaper == showWallpaper)
        return;

    m_showWallpaper = showWallpaper;
    Q_EMIT showWallpaperChanged(m_showWallpaper);
}

void BackgroundManager::sync(const QString &key, const QVariant &value)
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

void BackgroundManager::setConnections()
{
#if !defined Q_OS_ANDROID

    if(m_interface)
    {
        m_interface->disconnect();
        m_interface->deleteLater();
        m_interface = nullptr;
    }

    m_interface = new QDBusInterface (QStringLiteral("org.mauiman.Manager"),
                                      QStringLiteral("/Background"),
                                      QStringLiteral("org.mauiman.Background"),
                                      QDBusConnection::sessionBus(), this);
    if (m_interface->isValid())
    {
        connect(m_interface, SIGNAL(wallpaperSourceChanged(QString)), this, SLOT(onWallpaperChanged(QString)));
        connect(m_interface, SIGNAL(solidColorChanged(QString)), this, SLOT(onSolidColorChanged(QString)));
        connect(m_interface, SIGNAL(fitWallpaperChanged(bool)), this, SLOT(onFitWallpaperChanged(bool)));
        connect(m_interface, SIGNAL(showWallpaperChanged(bool)), this, SLOT(onShowWallpaperChanged(bool)));
        connect(m_interface, SIGNAL(dimWallpaperChanged(bool)), this, SLOT(onDimWallpaperChanged(bool)));

    }
#endif
}

void BackgroundManager::loadSettings()
{
    m_settings->beginModule(QStringLiteral("Background"));

#if !defined Q_OS_ANDROID

    if(m_interface && m_interface->isValid())
    {
        m_wallpaperSource = m_interface->property("wallpaperSource").toString();
        m_dimWallpaper = m_interface->property("dimWallpaper").toBool();
        m_showWallpaper = m_interface->property("showWallpaper").toBool();
        m_fitWallpaper = m_interface->property("fitWallpaper").toBool();
        m_solidColor = m_interface->property("solidColor").toString();
        return;
    }
#endif

    m_wallpaperSource = m_settings->load(QStringLiteral("Wallpaper"), m_wallpaperSource).toString();
    m_dimWallpaper = m_settings->load(QStringLiteral("DimWallpaper"), m_dimWallpaper).toBool();
    m_showWallpaper = m_settings->load(QStringLiteral("ShowWallpaper"), m_showWallpaper).toBool();
    m_fitWallpaper = m_settings->load(QStringLiteral("FitWallpaper"), m_fitWallpaper).toBool();
    m_solidColor = m_settings->load(QStringLiteral("SolidColor"), m_solidColor).toString();
}
