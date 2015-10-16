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

/** This function is not needed in non-debug builds. Normally it is excluded
from release builds, but just to save a few hundred bytes, lets go ahead and
protect the developer who may have forgot to exclude this file.*/
#ifdef VWCL_DEBUG
	VINT VShowLastErrorMessage(HWND hWndParent)
	{
		VTCHAR	szString[4096] =	{VTEXT('\0')};
		VDWORD	nLastError =		GetLastError();

		if ( nLastError )
			FormatMessage(	FORMAT_MESSAGE_FROM_SYSTEM,
							NULL,
							nLastError,
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							szString,
							VARRAY_SIZE(szString),
							NULL);

		if ( szString[0] != VTEXT('\0') )
		{
			#ifdef VWCL_CONSOLE_APP
				VODS(szString)
			#else
				return MessageBox(	hWndParent,
									szString,
									VWCL_DEBUG_MESSAGE_STRING,
									MB_ICONINFORMATION);
			#endif
		}

		return IDOK;
	}
#endif

