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

#ifndef VAPPLICATION
#define VAPPLICATION

#include "vstandard.h"
#include "strings/vstring.hpp"

/* Forward declaration(s).*/
class VApplication;

/* Set the VApplication application object. This function is implemented in
vapplication.cpp and is set when a VApplication object is created. If
VGetApp() is called before an application object is created, a default
application object will be created by VWCL, so if an application wants to
not use the default application object, it must instantiate its own
VApplication derived object before calling other VWCL classes or functions,
or must replace the VWCL global object with a call to VSetApp().*/
void VSetApp(VApplication const& appObject);

/* VApplication encapsulates startup code and support for
internationalization. VWCL maintains a global application object of this
class, and a reference can be returned to it with a call to the global
function VGetApp(). This object maintains the application name (title)
and if the application has the concept of a "current file" this class also
maintains the file name string. See the above VSetApp() global function for
more information on replacing the default VWCL application object.*/
class VApplication
{
public:
	/* Standard constructor initializes class variables as needed. The Init()
	function must always be called for all VWCL applications that use the
	VApplication global object, or for applications that implement their own
	VApplication class. See Init() for more information.*/
	VApplication()
	{
		VSetApp(VREF_THIS);

		#ifndef VWCL_NO_WIN32_API
			/* Initialize.*/
			m_hInstance =				NULL;
			m_hResource =				NULL;
			m_hrActiveXInitialized =	E_UNEXPECTED;

			#ifndef VWCL_CONSOLE_APP
				m_nCommandShow =		0;
			#endif
		#endif
	}

	/* Virtual destructor unregisters VWCL window clases and perform other
	as-needed application cleanup.*/
	virtual ~VApplication()
	{
		#ifndef VWCL_DONT_REGISTER_CLASSES
			/* Unregister window classes.*/
			if ( m_hInstance )
				UnregisterClass(VWINDOWCLASS, m_hInstance);
		#endif

		/* Free ActiveX as needed.*/
		InitActiveX(VFALSE);
	}

	/* Return the application title string object.*/
	VString&				GetAppTitle() const
		{ return (VString&)m_strAppTitle; }

	/* Return a reference to the current file name string object.*/
	VString&				GetCurrentFile() const
		{ return (VString&)m_strCurrentFile; }

	/* Determine if the command specified by pszOption is a command line
	option to the program. If it is, a pointer will be returned to it,
	otherwise NULL will be returned. This function will look at each command
	line parameter until it finds pszOption. In addition, it will look past
	the first character of the option to determine if it qualifies. Since
	many command line parameters start with -, /, or \, looking past this
	first character allows this type of command line, as well as those without
	prefaces. The comparisons are not case sensitive by default, unless
	bCaseSensitive is set to VTRUE.*/
	VSTRING_CONST			IsCommandLineOption(
		VSTRING_CONST	pszOption,
		VBOOL			bCaseSensitive = VFALSE) const
	{
		for ( VINT i = 1; i < __argc; i++ )
		{
			if ( bCaseSensitive )
			{
				if (	VSTRCMP(__argv[i], pszOption) == 0 ||
						VSTRCMP(__argv[i] + 1, pszOption) == 0 )
					return __argv[i];
			}
			else if (	VSTRCMP_NOCASE(__argv[i], pszOption) == 0 ||
						VSTRCMP_NOCASE(__argv[i] + 1, pszOption) == 0 )
				return __argv[i];
		}

		return NULL;
	}

	/* Set the current file name and update the main window, or console
	window, caption. If pszFileName is NULL, "Untitled" will be used instead.
	Returns VTRUE if title was set, VFALSE otherwise. The application name is
	included in the caption, as well as pszFileName. If bDefault is VFALSE,
	the function will not use the word "Default" if pszFileName is NULL.
	bDefault is not used anytime pszFileName is not NULL.*/
	virtual VBOOL			SetCurrentFile(	VSTRING_CONST	pszFileName,
											VBOOL			bDefault = VTRUE);

	#ifndef VWCL_NO_WIN32_API
		#ifndef VWCL_CONSOLE_APP
			/* Get command show param (the ShowWindow() API SW_xxx Constant).*/
			VINT			GetCommandShow() const
				{ return m_nCommandShow; }
		#endif

		/* Get the application instance handle.*/
		virtual HINSTANCE	GetInstanceHandle()
		{
			VASSERT(m_hInstance)
			return m_hInstance;
		}

