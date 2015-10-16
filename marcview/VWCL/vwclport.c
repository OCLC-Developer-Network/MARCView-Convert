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

#include "vstandard.h"

/** This file provides portability of non-standard (but common) functions
and variables used in Windows and UNIX development.*/

/* Globals for use when UNIX is the target OS.*/
#ifdef VWCL_TARGET_UNIX
	VUINT		gn_vwcl_argc;
	VSTRING*	gpp_vwcl_argv;
#endif

/** Internal function to get the mode (type) of a file.*/
VUINT vwcl_port_GetFileMode(VSTRING_CONST pszName)
{
	if ( pszName )
	{
		struct stat fileSTAT;
		VZEROSTRUCT(fileSTAT);

		if ( stat(pszName, &fileSTAT) == 0 )
			return fileSTAT.st_mode;
	}

	return 0;
}

/** Determine if a file exists.*/
VBOOL VDoesFileExist(VSTRING_CONST pszFileName)
{
	VUINT nMode = vwcl_port_GetFileMode(pszFileName);

	if ( nMode )
		return (!(nMode & S_IFDIR) && (nMode & S_IFREG)) ? VTRUE : VFALSE;

	return VFALSE;
}

/** Determine if a directory exists.*/
VBOOL VDoesDirectoryExist(VSTRING_CONST pszDirectoryName)
{
	VUINT nMode = vwcl_port_GetFileMode(pszDirectoryName);

	if ( nMode )
		return (nMode & S_IFDIR) ? VTRUE : VFALSE;

	return VFALSE;
}

/** Return the argc value normally passed to main(). This will work for
applications that use WinMain() as well.*/
VUINT VGetARGC()
{
	#ifdef VWCL_TARGET_UNIX
		return gn_vwcl_argc;
	#else
		return (VUINT)__argc;
	#endif
}

/** Return one of the argv values normally passed to main(). This will
work for applications that use WinMain() as well.*/
VSTRING_CONST VGetARGV(VUINT nIndex)
{
	/* Make sure caller is asking for a valid index.*/
	if ( nIndex < VGetARGC() )
	{
		#ifdef VWCL_TARGET_UNIX
			return gpp_vwcl_argv[nIndex];
		#else
			return __argv[nIndex];
		#endif
	}

	return NULL;
}

/** Set the command line parameters from main(). Windows applications
do not need to do this.*/
void VSetARGC_ARGV(VUINT nARGC, VSTRING* ppszARGV)
{
	#ifdef VWCL_TARGET_UNIX
		gn_vwcl_argc =	nARGC;
		gpp_vwcl_argv =	ppszARGV;
	#endif
}

/** This routine is used by all other xTOA functions and is the only one
that actually does the conversions.*/
VCHAR* VConvertNumbertoASCII(	VULONG	nValue,
								VCHAR*	pszBuffer,
								VUINT	nRadix,
								VBOOL	bNegative)
{
	VCHAR* pszOffset =		pszBuffer;
	VCHAR* pszFirstDigit =	pszOffset;

	/* This must be known!*/
	VASSERT(pszBuffer)

	/* Radix (base) must be between 2 and 36!*/
	VASSERT(nRadix >= 2 && nRadix <= 36)

	if ( !pszBuffer )
		return NULL;

	/* Are we really dealing with a negative value? Handle this case.*/
	if ( bNegative )
	{
		*pszOffset = '-';
		pszOffset++;
		pszFirstDigit++;
		nValue = (VULONG)(-(VLONG)nValue);
	}

	do
		{
			VUINT nDigit = (VUINT)(nValue % nRadix);
			nValue /= nRadix;

			*pszOffset =
				(VCHAR)((nDigit > 9) ? (nDigit - 10 + 'a') : (nDigit + '0'));

			pszOffset++;
		}
	while (nValue > 0);

	/* We now have the digit of the number in the buffer, but in reverse
	order. Reverse them.*/
	*pszOffset = '\0';
	pszOffset--;

	do
		{
			VCHAR chTemp =		*pszOffset;
			*pszOffset =		*pszFirstDigit;
			*pszFirstDigit =	chTemp;
			pszOffset--;
			pszFirstDigit++;
		}
	while (pszFirstDigit < pszOffset);

	return pszBuffer;
}

/** Convert VINT to ASCII.*/
VCHAR* VITOA(VINT nValue, VCHAR* pszBuffer, VUINT nRadix)
	{ return VLTOA((VLONG)nValue, pszBuffer, nRadix); }

/** Convert VUINT to ASCII.*/
VCHAR* VUITOA(VUINT nValue, VCHAR* pszBuffer, VUINT nRadix)
	{ return VULTOA((VULONG)nValue, pszBuffer, nRadix); }

/** Convert VLONG to ASCII.*/
VCHAR* VLTOA(VLONG nValue, VCHAR* pszBuffer, VUINT nRadix)
{
	return VConvertNumbertoASCII(	(VULONG)nValue,
									pszBuffer,
									nRadix,
									(VBOOL)((nRadix == 10 && nValue < 0)
									? VTRUE : VFALSE));
}

/** Convert VULONG to ASCII.*/
VCHAR* VULTOA(VULONG nValue, VCHAR* pszBuffer, VUINT nRadix)
	{ return VConvertNumbertoASCII(nValue, pszBuffer, nRadix, VFALSE); }

/** Convert pszConvert to lowercase, returning a pointer to it.*/
VSTRING VSTRLWR(VSTRING pszConvert)
{
	if ( pszConvert )
	{
		VSTRING pszOffset = pszConvert;

		while ( *pszOffset != VTEXT('\0') )
		{
			if ( VISUPPER(*pszOffset) )
				*pszOffset = VTOLOWER(*pszOffset);

			pszOffset++;
		}
	}

	return pszConvert;
}

/** Convert pszConvert to uppercase, returning a pointer to it.*/
VSTRING VSTRUPR(VSTRING pszConvert)
{
	if ( pszConvert )
	{
		VSTRING pszOffset = pszConvert;

		while ( *pszOffset != VTEXT('\0') )
		{
			if ( VISLOWER(*pszOffset) )
				*pszOffset = VTOUPPER(*pszOffset);

			pszOffset++;
		}
	}

	return pszConvert;
}

