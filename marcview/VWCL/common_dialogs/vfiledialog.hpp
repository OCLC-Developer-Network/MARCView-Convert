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

Primary Author of this source code file:	Simon Capewell
											(simonc@bristol.crosswinds.net)
Other Author(s) of this source code file:	Todd Osborne
											(todd@vwcl.org)

VWCL and all source code are copyright (c) 1996-2000 by The VWCL Alliance.*/

#ifndef VFILEDIALOG
#define VFILEDIALOG

#include "../vstandard.h"
#include "../strings/vstring.hpp"
#include "vcommondialog.hpp"

/* Some (older) compilers fail to define this.*/
#ifndef OFN_ENABLESIZING
	#define OFN_ENABLESIZING 0x00800000
#endif

/** VFileDialog is a common dialog wrapper which supports the File Open
and Save As dialog boxes. When opening files, it supports both single
and multiple file selections, and has excellent support for user defined
filters.*/
class VFileDialog : public VCommonDialog
{
public:
	/** Default constructur initializes base class with RTTI information
	and initializes internal members.*/
	VFileDialog()
		: VCommonDialog(VWCL_RTTI_FILE_DIALOG)
		{ m_pszCurrentMultiSelectFileName = NULL; }

	/** Add a new filter that will be displayed to the user when the dialog
	is shown. pszDisplay is what is shown to the user when they click the
	types combobox. pszFileExtension is what is used by the common dialog
	to determine what file types to display to the user. The first
	extension added using this function will be the default displayed to
	the user. This can be changed by overriding the PreShow() function and
	altering the OPENFILENAME structure nFilterIndex member. This is a 1
	based index, so the first item you add using this function is 1, not 0.
	An example would be for Microsoft Word to show filters for DOC and DOT
	files, calls to this function would look like this:
	AddFilter("Word Documents", "doc");
	AddFilter("Word Templates", "dot");
	AddFilter("All Files", "*.*");*/
	void					AddFilter(	VSTRING_CONST pszDisplay,
										VSTRING_CONST pszFileExtension)
	{
		VASSERT(VSTRLEN_CHECK(pszDisplay))
		VASSERT(VSTRLEN_CHECK(pszFileExtension))

		/* Build filter string except don't terminate with final \n\n,
		that will be done later. We do append one trailing \n.*/
		m_strFilters += pszDisplay;
		m_strFilters += "\n*.";
		m_strFilters += pszFileExtension;
		m_strFilters += '\n';
	}

	/** Same as above, but loads display and file extension strings
	from resources.*/
	void					AddFilter(	VUINT		nStringIDDisplay,
										VUINT		nStringIDFileExtension,
										HINSTANCE	hResource = NULL)
	{
		AddFilter(	VString(nStringIDDisplay, hResource),
					VString(nStringIDFileExtension, hResource));
	}

