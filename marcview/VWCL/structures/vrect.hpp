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

#ifndef VRECT
#define VRECT

#include "../vstandard.h"
#include "vpoint.hpp"

/** Macro to create an empty VRect object (coordinates of 0, 0, 0, 0).*/
#define VRECT_EMPTY		VRect(0,0,0,0)

/** Macro to retrieve the left, top, right, and bottom coordinates
of a VRect object.*/
#define VRECT_BREAK(r)	r.GetLeft(), r.GetTop(), r.GetRight(), r.GetBottom()

/** Macro to retrieve the left, top, and width and height coordinates
of a VRect object.*/
#define VRECT_BREAK_WIDTH_HEIGHT(r)	r.GetLeft(), r.GetTop(), r.GetWidth(), r.GetHeight()

/** VRect provides a wrapper with enhancements to the RECT structure.*/
class VRect
{
public:
	/** Construct with existing X, Y, and CX, CY coordinates, or default
	of 0, 0, 0, 0.*/
	VRect(	VLONG nx = 0,
			VLONG ny = 0,
			VLONG ncx = 0,
			VLONG ncy = 0)
		{ Set(nx, ny, ncx, ncy); }

	/** Construct with existing VRect object.*/
	VRect(VRect const& r)
		{ Set(r); }

	/** Construct by converting from an existing RECT structure.*/
	VRect(RECT const& r)
		{ Set(r); }

	/** Virtual destructor does nothing.*/
	virtual ~VRect()
		{;}

	/** Same as calling IsEqual() to determine if an existing VRect
	object has the same coordinates as this object.*/
	VBOOL operator	==(VRect const& r)
		{ return IsEqual(r); }

	/** Same as calling !IsEqual() to determine if an existing VRect
	object does not have the same coordinates as this object.*/
	VBOOL operator	!=(VRect const& r)
		{ return !IsEqual(r); }

	/** Same as calling Set() to set the coordinates of this object
	to those of an existing VRect object.*/
	void operator =(VRect const& r)
		{ Set(r); }

	/** Add or subtract nDistance to or from the left coordinate of
	the rectangle.*/
	void		AddLeft(VLONG nDistance)
		{ m_Rect.left += nDistance; }

	/** Add or subtract nDistance to or from the top coordinate of
	the rectangle.*/
	void		AddTop(VLONG nDistance)
		{ m_Rect.top += nDistance; }

	/** Add or subtract nDistance to or from the right coordinate of
	the rectangle.*/
	void		AddRight(VLONG nDistance)
		{ m_Rect.right += nDistance; }

	/** Add or subtract nDistance to or from the bottom coordinate of
	the rectangle.*/
	void		AddBottom(VLONG nDistance)
		{ m_Rect.bottom += nDistance; }

	/** Shrink the rectangle by nDistanceX left to right, and nDistanceY
	top to bottom.*/
	void		Deflate(	VULONG nDistanceX,
							VULONG nDistanceY)
		{ InflateRect(&m_Rect, -1 * nDistanceX, -1 * nDistanceY); }

	/** Set the coordinates of this rectangle to 0, 0, 0, 0.*/
	void		Empty()
		{ SetRectEmpty(&m_Rect); }

	/** Return a reference to the bottom coordinate of this rectangle.*/
	VLONG&		GetBottom() const
		{ return (VLONG&)m_Rect.bottom; }

	/** Return a VPoint object which contains the coordinates of the
	bottom and left parts of this rectangle.*/
	VPoint		GetBottomLeft() const
		{ return VPoint(m_Rect.left, m_Rect.bottom); }

	/** Return a VPoint object which contains the coordinates of the
	bottom and right parts of this rectangle.*/
	VPoint		GetBottomRight() const
		{ return VPoint(m_Rect.right, m_Rect.bottom); }

	/** Return a VPoint object which contains the coordinates of the
	center of this rectangle.*/
	VPoint		GetCenterPoint() const
	{
		return VPoint(	(m_Rect.left + m_Rect.right) / 2,
						(m_Rect.top + m_Rect.bottom) / 2);
	}

