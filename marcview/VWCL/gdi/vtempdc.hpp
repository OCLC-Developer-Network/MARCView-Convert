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

#ifndef VTEMPDC
#define VTEMPDC

#include "../vstandard.h"
#include "vdc.hpp"

/** VTempDC is a VDC object that will never free the wrapped Windows device
context (HDC). This class is mostly useful when you need to pass a VDC
object reference to another VWCL class or function, but when you have only
a HDC device context handle that you, or VWCL, does not own, or wish to not
have deleted by a VDC class.*/
class VTempDC : public VDC
{
public:
	/** Standard constructor must be initialized with an existing (valid)
	device context (HDC).*/
	VTempDC(HDC hDC) : VDC(hDC, VFALSE)
		{ VASSERT(hDC) }

	/** Standard constructor must be initialized with an existing (valid)
	device context object (VDC). When we call the base class constructor,
	we cast away the const-ness because the base class could modify the
	sourceDC if we passed VTRUE for bFreeDC, but we don't, so sourceDC
	really is const.*/
	VTempDC(VDC& sourceDC) : VDC(sourceDC, VFALSE)
		{ VASSERT(sourceDC.GetHandle()) }

	/** Override of base class Free() function that does absolutely nothing.
	Thus, it is safe to call Free() on these objects and not have to worry
	about accidentally causing any harm to the internal HDC member.*/
	virtual void	Free()
		{;}
};

#endif /* VTEMPDC*/
