#include "settingssaver.h"

SettingsManager::SettingsManager(QString settingsGroup)
    :settingsGroup(settingsGroup)
{
    cfg = new QSettings(QCoreApplication::applicationName()+".ini", QSettings::IniFormat);
    cfg->beginGroup(settingsGroup);
}

SettingsManager::~SettingsManager()
{
    delete cfg;
}

void SettingsManager::loadAuthData()
{
}

void SettingsManager::saveAuthData() const
{
}
