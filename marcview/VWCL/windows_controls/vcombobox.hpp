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

#ifndef VCOMBOBOX
#define VCOMBOBOX

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"

/** When dealing with combobox item indexes, this value can be used to mean
the selected item in a single selection list, or the first selected item
in a multiple selection list.*/
#define VSELECTED_COMBO_BOX_ITEM	0xffffffff

/** VComboBox encapuslates the standard Windows combobox control. For Windows
95 and Windows 98, an item index parameter is limited to 16-bit values. This
means comboboxes cannot contain more than 32,767 items. Although the number
of items is restricted, the total size in bytes of the items in a comboxbox
is limited only by available memory.*/
class VComboBox : public VWindow
{
public:
	/** Default constructor initializes RTTI information in base class.*/
	VComboBox(VRTTI_TYPE nRTTI = VWCL_RTTI_COMBOBOX)
		: VWindow(nRTTI)
		{;}

	/** Same as AddString(), but used when adding items to an owner-drawn
	combobox that does not have the CBS_HASSTRINGS style and where the item
	data can be represented numerically.*/
	VLONG		AddItem(VLONG nValue) const
	{
		/* Verify correct style for using this function.*/
		VASSERT(IsOwnerDrawn())
		VASSERT(!(Style() & CBS_HASSTRINGS))

		/* Add to combobox.*/
		return AddString((VSTRING_CONST)nValue);
	}

	/** Same as AddString(), but used when adding items to an owner-drawn
	combobox that does not have the CBS_HASSTRINGS style and where the item
	data is a pointer to an unknown type.*/
	VLONG		AddItem(VVOIDPTR_CONST pVoid) const
		{ return AddItem((VLONG)pVoid); }

	/** Add a string to the listbox portion of a combobox. If the combobox
	does not have the CBS_SORT style, the string is added to the end of the
	list. Otherwise, the string is inserted into the list, and the list is
	sorted. The return value is the zero-based index to the string in the
	listbox portion of the combobox. If an error occurs, the return value is
	CB_ERR. If insufficient space is available to store the new string, it
	is CB_ERRSPACE. If you create an owner-drawn combobox with the CBS_SORT
	style, but without the CBS_HASSTRINGS style, the WM_COMPAREITEM message
	is sent one or more times to the owner of the combobox so the new item
	can be properly placed in the list. To insert a string at a specific
	location within the list, use one of the Insertxxx() functions. pszString
	is a pointer to the null-terminated string to be added. If you create
	the combobox with an owner-drawn style, but without the CBS_HASSTRINGS
	style, the value of the pszString parameter is stored as item data
	rather than the string it would otherwise point to. The item data can be
	retrieved or reset with the ItemData() functions. If this is the case,
	the AddItem() function makes more sense to use and avoids the typecast.*/
	VLONG		AddString(VSTRING_CONST pszString) const
		{ return VWINDOW_SM2(CB_ADDSTRING, 0, pszString); }

	/** Copy the current selection, if any, in the edit control to the
	clipboard in CF_TEXT format. This only works for comboboxes that have
	an associated edit control.*/
	void		Copy() const
		{ VWINDOW_SM0(WM_COPY); }

	/** Cut the current selection, if any, in the edit control and copy the
	deleted text to the clipboard in CF_TEXT format. This only works for
	comboboxes that have an associated edit control.*/
	void		Cut() const
		{ VWINDOW_SM0(WM_CUT); }

