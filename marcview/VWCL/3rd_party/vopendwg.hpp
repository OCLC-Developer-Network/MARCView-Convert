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

#ifndef VOPENDWG
#define VOPENDWG

#include "../vstandard.h"
#include "../collections/vlinkedlistitem.hpp"
#include "../collections/vlinkedlistmanager.hpp"
#include "../collections/vunsortedstringlinkedlist.hpp"
#include "../numbers/vbitmask.hpp"
#include "../parsers/vdelimitedstring.hpp"
#include "../strings/vstring.hpp"

/* Include these if doing a Win32 GUI application.*/
#ifndef VWCL_NO_WIN32_API
	#ifndef VWCL_CONSOLE_APP
		#include "../gdi/vbrush.hpp"
		#include "../gdi/vdc.hpp"
		#include "../gdi/vpen.hpp"
		#include "../gdi/vtempdc.hpp"
		#include "../gdi/vrecttracker.hpp"
		#include "../gdi/vlinetracker.hpp"
		#include "../numbers/vbitmask.hpp"
		#include "../structures/vpoint.hpp"
		#include "../structures/vrect.hpp"
		#include "../utility/vwaitcursor.hpp"
		#ifndef VWCL_NO_VWINDOW
			#include "../windows/vwindow.hpp"
		#endif
	#endif
#endif

/* Include OpenDWG Toolkit and ViewKit.*/
#include <io.h>
#include <ad2.h>
#include <ad3.h>

/* Macro to return VOpenDWG* from user data.*/
#define VOPENDWG_PTR(pUserData) ((VOpenDWG*)pUserData)

/* Macro to determine if a string passed from OpenDWG to our
callback is valid.*/
#define VOPENDWG_STRING_CHECK(psz)	psz && *psz != 0

/** Macro to initialize static members of VOpenDWG. This must be used once
at application scope to give linkage.*/
#define	VOPENDWG_INIT_STATICS \
	VUnSortedStringLinkedList VOpenDWGSettings::m_listFontPaths;	\
	HGLOBAL VOpenDWG::m_hInitFileMem =					NULL;		\
	VSTRING VOpenDWG::m_pszInitFile =					NULL;		\
	VSTRING VOpenDWG::m_pszInitFileOffSet =				NULL;		\
	VBOOL VOpenDWG::m_bInitialized =					VFALSE;		\
	VBOOL VOpenDWG::m_bCriticalErrorHandlerInstalled =	VFALSE;		\
	VOpenDWG* VOpenDWG::m_pThis =						NULL;

/** Function prototype for user installed error function.*/
typedef short (*VOPENDWG_CRITICAL_ERROR_HANDLER)(VSHORT nErrorNumber);

/* Local constants.*/
static VSTRING_CONST VOPENDWG_ADINIT_DAT = VTEXT("ADINIT_DAT");

/* Forward Declaration(s).*/
class VOpenDWG;

/* Default critical error handler.*/
static VSHORT criterrhandler(VSHORT nErrorNumber)
{
	exit(1);
	return 1;
}

/* This array maps AutoCAD Color Index values to their approximate RGB
COLORREF values. Because of how AutoCAD handles some colors, these may not
be exact. A simple AutoCAD ARX application is used to generate these values,
so they should be perfect, but they are not. It is my guess that AutoCAD is
doing something else with its palette management that causes slight
differences in colors between RGB and its own color mapping. This seems to
mostly affect dark, uncommon colors. The code snippit below is the relevant
code from the ARX application that generates this array:
#include <stdlib.h>
#include <stdio.h>
#define GetRValue(rgb)      ((BYTE)(rgb))
#define GetGValue(rgb)      ((BYTE)(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      ((BYTE)((rgb)>>16))

FILE* pFile = fopen("t:\\scratch\\cr.txt", "wt");
char sz[32];

for ( int i = 0; i <= 255; i++ )
{
	unsigned long cr = acdbGetRGB(i);

	int r = GetRValue(cr);
	int g = GetGValue(cr);
	int b = GetBValue(cr);

	sprintf(	sz,
				"RGB(
				%-3d,
				%-3d,
				%-3d),
				%d\n",
				r,
				g,
				b,
				i);
	fputs(sz, pFile);
}
fclose(pFile);*/

static COLORREF VOPENDWG_COLOR_MAP[] =
{
	RGB(0  ,0  ,0  ), /* 0.*/
	RGB(255,0  ,0  ), /* 1.*/
	RGB(255,255,0  ), /* 2.*/
	RGB(0  ,255,0  ), /* 3.*/
	RGB(0  ,255,255), /* 4.*/
	RGB(0  ,0  ,255), /* 5.*/
	RGB(255,0  ,255), /* 6.*/
	RGB(255,255,255), /* 7.*/
	RGB(128,128,128), /* 8.*/
	RGB(192,192,192), /* 9.*/
	RGB(255,0  ,0  ), /* 10.*/
	RGB(255,127,127), /* 11.*/
	RGB(204,0  ,0  ), /* 12.*/
	RGB(204,102,102), /* 13.*/
	RGB(153,0  ,0  ), /* 14.*/
	RGB(153,76 ,76 ), /* 15.*/
	RGB(127,0  ,0  ), /* 16.*/
	RGB(127,63 ,63 ), /* 17.*/
	RGB(76 ,0  ,0  ), /* 18.*/
	RGB(76 ,38 ,38 ), /* 19.*/
	RGB(255,63 ,0  ), /* 20.*/
	RGB(255,159,127), /* 21.*/
	RGB(204,51 ,0  ), /* 22.*/
	RGB(204,127,102), /* 23.*/
	RGB(153,38 ,0  ), /* 24.*/
	RGB(153,95 ,76 ), /* 25.*/
	RGB(127,31 ,0  ), /* 26.*/
	RGB(127,79 ,63 ), /* 27.*/
	RGB(76 ,19 ,0  ), /* 28.*/
	RGB(76 ,47 ,38 ), /* 29.*/
	RGB(255,127,0  ), /* 30.*/
	RGB(255,191,127), /* 31.*/
	RGB(204,102,0  ), /* 32.*/
	RGB(204,153,102), /* 33.*/
	RGB(153,76 ,0  ), /* 34.*/
	RGB(153,114,76 ), /* 35.*/
	RGB(127,63 ,0  ), /* 36.*/
	RGB(127,95 ,63 ), /* 37.*/
	RGB(76 ,38 ,0  ), /* 38.*/
	RGB(76 ,57 ,38 ), /* 39.*/
	RGB(255,191,0  ), /* 40.*/
	RGB(255,223,127), /* 41.*/
	RGB(204,153,0  ), /* 42.*/
	RGB(204,178,102), /* 43.*/
	RGB(153,114,0  ), /* 44.*/
	RGB(153,133,76 ), /* 45.*/
	RGB(127,95 ,0  ), /* 46.*/
	RGB(127,111,63 ), /* 47.*/
	RGB(76 ,57 ,0  ), /* 48.*/
	RGB(76 ,66 ,38 ), /* 49.*/
	RGB(255,255,0  ), /* 50.*/
	RGB(255,255,127), /* 51.*/
	RGB(204,204,0  ), /* 52.*/
	RGB(204,204,102), /* 53.*/
	RGB(153,153,0  ), /* 54.*/
	RGB(153,153,76 ), /* 55.*/
	RGB(127,127,0  ), /* 56.*/
	RGB(127,127,63 ), /* 57.*/
	RGB(76 ,76 ,0  ), /* 58.*/
	RGB(76 ,76 ,38 ), /* 59.*/
	RGB(191,255,0  ), /* 60.*/
	RGB(223,255,127), /* 61.*/
	RGB(153,204,0  ), /* 62.*/
	RGB(178,204,102), /* 63.*/
	RGB(114,153,0  ), /* 64.*/
	RGB(133,153,76 ), /* 65.*/
	RGB(95 ,127,0  ), /* 66.*/
	RGB(111,127,63 ), /* 67.*/
	RGB(57 ,76 ,0  ), /* 68.*/
	RGB(66 ,76 ,38 ), /* 69.*/
	RGB(127,255,0  ), /* 70.*/
	RGB(191,255,127), /* 71.*/
	RGB(102,204,0  ), /* 72.*/
	RGB(153,204,102), /* 73.*/
	RGB(76 ,153,0  ), /* 74.*/
	RGB(114,153,76 ), /* 75.*/
	RGB(63 ,127,0  ), /* 76.*/
	RGB(95 ,127,63 ), /* 77.*/
	RGB(38 ,76 ,0  ), /* 78.*/
	RGB(57 ,76 ,38 ), /* 79.*/
	RGB(63 ,255,0  ), /* 80.*/
	RGB(159,255,127), /* 81.*/
	RGB(51 ,204,0  ), /* 82.*/
	RGB(127,204,102), /* 83.*/
	RGB(38 ,153,0  ), /* 84.*/
	RGB(95 ,153,76 ), /* 85.*/
	RGB(31 ,127,0  ), /* 86.*/
	RGB(79 ,127,63 ), /* 87.*/
	RGB(19 ,76 ,0  ), /* 88.*/
	RGB(47 ,76 ,38 ), /* 89.*/
	RGB(0  ,255,0  ), /* 90.*/
	RGB(127,255,127), /* 91.*/
	RGB(0  ,204,0  ), /* 92.*/
	RGB(102,204,102), /* 93.*/
	RGB(0  ,153,0  ), /* 94.*/
	RGB(76 ,153,76 ), /* 95.*/
	RGB(0  ,127,0  ), /* 96.*/
	RGB(63 ,127,63 ), /* 97.*/
	RGB(0  ,76 ,0  ), /* 98.*/
	RGB(38 ,76 ,38 ), /* 99.*/
	RGB(0  ,255,63 ), /* 100.*/
	RGB(127,255,159), /* 101.*/
	RGB(0  ,204,51 ), /* 102.*/
	RGB(102,204,127), /* 103.*/
	RGB(0  ,153,38 ), /* 104.*/
	RGB(76 ,153,95 ), /* 105.*/
	RGB(0  ,127,31 ), /* 106.*/
	RGB(63 ,127,79 ), /* 107.*/
	RGB(0  ,76 ,19 ), /* 108.*/
	RGB(38 ,76 ,47 ), /* 109.*/
	RGB(0  ,255,127), /* 110.*/
	RGB(127,255,191), /* 111.*/
	RGB(0  ,204,102), /* 112.*/
	RGB(102,204,153), /* 113.*/
	RGB(0  ,153,76 ), /* 114.*/
	RGB(76 ,153,114), /* 115.*/
	RGB(0  ,127,63 ), /* 116.*/
	RGB(63 ,127,95 ), /* 117.*/
	RGB(0  ,76 ,38 ), /* 118.*/
	RGB(38 ,76 ,57 ), /* 119.*/
	RGB(0  ,255,191), /* 120.*/
	RGB(127,255,223), /* 121.*/
	RGB(0  ,204,153), /* 122.*/
	RGB(102,204,178), /* 123.*/
	RGB(0  ,153,114), /* 124.*/
	RGB(76 ,153,133), /* 125.*/
	RGB(0  ,127,95 ), /* 126.*/
	RGB(63 ,127,111), /* 127.*/
	RGB(0  ,76 ,57 ), /* 128.*/
	RGB(38 ,76 ,66 ), /* 129.*/
	RGB(0  ,255,255), /* 130.*/
	RGB(127,255,255), /* 131.*/
	RGB(0  ,204,204), /* 132.*/
	RGB(102,204,204), /* 133.*/
	RGB(0  ,153,153), /* 134.*/
	RGB(76 ,153,153), /* 135.*/
	RGB(0  ,127,127), /* 136.*/
	RGB(63 ,127,127), /* 137.*/
	RGB(0  ,76 ,76 ), /* 138.*/
	RGB(38 ,76 ,76 ), /* 139.*/
	RGB(0  ,191,255), /* 140.*/
	RGB(127,223,255), /* 141.*/
	RGB(0  ,153,204), /* 142.*/
	RGB(102,178,204), /* 143.*/
	RGB(0  ,114,153), /* 144.*/
	RGB(76 ,133,153), /* 145.*/
	RGB(0  ,95 ,127), /* 146.*/
	RGB(63 ,111,127), /* 147.*/
	RGB(0  ,57 ,76 ), /* 148.*/
	RGB(38 ,66 ,76 ), /* 149.*/
	RGB(0  ,127,255), /* 150.*/
	RGB(127,191,255), /* 151.*/
	RGB(0  ,102,204), /* 152.*/
	RGB(102,153,204), /* 153.*/
	RGB(0  ,76 ,153), /* 154.*/
	RGB(76 ,114,153), /* 155.*/
	RGB(0  ,63 ,127), /* 156.*/
	RGB(63 ,95 ,127), /* 157.*/
	RGB(0  ,38 ,76 ), /* 158.*/
	RGB(38 ,57 ,76 ), /* 159.*/
	RGB(0  ,63 ,255), /* 160.*/
	RGB(127,159,255), /* 161.*/
	RGB(0  ,51 ,204), /* 162.*/
	RGB(102,127,204), /* 163.*/
	RGB(0  ,38 ,153), /* 164.*/
	RGB(76 ,95 ,153), /* 165.*/
	RGB(0  ,31 ,127), /* 166.*/
	RGB(63 ,79 ,127), /* 167.*/
	RGB(0  ,19 ,76 ), /* 168.*/
	RGB(38 ,47 ,76 ), /* 169.*/
	RGB(0  ,0  ,255), /* 170.*/
	RGB(127,127,255), /* 171.*/
	RGB(0  ,0  ,204), /* 172.*/
	RGB(102,102,204), /* 173.*/
	RGB(0  ,0  ,153), /* 174.*/
	RGB(76 ,76 ,153), /* 175.*/
	RGB(0  ,0  ,127), /* 176.*/
	RGB(63 ,63 ,127), /* 177.*/
	RGB(0  ,0  ,76 ), /* 178.*/
	RGB(38 ,38 ,76 ), /* 179.*/
	RGB(63 ,0  ,255), /* 180.*/
	RGB(159,127,255), /* 181.*/
	RGB(51 ,0  ,204), /* 182.*/
	RGB(127,102,204), /* 183.*/
	RGB(38 ,0  ,153), /* 184.*/
	RGB(95 ,76 ,153), /* 185.*/
	RGB(31 ,0  ,127), /* 186.*/
	RGB(79 ,63 ,127), /* 187.*/
	RGB(19 ,0  ,76 ), /* 188.*/
	RGB(47 ,38 ,76 ), /* 189.*/
	RGB(127,0  ,255), /* 190.*/
	RGB(191,127,255), /* 191.*/
	RGB(102,0  ,204), /* 192.*/
	RGB(153,102,204), /* 193.*/
	RGB(76 ,0  ,153), /* 194.*/
	RGB(114,76 ,153), /* 195.*/
	RGB(63 ,0  ,127), /* 196.*/
	RGB(95 ,63 ,127), /* 197.*/
	RGB(38 ,0  ,76 ), /* 198.*/
	RGB(57 ,38 ,76 ), /* 199.*/
	RGB(191,0  ,255), /* 200.*/
	RGB(223,127,255), /* 201.*/
	RGB(153,0  ,204), /* 202.*/
	RGB(178,102,204), /* 203.*/
	RGB(114,0  ,153), /* 204.*/
	RGB(133,76 ,153), /* 205.*/
	RGB(95 ,0  ,127), /* 206.*/
	RGB(111,63 ,127), /* 207.*/
	RGB(57 ,0  ,76 ), /* 208.*/
	RGB(66 ,38 ,76 ), /* 209.*/
	RGB(255,0  ,255), /* 210.*/
	RGB(255,127,255), /* 211.*/
	RGB(204,0  ,204), /* 212.*/
	RGB(204,102,204), /* 213.*/
	RGB(153,0  ,153), /* 214.*/
	RGB(153,76 ,153), /* 215.*/
	RGB(127,0  ,127), /* 216.*/
	RGB(127,63 ,127), /* 217.*/
	RGB(76 ,0  ,76 ), /* 218.*/
	RGB(76 ,38 ,76 ), /* 219.*/
	RGB(255,0  ,191), /* 220.*/
	RGB(255,127,223), /* 221.*/
	RGB(204,0  ,153), /* 222.*/
	RGB(204,102,178), /* 223.*/
	RGB(153,0  ,114), /* 224.*/
	RGB(153,76 ,133), /* 225.*/
	RGB(127,0  ,95 ), /* 226.*/
	RGB(127,63 ,111), /* 227.*/
	RGB(76 ,0  ,57 ), /* 228.*/
	RGB(76 ,38 ,66 ), /* 229.*/
	RGB(255,0  ,127), /* 230.*/
	RGB(255,127,191), /* 231.*/
	RGB(204,0  ,102), /* 232.*/
	RGB(204,102,153), /* 233.*/
	RGB(153,0  ,76 ), /* 234.*/
	RGB(153,76 ,114), /* 235.*/
	RGB(127,0  ,63 ), /* 236.*/
	RGB(127,63 ,95 ), /* 237.*/
	RGB(76 ,0  ,38 ), /* 238.*/
	RGB(76 ,38 ,57 ), /* 239.*/
	RGB(255,0  ,63 ), /* 240.*/
	RGB(255,127,159), /* 241.*/
	RGB(204,0  ,51 ), /* 242.*/
	RGB(204,102,127), /* 243.*/
	RGB(153,0  ,38 ), /* 244.*/
	RGB(153,76 ,95 ), /* 245.*/
	RGB(127,0  ,31 ), /* 246.*/
	RGB(127,63 ,79 ), /* 247.*/
	RGB(76 ,0  ,19 ), /* 248.*/
	RGB(76 ,38 ,47 ), /* 249.*/
	RGB(51 ,51 ,51 ), /* 250.*/
	RGB(91 ,91 ,91 ), /* 251.*/
	RGB(132,132,132), /* 252.*/
	RGB(173,173,173), /* 253.*/
	RGB(214,214,214), /* 254.*/
	RGB(255,255,255), /* 255.*/
};

