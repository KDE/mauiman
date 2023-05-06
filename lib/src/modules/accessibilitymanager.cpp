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

    sync(QStringLiteral("setSingleClick"), m_singleClick);
    m_settings->save(QStringLiteral("SingleClick"), m_singleClick);

    Q_EMIT singleClickChanged(m_singleClick);
}

void AccessibilityManager::onSingleClickChanged(bool singleClick)
{
    if (m_singleClick == singleClick)
        return;

    m_singleClick = singleClick;
    Q_EMIT singleClickChanged(m_singleClick);
}

void AccessibilityManager::onScrollBarPolicyChanged(uint scrollBarPolicy)
{
    if (m_scrollBarPolicy == scrollBarPolicy)
        return;

    m_scrollBarPolicy = scrollBarPolicy;
    Q_EMIT scrollBarPolicyChanged(m_scrollBarPolicy);
}

void AccessibilityManager::onPlaySoundsChanged(bool playSounds)
{
    if (m_playSounds == playSounds)
        return;

    m_playSounds = playSounds;
    Q_EMIT playSoundsChanged(m_playSounds);
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

    m_interface = new QDBusInterface(QStringLiteral("org.mauiman.Manager"),
                                     QStringLiteral("/Accessibility"),
                                     QStringLiteral("org.mauiman.Accessibility"),
                                     QDBusConnection::sessionBus(), this);

    if (m_interface->isValid())
    {
        connect(m_interface, SIGNAL(singleClickChanged(bool)), this, SLOT(onSingleClickChanged(bool)));
        connect(m_interface, SIGNAL(playSoundsChanged(bool)), this, SLOT(onPlaySoundsChanged(bool)));
        connect(m_interface, SIGNAL(scrollBarPolicyChanged(uint)), this, SLOT(onScrollBarPolicyChanged(uint)));
    }
#endif
}

void AccessibilityManager::loadSettings()
{
    m_settings->beginModule(QStringLiteral("Accessibility"));

#if !defined Q_OS_ANDROID
    if(m_interface && m_interface->isValid())
    {
        m_singleClick = m_interface->property("singleClick").toBool();
        m_scrollBarPolicy = m_interface->property("scrollBarPolicy").toUInt();
        m_playSounds = m_interface->property("playSounds").toBool();
        return;
    }
#endif

    m_singleClick = m_settings->load(QStringLiteral("SingleClick"), m_singleClick).toBool();
    m_scrollBarPolicy = m_settings->load(QStringLiteral("ScrollBarPolicy"), m_scrollBarPolicy).toUInt();
    m_playSounds = m_settings->load(QStringLiteral("PlaySounds"), m_playSounds).toBool();
}

uint AccessibilityManager::scrollBarPolicy() const
{
    return m_scrollBarPolicy;
}

void AccessibilityManager::setScrollBarPolicy(uint newScrollBarPolicy)
{
    if (m_scrollBarPolicy == newScrollBarPolicy)
        return;

    m_scrollBarPolicy = newScrollBarPolicy;

    sync(QStringLiteral("setScrollBarPolicy"), m_scrollBarPolicy);
    m_settings->save(QStringLiteral("ScrollBarPolicy"), m_scrollBarPolicy);

    Q_EMIT scrollBarPolicyChanged(m_scrollBarPolicy);
}

bool AccessibilityManager::playSounds() const
{
    return m_playSounds;
}

void AccessibilityManager::setPlaySounds(bool newPlaySounds)
{
    if (m_playSounds == newPlaySounds)
        return;

    m_playSounds = newPlaySounds;

    sync(QStringLiteral("setPlaySounds"), m_playSounds);
    m_settings->save(QStringLiteral("PlaySounds"), m_playSounds);

    Q_EMIT playSoundsChanged(m_playSounds);
}
