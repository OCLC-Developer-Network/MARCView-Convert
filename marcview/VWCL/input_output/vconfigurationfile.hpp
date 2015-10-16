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

#ifndef VCONFIGURATIONFILE
#define VCONFIGURATIONFILE

#include "../vstandard.h"
#include "../collections/vlinkedlistitem.hpp"
#include "../collections/vlinkedlistmanager.hpp"
#include "../strings/vstring.hpp"
#include "vlinefilereader.hpp"

/* Forward declaration for friend association.*/
class VConfigurationFile;

/** Objects of this type represent a single command and parameter pair from
a configuration file read by the VConfigurationFile class. Only
VConfigurationFile is allowed to create objects of this type. Please see the
VConfigurationFile class for more information.*/
class VConfigurationPair : public VLinkedListItem
{
friend class VConfigurationFile;

public:
	/** Return a reference to the command name string.*/
	VString&			GetCommand() const
		{ return (VString&)m_strCommand; }

	/** Return a reference to the parameter name string.*/
	VString&			GetParameter() const
		{ return (VString&)m_strParameter; }

	/** Return VTRUE of VFALSE based on the parameter value. The strings
	True, Yes, On and 1 are considered VTRUE, while everything else is
	considered VFALSE, as is NULL. The strings are not case sensitive. It
	is not valid to use this function for parameter strings that contain
	multiple parameters separated by a delimiter.*/
	VBOOL				GetParameterBOOL() const
		{ return IsStringTrue(m_strParameter); }

	/** Return the next object of this type in the linked list.*/
	VConfigurationPair*	GetNextPair() const
		{ return (VConfigurationPair*)m_pNextItem; }

	/** Given a string, return VTRUE if it is considered true. The strings
	True, Yes, On and 1 are considered VTRUE, while everything else is
	considered VFALSE, as is NULL.*/
	static VBOOL		IsStringTrue(VSTRING_CONST pszString)
	{
		if ( pszString )
		{
			VSTRING_CONST TRUE_STRINGS[] =
				{
					VTEXT("TRUE"),
					VTEXT("YES"),
					VTEXT("ON"),
					VTEXT("1")
				};

			for ( VUINT i = 0; i < VARRAY_SIZE(TRUE_STRINGS); i++ )
			{
				if ( VSTRCMP_NOCASE(pszString, TRUE_STRINGS[i]) == 0 )
					return VTRUE;
			}
		}

		return VFALSE;
	}

protected:
	/** Constructor can only be called by VConfigurationFile.*/
	VConfigurationPair(	VSTRING_CONST pszCommand,
						VSTRING_CONST pszParameter)
	{
		/* The command is required.*/
		VASSERT(VSTRLEN_CHECK(pszCommand))

		/* Save values.*/
		m_strCommand =		pszCommand;
		m_strParameter =	pszParameter;
	}

	/** Destructor cannot be called directly on object.*/
	~VConfigurationPair()
		{;}

	/** Embedded Members.*/
	VString					m_strCommand;
	VString					m_strParameter;
};

/** VConfigurationFile is used to read configuration files. This is similar
to both the Windows style INI file reading, and the UNIX style CONF file
reading, though not exact implementations of either. This class is used to
process configuration files that are plain text, where options are
expressed as command and parameter pairs. For instance, if this line
appears in a configuration file:

FONT=Sans Serif

FONT is considered the command, and Sans Serif is the parameter. Commands
are always required, but parameters are optional. So this line is also
valid, though quite meaningless:

FONT

Commands and parameters are separated by the equal (=) sign. White space is
removed from the command and parameters. Only white space that appears left
and right are removed. Commands and parameters are allowed to have internal
spaces. In other words, this is also valid:

OPTIONS 1 = My Test Option

which means that the command is OPTION 1 and the parameter is My Test
Option. This class keeps these command and parameter pairs as a linked list
of VConfigurationPair objects. It is normally valid to have more than one
command in the list. For example, this results in 2 VConfigurationPair
objects being created:

COMMAND1=PARAMETER1
COMMAND1=PARAMETER2

This could be difficult to manage in your code. Support for overlapping, or
sharing, of parameters is provided by using a parameter terminator
character. This can be anything you desire, printable or not. This
terminator can be set during construction or afterwards with the
GetParameterTerminator() function. You would normally want to set
this value before calling the Read() function to read the configuration
file. As an example, assume you choose the comma (,) as your parameter
string terminator, the above example would be turned into a single
VConfigurationPair object whose parameter would be:

PARAMETER1,PARAMETER2

rather than having two pair objects with the same command name. If you use
this string overlap, it is a simple matter of using the VDelimitedString
class to walk these strings at appropriate times.*/
class VConfigurationFile : protected VLineFileReader
{
public:
	/** Constructor initializes class.When reading a configuration file, it
	is considered valid to have more than one entry for each command. If you
	would rather have only one VConfigurationPair object created for each
	command, pass a non-0 value for nParameterTerminator. This will cause
	only one command to appear in the linked list, but the parameter string
	will contain multiple strings, each separated by the nParameterTerminator
	character. These can easily be "walked" using the VDelimitedString
	class. This terminator can also be set after construction by using the
	GetParameterTerminator() function. If used, it should be set before
	calling the Read() function.*/
	VConfigurationFile(VTCHAR nParameterTerminator = 0)
		{ m_nParameterTerminator = nParameterTerminator; }

