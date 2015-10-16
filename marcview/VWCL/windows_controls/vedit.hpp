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

#ifndef VEDIT
#define VEDIT

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"

/** VEdit encapsulates the standard Windows edit control.*/
class VEdit : public VWindow
{
public:
	/** Default constructor initializes base class RTTI information.*/
	VEdit(VRTTI_TYPE nRTTI = VWCL_RTTI_EDIT)
		: VWindow(nRTTI)
		{;}

	/** Determine whether an edit control operation can be undone, that is,
	whether the control can respond to the EM_UNDO message. If the edit
	control can correctly process the EM_UNDO message, the return value is
	VTRUE, otherwise, it is VFALSE.*/
	VBOOL		CanUndo() const
		{ return (VBOOL)VWINDOW_SM0(EM_CANUNDO); }

#if(WINVER >= 0x0400)
	/** Retrieves the character index and line index of the character
	nearest a specified point in the client area of an edit control. The low
	word of the return value specifies the zero-based index of the character
	nearest the specified point. This index is relative to the beginning of
	the control, not the beginning of the line. If the specified point is
	beyond the last character in the edit control, the return value
	indicates the last character in the control. The high word of the
	return value specifies the zero-based index of the line that contains
	the character. For single-line edit controls, this value is zero. The
	index indicates the line delimiter if the specified point is beyond
	the last visible character in a line.*/
	VLONG		CharFromPos(	VWORD nXPosition,
								VWORD nYPosition) const
		{ return VWINDOW_SM3(EM_CHARFROMPOS, 0, nXPosition, nYPosition); }
#endif

	/** Copy the current selection, if any, in the edit control to the
	clipboard in CF_TEXT format.*/
	void		Copy() const
		{ VWINDOW_SM0(WM_COPY); }

	/** Create a new edit control. nStyle is a combination of WS_xxx window
	style bits or'd with these button specific bits:

	ES_AUTOHSCROLL - Automatically scrolls text to the right by 10
	characters when the user types a character at the end of the line. When
	the user presses the ENTER key, the control scrolls all text back to
	position 0.

	ES_AUTOVSCROLL - Automatically scrolls text up one page when the user
	presses ENTER on the last line.

	ES_CENTER - Centers text in a single-line or multiline edit control.

	ES_LEFT - Left aligns text in a single-line or multiline edit control.

	ES_LOWERCASE - Converts all characters to lowercase as they are typed
	into the edit control.

	ES_MULTILINE - Designates a multiple-line edit control. (The default is
	single line.) If the ES_AUTOVSCROLL style is specified, the edit
	control shows as many lines as possible and scrolls vertically when
	the user presses the ENTER key. If ES_AUTOVSCROLL is not given, the edit
	control shows as many lines as possible and beeps if ENTER is pressed
	when no more lines can be displayed. If the ES_AUTOHSCROLL style is
	specified, the multiple-line edit control automatically scrolls
	horizontally when the caret goes past the right edge of the control. To
	start a new line, the user must press ENTER. If ES_AUTOHSCROLL is not
	given, the control automatically wraps words to the beginning of the
	next line when necessary, a new line is also started if ENTER is
	pressed. The position of the wordwrap is determined by the window size.
	If the window size changes, the wordwrap position changes and the text
	is redisplayed. Multiple-line edit controls can have scroll bars. An
	edit control with scroll bars processes its own scroll-bar messages.
	Edit controls without scroll bars scroll as described above and process
	any scroll messages sent by the parent window.

	ES_NOHIDESEL - Normally, an edit control hides the selection when
	the control loses the input focus and inverts the selection when the
	control receives the input focus. Specifying ES_NOHIDESEL deletes this
	default action.

	ES_OEMCONVERT - Text entered in the edit control is converted from the
	ANSI character set to the OEM character set and then back to ANSI. This
	ensures proper character conversion when the application calls the
	AnsiToOem() Windows function to convert an ANSI string in the edit
	control to OEM characters. This style is most useful for edit controls
	that contain filenames.

	ES_PASSWORD - Displays all characters as an asterisk (*) as they are
	typed into the edit control. An application can use the
	SetPasswordChar() member function to change the character that is
	displayed.

	ES_RIGHT - Right aligns text in a single-line or multiline edit control.

	ES_UPPERCASE - Converts all characters to uppercase as they are typed
	into the edit control.

	ES_READONLY - Prevents the user from entering or editing text in the
	edit control.

	ES_WANTRETURN - Specifies that a carriage return be inserted when the
	user presses the ENTER key while entering text into a multiple-line edit
	control in a dialog box. Without this style, pressing the ENTER key has
	the same effect as pressing the dialog box’s default push button. This
	style has no effect on a single-line edit control.*/
	VBOOL		Create(	VWindow const&	windowParent,
						VRect const&	rect,
						VUINT			nID,
						VSTRING_CONST	pszText = NULL,
						VUINT			nStyle =
										WS_BORDER |
										WS_TABSTOP |
										WS_VISIBLE |
										ES_AUTOHSCROLL |
										ES_LEFT)
	{
		VASSERT(windowParent.GetSafeWindow())

		return VWindow::Create(	windowParent,
								rect,
								WS_CHILD | nStyle,
								VTEXT("EDIT"),
								pszText,
								nID);
	}

