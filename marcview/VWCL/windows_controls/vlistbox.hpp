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
Other Author(s) of this source code file: Michael Rich (istan@alltel.net)

VWCL and all source code are copyright (c) 1996-2000 by The VWCL Alliance.*/

#ifndef VLISTBOX
#define VLISTBOX

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"

/** When dealing with listbox item indexes, this value can be used to mean
the selected index in a single selection list, or the first selected item
in a multiple selection list.*/
#define VSELECTED_LIST_BOX_ITEM	0xffffffff

/** VListBox encapsulates the standard Windows listbox control. For Windows
95 and Windows 98, an item index parameter is limited to 16-bit values.
This means listboxes cannot contain more than 32,767 items. Although the
number of items is restricted, the total size in bytes of the items in a
listbox is limited only by available memory.*/
class VListBox : public VWindow
{
public:
	/** Default constructor initializes RTTI information in base class.*/
	VListBox(VRTTI_TYPE nRTTI = VWCL_RTTI_LISTBOX)
		: VWindow(nRTTI)
		{;}

	/** Add the specified filename to a listbox that contains a directory
	listing. The listbox must already have had the Dir() function called
	before AddFile(). The return value is the zero-based index of the file
	that was added, or LB_ERR if an error occurs.*/
	VLONG		AddFile(VSTRING_CONST pszFilename) const
		{ return VWINDOW_SM2(LB_ADDFILE, 0, pszFilename); }

	/** Same as AddString(), but used when adding items to an owner-drawn
	combobox that does not have the CBS_HASSTRINGS style, and where the
	item data is a numeric value.*/
	VLONG		AddItem(VLONG nValue) const
	{
		/* Verify correct style for using this function.*/
		VASSERT(IsOwnerDrawn())
		VASSERT(!(Style() & LBS_HASSTRINGS))

		/* Add to listbox.*/
		return AddString((VSTRING_CONST)nValue);
	}

	/** Same as AddString(), but used when adding items to an owner-drawn
	combobox that does not have the CBS_HASSTRINGS style, and where the
	item data is a pointer to an unknown type.*/
	VLONG		AddItem(VVOIDPTR_CONST pItem) const
		{ return AddItem((VLONG)pItem); }

	/** Add a string to a listbox. If the listbox does not have the LBS_SORT
	style, the string is added to the end of the list. Otherwise, the
	string is inserted into the list and the list is sorted. pszString is a
	pointer to the null-terminated string that is to be added. If you
	create the listbox with an owner-drawn style but without the
	LBS_HASSTRINGS style, the value of the pszString parameter is stored as
	item data instead of the string it would otherwise point to. You can
	use ItemData() to retrieve or modify the item data. If you create an
	owner-drawn listbox with the LBS_SORT style but not the LBS_HASSTRINGS
	style, the system sends the WM_COMPAREITEM message one or more times to
	the owner of the listbox to place the new item properly in the listbox.
	If this is the case, the AddItem() function makes more sense to use and
	avoids the typecast. To insert a string at a specific location within
	the list, use InsertString() or InsertItem(). The return value is the
	zero-based index of the string in the listbox. If an error occurs, the
	return value is LB_ERR. If there is insufficient space to store the new
	string, the return value is LB_ERRSPACE.*/
 	VLONG		AddString(VSTRING_CONST pszString) const
		{ return VWINDOW_SM2(LB_ADDSTRING, 0, pszString); }

	/** Retrieve the index of the anchor item, that is, the item from which
	a multiple selection starts. A multiple selection spans all items from
	the anchor item to the caret item. The return value is the index of the
	anchor item.*/
	VLONG		AnchorIndex() const
	{
		VASSERT(IsMultipleSelection())
		return VWINDOW_SM0(LB_GETANCHORINDEX);
	}

	/** Set the anchor item, that is, the item from which a multiple
	selection starts. A multiple selection spans all items from the anchor
	item to the caret item. nIndex specifies the index of the new anchor
	item. If the function succeeds, the return value is zero. If the
	function fails, the return value is LB_ERR.*/
	VLONG		AnchorIndex(VUINT nIndex) const
	{
		VASSERT(IsMultipleSelection())
		return VWINDOW_SM1(LB_SETANCHORINDEX, nIndex);
	}

	/** Determine the index of the item that has the focus rectangle in a
	multiple-selection listbox. The item may or may not be selected. The
	return value is the zero-based index of the listbox item that has the
	focus rectangle. If the listbox is a single-selection listbox, the
	return value is the zero-based index of the item that is selected, if
	any.*/
	VLONG		CaretIndex() const
	{
		VASSERT(IsMultipleSelection())
		return VWINDOW_SM0(LB_GETCARETINDEX);
	}

