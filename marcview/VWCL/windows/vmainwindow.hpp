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

#ifndef VMAINWINDOW
#define VMAINWINDOW

#include "../vstandard.h"
#include "../gdi/vclientdc.hpp"
#include "../numbers/vbitmask.hpp"
#include "../numbers/vrtti.hpp"
#include "../strings/vstring.hpp"
#include "../structures/vrect.hpp"
#include "../windows_controls/vmenu.hpp"
#include "vwindow.hpp"

/** VMainWindow is a window that is the main window of an application, which
normally has a caption and menu, and many times, a toolbar and status bar.
It provides support for all of these features. In addition, it can handle
the enabling and disabling of menu items and their associated toolbar
buttons. When something changes in your code that affects which menu and
toolbar items should be enabled, call the DoMenuItemUpdates() function. This
will indirectly call your override of GetMenuItemOptions(), which is where
you can place your enabling or disabling logic code. DoMenuItemUpdates()
should be called manually after you create and initialize your main
application window, most likely in an override of SetupWindow(). If
toolbars and-or statusbar are wanted, these classes, from the
common controls, must be included in your application BEFORE this class.*/
class VMainWindow : public VWindow
{
public:
	/** Default constructor initializes base class with RTTI information
	and initializes internal members.*/
	VMainWindow(VRTTI_TYPE nRTTI = VWCL_RTTI_MAIN_WINDOW)
		: VWindow(nRTTI)
	{
		/* Initialize.*/
		m_hAccel =				NULL;
		m_bUserDragSizeMode =	0;

		/* Set a reference to ourselves as the main window pointer
		in the app.*/
		MainWindow(VREF_THIS);

		#ifdef VTOOLBAR
			m_pToolBar = NULL;
		#endif

		#ifdef VSTATUSBAR
			m_pStatusBar = NULL;
		#endif

		#ifdef VREBAR
			m_pReBar = NULL;
		#endif
	}

	/** Virtual destructor verifies toolbar and statusbar objects
	are deleted and resets the main window reference.*/
	virtual ~VMainWindow()
	{
		/* Delete allocated memory.*/
		#ifdef VTOOLBAR
			VDELETE_NULL(m_pToolBar)
		#endif

		#ifdef VSTATUSBAR
			VDELETE_NULL(m_pStatusBar)
		#endif

		#ifdef VREBAR
			VDELETE_NULL(m_pReBar)
		#endif

		SetMainWindowNULL();
	}

	/** Control bar child window ID's.*/
	enum	{		IDW_TOOLBAR = 3500,
					IDW_STATUSBAR,
					IDW_REBAR,
			};

	/** Returns a reference to the accelerator table object used for
	translating keystrokes into commands. If your application uses
	accelerators, you should use this function to store a handle to them
	in this window.*/
	HACCEL&			Accelerators() const
		{ return (HACCEL&)m_hAccel; }

