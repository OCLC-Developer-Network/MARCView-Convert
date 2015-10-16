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

/*
960729 suggestions
	permit backward srchg or wrap FindNext (not done)
	keyboard access to buttons (not done)
	default button sb Next when at 1st rec, Prev when at last rec (not done)

HISTORY:
960824	RELEASE 2.0 RELEASED (Delphi) All further work is in Optima++ 1.5

961030	added Navigation Grid. always scrolls to current record

970115	implemented quicker scan of file in SP_UseFile, without any calls inside loop

970115	RELEASE 2.5 BETA

970118	added level-2 validation before record displayed, as bad records would cause
				display to blow up.
		
970131	took two days to find out why a far pointer was being copied to where there was room for
			only a near, thus blasting EntryOffset and resulting in garbage dir-tags. the lesson is
			that you shouldn't pass near pointers as fars within the DLL
			
970201	added some stuff for hiliting found terms. believe the only way to
			do this is to develop a concordance between the Latin and Plain records (just
			before displaying)
		discovered that first click in grid did nothing for normal files. believe fixed
			by putting check for _inFileDlg back into LeftButtonUp, where it didn't work
			before (maybe I had it in RightButtonUp?

970201	2.5 RELEASE CANDIDATE

970206	RELEASE 2.5 BETA SENT OUT
			hilite term after search (partially done)
			page up/down for scrolling thru a record (comes with richtext)
			search for presence of specific marc field (done)
			search of content designators

970211	to reduce DLL size, deleted Application class and MAIN.CPP file from SP_MARC
			target. had NO effect on DLL size! (backup in \marc\newmarc safe\

970220	permits varying number of junk chars after records (enclosed in ifdef R26)

970222	began working on preferences dialog. at one point something caused the top and left
			settings to be set to 3000, so app ran but off desktop. deleted INI to cure.
			
970224	masked text boxes (in record index column settings) apparently don't allow null
			entries. if you delete the default, it comes back. (if you enter a 2-char
			tag the program sets it to null.) solve this before releasing, so users can
			delete a column if they don't want all 4. (prob means using normal textbox
			and parsing.)

970315	RELEASE 2.6 BETA. 2.5 never released.

(for subsequent comments see MV_M.XLS)
*/


#if !defined(SP_MARC_H)
#include "SP_MARC.HPP"
#endif

#if !defined(SYMBOLS_H)
#include "SYMBOLS.HPP"
#endif

#include "math.h"
#include "wcm.hpp"	// C++ types

#include <stdlib.h>
#include <string.h>
#include <assert.h>	// may be able to remove this after all asserts converted to WASSERT
#include <ctype.h>
#include <sys\stat.h>	//050116 to get file stats in SP_Usefile

long iiMaxRecs;

#define ADDMESSAGE(x)		rMessage=rMessage+"\t"+x+"\n"

#ifdef _DEBUG
	#define ADDDEBUGMESSAGE(x)	rMessage=rMessage+"\tDEBUG\t"+x+"\n"
	#define DEBUGGERBREAK		_asm {int 3};	//causes debugger breakpoint so locals can be checked
#else
	#define ADDDEBUGMESSAGE(x)
	#define DEBUGGERBREAK
#endif

const BYTE 	LOWESTANSEL		= 161;	// A1
const BYTE 	LOWESTANSELDIA		= 224;	// E0
const BYTE 	MARCESCAPECODE	 	= 27;

const BYTE	fldtermstring [2] 	= "\x1E";
const BYTE	subtermstring [2] 	= "\x1F";

FILE*		f;

long			BaseAddress;
long			DirEntries;
long			EntryOffset;
long			FieldLength;
long			FieldOffset;
long			RecordOffset;		// exact offset of this record. caller adds length of this record
							//		in order to determine probable offset of next rec. 
							//		ReadRecord starts with this probable offset and skips
							//		non-digits looking for a Record Length. in this way we
							//		skip certain kinds of garbage between records.

//000209 not sure why they are BYTE instead of char
BYTE			TheRecord 	[zmaxreclen];
BYTE			TheField 		[zmaxfldlen];		// TheField and TheSubfield are copied from the record so they can have
BYTE			TheSubfield	[zmaxfldlen];		//		terminating 0's put in.

BYTE			Work	 		[zmaxreclen];		// used to store Latin or Plain version of record, field, or subfield
BYTE			debugWork		[zmaxfldlen];			// for debugging char conversions ***** shorter than Work !!!!! ******

BYTE			TheTag 		[4];				// copied from record so it can have a trailing 0

long			RecCount;
long			RecLength;
long			SubfieldLength;
long			SubfieldOffset;		//offset of next subfield
long*		pRecArray;

long*		pReport;				//990823 for SP_Statistics report

SP_prefs_rt		Preferences;
extern symbol_rt	Symbols	[ZNUMSYMS];

yCharSet		OriginalRecordCharset;		//this is the charset of the current record
yFormat		RecordFormat;
int			bUseOffsetArray;
int			bXML;				//040720 1 if we have an xml file
int			CheckIllegalChars;	//050125

//051222
extern charconvert_rt	MARC2UTF[zmarctoutf_size];
extern charconvert_rt	UTF2MARC[zutftomarc_size];
extern decomp_rt		DECOMP[zdecomp_size];

int			UseCodetables;		//060108 whether conversion requires codetables
#define		MAX_DIACRITICS 10		// number of dias that can modify a single base char
#define		MARCCHARLEN 11		// MARC symbols are 6 bytes max, but we have to
								// 		allow storage of an NCR which can be 10 bytes, plus \0
								
char	UTF8_Missing_Symbol[4];	// set to one of the following in SP_UseFile
// the default
#define		UTF8_GETA		"\xe3\x80\x93"    
// AAA wants this instead
#define		UTF8_DOUBLE_UNDERSCORE	"\xe2\x80\x97"

WString		TheClient;

//060531
WBool		ForceDoubleDias;	// setting for UTF-8 double ligature and tilde. set in SP_UseFile


typedef struct tagMARCOUTCOMB
{
	//each of these is long enough for the longest possible symbol, plus 1 for trailing 0
	char marc 	[MARCCHARLEN];
	char charset [2];
} marcoutcomb_rt;


typedef struct tagMARCINCOMB
{
	// includes trailing 0's
	char marc;
	char charset;
} marcincomb_rt;


/* definitions in subroutine names:
	Unicode		-- the actual uni character
	UtfNumber		-- the long int equiv of the Unicode
	Utf			-- the actual utf bytes
	UtfHex		-- the bytes in hex printable form with spaces ("D7 81"
	UtfDecimal	-- the bytes in decimal printable form with spaces ("215 197")
*/

// internal subroutines
//	-- parameters may be out of date and pgm will still compile, so the definition is the authority
int 		CheckFile 			(FILE* ff);
int 		ConvertCodes 			(BYTE* prec);
int 		FindMarcEquivalent  	(WString& rMessage, charconvert_rt& rcc);
int 		FindDecomp 			(WString rMessage, charconvert_rt& rcc, WString& rresults, WString& rcharsets);
int		FindUtfEquivalent 	    	(WString& rMessage, charconvert_rt& rcc);
int 		GetNextXMLField 		();
int 		GetOneMarcComb 		(WString& rMessage, char* thefield, marcincomb_rt marccomb[], char* charset, char* prev_basic, char* prev_ext);
int 		LookupOneUtfChar           (WString& rMessage, char* lookup, charconvert_rt& rcc);
WBool 	IsCombining			(charconvert_rt& rcc);
WString	LookupSymbol 			(char* lookup, yCharSet charset, yCharConvert target);
WString 	MakeEscapeSequence      (char current_cs, char* prev_basic_cs, char* prev_extended_cs);
int 		MakeLeaderIndex 		(int ldrbyte, int ldrvalue);
int 		OutputMarcChar 		(WString& rMessage, char mc, char charset, char* prev_basic, char* prev_ext, 
									yCharConvert target, long& roffset, int displaycodes);
int 		ReadRecord 			(WString& rMessage, long offset);
int 		ReadXMLFile 			();
int 		ReadXMLRecord 		    	(long offset);
WString 	StringToHex			(WString in);
WBool 	SubstituteGuts 		(char* output, int charlen, char orig, char* modifier, char subst);
WBool 	SubstitutePrecomposed	(marcoutcomb_rt marcout[], int mcindex);
WString	UnicodeToUtf			(WString in);
WString 	UtfToUtfHex 			(char* utf);
WString 	UtfToUnicode 			(WString& rMessage, char* utf);
long 	UtfToUnicodeNumber  	(WString& rMessage, char* in);
int 		ValidateTag 			(char* pts, int allowx);


/* 970107 seems to run fine without these
//961122 this doesn't seem to get called, breakpoint not hit when inside
int ar _pascal LibMain (HANDLE hInstance, WORD wDataSeg, WORD cbHeapSize, char far * lpCmdLine)
{
    return 1;
}

int ar _pascal WEP (int nParam)
{
    return 1;
}
*/


//////////////////////// GENERAL SUBROUTINES //////////////////////



//CheckFile -- assert file open
int CheckFile (FILE* ff)
{
	if (ff)
		return 1;
	else
    	return 0;
}
//CheckFile



//ConvertCodes
//this can be done in place, since length is not changing.
//nonreversable because ¶ is an ANSEL symbol
int ConvertCodes (BYTE* prec)
{
	char*	tempo;	//points to character in prec

	//replace field terminator
	tempo = strpbrk ((char const*) prec, (char const *) fldtermstring);
	if (tempo)
		*tempo = '¶';

	//replace subfield codes
	tempo = strpbrk ((char const*) prec, (char const *) subtermstring);
	while (tempo)
	{
		*tempo = '‡';	//double dagger
		tempo = strpbrk ((char const*) prec, (char const *) subtermstring);
	}
	
	//030820 replace unimarc nsb/nse
	/*030820 we can't do this here as we need the hex values to colorize the display
	tempo = strpbrk ((char const *) prec, (char const *) zNSB);
	while (tempo)
	{
		*tempo = '{';
		tempo = strpbrk ((char const *) prec, (char const *) zNSB);
	}
	tempo = strpbrk ((char const *) prec, (char const *) zNSE);
	while (tempo)
	{
		*tempo = '}';
		tempo = strpbrk ((char const *) prec, (char const *) zNSE);
	}
	*/
	
	return E_PERFECTSUCCESS;
}
//ConvertCodes



//ConvertHexToLong -- improvement on the one in yUtility
long ConvertHexToLong (char* ss)
{
	long ii = 0;
	int i, j;
	char c;
	
	char* s = strupr(strrev(ss));
	
	for (i = 0; i < strlen(s); i++)
	{
		c = s[i];
		if (isalpha(c))
			c = c - 55;
		else
			c = c - 48;
		
		j = pow(16, i);
		ii += c * j;
	}
	
	return ii;
}
//ConvertHexToLong



//FindDecomp
int FindDecomp (WString rMessage, charconvert_rt& rcc, WString& rresults, WString& rcharsets)
{
	int i, j;
	WString t;
	
	int high = sizeof (DECOMP) / sizeof (*DECOMP);
	long low = 0;
	long mid = (high + low) / 2;
	
	while (high >= low)
	{
		j = strcmp (rcc.utf8, DECOMP[mid].utf8);
		if (j == 0)
		{
			// found, now look up marc equiv for each char in compat
			WString s = DECOMP[mid].decomp;
			WStringArray ss = s.Parse();
			for (i = 0; i < ss.GetCount(); i++)
			{
				strcpy (rcc.utf8, ss[i].GetText());
				// we know they are in UTF2MARC because we created DECOMP that way, but still ...
				if (FindMarcEquivalent(rMessage, rcc))
				{
					// UNTESTED
					rresults.Concat(rcc.marc);
					rresults.Concat (zmarcsubterm);	// using sfd to separate these, as we can't use space
					rcharsets.Concat(rcc.charset[0]);
					rcharsets.Concat(' ');
				}
				else
					return 0;
			}
			
			return 1;
		}
		else if (j < 0)
			high = mid - 1;
		else
			low = mid + 1;
			
		mid = (high + low) / 2;
	}
	
	return 0;
}
//FindDecomp



//FindMarcEquivalent
int FindMarcEquivalent (WString& rMessage, charconvert_rt& rcc)
{
	// we want to get a MARC code in the current table, if poss, so use the cc.charset
	//		for this, which is the charset of the prev char
	
	int i, j;

	long high = sizeof (UTF2MARC) / sizeof (rcc);		// change this if we use this sub for other direction
	long low = 0;
	long mid = (high + low) / 2;
	
	// save passed-in values
	char c = rcc.charset [0];	// charset of previous character; we want to stay in same charset if possible
	char lookup [5];
	strcpy (lookup, rcc.utf8);
							
	// we want to use B or E char if matching charset not
	//		found; these hold these during scanning
	charconvert_rt B_nr, E_nr;	
	memset (&B_nr, 0, sizeof (B_nr));
	memset (&E_nr, 0, sizeof (E_nr));
	//int fdfdfd = sizeof (B_nr);
	
	while (high >= low)
	{
		j = strcmp (rcc.utf8, UTF2MARC [mid].utf8);
		if (j == 0)
		{
			// the utf's match, now we have to find the right charset
			
			rcc = UTF2MARC [mid];
			if (rcc.charset[0] == 'B')
				B_nr = rcc;
			else if (rcc.charset[0] == 'E')
				E_nr = rcc;
				
			// check charset and scan until a match found -- we have to scan in
			//		both directions
			i = 1;
			while ((rcc.charset [0] != c) && !strcmp (rcc.utf8, lookup))
			{
				// scan forwards
				rcc = UTF2MARC [mid + i];
				if (!strcmp (rcc.utf8, lookup))
				{
					if (rcc.charset[0] == 'B')
						B_nr = rcc;
					else if (rcc.charset[0] == 'E')
						E_nr = rcc;
				}
				i++;
			}
			if (!strcmp (rcc.utf8,lookup))	// found it
				return 1;
			else
			{
				//scan backwards
				rcc = UTF2MARC [mid];
				i = 1;
				while ((rcc.charset [0] != c) && !strcmp (rcc.utf8, lookup))
				{
					rcc = UTF2MARC [mid - i];
					if (!strcmp (rcc.utf8, lookup))
					{
						if (rcc.charset[0] == 'B')
							B_nr = rcc;
						else if (rcc.charset[0] == 'E')
							E_nr = rcc;
					}
					i++;
				}
				if (!strcmp (rcc.utf8, lookup))	// found it
					return 1;
				else		// can't find this character in the current charset
				{
					if (B_nr.charset[0])
						rcc = B_nr;
					else if (E_nr.charset[0])
						rcc = E_nr;
					else
						rcc = UTF2MARC [mid];
					return 1;
				}
			}
		}
		else if (j < 0)
			high = mid - 1;
		else
			low = mid + 1;
			
		mid = (high + low) / 2;
	}
	
	// not found in any charset in codetables, insert NCR value into MARC
	// -- set codetable to B because we want to report the NCR in basic latin
	// -- we use strncpy because we don't want to wipe out the 1st byte of cc.utf8
	// convert long to hex
	WString out;
	out.Sprintf ("%X", UtfToUnicodeNumber (rMessage, rcc.utf8));
	memset (rcc.marc, 0, sizeof(rcc.marc));
	strncpy (rcc.marc, out.GetText (), sizeof (rcc.marc) - 1);
	rcc.charset[0] = 'B';
	
	return 0;
}
//FindMarcEquivalent



//FindUtfEquivalent
int FindUtfEquivalent (WString& rMessage, charconvert_rt& rcc)
{
	// we have to get the UTF that is for the MARC code in the current codetable, which is the cc.charset
	
	int j;
	long ii;

	long high = sizeof (MARC2UTF) / sizeof (rcc);
	long low = 0;
	long mid = (high + low) / 2;
				
	
	while (high >= low)
	{
		j = strcmp(rcc.marc, MARC2UTF[mid].marc);
		
		if (j == 0)
		{
			// the utf's match, now we have to find the right charset
			// scan until a match found -- we have to scan in both directions

			// scan forwards
			ii = mid;
			while (!strcmp (rcc.marc, MARC2UTF[ii].marc))
			{
				// B and E are not explicitly indicated in record, so we consider a match like this:
				if 	(
						!strcmp(rcc.charset, MARC2UTF[ii].charset) ||
						((rcc.charset[0] == 'B') && (MARC2UTF[ii].charset[0] == 'E')) ||
						((rcc.charset[0] == 'E') && (MARC2UTF[ii].charset[0] == 'B'))
					)	
				{
					rcc = MARC2UTF[ii];
					return 1;
				}
				
				ii++;
			}
			
			// scan backwards
			ii = mid - 1;
			while (!strcmp (rcc.marc, MARC2UTF[ii].marc))
			{
				if 	(
						!strcmp(rcc.charset, MARC2UTF[ii].charset) ||
						((rcc.charset[0] == 'B') && (MARC2UTF[ii].charset[0] == 'E')) ||
						((rcc.charset[0] == 'E') && (MARC2UTF[ii].charset[0] == 'B'))
					)	
				{
					rcc = MARC2UTF[ii];
					return 1;
				}
				
				ii--;
			}
			
			// found char but wrong charset
			strcpy (rcc.utf8, UTF8_Missing_Symbol);
			return 0;
		}
			
		else if (j < 0)
			high = mid - 1;
		else
			low = mid + 1;
			
		mid = (high + low) / 2;
	}
	
	// not found in any charset in codetables, insert "missing" symbol
	strcpy (rcc.utf8, UTF8_Missing_Symbol);
	
	return 0;
}
//FindUtfEquivalent



//GetNextXMLField -- can't get leader this way because SP_Field gets only fields, and if this
//		gets leader it gets shook up
int GetNextXMLField ()
{
	int		Result = E_PERFECTSUCCESS;
	
    	memset (TheField, 0, sizeof (TheField)); 
	
	// EntryOffset is not an exact pointer as for MARC, just a place to start searching the record
	
	if (EntryOffset < 0)
		return E_UNEXPECTEDEOF;
		
	long i = strstr ((char const *) TheRecord + EntryOffset, "controlfield tag") - (char const *) TheRecord;
	long j = strstr ((char const *) TheRecord + EntryOffset, "datafield tag") - (char const *) TheRecord;
	long k = strstr ((char const *) TheRecord + EntryOffset, "record>") - (char const *) TheRecord;
	
	if ((i < 0) || (i > RecLength))
		i = zmaxreclen;
	if ((j < 0) || (j > RecLength))
		j = zmaxreclen;
	if ((k < 0) || (k > RecLength))
		return E_UNEXPECTEDEOF;
		
	long m = __min (i, __min (j, k));
	
	if (m == k)	// end of record
		return E_EXCEPTION;

	else if (m == i)	// control field
	{
		strncpy ((char*) TheTag, (char const*) TheRecord + i + 18, 3);
		
		j = strstr ((char const *) TheRecord + i, ">") - (char const *) TheRecord + 1;
		k = strstr ((char const *) TheRecord + i, "</") - (char const *) TheRecord;
		strncpy ((char*) TheField, (char const*) TheRecord + j, k - j);
		FieldLength = strlen ((char const*) TheField);
		EntryOffset = k + 10;		// any number to get past "</controlfield"
	}
	
	else		// m == j, data field
	{
		strncpy ((char*) TheTag, (char const*) TheRecord + j + 15, 3);

		// indicators
		strncpy ((char*) TheField, (char const*) TheRecord + j + 26, 1);
		strncat ((char*) TheField, (char const*) TheRecord + j + 35, 1);
		
		// each sf is on a sep line
		i = strstr ((char const *) TheRecord + j + 35, "subfield code") - (char const *) TheRecord;
		k = strstr ((char const *) TheRecord + j + 35, "datafield") - (char const *) TheRecord;
		while ((i > 0) && (k > 0) && (i < k))
		{
			TheField [strlen ((char const *) TheField)] = zmarcsubterm;
			strncat ((char*) TheField, (char const*) TheRecord + i + 15, 1);
			i = strstr ((char const *) TheRecord + i, ">") - (char const *) TheRecord + 1;
			k = strstr ((char const *) TheRecord + i, "</") - (char const *) TheRecord;
			if (k < 0)
				return E_UNEXPECTEDEOF;
			strncat ((char*) TheField, (char const*) TheRecord + i, k - i);
			
			i = strstr ((char const *) TheRecord + k, "subfield code") - (char const *) TheRecord;
			k = strstr ((char const *) TheRecord + k, "datafield") - (char const *) TheRecord;
		}
		
		FieldLength = strlen ((char const * ) TheField);
		EntryOffset = k + 10;		// any number to get past "</datafield"
	}
	
	return Result;
}
//GetNextXMLField



