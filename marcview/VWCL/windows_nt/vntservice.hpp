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

#ifndef VNTSERVICE
#define VNTSERVICE

#include "../vstandard.h"
#include "../vapplication.hpp"
#include "../strings/vstring.hpp"
#include "vnteventlog.hpp"

/** VNTService encapuslates writing services for Windows NT. Unlike other
classes in VWCL, it does present an actual framework for NT Services. Usage
of this class makes writing these services almost trivial, at least with
regards to the normal boilerplate code of installation, un-install, startup,
and shut down. Derived from VNTEventLog, services derived from this class
have built in logging ability. Calling code is required by the C++ spec
(currently) to initialize the static pseudo this pointer at global scrope
as follows:

VNTService* VNTService::m_pThis = NULL;

See the VNTServiceTest sample application for more information. Because of
the requirement to keep a single, static pseudo this pointer, an application
that is a service can only contain one service per process. In addition to
"being" a service, this class can also be used to control one.*/
class VNTService : public VNTEventLog
{
public:
	/** Event Messages. You should use the message compiler to build strings
	for each of these. The strings must have the same numbers as this
	enumeration. You can also use the default message table in the
	vntservice.mc file, which is included in the "resources" source tree.
	You must run the MC file through the message compiler to generated a
	RC file that contains these messages. This RC file is then included with
	your project. This is normally a resource or compile time include called
	vntservice.rc. EM_USER_STRING is a special case. Use it when you want
	to simply log a single string that is not defined in the message table
	to the NT Event Log.*/
	enum	{	EM_SERVICE_INSTALLED =		1,
				EM_SERVICE_NOT_INSTALLED =	2,
				EM_SERVICE_UNINSTALLED =	3,
				EM_SERVICE_NOT_REMOVED =	4,
				EM_HANDLER_FUNCTION_FAILURE=5,
				EM_WAITING_FOR_SCM =		6,
				EM_SERVICE_START_PENDING =	7,
				EM_SERVICE_STARTED =		8,
				EM_SERVICE_FAILED_TO_START=	9,
				EM_SERVICE_RUNNING =		10,
				EM_SERVICE_STOP_PENDING =	11,
				EM_SERVICE_STOPPED =		12,
				EM_SERVICE_CONTINUE_PENDING=13,
				EM_SERVICE_PAUSE_PENDING =	14,
				EM_SERVICE_PAUSED =			15,
				EM_USER_STRING =			16,
			};

    /** Default constructor can initialize the service name, types of control
	accepted from the NT Service Control Manager, the type of service, and
	the location of Event Log message resources. See vNTEventLog for very
	important information on the hMessageResources parameter.*/
	VNTService(	VSTRING_CONST	pszServiceName = NULL,
				VDWORD			nControlsAccepted =
									SERVICE_ACCEPT_STOP |
									SERVICE_ACCEPT_SHUTDOWN,
				VDWORD			nServiceType =
									SERVICE_WIN32_OWN_PROCESS,
				HINSTANCE		hMessageResources = NULL)
	{
		/* Copy the address of the current object so we can
		access it from the static member callback functions.*/
		m_pThis = this;

		/* Initialize class members.*/
		VZEROSTRUCT(m_Status);
		m_hServiceStatus =				NULL;
		m_nControlsAccepted =			nControlsAccepted;
		m_Status.dwServiceType =		nServiceType;
		m_Status.dwCurrentState =		SERVICE_STOPPED;
		m_Status.dwControlsAccepted =	nControlsAccepted;

		/* Initialize now?*/
		if ( pszServiceName )
			ServiceName(pszServiceName, hMessageResources);
	}

	/** Virtual destructor does nothing.*/
	virtual ~VNTService()
		{;}

	/** Return the service status handle for this service.*/
	SERVICE_STATUS_HANDLE	GetServiceStatusHandle() const
		{ return m_hServiceStatus; }

	/** Return a reference to the service status structure.*/
	SERVICE_STATUS&			GetServiceStatusStruct() const
		{ return (SERVICE_STATUS&)m_Status; }

