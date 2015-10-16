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

#ifndef VGDIOBJECT
#define VGDIOBJECT

#include "../vstandard.h"

/** VGDIObject is a base class for wrapping all Windows GDI objects such as
fonts, pens, and brushes.*/
class VGDIObject
{
public:
	/** Standard  constructor initializes the class with an existing GDI
	object or NULL. If the internal GDI object should not be deleted by
	this class during destruction, pass VFALSE for bDeleteObject. Objects
	of this type are normally selected into one or more device context
	objects using the VGDIObjectSelector class.*/
	VGDIObject(	HGDIOBJ	hGDIObject = NULL,
				VBOOL	bDeleteObject = VTRUE)
	{
		/* Initialize.*/
		m_hGDIObject =		hGDIObject;
		m_bDeleteObject =	bDeleteObject;
	}

	/** Virtual destructor deletes the internal GDI object as needed.*/
	virtual ~VGDIObject()
		{ Delete(); }

	/** Delete the GDI object currently wrapped by this class, if any. This
	function will not delete objects that were marked as non-deletable
	during construction, or if the internal m_bDeleteObject flag is set to
	VFALSE. See the SetHandle() function for more information about why
	this function is implemented.*/
	virtual void	Delete()
	{
		if ( m_hGDIObject && m_bDeleteObject )
		{
			DeleteObject(m_hGDIObject);
			m_hGDIObject = NULL;
		}
	}

	/** Return a reference to the handle of the wrapped GDI object.*/
	HGDIOBJ&		GetHandle() const
		{ return (HGDIOBJ&)m_hGDIObject; }

	/** Function to set the internal object handle. This function will
	delete any previous GDI object held in this class. This can be used to
	re-use VGDIObject classes that wrap different logical GDI objects. In a
	pure OOP world, this would not be considered good behaviour. But in
	reality, this offers a great deal of functionality to objects that
	expose internal GDI objects that can be manipulated. If we did not
	allow this to occur, the end result would be many dynamically allocated
	objects used to similuate this behaviour. For instance, if class A
	allows customization of a VBrush object, without this ability that
	brush would have to be deleted and reallocated with the new settings.
	This adds an obvious problem when dealing with GDI objects in that it
	makes it more prone to allocation and deallocation errors, and forces
	the use of pointers when references would be preferred.*/
	VBOOL			SetHandle(HGDIOBJ hGDIObject)
	{
		/* Delete and current object wrapped by this class.*/
		Delete();

		/* hGDIObject should be valid.*/
		VBOOL bValidObject =
			(hGDIObject && GetObjectType(hGDIObject) != 0) ? VTRUE : VFALSE;
		VASSERT(bValidObject)

		/* Save handle.*/
		m_hGDIObject = (bValidObject) ? hGDIObject : NULL;

		return (m_hGDIObject) ? VTRUE : VFALSE;
	}

protected:
	/** Get object information. Only used by derived classes who know the
	data type they are trying to get.*/
	VBOOL			GetInfo(	VINT		nBufferSize,
								VVOIDPTR	pBuffer) const
	{
		VASSERT(nBufferSize && pBuffer)

		if (	(m_hGDIObject && nBufferSize && pBuffer) &&
				GetObject(m_hGDIObject, nBufferSize, pBuffer) )
			return VTRUE;

		return VFALSE;
	}

	/** Embedded Members.*/
	HGDIOBJ			m_hGDIObject;
	VBOOL			m_bDeleteObject;
};

#endif /* VGDIOBJECT*/
