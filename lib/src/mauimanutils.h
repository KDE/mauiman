#pragma once

#include <QObject>
#include "mauiman_export.h"

/**
 * @brief The MauiManUtils class
 */
class MAUIMAN_EXPORT MauiManUtils : public QObject
{
    Q_OBJECT
    /**
     * Whether the server application is running, listening and broadcasting events.
     */
    Q_PROPERTY(bool serverRunning READ serverRunning NOTIFY serverRunningChanged)

public:
    explicit MauiManUtils(QObject *parent = nullptr);

    bool serverRunning() const;

    /**
     * @brief Invoke the `MauiManServer4` application to be launched.
     */
    static void startServer();

    /**
     * @brief Invoke the `MauiSettings` application to be launched at a given module name
     * @param module the name of the module to be launched
     */
    static void invokeManager(const QString &module);

    /**
     * @brief The name of the current desktop environment. Possible values are KDE, GNOME, or CASK
     * @return the name of the desktop environment as parsed form the env var `XDG_CURRENT_DESKTOP`
     */
    static QString currentDesktopSession();

    /**
     * @brief Whether the current desktop environment session is running Maui Shell
     * @return
     */
    static bool isMauiSession();

    /**
     * @brief Whether the current desktop environment session is Plasma from KDE
     * @return
     */
    static bool isPlasmaSession();

    /**
     * @brief Whether the current desktop environment is GNOME Shell
     * @return
     */
    static bool isGnomeSession();

Q_SIGNALS:
    void serverRunningChanged(bool state);

private:
    bool m_serverRunning = false;
};

