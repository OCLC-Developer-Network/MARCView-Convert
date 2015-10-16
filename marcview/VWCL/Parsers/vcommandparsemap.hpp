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

#ifndef VCOMMANDPARSEMAP
#define VCOMMANDPARSEMAP

#include "../vstandard.h"

/** VCommandParseMap wraps parsing command strings into their
command-parameter parts. This is most useful when developing communications
protocols or CGI (Web) form processing. The idea is that you have a string
that contains Command=Parameter pairs. For instance: VString s contains the
following sequence: <SENDFILE=C:\TEST.HPP><DOIT=NOW>. Running this string
through the Parse() command would yield an array of string pointers pointing
to the commands and an array of string pointer pointing to the parameters
(the section after the = sign).

Example: command pointer 1 would point to the string SENDFILE
         parameter pointer 1 would point to the string C:\TEST.HPP
         command pointer 2 would point to the string DOIT
         parameter pointer 2 would point to the string NOW

Parameter values may be NULL. In reality <=> are not used, but rather
non-printing ASCII values of 1, 2, and 3 are used to designate the start
of the command, the start of a parameter, and the end of a command. This
allows the command string to contain other printable codes. The actual
delimiters can be any 3 characaters you wish, including the "human readable"
<=> which can be set by calling SetHumanReadable(), or each delimeter can
be set manually with SetDelimiters(). The marker in the string that defines
the start of a command is called the marker command and is represented by
default as MARKER_COMMAND (ASCII 1). The marker in the string that defines
the start of a parameter is called the marker parameter and is represented
by default as MARKER_PARAMETER  (ASCII 2). The marker in the string that
defines the end of the command-parameter pair is called the marker end and
is represented by default as MARKER_END (ASCII 3). MARKER_COMMAND_ANYTHING
is a special case, which means that when parsing a string, don't look for
the MARKER_COMMAND character, but rather ANYTHING is valid. This is mostly
used for parsing URL encoded strings from HTML forms where nothing
signifies the start of a name-value pair. In this case, = is used for the
parameter marker and & is used as the end marker.

Normally, commands in the map are considered to be case sensitive. That is:
COMMAND_1 is not the same as command_1. This generally improves performance
when case sensitive lookups can be performed. If this is not the desired
behaviour, call GetCaseSensitivity() and set the reference to VFALSE.*/
class VCommandParseMap
{
public:
	/** Markers (delimeters, ASCII values).*/
	enum	{	MARKER_COMMAND =			1,
				MARKER_PARAMETER =			2,
				MARKER_END =				3,
				MARKER_COMMAND_ANYTHING =	0,
			};

	/** Default constructor initializes class members and can optionally
	place the class into "Human Readable" mode, where the marker command
	is <, the marker parameter is =, and the marker end is > ASCII value.
	In addition, command lookup case sensitivity can be set.*/
	VCommandParseMap(	VBOOL bHumanReadable = VFALSE,
						VBOOL bCommandLookupCaseSensitive = VTRUE)
	{
		/* Initialize defaults.*/
		m_pszCommand = m_pszParameter =	NULL;
		m_bCommandLookupCaseSensitive =	bCommandLookupCaseSensitive;

		/* Initialize other members.*/
		InitializeMembers(VTRUE);

		if ( bHumanReadable )
			SetHumanReadable();
		else
		{
			m_nMarkerCommand =		MARKER_COMMAND;
			m_nMarkerParameter =	MARKER_PARAMETER;
			m_nMarkerEnd =			MARKER_END;
		}
	}

	/** Virtual destructor verifies internal object cleanup.*/
	virtual ~VCommandParseMap()
		{ InitializeMembers(VFALSE); }

	/** Find the command-parameter array index which has the requested
	command information. Returns index into the array on success, -1 on
	failure. If bParamRequires is VTRUE, the function will return the index
	only if the command and a parameter for the command exist.*/
	VINT			Find(	VSTRING_CONST	pszCommand,
							VBOOL			bParamRequired) const
	{
		VASSERT(m_bIsParsed)
		VASSERT(pszCommand)

		for ( VUINT i = 0; i < m_nAllocatedPairs; i++ )
		{
			if ( m_pszCommand[i] )
			{
				if ( IsCommandEqual(m_pszCommand[i], pszCommand) )
				{
					/* Does this command require parameters?*/
					if ( !bParamRequired || m_pszParameter[i] )
						return i;
					else
						break;
				}
			}
			else
				break;
		}

		return -1;
	}

