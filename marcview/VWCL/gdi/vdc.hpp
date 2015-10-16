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

#ifndef VDCHPP
#define VDCHPP

#include "../vstandard.h"
#include "../structures/vpoint.hpp"
#include "../structures/vrect.hpp"

/** VDC is the base class for the wrapping of Windows Device Context's
(DC's). Other GDI objects are normally selected into VDC objects using the
VGDIObjectSelector class.*/
class VDC
{
public:
	/** Default constructor can be initialized with an existing device
	context. This DC, if specified, will be deleted with DeleteDC() when
	this object is destructed. If this is not the desired behaviour, a
	derived class can override the Free() function to perform needed
	cleanup. From outside the class, client code can also call
	GetHandle() = NULL to remove the internal HDC member value, thus
	preventing the class from attempting to delete the device context. The
	VTempDC class can also provide this functionality. Another way to
	prevent this base class from deleting the DC is by passing VFALSE for
	the bFreeDC parameter.*/
	VDC(	HDC		hDC = NULL,
			VBOOL	bFreeDC = VTRUE)
	{
		m_hDC =			(HDC)hDC;
		m_bFreeHDC =	bFreeDC;
		m_nSaveThis =	0;
	}

	/** Constructor must be initialized with an existing device context
	object. The existing device context handle (HDC) will not be Free'd by
	this instance of the class and is considered owned by another. If the
	instance of this object should indeed delete the HDC held in sourceDC,
	passing VTRUE for bFreeDC will make this object assume ownership, but
	will leave the HDC member of sourceDC in tact, but mark it so that it
	does not attempt to free the device context.*/
	VDC(	VDC&	sourceDC,
			VBOOL	bFreeDC = VFALSE)
	{
		m_hDC =			sourceDC.GetHandle();
		m_bFreeHDC =	bFreeDC;
		m_nSaveThis =	0;

		if ( bFreeDC )
			sourceDC.m_bFreeHDC = VFALSE;
	}

	/** Virtual destructor calls RestoreThis() and Free() in this class. See
	these functions for more information. Since this is a destructor, it
	will not be able to call derived class overrides of the Free() function.
	The base class Free() function attempts to delete the DC if owned by
	this class. If this is not the desired behaviour, from outside the
	class, client code can call GetHandle() = NULL to remove the internal
	HDC member value, thus preventing the class from attempting to delete
	the device context. The VTempDC class can also provide this
	functionality.*/
	virtual ~VDC()
	{
		if ( m_hDC )
		{
			RestoreThis();
			Free();
		}
	}

	/** Return the background mix mode of the device context. See the
	BackgroundMode() setter function for more information.*/
	VINT			BackgroundMode() const
	{
		VASSERT(m_hDC)
		return GetBkMode(m_hDC);
	}

	/** Sets the background mix mode of the device context. The background mix
	mode is used with text, hatched brushes, and pen styles that are not
	solid lines. nMode can be one of the following:

	OPAQUE - Background is filled with the current background color before
	the text, hatched brush, or pen is drawn.

	TRANSPARENT - Background remains untouched.

	The BackgroundMode() function affects the line styles for lines drawn
	using a pen created by the CreatePen() function. BackgroundMode() does
	not affect lines drawn using a pen created by the ExtCreatePen()
	function. The nMode parameter can also be set to driver-specific values.
	GDI passes such values to the device driver and otherwise ignores them.
	In Windows CE, the nMode parameter is stored internally as a single
	byte.*/
	VINT			BackgroundMode(VINT nMode) const
	{
		VASSERT(m_hDC)
		return SetBkMode(m_hDC, nMode);
	}

