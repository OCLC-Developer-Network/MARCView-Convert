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

#ifndef VTOOLBAR
#define VTOOLBAR

#include "../vstandard.h"
#include "../gdi/vdc.hpp"
#include "../gdi/vgdiobjectselector.hpp"
#include "../gdi/vpen.hpp"
#include "../gdi/vstockpen.hpp"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"
#include "vcommoncontrol.hpp"

/** VToolBar encapuslates the Win32 toolbar common control.*/
class VToolBar : public VCommonControl
{
public:
	/** Eumeration of toolbar window options. The default is OPTION_NONE
	(no options), but can be changed using the GetToolBarOptions() method.
	OPTION_DRAW_REBAR_STYLE causes the toolbar to draw a border around
	itself similar to the way some applications use a rebar control. It
	does NOT look good with all possible window style bits, but is
	generally used when TBSTYLE_FLAT is set. The recommended style bits
	when using this options are:
	WS_CHILD | WS_VISIBLE | CCS_NODIVIDER | CCS_NORESIZE | TBSTYLE_FLAT |
	TBSTYLE_TOOLTIPS. If this option is set, you must also set the VWindow
	base class option OPTION_NEED_ON_PAINT to enable custom painting. The
	easiest way to enable rebar style painting is to call the
	EnableRebarStyle() function.*/
	enum	{		OPTION_NONE =					0x0000,
					OPTION_DRAW_REBAR_STYLE =		0x0001,
			};

	/** Default constructor initializes base class RTTI information.*/
	VToolBar(VRTTI_TYPE nRTTI = VWCL_RTTI_TOOLBAR)
		: VCommonControl(nRTTI)
		{;}

	/** Adds one or more images to the list of button images available for
	the toolbar. Returns the index of the first new image if successful, or
	-1 otherwise. nNumButtons specifies the number of button images in the
	bitmap. addBitmap is a TBADDBITMAP structure that contains the
	identifier of a bitmap resource and the handle to the module instance
	with the executable file that contains the bitmap resource.*/
	VINT				AddBitmap(	VWPARAM				nNumButtons,
									TBADDBITMAP const&	addBitmap) const
		{ return (VINT)VWINDOW_SM2(TB_ADDBITMAP, nNumButtons, &addBitmap); }

	/** Same as above, but loads a bitmap from application resources.*/
	VINT				AddBitmap(	VWPARAM		nNumButtons,
									VUINT		nBitmapID,
									HINSTANCE	hResource = NULL) const
	{
		TBADDBITMAP addBitmap =
			{(hResource) ? hResource : VGetResourceHandle(), nBitmapID};

		return this->AddBitmap(nNumButtons, addBitmap);
	}

	/** Adds one or more buttons to a toolbar. Returns VTRUE if successful,
	or VFALSE otherwise. nNumButtons is the number of buttons to add.
	pButtons is the address of an array of TBBUTTON structures that contain
	information about the buttons to add. There must be the same number of
	elements in the array as buttons specified by nNumButtons.*/
	VBOOL				AddButtons(	VUINT			nNumButtons,
									TBBUTTON const*	pButtons) const
		{ return VWINDOW_SMB2(TB_ADDBUTTONS, nNumButtons, pButtons); }

	/** Adds a new string to the list of strings available for a toolbar.
	Returns the index of the first new string if successful, or -1 otherwise.
	hResource is the handle to the module instance with an executable file
	that contains the string resource. If nStringID points to one or more
	strings to add, this parameter is zero. nStringID is a resource
	identifier for the string resource, or the address of a buffer that
	contains one or more null-terminated strings to add to the list,
	depending on the value of hResource. The last string must be terminated
	with two null characters.*/
	VLONG				AddString(	VWORD		nStringID,
									HINSTANCE	hResource = NULL) const
	{
		return VWINDOW_SM3(TB_ADDSTRING, ((hResource) ? hResource : VGetResourceHandle()), nStringID, 0);
	}