	/** Create a new combobox control. nStyle is a combination of WS_xxx
	window style bits or'd with these combobox specific bits:

	CBS_AUTOHSCROLL - Automatically scrolls the text in the edit control
	to the right when the user types a character at the end of the line. If
	this style is not set, only text that fits within the rectangular
	boundary is allowed.

	CBS_DROPDOWN - Similar to CBS_SIMPLE, except that the listbox is not
	displayed unless the user selects an icon next to the edit control.

	CBS_DROPDOWNLIST - Similar to CBS_DROPDOWN, except that the edit control
	is replaced by a static text item that displays the current selection in
	the listbox.

	CBS_HASSTRINGS - An owner-draw combobox contains items consisting of
	strings. The combobox maintains the memory and pointers for the strings
	so the application can use the GetText() member function to retrieve
	the text for a particular item.

	CBS_OEMCONVERT - Text entered in the combobox edit control is converted
	from the ANSI character set to the OEM character set and then back to
	ANSI. This ensures proper character conversion when the application
	calls the AnsiToOem() Windows function to convert an ANSI string in the
	combobox to OEM characters. This style is most useful for comboboxes
	that contain filenames and applies only to comboboxes created with the
	CBS_SIMPLE or CBS_DROPDOWN styles.

	CBS_OWNERDRAWFIXED - The owner of the listbox is responsible for drawing
	its contents and the items in the listbox are all the same height.

	CBS_OWNERDRAWVARIABLE - The owner of the listbox is responsible for
	drawing its contents and the items in the listbox are variable in
	height.

	CBS_SIMPLE - The listbox is displayed at all times. The current
	selection in the listbox is displayed in the edit control.

	CBS_SORT - Automatically sorts strings entered into the listbox.

	CBS_DISABLENOSCROLL - The listbox shows a disabled vertical scroll bar
	when the listbox does not contain enough items to scroll. Without this
	style, the scroll bar is hidden when the listbox does not contain
	enough items.

	CBS_NOINTEGRALHEIGHT - Specifies that the size of the combobox is
	exactly the size specified by the application when it created the
	combobox. Normally, Windows sizes a combobox so that the combobox does
	not display partial items.*/
	VBOOL		Create(	VWindow const&	windowParent,
						VRect const&	rect,
						VUINT			nID,
						VUINT			nStyle =
										WS_BORDER |
										WS_TABSTOP |
										WS_VISIBLE |
										CBS_AUTOHSCROLL |
										CBS_DROPDOWN |
										CBS_SORT |
										CBS_NOINTEGRALHEIGHT)
	{
		VASSERT(windowParent.GetSafeWindow())

		return VWindow::Create(	windowParent,
								rect,
								WS_CHILD | nStyle,
								VTEXT("COMBOBOX"),
								NULL,
								nID);
	}

	/** Remove all items from the listbox and edit control portions of the
	combobox. If you create the combobox with an owner-drawn style, but
	without the CBS_HASSTRINGS style, the owner of the combobox receives a
	WM_DELETEITEM message for each item in the combobox.*/
	VLONG		DeleteAllItems() const
		{ return VWINDOW_SM0(CB_RESETCONTENT); }

	/** Delete an item in the listbox portion of the combobox at nIndex. The
	return value is a count of the items remaining in the list. If the
	index parameter specifies an index greater than the number of items in
	the list, the return value is CB_ERR. If you create the combobox with
	an owner-drawn style, but without the CBS_HASSTRINGS style, the system
	sends a WM_DELETEITEM message to the owner of the combobox so the
	application can free any additional data associated with the item.*/
	VLONG		DeleteIndex(VUINT nIndex = VSELECTED_COMBO_BOX_ITEM) const
	{
		return	(IndexOrSelection(nIndex) != VSELECTED_COMBO_BOX_ITEM)
				? VWINDOW_SM1(CB_DELETESTRING, nIndex)
				: CB_ERR;
	}

	/** Add a list of filenames to the listbox portion of the combobox.
	nAttributes specifies the attributes of the files to be added to the
	listbox. It can be any combination of the following values:

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
	the filename to add to the list. If the filename contains any wildcards
	(for example, *.*), all files that match and have the attributes
	specified by the nAttributes parameter are added to the list. The return
	value is the zero-based index of the last filename added to the list. If
	an error occurs, the return value is CB_ERR. If insufficient space is
	available to store the new strings, it is CB_ERRSPACE.*/
	VLONG		Dir(	VUINT			nAttributes,
						VSTRING_CONST	pszFileSpec) const
		{ return VWINDOW_SM2(CB_DIR, nAttributes, pszFileSpec); }

	/** Show or hide the listbox portion of the combobox which has the
	CBS_DROPDOWN or CBS_DROPDOWNLIST style. This function has no effect on
	a combobox created with the CBS_SIMPLE style.*/
	void		DropDown(VBOOL bDropDown = VTRUE) const
		{ VWINDOW_SM1(CB_SHOWDROPDOWN, bDropDown); }

#if(WINVER >= 0x0400)
	/** Retrieve the minimum allowable width, in pixels, of the listbox
	portion of the combobox with the CBS_DROPDOWN or CBS_DROPDOWNLIST style.
	If the function succeeds, the return value is the width, in pixels. If
	the function fails, the return value is CB_ERR. By default, the minimum
	allowable width of the drop-down listbox is 0. The width of the listbox
	is either the minimum allowable width or the combobox width, whichever
	is larger.*/
	VLONG		DroppedWidth() const
		{ return VWINDOW_SM0(CB_GETDROPPEDWIDTH); }

