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

#ifndef VLISTVIEW
#define VLISTVIEW

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vpoint.hpp"
#include "../structures/vrect.hpp"
#include "../strings/vstring.hpp"
#include "../windows/vwindow.hpp"
#include "vcommoncontrol.hpp"

#define VWCL_MAX_SINGLE_LISTVIEW_ITEM_LENGTH	1024
#define VWCL_LISTVIEW_COLUMN_PADDING_PIXELS		15

/** Macro that declares a LV_ITEM struct named item and zero-fills it.*/
#define VDECLARE_LV_ITEM	\
	LV_ITEM item;			\
	VZEROSTRUCT(item);

/** Macro that declares a LV_COLUMN struct named column and zero-fills it.*/
#define VDECLARE_LV_COLUMN	\
	LV_COLUMN column;		\
	VZEROSTRUCT(column);

/** VListView provides extended wrapping of the Win32 listview common control
with many enhancements.*/
class VListView : public VCommonControl
{
public:
	/** Default constructor initializes RTTI information.*/
	VListView(VRTTI_TYPE nRTTI = VWCL_RTTI_LISTVIEW)
		: VCommonControl(nRTTI)
		{;}

	/** Arranges items in icon view. Returns VTRUE if successful, or VFALSE
	otherwise. nCode specifies the alignment. This can be one of the
	following values:

	LVA_ALIGNLEFT - Aligns items along the left edge of the window.

	LVA_ALIGNTOP - Aligns items along the top edge of the window.

	LVA_DEFAULT - Aligns items according to the listview control's current
	alignment styles (the default value).

	LVA_SNAPTOGRID - Snaps all icons to the nearest grid position.*/
	VBOOL			Arrange(VUINT nCode = LVA_DEFAULT) const
		{ return VWINDOW_SMB1(LVM_ARRANGE, nCode); }

	/** Return the current background color.*/
	COLORREF		BackColor() const
		{ return (COLORREF)VWINDOW_SM0(LVM_GETBKCOLOR); }

	/** Set the current background color. Returns VTRUE on success, VFALSE
	on failure. crBackColor is the background color to set or the CLR_NONE
	value for no background color. listview controls with background colors
	redraw themselves significantly faster than those without background
	colors.*/
	VBOOL			BackColor(COLORREF crBackColor) const
		{ return VWINDOW_SMB2(LVM_SETBKCOLOR, 0, crBackColor); }

	/** Make columns wide enough to display all text in header and items.
	This function is sufficient for most best fit operations. See
	BestFixColumnsEx() for a more involved version. If nEndColumn is
	-1, the column count will be retreived.*/
	void			BestFitColumns(	VINT nStartColumn = 0,
									VINT nEndColumn = -1) const
	{
		/* If nEndColumn is -1, get the column count.*/
		if ( nEndColumn == -1 )
			nEndColumn = GetColumnCount() - 1;

		/* Get count of items in listview. If there are no items, we will
		use the column width.*/
		VUINT nCount = GetItemCount();

		for ( VINT i = nStartColumn; i <= nEndColumn; i++ )
			ColumnWidth(	i,
							(nCount)
							? LVSCW_AUTOSIZE
							: LVSCW_AUTOSIZE_USEHEADER);
	}

	/** Make columns wide enough to display all text in header and items. A
	static VWCL_MAX_SINGLE_LISTVIEW_ITEM_LENGTH buffer will be used if
	pszBuffer is NULL. This function differs from the above by checking the
	length of each item in the list and comparing it to the length of the
	header text. This ensures that the header is never truncated to a size
	smaller than what is needed to display the header text. The above
	function will shrink the header size to a size smaller than what is
	actually needed if items in the list are shorter than the header length.
	This function is much slower than the above version, but will make sure
	all text is visible. The slowness is because we have to get and then
	compare the text length width to make a determination about how to size
	the column. If Microsoft ever allows us a message to get the width of
	the largest text item in a column, this function will be just as fast as
	the above version. Microsoft stores the length of the largest string
	internally, but does not expose a method to get it, so we have to test
	each item for the longest string. if nEndColumn is -1, the column count
	will be retrieved.*/
	void			BestFitColumnsEx(	VINT	nStartColumn = 0,
										VINT	nEndColumn = -1,
										VSTRING	pszBuffer = NULL,
										VUINT	nBufferSize = 0) const
	{
		/* If the listview contains a lot of data, this will take a while.
		This routine is optimized for performance, and does not use other
		members functions which would create and destroy many structs,
		buffers, etc.*/

		/* Default VWCL_MAX_SINGLE_LISTVIEW_ITEM_LENGTH buffer.*/
		VTCHAR sz[VWCL_MAX_SINGLE_LISTVIEW_ITEM_LENGTH];

		/* Use default buffer?*/
		if ( !pszBuffer || !nBufferSize )
		{
			pszBuffer =		sz;
			nBufferSize =	VARRAY_SIZE(sz);
		}

		/* If nEndColumn is -1, get the column count.*/
		if ( nEndColumn == -1 )
			nEndColumn = GetColumnCount() - 1;

		/* Get count of items in listview.*/
		VUINT nCount =		GetItemCount();

		/* Initialize header handle.*/
		HWND hWndHeader =	GetHeader();

		/* Initialize structs.*/
		VDECLARE_LV_ITEM
		item.mask =			LVIF_TEXT;
		item.cchTextMax =	nBufferSize;
		item.pszText =		pszBuffer;

		/* Assume a header will not be more than
		VWCL_MAX_SINGLE_LISTVIEW_ITEM_LENGTH! If you are writing "War and
		Peace" in your header, you should review your GUI design.*/
		VTCHAR				szHeader[VWCL_MAX_SINGLE_LISTVIEW_ITEM_LENGTH];
		HD_ITEM				header;
		header.mask =		HDI_TEXT;
		header.pszText =	szHeader;
		header.cchTextMax =	VARRAY_SIZE(szHeader);

		for ( VINT i = nStartColumn; i <= nEndColumn; i++ )
		{
			item.iSubItem = i;

			/* Determine if the header is wider than the widest item
			in the list.*/
			VBOOL bHeaderIsWider = VTRUE;

			if (	nCount &&
					hWndHeader &&
					Header_GetItem(hWndHeader, i, &header) )
			{
				/* Get this headers text width.*/
				VINT nHeaderWidth = GetStringWidth(szHeader);

				/* Find first item in list that is wider than the header.*/
				for ( VUINT a = 0; a < nCount; a++ )
				{
					item.iItem = a;

					/* Get the text of this item in the listview and
					compare width.*/
					if (	GetItem(item) &&
							GetStringWidth(pszBuffer) > nHeaderWidth )
					{
						bHeaderIsWider = VFALSE;
						break;
					}
				}
			}
			else if ( !hWndHeader )
				bHeaderIsWider = VFALSE;

			ColumnWidth(	i,
							(bHeaderIsWider)
							? LVSCW_AUTOSIZE_USEHEADER
							: LVSCW_AUTOSIZE);
		}
	}

