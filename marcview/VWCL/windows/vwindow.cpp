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

#include "../vstandard.h"
#include "../collections/vlinkedlistmanager.hpp"
#include "../collections/vlinkedlistitem.hpp"
#include "../gdi/vpaintdc.hpp"
#include "../gdi/vtempdc.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vdialog.hpp"
#include "vwindow.hpp"
#include "vwindowmessagefilter.hpp"

/** Structure used internally during window enumerations.*/
typedef struct tagVWCL_ENUM_PARAM
{
	VUINT		nMessageOrID;		/* Message or ID.*/
	VVOIDPTR	pInData;			/* Normally input pointer.*/
	VVOIDPTR	pOutData;			/* Normally output pointer.*/

} VWCL_ENUM_PARAM;

/** Enumeration for VWLC_ENUM_PARAM::nMessageOrID in EnumChildWndProc() call.*/
enum	{		vwindow_ENUM_FIND_CHILD_BY_CLASS,
				vwindow_ENUM_ENABLE_CHILD_WINDOWS,
				vwindow_ENUM_SHOW_CHILD_WINDOWS,
		};

/** This is a private VLinkedListManager which never deletes objects left
in the list when it goes out of scope.*/
class vwindow_WindowList : public VLinkedListManager
{
public:
	/** Empty list, but don't have objects that may be left over
	delete themselves. We probably did not allocate them.*/
	~vwindow_WindowList()
	{
		/* If this asserts, it means the application is exiting with
		VWindow objects that remain. This will not cause a memory leak
		on Win32, since Windows free's memory associated with objects,
		but it is in general bad programming practice to not cleanup
		your own mess.*/
		VASSERT(!m_pFirstItem)

		/* Just in case. We don't want VLinkedListManager to delete
		objects we did not allocate.*/
		Empty();
	}
};

/** Global(s).*/
vwindow_WindowList	g_vwindow_listWindows;
VWindow*			g_vwindow_pMainWindow;
MSG					g_vwindow_CurrentMessage;
VWindow*			g_vwindow_pLastMessageWindow;

#ifndef VWCL_DONT_REGISTER_CLASSES
	VString	g_vwindow_strDefaultClassName;
#endif

/** Global Function to translate dialog messages.*/
VBOOL VTranslateDialogMessage(MSG const& msg)
{
	HWND hWndTop = msg.hwnd;

	/* Obtain the top level window. All dialogs are typically defined
	as top level popups.*/
	while ( hWndTop )
	{
		if ( GetWindowLong(hWndTop, GWL_STYLE) & WS_CHILD )
			hWndTop = GetParent(hWndTop);
		else
			break;
	}

	/* Obtain the associated window pointer (if a VWCL window).*/
	VWindow* pWindow = VWindow::GetVWindowFromHandle(hWndTop);

	if ( !pWindow )
		return VFALSE;

	VBOOL bResult = VFALSE;

	/* Is it a dialog box?*/
	if ( pWindow->IsVDialogType() )
	{
		bResult = IsDialogMessage(hWndTop, (MSG*)&msg);

		/* Is it a modeless property sheet?*/
		if (	!bResult &&
				pWindow->GetRTTI() == VWindow::VWCL_RTTI_PROPERTY_SHEET &&
				!((VDialog*)pWindow)->IsModal() )
			bResult = PropSheet_IsDialogMessage(hWndTop, &msg);
	}

	/* Return result.*/
	return bResult;
}