	/** Set the maximum allowable width, in pixels, of the listbox portion of
	the combobox with the CBS_DROPDOWN or CBS_DROPDOWNLIST style. nWidth
	specifies the width of the listbox, in pixels. If the function is
	successful, the return value is the new width of the listbox. If the
	method fails, the return value is CB_ERR. By default, the minimum
	allowable width of the dropdown listbox is 0. The width of the listbox
	is either the minimum allowable width or the combobox width, whichever
	is larger.*/
	VLONG		DroppedWidth(VUINT nWidth) const
		{ return VWINDOW_SM1(CB_SETDROPPEDWIDTH, nWidth); }
#endif

	/** Get the starting and ending character positions of the current
	selection in the edit control portion of the combobox. pnStart is a
	pointer to a 32-bit value that receives the starting position of the
	selection. This parameter can NULL. pnEnd is a pointer to a 32-bit value
	that receives the ending position of the selection. This parameter can
	be NULL. The return value is a zero-based 32-bit value with the starting
	position of the selection in the low-order word and with the ending
	position of the first character after the last selected character in
	the high-order word.*/
	VLONG		EditSelection(	VUINT* pnStart,
								VUINT* pnEnd) const
		{ return VWINDOW_SM2(CB_GETEDITSEL, pnStart, pnEnd); }

	/** Select characters in the edit control portion of the combobox.
	nStartChar specifies the starting position. If this parameter is set to
	-1, the selection, if any, is removed. nEndChar specifies the ending
	position. If this parameter is set to -1, all text from the starting
	position to the last character in the edit control is selected. The
	positions are zero-based. The first character of the edit control is in
	the zero position. The first character after the last selected character
	is in the ending position. For example, to select the first four
	characters of the edit control, use a starting position of 0 and an
	ending position of 4. If the function succeeds, the return value is
	VTRUE. If called on a combobox with the CBS_DROPDOWNLIST style, it is
	CB_ERR.*/
	VLONG		EditSelection(	VINT nStartChar,
								VINT nEndChar) const
		{ return VWINDOW_SM3(CB_SETEDITSEL, 0, nStartChar, nEndChar); }

	/** Determine whether a combobox has the default user interface, or the
	extended user interface. If the combobox has the extended user
	interface, the return value is VTRUE, otherwise it is VFALSE. By
	default, the F4 key opens or closes the list and the down arrow changes
	the current selection. In a combobox with the extended user interface,
	the F4 key is disabled and pressing the down arrow key opens the
	drop-down list.*/
	VBOOL		ExtendedUI() const
		{ return VWINDOW_SMB0(CB_GETEXTENDEDUI); }

	/** Select either the default user interface or the extended user
	interface for a combobox that has the CBS_DROPDOWN or CBS_DROPDOWNLIST
	style. If the operation succeeds, the return value is VTRUE. If an error
	occurs, it is VFALSE. By default, the F4 key opens or closes the list
	and the down arrow changes the current selection. In the extended user
	interface, the F4 key is disabled and the down arrow key opens the
	drop-down list.*/
	VBOOL		ExtendedUI(VBOOL bExtended = VTRUE) const
	{
		return	(VWINDOW_SM1(CB_SETEXTENDEDUI, bExtended) == CB_OKAY)
				? VTRUE : VFALSE;
	}

	/** Same as FindString() but used when searching an owner-drawn combobox
	without the CBS_HASSTRINGS style for a value added to the combobox with
	AddItem() or InsertItem().*/
	VLONG		FindItem(	VLONG nValue,
							VLONG nStartIndex = -1) const
	{
		/* Verify correct style for using this function.*/
		VASSERT(IsOwnerDrawn())
		VASSERT(!(Style() & CBS_HASSTRINGS))

		/* Find it.*/
		return FindString((VSTRING_CONST)nValue, nStartIndex);
	}

	/** Same as FindString() but used when searching an owner-drawn combobox
	without the CBS_HASSTRINGS style for a pointer value added to the
	combobox with AddItem() or InsertItem().*/
	VLONG		FindItem(	VVOIDPTR_CONST	pItem,
							VLONG			nStartIndex = -1) const
		{ return FindItem((VLONG)pItem, nStartIndex); }

