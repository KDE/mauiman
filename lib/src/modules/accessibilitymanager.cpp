#include "accessibilitymanager.h"
#include "settingsstore.h"
#include "mauimanutils.h"

#include <QDebug>

#if !defined Q_OS_ANDROID
#include <QDBusInterface>
#endif

using namespace MauiMan;

AccessibilityManager::AccessibilityManager(QObject *parent) : QObject(parent)
  ,m_settings(new MauiMan::SettingsStore(this))
{
    qDebug( " INIT ACCESSIBILITY MANAGER");

#if !defined Q_OS_ANDROID
    auto server = new MauiManUtils(this);
    if(server->serverRunning())
    {
        this->setConnections();
    }

    connect(server, &MauiManUtils::serverRunningChanged, [this](bool state)
    {
        if(state)
        {
            this->setConnections();
        }
    });
#endif

    loadSettings();
}

bool AccessibilityManager::singleClick() const
{
    return m_singleClick;
}

void AccessibilityManager::setSingleClick(bool singleClick)
{
    if (m_singleClick == singleClick)
        return;

    m_singleClick = singleClick;

    sync("setSingleClick", m_singleClick);
    m_settings->save("SingleClick", m_singleClick);

    emit singleClickChanged(m_singleClick);
}

void AccessibilityManager::onSingleClickChanged(bool singleClick)
{
    if (m_singleClick == singleClick)
        return;

    m_singleClick = singleClick;
    emit singleClickChanged(m_singleClick);
}

void AccessibilityManager::sync(const QString &key, const QVariant &value)
{
#if !defined Q_OS_ANDROID
    if (m_interface && m_interface->isValid())
    {
        m_interface->call(key, value);
    }
#endif
}

void AccessibilityManager::setConnections()
{
#if !defined Q_OS_ANDROID
    if(m_interface)
    {
        m_interface->disconnect();
        m_interface->deleteLater();
        m_interface = nullptr;
    }

    m_interface = new QDBusInterface("org.mauiman.Manager",
                                       "/Accessibility",
                                     "org.mauiman.Accessibility",
                                       QDBusConnection::sessionBus(), this);

    if (m_interface->isValid())
    {
        connect(m_interface, SIGNAL(singleClickChanged(bool)), this, SLOT(onSingleClickChanged(bool)));
    }
#endif
}

void AccessibilityManager::loadSettings()
{
    m_settings->beginModule("Accessibility");

#if !defined Q_OS_ANDROID
    if(m_interface && m_interface->isValid())
    {
        m_singleClick = m_interface->property("singleClick").toBool();
        return;
    }
#endif

    m_singleClick = m_settings->load("SingleClick", m_singleClick).toBool();
}