	/** Set the focus rectangle to the item at the specified index in a
	multiple-selection listbox. If the item is not visible, it is scrolled
	into view. nIndex specifies the zero-based index of the listbox item
	that is to receive the focus rectangle. If bScroll value is VFALSE, the
	item is scrolled until it is fully visible; if it is VTRUE, the item is
	scrolled until it is at least partially visible. If an error occurs,
	the return value is LB_ERR.*/
	VLONG		CaretIndex(	VUINT nIndex,
							VBOOL bScroll = VTRUE) const
	{
		VASSERT(IsMultipleSelection())
		return VWINDOW_SM3(LB_SETCARETINDEX, nIndex, bScroll, 0);
	}

	/** Retrieve the number of items in a listbox. The returned count is one
	greater than the index value of the last item (the index is zero-based).
	The return value is the number of items in the listbox, or LB_ERR if an
	error occurs.*/
	VLONG		Count() const
		{ return VWINDOW_SM0(LB_GETCOUNT); }

	/** Set the count of items in a listbox created with the LBS_NODATA
	style and not created with the LBS_HASSTRINGS style. nNumItems specifies
	the new count of items in the listbox. Supported only by listboxes
	created with the LBS_NODATA style and not created with the
	LBS_HASSTRINGS style. All other listboxes return LB_ERR. If an error
	occurs, the return value is LB_ERR. If there is insufficient memory to
	store the items, the return value is LB_ERRSPACE.*/
	VLONG		Count(VINT nNumItems) const
		{ return VWINDOW_SM1(LB_SETCOUNT, nNumItems); }

	/** Create a new listbox control. nStyle is a combination of WS_xxx
	style bits, or'd with these listbox specific bits:

	LBS_EXTENDEDSEL - The user can select multiple items using the SHIFT
	key and the mouse or special key combinations.

	LBS_HASSTRINGS - Specifies an owner-draw listbox that contains items
	consisting of strings. The listbox maintains the memory and pointers
	for the strings so the application can use the GetText() member function
	to retrieve the text for a particular item.

	LBS_MULTICOLUMN - Specifies a multicolumn listbox that is scrolled
	horizontally. The SetColumnWidth() member function sets the width of
	the columns.

	LBS_MULTIPLESEL - String selection is toggled each time the user clicks
	or double-clicks the string. Any number of strings can be selected.

	LBS_NOINTEGRALHEIGHT - The size of the listbox is exactly the size
	specified by the application when it created the listbox. Usually,
	Windows sizes a listbox so that the listbox does not display partial
	items.

	LBS_NOREDRAW - List-box display is not updated when changes are made.
	This style can be changed at any time by sending a WM_SETREDRAW message.

	LBS_NOTIFY - Parent window receives an input message whenever the user
	clicks or double-clicks a string.

	LBS_OWNERDRAWFIXED - The owner of the listbox is responsible for
	drawing its contents; the items in the listbox are the same height.

	LBS_OWNERDRAWVARIABLE - The owner of the listbox is responsible for
	drawing its contents; the items in the listbox are variable in height.

	LBS_SORT - Strings in the listbox are sorted alphabetically.

	LBS_STANDARD - Strings in the listbox are sorted alphabetically, and
	the parent window receives an input message whenever the user clicks or
	double-clicks a string. The listbox contains borders on all sides.

	LBS_USETABSTOPS - Allows a listbox to recognize and expand tab
	characters when drawing its strings. The default tab positions are 32
	dialog units. (A dialog unit is a horizontal or vertical distance. One
	horizontal dialog unit is equal to one-fourth of the current dialog base
	width unit. The dialog base units are computed based on the height and
	width of the current system font. The GetDialogBaseUnits() Windows
	function returns the current dialog base units in pixels.)

	LBS_WANTKEYBOARDINPUT - The owner of the listbox receives WM_VKEYTOITEM
	or WM_CHARTOITEM messages whenever the user presses a key while the
	listbox has input focus. This allows an application to perform special
	processing on the keyboard input.

	LBS_DISABLENOSCROLL - The listbox shows a disabled vertical scroll bar
	when the listbox does not contain enough items to scroll. Without this
	style, the scroll bar is hidden when the listbox does not contain enough
	items.*/
	VBOOL		Create(	VWindow const&	windowParent,
						VRect const&	rect,
						VUINT			nID,
						VUINT			nStyle =
										WS_BORDER |
										WS_TABSTOP |
										WS_VISIBLE |
										LBS_STANDARD)
	{
		VASSERT(windowParent.GetSafeWindow())

		return VWindow::Create(	windowParent,
								rect,
								WS_CHILD | nStyle,
								VTEXT("LISTBOX"),
								NULL,
								nID);
	}

	/** Remove all items from a listbox. If you create the listbox with an
	owner-drawn style, but without the LBS_HASSTRINGS style, the owner of
	the listbox receives a WM_DELETEITEM message for each item in the
	listbox.*/
	void		DeleteAllItems() const
		{ VWINDOW_SM0(LB_RESETCONTENT); }