	/** The BitBlt function performs a bit-block transfer of the color data
	corresponding to a rectangle of pixels from the specified source device
	context into this device context. nXDest specifies the logical
	x-coordinate of the upper-left corner of the destination rectangle.
	nYDest specifies the logical y-coordinate of the upper-left corner of
	the destination rectangle. nWidth specifies the logical width of the
	source and destination rectangles. nHeight specifies the logical height
	of the source and the destination rectangles. sourceDC specifies the
	source device context. nXSrc specifies the logical x-coordinate of the
	upper-left corner of the source rectangle. nYSrc specifies the logical
	y-coordinate of the upper-left corner of the source rectangle. nROP
	specifies a raster-operation code. These codes define how the color
	data for the source rectangle is to be combined with the color data for
	the destination rectangle to achieve the final color. The following
	list shows some common raster operation codes:

	BLACKNESS - Fills the destination rectangle using the color associated
	with index 0 in the physical palette. (This color is black for the
	default physical palette.)

	DSTINVERT - Inverts the destination rectangle.

	MERGECOPY - Merges the colors of the source rectangle with the specified
	pattern by using the Boolean AND operator.

	MERGEPAINT - Merges the colors of the inverted source rectangle with
	the colors of the destination rectangle by using the Boolean OR
	operator.

	NOTSRCCOPY - Copies the inverted source rectangle to the destination.

	NOTSRCERASE - Combines the colors of the source and destination
	rectangles by using the Boolean OR operator and then inverts the
	resultant color.

	PATCOPY - Copies the specified pattern into the destination bitmap.

	PATINVERT - Combines the colors of the specified pattern with the
	colors of the destination rectangle by using the Boolean XOR operator.

	PATPAINT - Combines the colors of the pattern with the colors of the
	inverted source rectangle by using the Boolean OR operator. The result
	of this operation is combined with the colors of the destination
	rectangle by using the Boolean OR operator.

	SRCAND - Combines the colors of the source and destination rectangles
	by using the Boolean AND operator.

	SRCCOPY - Copies the source rectangle directly to the destination
	rectangle.

	SRCERASE - Combines the inverted colors of the destination rectangle
	with the colors of the source rectangle by using the Boolean AND
	operator.

	SRCINVERT - Combines the colors of the source and destination rectangles
	by using the Boolean XOR operator.

	SRCPAINT - Combines the colors of the source and destination rectangles
	by using the Boolean OR operator.

	WHITENESS - Fills the destination rectangle using the color associated
	with index 1 in the physical palette. (This color is white for the
	default physical palette.)

	If a rotation or shear transformation is in effect in the source device
	context, BitBlt() returns an error. If other transformations exist in the
	source device context (and a matching transformation is not in effect
	in the destination device context), the rectangle in the destination
	device context is stretched, compressed, or rotated as necessary. If
	the color formats of the source and destination device contexts do not
	match, the BitBlt() function converts the source color format to match
	the destination format. When an enhanced metafile is being recorded,
	an error occurs if the source device context identifies an
	enhanced-metafile device context. Not all devices support the BitBlt()
	function. For more information, see the RC_BITBLT raster capability
	entry in the GetDeviceCaps() function as well as the following
	functions: MaskBlt(), PlgBlt(), StretchBlt(). BitBlt() returns an error
	if the source and destination device contexts represent different
	devices. In Windows CE 1.0, the nROP parameter can only be assigned the
	following values: SRCCOPY, SRCAND, SRCPAINT, and SRCINVERT. In Windows
	CE version 2.0, the nROP parameter can be any ROP3.*/
	VBOOL			BitBlt(	VINT	nXDest,
							VINT	nYDest,
							VINT	nWidth,
							VINT	nHeight,
							VDC&	sourceDC,
							VINT	nXSrc = 0,
							VINT	nYSrc = 0,
							VUINT	nROP = SRCCOPY) const
	{
		VASSERT(m_hDC && sourceDC.m_hDC)

		return ::BitBlt(	m_hDC,
							nXDest,
							nYDest,
							nWidth,
							nHeight,
							sourceDC.m_hDC,
							nXSrc,
							nYSrc,
							nROP);
	}

	/** Same as above, but uses a VRect object to get the destination
	coordinates.*/
	VBOOL			BitBlt(	VRect const&	r,
							VDC&			sourceDC,
							VINT			nXSrc = 0,
							VINT			nYSrc = 0,
							VUINT			nROP = SRCCOPY) const
	{
		return BitBlt(	VRECT_BREAK_WIDTH_HEIGHT(r),
						sourceDC,
						nXSrc,
						nYSrc,
						nROP);
	}

	/** Create a memory device context compatible with this object's DC,
	placing the result in dcDestination. Returns VTRUE on success, VFALSE
	on failure. On failure, the destinations HDC member variable is not
	touched.*/
	VBOOL			CreateCompatible(VDC& dcDestination) const
	{
		HDC hDC = CreateCompatibleDC(m_hDC);

		if ( hDC )
		{
			dcDestination.m_hDC = hDC;
			return VTRUE;
		}

		return VFALSE;
	}

	/** Draws a rectangle in the style used to indicate that the rectangle
	has the focus. Because DrawFocusRect() is an XOR function, calling it a
	second time with the same rectangle removes the rectangle from the
	screen. This function draws a rectangle that cannot be scrolled. To
	scroll an area containing a rectangle drawn by this function, call
	DrawFocusRect() to remove the rectangle from the screen, scroll the
	area, and then call DrawFocusRect() again to draw the rectangle in the
	new position.*/
	void			DrawFocusRect(VRect const& r) const
	{
		VASSERT(m_hDC)
		::DrawFocusRect(m_hDC, r.GetPointer());
	}

	/** Draws an icon in the client area of the window.*/
	void			DrawIcon(	VINT	x,
								VINT	y,
								HICON	hIcon) const
	{
		VASSERT(m_hDC && hIcon)
		::DrawIcon(m_hDC, x, y, hIcon);
	}