/** VOpenDWGLayer provides access to a single layer within a drawing.
Objects of this type can only be created by the VOpenDWG class, so please
see the LayerXXX() methods of that class.*/
class VOpenDWGLayer : public VLinkedListItem
{
friend class VOpenDWG;

public:
	/** Return the color for entities on this layer.*/
	VUSHORT			Color() const
		{ return m_nRealLayerColor; }

	/** Set the color for entities on this layer.*/
	void			Color(VUSHORT nColor)
	{
		/* Set into structure first.*/
		m_Layer.color = nColor;

		/* Save new real color.*/
		m_nRealLayerColor = 0;
	}

	/** Determine if this layer is frozen.*/
	VBOOL			Frozen() const
		{ return (m_Layer.flag & AD_LAYER_FROZEN) ? VTRUE : VFALSE; }

	/** Set this layer to be frozen or not.*/
	void			Frozen(VBOOL bFrozen)
		{ SetLayerFlagBit(AD_LAYER_FROZEN, bFrozen); }

	/** Determine if this layer is frozen in new viewports.*/
	VBOOL			FrozenViewPort() const
		{ return (m_Layer.flag & AD_LAYER_VPFROZEN) ? VTRUE : VFALSE; }

	/** Set this layer to be frozen or not in new viewports.*/
	void			FrozenViewPort(VBOOL bFrozen)
		{ SetLayerFlagBit(AD_LAYER_VPFROZEN, bFrozen); }

	/** Get a reference to the extended entity data, if any, attached to this
	layer. It is equal to AD_VMNULL if there is none.*/
	AD_VMADDR&		GetExtendedEntity() const
		{ return (AD_VMADDR&)m_Layer.xdblob; }

	/** Get a reference to the object handle for this layer.*/
	AD_OBJHANDLE&	GetHandle() const
		{ return (AD_OBJHANDLE&)m_Layer.objhandle; }

	/** Get a reference to the internal AD_LAY structure that we wrap.*/
	AD_LAY&			GetLayer() const
		{ return (AD_LAY&)m_Layer; }

	/** Get a reference to the object handle for this layer's linetype.*/
	AD_OBJHANDLE&	GetLinetypeObjectHandle() const
		{ return (AD_OBJHANDLE&)m_Layer.linetypeobjhandle; }

	/** Return a reference to the blob containing the reactors, if any.*/
	AD_VMADDR&		GetReactorBlob() const
		{ return (AD_VMADDR&)m_Layer.reactorblob; }

	/** Return the number of persistent reactors attached to this layer.*/
	VLONG			GetReactorCount() const
		{ return m_Layer.numreactors; }

	/** Return the XREF index which indicates the inex of the XREF in the
	blockheader list from which this layer comes. If this layer if from the
	current drawing (not an XREF), this value will be -1.*/
	VSHORT			GetXREFIndex() const
		{ return m_Layer.xrefindex; }

	/** Determine if this layer has been purged from the database.*/
	VBOOL			IsPurged() const
		{ return (m_Layer.purgedflag) ? VTRUE : VFALSE; }

	/** Determine if this layer is from an XREF in an external drawing.*/
	VBOOL			IsXREF() const
		{ return (GetXREFIndex() == -1) ? VFALSE : VTRUE; }

	/** Determine if this layer is locked.*/
	VBOOL			Locked() const
		{ return (m_Layer.flag & AD_LAYER_LOCKED) ? VTRUE : VFALSE; }

	/** Set this layer to be locked or not.*/
	void			Locked(VBOOL bLocked)
		{ SetLayerFlagBit(AD_LAYER_LOCKED, bLocked); }

	/** Return the string name of this layer.*/
	VSTRING_CONST	Name() const
		{ return m_Layer.name; }

	/** Set the string name of this layer. The name should not be more than 32
	characters in length if the target database is not AutoCAD 2000 or higher,
	and cannot be NULL. AutoCAD 2000 can support layer names up to 512
	characters, but will truncate the name if the database is saved in an
	earier format.*/
	void			Name(VSTRING_CONST pszName)
	{
		/* Null and 0 length is not allowed.*/
		VASSERT(VSTRLEN_CHECK(pszName))

		/* Must be less than VARRAY_SIZE(m_Layer.name) - 1 characters.*/
		VASSERT(VSTRLEN_CHECK(pszName) < VARRAY_SIZE(m_Layer.name) - 1)

		/* Copy string.*/
		VSTRNCPY(m_Layer.name, pszName, VARRAY_SIZE(m_Layer.name) - 1);
	}

	/** Objects of this type are members of a linked list. Call this function
	to get the next layer in the list, or NULL if the end of the layer list
	is found.*/
	VOpenDWGLayer*	Next() const
		{ return (VOpenDWGLayer*)m_pNextItem; }

	/** Determine if this layer is turned on.*/
	VBOOL			On() const
		{ return (m_Layer.color > -1) ? VTRUE : VFALSE; }

	/** Turn this layer on or off.*/
	void			On(VBOOL bOn)
		{ m_Layer.color = (bOn) ? m_nRealLayerColor : -1; }

	/** Save this layers information to the associated database. This is
	required after changing variables or other class information. This makes
	the changes to the layer permanent in the drawing database in memory only.
	Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			Save()
	{
		VASSERT(m_dbHandle)
		return (adReplaceLayer(m_dbHandle, &m_Layer)) ? VTRUE : VFALSE;
	}

private:
	/** Constructor must be created only by VOpenDWG classes, which passes a
	database handle and an initialized AD_LAY structure.*/
	VOpenDWGLayer(	AD_DB_HANDLE	dbHandle,
					AD_LAY&			layer)
	{
		VASSERT(dbHandle)

		/* Initialize members.*/
		m_dbHandle =		dbHandle;
		memcpy(&m_Layer, &layer, sizeof(m_Layer));

		/* Real color must be positive.*/
		if ( layer.color < 0 )
			m_nRealLayerColor =	layer.color * -1;
		else
			m_nRealLayerColor =	layer.color;
	}

	/** Destructor verifies that it is not possible for objects to be deleted
	by client code.*/
	~VOpenDWGLayer()
		{;}

	/** Internal function to set or unset a bit in the layer flag.*/
	void			SetLayerFlagBit(VCHAR nBit,
									VBOOL bSet)
	{
		if ( bSet )
			m_Layer.flag |= nBit;
		else
			m_Layer.flag &= ~nBit;
	}

	/** Embedded Members.*/
	AD_DB_HANDLE	m_dbHandle;
	AD_LAY			m_Layer;
	VUSHORT			m_nRealLayerColor;
};

/** VOpenDWGSettings provides access to variables that work on a global scope,
and not tied to a specific instance of a VOpenDWG or derived class object.
Since all members of this class are static, there is no need to get or create
an object of this type. Simply call the methods using the
VOpenDWGSettings::function_name syntax.*/
class VOpenDWGSettings
{
public:
	/** Get a the accuracy level. This determines, during the drawing process,
	the number of vectors generated for arcs. The default value of 1.0 is
	generally acceptable. Increasing this number will increase the number of
	vectors used, and decreasing it will do the inverse. A reasonble range of
	values is greater than 0 and less than 20.*/
	static VDOUBLE	Accuracy()
		{ return ad3parm.accuracy; }

	/** Set a the accuracy level. This determines, during the drawing process,
	the number of vectors generated for arcs. The default value of 1.0 is
	generally acceptable. Increasing this number will increase the number of
	vectors used, and decreasing it will do the inverse. A reasonble range of
	values is greater than 0 and less than 20.*/
	static void		Accuracy(VDOUBLE nAccuracy)
		{ ad3parm.accuracy = (nAccuracy > 0) ? nAccuracy : 1.0; }

	/** Add a font search path that will be used to locate fonts as needed. The
	font file name will be appended to this path when OpenDWG asks for a font
	to use. This is only valid for AutoCAD SHX type fonts, not TrueType, fonts
	which OpenDWG does not currently support. If the existing paths should be
	deleted first, pass VTRUE for bDeleteCurrent. It is valid to call this
	function with pszPath as NULL only when bDeleteCurrent is VFALSE.*/
	static VBOOL	AddFontPath(	VSTRING_CONST	pszPath,
									VBOOL			bDeleteCurrent = VFALSE)
	{
		/* Delete current entries?*/
		if ( bDeleteCurrent )
			m_listFontPaths.Free();

		/* Only deleting?*/
		if ( !pszPath && bDeleteCurrent )
			return VFALSE;

		VASSERT(VSTRLEN_CHECK(pszPath))

		VLinkedListStringItem* pstrNew = VNEW VLinkedListStringItem(pszPath);

		/* Valid?*/
		if ( pstrNew && pstrNew->IsNotEmpty() )
		{
			/* Make sure the string ends with a final VFILE_PATH_SEP_CHAR.*/
			pstrNew->EndWith(VFILE_PATH_SEP_CHAR);

			/* This will fail if the path is already in the list.*/
			if ( m_listFontPaths.Add(pstrNew) )
				return VTRUE;
		}

		/* Delete string on error.*/
		VDELETE_NULL(pstrNew)

		return VFALSE;
	}

	/** Get a the aspect ratio, which defaults to 1.0 is used to shrink or
	stretch the image in the X direction. This value can be used to fit
	the drawing image within the window, and to correct circle shapes. Larger
	values of aspect ratio will shrink the image, while smaller values will
	stretch it.*/
	static VDOUBLE	AspectRatio()
		{ return ad3parm.aspectratio; }

	/** Set a the aspect ratio, which defaults to 1.0 is used to shrink or
	stretch the image in the X direction. This value can be used to fit
	the drawing image within the window, and to correct circle shapes. Larger
	values of aspect ratio will shrink the image, while smaller values will
	stretch it.*/
	static void		AspectRatio(VDOUBLE nAspectRatio)
		{ ad3parm.aspectratio = (nAspectRatio) ? nAspectRatio : 1; }

	/** Enable or disable Display List Processing. This is not allowed when the
	pixel space size is greater than 32000 x 32000. However, this function is
	global to all OpenDWG classes, so you should determine if display list
	processing is appropriate for your application as a whole.*/
	static void		EnableDisplayListProcessing(VBOOL bEnable = VTRUE)
		{ adSetDlpMode((bEnable) ? DLPON : DLPOFF); }

	/** Return a reference to the linked list manager object used to
	store font paths.*/
	static VUnSortedStringLinkedList&
					GetFontPathList()
		{ return m_listFontPaths; }

	/** Determine if linetype generation is enabled.*/
	static VBOOL	LineTypesEnabled()
		{ return (ad3parm.dolinetypes == 1) ? VTRUE : VFALSE; }

	/** Enable or disable the generation of linetypes while vectorizing.
	The default is enabled.*/
	static void		LineTypesEnabled(VBOOL bEnable)
		{ ad3parm.dolinetypes = (bEnable) ? 1 : 0; }

	/** Get the maximum number of segments used to draw a circle. If it is 0,
	which is the default, there is effectively no limit.*/
	static VUSHORT	MaxCircleSegments()
		{ return ad3parm.maxcirclesegs; }

	/** Set the maximum number of segments used to draw a circle. If it is 0,
	which is the default, there is effectively no limit.*/
	static void		MaxCircleSegments(VUSHORT nSegments)
		{ ad3parm.maxcirclesegs = nSegments; }

	/** Determine if vectorizing is allowed.*/
	static VBOOL	VectorizingEnabled()
		{ return (ad3parm.keepvectorizing == 1) ? VTRUE : VFALSE; }

	/** When inside of the VOpenDWG::Draw() function, and the virtual
	functions it calls, this method can be used to stop the vectorization
	process if bEnable is VFALSE. This process will not stop immediately,
	but will stop at the next checkpoint. Once stopped, this function must
	be called with bEnable set to VTRUE to re-enable drawing.*/
	static void		VectorizingEnabled(VBOOL bEnable)
		{ ad3parm.keepvectorizing = (bEnable) ? 1 : 0; }

	/** Get the vector function limit. This determines, during the drawing
	process, how often the virtual function VOpenDWG::OnVectorCount() will
	be called. It will be called once for each x number of vectors that this
	value holds.*/
	static VSHORT	VectorLimit()
		{ return ad3parm.vecfunclimit; }

	/** Set the vector function limit. This determines, during the drawing
	process, how often the virtual function VOpenDWG::OnVectorCount() will
	be called. It will be called once for each x number of vectors that this
	value holds.*/
	static void		VectorLimit(VSHORT nLimit)
		{ ad3parm.vecfunclimit = nLimit; }

protected:
	/** Embedded Members.*/
	static VUnSortedStringLinkedList	m_listFontPaths;
};

