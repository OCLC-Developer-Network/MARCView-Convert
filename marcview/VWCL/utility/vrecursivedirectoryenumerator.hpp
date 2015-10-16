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

#ifndef VRECURSIVEDIRECTORYENUMERATOR
#define VRECURSIVEDIRECTORYENUMERATOR

#include "../vstandard.h"

/** VRecursiveDirectoryEnumeratorCallback is used to receive directory and
file notifications when a VRecursiveDirectoryEnumerator object is walking a
directory structure.*/
class VRecursiveDirectoryEnumeratorCallback
{
public:
	/** Override to receive directory or file information. Return VTRUE to
	continue enumeration, VFALSE to end it. Derived class can modify
	pszName, but MUST set it back to original string before returning.*/
	virtual VBOOL	OnRecursiveFound(	VSTRING					pszName,
										VSTRING_CONST			pszMask,
										WIN32_FIND_DATA const&	FindData,
										VBOOL					bDirectory,
										VVOIDPTR				pVoid1,
										VVOIDPTR				pVoid2) = 0;
};

/** VRecursiveDirectoryEnumerator iterates directories and files, calling
the VRecursiveDirectoryEnumeratorCallback::OnRecursiveFound() virtual
function each time a file system object is found.*/
class VRecursiveDirectoryEnumerator
{
public:
	/** Default constructor initializes internal members.*/
	VRecursiveDirectoryEnumerator()
		{ Reset(); }

	/** Virtual destructor does nothing.*/
	virtual ~VRecursiveDirectoryEnumerator()
		{;}

	/** Enumerate only subdirectories, never files, below the directory
	specified by pszDirectory. pVoid1 and pVoid2 are placeholders for any
	information you would like your derived
	VRecursiveDirectoryEnumeratorCallback class to receive when this class
	calls its OnRecursiveFound() virtual function.*/
	void			EnumDirectories(
		VRecursiveDirectoryEnumeratorCallback&	callback,
		VSTRING_CONST							pszDirectory,
		VVOIDPTR								pVoid1 = NULL,
		VVOIDPTR							pVoid2 = NULL)
		{ EnumFiles(callback, pszDirectory, VTEXT("\1"), pVoid1, pVoid2); }

	/** Function to enumerate files and sub-directories in a single
	directory specified by pszDirectory. If pszMask is "\1", only
	subdirectories will be returned. pVoid1 and pVoid2 are placeholders for
	any information you would like your derived
	VRecursiveDirectoryEnumeratorCallback class to receive when this class
	calls its OnRecursiveFound() virtual function.*/
	void			EnumFiles(
		VRecursiveDirectoryEnumeratorCallback&	callback,
		VSTRING_CONST							pszDirectory,
		VSTRING_CONST							pszMask = NULL,
		VVOIDPTR								pVoid1 = NULL,
		VVOIDPTR								pVoid2 = NULL)
	{
		Init(pszDirectory);
		EnumFilesInternal(callback, pszMask, pVoid1, pVoid2);
		SetErrorMode(m_nOldErrorMode);
		Reset();
	}

	/** Return the current recursion count.*/
	VUINT			GetRecursionCount() const
		{ return m_nRecursionCount; }

	/** Start recursing files at the directory specified by pszDirectory.
	pVoid1 and pVoid2 are placeholders for any information you would like
	your derived VRecursiveDirectoryEnumeratorCallback class to receive
	when this class calls its OnRecursiveFound() virtual function.*/
	void			Recurse(
		VRecursiveDirectoryEnumeratorCallback&	callback,
		VSTRING_CONST							pszDirectory,
		VSTRING_CONST							pszMask = NULL,
		VVOIDPTR								pVoid1 = NULL,
		VVOIDPTR								pVoid2 = NULL)
	{
		Init(pszDirectory);
		RecurseInternal(callback, pszMask, pVoid1, pVoid2);
		SetErrorMode(m_nOldErrorMode);
		Reset();
	}