	/** Draws formatted text in the specified rectangle. It formats the text
	according to the specified method (expanding tabs, justifying
	characters, breaking lines, and so forth). pszText is a pointer to the
	string to be drawn. If the nLength parameter is -1, the string must be
	null-terminated and the length will be retrieved. If nFormat includes
	DT_MODIFYSTRING, the function could add up to four additional
	characters to this string. The buffer containing the string should be
	large enough to accommodate these extra characters. nLength specifies
	the number of characters in the string. If nLength is -1, then the
	pszText parameter is assumed to be a pointer to a null-terminated
	string and DrawText() computes the character count automatically. r
	contains the rectangle (in logical coordinates) in which the text is to
	be formatted. nFormat specifies the method of formatting the text. It
	can be any combination of the following values:

	DT_BOTTOM - Justifies the text to the bottom of the rectangle. This
	value must be combined with DT_SINGLELINE.

	DT_CALCRECT - Determines the width and height of the rectangle. If
	there are multiple lines of text, DrawText() uses the width of the
	rectangle and extends the base of the rectangle to bound the last line
	of text. If there is only one line of text, DrawText() modifies the
	right side of the rectangle so that it bounds the last character in the
	line. In either case, DrawText() returns the height of the formatted
	text but does not draw the text.

	DT_CENTER - Centers text horizontally in the rectangle.

	DT_EDITCONTROL - Duplicates the text-displaying characteristics of a
	multiline edit control. Specifically, the average character width is
	calculated in the same manner as for an edit control, and the function
	does not display a partially visible last line.

	DT_END_ELLIPSIS or DT_PATH_ELLIPSIS - Replaces part of the given string
	with ellipses, if necessary, so that the result fits in the specified
	rectangle. The given string is not modified unless the DT_MODIFYSTRING
	flag is specified. You can specify DT_END_ELLIPSIS to replace
	characters at the end of the string, or DT_PATH_ELLIPSIS to replace
	characters in the middle of the string. If the string contains
	backslash (\) characters, DT_PATH_ELLIPSIS preserves as much as possible
	of the text after the last backslash.

	DT_EXPANDTABS - Expands tab characters. The default number of
	characters per tab is eight.

	DT_EXTERNALLEADING - Includes the font external leading in line height.
	Normally, external leading is not included in the height of a line of
	text.

	DT_INTERNAL - Uses the system font to calculate text metrics.

	DT_LEFT - Aligns text to the left.

	DT_MODIFYSTRING - Modifies the given string to match the displayed
	text. This flag has no effect unless the DT_END_ELLIPSIS or
	DT_PATH_ELLIPSIS flag is specified.

	DT_NOCLIP - Draws without clipping. DrawText() is somewhat faster when
	DT_NOCLIP is used.

	DT_NOPREFIX - Turns off processing of prefix characters. Normally,
	DrawText() interprets the mnemonic-prefix character & as a directive
	to underscore the character that follows, and the mnemonic-prefix
	characters && as a directive to print a single &. By specifying
	DT_NOPREFIX, this processing is turned off.

	DT_RIGHT - Aligns text to the right.

	DT_RTLREADING - Layout in right to left reading order for bi-directional
	text when the font selected into the DC is a Hebrew or Arabic font. The
	default reading order for all text is left to right.

	DT_SINGLELINE - Displays text on a single line only. Carriage returns
	and linefeeds do not break the line.

	DT_TABSTOP - Sets tab stops. Bits 15-8 (high-order byte of the low-order
	word) of the nFormat parameter specify the number of characters for
	each tab. The default number of characters per tab is eight.

	DT_TOP - Top-justifies text (single line only).

	DT_VCENTER - Centers text vertically (single line only).

	DT_WORDBREAK - Breaks words. Lines are automatically broken between
	words if a word would extend past the edge of the rectangle specified
	by the lpRect parameter. A carriage return-linefeed sequence also
	breaks the line.

	DT_WORD_ELLIPSIS - Truncates text that does not fit in the rectangle
	and adds ellipses. The DT_CALCRECT, DT_EXTERNALLEADING, DT_INTERNAL,
	DT_NOCLIP, and DT_NOPREFIX values cannot be used with the DT_TABSTOP
	value. The DrawText() function uses the device context's selected font,
	text color, and background color to draw the text. Unless the DT_NOCLIP
	format is used, DrawText() clips the text so that it does not appear
	outside the specified rectangle. All formatting is assumed to have
	multiple lines unless the DT_SINGLELINE format is specified. If the
	selected font is too large for the specified rectangle, the DrawText()
	function does not attempt to substitute a smaller font. On Windows CE,
	if DT_CALCRECT is specified for the nFormat parameter, you must set the
	right and bottom members of the rectangle. Windows CE does not support
	the DT_EXTERNALLEADING flag for the nFormat parameter. If the function
	succeeds, the return value is the height of the text. If the function
	fails, the return value is zero.*/
	VINT			DrawText(	VSTRING_CONST	pszText,
								VRect const&	r,
								VUINT			nFormat =
												DT_CENTER |
												DT_VCENTER |
												DT_SINGLELINE,
								VINT			nLength = -1) const
	{
		VASSERT(m_hDC && pszText)

		return ::DrawText(	m_hDC,
							pszText,
							nLength,
							r.GetPointer(),
							nFormat);
	}

	/** Fills a rectangle by using the specified brush. This function
	includes the left and top borders, but excludes the right and bottom
	borders of the rectangle. The brush identified by the hBrush parameter
	may be either a handle to a logical brush or a color value. If
	specifying a handle to a logical brush, call one of the following
	functions to obtain the handle: CreateHatchBrush(), CreatePatternBrush(),
	or CreateSolidBrush(). Additionally, you may retrieve a handle to one
	of the stock brushes by using the GetStockObject() function. If
	specifying a color value for the hBrush parameter, it must be one of the
	standard system colors (the value 1 must be added to the chosen color).
	For example: (HBRUSH)(COLOR_ENDCOLORS+1)). For a list of all the
	standard system colors see GetSysColor(). When filling the specified
	rectangle, FillRect() does not include the rectangle's right and bottom
	sides. GDI fills a rectangle up to, but not including, the right column
	and bottom row, regardless of the current mapping mode. In Windows CE
	version 1.0, the hBrush parameter cannot be a color brush. This function
	is the same in Windows CE version 2.0 as it is in Windows desktop
	platforms. Note: The VWCL brush classes, VBrush and VStockBrush also
	have a FillRect() function that can be used, which can make calling
	this function a little easier and more object oriented.*/
	void			FillRect(	VRect const&	r,
								HBRUSH			hBrush) const
	{
		VASSERT(m_hDC && hBrush)
		::FillRect(m_hDC, r.GetPointer(), hBrush);
	}

	/** Same as above, but allows the color to be specified by number to
	avoid additional typecast's. This function also adds 1 to the specified
	color, as required by the Win32 API call.*/
	void			FillRect(	VRect const&	r,
								VUINT			nColor) const
		{ FillRect(r, (HBRUSH)(nColor + 1)); }

