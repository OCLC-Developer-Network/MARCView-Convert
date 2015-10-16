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

Primary Author of this source code file:  Jan Sijm (jsijm@murdock.demon.nl)
Other Author(s) of this source code file: Todd Osborne (todd@vwcl.org)

VWCL and all source code are copyright (c) 1996-2000 by The VWCL Alliance.*/

#ifndef VREBAR
#define VREBAR

#include "..\vstandard.h"
#include "..\common_controls\vcommoncontrol.hpp"
#include "..\structures\vrect.hpp"
#include "..\windows\vwindow.hpp"
#include "..\windows\vtempwindow.hpp"

/** VReBar encapuslates the Win32 rebar common control. The Microsoft
implementation of the rebar control is not the most logical implementation
you might expect. One of the biggest problems with the rebar control is the
fact that the bands in the control are identified by the zero based index.
This is a problem because the index of a specific band changes if the band
is moved within the rebar using drag and drop. This makes it virtually
impossible to uniquely identify a specific rebar band. The VReBar class
uses a different approach. Each band in the rebar control is identified by
the ID of the child window associated with the band. You can also have
bands that are not associated with a child window, in which case you can
still use the index of the band. It's your responsibility to make sure that
the resource ID's of child windows do not overlap with the zero based index
of any of the rebar bands. Choosing child window resource ID's > 100 will
make sure that you are safe. Every method of the VReBar class will first
make an attempt to translate the specified ID to the associated band index.
If the this translation fails, the mathod will assume that the specified ID
is actually an index. VReBar requires that the VWindow option
OPTION_NEED_ON_PAINT be set. It does this for you in Create(). If VToolBar
objects are to be used with this class, you must include vtoolbar.hpp BEFORE
including this header file, vrebar.hpp.*/
class VReBar : public VCommonControl
{
public:
	/** Default constructor initializes base class RTTI information.*/
	VReBar(VRTTI_TYPE nRTTI = VWCL_RTTI_REBAR)
		: VCommonControl(nRTTI)
		{;}

	/** Retrieve the current background color.*/
	COLORREF	BackgroundColor() const
		{ return (COLORREF)VWINDOW_SM0(RB_GETBKCOLOR); }

	/** Set this current background color. Returns a COLORREF value that
	represent the previous color.*/
	COLORREF	BackgroundColor(COLORREF crBackground) const
		{ return (COLORREF)VWINDOW_SM2(RB_SETBKCOLOR, 0, crBackground); }

	/** Translate the ID of a band to the index of that band. When the band
	was inserted using either InsertToolBar() or InsertWindow(), the ID will
	be the ID that was passed to the Create() call of the toolbar or child
	window. If the band was inserted using the InsertBand() function, the ID
	was specified explicitly or was retrieved from the associated child
	window if available.*/
	VINT		BandIDToIndex(VUINT nID) const
		{ return (VINT)VWINDOW_SM1(RB_IDTOINDEX, nID); }

	/** Same as above, but returns nID if the index cannot be determined.*/
	VUINT		BandIDToIndexOrID(VUINT nID) const
	{
		VINT nIndex = BandIDToIndex(nID);
		
		return	(nIndex == -1)
				? nID
				: (VUINT)nIndex;
	}

