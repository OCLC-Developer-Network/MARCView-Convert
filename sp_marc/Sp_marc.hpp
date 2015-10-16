/*
Copyright 1992-2009 Systems Planning (www.systemsplanning.com)

Licensed under the Apache License, Version 2.0 (the "License"); 
you may not use this file except in compliance with the License. 
You may obtain a copy of the License at 

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software 
distributed under the License is distributed on an "AS IS" BASIS, 
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
See the License for the specific language governing permissions and 
limitations under the License.
*/

//961107	begin
//961202	converted most strings from char* to BYTE*
//			renamed typedefs LPUSTR, LPCUSTR, LPPUSTR to avoid confusion
//990619	4th param of SP_Subfield has to be pointer to return value


#if !defined (SP_MARC_H)
#define SP_MARC_H


typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef unsigned int        UINT;
typedef unsigned int        HANDLE;
typedef char _far *         LPSTR;
typedef const char _far *   LPCSTR;	
typedef char _far **        LPPSTR;
typedef BYTE _far *         LPUSTR;
typedef const BYTE _far *   LPCUSTR;
typedef BYTE _far **        LPPUSTR;


#include <stdio.h>
#include <wstring.hpp>


const char zmarcrecterm = 29;
const char zmarcfldterm = 30;
const char zmarcsubterm = 31;

const int  zerrmsglen	= 40;      	//to simplify process of building error message in 
                                   	//SP_ValidateRecord, each part of msg is this length

const long zmaxreclen = 100000L;	//051222 one less magic number
const long zmaxfldlen = 10000;		//051231


//must be a define, because it's used in sizing arrays
#define ZMAXERRORS	   5			//number of errors in SP_ValidateRecord before quitting


//altho we do not convert in all these directions, leave in, as some are used for Import/Export
typedef enum {UNKNOWN=0, ANSEL, LATIN, LEADER, MAKRBRKR, PLAIN, UNIMARC, UTF8} yCharSet;	//charset of record

//070912 added NOTSPECIFIED to trigger radio buttons for user to select
typedef enum {NOCONVERSION=0, TOANSEL, TOLATIN, TOMAKRBRKR, TOPLAIN, TOUNIMARC, TOUTF8,
				NOTSPECIFIED} yCharConvert;

typedef enum {NOTCONTROL=0, CONTROL} yControlField;
typedef enum {MARC21BIB, MARC21AUTH, UNIMARCBIB, UNIMARCAUTH, MARCXMLBIB, MARCXMLAUTH} yFormat;


//error codes (will get linker warnings of redefinition)
const int E_FATALVALERROR	=   3;	//file ok ...
const int E_MINORVALERROR	=   2;	// ... record has problems
const int E_PERFECTSUCCESS	=   1;	//both file and record ok
const int E_EXCEPTION		=   0;
const int E_GENERAL			=  -1;
const int E_FILENOTFOUND	=  -2;
const int E_FILENOTOPEN		=  -3;
const int E_UNEXPECTEDEOF 	=  -4;
const int E_NONMARC			=  -5;
const int E_MORETHANMAX		=  -6;
const int E_FUTUREFEATURE	=  -7;
const int E_NOMEMORY		=  -8;
const int E_SECURITY		=  -9;
const int E_CONVERSION		= -10;
const int E_BLOCKED			= -11;


/*050127 wanted to have error messages that can be applied everywhere,
			but weird compiler warning caused by this code. just a warning, but won't
			continue compiling
char* errortext [] = "May be a blocked or Unicode UCS-16 file";			// -11
					"Character conversion problem",						// -10
					"Illegal use of SP_MARC",							// -9
					"Out of memory",									// -8
					"Future feature",									// -7
					"Maximum records exceeded",						// -6
					"Not a legal MARC record",							// -5
					"Unexpected end of file",							// -4
					"File not open",									// -3
					"File not found or in use by another application",	// -2
					"Unspecified problem",								// -1
					"Nonfatal exception",								// 0
					"Perfect success",								// 1
					"Minor validation error",							// 2
					"Fatal validation error"};							// 3
*/


