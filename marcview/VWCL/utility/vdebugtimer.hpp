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

#ifndef VDEBUGTIMER
#define VDEBUGTIMER

#include "../vstandard.h"
#include "../strings/vstring.hpp"
#include "vtimer.hpp"

/** VDebugTimer shows a message box, or writes output to VODS() for console
apps, displaying elapsed time when destructed. The macro
VDEBUG_ELAPSED_TIME is used to instantiate an object of this type. In
release builds the macro compiles to nothing. This is useful for timing
function calls and code tuning. When using this macro, do not place a
semicolon after the macro name. This would leave a bogus extra semicolon in
release builds, causing some compilers to have problems.*/
class VDebugTimer : public VTimer
{
public:
	/** Destructor shows elapsed time using VODS() or MessageBox(),
	depending on the target application.*/
	~VDebugTimer()
	{
		VUINT nMS;

		VString s(VTEXT("Elapsed Time is: "));
		s += GetElapsedTime(&nMS);
		s += " second(s) and ";
		s += nMS;
		s += VTEXT(" Milliseconds.");

		#ifdef VWCL_CONSOLE_APP
			VODS(s.String())
		#else
			#ifndef VWCL_NO_WIN32_API
				MessageBox(	GetActiveWindow(),
							s,
							VTEXT("VDebugTimer"),
							MB_ICONINFORMATION);
			#else
				VODS(s.String())
			#endif
		#endif
	}
};

/** VDEBUG_ELAPSED_TIME is a debug build only macro that creates a
VDebugTimer object which shows a messagebox, or writes output to VODS() for
console apps, displaying elapsed time when destructed. In release builds the
macro compiles to nothing. This is useful for timing function calls and
code tuning. When using this macro, do not place a semicolon after the
macro name. This would leave a bogus extra semicolon in release builds,
causing some compilers to have problems.*/
#ifdef VWCL_DEBUG
	#define VDEBUG_ELAPSED_TIME VDebugTimer vwcl_DEBUG_TIMER;
#else
	#define VDEBUG_ELAPSED_TIME
#endif

#endif /* VDEBUGTIMER*/
