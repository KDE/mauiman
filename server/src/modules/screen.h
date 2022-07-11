#pragma once

#include <QObject>
#include <QString>
#include "code/modules/screenmanager.h"

#include <QObject>

class Screen : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)

public:
    explicit Screen(QObject *parent = nullptr);

    double scaleFactor() const;
    void setScaleFactor(double scaleFactor);

signals:
    void scaleFactorChanged(double scaleFactor);

private:
    double m_scaleFactor = MauiMan::ScreenManager::DefaultValues::scaleFactor;

};

