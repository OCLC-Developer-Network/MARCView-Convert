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

#ifndef VTAB
#define VTAB

#include "../vstandard.h"
#include "../numbers/vbitmask.hpp"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"
#include "vcommoncontrol.hpp"

#define VWCL_MAX_TAB_STRING_LENGTH	1024

/* Make sure new common control constants are defined.*/
#ifndef TCS_BOTTOM
	#define TCS_BOTTOM		0x0002
#endif

#ifndef TCS_RIGHT
	#define TCS_RIGHT		0x0002
#endif

#ifndef TCS_VERTICAL
	#define TCS_VERTICAL	0x0080
#endif

/** VTab is a simple wrapping of the Win32 tab common control with minor
enhancements. Unless otherwise stated, all functions that accept a tab
index as a parameter can also handle -1, which means the currently selected
tab index.*/
class VTab : public VCommonControl
{
public:
	/** Default constructor initializes class and base class.*/
	VTab(	VUINT		nOptions = OPTION_SHOW_IMAGE_FOR_ACTIVE_TAB,
			VRTTI_TYPE	nRTTI = VWCL_RTTI_TAB)
		: VCommonControl(nRTTI)
		{ m_Options = nOptions; }

	/** Tab Control Options. These are initialized in the constructor or
	by calling GetOptions().*/
	enum	{		OPTION_SHOW_IMAGE_FOR_ACTIVE_TAB =	0x0001,
			};

	/** If the OPTION_SHOW_IMAGE_FROM_ACTIVE_TAB option bit is set, update
	the image for the old and current selection. What this does is that it
	assumes a non-selected tab will not display an image, while a selected
	tab will. So, it removes the image for all non-selected tabs, and sets
	an image for the selected one. By default, it assumes the same image
	will be shown for all tabs when selected, like a thumbtack image for
	instance. This can be configured by passing a non-0 value for
	nActiveImageIndex, which is an index into a imagelist to show for the
	selected tab.*/
	void			ActiveImageUpdate(	VINT nOldSel,
										VINT nCurSel,
										VINT nActiveImageIndex = 0) const
	{
		/* Should we toggle active tab image?*/
		HIMAGELIST hImageList =
			m_Options.IsSet(OPTION_SHOW_IMAGE_FOR_ACTIVE_TAB)
				? ImageList() : NULL;

		if ( hImageList )
		{
			TC_ITEM item;
			item.mask =		TCIF_IMAGE;
			item.iImage =	-1;

			/* Toggle image display for active and old selection.*/
			if ( nOldSel != -1 )
				SetItem(item, nOldSel);

			if ( nCurSel != -1 )
			{
				item.iImage = nActiveImageIndex;
				SetItem(item, nCurSel);
			}
		}
	}

	/** Calculates a tab control's display area given a window rectangle, or
	calculates the window rectangle that would correspond to a specified
	display area. If bLarger is VTRUE, r specifies a display rectangle and
	receives the corresponding window rectangle. If this parameter is
	VFALSE, r specifies a window rectangle and receives the corresponding
	display area.*/
	void			AdjustRect(	VRect&	r,
								VBOOL	bLarger = VTRUE) const
		{ VWINDOW_SM2(TCM_ADJUSTRECT, bLarger, r.GetPointer()); }