	/** Create the rebar control. Returns VTRUE on success, VFALSE on error.
	nStyle can be any of the standard WS_xxx style bits described in
	VWindow::Create(), plus toolbar specific ones including:

	CCS_xxx constants that apply to the rebar common control.

	RBS_AUTOSIZE - Version 4.71. The rebar control will automatically
	change the layout of the bands when the size or position of the control
	changes. An RBN_AUTOSIZE notification will be sent when this occurs.

	RBS_BANDBORDERS - Version 4.70. The rebar control displays narrow lines
	to separate adjacent bands.

	RBS_DBLCLKTOGGLE - Version 4.71. The rebar band will toggle its
	maximized or  minimized state when the user double-clicks on the band.
	Without this style, the maximized or minimized state is toggled when the
	user single-clicks on the band.

	RBS_FIXEDORDER - Version 4.70. The rebar control always displays bands
	in the same order. You can move bands to different rows, but the band
	order is static.

	RBS_REGISTERDROP - Version 4.71. The rebar control generates
	RBN_GETOBJECT notification messages when an object is dragged over a
	band in the control. To receive the RBN_GETOBJECT notifications,
	initialize OLE with a call to OleInitialize or CoInitialize. VWCL will
	do this for you in VApplication::Init(), as long as the OLE libraries
	are preloaded. See VApplication for more information.

	RBS_TOOLTIPS - Version 4.70. Not yet supported.

	RBS_VARHEIGHT - Version 4.70. The rebar control displays bands at the
	minimum required height, when possible. Without this style, the rebar
	control displays all bands at the same height, using the height of the
	tallest visible band to determine the height of other bands.

	RBS_VERTICALGRIPPER - Version 4.71. The size grip will be displayed
	vertically instead of horizontally in a vertical rebar control. This
	style is ignored for rebar controls that do not have the CCS_VERT style.*/
	VBOOL		Create(	VWindow const&	windowParent,
						VRect const&	r,
						VUINT			nID,
						VUINT			nStyle =
										WS_BORDER |
										WS_VISIBLE |
										WS_CLIPSIBLINGS |
										WS_CLIPCHILDREN |
										RBS_TOOLTIPS |
										RBS_VARHEIGHT |
										RBS_BANDBORDERS)
	{
		/* Make sure OnPaint() is called.*/
		GetOptions().Append(OPTION_NEED_ON_PAINT);

		/* Create the rebar control.*/
		return VCommonControl::Create(	windowParent,
										r, nStyle | WS_CHILD,
										REBARCLASSNAME,
										NULL,
										nID);
	}

	/** Delete the specified rebar band. The rebar band is identified by the
	ID of the band or by the zero based index of the band. This method will
	first translate the nID parameter to the corresponding band index. If
	there is no corresponding band index, the method will assume that nID
	is a band index. The band is deleted from the rebar control, but this
	method will not touch the associated child window.*/
	VBOOL		DeleteBand(VUINT nID) const
		{ return VWINDOW_SMB2(RB_DELETEBAND, BandIDToIndexOrID(nID), VTRUE); }

	/** Retrieves the borders of a band. The result of this function can be
	used to calculate the usable area in a band. The r VRect object will
	receive the band borders. If the rebar control has the RBS_BANDBORDERS
	style, each member of this structure will receive the number of pixels,
	on the corresponding side  of the band that constitute the border. If
	the rebar control does not have the RBS_BANDBORDERS style, only the left
	value receives valid information. The rebar band is identified by the ID
	of the band or by the zero based index of the band. This method will
	first translate the nID parameter to the corresponding band index. If
	there is no corresponding band index, the method will assume that nID is
	a band index.*/
	void		GetBandBorders(VUINT nID, VRect& r) const
	{
		VWINDOW_SM2(RB_GETBANDBORDERS, BandIDToIndexOrID(nID), r.GetPointer());
	}

	/** Get the number of bands currently in the rebar control.*/
	VUINT		GetBandCount() const
		{ return (VUINT)VWINDOW_SM0(RB_GETBANDCOUNT); }

	/** Get information about the specified band in the rebar control. The
	rebar band is identified by the ID of the band or by the zero based
	index of the band. This method will first translate the nID parameter
	to the corresponding band index. If there is no corresponding band index,
	the method will assume that nID is a band index. The rbbi paramater is
	a reference to a REBARBANDINFO structure that will receive the requested
	band information. Set REBARBANDINFO.fMask to the items you want to
	retrieve. Additionally, you must set the cch member of the REBARBANDINFO
	structure to the size of the lpText buffer when RBBIM_TEXT is specified.*/
	VBOOL		GetBandInfo(VUINT nID, REBARBANDINFO& rbbi) const
	{
		rbbi.cbSize = sizeof(rbbi);
		return VWINDOW_SMB2(RB_GETBANDINFO, BandIDToIndexOrID(nID), &rbbi);
	}

	/** Retrieves the bounding rectangle for a given band in a rebar control.
	The r VRect object paramater will receive the bounds of the rebar band.
	The rebar band is identified by the ID of the band or by the zero based
	index of the band. This method will first translate the nID parameter to
	the corresponding band index. If there is no corresponding band index,
	the method will assume that nID is a band index.*/
	VBOOL		GetBandRect(VUINT nID, VRect& r) const
	{
		return VWINDOW_SMB2(RB_GETRECT, BandIDToIndexOrID(nID), r.GetPointer());
	}