//GetOneMarcComb	-- returns number of bytes read
//060110 we use this routine to avoid an even more complex loop in ConvertAnsel
//		like the one we have in ConvertUtf8
//	-- this gets combining chars until one is found that is not
//	-- charsets not mentioned just fall thru and insert the base char
int GetOneMarcComb 
		(WString& rMessage, 
		char* thefield, 
		marcincomb_rt marccomb[],
		char* current_charset,
		char* prev_basic,
		char* prev_ext)
{
	int i = 0, mcindex = 0, bytes_read = 0;
	
	while (TRUE)
	{
		// handle escape sequence
		if (thefield[i] == MARCESCAPECODE)
		{
			if ((thefield[i+1] == 'b') || (thefield[i+1] == 'p') || (thefield[i+1] == 'g'))
			{
				*current_charset = thefield[i+1];
				bytes_read = 2;
			}
			else if (thefield[i+1] == 's')
			{
				// \s resets both
				*prev_basic = 'B';
				*prev_ext = 'E';
				
				// \s can be followed by an extended latin char
				if (thefield[i+2] <= 0x7f)
					*current_charset = 'B';
				else
					*current_charset = 'E';
				bytes_read = 2;
			}
			else if ((thefield[i+1] == '$') 
					&& (thefield[i+2] == ',')
					&& (thefield[i+2] == ')') 
					&& (thefield[i+2] == '-'))
			{
				*current_charset = thefield[i+3];
				bytes_read = 4;
			}
			else
			{
				*current_charset = thefield[i+2];
				bytes_read = 3;
			}
		}
		
		i += bytes_read;


		// determine whether we are using G0 or G1
		if (thefield[i] <= 0x7f)
		{
			if (bytes_read == 0)	// if we didn't just read a new charset ...
				*current_charset = 	*prev_basic;	// this switches between basic and ext cs's for the same language
			else
				*prev_basic = *current_charset;
		}
		else
		{
			if (bytes_read == 0)	// if we didn't just read a new charset ...
				*current_charset = 	*prev_ext;	// this switches between basic and ext cs's for the same language
			else
				*prev_ext = *current_charset;
		}
		
		bytes_read = 0;	// so when we loop thru we won't add it again
		
		
		// ready to get chars	
		if (*current_charset == '1')	// multibyte CJK, always 3 bytes
		{
			marccomb[mcindex].marc = thefield[i];
			marccomb[mcindex++].charset = '1';
			marccomb[mcindex].marc = thefield[i+1];
			marccomb[mcindex++].charset = '1';
			marccomb[mcindex].marc = thefield[i+2];
			marccomb[mcindex++].charset = '1';
			i += 3;
			break;
		}
		
		else if 		// these are the ranges of combining dias in each charset
				(	(	((*current_charset == '2') && (thefield[i] >= 0x40) && (thefield[i] <= 0x4e)) ||
						((*current_charset == '3') && (thefield[i] >= 0x6b) && (thefield[i] <= 0x72)) ||
						((*current_charset == '4') && (thefield[i] >= 0xfd) && (thefield[i] <= 0xfe)) ||
						((*current_charset == 'E') && (thefield[i] >= 0xe0) && (thefield[i] <= 0xfe)) ||
						((*current_charset == 'S') && (thefield[i] >= 0x21) && (thefield[i] <= 0x27))
					) &&
						(i < MAX_DIACRITICS)	// we haven't coded what to do if this happens
				)
			{
				marccomb[mcindex].marc = thefield[i];
				marccomb[mcindex].charset = *current_charset;
				mcindex++;
				i++;
			}
			
		else
		{
			marccomb[mcindex].marc = thefield[i];
			marccomb[mcindex].charset = *current_charset;
			i++;
			break;
		}
	}
	
	return i;
}
//GetOneMarcComb



//IsCombining
WBool IsCombining(charconvert_rt& rcc)
{
	/*060103 if char not found in codetables, we don't know whether it's
			combining or not. most of them are not, as they are decomposable
			chars, but some are chars that we make into precomposed MARC chars
			by using SubstitutePrecomposed. For these, we just hard-wire the 
			combining value here 
			
		-- default value is '0' */
			
	switch (rcc.charset[0])
	{
		case 'B':	// basic latin
		case 'E':	// extended latin
			if (!strcmp (rcc.marc, "31B"))
				rcc.combining[0] = '1';
			break;
			
		case '3':	// basic arabic	
			if (!strcmp (rcc.marc, "653") || !strcmp (rcc.marc, "654") || !strcmp (rcc.marc, "655"))
				rcc.combining[0] = '1';
			break;
			
		case '4':	// extended arabic	
			if (!strcmp (rcc.marc, "654"))
				rcc.combining[0] = '1';
			break;
	}
		
	return (rcc.combining[0] == '1');
}
//IsCombining



//LookupOneUtfChar -- return the len, negative if not in codetables
// third param is actually a pointer to the array of marcchars
int LookupOneUtfChar (WString& rMessage, char* lookup, charconvert_rt& rcc)
{
	// we want to get a MARC code matching the current charset, if poss, so look 
	//		at rcc.charset to see what the one was for the prev char
						
	WString 			temp, temp2;
	WBool 			found = FALSE;
	
	int utflen;
	
	if (lookup [0] >= 240)	// actually the MARC repetoire has no 4-byte or longer UTFs
		utflen = 4;
	else if (lookup [0] >= 224)
		utflen = 3;
	else if (lookup [0] >= 161)
		utflen = 2;
	else
		utflen = 1;

	//051226 put it into the cc
	memset (rcc.utf8, 0, sizeof (rcc.utf8));
	memcpy (rcc.utf8, lookup, utflen);

	// clear marc and combining -- we can't clear the entire rcc because the 
	//		charset of the prev char is in rcc.charset
	memset (rcc.marc, 0, sizeof (rcc.marc));
	rcc.combining[0] = '0';
	
	//051229 shortcircuit for 1-byte utfs
	if (utflen == 1)
	{
		rcc.marc[0] = rcc.utf8[0];
		rcc.charset[0] = 'B';
		//set above//rcc.combining[0] = '0';
		found = TRUE;
	}
	else
	{
		if (UseCodetables && FindMarcEquivalent(rMessage, rcc))		// returns 1 if found, 0 if not
		{
			found = TRUE;
		}
		
		if (!found)		// don't use codetables. or, used but not found there so try our precomposed Symbols
		{
			WString marc = LookupSymbol(rcc.utf8, UTF8, TOANSEL);
			if (marc != "-1")		// found something
			{
				if (UseCodetables)	// for others this is normal conversion
				{
					temp = UtfToUtfHex(rcc.utf8);
					temp2 = StringToHex(marc);
					temp.Sprintf("Warning: Converting %s to %s", temp.GetText(), temp2.GetText());
					ADDMESSAGE(temp);
				}
				strcpy(rcc.marc, marc.GetText ());
				found = TRUE;
			}
		}
	}
	
	if (found)
		return utflen;
	else
		return -utflen;
}
//LookupOneUtfChar



//LookupSymbol -- looks up in Symbols.cpp
WString LookupSymbol (char* lookup, yCharSet charset, yCharConvert target)
{
	WBool 	found = FALSE;
	WString	result;
	
	// can't use binary search on Symbols because they aren't in order
	for (int k = 0; k < ZNUMSYMS; k++)
	{
		if ((charset == ANSEL) && !strcmp (lookup, Symbols [k].ansel))
		{
			found = TRUE;
			break;
		}
		else if ((charset == LATIN) && !strcmp (lookup, Symbols [k].latin))
		{
			found = TRUE;
			break;
		}
		else if ((charset == UTF8) && !strcmp (lookup, Symbols [k].utf8))
		{
			found = TRUE;
			break;
		}
	}


	//060105 even tho found, there may not be a conversion in our table, which
	//		we want to report as "not found"
	if (found)
	{
		switch (target)
		{
			case TOANSEL:
				if (Symbols[k].ansel[0])
					result.SetText (Symbols[k].ansel);
				else
					found = 0;
				break;
				
			case TOLATIN:
				if (Symbols[k].latin[0])
					result.SetText (Symbols[k].latin);
				else
					found = 0;
				break;
			
			case TOPLAIN:	//060105 an exception -- it's legal to have a NULL result
				result.SetText (Symbols[k].plain);
				break;
			
			case TOUTF8:
				if (Symbols[k].utf8[0])
					result.SetText (Symbols[k].utf8);
				else
					found = 0;
				break;
		}
	}
	
	if (found)
		return result;	// can be NULL if TOPLAIN (but only for TOPLAIN)
	else
		return "-1";
}
//LookupSymbol



//MakeEscapeSequence
//060126 modified because no esc needed if merely switching between G0 and G1. that is, we can have
//		Greek basic with an extended latin in the middle with no escapes
WString MakeEscapeSequence(char current_cs, char* prev_basic_cs, char* prev_extended_cs)
{
	WString 	s;
	WBool	current_is_basic = TRUE;	// overridden below if nec

	if ((current_cs == 'E') || (current_cs == '4') || (current_cs == 'Q'))
		current_is_basic = FALSE;
		
	if ((current_is_basic && (current_cs == *prev_basic_cs)) 
			|| (!current_is_basic && (current_cs == *prev_extended_cs)))
		return s;	// null
	
	//051223 according to Gary Smith at OCLC, on the UNICODE-MARC listserv, only ANSEL uses G1,
	//		ASCII and all other use G0 only. Joan Aliprand, one of the Unicode
	//		editors, agrees.
	//	-- only EACC are multi-byte
	
	s.Clear();
	s.Concat (MARCESCAPECODE);
	
	if ((current_cs != 'b') && (current_cs != 'p') && (current_cs != 's') && (current_cs != 'g'))
			// these four have just the escape and charset
	{
		if (current_cs == '1')	// eacc -- multibyte G0
			s.Concat (0x24);
		else if ((current_cs == 'E') || (current_cs == 'Q') || (current_cs == '4'))	// single-byte G1 sets (latin, cyrillic, and arabic)
			s.Concat (0x29);
		else // -- single-byte G0
			s.Concat (0x28);
	}
	
	s.Concat (current_cs);
	
	// if the prev charset was b, p, or g, then shifting back to B requires \s, not \(B
	if ((current_cs == 'B') 
			&& ((*prev_basic_cs == 'b') || (*prev_basic_cs == 'p') || (*prev_basic_cs == 'g')))
	{
		s.Clear();
		s.Concat (MARCESCAPECODE);
		s.Concat ('s');
	}
	
	// update for caller
	if (current_is_basic)
		*prev_basic_cs = current_cs;
	else
		*prev_extended_cs = current_cs;
	
	return s;
}
//MakeEscapeSequence



//MakeLeaderIndex (for Statistics)
//handles leader bytes 5 - 9 and 17-19, with values of space, number, or lower-case letter
//creates 0-based index into 2-D array
int MakeLeaderIndex (int ldrbyte, int ldrvalue)
{
	int x, y;

	//which byte?
	if (ldrbyte < 17)
		x = ldrbyte - 5;
	else
		x = ldrbyte - 12;
		
	//which value?	
	if (ldrvalue == 32)
		y = 0;				//byte 0 of the results array
		
	//check range of digits explicitly
	else if ((ldrvalue >= 48) && (ldrvalue <= 57))
		y = ldrvalue - 47;	//because digits are in bytes 1..10 of the results array
		
	//check range of digits explicitly
	else if ((ldrvalue >= 97) && (ldrvalue <= 122))
		y = ldrvalue - 86;	//because letters are in bytes 11..36 of the results array
		
	//all other values ignored (not legal in MARC)
	
	return (x * 37) + y;	//8 rows of 37 values
}
//MakeLeaderIndex



//OutputMarcChar -- helper for ConvertToAnsel ONLY!!
int OutputMarcChar 
		(WString& rMessage, 
		char mc, 
		char charset, 
		char* prev_basic, 
		char* prev_ext, 
		yCharConvert target, 
		long& roffset,
		int displaycodes)
{
	int Result = E_PERFECTSUCCESS;
	WString temp;
	
	// check for illegal chars -- could add for other charsets also
		
	if 
		(
			(
				(charset == 'B') || (charset == 'E') || (charset == 's')
			)
			&& 
			(
				(mc < 0x1b)
				||  (mc == 0x1c)
				||  ((mc >= 0x7f) && (mc <= 0x87))
				||  ((mc >= 0x8a) && (mc <= 0x8c))
				||  ((mc >= 0x8f) && (mc <= 0xa0))
				
				//060313 not sure why these weren't here before
				||  (mc == 0xaf)
				||  (mc == 0xbb)
				||  (mc == 0xbe)
				||  (mc == 0xbf)
				||  ((mc >= 0xc9) && (mc <= 0xdf))
				||  (mc == 0xfc)
				||  (mc == 0xfd)
				||  (mc == 0xff)
				//060313 end
			)
		)
	{
		temp.Sprintf("Problem: Illegal MARC-8 character %02X, codetable %c", mc, charset);
		ADDMESSAGE(temp);
		if (target == TOUTF8)
		{
			if (UseCodetables)
			{
				strcpy((char*) Work + roffset, UTF8_Missing_Symbol);
				roffset += strlen(UTF8_Missing_Symbol);
			}
			// for other clients, wait and see
		}
		else if ((target == TOLATIN) && displaycodes)
		{
			Work[roffset++] = 0x1c;	// our symbol for "cannot display"
		}
		// for TOPLAIN do nothing
		// for TOUNIMARC do nothing
		
		Result = E_CONVERSION;
	}
	else
	{
		if ((target == TOLATIN) && displaycodes)	// we want to show esc seq
		{
			temp = MakeEscapeSequence(charset, prev_basic, prev_ext);
			strcpy((char*) Work + roffset, temp.GetText());
			roffset += temp.GetLength();
		}
		Work[roffset++] = mc;
	}
	
	return Result;
}
//OutputMarcChar



//ReadRecord
int ReadRecord (WString& rMessage, long offset)
{
	int		c, i;
	long		len;
	int		Result;
	char		temp [6];

    	assert (f);
	fseek (f, offset, SEEK_SET);
	
    	memset (temp, 0, sizeof (temp)); 
	
	if (bUseOffsetArray)		// then we have an exact offset for the record
		fgets (temp, 6, f);	// fgets ok here because if EOL found, it's an error anyway
	else
	{
		// array of offsets not built -- we have just position of character after previous record, but
		//	need to scan for extra chars
		
		c = 'a';	//initialize
		while (!isdigit (c) && !feof (f))
		{
			c = fgetc (f);
			offset++;
		}
		
		if (feof (f))
			return E_EXCEPTION;	//this is a normal end of file
			
		offset--;
		temp [0] = (char) c;
		for (i = 1; i < 5; i++)
			temp [i] = (char) fgetc (f);
	}

	if (strlen (temp) < 5)	 //should this be if feof?
	{
		// test to see what caused this
		fseek (f, offset, SEEK_SET);
		c = fgetc (f);
		if (c == -1)	//eof
			return E_UNEXPECTEDEOF;
		else
			return E_EXCEPTION;	//050128 don't believe this can happen
	}
	

	RecLength = atol (temp);

	if (RecLength == 0)
    		return E_NONMARC;
	
	fseek (f, offset, SEEK_SET);

    	//961107 fgets stops at fldterm at end of dir
	//fgets (TheRecord, RecLength, f);
	for (len = 0; len < RecLength; len++)
    {
		c = fgetc (f);
		if (c < 0)	//eof
			return E_UNEXPECTEDEOF;
		TheRecord [len] = c;
	}
	TheRecord [len] = 0;


	//991204 if result of ReadRecord is <0, doesn't come thru here
	#ifdef _DEBUG
	//Result = SP_ValidateRecord (CheckIllegalChars, __FILE__, __LINE__);
	Result = SP_ValidateRecord (rMessage, CheckIllegalChars, __FILE__, __LINE__);
	#else
	//Result = SP_ValidateRecord (CheckIllegalChars);
	Result = SP_ValidateRecord (rMessage, CheckIllegalChars);
	#endif
	
	
	//991215 moved from above to be after skipping extra characters. MV uses this,
	//			and it is returned to Super and Export also, so keep for all versions
	RecordOffset = offset;	
	
	return Result;
}
//ReadRecord



//040720 ReadXMLFile
int ReadXMLFile ()
{
	int 		Result = E_PERFECTSUCCESS;
	char		buffer [9999];
	char*	marker;
	char*	tagstart;
	long		filepos, recoffset, nextread;
	
	// keep in mind that tags can be prefixed, such as <marc:record
	
	// line endings should be irrelevant. should handle multiple tags on a line,
	//		end-tags on same or different lines, etc.
	
	// no assumption that record is <= 99,999 bytes
	
	// assumes that "record" or ":record" would not start a line, but would always have the
	//		"<" or "<marc" on the same line
	
	// this only finds the start of each record, in contrast to ReadXMLRecord which acquires the full record
	while (!feof (f))
	{
		while (!feof (f) && !strstr (buffer, "<record") && !strstr (buffer, ":record"))
			fgets (buffer, 9999, f);	
		if (feof (f))
			break;
		marker = strstr (buffer, "<record");
		if (!marker)
			marker = strstr (buffer, ":record");
		tagstart = marker;
		while (*tagstart != '<')
			tagstart--;
		if (*(tagstart + 1) != '/')	//don't process endtag
		{
			RecCount++;
			marker = strstr (marker, ">");
			filepos = ftell (f);
			recoffset = filepos - strlen (buffer) + (tagstart - buffer);
			
			// for testing -- messes up marker
			//fseek (f, recoffset, SEEK_SET);
			//fgets (buffer, 20, f);
			
			nextread = recoffset + (marker - buffer);
			
			// for testing
			//fseek (f, nextread, SEEK_SET);
			//fgets (buffer, 20, f);
			
			pRecArray [RecCount] = recoffset;		// i.e. the starting position of <record or <marc:record
			fseek (f, nextread, SEEK_SET);	// start of next element
		}
		fgets (buffer, 9999, f);
	}

	//050127 no longer since we don't use nextoffset
	//to read last record, we need to fill array for lastrecord+1 with filelen
	//fseek (f, 0, SEEK_END);
	//pRecArray [RecCount + 1] = ftell (f);
	
	return Result;
}
//ReadXMLFile