	/** Create a main window with a caption and styles normally used for
	main windows. This function will not return until main program loop
	exits. Return value should be passed back to Windows from the WinMain()
	function. If a rectangle is not specified (VRECT_EMPTY), Windows will
	decide the rectangle to use. nMenuID is the resource ID of the
	applications main main, or 0 if not used. Pass a pointer to an array of
	TBBUTTON structs and the number of items in the array to create an
	initial toolbar. NOTE: If you pass VFALSE for bEnterMessageLoop, the
	return value on success will be 1 and the function will return
	immediately. If toolbars and-or statusbar are wanted, these classes,
	from the common controls, must be included in your application BEFORE
	this class.*/
	VINT			Create(	VUINT			nMenuID,
							VRect const&	r,
							TBBUTTON const*	pToolBarButtons = NULL,
							VUINT			nNumToolBarButtons = 0,
							VUINT			nToolBarBitmapID = 0,
							VBOOL			bInitialStatusBar = VTRUE,
							VUINT			nStyle = WS_OVERLAPPEDWINDOW,
							VBOOL			bEnterMessageLoop = VTRUE)
	{
		/* We are sensitive to header inclusion order. Make sure user
		knows this!*/
		#ifdef VWCL_DEBUG
			if ( pToolBarButtons || nNumToolBarButtons || nToolBarBitmapID )
			{
				#ifndef VTOOLBAR
					MessageBox("It looks like you want a toolbar for your \
								main window. Please include \
								common_controls/vtoolbar.hpp before \
								including vmainwindow.hpp.",
								VWCL_DEBUG_MESSAGE_STRING);
				#endif
			}

			if ( bInitialStatusBar )
			{
				#ifndef VSTATUSBAR
					MessageBox("It looks like you want a statusbar for \
								your main window. Please include \
								common_controls/vstatusbar.hpp before \
								including vmainwindow.hpp.",
								VWCL_DEBUG_MESSAGE_STRING);
				#endif
			}
		#endif

		/* Load the menu?*/
		if ( nMenuID )
			VVERIFY(m_Menu.Load(nMenuID))

		/* We don't want to call SetupWindow() yet, so set that bit.*/
		m_Options.Save();
		m_Options.Append(OPTION_DONT_CALL_SETUP_WINDOW);

		/* Create the window.*/
		if (	VWindow::Create(	VWINDOW_EMPTY,
									r,
									nStyle,
									NULL,
									VGetAppTitle(),
									(VUINT)m_Menu.GetHandle()) )
		{
			/* Reset options.*/
			m_Options.Restore();

			#ifdef VREBAR
				/* Allocate the VReBar control.*/
				m_pReBar = AllocReBar();

				if(	m_pReBar &&
					!m_pReBar->Create(VREF_THIS, VRECT_EMPTY, IDW_REBAR) )
					VDELETE_NULL(m_pReBar)
			#endif

			/* Create control bars?*/
			#ifdef VTOOLBAR
				if (	pToolBarButtons &&
						nNumToolBarButtons &&
						nToolBarBitmapID )
				{
					m_pToolBar = AllocToolBar();

					if ( m_pToolBar )
					{
						/* Set toolbar parent window based on rebar or not.*/
						#ifdef VREBAR
							if ( m_pReBar )
								m_pToolBar->Create(	VREF_PTR(m_pReBar),
													VRECT_EMPTY,
													IDW_TOOLBAR,
													WS_CHILD |
													WS_VISIBLE |
													CCS_NODIVIDER |
													CCS_NORESIZE |
													TBSTYLE_FLAT |
													TBSTYLE_TOOLTIPS);
							else
								m_pToolBar->Create(	VREF_THIS,
													VRECT_EMPTY,
													IDW_TOOLBAR);
						#else
							 m_pToolBar->Create(	VREF_THIS,
													VRECT_EMPTY,
													IDW_TOOLBAR);
						#endif

						if ( m_pToolBar->GetSafeWindow() )
						{
							/* Save toolbar in menu object.*/
							m_Menu.ToolBar(m_pToolBar);

							m_pToolBar->AddBitmap(	nNumToolBarButtons,
													nToolBarBitmapID);

							m_pToolBar->AddButtons(	nNumToolBarButtons,
													pToolBarButtons);

							#ifdef VREBAR
								/* Attach the toolbar to the VReBar.*/
								if ( m_pReBar )
									m_pReBar->
										InsertToolBar(VREF_PTR(m_pToolBar));
							#endif
						}
						else
							VDELETE_NULL(m_pToolBar)
					}
				}
			#endif

			#ifdef VSTATUSBAR
				if ( bInitialStatusBar )
				{
					m_pStatusBar = AllocStatusBar();

					if ( m_pStatusBar )
					{
						VUINT nStatusStyle =
							WS_BORDER |
							WS_VISIBLE |
							CCS_BOTTOM |
							CCS_NOMOVEY;

						/* If we are not being created as a child window,
						SBARS_SIZEGRIP should be turned on.*/
						if ( !(nStyle & WS_CHILD) )
							nStatusStyle |= SBARS_SIZEGRIP;

						if ( !m_pStatusBar->Create(	VREF_THIS,
													VRECT_EMPTY,
													IDW_STATUSBAR,
													NULL,
													nStatusStyle) )
							VDELETE_NULL(m_pStatusBar)
					}
				}
			#endif

			/* Now call SetupWindow() since child controls are created.*/
			if (	m_Options.IsSet(OPTION_DONT_CALL_SETUP_WINDOW) ||
					SetupWindow() )
			{
				/* Enter message loop or return success.*/
				if ( bEnterMessageLoop )
				{
					/* Window should be visible.*/
					if ( !IsVisible() )
						Show(VGetCommandShow());

					return EnterMessageLoop();
				}

				return 1;
			}
			else
				Destroy();
		}

		return -1;
	}