typedef struct tagPREFS 
{
	int 			nonnumerictags;
	yCharSet		charset;
    long        	maxrecords;
} SP_prefs_rt;


/*050127 no longer used, but save for reference
//memory cover functions (expose for SP_VAL)
int 		bMalloc 		(void**, unsigned long);
int 		bRealloc 	(void**, unsigned long);
void 	vFree 		(void*);
*/


//exported functions (for WDLLEXPORT, see Optima manual p. 741
#ifdef __cplusplus
extern "C" {
#endif

#if !defined WDLLIMPORT
	#define WDLLIMPORT __declspec(dllimport) __stdcall
#endif

#if !defined WDLLEXPORT
	#ifdef __BORLANDC__
		#define WDLLEXPORT _export _far _pascal
	#else
		#define WDLLEXPORT __declspec(dllexport) __stdcall
	#endif
#endif

//051230 we expose these for MVTest, but should not be called by MV/MC
//long 	WDLLEXPORT 	UtfToUnicodeDecimal	(char*);

// externally callable
void		WDLLEXPORT	SP_CloseFile			();
int 		WDLLEXPORT 	SP_ConvertAnsel 		(WString&, LPPUSTR, long&, yCharConvert, int displaycodes=0);
int		WDLLEXPORT	SP_ConvertLatin		(WString&, LPPUSTR, long&, yCharConvert, int displaycodes=0);
int 		WDLLEXPORT 	SP_ConvertUnimarc		(WString&, LPPUSTR, long&, yCharConvert, int displaycodes=0);
int 		WDLLEXPORT 	SP_ConvertUTF8			(WString&, LPPUSTR, long&, yCharConvert, int displaycodes=0);
int 		WDLLEXPORT 	SP_Field				(LPUSTR, LPPUSTR, long&, LPPUSTR);
int	 	WDLLEXPORT 	SP_FieldIsControlField 	(BYTE*);
SP_prefs_rt WDLLEXPORT	SP_GetPreferences		();
int	 	WDLLEXPORT 	SP_InitializeSymbols	();
int 		WDLLEXPORT 	SP_LatinToAnsel		(LPPUSTR, long&);
int 		WDLLEXPORT 	SP_LatinToLatin		(LPPUSTR);
int 		WDLLEXPORT 	SP_NextRecordOffset 	(long&);
long		WDLLEXPORT 	SP_PhysicalRecordLength	(long);

/*060202
int 		WDLLEXPORT 	SP_Record				(long, LPPUSTR, LPPSTR);
int 		WDLLEXPORT 	SP_RecordByOffset 		(long&, LPPUSTR, long&, LPPSTR);*/
int 		WDLLEXPORT 	SP_Record				(WString&, long, LPPUSTR);
int 		WDLLEXPORT 	SP_RecordByOffset 		(WString&, long&, LPPUSTR, long&);

int		WDLLEXPORT	SP_RecordFormat		();
void 	WDLLEXPORT	SP_ResetField			(void);
long		WDLLEXPORT	SP_RecordOffset		(long);
void 	WDLLEXPORT	SP_ResetRecord			(void);
void 	WDLLEXPORT 	SP_SetPreferences		(SP_prefs_rt);
int		WDLLEXPORT	SP_Statistics			(long far **);
int 		WDLLEXPORT 	SP_Subfield			(char, LPPUSTR, long&, char*, yCharConvert);
void		WDLLEXPORT 	SP_Terminate			(void);
int 		WDLLEXPORT 	SP_Unblock			(LPCUSTR, LPCUSTR, LPCUSTR, long&, long&, long, long, long);
int 		WDLLEXPORT 	SP_UseFile			(LPCUSTR, LPCUSTR, long&);


#ifdef _DEBUG
int			WDLLEXPORT	SP_ValidateRecord		(WString&, int, char*, int);
#else
int			WDLLEXPORT	SP_ValidateRecord		(WString&, int);
#endif


#ifdef __cplusplus
};
#endif


#endif