	/** Base class function calls DeleteDC() if this objects owns the device
	context, as determined by the m_bFreeHDC member. If this value is
	VTRUE, the HDC member will be deleted. Override to do other cleanup as
	needed, but derived code should also check m_bFreeHDC and act
	accordingly.*/
	virtual void	Free()
	{
		if ( m_bFreeHDC )
		{
			VASSERT(m_hDC)
			DeleteDC(m_hDC);
			m_hDC = NULL;
		}
	}

	/** Return a reference to the internal device context (HDC) handle.*/
	HDC&			GetHandle() const
		{ return (HDC&)m_hDC; }

 	/** Draws a line from the current position up to, but not including, the
	specified point. The coordinates of the line's ending point are
	specified in logical units. The line is drawn by using the current pen
	and, if the pen is a geometric pen, the current brush. If LineTo()
	succeeds, the current position is set to the specified ending point.*/
	void			LineTo(	VINT nX,
							VINT nY) const
	{
		VASSERT(m_hDC)
		::LineTo(m_hDC, nX, nY);
	}

 	/** Same as above but uses VPoint object to get the ending coordinates.*/
	void			LineTo(VPoint const& pt) const
		{ LineTo(VPOINT_BREAK(pt)); }

	/** Combines the color data for the source and destination bitmaps using
	the specified mask and raster operation. nXDest specifies the logical
	x-coordinate of the upper-left corner of the destination rectangle.
	nYDest specifies the logical y-coordinate of the upper-left corner of
	the destination rectangle. nWidth specifies the width, in logical units,
	of the destination rectangle and source bitmap. nHeight specifies the
	height, in logical units, of the destination rectangle and source
	bitmap. sourceDC specifies the source device context. It must have a
	NULL DC handle if the nROP parameter specifies a raster operation that
	does not include a source. nXSrc specifies the logical x-coordinate of
	the upper-left corner of the source bitmap. nYSrc specifies the logical
	y-coordinate of the upper-left corner of the source bitmap. hBitmap is a
	handle to the monochrome mask bitmap combined with the color bitmap in
	the source device context. nXMask specifies the horizontal pixel offset
	for the mask bitmap specified by the hBitmap parameter. nYMask specifies
	the vertical pixel offset for the mask bitmap specified by the hBitmap
	parameter. nROP specifies both foreground and background ternary raster
	operation codes that the function uses to control the combination of
	source and destination data. The background raster operation code is
	stored in the high-order byte of the high-order word of this value; the
	foreground raster operation code is stored in the low-order byte of the
	high-order word of this value; the low-order word of this value is
	ignored, and should be zero. The macro MAKEROP4 creates such
	combinations of foreground and background raster operation codes. For a
	list of common raster operation codes, see the BitBlt() function. A
	value of 1 in the mask specified by hBitmap indicates that the
	foreground raster operation code specified by nROP should be applied at
	that location. A value of 0 in the mask indicates that the background
	raster operation code specified by nROP should be applied at that
	location. If the raster operations require a source, the mask rectangle
	must cover the source rectangle. If it does not, the function will
	fail. If the raster operations do not require a source, the mask
	rectangle must cover the destination rectangle. If it does not, the
	function will fail. If a rotation or shear transformation is in effect
	for the source device context when this function is called, an error
	occurs. However, other types of transformation are allowed. If the
	color formats of the source, pattern, and destination bitmaps differ,
	this function converts the pattern or source format, or both, to match
	the destination format. If the mask bitmap is not a monochrome bitmap,
	an error occurs. When an enhanced metafile is being recorded, an error
	occurs (and the function returns VFALSE) if the source device context
	identifies an enhanced-metafile device context. Not all devices support
	the MaskBlt() function. An application should call the GetDeviceCaps()
	function to determine whether a device supports this function. If no
	mask bitmap is supplied, this function behaves exactly like BitBlt(),
	using the foreground raster operation code. Windows CE 1.0 supports only
	the SRCCOPY and SRCINVERT raster operations. This function is the same
	in Windows CE version 2.0 as it is in Windows desktop platforms. Returns
	VTRUE on success, VFALSE on failure.*/
	VBOOL			MaskBlt(	VINT	nXDest,
								VINT	nYDest,
								VINT	nWidth,
								VINT	nHeight,
								VDC&	sourceDC,
								VINT	nXSrc,
								VINT	nYSrc,
								HBITMAP	hBitmap,
								VINT	nXMask,
								VINT	nYMask,
								VUINT	nROP) const
	{
		VASSERT(m_hDC && sourceDC.m_hDC)

		return ::MaskBlt(	m_hDC,
							nXDest,
							nYDest,
							nWidth,
							nHeight,
							sourceDC.m_hDC,
							nXSrc,
							nYSrc,
							hBitmap,
							nXMask,
							nYMask,
							nROP);
	}

	/** Same as above, but uses a VRect object to get the destination
	coordinates.*/
	VBOOL			MaskBlt(	VRect const&	r,
								VDC&			sourceDC,
								VINT			nXSrc,
								VINT			nYSrc,
								HBITMAP			hBitmap,
								VINT			nXMask,
								VINT			nYMask,
								VUINT			nROP) const
	{
		return MaskBlt(	VRECT_BREAK_WIDTH_HEIGHT(r),
						sourceDC,
						nXSrc,
						nYSrc,
						hBitmap,
						nXMask,
						nYMask,
						nROP);
	}

 	/** Moves the current position for drawing in the DC to the point
	specified. If the original position is wanted, pass a valid pointer to
	a VPoint object as pOldPos, otherwise NULL is fine.*/
	void			MoveTo(	VINT	nX,
							VINT	nY,
							VPoint* pOldPos = NULL) const
	{
		VASSERT(m_hDC)
		MoveToEx(m_hDC, nX, nY, (pOldPos) ? pOldPos->GetPointer() : NULL);
	}

