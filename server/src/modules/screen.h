#pragma once

#include <QObject>
#include <QString>
#include "modules/screenmanager.h"

#include <QObject>

class Screen : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)
    Q_PROPERTY(uint orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)

public:
    explicit Screen(QObject *parent = nullptr);

    double scaleFactor() const;
    void setScaleFactor(double scaleFactor);

    uint orientation() const;
    void setOrientation(uint orientation);

Q_SIGNALS:
    void scaleFactorChanged(double scaleFactor);

    void orientationChanged(uint orientation);

private:
    double m_scaleFactor = MauiMan::ScreenManager::DefaultValues::scaleFactor;
    uint m_orientation = MauiMan::ScreenManager::DefaultValues::orientation;
};

