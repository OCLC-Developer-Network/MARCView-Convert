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

#ifndef VBRUSH
#define VBRUSH

#include "../vstandard.h"
#include "vgdiobject.hpp"
#include "vbitmap.hpp"

/** VBrush is a VGDIObject that encapulates a HBRUSH GDI object. This class
is used only for custom created brush GDI objects, not stock brush objects.
This base class will delete the object during destruction. For Windows
defined stock brush objects, use VStockBrush instead. Because of the many
ways to create a brush, this class does not have a constructor that
initializes the class. You must use one of the CreateXXX() functions to
initialize to a valid brush.*/
class VBrush : public VGDIObject
{
public:
	/** Creates a logical brush that has the pattern specified by the
	specified device-independent bitmap (DIB). The brush can subsequently
	be selected into any device context that is associated with a device
	that supports raster operations. hGlobal is a handle to a global memory
	object containing a packed DIB, which consists of a BITMAPINFO structure
	immediately followed by an array of bytes defining the pixels of the
	bitmap. For Windows 95 and Windows 98, creating brushes from bitmaps or
	DIBs larger than 8x8 pixels is not supported. If a larger bitmap is
	specified, only a portion of the bitmap is used. nColorSpec specifies
	whether the bmiColors member of the BITMAPINFO structure is initialized
	and, if so, whether this member contains explicit red, green, blue (RGB)
	values or indices into a logical palette. The nColorSpec parameter must
	be one of the following values:

	DIB_PAL_COLORS - A color table is provided and consists of an array of
	16-bit indices into the logical palette of the device context into
	which the brush is to be selected.

	DIB_RGB_COLORS - A color table is provided and contains literal RGB
	values. When an application selects a two-color DIB pattern brush into
	a monochrome device context, the system does not acknowledge the colors
	specified in the DIB; instead, it displays the pattern brush using the
	current background and foreground colors of the device context. Pixels
	mapped to the first color of the DIB (offset 0 in the DIB color table)
	are displayed using the foreground color; pixels mapped to the second
	color (offset 1 in the color table) are displayed using the background
	color. This function returns VTRUE if the new brush was created, VFALSE
	otherwise.*/
	VBOOL			CreateDIBPattern(	HGLOBAL	hGlobal,
										VUINT	nColorSpec = DIB_RGB_COLORS)
	{
		VASSERT(hGlobal)

		if ( hGlobal )
			return SetHandle(CreateDIBPatternBrush(hGlobal, nColorSpec));

		return VFALSE;
	}

	/** Creates a logical brush that has the pattern specified by the
	device-independent bitmap (DIB). pPackedDIB is a pointer to a packed
	DIB consisting of a BITMAPINFO structure immediately followed by an
	array of bytes defining the pixels of the bitmap. For Windows 95 and
	Windows 98, creating brushes from bitmaps or DIBs larger than 8x8
	pixels is not supported. If a larger bitmap is specified, only a portion
	of the bitmap is used. nUsage specifies whether the bmiColors member of
	the BITMAPINFO structure contains a valid color table and, if so,
	whether the entries in this color table contain explicit red, green,
	blue (RGB) values or palette indices. The nUsage parameter must be one
	of the following values:

	DIB_PAL_COLORS - A color table is provided and consists of an array of
	16-bit indices into the logical palette of the device context into
	which the brush is to be selected.

	DIB_RGB_COLORS - A color table is provided and contains literal RGB
	values.

	Windows CE version 1.0 does not support the DIB_PAL_COLORS flag for the
	nUsage parameter. In Windows CE version 2.0, set the nUsage parameter
	to DIB_RGB_COLORS. When an 8 bpp bitmap is used, you can set nUsage to
	DIB_PAL_COLORS, however, in that case, Windows CE will ignore the
	values in the bmiColors array member of the BITMAPINFO structure. This
	function returns VTRUE if the new brush was created, VFALSE otherwise.*/
	VBOOL			CreateDIBPatternPt(	VVOIDPTR_CONST	pPackedDIB,
										VUINT			nUsage =
														DIB_RGB_COLORS)
	{
		VASSERT(pPackedDIB)

		if ( pPackedDIB )
			return SetHandle(CreateDIBPatternBrushPt(pPackedDIB, nUsage));

		return VFALSE;
	}

