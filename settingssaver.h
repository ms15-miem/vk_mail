#ifndef SETTINGSSAVER_H
#define SETTINGSSAVER_H

#include <QSettings>
#include <memory>

class SettingsManager
{
public:
    SettingsManager(QString settingsGroup);
    ~SettingsManager();

private:
    const QString settingsGroup;

protected:
    void loadSettings();
    void saveSettings();
    std::unique_ptr<QSettings> cfg;
};

#endif // SETTINGSSAVER_H
