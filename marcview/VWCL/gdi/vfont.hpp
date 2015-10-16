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
Other Author(s) of this source code file:	Thore B. Karlsen
											(sid6581@mail.utexas.edu)

VWCL and all source code are copyright (c) 1996-2000 by The VWCL Alliance.*/

#ifndef VFONT
#define VFONT

#include "../vstandard.h"
#include "../numbers/vbitmask.hpp"
#include "vgdiobject.hpp"

/** VFont is a VGDIObject that encapulates a HFONT GDI object. This class is
used only for custom created font GDI objects, not stock font objects. This
class will delete the object during destruction. For Windows defined stock
font objects, use VStockFont instead. Because of the many ways to create a
font, this class has a constructor that is used only when wrapping an
existing HFONT object. Otherwise, you must use one of the CreateXXX()
functions to initialize to a valid font.*/
class VFont : public VGDIObject
{
public:
	/** Default constructor is initialized by having it take ownership of an
	existing HFONT object. If this handle is NULL, then you must call one
	of the CreateXXX() method to create the font. If hFont is not NULL, and
	bDuplicate is VTRUE, this object will create a new font that is
	identical to hFont, but a separate GDI object.*/
	VFont(	HFONT hFont = NULL,
			VBOOL bDuplicate = VFALSE)
	{
		if ( hFont )
		{
			if ( bDuplicate )
				Duplicate(hFont);
			else
				SetHandle(hFont);
		}
	}