//ReadXMLRecord
int ReadXMLRecord (long offset)
{
	int 		Result = E_PERFECTSUCCESS;
	char		buffer [9999];
	char*	marker;
	char*	tagstart;
	char*	slash;
	int		bFirstline = 1;
	int		OK = 0;
	int		j;
	
	//we assume that each buffer is on a separate line so we can use fgets
	
	//keep in mind that tags can be prefixed, such as <marc:record
	
    	memset (TheRecord, 0, sizeof (TheRecord)); 
    	memset (buffer, 0, sizeof (buffer)); 
	
	fseek (f, offset, SEEK_SET);

	/*050127 rewritten so as not to use nextoffset
	while (jj < nextoffset)
	{
		fgets (buffer, 9999, f);
		jj += strlen (buffer);
		
		//040803 truncate at CRLF or LF
		if (buffer [strlen (buffer) - 2] == 13)
			buffer [strlen (buffer) - 2] = 0;		// CRLF
		else
			buffer [strlen (buffer) - 1] = 0;		// LF
		
		//strip leading blanks and tabs -- not sure why!
		i = 0;
		while ((buffer [i] == 9) || (buffer [i] == 32))
			i++;
			
		j = strlen (buffer) - i;
		memmove (buffer, buffer + i, j + 1);	//plus 1 for the \0
		
		strcat ((char*) TheRecord, buffer);
	}
	*/
	
	// keep in mind that tags can be prefixed, such as <marc:record
	
	// line endings should be irrelevant. should handle multiple tags on a line,
	//		end-tags on same or different lines, etc.
	
	// no assumption that record is <= 99,999 bytes
	
	// assumes that "record" or ":record" would not start a line, but would always have the
	//		"<" or "<marc" on the same line

	fgets (buffer, 9999, f);
	
	if (!bUseOffsetArray)		// then <record> may be preceded by other stuff
	{
		while (TRUE)
		{
			marker = strstr (buffer, "<record");
			if (!marker)
				marker = strstr (buffer, ":record");
			if (marker)
			{
				tagstart = marker;
				while (*tagstart != '<')
					tagstart--;
				
				j = strlen (buffer);
				memmove (buffer, tagstart, j + 1);	//plus 1 for the \0
				break;
			}
			
			fgets (buffer, 9999, f);
		}
	}
	
	while (TRUE)
	{
		marker = strstr (buffer, "</record>");
		if (marker)
			OK = 1;
			
		else
		{
			// if this is the first line of this record (excluding lines before
			//		the <record> tag), it starts with "<xxx:record" and we
			//		have to skip over that to find the </xxx:record>
			if (bFirstline)
				marker = strstr (buffer + 20, ":record>");
			else
				marker = strstr (buffer, ":record>");

			if (marker)
			{
				slash = marker;
				while (*slash != '/')
					slash--;
				tagstart = marker;
				while (*tagstart != '<')
					tagstart--;
				
				// reality check
				if (slash - tagstart == 1)		// not sure what to do if not!
					OK = 1;
			}
		}
		
		if (OK)
		{
			// move to end of </xxx:record>
			while (*marker != '>')
				marker++;
		
			*(marker + 1) = 0;	// terminate after "</xxx:record>"
			strcat ((char*) TheRecord, buffer);	// store record so far and go on to next read
			break;
		}
		else
		{
			bFirstline = 0;
			strcat ((char*) TheRecord, buffer);	// store record so far and go on to next read
			fgets (buffer, 9999, f);
		}
	}


	//040721 we do not attempt to validate the XML record as we do for MARC
	//		-- in fact we assume the file is valid XML -- perhaps this should be checked later

	//991215 moved from above to be after skipping extra characters. MV uses this,
	//			and it is returned to Super and Export also, so keep for all versions
	RecordOffset = offset;	
	RecLength = strlen ((char const *) TheRecord);
	
	return Result;
}
//ReadXMLRecord



//StringToHex -- convert a string to its hex representation
WString StringToHex(WString in)
{
	WString out;
	int i, j;
	char buffer [20];
	memset(buffer, 0, sizeof (buffer));
	
	i = in.GetLength();
	for (j = 0; j < i; j++)
	{
		/*
		if (in.GetCharacter(j) < 16)
			buffer[(2*j) - 1] = '0';
		itoa (in.GetCharacter(j), buffer + (2*j), 16);
		*/
		sprintf(buffer + (2*j), "%02X", (char) in.GetCharacter(j));
	}
	strupr (buffer);
	
	out.SetText(buffer);
	return out;
}
//StringToHex



//SubstituteGuts
WBool SubstituteGuts (marcoutcomb_rt marcout[], int mcindex, char orig, char* modifier, char subst)
{
	int 	found = 0;
	int	i, j;
	
	// see if base char is one too look for
	if (marcout[0].marc[0] == orig)
	{
		// we have to loop because the modifier may not be adjacent to the base char
		for (i = 0; i < mcindex; i++)
		{
			if (!strcmp (marcout[i].marc, modifier))
			{
				found = 1;
				break;
			}
		}
		
		if (found)
		{
			// replace base char, keep same charset
			marcout[0].marc[0] = subst; 
			
			// move all following dias down one to overwrite the one replaced
			//	-- this also copies the first blank one over the last filled one,
			//			thus nulling it
			//	-- not sure what will happen if we have 10 modifiers
			for (j = i; j < mcindex; j++)
			{
				strcpy(marcout[j].marc, marcout[j+1].marc);
				marcout[j].charset[0] = marcout[j+1].charset[0];
				if (j == MAX_DIACRITICS - 1)
					break;
			}
		}
	}
	
	return found;
}
//SubstituteGuts



//SubstitutePrecomposed -- looks thru the marcout array and replaces with any MARC-8 precomposed
//		chars such as hook-o. marcout contains one letter and all its modifiers.
//	-- this will fail if the NCRs become added to MARC Codetables, in which
//		case we will need to look for the UTF8s instead
WBool SubstitutePrecomposed (marcoutcomb_rt marcout[], int mcindex)
{
	WBool found = FALSE;
	
	// look at charset of base char
	switch (marcout[0].charset[0])
	{
		case 'B':	// basic latin
		case 's':	// likewise
			// params are plain MARC-8 letter, dia, and pre-composed substitute
			found = found || SubstituteGuts (marcout, mcindex, 'O', "&#x31B;", 0xac);
			found = found || SubstituteGuts (marcout, mcindex, 'U', "&#x31B;", 0xad);
			found = found || SubstituteGuts (marcout, mcindex, 'o', "&#x31B;", 0xbc);
			found = found || SubstituteGuts (marcout, mcindex, 'u', "&#x31B;", 0xbd);
			break;
			
		case '3':	// basic arabic	
			found = found || SubstituteGuts (marcout, mcindex, 0x47, "&#x653;", 0x42);
			found = found || SubstituteGuts (marcout, mcindex, 0x47, "&#x654;", 0x43);
			found = found || SubstituteGuts (marcout, mcindex, 0x68, "&#x654;", 0x44);
			found = found || SubstituteGuts (marcout, mcindex, 0x47, "&#x655;", 0x45);
			found = found || SubstituteGuts (marcout, mcindex, 0x6a, "&#x654;", 0x46);
			break;
			
		case '4':	// extended arabic	
			found = found || SubstituteGuts (marcout, mcindex, 0x67, "&#x654;", 0x6e);
			found = found || SubstituteGuts (marcout, mcindex, 0xf7, "&#x654;", 0x78);
			break;
	}
	
	return found;	// TRUE if any replacement
}
//SubstitutePrecomposed



//UnicodeToUtf
WString UnicodeToUtf(WString in)
{
	char		ncr [7];
	memset(ncr, 0, sizeof(ncr));
	
	long decimal = ConvertHexToLong((char*) in.GetText());
	
	if (decimal <= 127)
		ncr[0] = (char) decimal;
	else if (decimal <= 2047)
	{
		ncr[0] = 192 + (int) (decimal / 64);
		ncr[1] = 128 + (int) (decimal % 64);
	}
	else if (decimal <= 65535L)
	{
		ncr[0] = 224 + (int) (decimal / 4096);
		ncr[1] = 128 + (int) ((decimal / 64) % 64);
		ncr[2] = 128 + (int) (decimal % 64);
	}
	else if (decimal <= 2097151L)
	{
		ncr[0] = 240 + (int) (decimal / 262144L);
		ncr[1] = 128 + (int) ((decimal / 4096) % 64);
		ncr[2] = 128 + (int) ((decimal / 64) % 64);
		ncr[3] = 128 + (int) (decimal % 64);
	}
	else if (decimal <= 67108863L)
	{
		ncr[0] = 248 + (int) (decimal / 16777216L);
		ncr[1] = 128 + (int) ((decimal / 262144L) % 64);
		ncr[2] = 128 + (int) ((decimal / 4096) % 64);
		ncr[3] = 128 + (int) ((decimal / 64) % 64);
		ncr[4] = 128 + (int) (decimal % 64);
	}
	else if (decimal <= 2147483647L)
	{
		ncr[0] = 252 + (int) (decimal / 1073741824L);
		ncr[1] = 128 + (int) ((decimal / 16777216L) % 64);
		ncr[2] = 128 + (int) ((decimal / 262144L) % 64);
		ncr[3] = 128 + (int) ((decimal / 4096) % 64);
		ncr[4] = 128 + (int) ((decimal / 64) % 64);
		ncr[5] = 128 + (int) (decimal % 64);
	}
    /* here's the algorithm used above -- www1.tip.nl/~t876506/utf8tbl.html

Start with the Unicode number expressed as a decimal number and call this ud.

If ud <128 (7F hex) then UTF-8 is 1 byte long, the value of ud.

If ud >=128 and <=2047 (7FF hex) then UTF-8 is 2 bytes long.
   byte 1 = 192 + (ud div 64)
   byte 2 = 128 + (ud mod 64)

If ud >=2048 and <=65535 (FFFF hex) then UTF-8 is 3 bytes long.
   byte 1 = 224 + (ud div 4096)
   byte 2 = 128 + ((ud div 64) mod 64)
   byte 3 = 128 + (ud mod 64)

If ud >=65536 and <=2097151 (1FFFFF hex) then UTF-8 is 4 bytes long.
   byte 1 = 240 + (ud div 262144)
   byte 2 = 128 + ((ud div 4096) mod 64)
   byte 3 = 128 + ((ud div 64) mod 64)
   byte 4 = 128 + (ud mod 64)

If ud >=2097152 and <=67108863 (3FFFFFF hex) then UTF-8 is 5 bytes long.
   byte 1 = 248 + (ud div 16777216)
   byte 2 = 128 + ((ud div 262144) mod 64)
   byte 3 = 128 + ((ud div 4096) mod 64)
   byte 4 = 128 + ((ud div 64) mod 64)
   byte 5 = 128 + (ud mod 64)

If ud >=67108864 and <=2147483647 (7FFFFFFF hex) then UTF-8 is 6 bytes long.
   byte 1 = 252 + (ud div 1073741824)
   byte 2 = 128 + ((ud div 16777216) mod 64)
   byte 3 = 128 + ((ud div 262144) mod 64)
   byte 4 = 128 + ((ud div 4096) mod 64)
   byte 5 = 128 + ((ud div 64) mod 64)
   byte 6 = 128 + (ud mod 64)

The operation div means integer division and mod means the rest after integer division.
For positive numbers a div b = int(a/b) and a mod b = (a/b-int(a/b))*b.
UTF-8 sequences of 4 bytes and longer are at the moment not supported by the regular browsers.
The highest character position currently (Unicode 3.2) defined is number 10FFFF hex (1114111 dec) in a 'private use' area. The highest character with an actual glyph is number E007F hex (917631 dec), the CANCEL TAG character.


	*/
	
	WString t;
	t.SetText(ncr);
	
	return t;
}
//UnicodeToUtf



//UtfToUtfHex
WString UtfToUtfHex (char* utf)
{
	WString s, t;
	for (int i = 0; i < strlen (utf); i++)
	{
		s.Sprintf("%X ", utf[i]);
		t += s;
	}
	
	t.Trim();
	
	return t;
}
//UtfToUtfHex



//UtfToUnicode
WString UtfToUnicode (WString& rMessage, char* utf)
{
	WString s;
	s.Sprintf("%X", UtfToUnicodeNumber(rMessage, utf));
	return s;
}
//UtfToUnicode



//UtfToUnicodeNumber -- unlike the other conversion routines, this does not convert a field 
//		but only a single character, for error-reporting purposes. It converts UTF-8 to the 
//		decimal value of the Unicode
long UtfToUnicodeNumber (WString& rMessage, char* in)
{
	int len = strlen (in);
	long unicode;
	WString temp;		// debugging
	switch (len)	// we're assuming a well-formed UTF-8
	{
		case 1:	// untested
			unicode = in [0]; 
			temp.Sprintf ("UtfToUnicodeNumber length 1: %s in, %lu out", in, unicode);
			ADDDEBUGMESSAGE(temp.GetText());
			break;
		case 2:
			unicode = 
				(64 * (in [0] - 192)) + 
				(in [1] - 128);
			break;
		case 3:
			unicode = 
				(4096 * (in [0] - 224)) + 
				(64 * (in [1] - 128)) + 
				(in [2] - 128);
			break;
		case 4:  // untested
			unicode = 
				(262144 * (in [0] - 240)) + 
				(4096 * (in [1] - 128)) + 
				(64 * (in [2] - 128)) + 
				(in [3] - 128);
			temp.Sprintf ("UtfToUnicodeNumber length 4: %s in, %lu out", in, unicode);
			ADDDEBUGMESSAGE(temp.GetText());
			break;
		case 5: // untested 
			unicode = 
				(16777216 * (in [0] - 248)) + 
				(262144 * (in [1] - 128)) + 
				(4096 * (in [2] - 128)) + 
				(64 * (in [3] - 128)) + 
				(in [4] - 128);
			temp.Sprintf ("UtfToUnicodeNumber length 5: %s in, %lu out", in, unicode);
			ADDDEBUGMESSAGE(temp.GetText());
			break;
		case 6: // untested
			unicode = 
				(1073741824 * (in [0] - 252)) + 
				(16777216 * (in [1] - 128)) + 
				(262144 * (in [2] - 128)) + 
				(4096 * (in [3] - 128)) + 
				(64 * (in [4] - 128)) + 
				(in [5] - 128);
			temp.Sprintf ("UtfToUnicodeNumber length 6: %s in, %lu out", in, unicode);
			ADDDEBUGMESSAGE(temp.GetText());
			break;
	}
	
	
    /* here's the algorithm used above -- www1.tip.nl/~t876506/utf8tbl.html

If z is between and including 0 - 127, then there is 1 byte z. The decimal Unicode value ud = the value of z.

If z is between and including 192 - 223, then there are 2 bytes z y; ud = (z-192)*64 + (y-128)

If z is between and including 224 - 239, then there are 3 bytes z y x; ud = (z-224)*4096 + (y-128)*64 + (x-128)

If z is between and including 240 - 247, then there are 4 bytes z y x w; ud = (z-240)*262144 + (y-128)*4096 + (x-128)*64 + (w-128)

If z is between and including 248 - 251, then there are 5 bytes z y x w v; ud = (z-248)*16777216 + (y-128)*262144 + (x-128)*4096 + (w-128)*64 + (v-128)

If z is 252 or 253, then there are 6 bytes z y x w v u; ud = (z-252)*1073741824 + (y-128)*16777216 + (x-128)*262144 + (w-128)*4096 + (v-128)*64 + (u-128)

If z = 254 or 255 then there is something wrong!

Example: take the decimal Unicode designation 8482 (decimal), which is for the trademark sign. This number is larger than 2048, so we get three numbers.
The first number is 224 + (8482 div 4096) = 224 + 2 = 226.
The second number is 128 + (8482 div 64) mod 64) = 128 + (132 mod 64) = 128 + 4 = 132.
The third number is 128 + (8482 mod 64) = 128 + 34 = 162.
Now the other way round. We see the numbers 226, 132 and 162. What is the decimal Unicode value?
In this case: (226-224)*4096+(132-128)*64+(162-128) = 8482.
*/

	/*051229 we used to return the hex value
	out.Sprintf ("%X", unicode);
	return out;
	*/
	return unicode;
}
//UtfToUnicodeNumber



//ValidateTag
//called by SP_Field to ensure its caller sent valid parameter
//validates tag with or without a subfield code
//tests length, numericalness
//later, ensure tag is valid, ensure sfc is valid
//970204 converts X to x
//970205 worried about long ptrs, trying short
//060118 return various codes depending on severity (used to return 0 for all problems)
//060118 add allowx. x allowed or not depending on whether we are validating a tag in the
//			SP_Field params, or a tag in the record
int ValidateTag (char* pts, int allowx)
{
	if (strlen (pts) != 3)
		return E_NONMARC;
		
	//060118 this prevents finding illegal uppercase sfc's, but we need it to lowercase
	//		any x's. change control in calling loop to fix this
	strlwr (pts);	
	
	if (allowx)
	{
		if (strspnp (pts, "0123456789xX") == NULL)
			return E_PERFECTSUCCESS;
		else
		{
			// found something besides 0..9, x, or X
			
			if (strspnp (pts, "0123456789abcdefghijklmnopqrstuvwxyzX") == NULL)
			{
				// user prefs allow numeric tags, ok, otherwise warn
				if (Preferences.nonnumerictags == 1)
					return E_PERFECTSUCCESS;
				else
					return E_GENERAL;
			}
			else
			{
				// we found something besides 0..9, a..z, or X -- always illegal
				return E_NONMARC;
			}
		}
	}
	else
	{
		if (strspnp (pts, "0123456789") == NULL)
			return E_PERFECTSUCCESS;
		else
		{
			// found something besides 0..9
			
			if (strspnp (pts, "0123456789abcdefghijklmnopqrstuvwxyz") == NULL)
			{
				// user prefs allow numeric tags, ok, otherwise warn
				if (Preferences.nonnumerictags == 1)
					return E_PERFECTSUCCESS;
				else
					return E_GENERAL;
			}
			else
			{
				// we found something besides 0..9, a..z -- always illegal
				return E_NONMARC;
			}
		}
	}

	//return E_PERFECTSUCCESS;
}
//ValidateTag



//////////////////////// MEMORY COVER FUNCTIONS ///////////////////


//bMalloc
int bMalloc (void **ppv, unsigned long size)
{
	BYTE **ppb = (BYTE**) ppv;
    assert ((ppv != NULL) && (size != 0));
	*ppb = (BYTE*) malloc (size);
	
	/*991218
	if (*ppb != NULL)
		MEMSET(*ppb,MallocGarbage,size);
	*/
	
	//ADDPTR(*ppb,size);	980613
	return (*ppb != NULL);
}
//bMalloc


//991219 no longer called by sp_marc, but still used by sp_val
//bRealloc
int bRealloc (void **ppv, unsigned long newsize)
{
	BYTE** ppb = (BYTE**) ppv;
	BYTE *pbNew;
	#ifdef _DEBUG
	/*980613
	unsigned long oldsize;
	assert ((ppb != NULL) && (newsize != 0));
	oldsize = GETSIZE(*ppb);
	//if shrinking, fill tail
	if (newsize < oldsize)
    	memset ((*ppb) + newsize, ReallocGarbage, oldsize - newsize);
	*/
	#endif
	pbNew = (BYTE*) realloc (*ppb, newsize);
	if (pbNew != NULL)
    {
		#ifdef _DEBUG
		/*980613
		//if expanding, fill tail
		if (newsize > oldsize)
        	memset (pbNew + oldsize, ReallocGarbage, newsize - oldsize);
		//update memarray
		CHGMEM(*ppb,pbNew,newsize);
		*/
		#endif
		*ppb = pbNew;
    }
    return (pbNew != NULL);
}
//bRealloc


//vFree -- differs from others in that checks pointer itself
void vFree (void *pv)
{
#ifdef _DEBUG
	//int		i;	980613
#endif
	//assert (pv != NULL);
	if (pv)
    {
		#ifdef _DEBUG
		/*980613
		i = GETSIZE(pv);	980613
		memset (pv, FreeGarbage, i);
		DELMEM(pv);
		*/
		#endif
		free (pv);
    }
}
//bFree


//////////////////////// EXPORTED FUNCTIONS ///////////////////////



//SP_CloseFile
//970821 need to close viewing file before exporting, and export file
//	before viewing again
void WDLLEXPORT SP_CloseFile ()
{
	fclose (f);
}
//SP_CloseFile