	/** Search the listbox portion of the combobox for an item beginning
	with the characters in a specified pszFind string. The return value is
	the zero-based index of the matching item. If the search is unsuccessful,
	it is CB_ERR. nStartIndex specifies the zero-based index of the item
	preceding the first item to be searched. When the search reaches the
	bottom of the listbox, it continues from the top of the listbox back to
	the item specified by the pszFind is a pointer to a null-terminated
	string to search for. This string can contain a complete filename,
	including the extension. The search is not case sensitive, so this
	string can contain any combination of uppercase and lowercase letters.
	If nStartIndex is -1, the entire listbox is searched from the beginning.
	If you create the combobox with an owner-drawn style but without the
	CBS_HASSTRINGS style, what the CB_FINDSTRING message does depends on
	whether your application uses the CBS_SORT style. If you use the
	CBS_SORT style, WM_COMPAREITEM messages are sent to the owner of the
	combobox to determine which item matches the specified string. If you do
	not use the CBS_SORT style, the CB_FINDSTRING message searches for a
	list item that matches the value of the pszFind parameter.*/
	VLONG		FindString(	VSTRING_CONST	pszFind,
							VLONG			nStartIndex = -1) const
		{ return VWINDOW_SM2(CB_FINDSTRING, nStartIndex, pszFind); }

	/** Find the first listbox string in a combobox that matches the string
	specified in the pszFind parameter. The return value is the zero-based
	index of the matching item. If the search is unsuccessful, it is CB_ERR.
	nStartIndex specifies the zero-based index of the item preceding the
	first item to be searched. When the search reaches the bottom of the
	listbox, it continues from the top of the listbox back to the item
	specified by the nStartIndex parameter. If nStartIndex is -1, the
	entire listbox is searched from the beginning. pszFind is a pointer to a
	null-terminated string to search for. This string can contain a
	complete filename, including the extension. The search is not case
	sensitive, so this string can contain any combination of uppercase and
	lowercase letters. If you create the combobox with an owner-drawn style
	but without the CBS_HASSTRINGS style, what the FindStringExact()
	function does depends on whether your application uses the CBS_SORT
	style. If you use the CBS_SORT style, WM_COMPAREITEM messages are sent
	to the owner of the combobox to determine which item matches the
	specified string. If you do not use the CBS_SORT style, the function
	searches for a list item that matches the value of the pszFind
	parameter.*/
	VLONG		FindStringExact(	VSTRING_CONST	pszFind,
									VLONG			nStartIndex = -1) const
		{ return VWINDOW_SM2(CB_FINDSTRINGEXACT, nStartIndex, pszFind); }

	/** Retrieve the number of items in the listbox portion of the combobox.
	The return value is the number of items in the listbox. If an error
	occurs, it is CB_ERR. The index is zero-based, so the returned count is
	one greater than the index value of the last item.*/
	VLONG		GetCount() const
		{ return VWINDOW_SM0(CB_GETCOUNT); }

	/** Retrieve the screen coordinates of the drop-down listbox portion of
	the combobox into a VRect object. If the function succeeds, the return
	value is VTRUE. If the functions fails, the return value is VFALSE.*/
	VBOOL		GetDroppedControlRect(VRect& rect) const
	{
		return VWINDOW_SMB2(CB_GETDROPPEDCONTROLRECT, 0, rect.GetPointer());
	}

	/** Retrieve a string from the listbox portion of the combobox. nIndex
	specifies the zero-based index of the string to retrieve. strBuffer is
	the string object that receives the string. The return value is the
	internal string pointer of strBuffer, or NULL on failure. It is not
	valid to call this function with a combobox created as an owner-drawn
	style but without the CBS_HASSTRINGS style. This is valid to do for
	Windows, but use of the ItemData() functions is required by VWCL.
	strBuffer will not be modified on failure.*/
	VSTRING		GetIndexText(	VString&	strBuffer,
								VUINT		nIndex =
											VSELECTED_COMBO_BOX_ITEM) const
	{
		if ( IndexOrSelection(nIndex) != VSELECTED_COMBO_BOX_ITEM )
		{
			VLONG nLength = GetIndexTextLength(nIndex);

			/* Allocate memory for item text and get it.*/
			if ( nLength > 0 && strBuffer.String(NULL, nLength + 1) )
			{
				VWINDOW_SM2(CB_GETLBTEXT, nIndex, strBuffer.String());
				strBuffer.UpdateLength(nLength);

				return strBuffer;
			}
		}

		return NULL;
	}

	/** Retrieve the length, in characters, of a string in the list of a
	combobox. nIndex specifies the zero-based index of the string. The
	return value is the length of the string, in characters, excluding the
	terminating null character. Under certain conditions, this value may
	actually be greater than the length of the text. If the nIndex parameter
	does not specify a valid index, the return value is CB_ERR. Under
	certain conditions, the return value is larger than the actual length
	of the text. This occurs with certain mixtures of ANSI and Unicode, and
	is due to the operating system allowing for the possible existence of
	double-byte character set (DBCS) characters within the text. The return
	value, however, will always be at least as large as the actual length
	of the text, so you can always use it to guide buffer allocation. This
	behavior can occur when an application uses both ANSI functions and
	common dialogs, which use Unicode.*/
	VLONG		GetIndexTextLength(VUINT	nIndex =
											VSELECTED_COMBO_BOX_ITEM) const
	{
		return	(IndexOrSelection(nIndex) != VSELECTED_COMBO_BOX_ITEM)
				? VWINDOW_SM1(CB_GETLBTEXTLEN, nIndex)
				: CB_ERR;
	}

