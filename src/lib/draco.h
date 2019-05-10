/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
# Available under the 3-clause BSD license
# See the LICENSE file for full details
#
*/

#ifndef DRACO_H
#define DRACO_H

#include <QObject>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QDirIterator>
#include <QApplication>
#include <QVariant>
#include <QFont>

#define DE_SESSION_SETTINGS "sessionsettings"
#define DE_DESKTOP_SETTINGS "desktopsettings"
#define DE_PLUGIN_SETTINGS "pluginsettings"
#define DE_ENV_SETTINGS "envsettings"

class Draco : public QObject
{
    Q_OBJECT

public:
    Draco();
    virtual ~Draco();

    static const QString desktopSessionName();
    static const QString desktopSessionPath();
    static const QString desktopSessionFullPath();
    static const QString powerdSessionName();
    static const QString powerdSessionPath();
    static const QString powerdSessionFullPath();
    static const QString powerSessionName();
    static const QString powerSessionPath();
    static const QString powerSessionFullPath();
    static const QString xconfig();
    static const QString storageApp();
    static const QString powerApp();
    static const QString desktopApp();
    static const QString launcherApp();
    static const QString terminalApp();
    static const QString configDir();
    static const QString sessionSettingsFile();
    static const QString desktopSettingsFile();
    static const QString envSettingsFile();
    static const QString powerSettingsFile();
    static const QString storageSettingsFile();
    static const QString xconfigSettingsFile();
    static const QString keyboardSettingsFile();
    static const QString themeSettingsFile();
    static QStringList iconLocations(const QString &appPath);
    static QStringList pixmapLocations(const QString &appPath);
    static QStringList applicationLocations(const QString &appPath);
    static const QString xdgConfigHome();
    static const QString xdgDataHome();
    static bool xdgOpenCheck();
    static void xdgMimeCheck();
    static const QString windowManager();
    static const QString windowManagerConf();
    static const QString windowManagerCmdStart();
    static const QString windowManagerCmdReConf();
    static const QString dracoStyleConf();
    static const QString panelStyleSheet();
    static const QString desktopStyleSheet();
    static void themeEngineCheckConf(const QString &theme = QString());
    static void checkGtk2Conf(const QString &theme = QString(), QFont font = QFont());
    static void checkGtk3Conf(const QString &theme = QString(), QFont font = QFont());
    static void checkConfigs();
    static const QString getProperMime(const QString &mime);
    static const QString filterIconName(const QString &name);
    static QVariant readSetting(QString conf,
                                QString key,
                                QVariant fallback = QVariant());
    static void writeSetting(QString conf,
                             QString key,
                             QVariant value);
    static bool isBlacklistedApplication(const QString &exec);
    static const QString getOSReleaseInfo(const QString &type);
    static bool kernelCanResume();
};

#endif // DRACO_H