	/** Retrieves the callback mask for the listview control.*/
	VUINT			CallbackMask() const
		{ return (VUINT)VWINDOW_SM0(LVM_GETCALLBACKMASK); }

	/** Set the callback mask for the listview control. Returns VTRUE on
	success, VFALSE on failure.*/
	VBOOL			CallbackMask(VUINT nMask) const
		{ return VWINDOW_SMB1(LVM_SETCALLBACKMASK, nMask); }

	/** Get the checked state of a list view with the LVS_EX_CHECKBOXES
	extended style sets. Returns VTRUE if checked, VFALSE if not checked.
	The result is not reliable if the LVS_EX_CHECKBOXES extended style is
	not used!*/
	VBOOL			Checked(VINT nIndex) const
	{
		return ((ItemState(nIndex, LVIS_STATEIMAGEMASK) >> 12) - 1)
			? VTRUE : VFALSE;
	}

	/** Set the checked state of a list view with the LVS_EX_CHECKBOXES
	extended style sets.*/
	void			Checked(VINT nIndex, VBOOL bChecked) const
	{
		ItemState(	nIndex,
					INDEXTOSTATEIMAGEMASK((bChecked) + 1),
					LVIS_STATEIMAGEMASK);
	}

	/** Retrieves the width of a column in report or listview.*/
	VINT			ColumnWidth(VINT nColumn) const
		{ return (VINT)VWINDOW_SM1(LVM_GETCOLUMNWIDTH, nColumn); }

	/** Sets the width (in view coordinates) of a list view column. nColumn
	is the column index to set the width for. nx can be the width to set
	the column to, or one of the following values:

	LVSCW_AUTOSIZE - Automatically sizes the column.

	LVSCW_AUTOSIZE_USEHEADER  Automatically sizes the column to fit the
	header text.

	Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			ColumnWidth(	VINT nColumn,
									VINT nx) const
		{ return VWINDOW_SMB2(LVM_SETCOLUMNWIDTH, nColumn, nx); }

	/** Create the listview control. Returns VTRUE on success, VFALSE on
	error. nStyle can be any of the standard WS_xxx style bits described in
	VWindow::Create(), plus list view specific ones including:

	LVS_ALIGNLEFT - Specifies that items are left-aligned in icon and
	small icon view.

	LVS_ALIGNTOP - Specifies that items are aligned with the top of the
	control in icon and small icon view.

	LVS_AUTOARRANGE - Specifies that icons are automatically kept arranged
	in icon view and small icon view.

	LVS_EDITLABELS - Allows item text to be edited in place. The parent
	window must process the LVN_ENDLABELEDIT notification message.

	LVS_ICON - Specifies icon view.

	LVS_LIST - Specifies list view.

	LVS_NOCOLUMNHEADER - Specifies that a column header is not displayed
	in report view. By default, columns have headers in report view.

	LVS_NOLABELWRAP - Displays item text on a single line in icon view. By
	default, item text can wrap in icon view.

	LVS_NOSCROLL - Disables scrolling. All items must be within the client
	area.

	LVS_NOSORTHEADER - Specifies that column headers do not work like
	buttons. This style is useful if clicking a column header in report
	view does not carry out an action, such as sorting.

	LVS_OWNERDRAWFIXED - Enables the owner window to paint items in report
	view. The list view control sends a WM_DRAWITEM message to paint each
	item; it does not send separate messages for each subitem. The itemData
	member of the DRAWITEMSTRUCT structure contains the item data for the
	specified list view item.

	LVS_REPORT - Specifies report view.

	LVS_SHAREIMAGELISTS - Specifies that the control does not take
	ownership of the image lists assigned to it (that is, it does not
	destroy the image lists when it is destroyed). This style enables
	the same image lists to be used with multiple list view controls.

	LVS_SHOWSELALWAYS - Always show the selection, if any, even if the
	control does not have the focus.

	LVS_SINGLESEL - Allows only one item at a time to be selected. By
	default, multiple items can be selected.

	LVS_SMALLICON - Specifies small icon view.

	LVS_SORTASCENDING - Sorts items based on item text in ascending order.

	LVS_SORTDESCENDING - Sorts items based on item text in descending order.*/
	VBOOL			Create(	VWindow const&	windowParent,
							VRect const&	r,
							VUINT			nID,
							VUINT			nStyle =
											WS_BORDER |
											WS_TABSTOP |
											WS_VISIBLE |
											LVS_REPORT |
											LVS_SINGLESEL |
											LVS_SHOWSELALWAYS)
	{
		return VCommonControl::Create(	windowParent,
										r, WS_CHILD | nStyle,
										WC_LISTVIEW,
										NULL,
										nID);
	}

	/** Creates a drag image list for the specified item. pt is a VPoint
	object that receives the initial location of the upper-left corner of
	the image, in view coordinates. Returns the handle to the drag image
	list if successful, or NULL otherwise.*/
	HIMAGELIST		CreateDragImage(	VINT	nIndex,
										VPoint&	pt) const
	{
		return (HIMAGELIST)VWINDOW_SM2(LVM_CREATEDRAGIMAGE, nIndex, pt.GetPointer());
	}

	/** Delete all items in the listview. Returns VTRUE on success, VFALSE
	on failure.*/
	VBOOL			DeleteAllItems() const
		{ return VWINDOW_SMB0(LVM_DELETEALLITEMS); }

	/** Delete the column specified by nColumn, which is a zero-based index.
	Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			DeleteColumn(VINT nColumn) const
		{ return VWINDOW_SMB1(LVM_DELETECOLUMN, nColumn); }

	/** Removes an item from a listview control. Returns VTRUE if successful,
	or VFALSE otherwise. nItem is the index of the listview item to delete.*/
	VBOOL			DeleteItem(VINT nIndex) const
		{ return VWINDOW_SMB1(LVM_DELETEITEM, nIndex); }