	/** Cut the current selection, if any, in the edit control and copy
	the deleted text to the clipboard in CF_TEXT format.*/
	void		Cut() const
		{ VWINDOW_SM0(WM_CUT); }

	/** Reset the undo flag of an edit control. The undo flag is set
	whenever an operation within the edit control can be undone. The undo
	flag is automatically reset whenever the edit control receives a
	WM_SETTEXT or EM_SETHANDLE message.*/
	void		EmptyUndoBuffer() const
		{ VWINDOW_SM0(EM_EMPTYUNDOBUFFER); }

	/** Set the inclusion flag of soft line break characters on or off
	within a multiline edit control. A soft line break consists of two
	carriage returns and a linefeed and is inserted at the end of a line
	that is broken because of word wrapping. bAddEOL specifies whether
	soft line break characters are to be inserted. A value of VTRUE inserts
	the characters; a value of VFALSE removes them. This function affects
	only the buffer returned by the Handle() function and the text returned
	by Text(). It has no effect on the display of the text within the edit
	control. The FormatLines() function does not affect a line that ends
	with a hard line break. A hard line break consists of one carriage
	return and a linefeed. Note that the size of the text changes when this
	message is processed.*/
	void		FormatLines(VBOOL bAddEOL) const
	{
		VASSERT(IsMultiLine())
		VWINDOW_SM1(EM_FMTLINES, bAddEOL);
	}

	/** Determine the uppermost visible line in an edit control. The return
	value is the zero based index of the uppermost visible line in a
	multiline edit control. For single-line edit controls, the return value
	is the zero based index of the first visible character.*/
	VLONG		GetFirstVisibileLine() const
		{ return VWINDOW_SM0(EM_GETFIRSTVISIBLELINE); }

	/** Retrieve the formatting rectangle of an edit control. The formatting
	rectangle is the limiting rectangle of the text. The limiting rectangle
	is independent of the size of the edit-control window.*/
	void		GetFormattingRect(VRect& rect) const
		{ VWINDOW_SM2(EM_GETRECT, 0, rect.GetPointer()); }

