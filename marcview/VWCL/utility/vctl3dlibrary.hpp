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

#ifndef VCTL3DLIBRARY
#define VCTL3DLIBRARY

#include "../vstandard.h"
#include "vlibrary.hpp"

/** CTL3D runtime pointers to function prototypes.*/
typedef VWINBOOL (WINAPI* VCTL3D)(HANDLE);
typedef VWINBOOL (WINAPI* VCTL3D2)(HANDLE, WORD);

/** VCtl3DLibrary enables the dynamic use of the Microsoft 3-D Controls
Library. Simply creating an object os this type will start CTL3Dxx.DLL and
subclass all dialog boxes to use 3-D controls. Create will fail if NO3D is
specified on the command line. Objects of this class should be kept in
scope throughout the running of your application, or until you no longer
want 3D dialog support. This support is not needed on Windows NT 4.x or
higher, or Windows 95 or higher.*/
class VCtl3DLibrary : public VLibrary
{
public:
	/** Default constructor initializes the CTL3D library as needed.
	Skips if NO3D is specified on the application command line.*/
	VCtl3DLibrary()
	{
		/* VWCL must be initialized first!*/
		VASSERT(VGetInstanceHandle())

		/* Check for NO3D on command line.*/
		if ( !VGetApp().IsCommandLineOption(VTEXT("NO3D")) )
		{
			/* Try to load the library.*/
			if ( Set("CTL3D32.DLL") )
			{
				VCTL3D pReg =
					(VCTL3D)GetProcAddress(m_hInstance, "Ctl3dRegister");

				VCTL3D pSub =
					(VCTL3D)GetProcAddress(m_hInstance, "Ctl3dAutoSubclass");

				if ( pReg && pSub )
				{
					/* Register this app and subclass all dialogs.*/
					pReg(VGetInstanceHandle());
					pSub(VGetInstanceHandle());
				}
				else
					Free();
			}
		}
	}

	/** Virtual destructor unregisters CTL3D support as needed.*/
	virtual ~VCtl3DLibrary()
	{
		/* Un-register this app from CTL3Dxx.DLL?*/
		if ( m_hInstance )
		{
			VCTL3D pUnReg =
				(VCTL3D)GetProcAddress(m_hInstance, "Ctl3dUnregister");

			if ( pUnReg )
				pUnReg(VGetInstanceHandle());
		}
	}
};

#endif /* VCTL3DLIBRARY*/