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

#ifndef VTEMPWINDOW
#define VTEMPWINDOW

#include "../vstandard.h"
#include "vwindow.hpp"

/** VTempWindow is used to quickly wrap a HWND into a non-VWCL controlled
VWindow object. This is mostly useful when needing to pass a VWindow object
reference to a method that requires one, when the only thing you know is
the windows HWND. The VWindow::m_hWindow is set to NULL in the destructor
so the VWindow base class does not attempt to destroy the window. The window
handle (HWND) is not sub-classed or owned in any way by VWCL.*/
class VTempWindow : public VWindow
{
public:
	/** Default constructor is normally passed a windows handle (HWND).*/
	VTempWindow(HWND hWnd)
		{ m_hWindow = hWnd; }

	/** Virtual destructor sets the internal handle (HWND) to NULL,
	preventing the base class for destroying it.*/
	virtual ~VTempWindow()
		{ m_hWindow = NULL; }
};

#endif	/* VTEMPWINDOW*/
