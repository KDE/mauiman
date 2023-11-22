#pragma once

#include <QObject>
#include "mauiman_export.h"

class QDBusInterface;
namespace MauiMan
{

class SettingsStore;

/**
 * @brief The AccessibilityManager class contains properties for changing visual and hearing clues.
 */
class MAUIMAN_EXPORT AccessibilityManager : public QObject
{
    Q_OBJECT
    /**
     * Whether to open items with a single click.
     * By default this is set to `true` for mobile, and `false` for desktop.
     */
    Q_PROPERTY(bool singleClick READ singleClick WRITE setSingleClick NOTIFY singleClickChanged)
    
    /**
     * The policy for showing the scroll bars. The possible values are:
     * - 0 Always visible
     * - 1 Visible when needed
     * - 2 Auto Hide
     * - 3 Always hidden
     */
    Q_PROPERTY(uint scrollBarPolicy READ scrollBarPolicy WRITE setScrollBarPolicy NOTIFY scrollBarPolicyChanged)
    
    /**
     * Whether the user prefers the application to emit notification or alarm sounds.
     */
    Q_PROPERTY(bool playSounds READ playSounds WRITE setPlaySounds NOTIFY playSoundsChanged)

public:

    struct DefaultValues
    {
        static bool isMobile()
        {
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS) || defined(UBUNTU_TOUCH)
            return true;
#else
            return QByteArrayList{"1", "true"}.contains(qgetenv("QT_QUICK_CONTROLS_MOBILE"));
#endif
        }
        static inline const bool singleClick = DefaultValues::isMobile();
        static inline const bool playSounds = true;
        static inline const uint scrollBarPolicy = DefaultValues::isMobile() ? 3 : 0;
    };

    explicit AccessibilityManager(QObject *parent = nullptr);

    bool singleClick() const;
    void setSingleClick(bool singleClick);

    uint scrollBarPolicy() const;
    void setScrollBarPolicy(uint newScrollBarPolicy);

    bool playSounds() const;
    void setPlaySounds(bool newPlaySounds);

private Q_SLOTS:
    void onSingleClickChanged(bool singleClick);
    void onScrollBarPolicyChanged(uint scrollBarPolicy);
    void onPlaySoundsChanged(bool playSounds);

private:
#if !defined Q_OS_ANDROID
    QDBusInterface *m_interface = nullptr;
#endif
    MauiMan::SettingsStore *m_settings;

    bool m_singleClick = AccessibilityManager::DefaultValues::singleClick;
    uint m_scrollBarPolicy = AccessibilityManager::DefaultValues::scrollBarPolicy;
    bool m_playSounds = AccessibilityManager::DefaultValues::playSounds;

    void sync(const QString &key, const QVariant &value);
    void setConnections();
    void loadSettings();

Q_SIGNALS:
    void singleClickChanged(bool);
    void scrollBarPolicyChanged(uint);
    void playSoundsChanged(bool);
};

}
