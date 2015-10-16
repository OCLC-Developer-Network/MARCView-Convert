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

#ifndef VTIMER
#define VTIMER

#include "../vstandard.h"

/** VTimer is a simple object for keeping track of elapsed time.*/
class VTimer
{
public:
	/** Default constructor initializes the starting time.*/
	VTimer()
		{ Reset(); }

	/** Virtual destructor does nothing.*/
	virtual ~VTimer()
		{;}

	/** Return the elapsed time since object construction or last call to
	Reset(). The return value is number of seconds elapsed. If milliseconds
	are also desired, pass a non-NULL value for pnMilliseconds.*/
	VUINT	GetElapsedTime(VUINT* pnMilliseconds = NULL) const
	{
		struct timeb timeNow;
		ftime(&timeNow);

		/* Does caller want milliseconds?*/
		if ( pnMilliseconds )
		{
			/* Add 1000 if now ms is less than original.*/
			if ( timeNow.millitm < m_timeStart.millitm )
				timeNow.millitm += 1000;

			*pnMilliseconds = timeNow.millitm - m_timeStart.millitm;
		}

		return timeNow.time - m_timeStart.time;
	}

	/** Reset start time to current time.*/
	void	Reset()
		{ ftime(&m_timeStart); }

protected:
	/** Embedded Members.*/
	struct timeb	m_timeStart;
};

#endif /* VTIMER*/