	/** Delete an item from the listbox at nIndex. If an application creates
	the listbox with an owner-drawn style but without the LBS_HASSTRINGS
	style, the system sends a WM_DELETEITEM message to the owner of the
	listbox so the application can free any additional memory associated
	with the item. The return value is a count of the items remaining in
	the list. The return value is LB_ERR if the index parameter specifies
	an index greater than the number of items in the list.*/
	VLONG		DeleteIndex(VUINT nIndex = VSELECTED_LIST_BOX_ITEM) const
	{
		return (	IndexOrSelection(nIndex) != VSELECTED_LIST_BOX_ITEM)
					? VWINDOW_SM1(LB_DELETESTRING, nIndex)
					: LB_ERR;
	}

	/** Add a list of filenames to a listbox. nAttributes specifies the
	attributes of the files to be added to the listbox. This parameter can
	be a combination of the following values:

	DDL_ARCHIVE - Includes archived files.

	DDL_DIRECTORY - Includes subdirectories. Subdirectory names are
	enclosed in square brackets ([ ]).

	DDL_DRIVES - Includes drives. Drives are listed in the form [-x-],
	where x is the drive letter.

	DDL_EXCLUSIVE - Includes only files with the specified attributes. By
	default, read-write files are listed even if DDL_READWRITE is not
	specified.

	DDL_HIDDEN - Includes hidden files.

	DDL_READONLY - Includes read-only files.

	DDL_READWRITE - Includes read-write files with no additional attributes.

	DDL_SYSTEM - Includes system files.

	pszFileSpec is a pointer to the null-terminated string that specifies
	the filename to add to the list. If the filename contains wildcards
	(for example, *.*), all files that match the wildcards and have the
	attributes specified by the nAttributes parameter are added to the list.
	The return value is the zero-based index of the last filename added to
	the list. If an error occurs, the return value is LB_ERR. If there is
	insufficient space to store the new strings, the return value is
	LB_ERRSPACE.*/
	VLONG		Dir(	VUINT			nAttributes,
						VSTRING_CONST	pszFileSpec) const
		{ return VWINDOW_SM2(LB_DIR, nAttributes, pszFileSpec); }

	/** Same as FindString() but used when searching an owner-drawn listbox
	without the LBS_HASSTRINGS style for a value added to the listbox with
	AddItem() or InsertItem().*/
	VLONG		FindItem(	VLONG nValue,
							VLONG nStartIndex = -1) const
	{
		/* Verify correct style for using this function.*/
		VASSERT(IsOwnerDrawn())
		VASSERT(!(Style() & LBS_HASSTRINGS))

		/* Find it.*/
		return FindString((VSTRING_CONST)nValue, nStartIndex);
	}

	/** Same as FindString() but used when searching an owner-drawn listbox
	without the LBS_HASSTRINGS style for a pointer value added to the
	listbox with AddItem() or InsertItem().*/
	VLONG		FindItem(	VVOIDPTR_CONST	pItem,
							VLONG			nStartIndex = -1) const
		{ return FindItem((VLONG)pItem, nStartIndex); }

	/** Find the first string in a listbox that contains the specified
	prefix. nIndexStart specifies the zero-based index of the item before
	the first item to be searched. When the search reaches the bottom of
	the listbox, it continues from the top of the listbox back to the item
	specified by the nIndexStart parameter. If nIndexStart is -1, the
	entire listbox is searched from the beginning. pszFind is a pointer to
	the null-terminated string that contains the prefix to search for. The
	search is case independent, so this string can contain any combination
	of uppercase and lowercase letters. If you create the listbox with an
	owner-drawn style but without the LBS_HASSTRINGS style, this function
	returns the index of the item with an item data value that matches the
	value supplied as the pszFind parameter. The item data is stored during
	calls to AddItem() or InsertItem(), or by the ItemData() function. The
	return value is the index of the matching item, or LB_ERR if the search
	was unsuccessful.*/
	VLONG		FindString(	VSTRING_CONST	pszFind,
							VLONG			nIndexStart = -1) const
		{ return VWINDOW_SM2(LB_FINDSTRING, nIndexStart, pszFind); }