	/** Create the tab control. Returns VTRUE on success, VFALSE on error.
	nStyle can be any of the standard WS_xxx style bits described in
	VWindow::Create(), plus tab specific ones including:

	TCS_BUTTONS - Modifies the appearance of the tabs to look like buttons.

	TCS_FIXEDWIDTH - Makes all tabs the same width. (By default, the tab
	control automatically sizes each tab to fit its icon.) You cannot use
	this style with the TCS_RIGHTJUSTIFY style.

	TCS_FOCUSNEVER - Specifies that a tab never receives the input focus.

	TCS_FOCUSONBUTTONDOWN - Specifies that a tab receives the input focus
	when clicked. This style is typically used only with the TCS_BUTTONS
	style.

	TCS_FORCEICONLEFT - Forces the icon to the left, but leaves the tab
	label centered. (By default, the control centers the icon and label
	with the icon to the left of the label.)

	TCS_FORCELABELLEFT - Left-aligns both the icon and label.

	TCS_MULTILINE - Causes a tab control to display multiple rows of tabs.
	Thus all tabs can be displayed at once. (By default, a tab control
	displays a single row of tabs.)

	TCS_OWNERDRAWFIXED - Specifies that the parent window draws the tabs in
	the control.

	TCS_RIGHTJUSTIFY - Right justifies tabs. (By default, tabs are
	left-justified within each row.)

	TCS_SHAREIMAGELISTS - Specifies that a tab control’s image lists are
	not destroyed when the control is destroyed.

	TCS_TOOLTIPS - Specifies that the tab control has a tool tip control
	associated with it.

	TCS_TABS - Tabs appear as tabs, and a border is drawn around the
	display area. This style is the default.

	TCS_SINGLELINE - Displays only one row of tabs. The user can scroll to
	see more tabs, if necessary. This style is the default.

	TCS_RAGGEDRIGHT - Does not stretch each row of tabs to fill the
	entire width of the control. This style is the default.*/
	VBOOL			Create(	VWindow const&	windowParent,
							VRect const&	r,
							VUINT			nID,
							VUINT			nStyle =
											WS_TABSTOP |
											WS_VISIBLE |
											TCS_MULTILINE |
											TCS_TABS)
	{
		/* Correct style bits which conflict with each other.*/
		if ( nStyle & TCS_RIGHT || nStyle & TCS_VERTICAL )
		{
			nStyle |= TCS_MULTILINE;
			nStyle &= ~TCS_SINGLELINE;
		}

		return VCommonControl::Create(	windowParent,
										r,
										WS_CHILD | nStyle,
										WC_TABCONTROL,
										NULL,
										nID);
	}

	/** Call to destroy the image list held in the control.*/
	void			DestroyImageList() const
	{
		HIMAGELIST hImageList = ImageList();

		if ( hImageList )
		{
			ImageList((HIMAGELIST)NULL);
			ImageList_Destroy(hImageList);
		}
	}

	/** Delete the tab at nIndex. Returns VTRUE on success, VFALSE on
	failure. If successful, no tab will be selected.*/
	VBOOL			DeleteItem(VINT nIndex = -1) const
	{
		VASSERT(nIndex >= -1 && nIndex < GetItemCount())
		return VWINDOW_SMB1(TCM_DELETEITEM, ItemOrSelection(nIndex));
	}

	/** Delete all tabs from the control. Returns VTRUE on success, VFALSE
	on failure.*/
	VBOOL			DeleteAllItems() const
		{ return VWINDOW_SMB0(TCM_DELETEALLITEMS); }

	/** Returns the index of the item that has the focus in a tab control.
	The item that has the focus may be different than the selected item.*/
	VINT			Focus() const
		{ return (VINT)VWINDOW_SM0(TCM_GETCURFOCUS); }

	/** Sets the focus to a specified tab in a tab control. nIndex is the
	index of the tab that gets the focus. If the tab control has the
	TCS_BUTTONS style (button mode), the tab with the focus may be different
	from the selected tab. For example, when a tab is selected, the user
	can press the arrow keys to set the focus to a different tab without
	changing the selected tab. In button mode, TCM_SETCURFOCUS sets the
	input focus to the button associated with the specified tab, but it
	does not change the selected tab. If the tab control does not have the
	TCS_BUTTONS style, changing the focus also changes the selected tab. In
	this case, the tab control sends the TCN_SELCHANGING and TCN_SELCHANGE
	notification messages to its parent window.*/
	void			Focus(VINT nIndex) const
		{ VWINDOW_SM1(TCM_SETCURFOCUS, ItemOrSelection(nIndex)); }

	/** Retrieves information about a tab in a tab control. Returns VTRUE if
	successful, or VFALSE otherwise. nIndex is the index of the tab. item is
	a reference to a TC_ITEM structure that specifies the information to
	retrieve on input and receives information about the tab on output. When
	the function is called, the mask member specifies which attributes to
	return. If the mask member specifies the TCIF_TEXT value, the pszText
	member must contain the address of the buffer that receives the item
	text, and the cchTextMax member must specify the size of the buffer.*/
	VBOOL			GetItem(	TC_ITEM&	item,
								VINT		nIndex = -1) const
	{
		VASSERT(nIndex >= -1 && nIndex < GetItemCount())
		return VWINDOW_SMB2(TCM_GETITEM, ItemOrSelection(nIndex), &item);
	}

