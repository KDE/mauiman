#pragma once

#include <QObject>
#include <QRect>
#include <QList>

#include "mauiman_export.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define QT5_BASE
#include <QTouchDevice>
class QInputInfoManager;
#else
#define QT6_BASE
class QInputDevice;
#endif

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
        [[nodiscard]] static uint getDefaultMode()
        {

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS) || defined(UBUNTU_TOUCH)
            return MauiMan::FormFactorInfo::Mode::Phone;
#else

            return QByteArrayList{"1", "true"}.contains(qgetenv("QT_QUICK_CONTROLS_MOBILE")) ? MauiMan::FormFactorInfo::Mode::Phone : MauiMan::FormFactorInfo::Mode::Desktop;
#endif
        }

        [[nodiscard]] static bool getHasTouchScreen()
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
    
    [[nodiscard]] uint bestMode() const;

    [[nodiscard]] uint defaultMode() const;

    [[nodiscard]] bool hasKeyboard() const;

    [[nodiscard]] bool hasTouchscreen() const;

    [[nodiscard]] bool hasMouse() const;

    [[nodiscard]] bool hasTouchpad() const;

    [[nodiscard]] QRect screenSize();
    [[nodiscard]] Qt::ScreenOrientation screenOrientation();

private:
    uint m_bestMode = FormFactorInfo::DefaultValues::defaultMode;

    uint m_defaultMode = FormFactorInfo::DefaultValues::defaultMode;

    bool m_hasKeyboard = true;

    bool m_hasTouchscreen = FormFactorInfo::DefaultValues::hasTouchscreen;

    bool m_hasMouse = true;
    bool m_hasTouchpad = true;
        
    QRect m_screenSize;
    Qt::ScreenOrientation m_screenOrientation;

#ifdef QT5_BASE
    void checkInputs(const QInputInfoManager *inputManager);
#elif defined QT6_BASE
    void checkInputs(const QList<const QInputDevice *> &devices);
#endif
    void findBestMode();
    
Q_SIGNALS:
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
    explicit FormFactorManager(QObject *parent = nullptr);

    [[nodiscard]] uint preferredMode() const;
    void setPreferredMode(uint preferredMode);

private Q_SLOTS:
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

Q_SIGNALS:
    void preferredModeChanged(uint preferredMode);

};
}
