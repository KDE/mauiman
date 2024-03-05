#pragma once

#include <QObject>

#include "modules/formfactormanager.h"

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
    Q_PROPERTY(bool forceTouchScreen READ forceTouchScreen WRITE setForceTouchScreen NOTIFY forceTouchScreenChanged)

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
    void setPreferredMode(uint preferredMode);

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

    
    bool forceTouchScreen() const;

    void setForceTouchScreen(bool newForceTouchScreen);

private Q_SLOTS:
    void setBestMode(uint bestMode);
    void setHasKeyboard(bool value);
    void setHasMouse(bool value);
    void setHasTouchpad(bool value);
    void setHasTouchscreen(bool value);

private:
    MauiMan::FormFactorInfo *m_manager;
    uint m_preferredMode = MauiMan::FormFactorManager::DefaultValues::defaultMode;

    uint m_bestMode = MauiMan::FormFactorManager::DefaultValues::defaultMode;

    uint m_defaultMode = MauiMan::FormFactorManager::DefaultValues::defaultMode;

    bool m_hasKeyboard = false;

    bool m_hasTouchscreen = false;

    bool m_hasMouse = false;

    bool m_hasTouchpad= false;

    bool hasTouchScreen() const;

    bool m_forceTouchScreen = false;

Q_SIGNALS:
    void preferredModeChanged(uint preferredMode);
    void bestModeChanged(uint bestMode);
    void defaultModeChanged(uint defaultMode);
    void hasKeyboardChanged(bool hasKeyboard);
    void hasTouchscreenChanged(bool hasTouchscreen);
    void hasMouseChanged(bool hasMouse);
    void hasTouchpadChanged(bool hasTouchpad);
    void forceTouchScreenChanged(bool forceTouchScreen);
};