	/** Virtual destructor does nothing.*/
	virtual ~VConfigurationFile()
		{;}

	/** Add a new command and parameter pair. pszCommand is required, but the
	parameter is optional. If multiple parameter strings are combined using
	the parameter terminator, and a command pair already exists with the
	name of pszCommand, a new pair will not be created. Instead, it will
	honor the parameter terminator and append the parameter to the existing
	pair object. Unlike the OnLine(), function which processes lines from
	the configuration file, this function does not removed white space from
	the command and parameter string. This must be done by calling code if
	desired. NOTE: An important side affect of this function is that since
	it is virtual, you can override it in a derived class. You could then
	implement your own management of pszCommand and pszParameter. In other
	words, you can handle this function, and return NULL. This will prevent
	the pair from going into the internal linked list. You could for
	instance, handle your own processing and configuration settings during
	an override of this function, and not deal with a two part Read-Parse
	cycle.*/
	virtual VConfigurationPair*	AddNewPair(	VSTRING_CONST pszCommand,
											VSTRING_CONST pszParameter)
	{
		VASSERT(VSTRLEN_CHECK(pszCommand))

		if ( !pszCommand )
			return NULL;

		VConfigurationPair* pPair = NULL;

		/* Are we combining parameters?*/
		if ( m_nParameterTerminator )
		{
			/* Try to find an existing command with the same name.*/
			pPair = FindCommand(pszCommand);

			/* Did we find it? Do we have a string to add to it?*/
			if ( pPair && pszParameter )
			{
				/* Get parameter string reference.*/
				VString& sParameter = pPair->GetParameter();

				/* Is the parameter string not empty? If not, append
				terminator character.*/
				if ( sParameter.IsNotEmpty() )
					sParameter.Append(m_nParameterTerminator);

				/* Append this parameter.*/
				sParameter += pszParameter;
			}
		}

		/* Do we need to allocate a new pair object?*/
		if ( !pPair )
		{
			/* Allocate a new command-parameter pair object.*/
			pPair = VNEW VConfigurationPair(pszCommand, pszParameter);

			if ( pPair )
				m_listPairs.Add(pPair);
		}

		return pPair;
	}

	/** Eat leading white space from string pointer, returning a pointer
	to the new string offset.*/
	VSTRING						EatLeadingWhiteSpace(VSTRING pszString)
	{
		/* End of string?*/
		while ( pszString && *pszString != VTEXT('\0') )
		{
			/* If this is a space or a non-printable character, keep going.*/
			if ( isspace(*pszString) || !isprint(*pszString) )
				pszString++;
			else
				return pszString;
		}

		return (pszString && *pszString != VTEXT('\0')) ? pszString : NULL;
	}