/** VOpenDWG is the base class for support of OpenDWG, which is a toolkit for
reading, writing, and displaying of AutoCAD and other vendors DWG and DXF CAD
drawing files. VWCL does not ship the OpenDWG ToolKit or ViewKit, but it is
available at no charge from The OpenDWG Alliance on the web at www.opendwg.org.
VWCL supports both the ToolKit and the ViewKit, but linking with the ViewKit
library is required. This header includes the header files ad2.h, ad3.h, and
odio.h, which must be downloaded separately. VWCL support for OpenDWG requires
the use of the ViewKit libraries and headers, and it is not possible to use
only the ToolKit with VWCL. The reason for this is that many functions that
should be considered required even for non-vectorizing are only implemented
in the ViewKit version of the library. This includes functions to work with
XREF's, among others. All OpenDWG headers must be found in your include file
path. OpenDWG uses many user defined callback functions, which VWCL maps into
VOpenDWG as virutal function calls. These virtual functions can be overriden
in a derived class. It is well beyond the scope of VWCL documentation to
attempt to document OpenDWG. This documentation is also downloadable from
The OpenDWG Alliance. It is also safe to say that to use these classes
effectively, you should have a good understanding of how OpenDWG works,
and CAD principles in general. VWCL provides a nice object oriented way to
work with OpenDWG, but is fairly close to the metal and not terribly abstract.
This is by design, since performance when dealing with CAD systems is usually
a top priority.

The VOpenDWG class encapsulates working with a single DWG or DXF file,
generally called a database. It supports many of the functions implemented
in the OpenDWG ToolKit and ViewKit. Because of the way OpenDWG is coded, it
does not lend itself especially well to being wrapped by C++ objects. For
instance, some callback functions are passed a user defined pointer, which
is perfect for wrapping in C++, since that void pointer can be cast to an
object. Not all functions support this, which adds a layer of complexity.
Also, since some functions are only implemented in the ViewKit, like working
with XREF drawings, we must initialize the ViewKit as well. Since most of the
ViewKit functions are passed a user defined pointer to their callback, we
cannot make a derived class to handle the vectorization, since these functions
would also be called for the base class. An attempt to cast the user defined
pointer to a derived class would fail when a derived class is not used, so the
simplest way to correct this oversight is to create a single, powerful class
that is able to work with the CAD database, and also be able to vectorize the
drawing.

The OpenDWG ViewKit is a general purpose vectorizer for CAD drawings. It is
layered on top of the OpenDWG ToolKit and provides the basic vectorization
code. It is very important to note that this class, like the ViewKit, does
not imply rendering to any device. It can be used to draw to the screen,
a plotter, or be used as an export file filter. The Draw() function simply
starts the vectorization process, which calls the virtual functions
OnLineTo(), OnMoveTo(), OnPenColor(), OnPaintPolygon() and others. It is the
responsibility of these virtual functions to do the actual drawing. A sample
of this can be seen in the class VOpenDWGWindow, which indeed does render
drawings to the screen. It is important to note that OpenDWG uses Cartesian
coordinates, where the Y axis extends from 0 at the bottom of the screen to
larger numbers upward. This is the exact opposite of how most drawing in the
Windows world works. During vectorization, VWCL will convert the Y axis to an
appropriate value so that axis starts at the top of the pixel space and
extends positively down the screen. All points and rectangles used by this
class also assume Y will increase going down the virtual screen, or pixel
space. Cartesion Coordinates can be used by setting or unsetting the option
bit OPTION_USE_CARTESIAN_COORDINATES from GetDrawingOptions().

None of the functions in this class will ever call the Draw() method directly.
You must do this yourself. In other words, calling ZoomExtents() only sets the
internal variables inside this class and inside of OpenDWG, and does not cause
a Draw() to occur. When it is time, as deemed by you, to vectorize the drawing,
you must call Draw() to begin the process.

Because this class uses several static variables and objects, and the C++
specification that most compiler vendors follow requires static to be
initialized at application scope, you must include the macro
VOPENDWG_INIT_STATICS once in your program to give these static objects
linkage. Configuration of these classes is done through the
VOpenDWGSettings class.*/
class VOpenDWG
{
public:
	/** Default constructor initializes this class to default values. The
	static Init(VTRUE) function must be called at least once per application
	run to initialize OpenDWG and VWCL support for it. This MUST be done
	before creating a VOpenDWG (or derived) object. This constructor will
	assert if this has not been done.*/
	VOpenDWG()
	{
		/* OpenDWG must already be initialized with VOpenDWG::Init().*/
		VASSERT(IsInitialized())

		/* Initialize members.*/
		m_dbHandle = NULL;
		ResetVectorState(VTRUE);

		/* Do not allow duplicates in lists.*/
		m_listBadXREFs.GetOptions().Append(
			VSortedStringLinkedList::OPTION_NO_DUPLICATES);

		m_listBadFonts.GetOptions().Append(
			VSortedStringLinkedList::OPTION_NO_DUPLICATES);
	}

	/** Virtual destructor verifies the drawing, if any, is properly closed.*/
	virtual ~VOpenDWG()
		{ CloseDrawing(VTRUE); }

	/** Determine if full extents have been calculated, usually done after
	the first call to Draw() or DrawSilent().*/
	VBOOL			AreExtentsCalculated() const
	{	VASSERT(m_dbHandle)
		return (adExtentsCalculated(m_dbHandle)) ? VTRUE : VFALSE;
	}

	/** Determine if a call to ZoomPrevious() would succeed.*/
	VBOOL			CanZoomPrevious() const
	{
		if ( IsPixelSpaceValid() && m_dbHandle )
			return m_State.IsSet(STATE_ZOOM_PREVIOUS_ALLOWED);

		return VFALSE;
	}

	/** Close the drawing, if needed. If a drawing is open, this function will
	call the OnCloseDrawing() virtual function before actually closing the
	drawing. This override can prevent the closing of the drawing. This
	function will return VTRUE if a drawing is still open (not closed),
	or VFALSE if there was no drawing to close, or if OnCloseDrawing() returns
	VFALSE. If bForceClose is VTRUE, the return value from OnCloseDrawing()
	will be ignored, and the drawing closed. It is important to note that if
	this object is a copy of another, this function will not close the drawing
	or call any of the virtual functions.*/
	VBOOL			CloseDrawing(VBOOL bForceClose = VFALSE)
	{
		/* Notify derived class. They can stop the closing.*/
		if ( m_dbHandle )
		{
			m_State.Append(STATE_CLOSE_IN_PROGRESS);

			if ( OnCloseDrawing() || bForceClose )
			{
				/* Reset the vectorizer state.*/
				ResetVectorState(VFALSE);

				/* Close the database.*/
				adSetCallbackUserdata(m_dbHandle, NULL);
				adCloseFile(m_dbHandle);

				/* NULLify handle.*/
				m_dbHandle = NULL;

				/* Free current layer objects.*/
				m_listLayers.Free();

				/* Free list of bad XREF's.*/
				m_listBadXREFs.Free();

				/* Free list of bad fonts.*/
				m_listBadFonts.Free();

				/* Free file name.*/
				m_strFileName.Empty();
			}

			/* Reset flag.*/
			m_State.Remove(STATE_CLOSE_IN_PROGRESS);
		}

		return (m_dbHandle) ? VTRUE : VFALSE;
	}

	/** Get a reference to the last (current) pen color that is being used
	for drawing.*/
	VUSHORT&		CurrentPenColor() const
		{ return (VUSHORT&)m_nCurrentPenColor; }

	/** Start the vectorization process. Returns VTRUE on success, VFALSE on
	failure. During vectorization, derived class OnXXX() function will be
	called. Derived classes are responsible for drawing however they see fit,
	and most should call the base class functions to keep this object
	syncronized. If the pixel space is invalid because SetPixelSpace() was
	not used or set the values to empty, this function will not perform the
	drawing code, since it would result in a divide by 0 error inside of
	OpenDWG.*/
	virtual VBOOL	Draw()
	{
		VASSERT(m_dbHandle)

		/* Did you call SetPixelSpace() to set the pixel area to draw into?*/
		VASSERT(IsPixelSpaceValid())

		/* Did you call VOpenDWGSettings::EnableVectorizing(VFALSE) and not
		re-enable it? It looks like it!*/
		VASSERT(VOpenDWGSettings::VectorizingEnabled())

		VBOOL bSuccess = VFALSE;

		/* Check for valid pixel space and vectorizing allowed.*/
		if (	IsPixelSpaceValid() &&
				VOpenDWGSettings::VectorizingEnabled() )
		{
			/* Set the view transformation.*/
			adMakeViewTransform(	m_dbHandle,
									&m_viewParams,
									&m_rWindow,
									&m_rDisplayList);

			/* Draw by layer?*/
			if ( m_Options.IsSet(OPTION_DRAW_BY_LAYER) )
			{
				VOpenDWGLayer* pLayer = LayerGetFirst();

				if ( pLayer )
				{
					/* We need to make a copy of the current layer state.*/
					VUINT nNumLayers = m_listLayers.GetCount();
					VASSERT(nNumLayers)

					/* We only need to determine On/Off state.*/
					VBOOL* pbState = VNEW VBOOL[nNumLayers];

					if ( pbState )
					{
						VUINT nStateIndex = 0;

						while ( pLayer )
						{
							/* Save current state.*/
							pbState[nStateIndex] = pLayer->On();

							/* Turn the layer off for now.*/
							pLayer->On(VFALSE);
							pLayer->Save();

							/* Next please.*/
							nStateIndex++;
							pLayer = pLayer->Next();
						}

						/* Reset layer list and walk layers to draw them.*/
						pLayer =		LayerGetFirst();
						nStateIndex =	0;

						while ( pLayer )
						{
							/* Should we draw this layer? Only if it was
							originally on and OnDrawLayer() returns VTRUE.*/
							if	(	(pbState[nStateIndex]) &&
									(m_State.IsSet(STATE_INSIDE_DRAW_SILENT) ||
									OnDrawLayer(VREF_PTR(pLayer), nStateIndex))
								)
							{
								pLayer->On(VTRUE);
								pLayer->Save();

								/* Draw it.*/
								ForceRegen();
								adDrawDrawing(m_dbHandle);

								/* Turn it back off.*/
								pLayer->On(VFALSE);
								pLayer->Save();

								if ( m_State.
									IsNotSet(STATE_INSIDE_DRAW_SILENT) )
									OnVectorizePercent(0);
							}

							/* Next please.*/
							nStateIndex++;
							pLayer = pLayer->Next();
						}

						/* Reset layer list and walk layers to reset
						original values.*/
						pLayer =		LayerGetFirst();
						nStateIndex =	0;

						while ( pLayer )
						{
							pLayer->On(pbState[nStateIndex]);
							pLayer->Save();

							/* Next please.*/
							nStateIndex++;
							pLayer = pLayer->Next();
						}

						/* Free allocated memory.*/
						VDELETE_ARRAY_NULL(pbState)

						/* Update progress percent to 0.*/
						if ( m_State.IsNotSet(STATE_INSIDE_DRAW_SILENT) )
							OnVectorizePercent(0);

						/* Success.*/
						bSuccess = VTRUE;
					}
				}
			}

			/* Still need to draw?*/
			if ( !bSuccess && adDrawDrawing(m_dbHandle) )
				bSuccess = VTRUE;

			/* Update progress percent to 0.*/
			if ( m_State.IsNotSet(STATE_INSIDE_DRAW_SILENT) )
				OnVectorizePercent(0);
		}

		return bSuccess;
	}

	/** This function causes OpenDWG to calculate the drawing extents but does
	not call any of the virtual functions. This is useful for cases where the
	drawn size of the drawing does not yet match the pixel space.*/
	void			DrawSilent()
	{
		VASSERT(m_dbHandle)

		m_State.Append(STATE_INSIDE_DRAW_SILENT);

		/* Set callback pointer to NULL.*/
		adSetCallbackUserdata(m_dbHandle, NULL);

		/* Draw now.*/
		Draw();

		/* Reset callback pointer.*/
		adSetCallbackUserdata(m_dbHandle, this);

		m_State.Remove(STATE_INSIDE_DRAW_SILENT);
	}

	/** Force a drawing to be regenerated, rather that drawn from
	display list.*/
	void			ForceRegen() const
	{
		VASSERT(m_dbHandle)
		adForceRegen(m_dbHandle);
	}

	/** Get a COLORREF (AutoCAD color) value from an OpenDWG color index.*/
	static COLORREF	GetColor(VUSHORT nIndex)
	{
		VASSERT(nIndex < VARRAY_SIZE(VOPENDWG_COLOR_MAP))
		return VOPENDWG_COLOR_MAP[nIndex];
	}

	/** Return a reference to the list of bad fonts for this drawing.*/
	VUnSortedStringLinkedList&	GetBadFontList() const
		{ return (VUnSortedStringLinkedList&)m_listBadFonts; }

	/** Return a reference to the list of bad XREF's for this drawing.*/
	VUnSortedStringLinkedList&	GetBadXREFList() const
		{ return (VUnSortedStringLinkedList&)m_listBadXREFs; }

	/** Return a reference to the display list rectangle.*/
	rectlong&		GetDisplayListRect() const
		{ return (rectlong&)m_rDisplayList; }

	/** Options that can be used with this class. Settable by use the
	GetOptions() function.*/
	enum	{		OPTION_USE_CARTESIAN_COORDINATES =	0x0001,
					OPTION_DRAW_BY_LAYER =				0x0002,
			};

	/** Return a reference to the object used to hold the above option bits.*/
	VBitmask&		GetDrawingOptions() const
		{ return (VBitmask&)m_Options; }

	/** Return a reference to the file name string object. This holds the fully
	qualified path to the drawing, if known.*/
	VString&		GetFileName() const
		{ return (VString&)m_strFileName; }

	/** Return the handle to the open drawing.*/
	AD_DB_HANDLE	GetHandle() const
		{ return m_dbHandle; }

	/** Return a pointer to the drawing header object. This pointer must NOT
	be deleted by calling code.*/
	AD_DWGHDR*		GetHeader() const
	{
		VASSERT(m_dbHandle)
		return adHeaderPointer(m_dbHandle);
	}

	/** Get a reference to the last (current) X position where drawing
	is occurring.*/
	VLONG&			GetLastXPosition() const
		{ return (VLONG&)m_nLastXPosition; }

	/** Get a reference to the last (current) Y position where drawing
	is occurring.*/
	VLONG&			GetLastYPosition() const
		{ return (VLONG&)m_nLastYPosition; }

	/** Return a reference to the list of layers for this drawing. The list
	will be initialized if needed.*/
	VLinkedListManager&	GetLayerList()
	{
		InitLayerList();
		return m_listLayers;
	}

	/** Return a reference to the pixel space rectangle.*/
	rectlong&		GetPixelSpace() const
		{ return (rectlong&)m_rWindow; }

	/** Return the pixel space width.*/
	VLONG			GetPixelSpaceWidth() const
		{ return m_rWindow.Xmax; }

	/** Return the pixel space height.*/
	VLONG			GetPixelSpaceHeight() const
		{ return m_rWindow.Ymax; }

	/** Get the raw extents.*/
	void			GetRawExtents(	VDOUBLE& nRawXSize,
									VDOUBLE& nRawYSize) const
	{
		VASSERT(m_dbHandle)
		adRawExtentsSizes(m_dbHandle, &nRawXSize, &nRawYSize);
	}

