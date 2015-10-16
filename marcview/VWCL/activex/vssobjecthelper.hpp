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

#ifndef VSSOBJECTHELPER
#define VSSOBJECTHELPER

/** Structured Storage helpers with enhancements including ANSI/ASCII/UNICODE
based string functions, boolean return values, and extended functionality.
When using these classes it is assumed that the class takes ownership of the
structured storage interface pointer. As such, it will call Release() when
appropriate. If calling code wishes to keep an interface pointer longer than
the scope of this object, you will need to AddRef() the interface before this
object goes out of scope. Doing this, however, may prevent the rename element
caching from working since it can only rename elements after they have been
released for the final time. Care must be taken here.*/

#include "../vstandard.h"
#include "../collections/vptrarray.hpp"
#include "../numbers/vrtti.hpp"
#include "../strings/vstring.hpp"
#include "vunknown.hpp"
#include <cguid.h>

/* Forward Declaration.*/
class VSSStorageHelper;

/* The default open and creation mode of structured storage elements.*/
#define VSS_DEFAULT_MODE STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE

/** VSSObjectHelper is an abstract base class for VSSStorageHelper and
VSSStreamHelper. Since these interfaces have many of the same functions,
they can be implemented by one base class, which can use either IStorage or
IStream interfaces. These classes ARE NOT implementation of IStorage or
IStreaam, they simply provide wrappers to access other IStorage and IStream
objects.*/
class VSSObjectHelper : public VRTTI
{
public:
	/** Runtime Type Info returned from GetRTTI().*/
	enum	{	VSS_RTTI_STORAGE,
				VSS_RTTI_STREAM,
			};

	/** Virtual destructor releases wrapped object, if needed.*/
	virtual ~VSSObjectHelper()
	{
		if ( m_pWrappedObject )
			Release();
	}

	/** AddRef() the contained interface.*/
	VULONG			AddRef()
	{
		VASSERT(m_pWrappedObject)
		return m_pWrappedObject->AddRef();
	}

	/** Ensures that any changes made to a storage or stream object open in
	transacted mode are reflected in the parent storage; for a root storage,
	reflects the changes in the actual device, for example, a file on disk.
	For a root storage object opened in direct mode, this method has no effect
	except to flush all memory buffers to the disk. For non-root storage
	objects in direct mode, this method has no effect. The object must support
	transactions for this to occur, but using commit on non-transacted objects
	does not present a problem. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			Commit(VDWORD nCommitFlags = STGC_DEFAULT)
	{
		if ( GetRTTI() == VSS_RTTI_STORAGE )
			return	(SUCCEEDED(GetStorage()->Commit(nCommitFlags)))
					? VTRUE : VFALSE;

		return (SUCCEEDED(GetStream()->Commit(nCommitFlags))) ? VTRUE : VFALSE;
	}

	/** Return a reference to the boolelan flag that indicates if the storage
	or stream should be committed before release. The object must support
	transactions for this to occur, but using commit on non-transacted
	objects does not present a problem.*/
	VBOOL&			GetAutoCommit() const
		{ return (VBOOL&)m_bAutoCommit; }

	/** Get the creation time of the object into the fileTime structure.
	Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			GetCreateTime(FILETIME& fileTime) const
		{ return GetTime(GET_TIME_CREATE, fileTime); }

	/** Get the last access time of the object into the fileTime structure.
	Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			GetLastAccessTime(FILETIME& fileTime) const
		{ return GetTime(GET_TIME_LAST_ACCESS, fileTime); }

	/** Get the last modification time of the object into the fileTime
	structure. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			GetLastModificationTime(FILETIME& fileTime) const
		{ return GetTime(GET_TIME_MODIFIED, fileTime); }

	/** Return the access mode used to open or create this object. This
	function asks the object for the mode.*/
	VDWORD			GetMode() const
	{
		STATSTG stg;
		Stat(stg);
		
		return stg.grfMode;
	}

