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

#ifndef VPRINTDIALOG
#define VPRINTDIALOG

#include "../vstandard.h"
#include "../gdi/vdc.hpp"
#include "vcommondialog.hpp"

/** VPrintDialog displays the standard Print and Print Setup common dialog
box.*/
class VPrintDialog : public VCommonDialog
{
public:
	/** Default constructor initializes base class with RTTI information
	and initializes class members.*/
	VPrintDialog()
		: VCommonDialog(VWCL_RTTI_PRINT_DIALOG)
	{
		m_nFromPage = m_nMinPage = m_nCopies = m_nToPage = m_nMaxPage = 1;
		m_hDevMode = m_hDevNames = NULL;
	}

	/** Show the dialog box. Return VTRUE on success, or VFALSE if the user
	cancelled the dialog, or an error occurred. nFlags corresponds to the
	PRINTDLG structure Flags members. Please see this structure for more
	information. If a Print Setup dialog box is wanted, pass VTRUE for
	bPrintSetup. Otherwise a normal Print dialog will be presented. If this
	class shouldhave the dialog create a printer device context for it,
	the flag PD_RETURNDC must be used. This is a default paremeter.*/
	VBOOL					DoModal(	VWindow const&	windowParent,
										VBOOL			bPrintSetup = VFALSE,
										VDWORD			nFlags =
														PD_ALLPAGES |
														PD_HIDEPRINTTOFILE |
														PD_NOSELECTION |
														PD_RETURNDC)
	{
		/* Initialize the PRINTDLG structure.*/
		PRINTDLG pd;
		VZEROSTRUCT(pd);
		pd.lStructSize =	sizeof(pd);
		pd.hwndOwner =		windowParent.GetHandle();
		pd.hDevMode =		m_hDevMode;
		pd.hDevNames =		m_hDevNames;
		pd.nFromPage =		m_nFromPage;
		pd.nToPage =		m_nToPage;
		pd.nCopies =		m_nCopies;
		pd.nMinPage =		m_nMinPage;
		pd.nMaxPage =		m_nMaxPage;
		pd.lCustData =		(VDWORD)this;
		pd.Flags =			nFlags;

		/* Print setup dialog?*/
		if ( bPrintSetup )
		{
			pd.lpfnSetupHook =	HookProc;
			pd.Flags |=			(PD_PRINTSETUP | PD_ENABLESETUPHOOK);
			pd.Flags &=			~PD_RETURNDC;
		}
		else
		{
			pd.lpfnPrintHook =	HookProc;
			pd.Flags |=			PD_ENABLEPRINTHOOK;
		}

		/* Call virtual function before showing the dialog.*/
		PreShow(pd);

		if ( PrintDlg(&pd) )
		{
			/* Delete any current DC we might have.*/
			if ( m_PrinterDC.GetHandle() )
				m_PrinterDC.Free();

			/* Save the current printer DC, if any.*/
			m_PrinterDC.GetHandle() = pd.hDC;

			/* Save structure members back to class members.*/
			m_nFromPage =	pd.nFromPage;
			m_nToPage =		pd.nToPage;
			m_nCopies =		pd.nCopies;

			/* Save modes.*/
			m_hDevMode =	pd.hDevMode;
			m_hDevNames =	pd.hDevNames;

			/* Success.*/
			return VTRUE;
		}

		return VFALSE;
	}

	/** Return a reference to the number of copies. This can be set before
	showing the dialog box, and is what the user selected after returning
	from DoModal(). The default is 1.*/
	VWORD&					GetCopies() const
		{ return (VWORD&)m_nCopies; }

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

	/** Return a reference to the beginning (from) page. This can be set
	before showing the dialog box, and is what the user selected after
	returning from DoModal(). The default is 1.*/
	VWORD&					GetFromPage() const
		{ return (VWORD&)m_nFromPage; }

	/** Return a reference to the minimum value for the page range specified
	in the From and To pages. If this equal the maximum page value, the
	Pages radio button and the starting and ending page controls are
	disabled. The default is 1.*/
	VWORD&					GetMinPage() const
		{ return (VWORD&)m_nMinPage; }

	/** Return a reference to the maximum value for the page range specified
	in the From and To pages. The default is 1.*/
	VWORD&					GetMaxPage() const
		{ return (VWORD&)m_nMaxPage; }

	/** Return a reference to the device context used for printing. This
	will be NULL until DoModal() return VTRUE.*/
	VDC&					GetPrinterDC() const
		{ return (VDC&)m_PrinterDC; }

	/** Return a reference to the ending (top) page. This can be set before
	showing the dialog box, and is what the user selected after returning
	from DoModal(). The default is 1.*/
	VWORD&					GetToPage() const
		{ return (VWORD&)m_nToPage; }

	/** Set the internal device mode and names handles. These are normally
	populated by this class, but could be given from a page setup dialog
	box using the VPageSetupDialog common dialog class. If you use both
	classes, when each others DoModal() function returns VTRUE, the device
	modes and names should be set into each other. For instance, if you use
	VPageSetupDialog and its DoModal() function returns VTRUE, you should
	call this function and pass the results of
	VPageSetupDialog::GetDevMode(), and VPageSetupDialog::GetDevNames() to
	this function. If at a later time this dialog box resets these values,
	you should call this same function in VPageSetupDialog to update its
	values. This idea is to keep them in sync.*/
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
			VPrintDialog* pThis =
				(VPrintDialog*)((PRINTDLG*)lParam)->lCustData;
			VASSERT(pThis)

			pThis->Attach(hWnd);

			return 1;
		}

		return 0;
	}

	/** Called just before the dialog box is shown. An override can
	adjust PRINTDLG members as needed.*/
	virtual void			PreShow(PRINTDLG& pd)
		{;}

	/** Embedded Members.*/
	VDC						m_PrinterDC;
	HANDLE					m_hDevMode;
	HANDLE					m_hDevNames;
	VWORD					m_nFromPage;
    VWORD					m_nToPage;
    VWORD					m_nMinPage;
    VWORD					m_nMaxPage;
    VWORD					m_nCopies;
};

#endif /* VPRINTDIALOG*/
