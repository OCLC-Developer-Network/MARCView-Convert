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

#ifndef VLIBRARY
#define VLIBRARY

#include "../vstandard.h"

/** VLibrary supports dynamically loaded DLL's and other executable files.*/
class VLibrary
{
public:
	/** Default constructor initializes internal members and can optionally
	take ownership of a previously opened library if hLibrary is given. If
	not NULL, this class will free the library when appropriate.*/
	VLibrary(HINSTANCE hLibrary = NULL)
		{ m_hInstance = hLibrary; }

	/** Constructor that will attempt to load a library based on
	a file name.*/
	VLibrary(VSTRING_CONST pszFileName)
	{
		m_hInstance = NULL;
		Set(pszFileName);
	}

	/** Virtual destructor verifies the library wrapped by this object
	is free'd.*/
	virtual ~VLibrary()
		{ Free(); }

	/** Free the library currently wrapped by this object, as needed.*/
	void		Free()
	{
		if ( m_hInstance )
		{
			FreeLibrary(m_hInstance);
			m_hInstance = NULL;
		}
	}

	/** Return the instance handle to the library currently wrapped
	by this object.*/
	HINSTANCE	GetHandle() const
		{ return m_hInstance; }

	/** Make the library specified by hLibrary the current libray this
	object wraps. If this object is already associated with a current
	library, it will be free'd first. Calling this function with hLibrary
	set to NULL is the same as calling Free(). Returns the handle to the
	newly wrapped library, or NULL.*/
	HINSTANCE	Set(HINSTANCE hLibrary)
	{
		Free();
		m_hInstance = hLibrary;
		return m_hInstance;
	}

	/** Load the library specified by pszFileName and make it the current
	libray this object wraps. If this object is already associated with a
	current library, it will be free'd first. Calling this function with
	hLibrary set to NULL is the same as calling Free(). Returns the handle
	to the newly wrapped library, or NULL.*/
	HINSTANCE	Set(VSTRING_CONST pszFileName)
	{
		VASSERT(VSTRLEN_CHECK(pszFileName))
		return Set(LoadLibrary(pszFileName));
	}

protected:
	/** Embedded Members.*/
	HINSTANCE	m_hInstance;
};

#endif /* VLIBRARY*/