	/** Retrieves the color scheme information from the rebar control. cs is
	a reference to a COLORSCHEME structure that will receive the color
	scheme information. The rebar control uses the color scheme information
	when drawing the 3-D elements in the control and bands. This function
	will automatically initialize the COLORSCHEME structure size.*/
	VBOOL		GetColorScheme(COLORSCHEME& cs) const
	{
		cs.dwSize = sizeof(cs);
		return VWINDOW_SMB2(RB_GETCOLORSCHEME, 0, &cs);
	}

	/** Get the height, in pixels, of the rebar control.*/
	VUINT		GetHeight() const
		{ return (VUINT)VWINDOW_SM0(RB_GETBARHEIGHT); }

	/** Retrieves information about the rebar control and the image list it
	uses. The rbi is a reference to a REBARINFO structure that will receive
	the requested information. Set REBARINFO.fMask to the items you want to
	retrieve. The HIMAGELIST handle can be wrapped by the VImageList class.
	The REBARINFO structure size will be initialized by this function.*/
	VBOOL		GetInfo(REBARINFO& rbi) const
	{
		rbi.cbSize = sizeof(rbi);
		return VWINDOW_SMB2(RB_GETBARINFO, 0, &rbi);
	}

	/** Get the number of rows of bands currently in the rebar control.*/
	VUINT		GetRowCount() const
		{ return (VUINT)VWINDOW_SM0(RB_GETROWCOUNT); }

	/** Retrieves the height of a row in a rebar control. The height of the
	row that contains the specified band will be retrieved.*/
	VUINT		GetRowHeight(VUINT nID) const
		{ return (VUINT)VWINDOW_SM1(RB_GETROWHEIGHT, BandIDToIndexOrID(nID)); }

	/** Hide the specified rebar band. The rebar band is identified by the ID
	of the band or by the zero based index of the band. This method will
	first translate the nID parameter to the corresponding band index. If
	there is no corresponding band index, the method will assume that nID
	is a band index.*/
	void			HideBand(VUINT nID) const
		{ ShowBand(nID, VFALSE); }

	/** Hide the specified rebar band. The rebar band is identified by the ID
	of the band or by the zero based index of the band. This method will
	first translate the nID parameter to the corresponding band index. If
	there is no corresponding band index, the method will assume that nID is
	a band index. This method will first check if the specified band is
	already hidden before actually hiding the band. If the associated rebar
	control is not visible, it will be made visible.*/
	void			HideBandEx(VUINT nID) const
		{ ShowBandEx(nID, VFALSE); }