//SP_ConvertAnsel
// converts MARC-8 codes to UTF8 for the 0-terminated string pointed in *ppold
//		(normally a field or subfield), and returns address of the new 0-terminated string in *ppold
//	-- WARNING -- result is NOT a marc record, since lengths are changed but directory not updated
//060104 added rMessage, removed rbadchar
int WDLLEXPORT SP_ConvertAnsel 
		(WString& rMessage, 
		LPPUSTR ppold, 
		long& rlen, 
		yCharConvert target, 
		int displaycodes)
{
	if ((target != TOLATIN) && (target != TOPLAIN) && (target != TOUNIMARC) && (target != TOUTF8))
		return E_CONVERSION;
	
	int 			Result = E_PERFECTSUCCESS;
	int			temp_result;
	int			i, j, k, bytes_read;
	char			tofind [2];
	
	WString		targetstring, temp, temp2;
	char			curr_basic_cs = 'B';		// basic latin
	char			curr_extended_cs = 'E';	// extended latin
	char 		current_charset = 'B';	// basic latin
		
	char			letterset [MAX_DIACRITICS+2];		//the letter plus all its dias. 
										//	we store these because the dias have
										//	to be output AFTER the letter in UTF8
	int			letterset_chunk;
	
	marcincomb_rt	marcin[MAX_DIACRITICS+1];	// stores a base char and all its dias, plus their charsets
	WBool		hetero;					// true if all charsets for a combination are the same
	
	WBool		found;
	
	BYTE* pold = (BYTE*) *ppold;	//easier to work with
	long poldlen = strlen ((char*) pold);
	
	long oo = 0;	//pold index
	long nn = 0;	//Work index
	long& rnn = nn;

	char aUNIMARC [256] =
	{//		0	1	2	3	4	5	6	7	8	9	a	b	c	d	e	f
	/*0*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/*1*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	29, 30,  31,
	/*2*/	' ',	'!','\"','#','$',	'%', '&','\'','(',')','*','+', ',', '-','.',	'/', 
	/*3*/	'0',	'1',	'2',	'3',	'4','5', '6',	'7',	'8',	'9',	':',	';',	'<',	'=',	'>',	'?', 
	/*4*/	'@',	'A',	'B',	'C',	'D',	'E',	'F',	'G',	'H',	'I',	'J',	'K',	'L',	'M',	'N',	'O',
	/*5*/	'P',	'Q',	'R',	'S',	'T',	'U',	'V',	'W',	'X',	'Y',	'Z',	'[',	'\\',']','^', '_', 
	/*6*/	96,	'a',	'b',	'c',	'd',	'e',	'f',	'g',	'h',	'i',	'j',	'k',	'l',	'm',	'n',	'o', 
	/*7*/	'p',	'q',	'r',	's',	't',	'u',	'v',	'w',	'x',	'y',	'z',	'{',	'|',	'}',	126,	0,
	/*8*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/*9*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/*a*/	0,	232,	233,	226,	236,	225,	234,	189,	183,	172,	175,	0,	0,	0,	177,	0,
	/*b*/	176,	248,	249,	242,	252,	241,	250,	190,	245,	163,	243,	0,	0,	0,	0,	0,
	/*c*/	0,	0,	174,	173,	188,	191,	161,	251,	0,	0,	0,	0,	0,	0,	0,	0,
	/*d*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/*e*/	0,	193,	194,	195,	196, 197,	198,	199,	200,	207,	202,	221,	222,	203,	205,	0,
	/*f*/	208,	211,	214,	215,	212,	217,	216,	210,	209,	213,	221,	223,	0,	0,	204,	0};
	
	/*060312 notes on Ansel-to-UNIMARC conversion
			1. detection of the following should produce a warning, as we have not mapped 
					them yet -- for those without UNIMARC equivs maybe we should substitute
					nearest equivalent:
							1b, 8d, 8e, ab, c0, c1, c8
			2. the following illegal characters should produce warnings, regardless of
					the values in the map shown above:
							1c, 7f - 87, 8a - 8c, 8f - a0, af, bb, be, bf, c9 - df,
							e0, fc, fd, ff
			3. the following ansel codes convert to two bytes in UNIMARC and need to be
					coded as exceptions:
							ac -> ce 4f
							ad -> ce 55
							bc -> ce 6f
							bd -> ce 75
							ef -> c7 c6
			4. dbl lig 1st half and dbl tilde 1st half have the same UNIMARC code
	*/
	
	char		unimarc_latch = ' ';
	
	charconvert_rt	cc;
	charconvert_rt& rcc = cc;
	WString temp_es;


	// MAIN LOOP
	
	while ((oo < poldlen) && (nn < zmaxreclen))
	{
		
		// ASCII CHARACTERS -- the normal case
		
		//060131 this shouldn't apply when using E
		if 
			(
				(current_charset == 'B')
				&& 
				(
					(
						(target != TOUNIMARC)
						&& (pold[oo] < LOWESTANSEL) 
						&& (pold[oo] != MARCESCAPECODE)
					)
					||
					(
						(target == TOUNIMARC)
						&& (pold[oo] < LOWESTANSEL) 
						&& (pold[oo] != MARCESCAPECODE)
						&& (pold[oo] != 0x8d)	// not yet ...
						&& (pold[oo] != 0x8e)	// ... handled, see doc
					)
				)
			)
		{
			// this also checks for illegal MARC-8 chars
			#ifdef _DEBUG
			char ccc = pold[oo];
			#endif
			temp_result = OutputMarcChar 
							(rMessage, 
							pold[oo], 
							current_charset, 
							&curr_basic_cs, 
							&curr_extended_cs, 
							target, 
							rnn,
							displaycodes);
			if (temp_result != E_PERFECTSUCCESS)
				Result = temp_result;
			oo++;
		}
		
	
		// ALL OTHER CHARS FOR ALL TARGET CHARSETS
		
		else
		{
			// get a base char and all its modifiers -- updates last 4 params
			memset(marcin, 0, sizeof(marcin));
			bytes_read = GetOneMarcComb 
							(rMessage, 
							(char*) pold + oo, 
							marcin, 
							&current_charset,
							&curr_basic_cs,
							&curr_extended_cs);
			oo += bytes_read;
			
			
			// we now have a set of dias followed by a base char
			// current_charset is that of the base char
			
			
			//060127 shortcut if the field or sf ends in a combining dia -- that is,
			//		if the base char is 1e or 1f
			i = MAX_DIACRITICS;
			while (!marcin[i].marc)
				i--;		// finding last char
				
			// see if there are hanging dias at end of fld or sf
			if ((i > 0) && ((marcin[i].marc == 0x1e) || (marcin[i].marc == 0x1f)))
			{
				// remove the dias
				char c = marcin[i].marc;
				memset(marcin, 0, sizeof(marcin));
				marcin[0].marc = c;
				marcin[0].charset = 'B';
				
				temp.Sprintf("Warning: Program removed diacritics modifying %02X", c);
				ADDMESSAGE(temp);
				
				//061103 this should just be a warning
				//Result = E_CONVERSION;
			}			

			
			// marcin consists of an array of records, where each record consists
			//		of one char and its charset
			//	-- now we need to get the chars into a contiguous string
			hetero = TRUE;	// whether letterset is all Bs or all Es, or is it mixed
			memset(letterset, 0, sizeof(letterset));
			for (i = 0; i <= MAX_DIACRITICS; i++)
			{
				if (!marcin[i].marc)
					break;
				letterset[i] = marcin[i].marc;
				
				// see if the chars in the comb have varying charsets (except B & E are OK)
				if (marcin[i].charset != current_charset) 
				{
					if (((marcin[i].charset == 'B') && (current_charset == 'E'))
							|| ((marcin[i].charset == 'E') && (current_charset == 'B')))
						hetero = TRUE;
					else
						hetero = FALSE;
				}
			}			
			
			
			// READY TO CONVERT A BASE CHAR AND ITS MODIFIERS
			
			if ((target == TOLATIN) || (target == TOPLAIN))
			{
				// for TOLATIN and TOPLAIN, we convert only the closest modifier
				
				if (((current_charset == 'B') || (current_charset == 'E')) && hetero)
				{
					i = strlen(letterset) - 1;		// index of base char
					found = FALSE;
					
					if ((i > 0) && (target == TOLATIN))		// base char has modifiers
					{
						// look up the letter and its closest dia as a combination -- ignore any dias except the last
						targetstring = LookupSymbol((char*)letterset + i - 1, ANSEL, target);
						if (targetstring != "-1")
							found = TRUE;
						else
							temp = StringToHex(letterset + i - 1);	// for error msg
					}
					
					// either we didn't find the 2-byte lookup,
					//		or we have only 1 byte or it's TOPLAIN and we haven't looked yet 
					if (!found)
					{
						if (letterset[i] >= LOWESTANSEL)
						{
							// look for the character by itself
							targetstring = LookupSymbol((char*) letterset + i, ANSEL, target);
							if (targetstring != "-1")
								found = TRUE;
							else
								temp = StringToHex(letterset + i);	// for error msg
						}
						else
						{
							targetstring.SetText((char*) letterset + i);	// just use the base char
							found = TRUE;
						}
					}
					
					if (found)
					{
						strcpy((char*) Work + nn, targetstring.GetText());
						nn += targetstring.GetLength();
					}
					else
					{
						ADDDEBUGMESSAGE("Next error message not tested");
						temp.Sprintf("Problem: MARC-8 %s not found in conversion tables, codetable %c", temp.GetText(), current_charset);
						ADDMESSAGE(temp);
						Result = E_CONVERSION;
					}
				}	// B or E
				
				else if ((current_charset == 'b') || (current_charset == 'p'))
				{
					// for sub/superscripts just output the char
					temp_result = OutputMarcChar 
									(rMessage, 
									letterset[0], 
									current_charset, 
									&curr_basic_cs, 
									&curr_extended_cs, 
									target, 
									rnn,
									displaycodes);
					if (temp_result != E_PERFECTSUCCESS)
						Result = temp_result;
				}
				
				else		// not B or E or b or p
				{
					if (target == TOLATIN)
					{
						if (displaycodes)
							Work[nn++] = 0x1c;	// our symbol for "cannot display"
						else
						{
							ADDDEBUGMESSAGE("Next error message not tested");
							temp = StringToHex(letterset);
							temp.Sprintf("Problem: MARC-8 %s not found in conversion tables, codetable %c", temp.GetText(), current_charset);
							ADDMESSAGE(temp);
							Result = E_CONVERSION;
						}
					}
					// for TOPLAIN do nothing
				}
			}
			
			
			// TOUNIMARC
		
			else if (target == TOUNIMARC)
			{
				if ((current_charset != 'B') && (current_charset != 'E')
						&& (current_charset != 'b') && (current_charset != 'p'))
				{
					ADDDEBUGMESSAGE("Next error message not tested");
					temp.Sprintf("Problem: Cannot convert UNIMARC characters in codetable %c", current_charset);
					ADDMESSAGE(temp);
					Result = E_CONVERSION;
					return Result;
				}
				
				// sub- and superscripts
				else if (current_charset == 'b')
				{
					//sent out 8b and then the char
					Work[nn] = 0x8b;
					nn++;
					Work[nn] = marcin[0].marc;
					nn++;
					unimarc_latch = current_charset;
				}
				else if (current_charset == 'p')
				{
					//sent out 8c and then the char
					Work[nn] = 0x8c;
					nn++;
					Work[nn] = marcin[0].marc;
					nn++;
					unimarc_latch = current_charset;
				}
				else if (unimarc_latch != ' ')
				{
					if (unimarc_latch == 'b')
						Work[nn] = 0x8c;
					else
						Work[nn] = 0x8b;
					nn++;
					oo--;	// so we pick up the next char in the main loop. we can't just output
							//		it here bec it may need translation
					unimarc_latch = ' ';
				}
				
				// characters we don't handle yet
				else if ((pold[oo] == 0x8d)
						|| (pold[oo] == 0x8e)
						|| (pold[oo] == 0xab)
						|| (pold[oo] == 0xc0)
						|| (pold[oo] == 0xc1)
						|| (pold[oo] == 0xc8))
				{
					temp.Sprintf("Problem: Unhandled UNIMARC character %X", pold[oo]);
					ADDMESSAGE(temp);
					Result = E_CONVERSION;
				}
				
				// characters that become two UNIMARC chars
				else if (pold[oo] == 0xac)
				{
					Work[nn] = 0xce;
					nn++;
					Work[nn] = 0x4f;
					nn++;
				}
				else if (pold[oo] == 0xad)
				{
					Work[nn] = 0xce;
					nn++;
					Work[nn] = 0x55;
					nn++;
				}
				else if (pold[oo] == 0xbc)
				{
					Work[nn] = 0xce;
					nn++;
					Work[nn] = 0x6f;
					nn++;
				}
				else if (pold[oo] == 0xbd)
				{
					Work[nn] = 0xce;
					nn++;
					Work[nn] = 0x75;
					nn++;
				}
				else if (pold[oo] == 0xef)
				{
					Work[nn] = 0xc7;
					nn++;
					Work[nn] = 0xc6;
					nn++;
				}
				
				// chars that use the conversion map above
				else
				{
					// we loop thru letterset to do our 1:1 conversion
					for (i = 0; i < sizeof(letterset); i++)
					{
						if (!letterset[i])
							break;
						Work[nn] = aUNIMARC[letterset[i]];
						nn++;
					}
				}
			}
		
		
			// FOR TOUTF8 WE HAVE TO LOOK AT ALL MODIFIERS, NOT JUST THE LAST AS WE DID FOR TOLATIN
			
			else if (target == TOUTF8)
			{
				found = FALSE;
				i = strlen(letterset) - 1;	// index of base char
				
				if (UseCodetables)
				{
					// LOOK FIRST IN CODETABLES
					
					// we want to look up the base char and its two closest modifiers* together to get 
					//		precomposed UTF if available. if not, then the base char and the one
					//		closest modifier. if that isn't found either, then look them all up
					//		separately
					// 	* this is because codetables' longest MARC-8 is 3 bytes
					
					rcc.charset[0] = current_charset;
					
					if ((strlen(letterset) > 1) && hetero)
					{
						if (strlen(letterset) > 2)	// if there are two or more modifiers ...
						{
							letterset_chunk = 3;
							memset(rcc.marc, 0, sizeof(rcc.marc));
							strcpy(rcc.marc, letterset + strlen(letterset) - letterset_chunk);
							
							if (FindUtfEquivalent(rMessage, rcc))
							{
								found = TRUE;
								strcpy((char*) Work + nn, rcc.utf8);
								nn += strlen(rcc.utf8);
							}
						}
						
						if (!found)	// if base char and two modifiers not found ...
						{
							// ... look for base char and one modifier
							letterset_chunk = 2;
							memset(rcc.marc, 0, sizeof(rcc.marc));
							strcpy(rcc.marc, letterset + strlen(letterset) - letterset_chunk);
							
							if (FindUtfEquivalent(rMessage, rcc))
							{
								found = TRUE;
								strcpy((char*) Work + nn, rcc.utf8);
								nn += strlen(rcc.utf8);
							}
						}
					}
					
					if (!found)
						letterset_chunk = 0;	// letterset_chunk is the size of the chunk found; if 0,
											//		we'll lookup the base char and all modifiers separately
					
					// this also looks up any modifiers before the letterset_chunk
					found = TRUE;
					memset(&cc, 0, sizeof(cc));
					for (k = strlen(letterset) - letterset_chunk - 1; k >= 0; k--)	// from base char backwards
					{
						rcc.marc[0] = letterset[k];	// lookup up single chars
						rcc.charset[0] = marcin[k].charset;
						
						if (FindUtfEquivalent (rMessage, rcc))
						{
							strcpy ((char*) Work + nn, rcc.utf8);
							nn += strlen (rcc.utf8);
						}
						else
						{
							found = FALSE;
							break;	// no point in trying to output the symbol with only some of its modifiers,
									//		so we look for them in Symbols instead
						}
					}
				}
				
				
				/*071001 if client uses codetables, don't bother looking in symbols
				// 1. CLIENT DOESN'T USE CODETABLES
				// or
				// 2. NOT FOUND IN CODETABLES, LOOK IN SYMBOLS
				if (!found && ((current_charset == 'B') || (current_charset == 'E')) && hetero)*/
				else
				
				{
					// first look up base char with its closest single modifier -- or could be a
					//		single-byte MARC-8 if cli dn use codetables
					
					#if 0 // refactored and corrected below
					// i is index of base char
					//070925 i could be 0; if so, it resulted in looking up the char BEFORE letterset :-(
					if (i < 1)
						i = 1;
						
					targetstring = LookupSymbol ((char*) letterset + i - 1, ANSEL, target);
					
					if (targetstring == "-1")
						k = i;	// to look at all symbols separately
					else
					{
						strcpy ((char*) Work + nn, (char*) targetstring.GetText());
						nn += targetstring.GetLength();
						k = i - 2;	// to look at any symbols except the one or two we just converted
						found = TRUE;
						
						if (UseCodetables)	// not of interest otherwise
						{
							temp = StringToHex(letterset + i - 1);
							temp2 = StringToHex(letterset);
							temp.Sprintf("Letterset %s not found in Codetables, code %s found in Symbols, codetable %c", 
									temp2.GetText(), temp.GetText(), current_charset);
							ADDDEBUGMESSAGE(temp.GetText());
						}
					}
					
					
					// convert the base char and then the dias in reverse order
					//070930 for a single-byte illegal MARC-8 char, there is only 1 byte in letterset,
					//		so no point in doing this
					if (strlen(letterset) > 1)
					{
						for (j = k; j >= 0; j--)
						{
							memset(tofind, 0, sizeof(tofind));
							tofind[0] = letterset[j];
							
							/*070925 LookupSymbol was causing errors because if the base char was a normal alpha it
									wasn't found, causing a spurious error. for example, z-acute not in table,
									so here we look up char seply, and the z was nf in Symbols. */
							if (isprint(tofind[0]))
							{
								Work[nn] = tofind[0];
								nn++;
							}
							else
							{
								targetstring = LookupSymbol((char*) tofind, ANSEL, target);
								if (targetstring == "-1")
								{
									found = FALSE;
									break;
								}
								else
								{
									strcpy ((char*) Work + nn, (char*) targetstring.GetText ());
									nn += targetstring.GetLength ();
								
									if (UseCodetables)	// not of interest otherwise
									{
										temp = StringToHex(tofind);
										temp2 = StringToHex(letterset);
										temp.Sprintf("Letterset %s not found in Codetables, code %s found in Symbols, codetable %c", 
												temp2.GetText(), temp.GetText(), current_charset);
										ADDDEBUGMESSAGE(temp.GetText());
									}
								}
							}
						}
					}
					#endif
					//071001 refactored and corrected
					// easier to handle single ANSELs separately
					if (strlen(letterset) == 1)
					{
						targetstring = LookupSymbol((char*) letterset, ANSEL, target);
						
						if (targetstring != "-1")
						{
							Work[nn] = letterset[0];
							nn++;
							found = TRUE;
						}
					}
					else
					{
						// first look up base char with its closest single modifier
						// i is index of base char
						targetstring = LookupSymbol ((char*) letterset + i - 1, ANSEL, target);
						
						if (targetstring == "-1")
						{
							k = i;	// to look at all symbols separately
							found = FALSE;
						}
						else
						{
							strcpy ((char*) Work + nn, (char*) targetstring.GetText());
							nn += targetstring.GetLength();
							k = i - 2;	// to look at any symbols except the two we just converted
							found = TRUE;	// if there are only 2 chars this wb the final setting
						}
					
					
						// convert any not so far converted one at a time, in reverse order
						// found wb TRUE if base char and nearest dia were found, FALSE otherwise
						for (j = k; j >= 0; j--)
						{
							found = TRUE;
							memset(tofind, 0, sizeof(tofind));
							tofind[0] = letterset[j];
							
							/*070925 LookupSymbol was causing errors because if the base char was a normal alpha it
									wasn't found, causing a spurious error. for example, z-acute not in table,
									so here we look up char seply, and the z was nf in Symbols. */
							if (isprint(tofind[0]))
							{
								Work[nn] = tofind[0];
								nn++;
							}
							else
							{
								targetstring = LookupSymbol((char*) tofind, ANSEL, target);
								if (targetstring == "-1")
								{
									found = FALSE;
									break;
								}
								else
								{
									strcpy ((char*) Work + nn, (char*) targetstring.GetText ());
									nn += targetstring.GetLength ();
								
									if (UseCodetables)	// not of interest otherwise
									{
										temp = StringToHex(tofind);
										temp2 = StringToHex(letterset);
										temp.Sprintf("Letterset %s not found in Codetables, code %s found in Symbols, codetable %c", 
												temp2.GetText(), temp.GetText(), current_charset);
										ADDDEBUGMESSAGE(temp.GetText());
									}
								}
							}
						}
					}
					
					
					
					//070925 if not using codetables we need to revert to charset B or the next 
					//		char will be treated as a non-ascii
					//	-- may be ok even if using codetables
					if (!UseCodetables)
						current_charset = 'B';
				}	// B or E
				
				
				// NOT FOUND IN EITHER PLACE
				
				if (!found)
				{
					strcpy ((char*) Work + nn, UTF8_Missing_Symbol);
					nn += strlen(UTF8_Missing_Symbol);
					temp = StringToHex(letterset);
					temp.Sprintf("Problem: MARC-8 %s not found in conversion tables, codetable %c", temp.GetText(), current_charset);
					ADDMESSAGE(temp);
					Result = E_CONVERSION;
				}
			}	// TOUTF8
		}	// not an ASCII char
	}
						

	// for AAA and CCC, look for NCRs and convert to UTF-8
	if (UseCodetables)
	{
		WString s, t;

		Work[nn] = 0;		//because not memsetting before we start
		s.SetText((char const*) Work);
		int i, j;
		i = s.Position("&#x");
		while (i != NOT_FOUND)
		{
			j = s.Position(";", i);
			t = UnicodeToUtf(s.Substring(i + 3, j - i - 3));	// 10 is the longest possible NCR
			s = s.Substring(0, i) + t + s.Substring(j+1);
			i = s.Position("&#x");
		}
		i = s.GetLength();
		strcpy((char*) Work, (char*) s.GetText());
		Work[i] = 0;
		nn = strlen((char*) Work);
	}


	//060531 if switch is on, don't use single-dia form of double ligature and tilde, use 2-dia form
	if (ForceDoubleDias)
	{
		// double ligature -- is BETWEEN the two letters. replace with separate dias AFTER each letter
		if (strstr((char const*) Work, "\xcd\xa1"))
		{
			WString s;
			s.SetText((char const*) Work);
			i = s.Position("\xcd\xa1");
			while (i != NOT_FOUND)
			{
				s = s.Substring(0, i) + "\xef\xb8\xa0" + s.Substring(i + 2, 1) + "\xef\xb8\xa1" + s.Substring(i + 3);
				strcpy((char*) Work, (char*) s.GetText());
				nn += 4;
				i = s.Position("\xcd\xa1");
			}
		}
		// double tilde -- is BETWEEN the two letters. replace with separate dias AFTER each letter
		if (strstr((char const*) Work, "\xcd\xa0"))		// not tested
		{
			WString s;
			s.SetText((char const*) Work);
			i = s.Position("\xcd\xa0");
			while (i != NOT_FOUND)
			{
				s = s.Substring(0, i) + "\xef\xb8\xa2" + s.Substring(i + 2, 1) + "\xef\xb8\xa3" + s.Substring(i + 3);
				strcpy((char*) Work, (char*) s.GetText());
				nn += 4;
				i = s.Position("\xcd\xa0");
			}
		}
	}
	
	
	if (nn >= zmaxreclen)
	{
		ADDDEBUGMESSAGE("Next error message not tested");
		ADDMESSAGE("Problem: Record longer than 99,999 characters");
		return E_CONVERSION;
	}

	Work[nn] = 0;	//because not memsetting before we start
	
	
	// displaycodes are only for display, and we always display in latin
	if ((target == TOLATIN) && displaycodes)
		ConvertCodes (Work);	//return code not checked
	

	//return results
	*ppold = Work;	//because Work will go out of scope
	rlen = nn;
	
	return Result;
}
//SP_ConvertAnsel