	/** Return the number of tabs in the control.*/
	VINT			GetItemCount() const
		{ return (VINT)VWINDOW_SM0(TCM_GETITEMCOUNT); }

	/** Retrieves the bounding rectangle for a tab in a tab control. Returns
	VTRUE if successful, or VFALSE otherwise. nIndex is the index of the
	tab. VRect is a rectangle that receives the bounding rectangle of the
	tab, in viewport coordinates.*/
	VBOOL			GetItemRect(	VRect&	r,
									VINT	nIndex = -1) const
	{
		VASSERT(nIndex >= -1 && nIndex < GetItemCount())

		return VWINDOW_SMB2(TCM_GETITEMRECT, ItemOrSelection(nIndex), r.GetPointer());
	}

	/** Returns a pointer to the string contained within the strBuffer
	parameter on success, NULL on failure. Normally, the entire string will
	be returned, but if only part of it is desired, you can pass a non-zero
	value for nMaxChars. nMaxChars excludes the NULL terminator, so if you
	want 2 bytes returned, pass 2 for this variable. strBuffer will not be
	modified on failure. nIndex specifies that tab to get the text for.*/
	VSTRING			GetItemText(	VString&	strBuffer,
									VINT		nIndex = -1,
									VUINT		nMaxChars = 0) const
	{
		VTCHAR				sz[VWCL_MAX_TAB_STRING_LENGTH];
		TC_ITEM				item;
		item.mask =			TCIF_TEXT;
		item.pszText =		sz;
		item.cchTextMax =	(nMaxChars) ? nMaxChars + 1 : VARRAY_SIZE(sz);

		return (GetItem(item)) ? strBuffer.String(sz, 0, nMaxChars) : NULL;
	}

	/** Return a reference to the options that apply to this class.*/
	VBitmask&		GetOptions() const
		{ return (VBitmask&)m_Options; }

	/** Retrieves the current number of rows of tabs in a tab control. Only
	tab controls that have the TCS_MULTILINE style can have multiple rows
	of tabs.*/
	VINT			GetRowCount() const
		{ return (VINT)VWINDOW_SM0(TCM_GETROWCOUNT); }

	/** Determines the location of the specified point at nx, ny relative to
	the client area of a tab control. When the function returns, the
	pnHitTest (if given) value is a hit test value that indicates
	the location of the specified point. This value can be one OR more of
	the following:

	TCHT_NOWHERE - The position is not over a tab.

	TCHT_ONITEM - The position is over a tab but not over its icon or
	its text. For owner-drawn tab controls, this value is specified if the
	position is anywhere over a tab.

	TCHT_ONITEMICON - The position is over a tab's icon.

	TCHT_ONITEMLABEL - The position is over a tab's text.

	TCHT_ONITEM - is a bitwise-OR operation on TCHT_ONITEMICON and
	TCHT_ONITEMLABEL.

	The functions returns the index of the tab if hit test is found on, on
	success, or -1 if the location does not specifiy a tabs location.*/
	VINT			HitTest(	VLONG	nx,
								VLONG	ny,
								VUINT*	pnHitTest = NULL) const
	{
		if ( pnHitTest )
			*pnHitTest = 0;

		TC_HITTESTINFO	hti;
		hti.pt.x =		nx;
		hti.pt.y =		ny;

		VINT nIndex = (VINT)VWINDOW_SM2(TCM_HITTEST, 0, &hti);

		if ( nIndex != -1 && pnHitTest )
			*pnHitTest = hti.flags;

		return nIndex;
	}

	/** Returns the image list associated with a tab control, or NULL if
	there is not one.*/
	HIMAGELIST		ImageList() const
		{ return (HIMAGELIST)VWINDOW_SM0(TCM_GETIMAGELIST); }

	/** Set the image list associated with a tab control. Returns a handle
	to the previous imagelist, or NULL if there is not one.*/
	HIMAGELIST		ImageList(HIMAGELIST hImageList) const
		{ return (HIMAGELIST)VWINDOW_SM2(TCM_SETIMAGELIST, 0, hImageList); }

