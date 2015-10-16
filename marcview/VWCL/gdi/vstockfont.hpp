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

#ifndef VSTOCKFONT
#define VSTOCKFONT

#include "../vstandard.h"
#include "vgdistockobject.hpp"

/** VStockFont is a VGDIStockObject that only knows how to deal with
Windows defined stock fonts.*/
class VStockFont : public VGDIStockObject
{
public:
	/** Default  constructor initializes the class with a Windows stock
	brush object. See the Win32 GetStockObject() function for all possible
	values that can be passed for the nStockFontID parameter. At the time
	of the last class update these were:

	ANSI_FIXED_FONT - Windows fixed-pitch (monospace) system font.

	ANSI_VAR_FONT - Windows variable-pitch (proportional space) system font.

	DEVICE_DEFAULT_FONT - Windows NT: Device-dependent font.

	DEFAULT_GUI_FONT - Default font for user interface objects such as
	menus and dialog boxes.

	OEM_FIXED_FONT - Original equipment manufacturer (OEM) dependent
	fixed-pitch (monospace) font.

	SYSTEM_FONT - System font. By default, the system uses the system font
	to draw menus, dialog box controls, and text.

	SYSTEM_FIXED_FONT - Fixed-pitch (monospace) system font. This stock
	object is provided only for compatibility with 16-bit Windows versions
	earlier than 3.0.*/
	VStockFont(VINT nStockFontID = ANSI_VAR_FONT)
		: VGDIStockObject(nStockFontID)
		{ VASSERT_STOCK_FONT(nStockFontID) }

	/** Return a reference to the font GDI object handle.*/
	HFONT&		GetHandle() const
		{ return (HFONT&)VGDIStockObject::GetHandle(); }

	/** Get the font information into a LOGFONT structure.*/
	VBOOL		GetInfo(LOGFONT& lf) const
		{ return VGDIObject::GetInfo(sizeof(LOGFONT), &lf); }
};

#endif /* VSTOCKFONT*/