	/** Causes a toolbar to be resized. An application calls AutoSize() after
	causing the size of a toolbar to change either by setting the button or
	bitmap size or by adding strings for the first time. This base class
	function sends the TB_AUTOSIZE message which sizes the toolbar to fit
	the parent window. An override of this message can size the toolbar
	however it see's fit. Make sure to check the window style bits, since
	some of them cause the toolbar to be sized in other ways. If the
	toolbar option OPTION_DRAW_REBAR_STYLE is set, this function will not
	send the TB_AUTOSIZE message, but will get the toolbar height based on
	the button in it, and size the toolbar to fit snuggly inside the rebar
	drawing area. It will still be the width of the parent window.*/
	virtual void		AutoSize()
	{
		if ( m_ToolBarOptions.IsSet(OPTION_DRAW_REBAR_STYLE) )
		{
			VRect r;
			VWindow* pParent = GetVParent();
			VASSERT(pParent)

			if ( pParent )
			{
				/* Get the parent window. We only use it for its width.*/
				pParent->GetClientRect(r);

				/* Get the rectangle of the last tab. We need it for its
				bottom point.*/
				VRect rButton;
				GetItemRect(GetButtonCount() - 1, rButton);

				/* Make the toolbar fit snug with rebar drawing area.*/
				r.GetBottom() = rButton.GetBottom();

				/* Move toolbar into place.*/
				Move(r);

				return;
			}
		}

		VWINDOW_SM0(TB_AUTOSIZE);
	}

	/** Changes the bitmap for a button in a toolbar. Returns VTRUE if
	successful, or VFALSE otherwise. nButtonID is the command identifier of
	the button that is to receive a new bitmap. nBitmapIndex is the
	zero-based index of an image in the toolbar's image list. The system
	displays the specified image in the button.*/
	VBOOL				ChangeBitmap(	VUINT nButtonID,
										VWORD nBitmapIndex) const
		{ return VWINDOW_SMB3(TB_CHANGEBITMAP, nButtonID, nBitmapIndex, 0); }

	/** Checks or unchecks a given button in the toolbar. Returns VTRUE if
	successful, or VFALSE otherwise. nButtonID is the command identifier of
	the button to check. if bCheck is VTRUE the button will be placed in a
	checked state, unchecked otherwise.*/
	VBOOL				CheckButton(	VWPARAM	nButtonID,
										VBOOL	bCheck = VTRUE) const
	{
		if ( IsButtonChecked(nButtonID) != bCheck )
			return VWINDOW_SMB3(TB_CHECKBUTTON, nButtonID, bCheck, 0);

		return VTRUE;
	}

	/** Returns the zero-based index for the button associated with the
	specified command identifier.*/
	VUINT				CommandToIndex(VWPARAM nButtonID) const
		{ return (VUINT)VWINDOW_SM1(TB_COMMANDTOINDEX, nButtonID); }

