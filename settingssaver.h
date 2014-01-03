#ifndef SETTINGSSAVER_H
#define SETTINGSSAVER_H

#include <QSettings>
#include <QCoreApplication>

class SettingsManager
{
public:
    SettingsManager(QString settingsGroup);
    ~SettingsManager();
private:
    const QString settingsGroup;
protected:
    QSettings *cfg;
    void loadSettings();
};

#endif // SETTINGSSAVER_H