	/** Add a band to the rebar control. The rbbi paramater is a REBARBANDINFO
	structure that describes the band to be inserted into the rebar control.
	The nIndex paramater specifies the zero based index where the band will
	be inserted. If you set this parameter to -1, the control will add the
	new band at the last location. The band will be identified by the wID
	value specified in the REBARBANDINFO structure. If REBARBANDINFO.wID is
	not specified, this method will try to retrieve the ID of the associated
	child window. If no child window is associated with this band
	(e.g. REBARBANDINFO.hwndChild == NULL), the specified index will be
	used to identify the band.*/
	VBOOL			InsertBand(	REBARBANDINFO&	rbbi,
								VINT			nIndex = -1) const
	{
		if ( rbbi.hwndChild && !(rbbi.fMask & RBBIM_ID) )
		{
			/* Make sure this band has an identifier.*/
			rbbi.fMask |=	RBBIM_ID;
			rbbi.wID =		::GetWindowLong(rbbi.hwndChild, GWL_ID);
		}

		/* Insert the band into the rebar control.*/
		return VWINDOW_SMB2(RB_INSERTBAND, nIndex, &rbbi);
	}

#ifdef VTOOLBAR
	/** Add a VToolBar control (or a derived class) to the VReBar control.
	This function will determine the size of the toolbar and create a band
	in the rebar that matches the size of the toolbar. The band will be
	identified by the ID that was passed to the Create() method of the
	toolbar. The band containing the default toolbar created by
	VMainWindow will be identified by IDW_TOOLBAR. The nIndex paramater
	specifies the zero based index where the band will be inserted. If you
	set this parameter to -1, the control will add the new band at the last
	location.  The nStyle parameter can be a combination of the
	following styles:

	RBBS_BREAK - The band is on a new line.

	RBBS_CHILDEDGE - The band has an edge at the top and bottom of the
	child window.

	RBBS_FIXEDSIZE - The band can't be sized. With this style, the sizing
	grip is not displayed on the band.

	RBBS_GRIPPERALWAYS - Version 4.71. The band will always have a sizing
	grip, even if it is the only band in the rebar.

	RBBS_HIDDEN - The band will not be visible.

	RBBS_CHEVRON - Version 5.00. The band will have a chevron.

	RBBS_NOGRIPPER - Version 4.71. The band will never have a sizing grip,
	even if there is more than one band in the rebar.

	RBBS_VARIABLEHEIGHT - Version 4.71. The band can be resized by the
	rebar control; cyIntegral and cyMaxChild affect how the rebar will
	resize the band.

	The recommended style for the toolbar that will be added to the rebar
	control is: WS_CHILD | WS_VISIBLE | CCS_NODIVIDER | CCS_NORESIZE |
	TBSTYLE_FLAT | TBSTYLE_TOOLTIPS. If you need more control over the
	method used to insert the band into the rebar control, see either the
	InsertWindow() or InsertBand methods().*/
	VBOOL		InsertToolBar(	VToolBar const&	wndToolbar,
								VINT			nIndex = -1,
								VUINT			nStyle =
												RBBS_GRIPPERALWAYS |
												RBBS_CHILDEDGE |
												RBBS_NOVERT) const
	{
		/** Get the size of the toolbar.*/
		SIZE size;
		wndToolbar.VWINDOW_SM2(TB_GETMAXSIZE, 0, &size);

		/* Initialize the structure needed to add the toolbar control.*/
		REBARBANDINFO rbi;
		VZEROSTRUCT(rbi);
		rbi.cbSize =		sizeof(rbi);
		rbi.fMask =			RBBIM_CHILD | RBBIM_SIZE | RBBIM_CHILDSIZE |
							RBBIM_STYLE | RBBIM_ID | RBBIM_IDEALSIZE;
		rbi.fStyle =		nStyle;
		rbi.cxMinChild =	size.cx;
		rbi.cyMinChild =	size.cy;
		rbi.cyMaxChild =	size.cy;
		rbi.cyChild =		size.cy;
		rbi.cx =			size.cx;
		rbi.cxIdeal =		size.cx;
		rbi.wID =			wndToolbar.Long(GWL_ID);
		rbi.hwndChild =		wndToolbar.GetHandle();

		/* Insert the toolbar into the rebar control.*/
		return InsertBand(rbi, nIndex);
	}
#endif

	/** Add any child window derived from VWindow to the VReBar control. This
	function will determine the size of the child window and create a band
	in the rebar that matches the size of the child window. The band will be
	identified by the ID that was passed to the Create() method of the child
	window. The nIndex paramater specifies the zero based index where the
	band will be inserted.  If you set this parameter to -1, the control will
	add the new band at the last location. The pszText parameter can be used
	to add a label to the rebar band, which will be displayed in front of
	the child window. The nIndex paramater specifies the zero based index
	where the band will be inserted. If you set this parameter to -1, the
	control will add the new band at the last location.  The nStyle
	parameter can be a combination of the following styles:

	RBBS_BREAK - The band is on a new line.

	RBBS_CHILDEDGE - The band has an edge at the top and bottom of the
	child window.

	RBBS_FIXEDSIZE - The band can't be sized. With this style, the sizing
	grip is not displayed on the band.

	RBBS_GRIPPERALWAYS - Version 4.71. The band will always have a sizing
	grip, even if it is the only band in the rebar.

	RBBS_HIDDEN - The band will not be visible.

	RBBS_CHEVRON - Version 5.00. The band will have a chevron.

	RBBS_NOGRIPPER - Version 4.71. The band will never have a sizing grip,
	even if there is more than one band in the rebar.

	RBBS_VARIABLEHEIGHT - Version 4.71. The band can be resized by the
	rebar control.

	If you need more control over the method used to insert the band into
	the rebar control, see the InsertBand() method.*/
	VBOOL		InsertWindow(	VWindow const&	window,
								VSTRING_CONST	pszText = NULL,
								VINT			nIndex = -1,
								VUINT			nStyle =
												RBBS_GRIPPERALWAYS |
												RBBS_CHILDEDGE |
												RBBS_NOVERT) const
	{
		/** Get the size of the child window.*/
		VRect r;
		window.GetRect(r);

		/* Initialize the structure needed to add the window.*/
		REBARBANDINFO rbi;
		VZEROSTRUCT(rbi);
		rbi.cbSize     =	sizeof(rbi);
		rbi.fMask      =	RBBIM_CHILD | RBBIM_SIZE | RBBIM_CHILDSIZE |
							RBBIM_STYLE | RBBIM_ID | RBBIM_IDEALSIZE;
		rbi.fStyle     =	nStyle;
		rbi.cxMinChild =	r.GetWidth();
		rbi.cyMinChild =	r.GetHeight();
		rbi.cyMaxChild =	r.GetHeight();
		rbi.cyChild    =	r.GetHeight();
		rbi.cx         =	r.GetWidth();
		rbi.cxIdeal    =	r.GetWidth();
		rbi.wID        =	window.Long(GWL_ID);
		rbi.hwndChild  =	window.GetHandle();

		if ( pszText )
		{
			rbi.fMask |=	RBBIM_TEXT;
			rbi.lpText =	(VSTRING)pszText;
		}

		/* Insert the child window into the rebar control.*/
		return InsertBand(rbi, nIndex);
	}

