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

#ifndef VWINDOWDC
#define VWINDOWDC

#include "../vstandard.h"
#include "../windows/vwindow.hpp"
#include "vdcwithwindow.hpp"

/** VWindowDC wraps Device Contexts (DC's) for drawing in non-client area
of window which includes the window frame and caption areas.*/
class VWindowDC : public VDCWithWindow
{
public:
	/** Standard constructor initializes class by getting the DC for the
	window via the GetWindowDC() function. If the window referenced by
	window is not associated with a Windows Window (VWindow::GetHandle()
	returns NULL), the device context wrapped by this object will be for
	the whole screen, not the window parameter.*/
	VWindowDC(VWindow const& window) : VDCWithWindow(window)
	{
		m_hDC = GetWindowDC(window.GetHandle());
		VASSERT(m_hDC)
	}
};


#endif /* VWINDOWDC*/
