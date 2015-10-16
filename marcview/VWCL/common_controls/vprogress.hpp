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

#ifndef VPROGRESS
#define VPROGRESS

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"
#include "vcommoncontrol.hpp"

/** VProgress is simple wrapping of the Win32 progress bar common control.*/
class VProgress : public VCommonControl
{
public:
	/** Default constructor initializes RTTI information in base class.*/
	VProgress(VRTTI_TYPE nRTTI = VWCL_RTTI_PROGRESS)
		: VCommonControl(nRTTI)
		{;}

	/** Create the progress control. Returns VTRUE on success, VFALSE on
	error. nStyle can be any of the standard WS_xxx style bits described in
	VWindow::Create(), plus progress control specific ones including:

	PBS_VERTICAL - Displays progress information vertically, top to bottom.
	Without this flag, the progress bar control displays horizontally, left
	to right.

	PBS_SMOOTH - Displays gradual, smooth filling in the progress bar
	control. Without this flag, the control will fill with blocks.*/
	VBOOL	Create(	VWindow const&	windowParent,
					VRect const&	r,
					VUINT			nID,
					VUINT			nStyle = WS_BORDER | WS_VISIBLE)
	{
		return VCommonControl::Create(	windowParent,
										r,
										WS_CHILD | nStyle,
										PROGRESS_CLASS,
										NULL,
										nID);
	}

	/** Advances the current position of a progress bar by a specified
	increment and redraws the bar to reflect the new position.*/
	VINT	OffsetPos(VINT nPos) const
		{ return (VINT)VWINDOW_SM1(PBM_DELTAPOS, nPos); }

	/** Sets the current position for a progress bar and redraws the bar to
	reflect the new position. Returns the previous position.*/
	VINT	SetPos(VINT nPos) const
		{ return VWINDOW_SM1(PBM_SETPOS, nPos); }

	/** Sets the minimum and maximum values for a progress bar and redraws
	the bar to reflect the new range. Returns the previous range values if
	successful, or zero otherwise. The low-order word specifies the previous
	minimum value, and the high-order word specifies the previous maximum
	value.*/
	VLONG	SetRange(	VINT nLower,
						VINT nUpper) const
		{ return VWINDOW_SM3(PBM_SETRANGE, 0, nLower, nUpper); }

	/** Specifies the step increment for a progress bar. The step increment
	is the amount by which the progress bar increases its current position
	whenever StepIt() is used. By default, the step increment is set to
	10.Returns the previous step increment.*/
	VINT	SetStep(VINT nStep) const
		{ return VWINDOW_SM1(PBM_SETSTEP, nStep); }

	/** Advances the current position for a progress bar by the step
	increment and redraws the bar to reflect the new position. An application
	sets the step increment with SetStep() function. Returns the
	previous position. When the position exceeds the maximum range value,
	this message resets the current position so that the progress indicator
	starts over again from the beginning.*/
	VINT	StepIt() const
		{ return (VINT)VWINDOW_SM0(PBM_STEPIT); }
};

#endif /* VPROGRESS*/
