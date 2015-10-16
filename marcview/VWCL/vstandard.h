


//WARNING -- I'VE COMMENTED OUT MUCH OF THIS TO AVOID COMPILATION MESSAGES, 
//		MIGHT BE SAFER TO RE-EXTRACT FROM ..\DOWNLOAD




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

#ifndef VSTANDARD
#define VSTANDARD

/** This is the standard include file for the Virtual Windows Class Library
(VWCL). This header replaces the older VWCL.H and is used only to define basic
functionality, and implies nothing about the application being written. In
fact, the application can be C++ or a mix of C and C++. Previous releases of
VWCL (Prior to December 1997) used the preprocessor extensively to include or
exclude code. VWCL grew to such a size that this method proved extremely hard
to maintain. The new method requires the users of VWCL to manually include the
classes they need. Those classes, should they require others, will include
those other headers as needed. This makes the code more maintainable, and also
has the benefit of better code re-use and builds smaller and tighter
applications by only including required code. There are still some basic
preprocessor command that can, or should, be defined:

VWCL_DONT_REGISTER_CLASSES
Causes VWCL initialization code to not register standard VWCL window class(es).
This is most useful for dialog box based applications, or where VWCL is being
compiled into a shared DLL.

VWCL_EXPORT_DLL
Define this when building a DLL version of class library. A shared VWCL DLL
can only be loaded once per process. VWCL has no state or resource management
functions, so multiple DLL's that use VWCL dynamically linked into one EXE
would fail to locate resources outside of the calling programs resources.
A call to VGetApp().Init() from these DLL's would corrupt the EXE's global
VApplication object.

VWCL_IMPORT_DLL
Define this when using VWCL from a DLL (importing).

VWCL_NO_WIN32_API
Define when the Win32 API is not available. WINE (www.winehq.com), and
MainWin (www.mainsoft.com) are implementations of Win32 on UNIX platforms.

VWCL_NO_VWINDOW
Define when VWindow should not be included by VApplication and other
non-Windowing classes.

VWCL_TARGET_IIS_EXTENSION
Define when building a Internet Information Server extension application.

VWCL_TARGET_LINUX
Defines Linux as the target UNIX platform.

VWCL_TARGET_SOLARIS
Defines Sun Microsystems Solaris as the target UNIX platform.

VWCL_TARGET_SCO
Defines SCO UNIX as the target UNIX platform.

VWCL_TARGET_HPUX
Defines HPUX as the target UNIX platform.

VWCL_TARGET_BSD
Defines BSD as the target UNIX platform.

VWCL_TARGET_DEC
Defines DEC Digital Unix (Tru64) as the target UNIX platform.

VWCL_TARGET_MAC
Defines Apple Macintosh as the target platform.

The following directives will be defined by this header file and should
not be defined elsewhere:

VWCL_CONSOLE_APP
Causes VWCL to eliminate code not needed when a console app is the target.

VWCL_TARGET_UNIX
Instructs VWCL classes to compile for (generic) UNIX platforms.

VWCL_DEBUG
VWCL directive defined when a debug build is the target for all platforms.

VWCL_UNICODE
VWCL directive defined when UNICODE is being used.*/

/* Make sure user did not already specify a directive this header defines.*/
#ifdef VWCL_CONSOLE_APP
	#error VWCL_CONSOLE_APP should not yet be defined. \
	For Windows console mode, define _CONSOLE
#endif
#ifdef VWCL_TARGET_UNIX
	#error VWCL_TARGET_UNIX should not yet be defined. \
	Define VWCL_TARGET_LINUX, VWCL_TARGET_SOLARIS, VWCL_TARGET_SCO, \
	VWCL_TARGET_HPUX, VWCL_TARGET_BSD, or VWCL_TARGET_DEC for UNIX \
	compatiblity.
#endif
#ifdef VWCL_DEBUG
	#error VWCL_DEBUG should not yet be defined. \
	For Windows, define _DEBUG. For UNIX, do NOT define NDEBUG.
#endif
#ifdef VWCL_UNICODE
	#error VWCL_UNICODE should not yet be defined. \
	For Windows, define _UNICODE.
#endif
#ifdef VODS
	#error VODS should not yet be defined.
#endif

/* Force strict type checking.*/
#ifndef STRICT
	#define STRICT
#endif

/* First, determine if a UNIX platform is the target.*/
#if defined VWCL_TARGET_LINUX
	#define VWCL_TARGET_UNIX
#elif defined VWCL_TARGET_SOLARIS
	#define VWCL_TARGET_UNIX
#elif defined VWCL_TARGET_SCO
	#define VWCL_TARGET_UNIX
