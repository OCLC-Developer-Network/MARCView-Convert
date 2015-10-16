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

#include "../vstandard.h"

#define HIMETRIC_PER_INCH	2540	/* Number of HIMETRIC units per inch.*/
#define MAP_PIX_TO_LOGHIM(x,ppli)   MulDiv(HIMETRIC_PER_INCH, (x), (ppli))
#define MAP_LOGHIM_TO_PIX(x,ppli)   MulDiv((ppli), (x), HIMETRIC_PER_INCH)

void VMapCoords(HDC hDC, SIZEL* pSizeL, VBOOL bToPixels)
{
	/* Get the DC as needed.*/
	HDC		hThisDC =	(hDC) ? (HDC)hDC :	GetDC(NULL);
	VBOOL	bSysDC =	(hDC) ? VTRUE :		VFALSE;

	VASSERT(hThisDC && pSizeL)

	if ( hThisDC )
	{
		VINT nLogX = GetDeviceCaps(hThisDC, LOGPIXELSX);
		VINT nLogY = GetDeviceCaps(hThisDC, LOGPIXELSY);

		if ( bToPixels )
		{
			pSizeL->cx = MAP_LOGHIM_TO_PIX(pSizeL->cx, nLogX);
			pSizeL->cy = MAP_LOGHIM_TO_PIX(pSizeL->cy, nLogY);
		}
		else
		{
			pSizeL->cx = MAP_PIX_TO_LOGHIM(pSizeL->cx, nLogX);
			pSizeL->cy = MAP_PIX_TO_LOGHIM(pSizeL->cy, nLogY);
		}

		/* Did we get the DC? If yes, release it.*/
		if ( bSysDC )
			ReleaseDC(NULL, hThisDC);
	}
}