 	/** Same as above, but uses a VPoint object to get the destination
	coordinates.*/
	void			MoveTo(	VPoint const&	pt,
							VPoint*			pOldPos = NULL) const
		{ MoveTo(VPOINT_BREAK(pt), pOldPos); }

	/** Paint the given rectangle using the brush that is currently selected
	into the device context. The brush color and the surface color(s) are
	combined by using the given raster operation. nXLeft specifies the
	x-coordinate, in logical units, of the upper-left corner of the
	rectangle to be filled. nYLeft specifies the y-coordinate, in logical
	units, of the upper-left corner of the rectangle to be filled. nWidth
	specifies the width, in logical units, of the rectangle. nHeight
	specifies the height, in logical units, of the rectangle. nROP specifies
	the raster operation code. This code may be one of the following values:

	PATCOPY - Copies the specified pattern into the destination bitmap.

	PATINVERT - Combines the colors of the specified pattern with the
	colors of the destination rectangle by using the Boolean OR operator.

	DSTINVERT - Inverts the destination rectangle.

	BLACKNESS - Fills the destination rectangle using the color associated
	with index 0 in the physical palette. (This color is black for the
	default physical palette.)

	WHITENESS - Fills the destination rectangle using the color associated
	with index 1 in the physical palette. (This color is white for the
	default physical palette.)

	The values of the nROP parameter for this function are a limited subset
	of the full 256 ternary raster-operation codes; in particular, an
	operation code that refers to a source rectangle cannot be used. Not all
	devices support the PatBlt() function. For more information, see the
	description of the RC_BITBLT capability in the GetDeviceCaps() function.
	Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			PatBlt(	VINT nXLeft,
							VINT nYLeft,
							VINT nWidth,
							VINT nHeight,
							VUINT nROP = PATCOPY) const
	{
		VASSERT(m_hDC)

		return (::PatBlt(	m_hDC,
							nXLeft,
							nYLeft,
							nWidth,
							nHeight,
							nROP) != 0) ? VTRUE : VFALSE;
	}

	/** Same as above, but uses a VRect object to get the destination
	coordinates.*/
	VBOOL			PatBlt(	VRect const&	r,
							VUINT			nROP = PATCOPY) const
		{ return PatBlt(VRECT_BREAK_WIDTH_HEIGHT(r), nROP); }

	/** Performs a bit-block transfer of the bits of color data from the
	specified rectangle in the source device context to the specified
	parallelogram in the destination device context. If the given bitmask
	handle identifies a valid monochrome bitmap, the function uses this
	bitmap to mask the bits of color data from the source rectangle. nXSrc
	specifies the x-coordinate, in logical units, of the upper-left corner
	of the source rectangle. nYSrc specifies the y-coordinate, in logical
	units, of the upper-left corner of the source rectangle. nWidth
	specifies the width, in logical units, of the source rectangle. nHeight
	specifies the height, in logical units, of the source rectangle. pPoints
	is a pointer to an array of three points in logical space that identify
	three corners of the destination parallelogram. The upper-left corner
	of the source rectangle is mapped to the first point in this array, the
	upper-right corner to the second point in this array, and the lower-left
	corner to the third point. The lower-right corner of the source
	rectangle is mapped to the implicit fourth point in the parallelogram.
	sourceDC specifies the source device context. hBitmap is a handle to an
	optional monochrome bitmap that is used to mask the colors of the
	source rectangle. nXMask specifies the x-coordinate of the upper-left
	corner of the the monochrome bitmap. nYMask specifies the y-coordinate
	of the upper-left corner of the the monochrome bitmap. The fourth
	vertex of the parallelogram (D) is defined by treating the first three
	points (A, B, and C) as vectors and computing D = B + C - A. If the
	bitmask exists, a value of 1 in the mask indicates that the source
	pixel color should be copied to the destination. A value of 0 in the
	mask indicates that the destination pixel color is not to be changed. If
	the mask rectangle is smaller than the source and destination
	rectangles, the function replicates the mask pattern. Scaling,
	translation, and reflection transformations are allowed in the
	source device context however, rotation and shear transformations are
	not. If the mask bitmap is not a monochrome bitmap, an error occurs. The
	stretching mode for the destination device context is used to determine
	how to stretch or compress the pixels, if that is necessary. When an
	enhanced metafile is being recorded, an error occurs if the source
	device context identifies an enhanced-metafile device context. The
	destination coordinates are transformed according to the destination
	device context; the source coordinates are transformed according to the
	source device context. If the source transformation has a rotation or
	shear, an error is returned. If the destination and source rectangles
	do not have the same color format, PlgBlt() converts the source
	rectangle to match the destination rectangle. Not all devices support
	the PlgBlt() function. For more information, see the description of the
	RC_BITBLT raster capability in the GetDeviceCaps() function. If the
	source and destination device contexts represent incompatible devices,
	PlgBlt() returns an error. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			PlgBlt(	VINT			nXSrc,
							VINT			nYSrc,
							VINT			nWidth,
							VINT			nHeight,
							POINT const*	pPoints,
							VDC&			sourceDC,
							HBITMAP			hBitmap,
							VINT			nXMask,
							VINT			nYMask) const
	{
		VASSERT(m_hDC && sourceDC.m_hDC)

		return ::PlgBlt(	m_hDC,
							pPoints,
							sourceDC.m_hDC,
							nXSrc,
							nYSrc,
							nWidth,
							nHeight,
							hBitmap,
							nXMask,
							nYMask);
	}

	/** Same as above, but uses a VRect object to get the destination
	coordinates.*/
	VBOOL			PlgBlt(	VRect const&	r,
							POINT const*	pPoints,
							VDC&			sourceDC,
							HBITMAP			hBitmap,
							VINT			nXMask,
							VINT			nYMask) const
	{
		return PlgBlt(	VRECT_BREAK_WIDTH_HEIGHT(r),
						pPoints,
						sourceDC,
						hBitmap,
						nXMask,
						nYMask);
	}

