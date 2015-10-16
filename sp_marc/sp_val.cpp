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

#if !defined(SP_MARC_H)
#include "SP_MARC.HPP"
#endif


#include <stdlib.h>
#include <string.h>
//#include <assert.h>


extern long			BaseAddress;
extern long			DirEntries;
extern long			EntryOffset;
extern long			FieldLength;
extern long			FieldOffset;
extern long			RecLength;
extern long			SubfieldLength;
extern long			SubfieldOffset;	//offset of next subfield
extern SP_prefs_rt	Preferences;

extern BYTE			TheRecord [zmaxreclen];

int         numerrs;

#ifdef _DEBUG
	#define DEBUGGERBREAK	_asm {int 3};	//causes debugger breakpoint so locals can be checked
#else
	#define DEBUGGERBREAK
#endif

#define		VFLDSTATUS			9



//error messages -- these should go in a string resource but can't access one from optima without using
//		optima classes
//990822 if the messages were left to the caller, or defined in sp_marc.h, wouldn't 
//		need this structure and errcodes could be a diff range from other errs
/*
char*	msgtext [] =   {"RecordLength is not numeric",		//0
						"RecordLength/record mismatch",		//1
						"RecordLength is out of range",		//2
						"Record Terminator missing",		//3 -- do not change this msg as LLL logic depends on exact wording
						"BaseAddress is not numeric",		//4
						"BaseAddress out of range",		//5
						"Nonnumeric character in tag ",		//6
						"Directory length is wrong",		//7
						"Directory/BaseAddress conflict",	//8	-- this is 30 chars
						"Field is ",						//VFLDSTATUS
						"No terminator after directory",	//10
						"Record Terminator inside record",	//11
						"Illegal character"};				//12
*/

