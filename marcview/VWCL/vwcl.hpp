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

#ifndef VWCLHPP
#define VWCLHPP

/* This header file should NOT be considered a true part of VWCL. It is
really only an aid to development. When creating new projects that are
either based in VWCL, or use VWCL, it may be easier to include only this
file in your project. It will include EVERYTHING VWCL has to offer, and
may result in your application being bloated, or requiring you to link
with libraries that are not really needed. However, it will still only
include support for all VWCL classes under certain conditions. The family
header files will honor the preprocessor directives in vstandard.h, and
will, for instance, exclude things like windows and other GUI related
classes when a console application is being built. Once you determine your
applications true need of specific VWCL classes, you should remove your
reference to this header and substitute the headers you actually use.
Since VWCL classes always include headers they require, you only need to
look at your source code (not VWCL's) and include the VWCL classes you
actually use. VWCL will take care of including classes those classes need.
NOTE: This header does not include the 3rd Party files contained in
vfamily_3rd_party.hpp, since most developers do not have the header files
required to compile this. If you do, you should uncomment this include
statement.*/

#ifndef __cplusplus
	#error	Inclusion of vwcl.hpp requires a C++ compiler! \
			Only vstandard.h does not.
#endif

#include "vstandard.h"

/* Include common controls first, since VWCL classes may conditionally
compile support for them.*/
#include "vfamily_common_controls.hpp"
#include "vfamily_3rd_party.hpp"
#include "vfamily_activex.hpp"
#include "vfamily_collections.hpp"
#include "vfamily_common_dialogs.hpp"
#include "vfamily_gdi.hpp"
#include "vfamily_html.hpp"
#include "vfamily_input_output.hpp"
#include "vfamily_networking.hpp"
#include "vfamily_numbers.hpp"
#include "vfamily_parsers.hpp"
#include "vfamily_splitters.hpp"
#include "vfamily_strings.hpp"
#include "vfamily_structures.hpp"
#include "vfamily_threading.hpp"
#include "vfamily_utility.hpp"
#include "vfamily_windows.hpp"
#include "vfamily_windows_controls.hpp"
#include "vfamily_windows_nt.hpp"

#endif /* VWCLHPP*/
