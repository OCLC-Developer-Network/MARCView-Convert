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

Primary Author of this source code file:	Michael Miller
											(michmill@microsoft.com)
Other Author(s) of this source code file:	Todd Osborne
											(todd@vwcl.org)

VWCL and all source code are copyright (c) 1996-2000 by The VWCL Alliance.*/

#ifndef VVERSIONINFO
#define VVERSIONINFO

#include "../vstandard.h"
#include "../strings/vstring.hpp"
#include <winver.h>

/** VVersionInfo gets version information from executable file resources. It
makes retrieval of this version stamp information trivial, by providing an
excellent wrapper around the very arcane and incredibly hard to use version
Win32 API calls. Because of how version information is stored, this only
works on Win32 executables. All GetXXX() functions return a pointer to an
internal memory block allocated by Windows. These returned strings should
never be modified in any way by client code. If you need to modify them,
make a copy first. All GetXXX() functions return a string pointer on
success, NULL on failure. Before calling any of the GetXXX() function, the
class must first be initialized with InitFromFile() or InitFromInstance().*/
class VVersionInfo
{
public:
	/** Default constructor initializes internal members.*/
	VVersionInfo()
		{ m_pVerInfoBlock = NULL; }

	/** Virtual destructor free's internally allocated memory.*/
	virtual ~VVersionInfo()
		{ Delete(); }

	/** Delete the internal memory allocations.*/
	void			Delete()
		{ VDELETE_ARRAY_NULL(m_pVerInfoBlock) }

	/** Get the Ccomments.*/
	VSTRING_CONST	GetComments() const
		{ return GetItem(VTEXT("Comments")); }

	/** Get the Company Name.*/
	VSTRING_CONST	GetCompanyName() const
		{ return GetItem(VTEXT("CompanyName")); }

	/** Get the File Description.*/
	VSTRING_CONST	GetFileDescription() const
		{ return GetItem(VTEXT("FileDescription")); }

	/** Get the File Version.*/
	VSTRING_CONST	GetFileVersion() const
		{ return GetItem(VTEXT("FileVersion")); }

	/** Get the Internal Name.*/
	VSTRING_CONST	GetInternalName() const
		{ return GetItem(VTEXT("InternalName")); }

	/** Get the Legal Copyright.*/
	VSTRING_CONST	GetLegalCopyright() const
		{ return GetItem(VTEXT("LegalCopyright")); }

	/** Get the LegalTrademarks.*/
	VSTRING_CONST	GetLegalTrademarks() const
		{ return GetItem(VTEXT("LegalTrademarks")); }

	/** Get the Original Filename.*/
	VSTRING_CONST	GetOriginalFilename() const
		{ return GetItem(VTEXT("OriginalFilename")); }

	/** Get the Private Build.*/
	VSTRING_CONST	GetPrivateBuild() const
		{ return GetItem(VTEXT("PrivateBuild")); }

	/** Get the Product Name.*/
	VSTRING_CONST	GetProductName() const
		{ return GetItem(VTEXT("ProductName")); }

	/** Get the Product Version.*/
	VSTRING_CONST	GetProductVersion() const
		{ return GetItem(VTEXT("ProductVersion")); }

	/** Get the Special Build.*/
	VSTRING_CONST	GetSpecialBuild() const
		{ return GetItem(VTEXT("SpecialBuild")); }

	/** Initialize version information from file. Returns VTRUE on success,
	VFALSE on failure.*/
	VBOOL			InitFromFileName(VSTRING_CONST pszFileName)
	{
		VASSERT(VSTRLEN_CHECK(pszFileName))

		/* If previous m_pVerInfoBlock was allocated, delete it now.*/
		Delete();

		/* Get the size of the version information.*/
		VDWORD nHandle = 0;

		VDWORD nSize =
			GetFileVersionInfoSize((VSTRING)pszFileName, &nHandle);

		/* Does the file does have VERSION information?*/
		if ( nSize )
		{
			/* Allocate memory for the version information block.*/
			m_pVerInfoBlock = VNEW VCHAR[nSize];

			if (	m_pVerInfoBlock &&
					GetFileVersionInfo(	(VSTRING)pszFileName,
										nHandle,
										nSize,
										m_pVerInfoBlock) )
			{
				/* Get the language ID.*/
				VTCHAR	szLang		[30];
				VSTRING	pszLang =	szLang;
				VUINT	nLen =		0;

				if (	VerQueryValue(	m_pVerInfoBlock,
										VTEXT("\\VarFileInfo\\Translation"),
										(VVOIDPTR*)&pszLang,
										&nLen) )
				{
					/* Swap the words so wsprintf() will print the
					lang-charset in the correct format.*/
					*(VDWORD*)pszLang =
						MAKELONG(	HIWORD(*(VDWORD*)pszLang),
									LOWORD(*(VDWORD*)pszLang));

					/* Now save the whole string info including the language
					ID and final backslash as m_strTransBlock.*/
					VTCHAR sz[256];
					wsprintf(	sz,
								VTEXT("\\StringFileInfo\\%08lx\\"),
								*(VDWORD*)(pszLang));

					if ( m_strTransBlock.String(sz) )
						return VTRUE;
				}
			}

			/* Cleanup on failure.*/
			Delete();
		}

		/* Failure.*/
		return VFALSE;
	}

	/** Initialize from already loaded module, or if hInstance is NULL, use
	the current process.*/
	VBOOL			InitFromInstance(HINSTANCE hInstance = NULL)
	{
		VString s;
		return	(s.GetModulePath(VFALSE, VFALSE, hInstance))
				? InitFromFileName(s)
				: VFALSE;
	}

protected:
	/** Get a specific item (CompanyName, etc).*/
	VSTRING_CONST	GetItem(VSTRING_CONST pszItem) const
	{
		/* This must have been initialized by one of the Init() functions!*/
		VASSERT(m_pVerInfoBlock)

		/* This must be known!*/
		VASSERT(pszItem)

		VSTRING pszResult = NULL;

		/* Verify memory allocation succeeded from InitXXX() function.*/
		if ( m_pVerInfoBlock )
		{
			VUINT nLen = 0;

			/* Build sub-block.*/
			VString s(m_strTransBlock);
			s += pszItem;

			if (	s.GetErrorCount() ||
					!VerQueryValue(	m_pVerInfoBlock,
									s,
									(VVOIDPTR*)&pszResult,
									&nLen) )
				pszResult = NULL;
		}

		return pszResult;
	}

	/** Embedded Members.*/
	VString		m_strTransBlock;
	VVOIDPTR	m_pVerInfoBlock;
};

#endif /* VVERSIONINFO*/
