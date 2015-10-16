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

#ifndef VTREEVIEW
#define VTREEVIEW

#include "../vstandard.h"
#include "../collections/vptrarray.hpp"
#include "../gdi/vclientdc.hpp"
#include "../numbers/vbitmask.hpp"
#include "../numbers/vrtti.hpp"
#include "../parsers/vdelimitedstring.hpp"
#include "../strings/vstring.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"
#include "vcommoncontrol.hpp"

#define	VWCL_TREE_EXPAND_MARKER_LPARAM		-1
#define VWCL_MAX_SINGLE_TREE_ITEM_LENGTH	1024

/** Macro that declares a TV_ITEM struct named item and zero-fills it.*/
#define VDECLARE_TV_ITEM	\
	TV_ITEM item;			\
	VZEROSTRUCT(item);

/** Macro that declares a TV_INSERTSTRUCT struct named is and zero-fills it.*/
#define VDECLARE_TV_INSERTSTRUCT	\
	TV_INSERTSTRUCT is;				\
	VZEROSTRUCT(is);

/** VTreeView provides extended functionality (not simple message wrappers)
for the Win32 tree common control. These include the ability to toggle an
image during expansion, which is most useful when displaying a series of
open or closed folders, support for delayed population of the tree
(population only as needed), drag and drop, extended search ability, the
ability to display folders in bold type, and path or folder navigation.
Most functions that accept a HTREEITEM value can also accept NULL, unless
otherwise noted. If NULL, the currently selected item is assumed. Because
of this, some functions expect a tree item to be selected, so you should
provide a default selection and use the TVS_SHOWSELALWAYS window style bit,
or always provide a valid HTREEITEM to functions.*/
class VTreeView : public VCommonControl
{
public:
	/** Options that apply to this class and its known VWCL derivatives.
	These are stored in the VBitmask m_Options member which which can be
	modified by using the GetOptions() method, or during construction.*/
	enum	{			OPTION_TOGGLE_IMAGE_ON_EXPAND =				0x0001,
						OPTION_DISPLAY_FOLDERS_IN_BOLD_TYPE =		0x0002,
						OPTION_DIR_TREE_DISPLAY_NONE =				0x0004,
						OPTION_DIR_TREE_DISPLAY_DIRECTORIES =		0x0008,
						OPTION_DIR_TREE_DISPLAY_FILES =				0x0010,
						OPTION_DIR_TREE_DISPLAY_FILE_EXTENSIONS =	0x0020,
						OPTION_DELAYED_POPULATION =					0x0040,
						OPTION_DIR_TREE_DISPLAY_REMOTE =			0x0080,
						OPTION_DIR_TREE_DISPLAY_REMOVABLE =			0x0100,
						OPTION_DIR_TREE_DISPLAY_FIXED =				0x0200,
						OPTION_DIR_TREE_DISPLAY_CDROM =				0x0400,
						OPTION_DIR_TREE_DISPLAY_RAMDISK =			0x0800,
						OPTION_HIERARCHICAL_DATABASE_TREE_DISPLAY_ITEMS =
							OPTION_DIR_TREE_DISPLAY_FILES,
						OPTION_DEFAULT =
							OPTION_TOGGLE_IMAGE_ON_EXPAND |
							OPTION_DISPLAY_FOLDERS_IN_BOLD_TYPE,
			};

	/** Default constructor initializes options, RTTI information, and class
	members.*/
	VTreeView(	VUINT		nOptions = OPTION_DEFAULT,
				VRTTI_TYPE	nRTTI = VWCL_RTTI_TREEVIEW)
		: VCommonControl(nRTTI)
	{
		m_bDragging =		VFALSE;
		m_hLastDropTarget =	NULL;
		m_Options =			nOptions;
	}

	/* These functions support Drag and Drop.*/

	/** Call to start drag and drop, including capturing the mouse.
	OnBeginDrag() does this for you if TVS_DISABLEDRAGDROP is not set. If
	bSelect is VTRUE, the item will be selected first, which is the default.*/
	virtual void		BeginDragDrop(	HTREEITEM	hItemDrag = NULL,
										VBOOL		bSelect = VTRUE)
	{
		/* A drag drop op should not already be happening!*/
		VASSERT(!m_bDragging)

		/* Set flag, capture, and reset last drop target (just for safety).*/
		m_bDragging =		VTRUE;
		m_hLastDropTarget = NULL;
		SetCapture();

		if ( bSelect )
			Selection(ItemOrSelection(hItemDrag));
	}

	/** Call to cancel the current drag and drop operation.*/
	void				CancelDragDrop()
	{
		/* A drag drop op should already be happening!*/
		VASSERT(m_bDragging)

		/* Cleanup after a drag and drop operation.*/
		DragSelectTarget(NULL);
		m_bDragging = VFALSE;
		ReleaseCapture();
	}

	/** Select hItemSelect, erase old selection rect from screen, and draw
	new selection rect around hItemSelect (if not NULL). Returns hItemSelect.*/
	HTREEITEM			DragSelectTarget(HTREEITEM hItemSelect)
	{
		if ( hItemSelect != m_hLastDropTarget )
		{
			VClientDC dc(VREF_THIS);

			if ( dc.GetHandle() )
			{
				VRect r;

				/* Erase last focus rect?*/
				if ( m_hLastDropTarget )
				{
					GetItemRect(r, m_hLastDropTarget);
					dc.DrawFocusRect(r);
				}

				/* Draw the new selection rect?*/
				if ( hItemSelect )
				{
					GetItemRect(r, hItemSelect);
					dc.DrawFocusRect(r);
				}
			}

			/* Assign hItemSelect to last drop target.*/
			m_hLastDropTarget = hItemSelect;
		}

		return hItemSelect;
	}

	/** Does a hittest and selects the item at nx, ny if any. nx and ny are
	in client coordinates. Returns newly selected item, or NULL if there is
	not a tree item at the specified point.*/
	HTREEITEM			HitTestSelect(	VLONG nx,
										VLONG ny) const
	{
		HTREEITEM hItem = HitTest(nx, ny);

		if ( hItem )
			Selection(hItem);

		return hItem;
	}

	/** Call to do a hit test and call DragSelectTarget() on the item at nx,
	ny if any item it found at that point. Returns newly selected item, or
	NULL if there is not a tree item at the specified point.*/
	HTREEITEM			HitTestSelectDropTarget(	VLONG nx,
													VLONG ny)
		{ return DragSelectTarget(HitTest(nx, ny)); }

	/** Return the curent dragging state. VTRUE if dragging, VFALSE is not.*/
	VBOOL				IsDragging() const
		{ return m_bDragging; }

	/** Return the last drop target during a drag and drop operation.*/
	HTREEITEM			GetLastDropTarget() const
		{ return m_hLastDropTarget; }