	/** Show the dialog box. Returns VTRUE on success, VFALSE if the user
	cancelled the dialog. If bOpenDialog is VTURE, a file open dialog will
	be shown, otherwise a Save As dialog will be. nFlags corresponds to the
	OPENFILENAME structure Flags member. See this structure for more
	information. If the user should be presented with a default filename, it
	can be specified in pszFileName. If pszFileName is NULL, the last known
	filename will be used. If pszInitialDirectory not NULL, files in this
	directory will be shown, otherwise the last used directory will be used.
	pszDefaultExtension if not NULL will cause this extension to be appended
	automatically if the user did not enter a file extension. pszCaption, if
	not NULL, will be used as the dialog title. If NULL, the default of
	Open or Save As will be used.*/
	VBOOL					DoModal(	VWindow const&	windowParent,
										VBOOL			bOpenDialog =
														VTRUE,
										VDWORD			nFlags =
														OFN_HIDEREADONLY,
										VSTRING_CONST	pszFileName =
														NULL,
										VSTRING_CONST	pszInitialDirectory =
														NULL,
										VSTRING_CONST	pszDefaultExtension =
														NULL,
										VSTRING_CONST	pszCaption =
														NULL)
	{
		/* Big buffer to support multiple file selections (many).*/
		VTCHAR sz[8192];

		/* User specified starting directory? In not, use default.*/
		if ( !pszInitialDirectory )
		{
			if (	m_strCurrentDirectory.IsEmpty() &&
					GetCurrentDirectory(VARRAY_SIZE(sz), sz) )
				m_strCurrentDirectory = sz;

			pszInitialDirectory = m_strCurrentDirectory;
		}

		/* User specified filename? If not, use current one, if any.*/
		if ( pszFileName )
			VSTRCPY(sz, pszFileName);
		else if ( m_strSingleSelectFileName.IsNotEmpty() )
			VSTRCPY(sz, m_strSingleSelectFileName);
		else
			sz[0] = VTEXT('\0');

		/* Initialize structure.*/
		OPENFILENAME ofn;
		VZEROSTRUCT(ofn);
		ofn.lStructSize =		sizeof(ofn);
		ofn.hwndOwner =			windowParent.GetHandle();
		ofn.lpstrFile =			sz;
		ofn.nMaxFile =			VARRAY_SIZE(sz);
		ofn.lpstrInitialDir =	pszInitialDirectory;
		ofn.lpstrTitle =		pszCaption;
		ofn.lpstrDefExt =		pszDefaultExtension;
		ofn.lCustData =			(VDWORD)this;
		ofn.lpfnHook =			HookProc;
		ofn.Flags =				nFlags |
								OFN_EXPLORER |
								OFN_NOCHANGEDIR |
								OFN_PATHMUSTEXIST |
								OFN_ENABLEHOOK |
								OFN_ENABLESIZING;

		/* Filters? Copy the string before modifying it.*/
		VString sFilters(m_strFilters);

		if ( sFilters.IsNotEmpty() )
		{
			/* Add trailing newline to make it valid.*/
			sFilters += '\n';

			/* Set into ofn.*/
			ofn.lpstrFilter =	sFilters.MakeFilter();
			ofn.nFilterIndex =	1;
		}

		/* Determine correct options bits.*/
		if ( bOpenDialog )
			ofn.Flags |= OFN_FILEMUSTEXIST;
		else
		{
			/* When saving, prompt user if they are overwriting a file,
			and don't allow multiple selections for saving.*/
			ofn.Flags |= OFN_OVERWRITEPROMPT;
			ofn.Flags &= ~OFN_ALLOWMULTISELECT;
		}

		/* Call virtual function before showing the dialog.*/
		PreShow(ofn);

		/* Show the dialog.*/
		if ( ((bOpenDialog) ? GetOpenFileName(&ofn) : GetSaveFileName(&ofn)) )
		{
			/* Reset selection filenames.*/
			m_strSingleSelectFileName.Empty();
			m_strMultiSelectFileNames.Empty();

			/* Reset multiselect pointer.*/
			m_pszCurrentMultiSelectFileName = NULL;

			if ( ofn.Flags & OFN_ALLOWMULTISELECT )
			{
				/* We have to determine the exact string length
				for multiple selections dialogs. including two
				terminating 0's.*/
				VUINT nLength = 0;

				/* We also need to determine how many filenames are
				selected. If only 1, he have to handle this.*/
				VUINT nSelected = 0;

				/* Reset pointer.*/
				VSTRING pszOffSet = sz;

				while ( pszOffSet )
				{
					pszOffSet = VSTRCHR(pszOffSet, VTEXT('\0'));

					if ( pszOffSet )
					{
						nLength = pszOffSet - sz;

						/* Is the next character also a 0? If it is, we are
						done counting.*/
						if ( *(pszOffSet + 1) == VTEXT('\0') )
						{
							nLength++;
							pszOffSet = NULL;

							if ( !nSelected )
								nSelected = 1;
						}
						else
						{
							nSelected++;
							pszOffSet++;
						}
					}
				}

				/* Allocate memory in string for entire string.*/
				if (	nLength &&
						m_strMultiSelectFileNames.String(NULL, nLength + 1) )
				{
					/* Tell VString about new length.*/
					m_strMultiSelectFileNames.UpdateLength(nLength);

					/* We cannot use normal string functions to copy, so
					do a memcpy() instead.*/
					memcpy(m_strMultiSelectFileNames.String(), sz, nLength);

					/* Set the multiselect offset pointer now to the first
					filename.*/
					m_pszCurrentMultiSelectFileName =
						m_strMultiSelectFileNames.String() + ofn.nFileOffset;

					/* If there was only 1 file selected,
					m_strMultiSelectFileNames will contain the whole path.
					This is not desired, so we will have to place a
					terminator in there.*/
					if ( nSelected == 1 )
						m_strMultiSelectFileNames.CharAt(
							ofn.nFileOffset - 1,
							VTEXT('\0'));
				}
			}
			/* Save single selection string.*/
			else
				m_strSingleSelectFileName = sz;

			if (	m_strSingleSelectFileName.IsNotEmpty() ||
					m_strMultiSelectFileNames.IsNotEmpty() )
			{
				/* When a multiple selection is done at the root,
				ofn.nFileOffset is wrong. Adjust it.*/
				if (	ofn.Flags & OFN_ALLOWMULTISELECT &&
						sz[ofn.nFileOffset - 2] == VFILE_PATH_SEP_CHAR )
					ofn.nFileOffset--;

				/* Save the current directory to use later.*/
				if ( m_strCurrentDirectory.String(sz, 0, ofn.nFileOffset - 1) )
					return VTRUE;
			}
		}

		return VFALSE;
	}

