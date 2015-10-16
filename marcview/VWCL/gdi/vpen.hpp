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

#ifndef VPEN
#define VPEN

#include "../vstandard.h"
#include "vgdiobject.hpp"

/** VPen is a VGDIObject that encapulates a HPEN GDI object. This class is
used only for custom created pen GDI objects, not stock pen objects. This
class will delete the object during destruction. For Windows defined stock
pen objects, use VStockPen instead.*/
class VPen : public VGDIObject
{
public:
	/** Default  constructor initializes the class with pen creation
	information. All parameters are the same as for the Create() function,
	except bCreate. If bCreate is VTRUE, the pen will be created during
	object construction. The default is to require an additional call to
	Create().*/
	VPen(	VBOOL		bCreate = VFALSE,
			VINT		nStyle = PS_SOLID,
			VINT		nWidth = 1,
			COLORREF	crColor = RGB(0,0,0))
	{
		if ( bCreate )
			Create(nStyle, nWidth, crColor);
	}

	/** Creates a logical pen that has the specified style, width, and
	color. The pen can subsequently be selected into a device context and
	used to draw lines and curves. If a VDC object is passed as pDCSelect,
	the object will be selected into the device context. Values for nStyle
	can be one of the following:

	PS_SOLID - Pen is solid.

	PS_DASH - Pen is dashed. This style is valid only when the pen width
	is one or less in device units.

	PS_DOT - Pen is dotted. This style is valid only when the pen width is
	one or less in device units.

	PS_DASHDOT - Pen has alternating dashes and dots. This style is valid
	only when the pen width is one or less in device units.

	PS_DASHDOTDOT - Pen has alternating dashes and double dots. This style
	is valid only when the pen width is one or less in device units.

	PS_NULL - Pen is invisible.

	PS_INSIDEFRAME - Pen is solid. When this pen is used in any graphics
	device interface (GDI) drawing function that takes a bounding rectangle,
	the dimensions of the figure are shrunk so that it fits entirely in the
	bounding rectangle, taking into account the width of the pen. This
	applies only to geometric pens.

	nWidth specifies the width of the pen, in logical units. If nWidth is
	zero, the pen is a single pixel wide, regardless of the current
	transformation. crColor specifies a color reference for the pen color.
	See the RGB macro for more information. This function returns creates a
	pen with the specified width bit with the PS_SOLID style if you specify
	a width greater than one for the following styles: PS_DASH, PS_DOT,
	PS_DASHDOT, PS_DASHDOTDOT. After an application creates a logical pen,
	it can select that pen into a device context by calling the Select()
	function. After a pen is selected into a device context, it can be used
	to draw lines and curves.If the value specified by the nWidth parameter
	is zero, a line drawn with the created pen will always be a single
	pixel wide regardless of the current transformation. If the value
	specified by nWidth is greater than 1, the nPenStyle parameter must be
	PS_NULL, PS_SOLID, or PS_INSIDEFRAME. If the value specified by nWidth
	is greater than 1 and nPenStyle is PS_INSIDEFRAME, the line associated
	with the pen is drawn inside the frame of all primitives except polygons
	and polylines. If the value specified by nWidth is greater than 1,
	nPenStyle is PS_INSIDEFRAME, and the color specified by the crColor
	parameter does not match one of the entries in the logical palette, the
	system draws lines by using a dithered color. Dithered colors are not
	available with solid pens. When you no longer need the pen, call the
	Delete() function to delete it, or destruct the object. Windows CE only
	supports wide lines for solid pens. Windows CE does not support dotted
	or inside frame pens, nor does it support any user-specified endcap
	styles (for example, PS_ENDCAP_ROUND). This function returns VTRUE if
	the new pen was created, VFALSE otherwise.*/
	VBOOL	Create(	VUINT		nStyle = PS_SOLID,
					VINT		nWidth = 1,
					COLORREF	crColor = RGB(0,0,0))
	{
		LOGPEN lp;
		lp.lopnStyle =		nStyle;
		lp.lopnWidth.x =	nWidth;
		lp.lopnWidth.y =	0;
		lp.lopnColor =		crColor;

		return CreateIndirect(lp);
	}

	/** Same as above, but initializes with a LOGPEN structure.*/
	VBOOL	CreateIndirect(LOGPEN const& LogPen)
		{ return SetHandle(CreatePenIndirect(&LogPen)); }

	/** Return a reference to the pen GDI object handle.*/
	HPEN&	GetHandle() const
		{ return (HPEN&)VGDIObject::GetHandle(); }

	/** Get the pen information into a LOGPEN structure.*/
	VBOOL	GetInfo(LOGPEN& lp) const
		{ return VGDIObject::GetInfo(sizeof(LOGPEN), &lp); }
};

#endif /* VPEN*/
