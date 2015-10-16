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
Other Author(s) of this source code file:	Daniel Halan
											(daniel@image-research.se)

VWCL and all source code are copyright (c) 1996-2000 by The VWCL Alliance.*/

#ifndef VGAUGE
#define VGAUGE

#include "../vstandard.h"
#include "../numbers/vbitmask.hpp"
#include "../structures/vrect.hpp"
#include "vdc.hpp"
#include "v3drect.hpp"

/** VGauge fills a rect with color as progress is made. This class assumes
a value of 0 means the gauge should not display any filled area, and a
value of 100 is a completely filled gauage. This class uses a floating
point calculation to determine the filling of the gauge, and is thus very
accurate. The class is well suited for a progress bar, and supports calling
the OnDrawComplete() virtual function to allow drawing of text or other
objects after the Draw() function completes.

The gauge has two basic methods of use. In the first method, the filled
part of the gauge is determined by a percentage value of 0 to 100. You
specifiy the current percentage using the Percent() function and the class
fills the rectangle with that amount of filled area.

The other method is a wrapping of the first. In this method, you specify
the minimum and maximum values in a range using the GetMin(), GetMax() or
SetMinMax() functions, and the percentage is calulated during the Position()
call. This allows the gauge to function relative to some other range, not
simply the rectangle size.

You can use either method and intermix the two, as they both keep the class
internal members in a consistent state.*/
class VGauge : public V3DRect
{
public:
	/** Construct with existing X, Y, and CX, CY coordinates, or default of
	0, 0, 0, 0. Can also be initialized with class options (which default
	to OPTION_GAUGE_DEFAULT), the rectangle fill color (defaults to gray,
	the pen color to use on the left and top edges of the rectangle
	(defaults to dark gray), the pen color to use on the right and bottom
	edges of the rectangle (defaults to white), the border width (defaults
	to 1 pixel), and the gauage fill color (defaults to blue).*/
	VGauge(	VLONG		nx = 0,
			VLONG		ny = 0,
			VLONG		ncx = 0,
			VLONG		ncy = 0,
			VUINT		nOptions = OPTION_GAUGE_DEFAULT,
			COLORREF	crFill = RGB(192,192,192),
			COLORREF	crLeftTop = RGB(127,127,127),
			COLORREF	crRightBottom = RGB(255,255,255),
			VUINT		nBorderWidth = 1,
			COLORREF	crGaugeColor = RGB(0,0,255))
		: V3DRect(	nx,
					ny,
					ncx,
					ncy,
					nOptions,
					crFill,
					crLeftTop,
					crRightBottom,
					nBorderWidth)
		{ ResetMembers(crGaugeColor); }

	/** Construct with existing VRect object. Can also be initialized with
	class options (which default to OPTION_GAUGE_DEFAULT), the rectangle
	fill color (defaults to gray, the pen color to use on the left and top
	edges of the rectangle (defaults to dark gray), the pen color to use on
	the right and bottom edges of the rectangle (defaults to white), the
	border width (defaults to 1 pixel), and the gauage fill color (defaults
	to blue).*/
	VGauge(	VRect const&	r,
			VUINT			nOptions = OPTION_GAUGE_DEFAULT,
			COLORREF		crFill = RGB(192,192,192),
			COLORREF		crLeftTop = RGB(127,127,127),
			COLORREF		crRightBottom = RGB(255,255,255),
			VUINT			nBorderWidth = 1,
			COLORREF		crGaugeColor = RGB(0,0,255))
		: V3DRect(	r,
					nOptions,
					crFill,
					crLeftTop,
					crRightBottom,
					nBorderWidth)
		{ ResetMembers(crGaugeColor); }