	/** Draws a polygon consisting of two or more vertices connected by
	straight lines. The polygon is outlined by using the current pen and
	filled by using the current brush and polygon fill mode. pPoints is a
	pointer to an array of POINT structures that specify the vertices of
	the polygon. nCount specifies the number of vertices in the array. This
	value must be greater than or equal to 2. The polygon is closed
	automatically by drawing a line from the last vertex to the first. The
	current DC position is neither used nor updated by the function.*/
	VBOOL			Polygon(	POINT const*	pPoints,
								VUINT			nCount) const
	{
		VASSERT(m_hDC && pPoints && nCount > 1)
		return (::Polygon(m_hDC, pPoints, nCount)) ? VTRUE : VFALSE;
	}

	/** Return the current polygon fill mode. This will be either ALTERNATE
	or WINDING. See the mutator function for more information.*/
	VINT			PolygonFillMode() const
	{
		VASSERT(m_hDC)
		return GetPolyFillMode(m_hDC);
	}

	/** Set the current polygon fill mode. This will be either ALTERNATE or
	WINDING and are explained as:

	ALTERNATE - Fills the area between odd-numbered and even-numbered
	polygon sides on each scan line.

	WINDING - Fills any region with a nonzero winding value.

	In general, the modes differ only in cases where a complex, overlapping
	polygon must be filled (for example, a five-sided polygon that forms a
	five-pointed star with a pentagon in the center). In such cases,
	ALTERNATE mode fills every other enclosed region within the polygon
	(that is, the points of the star), but WINDING mode fills all regions
	(that is, the points and the pentagon). When the fill mode is ALTERNATE,
	GDI fills the area between odd-numbered and even-numbered polygon sides
	on each scan line. That is, GDI fills the area between the first and
	second side, between the third and fourth side, and so on. When the fill
	mode is WINDING, GDI fills any region that has a nonzero winding value.
	This value is defined as the number of times a pen used to draw the
	polygon would go around the region. The direction of each edge of the
	polygon is important. The return value is the previous fill mode.*/
	VINT			PolygonFillMode(VINT nMode) const
	{
		VASSERT(m_hDC && (nMode == ALTERNATE || nMode == WINDING))
		return GetPolyFillMode(m_hDC);
	}

	/** Draws a rectangle. The rectangle is outlined by using the current
	pen and filled by using the current brush. The current DC position is
	neither used, nor updated, by Rectangle.*/
	void			Rectangle(	VINT nLeft,
								VINT nTop,
								VINT nRight,
								VINT nBottom) const
	{
		VASSERT(m_hDC)
		::Rectangle(m_hDC, nLeft, nTop, nRight, nBottom);
	}

	/** Same as above, but uses a VRect object to get the rectangle
	coordinates.*/
	void			Rectangle(VRect const& r) const
		{ Rectangle(VRECT_BREAK(r)); }

	/** Enumeration for the RectangleEx() function, which determine which
	sides of the rectangle to draw. The only bit that should need explaining
	is RECTANGLE_EX_3D_STYLE. If this bit is set, when drawing the lines
	that comprise the rectangle, the line will be drawn to the next
	corresponding starting point. For instance, when drawing the left side
	of the rectangle, the drawing of the left side will end one pixel into
	the top line. This is mostly useful when drawing 3D rectangles.*/
	enum	{		RECTANGLE_EX_DRAW_LEFT =		0x0001,
					RECTANGLE_EX_DRAW_TOP =			0x0002,
					RECTANGLE_EX_DRAW_RIGHT =		0x0004,
					RECTANGLE_EX_DRAW_BOTTOM =		0x0008,
					RECTANGLE_EX_3D_STYLE =			0x0010,
					RECTANGLE_EX_DRAW_ALL =			RECTANGLE_EX_DRAW_LEFT |
													RECTANGLE_EX_DRAW_TOP |
													RECTANGLE_EX_DRAW_RIGHT |
													RECTANGLE_EX_DRAW_BOTTOM,
			};

	/** Similar to above rectangle drawing functions, but draws the rectangle
	one line at a time, allowing calling code to specify which sides of the
	rectangle to draw. These are passed as nOptions which is a combination
	of the above numeration. Unlike the Rectangle() functions, this function
	does not fill the rectangle with the selected brush, it only draws the
	rectangle lines, using the currently selected pen. The DC position is
	updated by this function. It is important to note that this function
	adds 1 to the right and bottom edges of the r rectangle parameter, so
	that it draws consistantly with the way Rectangle() does.*/
	void			RectangleEx(	VRect const&	r,
									VUINT			nOptions =
													RECTANGLE_EX_DRAW_ALL) const
	{
		/* For speed, get these values.*/
		VLONG nl = r.GetLeft();
		VLONG nt = r.GetTop();
		VLONG nr = r.GetRight() - 1;
		VLONG nb = r.GetBottom() - 1;

		if ( nOptions & RECTANGLE_EX_DRAW_LEFT )
		{
			MoveTo(nl, nb - ((nOptions & RECTANGLE_EX_3D_STYLE) ? 1 : 0));
			LineTo(nl, nt);

			if ( nOptions & RECTANGLE_EX_3D_STYLE )
				LineTo(nl + 1, nt);
		}

		if ( nOptions & RECTANGLE_EX_DRAW_TOP )
		{
			MoveTo(nl, nt);
			LineTo(nr, nt);

			if ( nOptions & RECTANGLE_EX_3D_STYLE )
				LineTo(nr, nt + 1);
		}

		if ( nOptions & RECTANGLE_EX_DRAW_RIGHT )
		{
			MoveTo(nr, nt + ((nOptions & RECTANGLE_EX_3D_STYLE) ? 1 : 0));
			LineTo(nr, nb);

			if ( nOptions & RECTANGLE_EX_3D_STYLE )
				LineTo(nr - 1, nb);
		}

		if ( nOptions & RECTANGLE_EX_DRAW_BOTTOM )
		{
			MoveTo(nr, nb);
			LineTo(nl - ((nOptions & RECTANGLE_EX_3D_STYLE) ? 1 : 0), nb);
		}
	}