	/* These functions support Tree Path Navigation.*/

	/** Get the full string path to item, separated by pszSeparator. The
	return value is a pointer to the string contained within the strBuffer
	parameter, or NULL on failure.*/
	VSTRING				GetPath(	VString&		strBuffer,
									HTREEITEM		hItem = NULL,
									VSTRING_CONST	pszSeparator =
													VFILE_PATH_SEP)
	{
		VASSERT(VSTRLEN_CHECK(pszSeparator))

		strBuffer.Empty();

		/* First, we need to reverse up the tree.*/
		VString		sItem;
		VPtrArray	array;
		VPTRARRAY_INDEX	nIndex =
			GetReversePath(array, ItemOrSelection(hItem));

		/* Build string by walking array backwards.*/
		while ( --nIndex >= 0 )
		{
			/* Get the text of this item.*/
			strBuffer += GetItemText(sItem, (HTREEITEM)array[nIndex]);

			/* Append separator if not last item.*/
			if ( nIndex )
				strBuffer += pszSeparator;
		}

		return strBuffer;
	}

	/** Get the full path to hItem, reversing up the tree to a root item.
	This function add's each found item to a VPtrArray collection object.
	Traverse the array backwards to get the forward path, or use GetPath().
	Return value is the number of item in the array, not the number of items
	added. The only time this function returns the number of items added is
	if the array is 0 size when called.*/
	VPTRARRAY_INDEX		GetReversePath(	VPtrArray&	array,
										HTREEITEM	hItem = NULL)
	{
		/* Starting at hItem, reverse tree until a root item is found
		to build reverse order path.*/
		HTREEITEM hParent = ItemOrSelection(hItem);

		while ( hParent )
		{
			/* Add to array.*/
			if ( array.Add(hParent) != -1 )
			{
				/* If this item is a root item, we are done. Call
				virtual function to determine.*/
				if ( IsRootItem(hParent) )
					break;
				else
					hParent = GetParent(hParent);
			}
			else
				break;
		}

		return array.Size();
	}

	/** Find the tree item specified by the path. The return value is the
	closest item in the tree to the specified path, even if the exact
	location cannot be found. If a pointer to a VBOOL is given as
	pbTotalCompletion, this function will set it to indicate if a total
	path completion was accomplished.*/
	HTREEITEM			FindItemByPath(	VSTRING_CONST	pszPath,
										HTREEITEM		hParent = NULL,
										VSTRING_CONST	pszSeparator =
														VFILE_PATH_SEP,
										VBOOL			bCaseSensitive =
														VFALSE,
										VBOOL*			pbTotalCompletion =
														NULL)
	{
		VASSERT(VSTRLEN_CHECK(pszPath))
		VASSERT(VSTRLEN_CHECK(pszSeparator))

		/* Initialize out parameters.*/
		if ( pbTotalCompletion )
			*pbTotalCompletion = VTRUE;

		HTREEITEM hItem = NULL;

		/* Copy pszString since we modify it.*/
		VString sFind(pszPath);

		if ( sFind.IsNotEmpty() )
		{
			VDelimitedString	ds(sFind, *pszSeparator);
			VSTRING				pszOffSet;
			hItem = hParent;

			while ( (pszOffSet = ds.Next()) )
			{
				/* Find next string in the tree.*/
				HTREEITEM hNextItem =
					FindChildItemByString(pszOffSet, hItem, bCaseSensitive);

				/* If not found, exit loop. Will stop navigating at last
				valid item.*/
				if ( hNextItem )
					hItem = hNextItem;
				else
				{
					if ( pbTotalCompletion )
						*pbTotalCompletion = VFALSE;

					break;
				}
			}
		}

		return hItem;
	}

	/** This function is called when reverse traversing a tree path. The
	default implementation just checks to see if hItem is a root item
	(has no parent). Return VFALSE to continue reverse building of tree,
	or VTRUE to stop it when a root item is found.*/
	virtual VBOOL		IsRootItem(HTREEITEM hItem)
	{
		VASSERT(hItem)
		return (!GetParent(hItem)) ? VTRUE : VFALSE;
	}

	/** Navigate to specified location, from hParent, populating tree as
	needed to get there. The string is expected to be delimeted by some
	character that separates the string into strings in the tree. For
	example, assume the following tree:
	Root
	   Child1
	     Child2
	Then a possible string to navigate to Child2 would be
	"Root/Child1/Child2" assuming the separator is /.*/
	HTREEITEM			NavigateTo(	VSTRING_CONST	pszPath,
									HTREEITEM		hParent =
													NULL,
									VSTRING_CONST	pszSeparator =
													VFILE_PATH_SEP,
									VBOOL			bCaseSensitive =
													VFALSE)
	{
		VASSERT(VSTRLEN_CHECK(pszPath))
		VASSERT(VSTRLEN_CHECK(pszSeparator))

		HTREEITEM hItem =
			FindItemByPath(pszPath, hParent, pszSeparator, bCaseSensitive);

		if ( hItem )
		{
			EnsureVisible(hItem);
			Selection(hItem);
		}

		return hItem;
	}

	/* These functions support delayed population of tree's.*/

	/** Starting at hParent, return the inserted expand marker item, or NULL
	if none is found. This is a special marker that is used to determine if
	the parent folder should populate the tree with valid items at a later
	time.*/
	HTREEITEM			FindExpandMarker(HTREEITEM hParent) const
	{
		HTREEITEM hExpandMarker = (hParent) ? GetChild(hParent) : NULL;

		if (	hExpandMarker &&
				LParam(hExpandMarker) != VWCL_TREE_EXPAND_MARKER_LPARAM )
			hExpandMarker = NULL;

		return hExpandMarker;
	}

	/** Insert an expand marker (for delayed expansion) via Populate(). This
	is a special marker that is used to determine if the parent folder
	should populate the tree with valid items at a later time.*/
	HTREEITEM			InsertExpandMarker(HTREEITEM hParent = NULL) const
	{
		VASSERT(hParent)

		VDECLARE_TV_INSERTSTRUCT
		is.hInsertAfter =	TVI_LAST;
		is.hParent =		ItemOrSelection(hParent);
		is.item.lParam =	VWCL_TREE_EXPAND_MARKER_LPARAM;
		is.item.mask =		TVIF_PARAM;

		return InsertItem(is);
	}

	/** Populate tree starting at hParent. This function is called indirectly
	from several places to populate a tree starting at hParent. The base
	class does nothing, and an override is responsible for populating child
	items.*/
	virtual void		Populate(HTREEITEM hParent)
		{;}

	/* These functions support searching for child items.*/

