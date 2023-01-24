#pragma once

#include <QObject>
#include <QRect>

#include "modules/formfactormanager.h"

class QInputInfoManager;
class FormFactor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint preferredMode READ preferredMode WRITE setPreferredMode NOTIFY preferredModeChanged)
    Q_PROPERTY(uint bestMode READ bestMode NOTIFY bestModeChanged)
    Q_PROPERTY(uint defaultMode READ defaultMode CONSTANT)

    Q_PROPERTY(bool hasKeyboard READ hasKeyboard NOTIFY hasKeyboardChanged)
    Q_PROPERTY(bool hasTouchscreen READ hasTouchscreen NOTIFY hasTouchscreenChanged)
    Q_PROPERTY(bool hasMouse READ hasMouse NOTIFY hasMouseChanged)
    Q_PROPERTY(bool hasTouchpad READ hasTouchpad NOTIFY hasTouchpadChanged)

    Q_PROPERTY(QRect screenSize READ screenSize NOTIFY screenSizeChanged)
    Q_PROPERTY(Qt::ScreenOrientation screenOrientation READ screenOrientation NOTIFY screenOrientationChanged)

public:
    explicit FormFactor(QObject *parent = nullptr);

    /**
     * @brief preferredMode
     * Mode picked manually by the user as the preffered one over the default and best fit modes.
     * 0 - Desktop
     * 1 - Tablet
     * 2 - Phone
     * @return
     */
    uint preferredMode() const;

    /**
     * @brief bestMode
     * The result of computing different variables such as inputs, architecture, and screensize resulting in what mode is the best fit.
     * @return
     */
    uint bestMode() const;

    /**
     * @brief defaultMode
     * @return
     */
    uint defaultMode() const;

    bool hasKeyboard() const;

    bool hasTouchscreen() const;

    bool hasMouse() const;

    bool hasTouchpad() const;

public slots:
    void setPreferredMode(uint preferredMode);

    void setBestMode(uint bestMode);

    void setDefaultMode(uint defaultMode);
    QRect screenSize();
    Qt::ScreenOrientation screenOrientation();

private:
    uint m_preferredMode = MauiMan::FormFactorManager::DefaultValues::preferredMode;

    uint m_bestMode = 0;

    uint m_defaultMode = 0;

    bool m_hasKeyboard = false;

    bool m_hasTouchscreen = false;

    bool m_hasMouse = false;

    bool m_hasTouchpad= false;

    bool hasTouchScreen() const;

    QRect m_screenSize;
    Qt::ScreenOrientation m_screenOrientation;

    void checkInputs(const QInputInfoManager *inputManager);

    void findBestMode();

signals:

    void preferredModeChanged(uint preferredMode);
    void bestModeChanged(uint bestMode);
    void defaultModeChanged(uint defaultMode);
    void hasKeyboardChanged(bool hasKeyboard);
    void hasTouchscreenChanged(bool hasTouchscreen);
    void hasMouseChanged(bool hasMouse);
    void hasTouchpadChanged(bool hasTouchpad);
    void screenSizeChanged(QRect screenSize);
    void screenOrientationChanged(Qt::ScreenOrientation screenOrientation);
};