	/** Create the toolbar control. Returns VTRUE on success, VFALSE on error.
	nStyle can be any of the standard WS_xxx style bits described in
	VWindow::Create(), plus toolbar specific ones including:

	CCS_ADJUSTABLE - Allows toolbars to be customized by the user. If this
	style is used, the toolbar’s owner window must handle the customization
	notification messages sent by the toolbar.

	CCS_BOTTOM - Causes the control to position itself at the bottom of the
	parent window’s client area and sets the width to be the same as the
	parent window’s width.

	CCS_NODIVIDER - Prevents a two-pixel highlight from being drawn at the
	top of the control.

	CCS_NOHILITE - Prevents a one-pixel highlight from being drawn at the
	top of the control.

	CCS_NOMOVEY - Causes the control to resize and move itself horizontally,
	but not vertically, in response to a WM_SIZE message. If the
	CCS_NORESIZE style is used, this style does not apply.

	CCS_NOPARENTALIGN - Prevents the control from automatically moving to
	the top or bottom of the parent window. Instead, the control keeps its
	position within the parent window despite changes to the size of the
	parent window. If the CCS_TOP or CCS_BOTTOM style is also used, the
	height is adjusted to the default, but the position and width remain
	unchanged.

	CCS_NORESIZE - Prevents the control from using the default width and
	height when setting its initial size or a new size. Instead, the
	control uses the width and height specified in the request for creation
	or sizing.

	CCS_TOP - Causes the control to position itself at the top of the
	parent window’s client area and sets the width to be the same as the
	parent window’s width. Toolbars have this style by default.

	TBSTYLE_ALTDRAG - Allows users to change a toolbar button's position
	by dragging it while holding down ALT. If this style is not specified,
	the user must hold down SHIFT while dragging a button. Note that the
	CCS_ADJUSTABLE style must be specified to enable toolbar buttons to be
	dragged.

	TBSTYLE_CUSTOMERASE - Version 4.70 - Generates NM_CUSTOMDRAW notification
	messages when it processes WM_ERASEBKGND messages.

	TBSTYLE_FLAT - Version 4.70 - Creates a flat toolbar. In a flat toolbar,
	both the toolbar and the buttons are transparent. Button text appears
	under button bitmaps. To prevent repainting problems, this style should
	be set before the toolbar control becomes visible.

	TBSTYLE_LIST - Version 4.70 - Places button text to the right of button
	bitmaps. To prevent repainting problems, this style should be set
	before the toolbar control becomes visible.

	TBSTYLE_REGISTERDROP - Version 4.71 - Generates TBN_GETOBJECT
	notification messages to request drop target objects when the pointer
	passes over toolbar buttons.

	TBSTYLE_TOOLTIPS - Creates a tooltip control that an application can
	use to display descriptive text for the buttons in the toolbar.

	TBSTYLE_TRANSPARENT - Version 4.71 - Creates a transparent toolbar. In
	a transparent toolbar, the toolbar is transparent but the buttons are
	not. Button text appears under button bitmaps. To prevent repainting
	problems, this style should be set before the toolbar control becomes
	visible.

	TBSTYLE_WRAPABLE - Creates a toolbar that can have multiple lines of
	buttons. Toolbar buttons can "wrap" to the next line when the toolbar
	becomes too narrow to include all buttons on the same line. Wrapping
	occurs on separation and nongroup boundaries.*/
	VBOOL				Create(	VWindow const&	windowParent,
								VRect const&	r,
								VUINT			nID,
								VUINT			nStyle =
												WS_BORDER |
												WS_VISIBLE |
												CCS_TOP |
												TBSTYLE_TOOLTIPS |
												TBSTYLE_WRAPABLE)
	{
		return VCommonControl::Create(	windowParent,
										r,
										nStyle | WS_CHILD,
										TOOLBARCLASSNAME,
										NULL,
										nID);
	}

	/** Displays the Customize Toolbar dialog box.*/
	void				Customize() const
		{ VWINDOW_SM0(TB_CUSTOMIZE); }

	/** Deletes a button from the toolbar. Returns VTRUE if successful, or
	VFALSE otherwise. nButtonIndex is the zero-based index of the button
	to delete.*/
	VBOOL				DeleteButton(VWPARAM nButtonIndex) const
		{ return VWINDOW_SMB1(TB_DELETEBUTTON, nButtonIndex); }

	/** Enables or disables the specified button in the toolbar. Returns
	VTRUE if successful, or VFALSE otherwise. nButtonID is the command
	identifier of the button to enable or disable. If bEnable is VTRUE, the
	button is enabled. If it is VFALSE, the button is disabled. When a
	button has been enabled, it can be pressed and checked.*/
	VBOOL				EnableButton(	VWPARAM	nButtonID,
										VBOOL	bEnable = VTRUE) const
	{
		if ( IsButtonEnabled(nButtonID) != bEnable )
			return VWINDOW_SMB3(TB_ENABLEBUTTON, nButtonID, bEnable, 0);

		return VTRUE;
	}

