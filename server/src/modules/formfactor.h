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

    Q_PROPERTY(bool hasKeyboard READ hasKeyboard WRITE setHasKeyboard NOTIFY hasKeyboardChanged)
    Q_PROPERTY(bool hasTouchscreen READ hasTouchscreen WRITE setHasTouchscreen NOTIFY hasTouchscreenChanged)
    Q_PROPERTY(bool hasMouse READ hasMouse WRITE setHasMouse NOTIFY hasMouseChanged)


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

public slots:
    void setPreferredMode(uint preferredMode);

    void setBestMode(uint bestMode);

    void setDefaultMode(uint defaultMode);

    void setHasKeyboard(bool hasKeyboard);

    void setHasTouchscreen(bool hasTouchscreen);

    void setHasMouse(bool hasMouse);

private:
    uint m_preferredMode = MauiMan::FormFactorManager::DefaultValues::preferredMode;

    uint m_bestMode = 0;

    uint m_defaultMode = 0;

    bool m_hasKeyboard = false;

    bool m_hasTouchscreen = false;

    bool m_hasMouse = false;

    bool hasTouchScreen() const;
    void checkInputs(const QInputInfoManager *inputManager);

    QRect screenSize();
    Qt::Orientation screenOrientation();

signals:

    void preferredModeChanged(uint preferredMode);
    void bestModeChanged(uint bestMode);
    void defaultModeChanged(uint defaultMode);
    void hasKeyboardChanged(bool hasKeyboard);
    void hasTouchscreenChanged(bool hasTouchscreen);
    void hasMouseChanged(bool hasMouse);
};