	/** Find the command in the array, returning a pointer to it, or NULL
	if not found.*/
	VSTRING			FindCommand(VSTRING_CONST pszCommand) const
	{
		VASSERT(m_bIsParsed)

		VINT nIndex = Find(pszCommand, VFALSE);

		return (nIndex != -1) ? m_pszCommand[nIndex] : NULL;
	}

	/** Find the parameter for a command, returning pointer to it, or NULL
	if not found. If the length of the parameter is 0, NULL will also be
	returned. This allows commands to have empty (NULL) parameters.*/
	VSTRING			FindCommandParameter(VSTRING_CONST pszCommand) const
	{
		VASSERT(m_bIsParsed)

		VINT nIndex = Find(pszCommand, VTRUE);

		return	(nIndex != -1)
				? (*m_pszParameter[nIndex] != 0)
				? m_pszParameter[nIndex]
				: NULL
				: NULL;
	}

	/** Return a reference to the boolean value that determines case
	sensitivity used when performing command lookup's. The default value is
	VTRUE (can be set to VFALSE during construction), which means the
	command must exactly equal the string passed to functions that perform
	lookups based on name. If case insensitive comparison should be used,
	set the value to VFALSE.*/
	VBOOL&			GetCaseSensitivity() const
		{ return (VBOOL&)m_bCommandLookupCaseSensitive; }

	/** Return the command at a given index. If the index is out of bounds,
	NULL will be returned.*/
	VSTRING			GetCommandAt(VUINT nIndex) const
	{
		VASSERT(m_bIsParsed)
		return (nIndex < m_nLastCommandCount) ? m_pszCommand[nIndex] : NULL;
	}

	/** Return the last number of commands found in the last Parse()
	command. Valid even after un-parse is called.*/
	VUINT			GetLastCommandCount() const
		{ return m_nLastCommandCount; }

	/** Return the parameter at a given index. If the index is out of
	bounds, NULL will be returned.*/
	VSTRING			GetParameterAt(VUINT nIndex) const
	{
		VASSERT(m_bIsParsed)
		return (nIndex < m_nLastCommandCount) ? m_pszParameter[nIndex] : NULL;
	}

	/** Initialize members to default values.*/
	void			InitializeMembers(VBOOL bResetLastCommandCount = VTRUE)
	{
		m_bIsParsed =		VFALSE;
		m_nAllocatedPairs = 0;

		VDELETE_ARRAY_NULL(m_pszCommand)
		VDELETE_ARRAY_NULL(m_pszParameter)

		if ( bResetLastCommandCount )
			m_nLastCommandCount = 0;
	}

	/** Returns VTRUE if the string for which this object parses is in
	a parsed state, VFALSE otherwise.*/
	VBOOL			IsParsed() const
		{ return m_bIsParsed; }