	/** Begins in-place editing of the specified listview item's text. The
	function implicitly selects and focuses the specified item. Returns the
	handle to the edit control that is used to edit the item text if
	successful, or NULL otherwise. nIndex is the zero-based of the listview
	item. To cancel editing, set nItem to -1. When the user completes or
	cancels editing, the edit control is destroyed and the handle is no
	longer valid. You can safely subclass the edit control, but you should
	not destroy it. This function verifies the focus is on this control
	before sending the message, as reqiured by the control.*/
	HWND			EditLabel(VINT nIndex) const
	{
		SetFocus();
		return (HWND)VWINDOW_SM1(LVM_EDITLABEL, nIndex);
	}

	/** Stop inplace label editing.*/
	void			EndEditLabel()
		{ EditLabel(-1); }

	/** Ensures that a listview item is either entirely or partially visible,
	scrolling the listview control if necessary. if bPartialOK VTRUE, no
	scrolling occurs if the item is at least partially visible. Returns VTRUE
	on success, VFALSE on failure.*/
	VBOOL			EnsureVisible(	VINT	nIndex,
									VBOOL	bPartialOK = VFALSE) const
		{ return VWINDOW_SMB3(LVM_ENSUREVISIBLE, nIndex, bPartialOK, 0); }

	/** Searches for a listview item with the specified characteristics.
	nStartIndex is the index of the item to begin the search with or -1 to
	start from the beginning. The specified item is itself excluded from the
	search. findInfo is a LV_FINDINFO structure that contains information
	about what to search for. It must be pre-initialized.*/
	VINT			FindItem(	LV_FINDINFO&	findInfo,
								VINT			nStartIndex = -1) const
		{ return (VINT)VWINDOW_SM2(LVM_FINDITEM, nStartIndex, &findInfo); }

	/** Retrieves the attributes of the listview control's column. column is
	a structure that specifies the information to retrieve and receives
	information about the column. The mask member specifies which column
	attributes to retrieve. If the mask member specifies the LVCF_TEXT value,
	the pszText member must contain the address of the buffer that receives
	the item text and the cchTextMax member must specify the size of the
	buffer. This structure must be initialized to the information you wish
	to return before calling this function. On output, it will have the
	requested information.*/
	VBOOL			GetColumn(	LV_COLUMN&	column,
								VINT		nColumn) const
	{
		#ifdef VWCL_DEBUG
			if ( column.mask & LVCF_TEXT )
				VASSERT(column.pszText && column.cchTextMax)
		#endif

		return VWINDOW_SMB2(LVM_GETCOLUMN, nColumn, &column);
	}

	/** Return the number of columns in the listview.*/
	VINT			GetColumnCount() const
	{
		VDECLARE_LV_COLUMN
		column.mask = LVCF_SUBITEM;

		VINT nIndex = 0;

		while ( VTRUE )
		{
			column.iSubItem = nIndex;

			if ( !GetColumn(column, nIndex) )
				return nIndex;

			nIndex++;
		}

		return 0;
	}

	/** Returns a pointer to the string contained within the strBuffer
	parameter on success, NULL on failure. Normally, the entire string will
	be returned, but if only part of it is desired, you can pass a non-zero
	value for nMaxChars. nMaxChars excludes the NULL terminator, so if you
	want 2 bytes returned, pass 2 for this variable. strBuffer will not be
	modified on failure.*/
	virtual VSTRING	GetColumnText(	VString&	strBuffer,
									VINT		nColumn,
									VUINT		nMaxChars = 0) const
	{
		VTCHAR sz[VWCL_MAX_SINGLE_LISTVIEW_ITEM_LENGTH];

		VDECLARE_LV_COLUMN
		column.pszText=		sz;
		column.cchTextMax =	(nMaxChars) ? nMaxChars + 1 : VARRAY_SIZE(sz);
		column.mask =		LVCF_TEXT;

		if ( GetColumn(column, nColumn) )
			return strBuffer.String(sz, 0, nMaxChars);

		return NULL;
	}

	/** Calculates the number of items that can fit vertically in the
	visible area of a listview control when in list or report view. Only
	fully visible items are counted.*/
	VINT			GetCountPerPage() const
		{ return (VINT)VWINDOW_SM0(LVM_GETCOUNTPERPAGE); }

	/** Retrieves the handle to the edit control being used to edit a
	listview item's text. If no label is being edited, the return value is
	NULL.*/
	HWND			GetEditControl() const
		{ return (HWND)VWINDOW_SM0(LVM_GETEDITCONTROL); }

	/** Get the first selected item index. Returns -1 if nothing is selected.*/
	VINT			GetFirstSelectedItem() const
	{
		/* Skip if nothing is selected.*/
		if ( GetSelectedCount() )
		{
			VINT nCount = GetItemCount();

			for ( VINT i = 0; i < nCount; i++ )
			{
				if ( IsSelected(i) )
					return i;
			}
		}

		return -1;
	}

	/** Retrieves the incremental search string of a listview control. Returns
	the number of characters in the incremental search string, or zero if
	the listview control is not in incremental search mode. pszString is the
	address of a buffer that receives the incremental search string. The
	incremental search string is the character sequence that the user types
	while the listview has the input focus. Each time the user types a
	character, the system appends the character to the search string and then
	searches for a matching item. If the system finds a match, it selects
	the item and, if necessary, scrolls it into view. Returns the number of
	characters in the incremental search string, or zero if the listview
	control is not in incremental search mode. A time-out period is
	associated with each character that the user types. If the time-out
	period elapses before the user types another character, the incremental
	search string is reset.*/
	VLONG			GetISearchString(VSTRING pszString) const
	{
		VASSERT(pszString)
		return VWINDOW_SM2(LVM_GETISEARCHSTRING, 0, pszString);
	}

	/** Retrieves some or all of a listview item's attributes. Returns VTRUE
	on success, VFALSE on failure. item is a  LV_ITEM structure that
	specifies the information to retrieve and receives information about the
	list view item. When the function is called, the iItem and iSubItem
	members identify the item or subitem to retrieve information about, and
	the mask member specifies which attributes to retrieve. For a list of
	possible values, see the description of the LV_ITEM structure. If the
	mask member specifies the LVIF_TEXT value, the pszText member must
	contain the address of the buffer that receives the item text, and the
	cchTextMax member must specify the size of the buffer. If the mask
	member specifies the LVIF_STATE value, the stateMask member must specify
	the item state bits to retrieve. On output, the state member contains the
	values of the specified state bits.*/
	VBOOL			GetItem(LV_ITEM& item) const
	{
		#ifdef VWCL_DEBUG
			if ( item.mask & LVIF_TEXT )
				VASSERT(item.pszText && item.cchTextMax)
		#endif

		return VWINDOW_SMB2(LVM_GETITEM, 0, &item);
	}