/** Local callback function to enumerate child windows (for various reasons).
VLPARAM is a pointer to a VWLC_ENUM_PARAM struct. This function is internal
to VWindow. The public interface is much cleaner, using functions such as
FindChildWindowByClass(), EnableChildren(), and ShowChildren().*/
static VBOOL CALLBACK vwindow_EnumChildWndProc(	HWND	hWnd,
												VLPARAM	lParam)
{
	VWCL_ENUM_PARAM* pEnumParam = (VWCL_ENUM_PARAM*)lParam;
	VASSERT(pEnumParam)

	/* Determine what we need to do.*/
	switch ( pEnumParam->nMessageOrID )
	{
		case vwindow_ENUM_FIND_CHILD_BY_CLASS:
		{
			/* pEnumParam->pInData is a string.*/
			VSTRING_CONST pszFind = (VSTRING_CONST)pEnumParam->pInData;
			VASSERT(VSTRLEN_CHECK(pszFind))

			/* Get the class of this window.*/
			VTCHAR szClass[50];
			GetClassName(hWnd, szClass, VARRAY_SIZE(szClass));

			/* Did we find a hit?*/
			if ( VSTRCMP_NOCASE(szClass, pszFind) == 0 )
			{
				/* Set the hWnd into the pEnumParam->pOutData
				field and quit.*/
				pEnumParam->pOutData = hWnd;
				return VFALSE;
			}

			break;
		}

		case vwindow_ENUM_ENABLE_CHILD_WINDOWS:
		{
			/* pEnumParam->pInData is a VBOOL, which tells us
			to enable or disable.*/
			EnableWindow(hWnd, (VBOOL)pEnumParam->pInData);
			break;
		}

		case vwindow_ENUM_SHOW_CHILD_WINDOWS:
		{
			/* pEnumParam->pInData is a VBOOL, which tells us
			to show or hide.*/
			ShowWindow(	hWnd,
						(pEnumParam->pInData)
						? SW_SHOWNORMAL
						: SW_HIDE);
			break;
		}
	}

	return VTRUE;
}

/** Local function to start an enumeration of child windows.*/
void vwindow_EnumChildren(	VWindow const&		window,
							VWCL_ENUM_PARAM&	EnumParam)
{
	VASSERT(window.GetSafeWindow())
	EnumChildWindows(	window.GetHandle(),
						(WNDENUMPROC)vwindow_EnumChildWndProc,
						(VLPARAM)&EnumParam);
}

/** Local function to attach. If it fails, it will also destroy
dialog boxes.*/
VBOOL vwindow_SafeAttach(	VWindow*	pWindow,
							HWND		hWnd)
{
	VASSERT(pWindow && hWnd && IsWindow(hWnd))

	/* If Attach fails, Detach will already have been called.*/
	if ( !pWindow->Attach(hWnd, VTRUE) )
	{
		/* Kill dialog boxes.*/
		if ( pWindow->IsVDialogType() )
		{
			if ( ((VDialog*)pWindow)->IsModal() )
				EndDialog(hWnd, IDCANCEL);
			else
				DestroyWindow(hWnd);
		}

		return VFALSE;
	}

	return VTRUE;
}

/** Destructor for VWindowMessageFilter removes the filter from
all VWindow objects.*/
VWindowMessageFilter::~VWindowMessageFilter()
{
	VWindow* pWindow = (VWindow*)g_vwindow_listWindows.m_pFirstItem;

	while ( pWindow )
	{
		pWindow->InstallMessageFilter(VREF_THIS, VFALSE, VTRUE);
		pWindow = (VWindow*)pWindow->m_pNextItem;
	}
}

/* Start of VWindow implementation.*/
VWindow::VWindow(VRTTI_TYPE nRTTI)
{
	/* Initialize member variables to default.*/
	m_hWindow =								NULL;
	m_pfnOldWndProc =						NULL;
	m_Options =								OPTION_NONE;
	m_nOptionsPreNoDrawResize =				0;
	m_nLastKnownNonClientHitTestResult =	HTNOWHERE;
	m_pInstalledFilters =					NULL;
	GetRTTI() =								nRTTI;

	/* Add to list of VWindow objects. As a slight possible speed booster,
	if this window is a main window object, add to beginning of the linked
	list to make lookups faster. In theory, the main window receives most
	of the messages in the application, so try to handle this case. No harm
	of course is done if this is not true for the application being built.*/
	g_vwindow_listWindows.Add(	this,
								(nRTTI == VWCL_RTTI_MAIN_WINDOW ||
								nRTTI == VWCL_RTTI_MAIN_DIALOG)
								? VTRUE : VFALSE);
}

VWindow::~VWindow()
{
	VDELETE_NULL(m_pInstalledFilters)
	Destroy();

	/* If this window had the last message, remove the cached pointer.*/
	if ( g_vwindow_pLastMessageWindow == this )
		g_vwindow_pLastMessageWindow = NULL;

	/* Remove from list of VWindow objects, but don't delete
	the object! We didn't allocate it.*/
	g_vwindow_listWindows.Delete(this, VFALSE);
}