	/** Search the children of the hParent folder looking for an item with
	the lParam (user data) set to nParam. Returns the handle to the item on
	success, NULL on failure.*/
	virtual HTREEITEM	FindChildItemByLParam(	VLPARAM		nParam,
												HTREEITEM	hParent = NULL)
	{
		/* Make sure hParent is populated.*/
		PopulateIfNeeded(hParent);

		HTREEITEM hFound = GetChild(hParent);

		while ( hFound )
		{
			if ( LParam(hFound) != nParam )
				hFound = GetNextSib(hFound);
			else
				break;
		}

		return hFound;
	}

	/** Search the children of the hParent folder looking for an item with
	the text pszText. Returns the handle to the item on success, NULL on
	failure.*/
	virtual HTREEITEM	FindChildItemByString(
		VSTRING_CONST	pszText,
		HTREEITEM		hParent = NULL,
		VBOOL			bCaseSensitive = VFALSE)
	{
		VASSERT(VSTRLEN_CHECK(pszText))

		/* Make sure hParent is populated if DELAYED_POPULATION is set.*/
		PopulateIfNeeded(hParent);

		HTREEITEM	hFound = GetChild(hParent);
		VString		sItem;

		while ( hFound )
		{
			if ( GetItemText(sItem, hFound) )
			{
				if (	((bCaseSensitive)
						? sItem.Compare(pszText)
						: sItem.CompareNoCase(pszText)) != 0 )
					hFound = GetNextSib(hFound);
				else
					break;
			}
			else
				hFound = NULL;
		}

		return hFound;
	}

	/* The following functions are not specific to a particular
	functionality and are alphabetical.*/

	/** Call to check state mask against current state, for instance, to
	determine expanded state. Returns VTRUE if bits are set in the mask,
	VFALSE otherwise.*/
	VBOOL				CheckState(	HTREEITEM	hItem = NULL,
									VUINT		nState = TVIS_EXPANDED) const
	{
		VDECLARE_TV_ITEM
		item.mask =			TVIF_HANDLE | TVIF_STATE;
		item.hItem =		ItemOrSelection(hItem);
		item.stateMask =	TVIS_BOLD | TVIS_CUT | TVIS_DROPHILITED |
							TVIS_EXPANDED | TVIS_EXPANDEDONCE | TVIS_SELECTED;

		return ((GetItem(item)) && (item.state & nState)) ? VTRUE : VFALSE;
	}

	/** Collapse, expand, or toggle the state of hItem. nFlags can be one of
	the following values:

	TVE_COLLAPSE - Collapses the list.

	TVE_COLLAPSERESET - Collapses the list and removes the child items.

	TVE_EXPAND - Expands the list.

	TVE_TOGGLE - Collapses the list if it is currently expanded or expands
	it if it is currently collapsed.

	Returns VTRUE on success, VFALSE on failure.*/
	virtual VBOOL		Collapse(	HTREEITEM	hItem = NULL,
									VUINT		nFlags = TVE_COLLAPSE)
	{
		hItem = ItemOrSelection(hItem);

		if ( VWINDOW_SM2(TVM_EXPAND, nFlags, hItem) )
		{
			/* Verifies expand marker is put back in place if also
			deleting child items and supporting delayed population.*/
			if (	nFlags & TVE_COLLAPSERESET &&
					m_Options.IsSet(OPTION_DELAYED_POPULATION) )
				InsertExpandMarker(hItem);

			return VTRUE;
		}

		return VFALSE;
	}

	/** Same as Collapse(), but removes child items in the process.*/
	VBOOL				CollapseReset(HTREEITEM hItem = NULL)
		{ return Collapse(hItem, TVE_COLLAPSE | TVE_COLLAPSERESET); }

	/** Create the treeview control. Returns VTRUE on success, VFALSE on
	error. nStyle can be any of the standard WS_xxx style bits described
	in VWindow::Create(), plus treeview control specific ones including:

	TVS_CHECKBOXES - Version 4.70 - Enables check boxes for items in a tree
	view control. Effectively, when set to this style, the control creates
	and sets a state image list usingDrawFrameControl. Once the control is
	created with this style, it cannot be removed. If it is necessary to
	remove this style, the control must be destroyed and a new one created
	in its place.

	TVS_DISABLEDRAGDROP - Prevents the tree view control from sending
	TVN_BEGINDRAG notification messages.

	TVS_EDITLABELS - Allows the user to edit the labels of tree view items.

	TVS_FULLROWSELECT - Version 4.71 - Enables full-row selection in the
	tree view. The entire row of the selected item is highlighted, and
	clicking anywhere on an item's row will cause it to be selected. This
	style cannot be used in conjunction with the TVS_HASLINES style.

	TVS_HASBUTTONS - Displays plus (+) and minus (-) buttons next to parent
	items. The user clicks the buttons to expand or collapse a parent item's
	list of child items. To include buttons with items at the root of the
	tree view, TVS_LINESATROOT must also be specified.

	TVS_HASLINES - Uses lines to show the hierarchy of items.

	TVS_INFOTIP - Version 4.71 - The tree view control will send the
	TVN_GETINFOTIP notification to obtain tooltip information.

	TVS_LINESATROOT - Uses lines to link items at the root of the tree view
	control. This value is ignored if TVS_HASLINES is not also specified.

	TVS_NONEVENHEIGHT - Version 4.71 - The height of the items can be set
	to an odd height with the TVM_SETITEMHEIGHT message. By default, the
	height of items must be an even value.

	TVS_NOSCROLL - Version 4.71 - Disables both horizontal and vertical
	scrolling in the control. The control will not display any scroll bars.

	TVS_NOTOOLTIPS - Version 4.70 - The tree view control does not support
	tooltips.

	TVS_RTLREADING - Version 4.70 - Displays text using right-to-left
	reading order on Hebrew or Arabic systems.

	TVS_SHOWSELALWAYS - Causes a selected item to remain selected when the
	tree view control loses focus.

	TVS_SINGLEEXPAND - Version 4.71 - When this style is enabled, changing
	the selection in the tree view will automatically cause the item being
	selected to expand and the item being unselected to collapse. If the
	mouse is used to single-click the selected item and that item is closed,
	it will be expanded. If the selected item is single-clicked when it is
	open, it will be collapsed.

	TVS_TRACKSELECT - Version 4.70 - Enables hot tracking in a tree view
	control.*/
	VBOOL				Create(	VWindow const&	windowParent,
								VRect const&	r,
								VUINT			nID,
								VUINT			nStyle =
												WS_BORDER |
												WS_TABSTOP |
												WS_VISIBLE |
												TVS_HASBUTTONS |
												TVS_HASLINES |
												TVS_LINESATROOT |
												TVS_EDITLABELS |
												TVS_DISABLEDRAGDROP |
												TVS_SHOWSELALWAYS)
	{
		return VCommonControl::Create(	windowParent,
										r,
										WS_CHILD | nStyle,
										WC_TREEVIEW,
										NULL,
										nID);
	}

