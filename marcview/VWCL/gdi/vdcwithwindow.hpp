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

#ifndef VDCWITHWINDOW
#define VDCWITHWINDOW

#include "../vstandard.h"
#include "../windows/vwindow.hpp"
#include "vdc.hpp"

/** VDCWithWindow is a VDC object that requires a window for proper usage.
It is primarily a base class for more specific device context classes.*/
class VDCWithWindow : public VDC
{
public:
	/** Standard constructor initializes by saving a pointer to the
	associated VWindow. The internal device context handle (HDC) is
	initialized to NULL and must be set to a valid value elsewhere.*/
	VDCWithWindow(VWindow const& window)
	{
		m_pWindow = (VWindow*)&window;
		VASSERT(m_pWindow)
	}

	/** Virtual destructor calls local Free() variant, which uses
	ReleaseDC(), not the base class DeleteDC().*/
	virtual ~VDCWithWindow()
		{ Free(); }

	/** Return the internal VWindow object reference.*/
	VWindow&		GetWindow() const
	{
		VASSERT(m_pWindow)
		return (VWindow&)VREF_PTR(m_pWindow);
	}

	/** Override uses ReleaseDC(), not the base class DeleteDC(), but still
	honors m_bFreeHDC member to skip if the device context is not owned by
	this object.*/
	virtual void	Free()
	{
		if ( m_bFreeHDC && m_hDC )
		{
			VASSERT(m_pWindow)
			ReleaseDC(m_pWindow->GetHandle(), m_hDC);
			m_hDC = NULL;
		}
	}

protected:
	/** Embedded Members.*/
	VWindow*		m_pWindow;
};

#endif /* VDCWITHWINDOW*/