	/** Parse a string into command-parameter pairs. pszCommand will be
	modified, and MUST remain in scope during entire life of this object.
	Commands are inside m_nMarkerCommand and m_nMarkerEnd delimeter tags
	and parameters follow the m_nMarkerParameter inside of each command.
	pszCommandString does not need to be 0 terminated, and this is why the
	buffer size is required. If the buffer size parameter is 0,
	pszCommandString must be a 0 terminated string so the length can be
	retreived. This function honors various posibilities for the marker
	beginning tag as defined by the class information at the top of this
	header file. See that section for more information.*/
	VUINT			Parse(	VSTRING	pszCommandString,
							VUINT	nBufferSize = 0,
							VUINT	nCommandCount = 0)
	{
		VASSERT(!m_bIsParsed)
		VASSERT(pszCommandString)

		/* As a check against something bad, check the first byte to
		make sure it is m_nMarkerCommand. If no marker command is used
		we have to skip this check.*/
		if (	m_nMarkerCommand != MARKER_COMMAND_ANYTHING &&
				*pszCommandString != m_nMarkerCommand )
			return 0;

		/* Get buffer size if not given.*/
		if ( !nBufferSize )
			nBufferSize = VSTRLEN(pszCommandString);

		VASSERT(nBufferSize)

		/* Count the number of m_nMarkerCommand's we find in the string
		to determine the number of command->parameter pairs we need to
		allocate.*/
		if ( !nCommandCount )
		{
			/* If no command marker is to be used, the command
			count MUST have already been determined!*/
			VASSERT(m_nMarkerCommand != MARKER_COMMAND_ANYTHING)

			for ( VUINT i = 0; i < nBufferSize; i++ )
			{
				if ( *(pszCommandString + i) == m_nMarkerCommand )
					nCommandCount++;
			}
		}

		VASSERT(nCommandCount)

		/* Save count of this commands parameters.*/
		m_nLastCommandCount = nCommandCount;

		VUINT nCount = 0;

		if ( nCommandCount && nBufferSize )
		{
			/* Re-Initialize members.*/
			InitializeMembers(VFALSE);

			/* Allocate arrays of command and parameter pairs.*/
			m_pszCommand =		VNEW VTCHAR*[nCommandCount];
			m_pszParameter =	VNEW VTCHAR*[nCommandCount];

			if ( m_pszCommand && m_pszParameter )
			{
				/* Clear buffers.*/
				VZEROMEMORY(m_pszCommand,	sizeof(VTCHAR*) * nCommandCount);
				VZEROMEMORY(m_pszParameter,	sizeof(VTCHAR*) * nCommandCount);

				/* Save allocated pair count.*/
				m_nAllocatedPairs = nCommandCount;

				/* Assume we will be able to parse without errors.*/
				m_bIsParsed = VTRUE;

				/* Walk string looking for m_nMarkerCommand start
				and m_nMarkerEnd pairs.*/
				VUINT nOffSet = 0;

				while ( nCount < m_nAllocatedPairs )
				{
					/* Is this a m_nMarkerCommand tag, or is
					MARKER_COMMAND_ANYTHING used?*/
					if (	m_nMarkerCommand == MARKER_COMMAND_ANYTHING ||
							*(pszCommandString + nOffSet) == m_nMarkerCommand )
					{
						VBOOL bFound = VFALSE;

						/* Find the end tag.*/
						for ( VUINT i = nOffSet + 1; i < nBufferSize; i++ )
						{
							/* Is this the end tag?*/
							if ( *(pszCommandString + i) == m_nMarkerEnd )
							{
								/* We found the end tag.*/
								bFound = VTRUE;

								/* Save command pointer.*/
								m_pszCommand[nCount] =
									pszCommandString +
									nOffSet +
									((m_nMarkerCommand ==
										MARKER_COMMAND_ANYTHING)
									? 0
									: 1);

								/* Terminate at this end tag.*/
								*(pszCommandString + i) = VTEXT('\0');

								/* Does the command have any parameters?
								If so, offset past the m_nMarkerParameter.*/
								VSTRING pszParams =
									VSTRCHR(	m_pszCommand[nCount],
												m_nMarkerParameter);

								if ( pszParams )
								{
									/* Terminate at parameter and save.*/
									*pszParams = VTEXT('\0');
									m_pszParameter[nCount] = pszParams + 1;
								}

								/* Offset count of pairs.*/
								nCount++;

								/* Offset index past current position.*/
								nOffSet = i + 1;

								/* Break internal loop.*/
								break;
							}
						}

						/* If we did not find a right tag, that is a
						parsing error.*/
						if ( !bFound )
						{
							VASSERT(VFALSE)
							nCount = 0;

							/* Cannot unparse string with no marker command.*/
							if ( m_nMarkerCommand != MARKER_COMMAND_ANYTHING )
								UnParse();
						}
					}
					else
						break;
				}
			}
		}

		if ( !nCount )
			InitializeMembers(VFALSE);

		return nCount;
	}

