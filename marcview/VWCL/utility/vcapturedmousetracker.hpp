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

#ifndef VCAPTUREDMOUSETRACKER
#define VCAPTUREDMOUSETRACKER

#include "../vstandard.h"
#include "../numbers/vbitmask.hpp"
#include "../structures/vpoint.hpp"
#include "../windows/vwindow.hpp"

/** VCapturedMouseTracker tracks mouse movement of the cursor postion during
a captured mouse condition, allowing keyboard movement, and allows user to
cancel by using the right mouse button or the escape key. It also allows
reversal of the mouse keys, so that the left button cancels the operation.*/
class VCapturedMouseTracker
{
public:
	/** Default constructor initializes class options and can set the
	keyboard increment, which determines when the user moves the tracker
	with the keyboard, how many pixels are used to make the movement.*/
	VCapturedMouseTracker(	VUINT	nOptions = OPTION_DEFAULT,
							VUSHORT	nKeyboardIncrement = 5)
	{
		m_Options = nOptions;
		m_nKeyboardIncrement = nKeyboardIncrement;
	}

	/** Virtual destructor does nothing.*/
	virtual ~VCapturedMouseTracker()
		{;}

	/** Options that can be OR'd for special effects. Defaults to
	OPTION_DEFAULT. This enumeration includes options that apply to this
	class and known VWCL derivatives.*/
	enum	{		OPTION_RIGHT_MOUSE_TRACK =				0x0001,
					OPTION_ALLOW_INVERTED_TRACKING =		0x0002,
					OPTION_DISABLE_KEYBOARD_MOVE =			0x0004,
					OPTION_DEFAULT =
						OPTION_ALLOW_INVERTED_TRACKING,
					OPTION_RECT_TRACKER_FIX_RECT_LEFT =		0x0008,
					OPTION_RECT_TRACKER_FIX_RECT_RIGHT =	0x0010,
					OPTION_RECT_TRACKER_FIX_RECT_TOP =		0x0020,
					OPTION_RECT_TRACKER_FIX_RECT_BOTTOM =	0x0040,
					OPTION_RECT_TRACKER_DISABLE_DRAWING =	0x0080,
					OPTION_RECT_TRACKER_ENFORCE_LIMITS =	0x0100,
			};

	/** Return a reference to the number of pixels the cursor gets moved
	when the user uses the arrow keys during tracking.*/
	VUSHORT&		GetKeyboardIncrement() const
		{ return (VUSHORT&)m_nKeyboardIncrement; }

	/** Return a reference to the options that apply to this class and
	those derived from it.*/
	VBitmask&		GetOptions() const
		{ return (VBitmask&)m_Options; }

