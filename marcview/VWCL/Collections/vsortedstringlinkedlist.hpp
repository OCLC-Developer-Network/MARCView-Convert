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

#ifndef VSORTEDSTRINGLINKEDLIST
#define VSORTEDSTRINGLINKEDLIST

#include "../vstandard.h"
#include "../numbers/vbitmask.hpp"
#include "../parsers/vdelimitedstring.hpp"
#include "vlinkedliststringitem.hpp"
#include "vlinkedlistmanager.hpp"

#define BEEP WAppObject.Beep (WBeepHand)


/** VSortedStringLinkedList manages a linked list of sorted strings. In
addition, this class can be used to store a list of unsorted strings by
using the option bit OPTION_UNSORTED, which has a higher priority within
the class than the other option bits which control sorting. The
VUnSortedStringLinkedList class is a simple derivation of this class which
sets the OPTION_UNSORTED bit automatically during construction.*/
class VSortedStringLinkedList : public VLinkedListManager
{
public:
	/** Options that apply to this class. These default to OPTION_DEFAULT,
	but can be set by using GetOptions().*/
	enum	{				OPTION_SORT_ASCENDING =		0x0001,
							OPTION_SORT_DESCENDING =	0x0002,
							OPTION_NO_DUPLICATES =		0x0004,
							OPTION_CASE_SENSITIVE =		0x0008,
							OPTION_UNSORTED =			0x0010,
							OPTION_DEFAULT =			OPTION_SORT_ASCENDING,
			};

	/** Same as Add(VLinkedListStringItem*).*/
	VBOOL operator += (VLinkedListStringItem* pItem)
		{ return Add(pItem); }

	/** Same as Add(VSTRING_CONST).*/
	VLinkedListStringItem* operator += (VSTRING_CONST pszString)
		{ return Add(pszString); }

	/** Same as Copy().*/
	void operator += (VSortedStringLinkedList const& original)
		{ Copy(original); }

	/** Same as Copy(), but free's the current linked list objects first
	and makes the options for this class match original.*/
	void operator = (VSortedStringLinkedList const& original)
	{
		Free();
		m_Options = original.m_Options;
		Copy(original);
	}

	/** Default constructor initializes options to default. These can also
	be set with GetOptions().*/
	VSortedStringLinkedList(VUINT nOptions = OPTION_DEFAULT)
		{ m_Options = nOptions; }

	/** Copy constructor.*/
	VSortedStringLinkedList(VSortedStringLinkedList const& original)
	{
		m_Options = original.m_Options;
		Copy(original);
	}

	/** Add a string object to the list in sorted order. Return VTRUE on
	success, VFALSE on failure. pItem must be valid, however the string
	object contained within it is not required to have a string, it can
	be empty.*/
	VBOOL					Add(VLinkedListStringItem* pItem)
	{
		/* pItem must be known! It can be a NULL string however.*/
		VASSERT(pItem)

		VSTRING_CONST pszString = pItem->String();

		/* Should we check for duplicate?*/
		if (	pszString &&
				m_Options.IsSet(OPTION_NO_DUPLICATES) && Find(pszString) )
			return VFALSE;

		/* No other strings in list? No sorting to be done?*/
		if ( !m_pFirstItem || m_Options.IsSet(OPTION_UNSORTED) )
			VLinkedListManager::Add(pItem);
		/* NULL string? Add at beginning of list.*/
		else if ( !VSTRLEN_CHECK(pszString) )
			VLinkedListManager::Add(pItem, VTRUE);
		/* Insert into linked list at sorted location.*/
		else
		{
			/* Find the item that comes before this one alphabetically,
			and insert there.*/
			VLinkedListStringItem* pThisItem =
				GetFirstItem();

			VLinkedListStringItem* pPreviousItem =
				NULL;

			VBOOL bCaseSensitive =
				m_Options.IsSet(OPTION_CASE_SENSITIVE);

			VBOOL bAscending =
				m_Options.IsSet(OPTION_SORT_ASCENDING);

			while ( pThisItem )
			{
				VBOOL	bBefore =	VFALSE;
				VINT	nInsert =	0;

				/* If this string is empty, insert before.*/
				if ( pThisItem->IsEmpty() )
					bBefore = VTRUE;
				else
					nInsert =	(bCaseSensitive)
								? VSTRCMP(	pszString,
											pThisItem->String())
								: VSTRCMP_NOCASE(	pszString,
													pThisItem->String());

				if (	bBefore ||
						(bAscending && nInsert <= 0) ||
						(!bAscending && nInsert >= 0) )
				{
					/* Insert into linked list before this item.*/
					if ( pPreviousItem )
						pPreviousItem->m_pNextItem = pItem;
					else
						m_pFirstItem = pItem;

					pItem->m_pNextItem = pThisItem;

					/* Offset count in base class.*/
					m_nItemCount++;

					/* Exit loop now.*/
					break;
				}
				else
				{
					pPreviousItem = pThisItem;
					pThisItem =		pThisItem->GetNextItem();

					/* Did we hit the end of the list? If yes, insert now.*/
					if ( !pThisItem )
					{
						pPreviousItem->m_pNextItem =	pItem;
						m_pLastItem =					pItem;

						/* Offset count in base class.*/
						m_nItemCount++;
					}
				}
			}
		}

		return VTRUE;
	}

