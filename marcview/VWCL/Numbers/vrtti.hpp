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

#ifndef VRTTIHPP
#define VRTTIHPP

#include "../vstandard.h"

typedef VUINT VRTTI_TYPE;

/** VRTTI provides a very fast, compiler independant, trivial implementation
of Runtime Type Information. Objects normally derive this functionality,
which is nothing more than a numeric value assigned to derived classes.*/
class VRTTI
{
public:
	/** Default constructor can initialize the RTTI value. Defaults to 0.*/
	VRTTI(VRTTI_TYPE nRTTI = 0)
		{ m_nRTTI = nRTTI; }

	/** Virtual destructor does nothing.*/
	virtual ~VRTTI()
		{;}

	/** Return a reference to the runtime type value.*/
	VRTTI_TYPE&	GetRTTI() const
		{ return (VRTTI_TYPE&)m_nRTTI; }

protected:
	VRTTI_TYPE	m_nRTTI;
};

#endif /* VRTTIHPP*/
