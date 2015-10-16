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

#include "vapplication.hpp"

#ifndef VWCL_CONSOLE_APP
	#ifndef VWCL_NO_VWINDOW
		#include "windows/vwindow.hpp"
	#endif
#endif

#ifndef VWCL_NO_WIN32_API
	/** Pointers to functions in OLE32.DLL (used by InitActiveX() which
	initializes ActiveX as needed).*/
	typedef HRESULT		(WINAPI *VOLEINITIALIZE)	(VVOIDPTR pvReserved);
	typedef HRESULT		(WINAPI *VOLEUNINITIALIZE)	(void);
#endif

/** The application object global pointer and flag to indicate if VWCL created
the object or not.*/
VApplication*	g_vapplication_pTheApp =				NULL;
VBOOL			g_vapplication_bLocalApplicatonObject =	VFALSE;

#ifdef VWCL_DEBUG
	VBOOL		g_vapplication_bAtExitFreeApplicationObjectCalled = VFALSE;
#endif

/** atexit() function to delete the VWCL allocated VApplication object.*/
void vapplication_AtExitFreeApplicationObject()
{
	if ( g_vapplication_bLocalApplicatonObject )
	{
		VDELETE_NULL(g_vapplication_pTheApp)

		#ifdef VWCL_DEBUG
			g_vapplication_bAtExitFreeApplicationObjectCalled = VTRUE;
		#endif
	}
}

/** Get the application object pointer.*/
VApplication& VGetApp()
{
	/* Allocate default VApplication object if needed.*/
	if ( !g_vapplication_pTheApp )
	{
		/* VGetApp() should never be called after the
		AtExitFreeApplicationObject() is called from atexit()!*/
		VASSERT(!g_vapplication_bAtExitFreeApplicationObjectCalled)

		/* Allocate default application object. The constructor for
		VApplication calls VSetApp().*/
		VNEW VApplication;

		/* Set flag to indicate that VWCL owns the application object.*/
		g_vapplication_bLocalApplicatonObject = VTRUE;

		/* Register atexit() function to delete application object.*/
		atexit(vapplication_AtExitFreeApplicationObject);
	}

	VASSERT(g_vapplication_pTheApp)

	return VREF_PTR(g_vapplication_pTheApp);
}

/** Set the application object pointer.*/
void VSetApp(VApplication const& appObject)
{
	/* Do nothing if nothing changed.*/
	if ( &appObject != g_vapplication_pTheApp )
	{
		/* Should we delete the VWCL allocated app object now?*/
		if ( g_vapplication_bLocalApplicatonObject )
			VDELETE_NULL(g_vapplication_pTheApp)

		/* Make assignment.*/
		g_vapplication_pTheApp = (VApplication*)&appObject;

		/* Set flag to indicate that VWCL does not own the application
		object. This will be reset to VTRUE by VGetApp() if VWCL indeed
		does own the application object.*/
		g_vapplication_bLocalApplicatonObject = VFALSE;
	}
}

/** Return a static string buffer to the applications title, or name.*/
VSTRING_CONST	VGetAppTitle()
	{ return VGetApp().GetAppTitle(); }

#ifndef VWCL_CONSOLE_APP
	/** Return the show command (ShowWindow() SW_xxx constant passed on the
	command line).*/
	VINT		VGetCommandShow()
		{ return VGetApp().GetCommandShow(); }
#endif

/** Return the global instance handle of the application or DLL.*/
HINSTANCE		VGetInstanceHandle()
	{ return VGetApp().GetInstanceHandle(); }

/** Return the instance handle where resources are held.*/
HINSTANCE		VGetResourceHandle()
	{ return VGetApp().GetResourceHandle(); }

#ifndef VWCL_NO_WIN32_API
	VBOOL VApplication::SetCurrentFile(	VSTRING_CONST	pszFileName,
										VBOOL			bDefault)
	{
		/* If pszFileName IS the current filename pointer, do nothing.*/
		if ( pszFileName && pszFileName == m_strCurrentFile.String() )
			return VTRUE;

		/* Save current filename string.*/
		m_strCurrentFile = pszFileName;

		/* Build display string.*/
		VString s(VGetAppTitle());

		VSTRING_CONST pszTitle =
			(pszFileName) ? pszFileName : (bDefault) ? VTEXT("Untitled") : NULL;

		if ( s.IsNotEmpty() && pszTitle )
		{
			s += VTEXT(" - ");
			s += pszTitle;
		}

		#ifdef VWCL_CONSOLE_APP
			if ( !SetConsoleTitle(s) )
				return VFALSE;
		#else
			#ifndef VWCL_NO_VWINDOW
				VWindow::MainWindow().SetText(s);
			#endif
		#endif

		return VTRUE;
	}
