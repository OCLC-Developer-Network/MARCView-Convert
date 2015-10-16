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

#ifndef VFINDREPLACEDIALOG
#define VFINDREPLACEDIALOG

#include "../vstandard.h"
#include "../strings/vstring.hpp"
#include "../windows/vwindowmessagefilter.hpp"
#include "vcommondialog.hpp"

static VUINT vwcl_gs_vfindreplacedialog_nMsgFIND = 0;

/** VFindReplaceDialog implements the Find and Replace common dialog box.*/
class VFindReplaceDialog : public VCommonDialog, public VWindowMessageFilter
{
public:
	/** Default constructur initializes base class with RTTI information
	and initializes internal members.*/
	VFindReplaceDialog()
		: VCommonDialog(VWCL_RTTI_FIND_DIALOG)
	{
		VZEROSTRUCT(m_FindReplace);

		/* The base class assumes a modal dialog. We aren't, so change this.*/
		m_bModal = VFALSE;

		if ( !vwcl_gs_vfindreplacedialog_nMsgFIND )
			vwcl_gs_vfindreplacedialog_nMsgFIND =
				RegisterWindowMessage(FINDMSGSTRING);
	}

	/** Create a non-modal dialog box. There is no such thing as a modal
	find and replace dialog. On success, a valid HWND is returned, which
	can be wrapped in a VWindow object. NULL is returned on failure. There
	is no need to manually destroy the window unless you want to. It will
	be destroyed when the user closes the dialog box or when this class is
	reused. Because this dialog is implemented as a non-modal window, your
	main window must call VTranslateDialogMessage() from an override of
	PreTranslateMessage(). This is not possible for modal VMainDialog
	applications, but is easy using VMainWindow or one of its derived
	classes, or a non-modal VMainDialog. After this function is called the
	first time, each future call will initialize the dialog with the
	previous search and replace strings. You can however still specify
	these strings by using GetFindString() and GetReplaceString(). You can
	also specify the maximum size of the these strings by using the
	VString::String(NULL, nSize) function. This means you must pre-allocate
	a string with a size that is sufficient for your application. You can
	then set strings into these members by using the VString::AppendXX()
	functions. DO NOT use the VString.String(string) method, or any of the
	other VString methods or operators that call the String(string) function.
	The reason is that those methods set the VString size to be the exact
	size of the string you pass it. This would prevent the user from
	searching for or replacing a string larger that what you set, and this
	is most likely a bad thing. If the strings are not preallocated by you,
	they will be set to a default max size of 256 bytes, yielding a max
	string length of 255 characters. The nFlags parameter is the same as
	the Flags parameter of the FINDREPLACE structure. Please see this
	structure documentation for more information. Once created, this type
	of dialog does not actually do and Finding or Replacing. That is
	application specific. It will automatically call overrides of the
	OnFind() and OnReplace() functions, which you must implement in a
	derived class.*/
	HWND				Create(	VWindow const&	windowParent,
								VBOOL			bFind = VTRUE,
								VDWORD			nFlags = 0)
	{
		/* Re-create window?*/
		Destroy();

		/* If we have not allocated properly sized storage, do this now.*/
		if ( m_strFind.GetLength(VTRUE) < 2 )
			m_strFind.String(NULL, 256);

		if ( !bFind && m_strReplace.GetLength(VTRUE) < 2 )
			m_strReplace.String(NULL, 256);

		/* Verify memory allocations.*/
		if (	m_strFind.GetLength(VTRUE) &&
				((bFind || m_strReplace.GetLength(VTRUE))) )
		{
			/* Initialize structure members.*/
			VZEROSTRUCT(m_FindReplace);
			m_FindReplace.lStructSize =			sizeof(m_FindReplace);
			m_FindReplace.lCustData =			(VDWORD)this;
			m_FindReplace.lpfnHook =			HookProc;
			m_FindReplace.hwndOwner =			windowParent.GetHandle();
			m_FindReplace.Flags =				nFlags | FR_ENABLEHOOK;
			m_FindReplace.lpstrFindWhat =		(VSTRING)m_strFind;
			m_FindReplace.wFindWhatLen =		m_strFind.GetLength(VTRUE) - 1;

			if ( !bFind )
				m_FindReplace.lpstrReplaceWith =
					(VSTRING)m_strReplace;

			if ( !bFind )
				m_FindReplace.wReplaceWithLen =
					m_strReplace.GetLength(VTRUE) - 1;

			/* Call virtual function before showing the dialog.*/
			PreShow(m_FindReplace);

			/* Save find or replace flag.*/
			if ( bFind )
				return FindText(&m_FindReplace);

			return ReplaceText(&m_FindReplace);
		}

		return NULL;
	}

	/** Return a reference to the internal string used to hold find text.
	See Create() for information on the proper use of this VString member.*/
	VString&				GetFindString() const
		{ return (VString&)m_strFind; }