//SP_ConvertLatin
//060104 adding rMessage, removing rbadchar
int WDLLEXPORT SP_ConvertLatin (WString& rMessage, LPPUSTR ppold, long& rlen, yCharConvert target, int displaycodes)
{
	if ((target != TOANSEL) && (target != TOLATIN) && (target != TOPLAIN))	// others not coded yet
		return E_CONVERSION;

	int Result = E_PERFECTSUCCESS;
	
	WString temp;
		
	if ((target == TOANSEL) || (target == TOPLAIN))
	{
		// symbols not found are NOT transferred to output
		//********* warning, does not process characters 128 - 159 which Windows uses for additional characters ****
		
		long				nn, oo;
		char				tofind [2];
		tofind [1] = 0;	// required by LookupSymbol
		WString			targetstring;
		//int				j;
		
		char*			pold = (char*) *ppold;	//simplifies work
		
		nn = 0;	//pnew index
		oo = 0;	//pold index
		
		while ((oo < strlen ((char*) pold)) && (nn < zmaxreclen))
		{
			if (pold[oo] > 159)	//128-159 are undefined in Latin-1 (some are used by Windows tho)
			{
				tofind [0] = pold [oo++];
				targetstring = LookupSymbol ((char*) tofind, LATIN, target);
				if (targetstring == "-1")
				{
					//rbadchar = tofind [0];
					temp.Sprintf("Latin-1 %c not found in conversion tables", tofind[0]);
					ADDMESSAGE(temp);
					Result = E_CONVERSION;
				}
				else
				{
					/*061004 simpler
					for (j = 0; j < targetstring.GetLength (); j++)
						Work [nn + j] = (BYTE) targetstring.GetCharacter (j);
					nn += targetstring.GetLength ();
					*/
					strcpy((char*) Work + nn, targetstring.GetText());
					nn += targetstring.GetLength();
				}
			}
			
			//normal characters
			else
				Work [nn++] = pold [oo++];
		}
	
		if (oo < strlen ((char*) pold))
		{
			//rbadchar = -2;
			ADDDEBUGMESSAGE("Next error message not tested");
			ADDMESSAGE("Problem: Record longer than 99,999 characters");
			return E_CONVERSION;
		}
	
		Work[nn] = 0;
		
		//return results
		*ppold = Work;
		rlen = nn;
	}
	
	else if (target == TOLATIN)
	{
		// all we do here is copy the input to Work and then optionally convertcodes
		// purpose is to convert sfc and ft to double dagger and para
		
		/*050208 this caused a GPF
		_fmemcpy (Work, *ppold, _fstrlen ((char*) *ppold));
		Work [_fstrlen ((char*) *ppold)] = 0;*/
		char* pold = (char*) *ppold;
		memcpy (Work, pold, strlen (pold));
		Work [strlen (pold)] = 0;
	
		// displaycodes are only for display, and we always display in latin
		if ((target == TOLATIN) && displaycodes)
			ConvertCodes (Work);	//return code not checked
	
		//return results
		*ppold = Work;	//because Work will go out of scope
	}
	
	
	#ifdef _DEBUG
	// compare with old sub
	if ((target == TOANSEL) || (target == TOLATIN))
	{
		int i2;
		if (target == TOANSEL)
			i2 = SP_LatinToAnsel (ppold, rlen);
		else if (target == TOLATIN)
			i2 = SP_LatinToLatin (ppold);
		
		int i3 = strlen ((char const*) debugWork);
		int i4 = strlen ((char const*) Work);
		if ((Result != i2) || (i3 != i4))
			DEBUGGERBREAK;
		for (int i5 = 0; i5 < strlen ((char const*) debugWork); i5++)
		{
			if (debugWork [i5] != Work [i5])
				DEBUGGERBREAK;
				break;
		}
	}
	#endif
		
	return Result;
}
//SP_ConvertLatin



//SP_ConvertUnimarc
//translates UNIMARC codes to latin-1 or plain for the 0-terminated string pointed in *ppold
//(normally a field or subfield), and returns address of the new 0-terminated string in *ppold
//WARNING -- result is NOT a marc record, since lengths are changed but directory not updated
//	-- this translates only codes found in Latin1, removes other UNIMARC codes
//060104 adding rMessage removing rbadchar (which was never used)
int WDLLEXPORT SP_ConvertUnimarc (WString& rMessage, LPPUSTR ppold, long& rlen, yCharConvert target, int displaycodes)
{
	if ((target != TOLATIN) && (target != TOPLAIN))	// others not coded yet
		return E_CONVERSION;
	
	int			bfound;
	int			k, m;
	long			nn, oo;
	BYTE*		pold;
	BYTE			tofind [2];
	int			Result = E_PERFECTSUCCESS;

	pold = (BYTE*) *ppold;	//easier to work with
	long poldlen = strlen ((char*) pold);
	
	oo = 0;	//pold index
	nn = 0;	//pnew index

	//060327 8b and 8c now convert to 1c (were 0's) -- sub/superscript indicators
	char aLatin [256] =
	{//		0	1	2	3	4	5	6	7	8	9	a	b	c	d	e	f
	/*0*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/*1*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	27,	0,	29, 30,	31,
	/*2*/	' ',	'!','\"','#','$',	'%', '&','\'','(',')','*','+', ',', '-','.',	'/', 
	/*3*/	'0',	'1',	'2',	'3',	'4','5', '6',	'7',	'8',	'9',	':',	';',	'<',	'=',	'>',	'?', 
	/*4*/	'@',	'A',	'B',	'C',	'D',	'E',	'F',	'G',	'H',	'I',	'J',	'K',	'L',	'M',	'N',	'O',
	/*5*/	'P',	'Q',	'R',	'S',	'T',	'U',	'V',	'W',	'X',	'Y',	'Z',	'[',	'\\',']','^', '_', 
	/*6*/	96,	'a',	'b',	'c',	'd',	'e',	'f',	'g',	'h',	'i',	'j',	'k',	'l',	'm',	'n',	'o', 
	/*7*/	'p',	'q',	'r',	's',	't',	'u',	'v',	'w',	'x',	'y',	'z',	'{',	'|',	'}',	0,	0,
	/*8*/	0,	0,	0,	0,	0,	0,	0,	0,	136,	137,	0,	28,	28,	0,	0,	0,
	/*9*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/*a*/	0,	'¡','\"','£',	'$',	'¥',	'†',	'¶',	'´','\'','\"','\"',172,'©','p',	'®',
	/*b*/	'‘',	'’','\'',0,	0,	0,	'‡',	'·',	'”','\'','\"','\"','#','’','”',	'¿',
	/*c*/	0,	193,	194,	195,	196,	0,	0,	0,	200,	201,	202,	0,	0,	0,	206,	0,
	/*d*/	208,	0,	0,	0,	0,	0,	0,	0,	0,	0,	'|',	0,	0,	0,	0,	0,
	/*e*/	0,	'Æ',	'Ð',	0,	0,	0,	230,	0,	'L',	'Ø',	'Œ',	0,	'Þ',	0,	0,	0,
	/*f*/	0,	'æ',	'd',	'ð',	0,	'i',	246,	0,	'l',	'ø',	'œ',	251,	'þ',	0,	0,	0};

	char aPlain [256] =
	{//		0	1	2	3	4	5	6	7	8	9	a	b	c	d	e	f
	/*0*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/*1*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	27,	0,	29, 30,	31,
	/*2*/	' ',	'!','\"','#','$',	'%', '&','\'','(',')','*','+', ',', '-','.',	'/', 
	/*3*/	'0',	'1',	'2',	'3',	'4','5', '6',	'7',	'8',	'9',	':',	';',	'<',	'=',	'>',	'?', 
	/*4*/	'@',	'A',	'B',	'C',	'D',	'E',	'F',	'G',	'H',	'I',	'J',	'K',	'L',	'M',	'N',	'O',
	/*5*/	'P',	'Q',	'R',	'S',	'T',	'U',	'V',	'W',	'X',	'Y',	'Z',	'[',	'\\',']','^', '_', 
	/*6*/	96,	'a',	'b',	'c',	'd',	'e',	'f',	'g',	'h',	'i',	'j',	'k',	'l',	'm',	'n',	'o', 
	/*7*/	'p',	'q',	'r',	's',	't',	'u',	'v',	'w',	'x',	'y',	'z',	'{',	'|',	'}',	0,	0,
	/*8*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/*9*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/*a*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/*b*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/*c*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/*d*/	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/*e*/	0,	225,	'D',	0,	0,	0,	230,	0,	'L',	'O',	234,	0,	236,	0,	0,	0,
	/*f*/	0,	241,	'd',	'd',	0,	'i',	246,	0,	'l',	'o',	250,	251,	252,	0,	0,	0};


	while ((oo < poldlen) && (nn < zmaxreclen))
	{
		//characters to ignore
		if (((target == TOLATIN) && (aLatin [pold[oo]] == 0)) ||
				((target == TOPLAIN) && (aPlain [pold[oo]] == 0)))
		{
			//do nothing
		}

		//escape code -- greek chars -- same for both latin and plain
		else if (pold[oo] == MARCESCAPECODE)
		{
			if (pold [oo + 1] == 125)	//start of Greek
			{
				oo += 2;	//now at first Greek char
				while ((pold[oo] != MARCESCAPECODE) && (pold [oo + 1] != 124))	//escape marking end of Greek
				{
					Work[nn] = pold[oo] - 184;	//supposed to convert E1 (225) to 'a' (61), etc.
					nn++;
					oo++;
				}
			}
			//else ignore escape code
		}

		//chars that need special processing
		/*
		else if (
		{
		}
		*/
		
		//chars to look up in symbol table (2- or more -byte symbols
		else if ((pold[oo] == 96) ||
				(pold[oo] == 126) ||
				(pold[oo] == 172) ||
				((pold[oo] >= 192) && (pold[oo] <= 223)) ||
				(pold[oo] == 230) ||
				(pold[oo] == 246) ||
				(pold[oo] == 251))
		{
			//skip over dias except last
			while ((pold [oo+1] == 96) ||
					(pold [oo+1] == 126) ||
					(pold [oo+1] == 172) ||
					((pold [oo+1] >= 192) && (pold[oo] <= 223)) ||
					(pold [oo+1] == 230) ||
					(pold [oo+1] == 246) ||
					(pold [oo+1] == 251))
				oo++;
			
			
			//if nonspacing diacritic precedes non-ascii char, ignore the former. always for plain
			//060331 this can't do what it says it does!
			if ((target == TOPLAIN) || (pold [oo+1] >= 160))
			{
				if (target == TOLATIN)
					Work[nn] = aLatin [pold[oo]];
				else
					Work[nn] = aPlain [pold[oo]];
				nn++;
			}
			else
			{
				tofind [0] = pold[oo];
				tofind [1] = pold [oo+1];
				oo++;
				
				//unimarc seems to have two graves, we don't make a distinction
				if (tofind [0] == 96)
					tofind [0] = 193;
				
				//likewise both a dieresis and an umlaut
				if (tofind [0] == 200)
					tofind [0] = 201;
				
				//now look them up and replace (TOLATIN only)
				bfound = 0;
				for (k = 0; k < ZNUMSYMS; k++)
				{
					if ((tofind [0] == Symbols[k].unimarc [0]) &&
						(tofind [1] == Symbols[k].unimarc [1]))
					{
						m = 0;
						while (Symbols[k].latin[m])
						{
							Work[nn] = Symbols[k].latin[m];
							nn++;
							m++;
						}
						bfound = 1;
						break;
					}
				}
				
				if (bfound == 0)	
				{					
					Work[nn] = pold[oo];	//oo has already been incremented to the letter byte
					nn++;
				}
			}
		}
		
		//chars to look up in array
		else
		{
			if (target == TOLATIN)
				Work[nn] = aLatin [pold[oo]];
			else
				Work[nn] = aPlain [pold[oo]];
			nn++;
		}
		
		oo++;		//for all types of chars
	}
	
	
	if (oo < poldlen)
		return E_CONVERSION;

	Work[nn] = 0;	//because not memsetting before we start

	if ((target == TOLATIN) && displaycodes)
		ConvertCodes (Work);	//return code not checked

	//return results
	*ppold = Work;	//because Work will go out of scope
	rlen = nn;
	
	return Result;
}
//SP_ConvertUnimarc



/*	//debugging: writes a file showing Symbols.cpp -- this will be run for each field unless we stop it
	FILE*	fout;
	int		j, m;
	if ((fout = fopen ("symboltable.txt", "w")) != NULL)
	{
		fputs ("  #   Ansel       Latin     Plain     Unimarc     UTF-8\n", fout);
		for (int k = 0; k < ZNUMSYMS; k++)
		{
			fprintf (fout, "%3d   ", k);
			
			m = 0;
			while (Symbols [k].ansel [m])
			{
				fprintf (fout, "%3d ", Symbols [k].ansel [m]);
				m++;
			}
			for (j = m; j < 3; j++)
				fputs ("    ", fout);
			
			m = 0;
			while (Symbols [k].latin [m])
			{
				fprintf (fout, "%c", Symbols [k].latin [m]);
				m++;
			}
			for (j = m; j < 10; j++)
				fputs (" ", fout);
			
			m = 0;
			while (Symbols [k].plain [m])
			{
				fprintf (fout, "%c", Symbols [k].plain [m]);
				m++;
			}
			for (j = m; j < 10; j++)
				fputs (" ", fout);
			
			m = 0;
			while (Symbols [k].unimarc [m])
			{
				fprintf (fout, "%3d ", Symbols [k].unimarc [m]);
				m++;
			}
			for (j = m; j < 3; j++)
				fputs ("    ", fout);
			
			m = 0;
			while (Symbols [k].utf8 [m])
			{
				fprintf (fout, "%3d ", Symbols [k].utf8 [m]);
				m++;
			}
			
			fputc ('\n', fout);
		}

		fclose (fout);
	}
	* /
	/ *
	// for utf to marc conversion
	FILE*	futf2marc;
	futf2marc = fopen ("t_utf2marc.txt", "a");
	
	// for output of conversion
	FILE*	futfout;
	futfout = fopen ("t_utfconvert.txt", "a");
*/