	/** Start recursing ONLY directories starting at pszDirectory. Only
	directories will be sent to the callback function, never files. pVoid1
	and pVoid2 are placeholders for any information you would like your
	derived VRecursiveDirectoryEnumeratorCallback class to receive when this
	class calls its OnRecursiveFound() virtual function.*/
	void			RecurseDirectories(
		VRecursiveDirectoryEnumeratorCallback&	callback,
		VSTRING_CONST							pszDirectory,
		VVOIDPTR								pVoid1 = NULL,
		VVOIDPTR								pVoid2 = NULL)
	{
		Init(pszDirectory);
		RecurseInternal(callback, VTEXT("\1"), pVoid1, pVoid2);
		SetErrorMode(m_nOldErrorMode);
		Reset();
	}

private:
	/** Internal function to enumerate files and sub-directories in a single
	directory. Returns VTRUE if completed, VFALSE otherwise.*/
	VBOOL			EnumFilesInternal(
		VRecursiveDirectoryEnumeratorCallback&	callback,
		VSTRING_CONST							pszMask,
		VVOIDPTR								pVoid1,
		VVOIDPTR								pVoid2)
	{
		/* Get items in this directory.*/
		WIN32_FIND_DATA fd;
		VINT nDirectoryLen =
			VSTRLEN(m_szBuffer);

		VBOOL bUserCancelled =
			VFALSE;

		VBOOL bDirectoriesOnly =
			(pszMask && VSTRCMP(pszMask, VTEXT("\1")) == 0) ? VTRUE : VFALSE;

		/* Append path separator.*/
		VSTRCPY(&m_szBuffer[nDirectoryLen], VFILE_PATH_SEP);

		/* Append search pattern.*/
		VSTRCPY(	&m_szBuffer[nDirectoryLen + 1],
					(!pszMask || bDirectoriesOnly)
					? VTEXT("*.*")
					: pszMask);

		/* Find the first file.*/
		HANDLE hFind = FindFirstFile(m_szBuffer, &fd);

		while ( !bUserCancelled && hFind != INVALID_HANDLE_VALUE )
		{
			if ( IsValidName(fd.cFileName) )
			{
				VBOOL bIsFile =
					(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					? VFALSE
					: VTRUE;

				/* Don't call OnRecursiveFound() with a file if only
				directories are asked for, otherwise call virtual function
				for notification after building full path.*/
				if (	(bIsFile && !bDirectoriesOnly) ||
						(!bIsFile && bDirectoriesOnly) )
				{
					VSTRCPY(&m_szBuffer[nDirectoryLen + 1], fd.cFileName);

					bUserCancelled =
						!callback.OnRecursiveFound(	m_szBuffer,
													pszMask,
													fd,
													!bIsFile,
													pVoid1,
													pVoid2);
				}
			}

			/* Find next and keep looping.*/
			if ( !bUserCancelled )
			{
				if ( !FindNextFile(hFind, &fd) )
					break;
			}
		}

		if ( hFind != INVALID_HANDLE_VALUE )
			FindClose(hFind);

		/* Fix directory buffer to pre-modified state.*/
		m_szBuffer[nDirectoryLen] = '\0';

		/* bUserCancelled should be VTRUE if we completed all enumerations.*/
		return (bUserCancelled) ? VFALSE : VTRUE;
	}

	/** Internal function called before starting public function calls.*/
	void			Init(VSTRING_CONST pszDirectory)
	{
		VASSERT(pszDirectory)

		/* Reset recursion count to 0.*/
		m_nRecursionCount = 0;

		/* Copy to local buffer and get length.*/
		VINT nLen = VSTRLEN(VSTRCPY(m_szBuffer, pszDirectory));
		VASSERT(nLen)

		/* If pszDirectory ends with \ or /, remove it.*/
		if (	m_szBuffer[nLen - 1] == VTEXT('\\') ||
				m_szBuffer[nLen - 1] == VTEXT('/') )
			m_szBuffer[nLen - 1] = VTEXT('\0');

		/* Don't show errors to user when accessing file system.*/
		m_nOldErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
	}

	/** Determine if a string is a valid file or directory name.*/
	VBOOL			IsValidName(VSTRING_CONST pszName)
	{
		VASSERT(pszName)
		return (	VSTRCMP(pszName, VTEXT(".")) == 0 ||
					VSTRCMP(pszName, VTEXT("..")) == 0)
					? VFALSE : VTRUE;
	}

	/** Internal recursive function.*/
	void			RecurseInternal(
		VRecursiveDirectoryEnumeratorCallback&	callback,
		VSTRING_CONST							pszMask,
		VVOIDPTR								pVoid1,
		VVOIDPTR								pVoid2)
	{
		/* Save directory length.*/
		VINT nDirectoryLen = VSTRLEN(m_szBuffer);

		#ifdef VWCL_DEBUG
			/* Verify input param is a directory.*/
			VDWORD nAttr = GetFileAttributes(m_szBuffer);
			VASSERT(nAttr != 0xFFFFFFFF && (nAttr & FILE_ATTRIBUTE_DIRECTORY))
		#endif

		/* Enumerate files and subdirectories in this directory.*/
		if ( EnumFilesInternal(callback, pszMask, pVoid1, pVoid2) )
		{
			/* Get subdirectories in this directory for recursion.*/
			WIN32_FIND_DATA fd;

			/* Append wildcard search pattern.*/
			VSTRCPY(&m_szBuffer[nDirectoryLen], VTEXT("\\*.*"));

			/* Find the first file.*/
			HANDLE hFind = FindFirstFile(m_szBuffer, &fd);

			while ( hFind != INVALID_HANDLE_VALUE )
			{
				/* Is this a directory? If yes, recurse it.*/
				if (	(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
						IsValidName(fd.cFileName) )
				{
					/* Recurse after building full path.*/
					m_nRecursionCount++;
					VSTRCPY(&m_szBuffer[nDirectoryLen + 1], fd.cFileName);
					RecurseInternal(callback, pszMask, pVoid1, pVoid2);
				}

				/* Find next and keep looping.*/
				if ( !FindNextFile(hFind, &fd) )
					break;
			}

			if ( hFind != INVALID_HANDLE_VALUE )
				FindClose(hFind);
		}

		/* Fix directory buffer to pre-modified state.*/
		m_szBuffer[nDirectoryLen] = '\0';
	}

	/** Reset internal members.*/
	void			Reset()
	{
		m_nRecursionCount = m_nOldErrorMode = 0;
		VZEROMEMORY(m_szBuffer, VARRAY_SIZE(m_szBuffer));
	}

	/** Embedded Members.*/
	VUINT			m_nRecursionCount;
	VUINT			m_nOldErrorMode;
	VTCHAR			m_szBuffer[1024 + 1];
};

#endif /* VRECURSIVEDIRECTORYENUMERATOR*/
