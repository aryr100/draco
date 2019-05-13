/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
#
*/

#ifndef SCREENS_H
#define SCREENS_H

#include <QMap>
#include <QString>

#include <X11/extensions/Xrandr.h>

class Screens
{
public:
    static QMap<QString,bool> outputsDpy(Display *dpy);
    static QMap<QString,bool> outputs();
    static QString internalDpy(Display *dpy);
    static QString internal();
};

#endif // SCREENS_H
