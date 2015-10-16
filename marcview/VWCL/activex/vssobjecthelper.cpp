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

#include "../vstandard.h"
#include "vunknown.hpp"
#include "vssobjecthelper.hpp"
#include "vssstoragehelper.hpp"

VULONG VSSObjectHelper::Release()
{
	VASSERT(m_pWrappedObject)

	/* First AddRef() the object for our own use.*/
	AddRef();

	/* Call release on the object now and save result.*/
	VULONG nResult = m_pWrappedObject->Release();

	/* Should the object be completely released? If still 1, yes.*/
	if ( nResult == 1 )
	{
		/* If this is a storage, delete child elements VStorage derived
		class allocated.*/
		if ( GetRTTI() == VSS_RTTI_STORAGE )
		{
			VPtrArray& array =
				((VSSStorageHelper*)this)->GetCollection();

			VPTRARRAY_INDEX	nSize =	array.Size();

			for ( VPTRARRAY_INDEX i = 0; i < nSize; i++ )
				VDELETE (VSSObjectHelper*)array.At(i);
		}

		/* Should we auto-commit this element?*/
		if ( m_bAutoCommit )
			Commit();

		VString strOldName;

		/* Get the current name now if we are going to rename it.*/
		if ( m_pParentStorage && m_strRename.IsNotEmpty() )
			GetName(strOldName, VFALSE);

		/* Do the final release now and set nResult to 0.*/
		m_pWrappedObject->Release();
		nResult = 0;

		/* Do we need to rename this element before permanently going away?*/
		if ( strOldName.IsNotEmpty() )
			m_pParentStorage->RenameElement(
				VTEXT_ANY_NON_CONST(strOldName),
				VTEXT_ANY_NON_CONST(m_strRename));

		/* Reset internal member variables.*/
		ResetMembers(NULL, NULL);
	}

	/* Return the result.*/
	return nResult;
}
