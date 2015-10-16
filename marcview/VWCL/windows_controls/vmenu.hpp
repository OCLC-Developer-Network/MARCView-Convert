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

Primary Author of this source code file:	Karl Edwall
											(edwallk@bigpond.com)
Other Author(s) of this source code file:	Todd Osborne
											(todd@vwcl.org)
											Jan G.P. Sijm
											(jsijm@murdock.demon.nl)

VWCL and all source code are copyright (c) 1996-2000 by The VWCL Alliance.*/

#ifndef VMENU
#define VMENU

#include "../vstandard.h"
#include "../gdi/vbitmap.hpp"
#include "../strings/vstring.hpp"

/** The VMenu class is an encapsulation of the Windows HMENU. It provides
member functions for creating, tracking, updating, and destroying a Windows
menu. In addition, it supports associating a VToolBar object with the menu.
As changes are made to the menu, they are also reflected in the toolbar.
Using this method allows this class to control the associated toolbar
control. The VToolBar header file (vtoolbar.hpp) must be included before
this file to get this support.*/
class VMenu
{
public:
	/** Default constructor initializes internal members. If the menu
	wrapped by this class should destroy the menu, either automatically
	during destruction, or via the Destroy() method, pass VTRUE for
	bDestroyMenu. Otherwise this class will simply wrap a HMENU and not
	destroy it.*/
	VMenu(	HMENU hMenu = NULL,
			VBOOL bDestroyMenu = VTRUE)
	{
		m_hMenu = NULL;
		Attach(hMenu, bDestroyMenu);
	}

	/** Construct object with existing VMenu object (copy constructor).
	This copy will not destroy the menu.*/
	VMenu(VMenu const& menu)
	{
		m_hMenu = menu.GetHandle();
		m_bDestroyMenu = VFALSE;
	}

	/** Virtual destructor destroys the menu (as needed).*/
	virtual ~VMenu()
		{ Destroy(); }

	/** Compare two VMenu instances for equality.*/
	VBOOL operator == (VMenu const& menu) const
		{ return (menu.GetHandle() == m_hMenu) ? VTRUE : VFALSE; }

	/** Compare two VMenu instances for inequality.*/
	VBOOL operator != (VMenu const& menu) const
		{ return (menu.GetHandle() == m_hMenu) ? VFALSE : VTRUE; }

	/** Assign an existing VMenu object to this object. It will not be
	destroyed by this object.*/
	void operator = (VMenu const& menu)
	{
		Attach(menu.GetHandle());
		m_bDestroyMenu = VFALSE;
	}

	/** Assign an existing HMENU object to this object. The HMENU will be
	destroyed by this object. It assumes ownership. If this class should
	not assume ownership of the menu, you should use the Attach() function
	with the bDestroyMenu flag set to VFALSE, so that this object does not
	destroy the menu.*/
	void operator = (HMENU hMenu)
	{
		Attach(hMenu);
		m_bDestroyMenu = VTRUE;
	}

