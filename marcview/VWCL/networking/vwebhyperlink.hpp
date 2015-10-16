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

#ifndef VWEBHYPERLINK
#define VWEBHYPERLINK

#include "../vstandard.h"
#include "../utility/vshellregistrar.hpp"

/* Local string constants.*/
static VSTRING_CONST VWEBHYPERLINK_IEXPLORE_STRING = {VTEXT("IEXPLORE")};
static VSTRING_CONST VWEBHYPERLINK_NETSCAPE_STRING = {VTEXT("NETSCAPE")};

/** VWebHyperLink is used to communicate with web browsers, telling them to
navigate to a web site. Using this class is much safer than the
ShellExecute() API call. In addition to providing more options, this class
has much less likelihood of failure. Whereas ShellExecute(), and the similar
API calls, give up rather easily, this class can automatically try several
methods to open a browser and display a site. It can use DDE, launch a web
browser, communicate with a web browser when the window handle (HWND) is
known, and attempt to locate the default system browser by looking in the
registry.*/
class VWebHyperLink
{
public:
	/** Default constructor initializes internal variables.*/
	VWebHyperLink()
	{
		m_pszServiceName =	NULL;
		m_nDDE =			0;
	}

	/** Virtual destructor un-initializes DDE communications as needed.*/
	virtual ~VWebHyperLink()
	{
		if ( m_nDDE )
			DdeUninitialize(m_nDDE);
	}

	/** Browse() function error results.*/
	enum	{	BROWSE_ERROR_NONE,
				BROWSE_ERROR_LAUNCH_FAILED,
				BROWSE_ERROR_DDE_INIT_FAILED,
				BROWSE_ERROR_NO_BROWSER_FOUND,
			};

	/** Browse pszURL in default, or last used WWW browser. Return value is
	one of the enumerations above. This function will first try to establish
	a DDE connection with a running browser. If this fails, the default
	browser will be used to view the site. If you want to force a certain
	browser to open the site, use BrowsePath() instead.*/
	VUINT		Browse(VSTRING_CONST pszURL)
	{
		/* URL must be known!*/
		VASSERT(VSTRLEN_CHECK(pszURL))

		/* If needed, we will get the path to the browser here.*/
		VString strBrowserPath;

		/* We will try first with an existing DDE service name.*/
		VSTRING_CONST pszServiceName = m_pszServiceName;

		if ( !pszServiceName )
		{
			/* Determine the service name from the path to the browser.*/
			if (	GetDefaultBrowserPath(strBrowserPath) ||
					strBrowserPath.IsNotEmpty() )
			{
				strBrowserPath.UpperCase();

				if ( strBrowserPath.Find(VWEBHYPERLINK_NETSCAPE_STRING) != -1 )
					pszServiceName = VWEBHYPERLINK_NETSCAPE_STRING;
				else
					pszServiceName = VWEBHYPERLINK_IEXPLORE_STRING;

				strBrowserPath.LowerCase();
			}
		}

		/* If we fail to find a browser from the path, just try Internet
		Explorer. If that fails, BrowseDDE() can try Netscape. Sometimes the
		OS knows more than us about how stuff is installed, and sometimes
		other browsers respond to these DDE requests.*/
		if ( !pszServiceName )
			pszServiceName = VWEBHYPERLINK_IEXPLORE_STRING;

		/* Attempt to Browse with this DDE name.*/
		VUINT nResult = BrowseDDE(pszURL, pszServiceName, VTRUE);

		if ( nResult == BROWSE_ERROR_DDE_INIT_FAILED )
		{
			/* Since we could not browse with the DDE connection, we must
			launch the browser. If we did not get the browser path, get it
			now.*/
			if ( GetDefaultBrowserPath(strBrowserPath) )
				nResult = BrowsePath(pszURL, strBrowserPath);
			else
				nResult = BROWSE_ERROR_NO_BROWSER_FOUND;
		}

		return nResult;
	}