	/** Copy a line of text from an edit control and place it in a specified
	buffer. nLineNumber specifies the zero based index of the line to
	retrieve from a multiline edit control. A value of zero specifies the
	topmost line. This parameter is ignored by a single-line edit control.
	pszBuffer is a pointer to the buffer that receives a copy of the line.
	The first word of the buffer specifies the maximum number of characters
	that can be copied to the buffer, and is initialized by this function.
	The return value is the number of characters copied. The return value is
	zero if the line number specified by the line parameter is greater than
	the number of lines in the edit control. The copied line does not
	contain a terminating null character.*/
	VLONG		GetLine(	VUINT	nLineNumber,
							VSTRING	pszBuffer,
							VUINT	nBufferSize) const
	{
		VASSERT(pszBuffer && nBufferSize)

		*pszBuffer = nBufferSize;
		return VWINDOW_SM2(EM_GETLINE, nLineNumber, pszBuffer);
	}

	/** A much safer version of the above function. This version gets the
	line text into a VString object which is dynamically allocated and NULL
	terminated.*/
	VLONG		GetLine(	VUINT		nLineNumber,
							VString&	strBuffer) const
	{
		VLONG nLength = GetLineLength(nLineNumber);

		if ( nLength )
		{
			/* Allocate one more byte than needed for NULL termination
			and get line.*/
			if ( strBuffer.String(NULL, nLength + 1) )
			{
				nLength = GetLine(nLineNumber, strBuffer.String(), nLength);
				strBuffer.UpdateLength(nLength);
			}
			else
				nLength = 0;
		}

		return nLength;
	}

	/** Retrieve the number of lines in a multiline edit control. The return
	value is an integer specifying the number of lines in the multiline
	edit control. If no text is in the edit control, the return value is 1.*/
	VUINT		GetLineCount() const
	{
		VASSERT(IsMultiLine())
		return (VUINT)VWINDOW_SM0(EM_GETLINECOUNT);
	}

	/** Retrieve the character index of a line in a multiline edit control.
	The character index is the number of characters from the beginning of
	the edit control to the specified line. nLineNumber specifies the
	zero-based line number. A value of -1 specifies the current line number
	(the line that contains the caret). The return value is the character
	index of the line specified in the line parameter, or it is -1 if the
	specified line number is greater than the number of lines in the edit
	control.*/
	VLONG		GetLineIndex(VLONG nLineNumber)
	{
		VASSERT(IsMultiLine())
		return VWINDOW_SM1(EM_LINEINDEX, nLineNumber);
	}

	/** Retrieve the length of a line, in characters, in a multiline edit
	control. nCharIndex specifies the character index of a character in the
	line whose length is to be retrieved. If this parameter is -1, the
	function returns the number of unselected characters on lines containing
	selected characters. For example, if the selection extended from the
	fourth character of one line through the eighth character from the end
	of the next line, the return value would be 10 (three characters on the
	first line and seven on the next).*/
	VLONG		GetLineLength(VLONG nCharIndex) const
	{
		VASSERT(IsMultiLine())
		return VWINDOW_SM1(EM_LINELENGTH, nCharIndex);
	}

	/** Retrieve the position of the scroll box (thumb) in a multiline
	edit control.*/
	VLONG		GetThumb() const
	{
		VASSERT(IsMultiLine())
		return VWINDOW_SM0(EM_GETTHUMB);
	}

	/** Retrieve a handle of the memory currently allocated for a multiline
	edit control's text. The return value is a memory handle identifying the
	buffer that holds the content of the edit control. If an error occurs,
	such as sending the message to a single-line edit control, the return
	value is zero (NULL).*/
	HLOCAL		Handle() const
	{
		VASSERT(IsMultiLine())
		return (HLOCAL)VWINDOW_SM0(EM_GETHANDLE);
	}

	/** Set the handle of the memory that will be used by a multiline edit
	control. hLocal is a handle to the memory the edit control uses to
	store the currently displayed text instead of allocating its own
	memory. If necessary, the control reallocates this memory. Before an
	application sets a new memory handle, it should call Handle() to
	retrieve the handle of the current memory buffer and should free that
	memory. An edit control automatically reallocates the given buffer
	whenever it needs additional space for text, or it removes enough text
	so that additional space is no longer needed. This function clears the
	undo buffer and the internal modification flag. The edit control window
	is redrawn. An application can call this function on a multiline edit
	control in a dialog box only if it has created the dialog box with the
	DS_LOCALEDIT style flag set.*/
	void		Handle(HLOCAL hLocal) const
	{
		VASSERT(IsMultiLine())
		VWINDOW_SM1(EM_SETHANDLE, hLocal);
	}

