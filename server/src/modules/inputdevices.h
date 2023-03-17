#pragma once

#include <QObject>
#include <QString>

#include "modules/inputdevicesmanager.h"

class InputDevices : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString keyboardLayout READ keyboardLayout WRITE setKeyboardLayout NOTIFY keyboardLayoutChanged)
    Q_PROPERTY(QString keyboardModel READ keyboardModel WRITE setKeyboardModel NOTIFY keyboardModelChanged)
    Q_PROPERTY(QString keyboardVariant READ keyboardVariant WRITE setKeyboardVariant NOTIFY keyboardVariantChanged)
    Q_PROPERTY(QString keyboardOptions READ keyboardOptions WRITE setKeyboardOptions NOTIFY keyboardOptionsChanged)
    Q_PROPERTY(QString keyboardRules READ keyboardRules WRITE setKeyboardRules NOTIFY keyboardRulesChanged)

public:
    explicit InputDevices(QObject *parent = nullptr);

    QString keyboardLayout() const;
    void setKeyboardLayout(const QString &keyboardLayout);

    QString keyboardModel() const;
    void setKeyboardModel(const QString &keyboardModel);

    QString keyboardVariant() const;
    void setKeyboardVariant(const QString &keyboardVariant);

    QString keyboardOptions() const;
    void setKeyboardOptions(const QString &keyboardOptions);

    QString keyboardRules() const;
    void setKeyboardRules(const QString &keyboardRules);

private:
    QString m_keyboardLayout = MauiMan::InputDevicesManager::DefaultValues::keyboardLayout;
    QString m_keyboardModel = MauiMan::InputDevicesManager::DefaultValues::keyboardModel;
    QString m_keyboardVariant = MauiMan::InputDevicesManager::DefaultValues::keyboardVariant;
    QString m_keyboardOptions = MauiMan::InputDevicesManager::DefaultValues::keyboardOptions;
    QString m_keyboardRules = MauiMan::InputDevicesManager::DefaultValues::keyboardRules;

Q_SIGNALS:
    void keyboardLayoutChanged(QString keyboardLayout);
    void keyboardModelChanged(QString keyboardModel);
    void keyboardVariantChanged(QString keyboardVariant);
    void keyboardOptionsChanged(QString keyboardOptions);
    void keyboardRulesChanged(QString keyboardRules);
};