	/** Appends a new item to the end of the specified menu bar, drop-down
	menu, submenu, or shortcut menu. You can use this function to specify
	the content, appearance, and behavior of the menu item. nFlags specifies
	flags to control the appearance and behavior of the new menu item. This
	parameter can be a combination of the following values:

	MF_BITMAP - Uses a bitmap as the menu item. The pszNewItem parameter
	contains the handle to the bitmap.

	MF_CHECKED - Places a check mark next to the menu item. If the
	application provides check-mark bitmaps, this flag displays the check
	mark bitmap next to the menu item.

	MF_DISABLED - Disables the menu item so it cannot be selected, but the
	flag does not gray it.

	MF_ENABLED - Enables the menu item so it can be selected, and restores
	it from its grayed state.

	MF_GRAYED - Disables the menu item and grays it so it cannot be
	selected.

	MF_MENUBARBREAK - Functions the same as the MF_MENUBREAK flag for a
	menu bar. For a drop-down menu, submenu, or shortcut menu, the new
	column is separated from the old column by a vertical line.

	MF_MENUBREAK - Places the item on a new line (for a menu bar) or in a
	new column (for a drop-down menu, submenu, or shortcut menu) without
	separating columns.

	MF_OWNERDRAW - Specifies that the item is an owner-drawn item. Before
	the menu is displayed for the first time, the window that owns the menu
	receives a WM_MEASUREITEM message to retrieve the width and height of
	the menu item. The WM_DRAWITEM message is then sent to the window
	procedure of the owner window whenever the appearance of the menu item
	must be updated.

	MF_POPUP - Specifies that the menu item opens a drop-down menu or
	submenu. The nIDNewItem parameter specifies the handle to the drop-down
	menu or submenu. This flag is used to add a menu name to a menu bar, or
	a menu item that opens a submenu to a drop-down menu, submenu, or
	shortcut menu.

	MF_SEPARATOR - Draws a horizontal dividing line. This flag is used only
	in a drop-down menu, submenu, or shortcut menu. The line cannot be
	grayed, disabled, or highlighted. The pszNewItem and nIDNewItem
	parameters are ignored.

	MF_STRING - Specifies that the menu item is a text string; the
	pszNewItem parameter points to the string.

	MF_UNCHECKED - Does not place a check mark next to the item (default).
	If the application supplies check-mark bitmaps, this flag displays the
	unchecked bitmap next to the menu item.

	The following groups of flags cannot be used together:

	MF_DISABLED, MF_ENABLED, and MF_GRAYED

	MF_BITMAP, MF_STRING, and MF_OWNERDRAW

	MF_MENUBARBREAK and MF_MENUBREAK

	MF_CHECKED and MF_UNCHECKED

	Windows CE: The following flags are not supported for the nFlags
	parameter:

	MF_BITMAP, MF_DISABLED, MFS_GRAYED. MF_GRAYED can be used in place of
	MF_DISABLED and MFS_GRAYED. Windows CE 1.0 does not support cascading
	menus. If you are using Windows CE 1.0, you cannot insert an MF_POPUP
	menu into another pop-up menu. In Windows CE 1.0, the following flags
	are not supported: MF_POPUP, MF_MENUBREAK, MF_MENUBARBREAK. In Windows
	CE versions 2.0 and later, these flags are supported. Cascading menus
	are also supported.

	nIDNewItem specifies either the identifier of the new menu item or, if
	the nFlags parameter is set to MF_POPUP, the handle to the drop-down
	menu or submenu. pszNewItem specifies the content of the new menu item.
	The interpretation of pszNewItem depends on whether the nFlags parameter
	includes the MF_BITMAP, MF_OWNERDRAW, or MF_STRING flag, as follows:

	MF_BITMAP - Contains a bitmap handle.

	MF_OWNERDRAW - Contains a 32-bit value supplied by the application that
	can be used to maintain additional data related to the menu item. The
	value is in the itemData member of the structure pointed to by the
	lparam parameter of the WM_MEASUREITEM or WM_DRAWITEM message sent when
	the menu is created or its appearance is updated.

	MF_STRING - Contains a pointer to a null-terminated string.*/
	VBOOL	AppendItem(	VUINT			nFlags,
						VUINT			nIDNewItem,
						VSTRING_CONST	pszNewItem) const
	{
		VASSERT(GetSafeHandle())

		return (AppendMenu(	m_hMenu,
							nFlags,
							nIDNewItem,
							pszNewItem))
							? VTRUE : VFALSE;
	}

	/** Appends a new bitmap item to the end of the specified menu bar,
	drop-down menu, submenu, or shortcut menu. You can use this function to
	specify the content, appearance, and behavior of the menu item. See
	AppendItem() for more information on parameters.*/
	VBOOL	AppendBitmapItem(	VUINT			nFlags,
								VUINT			nIDNewItem,
								VBitmap const&	bitmap) const
	{
		return AppendItem(	nFlags | MF_BITMAP,
							nIDNewItem,
							(VSTRING_CONST)bitmap.GetHandle());
	}

	/** Attached the supplied menu handle to the clas object. An existing
	menu must not already be attached. This class will assume ownership of
	the menu, and destroy it when the class object goes out of scope. If
	this is not desired, pass VFALSE for the bDestroyMenu parameter. hMenu
	can be NULL, which makes this function work like Detach(). Returns
	VTRUE if this class is wrapping a menu, VFALSE otherwise.*/
	VBOOL	Attach(	HMENU hMenu,
					VBOOL bDestroyMenu = VTRUE)
	{
		VASSERT(!m_hMenu)

		m_hMenu = hMenu;
		m_bDestroyMenu = bDestroyMenu;

		return (m_hMenu) ? VTRUE : VFALSE;
	}