#endif

#ifndef VWCL_NO_WIN32_API
	#ifndef VWCL_CONSOLE_APP
		#ifndef VWCL_DONT_REGISTER_CLASSES
			void VApplication::Init(	HINSTANCE	hInstance,
										VINT		nCommandShow,
										VUINT		nIconID,
										HBRUSH		hBackgroundBrush,
										WNDCLASS*	pWindowClass)
		#else
			void VApplication::Init(	HINSTANCE	hInstance,
										VINT		nCommandShow)
		#endif
		{
			VASSERT(hInstance)

			/* Global application object should be uninitialized, you are
			walking on it!*/
			VASSERT(m_hInstance == NULL)
			m_hInstance = hInstance;

			/* If resource handle was not previously set, set to same
			as application.*/
			if ( !m_hResource )
				m_hResource = hInstance;

			/* Initialize Component Object Model (as needed).*/
			InitActiveX(VTRUE);

			/* Save show value.*/
			m_nCommandShow = nCommandShow;

			#ifndef VWCL_DONT_REGISTER_CLASSES
				/* Register VWindow class.*/
				WNDCLASS wc;

				/* Use default window class struct?*/
				if ( !pWindowClass )
				{
					VZEROSTRUCT(wc);
					pWindowClass = &wc;

					/* Initialize defaults.*/
					pWindowClass->style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;

					/* We allow some basic customizations here,
					icon and background brush.*/
					pWindowClass->hbrBackground = hBackgroundBrush;

					if ( nIconID )
					{
						pWindowClass->hIcon = LoadIcon(
							VGetInstanceHandle(),
							MAKEINTRESOURCE(nIconID));
						
						VASSERT(pWindowClass->hIcon)
					}
					else
						pWindowClass->hIcon = LoadIcon(NULL, IDI_APPLICATION);
				}

				/* Some things we can modify if left empty.*/
				if ( !pWindowClass->hIcon && nIconID )
					pWindowClass->hIcon = LoadIcon(	VGetInstanceHandle(),
													MAKEINTRESOURCE(nIconID));

				if ( !pWindowClass->hInstance )
					pWindowClass->hInstance = m_hInstance;

				if ( !pWindowClass->lpfnWndProc )
					pWindowClass->lpfnWndProc = VWindow::WindowProcShared;

				if ( !pWindowClass->hCursor )
					pWindowClass->hCursor = LoadCursor(NULL, IDC_ARROW);

				if ( !pWindowClass->lpszClassName )
					pWindowClass->lpszClassName = VWindow::DefaultClassName();

				/* This will fail if the class is already registered. That is
				ok. If the class cannot be registered, the first attempt to
				create the class will fail, so that can be caught then.*/
				RegisterClass(pWindowClass);
			#endif
		}
	#endif
#endif

#ifndef VWCL_NO_WIN32_API
	void VApplication::InitActiveX(VBOOL bInit)
	{
		/* Anything to do?*/
		if ( bInit && IsActiveXInitialized() )
			return;

		if ( !bInit && !IsActiveXInitialized() )
			return;

		/* Is ActiveX loaded?*/
		HINSTANCE hActiveX = GetModuleHandle(VTEXT("OLE32.DLL"));

		if ( hActiveX )
		{
			if ( bInit )
			{
				VOLEINITIALIZE pfn =
					(VOLEINITIALIZE)GetProcAddress(hActiveX, "OleInitialize");

				VASSERT(pfn)

				if ( pfn )
					m_hrActiveXInitialized = pfn(NULL);

				/* Was ActiveX needed but not initialized?*/
				VASSERT(SUCCEEDED(m_hrActiveXInitialized))
			}
			else
			{
				VOLEUNINITIALIZE pfn =
					(VOLEUNINITIALIZE)GetProcAddress(	hActiveX,
														"OleUninitialize");
				VASSERT(pfn)

				if ( pfn )
				{
					pfn();
					m_hrActiveXInitialized = E_UNEXPECTED;
				}
			}
		}
	}
#endif
