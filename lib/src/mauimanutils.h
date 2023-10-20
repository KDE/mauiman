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
     * Whether the server application is running and listening and broadcasting events.
     */
    Q_PROPERTY(bool serverRunning READ serverRunning NOTIFY serverRunningChanged)

public:
    explicit MauiManUtils(QObject *parent = nullptr);

    bool serverRunning() const;

    /**
     * @brief startServer
     */
    static void startServer();

    /**
     * @brief invokeManager
     * @param module
     */
    static void invokeManager(const QString &module);

    /**
     * @brief currentDesktopSession
     * @return
     */
    static QString currentDesktopSession();

    /**
     * @brief isMauiSession
     * @return
     */
    static bool isMauiSession();

    /**
     * @brief isPlasmaSession
     * @return
     */
    static bool isPlasmaSession();

    /**
     * @brief isGnomeSession
     * @return
     */
    static bool isGnomeSession();

Q_SIGNALS:
    void serverRunningChanged(bool state);

private:
    bool m_serverRunning = false;
};