	/** Return the file type of the drawing database. Returns one of the
	OpenDWG defined constants AD_DWG, AD_DXF or AD_BDXF.*/
	VSHORT			GetType() const
	{
		VASSERT(m_dbHandle)
		return adFileTypeLoaded(m_dbHandle);
	}

	/** Return the AutoCAD version of the drawing database. This will be one
	of the predefined version constants, AD_ACAD25,AD_ACAD26, AD_ACAD9,
	AD_ACAD10, AD_ACAD11, AD_ACAD13, AD_ACAD14, or AD_ACAD2000.*/
	VSHORT			GetVersion() const
	{
		VASSERT(m_dbHandle)
		return adFileVersionLoaded(m_dbHandle);
	}

	/** Return a reference to the the current view parameters.*/
	AD_VIEW_PARMS&	GetView() const
		{ return (AD_VIEW_PARMS&)m_viewParams; }

	/** Initialize or un-initialize OpenDWG and VWCL support for it as needed.
	Returns VTRUE if initialized, VFALSE otherwise. If the OpenDWG error code
	return is needed, set pnError to a valid short. If pszADINIT_DATFileName
	is NULL, it is assumed that the ADINIT.DAT file is stored in the
	application resources in a user define type called "ADINIT_DAT" with the
	resource name of "ADINIT_DAT". This virtual file, embedded in resources,
	will then be used to initialize OpenDWG. Init(VTRUE) should be called only
	once per run of the application, and Init(VFALSE) should be called when
	OpenDWG support is no longer needed.*/
	static VBOOL	Init(	VBOOL			bInitialize = VTRUE,
							VSTRING_CONST	pszADINIT_DATFileName = NULL,
							VSHORT*			pnError = NULL)
	{
		/* Initialize out parameter?*/
		if ( pnError )
			*pnError = 0;

		/* Anything to do?*/
		if ( bInitialize == m_bInitialized )
			return m_bInitialized;

		/* Initialize or un-initialize?*/
		if ( bInitialize )
		{
			/* Initialize members of static class(es).*/
			VOpenDWGSettings::GetFontPathList().GetOptions().Append(
				VSortedStringLinkedList::OPTION_NO_DUPLICATES);

			/* Set critical error handler to our own, unless already done.*/
			if ( !m_bCriticalErrorHandlerInstalled )
				SetCriticalErrorHandler(criterrhandler);

			/* Set function pointers to read adinit.dat file that is embedded
			in our resources?*/
			if ( !pszADINIT_DATFileName )
			{
				adSetAd2OpenInitFileFn(vOpenInitFile);
				adSetAd2ReadInitFileCharFn(vReadInitFileChar);
				adSetAd2ReadInitFileCharsFn(vReadInitFileChars);
				adSetAd2CloseInitFileFn(vCloseInitFile);
			}

			VSHORT nError = 0;

			/* Read the initialization file embedded in our resources, or
			read from specified file.*/
			if ( adInitAd2(	(VVOIDPTR)pszADINIT_DATFileName,
							(pszADINIT_DATFileName) ? 0 : 1,
							&nError) )
			{
				/* Initialize OpenDWG Viewkit.*/
				if ( adInitAd3() )
				{
					/* Initialize color map.*/
					for (	VUSHORT i = 0;
							i <  VARRAY_SIZE(ad3parm.colormap);
							i++ )
						ad3parm.colormap[i] = i;

					/* Complete initialization.*/
					adAcceptR14BetaFiles();
					adSetupDwgRead();
					adSetupDxfRead();

					/* Set callback functions.*/
					ResetCallbacks();

					/* Set initialize flag.*/
					m_bInitialized = VTRUE;
				}
			}
			else if ( pnError )
				*pnError = nError;
		}
		else if ( m_bInitialized )
		{
			adCloseAd3();
			adCloseAd2();
			m_bInitialized = VFALSE;
		}

		return m_bInitialized;
	}

	/** Returns VTRUE if a drawing is currently being closed, that is, virtual
	functions are being called as part of the cleanup cycle.*/
	VBOOL			IsBeingClosed() const
		{ return m_State.IsSet(STATE_CLOSE_IN_PROGRESS); }

	/** Determine if the current pixel space is valid, that is, a rectangle
	with positive width and height.*/
	VBOOL			IsPixelSpaceValid() const
	{
		return	(GetPixelSpaceWidth() > 0 && GetPixelSpaceHeight() > 0)
				? VTRUE : VFALSE;
	}

	/** Returns VTRUE if the OpenDWG toolkit has been initialized.*/
	static VBOOL	IsInitialized()
		{ return m_bInitialized; }

	/** Add a new layer. On success a new VOpenDWGLayer object will be
	returned. The only initialized member of this object will be the name
	and internal handles. Calling code is then responsible for setting the
	layer information and using the Save() function of the VOpenDWGLayer
	object to set the layer into the database. NULL will be returned on
	failure, or if a layer with the same name already existed in the layer
	table.*/
	VOpenDWGLayer*	LayerAdd(VSTRING_CONST pszName)
	{
		VOpenDWGLayer* pLayer = NULL;

		/* Don't allow duplicates.*/
		if ( !LayerFind(pszName) )
		{
			AD_LAY layer;
			VZEROSTRUCT(layer);

			/* Initialize members.*/
			VSTRCPY(layer.name, pszName);
			layer.xrefindex = -1;

			/* Generate default handles.*/
			adGenerateObjhandle(m_dbHandle, layer.objhandle);
			adGenerateObjhandle(m_dbHandle, layer.linetypeobjhandle);

			if ( adAddLayer(m_dbHandle, &layer) )
			{
				pLayer = VNEW VOpenDWGLayer(m_dbHandle, layer);

				if ( pLayer )
					m_listLayers.Add(pLayer);
				else
					adDeleteLayer(m_dbHandle, layer.objhandle);
			}
		}

		return pLayer;
	}

	/** Delete a layer using the layer name. Returns VTRUE if the layer was
	found and deleted, VFALSE otherwise.*/
	VBOOL			LayerDelete(VSTRING_CONST pszName)
	{
		VOpenDWGLayer* pLayer = LayerFind(pszName);

		if ( pLayer && adDeleteLayer(m_dbHandle, pLayer->GetHandle()) )
		{
			m_listLayers.Delete(pLayer);
			return VTRUE;
		}

		return VFALSE;
	}

	/** Return a layer object when given a name, or NULL if a layer with the
	specified name was not found.*/
	VOpenDWGLayer*	LayerFind(VSTRING_CONST pszName)
	{
		/* The name must be given!*/
		VASSERT(VSTRLEN_CHECK(pszName))

		VOpenDWGLayer* pLayer = LayerGetFirst();

		while ( pLayer )
		{
			if ( VSTRCMP_NOCASE(pszName, pLayer->Name()) == 0 )
				break;
			else
				pLayer = pLayer->Next();
		}

		return pLayer;
	}

	/** Return the number of layers in the drawing.*/
	VLONG			LayerGetCount() const
	{
		VASSERT(m_dbHandle)
		return adNumLayers(m_dbHandle);
	}

	/** Return a pointer to the first VOpenDWGLayer object in the layers in
	this drawing. The object returned from this function is the first
	VOpenDWGLayer object, and is part of a linked list. Call the returned
	objects Next() function to get the next layer in the list. This function
	will return NULL if the layer information cannot be determined. Objects
	returned from this function MUST NOT be deleted by calling code. They
	are handled internally by this class and deallocated as needed.*/
	VOpenDWGLayer*	LayerGetFirst()
		{ return InitLayerList(); }

	/** Returns VTRUE if the layer specified by pszLayerName is turned on,
	VFALSE otherwise.*/
	VBOOL			LayerOnOff(VSTRING_CONST pszLayerName)
	{
		VASSERT(VSTRLEN_CHECK(pszLayerName))

		VOpenDWGLayer* pLayer = LayerFind(pszLayerName);

		if ( pLayer )
			return pLayer->On();

		return VFALSE;
	}

	/** Turn on or off a layer or multiple layers. Wildcard characters are
	allowed for this function. See the VString::IsWildCardMatch() function
	for more information about how wildcards can be specified and which ones
	are supported. If specified, all layers that match the wildcard will be
	turned on or turned off. VTRUE will be returned if any single layer that
	was specified was turned on or off. VFALSE will be returned if no layers
	were modified. To specify more than one layer, separate them with a
	nDelimiterChar character, which defaults to a comma separated list. Any
	embedded spaces will be trimmed from the layer name before processing.*/
	VBOOL			LayerOnOff(	VSTRING_CONST	pszLayerName,
								VBOOL			bOn,
								VTCHAR			nDelimiterChar = VTEXT(','))
	{
		VASSERT(VSTRLEN_CHECK(pszLayerName))

		/* Initialize return value.*/
		VBOOL bResult = VFALSE;

		/* Walk list of layers specified by pszLayerName.*/
		VString				sLayers(pszLayerName);
		VDelimitedString	ds(sLayers, nDelimiterChar);
		VSTRING_CONST		pszThisLayer;

		while ( (pszThisLayer = ds.Next()) )
		{
			VString sThisLayer(pszThisLayer);
			sThisLayer.Trim();

			/* Walk layers.*/
			VOpenDWGLayer* pLayer = LayerGetFirst();

			while ( pLayer )
			{
				VBOOL bThisResult = VFALSE;
				VBOOL bBreakLoop =	VFALSE;

				/* Direct hit?*/
				if ( sThisLayer == pLayer->Name() )
				{
					bThisResult =	VTRUE;
					bBreakLoop =	VTRUE;
				}
				/* Does this layer match criteria?*/
				else
				{
					VString sLayer(pLayer->Name());

					if (	sLayer.IsNotEmpty() &&
							sLayer.IsWildCardMatch(sThisLayer, VFALSE) )
						bThisResult = VTRUE;
				}

				/* Do something to this layer?*/
				if ( bThisResult )
				{
					pLayer->On(bOn);
					pLayer->Save();

					/* Save final result.*/
					bResult = VTRUE;
				}

				/* Next layer.*/
				if ( bBreakLoop )
					pLayer = NULL;
				else
					pLayer = pLayer->Next();
			}
		}

		return bResult;
	}

	/** Create a new, initialized but empty, drawing. Returns VTRUE on success,
	VFALSE on failure. This function returns VTRUE if a new drawing was
	created. It will return VFALSE otherwise. It is important to note that
	this function can return VFALSE and still have a drawing open. This
	would be the case when a drawing was already open and the CloseDrawing()
	function fails or was cancelled. This function will call the virtual
	function OnNewDrawing() after the drawing database has been created.
	OnNewDrawing() also has the ability to return VFALSE, which will result in
	the new database being closed. Please see the OpenDWG documentation for
	the adNewFile function for information about parameters to this function.
	The default parameters create a new drawing database compatible with
	AutoCAD Release 14.*/
	VBOOL			NewDrawing(	AD_OBJHANDLE*	pStockObjHandle = NULL,
								AD_CLASS*		pClasses = NULL,
								VSHORT			nNumClasses = 0,
								VSHORT			nVersion = AD_ACAD14)
	{
		VBOOL bSuccess = VFALSE;

		/* Close current file?*/
		if ( !CloseDrawing() )
		{
			/* Create a new database.*/
			m_dbHandle = adNewFile(	pStockObjHandle,
									pClasses,
									nNumClasses,
									nVersion);

			/* Complete initialization.*/
			if ( m_dbHandle )
				bSuccess = CompleteNewInit(NULL);
		}

		return bSuccess;
	}

	/** Open a drawing. Returns VTRUE on success, VFALSE on failure. Pass VTRUE
	for bA2KObjects if you want AutoCAD 2000 default objects added to the
	database, otherwise pass VFALSE for the bA2KObjects parameter.  If you
	would like to load an R14 or earlier file and save it as R14 or earlier,
	then pass VFALSE for this argument.  This will prevent the proxy warning
	box from appearing when such files are loaded into earlier versions of
	AutoCAD. This function returns VTRUE if the drawing in pszFileName was
	opened. It will return VFALSE otherwise. It is important to note that this
	function can return VFALSE and still have a drawing open. This would be
	the case when a drawing was already open and the CloseDrawing() function
	fails or was cancelled. This function will call the OnOpenDrawing()
	virtual function, which can also return VFALSE. In this case the newly
	opened drawing would be closed.*/
	VBOOL			OpenDrawing(	VSTRING_CONST	pszFileName,
									VBOOL			bA2KObjects = VFALSE)
	{
		/* Filename must be known!*/
		VASSERT(pszFileName)

		VBOOL bSuccess = VFALSE;

		/* Close current file?*/
		if ( !CloseDrawing() )
		{
			/* OpenDWG does not provide a method for this function on a per
			database level, so we must use a static variable to hold the
			"current" object for which we are dealing with.*/
			m_pThis = this;

			/* Open the database (drawing).*/
			m_dbHandle = adLoadFile(	(VVOIDPTR)pszFileName,
										AD_PRELOAD_ALL,
										(bA2KObjects) ? 1 : 0);

			/* Reset global pointer.*/
			m_pThis = NULL;

			/* Complete initialization.*/
			if ( m_dbHandle )
				bSuccess = CompleteNewInit(pszFileName);

			/* Set progress to 0 (done).*/
			OnLoadPercent(0);
		}

		return bSuccess;
	}

	/** Pan, or move the drawing, from point ptOrigin to ptDest.*/
	void			Pan(	VPoint const& ptOrigin,
							VPoint const& ptDest)
	{
		VASSERT(m_dbHandle)
		VASSERT(IsPixelSpaceValid())

		if ( IsPixelSpaceValid() )
		{
			/* Calculate delta in pixels.*/
			VDOUBLE nDeltaX =
				(ptOrigin.GetX() - ptDest.GetX()) * m_viewParams.xsize /
				m_rWindow.Xmax;

			VDOUBLE nDeltaY =
				(ptOrigin.GetY() - ptDest.GetY()) * m_viewParams.ysize /
				m_rWindow.Ymax;

			if ( nDeltaX || nDeltaY )
			{
				adPanTarget(	&m_viewParams,
								nDeltaX,
								(m_Options.IsSet(
									OPTION_USE_CARTESIAN_COORDINATES))
								? nDeltaY : -1 * nDeltaY);
			}
		}
	}

	/** Reset internal callback functions to call all OnXXX() member virtual
	functions. For performance reasons, some of these are allowed to be set
	to NOOP's using the RestrictCallbacks() function. This resets these
	callbacks. This resetting is also called when each new object of this
	type is created, and since these are not local to this class, it affects
	all objects globally throughout the application. See RestrictCallbacks()
	for more information.*/
	static void		ResetCallbacks()
	{
		/* Set non-vectorizing callback function pointers.*/
		adSetAd2DxfscanometerFn(vDXFScanometer);
		adSetAd2ExamineEntityLoadFn(vExamineEntityLoad);
		adSetAd2ExamineshapefileFn(vExamineFontFile);
		adSetAd3ExaminexrefFn(vExamineXREF);
		adSetAd2LoadometerFn(vLoadometer);
		adSetAd2SaveometerFn(vSaveometer);
		adSetAd3XrefcompleteFn(vXREFComplete);
		adSetAd3XrefhandleisFn(vXREFHandleIs);
		adSetAd3XrefopennotifyFn(vXREFOpenNotify);

		/* Set vectorizing callback function pointers.*/
		adSetAd3ExamineentityFn(vExamineEntity);
		adSetAd3MoveToFn(vMoveTo);
		adSetAd3LineToFn(vLineTo);
		adSetAd3PaintPolyFn(vPaintPoly);
		adSetAd3PenColorFn(vPenColor);
		adSetAd3OdometerFn(vLoadometerV);
		adSetAd3VectorFn(vVectorCount);
	}

