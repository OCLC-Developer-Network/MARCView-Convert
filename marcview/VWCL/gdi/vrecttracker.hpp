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

#ifndef VRECTTRACKER
#define VRECTTRACKER

#include "../vstandard.h"
#include "../numbers/vbitmask.hpp"
#include "../structures/vpoint.hpp"
#include "../structures/vrect.hpp"
#include "../utility/vcapturedmousetracker.hpp"
#include "../windows/vwindow.hpp"
#include "vdc.hpp"
#include "vclientdc.hpp"
#include "vbrush.hpp"
#include "vgdiobjectselector.hpp"
#include "vpen.hpp"

/** VRectTracker supports framing, object selection, and movement.*/
class VRectTracker : public VCapturedMouseTracker
{
public:
	/** Default constructor initializes the class and create default or user
	specified GDI objects needed for drawing.*/
	VRectTracker(	VRect const*	pRectInitial = NULL,
					VINT			nBorderPenStyle = PS_DOT,
					VINT			nBorderPenWidth = 0,
					COLORREF		crBorderPenColor = 0,
					VUINT			nOptions = OPTION_DEFAULT,
					VUSHORT			nKeyboardIncrement = 5)
		: VCapturedMouseTracker(nOptions, nKeyboardIncrement)
	{
		/* Initialize.*/
		m_BorderPen.Create(	nBorderPenStyle,
							nBorderPenWidth,
							crBorderPenColor);

		if ( pRectInitial )
			m_Rect = VREF_PTR(pRectInitial);

		ResetTempMembers();
	}

	/** Allow user to drag the rectangle with the mouse. Returns VTRUE if
	function completes, or VFALSE if the users cancelled dragging by using
	one of the supported cancel methods of the VCapturedMouseTracker base
	class, of if the resulting rectangle has no area. On a VTRUE result,
	the internal rectangle object will be normalized so that left and top
	is greater than the right and bottom.*/
	VBOOL			Drag(	VWindow const&	window,
							VPoint const&	ptFrom)
	{
		VASSERT(window.GetSafeWindow())

		/* Temporarily disable drawing.*/
		m_Options.Save();
		m_Options.Append(OPTION_RECT_TRACKER_DISABLE_DRAWING);

		/* Update screen (erase current rectangle).*/
		window.InvalidateRectUpdate(m_Rect);

		/* Re-Enable old settings. TrackOrDrag() will cause the
		rect to be redrawn using an XOR pen.*/
		m_Options.Restore();

		return TrackOrDrag(window, ptFrom, VFALSE, VFALSE);
	}

	/** Used for drawing from outside of class.*/
	void			Draw(VDC& dc)
	{
		m_pDC = &dc;
		InternalDraw(VFALSE);
		m_pDC = NULL;
	}

	/** Return a reference to the brush used for drawing the rectangle fill.
	This brush is not initialized during construction, but can be modified
	by client code.*/
	VBrush&			GetFillBrush() const
		{ return (VBrush&)m_FillBrush; }

	/** Return a reference to the pen used for drawing the border. This pen
	is initialized during construction, but can be modified by client code.*/
	VPen&			GetBorderPen() const
		{ return (VPen&)m_BorderPen; }

	/** Return a reference to the optional rectangle that can be filled with
	minumum and maximum values where the user can move, or rubber band a
	rectangle. In addition to this rect being filled, you must also set the
	OPTION_RECT_TRACKER_ENFORCE_LIMITS flag via GetOptions().*/
	VRect&			GetLimitsRect() const
		{ return (VRect&)m_rLimits; }

	/** Get the internal VRect object that represents the rectangle being
	moved or tracked.*/
	VRect&			GetRect() const
		{ return (VRect&)m_Rect; }