	/** Set the OPTION_DRAW_REBAR_STYLE and base VWindow classes
	OPTION_NEED_ON_PAINT options. This causes the toolbar to draw a border
	around itself similar to the way some applications use a rebar control.
	It does not look good with all possible window style bits, but is
	generally used when TBSTYLE_FLAT is set. The recommended style bits
	when using this options are:
	WS_CHILD | WS_VISIBLE | CCS_NODIVIDER | CCS_NORESIZE | TBSTYLE_FLAT |
	TBSTYLE_TOOLTIPS. This function MUST be called before the window is
	created.*/
	void				EnableRebarStyle()
	{
		/* The window must not already exist!*/
		VASSERT(!GetSafeWindow())

		/* Set local and base class option bits.*/
		m_ToolBarOptions.Append(OPTION_DRAW_REBAR_STYLE);
		GetOptions().Append(OPTION_NEED_ON_PAINT);
	}

	/** Retrieves the index of the bitmap associated with a button in a
	toolbar. Returns the index of the bitmap if successful, or zero
	otherwise.*/
	VUINT				GetBitmap(VWPARAM nButtonID) const
		{ return (VUINT)VWINDOW_SM1(TB_GETBITMAP, nButtonID); }

	/** Retrieves information about the specified button in the toolbar.
	Returns VTRUE if successful, or VFALSE otherwise. nButtonIndex is the
	zero-based index of the button for which to retrieve information.
	button is a reference to the TBBUTTON structure that receives the
	button information.*/
	VBOOL				GetButton(	VWPARAM		nButtonIndex,
									TBBUTTON&	button) const
		{ return VWINDOW_SMB2(TB_GETBUTTON, nButtonIndex, &button); }

	/** Return the number of buttons currently in the toolbar.*/
	VUINT				GetButtonCount() const
		{ return (VUINT)VWINDOW_SM0(TB_BUTTONCOUNT); }

	/** Retrieves the text of a button in a toolbar. Returns the length, in
	characters, of the string copied to the specified buffer if successful.
	(The length does not include the terminating null character.) Otherwise,
	the return value is -1. nButtonID is the command identifier of the
	button whose text is to be retrieved. pszBuffer is a pointer to the
	buffer that receives the button text. The brain-dead implementation of
	the toolbar control by Microsoft does not allow us to get the length of
	the string, so we canot dynamically allocate this string and you must
	make sure pszBuffer is sufficiently large enough to handle the text.
	Microsoft did not even provide a way to specifiy the max length of
	pszBuffer!*/
	VINT				GetButtonText(	VWPARAM	nButtonID,
										VSTRING	pszBuffer) const
		{ return (VINT)VWINDOW_SM2(TB_GETBUTTONTEXT, nButtonID, pszBuffer); }

	/** Retrieves the bounding rectangle of a button in the toolbar. Returns
	VTRUE if successful, or VFALSE otherwise. nButtonIndex is the zero-based
	index of the button for which to retrieve information. r is a VRect
	object that receives the coordinates of the bounding rectangle. This
	function does not retrieve the bounding rectangle for buttons whose
	state is set to the TBSTATE_HIDDEN value.*/
	VBOOL				GetItemRect(	VWPARAM	nButtonIndex,
										VRect&	r) const
		{ return VWINDOW_SMB2(TB_GETITEMRECT, nButtonIndex, r.GetPointer()); }

	/** Retrieve the toolbar options object. See enumeration at top of class
	for more information.*/
	VBitmask&			GetToolBarOptions() const
		{ return (VBitmask&)m_ToolBarOptions; }

	/** Sets or clears the indeterminate state of the specified button in
	the toolbar. Returns VTRUE if successful, or VFALSE otherwise. nButtonID
	is the command identifier of the button whose indeterminate state is to
	be set or cleared. If bIndeterminate is VTRUE, the indeterminate state
	is set. If it is VFALSE, the state is cleared.*/
	VBOOL				Indeterminate(	VWPARAM	nButtonID,
										VBOOL	bIndeterminate = VTRUE) const
		{ return VWINDOW_SMB3(TB_INDETERMINATE, nButtonID, bIndeterminate, 0); }

