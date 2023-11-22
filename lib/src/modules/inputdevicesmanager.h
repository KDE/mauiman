#pragma once
#include <QObject>
#include "mauiman_export.h"

class QDBusInterface;
namespace MauiMan
{
    class SettingsStore;
    
    /**
     * @brief The InputDevicesManager class exposes all the system input devices properties
     */
    class MAUIMAN_EXPORT InputDevicesManager : public QObject
    {
        Q_OBJECT
        /**
         * The preferred keyboard language layout.
         */
        Q_PROPERTY(QString keyboardLayout READ keyboardLayout WRITE setKeyboardLayout NOTIFY keyboardLayoutChanged)
        
        /**
         * The preferred keyboard model
         */
        Q_PROPERTY(QString keyboardModel READ keyboardModel WRITE setKeyboardModel NOTIFY keyboardModelChanged)
        
        /**
         * The preferred keyboard layout variant.
         */
        Q_PROPERTY(QString keyboardVariant READ keyboardVariant WRITE setKeyboardVariant NOTIFY keyboardVariantChanged)
        
        /**
         * The keyboard layout options.
         */
        Q_PROPERTY(QString keyboardOptions READ keyboardOptions WRITE setKeyboardOptions NOTIFY keyboardOptionsChanged)
        
        /**
         * The keyboard layout custom rules.
         */
        Q_PROPERTY(QString keyboardRules READ keyboardRules WRITE setKeyboardRules NOTIFY keyboardRulesChanged)
        
    public:
        
        /**
         * The InputDevices module default values.
         */
        struct DefaultValues
        {
            static inline const QString keyboardLayout =  QStringLiteral("us");
            static inline const QString keyboardModel =  QStringLiteral("");
            static inline const QString keyboardOptions =  QStringLiteral("grp:shifts_toggle,compose:ralt,ctrl:nocaps");
            static inline const QString keyboardVariant =  QStringLiteral("");
            static inline const QString keyboardRules =  QStringLiteral("");
        };
        explicit InputDevicesManager(QObject *parent = nullptr);
        
        QString keyboardLayout() const;
        void setKeyboardLayout(QString keyboardLayout);
        
        QString keyboardModel() const;
        void setKeyboardModel(QString keyboardModel);
        
        QString keyboardVariant() const;
        void setKeyboardVariant(QString keyboardVariant);
        
        QString keyboardOptions() const;
        void setKeyboardOptions(QString keyboardOptions);
        
        QString keyboardRules() const;
        void setKeyboardRules(QString keyboardRules);
        
    private Q_SLOTS:
        void onKeyboardLayoutChanged(const QString &keyboardLayout);
        void onKeyboardModelChanged(const QString &keyboardModel);
        void onKeyboardOptionsChanged(const QString &keyboardOptions);
        void onKeyboardRulesChanged(const QString &keyboardRules);
        void onKeyboardVariantChanged (const QString &keyboardVariant);
        
    private:
        #if !defined Q_OS_ANDROID
        QDBusInterface *m_interface = nullptr;
        #endif
        MauiMan::SettingsStore *m_settings;
        
        void sync(const QString &key, const QVariant &value);
        void setConnections();
        void loadSettings();
        
        QString m_keyboardLayout = InputDevicesManager::DefaultValues::keyboardLayout;
        QString m_keyboardModel = InputDevicesManager::DefaultValues::keyboardModel;
        QString m_keyboardVariant = InputDevicesManager::DefaultValues::keyboardVariant;
        QString m_keyboardOptions = InputDevicesManager::DefaultValues::keyboardOptions;
        QString m_keyboardRules = InputDevicesManager::DefaultValues::keyboardRules;
        
    Q_SIGNALS:
        void keyboardLayoutChanged(QString keyboardLayout);
        void keyboardModelChanged(QString keyboardModel);
        void keyboardVariantChanged(QString keyboardVariant);
        void keyboardOptionsChanged(QString keyboardOptions);
        void keyboardRulesChanged(QString keyboardRules);
    };
}
