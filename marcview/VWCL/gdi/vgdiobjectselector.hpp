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
Other Author(s) of this source code file: Laszlo Radanyi (lrd@plea.se)
Primary Class Designer:                   Laszlo Radanyi (lrd@plea.se)

VWCL and all source code are copyright (c) 1996-2000 by The VWCL Alliance.*/

#ifndef VGDIOBJECTSELECTOR
#define VGDIOBJECTSELECTOR

#include "../vstandard.h"
#include "vdc.hpp"
#include "vgdiobject.hpp"

/** VGDIObjectSelector assists with the selection and un-selection of GDI
objects (like pens and brushes) into and out of a device context. It
verifies that objects selected into a DC during construction are
unselected from the DC during object destruction. This helps to prevent GDI
resource leaks and situations where GDI objects are deleted while they are
selected into a device context, which is a potential crash situation.
VGDIObjectSelector objects are usually short lived, and should remain in
scope for less time than the device context and the GDI object they select.
A typical usage of this class would be as follows, assuming you have a VDC
object named dc:

VPen pen1(VTRUE, PS_SOLID, 1, RGB(255,255,255));
VGDIObjectSelector os1(dc, pen1);
...(draw with pen1)...
VPen pen2(VTRUE, PS_DOT, 10, RGB(255,0,0));
VGDIObjectSelector os2(dc, pen2);
...(draw with pen2)...
VGDIObjectSelector os3(dc, GetStockObject(WHITE_BRUSH));
...(draw with white brush)...

See the constructor and destructor comments for more information about the
proper usage of this class, and the ways in which is attempts to verify
propery device context selection.*/
class VGDIObjectSelector
{
public:
	/** Initialize by selecting a VGDIObject into the devic context at dc.
	The VDC object at dc must remain in scope beyond the lifetime of this
	object. The object specified for gdiObject must be valid and must have
	already been initialized or created as needed.*/
	VGDIObjectSelector(	VDC&		dc,
						VGDIObject&	gdiObject)
		{ Init(dc, gdiObject.GetHandle()); }

	/** Initialize by selecting a Windows GDI object into the devic context
	at dc. The VDC object at dc must remain in scope beyond the lifetime of
	this object. The object specified for hObject must be valid and must
	have already been initialized or created as needed.*/
	VGDIObjectSelector(	VDC&	dc,
						HGDIOBJ	hObject)
		{ Init(dc, hObject); }

	/** Virtual destructor restores the previously selected object, which
	must still be a valid GDI object. This insures that the object that was
	selected into the device context by this object can safely be deleted.*/
	virtual ~VGDIObjectSelector()
	{
		/* The DC must always be valid! It must outlive this object in scope.*/
		VASSERT(m_pDC && m_pDC->GetHandle())

		/* Did we initialize properly? We did if we have an old object
		to select. It must still be valid (not deleted)!*/
		VBOOL bValidObject =
			(m_hOldObject && GetObjectType(m_hOldObject) != 0)
				? VTRUE : VFALSE;
		VASSERT(bValidObject)

		/* Select it now?*/
		if ( bValidObject )
			m_pDC->SelectObject(m_hOldObject);
	}

protected:
	/** Internal function initializes the class for all constructors.*/
	void	Init(VDC& dc, HGDIOBJ hObject)
	{
		/* The DC must always be valid!*/
		VASSERT(dc.GetHandle())

		/* The GDI object must be valid!*/
		VBOOL bValidObject =
			(hObject && GetObjectType(hObject) != 0) ? VTRUE : VFALSE;
		VASSERT(bValidObject)

		/* Initialize and select hObject.*/
		m_pDC =			&dc;
		m_hOldObject =	(bValidObject) ? dc.SelectObject(hObject) : NULL;
	}

	/** Embedded Members.*/
	VDC*	m_pDC;
	HGDIOBJ	m_hOldObject;
};

#endif /* VGDIOBJECTSELECTOR*/