	/** This function restores a device context (DC) to the specified state.
	The device context is restored by popping state information off a stack
	created by earlier calls to the Save() function. nSavedDC specifies the
	instance of the device context to be restored. If this parameter is
	positive, nSavedDC represents a specific instance of the device context
	to be restored. If this parameter is negative, nSavedDC represents an
	instance relative to the current device context. For example, -1
	restores the most recently saved state. Returns VTRUE on success, VFALSE
	on failure.*/
	VBOOL			Restore(VINT nSaveDC = -1) const
	{
		VASSERT(m_hDC)
		return RestoreDC(m_hDC, nSaveDC);
	}

	/** This function restores a device context (DC) to the state set by the
	last SaveThis() function call. Please see this function for more
	information.*/
	VBOOL			RestoreThis()
	{
		VASSERT(m_hDC)

		VBOOL bResult =	VFALSE;

		if ( m_nSaveThis )
		{
			bResult =		Restore(m_nSaveThis);
			m_nSaveThis =	0;
		}

		return bResult;
	}

	/** Return the current foreground mix mode. See the ROP2() mutator
	function for more information.*/
	VINT			ROP2() const
	{
		VASSERT(m_hDC)
		return GetROP2(m_hDC);
	}

	/** Sets the current foreground mix mode. GDI uses the foreground mix
	mode to combine pens and interiors of filled objects with the colors
	already on the screen. The foreground mix mode defines how colors from
	the brush or pen and the colors in the existing image are to be
	combined. nROP specifies the new mix mode and can be any one of the
	following:

	R2_BLACK - Pixel is always 0.

	R2_COPYPEN - Pixel is the pen color.

	R2_MASKNOTPEN - Pixel is a combination of the colors common to both
	the screen and the inverse of the pen.

	R2_MASKPEN - Pixel is a combination of the colors common to both
	the pen and the screen.

	R2_MASKPENNOT - Pixel is a combination of the colors common to both
	the pen and the inverse of the screen.

	R2_MERGENOTPEN - Pixel is a combination of the screen color and
	the inverse of the pen color.

	R2_MERGEPEN - Pixel is a combination of the pen color and the screen
	color.

	R2_MERGEPENNOT - Pixel is a combination of the pen color and the
	inverse of the screen color.

	R2_NOP - Pixel remains unchanged.

	R2_NOT - Pixel is the inverse of the screen color.

	R2_NOTCOPYPEN - Pixel is the inverse of the pen color.

	R2_NOTMASKPEN - Pixel is the inverse of the R2_MASKPEN color.

	R2_NOTMERGEPEN - Pixel is the inverse of the R2_MERGEPEN color.

	R2_NOTXORPEN - Pixel is the inverse of the R2_XORPEN color.

	R2_WHITE - Pixel is always 1.

	R2_XORPEN - Pixel is a combination of the colors in the pen and in the
	screen, but not in both. Mix modes define how GDI combines source and
	destination colors when drawing with the current pen. The mix modes are
	binary raster operation codes, representing all possible Boolean
	functions of two variables, using the binary operations AND, OR, and
	XOR (exclusive OR), and the unary operation NOT. The mix mode is for
	raster devices only, it is not available for vector devices. If the
	function succeeds, the return value specifies the previous mix mode. If
	the function fails, the return value is zero.*/
	VINT			ROP2(VINT nROP) const
	{
		VASSERT(m_hDC)
		return SetROP2(m_hDC, nROP);
	}

	/** This function saves the current state of the device context by
	copying data describing selected objects and graphic modes (such as the
	bitmap, brush, palette, font, pen, region, drawing mode, and mapping
	mode) to a context stack. This state can then be restored with a call to
	Restore(). If the function succeeds, the return value identifies the
	saved device context. If the function fails, the return value is zero.
	The Save() function can be used any number of times to save any number
	of instances of the device context's state. A saved state can later be
	restored by using the Restore() function.*/
	VINT			Save() const
	{
		VASSERT(m_hDC)
		return SaveDC(m_hDC);
	}

	/** Same as above, but saves the current state into a member variable
	local to this class. This class can save the current state at any time,
	but each call to SaveThis() should be matched by a call to RestoreThis().
	In other words, whereas the Save() function can return multiple
	snapshots of this device context that can later be restored, this
	function saves only one snapshot in this class, which will be restored
	by the next call to RestoreThis(). If this function is called a second
	time before RestoreThis() is called, RestoreThis() will be
	automatically called. Also, the this class goes out of scope with a
	saved state, it will be restored before going out of scope.*/
	void			SaveThis()
	{
		VASSERT(m_hDC)
		RestoreThis();
		m_nSaveThis = Save();
	}

