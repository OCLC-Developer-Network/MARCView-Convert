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

#ifndef VUNKNOWN
#define VUNKNOWN

#include "../vstandard.h"
#include "../numbers/vreferencecount.hpp"
#include "../strings/ustring.h"

/* Include OLE/ActiveX headers.*/
#include <ole2.h>
#include <olectl.h>
#include <oledlg.h>

/** Macro to implement the Create() function for many VWCL ActiveX classes.
The normal use of this macro is to pass in a pointer from a call to VNEW.
This macro then checks for error condition, and if present, releases the
object and returns NULL to the caller. Since VWCL COM objects must be
allocated on the heap, their constructors are protected and a static
Create() function is normally required to instantiate them.*/
#define VWCL_ACTIVEX_IMPLEMENT_STATIC_CREATE(pNewObject)	\
		if ( pNewObject && !pNewObject->IsInErrorState() )	\
			return pNewObject;								\
		if ( pNewObject )									\
			pNewObject->Release();							\
		return NULL;

/** This macro implements, in derived classes, support for the IUnknown
standard methods AddRef() and Release(). It is needed only by classes
that derive directly from a COM interface base class. If a class derives
only from a VWCL COM class, they need not implement this macro.*/
#define VWCL_ACTIVEX_IMPLEMENT_IUNKNOWN_METHODS_ADDREF_RELEASE(base_class)	\
		STDMETHODIMP_(VULONG) AddRef(void)		\
			{ return base_class::AddRef(); }	\
		STDMETHODIMP_(VULONG) Release(void)		\
			{ return base_class::Release(); }

/** This macro implements, in derived classes, support for all IUnknown
standard methods. It is needed only by classes that derive directly from
a COM interface base class. If a class derives only from a VWCL COM class,
they need not implement this macro.*/
#define VWCL_ACTIVEX_IMPLEMENT_IUNKNOWN_METHODS(base_class)					\
		VWCL_ACTIVEX_IMPLEMENT_IUNKNOWN_METHODS_ADDREF_RELEASE(base_class)	\
		STDMETHODIMP QueryInterface(REFIID riid, VVOIDPTR* ppvObj)			\
			{ return base_class::QueryInterface(riid, ppvObj); }

/** Macro to completely implement DllCanUnloadNow() which much be exported
from DLL COM objects. Simply place this macro anywhere in your source code
and export the function in the module DEF file.*/
#define VWCL_ACTIVEX_IMPLEMENT_DLL_CAN_UNLOAD_NOW	\
	STDAPI DllCanUnloadNow()						\
		{ return (VUnknown::GetTotalObjectReferenceCount()) ? S_FALSE : S_OK; }

/** VUnknown implements the most basic of COM types, IUnknown. It handles
AddRef() and Release() internally and can call a virtual function to perform
QueryInterface(). The class is primarily designed to be a base class for other
VWCL classes that implement COM interfaces. For example, the class can be
setup and initialized with an Interface ID that is not IUnknown, but some
other IID. The base class can then provide QueryInterface() resolution for the
derived object IID, as well as for IUnknown itself. Because VUnknown objects
are COM objects, their destructors are protected so that delete cannot be
called from outside of the class. In addition, they must be allocated on the
heap with a static Create() helper function, so their constructors are also
protected. These objects are deleted when their reference count hits 0.
vactivexglobal.cpp must be included in projects that use VWCL ActiveX.*/
class VUnknown : public IUnknown
{
public:
	/** Standard IUnknown::AddRef(), also increments count of total VWCL
	object reference count.*/
	STDMETHODIMP_(VULONG)	AddRef(void)
	{
		m_TotalObjectReferenceCount.Add();
		return m_ReferenceCount.Add();
	}

	/** Standard IUnknown::Release(), also decements count of total VWCL
	object reference count. When the reference count reaches 0, this object
	will delete itself.*/
	STDMETHODIMP_(VULONG)	Release(void)
	{
		/* Decrement total object count.*/
		m_TotalObjectReferenceCount.Subtract();

		/* Decrement reference count, deleting object if needed.*/
		if ( m_ReferenceCount.Subtract() == 0 )
		{
			VDELETE this;
			return 0;
		}

		return m_ReferenceCount.Count();
	}