	/** Retrieve the index of the currently selected item, if any, in the
	listbox of a combobox. The return value is the zero-based index of the
	currently selected item. If no item is selected, it is CB_ERR.*/
	VLONG		GetSelection() const
		{ return VWINDOW_SM0(CB_GETCURSEL); }

#if(WINVER >= 0x0400)
	/** Retrieve from a combobox the width, in pixels, by which the listbox
	can be scrolled horizontally (the scrollable width). This is applicable
	only if the listbox has a horizontal scroll bar. The return value is
	the scrollable width, in pixels.*/
	VLONG		HorizontalExtent() const
		{ return VWINDOW_SM0(CB_GETHORIZONTALEXTENT); }

	/** Set the width, in pixels, by which a listbox can be scrolled
	horizontally (the scrollable width). If the width of the listbox is
	smaller than this value, the horizontal scroll bar horizontally scrolls
	items in the listbox. If the width of the listbox is equal to or
	greater than this value, the horizontal scroll bar is hidden or, if
	the combobox has the CBS_DISABLENOSCROLL style, disabled.*/
	void		HorizontalExtent(VLONG nExtent) const
		{ VWINDOW_SM1(CB_SETHORIZONTALEXTENT, nExtent); }
#endif

	/** Determine the height of list items or the selection field in a
	combobox. nIndex specifies the combobox index whose height is to be
	retrieved. This parameter must be -1 to retrieve the height of the
	selection field. It must be zero to retrieve the height of list items,
	unless the combobox has the CBS_OWNERDRAWVARIABLE style. In that case,
	the nIndex parameter is the zero-based index of a specific list item.
	The return value is the height, in pixels, of the list items in a
	combobox. If the combobox has the CBS_OWNERDRAWVARIABLE style, it is
	the height of the item specified by the nIndex parameter. If nIndex is
	-1, the return value is the height of the edit control (or static-text)
	portion of the combobox. If an error occurs, the return value is
	CB_ERR.*/
	VLONG		IndexHeight(VUINT nIndex = VSELECTED_COMBO_BOX_ITEM) const
	{
		return	(IndexOrSelection(nIndex) != VSELECTED_COMBO_BOX_ITEM)
				? VWINDOW_SM1(CB_GETITEMHEIGHT, nIndex)
				: CB_ERR;
	}

	/** Set the height of list items or the selection field in a combobox. If
	the index or height is invalid, the return value is CB_ERR. The
	selection field height in a combobox is set independently of the height
	of the list items. An application must ensure that the height of the
	selection field is not smaller than the height of a particular list
	item. nIndex specifies the component of the combobox for which to set
	the height. This parameter must be -1 to set the height of the
	selection field. It must be zero to set the height of list items,
	unless the combobox has the CBS_OWNERDRAWVARIABLE style. In that case,
	the index parameter is the zero-based index of a specific list item.
	nHeight specifies the height, in pixels, of the combobox component
	identified by nIndex.*/
	VLONG		IndexHeight(	VUINT nHeight,
								VUINT nIndex = VSELECTED_COMBO_BOX_ITEM) const
	{
		return	(IndexOrSelection(nIndex) != VSELECTED_COMBO_BOX_ITEM)
				? VWINDOW_SM2(CB_SETITEMHEIGHT, nIndex, nHeight)
				: CB_ERR;
	}