#elif defined VWCL_TARGET_HPUX
	#define VWCL_TARGET_UNIX
#elif defined VWCL_TARGET_BSD
	#define VWCL_TARGET_UNIX
#elif defined VWCL_TARGET_DEC
	#define VWCL_TARGET_UNIX
#endif

/* Should VWCL_CONSOLE_APP be defined?*/
#ifndef VWCL_TARGET_UNIX
	#ifdef _CONSOLE
		#define VWCL_CONSOLE_APP
	#endif
#endif

/* Define other settings for console applications.*/
#ifdef VWCL_CONSOLE_APP
	#ifndef VWCL_DONT_REGISTER_CLASSES
		#define VWCL_DONT_REGISTER_CLASSES
	#endif
	#ifndef VWCL_NO_VWINDOW
		#define VWCL_NO_VWINDOW
	#endif
#endif

/* Should VWCL_DEBUG be defined?*/
#ifdef VWCL_TARGET_UNIX
	#ifndef NDEBUG
		#define VWCL_DEBUG
	#endif
#elif defined _DEBUG
	#define VWCL_DEBUG
#endif

/* Should VWCL_UNICODE be defined?*/
#ifdef _UNICODE
	#define VWCL_UNICODE
#endif

/* Include CRT standard headers.*/
#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef VWCL_DEBUG
	#include <assert.h>
#endif

/* Include platform specific headers.*/
#ifdef VWCL_TARGET_UNIX
	/* UNIX specific headers.*/
	#include <unistd.h>
	#include <stdarg.h>
#else
	/* Windows specific headers.*/
											/*000502 these don't compile, anyway, i don't need now
	#include <windows.h>
	#include <windowsx.h>
	#include <commctrl.h>
	#include <commdlg.h>
											*/
	#include <direct.h>
	#include <io.h>
	#include <tchar.h>
	#ifndef va_start
		#include <varargs.h>
	#endif
	/* Include Internet Information Server?*/
	#ifdef VWCL_TARGET_IIS_EXTENSION
		#include <httpext.h>
		#include <httpfilt.h>
	#endif
#endif

/* This section defines standard C/C++ data types as VWCL data types.*/

/** Target Dependant Signed Integer.*/
#define VINT			int
/** Target Dependant Unsigned Integer.*/
#define VUINT			unsigned VINT
/** Always char.*/
#define VCHAR			char
/** Always unsigned char.*/
#define VUCHAR			unsigned VCHAR
/** Always the same as VUCHAR.*/
#define VBYTE			VUCHAR
/** Target Dependant Signed Short Integer.*/
#define VSHORT			short
/** Target Dependant Unsigned Short Integer.*/
#define VUSHORT			unsigned VSHORT
/** Target Dependant Signed Long Integer.*/
#define VLONG			long
/** Target Dependant Unsigned Long Integer.*/
#define VULONG			unsigned VLONG
/** Target Dependant Float.*/
#define VFLOAT			float
/** Target Dependant Double.*/
#define VDOUBLE			double
/** Target Dependant Long Double.*/
#define VLONGDOUBLE		long VDOUBLE
/** Always a void*.*/
#define VVOIDPTR		void*
/** Always a void const*.*/
#define VVOIDPTR_CONST	void const*
/** Same as VUCHAR, always a 1 byte (8 bit) Unsigned.*/
#define VBOOL			VUCHAR

/* This section maps Windows defined data types as new types, or uses
existing types when available.*/
#ifdef VWCL_NO_WIN32_API
	/** Same as VCHAR (No UNICODE support).*/
	#define VTCHAR			VCHAR
	/** Same as VTCHAR* (No UNICODE support).*/
	#define VSTRING			VTCHAR*
	/** Same as VTCHAR* (No UNICODE support - const).*/
	#define VSTRING_CONST	VTCHAR const*
	/** Same as VBOOL.*/
	#define VWINBOOL		VBOOL
	/** Same as VUSHORT.*/
	#define VWORD			VUSHORT
	/** Same as VULONG.*/
	#define VDWORD			VULONG
	/** Same as VUINT.*/
	#define VWPARAM			VUINT
	/** Same as VLONG.*/
	#define VLPARAM			VLONG
#else
	/** Same as TCHAR (UNICODE supported).*/
	#define VTCHAR			TCHAR
	/** Same as VTCHAR* (UNICODE supported).*/
	#define VSTRING			VTCHAR*
	/** Same as VTCHAR const* (UNICODE supported - const).*/
	#define VSTRING_CONST	VTCHAR const*
	/** Same as Windows defined BOOL.*/
	#define VWINBOOL		BOOL
	/** Same as Windows defined WORD.*/
	#define VWORD			WORD
	/** Same as Windows defined DWORD.*/
	#define VDWORD			DWORD
	/** Same as Windows defined WPARAM.*/
	#define VWPARAM			WPARAM
	/** Same as Windows defined LPARAM.*/
	#define VLPARAM			LPARAM