	/** Return the total number of items in the listview.*/
	VINT			GetItemCount() const
		{ return (VINT)VWINDOW_SM0(LVM_GETITEMCOUNT); }

	/** Retrieves the position of a listview item. pt is a VPoint object that
	receives the position of the item's upper-left corner, in view
	coordinates. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			GetItemPosition(	VINT	nIndex,
										VPoint&	pt) const
		{ return VWINDOW_SMB2(LVM_GETITEMPOSITION, nIndex, pt.GetPointer()); }

	/** Retrieves the bounding rectangle for all or part of an item in the
	current view. Returns VTRUE on success, VFALSE on failure. On success,
	the rectangle will be copied into the rect object. nCode specifies the
	portion of the listview item for which to retrieve the bounding
	rectangle. This parameter can be one of the following values:

	LVIR_BOUNDS - Returns the bounding rectangle of the entire item,
	including the icon and label.

	LVIR_ICON - Returns the bounding rectangle of the icon or small icon.

	LVIR_LABEL - Returns the bounding rectangle of the item text.

	LVIR_SELECTBOUNDS - Returns the union of the LVIR_ICON and LVIR_LABEL
	rectangles, but excludes columns in details view.*/
	VBOOL			GetItemRect(	VINT	nIndex,
									VRect&	r,
									VLONG	nCode = LVIR_BOUNDS) const
	{
		r.GetLeft() = nCode;
		return VWINDOW_SMB2(LVM_GETITEMRECT, nIndex, r.GetPointer());
	}

	/** Determines the spacing between items in a listview control. Returns
	the amount of spacing between items. The horizontal spacing is contained
	in the low-order WORD and the vertical spacing is contained in the
	high-order WORD.  bSmallIconView is VTRUE for small icon view, or
	VFALSE for icon view.*/
	VDWORD			GetItemSpacing(VBOOL bSmallIconView = VFALSE) const
		{ return (VDWORD)VWINDOW_SM1(LVM_GETITEMSPACING, bSmallIconView); }

	/** Returns a pointer to the string contained within the strBuffer
	parameter on success, NULL on failure. Normally, the entire string will
	be returned, but if only part of it is desired, you can pass a non-zero
	value for nMaxChars. nMaxChars excludes the NULL terminator, so if you
	want 2 bytes returned, pass 2 for this variable. strBuffer will not be
	modified on failure. nIndex is the index (top to bottom) of the item in
	the listview to get.*/
	virtual VSTRING	GetItemText(	VString&	strBuffer,
									VINT		nIndex,
									VINT		nSubItem = 0,
									VUINT		nMaxChars = 0) const
	{
		VTCHAR sz[VWCL_MAX_SINGLE_LISTVIEW_ITEM_LENGTH];

		VDECLARE_LV_ITEM
		item.mask =			LVIF_TEXT;
		item.iItem =		nIndex;
		item.iSubItem =		nSubItem;
		item.cchTextMax =	(nMaxChars) ? nMaxChars + 1 : VARRAY_SIZE(sz);
		item.pszText =		sz;

		return (GetItem(item)) ? strBuffer.String(sz, 0, nMaxChars) : NULL;
	}

	/** Get the HWND to the child header control, or NULL if there is not one.*/
	HWND			GetHeader() const
		{ return FindChildWindowByClass(WC_HEADER); }

	/** Searches for a listview item that has the specified properties and
	bears the specified relationship to the specified item at nStartIndex.
	Returns the index of the next item if successful, or -1 otherwise.
	nStartIndex is the index of the item to begin the search with, or -1 to
	find the first item that matches the specified flags. The specified
	item itself is excluded from the search. nFlags specifies the
	relationship to the item specified in nStartIndex. This can be one or a
	combination of the following values:

	LVNI_ALL - Searches for a subsequent item by index (the default value).
	The physical relationship to nStartIndex can be specified with one of
	the following values:

	LVNI_ABOVE - Searches for an item that is above the specified item.

	LVNI_BELOW - Searches for an item that is below the specified item.

	LVNI_TOLEFT - Searches for an item to the left of the specified item.

	LVNI_TORIGHT - Searches for an item to the right of the specified item.
	The state of the item to find can be specified with one or a
	combination of the following values:

	LVNI_CUT - The item has the LVIS_CUT state flag set.

	LVNI_DROPHILITED - The item has the LVIS_DROPHILITED state flag set.

	LVNI_FOCUSED - The item has the LVIS_FOCUSED state flag set.

	LVNI_SELECTED - The item has the LVIS_SELECTED state flag set.

	If an item does not have all of the specified state flags set, the
	search continues with the next item.*/
	VINT			GetNextItem(	VINT nFlags = LVNI_ALL,
									VINT nStartIndex = -1) const
		{ return (VINT)VWINDOW_SM2(LVM_GETNEXTITEM, nStartIndex, nFlags); }

	/** Retrieves the current view origin for a listview control. Returns
	VTRUE if successful, or VFALSE if the current view is list or report
	view. The point is placed in the pt VPoint object.*/
	VBOOL			GetOrigin(VPoint& pt) const
		{ return VWINDOW_SMB2(LVM_GETORIGIN, 0, pt.GetPointer()); }

	/** Return the number of items currently selected in the listview.*/
	VINT			GetSelectedCount() const
		{ return (VINT)VWINDOW_SM0(LVM_GETSELECTEDCOUNT); }

	/** Determines the width of a specified string using the listview
	control's current font. The GetStringWidth() function returns the exact
	width, in pixels, of the specified string. If you use the returned string
	width as the column width in the LVM_SETCOLUMNWIDTH message, the string
	will be truncated. To get the column width that can contain the string
	without truncating it, you must add padding to the returned string width.
	Microsoft won't say how much, so guess.*/
	VINT			GetStringWidth(VSTRING_CONST pszString) const
	{
		if ( pszString )
			return (VINT)VWINDOW_SM2(LVM_GETSTRINGWIDTH, 0, pszString);

		return 0;
	}

