#pragma once

#include <QObject>
#include <QString>

#include "mauiman_export.h"

class QDBusInterface;
namespace MauiMan
{
class SettingsStore;

/**
 * The ScreenManager class exposes all the system screen properties.
 */
class MAUIMAN_EXPORT ScreenManager : public QObject
{
    Q_OBJECT
    /**
     * The preferred scale factor for the main screen.
     */
    Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)
    
    /**
     * The preferred orientation of the main screen.
     * Possible values are:
     * - 0 Horizontal - Landscape
     * - 1 Vertical - Portrait
     */
    Q_PROPERTY(uint orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)

public:
    
    /**
     * @brief The Screen module default values.
     */
    struct DefaultValues
    {
        static inline const double scaleFactor = 1;
        static inline const double orientation = 0;
    } ;

    explicit ScreenManager(QObject * parent = nullptr);

    double scaleFactor() const;
    void setScaleFactor(double scaleFactor);

    uint orientation() const;
    void setOrientation(uint orientation);

private Q_SLOTS:
    void onScaleFactorChanged(double scale);
    void onOrientationChanged(uint orientation);

Q_SIGNALS:
    void scaleFactorChanged(double scaleFactor);
    void orientationChanged(uint orientation);

private:
#if !defined Q_OS_ANDROID
    QDBusInterface *m_interface = nullptr;
#endif
    MauiMan::SettingsStore *m_settings;

    void sync(const QString &key, const QVariant &value);
    void setConnections();
    void loadSettings();

    double m_scaleFactor = ScreenManager::DefaultValues::scaleFactor;
    uint m_orientation = ScreenManager::DefaultValues::orientation;
};
}
