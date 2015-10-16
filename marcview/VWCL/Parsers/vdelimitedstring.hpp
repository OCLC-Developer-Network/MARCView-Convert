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

#ifndef VDELIMITEDSTRING
#define VDELIMITEDSTRING

#include "../vstandard.h"

/** VDelimitedString supports walking a string of delimited values. This
class DOES modify the original string, which the caller owns. You should
copy string if this is not desired. The original string will be fixed only
if Next() is called until it returns NULL, or if FixLast() is called.*/
class VDelimitedString
{
public:
	/** Default constructor must be initialized with a C style string, or
	NULL. In addition, the delimiter character can be specified in
	nDelimiterChar. A comma separated string is the default.*/
	VDelimitedString(	VSTRING	pszString = NULL,
						VTCHAR	nDelimiterChar = VTEXT(','))
	{
		m_pszString = m_pszOffSet = NULL;
		Reset(pszString, nDelimiterChar);
	}

	/** Get the next, or first, element of the string and move offset past
	next delimiter. Returns NULL when the string contains no more
	delimiters. This function does modify the internal string by
	inserting 0 terminators as needed. This will be corrected on the next
	call to Next(), or if FixLast() is called.*/
	VSTRING	Next(VBOOL bFixLast = VTRUE)
	{
		VASSERT(m_nDelimiterChar)

		/* The result is where we left off last time.*/
		VSTRING pszResult = m_pszOffSet;

		if ( m_pszOffSet )
		{
			/* Fix last string (if needed).*/
			if ( bFixLast )
				FixLast();

			/* If we are now on a terminator, the string had a
			trailing separator char.*/
			if ( *m_pszOffSet == VTEXT('\0') )
			{
				pszResult =		NULL;
				m_pszOffSet =	NULL;
			}
			else
			{
				VSTRING pszNext = VSTRCHR(m_pszOffSet, m_nDelimiterChar);

				if ( pszNext )
					*pszNext = '\0';

				/* Increment offset.*/
				if ( m_pszOffSet && pszNext )
					m_pszOffSet = pszNext + 1;
				else
					m_pszOffSet = NULL;
			}
		}

		return pszResult;
	}

	/** Fix (un-terminate) the last termination this class performed
	in a previous call to Next().*/
	void	FixLast()
	{
		VASSERT(m_nDelimiterChar)

		if ( m_pszOffSet && m_pszOffSet != m_pszString )
			*(m_pszOffSet - 1) = m_nDelimiterChar;
	}

	/** Reset internal buffers (to use the same class again with a different
	string). If pszString is NULL, this will reset with the same string as
	used before. You can also change the delimiter character with this
	function.*/
	void	Reset(	VSTRING	pszString = NULL,
					VTCHAR	nDelimiterChar = VTEXT(','))
	{
		VASSERT(nDelimiterChar)

		m_nDelimiterChar =	nDelimiterChar;
		m_pszString =		(pszString) ? pszString : m_pszString;
		m_pszOffSet =		m_pszString;
	}

protected:
	/** Embedded Members.*/
	VTCHAR	m_nDelimiterChar;
	VSTRING	m_pszString;
	VSTRING	m_pszOffSet;
};

#endif /* VDELIMITEDSTRING*/