	/** Add a string to the list, creating a VLinkedListStringItem object
	to hold it. This function will create a copy of pszString when it
	creates the VLinkedListStringItem to hold it. Returns
	VLinkedListStringItem object pointer on success, NULL on failure.*/
	VLinkedListStringItem*	Add(VSTRING_CONST pszString)
	{
		//BEEP;
		
		/* Should we check for duplicate?*/
		if (	!pszString ||
				(m_Options.IsSet(OPTION_NO_DUPLICATES) && Find(pszString)) )
			return NULL;

		/* Allocate VLinkedListStringItem item object and initialize it.*/
		VLinkedListStringItem* pNewItem =
			VNEW VLinkedListStringItem(pszString);

		/* Add to list.*/
		if ( pNewItem && pNewItem->String() )
		{
			/* Temporarily un-set no duplicates so we don't check again.*/
			m_Options.Save();
			m_Options.Remove(OPTION_NO_DUPLICATES);

			/* Add to linked list now.*/
			Add(pNewItem);

			/* Reset use specified options.*/
			m_Options.Restore();
		}
		else
			VDELETE_NULL(pNewItem)

		return pNewItem;
	}

	/** Copy all items from another linked list to this one. This function
	copies the internal strings objects, so it is valid for this object to
	outlive the original. However, this class only copies the objects as
	VLinkedListStringItem's, so if a derived class is actually using a
	class derived from VLinkedListStringItem, this copy may not be correct.
	Returns VTRUE if at least one string was copied from the original,
	VFALSE otherwise. If this container class is not empty when this method
	is called, the strings from the original list will be merged and sorted
	as needed with the existing strings. This function honors the other
	options set, such as eliminating duplicates and sorting order.*/
	VBOOL					Copy(VSortedStringLinkedList const& original)
	{
		VBOOL					bResult = VFALSE;
		VLinkedListStringItem*	pItem = original.GetFirstItem();

		while ( pItem )
		{
			if ( Add(pItem->String()) )
				bResult = VTRUE;

			pItem = pItem->GetNextItem();
		}

		return bResult;
	}

	/** Find an existing string item in the list. Returns
	VLinkedListStringItem pointer on success, NULL on failure.*/
	VLinkedListStringItem*	Find(VSTRING_CONST pszString) const
	{
		if ( pszString )
		{
			VLinkedListStringItem* pItem = GetFirstItem();

			VBOOL bCaseSensitive = m_Options.IsSet(OPTION_CASE_SENSITIVE);

			while ( pItem )
			{
				VSTRING_CONST pszThisString = pItem->String();

				if ( pszThisString )
				{
					if ( bCaseSensitive )
					{
						if ( VSTRCMP(pszString, pszThisString) == 0 )
							return pItem;
					}
					else
					{
						if ( VSTRCMP_NOCASE(pszString, pszThisString) == 0 )
							return pItem;
					}
				}

				pItem = pItem->GetNextItem();
			}
		}

		return NULL;
	}

	/** Get the first VLinkedListStringItem in the list, or NULL if none
	are in the list.*/
	VLinkedListStringItem*	GetFirstItem() const
		{ return (VLinkedListStringItem*)m_pFirstItem; }

	/** Return a reference to the options that can be applied to this class.*/
	VBitmask&				GetOptions() const
		{ return (VBitmask&)m_Options; }

	/** Given a string (like a file path) that is separated by nDelimiterChar
	characters, parse into component parts and add to list. If this
	container class is not empty when this method is called, the strings
	parsed from pszString will be merged and sorted as needed with the
	existing strings. This function honors the other options set, such as
	eliminating duplicates and sorting order. Returns a pointer to the first
	new string (VLinkedListStringItem) added to the list.*/
	VLinkedListStringItem*	ParseInto(
		VSTRING_CONST	pszString,
		VTCHAR			nDelimiterChar = VFILE_PATH_SEP_CHAR)
	{
		VASSERT(VSTRLEN_CHECK(pszString))

		VLinkedListStringItem* pFirstItem = NULL;

		if ( VSTRLEN_CHECK(pszString) )
		{
			/* Copy string since we parse it.*/
			VSTRING pszCurrent;
			VString s(pszString);
			VDelimitedString ds(s, nDelimiterChar);

			//000601 "Assignment found in boolean expression" -- OK
			while ( (pszCurrent == ds.Next()) )
			{
				if ( !pFirstItem )
					pFirstItem = Add(pszCurrent);
				else
					Add(pszCurrent);
			}
		}

		return pFirstItem;
	}
	

	//000502 new function to return string of all items
	VString Export ()
	{
		//BEEP;
		
		VLinkedListStringItem* pItem = GetFirstItem();
		VString	sOut;

		while ( pItem )
		{
			sOut.Append (pItem->String());

			//070830 this sub is for building an Export TT. we no longer want the
			//		trailing \t, too hard to read and TT parses OK without it
			//sOut.Append ("\t-\t\n,");	//000601 for Export TT. 041101 added hyphen
			sOut.Append ("\t-\n,");

			pItem = pItem->GetNextItem();
		}
		
		return sOut.String ();
	}

protected:
	/** This class holds the options available to this class.*/
	VBitmask				m_Options;
};

#endif /* VSORTEDSTRINGLINKEDLIST*/
