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

#ifndef VFAMILYGDI
#define VFAMILYGDI

#include "vstandard.h"

/* This header includes all VWCL classes that implement some type interaction
with the Windows Graphics Device Interface (GDI).*/
#if !defined(VWCL_TARGET_CONSOLE) && !defined(VWCL_NO_WIN32_API)
	#include "gdi/v3drect.hpp"
	#include "gdi/vanimatedbitmap.hpp"
	#include "gdi/vbitmap.hpp"
	#include "gdi/vbrush.hpp"
	#include "gdi/vclientdc.hpp"
	#include "gdi/vdc.hpp"
	#include "gdi/vdcwithwindow.hpp"
	#include "gdi/vfont.hpp"
	#include "gdi/vgauge.hpp"
	#include "gdi/vgdiobject.hpp"
	#include "gdi/vgdiobjectselector.hpp"
	#include "gdi/vgdistockobject.hpp"
	#include "gdi/vlinetracker.hpp"
	#include "gdi/vpaintdc.hpp"
	#include "gdi/vpalette.hpp"
	#include "gdi/vpen.hpp"
	#include "gdi/vrecttracker.hpp"
	#include "gdi/vstockbrush.hpp"
	#include "gdi/vstockfont.hpp"
	#include "gdi/vstockpalette.hpp"
	#include "gdi/vstockpen.hpp"
	#include "gdi/vtempdc.hpp"
	#include "gdi/vtempgdiobject.hpp"
	#include "gdi/vwindowdc.hpp"
#endif

#endif /* VFAMILYGDI*/