	/** Sets the state of the specified menu item's check mark attribute to
	either checked or unchecked. The return value is the previous checked
	state, either MF_CHECKED or MF_UNCHECKED, or 0xffffffff on error.
	nIDCheckItem specifies the menu item whose check-mark attribute is to
	be set, as determined by the nCheck parameter. nCheck specifies flags
	that control the interpretation of the nIDCheckItem parameter and the
	state of the menu item's check-mark attribute. This parameter can be a
	combination of either MF_BYCOMMAND, or MF_BYPOSITION and MF_CHECKED or
	MF_UNCHECKED.

	MF_BYCOMMAND - Indicates that the nIDCheckItem parameter gives the
	identifier of the menu item. The MF_BYCOMMAND flag is the default, if
	neither the MF_BYCOMMAND nor MF_BYPOSITION flag is specified.

	MF_BYPOSITION -  Indicates that the nIDCheckItem parameter gives the
	zero-based relative position of the menu item.

	MF_CHECKED - Sets the check-mark attribute to the checked state.

	MF_UNCHECKED - Sets the check-mark attribute to the unchecked state.

	An item in a menu bar cannot have a check mark. The nIDCheckItem
	parameter identifies a item that opens a submenu or a command item. For
	a item that opens a submenu, the nIDCheckItem parameter must specify
	the position of the item. For a command item, the nIDCheckItem
	parameter can specify either the item's position or its identifier.
	This function will only update an associated toolbar if the
	nIDCheckItem is a menu item command ID, as determined by the nCheck
	parameter having the MF_BYCOMMAND bit set.*/
	VUINT	CheckItem(	VUINT nIDCheckItem,
						VUINT nCheck = MF_BYCOMMAND | MF_CHECKED) const
	{
		VASSERT(GetSafeHandle())

		#ifdef VTOOLBAR
			if ( m_pToolBar && !(nCheck & MF_BYPOSITION) )
				m_pToolBar->CheckButton(	nIDCheckItem,
											(nCheck & MF_CHECKED)
											? VTRUE : VFALSE);
		#endif

		return (VUINT)CheckMenuItem(m_hMenu, nIDCheckItem, nCheck);
	}

	/** Checks a specified menu item and makes it a radio item. At the same
	time, the function unchecks all other menu items in the associated
	group and clears the radio-item type flag for those items. nIDFirst is
	the identifier or position of the first menu item in the group. nIDLast
	is the identifier or position of the last menu item in the group.
	nIDCheck specifies the indentifier or position of the menu item to
	check. nFlags specifies the meaning of nIDFirst, nIDLast, and nIDCheck.
	If this parameter is MF_BYCOMMAND, the other parameters specify menu
	item identifiers. If it is MF_BYPOSITION, the other parameters specify
	the menu item positions. The function sets the MFT_RADIOCHECK type flag
	and the MFS_CHECKED state for the item specified by nIDCheck and, at
	the same time, clears both flags for all other items in the group. The
	checked item is displayed using a bullet bitmap instead of a check-mark
	bitmap. For more information about menu item type and state flags, see
	the MENUITEMINFO structure.*/
	VBOOL	CheckRadioItem(	VUINT nIDFirst,
							VUINT nIDLast,
							VUINT nIDItem,
							VUINT nFlags) const
	{
		VASSERT(GetSafeHandle())

		return CheckMenuRadioItem(	m_hMenu,
									nIDFirst,
									nIDLast,
									nIDItem,
									nFlags);
	}

	/** Creates a menu. The menu is initially empty, but it can be filled
	with menu items by using other methods.*/
	VBOOL	Create()
		{ return Attach(CreateMenu()); }

	/** Creates a drop-down menu, submenu, or shortcut menu. The menu is
	initially empty, but it can be filled with menu items by using other
	methods.*/
	VBOOL	CreatePopup()
		{ return Attach(CreatePopupMenu()); }

	/** Deletes an item from the specified menu. If the menu item opens a
	menu or submenu, this function destroys the handle to the menu or
	submenu and frees the memory used by the menu or submenu.
	nCommandOrPosition specifies the menu item to be deleted, as determined
	by the nFlags parameter. nFlags specifies how the nCommandOrPosition
	parameter is interpreted. This parameter must be one of the following
	values:

	MF_BYCOMMAND - Indicates that nCommandOrPosition gives the identifier
	of the menu item. The MF_BYCOMMAND flag is the default flag if neither
	the MF_BYCOMMAND nor MF_BYPOSITION flag is specified.

	MF_BYPOSITION - Indicates that nCommandOrPosition gives the zero-based
	relative position of the menu item.

	This function will only delete a button from an associated toolbar if
	the nCommandOrPosition is a menu item command ID, as determined by the
	nFlags parameter having the MF_BYCOMMAND bit set.*/
	VBOOL	DeleteItem(	VUINT nCommandOrPosition,
						VUINT nFlags = MF_BYCOMMAND) const
	{
		VASSERT(GetSafeHandle())

		#ifdef VTOOLBAR
			if ( m_pToolBar && !(nFlags & MF_BYPOSITION) )
				m_pToolBar->DeleteButton(
					m_pToolBar->CommandToIndex(nCommandOrPosition));
		#endif

		return (DeleteMenu(	m_hMenu,
							nCommandOrPosition, nFlags))
							? VTRUE : VFALSE;
	}