	/** Select a GDI object into the device context, returning the previous
	object.*/
	HGDIOBJ			SelectObject(HGDIOBJ hObject) const
	{
		VASSERT(m_hDC)
		return ::SelectObject(m_hDC, hObject);
	}

	/** Copies a bitmap from a source rectangle into a destination rectangle,
	stretching or compressing the bitmap to fit the dimensions of the
	destination rectangle, if necessary. The system stretches or compresses
	the bitmap according to the stretching mode currently set in the
	destination device context. nXOriginDest specifies the x-coordinate, in
	logical units, of the upper-left corner of the destination rectangle.
	nYOriginDest specifies the y-coordinate, in logical units, of the
	upper-left corner of the destination rectangle. nWidthDest specifies
	the width, in logical units, of the destination rectangle. nHeightDest
	specifies the height, in logical units, of the destination rectangle.
	sourceDC specifies the source device context. nXOriginSrc specifies
	the x-coordinate, in logical units, of the upper-left corner of the
	source rectangle. nYOriginSrc specifies the y-coordinate, in logical
	units, of the upper-left corner of the source rectangle. nWidthSrc
	specifies the width, in logical units, of the source rectangle.
	nHeightSrc specifies the height, in logical units, of the source
	rectangle. nROP specifies the raster operation to be performed. Raster
	operation codes define how the system combines colors in output
	operations that involve a brush, a source bitmap, and a destination
	bitmap. See BitBlt() for a list of common raster operation codes.
	StretchBlt() stretches or compresses the source bitmap in memory and
	then copies the result to the destination rectangle. The color data for
	pattern or destination pixels is merged after the stretching or
	compression occurs. When an enhanced metafile is being recorded, an
	error occurs (and the function returns VFALSE) if the source device
	context identifies an enhanced-metafile device context. If the
	specified raster operation requires a brush, the system uses the brush
	currently selected into the destination device context. The
	destination coordinates are transformed by using the transformation
	currently specified for the destination device context the source
	coordinates are transformed by using the transformation currently
	specified for the source device context. If the source transformation
	has a rotation or shear, an error occurs. If destination, source, and
	pattern bitmaps do not have the same color format, StretchBlt()
	converts the source and pattern bitmaps to match the destination bitmap.
	If StretchBlt() must convert a monochrome bitmap to a color bitmap, it
	sets white bits (1) to the background color and black bits (0) to the
	foreground color. To convert a color bitmap to a monochrome bitmap, it
	sets pixels that match the background color to white (1) and sets all
	other pixels to black (0). The foreground and background colors of the
	device context with color are used. StretchBlt() creates a mirror image
	of a bitmap if the signs of the nWidthSrc and nWidthDest parameters or
	of the nHeightSrc and nHeightDest parameters differ. If nWidthSrc and
	nWidthDest have different signs, the function creates a mirror image of
	the bitmap along the x-axis. If nHeightSrc and nHeightDest have
	different signs, the function creates a mirror image of the bitmap
	along the y-axis. Not all devices support the StretchBlt function. For
	more information, see the GetDeviceCaps(). Windows CE version 1.0
	supports only the SRCCOPY and SRCINVERT raster operations. Windows CE
	version 1.0 does not support mirroring. This function is the same in
	Windows CE version 2.0 as it is in Windows desktop platforms. Returns
	VTRUE on success, VFALSE on failure.*/
	VBOOL			StretchBlt(	VINT	nXOriginDest,
								VINT	nYOriginDest,
								VINT	nWidthDest,
								VINT	nHeightDest,
								VDC&	sourceDC,
								VINT	nXOriginSrc,
								VINT	nYOriginSrc,
								VINT	nWidthSrc,
								VINT	nHeightSrc,
								VUINT	nROP = SRCCOPY) const
	{
		VASSERT(m_hDC && sourceDC.m_hDC)

		return ::StretchBlt(	m_hDC,
								nXOriginDest,
								nYOriginDest,
								nWidthDest,
								nHeightDest,
								sourceDC.m_hDC,
								nXOriginSrc,
								nYOriginSrc,
								nWidthSrc,
								nHeightSrc,
								nROP);
	}

	/** Same as above, but uses a VRect objects to get the coordinates.*/
	VBOOL			StretchBlt(	VRect const&	r,
								VDC&			sourceDC,
								VRect&			rSource,
								VUINT			nROP = SRCCOPY) const
	{
		return StretchBlt(	VRECT_BREAK_WIDTH_HEIGHT(r),
							sourceDC,
							VRECT_BREAK_WIDTH_HEIGHT(rSource),
							nROP);
	}

	/** Gets the text color for the device context. If the function fails,
	the return value is CLR_INVALID.*/
	COLORREF		TextColor() const
	{
		VASSERT(m_hDC)
		return GetTextColor(m_hDC);
	}

	/** Sets the text color for the device context to the specified color.
	The text color is used to draw the face of each character written by the
	TextOut() and ExtTextOut() functions (and other). The text color is also
	used in converting bitmaps from color to monochrome and vice versa. If
	the function succeeds, the return value is a color reference for the
	previous text color. If the function fails, the return value is
	CLR_INVALID.*/
	COLORREF		TextColor(COLORREF crColor) const
	{
		VASSERT(m_hDC)
		return SetTextColor(m_hDC, crColor);
	}

protected:
	/** Embedded Members.*/
	HDC				m_hDC;
	VBOOL			m_bFreeHDC;
	VINT			m_nSaveThis;
};

#endif /* VDCHPP*/
