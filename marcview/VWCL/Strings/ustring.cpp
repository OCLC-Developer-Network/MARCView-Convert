/* This file is not an official part of VWCL and is copyright
1997 by Don Box (dbox@develop.com).*/

#ifndef _USTRING_CPP
#define _USTRING_CPP

#include <tchar.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

/** Returns a new-ed wchar_t string based on psz.*/
wchar_t *uxdup(const char *psz)
{
    size_t cch = strlen(psz) + 1;
    wchar_t *pwszResult = (wchar_t*)malloc(cch * sizeof(wchar_t));
    if (pwszResult)
        mbstowcs(pwszResult, psz, cch);
    return pwszResult;
}

/** Returns a new-ed char string based on psz.*/
char *uxdup(const wchar_t *pwsz)
{
    size_t cch = wcslen(pwsz) + 1;
    char *pszResult = (char*)malloc(sizeof(char) * cch);
    if (pszResult)
        wcstombs(pszResult, pwsz, cch);
    return pszResult;
}

#endif

