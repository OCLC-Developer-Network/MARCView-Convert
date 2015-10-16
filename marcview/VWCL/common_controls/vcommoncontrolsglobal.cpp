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

#include "../windows/vwindow.hpp"

/* Pointer to function in COMCTL32.DLL. VWCL attempts to initialize common
controls only as needed. If the common control library exports the
InitCommonControlsEx() function, this is very efficient. Otherwise VWCL
will use the older InitCommonControls() to initialize all controls.*/
typedef VWINBOOL (WINAPI* VINITCOMMONCONTROLSEX)
	(INITCOMMONCONTROLSEX* pInitCtrls);

/** Local global that tells us what controls have been initialied.*/
VUINT gnControlsInitialized = 0;

/** Global function to initialize a single common control as needed.*/
void VInitCommonControl(VRTTI_TYPE nCommonControlType)
{
	/* Anything to do? If gnControlsInitialized == 0xffffffff
	then all control have been initialized.*/
	if ( gnControlsInitialized != 0xffffffff )
	{
		/* Get the bitmask for the control in question.*/
		VUINT nBits = 0;

		switch ( nCommonControlType )
		{
			case VWindow::VWCL_RTTI_LISTVIEW:
				nBits = ICC_LISTVIEW_CLASSES;
				break;

			case VWindow::VWCL_RTTI_TOOLBAR:
			case VWindow::VWCL_RTTI_STATUSBAR:
			case VWindow::VWCL_RTTI_TOOLTIP:
				nBits = ICC_BAR_CLASSES;
				break;

			case VWindow::VWCL_RTTI_TAB:
			case VWindow::VWCL_RTTI_TAB_WINDOW:
				nBits = ICC_TAB_CLASSES;
				break;

			case VWindow::VWCL_RTTI_TREEVIEW:
			case VWindow::VWCL_RTTI_DIRECTORY_TREEVIEW:
			case VWindow::VWCL_RTTI_HIERARCHICAL_DATABASE_TREEVIEW:
				nBits = ICC_TREEVIEW_CLASSES;
				break;

			case VWindow::VWCL_RTTI_PROGRESS:
				nBits = ICC_PROGRESS_CLASS;
				break;

			case VWindow::VWCL_RTTI_REBAR:
				nBits = ICC_COOL_CLASSES;
				break;

			#ifdef VWCL_DEBUG
				/* An unknown or unsupported common control type.*/
				default:
					VASSERT(VFALSE)
			#endif
		}

		/* Have we already initialized a control of this type? If yes, skip.*/
		if ( nBits && !(gnControlsInitialized & nBits) )
		{
			HINSTANCE hLib = GetModuleHandle(VTEXT("COMCTL32.DLL"));
			VASSERT(hLib)

			if ( hLib )
			{
				/* See if we find a InitCommonControlsEx() function in the
				COMCTL32.DLL library.*/
				VINITCOMMONCONTROLSEX pfn = (VINITCOMMONCONTROLSEX)
					GetProcAddress(	hLib,
									"InitCommonControlsEx");

				if ( pfn )
				{
					/* Populate INITCOMMONCONTROLSEX struct.*/
					INITCOMMONCONTROLSEX icc =
						{	sizeof(INITCOMMONCONTROLSEX),
							nBits
						};

					/* Make call and update the controls initialized bits.*/
					if ( pfn(&icc) )
						gnControlsInitialized |= nBits;

					return;
				}
			}

			/* Revert to old InitCommonControls() function and
			populate gnControlsInitialized with all bits.*/
			InitCommonControls();
			gnControlsInitialized = 0xffffffff;
		}
	}
}