	/** Browse pszURL with a known DDE service name
	(VWEBHYPERLINK_IEXPLORE_STRING or VWEBHYPERLINK_NETSCAPE_STRING) for
	instance. pszServiceName can be any DDE object that responds to the
	WWW_OpenURL handle, including other web browsers or HTML editors.
	Returns BROWSE_ERROR_NONE on success and BROWSE_ERROR_DDE_INIT_FAILED if
	a DDE connection to the browser could not be made. If bRecurseOnFailure
	is VTRUE, and the attempt to communicate via pszServiceName fails, then
	this function will attempt to browse via the other browser. For example
	if VWEBHYPERLINK_IEXPLORE_STRING is passed and fails, this function will
	then call itself with VWEBHYPERLINK_NETSCAPE_STRING as the service name.
	Only these two browsers are supported for recursion.*/
	VUINT		BrowseDDE(	VSTRING_CONST	pszURL,
							VSTRING_CONST	pszServiceName,
							VBOOL			bRecurseOnFailure = VTRUE)
	{
		/* These must be known!*/
		VASSERT(VSTRLEN_CHECK(pszURL) && VSTRLEN_CHECK(pszServiceName))

		/* Return code.*/
		VUINT nResult = BROWSE_ERROR_DDE_INIT_FAILED;

		/* Start DDE if not already done.*/
		if ( !m_nDDE )
		{
			if ( DdeInitialize(	&m_nDDE,
								NULL,
								APPCMD_CLIENTONLY |
								CBF_SKIP_ALLNOTIFICATIONS,
								0) != DMLERR_NO_ERROR )
			{
				m_nDDE = 0;
			}
		}

		/* Make sure DDE has been initialized.*/
		if ( m_nDDE )
		{
			HSZ hszServName = DdeCreateStringHandle(m_nDDE,
													pszServiceName,
													CP_WINANSI);

			if ( hszServName )
			{
				HSZ	hszTopic = DdeCreateStringHandle(	m_nDDE,
														VTEXT("WWW_OpenURL"),
														CP_WINANSI);

				if ( hszTopic )
				{
					/* TODO: DdeConnect() SOMETIMES fails if a DDE server
					that responds to WWW_OpenURL is not already running.*/
					HCONV hConv = DdeConnect(	m_nDDE,
												hszServName,
												hszTopic,
												NULL);

					/* If we connected, tell app to go to URL.*/
					if ( hConv )
					{
						/* Old version:
						wsprintf(sz, VTEXT("\"%s\",,-1,,,,"), pszURL);*/
						VString s("\"");
						s += pszURL;
						s += VTEXT("\",,-1,,,,");

						HSZ hszItem = DdeCreateStringHandle(m_nDDE,
															s,
															CP_WINANSI);

						if ( hszItem )
						{
							/* Success!*/
							nResult = BROWSE_ERROR_NONE;

							/* Tell client to go to the URL (assume success).*/
							DdeClientTransaction(	NULL,
													0,
													hConv,
													hszItem,
													CF_TEXT,
													XTYP_REQUEST,
													60000,
													NULL);

							DdeFreeStringHandle(m_nDDE, hszItem);
						}

						/* Disconnect conversation with browser.*/
						DdeDisconnect(hConv);
					}

					DdeFreeStringHandle(m_nDDE, hszTopic);
				}

				DdeFreeStringHandle(m_nDDE, hszServName);
			}
		}

		/* Should we try again with the other browser?*/
		if (	nResult == BROWSE_ERROR_DDE_INIT_FAILED &&
				bRecurseOnFailure )
		{
			if (	pszServiceName == VWEBHYPERLINK_IEXPLORE_STRING ||
					VSTRCMP_NOCASE(	pszServiceName,
									VWEBHYPERLINK_IEXPLORE_STRING) == 0 )
			{
				nResult = BrowseDDE(pszURL,
									VWEBHYPERLINK_NETSCAPE_STRING,
									VFALSE);
			}
			else if (	pszServiceName == VWEBHYPERLINK_NETSCAPE_STRING ||
						VSTRCMP_NOCASE(	pszServiceName,
										VWEBHYPERLINK_NETSCAPE_STRING) == 0 )
			{
				nResult = BrowseDDE(	pszURL,
										VWEBHYPERLINK_IEXPLORE_STRING,
										VFALSE);
			}
		}

		/* Set cache of service name, or set to NULL on failure to talk
		via DDE.*/
		if ( nResult != BROWSE_ERROR_DDE_INIT_FAILED )
		{
			if (	pszServiceName == VWEBHYPERLINK_IEXPLORE_STRING ||
					VSTRCMP_NOCASE(	pszServiceName,
									VWEBHYPERLINK_IEXPLORE_STRING) == 0 )
			{
				m_pszServiceName = VWEBHYPERLINK_IEXPLORE_STRING;
			}
			else if (	pszServiceName == VWEBHYPERLINK_NETSCAPE_STRING ||
						VSTRCMP_NOCASE(	pszServiceName,
										VWEBHYPERLINK_NETSCAPE_STRING) == 0 )
			{
				m_pszServiceName = VWEBHYPERLINK_NETSCAPE_STRING;
			}
		}
		else
			m_pszServiceName = NULL;

		return nResult;
	}