	/** Creates a dragging bitmap for the specified item in a tree view
	control. The function also creates an image list for the bitmap and
	adds the bitmap to the image list. An application can display the
	image when dragging the item by using the image list functions. If you
	create a tree view control without an associated image list, you cannot
	use the TVM_CREATEDRAGIMAGE message to create the image to display
	during a drag operation. You must implement your own method of creating
	a drag cursor. ImageList_Destroy() should be called on the returned
	imagelist when no longer needed, or you can wrap the returned imagelist
	in a VImageList object.*/
	HIMAGELIST			CreateDragImage(HTREEITEM hItem = NULL) const
	{
		return (HIMAGELIST)VWINDOW_SM2(TVM_CREATEDRAGIMAGE, 0, ItemOrSelection(hItem));
	}

	/** Delete all items from the tree view.*/
	void				DeleteAllItems() const
		{ VWINDOW_SM2(TVM_DELETEITEM, 0, TVI_ROOT); }

	/** Delete all children of the specified, or currently selected, item.*/
	void				DeleteChildren(HTREEITEM hParent = NULL) const
	{
		hParent =			ItemOrSelection(hParent);
		HTREEITEM hChild =	(hParent) ? GetChild(hParent) : NULL;

		while ( hChild )
		{
			DeleteItem(hChild);
			hChild = GetChild(hParent);
		}
	}

	/** Removes an item from a tree view control. Returns VTRUE on success,
	VFALSE on failure.*/
	VBOOL				DeleteItem(HTREEITEM hItem = NULL) const
		{ return VWINDOW_SMB2(TVM_DELETEITEM, 0, ItemOrSelection(hItem)); }

	/** Call to destroy imagelist held in the tree control.*/
	void				DestroyImageList() const
	{
		HIMAGELIST hImageList = ImageList();

		if ( hImageList )
		{
			ImageList((HIMAGELIST)NULL);
			ImageList_Destroy(hImageList);
		}
	}

	/** Begins in-place editing of the specified item's text, replacing the
	text of the item with a single-line edit control containing the text.
	This message implicitly selects and focuses the specified item. Returns
	the handle to the edit control used to edit the item text if successful,
	or NULL otherwise.*/
	HWND				EditLabel(HTREEITEM hItem = NULL) const
		{ return (HWND)VWINDOW_SM2(TVM_EDITLABEL, 0, ItemOrSelection(hItem)); }

	/** Ends the editing of a tree view item's label. bDontSave is a variable
	that indicates whether the editing is canceled without being saved to
	the label. If this parameter is VTRUE, the system cancels editing
	without saving the changes. Otherwise, the system saves the changes to
	the label. Returns VTRUE if successful, VFALSE otherwise.*/
	VBOOL				EndEditLabel(VBOOL bDontSave = VTRUE) const
		{ return VWINDOW_SMB1(TVM_ENDEDITLABELNOW, bDontSave); }

	/** Ensures that a tree view item is visible, expanding the parent item
	or scrolling the tree view control, if necessary. Returns VTRUE if the
	system scrolled the items in the tree view control and no items were
	expanded. Otherwise, the function returns VFALSE.*/
	VBOOL				EnsureVisible(HTREEITEM hItem = NULL) const
		{ return VWINDOW_SMB2(TVM_ENSUREVISIBLE, 0, ItemOrSelection(hItem)); }

	/** Expand the tree to show child items of hItem.*/
	VBOOL				Expand(HTREEITEM hItem = NULL)
		{ return Collapse(hItem, TVE_EXPAND); }

	/** Get the first child item of hItem, or NULL if hItem has no children.*/
	HTREEITEM			GetChild(HTREEITEM hItem = NULL) const
		{ return GetNextItem(hItem, TVGN_CHILD); }

	/** Returns the number of total items in the tree.*/
	VUINT				GetCount() const
		{ return (VUINT)VWINDOW_SM0(TVM_GETCOUNT); }

	/** Retrieves the item that is the target of a drag-and-drop operation,
	or NULL if none.*/
	HTREEITEM			GetDropHilight() const
		{ return GetNextItem(NULL, TVGN_DROPHILITE); }

	/** Retrieves the handle to the edit control being used to edit a tree
	view item's text. Return the handle to the edit control on success,
	NULL on failure.*/
	HWND				GetEditControl(HTREEITEM hItem = NULL) const
	{
		return (HWND)VWINDOW_SM2(TVM_GETEDITCONTROL, 0, ItemOrSelection(hItem));
	}

	/** Retrieves the first visible item. Returns NULL on failure.*/
	HTREEITEM			GetFirstVisible() const
		{ return GetNextItem(NULL, TVGN_FIRSTVISIBLE); }

	/** Retrieves the incremental search string for a tree view control. The
	tree view control uses the incremental search string to select an item
	based on characters typed by the user. Returns the number of characters
	in the incremental search string. If the tree view control is not in
	incremental search mode, the return value is zero. pszBuffer of the
	buffer that receives the incremental search string.*/
	VBOOL				GetISearchString(VSTRING pszBuffer) const
	{
		VASSERT(pszBuffer)
		return VWINDOW_SMB2(TVM_GETISEARCHSTRING, 0, pszBuffer);
	}

	/** Retrieves some or all of a tree view item's attributes. item is a
	pre-initialized (by you) TV_ITEM structure that specifies the
	information to retrieve and receives information about the item. When the
	message is sent, the hItem member identifies the item to retrieve
	information about and the mask member specifies the attributes to
	retrieve. If the mask member specifies the TVIF_TEXT value, the pszText
	member must contain the address of the buffer that receives the item
	text and the cchTextMax member must specify the size of the buffer. If
	the mask member specifies the TVIF_STATE value, the stateMask member
	must specify the item state bits to retrieve. On output, the state
	member contains the values of the specified state bits. Returns VTRUE
	if successful, or VFALSE otherwise.*/
	VBOOL				GetItem(TV_ITEM& item) const
	{
		VASSERT(item.hItem)

		/* The handle is always valid (or better be!).*/
		item.mask |= TVIF_HANDLE;

		#ifdef VWCL_DEBUG
			if ( item.mask & TVIF_TEXT )
				VASSERT(item.pszText && item.cchTextMax)
		#endif

		return VWINDOW_SMB2(TVM_GETITEM, 0, &item);
	}

	/** Retrieves the bounding rectangle for a tree view item and indicates
	whether the item is visible. If the item is visible and the bounding
	rectangle was successfully retrieved, the return value is VTRUE.
	Otherwise, the function returns VFALSE and does not retrieve the
	bounding rectangle.*/
	VBOOL				GetItemRect(	VRect&		r,
										HTREEITEM	hItem =
													NULL,
										VBOOL		bTextRectOnly =
													VTRUE) const
	{
		*(HTREEITEM*)(RECT*)r.GetPointer() =
			ItemOrSelection(hItem);

		return VWINDOW_SMB2(TVM_GETITEMRECT, bTextRectOnly, r.GetPointer());
	}

