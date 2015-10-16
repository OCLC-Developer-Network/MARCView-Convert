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

#ifndef VHTMLSTREAMBLOCK
#define VHTMLSTREAMBLOCK

#include "../vstandard.h"
#include "../strings/vstring.hpp"
#include "vhtmlstream.hpp"

/** This class is the same as VHTMLStream, but overrides OnStream() to send
output to a VString object. In addition, the constructor sends the <html>
starting tag, while the destructor sends the </html> ending tag to the
stream. This is useful mostly when writing a complete HTML stream in memory
before sending it to the client from a VWebServerReqeust object. It allows
error checking before sending and also may increase performance by sending
the entire stream to the client at one time. Some clients, and some servers,
have a problem moving small bits and bytes sent to it in rapid, small
chunks, and use of this class can correct some of these issues by
transferring content in bulk.*/
class VHTMLStreamBlock : public VHTMLStream
{
public:
	/** Default constuctor allocates the VString object with a default, or
	custom, granularity size. The default of 8k is sufficient for most HTML
	generation needs, but this value can be adjusted here. Because this
	object uses a VString, the granularity is not a maximum value. It
	represents how much the string block grows each time an allocation is
	needed that would require more memory than what is available. See
	VString for more information. After the string block object is created,
	the <html> header is sent to the stream. See VHTMLStream for more
	information on the pWebServerRequest parameter.*/
	VHTMLStreamBlock(	VWebServerRequest*	pWebServerRequest = NULL,
						VUINT				nGranularitySize = 8192)
		: VHTMLStream(pWebServerRequest, VFALSE)
	{
		m_pStringBlock = NULL;
		Reset(nGranularitySize, VTRUE);
	}

	/** Virtual destructor sends the </html> tag string to the stream (if
	needed), and calls the base class OnStream() function to send the cached
	content to the output device. If an error occurred while building the
	string block, as determined by VString::GetErrorCount(), the content
	WILL NOT be sent, so your code should likely check for an error before
	destructing this object. You can then choose to send the content
	yourself, or choose an alternate path.*/
	virtual ~VHTMLStreamBlock()
	{
		/* Stream the </html> end tag?*/
		if ( m_bNeedsHTMLEndTag )
			HTMLEnd();

		/* If no error, send the content now.*/
		if (	m_pStringBlock &&
				!m_pStringBlock->GetErrorCount() && m_pStringBlock->String() )
			VHTMLStream::OnStream(	m_pStringBlock->String(),
									m_pStringBlock->GetLength());

		/* Delete the stringblock object.*/
		VDELETE_NULL(m_pStringBlock)
	}

	/** Return a pointer to the internal string object used for caching
	content.*/
	VString*		GetString() const
		{ return (VString*)m_pStringBlock; }

	/** Base class override resets the stream to empty, using the current
	granularity.*/
	virtual void	Reset(VBOOL bSendHTMLStartTag = VTRUE)
	{
		Reset(	(m_pStringBlock) ? m_pStringBlock->GetGranularity() : 0,
				bSendHTMLStartTag);
	}

	/** Reset the internal VString object to empty, set the error condition
	to VFALSE, and re-sends the <html> start tag if bSendHTMLStartTag is
	VTRUE. See the constructor documentation for more information.*/
	void			Reset(	VUINT nGranularitySize,
							VBOOL bSendHTMLStartTag = VTRUE)
	{
		if ( !m_pStringBlock )
			m_pStringBlock = VNEW VString;
		else
			m_pStringBlock->Empty();

		/* Stream the html start tag.*/
		if ( m_pStringBlock )
		{
			/* Reset this flag for now.*/
			m_bNeedsHTMLEndTag = VFALSE;

			m_pStringBlock->GetGranularity() =
				(nGranularitySize) ? nGranularitySize : 8192;

			VHTMLStream::Reset(bSendHTMLStartTag);
		}
	}

protected:
	/** Override sends data to string block object.*/
	virtual void	OnStream(	VSTRING_CONST	pszText,
								VULONG			nLength = 0)
	{
		/* We need to check for pszText being NULL and be a NOOP.*/
		if ( pszText && m_pStringBlock )
			m_pStringBlock->Append(pszText);
		/* This will likely never happen, but if we don't have a VString,
		have the base class send pszText anyway.*/
		else
			VHTMLStream::OnStream(pszText, nLength);
	}

	/** Embedded Members.*/
	VString*		m_pStringBlock;
};

#endif /* VHTMLSTREAMBLOCK*/
