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

public:
    struct DefaultValues
    {
        static inline const double scaleFactor = 1;
    } ;

    explicit ScreenManager(QObject * parent = nullptr);

    double scaleFactor() const;
    void setScaleFactor(double scaleFactor);

private slots:
    void onScaleFactorChanged(double scale);

signals:
    void scaleFactorChanged(double scaleFactor);

private:
#if !defined Q_OS_ANDROID
    QDBusInterface *m_interface = nullptr;
#endif
    MauiMan::SettingsStore *m_settings;

    void sync(const QString &key, const QVariant &value);
    void setConnections();
    void loadSettings();
    double m_scaleFactor = ScreenManager::DefaultValues::scaleFactor;
};
}