	/** Inserts a button in the toolbar. Returns VTRUE if successful, or
	VFALSE otherwise. nButtonIndex is the zero-based index of a button. The
	message inserts the new button to the left of this button. button is a
	reference to a TBBUTTON structure containing information about the
	button to insert. It must be pre-initialized.*/
	VBOOL				InsertButton(	VWPARAM			nButtonIndex,
										TBBUTTON const&	button) const
		{ return VWINDOW_SMB2(TB_INSERTBUTTON, nButtonIndex, &button); }

	/** Determines whether the specified button in the toolbar is checked.
	Returns VTRUE if the button is checked, or VFALSE otherwise. nButtonID
	is the command identifier of the button to check.*/
	VBOOL				IsButtonChecked(VWPARAM nButtonID) const
		{ return VWINDOW_SMB1(TB_ISBUTTONCHECKED, nButtonID); }

	/** Determines whether the specified button in the toolbar is enabled.
	Returns VTRUE if the button is enabled, or VFALSE otherwise. nButtonID
	is the command identifier of the button to check.*/
	VBOOL				IsButtonEnabled(VWPARAM nButtonID) const
		{ return VWINDOW_SMB1(TB_ISBUTTONENABLED, nButtonID); }

	/** Determines whether the specified button in the toolbar is hidden.
	Returns VTRUE if the button is hidden, or VFALSE otherwise. nButtonID
	is the command identifier of the button to check.*/
	VBOOL				IsButtonHidden(VWPARAM nButtonID) const
		{ return VWINDOW_SMB1(TB_ISBUTTONHIDDEN, nButtonID); }

	/** Determines whether the specified button in the toolbar is in an
	indeterminate state. Returns VTRUE if the button is, or VFALSE
	otherwise. nButtonID is the command identifier of the button to check.*/
	VBOOL				IsButtonIndeterminate(VWPARAM nButtonID) const
		{ return VWINDOW_SMB1(TB_ISBUTTONINDETERMINATE, nButtonID); }

	/** Determines whether the specified button in the toolbar is pressed.
	Returns VTRUE if the button is pressed, or VFALSE otherwise. nButtonID
	is the command identifier of the button to check.*/
	VBOOL				IsButtonPressed(VWPARAM nButtonID) const
		{ return VWINDOW_SMB1(TB_ISBUTTONPRESSED, nButtonID); }

	/** Presses or releases the specified button in the toolbar. Returns
	VTRUE if successful, or VFALSE otherwise. nButtonID is the command
	identifier of the button to press or release. If bPress is VTRUE, the
	button is pressed. If it is VFALSE, the button is released.*/
	VBOOL				PressButton(	VWPARAM	nButtonID,
										VBOOL	bPress = VTRUE) const
		{ return VWINDOW_SMB3(TB_PRESSBUTTON, nButtonID, bPress, 0); }

	/** Returns the number of rows of buttons in a toolbar with the
	TBSTYLE_WRAPABLE style.*/
	VUINT				Rows() const
		{ return (VUINT)VWINDOW_SM0(TB_GETROWS); }

	/** Sets the number of rows of buttons in a toolbar. nNumRows is the
	number of rows requested. The minimum number of rows is one, and the
	maximum number of rows is equal to the number of buttons in the toolbar.
	bLarger is a flag that indicates whether to create more rows than
	requested when the system cannot create the number of rows specified by
	nNumRows. If this parameter is VTRUE, the system creates more rows. If
	it is VFALSE, the system creates fewer rows. r is a VRect object that
	receives the bounding rectangle of the toolbar after the rows are set.
	Because the system does not break up button groups when setting the
	number of rows, the resulting number of rows might differ from the
	number requested.*/
	void				Rows(	VWORD	nNumRows,
								VBOOL	bLarger,
								VRect&	r) const
	{
		VASSERT(nNumRows >= 1 && nNumRows <= GetButtonCount())
		VWINDOW_SM2(TB_SETROWS, MAKEWPARAM(nNumRows, bLarger), r.GetPointer());
	}