	/** Same as above, but lets you insert non-VWindow windows into the
	control.*/
	VBOOL		InsertWindow(	HWND			hWnd,
								VSTRING_CONST	pszText = NULL,
								VINT			nIndex = -1,
								VUINT			nStyle =
												RBBS_GRIPPERALWAYS |
												RBBS_CHILDEDGE |
												RBBS_NOVERT) const
		{ return InsertWindow(VTempWindow(hWnd), pszText, nIndex, nStyle); }

	/** Determine if the specified rebar band is visible. The rebar band is
	identified by the ID of the band or by the zero based index of the band.
	This method will first translate the nID parameter to the corresponding
	band index. If there is no corresponding band index, the method will
	assume that nID is a band index. This function uses VWindow::IsVisible()
	to determine if the band is visible. Please note that at this point,
	this method will only work for bands that contain child windows. We
	haven't found a way yet to determine visibility of a band when a child
	window is not available (e.g. this may become a problem when InsertBand()
	is used without specifying REBARBANDINFO.hwndChild). If
	VWindow::IsVisibleEx() should be used to determine visibility, pass
	VTRUE for bUseVisibleEx, or use the IsBandVisibleEx() function.*/
	VBOOL		IsBandVisible(	VUINT nID,
								VBOOL bUseVisibleEx = VFALSE) const
	{
		HWND hWnd =		NULL;
		VINT nIndex =	BandIDToIndex(nID);

		/* Is the specified ID is an index, retrieve the associated HWND?*/
		if ( nIndex == -1 )
			hWnd = IndexToHWND(nID);
		/* Translate ID to window handle?*/
		else
			hWnd = ::GetDlgItem(m_hWindow, nID);

		/* Is the band containing the child window visible?*/
		if ( hWnd )
		{
			VTempWindow tempWindow(hWnd);

			if ( bUseVisibleEx )
				return tempWindow.IsVisibleEx();
			else
				return tempWindow.IsVisible();
		}

		return VFALSE;
	}

	/** Same as above, but used VWindow::IsVisibleEx() to determine
	visibility state.*/
	VBOOL		IsBandVisibleEx(VUINT nID) const
		{ return IsBandVisible(nID, VTRUE); }

	/** Resizes a band in a rebar control to either its ideal or largest
	size. The bIdeal paramater indicates if the ideal width of the band
	should be used (see REBARBANDINFO) when the band is maximized. If this
	value is VTRUE, the ideal width will be used. If this value is VFALSE,
	the band will be made as large as possible. The rebar band is identified
	by the ID of the band or by the zero based index of the band. This method
	will first translate the nID parameter to the corresponding band index.
	If there is no corresponding band index, the method will assume that
	nID is a band index.*/
	void		MaximizeBand(	VUINT nID,
								VBOOL bIdeal = VTRUE) const
		{ VWINDOW_SM2(RB_MAXIMIZEBAND, BandIDToIndexOrID(nID), bIdeal); }

	/** Resizes a band in a rebar control to its smallest size. The rebar band
	is identified by the ID of the band or by the zero based index of the
	band. This method will first translate the nID parameter to the
	corresponding band index. If there is no corresponding band index, the
	method will assume that nID is a band index.*/
	void		MinimizeBand(VUINT nID) const
		{ VWINDOW_SM1(RB_MINIMIZEBAND, BandIDToIndexOrID(nID)); }

