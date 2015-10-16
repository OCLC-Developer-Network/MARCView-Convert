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

#ifndef VSTOCKBRUSH
#define VSTOCKBRUSH

#include "../vstandard.h"
#include "../structures/vrect.hpp"
#include "vdc.hpp"
#include "vgdistockobject.hpp"

/** VStockBrush is a VGDIStockObject that only knows how to deal with
Windows defined stock brushes.*/
class VStockBrush : public VGDIStockObject
{
public:
	/** Default  constructor initializes the class with a Windows stock
	brush object. See the Win32 GetStockObject() function for all possible
	values that can be passed for the nStockBrushID parameter. At the time
	of the last class update these were:

	BLACK_BRUSH - Black brush.

	WHITE_BRUSH - White brush.

	DKGRAY_BRUSH - Dark gray brush.

	DC_BRUSH - Windows 98, Windows NT 5.0 and later: Solid color brush.
	The default color is white. The color can be changed by using the
	SetDCBrushColor function.

	GRAY_BRUSH - Gray brush.

	HOLLOW_BRUSH - Hollow brush (equivalent to NULL_BRUSH).

	LTGRAY_BRUSH - Light gray brush.

	NULL_BRUSH - Null brush (equivalent to HOLLOW_BRUSH).*/
	VStockBrush(VINT nStockBrushID = BLACK_BRUSH)
		: VGDIStockObject(nStockBrushID)
		{ VASSERT_STOCK_BRUSH(nStockBrushID) }

	/** Fills a rectangle by using the brush. This function includes the left
	and top borders, but excludes the right and bottom borders of the
	rectangle. GDI fills a rectangle up to, but not including, the right
	column and bottom row, regardless of the current mapping mode. In
	Windows CE version 1.0, the brush cannot be a color brush. This function
	is the same in Windows CE version 2.0 as it is in Windows desktop
	platforms.*/
	void		FillRect(	VDC&			dc,
							VRect const&	r) const
		{ dc.FillRect(r, (HBRUSH)m_hGDIObject); }

	/** Return a reference to the brush GDI object handle.*/
	HBRUSH&		GetHandle() const
		{ return (HBRUSH&)VGDIStockObject::GetHandle(); }

	/** Get the brush information into a LOGBRUSH structure.*/
	VBOOL		GetInfo(LOGBRUSH& lb) const
		{ return VGDIObject::GetInfo(sizeof(LOGBRUSH), &lb); }
};

#endif /* VSTOCKBRUSH*/