	/** Destroy an attached menu and remove the association of the
	destroyed menu from this object, as needed.*/
	VBOOL	Destroy()
	{
		if ( !m_hMenu )
			return VFALSE;

		/* Only destroy if we should.*/
		if ( m_bDestroyMenu )
			DestroyMenu(m_hMenu);

		/* Always set to NULL.*/
		m_hMenu = NULL;

		return VTRUE;
	}

	/** Detach a HMENU from this object so that it is not deleted when
	the object goes out of scope.*/
	void	Detach()
		{ m_hMenu = NULL; }

	/** Enables, disables, or grays the specified menu item. nIDEnableItem
	specifies the menu item to be enabled, disabled, or grayed, as
	determined by the nEnable parameter. This parameter specifies an item
	in a menu bar, menu, or submenu. nEnable specifies flags that control
	the interpretation of the nIDEnableItem parameter and indicate whether
	the menu item is enabled, disabled, or grayed. This parameter must be a
	combination of either MF_BYCOMMAND or MF_BYPOSITION and MF_ENABLED,
	MF_DISABLED, or MF_GRAYED.

	MF_BYCOMMAND - Indicates that nIDEnableItem gives the identifier of
	the menu item. If neither the MF_BYCOMMAND nor MF_BYPOSITION flag is
	specified, the MF_BYCOMMAND flag is the default flag.

	MF_BYPOSITION - Indicates that nIDEnableItem gives the zero-based
	relative position of the menu item.

	MF_DISABLED - Indicates that the menu item is disabled, but not grayed,
	so it cannot be selected.

	MF_ENABLED - Indicates that the menu item is enabled and restored
	from a grayed state so that it can be selected.

	MF_GRAYED - Indicates that the menu item is disabled and grayed so
	that it cannot be selected.

	The return value specifies the previous state of the menu item (it is
	either MF_DISABLED, MF_ENABLED, or MF_GRAYED). If the menu item does
	not exist, the return value is 0xffffffff. An application must use the
	MF_BYPOSITION flag to specify the correct menu handle. If the menu
	handle to the menu bar is specified, the top-level menu item (an item
	in the menu bar) is affected. To set the state of an item in a drop-down
	menu or submenu by position, an application must specify the handle to
	the drop-down menu or submenu. When an application specifies the
	MF_BYCOMMAND flag, the system checks all items that open submenus in
	the menu identified by the specified menu handle. Therefore, unless
	duplicate menu items are present, specifying the menu handle to the
	menu bar is sufficient. Windows CE does not support the MF_DISABLED
	flag for the nEnable parameter. Menu items cannot be disabled without
	being grayed. To disable a menu item use the MF_GRAYED flag. This
	function will only enable or disable a button from an associated
	toolbar if the nIDEnableItem is a menu item command ID, as determined
	by he nEnable parameter having the MF_BYCOMMAND bit set.*/
	VUINT	EnableItem(	VUINT nIDEnableItem,
						VUINT nEnable = MF_BYCOMMAND | MF_ENABLED) const
	{
		VASSERT(GetSafeHandle())

		#ifdef VTOOLBAR
			if ( m_pToolBar && !(nEnable & MF_BYPOSITION) )
				m_pToolBar->EnableButton(	nIDEnableItem,
											(nEnable & MF_DISABLED ||
											nEnable & MF_GRAYED)
											? VFALSE : VTRUE);
		#endif

		return (VUINT)EnableMenuItem(m_hMenu, nIDEnableItem, nEnable);
	}

	/** Retrieves the Help context identifier associated with the
	specified menu.*/
	VDWORD	GetContextHelpId() const
	{
		VASSERT(GetSafeHandle())
		return GetMenuContextHelpId(m_hMenu);
	}

	/** Return the default menu item on the specified menu. bByPos specifies
	whether to retrieve the menu item's identifier or its position. If this
	parameter is VFALSE, the identifier is returned. Otherwise, the
	position is returned. gmdiFlags specifies how the function searches for
	menu items. This parameter can be zero or more of the following values:

	GMDI_GOINTOPOPUPS - Specifies that if the default item is one that
	opens a submenu, the function is to search recursively in the
	corresponding submenu. If the submenu has no default item, the return
	value identifies the item that opens the submenu. By default, the
	function returns the first default item on the specified menu,
	regardless of whether it is an item that opens a submenu.

	GMDI_USEDISABLED - Specifies that the function is to return a default
	item, even if it is disabled. By default, the function skips disabled
	or grayed items.

	If the function succeeds, the return value is the identifier or
	position of the menu item. If the function fails, the return value
	is -1.*/
	VUINT	GetDefaultItem(	VUINT gmdiFlags = GMDI_USEDISABLED,
							VBOOL bByPos = VFALSE) const
	{
		VASSERT(GetSafeHandle())
		return GetMenuDefaultItem(m_hMenu, bByPos, gmdiFlags);
	}

