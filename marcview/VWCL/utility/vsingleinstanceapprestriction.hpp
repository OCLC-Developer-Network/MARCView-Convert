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

#ifndef VSINGLEINSTANCEAPPRESTRICTION
#define VSINGLEINSTANCEAPPRESTRICTION

#include "vstandard.h"
#include "../strings/vstring.hpp"
#include "../utility/vhandle.hpp"

/** VSingleInstanceAppRestriction assists with applications where only a
single instance of the applicaton should be able to be run at a time. Most
applications use FindWindow() to search for previous instances of an
application, but this is very prone to failure, since applications often
change their window title, and since multiple copies of a program could be
started before a window is created. This class uses a named mutex
(Mutually Exclusive) kernel syncronization object. Once a mutex object is
created, all other attempts to create it fail, but return an error code that
informs us that another mutex with the same name already existed. This is
perfect for allowing only a single instance of an executable file to be
run. An object of this class should be created before your application does
any real processing, and a call to Init() made to determine if your
application should continue normal execution, or should exit without doing
anything. When this class determines that another instance is running, it
will call the HandleInstance() virtual function. The base class
implementation does nothing, except returns VFALSE, which is returned from
Init(). Other functions assist with passing parameters back into the
original running program. Objects created from the class MUST live the
entire lifetime of the application, and should be created on the stack or
heap in your WinMain() or main() function.*/
class VSingleInstanceAppRestriction
{
public:
	#ifndef VWCL_CONSOLE_APP
		/** This function can be used when Init() returns VFALSE, or during
		an override of HandleInstance(). This theory is that we know
		another instance of an application is running, so we will try to
		communicate with it, using the WM_COPYDATA message. To determine the
		window to talk to, we must first find it using either the window
		caption string, and-or the class name. The class name is safer,
		unless you are also using the default VWCL registered VWindow
		class name, since that could easily conflict with other applications
		running on the system. If you tell VWCL to use a custom class name
		(see VWindow::DefaultClassName), then it is safer to specify the
		class name. The only time you should use the window title is if your
		application does not alter the window title during its normal
		execution. Either pszWindowTitle or pszWindowClass must be
		specified. You can specify both. If neither is specified,
		pszWindowTitle will be set to VGetAppTitle(), which could return
		NULL if not already initialized. If command line parameters should
		be passed to the found window, set pszCommandLine to a non NULL
		value. nUserData will be passed as the dwData member of the
		COPYDATASTRUCT structure passed in the WM_COPYDATA message to the
		found window. nTimeOut specifies the number of seconds this routine
		should spend trying to find the other instance's window before it
		times out. The default is 60 seconds. You can pass 0 for this
		parameter, which causes this function to loop forever, but that is
		highly discouraged. When a window of a previous instance is found,
		this function will send the WM_COPYDATA message to it. You will
		have to override VWindow::WindowProc() to receive this message, if
		using VWindow or one of its derived classes. The WM_COPYDATA
		message contains a COPYDATASTRUCT pointer as the lParam value. The
		dwData member will be the same as the nData parameter to this
		function. The lpData member will be a string pointer to the
		pszCommandLine parameter, and cbData is the length of this string,
		including the NULL terminator. This function returns VTRUE if the
		window was found and the WM_COPYDATA message sent, VFALSE
		otherwise.
		NOTE: This function will also activates and shows the found
		application window. So you only really need to handle the
		WM_COPYDATA message if your application should process command
		line arguments. If the application should only be shown, it will
		be, and you don't need to process WM_COPYDATA.*/
		VBOOL	DoFindWindowStartup(	VSTRING_CONST	pszWindowTitle = NULL,
										VSTRING_CONST	pszWindowClass = NULL,
										VSTRING_CONST	pszCommandLine = NULL,
										VDWORD			nUserData = 0,
										VUINT			nTimeOut = 60)
		{
			if ( !pszWindowTitle && !pszWindowClass )
				pszWindowTitle = VGetAppTitle();

			/* Either pszWindowTitle or pszWindowClass must be known!*/
			VASSERT(VSTRLEN_CHECK(pszWindowTitle))
			VASSERT(VSTRLEN_CHECK(pszWindowClass))

			if ( pszWindowTitle || pszWindowClass )
			{
				/* Build struct and pass to WM_COPYDATA.*/
				COPYDATASTRUCT	cmdMsg;
				cmdMsg.dwData = nUserData;
				cmdMsg.lpData = (VVOIDPTR)pszCommandLine;
				cmdMsg.cbData =
					(pszCommandLine) ? VSTRLEN(pszCommandLine) + 1 : 0;

				/* Loop waiting to find the previous window. Wait
				no more than nTimeOut seconds (if specified).*/
				VDWORD nQuitWhen =
					(nTimeOut) ? 0 : GetTickCount() + (nTimeOut * 1000);

				while ( !nQuitWhen || GetTickCount() < nQuitWhen )
				{
					HWND hWnd = FindWindow(pszWindowClass, pszWindowTitle);

					if ( hWnd )
					{
						/* Make the window visible.*/
						ShowWindow(hWnd, SW_SHOWNORMAL);
						SetForegroundWindow(hWnd);

						/* Send the WM_COPYDATA message.*/
						SendMessage(hWnd, WM_COPYDATA, 0, (VLPARAM)&cmdMsg);

						return VTRUE;
					}
					else
						Sleep(50);
				}
			}

			return VFALSE;
		}
	#endif

