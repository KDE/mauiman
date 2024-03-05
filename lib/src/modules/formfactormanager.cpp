#include "formfactormanager.h"

#include "settingsstore.h"
#include "mauimanutils.h"

#include <QDebug>
#include <QSize>
#include <QScreen>
#include <QGuiApplication>

#if !defined Q_OS_ANDROID
#include <QDBusInterface>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define QT5_BASED
#include <QtSystemInfo/qinputinfo.h>
#else
#define QT6_BASED
#include <QInputDevice>
#endif
#endif

using namespace MauiMan;

#if !defined Q_OS_ANDROID
#ifdef QT5_BASE
static QString typeToString(QInputDevice::InputTypeFlags type)
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
    return typeString.join((QStringLiteral(", ")));
}
#elif defined QT6_BASE
static QString typeToString(QInputDevice::DeviceTypes type)
{
    qDebug() << type;
    QStringList typeString;
    if (type.testFlag(QInputDevice::DeviceType::Mouse))
        typeString << QStringLiteral("Mouse");
    if (type.testFlag(QInputDevice::DeviceType::TouchPad))
        typeString << QStringLiteral("TouchPad");
    if (type.testFlag(QInputDevice::DeviceType::TouchScreen))
        typeString << QStringLiteral("TouchScreen");
    if (type.testFlag(QInputDevice::DeviceType::Keyboard))
        typeString << QStringLiteral("Keyboard");
    if (typeString.isEmpty())
        typeString << QStringLiteral("Unknown");
    return typeString.join((QStringLiteral(", ")));
}
#endif
#endif

void FormFactorManager::sync(const QString &key, const QVariant &value)
{
    #if !defined Q_OS_ANDROID
    if (m_interface && m_interface->isValid())
    {
        m_interface->call(key, value);
    }
    #endif
}

void FormFactorManager::setConnections()
{
    #if !defined Q_OS_ANDROID
    if(m_interface)
    {
        m_interface->disconnect();
        m_interface->deleteLater();
        m_interface = nullptr;
    }

    m_interface = new QDBusInterface(QStringLiteral("org.mauiman.Manager"),
                                     QStringLiteral("/FormFactor"),
                                     QStringLiteral("org.mauiman.FormFactor"),
                                     QDBusConnection::sessionBus(), this);

    if (m_interface->isValid())
    {
        connect(m_interface, SIGNAL(preferredModeChanged(uint)), this, SLOT(onPreferredModeChanged(uint)));
        connect(m_interface, SIGNAL(forceTouchScreenChanged(bool)), this, SLOT(onForceTouchScreenChanged(bool)));
    }
    #endif
}

void FormFactorManager::loadSettings()
{
    m_settings->beginModule(QStringLiteral("FormFactor"));

    #if !defined Q_OS_ANDROID
    if(m_interface && m_interface->isValid())
    {
        m_preferredMode = m_interface->property("preferredMode").toUInt();
        m_forceTouchScreen = m_interface->property("forceTouchScreen").toBool();
        return;
    }
    #endif

    m_preferredMode = m_settings->load(QStringLiteral("PreferredMode"), m_preferredMode).toUInt();
    m_forceTouchScreen = m_settings->load(QStringLiteral("ForceTouchScreen"), m_forceTouchScreen).toBool();
}

FormFactorManager::FormFactorManager(QObject *parent) : MauiMan::FormFactorInfo(parent)
,m_settings(new MauiMan::SettingsStore(this))
,m_info(new MauiMan::FormFactorInfo(this))
{
    qDebug( " INIT FORMFACTOR MANAGER");

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
    m_preferredMode = defaultMode();

    loadSettings();
}

uint FormFactorManager::preferredMode() const
{
    return m_preferredMode;
}

uint FormFactorInfo::bestMode() const
{
    return m_bestMode;
}

uint FormFactorInfo::defaultMode() const
{
    return m_defaultMode;
}

bool FormFactorInfo::hasKeyboard() const
{
    return m_hasKeyboard;
}

bool FormFactorInfo::hasTouchscreen() const
{
    return m_hasTouchscreen;
}

bool FormFactorInfo::hasMouse() const
{
    return m_hasMouse;
}

bool FormFactorInfo::hasTouchpad() const
{
    return m_hasTouchpad;
}

void FormFactorManager::setPreferredMode(uint preferredMode)
{
    if (m_preferredMode == preferredMode)
        return;

    m_preferredMode = preferredMode;

    sync(QStringLiteral("setPreferredMode"), m_preferredMode);
    m_settings->save(QStringLiteral("PreferredMode"), m_preferredMode);

    Q_EMIT preferredModeChanged(m_preferredMode);
}

bool FormFactorManager::forceTouchScreen() const
{
    return m_forceTouchScreen;
}

void FormFactorManager::setForceTouchScreen(bool newForceTouchScreen)
{
    if (m_forceTouchScreen == newForceTouchScreen)
        return;

    m_forceTouchScreen = newForceTouchScreen;

    sync(QStringLiteral("forceTouchScreen"), m_forceTouchScreen);
    m_settings->save(QStringLiteral("ForceTouchScreen"), m_forceTouchScreen);

    Q_EMIT forceTouchScreenChanged(m_forceTouchScreen);
}