	/** Retrieves the index of the topmost visible item when in list or
	report view, or 0 if the list is in icon or small icon view.*/
	VINT			GetTopIndex() const
		{ return (VINT)VWINDOW_SM0(LVM_GETTOPINDEX); }

	/** Retrieves the bounding rectangle of all items in the listview control.
	The listview must be in icon or small icon view. The rectangle is
	placed into the VRect object specified by r. Returns VTRUE on success,
	VFALSE on failure.*/
	VBOOL			GetViewRect(VRect& r) const
		{ return VWINDOW_SMB2(LVM_GETVIEWRECT, 0, r.GetPointer()); }

	/** Determines the location of the specified point at nx, ny relative to
	the client area of a listview control. When the function returns, the
	pnHitTest (if given) value is a hit test value that indicates the
	location of the specified point. This value can be one OR more of the
	following:

	LVHT_ABOVE - The position is above the control's client area.

	LVHT_BELOW - The position is below the control's client area.

	LVHT_NOWHERE - The position is inside the listview control's client
	window, but it is not over a list item.

	LVHT_ONITEMICON - The position is over a listview item's icon.

	LVHT_ONITEMLABEL - The position is over a listview item's text.

	LVHT_ONITEMSTATEICON - The position is over the state image of
	a listview item.

	LVHT_TOLEFT - The position is to the left of the listview control's
	client area.

	LVHT_TORIGHT - The position is to the right of the listview control's
	client area.

	Returns the index to the listview item that occupies the specified
	point, or -1 if no item occupies the point.*/
	VINT			HitTest(	VLONG	nx,
								VLONG	ny,
								VUINT*	pnHitTest = NULL) const
	{
		if ( pnHitTest )
			*pnHitTest = 0;

		LV_HITTESTINFO	hti;
		hti.pt.x =		nx;
		hti.pt.y =		ny;

		VINT nResult = (VINT)VWINDOW_SM2(LVM_HITTEST, 0, &hti);

		if ( pnHitTest )
			*pnHitTest = hti.flags;

		return nResult;
	}

	/** Returns the handle to an image list used for drawing listview items.
	nImageList specifies the imagelist to retrieve. This parameter can be
	one of the following values:

	LVSIL_NORMAL - Image list with large icons.

	LVSIL_SMALL - Image list with small icons.

	LVSIL_STATE- Image list with state images.

	Returns the handle to the specified image list if successful,
	or NULL otherwise.*/
	HIMAGELIST		ImageList(VINT nImageList = LVSIL_NORMAL) const
		{ return (HIMAGELIST)VWINDOW_SM1(LVM_GETIMAGELIST, nImageList); }

	/** Assigns an image list to the listview control. Returns the handle to
	the image list previously associated with the control if successful, or
	NULL otherwise. nImageList can be one of the following:

	LVSIL_NORMAL - Image list with large icons.

	LVSIL_SMALL - Image list with small icons.

	LVSIL_STATE- Image list with state images.*/
	HIMAGELIST		ImageList(	HIMAGELIST	hImageList,
								VINT		nImageList = LVSIL_NORMAL) const
	{
		return (HIMAGELIST)VWINDOW_SM2(LVM_SETIMAGELIST, nImageList, hImageList);
	}

	/** Inserts a new column into the listview control. column is a
	pre-initialized LV_COLUMN structure that defines information about the
	column. Returns the column index on success, -1 on failure. If nColumn
	is -1, the column will be added to the end of the existing columns. If
	the format (fmt) member of column is -1, LVCFMT_LEFT will be used as a
	default. If the width (cx) member of column is -1, a default column
	width will be used. If the column text member (pszText) is not NULL, the
	LVCF_TEXT mask will be applied, otherwise it will we removed.*/
	VINT			InsertColumn(	LV_COLUMN&	column,
									VINT		nColumn = -1) const
	{
		/* Default format?*/
		if ( column.fmt == -1 )
			column.fmt = LVCFMT_LEFT;

		if ( column.fmt )
			column.mask |= LVCF_FMT;
		else
			column.mask &= ~LVCF_FMT;

		/* Is there text?*/
		if ( column.pszText )
			column.mask |= LVCF_TEXT;
		else
			column.mask &= ~LVCF_TEXT;

		/* Should we specify the width?*/
		if ( column.cx == - 1 )
			column.cx =
				VWCL_LISTVIEW_COLUMN_PADDING_PIXELS + ((column.pszText)
				? GetStringWidth(column.pszText)
				: 0);

		/* The width is always valid.*/
		column.mask |= LVCF_WIDTH;

		/* Insert at the next column?*/
		if ( nColumn == -1 )
			nColumn = GetColumnCount();

		return (VINT)VWINDOW_SM2(LVM_INSERTCOLUMN, nColumn, &column);
	}

	/** Same as above, but simpler to use in some cases where complete
	control over the LV_COLUMN structure is not wanted or needed.*/
	VINT			InsertColumn(	VSTRING_CONST	pszText,
									VINT			nFormat = -1,
									VINT			nColumn = -1,
									VINT			nWidth = -1) const
	{
		VDECLARE_LV_COLUMN
		column.mask =	LVCF_TEXT;
		column.pszText=	(VSTRING)pszText;
		column.fmt =	nFormat;
		column.cx =		nWidth;

		return InsertColumn(column, nColumn);
	}

	/** Same as above, but insert multiple columns based on string ID's in an
	array, not necessarily contiguous. The final array element must be 0.
	Each element of pStringIDArray is a string resource ID in the application
	resource handle specified by hResource. If NULL, VGetResourceHandle()
	will be used.*/
	void			InsertColumns(	VUINT*		pStringIDArray,
									VINT		nFormat = -1,
									VINT		nColumn = -1,
									VINT		nWidth = -1,
									HINSTANCE	hResource = NULL) const
	{
		VASSERT(pStringIDArray)

		VString s;

		VDECLARE_LV_COLUMN
		column.mask =	LVCF_TEXT;
		column.fmt =	nFormat;
		column.cx =		nWidth;

		/* Initialize array index.*/
		VUINT nIndex = 0;

		/* Preserve user specified insertion point.*/
		VINT nCurrentColumn = nColumn;

		/* Loop to add columns from string loaded from resources.*/
		while ( pStringIDArray[nIndex] )
		{
			if ( s.Load(pStringIDArray[nIndex], hResource) )
			{
				column.pszText = s;

				if ( InsertColumn(column, nCurrentColumn) == -1 )
					return;

				if ( nColumn != -1 )
					nCurrentColumn++;

				column.cx =	nWidth;
			}
			else
				break;

			nIndex++;
		}
	}