	/** Call to force menu item updates for all menus and, indirectly,
	toolbar buttons. This function should be called anywhere in your
	application where a state change affects which menu and toolbar items
	should be enabled or disabled. This function will indirectly call the
	virtual function GetMenuItemOptions() to determine which menu and
	toolbar commands should be enabled or disabled. This function is
	virtual, and an override does not need to call this base class function.
	This allows you to completely replace the menu and toolbar handling
	logic of this class with a single override. DoMenuItemUpdates() should
	be called from your main window initialization code, most likely in an
	override of SetupWindow(), to set the initial toolbar and menu state.*/
	virtual void	DoMenuItemUpdates()
	{
		/* Update menu handlers for all top-level menus.*/
		if ( m_Menu.GetHandle() )
		{
			VMenu			subMenu;
			VUINT nCount =	m_Menu.GetItemCount();

			for ( VUINT i = 0; i < nCount; i++ )
			{
				subMenu.Detach();

				if ( subMenu.Attach(m_Menu.GetSubMenu(i), VFALSE) )
				{
					#ifdef VTOOLBAR
						subMenu.ToolBar(m_pToolBar);
					#endif

					HandleMenuItemUpdate(subMenu);
				}
			}
		}
	}

	/** Calculate new client rect and call RecalcLayout(). This function
	should be called anytime your application causes a change to the size
	of the client area of the main window. It will calculate the new size
	of the client window by calling the xGetClientRect() function, and then
	calling the RecalcLayout() virtual function as a notification of the new
	size. If the rectangle is already known, a slight optimization can be
	realized by passing this rectangle as pKnownRect.*/
	void			DoRecalcLayout(VRect* pKnownRect = NULL)
	{
		if ( pKnownRect )
			RecalcLayout(VREF_PTR(pKnownRect));
		else
		{
			VRect r;
			xGetClientRect(r);
			RecalcLayout(r);
		}
	}

	/** Call to enter message loop. Return value is from final WM_QUIT
	message. This function extends the base class function by verifying
	that WinHelp closes your applications online help file if it was
	displayed, before exit.*/
	VWPARAM			EnterMessageLoop()
	{
		/* Enter message loop with current accelerators.*/
		VWPARAM nResult = VWindow::EnterMessageLoop(Accelerators());

		/* Close WinHelp if started.*/
		WinHelp(m_hWindow, NULL, HELP_QUIT, 0);

		return nResult;
	}

	/** Get a VWindow pointer to a control bar. This function will only
	return a pointer if the toolbar or statusbar object is being uses by
	this application, and was properly allocated. nControlBar can be either
	IDW_TOOLBAR, IDW_STATUSBAR, or IDW_REBAR, nothing else. Returns NULL if
	the control bar does not exist.*/
	VWindow*		GetControlBar(VUINT nControlBar = IDW_TOOLBAR) const
	{
		VASSERT(nControlBar == IDW_TOOLBAR || nControlBar == IDW_STATUSBAR || nControlBar == IDW_REBAR)

		VWindow* pControlBar = NULL;

		#ifdef VTOOLBAR
			if ( nControlBar == IDW_TOOLBAR )
				pControlBar = m_pToolBar;
		#endif

		#ifdef VSTATUSBAR
			if ( nControlBar == IDW_STATUSBAR )
				pControlBar = m_pStatusBar;
		#endif

		#ifdef VREBAR
			if( nControlBar == IDW_REBAR )
				pControlBar = m_pReBar;
		#endif

		return pControlBar;
	}

	/** Return a reference to the internal menu object. If this window does
	not have a menu associated with it, the returned VMenu object will not
	be initialized. This should be check with a call to VMenu::GetHandle().*/
	VMenu&			GetMenu() const
		{ return (VMenu&)m_Menu; }

	/** Determine if a control bar is visible. nControlBar can be either
	IDW_TOOLBAR, IDW_STATUSBAR or IDW_REBAR, This function uses the VWindow
	function IsVisibleEx(), which returns VTRUE if the window would be
	visible if the parent window is visible, and does not reflect the
	actual visible state. If asking for a toolbar that is part of a rebar
	control, this function will ask the rebar if the band is visible.*/
	VBOOL			IsControlBarVisible(VUINT nControlBar = IDW_TOOLBAR) const
	{
		#ifdef VREBAR
			if( m_pReBar && nControlBar == IDW_TOOLBAR )
				return m_pReBar->IsBandVisibleEx(IDW_TOOLBAR);
		#endif

		VWindow* pControlBar = GetControlBar(nControlBar);
		return (pControlBar) ? pControlBar->IsVisibleEx() : VFALSE;
	}

