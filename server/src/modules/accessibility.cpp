#include "accessibility.h"

#include <QDBusInterface>

#include "accessibilityadaptor.h"

#include "modules/accessibilitymanager.h"

#include "settingsstore.h"

Accessibility::Accessibility(QObject *parent) : QObject(parent)
{
    qDebug( " INIT ACCESSIBILITY MODULE");

    new AccessibilityAdaptor(this);
    if(!QDBusConnection::sessionBus().registerObject(QStringLiteral("/Accessibility"), this))
    {
        qDebug() << "FAILED TO REGISTER ACCESIBILITY DBUS OBJECT";
        return;
    }

    MauiMan::SettingsStore settings;
    settings.beginModule("Accessibility");
    m_singleClick = settings.load("SingleClick", m_singleClick).toBool();
    settings.endModule();
}

bool Accessibility::singleClick() const
{
    return m_singleClick;
}

void Accessibility::setSingleClick(bool singleClick)
{
    if (m_singleClick == singleClick)
        return;

    m_singleClick = singleClick;
    emit singleClickChanged(m_singleClick);
}