	/** Creates a logical font that has specific characteristics. The logical
	font can subsequently be selected as the font for any device. nHeight
	specifies the height, in logical units, of the font's character cell or
	character. The character height value (also known as the em height) is
	the character cell height value minus the internal-leading value. The
	font mapper interprets the value specified in nHeight in the following
	manner:

	> 0  - The font mapper transforms this value into device units and
	matches it against the cell height of the available fonts.

	0 - The font mapper uses a default height value when it searches for a
	match.

	< 0 - The font mapper transforms this value into device units and
	matches its absolute value against the character height of the
	available fonts.

	For all height comparisons, the font mapper looks for the largest font
	that does not exceed the requested size. This mapping occurs when the
	font is used for the first time. For the MM_TEXT mapping mode, you can
	use the following formula to specify a height for a font with a
	specified point size:

	nHeight = -MulDiv(PointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);

	nWidth specifies the average width, in logical units, of characters in
	the requested font. If this value is zero, the font mapper chooses a
	"closest match" value. The "closest match" value is determined by
	comparing the absolute values of the difference between the current
	device's aspect ratio and the digitized aspect ratio of available
	fonts. nEscapement specifies the angle, in tenths of degrees, between
	the escapement vector and the x-axis of the device. The escapement
	vector is parallel to the base line of a row of text. For Windows NT,
	when the graphics mode is set to GM_ADVANCED, you can specify the
	escapement angle of the string independently of the orientation angle
	of the string's characters. When the graphics mode is set to
	GM_COMPATIBLE, nEscapement specifies both the escapement and orientation.
	You should set nEscapement and nOrientation to the same value. For
	Windows 95, the nEscapement parameter specifies both the escapement and
	orientation. You should set nEscapement and nOrientation to the same
	value. nOrientation specifies the angle, in tenths of degrees, between
	each character's base line and the x-axis of the device. nWeight
	specifies the weight of the font in the range 0 through 1000. For
	example, 400 is normal and 700 is bold. If this value is zero, a default
	weight is used. The following values are defined for convenience:

	FW_DONTCARE 0

	FW_THIN 100

	FW_EXTRALIGHT 200

	FW_ULTRALIGHT 200

	FW_LIGHT 300

	FW_NORMAL 400

	FW_REGULAR 400

	FW_MEDIUM 500

	FW_SEMIBOLD 600

	FW_DEMIBOLD 600

	FW_BOLD 700

	FW_EXTRABOLD 800

	FW_ULTRABOLD 800

	FW_HEAVY 900

	FW_BLACK 900

	bItalic specifies an italic font if set to VTRUE. bUnderline specifies
	an underlined font if set to VTRUE. bStrikeOut specifies a strikeout
	font if set to VTRUE. nCharSet specifies the character set. The
	following values are predefined:

	ANSI_CHARSET

	BALTIC_CHARSET

	CHINESEBIG5_CHARSET

	DEFAULT_CHARSET

	EASTEUROPE_CHARSET

	GB2312_CHARSET

	GREEK_CHARSET

	HANGUL_CHARSET

	MAC_CHARSET

	OEM_CHARSET

	RUSSIAN_CHARSET

	SHIFTJIS_CHARSET

	SYMBOL_CHARSET

	TURKISH_CHARSET

	Korean Windows: JOHAB_CHARSET

	Middle-Eastern Windows: HEBREW_CHARSET, ARABIC_CHARSET

	Thai Windows: THAI_CHARSET

	The OEM_CHARSET value specifies a character set that is operating-system
	dependent. You can use the DEFAULT_CHARSET value to allow the name and
	size of a font to fully describe the logical font. If the specified font
	name does not exist, a font from any character set can be substituted
	for the specified font, so you should use DEFAULT_CHARSET sparingly to
	avoid unexpected results. Fonts with other character sets may exist in
	the operating system. If an application uses a font with an unknown
	character set, it should not attempt to translate or interpret strings
	that are rendered with that font. This parameter is important in the
	font mapping process. To ensure consistent results, specify a specific
	character set. If you specify a typeface name in the pszFaceName
	parameter, make sure that the nCharSet value matches the character set
	of the typeface specified in pszFaceName. nOutputPrecision specifies
	the output precision. The output precision defines how closely the
	output must match the requested font's height, width, character
	orientation, escapement, pitch, and font type. It can be one of the
	following values:

	OUT_CHARACTER_PRECIS - Not used.

	OUT_DEFAULT_PRECIS - Specifies the default font mapper behavior.

	OUT_DEVICE_PRECIS - Instructs the font mapper to choose a Device font
	when the system contains multiple fonts with the same name.

	OUT_OUTLINE_PRECIS - Windows NT: This value instructs the font mapper
	to choose from TrueType and other outline-based fonts.

	OUT_RASTER_PRECIS - Instructs the font mapper to choose a raster font
	when the system contains multiple fonts with the same name.

	OUT_STRING_PRECIS - This value is not used by the font mapper, but it
	is returned when raster fonts are enumerated.

	OUT_STROKE_PRECIS - Windows NT: This value is not used by the font
	mapper, but it is returned when TrueType, other outline-based fonts,
	and vector fonts are enumerated. Windows 95: This value is used to map
	vector fonts, and is returned when TrueType or vector fonts are
	enumerated.

	OUT_TT_ONLY_PRECIS - Instructs the font mapper to choose from only
	TrueType fonts. If there are no TrueType fonts installed in the
	system, the font mapper returns to default behavior.

	OUT_TT_PRECIS = Instructs the font mapper to choose a TrueType font
	when the system contains multiple fonts with the same name. Applications
	can use the OUT_DEVICE_PRECIS, OUT_RASTER_PRECIS, and OUT_TT_PRECIS
	values to control how the font mapper chooses a font when the operating
	system contains more than one font with a specified name. For example,
	if an operating system contains a font named Symbol in raster and
	TrueType form, specifying OUT_TT_PRECIS forces the font mapper to
	choose the TrueType version. Specifying OUT_TT_ONLY_PRECIS forces the
	font mapper to choose a TrueType font, even if it must substitute a
	TrueType font of another name. nClipPrecision specifies the clipping
	precision. The clipping precision defines how to clip characters that
	are partially outside the clipping region. It can be one or more of the
	following values:

	CLIP_DEFAULT_PRECIS - Specifies default clipping behavior.

	CLIP_CHARACTER_PRECIS - Not used.

	CLIP_STROKE_PRECIS - Not used by the font mapper, but is returned when
	raster, vector, or TrueType fonts are enumerated. Windows NT: For
	compatibility, this value is always returned when enumerating fonts.

	CLIP_MASK - Not used.

	CLIP_EMBEDDED - You must specify this flag to use an embedded read-only
	font.

	CLIP_LH_ANGLES - When this value is used, the rotation for all fonts
	depends on whether the orientation of the coordinate system is
	left-handed or right-handed. If not used, device fonts always rotate
	counterclockwise, but the rotation of other fonts is dependent on the
	orientation of the coordinate system. For more information about the
	orientation of coordinate systems, see the description of the
	nOrientation parameter.

	CLIP_TT_ALWAYS - Not used.

	nQuality specifies the output quality. The output quality defines how
	carefully GDI must attempt to match the logical-font attributes to
	those of an actual physical font. It can be one of the following values:

	DEFAULT_QUALITY - Appearance of the font does not matter.

	DRAFT_QUALITY - Appearance of the font is less important than when
	the PROOF_QUALITY value is used. For GDI raster fonts, scaling is
	enabled, which means that more font sizes are available, but the
	quality may be lower. Bold, italic, underline, and strikeout fonts are
	synthesized if necessary.

	PROOF_QUALITY - Character quality of the font is more important than
	exact matching of the logical-font attributes. For GDI raster fonts,
	scaling is disabled and the font closest in size is chosen. Although
	the chosen font size may not be mapped exactly when PROOF_QUALITY is
	used, the quality of the font is high and there is no distortion of
	appearance. Bold, italic, underline, and strikeout fonts are synthesized
	if necessary. nPitchAndFamily specifies the pitch and family of the
	font. The two low-order bits specify the pitch of the font and can be
	one of the following values:

	DEFAULT_PITCH

	FIXED_PITCH

	VARIABLE_PITCH

	The four high-order bits specify the font family and can be one of
	the following values:

	FF_DECORATIVE - Novelty fonts. Old English is an example.

	FF_DONTCARE - Don't care or don't know.

	FF_MODERN - Fonts with constant stroke width, with or without serifs.
	Pica, Elite, and Courier New are examples.

	FF_ROMAN - Fonts with variable stroke width and with serifs. MS Serif
	is an example.

	FF_SCRIPT - Fonts designed to look like handwriting. Script and Cursive
	are examples.

	FF_SWISS - Fonts with variable stroke width and without serifs. MS Sans
	Serif is an example.

	An application can specify a value for the fdwPitchAndFamily parameter
	by using the Boolean OR operator to join a pitch constant with a family
	constant. Font families describe the look of a font in a general way.
	They are intended for specifying fonts when the exact typeface
	requested is not available. pszFaceName is a pointer to a
	null-terminated string that specifies the typeface name of the font.
	The length of this string must not exceed 32 characters, including the
	null terminator. The EnumFontFamilies() function can be used to
	enumerate the typeface names of all currently available fonts. If
	pszFaceName is NULL, or points to an empty string, GDI uses the first
	font that matches the other specified attributes.  This function returns
	VTRUE if the new font was created, VFALSE otherwise.*/
	VBOOL	Create(	VLONG			nHeight,
					VLONG			nWidth = 0,
					VSTRING_CONST	pszFaceName = VTEXT("Arial"),
					VLONG			nEscapement = 0,
					VLONG			nOrientation = 0,
					VLONG			nWeight = FW_NORMAL,
					VBOOL			bItalic = VFALSE,
					VBOOL			bUnderline = VFALSE,
					VBOOL			bStrikeOut = VFALSE,
					VBYTE			nCharSet = ANSI_CHARSET,
					VBYTE			nOutPrecision = OUT_DEFAULT_PRECIS,
					VBYTE			nClipPrecision = CLIP_DEFAULT_PRECIS,
					VBYTE			nQuality = DEFAULT_QUALITY,
					VBYTE			nPitchAndFamily =
									DEFAULT_PITCH | FF_DONTCARE)
	{
		VASSERT(VSTRLEN_CHECK(pszFaceName))
		VASSERT(VSTRLEN_CHECK(pszFaceName) < LF_FACESIZE)

		if ( pszFaceName )
		{
			LOGFONT lf;
			VZEROSTRUCT(lf);

			/* Initialize LOGFONT struct.*/
			lf.lfHeight =		nHeight;
			lf.lfWidth =		nWidth;
			lf.lfEscapement =	nEscapement;
			lf.lfOrientation =	nOrientation;
			lf.lfWeight =		nWeight;
			lf.lfItalic =		bItalic;
			lf.lfUnderline =	bUnderline;
			lf.lfStrikeOut =	bStrikeOut;
			lf.lfCharSet =		nCharSet;
			lf.lfOutPrecision =	nOutPrecision;
			lf.lfClipPrecision=	nClipPrecision;
			lf.lfQuality =		nQuality;
			lf.lfPitchAndFamily=nPitchAndFamily;
			VSTRCPY(lf.lfFaceName, pszFaceName);

			return CreateIndirect(lf);
		}

		return VFALSE;
	}