	/** Find the first listbox string that matches the string specified in
	the pszFind parameter. nIndexStart specifies the zero-based index of
	the item before the first item to be searched. When the search reaches
	the bottom of the listbox, it continues from the top of the listbox
	back to the item specified by the nIndexStart parameter. If nIndexStart
	is -1, the entire listbox is searched from the beginning. pszFind is a
	pointer to the null-terminated string to search for. This string can
	contain a complete filename, including the extension. The search is not
	case sensitive, so this string can contain any combination of uppercase
	and lowercase letters. If an application creates the listbox with an
	owner-drawn style but without the LBS_HASSTRINGS style, the action
	taken by LB_FINDSTRINGEXACT depends on whether the LBS_SORT style is
	used. If LBS_SORT is used, the system sends WM_COMPAREITEM messages to
	the listbox owner to determine which item matches the specified string.
	Otherwise, LB_FINDSTRINGEXACT attempts to match the 32-bit item data
	value against the value of the pszFind parameter. The return value is
	the zero-based index of the matching item, or LB_ERR if the search
	was unsuccessful.*/
	VLONG		FindStringExact(	VSTRING_CONST	pszFind,
									VLONG			nIndexStart = -1) const
		{ return VWINDOW_SM2(LB_FINDSTRINGEXACT, nIndexStart, pszFind); }

	/** Retrieve the dimensions of the rectangle that bounds a listbox item
	as it is currently displayed in the listbox into a VRect object. nIndex
	specifies the zero-based index of the item. If an error occurs, the
	return value is LB_ERR.*/
	VLONG		GetIndexRect(	VRect&	rect,
								VUINT	nIndex = VSELECTED_LIST_BOX_ITEM) const
	{
		return (	IndexOrSelection(nIndex) != VSELECTED_LIST_BOX_ITEM)
					? VWINDOW_SM2(LB_GETITEMRECT, nIndex, rect.GetPointer())
					: LB_ERR;
	}

	/** Retrieve a string from the listbox. nIndex specifies the zero-based
	index of the string to retrieve. strBuffer is the string object that
	receives the string. The return value is the internal string pointer of
	strBuffer, or NULL on failure. It is not valid to call this function
	with a listbox created as an owner-drawn style but without the
	LBS_HASSTRINGS style. This is valid to do for Windows, but use of the
	ItemData() functions is required by VWCL. strBuffer will not be
	modified on failure.*/
	VSTRING		GetIndexText(	VString&	strBuffer,
								VUINT		nIndex =
											VSELECTED_LIST_BOX_ITEM) const
	{
		if ( IndexOrSelection(nIndex) != VSELECTED_LIST_BOX_ITEM )
		{
			VLONG nLength = GetIndexTextLength(nIndex);

			/* Allocate memory for item text and get it.*/
			if ( nLength > 0 && strBuffer.String(NULL, nLength + 1) )
			{
				VWINDOW_SM2(LB_GETTEXT, nIndex, strBuffer.String());
				strBuffer.UpdateLength(nLength);

				return strBuffer;
			}
		}

		return NULL;
	}

	/** Retrieve the length, in characters, of a string in the combobox.
	nIndex specifies the zero-based index of the string. The return value
	is the length of the string, in characters, excluding the terminating
	null character. Under certain conditions, this value may actually be
	greater than the length of the text. If the nIndex parameter does not
	specify a valid index, the return value is CB_ERR. Under certain
	conditions, the return value is larger than the actual length of the
	text. This occurs with certain mixtures of ANSI and Unicode, and is due
	to the operating system allowing for the possible existence of
	double-byte character set (DBCS) characters within the text. The return
	value, however, will always be at least as large as the actual length
	of the text, so you can always use it to guide buffer allocation. This
	behavior can occur when an application uses both ANSI functions and
	common dialogs, which use Unicode.*/
	VLONG		GetIndexTextLength(
		VUINT nIndex = VSELECTED_LIST_BOX_ITEM) const
	{
		return (	IndexOrSelection(nIndex) != VSELECTED_LIST_BOX_ITEM)
					? VWINDOW_SM1(LB_GETTEXTLEN, nIndex)
					: LB_ERR;
	}

	/** Retrieve the total number of selected items in a multiple-selection
	listbox. Uses GetSelection() to retrieve the selected item index in a
	singe selection listbox. If the listbox is a single-selection listbox,
	the return value is LB_ERR.*/
	VLONG		GetSelectedCount() const
	{
		VASSERT(IsMultipleSelection())
		return VWINDOW_SM0(LB_GETSELCOUNT);
	}

	/** Fill a buffer with an array of integers that specify the index
	numbers of selected items in a multiple-selection listbox. nNumItems
	specifies the maximum number of selected items whose item numbers are
	to be placed in the buffer. pItems is a pointer to a buffer large
	enough for the number of integers specified by the nNumItems parameter.
	The return value is the number of items placed in the buffer. If the
	listbox is a single-selection listbox, the return value is LB_ERR.*/
	VLONG		GetSelectedItems(	VUINT nNumItems,
									VUINT* pItems) const
	{
		VASSERT(nNumItems && pItems && IsMultipleSelection())
		return VWINDOW_SM2(LB_GETSELITEMS, nNumItems, pItems);
	}

