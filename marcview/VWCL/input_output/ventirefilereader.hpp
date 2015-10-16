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

#ifndef VENTIREFILEREADER
#define VENTIREFILEREADER

#include "../vstandard.h"
#include "../strings/vstring.hpp"

/** VEntireFileReader reads an entire file into a VString object. This is
most useful when reading text files. This class has the ability to read
binary files, but since they may contain non-printing characters and
inline terminators, this should be done with care and a solid understanding
of how VString works with dynamic memory.*/
class VEntireFileReader : public VString
{
public:
	/** Default constructor can be initialized with the filename to read,
	or NULL.*/
	VEntireFileReader(	VSTRING_CONST	pszFileName = NULL,
						size_t*			pnLength = NULL,
						VBOOL			bBinaryFile = VFALSE)
	{
		if ( pszFileName )
			Read(pszFileName, pnLength, bBinaryFile);
	}

	/** Read an entire file. Returns VTRUE on success, VFALSE on failure.
	The file is read into the VString base class object, which will not be
	modified on failure. If the length of the file read is desired, pass a
	non-NULL value for pnLength. If the file being read is binary, pass
	VTRUE for the bBinaryFile parameter, otherwise a text file is assumed.*/
	VBOOL	Read(	VSTRING_CONST	pszFileName,
					size_t*			pnLength = NULL,
					VBOOL			bBinaryFile = VFALSE)
	{
		VASSERT(pszFileName)

		VBOOL bSuccess = VFALSE;

		/* Initialize out parameter.*/
		if ( pnLength )
			*pnLength = 0;

		/* Determine the file size.*/
		struct stat fileSTAT;
		VZEROSTRUCT(fileSTAT);

		if (	stat(pszFileName, &fileSTAT) == 0 &&
				fileSTAT.st_size )
		{
			/* Open the file.*/
			FILE* pFILE = fopen(	pszFileName,
									(bBinaryFile)
									? VTEXT("rb")
									: VTEXT("rt"));

			if ( pFILE )
			{
				/* Read the file now.*/
				bSuccess = Read(pFILE, fileSTAT.st_size, pnLength);

				/* Close the file.*/
				fclose(pFILE);
			}
		}

		return bSuccess;
	}

	/** Same as above, but can be used when your application already has a
	file open and stored in a file pointer. The nFileSize parameter must be
	non-0 and should specify the number of bytes to read from the file.
	Before reading the file, the file position will be set to the beginning
	of the file. nFileSize does not have to be the exact size of the file.
	It should be at least as large as the file to read the whole file. The
	stat() function often times returns a larger file size than what is
	physically available in the file. On exit, pnLength (if specified) will
	be populated with the actual number of bytes read from the file. The
	string held in the VString base class will be sized to the exact number
	of bytes read from the file, with a NULL terminator appended.*/
	VBOOL	Read(	FILE*	pFILE,
					size_t	nFileSize,
					size_t*	pnLength = NULL)
	{
		/* This must be known.*/
		VASSERT(pFILE)

		/* Initialize out parameter.*/
		if ( pnLength )
			*pnLength = 0;

		if ( pFILE && nFileSize )
		{
			/* Pre-Allocate memory for read in local string for now.*/
			VString sRead;

			if (	nFileSize &&
					sRead.String(NULL, nFileSize + 1) )
			{
				/* Always set to beginning of file.*/
				fseek(pFILE, 0, SEEK_SET);

				/* Read file into memory.*/
				size_t nRead = fread(	sRead.String(),
										1,
										nFileSize,
										pFILE);

				if ( nRead )
				{
					/* The number of bytes read may be less that specified
					by stat(), so deal with this.*/

					/* Copy local string to object string.*/
					if ( String(sRead, 0, 0, nRead) )
					{
						/* Update length in string object.*/
						UpdateLength(nRead);

						/* Save length?*/
						if ( pnLength )
							*pnLength = nRead;

						return VTRUE;
					}
				}
			}
		}

		return VFALSE;
	}
};

#endif /* VENTIREFILEREADER*/