	/** Initialize the object. Returns VTRUE if another instance of the
	application is not already running, meaning you should continue your
	normal program startup. If VFALSE is returned, it means another
	instance is already running and you should either exit now, or handle
	the case where you want to provide custom startup code for the 2nd
	instance. This function does not in any way prevent a second instance
	of a program from running, only a way to detect if it is. pszMutexName
	should normally be specified, and can be the application name. This
	function, when naming a mutex, automatically prepends the string
	"vmutex" to whatever you pass as pszMutexName, to help prevent mutex
	name collisions. If pszMutexName is NULL, VGetAppTitle() will be used.
	VWCL does not have to be initialized for this to work, as you do not
	have to call VApplication::Init() before using this function.
	If pszMutexName is NULL, and you want the application name to be
	appended to the mutex name, you should use
	VGetApp().GetAppTitle().String(...) before calling this function. This
	is safe to do before calling VApplication::Init(). Or, just pass a
	string name for pszMutexName. If pszMutexName is NULL, and so is the
	application title, only the string "vmutex" will be used to name the
	string. This will work most of the time, but would fail in a case where
	multiple applications use this class with pszMutexName as NULL. Leaving
	pszMutexName as NULL when VGetAppTitle() would also return NULL is
	very highly discouraged. If this function detects that another instance
	is running, it does not automatically return VFALSE. Instead it calls
	the virtual function HandleInstance(). The base class version does
	return VFALSE, but an override of this function can do whatever
	you wish.*/
	VBOOL		Init(VSTRING_CONST pszMutexName = NULL)
	{
		/* Preface mutex name with vmutex.*/
		VString strMutex(VTEXT("vmutex"));

		/* Complete mutex name. If a mutex name is not specified,
		try to use application title.*/
		strMutex += (pszMutexName) ? pszMutexName : VGetAppTitle();

		/* Create the mutex and save mutex handle to be closed by VHandle.*/
		if (	strMutex.IsNotEmpty() &&
				m_Mutex.Set(CreateMutex(NULL, VFALSE, strMutex), VFALSE) )
		{
			/* If the mutex was created by us, return VTRUE to
			continue normal program execution.*/
			if ( GetLastError() != ERROR_ALREADY_EXISTS )
				return VTRUE;
			else
				return HandleInstance(strMutex);
		}

		/* The application should not continue to run.*/
		return VFALSE;
	}

protected:
	/** This function is called when the Init() function determines that
	another instance of the application is already running. The return
	value from this function is returned to the caller of Init(). This
	base class function returns VFALSE, indicating to the caller that
	another instance is already running.*/
	virtual VBOOL	HandleInstance(VSTRING_CONST pszMutexName)
		{ return VFALSE; }

	/** Embedded Members.*/
	VHandle			m_Mutex;
};

#endif /* VSINGLEINSTANCEAPPRESTRICTION*/
