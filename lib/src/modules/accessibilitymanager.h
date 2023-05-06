#pragma once

#include <QObject>
#include "mauiman_export.h"

class QDBusInterface;
namespace MauiMan
{

class SettingsStore;

class MAUIMAN_EXPORT AccessibilityManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool singleClick READ singleClick WRITE setSingleClick NOTIFY singleClickChanged)
    Q_PROPERTY(uint scrollBarPolicy READ scrollBarPolicy WRITE setScrollBarPolicy NOTIFY scrollBarPolicyChanged)
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
