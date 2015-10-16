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

#ifndef VDIALOG
#define VDIALOG

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "vwindow.hpp"

/** VDialog is the base dialog box encapsulation object and supports
modal and modeless dialog boxes.*/
class VDialog : public VWindow
{
public:
	/** Default constructor simply initializes VWindow base class with
	RTTI information and local members.*/
	VDialog(VRTTI_TYPE nRTTI = VWCL_RTTI_DIALOG)
		: VWindow(nRTTI)
		{ m_bModal = VFALSE; }

	/** Create a non-modal dialog box where the dialog template ID is known
	to a derived class. This function must be overridden in derived classes
	that self-initialize. The base class asserts and returns VFALSE.*/
	virtual VBOOL	Create(VWindow const& windowParent)
	{
		VASSERT(VFALSE)
		return VFALSE;
	}

	/** Create a non-modal dialog box using the dialog template ID specified
	in nID. The dialog template must reside in the module identified by
	hResource. If hResource is NULL, VGetResourceHandle() will be used.
	Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			Create(	VWindow const&	windowParent,
							VUINT			nID,
							HINSTANCE		hResource = NULL)
	{
		/* Modal dialogs cannot be created, only shown with DoModal().*/
		VASSERT(!IsModal())

		/* The dialog template ID must be known!*/
		VASSERT(nID)

		/* Set modality flag.*/
		m_bModal = VFALSE;

		/* Create the dialog box now, passing this object pointer as the
		lParam extra data. VWCL attaches dialog boxes in WM_INITDIALOG,
		which then calls SetupWindow() so that dialog boxes are initialized
		like all other VWCL windows.*/
		return (CreateDialogParam(	(hResource)
									? hResource
									: VGetResourceHandle(),
									MAKEINTRESOURCE(nID),
									windowParent.GetHandle(),
									(DLGPROC)WindowProcShared,
									(VLPARAM)this))
									? VTRUE : VFALSE;
	}

	/** Override makes sure EndModal(IDCANCEL) is called for modal dialogs.*/
	virtual void	Destroy()
	{
		if ( IsModal() )
			EndModal(IDCANCEL);
		else
			VWindow::Destroy();
	}

	/** Create and show a modal dialog box using the dialog template ID
	specified in nID. The dialog template must reside in the module
	identified by hResource. If hResource is NULL, VGetResourceHandle() will
	be used. The return value is the value returned from EndModal(),
	usually IDOK or IDCANCEL.*/
	VINT			DoModal(	VWindow const&	windowParent,
								VUINT			nID,
								HINSTANCE		hResource = NULL)
	{
		/* The dialog template ID must be known!*/
		VASSERT(nID)

		/* Set modality flag.*/
		m_bModal = TRUE;

		/* Keep track of previous focus window so it can be reset
		after dialog goes away.*/
		HWND hWndFocus = GetFocus();

		/* Create the dialog box now, passing this objects pointer as the
		lParam extra data. VWCL attaches dialog boxes in WM_INITDIALOG,
		which then calls SetupWindow() so that dialog boxes are initialized
		like all other VWCL windows.*/
		VINT nResult = DialogBoxParam(	(hResource)
										? hResource
										: VGetResourceHandle(),
										MAKEINTRESOURCE(nID),
										windowParent.GetHandle(),
										(DLGPROC)WindowProcShared,
										(VLPARAM)this);

		/* Reset previous focus.*/
		if ( hWndFocus )
			::SetFocus(hWndFocus);

		/* Update parent window now. It may have been obscurred by the
		dialog, and then processing may start. Doing this will result in
		the parent window being properly painted first, removing the shadow
		that would otherwise be left by the dialog box.*/
		if ( windowParent.GetSafeWindow() )
			windowParent.Update();

		return nResult;
	}

	/** Create a modal dialog box where the dialog template ID is known to a
	derived class. This function must be overridden and derived classes that
	self-initialize. The base class asserts and returns IDCANCEL.*/
	virtual VINT	DoModal(VWindow const& windowParent)
	{
		VASSERT(VFALSE)
		return IDCANCEL;
	}

	/** Determine if a dialog box is modal. Return VTRUE if modal, VFALSE
	if non-modal.*/
	virtual VBOOL	IsModal()
		{ return m_bModal; }

	/** Return VTRUE to indicate we, and all derived classes, are VDialog
	types.*/
	virtual VBOOL	IsVDialogType()
		{ return VTRUE; }

protected:
	/** End a modal dialog box, returning nResult to the caller of DoModal().
	This function should not be used for non-modal dialog boxes. Destroy()
	should be used to remove non-modal dialog boxes.*/
	virtual void	EndModal(VINT nResult = IDOK)
	{
		VASSERT(IsModal())

		if ( GetSafeWindow() && GetRTTI() != VWCL_RTTI_PROPERTY_PAGE )
			EndDialog(m_hWindow, nResult);
	}

	/** The dialog box is going away. Return VTRUE to destroy dialog box or
	VFALSE to keep it active. This is in response to command IDCANCEL or
	closing of the dialog box from the system menu. This does NOT apply to
	modeless dialogs. For modeless dialogs, OnCancel() will still get
	called, but the dialog will NOT be destroyed.*/
	virtual VBOOL	OnCancel()
		{ return VTRUE; }

	/** Override posts a message to be picked up by the IDCANCEL button
	command handler for modal dialogs. If a Cancel button does not exist
	on the dialog, EndModal(IDCANCEL) will be called instead. For non-modal
	dialog boxes, this function calls Destroy().*/
	virtual VLONG	OnClose()
	{
		if ( IsModal() )
		{
			if ( GetDialogItem(IDCANCEL) )
				PostMessage(WM_COMMAND, MAKEWPARAM(IDCANCEL, 0), 0);
			else
				EndModal(IDCANCEL);
		}
		else
			Destroy();

		return 0;
	}

	/** Override looks for OK and Cancel button hits.*/
	virtual VLONG	OnCommand(	VUINT	nNotifyCode,
								VUINT	nCommandID,
								HWND	hWndControl)
	{
		switch ( nCommandID )
		{
			case IDOK:
			{
				/* You should not have IDOK or IDCANCEL buttons
				on a property page!*/
				VASSERT(GetRTTI() != VWCL_RTTI_PROPERTY_PAGE)

				if ( OnOK() )
					EndModal(IDOK);

				return 0;
			}

			case IDCANCEL:
			{
				/* You should not have IDOK or IDCANCEL buttons
				on a property page!*/
				VASSERT(GetRTTI() != VWCL_RTTI_PROPERTY_PAGE)

				if ( OnCancel() )
					EndModal(IDCANCEL);

				return 0;
			}
		}

		return VWindow::OnCommand(nNotifyCode, nCommandID, hWndControl);
	}

	/** The dialog box is going away. Return VTRUE to destroy dialog box or
	VFALSE to keep it active. This is in response to command IDOK. This does
	NOT apply to modeless dialogs. For modeless dialogs, OnOK() will still
	get called, but dialog will NOT be destroyed.*/
	virtual VBOOL	OnOK()
		{ return VTRUE; }

	/** Embedded Members.*/
	VBOOL			m_bModal;
};

#endif /* VDIALOG*/