	/** Browse pszURL using the browser identified by hWnd. Returns
	BROWSE_ERROR_NO_BROWSER_FOUND if hWnd is not a valid Web Browser
	application. Otherwise return is same as BrowseDDE().*/
	VUINT			BrowseHWND(	VSTRING_CONST	pszURL,
								HWND			hWndWebBrowser)
	{
		VUINT nResult = BROWSE_ERROR_NO_BROWSER_FOUND;

		if ( IsWindowMSIE(hWndWebBrowser) )
			nResult = BrowseDDE(	pszURL,
									VWEBHYPERLINK_IEXPLORE_STRING,
									VFALSE);
		else if ( IsWindowNetscape(hWndWebBrowser) )
			nResult = BrowseDDE(	pszURL,
									VWEBHYPERLINK_NETSCAPE_STRING,
									VFALSE);

		return nResult;
	}

	/** Browse with a known browser. You must pass the URL and the path to
	the web browser to use. This function will return BROWSE_ERROR_NONE if
	successful, BROWSE_ERROR_LAUNCH_FAILED otherwise.*/
	static VUINT	BrowsePath(	VSTRING_CONST pszURL,
								VSTRING_CONST pszBrowserPath)
	{
		/* These must be known!*/
		VASSERT(VSTRLEN_CHECK(pszURL))
		VASSERT(VSTRLEN_CHECK(pszBrowserPath))

		STARTUPINFO			si;
		PROCESS_INFORMATION	pi;
		VZEROSTRUCT(si);
		VZEROSTRUCT(pi);

		/* Initialize process structs.*/
		si.cb =				sizeof(si);
		si.dwFlags =		STARTF_USESHOWWINDOW;
		si.wShowWindow =	SW_SHOWNORMAL;

		/* Build string to send to CreateProcess().*/
		VString strCP(pszBrowserPath);
		strCP += VTEXT(" ");
		strCP += pszURL;

		/* Attempt to launch the browser now.*/
		if (	strCP.IsNotEmpty() &&
				CreateProcess(	NULL,
								strCP,
								NULL,
								NULL,
								FALSE,
								CREATE_DEFAULT_ERROR_MODE |
								NORMAL_PRIORITY_CLASS,
								NULL,
								NULL,
								&si,
								&pi) )
		{
			/* Close handles initialized by CreateProcess().*/
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			return BROWSE_ERROR_NONE;
		}

		return BROWSE_ERROR_LAUNCH_FAILED;
	}