	/** Returns VTRUE if this window is a multiline edit control, VFALSE
	otherwise.*/
	VBOOL		IsMultiLine() const
		{ return (Style() & ES_MULTILINE) ? VTRUE : VFALSE; }

	/** Limit the amount of text the user may enter into an edit control.
	nTextLimit specifies the maximum number of characters the user can
	enter. If this parameter is zero, the text length is set to 0x7FFFFFFE
	characters for single line edit controls or 0xFFFFFFFF for multiline
	edit controls. This only limits the text the user can enter. It has no
	effect on any text already in the edit control when the function is
	called, nor does it affect the length of the text copied to the edit
	control. If an application places more text into an edit control than is
	specified in the LimitText() function, the user can edit the entire
	contents of the edit control. The default limit to the amount of text a
	user can enter in an edit control is 30,000 characters.*/
	void		LimitText(VUINT nTextLength) const
		{ VWINDOW_SM1(EM_LIMITTEXT, nTextLength); }

#if(WINVER >= 0x0400)
	/** Retrieve the current text limit, in characters, for an edit control.*/
	VLONG		LimitText() const
		{ return VWINDOW_SM0(EM_GETLIMITTEXT); }
#endif

	/** Retrieve the index of the line that contains the specified character
	index in a multiline edit control. A character index is the number of
	characters from the beginning of the edit control. nCharIndex specifies
	the character index of the character contained in the line whose number
	is to be retrieved. If the parameter is -1, either the line number of
	the current line (the line containing the caret) is retrieved or, if
	there is a selection, the line number of the line containing the
	beginning of the selection is retrieved.*/
	VLONG		LineFromChar(VLONG nCharIndex) const
	{
		VASSERT(IsMultiLine())
		return VWINDOW_SM1(EM_LINEFROMCHAR, nCharIndex);
	}

	/** Scroll the text vertically or horizontally in a multiline edit
	control. nXScroll specifies the number of characters to scroll
	horizontally. nYScroll specifies the number of lines to scroll
	vertically. The edit control does not scroll vertically past the last
	line of text in the edit control. If the current line plus the number
	of lines specified by the nYScroll parameter exceeds the total number
	of lines in the edit control, the value is adjusted so that the last
	line of the edit control is scrolled to the top of the edit control
	window. This function can be used to scroll horizontally past the last
	character of any line.*/
	void		LineScroll(	VUINT nXScroll,
							VUINT nYScroll) const
	{
		VASSERT(IsMultiLine())
		VWINDOW_SM2(EM_LINESCROLL, nXScroll, nYScroll);
	}

#if(WINVER >= 0x0400)
	/** Retrieve the widths of the left and right margins for an
	edit control.*/
	void		Margins(	VWORD& nLeftMargin,
							VWORD& nRightMargin) const
	{
		VLONG nMargins =	VWINDOW_SM0(EM_GETMARGINS);
		nLeftMargin =		LOWORD(nMargins);
		nRightMargin =		HIWORD(nMargins);
	}

