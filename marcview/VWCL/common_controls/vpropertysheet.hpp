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

Primary Author of this source code file:  Chris Waldren (chriswa@microsoft.com)
Other Author(s) of this source code file: Todd Osborne (todd@vwcl.org)

VWCL and all source code are copyright (c) 1996-2000 by The VWCL Alliance.*/

#ifndef VPROPERTYSHEET
#define VPROPERTYSHEET

#include "../vstandard.h"
#include "../collections/vptrarray.hpp"
#include "../windows/vwindow.hpp"
#include "vcommoncontrol.hpp"
#include "vpropertypage.hpp"

class VPropertySheet;

/** Local global for use during creation of property sheet (short lived).*/
static VPropertySheet* g_pPropSheetCurrent = NULL;

/** VPropertySheet contains VPropertyPage objects and acts as the parent
window. This is an encapsulation of the Windows property sheet and property
page functions.*/
class VPropertySheet : public VDialog
{
public:
	/** Default constructor initializes RTTI information and internal members.*/
	VPropertySheet(VRTTI_TYPE nRTTI = VWCL_RTTI_PROPERTY_SHEET)
		: VDialog(nRTTI)
	{
		VZEROSTRUCT(m_psHeader);
		m_pPages = NULL;
	}

	/** Virtual destructor verifies internal members are released.*/
	virtual ~VPropertySheet()
		{ Destroy(); }

	/** Add a single page. If nDialogID is not 0, VPropertyPage::Init() will
	be called on pPage.*/
	VBOOL					AddPage(	VPropertyPage&	propertyPage,
										VUINT			nDialogID = 0,
										HINSTANCE		hResource = NULL)
	{
		if ( nDialogID )
			propertyPage.Init(nDialogID, hResource);

		return (m_listPages.Add(&propertyPage) != -1) ? VTRUE : VFALSE;
	}

	/** Create and show the property sheet as a non-modal one with nStartPage
	being shown as the default tab. The nOptions parameter corresponds to
	the PROPSHEETHEADER structure dwFlags member and nOptions is OR'd with
	existing options set by this class or outside of the class when setting
	the value in this structure. See PROPSHEETHEADER for more information.
	When creating modeless property sheets, your main window object must
	override VWindow::PreTranslateMessage() and call the VWCL global
	function VTranslateDialogMessage() in order to implement normal keyboard
	processing.*/
	VBOOL					Create(	VWindow const&	wndParent,
									VPTRARRAY_INDEX	nStartPage = 0,
									VDWORD			nOptions =
													PSH_NOAPPLYNOW)
		{ return InternalCreate(wndParent, VFALSE, nStartPage, nOptions); }

	/** Override verifies proper cleanup.*/
	virtual void			Destroy()
	{
		/* Do default destruction first.*/
		VDialog::Destroy();

		/* Free allocate memory.*/
		VDELETE_ARRAY_NULL(m_pPages)
	}

	/** Create and show the property sheet as a modal one with nStartPage
	being shown as the default tab. The nOptions parameter corresponds to
	the PROPSHEETHEADER structure dwFlags member and nOptions is OR'd with
	existing options set by this class or outside of the class when setting
	the value in this structure. See PROPSHEETHEADER for more information.*/
	VBOOL					DoModal(	VWindow const&	wndParent,
										VUINT			nStartPage = 0,
										VDWORD			nOptions =
														PSH_NOAPPLYNOW)
		{ return InternalCreate(wndParent, VTRUE, nStartPage, nOptions); }

	/** Return a reference to the internal PROPSHEETHEADER structure.*/
	PROPSHEETHEADER&		GetPROPSHEETHEADER() const
		{ return (PROPSHEETHEADER&)m_psHeader; }

	/** Return the number of propery pages contained within the sheet.*/
	VUINT					GetPageCount() const
		{ return m_listPages.Size(); }

	/** Set the title of the property sheet. If bShowPropertiesForString is
	VTRUE, Windows will preface the title you specify in pszTitle with
	something along the lines of "Properties For".*/
	void					SetTitle(
		VSTRING_CONST	pszTitle,
		VBOOL			bShowPropertiesForString = VTRUE) const
	{
		VASSERT(pszTitle)
		VWINDOW_SM2(PSM_SETTITLE, (bShowPropertiesForString) ? PSH_PROPTITLE : 0, pszTitle);
	}

	/** Same as above, but set the title using string from resources. However,
	Microsoft, in their infinite wisdom, did not provide a way to specify
	from what resource handle to load the resources from. Your guess is as
	good as mine where the string will be loaded from. I wouldn't use this
	function unless the string to be loaded is in the calling executable
	files string resources.*/
	void					SetTitle(	VUINT nStringID,
										VBOOL bShowPropertiesForString =
										VTRUE) const
	{
		SetTitle(	(VSTRING_CONST)MAKELONG(nStringID, NULL),
					bShowPropertiesForString);
	}

	/** Set the active property page using the page ID.*/
	VBOOL					SetActivePage(VUINT nPageID) const
		{ return VWINDOW_SMB2(PSM_SETCURSELID, 0, nPageID); }

