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

#ifndef VLINEFILEREADER
#define VLINEFILEREADER

#include "../vstandard.h"

/** VLineFileReader is used to read a text file, one line at a time.*/
class VLineFileReader
{
public:
	/** Virtual destructor does nothing.*/
	virtual ~VLineFileReader()
		{;}

	/** Start reading a file. For each line found, OnLine() will be called.
	If the specified file was opened, the function returns VTRUE, otherwise
	it returns VFALSE. Each line in the file can be up to 4k in size.
	OnLine() will be called with a NULL pointer for pszString when empty
	lines are encountered. Empty lines are those that have no printable
	characters. If you would rather not have OnLine() called for empty
	lines, pass VFALSE for bCallOnLineForEmptyLines. The line number passed
	to OnLine() is only updated if OnLine() is called.*/
	VBOOL			Read(	VSTRING_CONST	pszFileName,
							VBOOL			bCallOnLineForEmptyLines = VTRUE)
	{
		VASSERT(VSTRLEN_CHECK(pszFileName))

		FILE* pFile = (pszFileName) ? fopen(pszFileName, "rt") : NULL;

		if ( pFile )
		{
			/* Each line can be up to 4k max.*/
			VTCHAR sz[4 * 1024];

			/* We keep track of line number.*/
			VUINT nLineNumber = 0;

			while ( fgets(sz, VARRAY_SIZE(sz), pFile) )
			{
				VSTRING	pszThisLine =	NULL;
				VBOOL	bPrintable =	VFALSE;
				VUINT	nIndex =		0;

				/* Find terminating character and determine if the line
				has any printable characters.*/
				while (	sz[nIndex] != VTEXT('\r') &&
						sz[nIndex] != VTEXT('\n') &&
						sz[nIndex] != VTEXT('\0') )
				{
					/* Have we already found a printable one? If so, skip
					checking for more.*/
					if ( !bPrintable )
						bPrintable = isprint(sz[nIndex]);

					nIndex++;
				}

				/* Was a printable character found?*/
				if ( bPrintable )
				{
					/* Terminate string at nIndex.*/
					sz[nIndex] = VTEXT('\0');

					/* Set pointer.*/
					pszThisLine = sz;
				}

				/* We don't call OnLine() for empty lines if
				bCallOnLineForEmptyLines is VFALSE.*/
				if ( bPrintable || bCallOnLineForEmptyLines )
				{
					/* Offset line count.*/
					nLineNumber++;

					/* Call virtual function for processing. If it returns
					VFALSE, stop.*/
					if ( !OnLine(pszThisLine, nLineNumber) )
						break;
				}
			}

			fclose(pFile);

			/* Success.*/
			return VTRUE;
		}

		return VFALSE;
	}

protected:
	/** This function is called when reading a file once for each line in
	the file. If the file should continue to be read, return VTRUE from an
	override of this function, otherwise return VFALSE to stop processing.
	This function will be called for empty lines, pszString will be NULL,
	otherwise pszString will be 0 terminated, with carriage and linefeeds
	removed. nLineNumber is 1 based, so the first line read will cause this
	value to be 1, not 0.*/
	virtual VBOOL	OnLine(	VSTRING	pszString,
							VUINT	nLineNumber)
		{ return VTRUE; }
};

#endif /* VLINEFILEREADER*/
