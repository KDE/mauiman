#include "inputdevices.h"
#include "inputdevicesadaptor.h"
#include <QDBusInterface>
#include "settingsstore.h"

InputDevices::InputDevices(QObject *parent) : QObject(parent)
{
    qDebug( " INIT INPUT_DEVICES MODULE");

    new InputDevicesAdaptor(this);
    if(!QDBusConnection::sessionBus().registerObject(QStringLiteral("/InputDevices"), this))
    {
        qDebug() << "FAILED TO REGISTER INPUT_DEVICES DBUS OBJECT";
        return;
    }

    MauiMan::SettingsStore settings;
    settings.beginModule(QStringLiteral("InputDevices"));
    m_keyboardLayout = settings.load(QStringLiteral("KeyboardLayout"), m_keyboardLayout).toString();
    m_keyboardModel = settings.load(QStringLiteral("KeyboardModel"), m_keyboardModel).toString();
    m_keyboardOptions = settings.load(QStringLiteral("KeyboardOptions"), m_keyboardOptions).toString();
    m_keyboardRules = settings.load(QStringLiteral("KeyboardRules"), m_keyboardRules).toString();
    m_keyboardVariant = settings.load(QStringLiteral("KeyboardVariant"), m_keyboardVariant).toString();
    settings.endModule();
}

QString InputDevices::keyboardLayout() const
{
    return m_keyboardLayout;
}

QString InputDevices::keyboardModel() const
{
    return m_keyboardModel;
}

QString InputDevices::keyboardVariant() const
{
    return m_keyboardVariant;
}

QString InputDevices::keyboardOptions() const
{
    return m_keyboardOptions;
}

QString InputDevices::keyboardRules() const
{
    return m_keyboardRules;
}

void InputDevices::setKeyboardLayout(const QString &keyboardLayout)
{
    if (m_keyboardLayout == keyboardLayout)
        return;

    m_keyboardLayout = keyboardLayout;
    emit keyboardLayoutChanged(m_keyboardLayout);
}

void InputDevices::setKeyboardModel(const QString &keyboardModel)
{
    if (m_keyboardModel == keyboardModel)
        return;

    m_keyboardModel = keyboardModel;
    emit keyboardModelChanged(m_keyboardModel);
}

void InputDevices::setKeyboardVariant(const QString &keyboardVariant)
{
    if (m_keyboardVariant == keyboardVariant)
        return;

    m_keyboardVariant = keyboardVariant;
    emit keyboardVariantChanged(m_keyboardVariant);
}

void InputDevices::setKeyboardOptions(const QString &keyboardOptions)
{
    if (m_keyboardOptions == keyboardOptions)
        return;

    m_keyboardOptions = keyboardOptions;
    emit keyboardOptionsChanged(m_keyboardOptions);
}

void InputDevices::setKeyboardRules(const QString &keyboardRules)
{
    if (m_keyboardRules == keyboardRules)
        return;

    m_keyboardRules = keyboardRules;
    emit keyboardRulesChanged(m_keyboardRules);
}