	/** Saves or restores the state of the toolbar. If bSave is VTRUE, the
	information is saved. If it is VFALSE, it is restored. saveParams is a
	structure that specifies the registry key, subkey, and value name for
	the toolbar state information. To save and restore a toolbar's settings
	using the TB_SAVERESTORE message, the parent window of the toolbar
	control must implement a handler for the TBN_GETBUTTONINFO notification
	message. The toolbar uses this notification to retrieve information
	about the buttons as they are read out of the registry.*/
	void				SaveRestore(	TBSAVEPARAMS&	saveParams,
										VBOOL			bSave = VTRUE) const
		{ VWINDOW_SM2(TB_SAVERESTORE, bSave, &saveParams); }

	/** Sets the size of the bitmapped images to be added to the toolbar.
	Returns VTRUE if successful, or VFALSE otherwise. nWidth is the width,
	in pixels, of the bitmapped images. nHeight is the height, in pixels, of
	the bitmapped images. The size can be set only before adding any bitmaps
	to the toolbar. If an application does not explicitly set the bitmap
	size, the size defaults to 16 by 15 pixels.*/
	VBOOL				SetBitmapSize(	VWORD	nWidth,
										VWORD	nHeight) const
		{ return VWINDOW_SMB3(TB_SETBITMAPSIZE, 0, nWidth, nHeight); }

	/** Sets the size of the buttons to be added to the toolbar. Returns
	VTRUE if successful, or VFALSE otherwise. nWidth is the width, in
	pixels, of the buttons. nHeight is the height, in pixels, of the
	buttons. The size can be set only before adding any buttons to the
	toolbar. If an application does not explicitly set the button size,
	the size defaults to 24 by 22 pixels.*/
	VBOOL				SetButtonSize(VWORD nWidth, VWORD nHeight) const
		{ return VWINDOW_SMB3(TB_SETBUTTONSIZE, 0, nWidth, nHeight); }

	/** Specifies the size of the TBBUTTON structure. The system uses the
	size to determine which version of the common control dynamic-link
	library (DLL) is being used. If an application uses the CreateWindowEx()
	function to create the toolbar, the application must send this message
	to the toolbar before sending the TB_ADDBITMAP or TB_ADDBUTTONS message.
	The CreateToolbarEx() function automatically sends TB_BUTTONSTRUCTSIZE,
	and the size of the TBBUTTON structure is a parameter of the function.
	VWCL created toolbar controls do this for you in SetupWindow().*/
	void				SetButtonStructSize() const
		{ VWINDOW_SM1(TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON)); }

	/** Sets the command identifier of a toolbar button. Returns VTRUE if
	successful, or VFALSE otherwise. nButtonIndex is the zero-based index
	of the button whose command identifier is to be set. nCommandID is the
	command identifier.*/
	VBOOL				SetCommandID(	VWPARAM	nButtonIndex,
										VUINT	nCommandID) const
		{ return VWINDOW_SMB2(TB_SETCMDID, nButtonIndex, nCommandID); }

	/** Sets the window to which the toolbar control sends notification
	messages. wndParent is the parent window which will receive notification
	messages. The SetNotifyParent() function does not change the parent
	window that was specified when the control was created. Calling the
	GetParent() function for a toolbar control will return the actual
	parent window, not the window specified in TB_SETPARENT. To change the
	control's parent window, call the Parent(window) function.*/
	void				SetNotifyParent(VWindow const& wndParent) const
		{ VWINDOW_SM1(TB_SETPARENT, wndParent.GetHandle()); }

	/** Show or hides the specified button in the toolbar. Returns VTRUE if
	successful, or VFALSE otherwise. nButtonID is the command identifier of
	the button to hide or show. If bShow is VFALSE, the button is hidden. If
	it is VTRUE, the button is shown.*/
	VBOOL				ShowButton(	VWPARAM	nButtonID,
									VBOOL	bShow = VTRUE) const
	{
		return VWINDOW_SMB3(TB_HIDEBUTTON, nButtonID, (bShow) ? VFALSE : VTRUE, 0);
	}

	/** Retrieves information about the state of the specified button in the
	toolbar, such as whether it is enabled, pressed, or checked. Returns the
	button state information if successful, or -1 otherwise. The button
	state information can be a combination of the values listed in Toolbar
	Button States. nButtonID is the command identifier of the button for
	which to retrieve information.*/
	VWORD				State(VWPARAM nButtonID) const
		{ (VWORD)VWINDOW_SM1(TB_GETSTATE, nButtonID); }

	/** Sets the state for the specified button in the toolbar. Returns VTRUE
	if successful, or VFALSE otherwise. nButton ID is the command identifier
	of the button. nState is a combination of the values listed in Toolbar
	Button States.*/
	VBOOL				State(	VWPARAM	nButtonID,
								VWORD	nState) const
		{ return VWINDOW_SMB3(TB_SETSTATE, nButtonID, nState, 0); }

	/** Retrieves the handle to the tooltip control, if any, associated with
	the toolbar, or NULL if none.*/
	HWND				ToolTips() const
		{ return (HWND)VWINDOW_SM0(TB_GETTOOLTIPS); }

	/** Associates a tooltip control with the toolbar. Any buttons added to a
	toolbar before calling this function will not be registered with the
	tooltip control.*/
	void				ToolTips(VWindow const& wndToolTips) const
		{ VWINDOW_SM1(TB_SETTOOLTIPS, wndToolTips.GetHandle()); }

