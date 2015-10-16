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

Primary Author of this source code file:	Todd Osborne
											(todd@vwcl.org)
Other Author(s) of this source code file:	Charles Mydlak
											(cmydlak@alphalink.com.au)

VWCL and all source code are copyright (c) 1996-2000 by The VWCL Alliance.*/

#ifndef VSCROLLBAR
#define VSCROLLBAR

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"

/** VScrollBar encapsulates the standard Windows scrollbar control.*/
class VScrollBar : public VWindow
{
public:
	/** Default constructor initializes base class RTTI information.*/
	VScrollBar(VRTTI_TYPE nRTTI = VWCL_RTTI_SCROLLBAR)
		: VWindow(nRTTI)
		{;}

	/** Create a new static control. nStyle is a combination of WS_xxx
	window style bits or'd with these button specific bits:

	SBS_BOTTOMALIGN - Used with the SBS_HORZ style. The bottom edge of
	the scroll bar is aligned with the bottom edge of the rectangle
	specified in the Create() member function. The scroll bar has the
	default height for system scroll bars.

	SBS_HORZ - Designates a horizontal scroll bar. If neither the
	SBS_BOTTOMALIGN nor SBS_TOPALIGN style is specified, the scroll bar
	has the height, width, and position given in the Create() member
	function.

	SBS_LEFTALIGN - Used with the SBS_VERT style. The left edge of the
	scroll bar is aligned with the left edge of the rectangle specified
	in the Create() member function. The scroll bar has the default width
	for system scroll bars.

	SBS_RIGHTALIGN - Used with the SBS_VERT style. The right edge of the
	scroll bar is aligned with the right edge of the rectangle specified
	in the Create() member function. The scroll bar has the default width
	for system scroll bars.

	SBS_SIZEBOX - Designates a size box. If neither the
	SBS_SIZEBOXBOTTOMRIGHTALIGN nor SBS_SIZEBOXTOPLEFTALIGN style is
	specified, the size box has the height, width, and position given in
	the Create() member function.

	SBS_SIZEBOXBOTTOMRIGHTALIGN - Used with the SBS_SIZEBOX style. The
	lower-right corner of the size box is aligned with the lower-right
	corner of the rectangle specified in the Create() member function.
	The size box has the default size for system size boxes.

	SBS_SIZEBOXTOPLEFTALIGN - Used with the SBS_SIZEBOX style. The
	upper-left corner of the size box is aligned with the upper-left
	corner of the rectangle specified in the Create() member function. The
	size box has the default size for system size boxes.

	SBS_TOPALIGN - Used with the SBS_HORZ style. The top edge of the
	scroll bar is aligned with the top edge of the rectangle specified
	in the Create() member function. The scroll bar has the default height
	for system scroll bars.

	SBS_VERT - Designates a vertical scroll bar. If neither the
	SBS_RIGHTALIGN nor SBS_LEFTALIGN style is specified, the scroll bar has
	the height, width, and position given in the Create() member function.*/
	VBOOL		Create(	VWindow const&	windowParent,
						VRect const&	rect,
						VUINT			nID,
						VUINT			nStyle =
										WS_VISIBLE |
										SBS_BOTTOMALIGN |
										SBS_HORZ)
	{
		VASSERT(windowParent.GetSafeWindow())

		return VWindow::Create(	windowParent,
								rect,
								WS_CHILD | nStyle,
								VTEXT("SCROLLBAR"),
								NULL,
								nID);
	}

	/** Enable or disable one or both arrows of a scroll bar control. nFlags
	can be one of the following values:

	ESB_DISABLE_BOTH - Disables both arrows on a scroll bar.

	ESB_DISABLE_DOWN - Disables the down arrow on a vertical scroll bar.

	ESB_DISABLE_LTUP - Disables the left arrow on a horizontal scroll bar
	or the up arrow on a vertical scroll bar.

	ESB_DISABLE_LEFT - Disables the left arrow on a horizontal scroll bar.

	ESB_DISABLE_RTDN - Disables the right arrow on a horizontal scroll bar
	or the down arrow on a vertical scroll bar.

	ESB_DISABLE_UP - Disables the up arrow on a vertical scroll bar.

	ESB_ENABLE_BOTH - Enables both arrows on a scroll bar.*/
	VBOOL		EnableArrows(VUINT nFlags = ESB_ENABLE_BOTH) const
		{ return VWINDOW_SMB1(SBM_ENABLE_ARROWS, nFlags); }

	/** Retrieve the current position of the scroll box of a scroll bar
	control. The current position is a relative value that depends on the
	current scrolling range. For example, if the scrolling range is 0
	through 100 and the scroll box is in the middle of the bar, the current
	position is 50.*/
	VLONG		Position() const
		{ return VWINDOW_SM0(SBM_GETPOS); }

	/** Set the position of the scroll box (thumb) and, if requested, redraw
	the scroll bar to reflect the new position of the scroll box. nPosition
	specifies the new position of the scroll box. It must be within the
	scrolling range. bRedraw specifies whether the scroll bar should be
	redrawn to reflect the new scroll box position. If this parameter is
	VTRUE, the scroll bar is redrawn. If it is VFALSE, the scroll bar is
	not redrawn. If the scroll bar control is redrawn by a subsequent call
	to another function, setting the bRedraw parameter to VFALSE is useful.
	If the position of the scroll box changed, the return value is the
	previous position of the scroll box; otherwise, it is zero.*/
	VLONG		Position(	VUINT nPosition,
							VBOOL bRedraw = VTRUE) const
		{ return VWINDOW_SM2(SBM_SETPOS, nPosition, bRedraw); }

