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

#ifndef VSPLITMAINWINDOW
#define VSPLITMAINWINDOW

#include "../vstandard.h"
#include "../gdi/vdc.hpp"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vmainwindow.hpp"
#include "vsplitsupport.hpp"

/** VSplitMainWindow is a main application window with split window support.
If derives its functionality from both VMainWindow and VSplitSupport, and
implements the VSplitSupport macro's to route messages to this class. See
VMainWindow and VSplitSupport documentation for more information.*/
class VSplitMainWindow : public VMainWindow, public VSplitSupport
{
public:
	/** Default constructor initializes RTTI information and
	split support options in base classes.*/
	VSplitMainWindow(	VUINT		nSplitSupportOptions =
									VSplitSupport::OPTION_DEFAULT,
						VRTTI_TYPE	nRTTI =
									VWCL_RTTI_SPLIT_MAIN_WINDOW)
		: VMainWindow(nRTTI), VSplitSupport(nSplitSupportOptions)
		{;}

protected:
	/** Macros implement message mapping to VSplitSupport base class.*/
	VWCL_SPLIT_ON_PAINT(VMainWindow)
	VWCL_SPLIT_WINDOW_PROC(VMainWindow)

	/** VMainWindow override sets cleint rectangle in VSplitSupport to r
	and calls VSplitSupport::RepositionChildren() to re-arrange the
	position of attached windows.*/
	virtual void	RecalcLayout(VRect rect)
	{
		SetClientRectangle(VRECT_BREAK(rect));
		RepositionChildren(VREF_THIS);
		VMainWindow::RecalcLayout(rect);
	}

	/** VMainWindow override moves split position to offset for showing
	or hiding control bars.*/
	virtual void	ShowControlBar(	VUINT nControlBar = IDW_TOOLBAR,
									VBOOL bShow = VTRUE)
	{
		/* This code is only needed when doing horizontal splits, and
		then only when visibile.*/
		if ( IsSplitterHorizontal() && IsVisible() )
		{
			VWindow const*	pControlBar =		GetControlBar(nControlBar);
			VINT			nControlHeight =	0;

			if ( pControlBar )
			{
				VRect r;
				pControlBar->GetRect(r);
				nControlHeight = r.GetHeight();
			}

			/* Determine if we need to offset the split position.*/
			if ( nControlHeight && bShow )
			{
				if ( nControlBar == IDW_TOOLBAR )
				{
					if ( GetSplitPosition() <= nControlHeight )
						GetSplitPosition() += nControlHeight;
				}
				else
				{
					VRect rClient, rSplitter;
					((VWindow*)this)->GetClientRect(rClient);
					GetSplitterRect(rSplitter);

					if (	rSplitter.GetBottom() >=
							rClient.GetBottom() - nControlHeight )
						GetSplitPosition() -= nControlHeight;
				}
			}
		}

		/* Let base class do the showing or hiding and window update.*/
		VMainWindow::ShowControlBar(nControlBar, bShow);
	}
};

#endif /* VSPLITMAINWINDOW*/