	/** Set the widths of the left and right margins for an edit control. The
	function redraws the control to reflect the new margins. nMargins
	specifies the margins to set. This parameter can be a combination of the
	following values:

	EC_LEFTMARGIN - Sets the left margin.

	EC_RIGHTMARGIN - Sets the right margin.

	EC_USEFONTINFO - Uses information about the current font of the edit
	control to set the margins.

	For a single line edit control, margins are set to the average width of
	characters in the font. For a multiline edit control, the right margin
	is set to the "A" width of the font (the distance added to the current
	position before drawing a character), and the left margin is set to the
	"C" width (the distance added to the current position to provide white
	space to the right of a character). If EC_USEFONTINFO is specified,
	nLeftMargin and nRightMargin are ignored. nLeftMargin specifies the
	width of the left margin, in pixels. This value is ignored if nMargins
	does not include EC_LEFTMARGIN or if EC_USEFONTINFO is specified.
	nRightMargin specifies the width of the right margin, in pixels. This
	value is ignored if nMargins does not include EC_RIGHTMARGIN or if
	EC_USEFONTINFO is specified.*/
	void		Margins(	VUINT nMargins,
							VWORD nLeftMargin,
							VWORD nRightMargin) const
		{ VWINDOW_SM3(EM_SETMARGINS, nMargins, nLeftMargin, nRightMargin); }
#endif

	/** Determine whether the content of an edit control has been modified.
	The system maintains an internal flag indicating whether the content of
	the edit control has been changed. This flag is cleared when the edit
	control is first created; alternatively, an application can call
	Modify(VFALSE) to clear the flag. If the content of edit control has
	been modified, the return value is VTRUE, otherwise it is VFALSE.*/
	VBOOL		Modify() const
		{ return VWINDOW_SMB0(EM_GETMODIFY); }

	/** Set or clear the modification flag for an edit control. The
	modification flag indicates whether the text within the edit control has
	been modified. It is automatically set whenever the user changes the
	text. Modify() can be called to retrieve the value of the modification
	flag. bModified specifies the new value for the modification flag. A
	value of VTRUE indicates the text has been modified, and a value of
	VFALSE indicates it has not been modified.*/
	void		Modify(VBOOL bModified) const
		{ VWINDOW_SM1(EM_SETMODIFY, bModified); }

	/** Retrieve the password character displayed in an edit control when the
	user enters text. The return value specifies the character to be
	displayed in place of the character typed by the user. The return
	value is NULL if no password character exists. If the edit control is
	created with the ES_PASSWORD style, the default password character is
	set to an asterisk (*).*/
	VUINT		PasswordCharacter() const
		{ return (VUINT)VWINDOW_SM0(EM_GETPASSWORDCHAR); }

	/** Set or remove the password character for a single line edit control
	when the user types text. When a password character is set, that
	character is displayed in place of each character the user types. nChar
	specifies the character to be displayed in place of the character typed
	by the user. If this parameter is zero, the characters typed by the user
	are displayed.*/
	void		PasswordCharacter(VUINT nChar) const
		{ VWINDOW_SM1(EM_SETPASSWORDCHAR, nChar); }

	/** Copy the current content of the clipboard to the edit control at the
	current caret position. Data is inserted only if the clipboard contains
	data in CF_TEXT format.*/
	void		Paste() const
		{ VWINDOW_SM0(WM_PASTE); }

#if(WINVER >= 0x0400)
	/** Retrieve the coordinates of the specified character in an edit
	control. nCharIndex specifies the zero based index of the character. A
	returned coordinate can be negative if the character has been scrolled
	outside the edit control's client area. The coordinates are truncated
	to integer values. If nCharIndex is the index of a line delimiter, the
	returned coordinates are of the position just past the last visible
	character in the line. If nCharIndex is greater than the index of the
	last character in the control, the returned coordinates are of the
	position just past the last character of the control. The return value
	is the position of the character, (x, y). For a single-line edit
	control, the y-coordinate is always zero.*/
	VLONG		PositionFromChar(VWPARAM nCharIndex) const
		{ return (EM_POSFROMCHAR, nCharIndex, 0); }
#endif

	/** Returns VTRUE is edit control is read only, VFALSE otherwise.*/
	VBOOL		ReadOnly() const
		{ return (Style() & ES_READONLY) ? VTRUE : VFALSE; }

