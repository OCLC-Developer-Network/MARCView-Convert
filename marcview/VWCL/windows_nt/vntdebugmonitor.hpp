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

#ifndef VNTDEBUGMONITOR
#define VNTDEBUGMONITOR

#include "../vstandard.h"
#include "../threading/vthread.hpp"
#include "../utility/vhandle.hpp"

/** VNTDebugMonitor monitors (in a thread) the output from the Win32 API
function OutputDebugString(). When it detects a string sent via
OutputDebugStrig(), or the VWCL VODS macro, it will call the OnDebugString()
pure virtual function with the process ID and string. Call Begin() function
to start the monitoring and End() to stop it, or destruct the class. This
class supports only Windows NT. NEVER call the VThread base class Begin() or
End() functions directly, as this could cause thread deadlock.*/
class VNTDebugMonitor : public VThread
{
public:
	/** Return codes from Begin() function.*/
	enum	{		BEGIN_ERROR_NONE,
					BEGIN_ERROR_EVENT_NOT_CREATED,
					BEGIN_ERROR_THREAD_NOT_CREATED,
			};

	/** Default constructor initializes internal members.*/
	VNTDebugMonitor()
		{ m_pSharedMemory = NULL; }

	/** Virtual destructor verifies object and thread cleanup.*/
	virtual ~VNTDebugMonitor()
		{ End(); }

	/** Start the monitoring of OutputDebugString(). If this function
	returns BEGIN_ERROR_EVENT_NOT_CREATED, it means a required event could
	not be set, most likely because one already exists. This code cannot be
	debugged in the Visual C++ IDE because it already creates an event for
	DBWIN_BUFFER_READY. The function will return BEGIN_ERROR_NONE if
	everything is OK.*/
	VUINT			Begin()
	{
		/* Initialize and set up security descriptors.*/
		SECURITY_DESCRIPTOR sd;

		if ( InitializeSecurityDescriptor(	&sd,
											SECURITY_DESCRIPTOR_REVISION) )
		{
			if ( SetSecurityDescriptorDacl(	&sd,
											VTRUE,
											(PACL)NULL,
											VFALSE) )
			{
				/* Initialize security attributes.*/
				SECURITY_ATTRIBUTES sa;
				sa.nLength =				sizeof(SECURITY_ATTRIBUTES);
				sa.bInheritHandle =			VTRUE;
				sa.lpSecurityDescriptor =	&sd;

				/* Make sure last error is 0.*/
				SetLastError(0);

				/* Create the debugging event.*/
				m_handleAckEvent.Set(CreateEvent(
					&sa,
					VFALSE,
					VFALSE,
					VTEXT("DBWIN_BUFFER_READY")));

				/* Make sure it does not already exist (globally in
				the system).*/
				if (	GetLastError() != ERROR_ALREADY_EXISTS &&
						m_handleAckEvent.GetHandle() )
				{
					/* Create the file mapping.*/
					if ( m_handleSharedFile.Set(
						CreateFileMapping(	(HANDLE)-1,
											&sa,
											PAGE_READWRITE,
											0,
											4096,
											VTEXT("DBWIN_BUFFER"))) )
					{
						/* Map view of this file.*/
						m_pSharedMemory =
							MapViewOfFile(	m_handleSharedFile.GetHandle(),
											FILE_MAP_READ,
											0,
											0,
											512);

						if ( m_pSharedMemory )
						{
							/* Create the ready event. This handle will be
							set to NULL when it is time to exit (signal).*/
							if ( m_handleReadyEvent.Set(
								CreateEvent(&sa,
											VFALSE,
											VFALSE,
											VTEXT("DBWIN_DATA_READY"))) )
							{
								if ( SetEvent(m_handleAckEvent.GetHandle()) )
									return	(VThread::Begin())
											? BEGIN_ERROR_NONE
											: BEGIN_ERROR_THREAD_NOT_CREATED;
							}
						}
					}
				}
			}
		}

		/* Cleanup on failure.*/
		End();

		return BEGIN_ERROR_EVENT_NOT_CREATED;
	}

	/** Stop monitoring output from OutputDebugString().*/
	VBOOL			End()
	{
		/* Unmap view of shared memory file?*/
		if ( m_pSharedMemory )
			UnmapViewOfFile(m_handleSharedFile.GetHandle());

		/* Close all open handles.*/
		m_handleAckEvent.	Close();
		m_handleSharedFile.	Close();

		/* Reset shared memory pointer.*/
		m_pSharedMemory =	NULL;

		/* If the ready event exists, signal it now.*/
		if ( m_handleReadyEvent.GetHandle() )
		{
			SetEvent(m_handleReadyEvent.GetHandle());
			m_handleReadyEvent.Close();
		}

		/* Have the main thread end.*/
		return VThread::End();
	}

protected:
	/** The real work gets done on this thread. It is started by the
	Begin() base class function.*/
	virtual void	OnEntryPoint()
	{
		VASSERT(m_pSharedMemory && m_handleAckEvent.GetHandle() && m_handleReadyEvent.GetHandle())

		VSTRING	pszString =
			(VSTRING)m_pSharedMemory + sizeof(VDWORD);

		VDWORD* pnThisPID =
			(VDWORD*)m_pSharedMemory;

		VDWORD nLastPID =
			0xffffffff;

		/* Loop until ShouldExit() returns VTRUE.*/
		while ( !ShouldExit() )
		{
			/* Wait for something to happen.*/
			VDWORD nReturn =
				WaitForSingleObject(m_handleReadyEvent.GetHandle(),
									INFINITE);

			/* If m_pSharedMemory goes to a NULL handle, it is time to exit.*/
			if (	m_pSharedMemory &&
					nReturn == WAIT_OBJECT_0 )
			{
				if ( nLastPID != *pnThisPID )
					nLastPID = *pnThisPID;

				OnDebugString(nLastPID, pszString);

				SetEvent(m_handleAckEvent.GetHandle());
			}
		}
	}

	/** Pure virtual function must be overriden to get string sent by
	OutputDebugString().*/
	virtual void	OnDebugString(	VDWORD			nPID,
									VSTRING_CONST	pszString) = 0;

	/** Embedded Members.*/
	VHandle				m_handleReadyEvent;
	VHandle				m_handleAckEvent;
	VHandle				m_handleSharedFile;
	VVOIDPTR			m_pSharedMemory;
};

#endif /* VNTDEBUGMONITOR*/