//Val2
//int Val2 (int checkillegals)
int Val2 (WString& rMessage, int checkillegals)
{
    int 	Result = E_PERFECTSUCCESS;
    
	long 	i, j;
    	long    	reclen, baseaddr;
	char		workstring [10];
	char*	p;
	
	p = (char*) TheRecord;	//saves a lot of casts
	
	//reclen is numeric
	memset (workstring, 0, sizeof (workstring));
	strncpy (workstring, p, 5);
	
	//970118 atol doesn't check for nonnumerics "56.(," will 
	//			convert to 56
	//reclen = atol (workstring);
    //if (reclen == 0)
	for (i = 0; i < 5; i++)
		if ((p [i] < '0') || (p [i] > '9'))
		{
			Result = E_FATALVALERROR;
			rMessage.Concat("\tRecordLength is not numeric\n");
			goto end;
		}
    
    //reclen == length of record
	reclen = atol (workstring);
	if (reclen != strlen (p))
	{
		Result = E_FATALVALERROR;
		rMessage.Concat("\tRecordLength/record mismatch\n");
		goto end;
	}

    //reclen longer than minimum
	if (reclen < 40)
	{
		Result = E_FATALVALERROR;
		rMessage.Concat("\tRecordLength is out of range\n");
		goto end;
	}
	
	//record terminator exists (can help check if record truncated)
	if (p [strlen (p) - 1] != zmarcrecterm)
	{
		Result = E_FATALVALERROR;
		rMessage.Concat("\tRecord Terminator missing\n");	// do not change this msg as LLL logic depends on exact wording
						//********** HOWEVER, ADDED \n *************************
		goto end;
	}
	
	//000209 see if internal RT
	i = strchr (p, 29) - p + 1;
	if (i != reclen)
	{
		Result = E_MINORVALERROR;	//nonfatal error
		rMessage.Concat("\tRecord Terminator inside record\n");
		//don't goto end, keep validating
	}

    //base address is numeric
	memset (workstring, 0, sizeof (workstring));
	strncpy (workstring, p + 12, 5);
	baseaddr = atol (workstring);
    if (baseaddr == 0)
	{
		Result = E_FATALVALERROR;
		rMessage.Concat("\tBaseAddress is not numeric\n");
		goto end;
	}

    //base address less than reclen
	if (baseaddr >= reclen)
	{
		Result = E_FATALVALERROR;
		rMessage.Concat("\tBaseAddress out of range\n");
		goto end;
	}

	//field terminator after directory
	if (p [baseaddr-1] != zmarcfldterm)
	{
		Result = E_FATALVALERROR;
		rMessage.Concat("\tNo terminator after directory\n");
		goto end;
	}
	
	//remaining directory validation depends on having terminator at end of it
	//might be possible to continue to find these problems even without a
	//terminator, but record is probably irretrievably corrupted, so no point
	
	//see if directory is non-numeric
	for (i = 24; p [i] != zmarcfldterm; i++)
		if ((p [i] < '0') || (p [i] > '9'))
		{
			j = i % 12;
			if (!((Preferences.nonnumerictags == 1) &&
				  ((j == 0) || (j == 1) || (j == 2)))) 
			{
				Result = E_MINORVALERROR;	//000501 was E_FATALVALERROR, trying as nonfatal
				rMessage.Concatf
						("\tNonnumeric character in tag %c%c%c\nChange Tools/Settings if legitimate\n",
						p[i], p[i+1], p[i+2]);
				goto end;
			}
		}
			
	//directory length is mod 12
	//first find zmarcfldterm at end of directory (won't be found if problem above)
	i = 24;
	while (p [i] != zmarcfldterm) i++;
	j = i - 24;
	if (j % 12 != 0)
	{
		Result = E_FATALVALERROR;
		rMessage.Concat("\tDirectory length is wrong\n");
		goto end;
	}

	//directory length corresponds to base address
	if (++i != baseaddr)
	{
		Result = E_FATALVALERROR;
		rMessage.Concat("\tDirectory/BaseAddress conflict\n");
		goto end;
	}
	
	//050125 check for legal chars
	// can't use strcspn as it is limited to an int (32K)
	if (checkillegals)
	{
		int c;
		while (*p)
		{
			c = *p++;
			/*060117 this list is out of date. should be
					00 - 1a, 1c, 7f - 87, 8a - 8c, 8f - a0, af, bb, be, bf, c9 - df,
					fc, fd, ff 
				-- since used only for LLL, don't change until discussed with them */
			if (((c >= 0) && (c <= 26)) ||			// 00-1A
					(c == 28) ||					// 1C
					((c >= 127) && (c <= 160))	||	// 7F-A0
					(c == 175) ||					// AF
					(c == 187) ||					// BB
					(c == 190) ||					// BE
					(c == 191) ||					// BF
					((c >= 201) && (c <= 223))	||	// C9-DF
					(c == 252) ||					// FC
					(c == 253) ||					// FD
					(c == 255))					// FF
			{
				Result = E_MINORVALERROR;	//nonfatal error
				rMessage.Concat("\tIllegal character\n");
				break;
			}
		}
	}

end:

    return Result;	//E_PERFECTSUCCESS if no errs, E_MINORVALERROR if minor error, E_FATALVALERROR if fatal
}
//Val2



//SP_ValidateRecord
#ifdef _DEBUG
//int WDLLEXPORT SP_ValidateRecord (int illegals, char* pfilename, int line)
int WDLLEXPORT SP_ValidateRecord (WString& rMessage, int illegals, char* pfilename, int line)
#else
//int WDLLEXPORT SP_ValidateRecord (int illegals)
int WDLLEXPORT SP_ValidateRecord (WString& rMessage, int illegals)
#endif
{
	numerrs = 0;
	
    	//return Val2 (illegals);	//E_PERFECTSUCCESS if no errs, E_MINORVALERROR if minor error, E_FATALVALERROR if fatal
    	return Val2 (rMessage, illegals);	//E_PERFECTSUCCESS if no errs, E_MINORVALERROR if minor error, E_FATALVALERROR if fatal
}
//SP_ValidateRecord