	/** Return a reference to the access mode used to open or create this
	object. This function returns the last known, or last set, access mode.*/
	VDWORD&			GetModeCached() const
		{ return (VDWORD&)m_nMode; }

	/** Get the name of this element into a VString object. Returns
	strBuffer.String() on success, NULL on failure. If bGetRenamedString
	is VTRUE, the default, this function will return the name that will be
	used when the object is renamed during the final release, although
	this name will not yet be valid in structure storage terms, because
	the renaming has not yet occurred. If bGetRenamedString is VTRUE, and
	the object has not been renamed, the name will get retrieved from the
	object.*/
	VSTRING			GetName(	VString&	strBuffer,
								VBOOL		bGetRenamedString = VTRUE) const
	{
		strBuffer.Empty();

		if ( bGetRenamedString && m_strRename.IsNotEmpty() )
			strBuffer = m_strRename;
		else
		{
			STATSTG stg;

			if ( Stat(stg, STATFLAG_DEFAULT) && stg.pwcsName )
			{
				strBuffer = _UNCC(stg.pwcsName);
				CoTaskMemFree(stg.pwcsName);
			}
		}

		return strBuffer;
	}

	/** Returns a reference to the parent VSSStorageHelper object pointer,
	if any.*/
	VSSStorageHelper*&	GetParentStorage() const
		{ return (VSSStorageHelper*&)m_pParentStorage; }

	/** Return a reference to the string that will be used to rename this
	object after release.*/
	VString&			GetRenameText() const
		{ return (VString&)m_strRename; }

	/** Get the reserved VDWORD (used to be storage format). This is poorly
	documented by Microsoft, and is here only for completeness.*/
	VDWORD			GetReserved() const
	{
		STATSTG stg;
		Stat(stg);
		
		return stg.reserved;
	}

	/** Return the internal IStorage object pointer. This function does
	not AddRef() it.*/
	IStorage*		GetStorage() const
	{
		VASSERT(GetRTTI() == VSS_RTTI_STORAGE)
		return (IStorage*)m_pWrappedObject;
	}

	/** Return the internal IStream object pointer. This function does
	not AddRef() it.*/
	IStream*		GetStream() const
	{
		VASSERT(GetRTTI() == VSS_RTTI_STREAM)
		return (IStream*)m_pWrappedObject;
	}

	/** Return the type of element. See STATSTG::type for more info. Returns 0
	on failure. It should be noted that this function asks the object for its
	type. In most cases you can use the VRTTI base class to return this
	information.*/
	VDWORD			GetType() const
	{
		STATSTG stg;
		return (Stat(stg)) ? stg.type : 0;
	}

	/** Determine if a storage or stream name is valid. See the VWCL global
	function VIsValidStructuredStorageName().*/
	static VBOOL	IsValidName(VSTRING_CONST pszName)
		{ return VIsValidStructuredStorageName(pszName); }

	/** Call QueryInterface() on the wrapped object. Returns VTRUE on success,
	VFALSE on failure.*/
	VBOOL			QueryInterface(	REFIID		iid,
									VVOIDPTR*	ppvObj) const
	{
		VASSERT(m_pWrappedObject)
		return (SUCCEEDED((m_pWrappedObject->QueryInterface(iid, ppvObj))))
			? VTRUE : VFALSE;
	}

	/** Call Release() on the wrapped object. If the object is to be renamed
	this will occur after release. If AutoCommit() is enabled, the transaction
	will be committed. Returns the result from the wrapped objects Release()
	call. If the object reference count falls to 0, the internal member of
	this class will be reset, effectively stopping future use.*/
	VULONG			Release();

	/** Revert, or rollback, the changes to a transacted object. The object
	must support transations for this to work. Returns VTRUE on success,
	VFALSE on failure.*/
	VBOOL			Revert()
	{
		VASSERT(m_pWrappedObject)

		if ( GetRTTI() == VSS_RTTI_STORAGE )
			return (SUCCEEDED(GetStorage()->Revert())) ? VTRUE : VFALSE;

		return (SUCCEEDED(GetStream()->Revert())) ? VTRUE : VFALSE;
	}

