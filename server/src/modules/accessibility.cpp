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
    settings.beginModule(QStringLiteral("Accessibility"));
    m_singleClick = settings.load(QStringLiteral("SingleClick"), m_singleClick).toBool();
    m_scrollBarPolicy = settings.load(QStringLiteral("ScrollBarPolicy"), m_scrollBarPolicy).toUInt();
    m_playSounds = settings.load(QStringLiteral("PlaySounds"), m_playSounds).toBool();
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
    Q_EMIT singleClickChanged(m_singleClick);
}

uint Accessibility::scrollBarPolicy() const
{
    return m_scrollBarPolicy;
}

void Accessibility::setScrollBarPolicy(uint newScrollBarPolicy)
{
    if (m_scrollBarPolicy == newScrollBarPolicy)
        return;

    m_scrollBarPolicy = newScrollBarPolicy;
    Q_EMIT scrollBarPolicyChanged(m_scrollBarPolicy);
}

bool Accessibility::playSounds() const
{
    return m_playSounds;
}

void Accessibility::setPlaySounds(bool newPlaySounds)
{
    if (m_playSounds == newPlaySounds)
        return;

    m_playSounds = newPlaySounds;
    Q_EMIT playSoundsChanged(newPlaySounds);
}