	/** Return a reference to the internal boolean flag that determines
	if this object will destroy the attached menu.*/
	VBOOL&	GetDestroyFlag() const
		{ return (VBOOL&)m_bDestroyMenu; }

	/** Return a reference to the internal menu (HMENU) handle.*/
	HMENU&	GetHandle() const
		{ return (HMENU&)m_hMenu; }

	/** Determine the number of items in the specified menu, or -1 on
	failure.*/
	VUINT	GetItemCount() const
	{
		VASSERT(GetSafeHandle())
		return GetMenuItemCount(m_hMenu);
	}

	/** Retrieves the menu item identifier of a menu item located at the
	specified position in a menu. nPos specifies the zero-based relative
	position of the menu item whose identifier is to be retrieved. The
	return value specifies the identifier of the given menu item. If the
	menu item identifier is NULL or if the specified item opens a submenu,
	the return value is 0xffffffff.*/
	VUINT	GetItemID(VUINT nPos) const
	{
		VASSERT(GetSafeHandle())
		return GetMenuItemID(m_hMenu, nPos);
	}

	/** Retrieves information about a menu item. nIDItem specified the
	identifier or position of the menu item to get information about. The
	meaning of this parameter depends on the value of bByPos. bByPos
	specifies the meaning of nIDItem. If this parameter is VFALSE, nIDItem
	is a menu item identifier. Otherwise, it is a menu item position. mii
	is a reference to a MENUITEMINFO structure that specifies the
	information to retrieve and receives information about the menu item.
	The MIM_CHECKMARKS flag value is not supported for the fMask member
	of the MENUITEMINFO structure referenced to by the mii parameter of
	this function. This function will set the cbSize member of mmi for you.*/
	VBOOL	GetItemInfo(	VUINT			nIDItem,
							MENUITEMINFO&	mii,
							VBOOL			bByPos) const
	{
		VASSERT(GetSafeHandle())

		mii.cbSize = sizeof(mii);
		return GetMenuItemInfo(m_hMenu, nIDItem, bByPos, &mii);
	}

	/** Retrieves the menu flags associated with the specified menu item. If
	the menu item opens a submenu, this function also returns the number of
	items in the submenu. nID specifies the menu item for which the menu
	flags are to be retrieved, as determined by the nFlags parameter.
	nFlags specifies how the nID parameter is interpreted. This parameter
	can be one of the following values:

	MF_BYCOMMAND - Indicates that the nID parameter gives the identifier
	of the menu item. The MF_BYCOMMAND flag is the default if neither the
	MF_BYCOMMAND nor MF_BYPOSITION flag is specified.

	MF_BYPOSITION - Indicates that the nID parameter gives the zero-based
	relative position of the menu item. If the specified item does not
	exist, the return value is 0xffffffff. If the menu item opens a
	submenu, the low-order byte of the return value contains the menu flags
	associated with the item, and the high-order byte contains the number
	of items in the submenu opened by the item. Otherwise, the return value
	is a mask (Boolean OR) of the menu flags. Following are the menu flags
	associated with the menu item:

	MF_CHECKED - Places a check mark next to the item (for drop-down
	menus, submenus, and shortcut menus only).

	MF_DISABLED - Disables the item.

	MF_GRAYED - Disables and grays the item.

	MF_HILITE - Highlights the item.

	MF_MENUBARBREAK - Functions the same as the MF_MENUBREAK flag, except
	for drop-down menus, submenus, and shortcut menus, where the new column
	is separated from the old column by a vertical line.

	MF_MENUBREAK - Places the item on a new line (for menu bars) or in a
	new column (for drop-down menus, submenus, and shortcut menus) without
	separating columns.

	MF_SEPARATOR - Creates a horizontal dividing line (for drop-down menus,
	submenus, and shortcut menus only).*/
	VUINT	GetItemState(	VUINT nID,
							VUINT nFlags) const
	{
		VASSERT(GetSafeHandle())
		return GetMenuState(m_hMenu, nID, nFlags);
	}

