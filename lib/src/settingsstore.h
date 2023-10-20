#pragma once
#include <QObject>
#include <QString>
#include <QVariant>
#include "mauiman_export.h"

class QSettings;

/**
 *
 */
namespace MauiMan
{

/**
 * @brief The SettingsStore class
 * Allows to store and read settings from MauiMan.
 */
class MAUIMAN_EXPORT SettingsStore : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief SettingsStore
     * @param parent
     */
    explicit SettingsStore(QObject *parent = nullptr);
    ~SettingsStore();

    /**
     * @brief load
     * @param key
     * @param defaultValue
     * @return
     */
    QVariant load(const QString &key, const QVariant &defaultValue);

    /**
     * @brief save
     * @param key
     * @param value
     */
    void save(const QString &key, const QVariant &value);

    /**
     * @brief beginModule
     * @param module
     */
    void beginModule(const QString &module);

    /**
     * @brief endModule
     */
    void endModule();

private:
    QSettings *m_settings;
};
}

