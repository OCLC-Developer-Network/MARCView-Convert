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

#ifndef VIUNKNOWNARRAY
#define VIUNKNOWNARRAY

#include "../vstandard.h"
#include "../collections/vptrarray.hpp"
#include "vunknown.hpp"

/** VIUnknownArray collects open COM object pointers. Added objects will be
released, in REVERSE ORDER, in ReleaseAll(), or when the VIUnknownArray
object is destructed. The thinking behind this class is simple. Many COM
objects return pointers to other COM objects. Many times to get a pointer
to a COM object you have to dig through multiple objects, getting pointers
to each one. When you are finally done with all of them, they must be
released, usually in the same order you retreived them. If an error occurs
anywhere in the chain, you still have to release the pointers you did
received. This class takes care of that. Simply call Add() each time you
get a pointer from a COM object. If your code encounters an error along
the way, you will not have to call Release() on each object. When this
object is destructed, all added COM objects will be released. This is a
collection of IUnknown object pointers, not the VWCL VUnknown class
object pointers. However, you can add VUnknown objects to this collection
by calling VUnknown::GetUnknown().*/
class VIUnknownArray
{
public:
	/** Virtual destructor verifies all contained unknown
	objects are released.*/
	virtual ~VIUnknownArray()
		{ ReleaseAll(); }

	/** Same as Add().*/
	VPTRARRAY_INDEX operator += (IUnknown const* pUnknown)
		{ return Add(pUnknown); }

	/** Add a new object at the end of the array. Returns index into
	array on success, -1 on failure.*/
	VPTRARRAY_INDEX	Add(IUnknown const* pUnknown)
	{
		VASSERT(pUnknown)
		return m_listObjects.Add((VVOIDPTR)pUnknown);
	}

	/** Return object at given index.*/
	IUnknown*		At(VPTRARRAY_INDEX nIndex) const
	{
		VASSERT(m_listObjects.At(nIndex))
		return (IUnknown*)m_listObjects.At(nIndex);
	}

	/** Return a reference to the internal VPtrArray object.*/
	VPtrArray&		GetCollection() const
		{ return (VPtrArray&)m_listObjects; }

	/** Release all objects in REVERSE ORDER, and remove from collection.*/
	void			ReleaseAll()
	{
		VPTRARRAY_INDEX nSize = m_listObjects.Size();

		while ( --nSize >= 0 )
			At(nSize)->Release();

		m_listObjects.RemoveAll();
	}

protected:
	/** Embedded Members.*/
	VPtrArray	m_listObjects;
};

#endif /* VIUNKNOWNARRAY*/