	/** Returns a pointer to the string contained within the strBuffer
	parameter on success, NULL on failure. Normally, the entire string
	will be returned, but if only part of it is desired, you can pass a
	non-zero value for nMaxChars. nMaxChars excludes the NULL terminator,
	so if you want 2 bytes returned, pass 2 for this variable. strBuffer
	will not be modified on failure.*/
	virtual VSTRING		GetItemText(	VString&	strBuffer,
										HTREEITEM	hItem = NULL,
										VUINT		nMaxChars = 0)
	{
		VTCHAR sz[VWCL_MAX_SINGLE_TREE_ITEM_LENGTH];

		VDECLARE_TV_ITEM
		item.hItem =		ItemOrSelection(hItem);
		item.pszText =		sz;
		item.cchTextMax =	(nMaxChars) ? nMaxChars + 1 : VARRAY_SIZE(sz);
		item.mask =			TVIF_TEXT;

		return (GetItem(item)) ? strBuffer.String(sz, 0, nMaxChars) : NULL;
	}

	/** Returns the handle to the next item, as specified by the nFlag
	parameter, which can be one of the following:

	TVGN_CARET - Retrieves the currently selected item.

	TVGN_CHILD - Retrieves the first child item of the item specified by
	the hItem parameter.

	TVGN_DROPHILITE - Retrieves the item that is the target of a
	drag-and-drop operation.

	TVGN_FIRSTVISIBLE - Retrieves the first item that is visible in the
	tree view window.

	TVGN_LASTVISIBLE - Version 4.71. Retrieves the last expanded item in
	the tree. This does not retrieve the last item visible in the tree
	view window.

	TVGN_NEXT - Retrieves the next sibling item.

	TVGN_NEXTVISIBLE - Retrieves the next visible item that follows the
	specified item. The specified item must be visible.

	TVGN_PARENT - Retrieves the parent of the specified item.

	TVGN_PREVIOUS - Retrieves the previous sibling item.

	TVGN_PREVIOUSVISIBLE - Retrieves the first visible item that precedes
	the specified item. The specified item must be visible.

	TVGN_ROOT - Retrieves the topmost or very first item of the tree view
	control.

	If the specified relationship cannot be resolved, the return value
	is NULL.*/
	HTREEITEM			GetNextItem(	HTREEITEM	hItem,
										VUINT		nFlag = TVGN_NEXT) const
		{ return (HTREEITEM)VWINDOW_SM2(TVM_GETNEXTITEM, nFlag, hItem); }

	/** Returns the handle to the next sibling of hItem, or NULL on failure.*/
	HTREEITEM			GetNextSib(HTREEITEM hItem = NULL) const
		{ return GetNextItem(ItemOrSelection(hItem), TVGN_NEXT); }

	/** Returns the handle to the next visible item, or NULL on failure.*/
	HTREEITEM			GetNextVisible(HTREEITEM hItem = NULL) const
		{ return GetNextItem(ItemOrSelection(hItem), TVGN_NEXTVISIBLE); }

	/** Return a reference to the bitmask object that holds class option bits.*/
	VBitmask&			GetOptions() const
		{ return (VBitmask&)m_Options; }

	/** Returns the handle to the parent of hItem, or NULL on failure.*/
	HTREEITEM			GetParent(HTREEITEM hItem = NULL) const
		{ return GetNextItem(ItemOrSelection(hItem), TVGN_PARENT); }

	/** Returns the handle to the previous visible item, or NULL
	on failure.*/
	HTREEITEM			GetPreviousVisible(HTREEITEM hItem = NULL) const
		{ return GetNextItem(ItemOrSelection(hItem), TVGN_PREVIOUSVISIBLE); }

	/** Returns the handle to the previous sibling of hItem, or NULL
	on failure.*/
	HTREEITEM			GetPrevSib(HTREEITEM hItem = NULL) const
		{ return GetNextItem(ItemOrSelection(hItem), TVGN_PREVIOUS); }

	/** Returns the handle to the root item, or NULL on failure.*/
	HTREEITEM			GetRoot() const
		{ return GetNextItem(NULL, TVGN_ROOT); }

	/** Returns the number of items that can be fully visible in the client
	window of a tree view control. The number of items that can be fully
	visible may be greater than the number of items in the control. The
	control calculates this value by dividing the height of the client
	window by the height of an item. Note that the return value is the
	number of items that can be fully visible. If you can see all of 20
	items and part of one more item, the return value is 20.*/
	VUINT				GetVisibleCount() const
		{ return (VUINT)VWINDOW_SM0(TVM_GETVISIBLECOUNT); }

	/** Determines the location of the specified point at nx, ny relative to
	the client area of a tree view control. When the function returns, the
	pnHitTest (if given) value is a hit test value that indicates the
	location of the specified point. This value can be one OR more of the
	following:

	TVHT_ABOVE- Above the client area.

	TVHT_BELOW - Below the client area.

	TVHT_NOWHERE - In the client area, but below the last item.

	TVHT_ONITEM - On the bitmap or label associated with an item.

	TVHT_ONITEMBUTTON - On the button associated with an item.

	TVHT_ONITEMICON - On the bitmap associated with an item.

	TVHT_ONITEMINDENT - In the indentation associated with an item.

	TVHT_ONITEMLABEL - On the label (string) associated with an item.

	TVHT_ONITEMRIGHT - In the area to the right of an item.

	TVHT_ONITEMSTATEICON - On the state icon for a tree view item that is in
	a user-defined state.

	TVHT_TOLEFT - To the left of the client area.

	TVHT_TORIGHT - To the right of the client area.

	Returns the handle to the tree view item that occupies the specified
	point, or NULL if no item occupies the point.*/
	HTREEITEM			HitTest(	VLONG	nx,
									VLONG	ny,
									VUINT*	pnHitTest = NULL) const
	{
		if ( pnHitTest )
			*pnHitTest = NULL;

		TVHITTESTINFO	hti;
		hti.pt.x =		nx;
		hti.pt.y =		ny;
		hti.flags =		0;
		hti.hItem =		NULL;

		VWINDOW_SM2(TVM_HITTEST, 0, &hti);

		if ( pnHitTest )
			*pnHitTest = hti.flags;

		return hti.hItem;
	}

	/** Returns the handle to the normal or state image list associated with
	a tree view control. nType can be one of the following values:

	TVSIL_NORMAL - Indicates the normal image list, which contains selected,
	nonselected, and overlay images for the items of a tree view control.

	TVSIL_STATE - Indicates the state image list. You can use state images
	to indicate application-defined item states. A state image is displayed
	to the left of an item's selected or nonselected image.

	Returns the imagelist handle on success, NULL on failure.*/
	HIMAGELIST			ImageList(VWPARAM nType = TVSIL_NORMAL) const
		{ return (HIMAGELIST)VWINDOW_SM1(TVM_GETIMAGELIST, nType); }

