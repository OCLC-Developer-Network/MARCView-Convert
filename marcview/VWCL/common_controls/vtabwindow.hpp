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

Primary Author of this source code file:  Todd Osborne (todd@vwcl.org)
Other Author(s) of this source code file:

VWCL and all source code are copyright (c) 1996-2000 by The VWCL Alliance.*/

#ifndef VTABWINDOW
#define VTABWINDOW

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"
#include "vcommoncontrol.hpp"
#include "vtab.hpp"

/** VTabWindow supports showing child dialog boxes and windows as tabbed
items. it can be used when you desire finer control over the tabbed
environment than what Microsoft provides in the property sheet controls. If
this is sufficient, we recommend using the VWCL VPropertySheet and
VPropertyPage classes instead of this one.*/
class VTabWindow : public VTab
{
public:
	/** Default constructor initializes base class and internal members.*/
	VTabWindow(	VUINT		nOptions = OPTION_SHOW_IMAGE_FOR_ACTIVE_TAB,
				VRTTI_TYPE	nRTTI = VWCL_RTTI_TAB_WINDOW)
		: VTab(nOptions, nRTTI)
	{
		m_nActiveTab =		-1;
		m_nChildWndOffSet =	3;
	}

	/** Return the currently active tab. The default is -1, no tab, so
	client code should set the active tab after creation.*/
	virtual VINT	ActiveTab() const
		{ return m_nActiveTab; }

	/** Set the active tab. If the setting was caused by a mouse click,
	bMouseActivated should be VTRUE. If the specified tab is already
	active, nothing will be done.*/
	virtual VINT	ActiveTab(	VINT	nIndex,
								VBOOL	bMouseActivated = VFALSE)
	{
		VASSERT(nIndex >= 0)

		/* Don't do anything if nIndex is already active.*/
		if ( nIndex != m_nActiveTab )
		{
			/* Set a tab to be active and activate associated window
			(if one exists).*/
			VINT nOldTab = m_nActiveTab;

			/* Save cache of active tab.*/
			m_nActiveTab = nIndex;

			/* Make tab control same setting.*/
			Selection(m_nActiveTab);

			/* Show active tab window and set position.*/
			SetActiveChildPosition();

			/* Hide old window.*/
			HWND hWndOld = (nOldTab != -1) ? Handle(nOldTab) : NULL;

			if ( hWndOld && ::IsWindow(hWndOld) )
				::ShowWindow(hWndOld, SW_HIDE);
		}

		return Selection();
	}

	/** Add a tab and associated window. It is valid to use a temporary or
	non-initialized window here, as it can be set at a later time. Returns
	the index of the tab associated with the window, or -1 on failure. If
	nIndex is -1, the tab will be added to end of existing tabs.*/
	VINT			AddTab(	VSTRING_CONST	pszText,
							VWindow const&	window,
							VINT			nIndex = -1)
	{
		#ifdef VWCL_DEBUG
			/* Window specified, but not a child!*/
			if ( window.GetHandle() )
				VASSERT(window.Style() & WS_CHILD)
		#endif

		/* Add tab to control.*/
		VINT nNewIndex = InsertItem(pszText, nIndex);

		if ( nNewIndex != -1 )
		{
			/* Save HWND of child in LParam.*/
			LParam(nNewIndex, (LPARAM)window.GetHandle());

			VDWORD nStyle = Style();

			/* If multiline, update UI now.*/
			if ( nStyle & TCS_MULTILINE )
			{
				if ( nStyle & TCS_OWNERDRAWFIXED )
					Invalidate();

				SetActiveChildPosition();
			}
		}

		return nNewIndex;
	}

	/** Delete all tabs. This only removes the tabs from the control, it
	DOES NOT destroy the child window or delete a contained VWindow object.
	The caller is responsible for appropriate cleanup of these objects.
	After returning from this function, there will not be an active tab.*/
	void			DeleteAllItems()
	{
		VTab::DeleteAllItems();
		m_nActiveTab = -1;
	}

	/** Delete a tab. This only removes the tab from the control, it DOES
	NOT destroy the child window or delete a contained VWindow object. The
	caller is responsible for appropriate cleanup of these objects. After
	returning from this function, there will not be an active tab. The
	caller should make a different tab active.*/
	void			DeleteTab(VINT nIndex)
	{
		VASSERT(nIndex >= 0)

		DeleteItem(nIndex);
		m_nActiveTab = -1;
	}