	/** Track using a "rubber banding" metaphor. Good for visual object
	selection where the user drags a rectangle around one or more objects.
	Returns VTRUE if the function completes, or VFALSE if the user
	cancelled tracking by using one of the supported cancel methods of the
	VCapturedMouseTracker base class, of if the resulting rectangle has no
	area. On a VTRUE result, the internal rectangle object will be
	normalized so that left and top is greater than the right and bottom. If
	bEraseRectWhenDone is VTRUE, the tracker rectangle will be erased when
	done.*/
	VBOOL			TrackRubberBand(	VWindow const&	window,
										VPoint const&	ptFrom,
										VBOOL			bEraseRectWhenDone =
														VTRUE)
		{ return TrackOrDrag(window, ptFrom, VTRUE, bEraseRectWhenDone); }

protected:
	/** Called to draw tracker. This function can use XOR drawing when
	tracking so calling Draw() a second time erases the first draw
	operation.*/
	void			InternalDraw(VBOOL bXOR)
	{
		VASSERT(m_pDC && m_pDC->GetHandle())

		if (	!m_Rect.IsEmpty() &&
				m_Options.IsNotSet(OPTION_RECT_TRACKER_DISABLE_DRAWING) )
		{
			VINT nOldROP;

			if ( bXOR )
				nOldROP = m_pDC->ROP2(R2_NOTXORPEN);

			/* Setup DC.*/
			VGDIObjectSelector os1(VREF_PTR(m_pDC), m_BorderPen);
			VINT nOldBkMode = m_pDC->BackgroundMode(TRANSPARENT);

			/* Draw rectangle.*/
			if ( m_FillBrush.GetHandle() )
			{
				VGDIObjectSelector os2(VREF_PTR(m_pDC), m_FillBrush);
				m_pDC->Rectangle(m_Rect);
			}
			else
				m_pDC->RectangleEx(m_Rect);

			/* Clean up DC.*/
			m_pDC->BackgroundMode(nOldBkMode);

			if ( bXOR )
				m_pDC->ROP2(nOldROP);
		}
	}

	/** This function gets called during tracking and dragging. It is valid
	to modify the points in ptCurrent if desired. On returning from this
	function, if the points are modified, the cursor position will be set
	to the new coordinates.*/
	virtual void	OnTrack(	VWindow const&	window,
								VPoint const&	ptFrom,
								VPoint&			ptCurrent)
	{
		/* Calculate and set new rectangle position.*/
		VRect rNew;

		if ( m_bTracking )
		{
			/* Validate and set new points.*/
			rNew.GetLeft() = ptFrom.GetX();
			rNew.GetTop() = ptFrom.GetY();

			rNew.GetRight() =
				(m_Options.IsSet(OPTION_ALLOW_INVERTED_TRACKING))
				? ptCurrent.GetX()
				: VMAX(ptCurrent.GetX(), m_Rect.GetLeft());

			rNew.GetBottom() =
				(m_Options.IsSet(OPTION_ALLOW_INVERTED_TRACKING))
				? ptCurrent.GetY()
				: VMAX(ptCurrent.GetY(), m_Rect.GetTop());
		}
		else
		{
			rNew.Set(m_Rect);

			if ( m_Options.IsNotSet(OPTION_RECT_TRACKER_FIX_RECT_LEFT) )
				rNew.GetLeft() =
					ptCurrent.GetX() - m_nxOffSet;

			if ( m_Options.IsNotSet(OPTION_RECT_TRACKER_FIX_RECT_TOP) )
				rNew.GetTop() =
					ptCurrent.GetY() - m_nyOffSet;

			if ( m_Options.IsNotSet(OPTION_RECT_TRACKER_FIX_RECT_RIGHT) )
				rNew.GetRight() =
					ptCurrent.GetX() - m_nxOffSet + m_Rect.GetWidth();

			if ( m_Options.IsNotSet(OPTION_RECT_TRACKER_FIX_RECT_BOTTOM) )
				rNew.GetBottom() =
					ptCurrent.GetY() - m_nyOffSet + m_Rect.GetHeight();
		}

		/* If limits are being used, enforce them also.*/
		if ( m_Options.IsSet(OPTION_RECT_TRACKER_ENFORCE_LIMITS) )
		{
			VLONG nWidth =	rNew.GetWidth();
			VLONG nHeight =	rNew.GetHeight();

			if ( rNew.GetLeft() < m_rLimits.GetLeft() )
			{
				rNew.GetLeft() =	m_rLimits.GetLeft();
				rNew.GetRight() =	rNew.GetLeft() + nWidth;
			}

			if ( rNew.GetTop() < m_rLimits.GetTop() )
			{
				rNew.GetTop() =		m_rLimits.GetTop();
				rNew.GetBottom() =	rNew.GetTop() + nHeight;
			}

			if ( rNew.GetRight() > m_rLimits.GetRight() )
			{
				rNew.GetRight() =	m_rLimits.GetRight();
				rNew.GetLeft() =	m_rLimits.GetRight() - nWidth;
			}

			if ( rNew.GetBottom() > m_rLimits.GetBottom() )
			{
				rNew.GetBottom() =	m_rLimits.GetBottom();
				rNew.GetTop() =		m_rLimits.GetBottom() - nHeight;
			}
		}

		/* Did the rectangle change?*/
		if ( m_Rect != rNew )
		{
			/* Draw to erase old rectangle (using XOR).*/
			InternalDraw(VTRUE);

			/* Save new rectangle and normalize it if tracking.*/
			m_Rect.Set(rNew);

			if ( m_bTracking )
				m_Rect.Normalize();

			/* Draw new rect (Using XOR).*/
			InternalDraw(VTRUE);
		}
	}

