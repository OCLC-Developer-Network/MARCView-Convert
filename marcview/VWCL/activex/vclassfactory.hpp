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

#ifndef VCLASSFACTORY
#define VCLASSFACTORY

#include "../vstandard.h"
#include "vunknown.hpp"

/** Macro to implement an instance of a class factory object using just
the class factory name the caller wants, and the class to instantiate. For
example, if you have an ActiveX object named MyObject, this macro could be
used as follows:

VWCL_ACTIVEX_IMPLEMENT_CLASS_FACTORY(MyObjectClassFactory, MyObject)

At this time, this macro does not support COM aggregation. The class named
by class_name must be able to create an object from a parameterless Create()
function. If creating an object that requires initialization,
class_name::Create() must call another function with correct values.*/
#define VWCL_ACTIVEX_IMPLEMENT_CLASS_FACTORY(class_factory_name, class_name)	\
	class class_factory_name : public VClassFactory				\
	{															\
	public:														\
		STDMETHODIMP CreateInstance(IUnknown* pUnknownOuter,	\
									REFIID riid,				\
									VVOIDPTR* ppvObj)			\
		{														\
			*ppvObj = NULL;										\
			if ( pUnknownOuter )								\
				return CLASS_E_NOAGGREGATION;					\
			*ppvObj = class_name::Create();						\
			return (*ppvObj) ? S_OK : E_NOINTERFACE;			\
		}														\
		static class_factory_name* Create()						\
		{														\
			class_factory_name* pNew = VNEW class_factory_name	\
			VWCL_ACTIVEX_IMPLEMENT_STATIC_CREATE(pNew) }		\
	};

/** The following 3 macros are used when handling a request for a class
factory, for instance in DllGetClassObject() function. Simply handle the
function by calling VWCL_ACTIVEX_CLASS_FACTORY_MAP_BEGIN followed by as
many VWCL_ACTIVEX_CLASS_FACTORY_MAP_OBJECT as the number of available
objects that your code can create, and finally end with a call to
VWCL_ACTIVEX_CLASS_FACTORY_MAP_END. For example, if you create an
InProc object, your DllGetClassObject() function would look like this,
assuming you can only return two objects, one of type MyObject and the
other MyObjectAlso. Note that the class MyObjectClassFactory can be
generated using the VWCL_ACTIVEX_IMPLEMENT_CLASS_FACTORY macro, as shown here:

VWCL_ACTIVEX_IMPLEMENT_CLASS_FACTORY(MyObjectClassFactory, MyObject)
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, VVOIDPTR* ppvObj)
{
	VWCL_ACTIVEX_CLASS_FACTORY_MAP_BEGIN
		VWCL_ACTIVEX_CLASS_FACTORY_MAP_OBJECT(rclsid, IID_IMyObject, MyObjectClassFactory)
		VWCL_ACTIVEX_CLASS_FACTORY_MAP_OBJECT(rclsid, IID_IMyObjectAlso, MyObjectAlsoClassFactory)
	VWCL_ACTIVEX_CLASS_FACTORY_MAP_END
}
.*/

/** Macro to begin a class factory map. See above example for
more information.*/
#define VWCL_ACTIVEX_CLASS_FACTORY_MAP_BEGIN	\
	HRESULT hr =	E_OUTOFMEMORY;				\
	*ppvObj =		NULL;						\
	VClassFactory* pClassFactory = NULL;

/** Macro to create an instance of a class factory for a single object.
See above example for more information.*/
#define VWCL_ACTIVEX_CLASS_FACTORY_MAP_OBJECT(reqeusted_object_id, object_id, class_factory_name)	\
	if ( !pClassFactory && reqeusted_object_id == object_id )	\
		pClassFactory = class_factory_name::Create();

/** Macro to signal the end of a class factory map. See above example for
more information.*/
#define VWCL_ACTIVEX_CLASS_FACTORY_MAP_END					\
	if ( pClassFactory )									\
	{														\
		hr = pClassFactory->QueryInterface(riid, ppvObj);	\
		pClassFactory->Release();							\
	}														\
	return hr;

/** VClassFactory acts as a class factory for another object. See the
VWCL_ACTIVEX_IMPLEMENT_CLASS_FACTORY, VWCL_ACTIVEX_CLASS_FACTORY_MAP_BEGIN,
VWCL_ACTIVEX_CLASS_FACTORY_MAP_OBJECT, and VWCL_ACTIVEX_CLASS_FACTORY_MAP_END
macro's for more information. It is not possible to instantiate an object
of this type directly, since it is missing the
IClassFactory::CreateInstance() method. This method can be implemented by a
derived class or by using the above mentioned macro's.*/
class VClassFactory : public IClassFactory, public VUnknown
{
public:
	/** Implement IUnknown standard methods.*/
	VWCL_ACTIVEX_IMPLEMENT_IUNKNOWN_METHODS(VUnknown)

	/** IClassFactory::LockServer() Method. We simply use AddRef()
	and Release() to lock the server.*/
	STDMETHODIMP			LockServer(VBOOL bLock)
	{
		if ( bLock )
			AddRef();
		else
			Release();
			
		return S_OK;
	}

protected:
	/** Protected constructor initializes the base VUnknown class with
	the class factory Interface ID IID_IClassFactory.*/
	VClassFactory()
		: VUnknown(IID_IClassFactory)
		{ m_pMostDerivedThis = this; }
};

#endif /* VCLASSFACTORY*/