	/** Sets the normal or state image list for a tree view control and
	redraws the control using the new images. nType can be one of the
	following values:

	TVSIL_NORMAL - Indicates the normal image list, which contains selected,
	nonselected, and overlay images for the items of a tree view control.

	TVSIL_STATE - Indicates the state image list. You can use state images
	to indicate application-defined item states. A state image is displayed
	to the left of an item's selected or nonselected image.

	Returns the handle to the previous image list, if any, or
	NULL otherwise.*/
	HIMAGELIST			ImageList(	HIMAGELIST	hImageList,
									VWPARAM		nType = TVSIL_NORMAL) const
		{ return (HIMAGELIST)VWINDOW_SM2(TVM_SETIMAGELIST, nType, hImageList); }

	/** Retrieves the amount, in pixels, that child items are indented
	relative to their parent items.*/
	VUINT				Indent() const
		{ return (VUINT)VWINDOW_SM0(TVM_GETINDENT); }

	/** Set the amount, in pixels, that child items are indented relative to
	their parent items.*/
	void				Indent(VUINT nIndent) const
		{ VWINDOW_SM1(TVM_SETINDENT, nIndent); }

	/** Initialize the image list. Default parameters assume a masking color
	the same as used for icon transparency. If hResouce is not given,
	VGetResourceHandle() will be used.*/
	VBOOL				InitImageList(	VUINT		nBitmapID,
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

		VASSERT(hImageList)

		return VFALSE;
	}

	/** Inserts a new item in a tree view control. item is a TV_INSERTSTRUCT
	that must be pre-initialized. Returns the HTREEITEM of the inserted
	item on success, NULL on failure.*/
	HTREEITEM			InsertItem(TV_INSERTSTRUCT const& item) const
		{ return (HTREEITEM)VWINDOW_SM2(TVM_INSERTITEM, 0, &item); }

	/** Returns either the hItem parameter or current selection.*/
	HTREEITEM			ItemOrSelection(HTREEITEM hItem) const
	{
		VASSERT(hItem || Selection())
		return (hItem) ? hItem : Selection();
	}

	/** Get the VLPARAM (user data) stored in the specified tree item.
	Returns -1 on failure or value on success.*/
	VLPARAM				LParam(	VLPARAM		nParam,
								HTREEITEM	hItem = NULL) const
	{
		VDECLARE_TV_ITEM
		item.hItem =	ItemOrSelection(hItem);
		item.lParam =	nParam;
		item.mask =		TVIF_PARAM;

		return (SetItem(item)) ? nParam : -1;
	}

	/** Set the VLPARAM (user data) stored in the specified tree item.
	Returns -1 on failure or value on success.*/
	VLPARAM				LParam(HTREEITEM hItem = NULL) const
	{
		VDECLARE_TV_ITEM
		item.hItem =	ItemOrSelection(hItem);
		item.mask =		TVIF_PARAM;

		return (GetItem(item)) ? item.lParam : -1;
	}

	/** Redraws the specified item in the style used to indicate the target
	of a drag-and-drop operation.*/
	VBOOL				SelectDropTarget(HTREEITEM hItem) const
		{ return Selection(hItem, TVGN_DROPHILITE); }

	/** Returns the handle to the currently selected item, or NULL if
	nothing is selected.*/
	HTREEITEM			Selection() const
		{ return GetNextItem(NULL, TVGN_CARET); }

	/** Set the currently selected item to hItem. Returns VTRUE on success,
	VFALSE on failure. nFlag can be one of the following:

	TVGN_CARET - Sets the selection to the specified item. The tree view
	control's parent window receives the TVN_SELCHANGING and TVN_SELCHANGED
	notification messages.

	TVGN_DROPHILITE - Redraws the specified item in the style used to
	indicate the target of a drag-and-drop operation.

	TVGN_FIRSTVISIBLE - Ensures that the specified item is visible, and, if
	possible, displays it at the top of the control's window. Tree view
	controls display as many items as will fit in the window. If the
	specified item is near the bottom of the control's hierarchy of items,
	it might not become the first visible item, depending on how many items
	fit in the window.*/
	VBOOL				Selection(	HTREEITEM	hItem,
									VWPARAM		nFlag = TVGN_CARET) const
		{ return VWINDOW_SMB2(TVM_SELECTITEM, nFlag, hItem); }

	/** Select the first visible item in the tree.*/
	VBOOL				SelectFirstVisible(HTREEITEM hItem = NULL) const
		{ return Selection(ItemOrSelection(hItem), TVGN_FIRSTVISIBLE); }

	/** Sets some or all of a tree view item's attributes. See the GetItem()
	function for more information. This function automatically sets the
	TVIF_HANDLE member of the mask since it is required.*/
	VBOOL				SetItem(TV_ITEM& item) const
	{
		VASSERT(item.hItem)

		/* The handle is always valid (or better be!).*/
		item.mask |= TVIF_HANDLE;

		return VWINDOW_SMB2(TVM_SETITEM, 0, &item);
	}

	/** Set the text for the item specified by hItem. Returns VTRUE on
	success, VFALSE on failure.*/
	VBOOL				SetItemText(	VSTRING_CONST	pszText,
										HTREEITEM		hItem = NULL) const
	{
		VDECLARE_TV_ITEM
		item.hItem =	ItemOrSelection(hItem);
		item.pszText =	(VSTRING)pszText;
		item.mask =		TVIF_TEXT;

		return SetItem(item);
	}

	/** Sorts the child items of the specified parent item in a tree view
	control. Returns VTRUE if successful, or VFALSE otherwise.*/
	VBOOL				SortChildren(HTREEITEM hItem = NULL) const
		{ return VWINDOW_SMB2(TVM_SORTCHILDREN, 0, ItemOrSelection(hItem)); }

	/** Sorts tree view items using an application-defined callback function
	that compares the items. pfnCompare is the address of an
	application-defined callback function, which is called during a sort
	operation each time the relative order of two list items needs to be
	compared. The callback function has the following form:

	VINT CALLBACK
		CompareFunc(VLPARAM nParam1, VLPARAM nParam2, VLPARAM nParamSort);

	The callback function must return a negative value if the first item
	should precede the second, a positive value if the first item should
	follow the second, or zero if the two items are equivalent. The nParam1
	and nParam2 parameters correspond to the lParam member of the TV_ITEM
	structure for the two items being compared. The nParamSort parameter
	corresponds to the nParam parameter. nParam is a user defined value
	that can be anything you desire. Returns VTRUE if successful, or VFALSE
	otherwise.*/
	VBOOL				SortChildrenCallback(	PFNTVCOMPARE	pfnCompare,
												HTREEITEM		hItem,
												VLPARAM			nParam) const
	{
		TV_SORTCB			cb;
		cb.hParent =		ItemOrSelection(hItem);
		cb.lpfnCompare =	pfnCompare;
		cb.lParam =			nParam;

		return (cb.hParent) ?
			VWINDOW_SMB2(TVM_SORTCHILDRENCB, 0, &cb) : VFALSE;
	}

