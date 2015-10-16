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

#ifndef VGDISTOCKOBJECT
#define VGDISTOCKOBJECT

#include "../vstandard.h"
#include "vtempgdiobject.hpp"

/* Define these if not already done.*/
#ifndef DC_BRUSH
	#define DC_BRUSH 18
#endif

#ifndef DC_PEN
	#define DC_PEN 19
#endif

/** Macro's for debug assertion for invalid stock object ID's.*/
#define VASSERT_STOCK_PEN(nStockPenID)			VASSERT(nStockPenID == BLACK_PEN || nStockPenID == WHITE_PEN || nStockPenID == NULL_PEN || nStockPenID == DC_PEN)
#define VASSERT_STOCK_BRUSH(nStockBrushID)		VASSERT(nStockBrushID == BLACK_BRUSH || nStockBrushID == DKGRAY_BRUSH || nStockBrushID == GRAY_BRUSH || nStockBrushID == HOLLOW_BRUSH || nStockBrushID == LTGRAY_BRUSH || nStockBrushID == NULL_BRUSH || nStockBrushID == WHITE_BRUSH || nStockBrushID == DC_BRUSH)
#define VASSERT_STOCK_FONT(nStockFontID)		VASSERT(nStockFontID == ANSI_FIXED_FONT || nStockFontID == ANSI_VAR_FONT || nStockFontID == DEVICE_DEFAULT_FONT || nStockFontID == DEFAULT_GUI_FONT || nStockFontID == OEM_FIXED_FONT || nStockFontID == SYSTEM_FONT || nStockFontID == SYSTEM_FIXED_FONT)
#define VASSERT_STOCK_PALETTE(nStockPaletteID)	VASSERT(nStockPaletteID == DEFAULT_PALETTE)

/** VGDIStockObject is a VTempGDIObject that only knows how to initialize
with a stock Windows object ID. This class is primarily a base class for
other stock GDI object classes.*/
class VGDIStockObject : public VTempGDIObject
{
public:
	/** Default constructor initializes the class with a Windows stock
	object.*/
	VGDIStockObject(VINT nStockObjectID = BLACK_PEN)
		: VTempGDIObject(GetStockObject(nStockObjectID))
		{;}

	/** Get a stock object handle into this object.*/
	VBOOL			GetStock(VINT nStockObjectID)
		{ return SetHandle(GetStockObject(nStockObjectID)); }
};

#endif /* VGDISTOCKOBJECT*/