	/** Install and Uninstall the service and write actions to the event
	log. If pszFullPath is given, it will be set as the path, otherwise the
	path will be retreived from the module name. The default command line
	processing calls this function with pszLogonAs and pszLogonPassword set
	to NULL. Override this function, and when bInstall is VTRUE, get the
	logon as and password if needed, and then call the base class function.
	pszLogonAs can be either a DomainName\UserName or just the UserName, in
	which case this function will prepend .\ which is required by the
	CreateService() API function. Returns VTRUE on success, VFALSE on
	failure.*/
	virtual VBOOL			Install(	VBOOL			bInstall =
															VTRUE,
										VDWORD			nStartType =
															SERVICE_AUTO_START,
										VSTRING_CONST	pszFullPath =
															NULL,
										VSTRING_CONST	pszLogonAs =
															NULL,
										VSTRING_CONST	pszLogonPassword =
															NULL,
										VBOOL			bStartAfterInstall =
															VTRUE,
										VBOOL			bSilentMode =
															VFALSE)
	{
		/* Service name should not be empty!*/
		VASSERT(m_strServiceName.IsNotEmpty())

		/* Assume failure.*/
		VBOOL bSuccess = VFALSE;

		/* Open the Service Control Manager.*/
		SC_HANDLE hSCM =
			OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

		if ( hSCM )
		{
			if ( bInstall )
			{
				/* Verify un-installed, first (silent mode).*/
				Install(VFALSE, 0, NULL, NULL, NULL, VFALSE, VTRUE);

				/* Get this executables file path (unless given
				in pszFullPath).*/
				VString sPath;

				if ( !pszFullPath )
					sPath.GetModulePath(VFALSE, VFALSE);
				else
					sPath = pszFullPath;

				if ( sPath.IsNotEmpty() )
				{
					/* If pszLogonAs is not NULL, verify domain name is
					present, and if not, .\ is present. If neither is the
					case, we will modify the logon as to be correct.*/
					VString strLogonAs;

					if ( pszLogonAs )
					{
						/* If we find a \ assume correct DomainName\UserName
						string.*/
						if ( !VSTRCHR(pszLogonAs, VTEXT('\\')) )
						{
							/* Domain Name not given, so does the
							string already contain .\ sequence as the
							first chars?*/
							if (	!(*pszLogonAs == VTEXT('.') &&
									*(pszLogonAs + 1) == VTEXT('\\')) )
							{
								strLogonAs = VTEXT(".\\");
								strLogonAs += pszLogonAs;
								pszLogonAs = strLogonAs;
							}
						}
					}

					/* We want no error condition now.*/
					SetLastError(0);

					/* Create the service.*/
					SC_HANDLE hService =
						CreateService(	hSCM,
										m_strServiceName,
										m_strServiceName,
										SERVICE_ALL_ACCESS,
										m_Status.dwServiceType,
										nStartType,
										SERVICE_ERROR_NORMAL,
										sPath,
										NULL,
										NULL,
										NULL,
										pszLogonAs,
										pszLogonPassword);

					if (	hService ||
							GetLastError() == ERROR_SERVICE_EXISTS )
					{
						/* Make or verify registry entries to
						support event logging.*/
						bSuccess =
							(m_hEventSource)
							? VTRUE
							: Register(m_strServiceName);

						if (	hService &&
								bSuccess &&
								bStartAfterInstall )
						{
							bSuccess =
								StartService(hService, 0, NULL);

							/* If service fails to start now, un-install it.*/
							if ( !bSuccess )
							{
								/* Close handle first.*/
								CloseServiceHandle(hService);
								hService = NULL;

								/* Un-Install in silent mode.*/
								Install(	VFALSE,
											0,
											NULL,
											NULL,
											NULL,
											VFALSE,
											VTRUE);
							}
						}

						if ( hService )
							CloseServiceHandle(hService);
					}
				}

				/* Log what happened if we can.*/
				LogEvent(	(bSuccess)
							? EM_SERVICE_INSTALLED
							: EM_SERVICE_NOT_INSTALLED,
							(bSuccess)
							? VFALSE
							: VTRUE);

				/* Notify via virtual function.*/
				OnInstall(bSuccess, bSilentMode);
			}
			else
			{
				SC_HANDLE hService =
					OpenService(	hSCM,
									m_strServiceName,
									SERVICE_QUERY_STATUS | DELETE);

				if ( hService )
				{
					/* If the service is running, stop it.*/
					SERVICE_STATUS ss;

					if ( QueryServiceStatus(hService, &ss) )
					{
						if ( ss.dwCurrentState != SERVICE_STOPPED )
						{
							/* Get a handle we can use to
							stop the service.*/
							SC_HANDLE hStopService =
								OpenService(	hSCM,
												m_strServiceName,
												SERVICE_STOP);

							if ( hStopService )
							{
								SERVICE_STATUS ss;
								ControlService(	hStopService,
												SERVICE_CONTROL_STOP, &ss);

								CloseServiceHandle(hStopService);
							}
						}
					}

					bSuccess =
						(hService && DeleteService(hService))
						? VTRUE
						: VFALSE;

					CloseServiceHandle(hService);
				}
				else
					bSuccess = VTRUE;

				/* Remove registry entries on success.*/
				if ( bSuccess )
				{
					VRegistry reg;

					if ( reg.CreateKey(GetRegistryKey(), HKEY_LOCAL_MACHINE) )
						reg.DeleteSubKeyNT(m_strServiceName);
				}

				/* Log what happened if we can.*/
				LogEvent(	(bSuccess)
							? EM_SERVICE_UNINSTALLED
							: EM_SERVICE_NOT_REMOVED,
							(bSuccess)
							? VFALSE
							: VTRUE);

				/* Notify via virtual function.*/
				OnUnInstall(bSuccess, bSilentMode);
			}

			CloseServiceHandle(hSCM);
		}

		return bSuccess;
	}