	/** Same as above, but insert multiple columns based on a string's in an
	array. The final string must be NULL.*/
	void			InsertColumns(	VSTRING_CONST	pszStringArray[],
									VINT			nFormat = -1,
									VINT			nColumn = -1,
									VINT			nWidth = -1) const
	{
		VASSERT(pszStringArray)

		VDECLARE_LV_COLUMN
		column.mask =	LVCF_TEXT;
		column.fmt =	nFormat;
		column.cx =		nWidth;

		/* Initialize array index.*/
		VUINT nIndex = 0;

		/* Preserve user specified insertion point.*/
		VINT nCurrentColumn = nColumn;

		/* Loop to add columns from string array.*/
		while ( pszStringArray[nIndex] )
		{
			column.pszText = (VSTRING)pszStringArray[nIndex];

			if ( InsertColumn(column, nCurrentColumn) == -1 )
				return;

			if ( nColumn != -1 )
				nCurrentColumn++;

			column.cx =	nWidth;

			nIndex++;
		}
	}

	/** Inserts a new item into the listview control. Returns the index of
	the new item if successful, or -1 otherwise. item is a pre-initialized
	LV_ITEM structure that specifies the attributes of the item to add. If
	the index in the item struct (iItem) is -1, the item will be added to
	the end of the list. If the item text (pszText) is not NULL, LVIF_TEXT
	will be appended to the mask, otherwise it will be removed.*/
	VINT			InsertItem(LV_ITEM& item) const
	{
		/* This is never valid and the listview rejects the add if not 0.
		Use SetItem() after adding the item.*/
		VASSERT(!item.iSubItem)

		/* Is there text?*/
		if ( item.pszText )
			item.mask |= LVIF_TEXT;
		else
			item.mask &= ~LVIF_TEXT;

		/* Insert as new item?*/
		if ( item.iItem == -1 )
			item.iItem = GetItemCount();

		return (VINT)VWINDOW_SM2(LVM_INSERTITEM, 0, &item);
	}

	/** Same as above, but constructs LV_ITEM structure with usable default
	values. If nIndex is -1, the item will be added at the end of the list.*/
	VINT			InsertItem(	VSTRING_CONST	pszText,
								VINT			nIndex = -1,
								VLPARAM			nParam = 0) const
	{
		VDECLARE_LV_ITEM
		item.mask =		LVIF_PARAM;
		item.lParam =	nParam;
		item.iItem =	nIndex;
		item.pszText =	(VSTRING)pszText;

		return InsertItem(item);
	}

	/** Insert multiple items into a listview from an array of strings. This
	inserts a new item (new line) and then starts populating columns to the
	right of the inserted item. Returns index of inserted item or -1 on
	failure. The final pointer in the pszStringArray MUST be NULL.*/
	VINT			InsertItems(	VSTRING_CONST	pszStringArray[],
									VINT			nIndex = -1) const
	{
		/* This must be known!*/
		VASSERT(pszStringArray && pszStringArray[0])

		VDECLARE_LV_ITEM
		item.iItem =	nIndex;
		item.pszText =	(VSTRING)pszStringArray[0];

		/* Insert the new item.*/
		VINT nInsertIndex = InsertItem(item);

		if ( nInsertIndex != -1 )
		{
			VINT nArrayIndex =	1;
			item.iSubItem =		1;

			/* Loop to add item's columns from string array.*/
			while ( pszStringArray[nArrayIndex] )
				SetItemText(	nInsertIndex,
								pszStringArray[nArrayIndex++],
								item.iSubItem++);
		}

		return nInsertIndex;
	}

	/** Returns VTRUE if the item at nIndex is currently selected, VFALSE
	otherwise.*/
	VBOOL			IsSelected(VINT nIndex) const
		{ return ItemState(nIndex, LVIS_SELECTED); }

	/** Retrieves the state of a listview item. The only valid bits in the
	return value are those that correspond to the bits set in the nStateMask
	parameter. nStateMask specifies what information to retrieve. This
	parameter can be a combination of the following values:

	LVIS_CUT - The item is marked for a cut-and-paste operation.

	LVIS_DROPHILITED - The item is highlighted as a drag-and-drop target.

	LVIS_FOCUSED - The item has the focus, so it is surrounded by a
	standard focus rectangle. Although more than one item may be selected,
	only one item can have the focus.

	LVIS_SELECTED - The item is selected. The appearance of a selected item
	depends on whether it has the focus and also on the system colors used
	for selection.

	LVIS_OVERLAYMASK - Use this mask to retrieve the item's overlay image
	index.

	LVIS_STATEIMAGEMASK - Use this mask to retrieve the item's state
	image index.

	An item's state information includes a set of bit flags as well as
	image list indexes that indicate the item's state image and overlay
	image.*/
	VUINT			ItemState(	VINT	nIndex,
								VUINT	nStateMask) const
		{ return (VUINT)VWINDOW_SM2(LVM_GETITEMSTATE, nIndex, nStateMask); }

	/** Changes the state of an item in the listview control. Returns VTRUE
	on success, VFALSE on failure. The stateMask member specifies which
	state bits to change, and the nState contains the new values for those
	bits. In nIndex is -1, the state update applies to ALL items into the
	listview.*/
	VBOOL			ItemState(	VINT	nIndex,
								VUINT	nStateMask,
								VUINT	nState) const
	{
		VDECLARE_LV_ITEM
		item.stateMask =	nStateMask;
		item.state =		nState;

		return VWINDOW_SMB2(LVM_SETITEMSTATE, nIndex, &item);
	}

	/** Set the VLPARAM (user data) stored in the item at nIndex. Returns -1
	on failure or value on success.*/
	VLPARAM			LParam(	VINT	nIndex,
							VLPARAM	nParam) const
	{
		/* nIndex cannot be <0!*/
		VASSERT(nIndex > -1)

		VDECLARE_LV_ITEM
		item.mask =		LVIF_PARAM;
		item.iItem =	nIndex;
		item.lParam =	nParam;

		return (SetItem(item)) ? item.lParam : -1;
	}