	/** Set the active property page using property page index.*/
	VBOOL					SetActivePageByIndex(VUINT nPageIndex) const
		{ return VWINDOW_SMB1(PSM_SETCURSEL, nPageIndex); }

protected:
	/** Internal function creates a Modal or Modeless property sheet.*/
	VBOOL					InternalCreate(	VWindow const&	wndParent,
											VBOOL			bModal,
											VUINT			nStartPage,
											VDWORD			nOptions = 0)
	{
		VUINT nPageCount = GetPageCount();

		/* Pages must have been added using AddPage().*/
		VASSERT(nPageCount)

		/* The start page must be less than the page count.*/
		VASSERT(nStartPage < GetPageCount())

		/* Clean up from previous showing, as needed.*/
		Destroy();

		/* Allocate required memory for page information pointers.*/
		m_pPages = VNEW PROPSHEETPAGE[nPageCount];

		if ( m_pPages )
		{
			/* Copy Property Page information to array.*/
			for ( VUINT i = 0; i < nPageCount; i++ )
				memcpy(	&m_pPages[i],
						&((VPropertyPage*)m_listPages[i])->GetPROPSHEETPAGE(),
						sizeof(PROPSHEETPAGE));

			/* Set up property sheet header.*/
			m_psHeader.dwSize =			sizeof(m_psHeader);
			m_psHeader.dwFlags |=		nOptions |
										PSH_PROPSHEETPAGE |
										PSH_USECALLBACK;
			m_psHeader.hwndParent =		wndParent.GetHandle();
			m_psHeader.hInstance =		VGetInstanceHandle();
			m_psHeader.pfnCallback =	PropertySheetProc;
			m_psHeader.nPages =			nPageCount;
			m_psHeader.ppsp =			m_pPages;
			m_psHeader.nStartPage =		nStartPage;

			/* Modal?*/
			if ( bModal )
				m_psHeader.dwFlags &= ~PSH_MODELESS;
			else
				m_psHeader.dwFlags |= PSH_MODELESS;

			/* Set flag in base class for modality too.*/
			m_bModal = bModal;

			/* Set temporary g_pPropSheetCurrent to this object.*/
			g_pPropSheetCurrent = this;

			/* Display the property sheet.*/
			if ( PropertySheet(&m_psHeader) != -1 )
				return VTRUE;
		}

		/* Free any allocations on error.*/
		VDELETE_ARRAY_NULL(m_pPages)

		return VFALSE;
	}

	/** Override verifies window is properly closed and property pages
	notified.*/
	virtual VLONG			OnClose()
	{
		OnCommand(0, IDCANCEL, NULL);
		return 0;
	}

	/** Override corrects property page mismanagement code from Microsoft.*/
	virtual VLONG			OnCommand(	VUINT	nNotifyCode,
										VUINT	nCommandID,
										HWND	hWndControl)
 	{
		VBOOL bCancelCommand = VFALSE;

		/* Did the user click the IDOK, IDCANCEL, or Apply button?*/
		if (	nCommandID == IDOK ||
				nCommandID == IDCANCEL ||
				nCommandID == 0x3021 )
		{
			VUINT nPages = GetPageCount();

			for ( VUINT i = 0; i < nPages; i++ )
			{
				VPropertyPage* pPage = (VPropertyPage*)m_listPages[i];

				/* Only call page functions when a window has been created.*/
				if ( pPage->GetSafeWindow() )
				{
					/* Determine which function to call.*/
					if ( nCommandID == IDOK || nCommandID == 0x3021 )
						bCancelCommand = (pPage->OnOK()) ? VFALSE : VTRUE;
					else
						bCancelCommand = (pPage->OnCancel()) ? VFALSE : VTRUE;

					if ( bCancelCommand )
					{
						SetActivePageByIndex(i);
						break;
					}
				}
			}

			/* Never close the page when applying, only when OK'ing.*/
			if ( nCommandID == 0x3021 )
				bCancelCommand = VTRUE;

			/* If not modal, destroy the window now.*/
			if ( !bCancelCommand && !m_bModal )
			{
				Destroy();
				bCancelCommand = VTRUE;
			}
		}

		return (bCancelCommand)
			? 0
			: VWindow::OnCommand(nNotifyCode, nCommandID, hWndControl);
	}

	/** Procedure Windows uses to commnicate with us.*/
	static VINT CALLBACK	PropertySheetProc(	HWND hWnd,
												VUINT nMessage,
												VLPARAM lParam)
	{
		if ( nMessage == PSCB_INITIALIZED )
		{
			/* Reset global g_pPropSheetCurrent after saving for
			local use. Globals suck.*/
			VPropertySheet* pThis =	g_pPropSheetCurrent;
			g_pPropSheetCurrent =	NULL;

			/* Temporary pointer must be known!*/
			VASSERT(pThis)

			/* Attach hWnd to this object.*/
			pThis->Attach(hWnd);
		}

		return 0;
	}

	/** Embedded Members.*/
	PROPSHEETHEADER			m_psHeader;
	PROPSHEETPAGE*			m_pPages;
	VPtrArray				m_listPages;
};

#endif /* VPROPERTYSHEET*/