	/** This enumeration is used with the RestrictCallbacks() function.*/
	enum	{		RESTRICT_ON_DXF_SCANOMETER =			0x0001,
					RESTRICT_ON_EXAMINE_ENTITY =			0x0002,
					RESTRICT_ON_EXAMINE_ENTITY_LOAD	=		0x0004,
					RESTRICT_ON_EXAMINE_FONT_FILE =			0x0008,
					RESTRICT_ON_EXAMINE_XREF =				0x0010,
					RESTRICT_ON_LOADPERCENT =				0x0020,
					RESTRICT_ON_SAVEOMETER =				0x0040,
					RESTRICT_ON_XREF_COMPLETE =				0x0080,
					RESTRICT_ON_XREF_HANDLE_IS =			0x0100,
					RESTRICT_ON_XREF_OPEN_NOTIFY =			0x0200,
					RESTRICT_ON_VECTOR_COUNT =				0x0400,
					RESTRICT_ON_VECTORIZE_PERCENT =			0x0800,
					RESTRICT_ALL_XREF_FUNCTIONS =
						RESTRICT_ON_EXAMINE_XREF |
						RESTRICT_ON_XREF_COMPLETE |
						RESTRICT_ON_XREF_HANDLE_IS |
						RESTRICT_ON_XREF_OPEN_NOTIFY,
					RESTRICT_ALL_PROGRESS_INDICATORS =
						RESTRICT_ON_DXF_SCANOMETER | RESTRICT_ON_LOADPERCENT |
						RESTRICT_ON_SAVEOMETER | RESTRICT_ON_VECTORIZE_PERCENT |
						RESTRICT_ON_VECTOR_COUNT,
			};

	/** To improve performance (usually slightly), this function can be used to
	instruct OpenDWG to not call some VWCL implemented callback functions.
	These callback functions are used to call the OnXXX() virtual functions
	in this class. If you eliminate a callback from functioning, the
	associated virtual function will not be called. This can be desired when
	you are writing an application where extremely high performance is
	required, and when you have no need to use those virtual functions or
	callbacks. If you later change your mind, the callbacks can be
	re-installed using the member function ResetCallbacks(). This function is
	passed a bitmask of functions to eliminate, and the value nMask is
	comprised by OR'ing the values from the above RESTRICT_XXX enumeration.
	You can also pass VTRUE for bAll to restrict all of these functions, in
	which case nMask can be 0. The enumeration has additional constants
	defined. These are RESTRICT_ALL_XREF_FUNCTIONS to eliminate all XREF
	functions and RESTRICT_ALL_PROGRESS_INDICATORS to restrict all functions
	that are used to provide progress or feedback to the user. This function
	is application global, it will affect all instances of VOpenDWG and
	derived class objects.*/
	static void		RestrictCallbacks(	VUINT nMask,
										VBOOL bAll = VFALSE)
	{
		/* All of them? Create complete masked bit set.*/
		if ( bAll )
			nMask = 0xffffffff;

		if ( nMask & RESTRICT_ON_DXF_SCANOMETER )
			adSetAd2DxfscanometerFn(NULL);

		if ( nMask & RESTRICT_ON_EXAMINE_ENTITY )
			adSetAd3ExamineentityFn(NULL);

		if ( nMask & RESTRICT_ON_EXAMINE_ENTITY_LOAD )
			adSetAd2ExamineEntityLoadFn(NULL);

		if ( nMask & RESTRICT_ON_EXAMINE_FONT_FILE )
			adSetAd2ExamineshapefileFn(NULL);

		if ( nMask & RESTRICT_ON_EXAMINE_XREF )
			adSetAd3ExaminexrefFn(NULL);

		if ( nMask & RESTRICT_ON_LOADPERCENT )
			adSetAd2LoadometerFn(NULL);

		if ( nMask & RESTRICT_ON_SAVEOMETER )
			adSetAd2SaveometerFn(NULL);

		if ( nMask & RESTRICT_ON_XREF_COMPLETE )
			adSetAd3XrefcompleteFn(NULL);

		if ( nMask & RESTRICT_ON_XREF_HANDLE_IS )
			adSetAd3XrefhandleisFn(NULL);

		if ( nMask & RESTRICT_ON_XREF_OPEN_NOTIFY )
			adSetAd3XrefopennotifyFn(NULL);

		if ( nMask & RESTRICT_ON_VECTOR_COUNT )
			adSetAd3VectorFn(NULL);

		if ( nMask & RESTRICT_ON_VECTORIZE_PERCENT )
			adSetAd3OdometerFn(NULL);
	}

	/** Save the current zoom view parameters to be restored later if needed.*/
	void			SaveCurrentZoom()
	{
		memcpy(	&m_viewParamsLastZoom,
				&m_viewParams,
				sizeof(m_viewParamsLastZoom));

		m_State.Append(STATE_ZOOM_PREVIOUS_ALLOWED);
	}

	/** Save the current drawing to a file. nFileType should be one of the
	defined constants AD_DWG, AD_DXF, or AD_BDXF. It can also be -1 to
	cause VWCL to use the same file type as when the drawing was opened
	or created. nVersion is the AutoCAD version of the file to be created,
	and should be one of the defined constants AD_ACAD25, AD_ACAD26,
	AD_ACAD9, AD_ACAD10, AD_ACAD11, AD_ACAD13, AD_ACAD14, or AD_ACAD2000. You
	can also pass -1 for this value and VWCL will save the file in the same
	version that it was opened with. The final four parameters are significant
	only if the file being written is a DXF file and you may simply send 0 for
	each of these values if you are not writing a DXF file. The VWCL provided
	defaults do nothing when not writing a DXF file, but are the "normal"
	parameters passed if a DXF file is being created. nDXFnegz, if set to 1,
	causes OpenDWG Toolkit to emit -0.0 (rather than 0.0) for small negative
	numbers, as AutoCAD does. nDXFdecprec is the decimal precision of the DXF
	file, and AutoCAD's default is 6. nDXFwritezeroes, if set to 1, causes the
	OpenDWG Toolkit to write Z coordinates with 0 values in cases where the
	comparable AutoCAD version emits them.  Finally, br12dxfvbls causes
	the OpenDWG Toolkit to emit a few extra header variables which are in
	AutoCAD Release 12, but not in Release 11. This variable is only
	significant if you are writing a Release 11 version DXF file. Returns
	VTRUE on success, VFALSE on failure. The default parameters save the file
	in the same file type and version as when it was created or opened. The
	only required parameter is the filename in pszFileName and even it can be
	NULL if a previous file was opened with this class and you want to save
	the new file with the same original file name.*/
	VBOOL			SaveDrawing(	VSTRING_CONST	pszFileName = NULL,
									VCHAR			nFileType = -1,
									VSHORT			nVersion = -1,
									VSHORT			nDXFnegz = 0,
									VSHORT			nDXFdecprec = 6,
									VSHORT			nDXFwritezeroes = 1,
									VCHAR			nr12dxfvbls = 1)
	{
		/* A drawing database must be open!*/
		VASSERT(m_dbHandle)

		if ( !pszFileName )
			pszFileName = m_strFileName;

		/* A file name must be given!*/
		VASSERT(VSTRLEN_CHECK(pszFileName))

		VBOOL bSuccess = VFALSE;

		if ( pszFileName )
		{
			/* Save this pointer during saves.*/
			m_pThis = this;

			if ( adSaveFile(	m_dbHandle,
								(VVOIDPTR)pszFileName,
								(VCHAR)((nFileType == -1)
									? GetType() : nFileType),
								(nVersion == -1) ? GetVersion() : nVersion,
								nDXFnegz,
								nDXFdecprec,
								nDXFwritezeroes,
								nr12dxfvbls) )
				bSuccess = VTRUE;

			/* Reset pointer.*/
			m_pThis = this;
		}

		return bSuccess;
	}

	/** Set pointer to user defined function which will be called during a
	critical error, as defined by OpenDWG. This handler should be set
	before calling Init(), otherwise the default VWCL handler will be used
	which simply calls exit(1), which is not likely to be the behavior you
	desire.*/
	static void		SetCriticalErrorHandler(
		VOPENDWG_CRITICAL_ERROR_HANDLER pfnCriticalErrorHandler)
	{
		adSetAd2CriticalErrorFn((pfnCriticalErrorHandler)
								? pfnCriticalErrorHandler : criterrhandler);

		m_bCriticalErrorHandlerInstalled = VTRUE;
	}

	/** Resize the "window" area. This is the area used when drawing. When
	vectorizing to the screen or printer, with width and height should
	represent the window, or window portion, you wish to draw the drawing
	into. This function should be called anytime the area that will be
	vectorized into is changed. This function can be called before a drawing
	is opened for vectorization. This function will return VTRUE if the pixel
	space changed, VFALSE if the current width and height is the same as what
	was passed as parameters.*/
	VBOOL			SetPixelSpace(	VLONG nWidth,
									VLONG nHeight)
	{
		/* TODO: This code does scales accurately, but could be improved
		to increase the visible drawing area with more math. These are
		skills that I don't possess, so please feel free to experiment
		with this function and propose changes. - Todd.*/

		/* Did anything change?*/
		if ( m_rWindow.Xmax != nWidth || m_rWindow.Ymax != nHeight )
		{
			/* Save new width and height. Xmin and Ymin must be 0 for now
			since OpenDWG does not support anything else.*/
			m_rWindow.Xmin = 0;
			m_rWindow.Ymin = 0;
			m_rWindow.Xmax = nWidth;
			m_rWindow.Ymax = nHeight;

			/* Just setting values before a drawing is open? If not, we have
			more calculations to do.*/
			if ( m_dbHandle )
			{
				/* Check for divide by zero first.*/
				if ( nWidth && m_viewParams.xsize )
				{
					/* Determine new aspect ratio.*/
					VDOUBLE nNewAspect= (VDOUBLE)nHeight / (VDOUBLE)nWidth;

					if ( nNewAspect > m_viewParams.ysize / m_viewParams.xsize )
						m_viewParams.ysize = m_viewParams.xsize * nNewAspect;
					else
						m_viewParams.xsize = m_viewParams.ysize / nNewAspect;

					/* Set regen flag.*/
					ForceRegen();
				}
			}

			return VTRUE;
		}

		return VFALSE;
	}

	/** Set the view to the view specified by the contents of an
	initialized AD_VIEW_PARMS structure.*/
	void			SetView(AD_VIEW_PARMS const& viewParams)
		{ memcpy(&m_viewParams, &viewParams, sizeof(m_viewParams)); }

	/** Zoom extents. This will cause the whole drawing to rendered to consume
	as much of the pixel space, as determined by SetPixelSpace(),
	as possible.*/
	void			ZoomExtents()
	{
		VASSERT(m_dbHandle)

		/* There must be a valid pixel space. Call SetPixelSpace() first!*/
		VASSERT(IsPixelSpaceValid())

		if ( IsPixelSpaceValid() )
		{
			/* Save the current zoom information and do the zoom.*/
			SaveCurrentZoom();
			adZoomExtents(m_dbHandle, &m_viewParams);
		}
	}

	/** Zoom to a named view. Returns VTRUE on success, VFALSE if the named
	view could not be found.*/
	VBOOL			ZoomNamedView(VSTRING_CONST pszName)
	{
		VASSERT(m_dbHandle)

		/* The view must be given!*/
		VASSERT(VSTRLEN_CHECK(pszName))

		/* TODO: This whole section will be replaced when full support
		for Views is implemented.*/
		AD_VIEW view;

		VLONG nCount = adNumViews(m_dbHandle);
		adStartViewGet(m_dbHandle);

		for ( VLONG i = 0; i < nCount; i++ )
		{
			VZEROSTRUCT(view);

			if ( adGetView(m_dbHandle, &view) )
			{
				if ( VSTRCMP_NOCASE(view.name, pszName) == 0 )
				{
					/* We found the view we are looking for.*/

					/* Save the current zoom information.*/
					SaveCurrentZoom();

					/* Set the view parameters to match.*/
					adSetViewParmsFromView(m_dbHandle, &m_viewParams, &view);

					/* Success.*/
					return VTRUE;
				}
			}
		}

		return VFALSE;
	}

	/** Zoom In or Out. nPercent is the amount of area to show from the
	previous view. A value smaller than 100 will result in a Zoom In
	operation, while a value greater than 100 will Zoom Out. A value
	of 0 or 100 is a NOOP.*/
	void			ZoomPercent(VUINT nPercent)
	{
		VASSERT(m_dbHandle)

		/* There must be a valid pixel space. Call SetPixelSpace() first!*/
		VASSERT(IsPixelSpaceValid())

		if ( nPercent != 0 && nPercent != 100 && IsPixelSpaceValid() )
		{
			/* Save the current zoom information.*/
			SaveCurrentZoom();

			VDOUBLE nDelta = ((VDOUBLE)nPercent / 100.0);

			m_viewParams.xsize *= nDelta;
			m_viewParams.ysize *= nDelta;
		}
	}

	/** Zoom to the last saved view (the previous zoom), if any. This is only
	valid to call after calling one of the other ZoomXXX() functions. If
	called before then, this function will be a NOOP. You can determine if
	a call to this function would be valid by using CanZoomPrevious().*/
	void			ZoomPrevious()
	{
		VASSERT(m_dbHandle)

		/* There must be a valid pixel space. Call SetPixelSpace() first!*/
		VASSERT(IsPixelSpaceValid())

		if ( CanZoomPrevious() )
		{
			/* Save current view first.*/
			AD_VIEW_PARMS now;
			memcpy(&now, &m_viewParams, sizeof(now));

			/* Save last zoom as current.*/
			memcpy(&m_viewParams, &m_viewParamsLastZoom, sizeof(m_viewParams));

			/* Save previous zoom as new last zoom.*/
			memcpy(&m_viewParamsLastZoom, &now, sizeof(m_viewParamsLastZoom));
		}
	}

