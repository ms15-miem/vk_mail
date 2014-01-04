#include "settingssaver.h"

#include <QCoreApplication>

SettingsManager::SettingsManager(QString settingsGroup)
    :settingsGroup(settingsGroup),
      cfg(new QSettings(QCoreApplication::applicationName()+".ini", QSettings::IniFormat))
{
    cfg->beginGroup(settingsGroup);
    loadSettings();
}

SettingsManager::~SettingsManager()
{
}

void SettingsManager::loadSettings()
{
}

void SettingsManager::saveSettings()
{
}
