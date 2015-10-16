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

#ifndef VBUTTON
#define VBUTTON

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"

/** VButton encapsulates the standard Windows button control and serves
as a base class for derived controls.*/
class VButton : public VWindow
{
public:
	/** Default constructor initializes base class RTTI information.*/
	VButton(VRTTI_TYPE nRTTI = VWCL_RTTI_BUTTON)
		: VWindow(nRTTI)
		{;}

#if(WINVER >= 0x0400)
	/** Simulate the user clicking a button. This function causes the button
	to receive a WM_LBUTTONDOWN and a WM_LBUTTONUP message, and the button's
	parent window to receive a BN_CLICKED notification message. If the
	button is in a dialog box, and the dialog box is not active, the
	BM_CLICK message might fail. To ensure success in this situation, call
	the SetActiveWindow() function to activate the dialog box before
	sending the BM_CLICK message to the button.*/
	void		Click() const
		{ VWINDOW_SM0(BM_CLICK); }
#endif

	/** Create a new button control. nStyle is a combination of
	WS_xxx window style bits or'd with these button specific bits:

	BS_AUTOCHECKBOX - Same as a check box, except that a check mark appears
	in the check box when the user selects the box; the check mark
	disappears the next time the user selects the box.

	BS_AUTORADIOBUTTON - Same as a radio button, except that when the user
	selects it, the button automatically highlights itself and removes the
	selection from any other radio buttons with the same style in the same
	group.

	BS_AUTO3STATE - Same as a three-state check box, except that the box
	changes its state when the user selects it.

	BS_CHECKBOX - Creates a small square that has text displayed to its
	right (unless this style is combined with the BS_LEFTTEXT style).

	BS_DEFPUSHBUTTON - Creates a button that has a heavy black border. The
	user can select this button by pressing the ENTER key. This style
	enables the user to quickly select the most likely option (the default
	option).

	BS_GROUPBOX - Creates a rectangle in which other buttons can be grouped.
	Any text associated with this style is displayed in the rectangle’s
	upper-left corner.

	BS_LEFTTEXT - When combined with a radio-button or check-box style, the
	text appears on the left side of the radio button or check box.

	BS_OWNERDRAW - Creates an owner-drawn button. The parent window should
	handle the WM_DRAWITEM message to draw the button.

	BS_PUSHBUTTON - Creates a pushbutton that posts a WM_COMMAND message to
	the owner window when the user selects the button.

	BS_RADIOBUTTON - Creates a small circle that has text displayed to its
	right (unless this style is combined with the BS_LEFTTEXT style). Radio
	buttons are usually used in groups of related but mutually exclusive
	choices.

	BS_3STATE - Same as a check box, except that the box can be dimmed as
	well as checked. The dimmed state typically is used to show that a
	check box has been disabled.*/
	VBOOL		Create(	VWindow const&	windowParent,
						VRect const&	r,
						VUINT			nID,
						VSTRING_CONST	pszText = NULL,
						VUINT			nStyle =
										WS_BORDER |
										WS_TABSTOP |
										WS_VISIBLE |
										BS_PUSHBUTTON)
	{
		VASSERT(windowParent.GetSafeWindow())

		return VWindow::Create(	windowParent,
								r,
								WS_CHILD | nStyle,
								VTEXT("BUTTON"),
								pszText,
								nID);
	}

	/** Determine the state of a button or check box. The return value
	specifies the current state of the button. You can use the following
	bitmasks to extract information about the state:

	0x0003 - Specifies the check state (radio buttons and check boxes only).
	A value of BST_UNCHECKED indicates the button is unchecked; a value of
	BST_CHECKED indicates the button is checked. A radio button is checked
	when it contains a dot; a check box is checked when it contains an X. A
	value of BST_INDETERMINATE indicates the check state is indeterminate
	(applies only if the button has the BS_3STATE or BS_AUTO3STATE style). A
	three-state check box is grayed when its state is indeterminate.

	BST_CHECKED - Indicates the button is checked.

	BST_FOCUS - Specifies the focus state. A nonzero value indicates that
	the button has the keyboard focus.

	BST_INDETERMINATE - Indicates the button is grayed because the state of
	the button is indeterminate. This value applies only if the button has
	the BS_3STATE or BS_AUTO3STATE style.

	BST_PUSHED - Specifies the highlight state. A nonzero value indicates
	that the button is highlighted. A button is automatically highlighted
	when the user positions the cursor over it and presses and holds the
	left mouse button. The highlighting is removed when the user releases
	the mouse button.

	BST_UNCHECKED - Indicates the button is unchecked. Same as a return
	value of zero.*/
	VLONG		GetState() const
		{ return VWINDOW_SM0(BM_GETSTATE); }

	/** Change the highlight state of a button. The highlight state indicates
	whether the button is highlighted as if the user had pushed it.
	bHighlight specifies whether the button is to be highlighted. A value
	of VTRUE highlights the button. A value of VFALSE removes any
	highlighting. Highlighting only affects the appearance of a button. It
	has no effect on the check state of a radio button or check box. A
	button is automatically highlighted when the user positions the cursor
	over it and presses and holds the left mouse button. The highlighting
	is removed when the user releases the mouse button.*/
	void		Highlight(VBOOL bHighlight = VTRUE) const
		{ VWINDOW_SM1(BM_SETSTATE, bHighlight); }

#if(WINVER >= 0x0400)
	/** Retrieve a handle to the image (icon or bitmap) associated with the
	button. nType specifies the type of image to associate with the button.
	This parameter can be one of the following values: IMAGE_BITMAP or
	IMAGE_ICON. The return value is a handle to the image, if any,
	otherwise, it is NULL.*/
	HANDLE		Image(VUINT nType = IMAGE_ICON) const
	{
		/* nType can only be IMAGE_ICON or IMAGE_BITMAP.*/
		VASSERT(nType == IMAGE_ICON || nType == IMAGE_BITMAP)

		return (HANDLE)VWINDOW_SM1(BM_GETIMAGE, nType);
	}

	/** Associate a new image (icon or bitmap) with the button. nType
	specifies the type of image to associate with the button. This parameter
	can be one of the following values: IMAGE_BITMAP or IMAGE_ICON. hImage
	is the handle to the image to associate with the button. The return
	value is a handle to the image previously associated with the button,
	if any, otherwise, it is NULL.*/
	HANDLE		Image(	HANDLE	hImage,
						VUINT	nType = IMAGE_ICON) const
	{
		/* nType can only be IMAGE_ICON or IMAGE_BITMAP.*/
		VASSERT(nType == IMAGE_ICON || nType == IMAGE_BITMAP)

		return (HANDLE)VWINDOW_SM2(BM_SETIMAGE, 0, hImage);
	}
#endif

	/** Returns VTRUE if the button is owner-drawn, VFALSE otherwise.*/
	VBOOL		IsOwnerDrawn()
		{ return (Style() & BS_OWNERDRAW) ? VTRUE : VFALSE; }
};

#endif /* VBUTTON*/
