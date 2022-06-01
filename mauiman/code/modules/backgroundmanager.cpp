#include "backgroundmanager.h"

#include "code/settingsstore.h"
#include "code/mauimanutils.h"

#include <QDebug>

using namespace MauiMan;
BackgroundManager::BackgroundManager(QObject *parent) : QObject(parent)
  ,m_settings(new MauiMan::SettingsStore(this))

{
    qDebug( " INIT BACKGORUND MANAGER");
    m_settings->beginModule("Background");

    auto server = new MauiManUtils(this);
    if(server->serverRunning())
    {
        this->setConnections();

    }

    loadSettings();


    connect(server, &MauiManUtils::serverRunningChanged, [this](bool state)
    {
        if(state)
        {
            this->setConnections();
        }
    });
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
    m_settings->save("Wallpaper", m_wallpaperSource);
    sync("setWallpaperSource", m_wallpaperSource);
    emit wallpaperSourceChanged(m_wallpaperSource);
}

void BackgroundManager::setDimWallpaper(bool dimWallpaper)
{
    if (m_dimWallpaper == dimWallpaper)
        return;

    m_dimWallpaper = dimWallpaper;
    m_settings->save("DimWallpaper", m_dimWallpaper);
    sync("setDimWallpaper", m_dimWallpaper);
    emit dimWallpaperChanged(m_dimWallpaper);
}

void BackgroundManager::setFitWallpaper(bool fitWallpaper)
{
    if (m_fitWallpaper == fitWallpaper)
        return;

    m_fitWallpaper = fitWallpaper;
    m_settings->save("FitWallpaper", m_fitWallpaper);
    sync("setFitWallpaper", m_fitWallpaper);
    emit fitWallpaperChanged(m_fitWallpaper);
}

void BackgroundManager::setSolidColor(QString solidColor)
{
    if (m_solidColor == solidColor)
        return;

    m_solidColor = solidColor;
    m_settings->save("SolidColor", m_solidColor);
    sync("setSolidColor", m_solidColor);
    emit solidColorChanged(m_solidColor);
}

void BackgroundManager::setShowWallpaper(bool showWallpaper)
{
    if (m_showWallpaper == showWallpaper)
        return;

    m_showWallpaper = showWallpaper;
    sync("setShowWallpaper", m_showWallpaper);
    m_settings->save("ShowWallpaper", m_showWallpaper);
    emit showWallpaperChanged(m_showWallpaper);
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
    emit wallpaperSourceDirChanged(m_wallpaperSourceDir);
}

void BackgroundManager::onWallpaperChanged(const QString &wallpaperSource)
{
    if (m_wallpaperSource == wallpaperSource)
        return;

    m_wallpaperSource = wallpaperSource;
    emit wallpaperSourceChanged(m_wallpaperSource);
}

void BackgroundManager::onSolidColorChanged(const QString &solidColor)
{
    if (m_solidColor == solidColor)
        return;

    m_solidColor = solidColor;
    emit solidColorChanged(m_solidColor);
}

void BackgroundManager::onFitWallpaperChanged(const bool &fitWallpaper)
{
    if (m_fitWallpaper == fitWallpaper)
        return;

    m_fitWallpaper = fitWallpaper;
    emit fitWallpaperChanged(m_fitWallpaper);
}

void BackgroundManager::onDimWallpaperChanged(const bool &dimWallpaper)
{
    if (m_dimWallpaper == dimWallpaper)
        return;

    m_dimWallpaper = dimWallpaper;
    emit dimWallpaperChanged(m_dimWallpaper);
}

void BackgroundManager::onShowWallpaperChanged(const bool &showWallpaper)
{
    if (m_showWallpaper == showWallpaper)
        return;

    m_showWallpaper = showWallpaper;
    emit showWallpaperChanged(m_showWallpaper);
}

void BackgroundManager::sync(const QString &key, const QVariant &value)
{
    if (m_interface && m_interface->isValid())
    {
        m_interface->call(key, value);
    }
}

void BackgroundManager::setConnections()
{
    if(m_interface)
    {
        m_interface->disconnect();
        m_interface->deleteLater();
        m_interface = nullptr;
    }

    m_interface = new QDBusInterface ("org.mauiman.Manager",
                                      "/Background",
                                      "org.mauiman.Background",
                                      QDBusConnection::sessionBus(), this);
    if (m_interface->isValid())
    {
        connect(m_interface, SIGNAL(wallpaperSourceChanged(QString)), this, SLOT(onWallpaperChanged(QString)));
        connect(m_interface, SIGNAL(solidColorChanged(QString)), this, SLOT(onSolidColorChanged(QString)));
        connect(m_interface, SIGNAL(fitWallpaperChanged(bool)), this, SLOT(onFitWallpaperChanged(bool)));
        connect(m_interface, SIGNAL(showWallpaperChanged(bool)), this, SLOT(onShowWallpaperChanged(bool)));
        connect(m_interface, SIGNAL(dimWallpaperChanged(bool)), this, SLOT(onDimWallpaperChanged(bool)));

    }
}

void BackgroundManager::loadSettings()
{
    if(m_interface && m_interface->isValid())
    {
        m_wallpaperSource = m_interface->property("wallpaperSource").toString();
        m_dimWallpaper = m_interface->property("dimWallpaper").toBool();
        m_showWallpaper = m_interface->property("showWallpaper").toBool();
        m_fitWallpaper = m_interface->property("fitWallpaper").toBool();
        m_solidColor = m_interface->property("solidColor").toString();
        return;
    }

    m_wallpaperSource = m_settings->load("Wallpaper", m_wallpaperSource).toString();
    m_dimWallpaper = m_settings->load("DimWallpaper", m_dimWallpaper).toBool();
    m_showWallpaper = m_settings->load("ShowWallpaper", m_showWallpaper).toBool();
    m_fitWallpaper = m_settings->load("FitWallpaper", m_fitWallpaper).toBool();
    m_solidColor = m_settings->load("SolidColor", m_solidColor).toString();
}