	/** Set nIndex to the selected item in a single selection combobox or
	first selected item in a multiple selection combobox if nIndex is
	VSELECTED_COMBO_BOX_ITEM. If nothing is selected, nIndex will be set to
	VSELECTED_COMBO_BOX_ITEM to indicate error. nIndex will be returned as
	well.*/
	VUINT		IndexOrSelection(VUINT& nIndex) const
	{
		if ( nIndex == VSELECTED_COMBO_BOX_ITEM )
			nIndex = GetSelection();

		return nIndex;
	}

#if(WINVER >= 0x0400)
	/** An application can call this to allocate memory before adding a large
	number of items to the listbox portion of the combobox. nNumItems
	specifies the number of items to add. nStringBytes specifies the amount
	of memory to allocate for item strings, in bytes. If the function is
	successful, the return value is the total number of items for which
	memory has been pre-allocated, that is, the total number of items added
	by all successful InitStorage() functions. If the function fails, the
	return value is CB_ERRSPACE. For Windows NT version 4.0, this method
	does not allocate the specified amount of memory, however it always
	returns the value specified in the nNumItems parameter. For Windows NT
	version 5.0, the function allocates memory and returns the success and
	error values described above. The InitStorage() function helps speed up
	the initialization of comboboxes that have a large number of items
	(over 100). It reserves the specified amount of memory so that
	subsequent Addxxx(), Insertxxx(), and Dir() functions take the shortest
	possible time. You can use estimates for the nNumItems and nStringBytes
	parameters. If you overestimate, the extra memory is allocated, if you
	underestimate, the normal allocation is used for items that exceed the
	requested amount.*/
	VLONG		InitStorage(	VUINT nNumItems,
								VUINT nStringBytes) const
		{ return VWINDOW_SM2(CB_INITSTORAGE, nNumItems, nStringBytes); }
#endif

	/** Same as InsertString(), but used when adding items to an owner-drawn
	combobox that does not have the CBS_HASSTRINGS style and where the item
	data is a numeric value.*/
	VLONG		InsertItem(	VLONG nValue,
							VUINT nIndex = VSELECTED_COMBO_BOX_ITEM) const
	{
		/* Verify correct style for using this function.*/
		VASSERT(IsOwnerDrawn())
		VASSERT(!(Style() & CBS_HASSTRINGS))

		return InsertString((VSTRING_CONST)nValue, nIndex);
	}

	/** Same as InsertString(), but used when adding items to an owner-drawn
	combobox that does not have the CBS_HASSTRINGS style and where the item
	data is a pointer to an unknown type.*/
	VLONG		InsertItem(	VVOIDPTR_CONST	pItem,
							VUINT			nIndex =
											VSELECTED_COMBO_BOX_ITEM) const
		{ return InsertItem((VLONG)pItem, nIndex); }

	/** Insert a string into the listbox portion of the combobox at a
	specified index. Unlike the AddString() function, the InsertString()
	does not cause a list with the CBS_SORT style to be sorted. nIndex
	specifies the zero-based index of the position at which to insert the
	string. If this parameter is -1, the string is added to the end of the
	list. pszString is a pointer to a null-terminated string to be inserted.
	If you create the combobox with an owner-drawn style but without the
	CBS_HASSTRINGS style, the value of the pszString parameter is stored
	rather than the string it would otherwise point to. The return value is
	the index of the position at which the string was inserted. If an error
	occurs, the return value is CB_ERR. If there is insufficient space
	available to store the new string, it is CB_ERRSPACE. pszString is a
	pointer to the null-terminated string to be inserted. If you create the
	combobox with an owner-drawn style but without the CBS_HASSTRINGS style,
	the value of the pszString parameter is stored rather than the string it
	would otherwise point to. The item data can be retrieved or modified
	with ItemData(). If this is the case, the InsertItem() function makes
	more sense to use and avoids the typecast.*/
	VLONG		InsertString(	VSTRING_CONST pszString,
								VUINT nIndex = VSELECTED_COMBO_BOX_ITEM) const
	{
		return	(IndexOrSelection(nIndex) != VSELECTED_COMBO_BOX_ITEM)
				? VWINDOW_SM2(CB_INSERTSTRING, nIndex, pszString)
				: CB_ERR;
	}

	/** Determine if the listbox portion of the combobox is dropped down. If
	the listbox is visible, the return value is VTRUE, otherwise it is
	VFALSE.*/
	VBOOL		IsDropped() const
		{ return VWINDOW_SMB0(CB_GETDROPPEDSTATE); }

	/** Returns VTRUE if the combobox is owner-drawn, VFALSE if not.*/
	VBOOL		IsOwnerDrawn() const
	{
		return(	(Style() & CBS_OWNERDRAWFIXED) ||
				(Style() & CBS_OWNERDRAWVARIABLE))
				? VTRUE : VFALSE;
	}

	/** Retrieve the application supplied 32-bit value associated with the
	specified item in the combobox. nIndex specifies the zero-based index
	of the item. The return value is the 32-bit value associated with the
	item. If an error occurs, it is CB_ERR. If the item is in an owner-drawn
	combobox and was created without the CBS_HASSTRINGS style, this 32-bit
	value was set when added to the list. Otherwise, it is the value set in
	a previous call to ItemData().*/
	VLONG		ItemData(VUINT nIndex = VSELECTED_COMBO_BOX_ITEM) const
	{
		return	(IndexOrSelection(nIndex) != VSELECTED_COMBO_BOX_ITEM)
				? VWINDOW_SM1(CB_GETITEMDATA, nIndex)
				: CB_ERR;
	}