	/** Copies the text string of the specified menu item into the specified
	buffer. nIDItem specifies the menu item to be read, as determined by the
	nFlag parameter. pszString is a pointer to the buffer that is to
	receive the null-terminated string. If pszString is NULL, the function
	returns the length of the menu string. nMaxCount specifies the maximum
	length, in characters, of the string to be copied. If the string is
	longer than the maximum specified in the nMaxCount parameter, the extra
	characters are truncated. If nMaxCount is 0, the function returns the
	length of the menu string. nFlag specifies how the nIDItem parameter is
	interpreted. This parameter must be one of the following values:

	MF_BYCOMMAND - Indicates that nIDItem gives the identifier of the menu
	item. If neither the MF_BYCOMMAND nor MF_BYPOSITION flag is specified,
	the MF_BYCOMMAND flag is the default flag.

	MF_BYPOSITION - Indicates that nIDItem gives the zero-based relative
	position of the menu item.

	If the function succeeds, the return value specifies the number of
	characters copied to the buffer, not including the terminating null
	character. If the function fails, the return value is zero. The
	nMaxCount parameter must be one larger than the number of characters in
	the text string to accommodate the terminating null character. If
	nMaxCount is 0, the function returns the length of the menu string. It
	is important to note that if a menu string has an & in it, which causes
	the next character to be underlined, this function will return the
	ampersand in the string. However, if testing for the length, Windows
	does not include this ampersand in the returned string length, so you
	must add 1 to the length returned.*/
	VUINT	GetItemString(	VUINT	nIDItem,
							VSTRING	pszString,
							VUINT	nMaxCount,
							VUINT	nFlag = MF_BYCOMMAND) const
	{
		VASSERT(GetSafeHandle())
		return GetMenuString(m_hMenu, nIDItem, pszString, nMaxCount, nFlag);
	}

	/** Same as above, but gets the menu string into a VString object.
	strBuffer will not normally be modified on failure.*/
	VUINT	GetItemString(	VUINT		nIDItem,
							VString&	strBuffer,
							VUINT		nFlag = MF_BYCOMMAND) const
	{
		/* Determine length.*/
		VUINT nLength = GetItemString(nIDItem, NULL, 0, nFlag);

		/* Add one character more for terminator, and one more in case
		this item has an & in it.*/
		if ( nLength && strBuffer.String(NULL, nLength + 2) )
		{
			if ( GetItemString(nIDItem, strBuffer, nLength + 1, nFlag) )
			{
				strBuffer.UpdateLength();
				return nLength;
			}

			strBuffer.Empty();
		}

		return 0;
	}

	/** Returns a valid handle to a HMENU or NULL if the menu is
	invalid (destroyed).*/
	HMENU	GetSafeHandle() const
		{ return (m_hMenu && IsMenu(m_hMenu)) ? m_hMenu : NULL; }

	/** Retrieves the handle to the drop-down menu or submenu activated
	by the specified menu item. nPos specifies the zero-based relative
	position in the given menu of an item that activates a drop-down menu
	or submenu. If the function succeeds, the return value is the handle
	to the drop-down menu or submenu activated by the menu item. If the
	menu item does not activate a drop-down menu or submenu, the return
	value is NULL.*/
	HMENU	GetSubMenu(VUINT nPos) const
	{
		VASSERT(GetSafeHandle())
		return ::GetSubMenu(m_hMenu, nPos);
	}

	/** Inserts a new menu item into a menu, moving other items down the
	menu. See AppendItem() for more information on parameters. nPos
	specifies the menu item before which the new menu item is to be
	inserted, as determined by the nFlags parameter.*/
	VBOOL	InsertItem(	VUINT			nPos,
						VUINT			nFlags,
						VUINT			nIDNewItem,
						VSTRING_CONST	pszNewItem) const
	{
		VASSERT(GetSafeHandle())
		return InsertMenu(m_hMenu, nPos, nFlags, nIDNewItem, pszNewItem);
	}

	/** Inserts a new bitmap menu item into a menu, moving other items down
	the menu. See AppendItem() for more information on parameters. nPos
	specifies the menu item before which the new menu item is to be
	inserted, as determined by the nFlags parameter.*/
	VBOOL	InsertBitmapItem(	VUINT			nPos,
								VUINT			nFlags,
								VUINT			nIDNewItem,
								VBitmap const&	bitmap) const
	{
		return InsertItem(	nPos,
							nFlags | MF_BITMAP,
							nIDNewItem,
							(VSTRING_CONST)bitmap.GetHandle());
	}

	/** Loads the specified menu from a menu resource and attaches it to
	this object.*/
	VBOOL	Load(	VSTRING_CONST	pszResourceName,
					HINSTANCE		hResource = NULL)
	{
		return Attach(	LoadMenu((hResource)
						? hResource
						: VGetResourceHandle(),
						pszResourceName));
	}

	/** Same as above, but allows the menu resource to be specified by ID
	rather than string name.*/
	VBOOL	Load(	VUINT		nIDResource,
					HINSTANCE	hResource = NULL)
	{
		VASSERT(nIDResource)
		return Load(MAKEINTRESOURCE(nIDResource), hResource);
	}

