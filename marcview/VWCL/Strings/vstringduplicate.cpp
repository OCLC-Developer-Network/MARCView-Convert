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
#include "ustring.h"

/** This file is based on the original works of Don Box (dbox@develop.com).
These routines provide the same conversions as Don's ustring.cpp module,
with the exception that these functions can deal with NULL and 0 length
strings. Depending on your intended use, link this file, or ustring.cpp,
into your code to implement the functions defined in Don's String816(),
_U, and _UNCC classes, and by the VTEXT_ANY and VTEXT_ANY_NON_CONST
VWCL macro's.*/
wchar_t* uxdup(const char* pszString)
{
    wchar_t*	pwszResult =	NULL;
	size_t		nCount =		(pszString) ? lstrlenA(pszString) : 0;

	if ( nCount )
	{
		nCount++;
		pwszResult = (wchar_t*)malloc(nCount * sizeof(wchar_t));

		if ( pwszResult )
			mbstowcs(pwszResult, pszString, nCount);
	}

	return pwszResult;
}

char* uxdup(const wchar_t* pwszString)
{
    char*	pszResult = NULL;
	size_t	nCount =	(pwszString) ? lstrlenW(pwszString) : 0;

	if ( nCount )
	{
		nCount++;
		pszResult = (char*)malloc(sizeof(char) * nCount);

		if ( pszResult )
			wcstombs(pszResult, pwszString, nCount);
	}

	return pszResult;
}