	/** Return half of the height of this rectangle.*/
	VLONG		GetHalfHeight() const
		{ return GetHeight() / 2; }

	/** Return half of the width of this rectangle.*/
	VLONG		GetHalfWidth() const
		{ return GetWidth() / 2; }

	/** Return the height of this rectangle.*/
	VLONG		GetHeight() const
		{ return m_Rect.bottom - m_Rect.top; }

	/** Return a reference to the left coordinate of this rectangle.*/
	VLONG&		GetLeft() const
		{ return (VLONG&)m_Rect.left; }

	/** Return the a pointer to the internal RECT structure.*/
	RECT*		GetPointer() const
		{ return (RECT*)&m_Rect; }

	/** Return a RECT structure reference to this object.*/
	RECT&		GetRECT() const
		{ return (RECT&)m_Rect; }

	/** Return a reference to the right coordinate of this rectangle.*/
	VLONG&		GetRight() const
		{ return (VLONG&)m_Rect.right; }

	/** Return a reference to the top coordinate of this rectangle.*/
	VLONG&		GetTop() const
		{ return (VLONG&)m_Rect.top; }

	/** Return a VPoint object which contains the coordinates of the
	left and top parts of this rectangle.*/
	VPoint		GetTopLeft() const
		{ return VPoint(m_Rect.left, m_Rect.top); }

	/** Return a VPoint object which contains the coordinates of the
	right and top parts of this rectangle.*/
	VPoint		GetTopRight() const
		{ return VPoint(m_Rect.right, m_Rect.top); }

	/** Return the width of this rectangle.*/
	VLONG		GetWidth() const
		{ return m_Rect.right - m_Rect.left; }

	/** Expand the rectangle by nDistanceX left to right, and nDistanceY
	top to bottom.*/
	void		Inflate(	VULONG nDistanceX,
							VULONG nDistanceY)
		{ InflateRect(&m_Rect, nDistanceX, nDistanceY); }

	/** Calculates the intersection of two source rectangles and places the
	coordinates of the intersection rectangle into this rectangle. If the
	source rectangles do not intersect, an empty rectangle (in which all
	coordinates are set to zero) is placed into this rectangle. If the
	rectangles intersect, the return value is VTRUE, otherwise it is VFALSE.*/
	VBOOL		Intersect(	VRect const& r1,
							VRect const& r2)
	{
		return (IntersectRect(	&m_Rect,
								r1.GetPointer(),
								r2.GetPointer())) ? VTRUE : VFALSE;
	}

	/** Returns VTRUE if the coordinates of this rectangle comprise an
	empty rectangle. An empty rectangle is one that has no area, that is,
	the coordinate of the right side is less than or equal to the coordinate
	of the left side, or the coordinate of the bottom side is less than or
	equal to the coordinate of the top side.*/
	VBOOL		IsEmpty() const
		{ return (IsRectEmpty(&m_Rect)) ? VTRUE : VFALSE; }

	/** Returns VTRUE if an existing VRect object has the same coordinates
	as this rectangle, VFALSE otherwise.*/
	VBOOL		IsEqual(VRect const& rcCompare) const
	{
		return (EqualRect(	&m_Rect,
							rcCompare.GetPointer())) ? VTRUE : VFALSE;
	}

	/** Returns VTRUE if an existing VRect object is completely inside
	this rectangle, VFALSE otherwise.*/
	VBOOL		IsInside(VRect const& rcCompare) const
	{
		if (	rcCompare.GetLeft()		>= m_Rect.left &&
				rcCompare.GetTop()		>= m_Rect.top &&
				rcCompare.GetRight()	<= m_Rect.right &&
				rcCompare.GetBottom()	<= m_Rect.bottom )
			return VTRUE;

		return VFALSE;
	}

	/** Returns VTRUE if the coordinates of this rectangle are 0,0,0,0.*/
	VBOOL		IsNothing() const
	{
		return (	!m_Rect.left &&
					!m_Rect.top &&
					!m_Rect.right &&
					!m_Rect.bottom) ? VTRUE : VFALSE;
	}