	/** Populate statStg with information obtained from the object.*/
	VBOOL			Stat(	STATSTG&	statStg,
							VDWORD		nStatFlag = STATFLAG_NONAME) const
	{
		VASSERT(m_pWrappedObject)

		/* Zero out structure.*/
		VZEROSTRUCT(statStg);

		if ( GetRTTI() == VSS_RTTI_STORAGE )
			return SUCCEEDED(GetStorage()->Stat(&statStg, nStatFlag))
				? VTRUE : VFALSE;

		return SUCCEEDED(GetStream()->Stat(&statStg, nStatFlag))
			? VTRUE : VFALSE;
	}

protected:
	/** Protected constructor must be initialized from a derived class.
	pWrappedObject does not have to set during construction, but must be
	set before using this class, or deriviatives.*/
	VSSObjectHelper(	VRTTI_TYPE			nType,
						IUnknown*			pWrappedObject,
						VSSStorageHelper*	pParentStorage)
		: VRTTI(nType)
	{
		VASSERT(nType == VSS_RTTI_STORAGE || nType == VSS_RTTI_STREAM)

		/* Initialize members.*/
		ResetMembers(pWrappedObject, pParentStorage);
	}

	/** Emeration of possible timestamps to get from the object for the
	GetTime() function.*/
	enum	{			GET_TIME_CREATE,
						GET_TIME_MODIFIED,
						GET_TIME_LAST_ACCESS,
			};

	/** Internal function to get a timestamp from the object.*/
	VBOOL				GetTime(	VUINT		nWhichTime,
									FILETIME&	fileTime) const
	{
		VASSERT(nWhichTime <= GET_TIME_LAST_ACCESS)

		STATSTG stg;

		if ( Stat(stg) )
		{
			fileTime =
				(nWhichTime == GET_TIME_CREATE) ? stg.ctime :
				(nWhichTime == GET_TIME_MODIFIED) ? stg.mtime : stg.atime;

			return VTRUE;
		}

		return VFALSE;
	}

	/** Initialize members to default values.*/
	void				ResetMembers(	IUnknown*			pWrappedObject,
										VSSStorageHelper*	pParentStorage)
	{
		m_pWrappedObject =	pWrappedObject;
		m_pParentStorage =	pParentStorage;
		m_bAutoCommit =		VFALSE;
		m_strRename.Empty();

		/* Initialize mode to defaults, or mode of object if known.*/
		if ( m_pWrappedObject )
			m_nMode = GetMode();
		else
			m_nMode = VSS_DEFAULT_MODE;
	}

	/** The default mode to use for creating and opening storages and streams.
	STGM_CREATE will be automatically used when creation is needed. This is
	set to VSS_DEFAULT_MODE as the default, or if the class is initialized
	with an existing object, the current mode of the object will be used. This
	variable can be set and reset at any time by calling GetMode().*/
	VDWORD				m_nMode;

	/** If transacted operations are in use, setting this to VTRUE will cause
	Commit() to be called when a storage or stream is released. Default is
	VFALSE. The object wrapped by this class must support transactions for
	this to work, but no harm is done if they do not.*/
	VBOOL				m_bAutoCommit;

	/** If this string is not empty, this element will be renamed on release.
	ActiveX does not allow renaming of elements that are open, this does,
	by causing the renaming to occur after the wrapped object is released.*/
	VString				m_strRename;

	/** Internal back pointer to parent VSSStorageHelper object.*/
	VSSStorageHelper*	m_pParentStorage;

	/** The storage or stream object that we wrap.*/
	IUnknown*			m_pWrappedObject;
};

#endif	/* VSSOBJECTHELPER*/
