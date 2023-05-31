#pragma once

#include <QObject>
#include <QString>

#include "mauiman_export.h"

class QDBusInterface;
namespace MauiMan
{
class SettingsStore;

class MAUIMAN_EXPORT ScreenManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)
    Q_PROPERTY(uint orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)

public:
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
