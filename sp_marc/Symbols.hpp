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


#if !defined (SYMBOLS_H)
#define SYMBOLS_H


//#define BYTE unsigned char
#define ZNUMSYMS  215


typedef struct tagSYMBOL 
{
	//each of these is long enough for the longest possible symbol, plus 1 for trailing 0
	//051227 converted from BYTE to char to save all the casts
	char ansel 	[3];
	char latin 	[5];
	char plain 	[5];
	char unimarc	[3];
	char utf8	[5];
} symbol_rt;


typedef struct tagCHARCONVERT
{
	//each of these is long enough for the longest possible symbol, plus 1 for trailing 0
	char marc 	[7];		//051231 was 4
	char utf8	[5];		//051231 was 4
	char combining [2];
	char charset [2];
} charconvert_rt;


typedef struct tagDECOMP 
{
	//each of these is long enough for the longest possible symbol, plus 1 for trailing 0
	char utf8	[5];
	char decomp 	[24];
} decomp_rt;


//051222 these are the sizes of the nonroman arrays
//051224 UTF-to-MARC is three smaller because we do not include Greek Symbols --
//		we prefer to convert to Basic Greek instead
//060119 also we removed the alternative MARC encodings, so that UTFs would convert to the preferred
const long zmarctoutf_size = 16694L;
const long zutftomarc_size = 16458L;
const int zdecomp_size = 5073;	//060119 not sure why more//4626;


void InitializeSymbols (void);
int InitializeCodetables (void);


#endif

