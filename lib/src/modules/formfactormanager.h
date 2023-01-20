#pragma once

#include <QObject>

#include "mauiman_export.h"

class QDBusInterface;
namespace MauiMan
{
class SettingsStore;
class MAUIMAN_EXPORT FormFactorManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint preferredMode READ preferredMode WRITE setPreferredMode NOTIFY preferredModeChanged FINAL)
    Q_PROPERTY(uint bestMode READ bestMode NOTIFY bestModeChanged FINAL)
    Q_PROPERTY(uint defaultMode READ defaultMode CONSTANT FINAL)

    Q_PROPERTY(bool hasKeyboard READ hasKeyboard WRITE setHasKeyboard NOTIFY hasKeyboardChanged FINAL)
    Q_PROPERTY(bool hasTouchscreen READ hasTouchscreen WRITE setHasTouchscreen NOTIFY hasTouchscreenChanged FINAL)
    Q_PROPERTY(bool hasMouse READ hasMouse WRITE setHasMouse NOTIFY hasMouseChanged FINAL)

public:
    enum Mode
    {
        Desktop = 0,
        Tablet,
        Phone
    }; Q_ENUM(Mode)

    struct DefaultValues
    {
        static inline const uint preferredMode = Mode::Desktop;
    } ;

    explicit FormFactorManager(QObject *parent = nullptr);

    uint preferredMode() const;

    uint bestMode() const;

    uint defaultMode() const;

    bool hasKeyboard() const;

    bool hasTouchscreen() const;

    bool hasMouse() const;

public slots:
    void setPreferredMode(uint preferredMode);

    void setHasKeyboard(bool hasKeyboard);

    void setHasTouchscreen(bool hasTouchscreen);

    void setHasMouse(bool hasMouse);

private slots:
    void onPreferredModeChanged(uint preferredMode);

private:
#if !defined Q_OS_ANDROID
    QDBusInterface *m_interface = nullptr;
#endif
    MauiMan::SettingsStore *m_settings;

    uint m_preferredMode = FormFactorManager::DefaultValues::preferredMode;

    uint m_bestMode;

    uint m_defaultMode;

    bool m_hasKeyboard;

    bool m_hasTouchscreen;

    bool m_hasMouse;

    bool m_serverRunning = false;

    void sync(const QString &key, const QVariant &value);
    void setConnections();
    void loadSettings();

signals:

    void preferredModeChanged(uint preferredMode);
    void bestModeChanged(uint bestMode);
    void hasKeyboardChanged(bool hasKeyboard);
    void hasTouchscreenChanged(bool hasTouchscreen);
    void hasMouseChanged(bool hasMouse);
};
}