#endif

/* This section creates macros which map non-portable C Runtime
Library Functions.*/

/** Define VASSERT macro, which really does compile away to nothing for
release builds. This macro already defines the trailing ; so users should
NOT append a semicolon to the end of this macro. The reason for this is that
if the semicolon is there, it results in double semicolons during debug
builds and a single semicolon in release builds. This normally does not do
any harm, but some compilers have been reported to have problems with this.*/
#ifdef VWCL_DEBUG
	#define VASSERT(expr)	assert(expr);
#else
	#define VASSERT(expr)
#endif

/** Define VVERIFY which compiles to VASSERT(expression) in debug build, and
just expression in release build. This is useful when the expression should
be evaluated in the debug and release builds, but when the expression is still
required to be computed in release builds. Like VASSERT, you should not
include the trailing semicolon. It will be inserted as needed in release
builds.*/
#ifdef VWCL_DEBUG
	#define	VVERIFY(expr) VASSERT(expr)
#else
	#define VVERIFY(expr) expr;
#endif

/** Define VVERIFY_EXPR which can be used like VVERIFY, except it can also be
used in a conditional construct such as a if or while statement. During
debug builds, the macro will assert if the expression is evaluated as false.
During release builds, the expression is evalulated, but will never assert.*/
#ifdef VWCL_DEBUG
	#define VVERIFY_EXPR(expr)	((expr) (_assert(#expr,__FILE__,__LINE__),0))
#else
	#define VVERIFY_EXPR(expr)	(expr)
#endif

/** OutputDebugString wrapper. Sends output to OutputDebugString() on Windows,
stderr otherwise. If stderr is wanted, even on Windows platforms, use
VODS_CONSOLE, which always sends output to stderr. Both macros compile
to nothing in a non-debug build. Under a UNIX target, VODS is the same as
VODS_CONSOLE and always sends output to stderr. In all cases, you should not
include a trailing semicolon. This macro will insert one as needed.*/
#ifdef VWCL_DEBUG
	#define VODS_CONSOLE(string)	fprintf(stderr, "\nDEBUG: %s\n", string);
	#ifdef VWCL_NO_WIN32_API
		#define VODS(string)		VODS_CONSOLE(string)
	#else
		#define VODS(string)		OutputDebugString(string);
	#endif
#else
	#define VODS_CONSOLE(string)
	#define VODS(string)
#endif

/** Macro's for min() and max().*/
#define VMIN(a, b)					((a) <= (b) ? (a) : (b))
#define VMAX(a, b)					((a) >= (b) ? (a) : (b))

/* String functions common to UNIX and Windows implemented in C runtime
libraries.*/
#define VSTRCAT				strcat
#define	VSTRCHR				strchr
#define	VSTRCMP				strcmp
#define	VSTRCPY				strcpy
#define	VSTRDUP				strdup
#define	VSTRNCPY			strncpy
#define	VSTRLEN				strlen
#define VSTRRCHR			strrchr
#define	VSTRSTR				strstr
#define VISLOWER			islower
#define VISUPPER			isupper
#define VTOLOWER			tolower
#define VTOUPPER			toupper

/* String functions NOT common to UNIX and Windows implemented in C runtime
libraries.*/
#ifdef VWCL_TARGET_UNIX
	#define	VSTRCMP_NOCASE	strcasecmp
	#define	VSTRNCMP_NOCASE	strncasecmp
#else
	#define	VSTRCMP_NOCASE	stricmp
	#define	VSTRNCMP_NOCASE	strnicmp
#endif

/** Macro to return string length, or 0 if string is NULL.*/
#define	VSTRLEN_CHECK(str)	((str) ? VSTRLEN(str) : 0)

/** VREF_PTR macro derefences any pointer to a reference.*/
#define	VREF_PTR(p)			*p

/** Use VREF_THIS when you have a "this" pointer and need to convert it to
a reference.*/
#define VREF_THIS			*this

/** Macro to determine static array size.*/
#define VARRAY_SIZE(the_array)		sizeof(the_array) / sizeof(the_array[0])

/** Macro to zero fill a section of memory.*/
#define VZEROMEMORY(address, bytes)	memset((VVOIDPTR)address, (VINT)0, (size_t)bytes)

/** Macro to zero fill all members of a structure.*/
#define VZEROSTRUCT(zero_struct)	VZEROMEMORY(&zero_struct, sizeof(zero_struct))