	/** Initialize image list. Default params assume a masking color the
	same as used for icon transparency. If hResouce is not given,
	VGetResourceHandle() will be used.*/
	VBOOL			InitImageList(	VUINT		nBitmapID,
									VINT		nImageWidth = 16,
									VINT		nGrow = 0,
									COLORREF	crMask = RGB(0, 128, 128),
									HINSTANCE	hResource = NULL) const
	{
		HIMAGELIST hImageList =
			ImageList_LoadBitmap(	(hResource)
									? hResource
									: VGetResourceHandle(),
									MAKEINTRESOURCE(nBitmapID),
									nImageWidth,
									nGrow,
									crMask);

		if ( hImageList )
		{
			ImageList(hImageList);
			return VTRUE;
		}

		VASSERT(VFALSE)

		return VFALSE;
	}

	/** Insert a new tab when only the tab name is known. Returns new tab
	index on success, -1 on failure. If nIndex is -1, the tab will be added
	to the end of the tab list, otherwise it will be inserted at nIndex.*/
	VINT			InsertItem(	VSTRING_CONST	pszText,
								VINT			nIndex = -1) const
	{
		TC_ITEM			item;
		item.mask =		TCIF_TEXT | TCIF_PARAM;
		item.pszText =	(VSTRING)pszText;
		item.lParam =	0;

		return InsertItem(item, nIndex);
	}

	/** Inserts a new tab in a tab control. Returns the index of the new tab
	if successful, or -1 otherwise. nIndex is the index of the new tab.
	item is a TC_ITEM structure that specifies the attributes of the tab.
	This structure must be pre-initialized before calling this function.
	The dwState and dwStateMask members of this structure are ignored by
	this function. If nIndex is -1, the tab will be added to the end of
	the tab list, otherwise it will be inserted at nIndex.*/
	VINT			InsertItem(	TC_ITEM const&	item,
								VINT			nIndex = -1) const
	{
		return (VINT)VWINDOW_SM2(TCM_INSERTITEM, ((nIndex == -1) ? GetItemCount() : nIndex), &item);
	}

	/** Returns either nIndex or, if -1, the selected tab.*/
	VINT			ItemOrSelection(VINT nIndex) const
		{ return (nIndex != -1) ? nIndex : Selection(); }

	/** Get the VLPARAM (user data) stored in the specified tab item.
	Returns -1 on failure or value on success.*/
	VLPARAM			LParam(VINT nIndex) const
	{
		VASSERT(nIndex >= -1 && nIndex < GetItemCount())

		TC_ITEM		item;
		item.mask = TCIF_PARAM;

		return (GetItem(item, ItemOrSelection(nIndex))) ? item.lParam : -1;
	}

	/** Set the VLPARAM (user data) stored in the specified tab item.
	Returns -1 on failure or value on success.*/
	VLPARAM			LParam(VINT nIndex, VLPARAM nParam) const
	{
		VASSERT(nIndex >= -1 && nIndex < GetItemCount())

		TC_ITEM			item;
		item.mask =		TCIF_PARAM;
		item.lParam =	nParam;

		return (SetItem(item, ItemOrSelection(nIndex))) ? nParam : -1;
	}

	/** Removes an image from a tab control's image list. nIndex is the
	index of the image to remove. The tab control updates each tab's image
	index, so each tab remains associated with the same image as before. If
	a tab is using the image being removed, the tab will be set to have no
	image.*/
	void			RemoveImage(VINT nIndex = -1) const
		{ VWINDOW_SM1(TCM_REMOVEIMAGE, ItemOrSelection(nIndex)); }

	/** Returns the currently selected tab in a tab control, or -1 if no
	tab is selected.*/
	VINT			Selection() const
		{ return (VINT)VWINDOW_SM0(TCM_GETCURSEL); }

	/** Set the current selection, and check for active image update.
	Removes image for soon to be inactive tab and sets image for soon to be
	active tab  It then selects a tab in a tab control. Returns the index
	of the previously selected tab if successful, or -1 otherwise. A tab
	control does not send a TCN_SELCHANGING or TCN_SELCHANGE notification
	message when a tab is selected using this function.*/
	VINT			Selection(VINT nIndex) const
	{
		ActiveImageUpdate(Selection(), nIndex);
		return (VINT)VWINDOW_SM1(TCM_SETCURSEL, nIndex);
	}

