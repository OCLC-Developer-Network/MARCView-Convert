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

#ifndef VCRITICALLINKEDLISTMANAGER
#define VCRITICALLINKEDLISTMANAGER

#include "../vstandard.h"
#include "../collections/vlinkedlistmanager.hpp"
#include "vcriticalsection.hpp"

/** VCriticalLinkedListManager manages a linked list of VLinkedListItem
objects using VLinkedListManager, and is thread safe using critical sections
via the VCriticalSection class. This is a simple derivation of both base
classes to yield a threadsafe implementation of the linked list manager. It
is critical to note that the VLinkedListManager functions Find() and
GetCount() are not implemented in this class. The reason is that objects
returned by Find(), or the count returned by GetCount() could be invalid
after returning, since the critical section would no longer be locked. You
must Enter() the critical section first, call the Find() or GetCount()
VLinkedListManager functions, do something with the returned information,
and then Leave() the critical section. This cannot be encapsulated in this
class.*/
class VCriticalLinkedListManager :
	public VLinkedListManager, public VCriticalSection
{
public:
	/** Virtual destructor calls Free() to delete the objects contained
	in the linked list, in a thread-safe manner.*/
	virtual ~VCriticalLinkedListManager()
		{ Free(); }

	/** Adds an item to the end of the linked list unless
	bAddAtBeginningOfList is VTRUE.*/
	virtual void	Add(	VLinkedListItem*	pItem,
							VBOOL				bAddAtBeginningOfList = VFALSE)
	{
		Enter();
		VLinkedListManager::Add(pItem, bAddAtBeginningOfList);
		Leave();
	}

	/** Delete pItem from the linked list. If bDeleteItem is VTRUE, delete
	will be called on pItem.*/
	virtual VBOOL	Delete(	VLinkedListItem*	pItem,
							VBOOL				bDeleteItem = VTRUE)
	{
		Enter();
		VLinkedListManager::Delete(pItem, bDeleteItem);
		Leave();
	}

	/** Empty the contents of the linked list, but do no delete objects
	currently in the list.*/
	virtual void	Empty()
	{
		Enter();
		VLinkedListManager::Empty();
		Leave();
	}

	/** Free all items's in linked list. This deletes the objects in the
	linked list. If you wish to only reset the linked list to contain no
	objects, but not delete the objects, call Empty().*/
	virtual void	Free()
	{
		Enter();
		VLinkedListManager::Free();
		Leave();
	}
};

#endif /* VCRITICALLINKEDLISTMANAGER*/