		/* Return a reference to the resource handle. By default this is the
		same as the instance handle, but can be set by modifying the handle
		returned from this function.*/
		virtual HINSTANCE&	GetResourceHandle()
		{
			VASSERT(m_hResource)
			return m_hResource;
		}
	#endif

	/* Initialize class library, application startup, and register window
	classes as needed. You can optionally provide a WNDCLASS structure that
	will be used to initialize the VWindow window class. pWindowClass can be
	NULL to use defaults. If NULL, you can still specify the background brush
	and icon if desired. However, if pWindowClass is not NULL, it is assumed
	to be the gospel, and the structure must be completely initialized. There
	are some modifications to this structure which Init() will make in the
	pWindowClass if they are 0 or NULL. These are as follows:
	pWindowClass->hIcon will be set to the icon loaded from nIconID only if
	nIconID is not 0.
	pWindowClass->hInstance will use the application instance handle.
	pWindowClass->lpfnWndProc will use the default VWindow shared window
	procedure.
	pWindowClass->hCursor will use the global arrow cursor.
	pWindowClass->lpszClassName will use the default class name retreived
	from VWindow::DefaultClassName().
	Functions are provided for when windowing applications are being built,
	and others for when a console app is the target.*/
	#ifndef VWCL_NO_WIN32_API
		#ifndef VWCL_CONSOLE_APP
			#ifndef VWCL_DONT_REGISTER_CLASSES
				void		Init(	HINSTANCE	hInstance,
									VINT		nCommandShow = SW_SHOWNORMAL,
									VUINT		nIconID = 0,
									HBRUSH		hBackgroundBrush =
										(HBRUSH)(COLOR_WINDOW + 1),
									WNDCLASS*	pWindowClass = NULL);
			#else
				void		Init(	HINSTANCE	hInstance,
									VINT		nCommandShow = SW_SHOWNORMAL);
			#endif
		#else
			void			Init(HINSTANCE hInstance = NULL)
			{
				/* Get this module's handle if not passed as parameter.*/
				m_hInstance = m_hResource =
					(hInstance) ? hInstance : GetModuleHandle(NULL);

				/* Initialize ActiveX as needed.*/
				InitActiveX(VTRUE);
			}
		#endif
	#endif

	#ifndef VWCL_NO_WIN32_API
		/* Returns VTRUE if ActiveX has been initialized, VFALSE otherwise.*/
		VBOOL				IsActiveXInitialized() const
			{ return (SUCCEEDED(m_hrActiveXInitialized)) ? VTRUE : VFALSE; }
	#endif

	/* Returns VTRUE if VWCL has been initialized, VFALSE otherwise.*/
	VBOOL					IsInitialized() const
	{
		#ifndef VWCL_NO_WIN32_API
			return (m_hInstance) ? VTRUE : VFALSE;
		#else
			return VTRUE;
		#endif
	}

protected:
	#ifndef VWCL_NO_WIN32_API
		/* Local function that initializes or un-initializes OLE/ActiveX. If
		the ActiveX libraries are implicitly loaded (linked with), it is
		assumed by VWCL that ActiveX should be (un)initialized for the user.
		Some compilers and linkers will not dynamically load a DLL until a
		function contained within it is called. This behaviour would cause
		this function to fail, since it dynamically resolves function pointers
		based on the library having already been loaded. If you see this
		behaviour, you can try to modify the linker to load the OLE libraries,
		or call LoadLibrary("OLE32.DLL") before initializing VWCL, or wrap the
		call to LoadLibrary() in a VLibrary class that remains in scope
		throughout the applications life. If you manually call LoadLibrary(),
		don't forget to use FreeLibrary() before your application exits, or
		this could cause a memory leak. Using VLibrary to handle loading of
		the OLE libraries is the preferred method since it will free the
		library automatically when destructed.*/
		virtual void		InitActiveX(VBOOL bInit);
	#endif

	/* Embedded Members.*/
	#ifndef VWCL_NO_WIN32_API
		HINSTANCE			m_hInstance;
		HINSTANCE			m_hResource;
		HRESULT				m_hrActiveXInitialized;
		#ifndef VWCL_CONSOLE_APP
			VINT			m_nCommandShow;
		#endif
	#endif

	/* Application title and currently opened file.*/
	VString					m_strAppTitle;
	VString					m_strCurrentFile;
};

#endif	/* VAPPLICATION*/
