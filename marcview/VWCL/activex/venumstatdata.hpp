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

#ifndef VENUMSTATDATA
#define VENUMSTATDATA

#include "../vstandard.h"
#include "vunknown.hpp"
#include "vactivexenum.hpp"

/** VEnumSTATDATA is an implementation of the IEnumSTATDATA interface which
is used to enumerate through an array of STATDATA structures, which contain
advisory connection information for a data object. VEnumSTATDATA has the
same methods as all enumerator interfaces: Next(), Skip)(), Reset(), and
Clone(). For general information on these methods, refer to IEnumXXX in the
Win32 documentation. IEnumSTATDATA is implemented to enumerate advisory
connections. Most applications will not implement this directly, but will
use the OLE-provided implementation. Pointers to this implementation are
available in two ways: In a data object, call CreateDataAdviseHolder() to
get a pointer to the OLE data advise holder object, and then, to implement
IDataObject::EnumDAdvise(), call IDataAdviseHolder::EnumAdvise(), which
creates the enumeration object and supplies a pointer to the implementation
of IEnumSTATDATA. OR In a compound document object, call
CreateOLEAdviseHolder() to get a pointer to the OLE advise holder object,
and then, to implement IOleObject::EnumAdvise(), call
IOleAdviseHolder::EnumAdvise(), which creates the enumeration object and
supplies a pointer to the implementation of IEnumSTATDATA. It is important
to note that this class makes a private copy of the STATDATA struct(s) that
are passed to the Create() function of this class and AddRef()'s required
objects. This ensures the data will be available at least as long as this
object is in scope. It also means that client code does not have to keep
the STATDATA structs available in memory. It is perfectly valid to
initialize this object with STATDATA struct(s) on the stack, and then
essentially forget about them, leaving their lifetime managed by this class.*/
class VEnumSTATDATA : public IEnumSTATDATA, public VActiveXEnum
{
public:
	/** Create and initialize an instance of this object. If a memory error
	occurs, NULL will be returned. pSTATDATAs must be a valid pointer to a
	STATDATA struct, or a pointer to an array of them. nCountOfElements is
	the number of STATDATA structs pointed to by pSTATDATAs, if this
	represents an array. The STATDATA structs must already have been
	initialized by client code, and are copied into the new objects private
	data.*/
	static VEnumSTATDATA*	Create(	STATDATA*	pSTATDATAs,
									VULONG		nCountOfElements = 1)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_CREATE_NON_OBJECT(VEnumSTATDATA, pSTATDATAs)
	}

	/** Implement IUnknown methods.*/
	VWCL_ACTIVEX_IMPLEMENT_IUNKNOWN_METHODS(VUnknown)

	/** Creates another enumerator that contains the same enumeration state
	as the current one. Using this function, a client can record a
	particular point in the enumeration sequence, and then return to that
	point at a later time. The new enumerator supports the same interface
	as the original one.*/
	STDMETHODIMP			Clone(IEnumSTATDATA** ppIEnumSTATDATA)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_CLONE_NON_OBJECT(VEnumSTATDATA, m_pSTATDATAs, ppIEnumSTATDATA)
	}

	/** Retrieves the next nCountOfElements items in the enumeration
	sequence. If there are fewer than the requested number of elements left
	in the sequence, it retrieves the remaining elements. The number of
	elements actually retrieved is returned through pnCountOfElementsFetched
	(unless the caller passed in NULL for that parameter). pSTATDATAs must
	be a valid pointer to a STATDATA struct, or a pointer to an array of
	them. nCountOfElements is the number of STATDATA structs pointed to by
	pSTATDATAs, if this represents an array. The STATDATA structs do not
	need to have been initialized by client code as they are initialized
	by this function.*/
	STDMETHODIMP			Next(	VULONG		nCountOfElements,
									STATDATA*	pSTATDATAs,
									VULONG*		pnCountOfElementsFetched)
	{
		VULONG nCurrentIndex = m_nCurrentIndex;

		VWCL_ACTIVEX_ENUM_IMPLEMENT_NEXT_NON_OBJECT(m_pSTATDATAs, pSTATDATAs, sizeof(STATDATA))

		if ( hResult == S_OK )
		{
			/* AddRef() each STATDATA::pAdvSink object.*/
			for ( VULONG i = 0; i < nGetCount; i++ )
			{
				if ( m_pSTATDATAs[nCurrentIndex + i].pAdvSink )
					m_pSTATDATAs[nCurrentIndex + i].pAdvSink->AddRef();
			}
		}

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
	VEnumSTATDATA(	STATDATA*	pSTATDATAs,
					VULONG		nCountOfElements,
					VULONG		nCurrentIndex)
		: VActiveXEnum(IID_IEnumSTATDATA, nCountOfElements, nCurrentIndex)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_CONSTRUCTOR_NON_OBJECT(m_pSTATDATAs, STATDATA, pSTATDATAs)

		/* A deep copy of a STATDATA struct requires that we also
		AddRef() STATDATA::pAdvSink.*/
		if ( !IsInErrorState() )
		{
			for ( VULONG i = 0; i < nCountOfElements; i++ )
			{
				if ( pSTATDATAs[i].pAdvSink )
					pSTATDATAs[i].pAdvSink->AddRef();
			}
		}
	}

	/** Protected destructor free's class allocated STATDATA array
	and associated objects.*/
	virtual ~VEnumSTATDATA()
	{
		/* Release STATDATA::pAdvSink.*/
		for ( VULONG i = 0; i < m_nCountOfElements; i++ )
		{
			if ( m_pSTATDATAs[i].pAdvSink )
				m_pSTATDATAs[i].pAdvSink->Release();
		}

		/* Free allocated STATDATA structs.*/
		VWCL_ACTIVEX_ENUM_IMPLEMENT_DESTRUCTOR_NON_OBJECT(m_pSTATDATAs)
	}

	/** Array of STATDATA structures that we copy and own.*/
	STATDATA*				m_pSTATDATAs;
};

#endif /** VENUMSTATDATA*/