/** Macro to define true and false values.*/
#define	VTRUE					1
#define	VFALSE					0

/** Macro to force operator new to return NULL and not throw an exception.
TODO: This is not yet implemented until the compiler vendors make ANSI
compliant C++ compilers.*/
#define	VNEW					new

/** For consistency with VNEW, we define VDELETE.*/
#define VDELETE					delete

/** VDELETE_NULL deletes an object and sets the pointer to NULL. Do NOT end
this macro with a ;*/
#define VDELETE_NULL(ptr)		{ delete ptr; ptr = NULL; }

/** When deleting array, use this version.*/
#define VDELETE_ARRAY			VDELETE []

/** VDELETE_ARRAY_NULL deletes an array of objects and sets the pointer to
NULL. Do NOT end this macro with a ;*/
#define VDELETE_ARRAY_NULL(ptr)	{ delete [] ptr; ptr = NULL; }

/** This macro will return either _T(string) or just (string), which on
Windows platforms, can be used to create a UNICODE string from a literal
string if UNICODE is defined.*/
#ifdef VWCL_TARGET_UNIX
	#define	VTEXT(str)			str
#else
	#ifdef VWCL_UNICODE
		#define VTEXT(str)		_T(str)
	#else
		#define	VTEXT(str)		str
	#endif
#endif

/** Define the default file path separator character.*/
#ifdef VWCL_TARGET_UNIX
	static VSTRING_CONST	VFILE_PATH_SEP =		VTEXT("/");
	static VTCHAR const		VFILE_PATH_SEP_CHAR =	VTEXT('/');
#else
	static VSTRING_CONST	VFILE_PATH_SEP =		VTEXT("\\");
	static VTCHAR const		VFILE_PATH_SEP_CHAR =	VTEXT('\\');
#endif

/** This macro simply expands to Don Box's (dbox@develop.com) _U macro, to take
any string, ANSI or UNICODE, and return whatever type of string is requested by
a function. ustring.cpp or vstringconvert.c must be included in the project.*/
#define		VTEXT_ANY			_U

/** Same as above, but adds non-const conversions.*/
#define		VTEXT_ANY_NON_CONST	_UNCC

/** Macro compiles to VShowLastErrorMessage() global function for Win32 debug
builds, nothing otherwise. expr is the expression to evaluate. If expr
evaluates to a 0 value, VShowLastErrorMessage() will be called, otherwise it
will not. In essence, this works exactly like VASSERT, where you expect expr
to be non-0, but shows a messagebox or sends output to VODS. Like VASSERT, do
not end this macro with a semicolon, since that would leave a bogus extra one
that some compilers may have problems with.*/
#ifndef VWCL_NO_WIN32_API
	#ifdef VWCL_DEBUG
		#define VSHOWLASTERRORMESSAGE(expr, hwnd)	if ( !(expr) ) VShowLastErrorMessage(hwnd);
	#else
		#define VSHOWLASTERRORMESSAGE(expr, hwnd)
	#endif
#else
	#define VSHOWLASTERRORMESSAGE(expr, hwnd)
#endif

/** During debug builds, this string is defined, mostly for use in messagebox
titles.*/
#ifdef VWCL_DEBUG
	//static VSTRING_CONST VWCL_DEBUG_MESSAGE_STRING =
	//	VTEXT("VWCL DEBUG MESSAGE");
#endif

#ifdef __cplusplus
	/* Forward Declaration(s).*/
	class						VApplication;

	/* Returns a reference to the VApplication application object.*/
	VApplication&				VGetApp();

	/* The following are only valid on Win32 platforms.*/
	#ifndef VWCL_NO_WIN32_API
		/** Handle dailog box message routing.*/
		#ifndef VWCL_CONSOLE_APP
			//VBOOL				VTranslateDialogMessage(MSG const& msg);
		#endif

		/* The standard window class that VWCL registers unless
		VWCL_DONT_REGISTER_CLASSES is defined.*/
		//static VSTRING_CONST	VWINDOWCLASS = VTEXT("VWindow");
	#endif
#endif