	/** Zoom to a window defined by the rectangle r. It will be normalized as
	needed with VRect::Normalize().*/
	void			ZoomWindow(VRect& r)
	{
		VASSERT(m_dbHandle)
		VASSERT(IsPixelSpaceValid())

		if ( IsPixelSpaceValid() )
		{
			/* Save the current zoom information.*/
			SaveCurrentZoom();

			r.Normalize();

			/* Calculates a new target for a zoom.*/
			VDOUBLE nDeltaX =
				((r.GetLeft() + r.GetRight()) / 2 - m_rWindow.Xmax / 2) *
				m_viewParams.xsize / m_rWindow.Xmax;

			VDOUBLE nDeltaY =
				((r.GetTop() + r.GetBottom()) / 2 - m_rWindow.Ymax / 2) *
				m_viewParams.ysize / m_rWindow.Ymax;

			adPanTarget(	&m_viewParams,
							nDeltaX,
							(m_Options.IsSet(OPTION_USE_CARTESIAN_COORDINATES))
							? nDeltaY : -1 * nDeltaY);

			/* Compute new x and y sizes during zoom.*/
			VDOUBLE nNewXSize =
				r.GetWidth() * (m_viewParams.xsize / m_rWindow.Xmax);

			VDOUBLE nNewYSize =
				r.GetHeight() * (m_viewParams.ysize / m_rWindow.Ymax);

			/* Set them to the proper aspect ratio.*/
			VDOUBLE nOldAspect = m_viewParams.ysize / m_viewParams.xsize;

			if ( nNewYSize / nNewXSize < nOldAspect )
				nNewYSize = nNewXSize * nOldAspect;
			else
				nNewXSize = nNewYSize / nOldAspect;

			m_viewParams.xsize = nNewXSize;
			m_viewParams.ysize = nNewYSize;
		}
	}

protected:
	/** These values are used internally by the object to maintain state
	information. They are held in the m_State bitmask.*/
	enum	{		STATE_CLOSE_IN_PROGRESS =		0x0001,
					STATE_INIT_VECTORIZE_SUCCESS =	0x0002,
					STATE_ZOOM_PREVIOUS_ALLOWED =	0x0004,
					STATE_INSIDE_DRAW_SILENT =		0x0008,
			};

	/** Internal function to complete initialization when a new drawing has
	been created or opened. Returns VTRUE if all internal initialization
	succeeded, VFALSE otherwise. On failure, the new drawing will have been
	closed. If pszFileName is NULL, this function assumes a new database is
	being created.*/
	VBOOL			CompleteNewInit(VSTRING_CONST pszFileName)
	{
		/* The database handle must be known at this point.*/
		VASSERT(m_dbHandle)

		VBOOL bSuccess = VFALSE;

		/* Set callback pointer.*/
		adSetCallbackUserdata(m_dbHandle, this);

		/* Save file name if known.*/
		m_strFileName = pszFileName;

		/* If m_rWindow.Xmax or Ymax is 0, set to 1 so that adInitVectorize()
		completes properly.*/
		VBOOL bFixX = VFALSE;
		VBOOL bFixY = VFALSE;

		if ( !m_rWindow.Xmax )
		{
			m_rWindow.Xmax = 1;
			bFixX = VTRUE;
		}

		if ( !m_rWindow.Ymax )
		{
			m_rWindow.Ymax = 1;
			bFixY = VTRUE;
		}

		/* Initialize vectorization.*/
		if ( adInitVectorize(	m_dbHandle,
								&m_viewParams,
								&m_rWindow,
								&m_rDisplayList) )
		{
			/* Set success flag.*/
			m_State.Append(STATE_INIT_VECTORIZE_SUCCESS);

			/* Reset bogus Xmax and Ymax?*/
			if ( bFixX )
				m_rWindow.Xmax = 0;

			if ( bFixY )
				m_rWindow.Ymax = 0;

			/* Notify derived objects. If they return VFALSE,
			close the drawing.*/
			if ( pszFileName )
				bSuccess = OnOpenDrawing(pszFileName);
			else
				bSuccess = OnNewDrawing();
		}

		/* Close drawing on error.*/
		if ( !bSuccess )
			CloseDrawing(VTRUE);

		return bSuccess;
	}

	/** Initialize the layer list as needed. Returns first layer in the
	list on success, NULL on failure.*/
	VOpenDWGLayer*	InitLayerList()
	{
		/* A drawing database must be open!*/
		VASSERT(m_dbHandle)

		/* Have we done this already?*/
		if ( m_listLayers.m_pFirstItem )
			return (VOpenDWGLayer*)m_listLayers.m_pFirstItem;

		AD_LAY layer;

		/* We need to initialize the layer list.*/
		adStartLayerGet(m_dbHandle);

		/* Determine number of layers.*/
		VLONG nCount = LayerGetCount();

		for ( VLONG i = 0;  i < nCount; i++)
		{
			VZEROSTRUCT(layer);

			if ( adGetLayer(m_dbHandle, &layer) )
			{
				VOpenDWGLayer* pLayer = VNEW VOpenDWGLayer(m_dbHandle, layer);

				if ( pLayer )
					m_listLayers.Add(pLayer);
			}
		}

		return (VOpenDWGLayer*)m_listLayers.m_pFirstItem;
	}

	/** Override to be notified of a bad font file. This class verifies that
	pszFontFileName is not NULL or 0 length. If it is, this function will not
	be called. It is NOT valid to copy a new file name string to pszFileName
	during overrides of this function. This base class implementation does
	nothing in response to this message.*/
	virtual void	OnBadFontFile(VSTRING pszFontFileName)
		{;}

	/** Override to be notified of a bad XREF. This class verifies that
	pszFileName is not NULL or 0 length. If it is, this function will not
	be called. It is NOT valid to copy a new file name string to pszFileName
	during overrides of this function. This base class implementation does
	nothing in response to this message. This function will only be called
	once for each bad XREF. This class keeps an internal list of bad XREF file
	names to make sure.*/
	virtual void	OnBadXREF(VSTRING pszFileName)
		{;}

	/** Called just before a drawing is closed. The base class does nothing,
	except returns VTRUE. Overrides should return VTRUE to allow the drawing
	to be closed, or VFALSE to keep it open.*/
	virtual VBOOL	OnCloseDrawing()
		{ return VTRUE; }

	/** Called from the Draw() function to notify that all points after this
	function refer to the specified layer. If the layer should not be drawn,
	return VFALSE, otherwise return VTRUE to cause the layer to be drawn. The
	base class returns VTRUE. This function will only be called for drawings
	that have layers AND if the drawing option OPTION_DRAW_BY_LAYER is
	enabled.*/
	virtual VBOOL	OnDrawLayer(	VOpenDWGLayer&	layer,
									VUINT			nLayerNumber)
		{ return VTRUE; }

	/** Called during the initial scanning of DXF files only. This base class
	implementation does nothing in response to this message.*/
	virtual void	OnDXFScanometer(VSHORT nPercent)
		{;}

	/** Called during vectorization so that you can examine the entity that is
	being drawn. This base class implementation does nothing in response to
	this message.*/
	virtual void	OnExamineEntity(	AD_ENT_HDR&	header,
										AD_ENT&		entity)
		{;}

	/** Override to examine an entity during the drawing loading phase. This
	base class implementation does nothing in response to this message.*/
	virtual void	OnExamineEntityLoad(	AD_ENT_HDR&	header,
											AD_ENT&		entity)
		{;}

	/** Examine font file, possibly replacing it with another. The base class
	searches the font paths set with the function AddFontPath(). If a usable
	font is not found, it allows OpenDWG default processing to find the font,
	or use the internal font if one cannot be found. When this function is
	called, pszNewFontFileName is set to a 0 length string. This function
	only works on SHX type fonts, not TrueType fonts.*/
	virtual void	OnExamineFontFile(	VSTRING pszFontFileName,
										VSTRING pszNewFontFileName)
	{
		/* We only deal with non TrueType fonts.*/
		VString s(pszFontFileName);
		s.LowerCase();

		/* If we find a pipe or .ttf, skip it.*/
		if ( s.Find(VTEXT('|')) != -1 || s.Find(VTEXT(".ttf")) != -1 )
			return;

		VString sFullPath;

		/* Walk the list of font paths looking for a match in font file name.*/
		VLinkedListStringItem* pItem =
			VOpenDWGSettings::GetFontPathList().GetFirstItem();

		while ( pItem )
		{
			sFullPath.Append(pItem->String());
			sFullPath.Append(pszFontFileName);

			if ( VDoesFileExist(sFullPath.String()) )
			{
				VSTRCPY(pszNewFontFileName, sFullPath);
				return;
			}

			sFullPath.Empty(VFALSE);
			pItem = pItem->GetNextItem();
		}

		/* Add this font to the bad list as needed. This will return VTRUE if
		it was added. We then call OnBadFontFile().*/
		if ( m_listBadFonts.Add(pszFontFileName) )
			OnBadFontFile(pszFontFileName);
	}

	/** Examine an XREF and possibly change the filename or path. If you want
	to replace the XREF to open with another, the file name string can be
	copied to pszNewXREFFileName in overrides of this function. The base class
	function looks to see if the drawing referenced by pszXREFFileName can
	be found in the same directory as the open drawing, if any. If the XREF
	file is found, the full path to it will be copied to pszNewXREFFileName.
	If the referenced file cannot be found, OnBadXREF will be called as a
	notification and the XREF will be excluded from future use. When this
	function is called, pszNewXREFFileName is set to a 0 length string. It
	should be left as this to cause the XREF to be removed.*/
	virtual void	OnExamineXREF(	VSTRING pszXREFFileName,
									VSTRING pszNewXREFFileName)
	{
		if ( m_strFileName.IsNotEmpty() )
		{
			/* Make copy.*/
			VString s(m_strFileName);

			/* Terminate string at last file path.*/
			s.ReplaceLastChar(VFILE_PATH_SEP_CHAR);
			s.UpdateLength();
			s += VFILE_PATH_SEP_CHAR;
			s += pszXREFFileName;

			if ( VDoesFileExist((VSTRING_CONST)s) )
			{
				VSTRCPY(pszNewXREFFileName, s);
				return;
			}
		}

		/* Add this XREF to the bad list as needed. This will return VTRUE if
		it was added. We then call OnBadXREF().*/
		if ( m_listBadXREFs.Add(pszXREFFileName) )
			OnBadXREF(pszXREFFileName);
	}

	/** Draw a line to a point, making it the current point. Default
	implementation saves parameters as current point. Derived class should
	draw a line from the current point to nx, ny and then call the base class
	function.*/
	virtual void	OnLineTo(	VLONG nx,
								VLONG ny)
	{
		m_nLastXPosition = nx;
		m_nLastYPosition = ny;
	}

	/** Called during drawing load to allow derived class to display progress.*/
	virtual void	OnLoadPercent(VSHORT nPercent)
		{;}

	/** Set specified point to be the current point. Default implementation
	just does that. Most implementations will not require this function to
	be overridden. If it is, the override should call this base class
	function.*/
	virtual void	OnMoveTo(	VLONG nx,
								VLONG ny)
	{
		m_nLastXPosition = nx;
		m_nLastYPosition = ny;
	}

	/** Called when a new drawing database has been successfully created.
	This base class does nothing. If an override of this function returns
	VFALSE, the drawing will be closed and VFALSE will be returned to the
	caller of the NewDrawing() method. Return VTRUE to allow the drawing to
	remain open.*/
	virtual VBOOL	OnNewDrawing()
		{ return VTRUE; }

	/** Called when a drawing has been successfully opened. This base class
	does nothing. If an override of this function returns VFALSE, the drawing
	will be closed and VFALSE will be returned to the caller of the
	OpenDrawing() method. Return VTRUE to allow the drawing to remain open.*/
	virtual VBOOL	OnOpenDrawing(VSTRING_CONST pszFileName)
		{ return VTRUE; }

	/** Draw a polygon through all the points in pPoints array. There are
	nNumPoints in the array. The base class does nothing, and requires an
	override of this function to draw polygons.*/
	virtual void	OnPaintPolygon(	POINT*	pPoints,
									VUINT	nNumPoints)
		{;}

	/** Set the active pen color. The base saves the color only. Most
	implementations will not require this function to be overridden. If it is,
	the override should call this base class function. If rendering to screen
	or anywhere that needs Windows pens and brushes, derived code can simply
	create a VOpenDWGResourceRecycler object and pass all calls to this
	function to VOpenDWGResourceRecycler::ActiveColor(nPenColor).*/
	virtual void	OnPenColor(VSHORT nPenColor)
		{ m_nCurrentPenColor = nPenColor; }

	/** Called during drawing save to allow derived class to display progress.*/
	virtual void	OnSaveometer(VSHORT nPercent)
		{;}

	/** Notification of XREF vectorization complete.*/
	virtual void	OnXREFComplete(AD_DB_HANDLE dbHandle)
		{;}

	/** Notification of XREF database handle opened automatically by OpenDWG.*/
	virtual void	OnXREFHandleIs(AD_DB_HANDLE dbHandle)
		{;}

	/** Notification of XREF having been opened. The base class does nothing.
	Despite the OpenDWG documentation, this function is only called AFTER
	a XREF has been opened, not as a notification that it is about to be,
	as the documentation claims.*/
	virtual void	OnXREFOpenNotify(VSTRING pszFileName)
		{;}

	/** Called for every 25 vectors (by default) during the vectorization
	process. How often this is actually called depends on the value set
	using VOpenDWGSettings::VectorLimit(). The base class does nothing.*/
	virtual void	OnVectorCount()
		{;}

	/** Called during drawing vectorization to allow derived class to
	display progress.*/
	virtual void	OnVectorizePercent(VSHORT nPercent)
		{;}

	/** Reset vectorization object state. This function does not reset the
	window rectangle unless bResetWindowRectangle is VTRUE.*/
	void			ResetVectorState(VBOOL bResetWindowRectangle)
	{
		/* Terminate vectorization as needed.*/
		if ( m_dbHandle && m_State.IsSet(STATE_INIT_VECTORIZE_SUCCESS) )
			adTerminateVectorize(m_dbHandle);

		m_nLastXPosition =			0;
		m_nLastYPosition =			0;
		m_nCurrentPenColor =		0;
		VZEROSTRUCT(m_rDisplayList);
		VZEROSTRUCT(m_viewParams);
		VZEROSTRUCT(m_viewParamsLastZoom);

		if ( bResetWindowRectangle )
			VZEROSTRUCT(m_rWindow);

		m_State.Remove(STATE_INIT_VECTORIZE_SUCCESS);
	}

	/** Embedded Members.*/
	static VOpenDWGSettings		m_Settings;
	static VOpenDWG*			m_pThis;
	VLinkedListManager			m_listLayers;
	VUnSortedStringLinkedList	m_listBadXREFs;
	VUnSortedStringLinkedList	m_listBadFonts;
	VString						m_strFileName;
	VBitmask					m_Options;
	VBitmask					m_State;
	AD_DB_HANDLE				m_dbHandle;
	VLONG						m_nLastXPosition;
	VLONG						m_nLastYPosition;
	VUSHORT						m_nCurrentPenColor;
	rectlong					m_rWindow;
	rectlong					m_rDisplayList;
	AD_VIEW_PARMS				m_viewParams;
	AD_VIEW_PARMS				m_viewParamsLastZoom;

	/** Static Initialization Members.*/
	static HGLOBAL				m_hInitFileMem;
	static VSTRING				m_pszInitFile;
	static VSTRING				m_pszInitFileOffSet;
	static VBOOL				m_bInitialized;
	static VBOOL				m_bCriticalErrorHandlerInstalled;

private:
	/* Initialization functions for reading adinit.dat from embedded
	resources.*/

	/** "Close" the adinit.dat file. Just reset global variables.*/
	static void		vCloseInitFile()
	{
		m_hInitFileMem =						NULL;
		m_pszInitFileOffSet = m_pszInitFile =	NULL;
	}

