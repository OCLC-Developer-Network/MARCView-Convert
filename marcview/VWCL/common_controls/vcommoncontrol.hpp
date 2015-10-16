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

#ifndef VCOMMONCONTROL
#define VCOMMONCONTROL

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../windows/vwindow.hpp"

/** VWCL attempts to initialize common controls only as needed. If the
common control library exports the InitCommonControlsEx() function, this is
very efficient. Otherwise VWCL will use the older InitCommonControls() to
initialize all controls. This function is implemented in
vcommoncontrolsglobal.cpp which must be linked with any program that uses
VWCL support for common controls.*/
void VInitCommonControl(VRTTI_TYPE nCommonControlType);

/** VCommonControl is the base class for other Win32 Common Control classes
supporting reflected notification messages. Notification messages are those
that are normally passed to the parent window of a common control, where
the parent window is expected to handle the message. This is not good for
object oriented design, so VWCL has support for intercepting these messages
and routing them back to the derived common control object through the
OnReflectedNotify() member function. This function in turn calls one of the
Onxxx() functions to carry out the notification. The nCommonControlResult
reference that is passed to the Onxxx() function is the result code that
will be returned to Windows. For most messages this return value is ignored,
but it is always control dependant what the result code return value should
be. It is best to consult a good common control reference manual to
determine the correct result. VWCL's handling of reflected notification
routing is determine by the return value. Return 0 if message was handled,
-1 if handled and parent should NOT be notified, or 1 if message was not
handled and parent should be notified. If -1 is returned, derived classes
must also set the value in nCommonControlResult to the return value
expected by the common control. VWCL's VWindow::WindowProcShared(WM_NOTIFY)
message of the parent window of the common control is where the routing to
OnReflectedNotify() occurs. If the value of -1 is returned from
OnReflectedNotify(), VWindow::OnNotify() will not be called.*/
class VCommonControl : public VWindow
{
protected:
	/** Protected constructor used only by derived classes. It is not valid
	to directly create an object of this type. This constructor verifies
	that the Win32 common controls libary has been initialized for support
	for the common control type. This is very efficient, since it usually
	means all controls are not initialized, rather they are initialized
	only when actually used.*/
	VCommonControl(VRTTI_TYPE nRTTI)
		: VWindow(nRTTI)
		{ VInitCommonControl(nRTTI); }

	/** Called when the user clicks on the common control window.*/
	virtual VLONG	OnClick(	NMHDR& nmhdr,
								VLONG& nCommonControlResult)
		{ return 1; }

	/** Called when the user double-clicks on the common control window.*/
	virtual VLONG	OnDoubleClick(	NMHDR& nmhdr,
									VLONG& nCommonControlResult)
		{ return 1; }

	/** Called when the common control window loses the input focus.*/
	virtual VLONG	OnKillFocus(	NMHDR& nmhdr,
									VLONG& nCommonControlResult)
		{ return 1; }

	/** Called when an out of memory error condition occurs.*/
	virtual VLONG	OnOutOfMemory(	NMHDR& nmhdr,
									VLONG& nCommonControlResult)
		{ return 1; }

	/** Called when the common control window receives the input focus.*/
	virtual VLONG	OnSetFocus(	NMHDR& nmhdr,
								VLONG& nCommonControlResult)
		{ return 1; }

	/** Called when the user presses the return key while the focus
	is in the common control window.*/
	virtual VLONG	OnReturn(	NMHDR& nmhdr,
								VLONG& nCommonControlResult)
		{ return 1; }

	/** Called when the user right-clicks on the common control window.*/
	virtual VLONG	OnRightClick(	NMHDR& nmhdr,
									VLONG& nCommonControlResult)
		{ return 1; }

	/** Called when the user double-right-clicks on the common control window.*/
	virtual VLONG	OnRightDoubleClick(	NMHDR& nmhdr,
										VLONG& nCommonControlResult)
		{ return 1; }

	/** Watches for notifications and calls reflected notify handlers to
	perform extended operations.*/
	virtual VLONG	OnReflectedNotify(	NMHDR& nmhdr,
										VLONG& nCommonControlResult)
	{
		switch ( nmhdr.code )
		{
			case NM_CLICK:
				return OnClick(nmhdr, nCommonControlResult);

			case NM_DBLCLK:
				return OnDoubleClick(nmhdr, nCommonControlResult);

			case NM_KILLFOCUS:
				return OnKillFocus(nmhdr, nCommonControlResult);

			case NM_OUTOFMEMORY:
				return OnOutOfMemory(nmhdr, nCommonControlResult);

			case NM_RCLICK:
				return OnRightClick(nmhdr, nCommonControlResult);

			case NM_RDBLCLK:
				return OnRightDoubleClick(nmhdr, nCommonControlResult);

			case NM_RETURN:
				return OnReturn(nmhdr, nCommonControlResult);

			case NM_SETFOCUS:
				return OnSetFocus(nmhdr, nCommonControlResult);
		}

		return VWindow::OnReflectedNotify(nmhdr, nCommonControlResult);
	}
};

#endif /* VCOMMONCONTROL*/