	/** Creates a logical brush that has the specified hatch pattern and
	color. nStyle can be one of the following values:

	HS_BDIAGONAL - 45-degree downward left-to-right hatch.

	HS_CROSS - Horizontal and vertical crosshatch.

	HS_DIAGCROSS - 45-degree crosshatch.

	HS_FDIAGONAL - 45-degree upward left-to-right hatch.

	HS_HORIZONTAL - Horizontal hatch.

	HS_VERTICAL - Vertical hatch.

	crColor specifies the foreground color of the brush that is used for
	the hatches. See the RGB macro for more information on COLORREF
	values. This function returns VTRUE if the new brush was created,
	VFALSE otherwise.*/
	VBOOL			CreateHatch(	VINT		nStyle = HS_BDIAGONAL,
									COLORREF	crColor = RGB(0,0,0))
		{ return SetHandle(CreateHatchBrush(nStyle, crColor)); }

	/** Same as above, but accepts red, green, and blue values to build a
	COLORREF value from.*/
	VBOOL			CreateHatch(	VINT	nStyle = HS_BDIAGONAL,
									VBYTE	nRedPart = 0,
									VBYTE	nGreenPart = 0,
									VBYTE	nBluePart = 0)
		{ return CreateHatch(nStyle, RGB(nRedPart, nGreenPart, nBluePart)); }

	/** Creates a logical brush with the specified bitmap pattern. The bitmap
	cannot be a DIB section bitmap, which is created by the CreateDIBSection()
	function. bitmap is the bitmap object to be used to create the logical
	brush. For Windows 95 and Windows 98, creating brushes from bitmaps or
	DIBs larger than 8x8 pixels is not supported. If a larger bitmap is
	specified, only a portion of the bitmap is used. This function returns
	VTRUE if the new brush was created, VFALSE otherwise.*/
	VBOOL			CreatePattern(VBitmap const& bitmap)
		{ return SetHandle(CreatePatternBrush(bitmap.GetHandle())); }

	/** Create a solid brush with the color specified in crColor. See the
	RGB macro for more information on COLORREF values. This function returns
	VTRUE if the new brush was created, VFALSE otherwise.*/
	VBOOL			CreateSolid(COLORREF crColor)
		{ return SetHandle(CreateSolidBrush(crColor)); }

	/** Same as above, but accepts red, green, and blue values to build a
	COLORREF value from.*/
	VBOOL			CreateSolid(	VBYTE nRedPart = 0,
									VBYTE nGreenPart = 0,
									VBYTE nBluePart = 0)
		{ return CreateSolid(RGB(nRedPart, nGreenPart, nBluePart)); }

	/** Fills a rectangle by using the brush. This function includes the
	left and top borders, but excludes the right and bottom borders of the
	rectangle. GDI fills a rectangle up to, but not including, the right
	column and bottom row, regardless of the current mapping mode. In
	Windows CE version 1.0, the brush cannot be a color brush. This function
	is the same in Windows CE version 2.0 as it is in Windows desktop
	platforms.*/
	void			FillRect(	VDC&			dc,
								VRect const&	r) const
		{ dc.FillRect(r, (HBRUSH)m_hGDIObject); }

	/** Return a reference to the brush GDI object handle.*/
	HBRUSH&			GetHandle() const
		{ return (HBRUSH&)VGDIObject::GetHandle(); }

	/** Get the brush information into a LOGBRUSH structure.*/
	VBOOL			GetInfo(LOGBRUSH& lb) const
		{ return VGDIObject::GetInfo(sizeof(LOGBRUSH), &lb); }
};

#endif /* VBRUSH*/
