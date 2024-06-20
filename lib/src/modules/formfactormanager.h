#pragma once

#include <QObject>
#include <QRect>
#include <QList>

#include "mauiman_export.h"

class QInputDevice;

class QDBusInterface;
namespace MauiMan
{
    class SettingsStore;

    /**
     * @brief The FormFactoInfo class contains information about the input devices available in the current system.
     */
    class MAUIMAN_EXPORT FormFactorInfo : public QObject
    {
        Q_OBJECT
        /**
         * The best fitted mode according to the available input devices and the screen size.
         */
        Q_PROPERTY(uint bestMode READ bestMode NOTIFY bestModeChanged FINAL)
        
        /**
         * The system preferred mode. This is picked up from the env var `QT_QUICK_CONTROLS_MOBILE`
         */
        Q_PROPERTY(uint defaultMode READ defaultMode CONSTANT FINAL)

        /**
         * Whether the device has a physical keyboard.
         */
        Q_PROPERTY(bool hasKeyboard READ hasKeyboard NOTIFY hasKeyboardChanged FINAL)
        
        /**
         * Whether the device has a touch screen.
         */
        Q_PROPERTY(bool hasTouchscreen READ hasTouchscreen NOTIFY hasTouchscreenChanged FINAL)
        
        /**
         * Whether the device has a physical mouse.
         */
        Q_PROPERTY(bool hasMouse READ hasMouse  NOTIFY hasMouseChanged FINAL)
        
        /**
         * Whether the device has a trackpad or touchpad 
         */
        Q_PROPERTY(bool hasTouchpad READ hasTouchpad NOTIFY hasTouchpadChanged)

        /**
         * The size of the main screen.
         */
        Q_PROPERTY(QRect screenSize READ screenSize NOTIFY screenSizeChanged)
        
        /**
         * The current orientation of the main screen.
         */
        Q_PROPERTY(Qt::ScreenOrientation screenOrientation READ screenOrientation NOTIFY screenOrientationChanged)

    public:
        /**
         * @brief The possible form factor modes the system can have based on the device capabilities.
         */
        enum Mode
        {
            /**
             * Is a desktop when the screen size if relative big, has a physical keyboard, mouse.
             */
            Desktop = 0,
            
            /**
             * Is a tablet when the devices has a relative big screen size, and it is a touch screen. There is not mouse present.
             */
            Tablet,
            
            /**
             * Is a mobile phone, the the screen size is small, has a touch screen and not peripheral input devices such as a a keyboard or mouse.
             */
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

        void checkInputs(const QList<const QInputDevice *> &devices);
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

    /**
     * @brief The FormFactorManager class exposes all the system form factor properties.
     */
    class MAUIMAN_EXPORT FormFactorManager : public FormFactorInfo
    {
        Q_OBJECT
        /**
         * The preferred mode to display information. The possible values are:
         * - 0 Desktop 
         * - 1 Tablet
         * - 2 Phone
         */
        Q_PROPERTY(uint preferredMode READ preferredMode WRITE setPreferredMode NOTIFY preferredModeChanged FINAL)

        /**
         * If a device is not detected to have a touch screen , and still it has it, this property can be used to force allow the touch screen interactions.
         */
        Q_PROPERTY(bool forceTouchScreen READ forceTouchScreen WRITE setForceTouchScreen NOTIFY forceTouchScreenChanged)


    public:
        explicit FormFactorManager(QObject *parent = nullptr);

        [[nodiscard]] uint preferredMode() const;
        void setPreferredMode(uint preferredMode);        

        [[nodiscard]] bool forceTouchScreen() const;
        void setForceTouchScreen(bool newForceTouchScreen);

    private Q_SLOTS:
        void onPreferredModeChanged(uint preferredMode);
        void onForceTouchScreenChanged(bool value);

    private:
        #if !defined Q_OS_ANDROID
        QDBusInterface *m_interface = nullptr;
        #endif
        MauiMan::SettingsStore *m_settings;
        FormFactorInfo *m_info;

        uint m_preferredMode;
        bool m_forceTouchScreen = false;

        void sync(const QString &key, const QVariant &value);
        void setConnections();
        void loadSettings();

    Q_SIGNALS:
        void preferredModeChanged(uint preferredMode);
        void forceTouchScreenChanged(bool forceTouchScreen);
    };
}