	/** Changes an existing menu item. This function is used to specify the
	content, appearance, and behavior of the menu item. nPos specifies the
	menu item to be changed, as determined by the nFlags parameter. nFlags
	specifies flags that control the interpretation of the nPos parameter
	and the content, appearance, and behavior of the menu item. This
	parameter must be a combination of one of the following required values
	and at least one of the values listed in the following Remarks section:

	MF_BYCOMMAND - Indicates that the nPos parameter gives the identifier
	of the menu item. The MF_BYCOMMAND flag is the default if neither the
	MF_BYCOMMAND nor MF_BYPOSITION flag is specified.

	MF_BYPOSITION - Indicates that the nPos parameter gives the zero-based
	relative position of the menu item. nIDNewItem specifies either the
	identifier of the modified menu item or, if the nFlags parameter has
	the MF_POPUP flag set, the handle to the drop-down menu or submenu.

	pszNewItem is a pointer to the content of the changed menu item. The
	interpretation of this parameter depends on whether the nFlags
	parameter includes the MF_BITMAP, MF_OWNERDRAW, or MF_STRING flag.

	MF_BITMAP - Contains a bitmap handle.

	MF_OWNERDRAW - Contains a 32-bit value supplied by an application that
	is used to maintain additional data related to the menu item. The value
	is in the itemData member of the structure pointed to by the lparam
	parameter of the WM_MEASUREITEM or WM_DRAWITEM messages sent when the
	menu item is created or its appearance is updated.

	MF_STRING - Contains a pointer to a null-terminated string (the default).

	Remarks: If Modify() replaces a menu item that opens a drop-down menu or
	submenu, the function destroys the old drop-down menu or submenu and
	free's the memory used by it. In order for keyboard accelerators to
	work with bitmap or owner-drawn menu items, the owner of the menu must
	process the WM_MENUCHAR message. See Owner-Drawn Menus and the
	WM_MENUCHAR message for more information. The application must call the
	DrawMenuBar() function whenever a menu changes, whether or not the menu
	is in a displayed window. To change the attributes of existing menu
	items, it is much faster to use the CheckItem() and EnableItem()
	functions. The following list describes the flags that may be set in the
	nFlags parameter:

	MF_BITMAP - Uses a bitmap as the menu item. The pszNewItem parameter
	contains the handle to the bitmap.

	MF_BYCOMMAND - Indicates that the nPos parameter specifies the
	identifier of the menu item (the default).

	MF_BYPOSITION - Indicates that the nPos parameter specifies the
	zero-based relative position of the new menu item.

	MF_CHECKED - Places a check mark next to the item. If your application
	provides check mark bitmaps, this flag displays a checked bitmap next
	to the menu item.

	MF_DISABLED - Disables the menu item so that it cannot be selected, but
	this flag does not gray it.

	MF_ENABLED - Enables the menu item so that it can be selected and
	restores it from its grayed state.

	MF_GRAYED - Disables the menu item and grays it so that it cannot be
	selected.

	MF_MENUBARBREAK - Functions the same as the MF_MENUBREAK flag for a
	menu bar. For a drop-down menu, submenu, or shortcut menu, the new
	column is separated from the old column by a vertical line.

	MF_MENUBREAK - Places the item on a new line (for menu bars) or in a
	new column (for a drop-down menu, submenu, or shortcut menu) without
	separating columns.

	MF_OWNERDRAW - Specifies that the item is an owner-drawn item. Before
	the menu is displayed for the first time, the window that owns the menu
	receives a WM_MEASUREITEM message to retrieve the width and height of
	the menu item. The WM_DRAWITEM message is then sent to the window
	procedure of the owner window whenever the appearance of the menu item
	must be updated.

	MF_POPUP - Specifies that the menu item opens a drop-down menu or
	submenu. The nIDNewItem parameter specifies the handle to the drop-down
	menu or submenu. This flag is used to add a menu name to a menu bar or
	a menu item that opens a submenu to a drop-down menu, submenu, or
	shortcut menu.

	MF_SEPARATOR - Draws a horizontal dividing line. This flag is used only
	in a drop-down menu, submenu, or shortcut menu. The line cannot be
	grayed, disabled, or highlighted. The pszNewItem and nIDNewItem
	parameters are ignored.

	MF_STRING - Specifies that the menu item is a text string, the
	pszNewItem parameter points to the string.

	MF_UNCHECKED - Does not place a check mark next to the item (the
	default). If your application supplies check mark bitmaps, this flag
	displays an unchecked bitmap next to the menu item.

	The following groups of flags cannot be used together:

	MF_BYCOMMAND and MF_BYPOSITION

	MF_DISABLED, MF_ENABLED, and MF_GRAYED

	MF_BITMAP, MF_STRING, MF_OWNERDRAW, and MF_SEPARATOR

	MF_MENUBARBREAK and MF_MENUBREAK

	MF_CHECKED and MF_UNCHECKED.*/
	VBOOL	ModifyItem(	VUINT			nPos,
						VUINT			nFlags,
						VUINT			nIDNewItem,
						VSTRING_CONST	pszNewItem) const
	{
		VASSERT(GetSafeHandle())
		return ModifyMenu(m_hMenu, nPos, nFlags, nIDNewItem, pszNewItem);
	}

