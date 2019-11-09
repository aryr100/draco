/*
#
# Draco Desktop Environment <https://dracolinux.org>
# Copyright (c) 2019, Ole-André Rodlie <ole.andre.rodlie@gmail.com>
# All rights reserved.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
#
# This file incorporates work covered by the following copyright and
# permission notice:
#
# Lumina Desktop Environment (https://lumina-desktop.org)
# Copyright (c) 2012-2017, Ken Moore (moorekou@gmail.com)
# All rights reserved
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the organization nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
*/

//===========================================
//  Lumina Desktop Source Code
//  Copyright (c) 2016, Ken Moore
//  Available under the 3-clause BSD license
//===========================================
#include "getPage.h"

//Add any sub-pages here
#include "page_main.h"
#include "page_wallpaper.h"

#include "page_autostart.h"
//#include "page_interface_desktop.h"
#include "page_interface_panels.h"
#include "page_session_options.h"
#include "page_keyboard.h"
#include "page_defaultapps.h"
//#include "page_power.h"
//#include "page_theme.h"

//#include "page_fluxbox_keys.h"
//#include "page_fluxbox_settings.h"
//#include "page_interface_menu.h"
//#include "page_session_locale.h"
//#include "page_compton.h"
//#include "page_soundtheme.h"
// #include "page_mouse_trueos.h"
// #include "page_bluetooth_trueos.h"




//Simplification function for creating a PAGEINFO structure
PAGEINFO Pages::PageInfo(QString ID, QString i_name, QString i_title, QString i_icon, QString i_comment, QString i_cat, QStringList i_sys, QStringList i_tags){
  PAGEINFO page;
  page.id = ID; page.name = i_name; page.title = i_title; page.icon = i_icon;
  page.comment = i_comment; page.category = i_cat; page.req_systems = i_sys;
  page.search_tags = i_tags;
  return page;
}