	/** Collapses the list at hItem if it is currently expanded or expands
	it if it is currently collapsed. Returns VTRUE if successful, or VFALSE
	otherwise.*/
	VBOOL				ToggleExpansion(	HTREEITEM	hItem =
														NULL,
											UINT		nFlags =
														TVE_TOGGLE) const
		{ return VWINDOW_SMB2(TVE_TOGGLE, nFlags, ItemOrSelection(hItem)); }

protected:
	/* These functions support Image Index Selection
	(OPTION_TOGGLE_IMAGE_ON_EXPAND option).*/

	/** Called when an item is expanded. Return the index into the imagelist
	to use. The default implementation uses index 0 to be a closed folder,
	index 1 to be and open folder image. The image at index 2 is considered
	to be a non-folder image. If the item is believed to be a folder, bFolder
	will be VTRUE. If the item is going to an expanded state (showing child
	items), bOpen will be VTRUE. nParam is the lParam (user data) of hItem.*/
	virtual VINT		GetImageIndex(	HTREEITEM	hItem,
										VLPARAM		nParam,
										VBOOL		bFolder,
										VBOOL		bOpen)
		{ return (bFolder) ? (bOpen) ? 1 : 0 : 2; }

	/** Toggle the image used for closed and open folders. It will call
	GetImageIndex() to determine the index to use for the tree item.*/
	virtual VLONG		OnItemExpanded(	NM_TREEVIEW&	nmTreeView,
										VLPARAM&		lCommonControlResult)
	{
		if ( m_Options.IsSet(OPTION_TOGGLE_IMAGE_ON_EXPAND) )
		{
			/* Change the image displayed for the new selection. Call
			virtual function GetImageIndex() to determine index to use.*/
			nmTreeView.itemNew.iImage =
				GetImageIndex(	nmTreeView.itemNew.hItem,
								nmTreeView.itemNew.lParam,
								VTRUE,
								(nmTreeView.itemNew.state & TVIS_EXPANDED)
								? VTRUE
								: VFALSE);

			nmTreeView.itemNew.iSelectedImage =
				nmTreeView.itemNew.iImage;

			SetItem(nmTreeView.itemNew);
		}

		/* Allow further processing.*/
		return 1;
	}

	/* These functions support drag and drop.*/

	/** This function is called when a drag and drop operation should begin.
	This will only occur if TVS_DISABLEDRAGDROP is NOT one of the window
	style bits. The default implementation calls BeginDragDrop() to initiate
	a drag and drop operation. Override to provide more selective, or
	restrictive, use of drag and drop.*/
	virtual VLONG		OnBeginDrag(	NM_TREEVIEW&	nmTreeView,
										VLPARAM&		lCommonControlResult)
	{
		BeginDragDrop(nmTreeView.itemNew.hItem);
		return 1;
	}

	/** Called from WM_MOUSEMOVE when dragging is taking place. Default
	action hilights drop target in the tree (if any). Derived class should
	use this function to set the cursor.*/
	virtual void		OnDragMove(	VWPARAM	wKeystate,
									VLONG	nx,
									VLONG	ny)
		{ HitTestSelectDropTarget(nx, ny); }

	/** Called when a source tree item is dropped at the end of a drag and
	drop operation. hSource is the source tree item, hDest is the
	destination tree item (which can be NULL) and hWnd is set if the source
	item was dropped on a different window than the tree control. In other
	words, hDest or hWnd will be NULL, they are mutually exclusive. The
	nKeyState is the keystate as defined by WM_LBUTTONUP.*/
	virtual void		OnDropItem(	HTREEITEM	hSource,
									HTREEITEM	hDest,
									HWND		hWnd,
									VWPARAM		nKeyState)
		{;}

	/* These function overrides support delayed population.*/

	/** Called when a tree item is expanding. The defalt implementation
	verifies tree is populated if delayed population is used.*/
	virtual VLONG		OnItemExpanding(
		NM_TREEVIEW&	nmTreeView,
		VLPARAM&		lCommonControlResult)
	{
		PopulateIfNeeded(nmTreeView.itemNew.hItem);
		return 1;
	}

	/** Populate the children of hParent if needed.*/
	void				PopulateIfNeeded(HTREEITEM hParent)
	{
		/* All classes that implement delayed population must have a
		function that sets the root items, therefore, hParent must be
		valid to need populated.*/
		if (	hParent &&
				hParent != TVI_ROOT &&
				m_Options.IsSet(OPTION_DELAYED_POPULATION) )
		{
			HTREEITEM hExpandMarker = FindExpandMarker(hParent);

			/* Only call Populate() if expand marker is found, or hParent
			has no children.*/
			if ( hExpandMarker || !GetChild(hParent) )
			{
				/* Remove the expand marker.*/
				if ( hExpandMarker )
					DeleteItem(hExpandMarker);

				/* Let derived class populate tree.*/
				Populate(hParent);
			}
		}
	}

	/* Balance of protected functions in alphabetical order.*/

	/** Called when the user starts to edit (inplace) a list item if the
	TVS_EDITLABELS style is set.*/
	virtual VLONG		OnBeginLabelEdit(
		TV_DISPINFO&	DispInfo,
		VLPARAM&		lCommonControlResult)
		{ return 1; }

	/** Called when the user starts to drag a list item with the right
	mouse if the TVS_DISABLEDRAGDROP style is NOT set.*/
	virtual VLONG		OnBeginRightDrag(
		NM_TREEVIEW&	nmTreeView,
		VLPARAM&		lCommonControlResult)
		{ return 1; }

	/** Called when an item is deleted is the list.*/
	virtual VLONG		OnDeleteItem(	NM_TREEVIEW&	nmTreeView,
										VLPARAM&		lCommonControlResult)
		{ return 1; }

	/** Called when the user ends editing (inplace) a list item if the
	TVS_EDITLABELS style is set.*/
	virtual VLONG		OnEndLabelEdit(	TV_DISPINFO&	DispInfo,
										VLPARAM&		lCommonControlResult)
		{ return 1; }