	/** Moves a band from one index to another. The rebar band to move is
	identified by the ID of the band or by the zero based index of the band.
	This method will first translate the nID parameter to the corresponding
	band index. If there is no corresponding band index, the method will
	assume that nID is a band index. The nIndexTo paramater is the new index.*/
	VBOOL		MoveBand(VUINT nID, VUINT nIndexTo) const
	{
		VUINT nCount =	GetBandCount();
		VUINT nIndex =	BandIDToIndexOrID(nID);

		/* Not valid to move to a band larger than the number created!*/
		VASSERT(nIndexTo < nCount)

		/* Move the band to the specified index.*/
		if ( nIndexTo < nCount )
			return VWINDOW_SMB2(RB_MOVEBAND, nIndex, nIndexTo);

		return VFALSE;
	}

	/** Retrieves the rebar control's current palette.*/
	HPALETTE	Palette() const
		{ return (HPALETTE)VWINDOW_SM0(RB_GETPALETTE); }

	/** Set the rebar control's current palette. Returns an HPALETTE that
	specifies the rebar control's previous palette.*/
	HPALETTE	Palette(HPALETTE hPalette) const
		{ return (HPALETTE)VWINDOW_SM2(RB_SETPALETTE, 0, hPalette); }

	/** Set characteristics of the specified band in the rebar control. The
	rebar band is identified by the ID of the band or by the zero based
	index of the band. This method will first translate the nID parameter
	to the corresponding band index. If there is no corresponding band
	index, the method will assume that nID is a band index. The rbbi
	paramater is a REBARBANDINFO structure that contains the desired band
	characteristics. The size of this structure will automatically be
	initialized, but you are required to populate the remaining values to
	desired ones.*/
	VBOOL		SetBandInfo(	VUINT			nID,
								REBARBANDINFO&	rbbi) const
	{
		rbbi.cbSize = sizeof(rbbi);
		return VWINDOW_SMB2(RB_SETBANDINFO, BandIDToIndexOrID(nID), &rbbi);
	}

	/** Set the color scheme information of the rebar control. cs is a
	COLORSCHEME structure that contains the color scheme information. The
	rebar control uses the color scheme information when drawing the 3-D
	elements in the control and bands. This function will set cs the
	structure size, but you are required to set the other values to those
	you desire.*/
	VBOOL		SetColorScheme(COLORSCHEME& cs) const
	{
		cs.dwSize = sizeof(cs);
		return VWINDOW_SMB2(RB_SETCOLORSCHEME, 0, &cs);
	}

	/** Set characteristics of the rebar control and the image list it uses.
	rbi is a REBARINFO structure that contains the information. The
	HIMAGELIST handle can be wrapped by the VImageList class. This function
	will set the rbi structure size, but you are required to set the other
	values to those you desire.*/
	VBOOL		SetInfo(REBARINFO& rbi) const
	{
		rbi.cbSize = sizeof(rbi);
		return VWINDOW_SMB2(RB_SETBARINFO, 0, &rbi);
	}

	/** Sets a rebar control's parent window. Returns the handle to the
	previous parent window, or NULL if there is no previous parent.*/
	HWND		SetParent(VWindow const& windowParent) const
		{ return (HWND)VWINDOW_SM1(RB_SETPARENT, windowParent.GetHandle()); }

	/** Show the specified rebar band. The rebar band is identified by the ID
	of the band or by the zero based index of the band. This method will
	first translate the nID parameter to the corresponding band index. If
	there is no corresponding band index, the method will assume that nID
	is a band index. The default bShow parameter for this functions shows
	the band, but you can pass VFALSE for it to hide the band, or use the
	HideBand() method.*/
	void		ShowBand(	VUINT nID,
							VBOOL bShow = VTRUE) const
		{ VWINDOW_SM2(RB_SHOWBAND, BandIDToIndexOrID(nID), bShow); }

	/** Show the specified rebar band. The rebar band is identified by the ID
	of the band or by the zero based index of the band. This method will
	first translate the nID parameter to the corresponding band index. If
	there is no corresponding band index, the method will assume that nID
	is a band index. This method will first check if the specified band is
	already visible before actually showing the band. If the associated rebar
	control is not visible, it will be made visible. The default bShow
	parameter for this functions shows the band, but you can pass VFALSE
	for it to hide the band, or use the HideBandEx() method.*/
	void		ShowBandEx(	VUINT nID,
							VBOOL bShow = VTRUE) const
	{
		/* Show the associated rebar first.*/
		if ( !IsVisibleEx() )
			Show();

		if ( bShow != IsBandVisibleEx(nID) )
			ShowBand(nID, bShow);
	}

