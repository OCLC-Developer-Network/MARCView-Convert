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

VWCL and all source code are copyright (c) 1996-2000 by The VWCL Alliance.*/

#ifndef VFILENAMEMUTEX
#define VFILENAMEMUTEX

#include "../vstandard.h"
#include "../utility/vhandle.hpp"
#include "../strings/vstring.hpp"

/** A file name mutex is one method of protecting access to a file across
many instances of the same application, or multiple applications that use
the same files. It does not provide a means of file locking at the file
system level, which means that different applications could still access
the file. It also cannot enforce a lock that is shared in one thread. For
instance, attempting to lock the same file (by its file name) using 2
objects of this type in the same thread will succeed. The class can only
protect access to a named file between different threads or processes. It
is intended to provide a way for a single application (or applications that
each deal with the same files), possibly running in multiple instances, to
detect when a file that it needs to access is available or not. The class
works by creating a Win32 named mutex from a file name. This named mutex is
unique throughout the system, and since it is a kernel object, it is
available by name to all processes running on the machine.*/
class VFileNameMutex
{
public:
	/** Constructor must be initialized with name of file to create the
	mutex for.*/
	VFileNameMutex(VSTRING_CONST pszFileName)
	{
		/* Initialize members.*/
		m_strFileName = pszFileName;
		m_bLocked =		VFALSE;

		VASSERT(m_strFileName.IsNotEmpty())

		/* Mutex's don't allow backslashes. Convert to _ instead.*/
		m_strFileName.ReplaceCharacters(VTEXT('\\'), VTEXT('_'));

		/* Since mutex naming is case sensitive, always force lowercase.*/
		m_strFileName.LowerCase();

		/* Create the mutex, or open an existing one, but don't
		acquire it right now.*/
		m_Mutex.Set(CreateMutex(NULL, VFALSE, m_strFileName));
		VASSERT(m_Mutex.GetHandle())
	}

	/** Virtual destructor verifies lock is released when object is destroyed.*/
	virtual ~VFileNameMutex()
		{ UnLock(); }

	/** Determine if the file is already locked.*/
	VBOOL		IsLocked() const
		{ return m_bLocked; }

	/** Get the lock. Returns VTRUE if lock was acquired, or if was already
	owned. VFALSE will be returned on error, or if the lock could not be
	set. By default, this function will wait forever for the lock to become
	available. If this is not desired, you can pass the number of seconds
	to wait for the nTimeOutSeconds parameter.*/
	VBOOL		Lock(VUINT nTimeOutSeconds = 0)
	{
		/* Handle attempt to lock twice and failure to initialize the
		class properly.*/
		if (	!IsLocked() &&
				m_strFileName.IsNotEmpty() &&
				m_Mutex.GetHandle() )
		{
			/* Acquire the lock now, and timeout if timeout seconds expire.*/
			if (  WaitForSingleObject(	m_Mutex.GetHandle(),
										(nTimeOutSeconds)
										? 1000 * nTimeOutSeconds
										: INFINITE)
										== WAIT_OBJECT_0)
			{
				m_bLocked = VTRUE;
			}
		}

		return IsLocked();
	}

	/** Release the lock, as needed. If the lock is not currently set, this
	function does nothing. Returns VTRUE if the mutex was unlocked, VFALSE
	otherwise. The call will fail if the thread that created the file lock
	was not the same thread that called this function.*/
	VBOOL		UnLock()
	{
		if (	IsLocked() &&
				VVERIFY_EXPR(ReleaseMutex(m_Mutex.GetHandle())) )
		{
			m_bLocked = VFALSE;
		}

		return (IsLocked()) ? VFALSE : VTRUE;
	}

protected:
	/** Embedded Members.*/
	VString		m_strFileName;
	VHandle		m_Mutex;
	VBOOL		m_bLocked;
};

#endif /* VFILENAMEMUTEX*/