	/** Return a reference to the internal string used to hold replacement
	text. See Create() for information on the proper use of this VString
	member.*/
	VString&				GetReplaceString() const
		{ return (VString&)m_strReplace; }

protected:
	/** Static callback procedure used to attach the HWND to
	VFindReplaceDialog.*/
	static VUINT CALLBACK	HookProc(	HWND	hWnd,
										VUINT	nMessage,
										WPARAM	wParam,
										LPARAM	lParam)
	{
		if ( nMessage == WM_INITDIALOG )
		{
			VASSERT(lParam)
			VFindReplaceDialog* pThis =
				(VFindReplaceDialog*)((FINDREPLACE*)lParam)->lCustData;
			VASSERT(pThis)
			VASSERT(!pThis->GetSafeWindow())

			/* Attach to VWCL.*/
			pThis->Attach(hWnd);

			/* Install a message filter in parent window to get
			notifications we care about.*/
			VWindow* pParent = pThis->GetVParent();

			/* The parent window is required!*/
			VASSERT(pParent)

			if ( pParent )
				pParent->InstallMessageFilter(VREF_PTR(pThis));

			return 1;
		}

		return 0;
	}

	/** Override allows the parent window to be free'd of message routing
	chores.*/
	virtual void			OnDestroy()
	{
		VWindow* pParent = GetVParent();

		if ( pParent )
			pParent->InstallMessageFilter(VREF_THIS, VFALSE);
	}

	/** VWindowMessageFilter base class override looks for messages that
	only we know about.*/
	virtual VBOOL			OnMessageFilter(	VWindow&	parentWindow,
												VUINT		nMessage,
												VWPARAM		wParam,
												VLPARAM		lParam)
	{
		if ( nMessage == vwcl_gs_vfindreplacedialog_nMsgFIND )
		{
			VBOOL bDown =
				(m_FindReplace.Flags & FR_DOWN) ? VTRUE : VFALSE;

			VBOOL bMatchCase =
				(m_FindReplace.Flags & FR_MATCHCASE) ? VTRUE : VFALSE;

			VBOOL bWholeWord =
				(m_FindReplace.Flags & FR_WHOLEWORD) ? VTRUE : VFALSE;

			/* This is a good time to have the VString objects
			update their length.*/
			m_strFind.UpdateLength();
			m_strReplace.UpdateLength();

			/* Prepare to call virtual functions.*/
			VBOOL bKeepDialog = VTRUE;

			if ( m_FindReplace.Flags & FR_FINDNEXT )
				bKeepDialog =
					OnFindNext(	m_strFind,
								bDown,
								bMatchCase,
								bWholeWord);

			else if ( m_FindReplace.Flags & FR_REPLACEALL )
				bKeepDialog =
					OnReplace(	m_strFind,
								m_strReplace,
								VTRUE,
								bDown,
								bMatchCase,
								bWholeWord);

			else if ( m_FindReplace.Flags & FR_REPLACE )
				bKeepDialog =
					OnReplace(	m_strFind,
								m_strReplace,
								VFALSE,
								bDown,
								bMatchCase,
								bWholeWord);

			if ( !bKeepDialog )
				Destroy();

			/* Message handled.*/
			return VFALSE;
		}

		/* Allow normal processing.*/
		return VTRUE;
	}

	/** This function is called when it is time to find the next string in
	your application that matched pszFind. bDown will be set to VTRUE if
	the user is searching "down" you document, VFALSE otherwise. If the
	user wants a case sensitive search, bMatchCase will be VTRUE, otherwise
	VFALSE. If you should match the whole word, bWholeWord will be VTRUE,
	otherwise it will be be VFALSE and you should match any part of the
	word. If you do not want to allow the user to specify down, match case,
	or whole word, you can specify this in the Create() call using the
	flags paramter. You should return VTRUE to continue finding the next
	string, VFALSE to close the dialog. The default implementation closes
	the dialog.*/
	virtual VBOOL			OnFindNext(	VSTRING_CONST	pszFind,
										VBOOL			bDown,
										VBOOL			bMatchCase,
										VBOOL			bWholeWord)
		{ return VFALSE; }

	/** Same as OnFindNext(), except used when replacing strings. Please see
	OnFindNext() for parameter information, except for pszReplace and
	bReplaceAll.When doing a replacement, pszReplace will be valid. If the
	user wants to replace all text bReplaceAll will be VTRUE, otherwise
	VFALSE.*/
	virtual VBOOL			OnReplace(	VSTRING_CONST	pszFind,
										VSTRING_CONST	pszReplace,
										VBOOL			bReplaceAll,
										VBOOL			bDown,
										VBOOL			bMatchCase,
										VBOOL			bWholeWord)
		{ return VFALSE; }

	/** Called just before the dialog box is shown. An override can
	adjust FINDREPLACE members as needed.*/
	virtual void			PreShow(FINDREPLACE& fr)
		{;}

	/** Embedded Members.*/
	FINDREPLACE				m_FindReplace;
	VString					m_strFind;
	VString					m_strReplace;
};

#endif /* VFINDREPLACEDIALOG*/