//SP_ConvertUTF8
//translates UTF-8 codes to MARC-8 equivalents for the 0-terminated string pointed in *ppold
//		and returns address of the new 0-terminated string in *ppold
//WARNING -- result is NOT a marc record, since lengths are changed but directory not updated
// ****************** WARNING: this does not yet handle conversion of unnormalized unicodes to 
//		Latin (see doc) *************************
//051231 rMessage replaced rbadchar as way to report problems
int WDLLEXPORT SP_ConvertUTF8 (WString& rMessage, LPPUSTR ppold, long& rlen, yCharConvert target, int displaycodes)
{
	if ((target != TOANSEL) && (target != TOLATIN) && (target != TOPLAIN))
		return E_CONVERSION;
	
	int				Result = E_PERFECTSUCCESS;
	char*			pold = (char*) *ppold;
	long 			poldlen = strlen ((char*) pold);
	long				nn = 0, oo = 0;	// nn is Work index, oo is pold index
	WString			temp;
	


	if (UseCodetables && (target == TOANSEL))
	{
		/* 	as we convert each utf we store the marc code in "output", then copy the
			marc codes in reverse order to Work. this is because unicode modifiers
			trail the base char but in marc they precede it
				
			strangely, a marc modifier can be in a different charset from its base character,
			so we have to insert the marc "escapesequence" (if necessary) into "output" ahead 
			of each marc code */
		
		marcoutcomb_rt	marcout[MAX_DIACRITICS+1];	// this allows a letter to have 10 un-precomposed modifiers
		int				mcindex = 0;
		char				curr_basic_cs = 'B';		// always true at start of field
		char				curr_extended_cs = 'E';	// always true at start of field
		int				utflen = 0;
		charconvert_rt 	cc;
		charconvert_rt&	rcc = cc;
		WBool 			iscombining;
		int				i;
		WBool			doublelig=FALSE, doubletilde=FALSE;	// if found after a letter, the NEXT letter needs a dia inserted
		WString			decompresults;
		WString&			rdecompresults = decompresults;
		WString			decompcharsets;
		WString&			rdecompcharsets = decompcharsets;
		WString			ncr_message;
		WString			this_es;

		//060120 for certain punctuation (non-combining) the correct charset is ambiguous until
		//		we see the next character. e.g. we don't want an opening quote mark to be in
		//		arabic if the following char is greek. this says we found one
		char				punc_on_hold = 0;
		
	
		// GET FIRST CHAR IN FIELD
		
		// -- we pass a charset so we know which charset to use for the next char, if it's 
		//		ambiguous. therefore we have to preload the charset for the first character
		//	-- we know it's basic latin because that is required for the sfc
		memset (&cc, 0, sizeof(cc));
		cc.charset[0] = 'B';		// basic latin
		memset (marcout, 0, sizeof(marcout));
		mcindex = 0;
			
			
		// lookup first unicode -- NOT a base char plus dias, just a single Unicode
		utflen = LookupOneUtfChar(rMessage, pold + oo, rcc);
		
		
		// MAIN LOOP
		
		while ((oo < poldlen) && (nn < zmaxreclen))
		{
			
			// CONVERT A BASE CHARACTER AND ANY TRAILING MODIFIERS
			
			// we convert the base char into first position of output, then any trailing modifiers.
			//		when all modifiers for a base char are in output (which we know when we find the
			//		next non-combining symbol), we break and output them in reverse order
			//	-- there is a \0 after each symbol
			
			memset(marcout, 0, sizeof(marcout));
			mcindex = 0;
					
			do
			{
				// ADD THE MARC CHAR TO THE MARCOUT ARRAY
				
				if (utflen > 0)
				{
					strcpy (marcout[mcindex].marc, cc.marc);
					strcpy (marcout[mcindex].charset, cc.charset);
					mcindex++;
				}
				else if (FindDecomp (rMessage, rcc, rdecompresults, rdecompcharsets))
				{
					WStringArray codes = decompresults.Parse("\x1f", TRUE, NULL);
					#ifdef _DEBUG
					temp.Sprintf("-- Decomposition results, %d strings", codes.GetCount());
					ADDDEBUGMESSAGE(temp);
					for (i = 0; i < codes.GetCount(); i++)
						ADDDEBUGMESSAGE(codes[i]);
					#endif
					WStringArray sets = decompcharsets.Parse();
			
					for (i = 0; i < codes.GetCount (); i++)
					{
						strcpy (marcout[mcindex].marc, codes[i].GetText());
						marcout[mcindex].charset[0] = (char) sets[i].GetCharacter(0);
						mcindex++;
					}
					
					#ifdef _DEBUG
					ADDDEBUGMESSAGE("-- Used decomposed characters");
					temp = "output characters: ";
					for (i = 0; i < mcindex; i++)
						temp.Concatf("%x ", marcout[i].marc);
					ADDDEBUGMESSAGE(temp);
					#endif
				}
				else		// not found
				{
					temp.SetText("&#x");
					temp.Concat(rcc.marc);
					temp.Concat(';');
					strcpy (marcout[mcindex].marc, temp.GetText());
					marcout[mcindex].charset[0] = 'B';	// must be B for NCR
					mcindex++;
					
					// build message to be used later
					// 	-- this is a mess. we can't output the error msg if the symbol will be precomposed,
					//		but what if another dia is an NCR? then we could have either a type-1 or
					//		type-2 error. but we don't know which NCR to hold the message for until
					//		after SubstitutePrecomposed
					ncr_message = UtfToUtfHex(rcc.utf8);
					
					//061103 malformed UTFs have NCRs like &#xFFFFF9;, so we don't want to show them
					//		except for AAA who want them
					if (TheClient == "AAA")
						ncr_message.Sprintf("Problem: &#x%s; (UTF-8 %s) not found in conversion tables", rcc.marc, ncr_message.GetText());
					else
						ncr_message.Sprintf("Problem: UTF-8 %s not found in conversion tables", ncr_message.GetText());
				}
				
				
				oo += abs(utflen);
				if (oo >= poldlen)
					break;


				utflen = LookupOneUtfChar(rMessage, pold + oo, rcc);
				
				//060103 see sub for discussion
				iscombining = IsCombining(rcc);
			} 
			while (iscombining);
			
	
			// OUTPUT THE MODIFIERS AND BASE CHARACTER IN REVERSE ORDER (FOR MARC)
			
			//060120 if we have been saving an opening punc mark, output it now
			if (punc_on_hold)
			{
				// convert greek left dbl quote to arabic or vice versa
				if ((punc_on_hold == 0x32) && (marcout[0].charset[0] == '3'))
					punc_on_hold = 0x7a;
				else if ((punc_on_hold == 0x7a) && (marcout[0].charset[0] == 'S'))
					punc_on_hold = 0x32;
				
				this_es = MakeEscapeSequence(marcout[0].charset[0], &curr_basic_cs, &curr_extended_cs);
				strcpy ((char*) Work + nn, this_es);
				nn += strlen(this_es);
				Work[nn++] = punc_on_hold;
				punc_on_hold = 0;
			}
			else		// don't check again
			{
				//060120 if char is opening punctuation, don't output it until we see
				//		what charset the following char is -- 060120 see notes in GetMostLogicalCharset
				if 	(		((marcout[0].marc[0] == 0x32) && (marcout[0].charset[0] == 'S'))
						||	((marcout[0].marc[0] == 0x7a) && (marcout[0].charset[0] == '3'))
					)
				{
					punc_on_hold = marcout[0].marc[0];
					continue;	// punc waits in marcout until following letter is added
				}
			}
				
				
			// if the previous letter had a double dia, output the 2d half
			if (doublelig)
			{
				// only the single-symbol dbl lig needs the 2d half added; if
				//		the two halves of the dbl lig were coded separately,
				//		we dn need to add here
				for (i = mcindex - 1; i >= 0; i--)
				{
					if ((marcout[i].marc[0] == 0xec) && (marcout[i].charset[0] == 'E'))
						break;
				}
				if (i < 0)	// 2d half not found
					Work[nn++] = 0xec;
				doublelig = FALSE;
			}
			else if (doubletilde)
			{
				// see note under doublelig
				for (i = mcindex - 1; i >= 0; i--)
				{
					if ((marcout[i].marc[0] == 0xfb) && (marcout[i].charset[0] == 'E'))
						break;
				}
				if (i < 0)	// 2d half not found
					Work[nn++] = 0xfb;
				doubletilde = FALSE;
			}
			
			
			// if we couldn't find a MARC code above we set the ncr_message. now we decide
			//		whether we need it. if we can subst a precomposed, no, otherwise yes
			if (mcindex > 1)
			{
				if (SubstitutePrecomposed (marcout, mcindex))
					ncr_message.Clear();
			}
			if (ncr_message.GetLength())
			{
				ADDMESSAGE(ncr_message);
				Result = E_CONVERSION;
			}
			ncr_message.Clear();
			

			// main output loop
			for (i = mcindex - 1; i >= 0; i--)
			{
				// sending the addresses allows MES to update them
				this_es = MakeEscapeSequence(marcout[i].charset[0], &curr_basic_cs, &curr_extended_cs);
				strcpy ((char*) Work + nn, this_es);
				nn += strlen(this_es);
				strcpy ((char*) Work + nn, marcout[i].marc);
				nn += strlen(marcout[i].marc);
				
				// look for 1st halves of double ligature and double tilde
				if ((marcout[i].marc[0] == 0xeb) && (marcout[i].charset[0] == 'E'))
					doublelig = TRUE;
				else if ((marcout[i].marc[0] == 0xfa) && (marcout[i].charset[0] == 'E'))
					doubletilde = TRUE;
			}
		}
	}
	
	else		// latin or plain, or TOANSEL but without codetables
	{
		/*051222 this works only on the modifiers for latin letters, modifiers for 
				other charsets are outside this range
				
			-- in other words, display AND SEARCHING !! of nonromans is not supported
		*/
			
		// LOAD A SINGLE CHARACTER AND ANY TRAILING MODIFIERS
		
		// we load the base symbol into bytes 0-3 of tofind, then any trailing modifiers
		//		into bytes 4+ (so we can output them in reverse order)
		
		#define		CHARLEN 5			// 4 for the char plus \0
		#define		TOFINDSIZE (CHARLEN * (MAX_DIACRITICS+1))	// == 55; allows for the letter plus 10 dias
		char			tofind [TOFINDSIZE];	// we store the base char and all modifiers
		WString		lookup;				// the part of tofind we are currently looking up
		int			utflen = 0;
		int 			tofindindex;
		WString		targetstring;
		int			i;
		
		while ((oo < poldlen) && (nn < zmaxreclen))
		{
			memset (tofind, 0, sizeof (tofind));
			
			if (pold[oo] >= 240)	//four-byte char
				utflen = 4;
			else if (pold[oo] >= 224)	//three-byte char
				utflen = 3;
			else if (pold[oo] >= 161)	//two-byte char
				utflen = 2;
			else		//one-byte char -- we hold this instead of outputing it in case it is followed by a standalone dia
				utflen = 1;
			
			
			//050104 no point in even looking for 3+-byte chars for latin or plain
			if (utflen >= 3)
			{
				if (target == TOANSEL)
				{
					// report it
					memcpy (tofind, pold + oo, utflen);	// copy the UTF base character
					temp = UtfToUtfHex(tofind);
					temp.Sprintf("Problem: UTF-8 %s not found in conversion tables", temp.GetText());
					ADDMESSAGE(temp);
					targetstring.Clear();		// drop character
					
					//061103 -- believe this problem should result in a "bad record"
					Result = E_CONVERSION;
				}
				else if (target == TOLATIN)
					targetstring = "\x1c";	// displays as empty box, in green
				else if (target == TOPLAIN)	// drop character
					targetstring.Clear();
					
				strcpy ((char*) Work + nn, (char*) targetstring.GetText ());
				nn += targetstring.GetLength ();
				oo += utflen;
			}
			else
			{
				memcpy (tofind, pold + oo, utflen);	// copy the UTF base character
				oo += utflen;
		
		
				// now scan ahead looking for the next standalone character
				// 	-- the range looked for are all the combining latin dias
				tofindindex = CHARLEN;
				while ((oo < poldlen) && 
						(((pold[oo] == 204) && (pold[oo+1] >= 128)) ||
						((pold[oo] == 205) && (pold[oo+1] <= 175))))
				{
					tofind [tofindindex] = pold [oo++];
					tofind [tofindindex+1] = pold [oo++];
					tofindindex += CHARLEN;
					
					if (tofindindex > TOFINDSIZE - 1)	// oops, we ran out of room
					{
						ADDDEBUGMESSAGE("Next error message not tested");
						temp = UtfToUtfHex(tofind);
						temp.Sprintf("Problem: Too many modifiers for %s, contact Systems Planning", temp.GetText());
						ADDMESSAGE(temp);
						return E_CONVERSION;	// we don't attempt to continue
					}
				}
				
				
				// OUTPUT THE TRAILING MODIFIERS (each is two bytes plus terminator)
		
				for (i = tofindindex - CHARLEN; i >= CHARLEN; i -= CHARLEN)
				{
					targetstring = LookupSymbol ((char*) (tofind + i), UTF8, target);
					if (targetstring == "-1")
					{
						temp = UtfToUtfHex(tofind + i);
						temp.Sprintf("Problem: UTF-8 %s not found in conversion tables", temp.GetText());
						ADDMESSAGE(temp);
						
						if (target == TOLATIN)
							targetstring = "\x1c";	//displays as empty box, in green
						else		// TOPLAIN and TOANSEL -- drop character
							targetstring.Clear ();
						
						Result = E_CONVERSION;
					}
					strcpy ((char*) Work + nn, (char*) targetstring.GetText ());
					nn += targetstring.GetLength ();
				}
				
				
				// OUTPUT THE BASE CHARACTER
				
				if (utflen == 1)	// plain old ascii
				{
					Work[nn] = tofind [0];
					nn++;
				}
				else
				{
					targetstring = LookupSymbol ((char*) (tofind), UTF8, target);
					if (targetstring == "-1")
					{
						temp = UtfToUtfHex(tofind + i);
						temp.Sprintf("Problem: UTF-8 %s not found in conversion tables", temp.GetText());
						ADDMESSAGE(temp);
						
						if (target == TOLATIN)
							targetstring = "\x1c";	//displays as empty box, in green
						else		// TOPLAIN and TOANSEL -- drop character
							targetstring.Clear ();
						
						Result = E_CONVERSION;
					}
					strcpy ((char*) Work + nn, (char*) targetstring.GetText ());
					nn += targetstring.GetLength ();
				}
			}	// 1- or 2-byte UTF
	
	
			/*
			#ifdef _DEBUG
			// debugging: write each non-ascii char and its ansel representation to a file
			//	-- each conversion on a single line so output can be sorted
			if (utflen > 1)
			{
				int k, m;
				fputs ("UTF-8\t", futf2marc);
				m = strlen ((char const*) tofind);
				for (k = 0; k < m; k++)
				{
					fprintf (futf2marc, "%x ", tofind [k]);
				}
					
				fputs ("\tMARC\t", futf2marc);
				
				for (k = 0; k < targetstring.GetLength (); k++)
				fprintf (futf2marc, "%x ", targetstring.GetCharacter (k));
					
				fputc ('\n', futf2marc);
			}
			#endif
			*/
		}
	}
	
	
	if (oo < strlen ((char*) pold))
	{
		ADDDEBUGMESSAGE("Next error message not tested");
		ADDMESSAGE("Problem: Record longer than 99,999 characters");
		return E_CONVERSION;
	}

	Work[nn] = 0;	//991223 essential since not memsetting


	/*050129 probably not for plain as this string is for searching (cf anseltoplain
	if ((target == TOLATIN) || (target == TOPLAIN))*/
	// displaycodes are only for display, and we always display in latin
	if ((target == TOLATIN) && displaycodes)
		ConvertCodes (Work);	//return code not checked
		

	/*
	#ifdef _DEBUG
	// write each field before and after
	int k, m;
	fputs ("\nUTF-8\n", futfout);
	fputs (pold, futfout);	// orig field
	fputc ('\n', futfout);
	m = strlen ((char const*) pold);
	for (k = 0; k < m; k++)
		fprintf (futfout, "%x ", pold [k]);	// orig field in hex
		
	fputs ("\nMARC-8\n", futfout);
	fputs ((char const*) Work, futfout);	// new field
	fputc ('\n', futfout);
	m = strlen ((char const*) Work);
	for (k = 0; k < m; k++)
		fprintf (futfout, "%x ", Work [k]);	// new field in hex
		
	fputc ('\n', futfout);
	
	fclose (futfout);		// displays every field before and after
	fclose (futf2marc);	// list of utf codes and ansel conversions
	#endif
	*/


	//return results
	*ppold = Work;
	rlen = nn;
	
	return Result;
}
//SP_ConvertUTF8



//SP_Field
//gets NEXT instance of the specified tag. if you want FIRST instance, reset the record first
//caller can specify exact tag or a tag mask such as 1xx, x10 (the latter not tested)
int WDLLEXPORT SP_Field (LPUSTR lptagtoget, LPPUSTR ppfld, long& rlen, LPPUSTR pptag)
{
	char* 	ptagtoget = (char*) lptagtoget;
	BYTE		datahold [6];
	int		offset = 99;		//010906 simplifies getting leader
	int		Result = E_PERFECTSUCCESS;
	
		
	//010426 make leader searchable -- let's hope these abbreviations don't replicate any non-numeric tags
	if (stricmp (ptagtoget, "ldr") == 0)	//leader as a whole
	{
		offset = 0;
		FieldLength = 24;
	}
	else if (stricmp (ptagtoget, "len") == 0)	//byte by byte
	{
		offset = 0;
		FieldLength = 5;
	}
	else if (stricmp (ptagtoget, "sta") == 0)
	{
		offset = 5;
		FieldLength = 1;
	}
	else if (stricmp (ptagtoget, "typ") == 0)
	{
		offset = 6;
		FieldLength = 1;
	}
	else if (stricmp (ptagtoget, "lev") == 0)
	{
		offset = 7;
		FieldLength = 1;
	}
	
	//two values for the same byte -- "ctl" is MARC21, "hie" is UNIMARC
	//	-- this will not prevent the user from using the acronym for the other kind of file -- not worth preventing
	else if (stricmp (ptagtoget, "ctl") == 0)
	{
		offset = 8;
		FieldLength = 1;
	}
	else if (stricmp (ptagtoget, "hie") == 0)
	{
		offset = 8;
		FieldLength = 1;
	}
	
	//applies to MARC21 only, not UNIMARC
	else if (stricmp (ptagtoget, "chr") == 0)
	{
		offset = 9;
		FieldLength = 1;
	}
	
	else if (stricmp (ptagtoget, "enc") == 0)
	{
		offset = 17;
		FieldLength = 1;
	}
	else if (stricmp (ptagtoget, "cat") == 0)
	{
		offset = 18;
		FieldLength = 1;
	}
	else if (stricmp (ptagtoget, "rel") == 0)
	{
		offset = 19;
		FieldLength = 1;
	}
	if (offset < 99)
	{
		if (bXML)
		{
			int i = strstr ((char const *) TheRecord, "leader>") - (char const *) TheRecord + 7;
			strncpy ((char*) TheField, (char const*) TheRecord + i + offset, FieldLength);
		}
		else
			strncpy ((char*) TheField, (char const*) TheRecord + offset, FieldLength);
			
		*(TheField + FieldLength) = 0;
		strcpy ((char*) TheTag, (char const*) ptagtoget);
		
		//return results
		*ppfld = TheField;
		*pptag = TheTag;
		rlen = - FieldLength;	//minus means we're in the leader
		
		return Result;
	}


	// NON-LEADER FIELDS

	// check tag to look for
	//	-- if ptagtoget indicates "next field" don't bother validating
	if (!((strlen (ptagtoget) == 0) || (stricmp (ptagtoget, "xxx") == 0) || (stricmp (ptagtoget, "   ") == 0)))
	{
		Result = ValidateTag (ptagtoget, 1);	// 1 means allow x/X in tag
		if (Result != E_PERFECTSUCCESS)
			return Result;
	}
	
	do
    {
		if (bXML)
		{
			// no good way to test if xml rec has been reset
			Result = GetNextXMLField ();
			if (Result == E_EXCEPTION)
				return Result;
		}
		
		else
		{
			if (EntryOffset + 1 >= BaseAddress)	//field not found. if this is the first iteration, record needs to be reset
				return E_EXCEPTION;
				
			EntryOffset += 12;
			strncpy ((char*) TheTag, (char const*) TheRecord + EntryOffset, 3);
			if (*TheTag == zmarcfldterm)
				return E_EXCEPTION;
		}
			
		// now check the one found -- 
		Result = ValidateTag ((char*) TheTag, 0);	// 0 means don't allow x/X in tag
		if (Result == E_NONMARC)
			return Result;	// otherwise we have a non-numeric so return the field anyway
		
	}	while  
			(	((*(ptagtoget + 0) != 'x') && (*(ptagtoget + 0) != (*(TheTag + 0)))) ||
			    	((*(ptagtoget + 1) != 'x') && (*(ptagtoget + 1) != (*(TheTag + 1)))) ||
		    		((*(ptagtoget + 2) != 'x') && (*(ptagtoget + 2) != (*(TheTag + 2))))
			);

	
	if (!bXML)
	{
		//find length of field
		memset (datahold, 0, sizeof (datahold));
		strncpy ((char*) datahold, (char const*) TheRecord + EntryOffset + 3, 4);
		FieldLength = atoi ((char const*) datahold);
		if (FieldLength == 0)
			return E_NONMARC;	//-5
	
		//find offset of field -- currently no test for invalid chars
		memset (datahold, 0, sizeof (datahold));
		strncpy ((char*) datahold, (char const*) TheRecord + EntryOffset + 7, 5);
		FieldOffset = atoi ((char const*) datahold);
	
		strncpy ((char*) TheField, (char const*) TheRecord + BaseAddress + FieldOffset, FieldLength);
		*(TheField + FieldLength) = 0;
	}


	SP_ResetField ();
	
	//060118 now we return non-numeric fields even if E_GENERAL
	if ((Result == E_PERFECTSUCCESS) || (Result == E_GENERAL))
	{
		//return results
		*ppfld = TheField;
		*pptag = TheTag;
		rlen = FieldLength;
	}
	
	return Result;
}
//SP_Field



