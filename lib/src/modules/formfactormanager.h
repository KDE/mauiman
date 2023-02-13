#pragma once

#include <QObject>
#include <QRect>

#include "mauiman_export.h"

class QInputInfoManager;
class QDBusInterface;
namespace MauiMan
{
class SettingsStore;

class MAUIMAN_EXPORT FormFactorInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint bestMode READ bestMode NOTIFY bestModeChanged FINAL)
    Q_PROPERTY(uint defaultMode READ defaultMode CONSTANT FINAL)

    Q_PROPERTY(bool hasKeyboard READ hasKeyboard NOTIFY hasKeyboardChanged FINAL)
    Q_PROPERTY(bool hasTouchscreen READ hasTouchscreen NOTIFY hasTouchscreenChanged FINAL)
    Q_PROPERTY(bool hasMouse READ hasMouse  NOTIFY hasMouseChanged FINAL)
    Q_PROPERTY(bool hasTouchpad READ hasTouchpad NOTIFY hasTouchpadChanged)

    Q_PROPERTY(QRect screenSize READ screenSize NOTIFY screenSizeChanged)
    Q_PROPERTY(Qt::ScreenOrientation screenOrientation READ screenOrientation NOTIFY screenOrientationChanged)
    
public:
    enum Mode
    {
        Desktop = 0,
        Tablet,
        Phone
    };

    struct DefaultValues
    {
        static uint getDefaultMode()
        {

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS) || defined(UBUNTU_TOUCH)
return MauiMan::FormFactorInfo::Mode::Phone;
#else

            return QByteArrayList{"1", "true"}.contains(qgetenv("QT_QUICK_CONTROLS_MOBILE")) ? MauiMan::FormFactorInfo::Mode::Phone : MauiMan::FormFactorInfo::Mode::Desktop;
            #endif
        }

        static bool getHasTouchScreen()
        {

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS) || defined(UBUNTU_TOUCH)
return true;
#else

            return false;
            #endif
        }

        static inline const uint defaultMode = DefaultValues::getDefaultMode();

        static inline const bool hasTouchscreen = DefaultValues::getHasTouchScreen();
    } ;

    explicit FormFactorInfo(QObject *parent);
    
     uint bestMode() const;

    uint defaultMode() const;

    bool hasKeyboard() const;

    bool hasTouchscreen() const;

    bool hasMouse() const;

    bool hasTouchpad() const;    

    QRect screenSize();
    Qt::ScreenOrientation screenOrientation();

private:

    uint m_bestMode = FormFactorInfo::DefaultValues::defaultMode;

    uint m_defaultMode = FormFactorInfo::DefaultValues::defaultMode;

     bool m_hasKeyboard = true;

    bool m_hasTouchscreen = FormFactorInfo::DefaultValues::hasTouchscreen;

    bool m_hasMouse = true;
    bool m_hasTouchpad = true;
    
    
    QRect m_screenSize;
    Qt::ScreenOrientation m_screenOrientation;


    void checkInputs(const QInputInfoManager *inputManager);

    void findBestMode();

    
signals:
    void bestModeChanged(uint bestMode);
    void defaultModeChanged(uint defaultMode);

    void hasKeyboardChanged(bool hasKeyboard);
    void hasTouchscreenChanged(bool hasTouchscreen);
    void hasMouseChanged(bool hasMouse);
    void hasTouchpadChanged(bool hasTouchpad);    
    
    void screenSizeChanged(QRect screenSize);
    void screenOrientationChanged(Qt::ScreenOrientation screenOrientation);
};


class MAUIMAN_EXPORT FormFactorManager : public FormFactorInfo
{
    Q_OBJECT
    Q_PROPERTY(uint preferredMode READ preferredMode WRITE setPreferredMode NOTIFY preferredModeChanged FINAL)

public:
//    Q_ENUM(FormFactorInfo::Mode)

    explicit FormFactorManager(QObject *parent = nullptr);

    uint preferredMode() const;

public slots:
    void setPreferredMode(uint preferredMode);

private slots:
    void onPreferredModeChanged(uint preferredMode);

private:
#if !defined Q_OS_ANDROID
    QDBusInterface *m_interface = nullptr;
#endif
    MauiMan::SettingsStore *m_settings;
    FormFactorInfo *m_info;

    uint m_preferredMode;

    void sync(const QString &key, const QVariant &value);
    void setConnections();
    void loadSettings();

signals:
    void preferredModeChanged(uint preferredMode);

};
}