	/** Retrieve the index of the currently selected item, if any, in a
	single-selection listbox, or the first selected item in a multiple
	selection listbox. If there is no selection, the return value is LB_ERR.*/
	VLONG		GetSelection() const
	{
		VLONG nItem = LB_ERR;

		if ( IsMultipleSelection() && GetSelectedCount() )
			GetSelectedItems(1, (VUINT*)&nItem);
		else
			nItem = VWINDOW_SM0(LB_GETCURSEL);

		return nItem;
	}

	/** Retrieve the index of the first visible item in a listbox. Initially
	the item with index 0 is at the top of the listbox, but if the listbox
	contents have been scrolled another item may be at the top. The return
	value is the index of the first visible item in the listbox.*/
	VLONG		GetTopIndex() const
		{ return VWINDOW_SM0(LB_GETTOPINDEX); }

	/** Retrieve from a listbox the width, in pixels, by which the listbox
	can be scrolled horizontally (the scrollable width) if the listbox has
	a horizontal scroll bar. To respond to this function, the listbox must
	have been defined with the WS_HSCROLL style. The return value is the
	scrollable width, in pixels, of the listbox.*/
	VLONG		HorizontalExtent() const
		{ return VWINDOW_SM0(LB_GETHORIZONTALEXTENT); }

	/** Set the width, in pixels, by which a listbox can be scrolled
	horizontally (the scrollable width). If the width of the listbox is
	smaller than this value, the horizontal scroll bar horizontally scrolls
	items in the listbox. If the width of the listbox is equal to or
	greater than this value, the horizontal scroll bar is hidden. To
	respond to this function, the listbox must have been defined with
	the WS_HSCROLL style. nWidth specifies the number of pixels by which
	the listbox can be scrolled.*/
	void		HorizontalExtent(VUINT nWidth) const
		{ VWINDOW_SM1(LB_SETHORIZONTALEXTENT, nWidth); }

#if(WINVER >= 0x0400)
	/** Retrieve the zero-based index of the item nearest the specified
	point in a listbox. nX specifies the x-coordinate of a point, relative
	to the upper-left corner of the client area of the listbox. nY specifies
	the y-coordinate of a point, relative to the upper-left corner of the
	client area of the listbox. The return value contains the index of the
	nearest item in the low-order word. The high-order word is zero if the
	specified point is in the client area of the listbox, or one if it is
	outside the client area.*/
	VLONG		IndexFromPoint(VINT nX, VINT nY) const
		{ return VWINDOW_SM3(LB_ITEMFROMPOINT, 0, nX, nY); }
#endif

	/** Retrieve the height of items in a listbox. nIndex specifies the
	zero-based index of the listbox item. This index is used only if the
	listbox has the LBS_OWNERDRAWVARIABLE style; otherwise, it must be
	zero. The return value is the height, in pixels, of each item in the
	listbox. The return value is the height of the item specified by the
	index parameter if the listbox has the LBS_OWNERDRAWVARIABLE style.
	The return value is LB_ERR if an error occurs.*/
	VINT		IndexHeight(VUINT nIndex) const
		{ return (VINT)VWINDOW_SM1(LB_GETITEMHEIGHT, nIndex); }

	/** Set the height, in pixels, of items in a listbox. If the listbox has
	the LBS_OWNERDRAWVARIABLE style, this message sets the height of the
	item specified by the nIndex parameter. Otherwise, this message sets
	the height of all items in the listbox. nIndex specifies the zero-based
	index of the item in the listbox. Use this parameter only if the listbox
	has the LBS_OWNERDRAWVARIABLE style; otherwise, set it to zero. If the
	index or height is invalid, the return value is LB_ERR.*/
	VLONG		IndexHeight(	VUINT	nIndex,
								VINT	nHeight) const
		{ return VWINDOW_SM3(LB_SETITEMHEIGHT, nIndex, nHeight, 0); }

	/** Set nIndex to the selected item in a single selection listbox or
	first selected item in a multiple selection listbox if nIndex is
	VSELECTED_LIST_BOX_ITEM. If nothing is selected, nIndex will be set
	to VSELECTED_LIST_BOX_ITEM to indicate error. nIndex will be returned
	as well.*/
	VUINT		IndexOrSelection(VUINT& nIndex) const
	{
		if ( nIndex == VSELECTED_LIST_BOX_ITEM )
			nIndex = GetSelection();

		return nIndex;
	}

#if(WINVER >= 0x0400)
	/** An application can call this to allocate memory before adding a
	large number of items to the listbox. nNumItems specifies the number of
	items to add. nStringBytes specifies the amount of memory to allocate
	for item strings, in bytes. If the function is successful, the return
	value is the total number of items for which memory has been
	pre-allocated, that is, the total number of items added by all
	successful InitStorage() functions. If the function fails, the return
	value is LB_ERRSPACE. For Windows NT version 4.0, this message does not
	allocate the specified amount of memory, however it always returns the
	value specified in the nNumItems parameter. For Windows NT version 5.0,
	the message allocates memory and returns the success and error values
	described above. The InitStorage() functions helps speed up the
	initialization of listboxes that have a large number of items (over 100).
	It reserves the specified amount of memory so that subsequent Addxxx(),
	Insertxxx(), and Dir() functions take the shortest possible time. You
	can use estimates for the nNumItems and nStringBytes parameters. If you
	overestimate, the extra memory is allocated, if you underestimate, the
	normal allocation is used for items that exceed the requested amount.*/
	VLONG		InitStorage(	VUINT nNumItems,
								VUINT nStringBytes) const
		{ return VWINDOW_SM2(LB_INITSTORAGE, nNumItems, nStringBytes); }
#endif

