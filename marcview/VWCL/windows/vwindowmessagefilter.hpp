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

#ifndef VWINDOWMESSAGEFILTER
#define VWINDOWMESSAGEFILTER

#include "../vstandard.h"
#include "../collections/vlinkedlistitem.hpp"
#include "../numbers/vreferencecount.hpp"

/* Forward declarations.*/
class VWindow;

/** VWindowMessageFilter is used to inspect messages sent to a VWindow
object before that object has a chance itself to handle the message. It is
not derived from VWindow, so it can be used any time message routing from a
VWindow object to an arbtitrary object of another type is needed. See the
OnMessageFilter() method and VWindow::InstallMessageFilter() for more
information.*/
class VWindowMessageFilter : private VLinkedListItem
{
friend class VWindow;

public:
	/** Destructor implemented in vwindow.cpp.*/
	~VWindowMessageFilter();

protected:
	/** See VWindow::InstallMessageFilter() for more information. An override
	of this function is getting messages sent to it BEFORE its associated
	VWindow. This is essentially an override of VWindow::WindowProc() but is
	kept separate to avoid confusion. If the message filter completely
	handles the message, and the associated VWindow should not be allowed
	to process the message, you should return VFALSE. Returning VTRUE
	allows the associated VWindow to handle the message. This is a pure
	virtual function that must be overriden in a derived class. IT IS NOT
	VALID for an override of this function to delete a VWindow object,
	whether the window parameter or another. This message should be
	considered a notification only, not a time to respond by deleting
	VWindow objects. It is valid to remove this filter from a VWindow
	during this notification.*/
	virtual VBOOL	OnMessageFilter(	VWindow&	window,
										VUINT		nMessage,
										VWPARAM		wParam,
										VLPARAM		lParam) = 0;

private:
	/** Used only by VWindow internally.*/
	VReferenceCount	m_ReferenceCount;
};

#endif	/* VWINDOWMESSAGEFILTER*/
