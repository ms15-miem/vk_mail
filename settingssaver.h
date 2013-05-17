#ifndef SETTINGSSAVER_H
#define SETTINGSSAVER_H

#include <QSettings>
#include <QCoreApplication>

class SettingsManager
{
public:
    SettingsManager(QString settingsGroup);
    virtual ~SettingsManager();

private:
    const QString settingsGroup;
protected:
    QSettings *cfg;
    virtual void saveAuthData() const;
    virtual void loadAuthData();
};

#endif // SETTINGSSAVER_H