VBOOL VWindow::Attach(	HWND	hWnd,
						VBOOL	bAlwaysCallSetupWindow)
{
	/* hWnd must be known and valid!*/
	VASSERT(hWnd && ::IsWindow(hWnd))

	VBOOL bCallSetupWindow = bAlwaysCallSetupWindow;

	/* Skip if window is already attached.*/
	if ( m_hWindow != hWnd || !IsAttached() )
	{
		/* Set flag to indicate we shold call SetupWindow().*/
		bCallSetupWindow = VTRUE;

		/* Set HWND into object.*/
		m_hWindow = hWnd;

		/* Subclass window (if needed).*/

		/* Is window a dialog? If yes, set DWL_DLGPROC to subclass it,
		not GWL_WNDPROC.*/
		if ( !IsAttached() )
			m_pfnOldWndProc =
				(WNDPROC)Long(	(IsVDialogType())
								? DWL_DLGPROC
								: GWL_WNDPROC,
								(LONG)WindowProcShared);
	}

	/* Call SetupWindow?*/
	if (	bCallSetupWindow &&
			GetOptions().IsNotSet(OPTION_DONT_CALL_SETUP_WINDOW) )
	{
		if ( !SetupWindow() )
		{
			Detach();
			return VFALSE;
		}
	}

	return VTRUE;
}

VBOOL VWindow::Create(	VWindow const&	windowParent,
						VRect const&	r,
						VUINT			nStyle,
						VSTRING_CONST	pszClassName,
						VSTRING_CONST	pszWindowName,
						VUINT			nIDorMenu)
{
	/* You have not called VApplication::Init() before creating
	window objects!*/
	VASSERT(VGetInstanceHandle() && VGetResourceHandle())

	/* Attemting to create a window for a VWindow object that
	already has a window attached to it!*/
	VASSERT(!GetSafeWindow())

	/* Dialog boxes should never call this function!*/
	VASSERT(!IsVDialogType())

	/* Check to see if we are trying to create a child window without
	a parent.*/
	#ifdef VWCL_DEBUG
		if ( (nStyle & WS_CHILD) && !windowParent.GetSafeWindow() )
			VASSERT(VFALSE)
	#endif

	/* Initialize result.*/
	VBOOL bSuccess = VFALSE;

	/* Initialize CREATESTRUCT.*/
	CREATESTRUCT cs;
	VZEROSTRUCT(cs);

	cs.lpszName =	pszWindowName;
	cs.style =		nStyle;
	cs.hMenu =		(HMENU)nIDorMenu;
	cs.hwndParent =	windowParent.m_hWindow;
	cs.lpszClass =	pszClassName;


	#ifndef VWCL_DONT_REGISTER_CLASSES
		if ( !cs.lpszClass )
			cs.lpszClass = DefaultClassName();
	#endif

	/* This must be known!*/
	VASSERT(cs.lpszClass)

	if ( r.IsEmpty() )
	{
		/* Keep empty rect if a child window is being created.*/
		if ( !(nStyle & WS_CHILD) )
			cs.x = cs.y = cs.cx = cs.cy = CW_USEDEFAULT;
	}
	else
	{
		cs.x =	r.GetLeft();
		cs.y =	r.GetTop();
		cs.cx = r.GetWidth();
		cs.cy = r.GetHeight();
	}

	/* Call derived class PreCreateWindow.*/
	if ( PreCreateWindow(cs) )
	{
		#ifdef VWCL_DEBUG
			SetLastError(0);
		#endif

		/* Create the window, passing a pointer to this object
		as extra parameter.*/
		m_hWindow = CreateWindowEx(	cs.dwExStyle,
									cs.lpszClass,
									cs.lpszName,
									cs.style,
									cs.x,
									cs.y,
									cs.cx,
									cs.cy,
									cs.hwndParent,
									cs.hMenu,
									VGetInstanceHandle(),
									this);

		#ifdef VWCL_DEBUG
			if ( !m_hWindow )
				VSHOWLASTERRORMESSAGE(	m_hWindow && ::IsWindow(m_hWindow),
										NULL)
		#endif

		/* Did the window get created?*/
		if ( m_hWindow )
		{
			/* If not a VWCL window, subclass it. In other words, if this
			was a window that did not use the VWCL registered window class,
			then WM_NCCREATE would not have been sent to WindowProcShared(),
			and we would not have subclassed it yet. This would be the case
			for all standard controls, common controls, and any other
			window that was created using a window procedure that is not
			VWindow::WindowProcShared.*/
			if ( !IsAttached() )
				bSuccess = vwindow_SafeAttach(this, m_hWindow);
			else
				bSuccess = VTRUE;
		}
	}

	return bSuccess;
}