	/** Test if the service is currently installed.*/
	VBOOL					IsInstalled() const
	{
		/* Service name should not be empty!*/
		VASSERT(m_strServiceName.IsNotEmpty())

		SC_HANDLE hService = NULL;

		/* Open the Service Control Manager.*/
		SC_HANDLE hSCM =
			OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

		if ( hSCM )
		{
			/* Try to open the service.*/
			hService =
				OpenService(hSCM, m_strServiceName, SERVICE_QUERY_CONFIG);

			/* Close service if found.*/
			if ( hService )
				CloseServiceHandle(hService);

			CloseServiceHandle(hSCM);
		}

		return (hService) ? VTRUE : VFALSE;
	}

	/** This function helps with determining what a service application
	should do at startup. Early in the execution of the application, a call
	should be made to this function to test for command line parameters. If
	this function determines that the service should start, it will return
	VTRUE. Otherwise, it means the user wanted to only install or un-install
	the service, or show text mode help. Command line parameters that this
	function understands are as follows:

	-I = Install the Service. Calls the Install(VTRUE) virtual function.

	-U = Uninstall the Service. Calls the Install(VFALSE) virtual function.

	-? = Calls the OnHelp() virtual function to show text mode help.

	If any other parameters are specified, this function will call
	the virtual function OnCommandLineParameter().*/
	VBOOL					ProcessCommandLine()
	{
		VBOOL bServiceShouldStart = VTRUE;

		/* See if at least 1 command line param is present.*/
		if ( VGetARGC() > 0 )
		{
			VApplication& theApp = VGetApp();

			if ( theApp.IsCommandLineOption(VTEXT("I")) )
			{
				Install(VTRUE);
				bServiceShouldStart = VFALSE;
			}
			else if ( theApp.IsCommandLineOption(VTEXT("U")) )
			{
				Install(VFALSE);
				bServiceShouldStart = VFALSE;
			}
			else if ( theApp.IsCommandLineOption(VTEXT("?")) )
			{
				OnHelp();
				bServiceShouldStart = VFALSE;
			}

			if ( bServiceShouldStart )
			{
				for ( VUINT i = 1; i < VGetARGC(); i++ )
				{
					if ( !OnCommandLineParameter(VGetARGV(i)) )
						bServiceShouldStart = VFALSE;
				}
			}
		}

		return bServiceShouldStart;
	}

	/** Return the name of this service.*/
	VSTRING_CONST			ServiceName() const
		{ return m_strServiceName; }

