#include "formfactor.h"
#include "formfactoradaptor.h"
#include <QDBusInterface>
#include <QMap>

#include <QScreen>
#include <QGuiApplication>

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
    #if defined(Q_OS_ANDROID) || defined(Q_OS_IOS) || defined(UBUNTU_TOUCH)
    m_defaultMode = MauiMan::FormFactorManager::Mode::Phone;
#endif

    m_preferredMode = m_defaultMode;

    auto inputDeviceManager = new QInputInfoManager(this);
    connect(inputDeviceManager, &QInputInfoManager::ready,[this, inputDeviceManager]()
    {
        inputDeviceManager->setFilter(QInputDevice::Mouse | QInputDevice::Keyboard | QInputDevice::TouchScreen | QInputDevice::TouchPad);
    });

    connect(inputDeviceManager, &QInputInfoManager::filterChanged,[this, inputDeviceManager](QInputDevice::InputTypeFlags )
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


    //** Ask for screen sizes and dimension etc to Cask via CaskServer**//

//    auto dummyApp = new QGuiApplication(0, []);
//    QScreen *screen = QGuiApplication::primaryScreen();
//    connect(screen, &QScreen::geometryChanged, [this](QRect rect)
//    {
//        m_screenSize = rect;
//        Q_EMIT screenSizeChanged(m_screenSize);

//        findBestMode();
//    });

//    connect(screen, &QScreen::primaryOrientationChanged, [this](Qt::ScreenOrientation orientation)
//    {
//        m_screenOrientation = orientation;
//        Q_EMIT screenOrientationChanged(m_screenOrientation);

//        findBestMode();
//    });

//    m_screenSize = screen->geometry();
//    m_screenOrientation = screen->primaryOrientation();

    findBestMode();

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

bool FormFactor::hasTouchpad() const
{
    return m_hasTouchpad;
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


bool FormFactor::hasTouchScreen() const
{
    return m_hasTouchscreen;
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
}

void FormFactor::findBestMode()
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

QRect FormFactor::screenSize()
{
    QScreen *screen = qApp->primaryScreen();
    return screen->geometry();
}

Qt::ScreenOrientation FormFactor::screenOrientation()
{
    QScreen *screen = qApp->primaryScreen();
    return screen->orientation();
}