	/** QueryInteface handles IID_IUnknown and derived class
	inteface ID resolution.*/
	STDMETHODIMP			QueryInterface(	REFIID		riid,
											VVOIDPTR*	ppvObj)
	{
		VASSERT(ppvObj)

		/* COM spec requires us to set this to NULL.*/
		*ppvObj = NULL;

		/* Asking for IUnknown?*/
		if ( riid == IID_IUnknown )
			*ppvObj = this;

		/* Asking for the MOST DERIVED object?*/
		else if ( riid == m_classIID )
			*ppvObj = m_pMostDerivedThis;

		/* If we are returning an object, AddRef() it.*/
		if ( *ppvObj )
		{
			AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	/* Set the current error state for the object.*/
	virtual void			Error(VBOOL bError)
		{ m_bError = bError; }

	/* Return an IUnknown pointer for the most derived class. This function
	DOES NOT AddRef() the object.*/
	IUnknown*				GetMostDerived() const
	{
		VASSERT(m_pMostDerivedThis)
		return (IUnknown*)m_pMostDerivedThis;
	}

	/* Return a reference to this objects reference count object.*/
	VReferenceCount&		GetReferenceCount() const
		{ return (VReferenceCount&)m_ReferenceCount; }

	/* The is a total object reference count, not just the count of references
	on THIS object. Since VWCL does not imply any framework, this is like a
	global variable. It can be used to determine the total number of
	outstanding VWCL ActiveX object references. For DLL based servers,
	they can call VUnknown::GetTotalObjectReferenceCount() to return this
	value when they are processing their DllCanUnloadNow() exported function.
	The macros VWCL_ACTIVEX_IMPLEMENT_DLL_CAN_UNLOAD_NOW can be used to
	implement DllCanUnloadNow().*/
	static VULONG			GetTotalObjectReferenceCount()
		{ return m_TotalObjectReferenceCount.Count(); }

	/* Return a reference to the total active objects reference count object
	for all VWCL ActiveX objects.*/
	static VReferenceCount&	GetTotalObjectReferenceCountObject()
		{ return m_TotalObjectReferenceCount; }

	/* Return the internal IUnknown interface. This function will NOT
	AddRef() the interface.*/
	IUnknown*				GetUnknown() const
		{ return (IUnknown*)this; }

	/* Returns VTRUE if the object is currently in an error state, VFALSE if
	everything is OK.*/
	VBOOL					IsInErrorState() const
		{ return m_bError; }

	/* The above mentioned total object count. It is initialized in
	vactivexglobal.cpp.*/
	static VReferenceCount	m_TotalObjectReferenceCount;

protected:
	/* Protected constructor that must be call from a derived clas. AddRef()'s
	during construction, so AddRef() is not needed after instantiation. The
	class must be initialized with the Interface ID (IID) of the object to
	implement IUnknown for, that is, the interface ID of the derived class.*/
	VUnknown(IID const& classIID)
	{
		/* Initialize members.*/
		m_pMostDerivedThis =	this;
		m_classIID =			classIID;
		m_bError =				VFALSE;
		AddRef();
	}

	/* Protected destructor verifies object is not destructing with outstanding
	references (in debug build only). VUnknown and derived objects should not
	call delete anywhere except within the Release() function. Even though
	these are C++ objects, they are also COM objects, and Release() is the
	gospel for COM objects regardless of how they are allocated.*/
	virtual ~VUnknown()
		{ VASSERT(m_ReferenceCount.Count() == 0) }

	/* These are used by the MOST DERIVED class. This is so that support for
	QueryInterface() for classes derived from this class can be handled in
	this base class. Derived classes tell their Interface ID (IID) in the
	constructor. However, it is not valid to pass a derived "this" pointer
	to a base constructor. For this reason, derived classes should initialize
	m_pMostDerivedThis to "this" in their constructor, or elsewhere before
	use.*/
	VVOIDPTR				m_pMostDerivedThis;
	IID						m_classIID;

	/* If the object is in an error state, this will be VTRUE.*/
	VBOOL					m_bError;

	/* This objects reference count.*/
	VReferenceCount			m_ReferenceCount;
};

#endif /* VUNKNOWN*/