	/** Set the service name. This function also initializes the NT Event
	Log and base class. Return return value is VTRUE as long as Service
	Name string was allocated. See VNTEventLog for very important
	information on the hMessageResources parameter. If pszServiceName is
	NULL, VGetAppTitle() will be used. Returns VTRUE on success, VFALSE on
	failure. If the VNTEventLog base class has already been initialized
	with a previous call to VNTEventLog::Register() this function will not
	attempt to re-register. It will use the previously registered event
	handle to log messages.*/
	VBOOL					ServiceName(VSTRING_CONST	pszServiceName,
										HINSTANCE		hMessageResources =
														NULL)
	{
		/* Save name.*/
		m_strServiceName.String(	(pszServiceName)
									? pszServiceName
									: VGetAppTitle());

		/* Name is required!*/
		VASSERT(m_strServiceName.IsNotEmpty())

		/* Verify event logging is enabled.*/
		if ( !m_hEventSource )
			Register(m_strServiceName, hMessageResources);

		return m_strServiceName.IsNotEmpty();
	}

	/** Set the current state status of the service and write to event log.*/
	virtual void			SetStatus(VDWORD nState)
	{
		/* Did the state change? If not, skip.*/
		if ( nState != m_Status.dwCurrentState )
		{
			/* Set internal status.*/
			m_Status.dwCurrentState = nState;

			/* If handle is known, tell NT about our status.*/
			if ( m_hServiceStatus )
				SetServiceStatus(m_hServiceStatus, &m_Status);

			VDWORD nEventID = 0;

			switch ( nState )
			{
				case SERVICE_STOPPED:
				{
					nEventID = EM_SERVICE_STOPPED;

					/* Set final state.*/
					m_Status.dwWin32ExitCode =	GetLastError();
					m_Status.dwCheckPoint =		0;
					m_Status.dwWaitHint =		0;

					/* Service status handle is now NULL.*/
					m_hServiceStatus = NULL;

					break;
				}

				case SERVICE_START_PENDING:
					nEventID = EM_SERVICE_START_PENDING;
					break;

				case SERVICE_STOP_PENDING:
					nEventID = EM_SERVICE_STOP_PENDING;
					break;

				case SERVICE_RUNNING:
					nEventID = EM_SERVICE_RUNNING;
					break;

				case SERVICE_CONTINUE_PENDING:
					nEventID = EM_SERVICE_CONTINUE_PENDING;
					break;

				case SERVICE_PAUSE_PENDING:
					nEventID = EM_SERVICE_PAUSE_PENDING;
					break;

				case SERVICE_PAUSED:
					nEventID = EM_SERVICE_PAUSED;
					break;
			}

			/* Write to event log?*/
			if ( nEventID )
				LogEvent(nEventID, VFALSE);
		}
	}

	/** Call to start the service application running in normal mode. This
	should be called as soon as possible when a service application is
	starting. NT's Service Control Manager won't wait around very long for
	a service application to start talking to it. This is how we talk to
	SCM.*/
	VBOOL					Start()
	{
		/* Service name should not be empty!*/
		VASSERT(m_strServiceName.IsNotEmpty())

		/* Start the initialization.*/
		SetStatus(SERVICE_START_PENDING);

		LogEvent(EM_WAITING_FOR_SCM, VFALSE);

		SERVICE_TABLE_ENTRY st[] =
			{	{m_strServiceName.String(), ServiceMain},
				{NULL, NULL}
			};

		return (StartServiceCtrlDispatcher(st)) ? VTRUE : VFALSE;
	}

protected:
    /** Called by base class when an unknown argument is passed on the
	command line. Return VTRUE to have ProcessCommandLine() return VTRUE to
	start the service, or return VFALSE to return VFALSE from
	ProcessCommandLine() and not start the service.*/
	virtual VBOOL			OnCommandLineParameter(VSTRING_CONST pszCommand)
		{ return VTRUE; }

	/** Override in derived class to show help (usually text mode).*/
	virtual void			OnHelp()
		{;}

	/** Called by base class when attempt to install the service succeeds or
	fails. If bSilentMode is VTRUE it means the function was invoked
	internally (like un-install before a fresh install), and you probably
	don't want to notify the user because they could get confused by
	conflicting messages.*/
	virtual void			OnInstall(	VBOOL bSuccess,
										VBOOL bSilentMode)
		{;}

	/** Override this function in derived class to do real work, otherwise
	your service will do nothing. It is not pure virtual since this class
	can be used to control services in addition to "being" a service.*/
	virtual void			OnRun()
		{;}

