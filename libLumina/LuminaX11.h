//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
//  This class governs all the XLib usage and interactions
//  and provides simpler Qt-based functions for use elsewhere
//===========================================
#ifndef _LUMINA_LIBRARY_X11_H
#define _LUMINA_LIBRARY_X11_H

//Qt includes
#include <QList>
#include <QString>
#include <QPixmap>
#include <QImage>
#include <QIcon>
#include <QPixmap>
#include <QX11Info>
#include <QDebug>
#include <QPainter>
#include <QObject>
#include <QFlags>


#include <xcb/xcb_ewmh.h>

//SYSTEM TRAY STANDARD DEFINITIONS
#define _NET_SYSTEM_TRAY_ORIENTATION_HORZ 0
#define _NET_SYSTEM_TRAY_ORIENTATION_VERT 1
#define SYSTEM_TRAY_REQUEST_DOCK 0
#define SYSTEM_TRAY_BEGIN_MESSAGE 1
#define SYSTEM_TRAY_CANCEL_MESSAGE 2

#define URGENCYHINT (1L << 8) //For window urgency detection

//XCB Library replacement for LX11 (Qt5 uses XCB instead of XLib)
class LXCB{
	
public:
	enum WINDOWSTATE {IGNORE, INVISIBLE, VISIBLE, ACTIVE, ATTENTION}; //note that this in order of priority
	enum ICCCM_STATE {WITHDRAWN, NORMAL, ICONIC};
	//Now enums which can have multiple values at once
	enum ICCCM_PROTOCOL {TAKE_FOCUS = 0x0, DELETE_WINDOW = 0x1}; //any combination 
	Q_DECLARE_FLAGS(ICCCM_PROTOCOLS, ICCCM_PROTOCOL);
	
	
	xcb_ewmh_connection_t EWMH; //This is where all the screen info and atoms are located

	LXCB();
	~LXCB();
		
	//== Main Interface functions ==
	// General Information
	QList<WId> WindowList(bool rawlist = false); //list all non-Lumina windows (rawlist -> all workspaces)
	unsigned int CurrentWorkspace();
	unsigned int NumberOfWorkspaces();
	WId ActiveWindow(); //fetch the ID for the currently active window
	
	//Session Modification
	bool CheckDisableXinerama(); //returns true if Xinerama was initially set but now disabled
	void RegisterVirtualRoots(QList<WId> roots);
	void SetCurrentWorkspace(int);

	//Window Information
	QString WindowClass(WId);
	unsigned int WindowWorkspace(WId); //The workspace the window is on
	QRect WindowGeometry(WId win, bool includeFrame = true); //the geometry of the window (frame excluded)
	QList<int> WindowFrameGeometry(WId win); //Returns: [top,bottom,left,right] sizes of the frame
	WINDOWSTATE WindowState(WId win); //Visible state of window
	QString WindowVisibleIconName(WId win); //_NET_WM_VISIBLE_ICON_NAME
	QString WindowIconName(WId win); //_NET_WM_ICON_NAME
	QString WindowVisibleName(WId win); //_NET_WM_VISIBLE_NAME
	QString WindowName(WId win); //_NET_WM_NAME
	QString OldWindowName(WId win); //WM_NAME (old standard)
	QString OldWindowIconName(WId win); //WM_ICON_NAME (old standard)
	bool WindowIsMaximized(WId win);
	int WindowIsFullscreen(WId win); //Returns the screen number if the window is fullscreen (or -1)
	QIcon WindowIcon(WId win); //_NET_WM_ICON
	
	//Window Modification
	// - SubStructure simplifications (not commonly used)
	void SelectInput(WId); //XSelectInput replacement (to see window events)
	uint GenerateDamageID(WId);
	
	// - General Window Modifications
	void SetAsSticky(WId); //Stick to all workspaces
	void SetDisableWMActions(WId); //Disable WM control (shortcuts/automatic functions)
	void SetAsPanel(WId); //Adjust all the window flags for a proper panel (cannot be done through Qt)
	void SetAsDesktop(WId); //Adjust window flags to set as the desktop
	void CloseWindow(WId); //request that the window be closed
	void KillClient(WId); //Force the application that created the window to close
	void MinimizeWindow(WId); //request that the window be unmapped/minimized
	void ActivateWindow(WId); //request that the window become active
	void RestoreWindow(WId); //Re-map/raise the window
	void MaximizeWindow(WId win, bool flagsonly = false); //request that the window become maximized
	void MoveResizeWindow(WId win, QRect geom);
	void ResizeWindow(WId win, int width, int height);
	void ResizeWindow(WId win, QSize sz){ ResizeWindow(win, sz.width(), sz.height()); } //overload for simplicity
	void ReserveLocation(WId win, QRect geom, QString loc);
	
	//Window Embedding/Detaching (for system tray)
	//void SetWindowBackground(QWidget *parent, QRect area, WId client);
	uint EmbedWindow(WId win, WId container); //returns the damage ID (or 0 for an error)
	bool UnembedWindow(WId win);
	QPixmap TrayImage(WId win);
	
	//System Tray Management
	WId startSystemTray(int screen = 0); //Startup the system tray (returns window ID for tray)
	void closeSystemTray(WId); //Close the system tray

	
	//============
	// WM Functions (directly changing properties/settings)
	//  - Using these directly may prevent the WM from seeing the change
	//============
	void WM_CloseWindow(WId win);
	
	// ICCCM Standards (older standards)
	// -- WM_NAME
	QString WM_ICCCM_GetName(WId win);
	void WM_ICCCM_SetName(WId win, QString name);
	// -- WM_ICON_NAME
	QString WM_ICCCM_GetIconName(WId win);
	void WM_ICCCM_SetIconName(WId win, QString name);
	// --- WM_CLIENT_MACHINE
	QString WM_ICCCM_GetClientMachine(WId win);
	void WM_ICCCM_SetClientMachine(WId win, QString name);
	// -- WM_CLASS
	QString WM_ICCCM_GetClass(WId win); //Returns: "<instance name>::::<class name>"
	void WM_ICCCM_SetClass(WId win, QString name);
	// -- WM_TRANSIENT_FOR
	WId WM_ICCCM_GetTransientFor(WId win); //Returns "win" for errors or no transient
	void WM_ICCCM_SetTransientFor(WId win, WId transient);
	// -- WM_SIZE_HINTS
	
	// -- WM_NORMAL_HINTS
	
	// -- WM_HINTS
	
	// -- WM_PROTOCOLS
	ICCCM_PROTOCOLS WM_ICCCM_GetProtocols(WId win);
	void WM_ICCCM_SetProtocols(WId win, ICCCM_PROTOCOLS flags);
	
	//NET_WM Standards (newer standards)
	void WM_Set_Root_Supported(); //set the atom list of supported features on the root window
	void WM_Set_Window_Supported(WId win); //set the atom list of supported features on the given window
	
private:
	QList<xcb_atom_t> ATOMS;
	QStringList atoms;

	void createWMAtoms(); //fill the private lists above
};

#endif