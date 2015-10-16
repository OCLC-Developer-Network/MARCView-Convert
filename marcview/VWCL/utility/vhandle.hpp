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

#ifndef VHANDLE
#define VHANDLE

#include "../vstandard.h"

/** VHandle stores a Windows HANDLE value, verifying it is closed in
destructor. Attempting to store either NULL or INVALID_HANDLE_VALUE will
cause this class to store NULL. This allows all Windows functions that use
handle values differently to use NULL as an invalid handle value. This
class should only be used by Windows objects whose HANDLE value can and
should be closed with a call to the Win32 API CloseHandle() function.*/
class VHandle
{
public:
	/** Default constructor can be initialized with a valid windows
	HANDLE of any type that can be closed with CloseHandle().*/
	VHandle(HANDLE hHandle = NULL)
		{ m_hHandle = hHandle; }

	/** Virtual destructor verifies the active handle, in any, is
	properly closed.*/
	virtual ~VHandle()
		{ Close(); }

	/** Close the currently wrapped handle, as needed.*/
	void		Close()
	{
		if ( m_hHandle )
		{
			CloseHandle(m_hHandle);
			m_hHandle = NULL;
		}
	}

	/** Return the handle that is currently open by this object.*/
	HANDLE		GetHandle() const
		{ return m_hHandle; }

	/** Set the handle this class is to manage. If a current handle exists,
	it will be closed only if bCloseCurrentHandle is VTRUE, which is the
	default. The return value is the handle managed by the class, or NULL.
	If hHandle is INVALID_HANDLE_VALUE, NULL will stored in this class
	instead. This makes all references to handles that are invalid return
	NULL, not the Windows mix of NULL or INVALID_HANDLE_VALUE.*/
	HANDLE		Set(	HANDLE	hHandle,
						VBOOL	bCloseCurrentHandle = VTRUE)
	{
		if ( bCloseCurrentHandle )
			Close();

		m_hHandle =
			(hHandle == INVALID_HANDLE_VALUE) ? NULL : hHandle;

		return m_hHandle;
	}

protected:
	/** Embedded Members.*/
	HANDLE		m_hHandle;
};

#endif /* VHANDLE*/