	/** Attempts to find the best layout of the bands for the given rectangle.
	The r paramater specifies the rectangle to which the rebar control
	should be sized. The rebar bands will be arranged and wrapped as
	necessary to fit the rectangle. Bands that have the RBBS_VARIABLEHEIGHT
	style will be resized as evenly as possible  to fit the rectangle. The
	height of a horizontal rebar or the width of a vertical rebar may change,
	depending on the new layout. Returns VTRUE if a layout change occurred.*/
	VBOOL		SizeToRect(VRect const& r) const
		{ return VWINDOW_SMB2(RB_SIZETORECT, 0, r.GetPointer()); }

	/** Retrieve the current text color.*/
	COLORREF	TextColor() const
		{ return (COLORREF)VWINDOW_SM0(RB_GETTEXTCOLOR); }

	/** Set the current text color. Returns a COLORREF value that represents
	the previous default text color.*/
	COLORREF	TextColor(COLORREF crText) const
		{ return (COLORREF)VWINDOW_SM2(RB_SETTEXTCOLOR, 0, crText); }

	/** Retrieves the handle to the tooltip control associated with a rebar
	control. Returns the handle to the tooltip control if successful, or
	NULL otherwise. A rebar control creates a tooltip control if it has the
	RBS_TOOLTIPS style.*/
	HWND		ToolTips() const
		{ return (HWND)VWINDOW_SM0(RB_GETTOOLTIPS); }

	/** Set the tooltip control associated with a rebar control. This can be
	a VToolTip object.*/
	void		ToolTips(VWindow const& wndToolTips) const
		{ VWINDOW_SM1(RB_SETTOOLTIPS, wndToolTips.GetHandle()); }

	/** Toggle the specified rebar band. The rebar band is identified by the
	ID of the band or by the zero based index of the band. This method will
	first translate the nID parameter to the corresponding band index. If
	there is no corresponding band index, the method will assume that nID
	is a band index. In all cases, this function will show this rebar
	control if it is hidden.*/
	void		ToggleBand(VUINT nID) const
	{
		if ( IsBandVisibleEx(nID) )
			HideBandEx(nID);
		else
			ShowBandEx(nID);
	}

protected:
	/** Translate the index of a band into the HWND of the associated child
	window.*/
	HWND			IndexToHWND(VUINT nIndex) const
	{
		/* Setup the REBARBANDINFO structure.*/
		REBARBANDINFO rbi;
		VZEROSTRUCT(rbi);
		rbi.cbSize = sizeof(REBARBANDINFO);
		rbi.fMask  = RBBIM_CHILD;

		if ( VWINDOW_SM2(RB_GETBANDINFO, nIndex, &rbi) )
			return rbi.hwndChild;

		return NULL;
	}

	/** Sent by a rebar control created with the RBS_AUTOSIZE style when the
	rebar automatically resizes itself. Return 0 if message was handled, -1
	if handled and parent should NOT be notified, or 1 if message was not
	handled and parent should be notified.*/
	virtual VLONG	OnAutoSize(NMRBAUTOSIZE& nmrbAutoSize)
		{ return 1; }

	/** Sent by a rebar control when the user begins dragging a band. Return
	0 if message was handled, -1 if handled and parent should NOT be
	notified, or 1 if message was not handled and parent should be notified.
	If -1 is returned, set lCommonControlResult to zero to allow the rebar
	to continue the drag operation, or nonzero to abort the drag operation.*/
	virtual VLONG	OnBeginDrag(	NMREBAR& nmReBar,
									VLPARAM& lCommonControlResult)
		{ return 1; }

	/** Sent by a rebar control when a band's child window is resized. Return
	0 if message was handled, -1 if handled and parent should NOT be
	notified, or 1 if message was not handled and parent should be notified.*/
	virtual VLONG	OnChildSize(NMREBARCHILDSIZE& nmReBarChildSize)
		{ return 1; }

	/** Sent by a rebar control after a band has been deleted. Return 0 if
	message was handled, -1 if handled and parent should NOT be notified,
	or 1 if message was not handled and parent should be notified.*/
	virtual VLONG	OnDeletedBand(NMREBAR& nmReBar)
		{ return 1; }

