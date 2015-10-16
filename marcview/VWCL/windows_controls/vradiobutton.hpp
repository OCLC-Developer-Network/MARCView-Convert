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

#ifndef VRADIOBUTTON
#define VRADIOBUTTON

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"
#include "vcheckbox.hpp"

/** VRadioButton encapsulates the standard Windows radio button control.*/
class VRadioButton : public VCheckBox
{
public:
	/** Default constructor initializes RTTI information in base classes.*/
	VRadioButton(VRTTI_TYPE nRTTI = VWCL_RTTI_RADIO_BUTTON)
		: VCheckBox(nRTTI)
		{;}

	/** Create a new radiobutton control. See VButton::Create() for more
	information on possible style bits.*/
	VBOOL		Create(	VWindow const&	windowParent,
						VRect const&	rect,
						VUINT			nID,
						VSTRING_CONST	pszText = NULL,
						VUINT			nStyle =
										WS_BORDER |
										WS_TABSTOP |
										WS_VISIBLE |
										BS_AUTORADIOBUTTON)
		{ return VButton::Create(windowParent, rect, nID, pszText, nStyle); }
};

#endif /* VRADIOBUTTON*/
