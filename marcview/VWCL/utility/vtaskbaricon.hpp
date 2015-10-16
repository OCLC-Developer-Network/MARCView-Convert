/* This source code is part of the Virtual Windows Class Library (VWCL).
VWCL is a public C++ class library, placed in the public domain, and is
open source software. VWCL is not governed by any rules other than these:

1) VWCL may be used in commercial and other applications.

2) VWCL may not be distributed, in source code form, in development related
projects, unless the developer is also a VWCL contributor.

3) VWCL may be used in development related projects in binary, compiled form,
by anyone.

4) VWCL shall remain open source software regardless of distribution method.

5) A proper copyright notice referencing the "VWCL Alliance" must be included
in the application and/or documentation.

6) No company or individual can ever claim ownership of VWCL.

7) VWCL source code may be modified or used as a base for another class
library.

8) Your use of this software forces your agreement to not hold any member,
individual or company, liable for any damages resulting from the use of the
provided source code. You understand the code is provided as-is, with no
warranty expressed or implied by any member of the VWCL Alliance. You use
this code at your own risk.

Primary Author of this source code file:  Jan Sijm (jsijm@murdock.demon.nl)
Other Author(s) of this source code file: Todd Osborne (todd@vwcl.org)

VWCL and all source code are copyright (c) 1996-2000 by The VWCL Alliance.*/

#ifndef VTASKBARICON
#define VTASKBARICON

#include "..\vstandard.h"
#include "..\strings\vstring.hpp"
#include "..\structures\vpoint.hpp"
#include "..\windows\vwindow.hpp"
#include "..\windows\vwindowmessagefilter.hpp"

/** VTaskBarIcon implements an icon that is visible in the notification
area of the taskbar (system tray).*/
class VTaskBarIcon : public VWindowMessageFilter
{
public:
	/** The default constructor of the VTaskBarIcon class initializes class
	members. The nID paramater is used to identify the VTaskBarIcon object.
	If your application uses multiple taskbar icons, pass a unique ID for
	each object of this type.*/
	VTaskBarIcon(VUINT nID = 0)
	{
		/* Initialize.*/
		m_nID =					nID;
		m_nCallbackMessage =	0;
		m_hIcon =				NULL;
		m_hWndOwner =			NULL;
	}

	/** The destructor will delete the icon from the taskbar and the
	icon associated with this object will be destroyed.*/
	~VTaskBarIcon()
	{
		if ( m_hWndOwner )
		{
			/* Initialize structure.*/
			NOTIFYICONDATA nid;
			VZEROSTRUCT(nid);
			nid.cbSize    = sizeof(nid);
			nid.hWnd      = m_hWndOwner;
			nid.uID       = m_nID;

			/* Delete the taskbar icon from the taskbar.*/
			Shell_NotifyIcon(NIM_DELETE, &nid);
		}

		/* Destroy the icon.*/
		if ( m_hIcon )
			DestroyIcon(m_hIcon);
	}