//List all the known pages
// **** Add new page entries here ****
QList<PAGEINFO> Pages::KnownPages(){
  // Valid Groups: ["appearance", "interface", "session", "user"]
  QList<PAGEINFO> list;
  //Reminder: <ID>, <name>, <title>, <icon>, <comment>, <category>, <server subsytem list>, <search tags>
  list << Pages::PageInfo("wallpaper",
                          QObject::tr("Wallpaper"),
                          QObject::tr("Wallpaper Settings"),
                          "preferences-desktop-wallpaper",
                          QObject::tr("Change background image(s)"),
                          "user",
                          QStringList(),
                          QStringList() << "background" << "wallpaper" << "color" << "image");
  list << Pages::PageInfo("autostart",
                          QObject::tr("Autostart"),
                          QObject::tr("Startup Settings"),
                          "applications-other",
                          QObject::tr("Automatically start applications or services"),
                          "user",
                          QStringList(),
                          QStringList() << "apps" << "autostart" << "services" << "xdg" << "startup" << "session");
  /*list << Pages::PageInfo("interface-desktop",
                          QObject::tr("Desktop"),
                          QObject::tr("Desktop Plugins"),
                          "preferences-desktop-icons",
                          QObject::tr("Change what icons or tools are embedded on the desktop"),
                          "appearance",
                          QStringList(),
                          QStringList() << "desktop" << "plugins" << "embed" << "icons" << "utilities");*/
  list << Pages::PageInfo("interface-panel",
                          QObject::tr("Panels"),
                          QObject::tr("Panels and Plugins"),
                          "preferences-desktop-display",
                          QObject::tr("Change any floating panels and what they show"),
                          "user",
                          QStringList(),
                          QStringList() << "desktop" << "toolbar" << "panel" << "floating" << "plugins");
  list << Pages::PageInfo("session-options",
                          QObject::tr("General"),
                          QObject::tr("General Settings"),
                          "preferences-system",
                          QObject::tr("Change basic user settings such as time/date formats"),
                          "user",
                          QStringList(),
                          QStringList() << "user"<<"settings"<<"time"<<"date"<<"icon"<<"reset"<<"numlock"<<"clock");
  list << Pages::PageInfo("keyboard",
                          QObject::tr("Keyboard"),
                          QObject::tr("User Settings"),
                          "preferences-desktop-keyboard",
                          QObject::tr("Change keyboard settings"),
                          "user",
                          QStringList(),
                          QStringList());
  /*list << Pages::PageInfo("power",
                          QObject::tr("Power"),
                          QObject::tr("User Settings"),
                          "ac-adapter",
                          QObject::tr("Change power settings"),
                          "user",
                          QStringList(),
                          QStringList());*/
  list << Pages::PageInfo("defaultapps",
                          QObject::tr("Applications"),
                          QObject::tr("Mimetype Settings"),
                          "preferences-desktop-default-applications",
                          QObject::tr("Change default applications"),
                          "user",
                          QStringList(),
                          QStringList() << "apps" << "default" << "services" << "xdg" << "session");

  //list << Pages::PageInfo(LUtils::AppToAbsolute("lthemeengine.desktop"), QObject::tr("Theme"), QObject::tr("Theme Settings"), "preferences-desktop-theme",QObject::tr("Change interface fonts and colors"), "appearance", QStringList(), QStringList() << "background" << "interface" << "color" << "theme" << "plugins");
  //list << Pages::PageInfo("compton", QObject::tr("Window Effects"), QObject::tr("Window Effects"), "window-duplicate",QObject::tr("Adjust transparency levels and window effects"), "appearance", QStringList(), QStringList() << "background" << "interface" << "color" << "transparency" << "windows" << "compositing");

  //list << Pages::PageInfo("fluxbox-keys", QObject::tr("Keyboard Shortcuts"), QObject::tr("Keyboard Shortcuts"), "preferences-desktop-keyboard",QObject::tr("Change keyboard shortcuts"), "session", QStringList(), QStringList() << "apps" << "fluxbox" << "keys" << "keyboard" << "session" << "launch");
  //list << Pages::PageInfo("fluxbox-settings", QObject::tr("Window Manager"), QObject::tr("Window Settings"), "preferences-system-windows-actions",QObject::tr("Change window settings and appearances"), "appearance", QStringList(), QStringList() << "window" << "frame" << "border" << "workspace" << "theme" << "fluxbox" << "session");
  //list << Pages::PageInfo("interface-menu", QObject::tr("Menu"), QObject::tr("Menu Plugins"), "format-list-unordered",QObject::tr("Change what options are shown on the desktop context menu"), "interface", QStringList(), QStringList() << "desktop" << "menu" << "plugins" << "shortcuts");
  //list << Pages::PageInfo("session-locale", QObject::tr("Localization"), QObject::tr("Locale Settings"), "preferences-desktop-locale",QObject::tr("Change the default locale settings for this user"), "user", QStringList(), QStringList() << "user"<<"locale"<<"language"<<"translations");
  //list << Pages::PageInfo("soundtheme", QObject::tr("Sound Themeing"), QObject::tr("Theme"), "media-playlist-audio",QObject::tr("Change basic sound settings"), "session", QStringList(), QStringList() << "session"<<"settings"<<"sound"<<"theme");
 // list << Pages::PageInfo("mouse-settings", QObject::tr("TrueOS Mouse Settings"), QObject::tr("TrueOS Mouse Settings"), "preferences-desktop-mouse",QObject::tr("Adjust mouse devices"), "user", QStringList(), QStringList() << "user"<<"speed"<<"accel"<<"mouse");
 // list << Pages::PageInfo("bluetooth-settings", QObject::tr("TrueOS Bluetooth Settings"), QObject::tr("TrueOS Bluetooth Settings"), "preferences-desktop-bluetooth",QObject::tr("Setup Bluetooth devices"), "user", QStringList(), QStringList() << "user"<<"bluetooth"<<"audio");

  return list;
}

PageWidget* Pages::GetNewPage(QString id, QWidget *parent){
  //Find the page that matches this "id"
  PageWidget* page = 0;
  if(id=="wallpaper"){ page = new page_wallpaper(parent); }
  else if(id=="autostart"){ page = new page_autostart(parent); }
  //else if(id=="interface-desktop"){ page = new page_interface_desktop(parent); }
  else if(id=="interface-panel"){ page = new page_interface_panels(parent); }
  else if(id=="session-options"){ page = new page_session_options(parent); }
  else if(id=="keyboard"){ page = new page_keyboard(parent); }
  //else if(id=="power"){ page = new page_power(parent); }
  else if(id=="defaultapps"){ page = new page_defaultapps(parent); }
  //else if(id=="theme"){ page = new page_theme(parent); }

  //else if(id=="fluxbox-keys"){ page = new page_fluxbox_keys(parent); }
  //else if(id=="fluxbox-settings"){ page = new page_fluxbox_settings(parent); }
  //else if(id=="interface-menu"){ page = new page_interface_menu(parent); }
  //else if(id=="session-locale"){ page = new page_session_locale(parent); }
  //else if(id=="compton"){ page = new page_compton(parent); }
  //else if(id=="soundtheme"){ page = new page_soundtheme(parent); }
 // else if(id=="mouse-settings"){ page = new page_mouse_trueos(parent); }
 // else if(id=="bluetooth-settings"){ page = new page_bluetooth_trueos(parent); }
  //Return the main control_panel page as the fallback/default
  if(page==0){ id.clear(); page = new page_main(parent); }
  page->setWhatsThis(id);
  return page;
}
