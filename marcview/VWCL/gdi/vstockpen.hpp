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

#ifndef VSTOCKPEN
#define VSTOCKPEN

#include "../vstandard.h"
#include "vdc.hpp"
#include "vgdistockobject.hpp"

/** VStockPen is a VGDIStockObject that only knows how to deal with Windows
defined stock pens.*/
class VStockPen : public VGDIStockObject
{
public:
	/** Default constructor initializes the class with a Windows stock black
	pen object. See the Win32 GetStockObject() function for all possible
	values that can be passed for the nStockPenID parameter. At the time of
	the last class update these were:

	BLACK_PEN - A black pen.

	WHITE_PEN - A white pen.

	DC_PEN - Windows 98, Windows NT 5.0 and later: Solid pen color. The
	default color is white. The color can be changed by using the
	SetDCPenColor function.*/
	VStockPen(VINT nStockPenID = BLACK_PEN)
		: VGDIStockObject(nStockPenID)
		{ VASSERT_STOCK_PEN(nStockPenID) }

	/** Return a reference to the brush pen object handle.*/
	HPEN&		GetHandle() const
		{ return (HPEN&)VGDIStockObject::GetHandle(); }

	/** Get the pen information into a LOGPEN structure.*/
	VBOOL		GetInfo(LOGPEN& lp) const
		{ return VGDIObject::GetInfo(sizeof(LOGPEN), &lp); }
};

#endif /* VSTOCKPEN*/
