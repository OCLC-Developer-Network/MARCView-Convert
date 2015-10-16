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

#ifndef VENUMOLEVERB
#define VENUMOLEVERB

#include "../vstandard.h"
#include "vunknown.hpp"
#include "vactivexenum.hpp"

/** VEnumOLEVERB is an implementation of the IEnumOLEVERB interface which
is used to enumerate the different verbs available for an object in order
of ascending verb number. An enumerator that implements the
IEnumOLEVERB interface is returned by IOleObject::EnumVerbs().
VEnumOLEVERB has the same methods as all enumerator interfaces: Next(),
Skip)(), Reset(), and Clone(). For general information on these methods,
refer to IEnumXXX in the Win32 documentation. You typically do not have to
implement this interface. The OLE default handler provides an
implementation that supplies the entries in the registry. Because calls to
IOleObject::EnumVerb() are always routed through the default handler, an
OLE application can let the default handler do the work by implementing
IOleObject::EnumVerb() as a stub that simply returns OLE_S_USEREG. This
informs the default handler that it should create the enumerator for you.
It is important to note that this class makes a private copy of the
OLEVERB struct(s) that are passed to the Create() function of this class.
This ensures the data will be available at least as long as this object is
in scope. It also means that client code does not have to keep the OLEVERB
structs available in memory. It is perfectly valid to initialize this object
with OLEVERB struct(s) on the stack, and then essentially forget about
them, leaving their lifetime managed by this class.*/
class VEnumOLEVERB : public IEnumOLEVERB, public VActiveXEnum
{
public:
	/** Create and initialize an instance of this object. If a memory error
	occurs, NULL will be returned. pOLEVERBs must be a valid pointer to a
	OLEVERB struct, or a pointer to an array of them. nCountOfElements is
	the number of OLEVERB structs pointed to by pOLEVERBs, if this
	represents an array. The OLEVERB structs must already have been
	initialized by client code, and are copied into the new objects
	private data.*/
	static VEnumOLEVERB*	Create(	OLEVERB*	pOLEVERBs,
									VULONG		nCountOfElements = 1)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_CREATE_NON_OBJECT(VEnumOLEVERB, pOLEVERBs)
	}

	/** Implement IUnknown methods.*/
	VWCL_ACTIVEX_IMPLEMENT_IUNKNOWN_METHODS(VUnknown)

	/** Creates another enumerator that contains the same enumeration state
	as the current one. Using this function, a client can record a
	particular point in the enumeration sequence, and then return to that
	point at a later time. The new enumerator supports the same interface
	as the original one.*/
	STDMETHODIMP			Clone(IEnumOLEVERB** ppIEnumOLEVERB)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_CLONE_NON_OBJECT(VEnumOLEVERB, m_pOLEVERBs, ppIEnumOLEVERB)
	}

	/** Retrieves the next nCountOfElements items in the enumeration
	sequence. If there are fewer than the requested number of elements left
	in the sequence, it retrieves the remaining elements. The number of
	elements actually retrieved is returned through pnCountOfElementsFetched
	(unless the caller passed in NULL for that parameter). pOLEVERBs must
	be a valid pointer to a OLEVERB struct, or a pointer to an array of
	them. nCountOfElements is the number of OLEVERB structs pointed to by
	pOLEVERBs, if this represents an array. The OLEVERB structs do not need
	to have been initialized by client code as they are initialized by
	this function.*/
	STDMETHODIMP			Next(	VULONG		nCountOfElements,
									OLEVERB*	pOLEVERBs,
									VULONG*		pnCountOfElementsFetched)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_NEXT_NON_OBJECT(m_pOLEVERBs, pOLEVERBs, sizeof(OLEVERB))
		return hResult;
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
	VEnumOLEVERB(	OLEVERB*	pOLEVERBs,
					VULONG		nCountOfElements,
					VULONG		nCurrentIndex)
		: VActiveXEnum(IID_IEnumOLEVERB, nCountOfElements, nCurrentIndex)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_CONSTRUCTOR_NON_OBJECT(m_pOLEVERBs, OLEVERB, pOLEVERBs)

		/* A deep copy of a OLEVERB struct requires that we also
		copy OLEVERB::lpszVerbName.*/
		if ( !IsInErrorState() )
		{
			for ( VULONG i = 0; i < nCountOfElements; i++ )
			{
				if ( pOLEVERBs[i].lpszVerbName )
				{
					m_pOLEVERBs[i].lpszVerbName =
						VNEW WCHAR[lstrlenW(pOLEVERBs[i].lpszVerbName) + 1];

					if ( m_pOLEVERBs[i].lpszVerbName )
						lstrcpyW(	m_pOLEVERBs[i].lpszVerbName,
									pOLEVERBs[i].lpszVerbName);
					else
					{
						Error(VTRUE);
						return;
					}
				}
			}
		}
	}

	/** Protected destructor free's class allocated OLEVERB array and
	associated memory.*/
	virtual ~VEnumOLEVERB()
	{
		/* Free allocated OLEVERB::lpszVerbName memory.*/
		for ( VULONG i = 0; i < m_nCountOfElements; i++ )
			VDELETE_NULL(m_pOLEVERBs[i].lpszVerbName)

		/* Free allocated FORMATETC structs.*/
		VWCL_ACTIVEX_ENUM_IMPLEMENT_DESTRUCTOR_NON_OBJECT(m_pOLEVERBs)
	}

	/** Array of OLEVERB structures that we copy and own.*/
	OLEVERB*				m_pOLEVERBs;
};

#endif /* VENUMOLEVERB*/
