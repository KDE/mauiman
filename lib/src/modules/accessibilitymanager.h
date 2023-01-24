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


public:

    struct DefaultValues
    {
        static inline const bool singleClick = false;
    };

    explicit AccessibilityManager(QObject *parent = nullptr);

    bool singleClick() const;

public slots:
    void setSingleClick(bool singleClick);

private slots:
    void onSingleClickChanged(bool singleClick);

private:
#if !defined Q_OS_ANDROID
    QDBusInterface *m_interface = nullptr;
#endif
    MauiMan::SettingsStore *m_settings;

    bool m_singleClick = AccessibilityManager::DefaultValues::singleClick;


    void sync(const QString &key, const QVariant &value);
    void setConnections();
    void loadSettings();

signals:

    void singleClickChanged(bool singleClick);
};

}
