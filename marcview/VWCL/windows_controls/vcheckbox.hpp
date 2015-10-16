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

#ifndef VCHECKBOX
#define VCHECKBOX

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"
#include "vbutton.hpp"

/** VCheckBox encapsulates the standard Windows checkbox control.*/
class VCheckBox : public VButton
{
public:
	/** Default constructor initializes base class RTTI information.*/
	VCheckBox(VRTTI_TYPE nRTTI = VWCL_RTTI_CHECKBOX)
		: VButton(nRTTI)
		{;}

	/** Set the check state of a radio button or check box. See
	GetCheck() for possible values for nCheck.*/
	void		Check(VUINT nCheck = BST_CHECKED) const
		{ VWINDOW_SM1(BM_SETCHECK, nCheck); }

	/** Create a new checkbox control. See VButton::Create() for more
	information on possible style bits.*/
	VBOOL		Create(	VWindow const&	windowParent,
						VRect const&	r,
						VUINT			nID,
						VSTRING_CONST	pszText = NULL,
						VUINT			nStyle =
										WS_BORDER |
										WS_TABSTOP |
										WS_VISIBLE |
										BS_AUTOCHECKBOX)
		{ return VButton::Create(windowParent, r, nID, pszText, nStyle); }

	/** Retrieve the check state of the radio button or check box. The
	return value from a button created with the BS_AUTOCHECKBOX,
	BS_AUTORADIOBUTTON, BS_AUTO3STATE, BS_CHECKBOX, BS_RADIOBUTTON, or
	BS_3STATE style can be one of the following:

	BST_CHECKED - Button is checked.

	BST_INDETERMINATE - Button is grayed, indicating an indeterminate
	state (applies only if the button has the BS_3STATE or BS_AUTO3STATE
	style).

	BST_UNCHECKED - Button is unchecked.

	If the button has any other style, the return value is zero.*/
	VLONG		GetCheck() const
		{ return VWINDOW_SM0(BM_GETCHECK); }

	/** Same as GetCheck(), but returns VTRUE if checked, VFALSE if not.*/
	VBOOL		IsChecked() const
		{ return (GetCheck() == BST_CHECKED) ? VTRUE : VFALSE; }
};

#endif /* VCHECKBOX*/
