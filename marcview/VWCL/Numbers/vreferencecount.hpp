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

#ifndef VREFERENCECOUNT
#define VREFERENCECOUNT

#include "../vstandard.h"

/** VReferenceCount maintains a generic reference count, useful for object
usage count tracking.*/
class VReferenceCount
{
public:
	/** Default constructor initializes with an object count of 0, unless
	nStartingCount is specified.*/
	VReferenceCount(VULONG nStartingCount = 0)
		{ m_nReferenceCount = nStartingCount; }

	/** Virtual destructor does nothing.*/
	virtual ~VReferenceCount()
		{;}

	/** Add 1 to the current reference count.
	Returns the new reference count.*/
	VULONG	Add()
		{ return ++m_nReferenceCount; }

	/** Return a reference to the current reference count.*/
	VULONG&	Count() const
		{ return (VULONG&)m_nReferenceCount; }

	/** Subtract 1 from the current reference count.
	Returns the new reference count.*/
	VULONG	Subtract()
	{
		VASSERT(m_nReferenceCount > 0)
		return --m_nReferenceCount;
	}

protected:
	/** Embedded Members.*/
	VULONG	m_nReferenceCount;
};

#endif /* VREFERENCECOUNT*/