	/** Get the VLPARAM (user data) stored in the item at nIndex. Returns
	-1 on failure or value on success.*/
	VLPARAM			LParam(VINT nIndex) const
	{
		/* nIndex cannot be <0!*/
		VASSERT(nIndex > -1)

		VDECLARE_LV_ITEM
		item.iItem =	nIndex;
		item.mask =		LVIF_PARAM;

		return (GetItem(item)) ? item.lParam : -1;
	}

	/** Forces a listview control to redraw a range of items. The specified
	items are not actually redrawn until the listview window receives a
	OnPaint() message to repaint. To repaint immediately, call the
	Update() function, or pass VTRUE for the bUpdate parameter, which is
	the default anyway. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			RedrawItems(	VINT	nFirst,
									VINT	nLast,
									VBOOL	bUpdate = VTRUE) const
	{
		if ( VWINDOW_SM2(LVM_REDRAWITEMS, nFirst, nLast) )
		{
			if ( bUpdate )
				VWindow::Update();

			return VTRUE;
		}

		return VFALSE;
	}

	/** Scrolls the content of a listview control. nDistanceX specifies the
	amount of horizontal scrolling in pixels. If the listview control is in
	listview, this value specifies the number of columns to scroll.
	nDistanceY specifies the amount of vertical scrolling in pixels. When
	the listview control is in report view, the control can only be scrolled
	vertically in whole line increments. Therefore, the nDistanceY parameter
	will be rounded to the nearest number of pixels that form a whole line
	increment. For example, if the height of a line is 16 pixels and 8 is
	passed for nDistanceY, the list will be scrolled by 16 pixels (1 line).
	If 7 is passed for dy, the list will be scrolled 0 pixels (0 lines).
	Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			Scroll(	VINT nDistanceX,
							VINT nDistanceY) const
		{ return VWINDOW_SMB2(LVM_SCROLL, nDistanceX, nDistanceY); }

	/** Select all items in the listview.*/
	void			SelectAll() const
		{ Selection(); }

	/** Select no items in the listview.*/
	void			SelectNone() const
		{ Selection(0, -1, VFALSE); }

	/** Set the current selection range from nStartIndex to nEndIndex. If
	nEndIndex is -1, all items in the list will be selected, or unselected.
	If bSelect is VTRUE, items will be selected, otherwise these will be
	un-selected.*/
	void			Selection(	VINT	nStartIndex = 0,
								VINT	nEndIndex = -1,
								VBOOL	bSelect = VTRUE) const
	{
		/* Select all or none?*/
		if ( nEndIndex == -1 )
			ItemState(-1, (bSelect) ? LVIS_SELECTED : 0, LVIS_SELECTED);
		else
		{
			VINT nCount = GetItemCount();

			if ( nEndIndex == -1 )
				nEndIndex = nCount - 1;

			VDECLARE_LV_ITEM

			for ( VINT i = 0; i < nCount; i++ )
			{
				/* Get the state information for this item.*/
				item.mask =			LVIF_STATE;
				item.stateMask =	LVIS_SELECTED;
				item.iSubItem =		0;
				item.iItem =		i;

				if (  GetItem(item) )
				{
					/* Set or remove selected bits.*/
					if ( bSelect && i >= nStartIndex && i <= nEndIndex )
						item.state |= LVIS_SELECTED;
					else
						item.state &= ~LVIS_SELECTED;

					/* Update the item.*/
					SetItem(item);
				}
			}
		}
	}

	/** Sets the attributes of a list view column. nColumn is the column
	index to set information for, and column is a pre-initialized LV_COLUMN
	structure containing what information to set. Returns VTRUE on success,
	VFALSE on failure.*/
	VBOOL			SetColumn(LV_COLUMN& column, VINT nColumn) const
		{ return VWINDOW_SMB2(LVM_SETCOLUMN, nColumn, &column); }

	/** Set the text associated with the column at nColumn. Returns VTRUE on
	success, VFALSE on failure.*/
	VBOOL			SetColumnText(	VINT			nColumn,
									VSTRING_CONST	pszText) const
	{
		VDECLARE_LV_COLUMN
		column.pszText =	(VSTRING)pszText;
		column.mask =		LVCF_TEXT;

		return SetColumn(column, nColumn);
	}

	/** Set the extended list view styles after creating the window. If these
	are supported depends on the version of the common controls library
	installed on the users machine. You should not depend on this function
	to work unless you specifically check the common controls DLL version.
	nExStyle can be one or more of the following:

	LVS_EX_CHECKBOXES - Version 4.70. - Enables check boxes for items in a
	list view control. Effectively, when set to this style, the control will
	create and set a state image list using DrawFrameControl(). Check boxes
	are visible and functional with all list view modes. The state of the
	check box for a given item is controlled by the CheckState() function.

	LVS_EX_FLATSB - Version 4.71. - Enables flat scroll bars in the list
	view. If you need more control over the appearance of the list view's
	scroll bars, you should manipulate the list view's scroll bars directly
	using the Flat Scroll Bar APIs.

	LVS_EX_FULLROWSELECT - Version 4.70. - When an item is selected, the
	item and all its subitems are highlighted. This style is available only
	in conjunction with the LVS_REPORT style.

	LVS_EX_GRIDLINES - Version 4.70. - Displays gridlines around items
	and subitems. This style is available only in conjunction with the
	LVS_REPORT style.

	LVS_EX_HEADERDRAGDROP - Version 4.70. - Enables drag-and-drop reordering
	of columns in a list view control. This style is only available to list
	view controls that use the LVS_REPORT style.

	LVS_EX_INFOTIP - Version 4.71. - The list view control sends an
	LVN_GETINFOTIP notification message to the parent window before
	displaying an item's tooltip. This style is only available to list
	view controls that use the LVS_ICON style.

	LVS_EX_MULTIWORKAREAS - Version 4.71. - If the list view control has
	the LVS_AUTOARRANGE style, the control will not autoarrange its icons
	until one or more work areas are defined (see LVM_SETWORKAREAS). To be
	effective, this style must be set before any work areas are defined and
	any items have been added to the control.

	LVS_EX_ONECLICKACTIVATE - Version 4.70. - The list view control sends an
	LVN_ITEMACTIVATE notification message to the parent window when the user
	clicks an item. This style also enables hot tracking in the list view
	control. Hot tracking means that when the cursor moves over an item, it
	is highlighted but not selected.

	LVS_EX_REGIONAL - Version 4.71. - The list view will create a region
	that includes only the item icons and text and set its window region to
	that using SetWindowRgn(). This will exclude any area that is not part
	of an item from the window region. This style is only available to list
	view controls that use the LVS_ICON style.

	LVS_EX_SUBITEMIMAGES - Version 4.70. - Allows images to be displayed
	for subitems. This style is available only in conjunction with the
	LVS_REPORT style.

	LVS_EX_TRACKSELECT - Version 4.70. - Enables hover selection in a list
	view control. Hover selection (also called track selection) means that
	an item is automatically selected when the cursor remains over the item
	for a certain period of time. The delay can be changed from the default
	system setting with the LVM_SETHOVERTIME message. This style applies to
	all styles of list view control.

	LVS_EX_TWOCLICKACTIVATE - Version 4.70. - The list view control sends an
	LVN_ITEMACTIVATE notification message to the parent window when the user
	double-clicks an item. This style also enables hot tracking in the list
	view control. Hot tracking means that when the cursor moves over an item,
	it is highlighted but not selected.

	LVS_EX_UNDERLINECOLD - Version 4.71. - Causes nonhot items to be
	displayed with underlined text. This style is ignored if
	LVS_EX_ONECLICKACTIVATE is not set.

	LVS_EX_UNDERLINEHOT - Version 4.71. - Causes hot items to be displayed
	with underlined text. This style is ignored if LVS_EX_ONECLICKACTIVATE
	or LVS_EX_TWOCLICKACTIVATE is not set.*/
	void			SetExtended(VDWORD nExStyle) const
		{ VWINDOW_SM2(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, nExStyle); }

