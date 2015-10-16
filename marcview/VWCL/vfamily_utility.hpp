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

#ifndef VFAMILYUTILITY
#define VFAMILYUTILITY

#include "vstandard.h"

/* This header includes all VWCL classes that are utilitarian in nature.*/
#if !defined(VWCL_TARGET_CONSOLE) && !defined(VWCL_NO_WIN32_API)
	#include "utility/vcapturedmousetracker.hpp"
	#include "utility/vctl3dlibrary.hpp"
	#include "utility/vwaitcursor.hpp"
#endif

#ifndef VWCL_NO_WIN32_API
	#include "utility/vdebugtimer.hpp"
	#include "utility/vhandle.hpp"
	#include "utility/vlibrary.hpp"
	#include "utility/vrecursivedirectoryenumerator.hpp"
	#include "utility/vregistry.hpp"
	#include "utility/vshellregistrar.hpp"
	#include "utility/vsingleinstanceapprestriction.hpp"
	#include "utility/vtaskbaricon.hpp"
	#include "utility/vtimer.hpp"
	#include "utility/vversioninfo.hpp"
#endif

#endif /* VFAMILYUTILITY*/