//SP_FieldIsControlField -- we pass a string so non-numeric tags can be handled
int WDLLEXPORT SP_FieldIsControlField (BYTE* tag)
{
	int i, j;
	i = strlen((char const*) tag);
	if (i != 3)
		return E_GENERAL;
		
	/*060421 adding check for all leader bytes -- treat them all as control flds
	if (!strcmp((char const*) tag, "LDR"))
		return 1;	//treat leader as control
	*/
	WString stag;
	stag.SetText((char const*) tag);
	stag.ToUppercase();
	if ((stag == "LDR") || (stag == "LEN") || (stag == "STA")
			|| (stag == "TYP") || (stag == "LEV") || (stag == "HIE")
			|| (stag == "CTL") || (stag == "CHR") || (stag == "ENC")
			|| (stag == "CAT") || (stag == "REL"))
		return 1;		// treat ldr and all its bytes as control fields
		
	for (j = 0; j < i; j++)
	{
		if (!isdigit (tag [j]))
			return 0;		// non-numeric tags are not control fields
						// -- don't check if non-numeric option is on, let caller do it if nec
	}
	
	int itag = atoi ((char const *) tag);

	if ((RecordFormat == MARC21BIB) || (RecordFormat == MARC21AUTH))
	{
		//should not ever be 0, as we have already handled non-numerics, but still...
		if ((itag > 0) && (itag < 10))	//050426 was <= 10
			return 1;
		else
			return 0;
	}
	else		//unimarc
	{
		if ((itag > 0) && (itag <= 5))	//per michele, 001 and 005 are the only two UNIMARC control fields
			return 1;
		else
			return 0;
	}
}
//SP_FieldIsControlField
	


//SP_GetPreferences
SP_prefs_rt WDLLEXPORT SP_GetPreferences ()
{
	SP_prefs_rt prefs;
	
	//011106//prefs.visiblecodes = Preferences.visiblecodes;
	prefs.nonnumerictags = Preferences.nonnumerictags;
	prefs.charset = Preferences.charset;
    	prefs.maxrecords = Preferences.maxrecords;
	
	return prefs;
}
//SP_GetPreferences



//SP_InitializeSymbols		
//051204 added param for loading codetables
//051217 added return code
int WDLLEXPORT SP_InitializeSymbols()
{
	int Result = E_PERFECTSUCCESS;	// not used. was for returning results of InitializeCodetables
	
	//normally done in SP_UseFile, but that isn't necessarily called before importing, so check so we don't do it twice
	if (Symbols [0].ansel [0] == 0)
		InitializeSymbols ();
	
	return Result;
}
//SP_InitializeSymbols



//SP_LatinToAnsel
//symbols not found are NOT transferred to output
//********* warning, does not process characters 128 - 159 which Windows uses for additional characters ****
int WDLLEXPORT SP_LatinToAnsel (LPPUSTR ppold, long& rlen)
{
	int				k;
	long				m, nn, oo;
	BYTE*			pold;
	int				found;
	int				Result = E_PERFECTSUCCESS;
	pold = (BYTE*) 	*ppold;	//simplifies work
	
	nn = 0;	//pnew index
	oo = 0;	//pold index
	
	while ((oo < strlen ((char*) pold)) && (nn < zmaxreclen))
	{
		//latin-1 characters
		if (pold[oo] > 159)	//128-159 are undefined in Latin-1 (some are used by Windows tho)
		{
			found = 0;
			for (k = 0; k < ZNUMSYMS; k++)
			{
				if ((Symbols [k].latin [0] == pold[oo]) &&
				    (Symbols [k].latin [1] == 0))	//looking only at one-byte latins
				{
					found = 1;
					m = 0;
					while ((Symbols [k].ansel [m]) && (m < 2))
					{
						debugWork[nn] = Symbols [k].ansel [m];
						m++;
						nn++;
					}
					break;
				}
			}
			
			//050130 i guess the rationale here is that chars not found are simply
			//			dropped, so it is an exception rather than a conversion error
			if (!found)
				Result = E_CONVERSION;		//050130 was EXCEPTION
				
			oo++;
		}
		
		//normal characters
		else
		{
			debugWork[nn] = pold[oo];

			nn++;
			oo++;
		}
	}

	if (oo < strlen ((char*) pold))
		return E_CONVERSION;

	debugWork[nn] = 0;
	
	//return results
	*ppold = debugWork;
	rlen = nn;
	
	return Result;
}
//SP_LatinToAnsel



//SP_LatinToLatin -- added 030202 to convert sfc and ft to double dagger and para
int WDLLEXPORT SP_LatinToLatin (LPPUSTR ppold)
{
	//BYTE*		pold;
	int			Result = E_PERFECTSUCCESS;

	_fmemcpy (debugWork, *ppold, _fstrlen ((char*) *ppold));	//easier to work with
	debugWork [_fstrlen ((char*) *ppold)] = 0;

	ConvertCodes (debugWork);	//return code not checked

	//return results
	*ppold = debugWork;	//because debugWork will go out of scope
	
	return Result;
}
//SP_LatinToLatin



//SP_NextRecordByOffset
//050219 gets the probable offset of a record and returns the probable offset of the next one
//	-- used when there is a missing or incorrect Record Terminator, so we ignore the putative
//		Record Length of the incoming offset and simply scan for the RT, returning the byte 
//		after it. this could, of course, be skipping more than one record, but we can't know 
//		that. see doc for discussion
int WDLLEXPORT SP_NextRecordOffset (long& roffset)
{
	int		Result = E_PERFECTSUCCESS;

	if (roffset < 0)
		return E_EXCEPTION;
	
	if (!CheckFile (f))
		return E_FILENOTOPEN;


	// doesn't work on XML at present
	if ((RecordFormat == MARCXMLBIB) || (RecordFormat == MARCXMLAUTH))
		return E_EXCEPTION;
	
	fseek (f, roffset, SEEK_SET);

	char c = 0;
	while ((c != 29) && !feof (f))	// 29 is 1D, the RT
		c = fgetc (f);
	
	if (feof (f))
		return E_UNEXPECTEDEOF;
		
	roffset = ftell (f);	// the char after the RT
	
	return Result;
}
//SP_NextRecordByOffset



//SP_PhysicalRecordLength
long WDLLEXPORT SP_PhysicalRecordLength (long n)
{
	return pRecArray [n + 1] - pRecArray [n];
}
//SP_PhysicalRecordLength



//SP_Record
int WDLLEXPORT SP_Record (WString& rMessage, long n, LPPUSTR pprec)
{
	if ((n > RecCount) || (n < 1))
		return E_EXCEPTION;

	long& 	roffset = pRecArray[n];	// this can change the array, but
								//		we wn use this sub unless the array
								//		had already been built
	
	long		reclen;	//not needed, just because SP_RBO needs it -- DO NOT REMOVE FROM SP_RBO!
	long&	rreclen = reclen;

	return SP_RecordByOffset (rMessage, roffset, pprec, rreclen);
}
//SP_Record



//SP_RecordByOffset
// gets the probable offset of record to read and returns the exact offset (they will be
//		different if there are garbage characters between records)
int WDLLEXPORT SP_RecordByOffset (WString& rMessage, long& roffset, LPPUSTR pprec, long& rreclen)
{
	BYTE		datahold [6];
	int		Result = E_PERFECTSUCCESS;
	
	if (roffset < 0)
		return E_EXCEPTION;
	
	if (!CheckFile (f))
		return E_FILENOTOPEN;


	if ((RecordFormat == MARCXMLBIB) || (RecordFormat == MARCXMLAUTH))
		Result = ReadXMLRecord (roffset);
	else
		Result = ReadRecord (rMessage, roffset);


	if (Result < 1)
		return Result;
		
	if (Result == 3)
	{
		return Result;
	}

				
	if ((RecordFormat != MARCXMLBIB) && (RecordFormat != MARCXMLAUTH))
	{
		//FIND BASE ADDRESS OF DATA
		memset (datahold, 0, sizeof (datahold));
		strncpy ((char*) datahold, (char const*) TheRecord + 12, 5);
		BaseAddress = atoi ((char const*) datahold);
		if (BaseAddress == 0)
			return E_NONMARC;	//-5
	
		DirEntries = (BaseAddress - 25) / 12;
	}
	
	SP_ResetRecord ();

	//return results
	roffset = RecordOffset;
	*pprec = TheRecord;
	rreclen = RecLength;

	return Result;
}
//SP_RecordByOffset



//SP_RecordFormat
int WDLLEXPORT SP_RecordFormat ()
{
	return RecordFormat;
}
//SP_RecordFormat



//SP_RecordOffset
long WDLLEXPORT SP_RecordOffset (long recnum)
{
	return pRecArray [recnum];
}
//SP_RecordOffset



//SP_ResetField
//resets offset of subfield so subfields can be read sequentially
void SP_ResetField ()
{
	SubfieldOffset = 0;	//961118 was 1, now 0-based
}
//SP_ResetField



//SP_ResetRecord
//resets dir-entry so fields can be read sequentially
void SP_ResetRecord ()
{
	if ((RecordFormat == MARCXMLBIB) || (RecordFormat == MARCXMLAUTH))
	{
		EntryOffset = strstr ((char const *) TheRecord, "controlfield") - (char const *) TheRecord;	// assuming every rec has at least one
		SubfieldOffset = 0;
	}
	else
	{
		EntryOffset = 12;		//970204 so adding 12 will get to first dir entry
		SubfieldOffset = 0;	//961118 was 1, now 0-based
	}
}
//SP_ResetRecord



//SP_SetPreferences
void WDLLEXPORT SP_SetPreferences (SP_prefs_rt prefs)
{
	Preferences.nonnumerictags = prefs.nonnumerictags;
	Preferences.charset = prefs.charset;
    	Preferences.maxrecords = prefs.maxrecords;      //990111
    	iiMaxRecs = prefs.maxrecords;	//works because SP_SetPreferences called at startup
}
//SP_SetPreferences



//SP_Statistics	990130
//handles numeric tags only at present
//format of report is an array of longs, 4 items for file:
//		number of records, length of file, minreclen, maxreclen
//	followed by 5 items for each tag USED in the file:
//		tag1, instances, minfldlen, maxfldlen, totalfldlen
//		tag2, instances, minfldlen, maxfldlen, totalfldlen
//		etc.
//	last item = -1
//990823 made report global pointer so memory could be freed
//		-- also made it get memory for all 1000 tags, since may be reused for various files
//000503 major overhaul to provide counts from the Leader
int WDLLEXPORT SP_Statistics (long far **ppreport)
{
	int		Result = E_PERFECTSUCCESS;
	
	//080409 believe these snb ref'd after returning from sub, but they are!
	long		tags    [1000];
	long		minlens [1000];
	long		maxlens [1000];
	long		totlens [1000];
	long		ldrstats [296];	//the 8 bytes we count times 37 possible values per byte (0..9, a..z, blank)
	
	int		i;
	int		j;
	long		ii, jj;
	long		minreclen = 99999, maxreclen = 0, totreclen = 0;
	char		stag [4];
	char		slen [5];
	char 	soffset [6];	//not used except to simplify reading of file
	int		tag, len;

	//debug -- outputs stats to file
	//FILE			*debug;
	
	
    assert (f);
	if (!CheckFile (f))
	{
		Result = E_FILENOTOPEN;
		goto abort;
	}
	
	for (i = 0; i < 1000; i++)
	{
		tags    [i] = 0;
		minlens [i] = 9999;
		maxlens [i] = 0;
		totlens [i] = 0;
	}
	
	for (i = 0; i < 296; i++)
		ldrstats [i] = 0;

	stag [3] = 0;
	slen [4] = 0;

	int ldrvalue;
	
	for (ii = 1; ii <= RecCount; ii++)
	{
		if (bXML)
		{
			// leader stats
			
			j = ReadXMLRecord (pRecArray [ii]);
			if (j < 1)
				return j;
			
			// get leader -- can't use GetNextXMLField for leader
			int i = strstr ((char const *) TheRecord, "leader>") - (char const *) TheRecord + 7;
			strncpy ((char*) TheField, (char const*) TheRecord + i, 24);
			TheField [24] = 0;

			for (i = 5; i < 10; i++)	//first group of 5 ldr bytes, bytes 5 - 9
			{
				ldrstats [MakeLeaderIndex (i, TheField [i])]++;
			}
			for (i = 17; i < 20; i++)	//second group of 3 ldr bytes, bytes 17 - 19
			{
				ldrstats [MakeLeaderIndex (i, TheField [i])]++;
			}

			// we also use the ldr to get reclens
			strncpy (slen, (char*) TheField, 5);
			jj = atol (slen);
			totreclen += jj;
			if (jj < minreclen)
				minreclen = jj;
			if (jj > maxreclen)
				maxreclen = jj;


			//field statistics
			SP_ResetRecord ();
			
			i = 99;
			while (i > 0)
			{
				i = GetNextXMLField ();
				if (i < 1)
					break;
					
				tag = atoi ((char const*) TheTag);
				tags [tag]++;
				
				len = strlen ((char const*) TheField);
				if (len < minlens [tag])
					minlens [tag] = len;
				if (len > maxlens [tag])
					maxlens [tag] = len;
				totlens [tag] += len;
			}
		}
		else
		{
			// leader stats
			
			// read directly from file
			fseek (f, pRecArray [ii] + 5, SEEK_SET);
			for (i = 0; i < 5; i++)	//first group of 5 ldr bytes, bytes 5 - 9
			{
				ldrvalue = fgetc (f);
				ldrstats [MakeLeaderIndex (5 + i, ldrvalue)]++;
			}
			fseek (f, pRecArray [ii] + 17, SEEK_SET);
			for (i = 0; i < 3; i++)	//second group of 3 ldr bytes, bytes 17 - 19
			{
				ldrvalue = fgetc (f);
				ldrstats [MakeLeaderIndex (17 + i, ldrvalue)]++;
			}
		
			//field statistics -- done by scanning directories
			fseek (f, pRecArray [ii] + 24, SEEK_SET);
			stag [0] = ' ';
			while (stag [0] != zmarcfldterm)
			{
				//fgets gets one less char than asked for
				fgets (stag, 4, f);
				fgets (slen, 5, f);
				fgets (soffset, 6, f);
				
				tag = atoi (stag);
				tags [tag]++;
				
				len = atoi (slen);
				if (len < minlens [tag])
					minlens [tag] = len;
				if (len > maxlens [tag])
					maxlens [tag] = len;
				totlens [tag] += len;
			}
		}
	}
		

	//prepare report
	//000503 added 296 items
	if (!pReport)
	{
		if (!bMalloc ((void**) &pReport, ((5 * 1001) + 296) * sizeof (long)))
		{
			Result = E_NOMEMORY;
			goto abort;
	   	}
	}
	
	
	pReport [0] = RecCount;
	fseek (f, 0, SEEK_END);
	pReport [1] = ftell (f);
	
	
	//min, max reclens
	//	-- this doesn't work for XML, as we want the MARC reclen not the actual reclen
	if (!bXML)
	{
		for (ii = 2; ii <= RecCount; ii++)
		{
			jj = pRecArray [ii] - pRecArray [ii - 1]; 
			totreclen += jj;
			if (jj < minreclen)
				minreclen = jj;
			if (jj > maxreclen)
				maxreclen = jj;
		}
	
		//990929 now we have to check the last record
		//		-- pReport [1] holds the file length, subtract the offset of the last record
		jj = pReport [1] - pRecArray [RecCount]; 
		totreclen += jj;
		if (jj < minreclen)
			minreclen = jj;
		if (jj > maxreclen)
			maxreclen = jj;
	}
	
	pReport [2] = minreclen;
	pReport [3] = maxreclen;
	
	//040724 can't calculate mean rec len from file len for XML
	pReport [4] = totreclen/RecCount;
	
	
	//000503 leader stats
	j = 5;	//first ldr stat goes in this item	//040724 was 4
	for (i = 0; i < 296; i++)
		pReport [j + i] = ldrstats [i];
	j = 301;	//first tag goes in this item	//040724 was 300
		
	for (i = 1; i < 1000; i++)	//0 position not used
	{
		if (tags [i] > 0)
		{
			pReport [j] = i;
			pReport [j+1] = tags [i];
			pReport [j+2] = minlens [i];
			pReport [j+3] = maxlens [i];
			pReport [j+4] = totlens [i];
			j += 5;
		}
	}	
		
	pReport [j] = -1;	//sentinel
	
	/*
	//debug
	debug = fopen ("stats.out", "wb");
	fwrite (pReport, sizeof (long), j + 1, debug);
	fclose (debug);
	*/
	
	//return results
	*ppreport = pReport;
	
abort:	
	return Result;
}
//SP_Statistics



