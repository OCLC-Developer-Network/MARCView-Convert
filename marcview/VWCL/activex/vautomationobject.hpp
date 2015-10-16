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

#ifndef VAUTOMATIONOBJECT
#define VAUTOMATIONOBJECT

#include "../vstandard.h"
#include "../strings/vstring.hpp"
#include "vunknown.hpp"

/** VAutomationObject is the base class for other classes that expose an
ActiveX (OLE) Automation interface through the IDispatch COM interface.
This class creates the basic IDispatch object for a class, and properly
handles QueryInterface().*/
class VAutomationObject : public VUnknown
{
public:
	/** Standard implementation of QueryInterface() returns object pointers
	for IID_IDispatch, the object specific IDispatch, and the type
	information interfaces. If another IID is requested, the function returns
	the base class QueryInterface() information.*/
	STDMETHODIMP				QueryInterface(	REFIID		riid,
												VVOIDPTR*	ppvObj)
	{
		if (	m_pDispatchInterace &&
				(riid == IID_IDispatch || riid == m_classDispatchIID) )
			return m_pDispatchInterace->QueryInterface(IID_IDispatch, ppvObj);

		if ( m_pTypeInfo && riid == IID_ITypeInfo )
			return m_pTypeInfo->QueryInterface(IID_ITypeInfo, ppvObj);

		return VUnknown::QueryInterface(riid, ppvObj);
	}

	/** Create a new VAutomationObject which must be initialized with the
	object Interface ID (IID) and Dispatch Interface (DIID). This version
	of Create() attempts to load the type library from application resources.
	On success, a VAutomationObject pointer will be returned, NULL on
	failure.*/
	static VAutomationObject*	Create(
		IID const&	classIID,
		IID const&	classDispatchIID,
		HINSTANCE	hTypeLibraryResourceInstance = NULL)
	{
		/* Get the module name of hTypeLibraryResourceInstance.*/
		VString s;

		/* Create from file and return.*/
		return (s.GetModulePath(VFALSE,
								VFALSE,
								(hTypeLibraryResourceInstance)
								? hTypeLibraryResourceInstance
								: VGetResourceHandle()))
								? Create(classIID, classDispatchIID, s)
								: NULL;
	}

	/** Create a new VAutomationObject which must be initialized with the
	object Interface ID (IID) and Dispatch Interface (DIID). This version
	of Create() attempts to load the type library from a file. On success,
	a VAutomationObject pointer will be returned, NULL on failure.*/
	static VAutomationObject*	Create(	IID const&		classIID,
										IID const&		classDispatchIID,
										VSTRING_CONST	pszFileName)
	{
		VAutomationObject* pNew =
			VNEW VAutomationObject(classIID, classDispatchIID, pszFileName);

		VWCL_ACTIVEX_IMPLEMENT_STATIC_CREATE(pNew)
	}

	/** Return the internal IDispatch interface. This function will NOT
	AddRef() the interface.*/
	IDispatch*					GetDispatch() const
		{ return m_pDispatchInterace; }

	/** Return the internal IDispatch interface ID.*/
	IID							GetDispatchIID() const
		{ return m_classDispatchIID; }

	/** Return the internal ITypeInfo interface. This function will NOT
	AddRef() the interface.*/
	ITypeInfo*					GetTypeInfo() const
		{ return m_pTypeInfo; }

protected:
	/** Protected constructor called from static Create() functions.*/
	VAutomationObject(	IID const&		classIID,
						IID const&		classDispatchIID,
						VSTRING_CONST	pszFileName)
		: VUnknown(classIID)
	{
		/* This must be known!*/
		VASSERT(pszFileName)

		/* GUID_NULL is not valid!*/
		VASSERT(classIID != GUID_NULL)

		/* Initialize members.*/
		m_classDispatchIID =	classDispatchIID;
		m_pMostDerivedThis =	this;
		m_pDispatchInterace =	NULL;
		m_pTypeInfo =			NULL;

		ITypeLib* pTypeLibrary;

		/* Load the type library.*/
		if ( SUCCEEDED(LoadTypeLib(VTEXT_ANY(pszFileName), &pTypeLibrary)) )
		{
			/* Get the type information.*/
			if ( SUCCEEDED(pTypeLibrary->GetTypeInfoOfGuid(	m_classIID,
															&m_pTypeInfo)) )
			{
				/* Create a standard IDispatch interface for this object.*/
				if ( FAILED(CreateStdDispatch(
					GetUnknown(),
					m_pMostDerivedThis,
					m_pTypeInfo,
					(LPUNKNOWN*)&m_pDispatchInterace)) )
				{
					m_pTypeInfo->Release();
					m_pTypeInfo = NULL;
				}
			}

			/* We don't need the type library any more.*/
			pTypeLibrary->Release();
		}

		/* Did we succeed?*/
		if ( !m_pDispatchInterace )
			Error(VTRUE);
	}

	/** Protected destructor releases internal dispatch and type
	information interfaces.*/
	virtual ~VAutomationObject()
	{
		if ( m_pDispatchInterace )
			m_pDispatchInterace->Release();

		if ( m_pTypeInfo )
			m_pTypeInfo->Release();
	}

	/** Embedded Members.*/
	IDispatch*					m_pDispatchInterace;
	ITypeInfo*					m_pTypeInfo;
	IID							m_classDispatchIID;
};

#endif /* VAUTOMATIONOBJECT*/