	/** Same as InsertString(), but used when adding items to an owner-drawn
	listbox that does not have the LBS_HASSTRINGS style, and where the item
	data is a numeric value.*/
	VLONG		InsertItem(	VLONG nValue,
							VUINT nIndex = VSELECTED_LIST_BOX_ITEM) const
	{
		/* Verify correct style for using this function.*/
		VASSERT(IsOwnerDrawn())
		VASSERT(!(Style() & LBS_HASSTRINGS))

		return InsertString((VSTRING_CONST)nValue, nIndex);
	}

	/** Same as InsertString(), but used when adding items to an owner-drawn
	listbox that does not have the LBS_HASSTRINGS style, and where the item
	data is a pointer to an unknown type.*/
	VLONG		InsertItem(	VVOIDPTR_CONST	pItem,
							VUINT			nIndex =
											VSELECTED_LIST_BOX_ITEM) const
		{ return InsertItem((VLONG)pItem, nIndex); }

	/** Insert a string into the listbox at a specified index. Unlike the
	AddString() function, the InsertString() does not cause a list with the
	LBS_SORT style to be sorted. nIndex specifies the zero-based index of
	the position at which to insert the string. If this parameter is -1, the
	string is added to the end of the list. pszString is a pointer to a
	null-terminated string to be inserted. If you create the listbox with an
	owner-drawn style but without the LBS_HASSTRINGS style, the value of
	the pszString parameter is stored rather than the string it would
	otherwise point to. The return value is the index of the position at
	which the string was inserted. If an error occurs, the return value is
	LB_ERR. If there is insufficient space available to store the new
	string, it is LB_ERRSPACE. pszString is a pointer to the null-terminated
	string to be inserted. If you create the listbox with an owner-drawn
	style but without the LBS_HASSTRINGS style, the value of the pszString
	parameter is stored rather than the string it would otherwise point to.
	The item data can be retrieved or modified with ItemData(). If this is
	the case, the InsertItem() function makes more sense to use and avoids
	the typecast.*/
	VLONG		InsertString(	VSTRING_CONST	pszText,
								VUINT			nIndex =
												VSELECTED_LIST_BOX_ITEM) const
	{
		return (	IndexOrSelection(nIndex) != VSELECTED_LIST_BOX_ITEM)
					? VWINDOW_SM2(LB_INSERTSTRING, nIndex, pszText)
					: LB_ERR;
	}

	/** Returns VTRUE if the listbox is multiple or extended selection,
	VFALSE if single selection.*/
	VBOOL		IsMultipleSelection() const
	{
		return (	(Style() & LBS_EXTENDEDSEL) ||
					(Style() & LBS_MULTIPLESEL))
					? VTRUE : VFALSE;
	}

	/** Returns VTRUE if the listbox is owner-drawn, VFALSE if not.*/
	VBOOL		IsOwnerDrawn() const
	{
		return (	(Style() & LBS_OWNERDRAWFIXED) ||
					(Style() & LBS_OWNERDRAWVARIABLE))
					? VTRUE : VFALSE; }

	/** Retrieve the selection state of an item. nIndex specifies the
	zero-based index of the item. If an item is selected, the return value
	is VTRUE, otherwise, it is VFALSE.*/
	VBOOL		IsSelected(VUINT nIndex) const
		{ return VWINDOW_SMB1(LB_GETSEL, nIndex); }

	/** Retrieve the application-defined 32-bit value associated with the
	specified listbox item. nIndex specifies the index of the item. The
	return value is the 32-bit value associated with the item, or LB_ERR if
	an error occurs. If the item is in an owner-drawn listbox and was
	created without the LBS_HASSTRINGS style, this 32-bit value was set when
	added to the list. Otherwise, it is the value set in a previous call to
	ItemData().*/
	VLONG		ItemData(VUINT nIndex) const
		{ return VWINDOW_SM1(LB_GETITEMDATA, nIndex); }

