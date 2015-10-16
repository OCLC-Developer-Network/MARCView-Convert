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

#ifndef VENUMMONIKER
#define VENUMMONIKER

#include "../vstandard.h"
#include "vunknown.hpp"
#include "vactivexenum.hpp"

/** VEnumMoniker is an implementation of the IEnumMoniker interface which
is used to enumerate the components of a moniker or to enumerate the
monikers in a table of monikers. VEnumMoniker has the same methods as
all enumerator interfaces: Next(), Skip)(), Reset(), and Clone().
For general information on these methods, refer to IEnumXXX in the Win32
documentation. You need to implement IEnumMoniker if you are writing a new
type of moniker and your monikers have an internal structure that can be
enumerated. Your implementation of IMoniker::Enum() must return an
enumerator that implements IEnumMoniker and can enumerate your moniker's
components. If your moniker has no structure that can be enumerated, your
IMoniker::Enum() method can simply return a NULL pointer. It is important
to note that this class makes a private copy of the IMoniker object
pointer(s) that are passed to the Create() function of this class. These
objects are AddRef()'d. This ensures the objects will be available at
least as long as this object is in scope. It also means that client code
does not have to keep the IMoniker objects available in memory.*/
class VEnumMoniker : public IEnumMoniker, public VActiveXEnum
{
public:
	/** Create and initialize an instance of this object. If a memory error
	occurs, NULL will be returned. ppMonikers must be a valid pointer to
	an IMoniker object, or a pointer to an array of them. nCountOfElements
	is the number of IMoniker objects pointed to by ppMonikers, if this
	represents an array. The moniker object(s) must already have been
	created or retreived by client code, and are copied into the new objects
	private data and AddRef()'d to ensure they remain in scope at least as long
	as this object.*/
	static VEnumMoniker*	Create(	IMoniker**	ppMonikers,
									VULONG		nCountOfElements = 1)
		{ VWCL_ACTIVEX_ENUM_IMPLEMENT_CREATE(VEnumMoniker, ppMonikers) }

	/** Implement IUnknown methods.*/
	VWCL_ACTIVEX_IMPLEMENT_IUNKNOWN_METHODS(VUnknown)

	/** Creates another enumerator that contains the same enumeration state
	as the current one. Using this function, a client can record a
	particular point in the enumeration sequence, and then return to that
	point at a later time. The new enumerator supports the same interface
	as the original one.*/
	STDMETHODIMP			Clone(IEnumMoniker** ppIEnumMoniker)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_CLONE(VEnumMoniker, m_ppMonikers, ppIEnumMoniker)
	}

	/** Retrieves the next nCountOfElements items in the enumeration
	sequence. If there are fewer than the requested number of elements left
	in the sequence, it retrieves the remaining elements. The number of
	elements actually retrieved is returned through pnCountOfElementsFetched
	(unless the caller passed in NULL for that parameter). ppMoniker must
	be a pointer to a pointer to an IMoniker object, or a pointer to a
	pointer of an array of them. nCountOfElements is the number of
	IMoniker object pointers to return, if this represents an array. The
	IMoniker pointers do not need to have been initialized by client code
	as they are initialized by this function.*/
	STDMETHODIMP			Next(	VULONG		nCountOfElements,
									IMoniker**	ppMonikers,
									VULONG*		pnCountOfElementsFetched)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_NEXT(m_ppMonikers, ppMonikers)
	}

	/** Resets the enumeration sequence to the beginning. COM SPEC: A call
	to this method, resetting the sequence, does not guarantee that the
	same set of objects will be enumerated after the reset, because it
	depends on the collection being enumerated. A static collection is
	reset to the beginning, but it can be too expensive for some
	collections, such as files in a directory, to guarantee this condition.*/
	STDMETHODIMP			Reset()
		{ return VActiveXEnum::Reset(); }

	/** Skips over the next nCountOfElements elements in the enumeration
	sequence. Return S_OK if the number of elements skipped is
	nCountOfElements, otherwise S_FALSE.*/
	STDMETHODIMP			Skip(VULONG nCountOfElements)
		{ return VActiveXEnum::Skip(nCountOfElements); }

protected:
	/** Protected constructor used for cloning and private initialization.*/
	VEnumMoniker(	IMoniker**	ppMonikers,
					VULONG		nCountOfElements,
					VULONG		nCurrentIndex)
		: VActiveXEnum(IID_IEnumMoniker, nCountOfElements, nCurrentIndex)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_CONSTRUCTOR(m_ppMonikers, IMoniker, ppMonikers)
	}

	/** Protected destructor releases allocated memory and objects.*/
	virtual ~VEnumMoniker()
		{ VWCL_ACTIVEX_ENUM_IMPLEMENT_DESTRUCTOR(m_ppMonikers) }

	/** Array of IMoniker objects that we copy and own.*/
	IMoniker**				m_ppMonikers;
};

#endif /* VENUMMONIKER*/
