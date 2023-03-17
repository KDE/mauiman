#include "inputdevicesmanager.h"
#include "settingsstore.h"
#include "mauimanutils.h"

#include <QDebug>

#if !defined Q_OS_ANDROID
#include <QDBusInterface>
#endif

using namespace MauiMan;
InputDevicesManager::InputDevicesManager(QObject *parent) : QObject(parent)
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

QString MauiMan::InputDevicesManager::keyboardLayout() const
{
    return m_keyboardLayout;
}

QString MauiMan::InputDevicesManager::keyboardModel() const
{
    return m_keyboardModel;
}

QString MauiMan::InputDevicesManager::keyboardVariant() const
{
    return m_keyboardVariant;
}

QString MauiMan::InputDevicesManager::keyboardOptions() const
{
    return m_keyboardOptions;
}

QString MauiMan::InputDevicesManager::keyboardRules() const
{
    return m_keyboardRules;
}

void MauiMan::InputDevicesManager::setKeyboardLayout(QString keyboardLayout)
{
    if (m_keyboardLayout == keyboardLayout)
        return;

    m_keyboardLayout = keyboardLayout;
    sync(QStringLiteral("setKeyboardLayout"), m_keyboardLayout);
    m_settings->save(QStringLiteral("KeyboardLayout"), m_keyboardLayout);
    Q_EMIT keyboardLayoutChanged(m_keyboardLayout);
}

void MauiMan::InputDevicesManager::setKeyboardModel(QString keyboardModel)
{
    if (m_keyboardModel == keyboardModel)
        return;

    m_keyboardModel = keyboardModel;
    sync(QStringLiteral("setKeyboardModel"), m_keyboardModel);
    m_settings->save(QStringLiteral("KeyboardModel"), m_keyboardModel);
    Q_EMIT keyboardModelChanged(m_keyboardModel);
}

void MauiMan::InputDevicesManager::setKeyboardVariant(QString keyboardVariant)
{
    if (m_keyboardVariant == keyboardVariant)
        return;

    m_keyboardVariant = keyboardVariant;
    sync(QStringLiteral("setKeyboardVariant"), m_keyboardVariant);
    m_settings->save(QStringLiteral("KeyboardVariant"), m_keyboardVariant);
    Q_EMIT keyboardVariantChanged(m_keyboardVariant);
}

void MauiMan::InputDevicesManager::setKeyboardOptions(QString keyboardOptions)
{
    if (m_keyboardOptions == keyboardOptions)
        return;

    m_keyboardOptions = keyboardOptions;
    sync(QStringLiteral("setKeyboardOptions"), m_keyboardOptions);
    m_settings->save(QStringLiteral("KeyboardOptions"), m_keyboardOptions);
    Q_EMIT keyboardOptionsChanged(m_keyboardOptions);
}

void MauiMan::InputDevicesManager::setKeyboardRules(QString keyboardRules)
{
    if (m_keyboardRules == keyboardRules)
        return;

    m_keyboardRules = keyboardRules;
    sync(QStringLiteral("setKeyboardRules"), m_keyboardRules);
    m_settings->save(QStringLiteral("KeyboardRules"), m_keyboardRules);
    Q_EMIT keyboardRulesChanged(m_keyboardRules);
}

void MauiMan::InputDevicesManager::onKeyboardLayoutChanged(const QString &keyboardLayout)
{
    if (m_keyboardLayout == keyboardLayout)
        return;

    m_keyboardLayout = keyboardLayout;
    Q_EMIT keyboardLayoutChanged(m_keyboardLayout);
}

void MauiMan::InputDevicesManager::onKeyboardModelChanged(const QString &keyboardModel)
{
    if (m_keyboardModel == keyboardModel)
        return;

    m_keyboardModel = keyboardModel;
    Q_EMIT keyboardModelChanged(m_keyboardModel);
}

void MauiMan::InputDevicesManager::onKeyboardOptionsChanged(const QString &keyboardOptions)
{
    if (m_keyboardOptions == keyboardOptions)
        return;

    m_keyboardOptions = keyboardOptions;
    Q_EMIT keyboardOptionsChanged(m_keyboardOptions);
}

void MauiMan::InputDevicesManager::onKeyboardRulesChanged(const QString &keyboardRules)
{
    if (m_keyboardRules == keyboardRules)
        return;

    m_keyboardRules = keyboardRules;
    Q_EMIT keyboardRulesChanged(m_keyboardRules);
}

void MauiMan::InputDevicesManager::onKeyboardVariantChanged(const QString &keyboardVariant)
{
    if (m_keyboardVariant == keyboardVariant)
        return;

    m_keyboardVariant = keyboardVariant;
    Q_EMIT keyboardVariantChanged(m_keyboardVariant);
}

void MauiMan::InputDevicesManager::sync(const QString &key, const QVariant &value)
{
#if !defined Q_OS_ANDROID
    if (m_interface && m_interface->isValid())
    {
        m_interface->call(key, value);
    }
#else
    Q_UNUSED(key)
    Q_UNUSED(value)
#endif
}

void MauiMan::InputDevicesManager::setConnections()
{
#if !defined Q_OS_ANDROID
    if(m_interface)
    {
        m_interface->disconnect();
        m_interface->deleteLater();
        m_interface = nullptr;
    }

    m_interface = new QDBusInterface (QStringLiteral("org.mauiman.Manager"),
                                      QStringLiteral("/InputDevices"),
                                      QStringLiteral("org.mauiman.InputDevices"),
                                      QDBusConnection::sessionBus(), this);
    if (m_interface->isValid())
    {
        connect(m_interface, SIGNAL(keyboardLayoutChanged(double)), this, SLOT(onKeyboardLayoutChanged(QString)));
        connect(m_interface, SIGNAL(keyboardModelChanged(double)), this, SLOT(onKeyboardModelChanged(QString)));
        connect(m_interface, SIGNAL(keyboardRulesChanged(double)), this, SLOT(onKeyboardRulesChanged(QString)));
        connect(m_interface, SIGNAL(keyboardOptionsChanged(double)), this, SLOT(onKeyboardOptionsChanged(QString)));
        connect(m_interface, SIGNAL(keyboardVariantChanged(double)), this, SLOT(onKeyboardVariantChanged(QString)));

    }
#endif
}

void MauiMan::InputDevicesManager::loadSettings()
{
    m_settings->beginModule(QStringLiteral("InputDevices"));

#if !defined Q_OS_ANDROID

    if(m_interface && m_interface->isValid())
    {
        m_keyboardLayout = m_interface->property("keyboardLayout").toString();
        m_keyboardModel = m_interface->property("keyboardModel").toString();
        m_keyboardOptions = m_interface->property("keyboardOptions").toString();
        m_keyboardRules = m_interface->property("keyboardRules").toString();
        m_keyboardVariant = m_interface->property("keyboardVariant").toString();
        return;
    }
#endif

    m_keyboardLayout =  m_settings->load(QStringLiteral("KeyboardLayout"), m_keyboardLayout).toString();
    m_keyboardModel = m_settings->load(QStringLiteral("KeyboardModel"), m_keyboardModel).toString();
    m_keyboardOptions = m_settings->load(QStringLiteral("KeyboardOptions"), m_keyboardOptions).toString();
    m_keyboardRules = m_settings->load(QStringLiteral("KeyboardRules"), m_keyboardRules).toString();
    m_keyboardVariant = m_settings->load(QStringLiteral("KeyboardVariant"), m_keyboardVariant).toString();
}