#ifndef VWCL_DONT_REGISTER_CLASSES
	void VWindow::DefaultClassName(VSTRING_CONST pszClassName)
		{ g_vwindow_strDefaultClassName = pszClassName; }

	VSTRING_CONST VWindow::DefaultClassName()
	{
		VSTRING_CONST pszName = VWINDOWCLASS;

		#ifndef VWCL_DONT_REGISTER_CLASSES
			if ( g_vwindow_strDefaultClassName.IsNotEmpty() )
				pszName = g_vwindow_strDefaultClassName;
		#endif

		return pszName;
	}
#endif

void VWindow::Detach()
{
	/* Detach only works when we have subclassed a window. It is not
	possible to detach a VWCL created and controlled window, since it
	would leave the window without a window procedure.*/
	if ( m_pfnOldWndProc )
	{
		/* Is window a dialog? If yes, set DWL_DLGPROC to un-subclass
		it, not GWL_WNDPROC.*/
		Long(	(IsVDialogType())
				? DWL_DLGPROC
				: GWL_WNDPROC,
				(VLONG)m_pfnOldWndProc);

		m_pfnOldWndProc = NULL;
	}
}

void VWindow::EnableChildren(VBOOL bEnable) const
{
	VWCL_ENUM_PARAM EnumParam =
		{vwindow_ENUM_ENABLE_CHILD_WINDOWS, (VVOIDPTR)bEnable, NULL};

	vwindow_EnumChildren(VREF_THIS, EnumParam);
}

VWPARAM VWindow::EnterMessageLoop(HACCEL hAccel)
{
	/* Enter message loop.*/
	while ( GetMessage(&g_vwindow_CurrentMessage, NULL, 0, 0) != 0 )
	{
		if (	!hAccel ||
				hAccel &&
				TranslateAccelerator(	m_hWindow,
										hAccel,
										&g_vwindow_CurrentMessage) == 0 )
		{
			if ( !PreTranslateMessage(g_vwindow_CurrentMessage) )
			{
				TranslateMessage(&g_vwindow_CurrentMessage);
				DispatchMessage(&g_vwindow_CurrentMessage);
			}
		}
	}

	return g_vwindow_CurrentMessage.wParam;
}

HWND VWindow::FindChildWindowByClass(VSTRING_CONST pszClass) const
{
	/* The class name must be known.*/
	VASSERT(VSTRLEN_CHECK(pszClass))

	/* Iterate child windows.*/
	VWCL_ENUM_PARAM EnumParam =
		{vwindow_ENUM_FIND_CHILD_BY_CLASS, (VVOIDPTR)pszClass, NULL};

	vwindow_EnumChildren(VREF_THIS, EnumParam);

	/* EnumParam.pOutData will be non-NULL if a handle was found.*/
	return (EnumParam.pOutData) ? (HWND)EnumParam.pOutData : NULL;
}

MSG& VWindow::GetCurrentMessage()
	{ return g_vwindow_CurrentMessage; }

VLinkedListManager& VWindow::GetLinkedList()
	{ return g_vwindow_listWindows; }

VWindow* VWindow::GetVWindowFromHandle(HWND hWnd)
{
	if ( hWnd )
	{
		/* Loop through linked list and find the window.*/
		VWindow* pWindow = (VWindow*)g_vwindow_listWindows.m_pFirstItem;

		while ( pWindow )
		{
			if ( pWindow->m_hWindow == hWnd )
				return pWindow;

			pWindow = (VWindow*)pWindow->m_pNextItem;
		}
	}

	return NULL;
}