	/** Return a reference to the last used directory. This string should
	never end in the final slash or backslash.*/
	VString&				GetDirectory() const
		{ return (VString&)m_strCurrentDirectory; }

	/** Return a reference to the filename selected. This function should
	only be used when the OFN_ALLOWMULTISELECT is NOT used as an option
	when opening files. It is safe to use when saving files since
	OFN_ALLOWMULTISELECT is not allowed. If multiple file selections are
	used, you should call GetNextFileName() in a loop until it returns
	VFALSE to receive each selected filename.*/
	VString&				GetFileName() const
		{ return (VString&)m_strSingleSelectFileName; }

	/** When multiple selection Open dialogs are allowed, using the
	OFN_ALLOWMULTISELECT flag, this function should be called in a loop to
	retreive each selected file name. This will be the entire path to the
	selected file. This function should be called until it returns VFALSE,
	even if you stop opening files, so that the internal index can be
	offset. When VFALSE is returned, or on error, the string at strBuffer
	is not modified.*/
	VBOOL					GetNextFileName(VString& strBuffer)
	{
		if ( m_pszCurrentMultiSelectFileName )
		{
			/* Be most efficient. Try to avoid memory allocations.*/
			strBuffer.Empty(VFALSE);

			/* Initialize with path first.*/
			strBuffer += m_strCurrentDirectory;

			/* Append path separator character.*/
			strBuffer += VFILE_PATH_SEP_CHAR;

			/* Append the filename.*/
			strBuffer += m_pszCurrentMultiSelectFileName;

			/* Find the next 0 terminator.*/
			m_pszCurrentMultiSelectFileName =
				VSTRCHR(m_pszCurrentMultiSelectFileName, VTEXT('\0'));

			/* If there is another NULL character after this one, we are done.*/
			if ( m_pszCurrentMultiSelectFileName )
			{
				if ( *(m_pszCurrentMultiSelectFileName + 1) == VTEXT('\0') )
					m_pszCurrentMultiSelectFileName = NULL;
				else
					m_pszCurrentMultiSelectFileName++;
			}

			return VTRUE;
		}

		return VFALSE;
	}

protected:
	/** Hook procedure attaches this window to VWCL.*/
	static VUINT CALLBACK	HookProc(	HWND	hWnd,
										VUINT	nMessage,
										VWPARAM	wParam,
										VLPARAM	lParam)
	{
		if ( nMessage == WM_INITDIALOG )
		{
			VASSERT(lParam)
			VFileDialog* pThis =
				(VFileDialog*)((OPENFILENAME*)lParam)->lCustData;
			VASSERT(pThis)
			VASSERT(!pThis->GetSafeWindow())

			pThis->Attach(hWnd);

			return 1;
		}
		return 0;
	}

