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

#ifndef VCOLORDIALOG
#define VCOLORDIALOG

#include "../vstandard.h"
#include "vcommondialog.hpp"

static VUINT vwcl_gs_vcolordialog_nMsgSETRGB = 0;

/** VColorDialog is a common dialog wrapper which supports the
standard color chooser dialog.*/
class VColorDialog : public VCommonDialog
{
public:
	/** Default constructor initializes base class with RTTI information
	and initializes internal members.*/
	VColorDialog()
		: VCommonDialog(VWCL_RTTI_COLOR_DIALOG)
	{
		m_crColor = 0;

		/* Register custom message (as needed).*/
		if ( !vwcl_gs_vcolordialog_nMsgSETRGB )
			vwcl_gs_vcolordialog_nMsgSETRGB =
				RegisterWindowMessage(SETRGBSTRING);
	}

	/** Return the selected color. This is valid to call after the dialog
	box has gone away (when DoModal() returns).*/
	COLORREF				Color() const
		{ return m_crColor; }

	/** Show the dialog box. Returns VTRUE on success, VFALSE if the user
	cancelled the dialog, or an error occurred. If crInitColor is given, it
	will be used to initialized the default color presented to the user. If
	not given, black is assumed. To use this dialog box over and over again,
	and have it initialized with the last chosen color, you can get the
	last color using the Color() method of this class when call DoModal().
	The nFlags parameter corresponds to the CHOOSECOLOR structure Flags
	parameter. Please see this structure for more information.*/
	VBOOL					DoModal(	VWindow const&	windowParent,
										COLORREF		crInitColor = 0,
										VDWORD			nFlags = CC_FULLOPEN)
	{
		static COLORREF sharedCustomColors[16];

		/* Initialize structure.*/
		CHOOSECOLOR cc;
		VZEROSTRUCT(cc);
		cc.lStructSize =	sizeof(cc);
		cc.lCustData =		(VDWORD)this;
		cc.lpfnHook =		HookProc;
		cc.hwndOwner =		windowParent.GetHandle();
		cc.rgbResult =		crInitColor;
		cc.lpCustColors =	sharedCustomColors;
		cc.Flags =			nFlags | CC_ENABLEHOOK | CC_RGBINIT;

		/* Call virtual function before showing the dialog.*/
		PreShow(cc);

		if ( ChooseColor(&cc) )
		{
			m_crColor = cc.rgbResult;
			return VTRUE;
		}

		return VFALSE;
	}

protected:
	/** Set the current color. This function is protected since it can only
	be called after DoModal() is called and a window created, which means
	there would be no good way to call it from outside the class, since the
	dialog would be in a modal state preventing other code from being able
	to call this member function.*/
	void					Color(COLORREF crColor) const
		{ VWINDOW_SM2(vwcl_gs_vcolordialog_nMsgSETRGB, 0, crColor); }

	/** Static callback procedure used to attach the HWND to the object.*/
	static VUINT CALLBACK	HookProc(	HWND	hWnd,
										VUINT	nMessage,
										VWPARAM	wParam,
										VLPARAM	lParam)
	{
		/* Verify attachment.*/
		if ( nMessage == WM_INITDIALOG )
		{
			VASSERT(lParam)
			VColorDialog* pThis =
				(VColorDialog*)((CHOOSECOLOR*)lParam)->lCustData;
			VASSERT(pThis)
			VASSERT(!pThis->GetSafeWindow())

			pThis->Attach(hWnd);

			return 1;
		}

		return 0;
	}

	/** Called just before the dialog box is shown. An override can adjust
	CHOOSECOLOR members as needed.*/
	virtual void			PreShow(CHOOSECOLOR& cc)
		{;}

	/** Embedded Members.*/
	COLORREF				m_crColor;
};

#endif /* VCOLORDIALOG*/
