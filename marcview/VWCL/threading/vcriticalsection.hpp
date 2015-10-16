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

#ifndef VCRITICALSECTION
#define VCRITICALSECTION

#include "../vstandard.h"

/** Prototype for TryEnterCriticalSection(). This function is resolved at
runtime to determine if the OS supports it.*/
typedef VWINBOOL
	(WINAPI* VTRY_ENTER_CRITICAL_SECTION)(LPCRITICAL_SECTION pCriticalSection);

/** VCriticalSection provides thread synchronization within the same process.
It is a simple wrapper around the Win32 critical section functions. Please
see the appropriate Win32 thread syncronization functions for more
information about critical sections and other syncronization methods.*/
class VCriticalSection
{
public:
	/** Default constructor initializes the critical section and
	internal member variables.*/
	VCriticalSection()
	{
		m_pfnTryEnterCriticalSection = (VTRY_ENTER_CRITICAL_SECTION)1;
		InitializeCriticalSection(&m_CriticalSection);
	}

	/** Virtual destructor deletes the internal critical section object.*/
	virtual ~VCriticalSection()
		{ DeleteCriticalSection(&m_CriticalSection); }

	/** Enter the critical section. This acquires the lock on the object,
	providing access to the protected code by only one thread in the
	process. This function will wait until another thread that has the
	critical section locked releases its lock. If no other thread has the
	critical section locked, this function will return immediately.*/
	void	Enter()
		{ EnterCriticalSection(&m_CriticalSection); }

	/** Leave the critical section, making it available for entry by another
	thread.*/
	void	Leave()
		{ LeaveCriticalSection(&m_CriticalSection); }

	/** Try to enter the critical section, returning immediately if not
	possible. If bWaitToEnterIfNeeded is VTRUE, then Enter() will be called
	before returning. This function is only supported when the underlying OS
	supports the feature. If the OS does not, this function acts like
	Enter() if bWaitToEnterIfNeeded is VTRUE, otherwise it returns VFALSE.
	In all cases, a VTRUE return means the critical section was entered,
	VFALSE means it was not.*/
	VBOOL	TryEnter(VBOOL bWaitToEnterIfNeeded = VFALSE)
	{
		/* Resolve function pointer if set to 1.*/
		if ( m_pfnTryEnterCriticalSection == (VTRY_ENTER_CRITICAL_SECTION)1 )
			m_pfnTryEnterCriticalSection = (VTRY_ENTER_CRITICAL_SECTION)
				GetProcAddress(	GetModuleHandle("KERNEL32"),
								"TryEnterCriticalSection");

		/* Use TryEnterCriticalSection() if available.*/
		if ( m_pfnTryEnterCriticalSection )
			return	(m_pfnTryEnterCriticalSection(&m_CriticalSection))
					? VTRUE : VFALSE;
		else if ( bWaitToEnterIfNeeded )
		{
			Enter();
			return VTRUE;
		}

		return VFALSE;
	}

protected:
	/** Embedded Members.*/
	CRITICAL_SECTION			m_CriticalSection;
	VTRY_ENTER_CRITICAL_SECTION	m_pfnTryEnterCriticalSection;
};

#endif	/* VCRITICALSECTION*/