	/** Set a 32-bit value associated with the specified item in the listbox.
	nIndex specifies the zero-based index of the item. If the specified
	item is in an owner-drawn listbox created without the LBS_HASSTRINGS
	style, this function replaces the 32-bit value set when the item was
	added to the list.If an error occurs, the return value is LB_ERR.*/
	VLONG		ItemData(	VUINT nIndex,
							VLONG nData) const
		{ return VWINDOW_SM2(LB_SETITEMDATA, nIndex, nData); }

	/** Same as above, but avoids the typecast if setting item data using
	a pointer.*/
	VLONG		ItemData(	VUINT		nIndex,
							VVOIDPTR	pData) const
		{ return ItemData(nIndex, (VLONG)pData); }

	/** Retrieve the current locale of the listbox. You can use the locale
	to determine the correct sorting order of displayed text (for listboxes
	with the LBS_SORT style). The return value is a 32-bit value that
	specifies the current locale of the listbox. The high-order word
	contains the country code and the low-order word contains the language
	identifier. The language identifier consists of a sublanguage identifier
	and a primary language identifier. Use the PRIMARYLANGID macro to
	extract the primary language identifier from the low-order word of the
	return value, and the SUBLANGID macro to extract the sublanguage
	identifier.*/
	VLONG		Locale() const
		{ return VWINDOW_SM0(LB_GETLOCALE); }

	/** Set the current locale of the listbox. You can use the locale to
	determine the correct sorting order of displayed text (for listboxes
	with the LBS_SORT style). nLocaleID specifies the locale identifier
	that the listbox will use for sorting when adding text. The return
	value is the previous locale identifier. If the nLocaleID parameter
	specifies a locale that is not installed on the system, the return
	value is LB_ERR and the current listbox locale is not changed. Use the
	MAKELCID macro to construct a locale identifier.*/
	VLONG		Locale(VUINT nLocaleID) const
		{ return VWINDOW_SM1(LB_SETLOCALE, nLocaleID); }

	/** Select an item and scroll it into view, if necessary. When the new
	item is selected, the listbox removes the highlight from the previously
	selected string. nIndex specifies the zero-based index of the item that
	is to be selected. If the nIndex parameter is -1, the listbox is set to
	have no selection. Use this function only with single-selection
	listboxes. You cannot use it to set or remove a selection in a
	multiple-selection listbox. If an error occurs, the return value is
	LB_ERR. If the index parameter is -1, the return value is LB_ERR even
	though no error occurred.*/
	VLONG		Select(VLONG nIndex) const
		{ return VWINDOW_SM1(LB_SETCURSEL, nIndex); }

	/** Select one or more consecutive items in a multiple-selection
	listbox. nFirst specifies the zero-based index of the first item to
	select. nLast specifies the zero-based index of the last item to
	select. If bSelect is VTRUE, the items will be selected, otherwise
	they will be un-selected. If an error occurs, the return value is
	LB_ERR.*/
	VLONG		SelectIndexRange(	VINT	nFirst,
									VINT	nLast,
									VBOOL	bSelect = VTRUE) const
		{ return VWINDOW_SM3(LB_SELITEMRANGE, bSelect, nFirst, nLast); }

	/** Select or UnSelect a string in a multiple-selection listbox. bSelect
	specifies how to set the selection. If the bSelect parameter is VTRUE,
	the string is selected and highlighted. If bSelect is VFALSE, the
	highlight is removed and the string is no longer selected. nIndex
	specifies the zero-based index of the string to set. If nIndex is -1,
	the selection is added to or removed from all strings, depending on the
	value of bSelect. Use this function only with multiple-selection
	listboxes. If an error occurs, the return value is LB_ERR.*/
	VLONG		SelectMultiple(	VBOOL bSelect,
								VLONG nIndex) const
	{
		VASSERT(IsMultipleSelection())
		return VWINDOW_SM2(LB_SETSEL, bSelect, nIndex);
	}

	/** Same as SelectString(), but used when selecting items in an
	owner-drawn listbox that does not have the LBS_HASSTRINGS style and
	where the item data is a numeric value.*/
	VLONG		SelectItem(	VLONG nValue,
							VLONG nStartIndex = -1) const
	{
		/* Verify correct style for using this function.*/
		VASSERT(IsOwnerDrawn())
		VASSERT(!(Style() & LBS_HASSTRINGS))

		return SelectString((VSTRING_CONST)nValue, nStartIndex);
	}

	/** Same as SelectString(), but used when selecting items in an
	owner-drawn listbox that does not have the LBS_HASSTRINGS style and
	where the item data is a pointer to an unknown type.*/
	VLONG		SelectItem(	const VVOIDPTR	pItem,
							VLONG			nStartIndex = -1) const
		{ return SelectItem((VLONG)pItem, nStartIndex); }