	/** Set the 32-bit value associated with the specified item in a
	combobox. nIndex specifies the item's zero-based index. nData specifies
	the new value to be associated with the item. If an error occurs, the
	return value is CB_ERR. If the specified item is in an owner-drawn
	combobox created without the CBS_HASSTRINGS style, this message
	replaces the 32-bit value set in the Addxxx() or Insertxxx() function
	that added the item to the combobox.*/
	VLONG		ItemData(	VLONG nData,
							VUINT nIndex = VSELECTED_COMBO_BOX_ITEM) const
	{
		return	(IndexOrSelection(nIndex) != VSELECTED_COMBO_BOX_ITEM)
				? VWINDOW_SM2(CB_SETITEMDATA, nIndex, nData)
				: CB_ERR;
	}

	/** Same as above, but avoids the typecast if setting item data using
	a pointer.*/
	VLONG		ItemData(	VUINT		nIndex,
							VVOIDPTR	pData) const
		{ return ItemData(nIndex, (VLONG)pData); }

	/** Limit the length of the text the user may type into the edit control
	portion of the combobox. nMaxChars specifies the maximum number of
	characters the user can enter. If this parameter is zero (default), the
	text length is set to 0x7FFFFFFE (2,147,483,646) characters. If the
	combobox does not have the CBS_AUTOHSCROLL style, setting the text
	limit to be larger than the size of the edit control has no effect.
	This function limits only the text the user can enter. It has no effect
	on any text already in the edit control when the method is called, nor
	does it affect the length of the text copied to the edit control when
	a string in the listbox is selected. The default limit to the text a
	user can enter in the edit control is 30,000 characters.*/
	VLONG		LimitText(VUINT nMaxChars = 0) const
		{ return VWINDOW_SM1(CB_LIMITTEXT, nMaxChars); }

	/** Retrieve the current locale of the combobox. The locale is used to
	determine the correct sorting order of displayed text for comboboxes
	with the CBS_SORT style and text added by using the AddString()
	function. The return value is a 32-bit value that specifies the current
	locale of the combobox. The high word contains the country code and the
	low-order word contains the language identifier. The language identifier
	is made up of a sublanguage identifier and a primary language identifier.
	The PRIMARYLANGID macro obtains the primary language identifier and the
	SUBLANGID macro obtains the sublanguage identifier.*/
	VLONG		Locale() const
		{ return VWINDOW_SM0(CB_GETLOCALE); }

	/** Set the current locale of the combobox. If the combobox has the
	CBS_SORT style and strings are added using CB_ADDSTRING, the locale of
	a combobox affects how list items are sorted. The return value is the
	previous locale identifier. If wParam specifies a locale not installed
	on the system, the return value is CB_ERR and the current combobox
	locale is not changed. Use the MAKELCID macro to construct a locale
	identifier and the MAKELANGID macro to construct a language identifier.
	The language identifier is made up of a primary language identifier and
	a sublanguage identifier.*/
	VLONG		Locale(VINT nLocalID) const
		{ return VWINDOW_SM1(CB_SETLOCALE, nLocalID); }

	/** Copy the current content of the clipboard to the edit control portion
	of the combobox at the current caret position. Data is inserted only if
	the clipboard contains data in CF_TEXT format and if the combobox has
	an associated edit control.*/
	void		Paste() const
		{ VWINDOW_SM0(WM_PASTE); }

	/** Select a string in the list of a combobox. If necessary, the list
	scrolls the string into view. The text in the edit control of the
	combobox changes to reflect the new selection, and any previous
	selection in the list is removed. If the function is successful, the
	return value is the index of the item selected. If index is greater
	than the number of items in the list, or if index is set to -1, the
	return value is CB_ERR and the selection is cleared.*/
	VLONG		Select(VLONG nIndex) const
		{ return VWINDOW_SM1(CB_SETCURSEL, nIndex); }

	/** Same as SelectString(), but used when selecting items in an
	owner-drawn combobox that does not have the CBS_HASSTRINGS style and
	where the item data is a numeric value.*/
	VLONG		SelectItem(	VLONG nValue,
							VLONG nStartIndex = -1) const
	{
		/* Verify correct style for using this function.*/
		VASSERT(IsOwnerDrawn())
		VASSERT(!(Style() & CBS_HASSTRINGS))

		return SelectString((VSTRING_CONST)nValue, nStartIndex);
	}

