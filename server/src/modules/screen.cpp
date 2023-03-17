#include "screen.h"
#include "screenadaptor.h"
#include <QDBusInterface>

#include "settingsstore.h"

#include <QDebug>

Screen::Screen(QObject *parent) : QObject(parent)
{
    qDebug( " INIT SCREEN MODULE");
    new ScreenAdaptor(this);
    if(!QDBusConnection::sessionBus().registerObject(QStringLiteral("/Screen"), this))
    {
        qDebug() << "FAILED TO REGISTER SCREEN DBUS OBJECT";
        return;
    }
    MauiMan::SettingsStore settings;
    settings.beginModule(QStringLiteral("Screen"));
    m_scaleFactor = settings.load(QStringLiteral("ScaleFactor"), m_scaleFactor).toFloat();
    m_orientation = settings.load(QStringLiteral("Orientation"), m_scaleFactor).toUInt();
    settings.endModule();
}

double Screen::scaleFactor() const
{
    return m_scaleFactor;
}

void Screen::setScaleFactor(double scaleFactor)
{
    if (m_scaleFactor == scaleFactor)
        return;

    m_scaleFactor = scaleFactor;
    emit scaleFactorChanged(m_scaleFactor);
}

uint Screen::orientation() const
{
    return m_orientation;
}

void Screen::setOrientation(uint orientation)
{
    if (m_orientation == orientation)
        return;

    m_orientation = orientation;
    emit orientationChanged(m_orientation);
}