/* Standard C style Global VWCL API's. These functions are (mostly)
implemented in vwclport.c.*/
#ifdef __cplusplus
	extern "C" {
#endif

/* Number to string conversions. In all cases, the parameters are as follows:
nValue - The number to convert.
pszBuffer - Where to place the output string. It must be large enough to hold
the string representation of the number.
nRadix - Base of value, must be in the range 2 - 36. Most of the time this
is base 10.
All functions return a pointer to pszBuffer on success, NULL on failure.*/

/** Convert a VINT to ASCII.*/
VCHAR*				VITOA(VINT nValue, VCHAR* pszBuffer, VUINT nRadix);
/** Convert a VUINT to ASCII.*/
VCHAR*				VUITOA(VUINT nValue, VCHAR* pszBuffer, VUINT nRadix);
/** Convert a VLONG to ASCII.*/
VCHAR*				VLTOA(VLONG nValue, VCHAR* pszBuffer, VUINT nRadix);
/** Convert a VULONG to ASCII.*/
VCHAR*				VULTOA(VULONG nValue, VCHAR* pszBuffer, VUINT nRadix);

/** Determine if a file, not a directory, exists.*/
//VBOOL				VDoesFileExist(VSTRING_CONST pszFileName);

/** Determine if a directory, not a file, exists.*/
//VBOOL				VDoesDirectoryExist(VSTRING_CONST pszDirectoryName);

/** Return a static string buffer to the applications title, or name. If
building a standard Windows GUI VWCL application, this functions will
already be implemented for you in VApplication. However, if you are picking
and choosing classes from VWCL to use in your application, you may have to
implement this C function because classes that require this will have to
have them to compile and link properly.*/
//VSTRING_CONST		VGetAppTitle();

/** Return the argc values passed to main() or WinMain(). For UNIX targets,
this must be set with VSetARGC_ARGV().*/
VUINT				VGetARGC();

/** Return one string from the argv array passed to main() or WinMain(). For
UNIX targets, this must be set with VSetARGC_ARGV(). nIndex specifies which
string from the array to return. It is valid to pass an invalid index for this
parameter since the function will return NULL if nIndex exceeds the number of
command line arguments.*/
//VSTRING_CONST		VGetARGV(VUINT nIndex);

/** This function must be used on UNIX targets to save the argc and argv passed
to main(). For Windows targets it is optional, and if fact does nothing.
VGetARGC() and VGetARGV() are still valid on Windows, they just don't have to
be set. On these platforms, the compiler stores these in global variables
which VWCL uses rather than the user specified ones.*/
//void				VSetARGC_ARGV(VUINT nARGC, VSTRING* ppszARGV);

/** Convert pszConvert to lowercase, returning a pointer to it.*/
VSTRING				VSTRLWR(VSTRING pszConvert);

/** Convert pszConvert to uppercase, returning a pointer to it.*/
VSTRING				VSTRUPR(VSTRING pszConvert);

#ifndef VWCL_NO_WIN32_API
	/** If building a standard Windows GUI VWCL application, these functions
	will already be implemented for you in vapplication.cpp and other global
	modules. However, if you are picking and choosing classes from VWCL to
	use in your application, you may have to implement these C style functions
	because classes that require these will have to have them to compile and
	link properly.*/

	/** Return the show command (ShowWindow() SW_xxx constant passed on the
	command line).*/
	VINT		VGetCommandShow();

	/** Return the global instance handle of the application or DLL.*/
	#define HINSTANCE WORD
	HINSTANCE	VGetInstanceHandle();

	/** Return the instance handle where resources are held.*/
	HINSTANCE	VGetResourceHandle();

	/* GDI Support Routines implemented in vgdiglobal.c.*/
	//void		VMapCoords(	HDC		hDC,
	//						SIZEL*	pSizeL,
	//						VBOOL	bToPixels);

	#define		VPixelsToHIMETRIC(hDC, pSizeL)		VMapCoords(hDC, pSizeL, VFALSE)
	#define		VPixelsFromHIMETRIC(hDC, pSizeL)	VMapCoords(hDC, pSizeL, VTRUE)

	/* ActiveX Support Routines implemented in vactivexglobal.cpp.*/

	/** Determine if a string is a valid ActiveX structured storage name.
	This includes length and valid characters.*/
	//VBOOL		VIsValidStructuredStorageName(VSTRING_CONST pszName);

	/** Given a C string, allocate a new string with CoTaskMemAlloc()
	and copy it.*/
	//VBOOL		VCoStringFromString(	VSTRING*		ppszCoString,
	//										VSTRING_CONST	pszString);

	/* Debugging Support Routines implemented in vdebugglobal.c.*/

	/** Show the result from GetLastError() as a message box, or VODS if a
	console application. This function is implemented in
	utility/vdebugglobal.c. This function is not available in release builds,
	so it should rarely be called directly. Rather, the VSHOWLASTERRORMESSAGE
	macro should be used.*/
	#ifdef VWCL_DEBUG
		//VINT	VShowLastErrorMessage(HWND hWndParent);
	#endif
#endif

#ifdef __cplusplus
	}
#endif

#endif /* VSTANDARD*/
