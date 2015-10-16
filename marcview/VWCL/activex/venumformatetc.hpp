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

#ifndef VENUMFORMATETC
#define VENUMFORMATETC

#include "../vstandard.h"
#include "vunknown.hpp"
#include "vactivexenum.hpp"

/** VEnumFORMATETC is an implementation of the IEnumFORMATETC interface
which is used to enumerate an array of FORMATETC structures.
VEnumFORMATETC has the same methods as all enumerator interfaces:
Next(), Skip)(), Reset(), and Clone(). For general information on these
methods, refer to IEnumXXX in the Win32 documentation. IEnumFORMATETC
must be implemented by all data objects to support calls to
IDataObject::EnumFormatEtc(), which supplies a pointer to the enumerator's
IEnumFORMATETC interface. If the data object supports a different set of
FORMATETC information depending on the direction of the data (whether a
call is intended for the SetData() or GetData() method of IDataObject), the
implementation of IEnumFORMATETC must be able to operate on both. The order
of formats enumerated through the IEnumFORMATETC object should be the same
as the order that the formats would be in when placed on the clipboard.
Typically, this order starts with private data formats and ends with
presentation formats such as CF_METAFILEPICT. It is important to note that
this class makes a private copy of the FORMATETC struct(s) that are passed
to the Create() function of this class. This ensures the data will be
available at least as long as this object is in scope. It also means that
client code does not have to keep the FORMATETC structs available in memory.
It is perfectly valid to initialize this object with FORMATETC struct(s) on
the stack, and then essentially forget about them, leaving their lifetime
managed by this class.*/
class VEnumFORMATETC : public IEnumFORMATETC, public VActiveXEnum
{
public:
	/** Create and initialize an instance of this object. If a memory error
	occurs, NULL will be returned. pFORMATETCs must be a valid pointer to
	a FORMATETC struct, or a pointer to an array of them. nCountOfElements
	is the number of FORMATETC structs pointed to by pFORMATETCs, if this
	represents an array. The FORMATETC structs must already have been
	initialized by client code, and are copied into the new objects private
	data.*/
	static VEnumFORMATETC*	Create(	FORMATETC*	pFORMATETCs,
									VULONG		nCountOfElements = 1)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_CREATE_NON_OBJECT(VEnumFORMATETC, pFORMATETCs)
	}

	/** Implement IUnknown methods.*/
	VWCL_ACTIVEX_IMPLEMENT_IUNKNOWN_METHODS(VUnknown)

	/** Creates another enumerator that contains the same enumeration state
	as the current one. Using this function, a client can record a
	particular point in the enumeration sequence, and then return to that
	point at a later time. The new enumerator supports the same interface
	as the original one.*/
	STDMETHODIMP			Clone(IEnumFORMATETC** ppIEnumFORMATETC)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_CLONE_NON_OBJECT(VEnumFORMATETC, m_pFORMATETCs, ppIEnumFORMATETC)
	}

	/** Retrieves the next nCountOfElements items in the enumeration
	sequence. If there are fewer than the requested number of elements
	left in the sequence, it retrieves the remaining elements. The number of
	elements actually retrieved is returned through pnCountOfElementsFetched
	(unless the caller passed in NULL for that parameter). pFORMATETCs must
	be a valid pointer to a FORMATETC struct, or a pointer to an array of
	them. nCountOfElements is the number of FORMATETC structs pointed to by
	pFORMATETCs, if this represents an array. The FORMATETC structs do not
	need to have been initialized by client code as they are initialized by
	this function.*/
	STDMETHODIMP			Next(	VULONG		nCountOfElements,
									FORMATETC*	pFORMATETCs,
									VULONG*		pnCountOfElementsFetched)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_NEXT_NON_OBJECT(m_pFORMATETCs, pFORMATETCs, sizeof(FORMATETC))
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
	VEnumFORMATETC(	FORMATETC*	pFORMATETCs,
					VULONG		nCountOfElements,
					VULONG		nCurrentIndex)
		: VActiveXEnum(IID_IEnumFORMATETC, nCountOfElements, nCurrentIndex)
	{
		VWCL_ACTIVEX_ENUM_IMPLEMENT_CONSTRUCTOR_NON_OBJECT(m_pFORMATETCs, FORMATETC, pFORMATETCs)

		/* A deep copy of a FORMATETC struct requires that we also copy
		FORMATETC::ptd which is a pointer to a DVTARGETDEVICE structure,
		which we also need to copy.*/
		if ( !IsInErrorState() )
		{
			for ( VULONG i = 0; i < nCountOfElements; i++ )
			{
				if ( pFORMATETCs[i].ptd )
				{
					m_pFORMATETCs[i].ptd = VNEW DVTARGETDEVICE;

					if ( m_pFORMATETCs[i].ptd )
						*m_pFORMATETCs[i].ptd = *pFORMATETCs[i].ptd;
					else
					{
						Error(VTRUE);
						return;
					}
				}
			}
		}
	}

	/** Protected destructor free's class allocated FORMATETC array
	and associated memory.*/
	virtual ~VEnumFORMATETC()
	{
		/* Free allocated DVTARGETDEVICE memory.*/
		for ( VULONG i = 0; i < m_nCountOfElements; i++ )
			VDELETE_NULL(m_pFORMATETCs[i].ptd)

		/* Free allocated FORMATETC structs.*/
		VWCL_ACTIVEX_ENUM_IMPLEMENT_DESTRUCTOR_NON_OBJECT(m_pFORMATETCs)
	}

	/** Array of FORMATETC structures that we copy and own.*/
	FORMATETC*				m_pFORMATETCs;
};

#endif /* VENUMFORMATETC*/