	/** Requests that a tree view control's parent window provide information
	needed to display or sort an item. The item member of DispInfo is a
	TVITEM structure whose mask, hItem, state, and lParam members specify
	the type of information required. You must fill the members of the
	structure with the appropriate information. This notification is called
	under the following circumstances:

	1) If the pszText member of the item's TVITEM structure is the
	LPSTR_TEXTCALLBACK value, the control sends this notification to
	retrieve the item's text. In this case, the mask member of DispInfo
	will have the TVIF_TEXT flag set.

	2) If the iImage or iSelectedImage member of the item's TVITEM structure is
	the I_IMAGECALLBACK value, the control sends this notification to
	retrieve the index of an item's icons in the control's image list. In
	this case, if the item is selected, the mask member of DispInfo will
	have the TVIF_SELECTEDIMAGE flag set. If the item is not selected, the
	mask member of DispInfo will have the TVIF_IMAGE flag set.

	3) If the cChildren member of the item's TVITEM structure is the
	I_CHILDRENCALLBACK value, the control sends this notification to
	retrieve a value that indicates whether the item has child items. In
	this case, the mask member of DispInfo will have the TVIF_CHILDREN flag
	set.*/
	virtual VLONG		OnGetDisplayInfo(
		TV_DISPINFO&	DisplayInfo,
		VLPARAM&		lCommonControlResult)
		{ return 1; }

	/** Called when a new item in the tree has been selected.*/
	virtual VLONG		OnSelelectionChanged(
		NM_TREEVIEW&	nmTreeView,
		VLPARAM&		lCommonControlResult)
		{ return 1; }

	/** Called when the selection in the tree is changing.*/
	virtual VLONG		OnSelelectionChanging(
		NM_TREEVIEW&	nmTreeView,
		VLPARAM&		lCommonControlResult)
		{ return 1; }

	/** Notifies a tree view control's parent window that it must update the
	information it maintains about an item.*/
	virtual VLONG		OnSetDisplayInfo(
		TV_DISPINFO&	DispInfo,
		VLPARAM&		lCommonControlResult)
		{ return 1; }

	/** Watches for notifications to call reflected notify handlers to
	perform extended operations.*/
	virtual VLONG		OnReflectedNotify(	NMHDR&		nmhdr,
											VLPARAM&	lCommonControlResult)
	{
		NM_TREEVIEW*	pNMTreeView = (LPNM_TREEVIEW)&nmhdr;
		TV_DISPINFO*	pTVDispInfo = (TV_DISPINFO*)&nmhdr;

		switch ( nmhdr.code )
		{
			case TVN_BEGINDRAG:
				return OnBeginDrag(			VREF_PTR(pNMTreeView),
											lCommonControlResult);

			case TVN_BEGINLABELEDIT:
				return OnBeginLabelEdit(	VREF_PTR(pTVDispInfo),
											lCommonControlResult);

			case TVN_BEGINRDRAG:
				return OnBeginRightDrag(	VREF_PTR(pNMTreeView),
											lCommonControlResult);

			case TVN_DELETEITEM:
				return OnDeleteItem(		VREF_PTR(pNMTreeView),
											lCommonControlResult);

			case TVN_ENDLABELEDIT:
				return OnEndLabelEdit(		VREF_PTR(pTVDispInfo),
											lCommonControlResult);

			case TVN_GETDISPINFO:
				return OnGetDisplayInfo(	VREF_PTR(pTVDispInfo),
											lCommonControlResult);

			case TVN_ITEMEXPANDED:
				return OnItemExpanded(		VREF_PTR(pNMTreeView),
											lCommonControlResult);

			case TVN_ITEMEXPANDING:
				return OnItemExpanding(		VREF_PTR(pNMTreeView),
											lCommonControlResult);

			case TVN_SELCHANGED:
				return OnSelelectionChanged(VREF_PTR(pNMTreeView),
											lCommonControlResult);

			case TVN_SELCHANGING:
				return OnSelelectionChanging(	VREF_PTR(pNMTreeView),
												lCommonControlResult);

			case TVN_SETDISPINFO:
				return OnSetDisplayInfo(	VREF_PTR(pTVDispInfo),
											lCommonControlResult);
		}

		return VCommonControl::OnReflectedNotify(nmhdr, lCommonControlResult);
	}

	/** Override looks for drag and drop operations, and F2 key to edit
	label if TVS_EDITLABELS style is set for this window. If Escape is
	pressed during drag and drop, CancelDragDrop() will be called.*/
	virtual VLONG		WindowProc(	HWND		hWnd,
									VUINT		nMessage,
									VWPARAM		wParam,
									VLPARAM		lParam)
	{
		switch ( nMessage )
		{
			case WM_KEYDOWN:
			{
				/* For F2, editing the tree item is the default behaviour
				if TVS_EDITLABELS bit is set.*/
				if ( (wParam == VK_F2) && (Style() & TVS_EDITLABELS) )
				{
					HTREEITEM hItem = Selection();

					if ( hItem )
						EditLabel(hItem);
				}
				/* If dragging, test for these.*/
				else if ( wParam == VK_ESCAPE && m_bDragging )
					CancelDragDrop();

				break;
			}

			case WM_CAPTURECHANGED:
			case WM_KILLFOCUS:
			case WM_LBUTTONUP:
			{
				/* If dragging, stop and clean up window.*/
				if ( m_bDragging )
				{
					/* Get the last drop target.*/
					HTREEITEM hLastDropTarget = m_hLastDropTarget;

					/* Clean up after drag and drop.*/
					CancelDragDrop();

					/* Since this switch handles more than one case, only
					execute the rest of this code if WM_LBUTTONUP.*/
					if ( nMessage == WM_LBUTTONUP )
					{
						/* Get the selection.*/
						HTREEITEM hItem = Selection();

						/* If no last drop target, check for drop on a
						window other that ourself.*/
						if ( hLastDropTarget )
						{
							if ( hItem )
							{
								/* Are the drop target and source the same?
								Is drop target already the parent?
								If not, we have a valid drop item. Call
								virtual function OnDropItem() to let derived
								class handle.*/
								if (	hItem != hLastDropTarget &&
										GetParent(hItem) != hLastDropTarget )
									OnDropItem(	hItem,
												hLastDropTarget,
												NULL,
												wParam);
							}
						}
						else
						{
							/* Was it dropped on a window?*/
							VPoint pt;
							pt.GetCursorPosition();
							HWND hWnd = WindowFromPoint(pt.GetPOINT());

							/* Call virtual function OnDropItem().*/
							if ( hWnd )
								OnDropItem(hItem, NULL, hWnd, wParam);
						}
					}
				}

				break;
			}

			case WM_MOUSEMOVE:
				if ( m_bDragging )
					OnDragMove(wParam, LOWORD(lParam), HIWORD(lParam));
				break;
		}

		return VCommonControl::WindowProc(hWnd, nMessage, wParam, lParam);
	}

	/** Embedded Members.*/
	VBitmask			m_Options;
	VBOOL				m_bDragging;
	HTREEITEM			m_hLastDropTarget;
};

#endif /* VTREEVIEW*/