	/** Search the list for an item that begins with the characters in a
	specified string. nStartIndex specifies the zero-based index of the
	item preceding the first item to be searched. When the search reaches
	the bottom of the list, it continues from the top of the list back to
	the item specified by the parameter. If nIndexStart is -1, the entire
	list is searched from the beginning. pszSelect is a pointer to a
	null-terminated string that contains the prefix to search for. The
	search is not case sensitive, so this string can contain any combination
	of uppercase and lowercase letters. If a matching item is found, it is
	selected and copied to the edit control. If the string is found, the
	return value is the index of the selected item. If the search is
	unsuccessful, the return value is LB_ERR and the current selection is
	not changed. If you create the listbox with an owner-drawn style but
	without the LBS_HASSTRINGS style, what the SelectString() function does
	depends on whether you use the LBS_SORT style. If the LBS_SORT style is
	used, the system sends WM_COMPAREITEM messages to the owner of the
	listbox to determine which item matches the specified string. If you do
	not use the LBS_SORT style, SelectString() attempts to match the item
	data value against the value of the pszSelect parameter.*/
	VLONG		SelectString(	VSTRING_CONST	pszSelect,
								VLONG			nStartIndex = -1) const
		{ return VWINDOW_SM2(LB_SELECTSTRING, nStartIndex, pszSelect); }

	/** An application calls SetColumnWidth() on a multiple-column listbox
	(created with the LBS_MULTICOLUMN style) to set the width, in pixels,
	of all columns in the listbox.*/
	void		SetColumnWidth(VUINT nColumnWidth) const
		{ VWINDOW_SM1(LB_SETCOLUMNWIDTH, nColumnWidth); }

	/** Modifies the string for a specified row in the listbox. nIndex
	specifies the zero-based index of the string to modify. pszString is the
	string that contains the new value. The return value is the index of
	the row in the listbox, or -1 on failure. It is not valid to call this
	function with a listbox created as an owner-drawn style but without the
	LBS_HASSTRINGS style. This is valid to do for Windows, but use of the
	ItemData() functions is required by VWCL.*/
	VLONG		SetIndexText(	VSTRING_CONST	pszString,
								VUINT			nIndex =
												VSELECTED_LIST_BOX_ITEM) const
	{
		#ifdef VWCL_DEBUG
			/* If owner-drawn, it must have the LBS_HASSTRINGS style set!*/
			if ( (IsOwnerDrawn()) && !(Style() & LBS_HASSTRINGS) )
			{
				VASSERT(VFALSE)
			}
		#endif

		/* Make sure the item actually exists.*/
		if ( IndexOrSelection(nIndex) != VSELECTED_LIST_BOX_ITEM )
		{
			/* Remember if its selected or not.*/
			VBOOL bSelected = IsSelected(nIndex);

			/* Save the existing item data value.*/
			VLONG nOldItemData = ItemData(nIndex);

			/* Remove the existing row from the box.*/
			DeleteIndex(nIndex);

			/* Create a new row to replace the one we just removed.*/
			VLONG nCurSlot = InsertString(pszString, nIndex);
			ItemData(nCurSlot, nOldItemData);

			/* Reset old selection.*/
			if ( bSelected )
			{
				if ( IsMultipleSelection() )
					SelectMultiple(VTRUE, nCurSlot);
				else
					Select(nCurSlot);
			}

			/* Return the index.*/
			return nCurSlot;
		}

		/* Return invalid row.*/
		return -1;
	}

	/** Set the tab-stop positions in a listbox. nNumTabs specifies the
	number of tab stops in the listbox. pnTabs is a pointer to the first
	member of an array of integers containing the tab stops, in dialog
	template units. The tab stops must be sorted in ascending order,
	backward tabs are not allowed. If all the specified tabs are set, the
	return value is VTRUE, otherwise, it is VFALSE. To respond to
	SetTabStops(), the listbox must have been created with the
	LBS_USETABSTOPS style. If nNumTabs parameter is 0, and pnTabs is NULL,
	the default tab stop is two dialog template units. If nNumTabs is 1,
	the listbox will have tab stops separated by the distance specified by
	pnTabs. If pnTabs points to more than a single value, a tab stop will
	be set for each value in pnTabs, up to the number specified by nNumTabs.
	The values specified by the pnTabs parameter are in dialog template
	units, which are the device-independent units used in dialog box
	templates. To convert measurements from dialog template units to screen
	units (pixels), use the MapDialogRect function.*/
	VBOOL		SetTabStops(	VUINT nNumTabs,
								VINT* pnTabs) const
		{ return VWINDOW_SMB2(LB_SETTABSTOPS, nNumTabs, pnTabs); }

	/** Ensure that a particular item in a listbox is visible. The system
	scrolls the listbox contents so that either the specified item appears
	at the top of the listbox or the maximum scroll range has been reached.
	If an error occurs, the return value is LB_ERR.*/
	VLONG		SetTopIndex(VUINT nIndex) const
		{ return VWINDOW_SM1(LB_SETTOPINDEX, nIndex); }
};

#endif /* VLISTBOX*/