	/** Override looks for notification messages to call virtual functions.*/
	virtual VLONG			OnNotify(VUINT nIDControl, NMHDR& nmhdr)
	{
		OFNOTIFY& notify = (OFNOTIFY&)nmhdr;

		switch ( notify.hdr.code )
		{
			case CDN_INITDONE:
				OnInitDone();
				break;

			case CDN_SELCHANGE:
				OnFileNameChange();
				break;

			case CDN_FOLDERCHANGE:
				OnFolderChange();
				break;

			case CDN_SHAREVIOLATION:
			{
				VUINT nResult = OnShareViolation(notify.pszFile);

				if ( nResult == OFN_SHAREWARN )
				{
					SetDialogResult(OFN_SHAREWARN);
					return 1;
				}

				return nResult;
			}

			case CDN_HELP:
				OnHelp();
				break;

			case CDN_FILEOK:
				if ( OnFileNameCheck(notify.lpOFN->lpstrFile) )
					return 0;
				SetDialogResult(1);
				return 1;

			case CDN_TYPECHANGE:
				OnTypeChange(notify.lpOFN->nFilterIndex);
				break;
		}

		return VCommonDialog::OnNotify(nIDControl, nmhdr);
	}

	/** Override this function to handle the WM_NOTIFY CDN_INITDONE message.
	The notification message is sent when the system has finished arranging
	controls in the Open or Save As dialog box to make room for the
	controls of the child dialog box.*/
	virtual void			OnInitDone()
		{;}

	/** Override this function to handle the WM_NOTIFY CDN_SELCHANGE message.
	The notification message is sent when the user selects a new file or
	folder in the file list of the Open or Save As dialog box.*/
	virtual void			OnFileNameChange()
		{;}

	/** Override this function if you want to provide custom validation of
	filenames that are entered in the dialog box. Return VTRUE to accept
	the filename and close the dialog, or VFALSE to reject it and keep the
	dialog open. If you decide this filename is invalid, you should notify
	the user to provide this feedback.*/
	virtual VBOOL			OnFileNameCheck(VSTRING_CONST pszFileName)
		{ return VTRUE; }

	/** Override this function to handle the WM_NOTIFY CDN_FOLDERCHANGE
	message. The notification message is sent when a new folder is opened
	in the Open or Save As dialog box.*/
	virtual void			OnFolderChange()
		{;}

	/** Override this function to handle the WM_NOTIFY CDN_HELP message. The
	notification message is sent when the user clicks the Help button in
	the Open or Save As dialog box.*/
	virtual void			OnHelp()
		{;}

	/** Override this function to provide custom handling of share violations.
	Valid return values are:

	OFN_SHAREFALLTHROUGH - The filename is returned from the dialog box.

	OFN_SHARENOWARN - No further action needs to be taken.

	OFN_SHAREWARN - The user receives the standard warning message for
	this error.*/
	virtual VUINT			OnShareViolation(VSTRING_CONST pszFileName)
		{ return OFN_SHAREWARN; }

	/** Override this function to handle the WM_NOTIFY CDN_TYPECHANGE
	message. The notification message is sent when the user selects a new
	file type from the list of file types in the Open or Save As dialog
	box. nFilterIndex is the index into the filters that were added to the
	dialog, and are 1, not 0, based.*/
	virtual void			OnTypeChange(VUINT nFilterIndex)
		{;}

	/** Called just before the dialog box is shown. An override can
	adjust OPENFILENAME members as needed.*/
	virtual void			PreShow(OPENFILENAME& ofn)
		{;}

	/** Embedded Members.*/
	VString					m_strFilters;
	VString					m_strSingleSelectFileName;
	VString					m_strMultiSelectFileNames;
	VString					m_strCurrentDirectory;
	VSTRING					m_pszCurrentMultiSelectFileName;
};

#endif /* VFILEDIALOG*/
