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

#ifndef VPOINT
#define VPOINT

#include "../vstandard.h"

/** Macro to create an empty VPoint object (coordinates of 0, 0).*/
#define VPOINT_EMPTY				VPoint(0,0)

/** Macro to retrieve the X and Y coordinates of a VPoint object.*/
#define VPOINT_BREAK(p)				p.GetX(), p.GetY()

/** VPoint provides a wrapper with enhancements to the POINT structure.*/
class VPoint
{
public:
	/** Construct with existing X and Y coordinates, or default of 0, 0.*/
	VPoint(	VLONG nXCoordinate = 0,
			VLONG nYCoordinate = 0)
		{ Set(nXCoordinate, nYCoordinate); }

	/** Construct by copying coordinates from an existing VPoint object.*/
	VPoint(VPoint const& pt)
		{ Set(pt); }

	/** Construct by converting coordinates from an existing POINT structure.*/
	VPoint(POINT const& pt)
		{ Set(pt); }

	/** Virtual destructor does nothing.*/
	virtual ~VPoint()
		{;}

	/** Same as calling IsEqual() to determine if an existing VPoint
	object has the same coordinates as this point.*/
	VBOOL operator	==(VPoint const& pt) const
		{ return IsEqual(pt); }

	/** Same as calling !IsEqual() to determine if an existing VPoint
	object does not have the same coordinates as this point.*/
	VBOOL operator	!=(VPoint const& pt) const
		{ return !IsEqual(pt); }

	/** Same as calling Set() to set the coordinates of this object to
	those of an existing VPoint object.*/
	void operator =(VPoint const& pt)
		{ Set(pt); }

	/** Add or subtract nDistance to or from the X coordinate.*/
	void		AddX(VLONG nDistance)
		{ m_Point.x += nDistance; }

	/** Add or subtract nDistance to or from the Y coordinate.*/
	void		AddY(VLONG nDistance)
		{ m_Point.y += nDistance; }

	/** Set the coordinates of this point to 0, 0.*/
	void		Empty()
		{ m_Point.x = m_Point.y = 0; }

	/** Set the point to be the current cursor position.*/
	void		GetCursorPosition()
		{ ::GetCursorPos(&m_Point); }

	/** Return a POINT structure reference to this object.*/
	POINT&		GetPOINT() const
		{ return (POINT&)m_Point; }

	/** Return the a pointer to the internal POINT structure.*/
	POINT*		GetPointer() const
		{ return (POINT*)&m_Point; }

	/** Return a reference to the X coordinate of this point.*/
	VLONG&		GetX() const
		{ return (VLONG&)m_Point.x; }

	/** Return a reference to the Y coordinate of this point.*/
	VLONG&		GetY() const
		{ return (VLONG&)m_Point.y; }

	/** Returns VTRUE if the coordinates of this point are 0, 0,
	VFALSE otherwise.*/
	VBOOL		IsEmpty() const
		{ return (!m_Point.x && !m_Point.y) ? VTRUE : VFALSE; }

	/** Returns VTRUE if an existing VPoint object has the same
	coordinates as this point, VFALSE otherwise.*/
	VBOOL		IsEqual(VPoint const& pt) const
	{
		return	(pt.GetX() == m_Point.x && pt.GetY() == m_Point.y)
				? VTRUE
				: VFALSE;
	}

	/** Offset the X coordinate of this point by nDistanceX, and the
	Y coordinate by nDistanceY. Either can be negative, which will subtract.*/
	void		Offset(VLONG nDistanceX, VLONG nDistanceY)
	{
		m_Point.x += nDistanceX;
		m_Point.y += nDistanceY;
	}

	/** Set the coordinates of this point to be the values nXCoordinate
	and nYCoordinate.*/
	void		Set(	VLONG nXCoordinate,
						VLONG nYCoordinate)
	{
		m_Point.x = nXCoordinate;
		m_Point.y = nYCoordinate;
	}

	/** Set the coordinates of this point to be the values from an
	existing VPoint object.*/
	void		Set(VPoint const& pt)
		{ Set(pt.GetPOINT()); }

	/** Set the coordinates of this point to be the values from an
	existing POINT structure.*/
	void		Set(POINT const& pt)
		{ Set(pt.x, pt.y); }

	/** Subtract nDistance to or from the X coordinate.*/
	void		SubtractX(VLONG nDistance)
		{ AddX(-nDistance); }

	/** Subtract nDistance to or from the Y coordinate.*/
	void		SubtractY(VLONG nDistance)
		{ AddY(-nDistance); }

protected:
	/** Embedded Members.*/
	POINT		m_Point;
};

#endif /* VPOINT*/
