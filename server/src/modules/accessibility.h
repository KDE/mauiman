#pragma once

#include <QObject>
#include "modules/accessibilitymanager.h"

class Accessibility : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool singleClick READ singleClick WRITE setSingleClick NOTIFY singleClickChanged)


public:
    explicit Accessibility(QObject *parent = nullptr);

    bool singleClick() const;
    void setSingleClick(bool singleClick);

private:
    bool m_singleClick = MauiMan::AccessibilityManager::DefaultValues::singleClick;


Q_SIGNALS:
    void singleClickChanged(bool singleClick);
};

