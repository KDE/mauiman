#pragma once
#include <QObject>
#include <QString>
#include <QVariant>
#include "mauiman_export.h"

class QSettings;

namespace MauiMan
{

/**
 * @brief The SettingsStore class
 * Allows to store and read settings for MauiMan from the local conf file.
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
     * @brief Load the value of a conf entry, with a possible default value
     * @param key the key name of the value
     * @param defaultValue the default fallback value in case the value with the given key does not exists
     * @return
     */
    QVariant load(const QString &key, const QVariant &defaultValue);

    /**
     * @brief Save a conf value entry to the local file
     * @param key the key name of the value
     * @param value the entry value
     */
    void save(const QString &key, const QVariant &value);

    /**
     * @brief Set up the module section to write to
     * @param module the module name
     */
    void beginModule(const QString &module);

    /**
     * @brief Finish writing or reading from a module section.
     */
    void endModule();

private:
    QSettings *m_settings;
};
}