	/** Show or hide a control bar. This function show or hides the control
	bar as needed. If the shown state changes, this function will
	automatically invalidate the client area and perform a layout
	recalculation which can be seen by overriding the RecalcLayout()
	function. nControlBar can be either IDW_TOOLBAR, IDW_STATUSBAR, or
	IDW_REBAR. This function will also call the virtual function
	DoMenuItemUpdates() to update menu item state. The invalidation,
	recalculations of client area and menu item updates are only performed
	if this window is visible at the time the function is called.*/
	virtual void	ShowControlBar(	VUINT nControlBar = IDW_TOOLBAR,
									VBOOL bShow = VTRUE)
	{
		/* Several people, including myself, are guilty of calling this
		function with the bShow parameter as a ShowWindow() SW_xxx constant.
		Try to gaurd against this.*/
		VASSERT(bShow == VTRUE || bShow == VFALSE)

		/* Anything to do?*/
		if ( IsControlBarVisible(nControlBar) != bShow )
		{
			VWindow* pControlBar = GetControlBar(nControlBar);

			if ( pControlBar )
			{
				if ( bShow )
					#ifdef VREBAR
						if( m_pReBar && nControlBar == IDW_TOOLBAR )
							m_pReBar->ShowBandEx(IDW_TOOLBAR);
						else
					#endif
						pControlBar->Show();
				else
				#ifdef VREBAR
					if ( m_pReBar && nControlBar == IDW_TOOLBAR )
						m_pReBar->HideBandEx(IDW_TOOLBAR);
					else
				#endif
						pControlBar->Hide();

				if ( IsVisible() )
				{
					/* Invalidate part of window not in use by control bars.*/
					VRect r;
					xGetClientRect(r);
					InvalidateRect(r);

					/* Notify of recalc.*/
					DoRecalcLayout(&r);

					/* Update menu items as needed.*/
					DoMenuItemUpdates();
				}
			}
		}
	}

	/** Toggle shown state of control car. nControlBar can be either
	IDW_TOOLBAR, IDW_STATUSBAR, or IDW_REBAR.*/
	void			ToggleControlBar(VUINT nControlBar = IDW_TOOLBAR)
	{
		if ( IsControlBarVisible(nControlBar) )
			ShowControlBar(nControlBar, VFALSE);
		else
			ShowControlBar(nControlBar);
	}

	#ifdef VSTATUSBAR
		/** Update text in the status bar, if a status bar exists. pszText
		is the text to set.*/
		void		UpdateStatusBar(	VSTRING_CONST	pszText,
										VUINT			nPart = 0)
		{
			if ( m_pStatusBar )
				m_pStatusBar->SetText(pszText, nPart);
		}

		/** Update text in the status bar, if a status bar exists, from a
		string loaded from resources. nStringID is the string resource ID
		of the text to set.*/
		void		UpdateStatusBar(	VUINT		nStringID,
										VUINT		nPart = 0,
										HINSTANCE	hResource = NULL)
		{
			if ( m_pStatusBar )
				m_pStatusBar->SetText(nStringID, nPart, hResource);
		}
	#endif

	/** Get the client rectangle, less the toolbar and status bar area. Will
	be same as GetClientRect() when control bars are not being used, or if
	they are not visible. The adjusted client rectangle is the total
	USABLE client area of the window.*/
	virtual void	xGetClientRect(VRect& r)
	{
		/* Get full client rectangle first.*/
		GetClientRect(r);

		#ifdef VREBAR
			if ( IsControlBarVisible(IDW_REBAR) )
			{
				VRect rReBar;
				m_pReBar->GetRect(rReBar);
				r.AddTop(rReBar.GetHeight());
			}
		#endif

		#ifdef VTOOLBAR
			#ifdef VREBAR
				if( !m_pReBar )
				{
			#endif

				if ( IsControlBarVisible(IDW_TOOLBAR) )
				{
					VRect rToolBar;
					m_pToolBar->GetRect(rToolBar);
					r.AddTop(rToolBar.GetHeight() - 1);
				}
			#ifdef VREBAR
				}
			#endif
		#endif

		#ifdef VSTATUSBAR
			if ( IsControlBarVisible(IDW_STATUSBAR) )
			{
				VRect rStatusBar;
				m_pStatusBar->GetRect(rStatusBar);

				if ( m_pStatusBar->Style() & WS_BORDER )
					r.SubtractBottom(rStatusBar.GetHeight() + 3);
				else
					r.SubtractBottom(rStatusBar.GetHeight() + 1);
			}
		#endif
	}

protected:
	#ifdef VREBAR
		/** Override to allocate a custom VReBar object.  The base class
		allocates a VReBar object.*/
		virtual VReBar*		AllocReBar()
			{ return VNEW VReBar; }
	#endif

