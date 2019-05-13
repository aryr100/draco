/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com>
# All rights reserved.
#
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
#
*/

#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QString>

class Manager : public QObject
{
    Q_OBJECT

public:
    explicit Manager(QObject *parent = NULL);

public slots:
    bool SetWakeAlarm(const QString &alarm);
    bool SetDisplayBacklight(const QString &device, int value);
    bool SetCpuGovernor(const QString &gov);
    bool SetCpuFrequency(const QString &freq);
};

#endif // MANAGER_H