	/** Call after calling Parse(), and after UnParse(VFALSE), to replace
	string terminators back into string. The result is exactly like Parse(),
	but this uses current memory allocations to perform the parsing.*/
	void			ReParse()
	{
		/* These must be known to do a re-parse!*/
		VASSERT(m_pszCommand && m_pszParameter)
		VASSERT(m_nAllocatedPairs)
		VASSERT(m_nLastCommandCount)

		/* ReParse() is NOT valid in this case!*/
		VASSERT(m_nMarkerCommand != MARKER_COMMAND_ANYTHING)

		for ( VUINT i = 0; i < m_nAllocatedPairs; i++ )
		{
			if ( m_pszCommand[i] )
			{
				/* Fix the parameter first.*/
				if ( m_pszParameter[i] )
				{
					VASSERT(*(m_pszParameter[i] - 1) == m_nMarkerParameter)
					*(m_pszParameter[i] - 1) = VTEXT('\0');
				}

				/* Replace m_nMarkerEnd with terminator.*/
				ReplaceChar((m_pszParameter[i])
							? m_pszParameter[i]
							: m_pszCommand[i],
							m_nMarkerEnd,
							VTEXT('\0'));
			}
		}

		m_bIsParsed = VTRUE;
	}

	/** Set the marker command, parameter, and marker end delimiter
	characters.*/
	void			SetDelimiters(	VTCHAR nMarkerCommand,
									VTCHAR nMarkerParameter,
									VTCHAR nMarkerEnd)
	{
		m_nMarkerCommand =		nMarkerCommand;
		m_nMarkerParameter =	nMarkerParameter;
		m_nMarkerEnd =			nMarkerEnd;
	}

	/** Set the marker command, parameter, and marker end delimiter
	characters to <, =, and > respectively.*/
	void			SetHumanReadable()
		{ SetDelimiters(VTEXT('<'), VTEXT('='), VTEXT('>')); }

	/** Undo a parse, returning to original string. If bFreeClassData is
	VTRUE, the memory allocations for the commands and parameters arrays
	will be free'd. This means the ReParse() command cannot be used later
	to reparse the string. UnParsing is NOT allowed if the marker command
	is MARKER_COMMAND_ANYTHING.*/
	void			UnParse(VBOOL bFreeClassData = VTRUE)
	{
		/* UnParse() is NOT valid in this case!*/
		VASSERT(m_nMarkerCommand != MARKER_COMMAND_ANYTHING)

		if ( m_bIsParsed )
		{
			for ( VUINT i = 0; i < m_nAllocatedPairs; i++ )
			{
				if ( m_pszCommand[i] )
				{
					/* Fix the parameter first.*/
					if ( m_pszParameter[i] )
					{
						VASSERT(*(m_pszParameter[i] - 1) == VTEXT('\0'))
						*(m_pszParameter[i] - 1) = m_nMarkerParameter;
					}

					/* Replace terminator with m_nMarkerEnd.*/
					ReplaceChar(	(m_pszParameter[i])
									? m_pszParameter[i]
									: m_pszCommand[i],
									VTEXT('\0'),
									m_nMarkerEnd);
				}
			}
		}

		m_bIsParsed = VFALSE;

		if ( bFreeClassData )
			InitializeMembers(VFALSE);
	}

protected:
	/** Local function to determine if a command is equal to the specified
	string. This function takes into the account the user specified case
	sensitivity for commands.*/
	VBOOL			IsCommandEqual(	VSTRING_CONST pszCommand,
									VSTRING_CONST pszCompare) const
	{
		/* These must be known!*/
		VASSERT(pszCommand && pszCompare)

		if ( m_bCommandLookupCaseSensitive )
			return (VSTRCMP(pszCommand, pszCompare) == 0) ? VTRUE : VFALSE;

		return (VSTRCMP_NOCASE(pszCommand, pszCompare) == 0) ? VTRUE : VFALSE;
	}

	/** Local function which replaces first occurance of nFind character
	in pszBuffer with nReplace character.*/
	void			ReplaceChar(	VSTRING	pszBuffer,
									VTCHAR	nFind,
									VTCHAR	nReplace) const
	{
		VSTRING pszOffSet = pszBuffer;
		VASSERT(pszOffSet)

		while ( *pszOffSet != nFind )
			pszOffSet++;

		*pszOffSet = nReplace;
	}

	/** Embedded Members.*/
	VSTRING*		m_pszCommand;
	VSTRING*		m_pszParameter;
	VUINT			m_nAllocatedPairs;
	VUINT			m_nLastCommandCount;
	VBOOL			m_bIsParsed;
	VBOOL			m_bCommandLookupCaseSensitive;
	VTCHAR			m_nMarkerCommand;
	VTCHAR			m_nMarkerParameter;
	VTCHAR			m_nMarkerEnd;
};

#endif /** VCOMMANDPARSEMAP*/