	/** Construct with existing RECT structure. Can also be initialized with
	class options (which default to OPTION_GAUGE_DEFAULT), the rectangle
	fill color (defaults to gray, the pen color to use on the left and top
	edges of the rectangle (defaults to dark gray), the pen color to use on
	the right and bottom edges of the rectangle (defaults to white), the
	border width (defaults to 1 pixel), and the gauage fill color (defaults
	to blue).*/
	VGauge(	RECT const&	r,
			VUINT		nOptions = OPTION_GAUGE_DEFAULT,
			COLORREF	crFill = RGB(192,192,192),
			COLORREF	crLeftTop = RGB(127,127,127),
			COLORREF	crRightBottom = RGB(255,255,255),
			VUINT		nBorderWidth = 1,
			COLORREF	crGaugeColor = RGB(0,0,255))
		: V3DRect(	r,
					nOptions,
					crFill,
					crLeftTop,
					crRightBottom,
					nBorderWidth)
		{ ResetMembers(crGaugeColor); }

	/** Draw the gauge.*/
	virtual void	Draw(VDC& dc)
	{
		if ( !IsEmpty() )
		{
			/* Save current rectangle.*/
			VRect rOriginal(VREF_THIS);

			/* Save current options.*/
			m_Options.Save();

			/* Verify proper options bits are set.*/
			m_Options.Append(OPTION_SHRINK_RECT_ON_DRAW | OPTION_NO_FILL);

			/* Draw (this will shrink the rect).*/
			V3DRect::Draw(dc);

			/* Initialize rectangles now.*/
			VRect rInsideBorders(VREF_THIS);
			VRect rFilledArea;
			VRect rUnFilledArea;

			/* At 0%?*/
			if ( m_nPercent )
			{
				/* Initialize filled area.*/
				rFilledArea = rInsideBorders;

				/* At 100%? If yes, the gauge will consume the
				entire remaining rectangle.*/
				if ( m_nPercent != 100 )
				{
					if ( m_Options.IsSet(OPTION_GAUGE_ORIENT_VERTICAL) )
					{
						VFLOAT nPixelsPerTick =
							(VFLOAT)((VFLOAT)rFilledArea.GetHeight()) / 100.0f;

						if ( m_Options.IsSet(OPTION_GAUGE_TOP_TO_BOTTOM) )
							rFilledArea.GetBottom() =
								VMIN(rFilledArea.GetTop()
								+ ((VLONG)(nPixelsPerTick
								* (VFLOAT)m_nPercent)),
								GetBottom());
						else
							rFilledArea.GetTop() =
								VMAX(rFilledArea.GetBottom()
								- (VLONG)(nPixelsPerTick
								* (VFLOAT)m_nPercent),
								GetTop());
					}
					else
					{
						VFLOAT nPixelsPerTick =
							(VFLOAT)((VFLOAT)rFilledArea.GetWidth()) / 100.0f;

						if ( m_Options.IsSet(OPTION_GAUGE_RIGHT_TO_LEFT) )
							rFilledArea.GetLeft() =
								VMAX(rFilledArea.GetRight()
								- (VLONG)(nPixelsPerTick
								* (VFLOAT)m_nPercent),
								GetLeft());
						else
							rFilledArea.GetRight() =
								VMIN(rFilledArea.GetLeft()
								+ (VLONG)(nPixelsPerTick
								* (VFLOAT)m_nPercent),
								GetRight());
					}
				}

				/* Display filled rectangle?*/
				if ( !rFilledArea.IsEmpty() )
				{
					VBrush brush;

					if ( brush.CreateSolid(m_crGaugeColor) )
						brush.FillRect(dc, rFilledArea);
				}
			}

			/* Now fill the area we did not fill with the gauge.*/
			if ( rFilledArea.IsEmpty() )
			{
				Fill(dc);
				rUnFilledArea = rInsideBorders;
			}
			else if (	rUnFilledArea.Subtract(rInsideBorders, rFilledArea) &&
						!rUnFilledArea.IsEmpty() )
			{
				Set(rUnFilledArea);
				Fill(dc);
			}

			/* Restore user options.*/
			m_Options.Restore();

			/* Reset original rectangle, or if we should shrink on draw,
			set to rInsideBorders.*/
			if ( m_Options.IsSet(OPTION_SHRINK_RECT_ON_DRAW) )
				Set(rInsideBorders);
			else
				Set(rOriginal);

			/* Call the OnDrawComplete() virtual function.*/
			OnDrawComplete(dc, rInsideBorders, rFilledArea, rUnFilledArea);
		}
	}