void VWindow::InstallMessageFilter(	VWindowMessageFilter&	messageFilter,
									VBOOL					bInstall,
									VBOOL					bRemoveAll)
{
	/* If bRemoveAll is VTRUE, bInstall must be VFALSE.*/
	if ( bRemoveAll )
		bInstall = VFALSE;

	/* Allocate linked list as needed.*/
	if ( !m_pInstalledFilters )
		m_pInstalledFilters = (bInstall) ? VNEW VLinkedListManager : NULL;

	if ( m_pInstalledFilters )
	{
		/* See if we can find this window already in the list.*/
		VWindowMessageFilter* pFilter =
			(VWindowMessageFilter*)m_pInstalledFilters->m_pFirstItem;

		while ( pFilter )
		{
			if ( pFilter == &messageFilter )
				break;
			else
				pFilter = (VWindowMessageFilter*)pFilter->m_pNextItem;
		}

		/* Did it already exist in the list?*/
		if ( pFilter )
		{
			/* Add to reference count if installing again.*/
			if ( bInstall )
				pFilter->m_ReferenceCount.Add();
			/* Subtract from reference count and see if we are done with
			it, or remove it if bRemoveAll is VTRUE. We don't delete the
			object in VLinkedListManager::Delete() since we did not
			allocate it.*/
			else if (	bRemoveAll ||
						pFilter->m_ReferenceCount.Subtract() == 0 )
				m_pInstalledFilters->Delete(pFilter, VFALSE);
		}
		/* Not in the list. Are we installing it?*/
		else if ( bInstall )
		{
			m_pInstalledFilters->Add(&messageFilter);
			messageFilter.m_ReferenceCount.Add();
		}

		/* If nothing in the list, free it.*/
		if ( !m_pInstalledFilters->m_pFirstItem )
			VDELETE_NULL(m_pInstalledFilters)
	}
}

VWindow& VWindow::MainWindow()
{
	VASSERT(g_vwindow_pMainWindow)
	return VREF_PTR(g_vwindow_pMainWindow);
}

void VWindow::MainWindow(VWindow& mainWindow)
	{ g_vwindow_pMainWindow = &mainWindow; }

void VWindow::SetMainWindowNULL()
	{ g_vwindow_pMainWindow = NULL; }

void VWindow::ShowChildren(VBOOL bShow) const
{
	VWCL_ENUM_PARAM EnumParam =
		{vwindow_ENUM_SHOW_CHILD_WINDOWS, (VVOIDPTR)bShow, NULL};

	vwindow_EnumChildren(VREF_THIS, EnumParam);
}