	/** Sets the attributes of a list view item. item is a pre-initialized
	LV_ITEM structure containing what information to set. Returns VTRUE on
	success, VFALSE on failure.*/
	VBOOL			SetItem(LV_ITEM& item) const
		{ return VWINDOW_SMB2(LVM_SETITEM, 0, &item); }

	/** Causes the list view control to allocate memory for the specified
	number of items or sets the virtual number of items in a virtual list
	view control. This depends on how the list view control was created. If
	the list view control was created without the LVS_OWNERDATA style,
	sending this message causes the control to allocate its internal data
	structures for the specified number of items. This prevents the control
	from having to allocate the data structures every time an item is added.
	If the list view control was created with the LVS_OWNERDATA style
	(a virtual list view), sending this message sets the virtual number of
	items that the control contains. Returns VTRUE on sucess, VFALSE on
	failure.*/
	VBOOL			SetItemCount(VINT nNumItems) const
		{ return VWINDOW_SMB1(LVM_SETITEMCOUNT, nNumItems); }

	/** Moves an item to a specified position in a list view control (must
	be in icon or small icon view). nx and ny specify the new position of
	the item's upper-left corner, in view coordinates. If the list view
	control has the LVS_AUTOARRANGE style, the items in the list view
	control are arranged after the position of the item is set. Returns
	VTRUE on sucess, VFALSE on failure.*/
	VBOOL			SetItemPosition(	VINT nIndex,
										VINT nx,
										VINT ny) const
		{ return VWINDOW_SMB3(LVM_SETITEMPOSITION, nIndex, nx, ny); }
		
	/** Same as above, but uses a VPoint object to set the coordinates based
	on each being 32 bits.*/
	void			SetItemPosition(	VINT			nIndex,
										VPoint const&	pt) const
		{ VWINDOW_SM2(LVM_SETITEMPOSITION32, nIndex, pt.GetPointer()); }

	/** Set the text associated with the item at nIndex to pszText. If
	nSubItem is not 0, the column that intersects nIndex will have its text
	set. Returns VTRUE on sucess, VFALSE on failure.*/
	VBOOL			SetItemText(	VINT			nIndex,
									VSTRING_CONST	pszText,
									VINT			nSubItem = 0) const
	{
		VDECLARE_LV_ITEM
		item.iItem =	nIndex;
		item.iSubItem =	nSubItem;
		item.pszText =	(VSTRING)pszText;
		item.mask =		LVIF_TEXT;

		return SetItem(item);
	}

	/** Sorts list view items using an application-defined callback function
	that compares the items. pfnCompare is the address of an
	application-defined callback function, which is called during a sort
	operation each time the relative order of two list items needs to be
	compared. The callback function has the following form:
	VINT CALLBACK CompareFunc(	VLPARAM nParam1,
								VLPARAM nParam2,
								VLPARAM nParamSort);
	The callback function must return a negative value if the first item
	should precede the second, a positive value if the first item should
	follow the second, or zero if the two items are equivalent. The nParam1
	and nParam2 parameters correspond to the lParam member of the LV_ITEM
	structure for the two items being compared. The nParamSort parameter
	corresponds to the nParam parameter. nParam is a user defined value that
	can be anything you desire. Returns VTRUE if successful, or VFALSE
	otherwise.*/
	VBOOL			SortItems(	PFNLVCOMPARE	pfnCompare,
								VLPARAM			nParam = 0) const
	{
		VASSERT(pfnCompare)
		return VWINDOW_SMB2(LVM_SORTITEMS, nParam, pfnCompare);
	}

	/** Retrieves the text background color of the listview control.*/
	COLORREF		TextBackColor() const
		{ return (COLORREF)VWINDOW_SM0(LVM_GETTEXTBKCOLOR); }

	/** Set the text background color of the listview control. Returns VTRUE
	on success, VFALSE on failure.*/
	VBOOL			TextBackColor(COLORREF crTextBackColor) const
		{ return VWINDOW_SMB2(LVM_SETTEXTBKCOLOR, 0, crTextBackColor); }

	/** Return the current text color.*/
	COLORREF		TextColor() const
		{ return (COLORREF)VWINDOW_SM0(LVM_GETTEXTCOLOR); }

	/** Set the current text color. Returns VTRUE on success, VFALSE
	on failure.*/
	VBOOL			TextColor(COLORREF crTextColor) const
		{ return VWINDOW_SMB2(LVM_SETTEXTCOLOR, 0, crTextColor); }

	/** Updates a list view item. If the list view control has the
	LVS_AUTOARRANGE style, this function causes the list view control to be
	arranged. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			UpdateItem(VINT nIndex) const
		{ return VWINDOW_SMB1(LVM_UPDATE, nIndex); }
};

#endif /* VLISTVIEW*/
