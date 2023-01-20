#include "formfactor.h"
#include "formfactoradaptor.h"
#include <QDBusInterface>
#include <QMap>


#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QTouchDevice>
#else
#include <QInputDevice>
#endif

#include "settingsstore.h"

#include <QtSystemInfo/qinputinfo.h>

static
QString typeToString(QInputDevice::InputTypeFlags type)
{
    qDebug() << type;
    QStringList typeString;
    if (type.testFlag(QInputDevice::Button))
        typeString << QStringLiteral("Button");
    if (type.testFlag(QInputDevice::Mouse))
        typeString << QStringLiteral("Mouse");
    if (type.testFlag(QInputDevice::TouchPad))
        typeString << QStringLiteral("TouchPad");
    if (type.testFlag(QInputDevice::TouchScreen))
        typeString << QStringLiteral("TouchScreen");
    if (type.testFlag(QInputDevice::Keyboard))
        typeString << QStringLiteral("Keyboard");
    if (type.testFlag(QInputDevice::Switch))
        typeString << QStringLiteral("Switch");

    if (typeString.isEmpty())
        typeString << QStringLiteral("Unknown");
    return typeString.join((", "));
}

FormFactor::FormFactor(QObject *parent) : QObject(parent)
{
    qDebug( " INIT BACKGORUND MODULE");
    new FormFactorAdaptor(this);
    if(!QDBusConnection::sessionBus().registerObject(QStringLiteral("/FormFactor"), this))
    {
        qDebug() << "FAILED TO REGISTER FORMFACTOR DBUS OBJECT";
        return;
    }

    //grab default values
    m_defaultMode = QByteArrayList{"1", "true"}.contains(qgetenv("QT_QUICK_CONTROLS_MOBILE")) ? MauiMan::FormFactorManager::Mode::Phone : MauiMan::FormFactorManager::Mode::Desktop;

    m_preferredMode = m_defaultMode;

    m_hasTouchscreen = hasTouchScreen();

    auto inputDeviceManager = new QInputInfoManager(this);
    connect(inputDeviceManager, &QInputInfoManager::ready,[this, inputDeviceManager]()
    {

        QMap <QString, QInputDevice *> map = inputDeviceManager->deviceMap();
        qDebug() << map.count();

        inputDeviceManager->setFilter(QInputDevice::Mouse | QInputDevice::Keyboard | QInputDevice::TouchScreen);
        checkInputs(inputDeviceManager);

    });

    connect(inputDeviceManager, &QInputInfoManager::deviceAdded,[this, inputDeviceManager](QInputDevice *)
    {
        checkInputs(inputDeviceManager);
    });
    connect(inputDeviceManager, &QInputInfoManager::deviceRemoved,[this, inputDeviceManager](QString)
    {
        checkInputs(inputDeviceManager);
    });

    //grab user preferences
    MauiMan::SettingsStore settings;
    settings.beginModule("FormFactor");
    m_preferredMode = settings.load("PreferredMode", m_preferredMode).toUInt();
    settings.endModule();

}

uint FormFactor::preferredMode() const
{
    return m_preferredMode;
}

uint FormFactor::bestMode() const
{
    return m_bestMode;
}

uint FormFactor::defaultMode() const
{
    return m_defaultMode;
}

bool FormFactor::hasKeyboard() const
{
    return m_hasKeyboard;
}

bool FormFactor::hasTouchscreen() const
{
    return m_hasTouchscreen;
}

bool FormFactor::hasMouse() const
{
    return m_hasMouse;
}

void FormFactor::setPreferredMode(uint preferredMode)
{
    if (m_preferredMode == preferredMode)
        return;

    m_preferredMode = preferredMode;
    Q_EMIT preferredModeChanged(m_preferredMode);
}

void FormFactor::setBestMode(uint bestMode)
{
    if (m_bestMode == bestMode)
        return;

    m_bestMode = bestMode;
    Q_EMIT bestModeChanged(m_bestMode);
}

void FormFactor::setDefaultMode(uint defaultMode)
{
    if (m_defaultMode == defaultMode)
        return;

    m_defaultMode = defaultMode;
    Q_EMIT defaultModeChanged(m_defaultMode);
}

void FormFactor::setHasKeyboard(bool hasKeyboard)
{
    if (m_hasKeyboard == hasKeyboard)
        return;

    m_hasKeyboard = hasKeyboard;
    Q_EMIT hasKeyboardChanged(m_hasKeyboard);
}

void FormFactor::setHasTouchscreen(bool hasTouchscreen)
{
    if (m_hasTouchscreen == hasTouchscreen)
        return;

    m_hasTouchscreen = hasTouchscreen;
    Q_EMIT hasTouchscreenChanged(m_hasTouchscreen);
}

void FormFactor::setHasMouse(bool hasMouse)
{
    if (m_hasMouse == hasMouse)
        return;

    m_hasMouse = hasMouse;
    Q_EMIT hasMouseChanged(m_hasMouse);
}

bool FormFactor::hasTouchScreen() const
{
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS) || defined(UBUNTU_TOUCH)
    return true;
#else

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const auto touchDevices = QTouchDevice::devices();
    const auto touchDeviceType = QTouchDevice::TouchScreen;
#else
    const auto touchDevices = QInputDevice::devices();
    const auto touchDeviceType = QInputDevice::DeviceType::TouchScreen;
#endif

    for (const auto &device : touchDevices) {
        if (device->type() == touchDeviceType) {
            return true;
        }
    }
#endif
    return false;
}

void FormFactor::checkInputs(const QInputInfoManager *inputManager)
{

    //        qDebug() <<"Found"<<  inputDeviceManager->deviceMap().count() << "input devices";
    //        QMapIterator<QString, QInputDevice*> i(inputDeviceManager->deviceMap());
    //        while (i.hasNext())
    //        {
    //            i.next();
    //            qDebug() << i.value()->name() << i.value()->identifier();
    //            qDebug() << "buttons count"<< i.value()->buttons().count();
    //            qDebug() << "switch count"<< i.value()->switches().count();
    //            qDebug() << "relativeAxes count"<< i.value()->relativeAxes().count();
    //            qDebug() << "absoluteAxes count"<< i.value()->absoluteAxes().count();
    //            qDebug() << "type" << typeToString(i.value()->types());

    //            qDebug();
    //            //       qDebug() << i.value()->name();
    //        }

    const int keyboardsCount = inputManager->count(QInputDevice::Keyboard);
    const int mouseCount = inputManager->count(QInputDevice::Mouse);
    const int touchCount = inputManager->count(QInputDevice::TouchScreen);

    m_hasKeyboard = keyboardsCount > 0;
    m_hasMouse = mouseCount > 0;
    m_hasTouchscreen = touchCount > 0;

    Q_EMIT hasKeyboardChanged(m_hasKeyboard);
    Q_EMIT hasMouseChanged(m_hasMouse);
    Q_EMIT hasTouchscreenChanged(m_hasTouchscreen);

    qDebug() << "Number of keyboards:" << keyboardsCount;
    qDebug() << "Number of mice:" << mouseCount;
    qDebug() << "Number of touchscreens:" << touchCount;
}
