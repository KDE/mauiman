#pragma once

#include <QObject>
#include "modules/accessibilitymanager.h"

class Accessibility : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool singleClick READ singleClick WRITE setSingleClick NOTIFY singleClickChanged)
    Q_PROPERTY(uint scrollBarPolicy READ scrollBarPolicy WRITE setScrollBarPolicy NOTIFY scrollBarPolicyChanged)
    Q_PROPERTY(bool playSounds READ playSounds WRITE setPlaySounds NOTIFY playSoundsChanged)

public:
    explicit Accessibility(QObject *parent = nullptr);

    bool singleClick() const;
    void setSingleClick(bool singleClick);

    /**
     * @brief scrollBarPolicy
     * The policy values are:
     * 0- AlwaysVisible
     * 1- AsNeeded
     * 2- Hidden
     * 3- AutoHide
     * @return
     */
    uint scrollBarPolicy() const;
    void setScrollBarPolicy(uint newScrollBarPolicy);

    bool playSounds() const;
    void setPlaySounds(bool newPlaySounds);

private:
    bool m_singleClick = MauiMan::AccessibilityManager::DefaultValues::singleClick;
    uint m_scrollBarPolicy = MauiMan::AccessibilityManager::DefaultValues::scrollBarPolicy;
    bool m_playSounds = MauiMan::AccessibilityManager::DefaultValues::playSounds;

Q_SIGNALS:
    void singleClickChanged(bool);
    void scrollBarPolicyChanged(uint);
    void playSoundsChanged(bool);
};