	/** Set or remove the read-only style (ES_READONLY) of an edit control.
	bReadOnly specifies whether to set or remove the ES_READONLY style. A
	value of VTRUE sets the ES_READONLY style, while a value of FALSE
	removes the ES_READONLY style.*/
	void		ReadOnly(VBOOL bReadOnly) const
		{ VWINDOW_SM1(EM_SETREADONLY, bReadOnly); }

	/** Scroll the text vertically in a multiline edit control. This function
	is equivalent to sending a WM_VSCROLL message to the edit control.
	nScroll specifies the action the scroll bar is to take. This parameter
	may be one of the following values:

	SB_LINEDOWN - Scrolls down one line.

	SB_LINEUP - Scrolls up one line.

	SB_PAGEDOWN - Scrolls down one page.

	SB_PAGEUP - Scrolls up one page.

	If the function is successful, the high-order word of the return value
	is VTRUE, and the low-order word is the number of lines that the
	command scrolls. The number returned may not be the same as the actual
	number of lines scrolled if the scrolling moves to the beginning or the
	end of the text. If the nScroll parameter specifies an invalid value,
	the return value is VFALSE.*/
	VLONG		Scroll(VUINT nScroll) const
	{
		VASSERT(IsMultiLine())
		return VWINDOW_SM1(EM_SCROLL, nScroll);
	}

	/** Scroll the caret into view in an edit control.*/
	VLONG		ScrollCaret() const
		{ return VWINDOW_SM0(EM_SCROLLCARET); }

	/** Get the starting and ending character positions of the current
	selection in an edit control. pnStartPosition is a pointer to a 32-bit
	value that receives the starting position of the selection. This
	parameter can be NULL. pnEndPosition is a pointer to a 32-bit value that
	receives the position of the first nonselected character after the end
	of the selection. This parameter can be NULL. The return value is a
	zero-based 32-bit value with the starting position of the selection in
	the low-order word and the position of the first character after the
	last selected character in the high-order word. If either of these
	values exceeds 65,535, the return value is -1.*/
	VLONG		Selection(	VDWORD* pnStartPosition,
							VDWORD* pnEndPosition) const
		{ return VWINDOW_SM2(EM_GETSEL, pnStartPosition, pnEndPosition); }

	/** Select a range of characters in an edit control. nStartPosition
	specifies the starting character position of the selection. nEndPosition
	specifies the ending character position of the selection. If the
	nStartPosition parameter is 0 and the nEnd parameter is -1, all the
	text in the edit control is selected. If nStart is -1, any current
	selection is removed. The caret is placed at the end of the selection
	indicated by the greater of the two values nEndPosition and
	nStartPosition.*/
	void		Selection(	VDWORD nStartPosition,
							VDWORD nEndPosition) const
		{ VWINDOW_SM2(EM_SETSEL, nStartPosition, nEndPosition); }

	/** Replace the current selection in an edit control with the specified
	text. pszReplace is a pointer to a null terminated string containing the
	replacement text. bCanUndo specifies whether the replacement operation
	can be undone. If this is VTRUE, the operation can be undone. If this
	is VFALSE , the operation cannot be undone. If there is no current
	selection, the replacement text is inserted at the current location
	of the caret.*/
	void		SelectionReplace(	VSTRING_CONST	pszReplace,
									VBOOL			bCanUndo = VTRUE) const
		{ VWINDOW_SM2(EM_REPLACESEL, bCanUndo, pszReplace); }

	/** Select all characters in the edit control.*/
	void		SelectAll() const
		{ Selection(0, -1); }

	/** Clear all currently selected text in the edit control.*/
	void		SelectNone() const
		{ Selection(-1, -1); }

