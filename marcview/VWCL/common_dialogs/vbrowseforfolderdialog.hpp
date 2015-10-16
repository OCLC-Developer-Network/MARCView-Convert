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

#ifndef VBROWSEFORFOLDERDIALOG
#define VBROWSEFORFOLDERDIALOG

#include <shlobj.h>
#include "../vstandard.h"
#include "../strings/vstring.hpp"
#include "vcommondialog.hpp"

/** VBrowseForFolderDialog is a common dialog wrapper which supports the
folder picker dialog. Even though this is considered a common dialog, it is
actually implemented by the Win32 shell library. To use this class you must
link with the shell32.lib import library.*/
class VBrowseForFolderDialog : public VCommonDialog
{
public:
	/** Default constructur initializes base class with RTTI information
	and initializes internal members.*/
	VBrowseForFolderDialog()
		: VCommonDialog(VWCL_RTTI_BROWSE_FOR_FOLDER_DIALOG)
		{;}

	/** Show the dialog box. Returns VTRUE on success, VFALSE if the user
	clicked the Cancel button or there was an error. pszCaption, if
	specified, is shown at the top of the dialog. nFlags corresponsd to the
	BROWSEINFO structures ulFlags member. Please see this structure for
	more information.*/
	VBOOL					DoModal(	VWindow const&	windowParent,
										VSTRING_CONST	pszCaption = NULL,
										VDWORD			nFlags =
														BIF_RETURNONLYFSDIRS)
	{
		/* Assume failure.*/
		VBOOL	bResult = VFALSE;
		VTCHAR	szDisplayName[MAX_PATH + 1];

		/* Setup BROWSEINFO struct now.*/
		BROWSEINFO bi;
		VZEROSTRUCT(bi);
		bi.lpszTitle =		pszCaption;
		bi.pszDisplayName =	szDisplayName;
		bi.hwndOwner =		windowParent.GetHandle();
		bi.ulFlags =		nFlags;
		bi.lpfn =			HookProc;
		bi.lParam =			(VLPARAM)this;

		/* Call virtual function before showing the dialog.*/
		PreShow(bi);

		ITEMIDLIST* pItem = SHBrowseForFolder(&bi);

		if ( pItem )
		{
			VTCHAR szPath[MAX_PATH + 1];

			/* This will fail if the folder isn't part of the file system.*/
			if ( SHGetPathFromIDList(pItem, szPath) )
			{
				/* Success?*/
				if (	m_strPath.String(szPath) &&
						m_strDisplayName.String(szDisplayName) )
					bResult = VTRUE;
			}

			/* Free the returned item id.*/
			LPMALLOC pMalloc;

			if ( SUCCEEDED(SHGetMalloc(&pMalloc)) )
				pMalloc->Free(pItem);
		}

		return bResult;
	}

	/** Return a reference to the display name string object.*/
	VString&				GetDisplayName() const
		{ return (VString&)m_strDisplayName; }

	/** Return a reference to the full path name string object.*/
	VString&				GetPath() const
		{ return (VString&)m_strPath; }

protected:
	/** Enable or disable the OK button.*/
	void					EnableOK(VBOOL bEnable = VTRUE) const
		{ VWINDOW_SM2(BFFM_ENABLEOK, 0, bEnable); }

	/** HookProc from common dialog routes messages to class members.*/
	static VINT CALLBACK	HookProc(	HWND	hWnd,
										VUINT	nMessage,
										VLPARAM	lParam,
										VLPARAM	pData)
	{
		VBrowseForFolderDialog* pThis = (VBrowseForFolderDialog*)pData;
		VASSERT(pThis)

		/* Verify attachment. BFFM_SELCHANGED can be sent before
		BFFM_INITIALIZED, so we have to deal with this.*/
		if ( pThis->Attach(hWnd) )
		{
			switch ( nMessage )
			{
				case BFFM_INITIALIZED:
					if ( pThis->m_strPath.IsNotEmpty() )
						pThis->SetSelection(pThis->m_strPath);
					break;

				case BFFM_SELCHANGED:
					pThis->OnSelectionChanged((ITEMIDLIST*)lParam);
					break;

				case BFFM_VALIDATEFAILED:
					return (pThis->OnValidateFailed((VSTRING_CONST)lParam))
						? 1 : 0;
			}
		}

		return 0;
	}

	/** Called when the selection changes. Base class does nothing.*/
	virtual void			OnSelectionChanged(ITEMIDLIST const* pFolder)
		{;}

	/** Called when the user types an invalid name in the edit box. Return
	VFALSE to dismiss the dialog or VTRUE to keep it displayed. This base
	class function returns VFALSE to dismiss the dialog.*/
	virtual VBOOL			OnValidateFailed(VSTRING_CONST pszFolderName)
		{ return VFALSE; }

	/** Called just before the dialog box is shown. An override can adjust
	BROWSEINFO members as needed.*/
	virtual void			PreShow(BROWSEINFO& bi)
		{;}

	/** Set the current selection using a string. This function is protected
	since it can only be called after DoModal() is called and a window
	created, which means there would be no good way to call it from outside
	the class, since the dialog would be in a modal state preventing other
	code from being able to call this member function.*/
	void					SetSelection(VSTRING_CONST pszPath) const
	{
		VASSERT(VSTRLEN_CHECK(pszPath))
		VWINDOW_SM2(BFFM_SETSELECTION, VTRUE, pszPath);
	}

	/** Set the current selection using ITEMIDLIST list. This function is
	protected since it can only be called after DoModal() is called and a
	window created, which means there would be no good way to call it from
	outside the class, since the dialog would be in a modal state
	preventing other code from being able to call this member function.*/
	void					SetSelection(ITEMIDLIST const* pItem) const
		{ VWINDOW_SM2(BFFM_SETSELECTION, VFALSE, pItem); }

	/** Set the current status using a string. This function is protected
	since it can only be called after DoModal() is called and a window
	created, which means there would be no good way to call it from outside
	the class, since the dialog would be in a modal state preventing other
	code from being able to call this member function.*/
	void					SetStatusText(VSTRING_CONST pszString) const
		{ VWINDOW_SM2(BFFM_SETSTATUSTEXT, 0, pszString); }

	/** Set the current status using a string ID. This function is protected
	since it can only be called after DoModal() is called and a window
	created, which means there would be no good way to call it from outside
	the class, since the dialog would be in a modal state preventing other
	code from being able to call this member function.*/
	void					SetStatusText(	VUINT		nStringID,
											HINSTANCE	hResource = NULL) const
		{ SetStatusText(VString(nStringID, hResource)); }

	/** Embedded Members.*/
	VString					m_strDisplayName;
	VString					m_strPath;
};

#endif /* VBROWSEFORFOLDERDIALOG*/