	/** Same as SelectString(), but used when selecting items in an
	owner-drawn combobox that does not have the CBS_HASSTRINGS style and
	where the item data is a pointer to an unknown type.*/
	VLONG		SelectItem(	VVOIDPTR_CONST	pItem,
							VLONG			nStartIndex = -1) const
		{ return SelectItem((VLONG)pItem, nStartIndex); }

	/** Search the list of a combobox for an item that begins with the
	characters in a specified string. nStartIndex specifies the zero-based
	index of the item preceding the first item to be searched. When the
	search reaches the bottom of the list, it continues from the top of the
	list back to the item specified by the parameter. If nIndexStart is -1,
	the entire list is searched from the beginning. pszSelect is a pointer
	to a null-terminated string that contains the prefix to search for. The
	search is not case sensitive, so this string can contain any combination
	of uppercase and lowercase letters. If a matching item is found, it is
	selected and copied to the edit control. If the string is found, the
	return value is the index of the selected item. If the search is
	unsuccessful, the return value is CB_ERR and the current selection is
	not changed. If you create the combobox with an owner-drawn style but
	without the CBS_HASSTRINGS style, what the SelectString() function does
	depends on whether you use the CBS_SORT style. If the CBS_SORT style is
	used, the system sends WM_COMPAREITEM messages to the owner of the
	combobox to determine which item matches the specified string. If you do
	not use the CBS_SORT style, SelectString() attempts to match the item
	data value against the value of the pszSelect parameter.*/
	VLONG		SelectString(	VSTRING_CONST	pszSelect,
								VLONG			nStartIndex = -1) const
		{ return VWINDOW_SM2(CB_SELECTSTRING, nStartIndex, pszSelect); }

	/** Modifies the string for a specified row in the combobox. nIndex
	specifies the zero-based index of the string to modify. pszString is the
	string that contains the new value. The return value is the index of the
	row in the listbox, or -1 on failure. It is not valid to call this
	function with a combobox created as an owner-drawn style but without the
	CBS_HASSTRINGS style. This is valid to do for Windows, but use of the
	ItemData() functions is required by VWCL.*/
	VLONG		SetIndexText(	VSTRING_CONST	pszString,
								VUINT			nIndex =
												VSELECTED_COMBO_BOX_ITEM) const
	{
		#ifdef VWCL_DEBUG
			/* If owner-drawn, it must have the CBS_HASSTRINGS style set!*/
			if ( (IsOwnerDrawn()) && !(Style() & CBS_HASSTRINGS) )
			{
				VASSERT(VFALSE)
			}
		#endif

		/* Make sure the item actually exists.*/
		if ( IndexOrSelection(nIndex) != VSELECTED_COMBO_BOX_ITEM )
		{
			/* Remember if its current or not.*/
			VLONG nCurSel =
				GetSelection();

			VBOOL bSelected =
				(nCurSel != CB_ERR && nCurSel == (VLONG)nIndex)
				? VTRUE
				: VFALSE;

			/* Save the existing item data value.*/
			VLONG nOldItemData = ItemData(nIndex);

			/* Remove the existing row from the box.*/
			DeleteIndex(nIndex);

			/* Create a new row to replace the one we just removed.*/
			VLONG nCurSlot = InsertString(pszString, nIndex);
			ItemData(nCurSlot, nOldItemData);

			/* Reset old selection.*/
			if ( bSelected )
				Select(nCurSlot);

			/* Return the index.*/
			return nCurSlot;
		}

		/* Return invalid row.*/
		return -1;
	}

#if(WINVER >= 0x0400)
	/** Retrieve the zero-based index of the first visible item in the
	listbox portion of a combobox. Initially the item with index 0 is at
	the top of the listbox, but if the listbox contents have been scrolled,
	another item may be at the top. If the function is successful, the
	return value is the index of the first visible item in the listbox of
	the combobox. If the function fails, the return value is CB_ERR.*/
	VLONG		TopIndex() const
		{ return VWINDOW_SM0(CB_GETTOPINDEX); }

	/** Ensure that a particular item is visible in the listbox of a
	combobox. The system scrolls the listbox contents so that either the
	specified item appears at the top of the listbox or the maximum scroll
	range has been reached. If the function is successful, the return value
	is zero. If the message fails, the return value is CB_ERR.*/
	VLONG		TopIndex(WPARAM nIndex) const
		{ return VWINDOW_SM1(CB_SETTOPINDEX, nIndex); }
#endif
};

#endif /* VCOMBOBOX*/