	/** Set the formatting rectangle of a multiline edit control. The
	formatting rectangle is the limiting rectangle of the text. The
	limiting rectangle is independent of the size of the edit control
	window. When the edit control is first created, the formatting
	rectangle is the same as the client area of the edit control window. By
	using the this function, an application can make the formatting
	rectangle larger or smaller than the edit control window. This function
	causes the text of the edit control to be redrawn if bRedraw is VTRUE.
	If the edit control does not have a horizontal scroll bar, and the
	formatting rectangle is set to be larger than the edit control window,
	lines of text exceeding the width of the edit control window (but
	smaller than the width of the formatting rectangle) are clipped instead
	of wrapped. If the edit control contains a border, the formatting
	rectangle is reduced by the size of the border. If you are adjusting
	the rectangle returned by a GeFormattingRect() call, you must remove
	the size of the border before using the rectangle with this function.*/
	void		SetFormattingRect(	VRect const&	rect,
									VBOOL			bRedraw = VTRUE) const
	{
		VWINDOW_SM2(	(bRedraw)
						? EM_SETRECT
						: EM_SETRECTNP,
						0,
						rect.GetPointer());
	}

	/** Set the tab stops in a multiline edit control. When text is copied
	to the control, any tab character in the text causes space to be
	generated up to the next tab stop. nNumTabStops specifies the number of
	tab stops contained in the pnTabs parameter. If this parameter is zero,
	the pnTabs parameter is ignored and default tab stops are set at every
	32 dialog template units. If this parameter is 1, tab stops are set at
	every n dialog template units, where n is the distance pointed to by the
	pnTabs parameter. If the nNumTabStops parameter is greater than 1,
	pnTabs points to an array of tab stops. pnTabs is a pointer to an array
	of unsigned integers specifying the tab stops, in dialog template units.
	If the nNumTabStops parameter is 1, pnTabs points to an unsigned integer
	containing the distance between all tab stops, in dialog template units.
	The SetTabStops() function does not automatically redraw the edit control
	window. If the application is changing the tab stops for text already
	in the edit control, it should call the Invalidate() function to redraw
	the edit control window. The values specified by the pnTabs parameter
	are in dialog template units, which are the device independent units
	used in dialog box templates. To convert measurements from dialog
	template units to screen units (pixels), use the MapDialogRect()
	function. If all the tabs are set, the return value is VTRUE,
	otherwise it is VFALSE.*/
	VBOOL		SetTabStops(	VUINT	nNumTabStops,
								VUINT*	pnTabs) const
	{
		VASSERT(IsMultiLine())
		return VWINDOW_SMB2(EM_SETTABSTOPS, nNumTabStops, pnTabs);
	}

	/** Undo the last edit control operation. An undo operation can also be
	undone. For example, you can restore deleted text with the first Undo(),
	and remove the text again with a second Undo() as long as there is no
	intervening edit operation. For a single line edit control, the return
	value is always VTRUE. For a multiline edit control, the return value
	is VTRUE if the undo operation is successful, or VFALSE if the undo
	operation fails.*/
	VBOOL		Undo() const
		{ return VWINDOW_SMB0(EM_UNDO); }

	/** Retrieve the address of the current word wrap function. A word wrap
	function scans a text buffer that contains text to be sent to the
	display, looking for the first word that does not fit on the current
	display line. The word wrap function places this word at the beginning
	of the next line on the display. A word wrap function defines the point
	at which the system should break a line of text for multiline edit
	controls, usually at a space character that separates two words. The
	return value specifies the address of the application defined word
	wrap function. The return value is NULL if no word wrap function
	exists.*/
	EDITWORDBREAKPROC	WordBreakProc() const
		{ return (EDITWORDBREAKPROC)VWINDOW_SM0(EM_GETWORDBREAKPROC); }

	/** Replace the default word wrap function with an application-defined
	word wrap function. ewbp specifies the address of the application
	defined word wrap function. For more information about breaking
	lines, see the description of the EditWordBreakProc callback function.
	See WordBreakProc() for more information.*/
	void				WordBreakProc(EDITWORDBREAKPROC pfn) const
		{ VWINDOW_SM2(EM_SETWORDBREAKPROC, 0, pfn); }
};

#endif /* VEDIT*/