	/** Constants used with the nAttributes parameter of CreateExact().*/
	enum	{	NORMAL =	0x0000,
				BOLD =		0x0001,
				ITALIC =	0x0002,
				UNDERLINE =	0x0004,
				STRIKEOUT =	0x0008,
			};

	/** Create a font based on a name and point size. pszName is the name of
	the font, like Arial. nPointSize is the number of points the font
	should be, and nAttributes is a bitwise combination of the above
	enumerated constants. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL	CreateExact(	VSTRING_CONST	pszFaceName,
							VUINT			nPointSize,
							VUINT			nAttributes)
	{
		VASSERT(VSTRLEN_CHECK(pszFaceName))
		VASSERT(VSTRLEN_CHECK(pszFaceName) < LF_FACESIZE)

		if ( pszFaceName )
		{
			/* Initialize LOGFONT structure.*/
			LOGFONT	lf;
			VZEROSTRUCT(lf);

			/* Get screen DC.*/
			HDC hDC = GetDC(NULL);

			if ( hDC )
			{
				/* Calculate size and release the DC.*/
				VINT cyDPI = GetDeviceCaps(hDC, LOGPIXELSY);
				ReleaseDC(NULL, hDC);
				hDC = NULL;

				/* Set attributes bitmask.*/
				VBitmask attr(nAttributes);

				/* Initialize the structure now.*/
				lf.lfHeight =			-MulDiv(nPointSize, cyDPI, 72);
				lf.lfWidth =			0;
				lf.lfEscapement =		0;
				lf.lfOrientation =		0;
				lf.lfWeight =			attr.IsSet(BOLD)
										? FW_BOLD
										: FW_DONTCARE;
				lf.lfItalic =			attr.IsSet(ITALIC) ? 1 : 0;
				lf.lfUnderline =		attr.IsSet(UNDERLINE) ? 1 : 0;
				lf.lfStrikeOut =		attr.IsSet(STRIKEOUT) ? 1 : 0;
				lf.lfCharSet =			DEFAULT_CHARSET;
				lf.lfOutPrecision =		OUT_DEFAULT_PRECIS;
				lf.lfClipPrecision =	CLIP_DEFAULT_PRECIS;
				lf.lfQuality =			DEFAULT_QUALITY;
				lf.lfPitchAndFamily =	DEFAULT_PITCH | FF_DONTCARE;
				VSTRCPY(lf.lfFaceName, pszFaceName);

				return CreateIndirect(lf);
			}
		}

		return VFALSE;
	}

	/** Same as above but uses a LOGFONT struct to initialize the font.*/
	VBOOL	CreateIndirect(LOGFONT const& LogFont)
	{
		VASSERT(VSTRLEN_CHECK(LogFont.lfFaceName))
		return SetHandle(CreateFontIndirect(&LogFont));
	}

	/** Given a HFONT, create a duplicate of it.*/
	VBOOL	Duplicate(HFONT hFont)
	{
		if ( hFont )
		{
			LOGFONT lf;

			if ( GetObject(hFont, sizeof(lf), &lf) )
				return CreateIndirect(lf);
		}

		return VFALSE;
	}

	/** Return a reference to the font GDI object handle.*/
	HFONT&	GetHandle() const
		{ return (HFONT&)VGDIObject::GetHandle(); }

	/** Get the font information into a LOGFONT structure.*/
	VBOOL	GetInfo(LOGFONT& lf) const
		{ return VGDIObject::GetInfo(sizeof(LOGFONT), &lf); }
};

#endif /* VFONT*/
