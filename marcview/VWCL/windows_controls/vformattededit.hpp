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

#ifndef VFORMATTEDEDIT
#define VFORMATTEDEDIT

#include "../vstandard.h"
#include "../numbers/vbitmask.hpp"
#include "../numbers/vrtti.hpp"
#include "../strings/vstring.hpp"
#include "../structures/vrect.hpp"
#include "vedit.hpp"

/** VFormattedEdit is a VEdit window which requires subclassing of a
Windows EDIT control, and validates data as entered.*/
class VFormattedEdit : public VEdit
{
public:
	/** Formatting constants held in m_Options which can be retreived
	using GetFormatOptions() or can be set during construction. Some can
	be or'd, others are mutually exclusive. ECF_NUMBER can be or'd with
	ECF_DECIMAL and ECF_NEGATIVE. All others are mutually exclusive to
	each other.*/
	enum	{
					FORMAT_OPTION_NONE =					0,
					FORMAT_OPTION_ALLOW_NUMBERS =			0x0001,
					FORMAT_OPTION_ALLOW_DECIMAL =			0x0002,
					FORMAT_OPTION_ALLOW_NEGATIVE =			0x0004,
					FORMAT_OPTION_ONLY_ALLOW_LOWERCASE =	0x0008,
					FORMAT_OPTION_ONLY_ALLOW_UPPERCASE =	0x0010,
					FORMAT_OPTION_FORCE_LOWER_CASE =		0x0020,
					FORMAT_OPTION_FORCE_UPPER_CASE =		0x0040,

			};

	/** Default constructor can be initialized with formatting options.
	It also initializes base class.*/
	VFormattedEdit(	VUINT		nFormatOptions = FORMAT_OPTION_NONE,
					VRTTI_TYPE	nRTTI = VWCL_RTTI_FORMATTED_EDIT)
		: VEdit(nRTTI)
		{ m_FormatOptions = nFormatOptions; }

	/** Return a reference to the VString object which holds the characters
	that and invalid and thus not allowed to be entered. If this string is
	not empty, the above formatting options will not be used. You must
	decide to use either this string of invalid characters to choose the
	above formatting options and leave this string empty.*/
	VString&		GetInvalid() const
		{ return (VString&)m_strInvalid; }

	/** Get the VBitmask object which holds the above option bits. The
	setting of these bits does not override the invalid string above. It
	must be empty for these settings to have any effect.*/
	VBitmask&		GetFormatOptions() const
		{ return (VBitmask&)m_FormatOptions; }

	/** Determine if a character is valid. If needed, this function will
	convert the case of nChar to make it valid.*/
	virtual VBOOL	IsValidCharacter(VTCHAR& nChar)
	{
		/* If the m_strInvalid member is not empty, we will
		check to make sure nChar is NOT in the string.*/
		if ( m_strInvalid.IsNotEmpty() )
			return (m_strInvalid.Find(nChar) != -1) ? VTRUE : VFALSE;

		/* Do we need to convert the case first?*/
		if ( m_FormatOptions.IsSet(FORMAT_OPTION_FORCE_LOWER_CASE) )
			nChar = tolower(nChar);
		else if ( m_FormatOptions.IsSet(FORMAT_OPTION_FORCE_UPPER_CASE) )
			nChar = toupper(nChar);

		VString s;

		/* If we are checking for a decimal make sure there is not
		already one in the control, and for a negative, make sure the - is
		in the first place and does not already exist.*/
		if (	nChar == VTEXT('.') &&
				m_FormatOptions.IsSet(FORMAT_OPTION_ALLOW_DECIMAL) )
		{
			if ( !GetTextLength() )
				return VTRUE;
			else if ( GetText(s) && s.Find(VTEXT('.')) == -1 )
				return VTRUE;
		}

		if (	nChar == VTEXT('-') &&
				m_FormatOptions.IsSet(FORMAT_OPTION_ALLOW_NEGATIVE) )
		{
			/* For the negative case, make sure we are inserting
			at the first position.*/
			if ( !GetTextLength() )
				return VTRUE;
			else
			{
				if (	(s.IsNotEmpty() ||
						GetText(s)) && s.Find(VTEXT('-') != -1) )
				{
					VDWORD nStartPosition, nEndPosition;
					Selection(&nStartPosition, &nEndPosition);

					if ( !nStartPosition && !nEndPosition )
						return VTRUE;
				}
			}
		}

		/* Make sure nChar is between 0 and 9?*/
		if ( m_FormatOptions.IsSet(FORMAT_OPTION_ALLOW_NUMBERS) )
			return	(nChar >= VTEXT('0') && nChar <= VTEXT('9'))
					? VTRUE : VFALSE;

		/* Make sure nChar is between a and z?*/
		if ( m_FormatOptions.IsSet(FORMAT_OPTION_ONLY_ALLOW_LOWERCASE) )
			return	(nChar >= VTEXT('a') && nChar <= VTEXT('z'))
					? VTRUE : VFALSE;

		/* Make sure nChar is between A and Z?*/
		if ( m_FormatOptions.IsSet(FORMAT_OPTION_ONLY_ALLOW_UPPERCASE) )
			return	(nChar >= VTEXT('A') && nChar <= VTEXT('Z'))
					? VTRUE : VFALSE;

		return VFALSE;
	}

protected:
	/** Window procedure override looks for messages we are interested in.*/
	virtual VLONG	WindowProc(	HWND	hWnd,
								VUINT	nMessage,
								VWPARAM	wParam,
								VLPARAM	lParam)
	{
		switch ( nMessage )
		{
			case WM_CHAR:
			{
				VTCHAR nChar = (VTCHAR)wParam;

				/* Check for valid input if format option bits are
				present or invalid string is not empty.*/
				if (	(nChar != VK_BACK) &&
						(m_FormatOptions.Bits() ||
						m_strInvalid.String()) )
				{
					if ( !IsValidCharacter(nChar) )
					{
						MessageBeep(MB_ICONEXCLAMATION);
						return 0;
					}
				}

				break;
			}

			/* Do not allow paste unless this control has no format
			options or invalid text chars.*/
			case WM_PASTE:
			{
				if ( m_FormatOptions.Bits() || m_strInvalid.String() )
				{
					MessageBeep(MB_ICONEXCLAMATION);
					return 0;
				}

				break;
			}
		}

		/* If we made it this far, the character is valid.*/
		return VEdit::WindowProc(hWnd, nMessage, wParam, lParam);
	}

	/** Embedded Members.*/
	VString			m_strInvalid;
	VBitmask		m_FormatOptions;
};

#endif /* VFORMATTEDEDIT*/