	/** Called by base class when attempt to un-install the service succeeds
	or fails. If bSilentMode is VTRUE it means the function was invoked
	internally (like un-install before a fresh install), and you probably
	don't want to notify the user because they could get confused by
	conflicting messages.*/
	virtual void			OnUnInstall(	VBOOL bSuccess,
											VBOOL bSilentMode)
		{;}

    /** NT's Service Control Manager is asking this service to continue
	after being in another state. The default implementation of this
	function sets the new state to the proposed state.*/
	virtual VDWORD			OnContinue(	VDWORD nCurrentState,
										VDWORD nProposedState)
		{ return nProposedState; }

    /** NT's Service Control Manager is interogating this service.*/
	virtual void			OnInterrogate()
		{;}

    /** NT's Service Control Manager is asking this service to pause after
	being in another state. The default implementation of this function
	sets the new state to the proposed state.*/
    virtual VDWORD			OnPause(	VDWORD nCurrentState,
										VDWORD nProposedState)
		{ return nProposedState; }

    /** NT's Service Control Manager is asking this service to shutdown after
	being in another state. The default implementation of this function
	sets the new state to the proposed state.*/
    virtual VDWORD			OnShutdown(	VDWORD nCurrentState,
										VDWORD nProposedState)
		{ return nProposedState; }

    /** NT's Service Control Manager is asking this service to stop after
	being in another state. The default implementation of this function
	sets the new state to the proposed state.*/
	virtual VDWORD			OnStop(	VDWORD nCurrentState,
									VDWORD nProposed)
	{
		SetStatus(SERVICE_STOP_PENDING);
		return nProposed;
	}

    /** NT's Service Control Manager is notifying this service of a
	user defined control. Only you know what this means.*/
    virtual void			OnUserControl(VDWORD nOpcode)
		{;}

	/** Initial entry point passed to Service Control Manager.*/
	static void WINAPI		ServiceMain(	VDWORD nArgc,
											VSTRING* pszArgv)
	{
		/* Service Name string should not be empty!*/
		VASSERT(m_pThis->m_strServiceName.IsNotEmpty())

		m_pThis->m_hServiceStatus =
			RegisterServiceCtrlHandler(m_pThis->m_strServiceName, SCMHandler);

		if ( m_pThis->m_hServiceStatus )
		{
			/* Tell the service manager we are running.*/
			m_pThis->SetStatus(SERVICE_RUNNING);

			/* OnRun() is where the real work is performed in derived class.*/
			m_pThis->OnRun();
		}
		else
			m_pThis->LogEvent(EM_HANDLER_FUNCTION_FAILURE, VTRUE);
	}

	/** "Controller" function called by Service Control Manager. This
	function calls the Onxxx handler functions and SetStatus() on their
	return value if changed.*/
	static void WINAPI		SCMHandler(VDWORD nOpcode)
	{
		/* Save the current state.*/
		VDWORD nNewState = m_pThis->m_Status.dwCurrentState;

		switch ( nOpcode )
		{
			case SERVICE_CONTROL_STOP:
				nNewState =
					m_pThis->OnStop(	m_pThis->m_Status.dwCurrentState,
										SERVICE_STOPPED);
				break;

			case SERVICE_CONTROL_PAUSE:
				nNewState =
					m_pThis->OnPause(	m_pThis->m_Status.dwCurrentState,
										SERVICE_PAUSED);
				break;

			case SERVICE_CONTROL_CONTINUE:
				nNewState =
					m_pThis->OnContinue(m_pThis->m_Status.dwCurrentState,
										SERVICE_RUNNING);
				break;

			case SERVICE_CONTROL_INTERROGATE:
				m_pThis->OnInterrogate();
				break;

			case SERVICE_CONTROL_SHUTDOWN:
				nNewState =
					m_pThis->OnShutdown(m_pThis->m_Status.dwCurrentState,
										SERVICE_STOPPED);
				break;

			default:
				m_pThis->OnUserControl(nOpcode);
				break;
		}

		/* Migrate state, as needed.*/
		m_pThis->SetStatus(nNewState);
	}

	/** Embedded Members.*/
	VString					m_strServiceName;
	VDWORD					m_nControlsAccepted;
	SERVICE_STATUS_HANDLE	m_hServiceStatus;
	SERVICE_STATUS			m_Status;
	static VNTService*		m_pThis;
};

#endif /* VNTSERVICE*/