	/** Return a reference to the number of pixels to offset child windows
	from edges (defaults to 3 pixels).*/
	VUSHORT&		GetChildWindowOffSet() const
		{ return (VUSHORT&)m_nChildWndOffSet; }

	/** Call to get the rectangle of the active tab. Returns VFALSE if a tab
	is not active, VTRUE otherwise.*/
	VBOOL			GetActiveChildPosition(VRect& r) const
	{
		if ( m_nActiveTab == -1 )
			return VFALSE;

		/* Get our client area.*/
		GetClientRect(r);

		/* Always offset child from client area by m_nChildWndOffSet pixels.*/
		r.Deflate(m_nChildWndOffSet, m_nChildWndOffSet);

		/* Get tab rect.*/
		VRect rTab;
		GetItemRect(rTab, m_nActiveTab);

		/* Now we need to determine if any newer common control
		styles for tabs are used.*/
		VDWORD nStyle = Style();

		/* Test for vertical.*/
		if ( nStyle & TCS_VERTICAL )
		{
			/* Check for right side.*/
			if ( nStyle & TCS_RIGHT )
				r.GetRight() = rTab.GetLeft() - 4;
			else
				r.GetLeft() = rTab.GetRight() + 4;
		}
		else if ( nStyle & TCS_BOTTOM )
			r.GetBottom() = rTab.GetTop() - 4;
		else
			r.GetTop() = rTab.GetBottom() + 4;

		/* Always return 1 less pixel on right and bottom for positioning.*/
		r.SubtractRight(1);
		r.SubtractBottom(1);

		return VTRUE;
	}

	/** Get the window at a specified tab index.*/
	virtual HWND	Handle(VINT nIndex)
		{ return (HWND)LParam(nIndex); }

	/** Set the window at a specified tab index, returning hWnd.*/
	HWND			Handle(	VINT nIndex,
							HWND hWnd)
	{
		LParam(nIndex, (VLPARAM)hWnd);
		return hWnd;
	}

	/** Override calls IsDialogMessage() on current child dialog or window
	to allow message processing.*/
	virtual VBOOL	PreTranslateMessage(MSG const& msg)
	{
		HWND hWndActive =
			(m_nActiveTab != -1) ? Handle(m_nActiveTab) : NULL;

		if ( hWndActive && IsDialogMessage(hWndActive, (MSG*)&msg) )
			return VTRUE;

		return VFALSE;
	}

	/** After changing the font or other font properties, this function
	may need to be called to update the screen.*/
	void			Refresh()
	{
		InvalidateUpdate();
		SetActiveChildPosition();
	}

protected:
	/** Override handles tab selection changes.*/
	virtual VLONG	OnReflectedNotify(	NMHDR&		nmhdr,
										VLPARAM&	lCommonControlResult)
	{
		if ( nmhdr.code == TCN_SELCHANGE )
			ActiveTab(Selection());

		return VTab::OnReflectedNotify(nmhdr, lCommonControlResult);
	}

	/** Overrides for window management.*/
	virtual VLONG	OnSize(	VWPARAM	wFlags,
							VINT	ncx,
							VINT	ncy)
	{
		/* Call the original window proc so it will update the tab
		rectangles.*/
		WindowProc(m_hWindow, WM_SIZE, wFlags, MAKELONG(ncx, ncy));

		/* Resize active child.*/
		SetActiveChildPosition();

		/* We handled this message.*/
		return 0;
	}

	/** Set the active child window position and show window if needed.*/
	virtual void	SetActiveChildPosition()
	{
		VRect rPos;

		if ( GetActiveChildPosition(rPos) )
		{
			/* Show new current window if not currently visible.*/
			HWND hWnd = Handle(m_nActiveTab);

			if ( hWnd && ::IsWindow(hWnd) )
			{
				VUINT nFlags = SWP_NOZORDER;

				if ( !::IsWindowVisible(hWnd) )
					nFlags |= SWP_SHOWWINDOW;

				::SetWindowPos(	hWnd,
								NULL,
								VRECT_BREAK_WIDTH_HEIGHT(rPos),
								nFlags);
			}
		}
	}

	/** Embedded Members.*/
	VINT			m_nActiveTab;
	VUSHORT			m_nChildWndOffSet;
};

#endif /* VTABWINDOW*/