	/** The Create() method will create the taskbar icon. Before calling this
	method, call one of the Icon() methods to set the icon that will be
	associated with this VTaskBarIcon object. You can also optionally set
	the tooltip text to display using GetToolTip(). If you do not explicitly
	specify a tooltip text, this method will use VGetAppTitle() for this
	purpose. The icon and text can optionally be passed to this function.*/
	VBOOL		Create(	VWindow&		ownerWindow,
						VUINT			nIconID = 0,
						VSTRING_CONST	pszToolTip = NULL,
						HINSTANCE		hResource = NULL)
	{
		/* Parent window must be valid!*/
		VASSERT(ownerWindow.GetSafeWindow())

		/* Don't call this function a second time!*/
		VASSERT(!m_hWndOwner)

		/* Save the owner window handle.*/
		m_hWndOwner = ownerWindow.GetHandle();

		/* Set an icon?*/
		if ( nIconID )
			Icon(nIconID, VFALSE, hResource);

		/* Must set an icon before calling Create!*/
		VASSERT(m_hIcon)

		/* Set a default tooltip if necessary.*/
		if ( pszToolTip )
			m_strTip = pszToolTip;
		else if ( m_strTip.IsEmpty() )
			m_strTip = VGetAppTitle();

		/* Initialize the structure.*/
		NOTIFYICONDATA nid;
		VZEROSTRUCT(nid);
		VZEROMEMORY(nid.szTip, VARRAY_SIZE(nid.szTip));
		nid.cbSize           = sizeof(nid);
		nid.hWnd             = m_hWndOwner;
		nid.uID              = m_nID;
		nid.uFlags           = NIF_ICON | NIF_TIP | NIF_MESSAGE;
		nid.uCallbackMessage = RegisterWindowMessage(VTEXT("VTaskBarIcon"));
		nid.hIcon            = m_hIcon;

		if ( m_strTip.IsNotEmpty() )
			VSTRNCPY(nid.szTip , m_strTip, VARRAY_SIZE(nid.szTip) - 1);

		/* Hook into ownerWindow to receive messages it gets.*/
		ownerWindow.InstallMessageFilter(VREF_THIS);

		/* Create the taskbar icon.*/
		if ( Shell_NotifyIcon(NIM_ADD, &nid) )
		{
			m_nCallbackMessage = nid.uCallbackMessage;
			return VTRUE;
		}

		return VFALSE;
	}

	/** Return a reference to the string that is displayed as a tooltip. If
	you modify this string, you should call the Update() method to set the
	value into the tray.*/
	VString&	GetToolTip() const
		{ return (VString&)m_strTip; }

	/** Return the handle to the icon that is currently associated with the
	object.*/
	HICON		Icon() const
		{ return m_hIcon; }

	/** This method will set the icon associated with this VTaskBarIcon
	object. If bUpdate is VFALSE, the icon will not be updated immediately,
	but will be updated when either the Create() or Update() method is
	called. This object will destroy the icon as needed, as it assumes
	ownership of it. It is valid for the icon to be NULL. This removes a
	previously installed icon, as needed.*/
	VBOOL		Icon(	HICON hIcon,
						VBOOL bUpdate = VFALSE)
	{
		/* Destroy current icon?*/
		if ( m_hIcon )
			DestroyIcon(m_hIcon);

		/* Set the handle of the icon.*/
		m_hIcon = hIcon;

		/* Update the icon of the taskbar icon?*/
		if ( bUpdate )
			return Update();

		return VTRUE;
	}

	/** Same as above, but loads the icon from application resources, or
	hResource if not NULL. It is valid for the icon ID to be 0. This removes
	a previously installed icon, as needed.*/
	VBOOL		Icon(	VUINT		nIconID,
						VBOOL		bUpdate = VFALSE,
						HINSTANCE	hResource = NULL)
	{
		HICON hIcon = NULL;

		/* Load the icon resource?*/
		if ( nIconID )
			hIcon = (HICON)LoadImage(	(hResource)
										? hResource
										: VGetResourceHandle(),
										MAKEINTRESOURCE(nIconID),
										IMAGE_ICON,
										GetSystemMetrics(SM_CXSMICON),
										GetSystemMetrics(SM_CYSMICON),
										LR_DEFAULTCOLOR);

		/* Set the icon.*/
		return Icon(hIcon, bUpdate);
	}