protected:
	/** Override perform rebar control style border painting if the
	OPTION_DRAW_REBAR_STYLE toolbar style is specified.*/
	virtual VBOOL		OnPaint(VDC& dc)
	{
		/* Allow toolbar to do normal painting first.*/
		VCommonControl::WindowProc(	m_hWindow,
									WM_PAINT,
									(VWPARAM)dc.GetHandle(),
									0);

		if ( m_ToolBarOptions.IsSet(OPTION_DRAW_REBAR_STYLE) )
		{
			/* Get our rectangle.*/
			VRect rClient;
			GetClientRect(rClient);

			/* Use a RECT structure for fastest drawing.*/
			RECT& r = rClient.GetRECT();

			/* Subtract from right and bottom edges.*/
			r.right--;
			r.bottom--;

			/* Create a dark gray pen first.*/
			VPen pen;

			if ( pen.Create(PS_SOLID, 1, RGB(127,127,127)) )
			{
				VGDIObjectSelector os(dc, pen);
				dc.MoveTo(r.left,		r.bottom - 1);
				dc.LineTo(r.left,		r.top);
				dc.LineTo(r.right - 1,	r.top);
				dc.LineTo(r.right - 1,	r.bottom - 1);
				dc.LineTo(r.left,		r.bottom - 1);
			}

			/* Now draw white part.*/
			VStockPen pen2(WHITE_PEN);
			VGDIObjectSelector os(dc, pen2);

			dc.MoveTo(r.left + 1,	r.bottom - 2);
			dc.LineTo(r.left + 1,	r.top + 1);
			dc.LineTo(r.right - 1,	r.top + 1);

			dc.MoveTo(r.right,		r.top);
			dc.LineTo(r.right,		r.bottom);
			dc.LineTo(r.left - 1,	r.bottom);
		}

		/* Message handled.*/
		return VFALSE;
	}

	/** Override initializes common control, which is required by
	the toolbar control.*/
	virtual VBOOL		SetupWindow()
	{
		/* Complte toolbar control initialization.*/
		SetButtonStructSize();
		SetButtonSize(24, 22);
		SetBitmapSize(16, 15);

		return VCommonControl::SetupWindow();
	}

	/** Embedded Members.*/
	VBitmask			m_ToolBarOptions;
};

#endif /* VTOOLBAR*/
