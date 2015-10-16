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

#ifndef VPAGESETUPDIALOG
#define VPAGESETUPDIALOG

#include "../vstandard.h"
#include "../structures/vpoint.hpp"
#include "../structures/vrect.hpp"
#include "vcommondialog.hpp"

/** VPageSetupDialog is a common dialog wrapper which supports the
Page Setup dialog box.*/
class VPageSetupDialog : public VCommonDialog
{
public:
	/** Default constructor initializes base class with RTTI information
	and initializes internal members.*/
	VPageSetupDialog()
		: VCommonDialog(VWCL_RTTI_PAGE_SETUP_DIALOG)
		{ m_hDevMode = m_hDevNames = NULL; }

	/** Show the dialog box. Returns VTRUE on success, VFALSE if the user
	clicked the Cancel button. nFlags corresponds to the PAGESETUPDLG
	structure Flags member. Please see this structure documentation for
	more information.*/
	VBOOL					DoModal(VWindow const&	windowParent,
									VDWORD			nFlags =
													PSD_DEFAULTMINMARGINS |
													PSD_INTHOUSANDTHSOFINCHES)
	{
		/* Initialize the PAGESETUPDLG structure.*/
		PAGESETUPDLG psd;
		VZEROSTRUCT(psd);
		psd.lStructSize =		sizeof(psd);
		psd.hwndOwner =			windowParent.GetHandle();
		psd.hDevMode =			m_hDevMode;
		psd.hDevNames =			m_hDevNames;
		psd.Flags =				nFlags | PSD_ENABLEPAGESETUPHOOK;
		psd.lCustData =			(DWORD)this;
		psd.lpfnPageSetupHook =	HookProc;

		/* Are margins known?*/
		if ( !m_rMinMargin.IsNothing() )
		{
			psd.rtMinMargin =	m_rMinMargin.GetRECT();
			psd.Flags |=		PSD_MINMARGINS;
		}

		if ( !m_rMargin.IsNothing() )
		{
			psd.rtMargin =		m_rMargin.GetRECT();
			psd.Flags |=		PSD_MARGINS;
		}

		/* Call virtual function before showing the dialog.*/
		PreShow(psd);

		if ( PageSetupDlg(&psd) )
		{
			/* Save modes.*/
			m_hDevMode =	psd.hDevMode;
			m_hDevNames =	psd.hDevNames;

			/* Save paper size.*/
			m_ptPaperSize = psd.ptPaperSize;

			/* Save margins?*/
			if ( !(psd.Flags & PSD_DISABLEMARGINS) )
			{
				m_rMinMargin =	psd.rtMinMargin;
				m_rMargin =		psd.rtMargin;
			}

			return VTRUE;
		}

		return VFALSE;
	}

	/** Return a reference to the DEVMODE structure. This is a handle, so it
	must locked with GlobalLock() before using it, and unlocked with
	GlobalLock() when you are done with it.*/
	HANDLE&					GetDevMode() const
		{ return (HANDLE&)m_hDevMode; }

	/** Return a reference to the DEVNAME structure. This is a handle, so it
	must locked with GlobalLock() before using it, and unlocked with
	GlobalLock() when you are done with it.*/
	HANDLE&					GetDevNames() const
		{ return (HANDLE&)m_hDevNames; }

	/** Return a reference to the left, top, right, and bottom margins. It
	is initialized to 0,0,0,0 and will populated after the first call to
	DoModal(). The dimensions default to thousandths of inches, but is
	controlled by passing either PSD_INTHOUSANDTHSOFINCHES or
	PSD_INHUNDREDTHSOFMILLIMETERS as one of the flags to DoModal().*/
	VRect&					GetMargin() const
		{ return (VRect&)m_rMargin; }

	/** Return a reference to the minimum allowable widths for the left,
	top, right, and bottom margins. These values must be less than or equal
	to the values specified using GetMargin(). It is initialized to 0,0,0,0
	and will populated after the first call to DoModal().The dimensions
	default to thousandths of inches, but is controlled by passing either
	PSD_INTHOUSANDTHSOFINCHES or PSD_INHUNDREDTHSOFMILLIMETERS as one of
	the flags to DoModal().*/
	VRect&					GetMinMargin() const
		{ return (VRect&)m_rMinMargin; }

	/** Return a reference to the dimensions of the paper selected by the
	user. This will be populated after the first call to DoModal(). The
	dimensions default to thousandths of inches, but is controlled by
	passing either PSD_INTHOUSANDTHSOFINCHES or
	PSD_INHUNDREDTHSOFMILLIMETERS as one of the flags to DoModal().*/
	VPoint&					GetPaperSize() const
		{ return (VPoint&)m_ptPaperSize; }

	/** Set the internal device mode and names handles. These are normally
	populated by this class, but could be given from a print dialog box
	using the VPrintDialog common dialog class. If you use both classes,
	when each others DoModal() function returns VTRUE, the device modes and
	names should be set into each other. For instance, if you use
	VPrintDialog and its DoModal() function returns VTRUE, you should call
	this function and pass the results of VPrintDialog::GetDevMode(), and
	VPrintDialog::GetDevNames() to this function. If at a later time this
	dialog box resets these values, you should call this same function in
	VPrintDialog to update its values. This idea is to keep them in sync.*/
	void					SetDevices(	HANDLE hDevMode,
										HANDLE hDevNames)
	{
		m_hDevMode =	hDevMode;
		m_hDevNames =	hDevNames;
	}

protected:
	/** Static callback procedure used to attach to VWCL.*/
	static VUINT CALLBACK	HookProc(	HWND	hWnd,
										VUINT	nMessage,
										VWPARAM	wParam,
										VLPARAM	lParam)
	{
		if ( nMessage == WM_INITDIALOG )
		{
			VASSERT(lParam)
			VPageSetupDialog* pThis =
				(VPageSetupDialog*)((PAGESETUPDLG*)lParam)->lCustData;
			VASSERT(pThis)

			pThis->Attach(hWnd);

			return 1;
		}

		return 0;
	}

	/** Called just before the dialog box is shown. An override can
	adjust PAGESETUPDLG members as needed.*/
	virtual void			PreShow(PAGESETUPDLG& psd)
		{;}

	/** Embedded Members.*/
	VRect					m_rMinMargin;
	VRect					m_rMargin;
	VPoint					m_ptPaperSize;
	HANDLE					m_hDevMode;
	HANDLE					m_hDevNames;
};

#endif /* VPAGESETUPDIALOG*/