	/** "Opens" the adinit.dat file.*/
	static VSHORT	vOpenInitFile()
	{
		/* Load the resource and get pointer to memory buffer.*/
		m_hInitFileMem = LoadResource(	VGetResourceHandle(),
										FindResourceEx(VGetResourceHandle(),
										VOPENDWG_ADINIT_DAT,
										VOPENDWG_ADINIT_DAT,
										MAKELANGID(LANG_NEUTRAL,
										SUBLANG_NEUTRAL)));
		if ( m_hInitFileMem )
		{
			m_pszInitFileOffSet = m_pszInitFile =
				(VSTRING)LockResource(m_hInitFileMem);

			return 1;
		}

		return 0;
	}

	/** Return a single character from resource init file and offset
	pointer by 1 char.*/
	static VCHAR	vReadInitFileChar()
	{
		VASSERT(m_pszInitFileOffSet)
		return *(++m_pszInitFileOffSet - 1);
	}

	/** Read nNumChars from resource init file and offset
	pointer by nNumChars.*/
	static void		vReadInitFileChars(	VSTRING psz,
										VUSHORT nNumChars)
	{
		VASSERT(psz && m_pszInitFileOffSet)
		memcpy(psz, m_pszInitFileOffSet, nNumChars);
		m_pszInitFileOffSet += nNumChars;
	}

	/* Non-vectorizing callback functions.*/

	/** Handle DXF file scanning.*/
	static void		vDXFScanometer(VSHORT nPercent)
		{ if ( m_pThis ) m_pThis->OnDXFScanometer(nPercent); }

	/** Examine entity during load.*/
	static void		vExamineEntityLoad(	PAD_ENT_HDR	pAD_ENT_HDR,
										PAD_ENT		pAD_ENT)
	{
		if ( m_pThis )
			m_pThis->OnExamineEntityLoad(	VREF_PTR(pAD_ENT_HDR),
											VREF_PTR(pAD_ENT));
	}

	/** Examine font file.*/
	static void		vExamineFontFile(	VSTRING		pszFontFileName,
										VSTRING		pszNewFontFileName,
										VVOIDPTR	pUserData)
	{
		if ( VOPENDWG_STRING_CHECK(pszFontFileName) )
		{
			*pszNewFontFileName = '\0';

			if ( pUserData )
				VOPENDWG_PTR(pUserData)->OnExamineFontFile(	pszFontFileName,
															pszNewFontFileName);

			if ( *pszNewFontFileName == '\0' )
				adDefaultExamineFile(	pszFontFileName,
										pszNewFontFileName,
										NULL);
		}
	}

	/** Examine a XREF and possibly change the filename or path.*/
	static void		vExamineXREF(	VSTRING		pszXREFFileName,
									VSTRING		pszNewXREFFileName,
									VVOIDPTR	pUserData)
	{
		if ( VOPENDWG_STRING_CHECK(pszXREFFileName) )
		{
			*pszNewXREFFileName = '\0';

			if ( pUserData )
				VOPENDWG_PTR(pUserData)->OnExamineXREF(	pszXREFFileName,
														pszNewXREFFileName);
		}
	}

	/** Displays progress during drawing loading.*/
	static void		vLoadometer(VSHORT nPercent)
	{
		if ( m_pThis )
			m_pThis->OnLoadPercent(nPercent);
	}

	/** Displays progress during drawing saving.*/
	static void		vSaveometer(VSHORT nPercent)
	{
		if ( m_pThis )
			m_pThis->OnSaveometer(nPercent);
	}

	/** Notify of XREF complete.*/
	static void		vXREFComplete(	AD_DB_HANDLE	dbHandle,
									VVOIDPTR		pUserData)
	{
		if ( pUserData )
			VOPENDWG_PTR(pUserData)->OnXREFComplete(dbHandle);
	}

	/** Notify of XREF database handle.*/
	static void		vXREFHandleIs(	AD_DB_HANDLE	dbHandle,
									VVOIDPTR		pUserData)
	{
		if ( pUserData )
			VOPENDWG_PTR(pUserData)->OnXREFHandleIs(dbHandle);
	}

	/** Notify of XREF opening.*/
	static void		vXREFOpenNotify(	VSTRING		pszFileName,
										VVOIDPTR	pUserData)
	{
		if ( pUserData )
			VOPENDWG_PTR(pUserData)->OnXREFOpenNotify(pszFileName);
	}

	/* Vectorizing callback functions.*/

	/** Examine entity.*/
	static void		vExamineEntity(	PAD_ENT_HDR	pAD_ENT_HDR,
									PAD_ENT		pAD_ENT,
									VVOIDPTR	pUserData)
	{
		if ( pUserData )
			VOPENDWG_PTR(pUserData)->OnExamineEntity(	VREF_PTR(pAD_ENT_HDR),
														VREF_PTR(pAD_ENT));
	}

	/** Move to a point.*/
	static void		vMoveTo(	VLONG		nx,
								VLONG		ny,
								VVOIDPTR	pUserData)
	{
		if ( pUserData )
		{
			VOpenDWG* pObj = (VOpenDWG*)pUserData;
			pObj->OnMoveTo(	nx,
							(pObj->m_Options.IsSet(
								OPTION_USE_CARTESIAN_COORDINATES))
							? ny : pObj->m_rWindow.Ymax - ny);
		}
	}

	/** Draw a line to a point.*/
	static void		vLineTo(	VLONG		nx,
								VLONG		ny,
								VVOIDPTR	pUserData)
	{
		if ( pUserData )
		{
			VOpenDWG* pObj = (VOpenDWG*)pUserData;
			pObj->OnLineTo(	nx,
							(pObj->m_Options.IsSet(
								OPTION_USE_CARTESIAN_COORDINATES))
							? ny : pObj->m_rWindow.Ymax - ny);
		}
	}

	/** Draw a polygon.*/
	static void		vPaintPoly(	polyHeadlong*		pHeadLong,
								struct _pointlong*	pPointLong,
								VVOIDPTR			pUserData)
	{
		if ( pUserData )
		{
			VASSERT(pPointLong)

			VOpenDWG* pObj = (VOpenDWG*)pUserData;

			VUINT nNumPoints = 1 + (pHeadLong->polyEnd - pHeadLong->polyBgn);

			if ( pObj->m_Options.IsNotSet(OPTION_USE_CARTESIAN_COORDINATES) )
			{
				for ( VUINT i = 0; i < nNumPoints; i++ )
					pPointLong[i].YC = pObj->m_rWindow.Ymax - pPointLong[i].YC;
			}

			pObj->OnPaintPolygon((POINT*)pPointLong, nNumPoints);
		}
	}

	/** Select a pen color.*/
	static void		vPenColor(	VSHORT		nColor,
								VVOIDPTR	pUserData)
	{
		if ( pUserData )
			VOPENDWG_PTR(pUserData)->OnPenColor(nColor);
	}

	/** Show progress percent during vectorization.*/
	static void		vLoadometerV(	VSHORT		nPercent,
									VVOIDPTR	pUserData)
	{
		if ( pUserData )
			VOPENDWG_PTR(pUserData)->OnVectorizePercent(nPercent);
	}

	/** Show vector count during vectorization.*/
	static void		vVectorCount(VVOIDPTR pUserData)
	{
		if ( pUserData )
			VOPENDWG_PTR(pUserData)->OnVectorCount();
	}
};