VLONG CALLBACK VWindow::WindowProcShared(	HWND	hWnd,
											VUINT	nMessage,
											VWPARAM	wParam,
											VLPARAM	lParam)
{
	/* We will need this.*/
	VWindow* pWindow = NULL;

	/* A message we need to attach a HWND?*/
	if ( nMessage == WM_NCCREATE || nMessage == WM_INITDIALOG )
	{
		VASSERT(lParam)

		/* A VWindow or PROPSHEETPAGE pointer must be passed as the
		lParam when creating dialog boxes.*/
		if ( nMessage == WM_INITDIALOG )
		{
			/* If the lParam is a VWindow pointer, it will be in
			the window list.*/
			pWindow =
				(VWindow*)g_vwindow_listWindows.Find((VWindow*)lParam);

			/* If not found, it should be a PROPSHEETPAGE pointer.*/
			if ( !pWindow )
				pWindow = (VWindow*)((PROPSHEETPAGE*)lParam)->lParam;
		}
		/* A VWindow pointer must be passed as the
		CREATESTRUCT::lpCreateParams which is in lParam.*/
		else
			pWindow = (VWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;

		/* This must be known now.*/
		VASSERT(pWindow)

		/* If vwindow_SafeAttach() fails, Detach will already have been
		called.*/
		if ( !vwindow_SafeAttach(pWindow, hWnd) )
			return VFALSE;
	}
	else
	{
		/* Try to get the VWindow pointer from the cache (last used).*/
		if (	g_vwindow_pLastMessageWindow &&
				g_vwindow_pLastMessageWindow->m_hWindow == hWnd )
		{
			VASSERT(g_vwindow_listWindows.Find(g_vwindow_pLastMessageWindow))
			pWindow = g_vwindow_pLastMessageWindow;
		}
		/* Try to get the VWindow pointer from the HWND and save in cache.*/
		else
			g_vwindow_pLastMessageWindow = pWindow =
				GetVWindowFromHandle(hWnd);
	}

	/* Verify we have an object and a window attached to it.*/
	if ( pWindow && pWindow->m_hWindow )
	{
		/* Implement support for base class message overrides.*/
		VLONG nResult =				VWINDOW_MESSAGE_NOT_HANDLED;
		VBOOL bContinueRouting =	VTRUE;
		VBOOL bDialogBox =			pWindow->IsVDialogType();

		/* See if there are filters to call first.*/
		if ( pWindow->m_pInstalledFilters )
		{
			VWindowMessageFilter* pNextFilter =	NULL;
			VWindowMessageFilter* pFilter =
				(VWindowMessageFilter*)pWindow->
					m_pInstalledFilters->m_pFirstItem;

			while ( pFilter )
			{
				/* Save the next filter in case this one is deleted
				during the call to OnMessageFilter().*/
				pNextFilter = (VWindowMessageFilter*)pFilter->m_pNextItem;

				/* If OnMessageFilter() returns VFALSE it means the filter
				handled the message.*/
				bContinueRouting =
					pFilter->OnMessageFilter(	VREF_PTR(pWindow),
												nMessage,
												wParam,
												lParam);

				/* See if m_pInstalledFilters went to NULL, or if we
				should stop routing now.*/
				pFilter =
					(bContinueRouting && pWindow->m_pInstalledFilters)
					? pNextFilter
					: NULL;
			}
		}

		if ( bContinueRouting )
		{
			switch ( nMessage )
			{
				case WM_CLOSE:
					nResult = pWindow->OnClose();
					break;

				case WM_COMMAND:
					nResult =
						pWindow->OnCommand(	HIWORD(wParam),
											LOWORD(wParam),
											(HWND)lParam);
					break;

				case WM_DESTROY:
					pWindow->OnDestroy();
					nResult = 0;
					break;

				/* Handle OPTION_NO_DRAW_RESIZE initialization.*/
				case WM_ENTERSIZEMOVE:
				{
					/* Always reset this value.*/
					pWindow->m_nOptionsPreNoDrawResize = 0;

					VBOOL bDragFullWindows = VFALSE;

					/* Determine if we should do this.*/
					if (	pWindow->GetOptions().
								IsSet(OPTION_NO_DRAW_RESIZE) &&
							pWindow->
								IsLastKnownNonClientHitTestResultInSizer() &&
							SystemParametersInfo(	SPI_GETDRAGFULLWINDOWS,
													0,
													&bDragFullWindows,
													0) &&
							bDragFullWindows )
					{
						pWindow->m_nOptionsPreNoDrawResize =
							pWindow->GetOptions().Bits();

						pWindow->GetOptions().Remove(OPTION_NEED_ON_PAINT);
						pWindow->ShowChildren(VFALSE);
						pWindow->InvalidateUpdate();
					}

					break;
				}

				/* Handle OPTION_NO_DRAW_RESIZE cleanup.*/
				case WM_EXITSIZEMOVE:
				{
					/* If this flag was not set, skip this code.*/
					if (	pWindow->m_nOptionsPreNoDrawResize &
							OPTION_NO_DRAW_RESIZE )
					{
						pWindow->GetOptions() =
							pWindow->m_nOptionsPreNoDrawResize;
						pWindow->ShowChildren(VTRUE);
						pWindow->Invalidate();

						/* Notify window that this is complete.*/
						pWindow->OnSizeNoDrawComplete();
					}

					/* Always reset this value.*/
					pWindow->m_nOptionsPreNoDrawResize = 0;

					break;
				}

				/* Save last known non-client hittest location.*/
				case WM_NCHITTEST:
				{
					/* We must handle dialog boxes differently.*/
					if ( pWindow->IsVDialogType() )
					{
						pWindow->m_nLastKnownNonClientHitTestResult =
							DefWindowProc(hWnd, nMessage, wParam, lParam);

						pWindow->SetDialogResult(
							pWindow->m_nLastKnownNonClientHitTestResult);

						nResult = 0;
					}
					else
						nResult =
							pWindow->m_nLastKnownNonClientHitTestResult =
								pWindow->WindowProc(	hWnd,
														nMessage,
														wParam,
														lParam);

					break;
				}

				case WM_NOTIFY:
				{
					NMHDR* pNMHDR = (LPNMHDR)lParam;

					/* Support for reflected WM_NOTIFY messages. Derived
					class must return 0 if message was handled, -1 if
					handled and parent should NOT be notified, or 1 if
					message was not handled and parent should be notified.
					If -1 is returned, derived classes must also set the
					value in nCommonControlResult to the return value
					expected by the common control.*/
					VWindow* pChildWindow =
						GetVWindowFromHandle(pNMHDR->hwndFrom);

					VLONG nCommonControlResult;

					if ( pChildWindow )
						nResult =
							pChildWindow->OnReflectedNotify(
								VREF_PTR(pNMHDR),
								nCommonControlResult);

					/* Return result code immediately to common control if
					message should not be sent to the parent and a return
					value is expected by the common control.*/
					if ( nResult == -1 )
						return nCommonControlResult;
					else
						nResult =
							pWindow->OnNotify(	(VUINT)wParam,
												(NMHDR&)VREF_PTR(pNMHDR));

					break;
				}

				case WM_PAINT:
				{
					/* We only work when the option bit OPTION_NEED_ON_PAINT
					is set.*/
					if ( pWindow->m_Options.IsSet(OPTION_NEED_ON_PAINT) )
					{
						/* We should completely handle this message.*/
						nResult = 0;

						if ( wParam )
						{
							/* Wrap wParam, which is a DC, in object.*/
							VTempDC dc((HDC)wParam);

							/* Call OnPaint() and possibly route to original
							window procedure.*/
							if (	pWindow->OnPaint(dc) &&
									!bDialogBox &&
									pWindow->m_pfnOldWndProc &&
									pWindow->m_pfnOldWndProc !=
										WindowProcShared )
								CallWindowProc(	pWindow->m_pfnOldWndProc,
												hWnd,
												WM_PAINT,
												(VWPARAM)dc.GetHandle(),
												lParam);
						}
						else
						{
							/* Create a paint DC.*/
							VPaintDC dc(VREF_PTR(pWindow));

							/* Don't call virtual function if we did
							not get a DC.*/
							if ( dc.GetHandle() )
							{
								/* Call OnPaint() and possibly route to
								original window procedure.*/
								if (	pWindow->OnPaint(dc) &&
										!bDialogBox &&
										pWindow->m_pfnOldWndProc &&
										pWindow->m_pfnOldWndProc !=
										WindowProcShared )
									CallWindowProc(	pWindow->m_pfnOldWndProc,
													hWnd,
													WM_PAINT,
													(VWPARAM)dc.GetHandle(),
													lParam);
							}
							else
								nResult = VWINDOW_MESSAGE_NOT_HANDLED;
						}
					}

					break;
				}

				case WM_SIZE:
					nResult =
						pWindow->OnSize(	wParam,
											LOWORD(lParam),
											HIWORD(lParam));
					break;
			}
		}
		/* Message handled by filter.*/
		else
			nResult = 0;

		/* pWindow must not have been deleted by message handlers!*/
		VASSERT(g_vwindow_listWindows.Find(pWindow))

		/* Do we still have a window? Call virtual window procedure?*/
		if (	pWindow->m_hWindow &&
				nResult == VWINDOW_MESSAGE_NOT_HANDLED )
			nResult = pWindow->WindowProc(hWnd, nMessage, wParam, lParam);

		/* Was this message a WM_INITDIALOG for a modal dialog box?
		Return VTRUE to have windows set the input focus.*/
		if (	nMessage == WM_INITDIALOG &&
				pWindow->IsVDialogType() &&
				((VDialog*)pWindow)->IsModal() )
			nResult = VTRUE;
		/* Is the window going away?*/
		else if ( nMessage == WM_NCDESTROY )
		{
			/* Un-wire object. This does NOT set VWindow::m_hWindow to NULL.*/
			pWindow->Detach();

			/* Set object to NULL window handle.*/
			pWindow->m_hWindow = NULL;

			/* Call PostDestroy() in derived object. It can do a delete
			this if needed.*/
			pWindow->PostDestroy();

			/* These are now (probably) invalid.*/
			g_vwindow_pLastMessageWindow = pWindow = NULL;
		}

		return nResult;
	}

	/* VWCL is not managing this window (yet-still), so let Windows
	handle the message. If a dialog box class, then always return 0, not
	the default window procedure.*/
	VCHAR sz[8];

	if (	GetClassName(	hWnd,
							sz,
							VARRAY_SIZE(sz)) &&
							VSTRCMP(sz, "#32770") == 0 )
		return 0;

	return DefWindowProc(hWnd, nMessage, wParam, lParam);
}