	#ifdef VSTATUSBAR
		/** Override to allocate a custom VStatusBar object. The base class
		allocates a VStatusBar object.*/
		virtual VStatusBar*	AllocStatusBar()
			{ return VNEW VStatusBar; }
	#endif

	#ifdef VTOOLBAR
		/** Override to allocate a custom VToolBar object. The base class
		allocates a VToolBar object.*/
		virtual VToolBar*	AllocToolBar()
			{ return VNEW VToolBar; }
	#endif

	/** Derived classes should override this function to enable/disable, and
	check/uncheck menu items. bEnable and bCheck are passed as references,
	so you should set their values to VTRUE or VFALSE accordingly. When
	this function is called, bEnable is VTRUE and bCheck is VFALSE. Derived
	code only needs to modify these values when the application state
	dictates that these options should be set to something other than
	defaults. For instance, the File->Exit menu option is seldom disabled,
	so there is not need in most applications to handle the nCommandID of
	ID_FILE_EXIT (or whatever you call it). Most applications implement
	this function as a switch statement on the nCommandID parameter, which
	is the menu command ID the function is asking for. This function will
	be called once for each menu item in your applications menu structure.*/
	virtual void	GetMenuItemOptions(	VMenu&	popupMenu,
										VUINT	nCommandID,
										VBOOL&	bEnable,
										VBOOL&	bCheck)
		{;}

	/** Called from DoMenuItemUpdates() to update toolbar and menu item
	state. This function will call GetMenuItemOptions() for each menu item
	it finds. This function recursively walks the menu stucture and calls
	GetMenuItemOptions() once for each menu item.*/
	void			HandleMenuItemUpdate(VMenu& popupMenu)
	{
		VASSERT(popupMenu.GetSafeHandle())

		/* Run through this popup menu sub items and enable/disable,
		and check/uncheck menu and toolbar items.*/
		VINT nCount = popupMenu.GetItemCount();

		for ( VINT i = 0; i < nCount; i++ )
		{
			VUINT nCommandID = popupMenu.GetItemID(i);

			/* Verify not 0.*/
			if ( nCommandID )
			{
				/* Check for another popup or invalid item.*/
				if ( nCommandID != 0xFFFFFFFF )
				{
					VBOOL bEnable =	VTRUE;
					VBOOL bCheck =	VFALSE;

					/* Call virtual function to determine state.*/
					GetMenuItemOptions(	popupMenu,
										nCommandID,
										bEnable,
										bCheck);

					/* Set menu item state.*/
					popupMenu.EnableItem(	nCommandID,
											MF_BYCOMMAND | ((bEnable)
											? MF_ENABLED
											: MF_GRAYED));

					popupMenu.CheckItem(	nCommandID,
											MF_BYCOMMAND | ((bCheck)
											? MF_CHECKED
											: MF_UNCHECKED));
				}
				else
				{
					/* Recursively check this popup menu.*/
					VMenu popupMenuNext;

					#ifdef VTOOLBAR
						popupMenuNext.ToolBar(popupMenu.ToolBar());
					#endif

					if ( popupMenuNext.Attach(	popupMenu.GetSubMenu(i),
												VFALSE) )
						HandleMenuItemUpdate(popupMenuNext);
				}
			}
		}
	}

	/** Override destroys menu (as needed) and calls PostQuitMessage()
	to exit the application.*/
	virtual void	OnDestroy()
	{
		m_Menu.Destroy();
		PostQuitMessage(0);
	}