	/** Start tracking mouse in VWindow starting at ptFrom. Returns VTRUE on
	success, VFALSE if the user cancelled the operation or the resulting
	point was the same as the from point. The ending position will be
	stored in the ptResult VPoint object on success. This function handles
	capturing the mouse, and releasing it when needed. The mouse should NOT
	be captured before calling this function. During the tracking, the
	virtual function OnTract() will be called each time the mouse position
	changes. If the users presses the enter key during tracking, it has the
	same effect as stopping the tracking by letting the mouse button up.
	This is a valid way of saying "I'm done, and return the result".*/
	VBOOL			Track(	VWindow const&	window,
							VPoint const&	ptFrom,
							VPoint&			ptResult)
	{
		VASSERT(window.GetSafeWindow())

		/* Initialize ptResult to ptFrom.*/
		ptResult = ptFrom;

		/* This tracks the current position. Initialize to ptFrom.*/
		VPoint ptCurrent(ptFrom);

		/* Set initial variables.*/
		VBOOL	bResult =			VFALSE;
		VBOOL	bKeepGoing =		VTRUE;
		VBOOL	bReleaseCapture =	VFALSE;
		MSG		msg;

		/* Capture the mouse?*/
		if ( !window.IsMouseCaptured() )
		{
			window.SetCapture();
			bReleaseCapture = VTRUE;
		}

		while ( bKeepGoing && GetMessage(&msg, NULL, 0, 0) )
		{
			/* If we lost the capture for any reason, stop!*/
			if ( GetCapture() != window.GetHandle() )
				break;

			switch ( msg.message )
			{
				case WM_LBUTTONDOWN:
					if ( m_Options.IsNotSet(OPTION_RIGHT_MOUSE_TRACK) )
						break;
					/* Fall through to WM_RBUTTONDOWN.*/

				case WM_RBUTTONDOWN:
					bKeepGoing = VFALSE;
					break;

				case WM_RBUTTONUP:
					if ( m_Options.IsNotSet(OPTION_RIGHT_MOUSE_TRACK) )
						break;
					/* Fall through to WM_LBUTTONUP.*/

				case WM_LBUTTONUP:
					bKeepGoing =	VFALSE;
					bResult =		VTRUE;
					break;

				case WM_KEYDOWN:
				{
					if ( msg.wParam == VK_ESCAPE )
						bKeepGoing = VFALSE;
					else if ( msg.wParam == VK_RETURN )
					{
						bKeepGoing =	VFALSE;
						bResult =		VTRUE;
					}
					else if (
						(m_Options.IsNotSet(OPTION_DISABLE_KEYBOARD_MOVE)) &&
						(msg.wParam == VK_LEFT ||
						msg.wParam == VK_RIGHT ||
						msg.wParam == VK_UP ||
						msg.wParam == VK_DOWN) )
					{
						/* Get cursor position into ptCurrent.*/
						ptCurrent.GetCursorPosition();

						/* Modify position based on keystroke.*/
						if ( msg.wParam == VK_LEFT )
							ptCurrent.SubtractX(m_nKeyboardIncrement);
						else if ( msg.wParam == VK_RIGHT )
							ptCurrent.AddX(m_nKeyboardIncrement);
						else if ( msg.wParam == VK_UP )
							ptCurrent.SubtractY(m_nKeyboardIncrement);
						else
							ptCurrent.AddY(m_nKeyboardIncrement);

						/* Move the screen cursor to new location and convert
						ptCurrent back to client coordinates.*/
						SetCursorPos(VPOINT_BREAK(ptCurrent));
						window.ScreenToClient(ptCurrent);

						/* Call virtual function.*/
						CallOnTrack(window, ptFrom, ptCurrent);
					}

					break;
				}

				/* Call virtual function to show what we are doing.*/
				case WM_MOUSEMOVE:
				{
					ptCurrent.Set(	(VINT)(VSHORT)LOWORD(msg.lParam),
									(VINT)(VSHORT)HIWORD(msg.lParam));
					CallOnTrack(window, ptFrom, ptCurrent);
					break;
				}

				default:
					((VWindow*)&window)->WindowProc(	msg.hwnd,
														msg.message,
														msg.wParam,
														msg.lParam);
					break;
			}
		}

		/* Did the points change while being tracked?*/
		if ( bResult && ptCurrent == ptFrom )
			bResult = VFALSE;

		/* Release the mouse capturing?*/
		window.IsMouseCaptured(bReleaseCapture);

		/* Save out parameter?*/
		if ( bResult )
			ptResult = ptCurrent;

		/* Call virtual function to notify of end of tracking.*/
		OnTrackEnd(window, ptFrom, ptCurrent, bResult);

		return bResult;
	}

protected:
	/** Local function that calls OnTrack() and updates the cursor position
	is OnTrack() changes them.*/
	void			CallOnTrack(	VWindow const&	window,
									VPoint const&	ptFrom,
									VPoint&			ptCurrent)
	{
		/* Save original point.*/
		VPoint ptOriginal = ptCurrent;

		/* Call virtual function.*/
		OnTrack(window, ptFrom, ptCurrent);

		/* Did the point change?*/
		if ( ptCurrent != ptOriginal )
		{
			/* Reuse ptOriginal, leave ptCurrent alone.*/
			ptOriginal = ptCurrent;

			/* Move the screen cursor to new location.*/
			window.ClientToScreen(ptOriginal);
			SetCursorPos(VPOINT_BREAK(ptOriginal));
		}
	}

	/** This function gets called during tracking. Base class does nothing.
	It is valid to modify the points in ptCurrent if desired. On returning
	from this function, if the points are modified, the cursor position
	will be set to the new coordinates.*/
	virtual void	OnTrack(	VWindow const&	window,
								VPoint const&	ptFrom,
								VPoint&			ptCurrent)
		{;}

	/** This notification function gets call when the tracking of the mouse
	is complete. If bSuccess is VTRUE, it means the user did not cancel the
	tracking, otherwise it is VFALSE.*/
	virtual void	OnTrackEnd(	VWindow const&	window,
								VPoint const&	ptFrom,
								VPoint&			ptEnd,
								VBOOL			bSuccess)
		{;}

	/** Embedded Members.*/
	VBitmask		m_Options;
	VUSHORT			m_nKeyboardIncrement;

};

#endif /* VCAPTUREDMOUSETRACKER*/