	/** Retrieve the minimum and maximum position values for the control.
	nMinPosition is a reference to a value that receives the minimum
	scrolling position. nMaxPosition is a reference to a value that
	receives the maximum scrolling position.*/
	void		GetRange(	VUINT& nMinPosition,
							VUINT& nMaxPosition) const
		{ VWINDOW_SM2(SBM_GETRANGE, &nMinPosition, &nMaxPosition); }

	/** Set the minimum and maximum position values for the control.
	nMinPosition specifies the minimum scrolling position. nMaxPosition
	specifies the maximum scrolling position. The default minimum and
	maximum position values are zero. The difference between the values
	specified by the nMinPosition and nMaxPosition parameters must not be
	greater than the value of MAXLONG. If the minimum and maximum position
	values are equal, the scroll bar control is hidden and, in effect,
	disabled. If the position of the scroll box changed, the return value
	is the previous position of the scroll box; otherwise, it is zero.*/
	VLONG		SetRange(	VUINT nMinPosition,
							VUINT nMaxPosition) const
		{ return VWINDOW_SM2(SBM_SETRANGE, nMinPosition, nMaxPosition); }

#if(WINVER >= 0x0400)
	/** Retrieve the parameters of a scroll bar. nMask specifies what
	scrollbar information you wish to retrieve. It can be a combination of
	the following values:

	SIF_ALL - Combination of SIF_PAGE, SIF_POS, SIF_RANGE, and SIF_TRACKPOS.

	SIF_PAGE - Copies the scroll page to the nPage member of the SCROLLINFO
	structure.

	SIF_POS - Copies the scroll position to the nPos member of the
	SCROLLINFO structure.

	SIF_RANGE - Copies the scroll range to the nMin and nMax members of
	the SCROLLINFO structure.

	SIF_TRACKPOS - Copies the current scroll box tracking position to the
	nTrackPos member of the SCROLLINFO structure.

	The messages that indicate scroll-bar position, WM_HSCROLL and
	WM_VSCROLL, provide only 16 bits of position data. However, the
	SCROLLINFO structure used by SBM_GETSCROLLINFO, SBM_SETSCROLLINFO,
	GetScrollInfo(), and SetScrollInfo() provides 32 bits of scroll-bar
	position data. You can use these messages and functions while
	processing either the WM_HSCROLL or WM_VSCROLL messages to obtain
	32-bit scroll-bar position data. To get the 32-bit position of the
	scroll box (thumb) during a SB_THUMBTRACK notification in a WM_HSCROLL
	or WM_VSCROLL message, call GetScrollInfo() with the SIF_TRACKPOS
	value for the nMask value. The function returns the tracking position
	of the scroll box in the nTrackPos member of the SCROLLINFO structure.
	This allows you to get the position of the scroll box as the user
	moves it. If the function retrieves any values, the return value is
	VTRUE; otherwise, it is VFALSE. It is not necessary for calling code
	to initialize the SCROLLINFO structure, that is done by this function.*/
	VBOOL		GetScrollInfo(	SCROLLINFO&	si,
								VUINT		nMask = SIF_ALL) const
	{
		VZEROSTRUCT(si);
		si.cbSize =	sizeof(SCROLLINFO);
		si.fMask =	nMask;

		return VWINDOW_SMB2(SBM_GETSCROLLINFO, 0, &si);
	}

	/** Set the parameters of a scroll bar. See GetScrollInfo() for more
	information. bRedraw specifies whether the scroll bar is redrawn to
	reflect the new scroll box position. If this parameter is VTRUE, the
	scroll bar is redrawn. If it is VFALSE, the scroll bar is not redrawn.
	The return value is the current position of the scroll box. It is not
	necessary for calling code to initialize the SCROLLINFO structure, that
	is done by this function.*/
	VLONG		SetScrollInfo(	SCROLLINFO&	si,
								VUINT		nMask = SIF_ALL,
								VBOOL		bRedraw = VTRUE) const
	{
		VZEROSTRUCT(si);
		si.cbSize =	sizeof(SCROLLINFO);
		si.fMask =	nMask;

		return VWINDOW_SM2(SBM_SETSCROLLINFO, bRedraw, &si);
	}
#endif

	/** Set the minimum and maximum position values and redraw the control.
	nMinPosition specifies the minimum scrolling position. nMaxPosition
	specifies the maximum scrolling position. The default minimum and
	maximum position values are zero. The difference between the values
	specified by the nMinPosition and nMaxPosition parameters must not be
	greater than the value of MAXLONG. If the minimum and maximum position
	values are equal, the scroll bar control is hidden and, in effect,
	disabled. If the position of the scroll box changed, the return value
	is the previous position of the scroll box; otherwise, it is zero.*/
	VLONG		SetRangeRedraw(	VUINT nMinPosition,
								VUINT nMaxPosition) const
	{
		return VWINDOW_SM2(SBM_SETRANGEREDRAW, nMinPosition, nMaxPosition);
	}
};

#endif /* VSCROLLBAR*/