	/** For a given hWnd, find the top level window (main window).*/
	static HWND		GetApplicationWindow(HWND hWnd)
	{
		VASSERT(hWnd && ::IsWindow(hWnd))

		HWND hWndApp =		hWnd;
		HWND hWndParent =	hWnd;

		/* We may have a child window, so keep going up the chain until
		the next parent is NULL.*/
		while ( hWndParent )
		{
			hWndParent = GetParent(hWndParent);

			if ( hWndParent )
				hWndApp = hWndParent;
		}

		return hWndApp;
	}

	/** Find what browser handles HTM files on this system. Returns VTRUE if
	a browser is found, and the program file exists, and copies the full
	path to the browser executable file to strPath. A browser may be
	installed on the system and not be found, so calling Browse() may still
	work. This could be the case when the browser is not installed
	correctly. This function uses
	VShellRegistrar::GetApplicationFromFileExtension() to locate the
	default browser. On a return of VFALSE, strPath may still be set to the
	browser path. In this case, it can be assumed that the browsers
	executable file does not exist at the location specified in the system
	registry. On entry, if strPath is not an empty string, this function
	will not attempt to get the default browser, but will instead check to
	see if the file exists.*/
	static VBOOL	GetDefaultBrowserPath(VString& strPath)
	{
		if (	strPath.IsNotEmpty() ||
				VShellRegistrar::GetApplicationFromFileExtension(
					VTEXT(".htm"),
					strPath) )

			return VDoesFileExist(strPath);

		return VFALSE;
	}

	/** Determine if hWnd is the Netscape Navigator main window. hWnd can be
	a child window of Netscape as well.*/
	static VBOOL	IsWindowNetscape(HWND hWnd)
	{
		/* hWnd must be valid!*/
		VASSERT(hWnd && IsWindow(hWnd))

		/* Copy first.*/
		HWND hWindow = hWnd;

		/* If hWindow is not a top level window, get it.*/
		if ( GetParent(hWindow) )
			hWindow = GetApplicationWindow(hWindow);

		/* Since Navigator-Communicator is MFC code, we can only look at
		the caption bar and hope we find Netscape there. MFC registers
		almost random window class names with the AFX preface. They are not
		the same from run to run.*/
		VString s(hWindow);

		if (	s.UpperCase() &&
				s.Find(VWEBHYPERLINK_NETSCAPE_STRING) != -1 )
			return VTRUE;

		return VFALSE;
	}

	/** Determine if hWnd is the Microsoft Internet Explorer main window.
	hWnd can be a child window of MSIE as well.*/
	static VBOOL	IsWindowMSIE(HWND hWnd)
	{
		/* hWnd must be valid!*/
		VASSERT(hWnd && IsWindow(hWnd))

		/* Copy first.*/
		HWND hWindow = hWnd;

		/* If hWindow is not a top level window, get it.*/
		if ( GetParent(hWindow) )
			hWindow = GetApplicationWindow(hWindow);

		VTCHAR sz[256];

		if ( GetClassName(hWindow, sz, VARRAY_SIZE(sz)) )
		{
			/* These are the known class names of MSIE. Microsoft can't
			decide what they want to call it.*/
			VSTRING_CONST MSIE[] =	{	VTEXT("IExplorer_Frame"),
										VTEXT("Internet Explorer_Frame"),
										VTEXT("IEFrame")
									};

			/* Is the window MSIE? Look at the class name.*/
			for ( VUINT i = 0; i < VARRAY_SIZE(MSIE); i++ )
			{
				if ( VSTRCMP_NOCASE(sz, MSIE[i]) == 0 )
					return VTRUE;
			}
		}

		return VFALSE;
	}

protected:
	/** Embedded Members.*/
	VDWORD			m_nDDE;
	VSTRING_CONST	m_pszServiceName;
};

#endif /* VWEBHYPERLINK*/