	/** Reset internal temporary members to default values.*/
	void			ResetTempMembers()
	{
		m_pDC =			NULL;
		m_bTracking =	VFALSE;
		m_nxOffSet =	0;
		m_nyOffSet =	0;
	}

	/** Track or drag the tracker.*/
	VBOOL			TrackOrDrag(	VWindow const&	window,
									VPoint const&	ptFrom,
									VBOOL			bTracking,
									VBOOL			bEraseRectWhenDone)
	{
		VASSERT(window.GetSafeWindow())

		VClientDC dc(window);

		if ( !dc.GetHandle() )
			return VFALSE;

		/* Save pointer to DC.*/
		m_pDC = &dc;

		/* Save tracking flag.*/
		m_bTracking = bTracking;

		/* If we are going to drag a rect, draw it now using the XOR pen
		since it was erased in the Drag() function prior to it calling
		this function.*/
		if ( !bTracking )
			InternalDraw(VTRUE);

		/* rOriginal, m_nxOffSet, and m_nyOffSet are only valid
		while dragging.*/
		VRect rOriginal;

		/* Initialize variables based on current action.*/
		if ( bTracking )
			m_Rect.Set(VPOINT_BREAK(ptFrom), VPOINT_BREAK(ptFrom));
		else
		{
			rOriginal = m_Rect;

			/* Save offset of mouse position from tracker when dragging.*/
			m_nxOffSet = ptFrom.GetX() - m_Rect.GetLeft();
			m_nyOffSet = ptFrom.GetY() - m_Rect.GetTop();
		}

		VBOOL bResult =
			VCapturedMouseTracker::Track(window, ptFrom, VPOINT_EMPTY);

		/* Tracking or dragging is now done. Check results.*/
		if ( bTracking )
		{
			/* If rect is empty, bResult should be VFALSE after tracking.*/
			if ( bResult && m_Rect.IsEmpty() )
				bResult = VFALSE;
		}
		else if ( bResult )
		{
			/* Did the rectangle change while being dragged?*/
			if ( m_Rect == rOriginal )
				bResult = VFALSE;
		}

		if ( !bResult )
		{
			/* Erase current rectangle.*/
			InternalDraw(VTRUE);

			/* If we were dragging, copy original rect and update.*/
			if ( !bTracking )
			{
				m_Rect = rOriginal;
				window.InvalidateRect(m_Rect);
			}
		}
		else if ( bEraseRectWhenDone )
			InternalDraw(VTRUE);

		/* If we were dragging, draw to fix XOR painting.*/
		if ( !bTracking )
			InternalDraw(VFALSE);

		/* Reset temps.*/
		ResetTempMembers();

		return bResult;
	}

	/** Embedded Members.*/
	VDC*			m_pDC;
	VRect			m_Rect;
	VRect			m_rLimits;
	VPen			m_BorderPen;
	VBrush			m_FillBrush;
	VBOOL			m_bTracking;
	VINT			m_nxOffSet;
	VINT			m_nyOffSet;
};

#endif /* VRECTTRACKER*/