	/** Returns VTRUE if the point specified by the VPoint object is inside
	the rectangle, VFALSE otherwise.*/
	VBOOL		IsPointInside(VPoint const& pt) const
		{ return (PtInRect(&m_Rect, pt.GetPOINT())) ? VTRUE : VFALSE; }

	/** Call to verify rectangle left and top coordinates are LESS than
	right and bottom coordinates.*/
	void		Normalize()
	{
		if ( m_Rect.left > m_Rect.right )
		{
			VLONG nTemp =	m_Rect.left;
			m_Rect.left =	m_Rect.right;
			m_Rect.right =	nTemp;
		}

		if ( m_Rect.top > m_Rect.bottom )
		{
			VLONG nTemp =	m_Rect.top;
			m_Rect.top =	m_Rect.bottom;
			m_Rect.bottom =	nTemp;
		}
	}

	/** Offset (move) the rectangle by nDistanceX and nDistanceY. Either
	can be negative, which will subtract.*/
	void		Offset(	VLONG nDistanceX,
						VLONG nDistanceY)
		{ OffsetRect(&m_Rect, nDistanceX, nDistanceY); }

	/** Set the rectangle coordinates to those passed to function.*/
	void		Set(	VLONG nx,
						VLONG ny,
						VLONG ncx,
						VLONG ncy)
		{ SetRect(&m_Rect, nx, ny, ncx, ncy); }

	/** Set the coordinates of this rectangle to be the values from an
	existing VRect object.*/
	void		Set(VRect const& r)
		{ Set(VRECT_BREAK(r)); }

	/** Set the coordinates of this rectangle to be the values from an
	existing RECT structure.*/
	void		Set(RECT const& r)
		{ Set(r.left, r.top, r.right, r.bottom); }

	/** Obtain the coordinates of a rectangle determined by subtracting one
	rectangle from another. rSubtract is subtracted from rSubtractFrom and
	the results left in this rectangle object. Returns VTRUE on success,
	VFALSE on failure. The function only subtracts the rectangle specified
	by rSubtract from the rectangle specified by rSubtractFrom when the
	rectangles intersect completely in either the x- or y-direction. For
	example, if rSubtractFrom has the coordinates (10,10,100,100) and
	rSubtract has the coordinates (50,50,150,150), the function sets the
	coordinates of this rectangle to (10,10,100,100). If rSubtractFrom has
	the coordinates (10,10,100,100) and rSubtract has the coordinates
	(50,10,150,150), however, the function sets the coordinates of this
	rectangle to (10,10,50,100).*/
	VBOOL		Subtract(	VRect const& rSubtractFrom,
							VRect const& rSubtract)
	{
		return	(SubtractRect(	&m_Rect,
								rSubtractFrom.GetPointer(),
								rSubtract.GetPointer())) ? VTRUE : VFALSE;
	}

	/** Subtract nDistance to or from the left coordinate of the rectangle.*/
	void		SubtractLeft(VLONG nDistance)
		{ AddLeft(-nDistance); }

	/** Subtract nDistance to or from the top coordinate of the rectangle.*/
	void		SubtractTop(VLONG nDistance)
		{ AddTop(-nDistance); }

	/** Subtract nDistance to or from the right coordinate of the rectangle.*/
	void		SubtractRight(VLONG nDistance)
		{ AddRight(-nDistance); }

	/** Subtract nDistance to or from the bottom coordinate of the rectangle.*/
	void		SubtractBottom(VLONG nDistance)
		{ AddBottom(-nDistance); }

	/** This function creates the union of two rectangles. The union is the
	smallest rectangle that contains both source rectangles. Returns VTRUE
	if a union was formed, VFALSE if the source rectangles did not overlap.*/
	VBOOL		Union(	VRect const& r1,
						VRect const& r2)
	{
		return (UnionRect(	&m_Rect,
							r1.GetPointer(), r2.GetPointer()))
							? VTRUE
							: VFALSE;
	}

protected:
	/** Embedded Members.*/
	RECT		m_Rect;
};

#endif /* VRECT*/