//SP_Subfield
//for fields without subfields (e.g. 001) returns data without field terminator
//		sfc param sb * or $ -- 040725 no longer returns control field if a 
//		specific sfc is specified -- that caused errors in find
//970205 assumes field user wants is in TheField, doesn't get
//970314 sfctoget of $ means entire field wanted, even if not control
//			(actually gets remainder of field from current sf offset, I think)
//990619 sfctoget of * means get next subfield
//		 fourth param has to be pointer in order to return value
int WDLLEXPORT SP_Subfield	(char sfctoget,
							LPPUSTR ppsubf, 
							long& rlen,
							char* psfc,
							yCharConvert target_charset)
{
	long			j, k;
	//970205 now that SP_Field not called, i think pSSS can be near
	BYTE*		pSSS = NULL;
	char			s [3];
	int			Result = E_PERFECTSUCCESS;
	BYTE*		pnewsub;
	long&		rSubfieldLength = SubfieldLength;
	long			badchar;
	long&		rbadchar = badchar;
	
	//051231 eventually rMessage will be passed to SP_Subfield; this is just so we can
	//		call ConvertUTF8 correctly (so therefore it is currently unused)
	WString		TheMessage;
	WString&		rMessage = TheMessage;
	
	
	s [0] = zmarcsubterm;
	s [1] = sfctoget;
	s [2] = 0;
		
	//find start of next sf
	pSSS = TheField + SubfieldOffset;
	if (*pSSS == zmarcfldterm)	//no more
	{
   		Result = E_EXCEPTION;
		goto end;
	}	   	

	//control field
	if (SP_FieldIsControlField (TheTag))
	{
		if ((sfctoget == '*') || (sfctoget == '$'))
			SubfieldLength = FieldLength - 1;	//subtract 1 for field terminator
			
		else		// user specified a subfield
		{
			Result = E_EXCEPTION;
			goto end;
		}	   	
	}

	//normal field
	else
	{
		if (sfctoget == '$')
		{
			SubfieldLength = FieldLength - SubfieldOffset;	//$ may come after other sfc's to get remainder
			//NOT TESTED EXCEPT WITH SUBFIELDOFFSET == 0
			*psfc = '$';
		}
		else		// it's either * or a sfc
		{
			if (sfctoget == '*')
			{
				//if this is the first subfield, need to get to the delimiter. for later subfields,
				//		pSSS is already positioned at the next subfield delim, so just need to grab it
				if (SubfieldOffset == 0)
				{
					s[1] = 0;		//so s is just the zmarcsubterm
					pSSS = (BYTE*) strstr ((char const*) pSSS, s);
				}
				// not sure why no alternative here, but that's how it was before refactoring
			}
			else		// caller requested a sfc
				pSSS = (BYTE*) strstr ((char const*) pSSS, s);
			
			if (pSSS == NULL)	// subfield not found
			{
				Result = E_EXCEPTION;
				goto end;
			}	   	
			
			//return sfc found
			*psfc = pSSS [1];
		
			//advance past sfc
			pSSS += 2;
			
			//find end of next subfield or field
			//	this complex algorithm is because strcspn will return position
			//	of terminating 0 if substring not found
			j = strcspn ((char const*) pSSS, (char const*) subtermstring);
			k = strcspn ((char const*) pSSS, (char const*) fldtermstring);
			if (j < k)
				SubfieldLength = j;
			else	// j > k, they cannot be equal, obviously
				SubfieldLength = k;
		}
	}
	
	SubfieldOffset = pSSS - TheField + SubfieldLength;	//NOT TESTED FOR SFCTOGET == $
	strncpy ((char*) TheSubfield, (char const*) pSSS, SubfieldLength);
	*(TheSubfield + SubfieldLength) = 0;


	//030907 if entire field requested and not control field, replace sfd/sfc with space
	if (!SP_FieldIsControlField (TheTag) && (sfctoget == '$'))
	{
		//strip inds, first sfd/sfc, field terminator
		SubfieldLength -= 5;	
		memmove (TheSubfield, TheSubfield + 4, SubfieldLength);
		*(TheSubfield + SubfieldLength) = 0;
		
		//replace other sfd/sfc with space
		char* x = (char*) memchr (TheSubfield, 31, SubfieldLength);
		while (x)
		{
			SubfieldLength--;
			*x = ' ';
			memmove (x + 1, x + 2, SubfieldLength);	//this is actually moving more than reqd
			*(TheSubfield + SubfieldLength) = 0;
			x = (char*) memchr (TheSubfield, 31, SubfieldLength);
		}
	}
	
	
	//030322 need to check each record and set its charset
	//030907 if file is unimarc, override this logic
	if ((RecordFormat == MARC21BIB) || (RecordFormat == MARC21AUTH))
	{
		if (Preferences.charset == LEADER)
		{
			//only a and blank are legal
			if (TheRecord [9] == 'a')
				OriginalRecordCharset = UTF8;
			else if (TheRecord [9] == ' ')
				OriginalRecordCharset = ANSEL;
			else
				OriginalRecordCharset = UNKNOWN;
		}
		else
			OriginalRecordCharset = Preferences.charset;
	}
	else if ((RecordFormat == UNIMARCBIB) || (RecordFormat == UNIMARCAUTH))
		OriginalRecordCharset = UNIMARC;
	else if ((RecordFormat == MARCXMLBIB) || (RecordFormat == MARCXMLAUTH))
		OriginalRecordCharset = UTF8;
	
	
	//051229 all these routines in production now
	if (target_charset != NOCONVERSION)
	{
		pnewsub = TheSubfield;
		
		switch (OriginalRecordCharset)
		{
			//050130 notice that convertcodes is ON for all these
			case ANSEL:
				Result = SP_ConvertAnsel(rMessage, (LPPUSTR) &pnewsub, rSubfieldLength, target_charset, 1);	// 1 means convertchars
				break;
			case LATIN:
				Result = SP_ConvertLatin(rMessage, (LPPUSTR) &pnewsub, rSubfieldLength, target_charset, 1);	// 1 means convertchars
				break;
			case UNIMARC:
				Result = SP_ConvertUnimarc(rMessage, (LPPUSTR) &pnewsub, rSubfieldLength, target_charset, 1);	// 1 means convertchars
				break;
			case UTF8:
				Result = SP_ConvertUTF8(rMessage, (LPPUSTR) &pnewsub, rSubfieldLength, target_charset, 1);	// 1 means convertchars
				break;
			default:
				Result = -50;		// was E_CONVERSION but this pinpoints the problem better
				break;
		}
	}

	//return results
	if (target_charset == NOCONVERSION)
		*ppsubf = TheSubfield;
	else
		*ppsubf = pnewsub;
		
	rlen = SubfieldLength;

end:  
	return Result;
}
//SP_Subfield



//SP_Terminate
//970821 this prevents using the DLL again. caller should use only when ready to
//	terminate. see also SP_CloseFile
void WDLLEXPORT SP_Terminate ()
{
	vFree (pRecArray);
	fclose (f);
	vFree ((void*) pReport);	//990823 -- vFree checks to see if pointer exists. if vFree not used, check it
	
	//991218//MEMLEAKS;
}
//SP_Terminate



//SP_Unblock
	/*970523 converted from pascal
	930623	program begun
			only for blocksize 2080, block header 32 bytes, one record per block

	970427	investigated problem of ^'s in RLIN.UNB, problem is in original file
				(see notes in folder)
			running file RLIN gives 'Unexpected EOF' message. There is a
				truncated block at the end of the file (as received from JJJ).
			
	970908  blocks can contain parts of more than one record, but this doesn't occur
				in the only file available to me (RLIN), so not coded
			do i need to handle Segment Indicator? (see MARC Tape Specs p. 36)
	
	970913	added checks after getting reclen to ensure numeric and min len
			added param for min len of data in a block (RLIN & OCLC it's 18),
				but then implemented a way to ignore fill chars without this param			
	*/			
	
int WDLLEXPORT SP_Unblock 
		(LPCUSTR appname, 
		LPCUSTR infilename, 
		LPCUSTR outfilename, 
		long& rblocks, 
		long& rrecs,
		
		long blocksize,
		long blockhdrsize,
		long blocklenoffset)
{
	int		Result = E_PERFECTSUCCESS;
	
	char	hilen, lolen;
	char	temp [6];
	FILE*	fin;
	FILE*	fout;
	int		bnewrec = 1;
	int		blockused;
	int		i, j;
	int		reclen;
	long	blockcount = 0, reccount = 0;
	long 	curroffset = 0;
	long	filelen;

#ifdef _DEBUG		
	long	tempo;
#endif


	if (stricmp ((char*) appname, "MV95") && 
			stricmp ((char*) appname, "MV98"))
	{
		Result = E_SECURITY;
		goto abort;
	}
	
	if ((fin = fopen ((char*) infilename, "rb")) == NULL)
   	{
		Result = E_FILENOTFOUND;
		goto badend;
	}
	
	if ((fout = fopen ((char*) outfilename, "wb")) == NULL)
   	{
		Result = E_FILENOTOPEN;
		goto badend;
	}

	//save file length
	fseek (fin, 0, SEEK_END);
	filelen = ftell (fin);
	
	fseek (fin, 0, SEEK_SET);
	while (!feof (fin))
	{
		//position to blockused
		fseek (fin, curroffset + blocklenoffset, SEEK_SET);

#ifdef _DEBUG		
		tempo = ftell (fin);
#endif

		//check that entire block is in input file
		if (ftell (fin) + blocksize - blocklenoffset > filelen)
		{
			Result = E_UNEXPECTEDEOF;
			//next two lines because badend will decrement, and they haven't yet been incremented
			blockcount++;
			reccount++;
			goto badend;
		}

		curroffset += blocksize;
		hilen = fgetc (fin);
		lolen = fgetc (fin);
		blockused = (256*hilen) + lolen;
		blockcount++;
		
		//move ahead 24 bytes to data
		fseek (fin, blockhdrsize - blocklenoffset - 2, SEEK_CUR);
		if (ftell (fin) > filelen)
		{
			Result = E_UNEXPECTEDEOF;
			//next line because badend will decrement, and this hasn't yet been incremented
			reccount++;
			goto badend;
		}
		
		if (bnewrec == 1)
		{
			//read marc reclen
			for (i = 0; i < 5; i++)
				temp [i] = fgetc (fin);
			temp [5] = 0;
			
			//970913 ensure reclen is only numerics, atoi doesn't check
			for (i = 0; i < 5; i++)
				if ((temp [i] < '0') || (temp [i] > '9'))
				{
					Result = E_NONMARC;
					//next line because badend will decrement, and this hasn't yet been incremented
					reccount++;
					goto badend;
				}
				
			reclen = atoi (temp);
			
			//970913
			if (reclen < 40)	//min len of a MARC rec
			{
				Result = E_NONMARC;
				//next line because badend will decrement, and this hasn't yet been incremented
				reccount++;
				goto badend;
			}
			
			reccount++;
			
			//move back 5 to beginning of record
			fseek (fin, -5, SEEK_CUR);
		}
		
		//970913 see if block has fill chars
		if (reclen < blockused)	//reclen is adjusted as each block read
			blockused = reclen;
	
		for (i = 0; i < blockused; i++)
			j = fputc (fgetc (fin), fout);	//check error in read and write
		
		reclen -= blockused;
		
		//this tells us whether next block is part of same record
		if (reclen > 0)
			bnewrec = 0;
		else
			bnewrec = 1;
	}
	
	goto end;
	
badend:	
	reccount--;
	blockcount--;
	
end:
	fclose (fout);
	fclose (fin);

abort:
	rblocks = blockcount;
	rrecs = reccount;

	return Result;
}
//SP_Unblock



//SP_UseFile -- reads entire file to ensure minimal MARCness. sets up array
//		of record offsets. initializes character translation table.
//		returns number of records in count
//if a need arises to have the caller open the file, use the File Management
//	routines that work on handles
//051217 added codetables param
//060517 appname can be parsed to find MC client
//070926 now UseCodetables set based on client code
int WDLLEXPORT SP_UseFile (LPCUSTR appname, LPCUSTR fname, long& rcount)
{
	int		c;
	long		filelen;
	long		i, j;
	int		Result = E_GENERAL;
	char		temp [6];	// length of record length plus \0
	int		maybeblocked=0;
	char		buffer [9999];

	/*
	//debug -- outputs to file
	FILE*	debug;
	debug = fopen ("usefile.out", "wb");
	fwrite ((void const*) appname, strlen ((char const*) appname), 1, debug);
	fclose (debug);
	*/
	
	//check to make sure caller has a right to use this DLL
	if (stricmp ((char*) appname, "MV98") &&
			strncmp ((char*) appname, "MC_", 3))
	{
		Result = E_SECURITY;
		goto abort;
	}
	
	if (!strncmp ((char*) appname, "MC_", 3))
	{
		TheClient.SetText((char const*) appname);
		TheClient = TheClient.Substring(3);
		
		UseCodetables = 
				((TheClient == "AAA") || (TheClient == "CCC") || (TheClient == "HHH")) 
				? TRUE 
				: FALSE;
		
		memset(UTF8_Missing_Symbol, 0, sizeof(UTF8_Missing_Symbol));
		if (TheClient == "AAA")
			strncpy(UTF8_Missing_Symbol, UTF8_DOUBLE_UNDERSCORE, 3);
		else
			strncpy(UTF8_Missing_Symbol, UTF8_GETA, 3);
			
		//060531 CCC wants 2-byte UFF-8 representation of double ligatures and tildes
		//	-- for now, everyone else gets the LC-preferred single-byte versions; cd be a user option later
		if (TheClient == "CCC")
			ForceDoubleDias = TRUE;
		else
			ForceDoubleDias = FALSE;
			
		//070926 base on client, not param
		if (TheClient == "LLL")
			CheckIllegalChars = TRUE;
		else
			CheckIllegalChars = FALSE;
	}
	
	
	//export does not load up recarray, although array left in for simplicity. if removed, see SP_Terminate
	if (!strncmp ((char*) appname, "MC_", 3))
		bUseOffsetArray = 0;
	else
		bUseOffsetArray = 1;


	//991218//MEMAVAIL;
	
	//close file if already open
	fclose (f);


	RecCount = 0;
	

	if ((f = fopen ((char*) fname, "rb")) == NULL)
   	{
		Result = E_FILENOTFOUND;
		goto abort;
	}

	if (!CheckFile (f))
	{
		Result = E_FILENOTOPEN;
		goto abort;
	}


	//050116 hung opening 0-length file -- message for Export includes a meaningless
	//			rec num, so we also check in Export for a better msg
	struct stat buf;
	stat ((char const*) fname, &buf);

	//save file length
	filelen = buf.st_size;
	
	if (filelen < 40)	// was 0, but 40 is min len of a MARC21 rec
	{
		Result = E_UNEXPECTEDEOF;
		goto abort;
	}

	
	//961122 one-time initializations (once per run, not each file)
	//		-- Super and Export don't use array, but create a 1-element array anyway
	if (pRecArray == NULL)
	{
		if (!bMalloc ((void**) &pRecArray, iiMaxRecs * sizeof (long)))
		{
			Result = E_NOMEMORY;
			goto abort;
    		}
				
		Result = SP_InitializeSymbols();
		if (Result != E_PERFECTSUCCESS)
			goto abort;
	}
      
		
	/*060115 tried opening Codetables only if there was 
			no pRecArray, to avoid opening them unnecessarily. But if 
			a user viewed a file, then converted one, they wnb open.*/
			
	if (UseCodetables)
	{
		Result = InitializeCodetables();
		if (Result != E_PERFECTSUCCESS)
		{
			if (Result == E_FILENOTOPEN)
				Result = E_CONVERSION;
			goto abort;
		}
	}
	
	
	//050126 moved from below to test if xml file for export or marctomarc
	//040720 see if XML file -- if logic changed, also change in yHexForm::Setup
	/*040721 xml files output by MSXML are prefixed by EF BB BF. the first of these is the
			unicode byte-order mark, not sure what the other two are. in any case we have 
			to check with and without the EF BB BF*/
	fseek (f, 0, SEEK_SET);
	memset (temp, 0, sizeof (temp)); 
	for (i = 0; i < 5; i++)
		temp [i] = fgetc (f);
		
		
	//050126 LLL XML files start with <collection>
	if (!strncmp (temp, "<?xml", 5) ||
			!strncmp (temp, "<coll", 5) ||
			((temp [3] == '<') && (temp [4] == '?')))
		bXML = TRUE;
	else
		bXML = FALSE;
	
	
	if (!bUseOffsetArray)
	{
		// if we're not building the array, we're done
		Result = E_PERFECTSUCCESS;
		goto abort;
	}

	
	if (bXML)	
	{
		Result = ReadXMLFile ();
		goto end;
	}
	else
	{
		//scan file to see if valid MARC file. the only check done is whether record length is
		//	numeric. it can be assumed that most problems will show from this.
	
		//main loop -- complex because it's looking for a string of 5 numerics, not
		//		just any numeric
		fseek (f, 0, SEEK_SET);
		while (!feof (f) && (RecCount < iiMaxRecs))
		{
			RecCount++;
	
			j = ftell (f);	//should be char just after prev rec
			do
			{
				Result = E_PERFECTSUCCESS;
				
				memset (temp, 0, sizeof (temp)); 
				
				fseek (f, j, SEEK_SET);
				c = ' ';
				
				//skip over non-numerics before the record (such as CRLFs after prev rec)
				while (!isdigit (c) && !feof (f))
					c = fgetc (f);
					
				if (feof (f))		//normal eof
				{
					Result = E_EXCEPTION;
					goto badend;
				}
					
				j = ftell (f);	//advance past nonnumerics
				
				
				//980917 if we skipped a lot of bytes at beginning of file, may be blocked
				if ((RecCount == 1) && (j > 10))
					maybeblocked = 1;
					
				
				temp [0] = c;	//first digit
				for (i = 1; i < 5; i++)
					temp [i] = fgetc (f);
				
				if (strlen (temp) < 5)
				{
					c = fgetc (f);
					if (c == -1)	//eof
						Result = E_EXCEPTION;
						
					else if (c == 26)	// if hex 1A test for eof just after 
					{
						c = fgetc (f);
						if (c == -1)	//eof
							Result = E_EXCEPTION;
						else
							Result = E_UNEXPECTEDEOF;
					}
					else
						Result = E_UNEXPECTEDEOF;
				}
		
				if (Result == E_PERFECTSUCCESS)
				{
					//970118 do this instead of atol because atol doesn't check for 
					//	nonnumerics: "56.(," will convert to 56
					for (i = 0; i < 5; i++)
						if ((temp [i] < '0') || (temp [i] > '9'))
						{
							Result = E_NONMARC;
							break;
						}
					if (Result == E_PERFECTSUCCESS)
						RecLength = atol (temp);
					if (RecLength < 40)		//min len of a MARC rec
						Result = E_NONMARC;
				}
			} while (Result < 0);	//that is, we keep trying for good recs even if we found a bad one
		
			//970118 fseek seems perfectly happy to read past the end of the CCI file, 
			//			neither returning an error nor setting eof
			fseek (f, RecLength - 5, SEEK_CUR);
			if (ftell (f) > filelen)
			{
				Result = E_UNEXPECTEDEOF;
				goto badend;
			}
	
			pRecArray [RecCount] = ftell (f) - RecLength;
	
		}  //while not eof
	}

		
	/*970118 should be normal termination if RecCount == iiMaxRecs
	if (!feof (f)) 
		Result = E_MORETHANMAX;*/
	if (RecCount == iiMaxRecs)
	{
		c = fgetc (f);	// should be the char after the end of last record
		if (c == -1)		// eof
			Result = E_EXCEPTION;
		else
			Result = E_MORETHANMAX;
		goto end;
	}
	else
		WASSERT(0);		// because eof should exit loop via goto

badend:

	/*050103 array element after last rec doesn't get an offset, so we
				couldn't determine reclen of last rec
		-- RecCount is currently one higher than last rec in file, so it's
				the one we want to set*/
	pRecArray [RecCount] = ftell (f);
		
	RecCount--;
	
end:
	rcount = RecCount;
	
	WASSERT (RecCount >= 0);
	
	//980917 return "blocked" warning in some cases
	if (maybeblocked == 1)
	{
		if 	(
					(Result == E_PERFECTSUCCESS) 
				|| 	(Result == E_EXCEPTION)
				|| 	(Result == E_GENERAL)
				|| 	(Result == E_UNEXPECTEDEOF)
				|| 	(Result == E_MORETHANMAX)
			)
			Result = E_BLOCKED;
	}
	
abort:	// despite the name, we can reach here normally if we are not creating the offset array


	//RecCount is meaningless if we don't create the offset array, so no attempt to return it here
	

	//return code
	//1 for success, 0 for unexpected eof, <1 for errors
	//030902 return 1 for MARC21, 0 for UNIMARC, <1 for errors
	if (Result >= 0)
	{
		if (bXML)
		{
			//050126 moved from ReadXMLFile
			// check leader of first XML rec to see if bib or auth
			fseek (f, 0, SEEK_SET);
			while (!feof (f))
			{
				fgets (buffer, 9999, f);
		
				if (strstr (buffer, "leader"))
				{
					int i = strstr (buffer, "leader>") - buffer + 13;
					if (buffer [i] == 'z')
						RecordFormat = MARCXMLAUTH;
					else
						RecordFormat = MARCXMLBIB;
					break;
				}
			}
		}
		else
		{
			//look for an 008, not used in unimarc (i hope) but mandatory in marc21
			int EntryOffset = 0;
			
			do
			{
				fseek (f, 24 + EntryOffset, SEEK_SET);
				fgets ((char*) TheTag, 4, f);
				EntryOffset += 12;
			} 
			//050130 first condition added for abnormal files
			//while ((*TheTag != zmarcfldterm) && strcmp ((char const*) TheTag, "008"));
			while ((ftell (f) < filelen) && (*TheTag != zmarcfldterm) && strcmp ((char const*) TheTag, "008"));
	
			fseek (f, 6, SEEK_SET);
			c = fgetc (f);
			
			if (*TheTag == zmarcfldterm)
			{
				Result = E_EXCEPTION;
				if ((c == 'x') || (c == 'y') || (c == 'z'))	//per michele
					RecordFormat = UNIMARCAUTH;
				else
					RecordFormat = UNIMARCBIB;
			}
			else
			{
				Result = E_PERFECTSUCCESS;
				if (c == 'z')
					RecordFormat = MARC21AUTH;
				else
					RecordFormat = MARC21BIB;
			}
		}
	}
	
	return Result;
}
//SP_UseFile