	/** Eat trailing white space from string pointer, terminating pszString
	in the process. If pszStringEnd is NULL, it will be calculated based on
	the length of pszString. It represents the current end of the string
	when this function is called.*/
	void						EatTrailingWhiteSpace(	VSTRING pszString,
														VSTRING pszStringEnd)
	{
		if ( !pszString )
			return;

		/* Do we need to determine string length?*/
		if ( !pszStringEnd )
			pszStringEnd = pszString + VSTRLEN_CHECK(pszString);

		VBOOL bWhiteSpace = VFALSE;

		/* Beginning of string?*/
		while ( pszString && pszStringEnd && pszString != pszStringEnd )
		{
			/* If this is a space or a non-printable character, keep going.*/
			if ( isspace(*pszStringEnd) || !isprint(*pszStringEnd) )
			{
				pszStringEnd--;
				bWhiteSpace = VTRUE;
			}
			else
			{
				/* Terminate string now, if we did anything to it.*/
				if ( bWhiteSpace )
					*(pszStringEnd + 1) = VTEXT('\0');

				return;
			}
		}
	}

	/** Return the first VConfigurationPair object in the list. Valid only
	after calling Read().*/
	VConfigurationPair*			GetFirstPair() const
		{ return (VConfigurationPair*)m_listPairs.m_pFirstItem; }

	/** Return a reference to the parameter terminator character. See
	constructor for more information.*/
	VTCHAR&						GetParameterTerminator() const
		{ return (VTCHAR&)m_nParameterTerminator; }

	/** Return the first VConfigurationPair object that has the command
	named pszCommand. This is not case sensitive. NULL will be returned if
	the command could not be found.*/
	VConfigurationPair*			FindCommand(VSTRING_CONST pszCommand) const
	{
		VASSERT(VSTRLEN_CHECK(pszCommand))

		if ( pszCommand )
		{
			VConfigurationPair*	pPair = GetFirstPair();

			while ( pPair )
			{
				if ( pPair->GetCommand() == pszCommand )
					return pPair;

				pPair = pPair->GetNextPair();
			}
		}

		return NULL;
	}

	/** Given a command string, return the parameter string object that is
	associated with it, or NULL on failure.*/
	VString*					FindParameter(VSTRING_CONST pszCommand) const
	{
		VConfigurationPair* pPair = FindCommand(pszCommand);

		if ( pPair )
			return &pPair->GetParameter();

		return NULL;
	}

	/** Read the specified configuration file. This will reset the internal
	linked list of pair objects.*/
	VBOOL						Read(VSTRING_CONST pszFileName)
	{
		/* Clear linked list.*/
		m_listPairs.Free();

		/* Let base class do the real work of reading the file.*/
		return VLineFileReader::Read(pszFileName, VFALSE);
	}

protected:
	/** This function is called by the base class for each line in the file.
	The base class implementation parses the line as needed. This function
	validates and parses the command and parameter strings, and removed
	unwanted white space. It then adds them to the internal linked list as
	needed using the AddNewPair() function.*/
	virtual VBOOL				OnLine(	VSTRING	pszString,
										VUINT	nLineNumber)
	{
		VSTRING pszCommand = EatLeadingWhiteSpace(pszString);

		/* Look for comment line. These start with # or ;. Also check for =
		sign which is invalid.*/
		if (	pszCommand &&
				*pszCommand != VTEXT('#') &&
				*pszCommand != VTEXT(';') &&
				*pszCommand != VTEXT('=') )
		{
			/* Initialize parameter value.*/
			VSTRING pszParameter = NULL;

			/* See if we can find an = sign.*/
			VSTRING pszEqual = VSTRCHR(pszCommand, VTEXT('='));

			if ( pszEqual )
			{
				/* Terminate string here.*/
				*pszEqual = VTEXT('\0');

				/* Remove trailing white space from around command part.*/
				EatTrailingWhiteSpace(pszCommand, pszEqual - 1);

				/* Eat leading white space are parameter.*/
				pszParameter = EatLeadingWhiteSpace(pszEqual + 1);

				/* Do we have a parameter? If yes, eat trailing white
				space around it.*/
				if ( pszParameter )
					EatTrailingWhiteSpace(pszParameter, NULL);
			}
			/* No equal sign, just use commmand part.*/
			else
				EatTrailingWhiteSpace(pszCommand, NULL);

			/* Add new pair now.*/
			AddNewPair(pszCommand, pszParameter);
		}

		/* Next line.*/
		return VTRUE;
	}

	/** Embedded Members.*/
	VLinkedListManager		m_listPairs;
	VTCHAR					m_nParameterTerminator;
};

#endif /* VCONFIGURATIONFILE*/