	/** Sets some or all of a tab's attributes. Returns VTRUE if successful,
	or VFALSE otherwise. nIndex specifies the index of the item to set
	information for. item is a TC_ITEM structure that contains the new
	item attributes. The mask member specifies which attributes to set. If
	the mask member specifies the LVIF_TEXT value, the pszText member is the
	address of a null-terminated string and the cchTextMax member is
	ignored.*/
	VBOOL			SetItem(	TC_ITEM&	item,
								VINT		nIndex = -1) const
	{
		VASSERT(nIndex >= -1 && nIndex < GetItemCount())
		return VWINDOW_SMB2(TCM_SETITEM, ItemOrSelection(nIndex), &item);
	}

	/** Sets the number of bytes per tab reserved for application-defined
	data in a tab control. Returns VTRUE if successful, or VFALSE otherwise.
	nCountOfBytes is the number of extra bytes. By default, the number of
	extra bytes is four. An application that changes the number of extra
	bytes cannot use the TC_ITEM structure to retrieve and set the
	application-defined data for a tab. Instead, you must define a new
	structure that consists of the TCITEMHEADER structure followed by
	application-defined members. An application should only change the
	number of extra bytes when a tab control does not contain any tabs.*/
	VBOOL			SetItemExtra(VINT nCountOfBytes) const
	{
		VASSERT(!GetItemCount())
		return VWINDOW_SMB1(TCM_SETITEMEXTRA, nCountOfBytes);
	}

	/** Sets the width and height of tabs in a fixed-width or owner-drawn
	tab control. Returns the old width and height. The width is in the
	low-order word of the return value, and the height is in the
	high-order word. nWidth and nHeight specify the new width and height,
	in pixels.*/
	VDWORD			SetItemSize(	VINT nWidth,
									VINT nHeight) const
		{ return (VDWORD)VWINDOW_SM3(TCM_SETITEMSIZE, 0, nWidth, nHeight); }

	/** Set the text of the tab at index nIndex to the string in pszText.*/
	VBOOL			SetItemText(	VSTRING_CONST	pszText,
									VINT			nIndex = -1) const
	{
		TC_ITEM			item;
		item.mask =		TCIF_TEXT;
		item.pszText =	(VSTRING)pszText;

		return SetItem(item, ItemOrSelection(nIndex));
	}

	/** Sets the amount of space (padding) around each tab's icon and label
	in a tab control. nWidth and nHeight specify the horizontal and vertical
	padding, in pixels.*/
	void			SetPadding(	VINT nWidth,
								VINT nHeight) const
		{ VWINDOW_SM3(TCM_SETPADDING, 0, nWidth, nHeight); }

	/** Retrieves the handle to the tooltip control associated with a tab
	control. Returns the handle to the tooltip control if successful, or
	NULL otherwise. A tab control creates a tooltip control if it has the
	TCS_TOOLTIPS style. You can also assign a tooltip control to a tab
	control by using the ToolTips() setter function.*/
	HWND			ToolTips() const
		{ return (HWND)VWINDOW_SM0(TCM_GETTOOLTIPS); }

	/** Associates a tooltip control with the tabs.*/
	void			ToolTips(VWindow const& wndToolTips) const
		{ VWINDOW_SM1(TCM_SETTOOLTIPS, wndToolTips.GetHandle()); }

protected:
	/** Calls ActiveImageUpdate() to update images when selections change.*/
	virtual VLONG	OnReflectedNotify(	NMHDR&		nmhdr,
										VLPARAM&	lCommonControlResult)
	{
		if ( nmhdr.code == TCN_SELCHANGE )
			ActiveImageUpdate(-1, Selection());
		else if ( nmhdr.code == TCN_SELCHANGING )
			ActiveImageUpdate(Selection(), -1);

		return VCommonControl::OnReflectedNotify(nmhdr, lCommonControlResult);
	}

	/** Embedded Members.*/
	VBitmask		m_Options;
};

#endif /* VTAB*/