void FormFactorManager::onPreferredModeChanged(uint preferredMode)
{
    if (m_preferredMode == preferredMode)
        return;

    m_preferredMode = preferredMode;
    Q_EMIT preferredModeChanged(m_preferredMode);
}

void FormFactorManager::onForceTouchScreenChanged(bool value)
{
    if (m_forceTouchScreen == value)
        return;

    m_forceTouchScreen = value;
    Q_EMIT forceTouchScreenChanged(m_forceTouchScreen);
}

void FormFactorInfo::findBestMode()
{
    uint bestMode = m_defaultMode;
    /*
     * 0- desktop
     * 1- tablet
     * 2- phone
     * */

    if(m_hasTouchscreen)
    {
        if(m_screenSize.width() > 1500)
        {
            if(m_hasKeyboard || m_hasMouse || m_hasTouchpad)
            {
                bestMode = 0; //A big touch screen and with keyboard/mouse/trackpad
            }else
            {
                bestMode = 1; //A big touch screen alone
            }
        }
        else if(m_screenSize.width()  > 500)
        {
            bestMode = 1; //A tablet size touch screen
        }
        else
        {
            bestMode = 2; //A mobile size touch screen
        }

    }else
    {

        if(m_screenSize.width() > 1500)
        {
            bestMode = 0; // A big screen

        }
        else if(m_screenSize.width()  > 500)
        {
            if(m_hasTouchpad)
            {
                bestMode = 1; // A small screen with a trackpad
            }else
            {
                bestMode = 0;
            }
        }
        else
        {
            bestMode = 1; //A mobile size touch screen
        }
    }

    m_bestMode = bestMode;
    Q_EMIT bestModeChanged(m_bestMode);
}

QRect FormFactorInfo::screenSize()
{
    QScreen *screen = qApp->primaryScreen();
    return screen->geometry();
}

Qt::ScreenOrientation FormFactorInfo::screenOrientation()
{
    QScreen *screen = qApp->primaryScreen();
    return screen->orientation();
}

#ifdef QT5_BASE
void FormFactorInfo::checkInputs(const QInputInfoManager *inputManager)
{
    #if !defined Q_OS_ANDROID

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
    const int trackpadCount = inputManager->count(QInputDevice::TouchPad);

    m_hasKeyboard = keyboardsCount > 0;
    m_hasMouse = mouseCount > 0;
    m_hasTouchscreen = touchCount > 0;
    m_hasTouchpad = trackpadCount > 0;

    Q_EMIT hasKeyboardChanged(m_hasKeyboard);
    Q_EMIT hasMouseChanged(m_hasMouse);
    Q_EMIT hasTouchscreenChanged(m_hasTouchscreen);
    Q_EMIT hasTouchpadChanged(m_hasTouchpad);

    qDebug() << "Number of keyboards:" << keyboardsCount;
    qDebug() << "Number of mice:" << mouseCount;
    qDebug() << "Number of touchscreens:" << touchCount;
    qDebug() << "Number of touchpads:" << trackpadCount;
    #endif
}
#elif defined QT6_BASE
void FormFactorInfo::checkInputs(const QList<const QInputDevice *> &devices)
{
    auto hasType = [devices](QInputDevice::DeviceType type) -> bool
    {
        auto res= std::find_if(devices.constBegin(), devices.constEnd(), [type](const QInputDevice *device)
        {
            return device->type() == type;
        });

        return res != std::end(devices);
    };

    m_hasKeyboard = hasType(QInputDevice::DeviceType::Keyboard);
    m_hasMouse =  hasType(QInputDevice::DeviceType::Mouse);
    m_hasTouchscreen =  hasType(QInputDevice::DeviceType::TouchScreen);
    m_hasTouchpad =  hasType(QInputDevice::DeviceType::TouchPad);

    Q_EMIT hasKeyboardChanged(m_hasKeyboard);
    Q_EMIT hasMouseChanged(m_hasMouse);
    Q_EMIT hasTouchscreenChanged(m_hasTouchscreen);
    Q_EMIT hasTouchpadChanged(m_hasTouchpad);
}
#endif

FormFactorInfo::FormFactorInfo(QObject *parent) : QObject(parent)
{
    qDebug( " INIT FORMFACTOR INFO");

    #if !defined Q_OS_ANDROID

    #ifdef QT5_BASED
    auto inputDeviceManager = new QInputInfoManager(this);
    connect(inputDeviceManager, &QInputInfoManager::ready,[ inputDeviceManager]()
    {
        inputDeviceManager->setFilter(QInputDevice::Mouse | QInputDevice::Keyboard | QInputDevice::TouchScreen | QInputDevice::TouchPad);
    });

    connect(inputDeviceManager, &QInputInfoManager::filterChanged,this,[this, inputDeviceManager](QInputDevice::InputTypeFlags )
    {
        checkInputs(inputDeviceManager);
        findBestMode();
    });

    connect(inputDeviceManager, &QInputInfoManager::deviceAdded,[this, inputDeviceManager](QInputDevice *)
    {
        checkInputs(inputDeviceManager);
        findBestMode();
    });

    connect(inputDeviceManager, &QInputInfoManager::deviceRemoved,[this, inputDeviceManager](QString)
    {
        checkInputs(inputDeviceManager);
        findBestMode();
    });
    #elif defined QT6_BASE
    checkInputs(QInputDevice::devices());
    #endif
    findBestMode();
    #endif

}
