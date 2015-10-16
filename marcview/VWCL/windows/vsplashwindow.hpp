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

#ifndef VSPLASHWINDOW
#define VSPLASHWINDOW

#include "../vstandard.h"
#include "../gdi/vbitmap.hpp"
#include "../numbers/vbitmask.hpp"
#include "../numbers/vrtti.hpp"
#include "../windows/vwindow.hpp"
#include "vtempwindow.hpp"

/** VSplashWindow creates a simple (16 color) splash screen. This class
will not create the window if NOLOGO is specified on the application
command line. In addition, no palette management is done by this class, so
attempting to show more than 16 colors can result in an undesired quality
display.*/
class VSplashWindow : public VWindow
{
public:
	/** Default constructor initializes RTTI information and sets
	the VWindow option to receive OnPaint() messages.*/
	VSplashWindow(VRTTI_TYPE nRTTI = VWCL_RTTI_SPLASH_WINDOW)
		: VWindow(nRTTI)
	{
		m_pBitmap = NULL;
		m_Options = OPTION_NEED_ON_PAINT;
	}

	/** Virtual destructor verifies internal bitmap is destroyed.*/
	virtual ~VSplashWindow()
		{ DeleteBitmapObject(); }

	/** Create the splash window (Use Destroy() or destructor to kill it).
	pszBitmap is the string name of the bitmap resource to show, which is
	loaded from hResource, or VGetResourceHandle() if NULL. See the
	alternate Create() function to specifiy the bitmap using a bitmap
	resource ID rather than the string name. Normally the splash window is
	shown until destructed, which is normally performed in the main window
	SetupWindow() virtual function override, or at another point when
	initialization is complete. To force the splash window to be seen for
	at least x number of seconds, pass the number of seconds as the
	nMinWaitSeconds parameter.*/
	VBOOL			Create(	VSTRING_CONST	pszBitmap,
							HINSTANCE		hResource = NULL,
							VUINT			nMinWaitSeconds = 0)
	{
		/* This must be given!*/
		VASSERT(pszBitmap)

		/* Check for NOLOGO on command line.*/
		if ( !VGetApp().IsCommandLineOption(VTEXT("NOLOGO")) )
		{
			m_pBitmap = VNEW VBitmap(pszBitmap, hResource);

			if ( m_pBitmap && m_pBitmap->GetHandle() )
			{
				/* Create the window same size as bitmap.*/
				VRect r(	0,
							0,
							m_pBitmap->GetWidth(),
							m_pBitmap->GetHeight());

				if ( VWindow::Create(	VTempWindow(GetDesktopWindow()),
										r,
										WS_POPUP) )
				{
					Center();
					Show();
					Update();

					if ( nMinWaitSeconds )
					{
						/* Kill nMinWaitSecond's.*/
						VUINT nNow = GetCurrentTime();
						while (	GetCurrentTime() < nNow +
								(nMinWaitSeconds * 1000) );
					}

					return VTRUE;
				}
			}

			DeleteBitmapObject();

			VASSERT(VFALSE)
		}

		return VFALSE;
	}

	/** Same as above, but uses bitmap ID rather than string name.*/
	VBOOL			Create(	VUINT		nBitmapID,
							HINSTANCE	hResource = NULL,
							VUINT		nMinWaitSeconds = 0)
	{
		VASSERT(nBitmapID)

		return Create(	MAKEINTRESOURCE(nBitmapID),
						hResource,
						nMinWaitSeconds);
	}

protected:
	/** Delete the bitmap object as needed.*/
	void			DeleteBitmapObject()
		{ VDELETE_NULL(m_pBitmap) }

	/** Override of OnPaint paints splash bitmap.*/
	virtual VBOOL	OnPaint(VDC& dc)
	{
		if ( m_pBitmap )
			m_pBitmap->Draw(dc);

		return VFALSE;
	}

	/** Override verifies bitmap clean up before class goes out
	of scope (when window is destroyed).*/
	virtual void	PostDestroy()
		{ DeleteBitmapObject(); }

	/** Embedded Members.*/
	VBitmap*		m_pBitmap;
};

#endif /* VSPLASHWINDOW*/