	/** Handle notifications from toolbar to update status bar.*/
	virtual VLONG	OnNotify(VUINT nIDControl, NMHDR& nmhdr)
	{
		#if defined(VTOOLBAR) || defined(VSTATUSBAR)
			switch ( nmhdr.code )
			{
				#ifdef VTOOLBAR
					case TTN_NEEDTEXT:
					{
						/* Retreive text for toolbar and statusbar?*/
						if ( m_pToolBar )
						{
							TOOLTIPTEXT* pToolTip = (TOOLTIPTEXT*)&nmhdr;

							/* Load string from menu string.*/
							VString s;

							if ( s.Load(pToolTip->hdr.idFrom) )
							{
								/* Tooltip text starts after newline
								char, or first char if not found.*/
								VSTRING pszTT =
									s.ReplaceFirstChar(VTEXT('\n'));

								if ( pszTT )
									pszTT++;
								else
									pszTT = s;

								VSTRNCPY(pToolTip->szText, pszTT, 80);

								#ifdef VSTATUSBAR
									/* Set string in pane 0.*/
									UpdateStatusBar(s);
								#endif
							}
						}

						return 0;
					}
				#endif

				#ifdef VSTATUSBAR
					/* The status bar should show the default
					text since a tooltip was hidden.*/
					case TTN_POP:
						UpdateStatusBar((VSTRING_CONST)NULL);
						return 0;
				#endif
			}
		#endif

		return VWindow::OnNotify(nIDControl, nmhdr);
	}

	/** Override updates control bars and causes RecalcLayout() to be
	sent to derived classes.*/
	virtual VLONG	OnSize(	VWPARAM	nFlags,
							VINT	ncx,
							VINT	ncy)
	{
		#ifdef VREBAR
			if ( m_pReBar )
				m_pReBar->Move(0, 0, ncx, 0, VTRUE);
		#endif

		#ifdef VTOOLBAR
			if ( m_pToolBar )
				m_pToolBar->AutoSize();
		#endif

		#ifdef VSTATUSBAR
			if ( m_pStatusBar )
				m_pStatusBar->Move(0, 0, ncx, 0, VTRUE);
		#endif

		DoRecalcLayout();

		return VWindow::OnSize(nFlags, ncx, ncy);
	}

	/** Called by VWindow when the user completes the sizing of a window
	when the option bit OPTION_NO_DRAW_RESIZE is used. We simply
	recalculate the window layout at this time.*/
	virtual void	OnSizeNoDrawComplete()
		{ DoRecalcLayout(); }

	/** Called by framework on WM_SIZE and control bar changes. r reflects
	the usable client area. Derived class can use this to resize child
	windows, etc. The base class function does nothing. Note that r is not
	passed as a reference here, so it is safe to modify it as needed in
	derived code.*/
	virtual void	RecalcLayout(VRect r)
		{;}

	/** Override manages main window messages that affect status bars.*/
	#ifdef VSTATUSBAR
		virtual VLONG	WindowProc(	HWND	hWnd,
									VUINT	nMessage,
									VWPARAM	wParam,
									VLPARAM	lParam)
		{
			if ( m_pStatusBar )
			{
				switch ( nMessage )
				{
					/* Handle menu selection.*/
					case WM_MENUSELECT:
					{
						VWORD	nCommandID =	LOWORD(wParam);
						VWORD	nFlags = 		HIWORD(wParam);
						VMenu	menuSelect((HMENU)lParam, VFALSE);

						VString strStatus;

						if (	nCommandID && !(nFlags == 0xFFFF &&
								!menuSelect.GetHandle()) )
						{
							/* Check for System Menu.*/
							if ( nFlags & MF_SYSMENU )
							{
								if ( menuSelect.GetItemString(	nCommandID,
																strStatus) )
								{
									/* Remove ampersand.*/
									VINT nAmp = strStatus.Find(VTEXT('&'));

									if ( nAmp != -1 )
										strStatus.RemoveSection(nAmp, 1);
								}
							}
							/* Load the string from resources and truncate
							string at newline (that text is for toolbars).*/
							else if ( strStatus.Load(nCommandID) )
								strStatus.ReplaceFirstChar(VTEXT('\n'));
						}

						/* Truncate string at tab (that text is for hotkeys).*/
						strStatus.ReplaceFirstChar(VTEXT('\t'));

						/* Set string in pane 0.*/
						UpdateStatusBar(strStatus);

						return 0;
					}
				}
			}

			return VWindow::WindowProc(hWnd, nMessage, wParam, lParam);
		}
	#endif

	/** Control bar members.*/
	#ifdef VREBAR
		VReBar*			m_pReBar;
	#endif

	#ifdef VTOOLBAR
		VToolBar*		m_pToolBar;
	#endif

	#ifdef VSTATUSBAR
		VStatusBar*		m_pStatusBar;
	#endif

	/** Embedded Members.*/
	VMenu				m_Menu;
	HACCEL				m_hAccel;
	VBOOL				m_bUserDragSizeMode;
};

#endif /* VMAINWINDOW*/