	/** Return a reference to the gauge color. (Defaults to RGB(0,0,255)
	which is blue).*/
	COLORREF&		GetGaugeColor() const
		{ return (COLORREF&)m_crGaugeColor; }

	/** Return a reference to the specified minimum value.*/
	VUINT&			GetMin() const
		{ return (VUINT&)m_nMinPos; }

	/** Return a reference to the specified maximum value.*/
	VUINT&			GetMax() const
		{ return (VUINT&)m_nMaxPos; }

	/** Set the gauge percentage (0 to 100) is valid. If nPercent is over
	100, it will be set to 0. If pDC is not NULL, the gauge will be redrawn
	to reflect the change.*/
	void			Percent(	VBYTE	nPercent,
								VDC*	pDC = NULL)
	{
		/* Do nothing?*/
		if ( nPercent != m_nPercent )
		{
			/* Reset?*/
			m_nPercent = (nPercent > 100) ? 0 : nPercent;

			/* Calculate position.*/
			m_nPosition =
				(VUINT)(((m_nMaxPos - m_nMinPos) *
				((VDOUBLE)m_nPercent /
				(VDOUBLE)100)));

			if ( pDC )
				Draw(VREF_PTR(pDC));
		}
	}

	/** Return the current gauge percentage.*/
	VBYTE			Percent() const
		{ return m_nPercent; }

	/** Set the position value, which is in the range specified
	by SetMinMax().*/
	void			Position(	VUINT	nPosition,
								VDC*	pDC = NULL)
	{
		if ( nPosition != m_nPosition )
		{
			if ( (nPosition < m_nMinPos) || (nPosition > m_nMaxPos) )
			{
				m_nPercent =	0;
				m_nPosition =	m_nMinPos;
			}
			else
			{
				m_nPosition = nPosition;
				m_nPercent =
					(nPosition)
					? (VUINT)(((VDOUBLE)(m_nPosition - m_nMinPos) /
					(VDOUBLE)(m_nMaxPos - m_nMinPos)) * 100)
					: 0;
			}

			if ( pDC )
				Draw(VREF_PTR(pDC));
		}
	}

	/** Return the current position value.*/
	VUINT			Position() const
		{ return m_nPosition; }

	/** Sets the minimum and maximum values. Used only when the Position()
	function is used.*/
	void			SetMinMax(	VUINT nMinPos,
								VUINT nMaxPos)
	{
		m_nMinPos = nMinPos;
		m_nMaxPos = nMaxPos;
	}

	/** Step the percentage by 1. If the percentage is over 100, it will be
	set to 0. If pDC is not NULL, the gauge will be redrawn to reflect the
	change.*/
	void			StepIt(VDC* pDC = NULL)
		{ Percent(m_nPercent + 1, pDC); }

protected:
	/** This virtual function is called after the drawing of the rectangle
	and gauge is complete. The rectangles passed to this function are
	defined as follows:

	rInsideBoreders - The area of the rectangle inside the border area
	defined by the base V3DRect class.

	rFilledArea - The portion of the rectangle consumed by the filled
	region, based on percentage or position.

	rUnFilledArea - The area not filled by rFilledArea.

	This is an appropriate time to draw custom text or other objects into
	the available rectangle area.*/
	virtual void	OnDrawComplete(	VDC&	dc,
									VRect&	rInsideBorders,
									VRect&	rFilledArea,
									VRect&	rUnFilledArea)
		{;}

	/** Local function to initialize members.*/
	void			ResetMembers(COLORREF crGaugeColor)
	{
		m_crGaugeColor =	crGaugeColor;
		m_nPercent =		0;
		m_nPosition =		0;
		m_nMinPos =			0;
		m_nMaxPos =			100;
	}

	/** Embedded Members.*/
	COLORREF		m_crGaugeColor;
	VBYTE			m_nPercent;
	VUINT			m_nPosition;
	VUINT			m_nMinPos;
	VUINT			m_nMaxPos;
};

#endif /* VGAUGE*/