/* Include extra support if doing a Win32 GUI application.*/
#ifndef VWCL_NO_WIN32_API
	#ifndef VWCL_CONSOLE_APP
		/** VOpenDWGResourceRecycler is uses to minimize the amount of GDI
		resources used and to increase the performance of the vectorization
		process. It works by creating 8 standard pens and brushes that
		correspond to the default AutoCAD defined colors, and then creates
		and reuses 8 more dynamically as needed. It never creates more than
		16 pens and brushes at a time, and only 8 that are dynamic. It is
		careful about GDI object selection and is highly optimized to execute
		very fast. To see a sample of how this class can be used effectively,
		see the VOpenDWGWindow::OnPaint() method.*/
		class VOpenDWGResourceRecycler
		{
		public:
			/** Default constructor initilializes GDI objects. We create only 16
			pens/brushes (because Windows is tight for resource memory). We
			leave the first 8 the same as the default AutoCAD colors, and we
			manage the next 8 to recreate pens and brushes dynamically as we
			need new colors. Initially, we create only the original AutoCAD
			8 colors.*/
			VOpenDWGResourceRecycler()
			{
				/* Initialize Members.*/
				m_nActiveColorIndex =	0;
				m_nActivePenAndBrush =	0;
				m_nNextColorToReUse =	8;
				m_hOldBrush =			NULL;
				m_hOldPen =				NULL;

				COLORREF cr;

				for ( VSHORT i = 0; i < 8; i++ )
				{
					cr =					VOpenDWG::GetColor(i);
					m_nCurrentColors[i] =	i;
					m_Pens[i].				Create(PS_SOLID, 0, cr);
					m_Brushes[i].			CreateSolid(cr);
				}

				/* Initialize other array members.*/
				for ( i = 8; i < VARRAY_SIZE(m_Pens); i++ )
					m_nCurrentColors[i] = 0;
			}

			/** Virtual destructor does nothing. In debug builds it check to
			see if CleanUp() was called before destruction.*/
			virtual ~VOpenDWGResourceRecycler()
				{ VASSERT(m_hOldPen == NULL && m_hOldBrush == NULL) }

			/** Return the active color.*/
			COLORREF		ActiveColor() const
				{ return VOpenDWG::GetColor(m_nActiveColorIndex); }

			/** Call to set the active pen and brush to the color specified by
			the OpenDWG color nColorIndex.*/
			void			ActiveColor(	VDC&	dc,
											VSHORT	nColorIndex)
			{
				/* Any change? Is this the first time we were called?*/
				if (	!m_hOldBrush ||
						!m_hOldPen ||
						nColorIndex != m_nActiveColorIndex )
				{
					/* Remove previously selected objects, if any.*/
					CleanUp(dc);

					VSHORT nFoundIndex = 0;

					/* Colors below 8 (index 0-7) do not change.*/
					if ( nColorIndex < 8 )
						nFoundIndex = nColorIndex;
					else
					{
						/* See if we already have the color.*/
						for ( VSHORT i = 8; i < VARRAY_SIZE(m_Pens); i++ )
						{
							if ( m_nCurrentColors[i] == nColorIndex )
							{
								nFoundIndex = i;
								break;
							}
						}

						/* If we don't have the color, we need to create a
						new pen and brush.*/
						if ( !nFoundIndex )
						{
							/* Get new color.*/
							COLORREF cr = VOpenDWG::GetColor(nColorIndex);

							/* Create new pen and brush.*/
							m_Pens[m_nNextColorToReUse].Create(	PS_SOLID,
																0,
																cr);

							m_Brushes[m_nNextColorToReUse].CreateSolid(cr);

							/* Save current color and index.*/
							m_nCurrentColors[m_nNextColorToReUse] =
								nColorIndex;

							nFoundIndex = m_nNextColorToReUse;

							/* Reset if stepping over bounds.*/
							if (	++m_nNextColorToReUse >
									(VARRAY_SIZE(m_Pens) - 1) )
								m_nNextColorToReUse = 8;
						}
					}

					/* Save active pen and brush.*/
					m_nActivePenAndBrush = nFoundIndex;

					/* Save active color.*/
					m_nActiveColorIndex = nColorIndex;

					/* Select into device context.*/
					m_hOldBrush =
						(HBRUSH)dc.SelectObject(
							m_Brushes[m_nActivePenAndBrush].GetHandle());

					m_hOldPen =
						(HPEN)dc.SelectObject(
							m_Pens[m_nActivePenAndBrush].GetHandle());
				}
			}

			/** When drawing is complete, this function should be called to
			un-select pens and brushes and reset internal members.*/
			void			CleanUp(VDC& dc)
			{
				if ( m_hOldBrush )
				{
					dc.SelectObject(m_hOldBrush);
					m_hOldBrush = NULL;
				}

				if ( m_hOldPen )
				{
					dc.SelectObject(m_hOldPen);
					m_hOldPen = NULL;
				}
			}

			/** Call to get the current color index.*/
			VSHORT			GetActiveColorIndex() const
				{ return m_nActiveColorIndex; }

		protected:
			/** Embedded Members.*/
			VPen			m_Pens				[16];
			VBrush			m_Brushes			[16];
			VSHORT			m_nCurrentColors	[16];
			VSHORT			m_nNextColorToReUse;
			VSHORT			m_nActiveColorIndex;
			VSHORT			m_nActivePenAndBrush;
			HBRUSH			m_hOldBrush;
			HPEN			m_hOldPen;
		};

		#ifndef VWCL_NO_VWINDOW
			/** VOpenDWGWindow implements the rendering of VOpenDWG vectors
			to a window. In addition, it provides methods to allow complete
			customization of the look and feel of the window, and provides
			optional support for default handling of mouse messages. In
			addition, it provides double buffered drawing support, where all
			vectorization is done to an offscreen memory device context and
			then blasted onto the screen all at once. This greatly improves
			performance, especially when hiding or showing a window.*/
			class VOpenDWGWindow : public VWindow, public VOpenDWG
			{
			public:
				/** Default constructor initializes the object.*/
				VOpenDWGWindow()
				{
					/* Initialize members.*/
					m_pCurrentDC =			NULL;
					m_pRectTracker =		NULL;
					m_pLineTracker =		NULL;
					m_pResourceRecycler =	NULL;
					m_pBrush =				NULL;
					m_State =				STATE_DEFAULT;
					m_hCursor =				LoadCursor(NULL, IDC_CROSS);
					m_bInsideOnPaint =		VFALSE;

					/* We need OnPaint() to be called.*/
					GetOptions().Append(OPTION_NEED_ON_PAINT);
				}

				/** Virtual destructor verifies proper object cleanup.*/
				virtual ~VOpenDWGWindow()
					{ EndDoubleBuffering(); }

				/** Return the cursor to display. This is initialized by this
				class to the cross-hair cursor.*/
				HCURSOR			Cursor() const
					{ return m_hCursor; }

				/** Set the cursor to display. This is initialized by this
				class to the cross-hair cursor.*/
				void			Cursor(HCURSOR hCursor)
					{ m_hCursor = hCursor; }

				/** Determine if double buffered drawing is enabled.*/
				VBOOL			DoubleBufferingEnabled() const
					{ return m_State.IsSet(STATE_ALLOW_DOUBLE_BUFFERING); }

				/** Enable or disable support for double buffered drawing.*/
				void			DoubleBufferingEnabled(VBOOL bEnable)
				{
					m_State.AppendOrRemove(	STATE_ALLOW_DOUBLE_BUFFERING,
											bEnable);

					if ( !bEnable )
						EndDoubleBuffering();
				}

				/** Force a redraw of the drawing area, causing a
				vectorization. This should be called anytime something changes
				in the underlying drawing that would affect the display. An
				example would be turning a layer on or off. This causes the
				drawing to be vectorized with no attempt to use the cached
				display or double buffering.*/
				void			DrawNow()
				{
					m_State.Append(STATE_FORCE_VECTORIZATION);
					InvalidateUpdate();
				}

				/** Return the handle to the underlying OpenDWG database,
				or NULL, if none is open.*/
				AD_DB_HANDLE	GetDrawingHandle() const
					{ return m_dbHandle; }

				/** Determine if Pan mode is turned on.*/
				VBOOL			PanWindowModeEnabled() const
					{ return m_State.IsSet(STATE_PAN_WINDOW_MODE); }

				/** Enable or disable automatic panning mode when the user
				holds the left mouse button down.*/
				void			PanWindowModeEnabled(VBOOL bEnable)
				{
					m_State.AppendOrRemove(STATE_PAN_WINDOW_MODE, bEnable);

					if ( bEnable )
						m_State.Remove(STATE_ZOOM_WINDOW_MODE);
				}

				/** Set a pointer to a VBrush to use as the background color.
				If not explicitly set, or set to NULL, this class will fill
				the background with a black brush. The brush you pass will
				be used instead of black, if not NULL. If your application
				uses many of these objects (to display multiple drawings),
				each of these objects can share a pointer to a single brush.
				You are responsible for de-allocating the brush object if you
				do not use the default one provided by this class. This also
				provides a method for you to allocate and customize the brush
				object that this class will use.*/
				void			SetBackgroundBrush(VBrush* pBrush)
					{ m_pBrush = pBrush; }

				/** Set a pointer to a VLineTracker. If not explicitly set, or
				set to NULL, this class will allocate one per drawing, as
				needed, when handling panning. If your application uses many
				of these objects (to display multiple drawings), each of these
				objects can share a pointer to a single line tracker. You are
				responsible for de-allocating the line tracker object if you do
				not use the default one provided by this class. This also
				provides a method for you to allocate and customize the line
				tracker object that this class will use.*/
				void			SetLineTracker(VLineTracker* pLineTracker)
					{ m_pLineTracker = pLineTracker; }

				/** Set a pointer to a VRectTracker. If not explicitly set, or
				set to NULL, this class will allocate one per drawing, as
				needed, when handling zooming. If your application uses
				many of these objects (to display multiple drawings), each of
				these objects can share a pointer to a single rect tracker.
				You are responsible for de-allocating the rect tracker object
				if you do not use the default one provided by this class. This
				also provides a method for you to allocate and customize the
				rect tracker object that this class will use.*/
				void			SetRectTracker(VRectTracker* pRectTracker)
					{ m_pRectTracker = pRectTracker; }

				/** Set a pointer to a VOpenDWGResourceRecycler. If not
				explicitly set, or set to NULL, this class will allocate
				one per drawing, as needed, when handling painting. If your
				application uses many of these objects (to display multiple
				drawings), each of these objects can share a pointer to a
				single resource recycler. You are responsible for
				de-allocating the resource recycler object if you do not use
				the default one provided by this class.*/
				void			SetRecycler(VOpenDWGResourceRecycler*
											pResourceRecycler)
					{ m_pResourceRecycler = pResourceRecycler; }

				/** Determine if Zoom Window mode is turned on.*/
				VBOOL			ZoomWindowModeEnabled() const
					{ return m_State.IsSet(STATE_ZOOM_WINDOW_MODE); }

				/** Enable or disable automatic zoom window mode when the user
				holds the left mouse button down.*/
				void			ZoomWindowModeEnabled(VBOOL bEnable)
				{
					m_State.AppendOrRemove(STATE_ZOOM_WINDOW_MODE, bEnable);

					if ( bEnable )
						m_State.Remove(STATE_PAN_WINDOW_MODE);
				}

			protected:
				/** States (private) that can be used with this class.*/
				enum	{			STATE_NONE =					0x0000,
									STATE_ZOOM_WINDOW_MODE =		0x0001,
									STATE_PAN_WINDOW_MODE =			0x0002,
									STATE_ALLOW_DOUBLE_BUFFERING =	0x0004,
									STATE_FORCE_VECTORIZATION =		0x0008,
									STATE_DEFAULT =
										STATE_ALLOW_DOUBLE_BUFFERING,
						};

				/** Safely delete the memory DC and bitmap used for
				double-buffered drawing. This function does prevent future
				paint operations from using double buffering. To do that,
				remove the STATE_ALLOW_DOUBLE_BUFFERING state bit.*/
				void					EndDoubleBuffering()
				{
					if ( m_MemoryDC.GetHandle() )
					{
						DeleteObject(
							GetCurrentObject(	m_MemoryDC.GetHandle(),
												OBJ_BITMAP));
						m_MemoryDC.Free();
					}
				}

				/** Fill background of DC with black, or user defined color.*/
				void					FillDC(VDC& dc, VRect& rClient)
				{
					/* Fill with background color first.*/
					HBRUSH hBrush = (m_pBrush) ? m_pBrush->GetHandle() : NULL;

					if ( !hBrush )
						hBrush = GetStockBrush(BLACK_BRUSH);

					dc.FillRect(rClient, hBrush);
				}

				/** Override looks for new drawing to perform class cleanup
				and UI updates.*/
				virtual VBOOL			OnNewDrawing()
				{
					/* Update UI.*/
					EndDoubleBuffering();
					DrawNow();

					return VOpenDWG::OnNewDrawing();
				}

				/** Override looks for drawing close to perform class cleanup
				and UI updates.*/
				virtual VBOOL			OnCloseDrawing()
				{
					/* Update UI.*/
					EndDoubleBuffering();
					DrawNow();

					return VOpenDWG::OnCloseDrawing();
				}

				/** Override looks for drawing opening and causes
				initial rendering.*/
				virtual VBOOL			OnOpenDrawing(
					VSTRING_CONST pszFileName)
				{
					if ( VOpenDWG::OnOpenDrawing(pszFileName) )
					{
						DrawNow();
						return VTRUE;
					}

					return VFALSE;
				}

				/** Override draws a line to a point, making it
				the current point.*/
				virtual void			OnLineTo(	VLONG nx,
													VLONG ny)
				{
					VASSERT(m_pCurrentDC)

					/* Draw line from current position to nx, ny.*/
					m_pCurrentDC->LineTo(nx, ny);

					/* Call base class to update current position.*/
					VOpenDWG::OnLineTo(nx, ny);
				}

				/** Override moves current DC point to nx, ny.*/
				virtual void			OnMoveTo(	VLONG nx,
													VLONG ny)
				{
					VASSERT(m_pCurrentDC)

					/* Set DC to current position.*/
					m_pCurrentDC->MoveTo(nx, ny);

					/* Call base class to update current position.*/
					VOpenDWG::OnMoveTo(nx, ny);
				}

				/** This function will be called after a panning operation was
				performed by this class as a notification that it was done.
				This panning, and thus this notification, will only be sent
				if the PanWindowModeEnabled(VTRUE) function has been called.*/
				virtual void			OnPanComplete()
					{;}

				/** Override draws a polygon.*/
				virtual void			OnPaintPolygon(	POINT*	pPoints,
														VUINT	nNumPoints)
				{
					VASSERT(m_pCurrentDC)
					m_pCurrentDC->Polygon(pPoints, nNumPoints);
				}

				/** Overrides sets pen color in base class and in resource
				recycler class.*/
				virtual void			OnPenColor(VSHORT nPenColor)
				{
					VASSERT(m_pCurrentDC)
					VASSERT(m_pResourceRecycler)

					/* Base class just saves the color value.*/
					VOpenDWG::OnPenColor(nPenColor);

					/* Set the current color in the resource recycler. This
					allocates the pen as needed and selects it into the DC.*/
					m_pResourceRecycler->ActiveColor(	VREF_PTR(m_pCurrentDC),
														nPenColor);
				}

				/** Override draws the drawing.*/
				virtual VBOOL			OnPaint(VDC& dc)
				{
					/* Don't allow entrance back into this function
					before it completes.*/
					if ( !m_bInsideOnPaint )
					{
						m_bInsideOnPaint = VTRUE;

						/* Are we a visible window?*/
						if ( IsVisible() )
						{
							VRect rClient;
							GetClientRect(rClient);

							/* Always fill screen DC to start with.*/
							FillDC(dc, rClient);

							/* Is a drawing open? Is it being closed now? Is
							there sufficient pixel space to render to?*/
							if (	GetDrawingHandle() &&
									!IsBeingClosed() &&
									IsPixelSpaceValid() )
							{
								/* If we need to vectorize the drawing this
								will be VTRUE.*/
								VBOOL bVectorizeDrawing = VTRUE;

								/* Set current DC to screen DC for now.*/
								m_pCurrentDC = &dc;

								/* Determine if the window size changed
								since the last drawing.*/
								if	(	GetPixelSpaceWidth() !=
											m_rLastDrawnClient.GetWidth() ||
										GetPixelSpaceHeight() !=
											m_rLastDrawnClient.GetHeight()
									)
									m_State.Append(STATE_FORCE_VECTORIZATION);

								/* Forcing regen?*/
								if ( m_State.IsSet(STATE_FORCE_VECTORIZATION) )
									ForceRegen();

								/* Create new memory DC, if needed. On failure
								we will draw directly to screen without double
								buffering. Is double buffering allowed?*/
								if ( m_State.IsSet(
									STATE_ALLOW_DOUBLE_BUFFERING) )
								{
									/* Forced to vectorize? If yes, end double
									buffering now.*/
									if ( m_State.IsSet(
										STATE_FORCE_VECTORIZATION) )
										EndDoubleBuffering();

									/* Do we need to create a memory DC?*/
									if ( !m_MemoryDC.GetHandle() )
									{
										/* Create bitmap to select into
										memory DC.*/
										HBITMAP hBitmap =
											CreateCompatibleBitmap(
												dc.GetHandle(),
												rClient.GetWidth(),
												rClient.GetHeight());

										if ( hBitmap )
										{
											if ( dc.CreateCompatible(
												m_MemoryDC) )
											{
												m_MemoryDC.SelectObject(
													hBitmap);
												FillDC(m_MemoryDC, rClient);
												m_pCurrentDC = &m_MemoryDC;
											}
											else
												DeleteObject(hBitmap);
										}
									}
									/* We don't have to vectorize the drawing
									this time. The memory DC matches the
									screen.*/
									else
									{
										m_pCurrentDC = &m_MemoryDC;
										bVectorizeDrawing = VFALSE;
									}
								}

								/* This must be known!*/
								VASSERT(m_pCurrentDC)

								/* Should we draw (vectorize) the drawing?*/
								if ( bVectorizeDrawing )
								{
									/* This could take a while
									for large drawings.*/
									VWaitCursor wait;

									/* Allocate resource recycler, if needed.*/
									VBOOL bLocalAllocRR = VFALSE;

									if ( !m_pResourceRecycler )
									{
										m_pResourceRecycler =
											VNEW VOpenDWGResourceRecycler;

										bLocalAllocRR = VTRUE;
									}

									if ( m_pResourceRecycler )
									{
										/* Vectorize it.*/
										VOpenDWG::Draw();

										/* Cleanup recycler.*/
										m_pResourceRecycler->
											CleanUp(VREF_PTR(m_pCurrentDC));

										/* Delete resource recycler if
										we allocated it.*/
										if ( bLocalAllocRR )
											VDELETE_NULL(m_pResourceRecycler)
									}
								}

								/* Blit the memory DC onto the screen DC?*/
								if ( m_pCurrentDC == &m_MemoryDC )
									dc.BitBlt(rClient, m_MemoryDC);

								/* Always reset this pointer.*/
								m_pCurrentDC = NULL;

								/* Always remove this bit.*/
								m_State.Remove(STATE_FORCE_VECTORIZATION);

								/* Save last know client area.*/
								m_rLastDrawnClient = rClient;
							}
						}

						/* Re-Enable this function.*/
						m_bInsideOnPaint = VFALSE;
					}

					/* Message handled.*/
					return VFALSE;
				}

				/** Override sets the pixel space.*/
				virtual VLONG			OnSize(	VWPARAM	nFlags,
												VINT	ncx,
												VINT	ncy)
				{
					/* Force vectorization next draw if needed.*/
					if ( ncx && ncy && SetPixelSpace(ncx, ncy) )
						m_State.Append(STATE_FORCE_VECTORIZATION);

					return VWindow::OnSize(nFlags, ncx, ncy);
				}

				/** This function will be called after a zoom window operation
				was performed by this class as a notification that it was
				done. This zooming, and thus this notification, will only
				be sent if the ZoomWindowModeEnabled(VTRUE) function has
				been called.*/
				virtual void			OnZoomWindowComplete()
					{;}

				/** Override handles messages not wrapped by VWCL.*/
				VLONG					WindowProc(	HWND	hWnd,
													VUINT	nMessage,
													VWPARAM	wParam,
													VLPARAM	lParam)
				{
					switch ( nMessage )
					{
						case WM_ERASEBKGND:
						{
							VTempDC dc((HDC)wParam);
							VRect	rClient;
							GetClientRect(rClient);
							FillDC(dc, rClient);

							/* Message Handled.*/
							return VTRUE;
						}

						/* Handle zooming or panning.*/
						case WM_LBUTTONDOWN:
						{
							/* Do nothing if a drawing is not open.*/
							if ( VOpenDWG::GetHandle() )
							{
								VBOOL	bUpdate =	VFALSE;
								VPoint	ptStart(	LOWORD(lParam),
													HIWORD(lParam));

								/* Zoom window mode?*/
								if ( ZoomWindowModeEnabled() )
								{
									/* Allocate default rectangle tracker?*/
									VRectTracker* pRT =
										(m_pRectTracker) ? m_pRectTracker
										: VNEW VRectTracker(NULL, PS_SOLID);

									if ( pRT )
									{
										/* Draw selection box. If it returns
										VTRUE, zoom to selection.*/
										if ( pRT->TrackRubberBand(
											VREF_THIS, ptStart) )
										{
											/* Zoom and notify derived class
											of this action.*/
											ZoomWindow(pRT->GetRect());
											OnZoomWindowComplete();
											bUpdate = VTRUE;
										}

										/* Delete rect tracker if we
										allocated it.*/
										if ( !m_pRectTracker )
											VDELETE_NULL(pRT)
									}
								}
								/* Pan window mode?*/
								else if ( PanWindowModeEnabled() )
								{
									VPoint ptEnd;

									/* Allocate default line tracker?*/
									VLineTracker* pLT =
										(m_pLineTracker) ? m_pLineTracker
										: VNEW VLineTracker;

									if ( pLT )
									{
										/* Draw line tracker. If it returns
										VTRUE, pan to selection.*/
										if ( pLT->Track(VREF_THIS,
														ptStart,
														ptEnd) )
										{
											/* Pan and notify derived class
											of this action.*/
											Pan(ptStart, ptEnd);
											OnPanComplete();
											bUpdate = VTRUE;
										}

										/* Delete line tracker if we
										allocated it.*/
										if ( !m_pLineTracker )
											VDELETE_NULL(pLT)
									}
								}

								/* Update UI?*/
								if ( bUpdate )
									DrawNow();
							}

							break;
						}

						case WM_SETCURSOR:
							SetCursor(m_hCursor);
							return VTRUE;
					}

					return VWindow::WindowProc(hWnd, nMessage, wParam, lParam);
				}

				/** Embedded Members.*/
				VOpenDWGResourceRecycler*	m_pResourceRecycler;
				VDC*						m_pCurrentDC;
				VDC							m_MemoryDC;
				VBitmask					m_State;
				VRectTracker*				m_pRectTracker;
				VLineTracker*				m_pLineTracker;
				VBrush*						m_pBrush;
				VRect						m_rLastDrawnClient;
				HCURSOR						m_hCursor;
				VBOOL						m_bInsideOnPaint;
			};
		#endif
	#endif
#endif

#endif /* VOPENDWG*/