	/** This method will update the icon and tooltip text associated with
	this object.*/
	VBOOL		Update()
	{
		/* Make sure Create was called before Update!*/
		VASSERT(m_hWndOwner)

		/* Initialize structure.*/
		NOTIFYICONDATA nid;
		VZEROSTRUCT(nid);
		VZEROMEMORY(nid.szTip, VARRAY_SIZE(nid.szTip));
		nid.cbSize    = sizeof(nid);
		nid.hWnd      = m_hWndOwner;
		nid.uID       = m_nID;
		nid.uFlags    = NIF_ICON | NIF_TIP;
		nid.hIcon     = m_hIcon;

		if ( m_strTip.IsNotEmpty() )
			VSTRNCPY(nid.szTip , m_strTip, VARRAY_SIZE(nid.szTip) - 1);

		/* Update the icon and tooltip associated with this object.*/
		return (Shell_NotifyIcon(NIM_MODIFY, &nid)) ? VTRUE : VFALSE;
	}

protected:
	/** VWindowMessageFilter override notifies us of message sent to
	owner window. We call our virtual functions for some of them.*/
	VBOOL			OnMessageFilter(	VWindow&	ownerWindow,
										VUINT		nMessage,
										VWPARAM		wParam,
										VLPARAM		lParam)
	{
		/* Is this our registered message?*/
		if ( nMessage == m_nCallbackMessage )
		{
			VPoint ptCurrent;
			ptCurrent.GetCursorPosition();

			/* Select event handler depending on mouse button and action.*/
			switch ( lParam )
			{
				case WM_LBUTTONDBLCLK:
					OnLeftButtonDoubleClick(ptCurrent);
					break;

				case WM_LBUTTONDOWN:
					OnLeftButtonDown(ptCurrent);
					break;

				case WM_LBUTTONUP:
					OnLeftButtonUp(ptCurrent);
					break;

				case WM_RBUTTONDBLCLK:
					OnRightButtonDoubleClick(ptCurrent);
					break;

				case WM_RBUTTONDOWN:
					OnRightButtonDown(ptCurrent);
					break;

				case WM_RBUTTONUP:
					OnRightButtonUp(ptCurrent);
					break;
			}
		}

		/* We only monitor messages, we don't trap them, so return
		VTRUE to allow owner to also get message.*/
		return VTRUE;
	}

	/** This event handler is called when the left mouse button is double
	clicked on the taskbar icon. The default implementation restores the
	owner window if it is minimized or not visible. ptCurrent is the current
	mouse position, relative to the screen, not a specific window.*/
	virtual void	OnLeftButtonDoubleClick(VPoint& ptCurrent)
	{
		VASSERT(m_hWndOwner && IsWindow(m_hWndOwner))

		if ( IsIconic(m_hWndOwner) || !IsWindowVisible(m_hWndOwner) )
		{
			ShowWindow(m_hWndOwner, SW_SHOWNORMAL);
			SetForegroundWindow(m_hWndOwner);
		}
	}

	/** This event handler is called when the left mouse button is pressed
	on the taskbar icon. The default implementation does nothing. ptCurrent
	is the current mouse position, relative to the screen, not a specific
	window.*/
	virtual void	OnLeftButtonDown(VPoint& ptCurrent)
		{;}

	/** This event handler is called when the left mouse button is released
	on the taskbar icon. The default implementation does nothing. ptCurrent
	is the current mouse position, relative to the screen, not a specific
	window.*/
	virtual void	OnLeftButtonUp(VPoint& ptCurrent)
		{;}

	/** This event handler is called when the right mouse button is double
	clicked on the taskbar icon. The default implementation does nothing.
	ptCurrent is the current mouse position, relative to the screen, not a
	specific window.*/
	virtual void	OnRightButtonDoubleClick(VPoint& ptCurrent)
		{;}

	/** This event handler is called when the right mouse button is pressed
	on the taskbar icon. The default implementation does nothing. ptCurrent
	is the current mouse position, relative to the screen, not a specific
	window.*/
	virtual void	OnRightButtonDown(VPoint& ptCurrent)
		{;}

	/** This event handler is called when the right mouse button is released
	on the taskbar icon. The default implementation does nothing. ptCurrent
	is the current mouse position, relative to the screen, not a specific
	window.*/
	virtual void	OnRightButtonUp(VPoint& ptCurrent)
		{;}

	/** Embedded Members.*/
	VString			m_strTip;
	VUINT			m_nID;
	VUINT			m_nCallbackMessage;
	HICON			m_hIcon;
	HWND			m_hWndOwner;
};

#endif /* VTASKBARICON*/
