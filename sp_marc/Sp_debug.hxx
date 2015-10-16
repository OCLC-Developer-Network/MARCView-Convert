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

#if !defined (SP_DEBUG_H)
#define SP_DEBUG_H

#ifdef _DEBUG

#include <assert.h>
#include <malloc.h>

//NEED NO "UNDEFINING" IF NOT DEBUG
	
//miscellaneous debug variables

long			dRecNum;
LPUSTR			dValMsg;
//unsigned int	MemStart, MemEnd;

//memory management

#define MallocGarbage	0xCA
#define ReallocGarbage	0xCC 
#define FreeGarbage		0xCF
#define ZMAXMEMREFS		10		//980612 was 10, but got assert err in line 63
								//980613 changed back to 10, believe caused by leak

typedef struct tagMEMREF
{
	void*		ptr;
	size_t		size;
} yMemRef;

yMemRef		MemArray [ZMAXMEMREFS];
int			Mi = 0;
	
void addMemRef (void* ptr, size_t size)
{
	int i;
	
	assert ((ptr != NULL) && (size > 0));
	
	//make sure pointer not already used
	for (i = 0; i <= Mi; i++)
	{
		if (MemArray [i].ptr == ptr)
			assert (0);
	}
	
	//look for first empty slot
	for (i = 0; i < ZMAXMEMREFS; i++)
	{
		if (MemArray [i].ptr == NULL)
		{
			MemArray [i].ptr = ptr;
			MemArray [i].size = size;
			if (i > Mi)
				Mi = i;
			break;
		}
	}
	
	//msg if no room
	if (MemArray [i].ptr == NULL)	//no room
	{
		assert (0);
		return;
	}

	return;
}
	

void chgMemRef (void* pold, void* pnew, size_t newsize)
{
	int		i;
	
	assert ((pold != NULL) && (pnew != NULL) && (newsize > 0));
	
	//make sure it's there
	for (i = 0; i <= Mi; i++)
	{
		if (MemArray [i].ptr == pold)
		{
			MemArray [i].ptr = pnew;
			MemArray [i].size = newsize;
			return;
		}
	}
	
	assert (0);
	return;
}

void delMemRef (void* ptr)
{
	int		i;
	
	assert (ptr != NULL);
	
	//make sure it's there
	for (i = 0; i <= Mi; i++)
	{
		if (MemArray [i].ptr == ptr)
		{
			MemArray [i].ptr = NULL;
			MemArray [i].size = 0;
			return;
		}
	}
	
	assert (0);
	return;
}
	
int getMemSize (void* ptr)
{
	int		i;
	
	assert (ptr != NULL);
	
	//make sure it's there
	for (i = 0; i <= Mi; i++)
	{
		if (MemArray [i].ptr == ptr)
			return MemArray [i].size;
	}
	
	assert (0);
	return 0;
}

void memAvail ()
{
	//this doesn't work, had 3 at start, 42 at end!!
	//MemStart = _freect (10000);
}

void memLeaks ()
{
	//int		i;
	
	//this doesn't work, had 3 at start, 42 at end!!
	//MemEnd = _freect (10000);
	//assert (MemEnd == MemStart);
	
	//all memory should be released
	//970204 ahem, now that this is working, got to take it out;
	//	hits every time i abort pgm
	//for (i = 0; i < ZMAXMEMREFS; i++)
	//	assert (MemArray [i].ptr == NULL);
}
	
	
//FROM HERE DOWN, NEED TO BE DEFINED AS NULL IF NOT DEBUG

//memory management stuff

#define ADDPTR(p,n)					addMemRef (p, n)
#define CHGMEM(pold,pnew,newsize)	chgMemRef (pold, pnew, newsize)
#define DELMEM(p)					delMemRef (p)
#define GETSIZE(p)					getMemSize (p)
#define MEMAVAIL					memAvail ()
#define MEMLEAKS					memLeaks ()
#define MEMSET(p,c,n)				memset (p, c, n)

//other stuff

#define ASSERTMARCRECORD(x)		SP_ValidateRecord ((LPPUSTR) &dValMsg, x, __FILE__, __LINE__)
	
	
#else	///////////////////// non-debug defines below here

//memory management stuff

#define ADDPTR(p,n)
#define CHGMEM(pold,pnew)
#define DELMEM(p)
#define GETSIZE(p)
#define MEMAVAIL
#define MEMLEAKS
#define MEMSET(p,c,n)

//other stuff

#define ASSERTMARCRECORD(x)

#endif




#endif