	/** Changes an existing bitmap menu item. See above function for more
	information.*/
	VBOOL	ModifyBitmapItem(	VUINT			nPos,
								VUINT			nFlags,
								VUINT			nIDNewItem,
								VBitmap const&	bitmap) const
	{
		return ModifyItem(	nPos,
							nFlags | MF_BITMAP,
							nIDNewItem,
							(VSTRING_CONST)bitmap.GetHandle());
	}

	/** Associates a Help context identifier with the menu.*/
	VBOOL	SetContextHelpId(VDWORD nContextHelpId) const
	{
		VASSERT(GetSafeHandle())
		return SetMenuContextHelpId(m_hMenu, nContextHelpId);
	}

	/** Sets the default menu item for the specified menu. nItem is the
	identifier or position of the new default menu item or -1 for no
	default item. The meaning of this parameter depends on the value of
	bByPos. bByPos value specifies the meaning of nItem. If this parameter
	is VFALSE, nItem is a menu item identifier. Otherwise, it is a menu
	item position.*/
	VBOOL	SetDefaultItem(	VUINT nItem,
							VBOOL bByPos = VFALSE) const
	{
		VASSERT(GetSafeHandle())
		return (SetMenuDefaultItem(m_hMenu, nItem, bByPos)) ? VTRUE : VFALSE;
	}

	/** Associates the specified bitmap with a menu item. Whether the menu
	item is checked or unchecked, the system displays the appropriate bitmap
	next to the menu item. nPos specifies the menu item to be changed, as
	determined by the nFlags parameter. nFlags specifies how the nPos
	parameter is interpreted. The nFlags parameter must be one of the
	following values:

	MF_BYCOMMAND - Indicates that nPos gives the identifier of the menu
	item. If neither MF_BYCOMMAND nor MF_BYPOSITION is specified,
	MF_BYCOMMAND is the default flag.

	MF_BYPOSITION - Indicates that nPos gives the zero-based relative
	position of the menu item.

	pBmpUnchecked, if not NULL, specifies the bitmap displayed when the
	menu item is not checked. pBmpChecked, if not NULL, specifies the
	bitmap displayed when the menu item is checked. If either the
	pBmpUnchecked or pBmpChecked parameter is NULL, the system displays
	nothing next to the menu item for the corresponding check state. If
	both parameters are NULL, the system displays the default check-mark
	bitmap when the item is checked, and removes the bitmap when the item
	is not checked. When the menu is destroyed, these bitmaps are not
	destroyed, as it is up to the application to destroy them. The checked
	and unchecked bitmaps should be monochrome. The system uses the Boolean
	AND operator to combine bitmaps with the menu so that the white part
	becomes transparent and the black part becomes the menu-item color. If
	you use color bitmaps, the results may be undesirable. Use the
	GetSystemMetrics() function with the CXMENUCHECK and CYMENUCHECK values
	to retrieve the bitmap dimensions.*/
	VBOOL	SetItemBitmaps(	VUINT			nPos,
							VUINT			nFlags,
							VBitmap const*	pBmpUnchecked,
							VBitmap const*	pBmpChecked) const
	{
		VASSERT(GetSafeHandle())
		return SetMenuItemBitmaps(	m_hMenu,
									nPos,
									nFlags,
									(HBITMAP)((pBmpUnchecked)
									? pBmpUnchecked->GetHandle()
									: NULL),
									(HBITMAP)((pBmpChecked)
									? pBmpChecked->GetHandle()
									: NULL));
	}

	#ifdef VTOOLBAR
		/** Return the toolbar control associated with this menu.*/
		VToolBar*	ToolBar() const
			{ return (VToolBar*)m_pToolBar; }

		/** Set the toolbar control that is to be associated with this
		menu. pToolBar can be NULL for none.*/
		void		ToolBar(VToolBar* pToolBar)
			{ m_pToolBar = pToolBar; }
	#endif

protected:
	/** Embedded Members.*/
	HMENU	m_hMenu;
	VBOOL	m_bDestroyMenu;
	#ifdef VTOOLBAR
		VToolBar*	m_pToolBar;
	#endif
};

#endif	/* VMENU*/
