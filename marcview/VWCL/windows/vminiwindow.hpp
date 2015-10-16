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

#ifndef VMINIWINDOW
#define VMINIWINDOW

#include "../vstandard.h"
#include "../gdi/vbrush.hpp"
#include "../gdi/vdc.hpp"
#include "../gdi/vgdiobjectselector.hpp"
#include "../gdi/vstockfont.hpp"
#include "../numbers/vrtti.hpp"
#include "../structures/vpoint.hpp"
#include "../structures/vrect.hpp"
#include "vwindow.hpp"

/** VMiniWindow displays a window with a small caption bar which defaults
to 12 pixels high. These windows are all client area, and have no real
caption bar. The base class draws the caption in dark blue with white text
in the caption using the ANSI_VAR_FONT font.*/
class VMiniWindow : public VWindow
{
public:
	/** Default constructor initializes the RTTI information in the VWindow
	base class. It also sets the VWindow option OPTION_NEED_ON_PAINT to
	that the OnPaint() notification is sent. This is where the drawing of
	the caption area takes place.*/
	VMiniWindow(VRTTI_TYPE nRTTI = VWCL_RTTI_MINI_WINDOW)
		: VWindow(nRTTI)
		{ m_Options = OPTION_NEED_ON_PAINT; }

	/** Override gets the caption rectangle area. This function uses a
	12 pixel caption area. Override to change.*/
	virtual void	GetCaptionRect(VRect& r)
	{
		GetClientRect(r);
		r.GetBottom() = 12;
	}

	/** Determine if the point at pt is in the dragging area. This is the
	fake caption area that we draw. Since the caption is drawn by us, and
	the window does not have the WS_CAPTION style set, we have to be able
	to determine when the user should be able to drag the window by
	implementing this caption area. This function will return VTRUE is pt
	is in the fake caption area, VFALSE otherwise.*/
	VBOOL			IsPointInClientArea(VPoint const& pt)
	{
		VRect r;
		GetCaptionRect(r);

		return r.IsPointInside(pt);
	}

protected:
	/** Override enforces correct window styles for mini-caption support.*/
	virtual VBOOL	PreCreateWindow(CREATESTRUCT& CreateStruct)
	{
		CreateStruct.style &= ~WS_CAPTION;
		CreateStruct.style |= WS_CLIPSIBLINGS;

		return VWindow::PreCreateWindow(CreateStruct);
	}

	/** Override paints the custom caption area.*/
	virtual VBOOL	OnPaint(VDC& dc)
	{
		/* Get the caption rectangle.*/
		VRect r;
		GetCaptionRect(r);

		VBrush br;

		/* Create a dark blue brush to use for caption background
		and select into the DC.*/
		if ( br.CreateSolid(0, 0, 127) )
		{
			br.FillRect(dc, r);

			/* Get and draw text.*/
			VString s;

			if ( GetText(s) )
			{
				VINT nOldBkMode =			dc.BackgroundMode(TRANSPARENT);
				COLORREF crOldTextColor =	dc.TextColor(RGB(255,255,255));

				/* Select a white color and set text options.*/
				VStockFont			font(ANSI_VAR_FONT);
				VGDIObjectSelector	os(dc, font);
				dc.DrawText(s, r, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

				/* Reset DC.*/
				dc.BackgroundMode(nOldBkMode);
				dc.TextColor(crOldTextColor);
			}
		}

		return VWindow::OnPaint(dc);
	}

	/** Window procedure looks for WM_NCHITTEST and calls
	IsPointInClientArea() as appropriate.*/
	virtual VLONG	WindowProc(	HWND	hWnd,
								VUINT	nMessage,
								VWPARAM	wParam,
								VLPARAM	lParam)
	{
		if ( nMessage == WM_NCHITTEST )
		{
			/* Translate coordinates from screen to client.*/
			VPoint pt(LOWORD(lParam), HIWORD(lParam));
			ScreenToClient(pt);

			/* Call virtual function to deterine if point is in a
			"drag area". If so, tell Windows the mouse is in a caption
			area (so it can be dragged).*/
			if ( IsPointInClientArea(pt) )
				return HTCAPTION;
		}

		return VWindow::WindowProc(hWnd, nMessage, wParam, lParam);
	}
};

#endif /* VMINIWINDOW*/