	/** Sent by a rebar control when a band is about to be deleted. Return 0
	if message was handled, -1 if handled and parent should NOT be notified,
	or 1 if message was not handled and parent should be notified.*/
	virtual VLONG	OnDeletingBand(NMREBAR& nmReBar)
		{ return 1; }

	/** Sent by a rebar control when the user stops dragging a band. Return 0
	if message was handled, -1 if handled and parent should NOT be notified,
	or 1 if message was not handled and parent should be notified.*/
	virtual VLONG	OnEndDrag(NMREBAR& nmReBar)
		{ return 1; }

	/** Sent by a rebar control when its height has changed. The default
	implementation provided by the VReBar class will make sure that the
	client area of the parent window is repainted when the height of the
	rebar changes. Return 0 if message was handled, -1 if handled and
	parent should NOT be notified, or 1 if message was not handled and
	parent should be notified.*/
	virtual VLONG	OnHeightChange(NMHDR& nmhdr)
	{
		/* Get the parent VWindow object.*/
		VWindow* pParent = GetVParent();
		VASSERT(pParent)

		if ( pParent )
		{
			VRect r;
			pParent->GetClientRect(r);
			pParent->InvalidateRect(r);
		}

		/* Message handled.*/
		return -1;
	}

	/** Sent by a rebar control when the user changes the layout of the
	control's bands. Return 0 if message was handled, -1 if handled and
	parent should NOT be notified, or 1 if message was not handled and
	parent should be notified.*/
	virtual VLONG	OnLayoutChanged(NMHDR& nmhdr)
		{ return 1; }

	/** Called when the rebar window receives a WM_PAINT message. The default
	implementation provided by the VReBar class will make sure that any
	child windows associated with bands in the rebar are also painted
	properly.Return VTRUE if VWCL should continue routing the message to
	Windows so that subclassed windows can perform their own painting. If
	VFALSE is returned, it is assumed a derived class override has performed
	all required painting.*/
	virtual VBOOL	OnPaint(VDC& dc)
	{
		REBARBANDINFO rbi;

		/* Get the number of bands in the rebar control.*/
		VUINT nCount = GetBandCount();

		for ( VUINT nIndex = 0; nIndex < nCount; nIndex++ )
		{
			/* Initialize structure.*/
			VZEROSTRUCT(rbi);
			rbi.fMask = RBBIM_CHILD;

			/* Get the band information.*/
			if ( GetBandInfo(nIndex, rbi) && rbi.hwndChild )
				::InvalidateRect(rbi.hwndChild, NULL, VTRUE);
		}

		return VTRUE;
	}

	/** Override watches for notifications to perform extended operations.
	This method is called by the parent window when it receives a
	WM_NOTIFY message from a common control. Return 0 if message was
	handled, -1 if handled and parent should NOT be notified, or 1 if
	message was not handled and parent should be notified. If -1 is
	returned, set the lCommonControlResult to the return value expected by
	the common control.*/
	virtual VLONG	OnReflectedNotify(	NMHDR&	nmhdr,
										VLONG&	lCommonControlResult)
	{
		/* Select operation by notification code.*/
		switch (nmhdr.code )
		{
			case RBN_AUTOSIZE:
				return OnAutoSize(VREF_PTR((NMRBAUTOSIZE*)&nmhdr));

			case RBN_CHILDSIZE:
				return OnChildSize(VREF_PTR((NMREBARCHILDSIZE*)&nmhdr));

			case RBN_BEGINDRAG:
				return OnBeginDrag(	VREF_PTR((NMREBAR*)&nmhdr),
									lCommonControlResult);

			case RBN_ENDDRAG:
				return OnEndDrag(VREF_PTR((NMREBAR*)&nmhdr));

			case RBN_DELETEDBAND:
				return OnDeletedBand(VREF_PTR((NMREBAR*)&nmhdr));

			case RBN_DELETINGBAND:
				return OnDeletingBand(VREF_PTR((NMREBAR*)&nmhdr));

			case RBN_HEIGHTCHANGE:
				return OnHeightChange(nmhdr);

			case RBN_LAYOUTCHANGED:
				return OnLayoutChanged(nmhdr);
		}

		/* Message not handled, notify parent.*/
		return VCommonControl::OnReflectedNotify(nmhdr, lCommonControlResult);
	}
};

#endif /* VREBAR*/
