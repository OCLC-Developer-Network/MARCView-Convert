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

#ifndef VHTMLSTREAM
#define VHTMLSTREAM

#include "../vstandard.h"
#include "../networking/vwebserverrequest.hpp"
#include <iostream.h>

/** Macro to use instead of assert() within the HTML classes. Sends output
to the stream.*/
#ifdef VWCL_DEBUG
	#define VASSERT_HTML(expr, reason)	if ( !expr ) Stream(VTEXT(reason));
#else
	#define VASSERT_HTML(expr, reason)
#endif

/** HTML Tag Strings to prevent string duplication.*/
static VSTRING_CONST VHTML_STRINGS[] =
		{	VTEXT("html"),
			VTEXT("strong"),
			VTEXT("a"),
			VTEXT("href"),
			VTEXT("p"),
			VTEXT("head"),
			VTEXT("br"),
			VTEXT("body"),
			VTEXT("blockquote"),
			VTEXT("target"),
			VTEXT("title"),
			VTEXT("meta"),
			VTEXT("b"),
			VTEXT("font"),
			VTEXT("code"),
			VTEXT("mailto:"),
			VTEXT("em"),
			VTEXT("table"),
			VTEXT("tr"),
			VTEXT("td"),
			VTEXT("div"),
			VTEXT("center"),
			VTEXT("select"),
			VTEXT("form"),
			VTEXT("option"),
			VTEXT("img")
		};

/** Enumeration of above HTML strings.*/
enum	{	VHTML_HTML,
			VHTML_STRONG,
			VHTML_ADDRESS,
			VHTML_HREF,
			VHTML_PARAGRAPH,
			VHTML_HEAD,
			VHTML_BREAK,
			VHTML_BODY,
			VHTML_BLOCKQUOTE,
			VHTML_TARGET,
			VHTML_TITLE,
			VHTML_META,
			VHTML_BOLD,
			VHTML_FONT,
			VHTML_CODE,
			VHTML_MAILTO,
			VHTML_EMPHASIS,
			VHTML_TABLE,
			VHTML_TABLE_ROW,
			VHTML_TABLE_DATA,
			VHTML_DIV,
			VHTML_CENTER,
			VHTML_SELECT,
			VHTML_FORM,
			VHTML_OPTION,
			VHTML_IMG
		};

/** Pseudonymns for the above enumeration.*/
enum	{	VHTML_A =			VHTML_ADDRESS,
			VHTML_P =			VHTML_PARAGRAPH,
			VHTML_BR =			VHTML_BREAK,
			VHTML_B =			VHTML_BOLD,
			VHTML_TABLE_CELL =	VHTML_TABLE_DATA,
			VHTML_IMAGE =		VHTML_IMG,
		};

/** Macro to return strings from the VHTML_STRINGS array. In debug builds
these are functions that perform bounds checking. In release builds, they
simply return the string at a given index.*/
#ifdef VWCL_DEBUG
	VSTRING_CONST VHTML_STRING(VUINT nIndex)
	{
		VASSERT(nIndex < VARRAY_SIZE(VHTML_STRINGS))
		return VHTML_STRINGS[nIndex];
	}
#else
	#define	VHTML_STRING(nIndex)	VHTML_STRINGS[nIndex]
#endif

/** Base class for serialization of an HTML stream. This base class sends
all output to the OnStream() virtual function, which sends the output to
VWebServerRequest::SendContent(), if a call has been made to
WebServerRequest(), which sets a pointer to this object. Otherwise, output
will go to stdout. VHTMLStreamBlock can be used to send output a VString
object, which essentially caches the content and then sends it in bulk to
an output device. This can be very important for web werver applications, so
please see VHTMLStreamBlock for more information.*/
class VHTMLStream
{
public:
	/** Default constructor initializes internal error flag to VFALSE, and
	can optionally set a pointer to a VWebServerRequest object which, via
	the VWebServerRequest::SendContent() function, will be the recipient of
	streamed HTML content. If left as a NULL pointer, or set to NULL with a
	call to WebServerRequest(), all output will go to stdout. If
	bSendHTMLStartTag is VTRUE, the <html> begin tag will be sent, otherwise
	it will not. Since derived classes would miss the streaming of this tag,
	it is disabled by default and derived classes must implement the calls
	themselves. However, if sent by this class, the trailing ending </html>
	tag will also be written to the stream during destruction.*/
	VHTMLStream(	VWebServerRequest*	pWebServerRequest = NULL,
					VBOOL				bSendHTMLStartTag = VFALSE)
	{
		m_bNeedsHTMLEndTag =	VFALSE;
		m_pWebServerRequest =	pWebServerRequest;

		if ( bSendHTMLStartTag )
		{
			HTMLBegin();
			m_bNeedsHTMLEndTag = VTRUE;
		}
	}

	/** Virtual destructor writes the </html> end tag only if not already
	written, and then only if needed.*/
	virtual ~VHTMLStream()
	{
		if ( m_bNeedsHTMLEndTag )
			HTMLEnd();
	}

	/** Stream text. Same as calling the Stream(VSTRING_CONST) function.*/
	void operator += (VSTRING_CONST pszText)
		{ Stream(pszText); }

	/** Write a single character to the stream.*/
	void				Char(VTCHAR nChar)
	{
		VTCHAR sz[2] = {nChar, '\0'};
		Stream(sz);
	}

	/** Begin a tag by streaming its leading tag. This function prefaces the
	write with <, followed, by the tag name, optional attributes prefaced
	with a space, and then a trailing > character is bTrailer is VTRUE.*/
	void				BeginTag(	VUINT			nID,
									VSTRING_CONST	pszAttributes = NULL,
									VBOOL			bTrailer = VTRUE)
	{
		Leader();
		Stream(nID);

		if ( pszAttributes )
		{
			Space();
			Stream(pszAttributes);
		}

		if ( bTrailer )
			Trailer();
	}

	/** Stream the <blockquote> tag and optional attributes to start the
	beginning of blockquoted text.*/
	void				BlockQuoteBegin(VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(VHTML_BLOCKQUOTE, pszAttributes); }

	/** Stream the </blockquote> tag to end blockquoted text.*/
	void				BlockQuoteEnd()
		{ EndTag(VHTML_BLOCKQUOTE); }

	/** Stream the <body> tag and optional attributes to start the body
	of the HTML content.*/
	void				BodyBegin(VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(VHTML_BODY, pszAttributes); }

	/** Stream the </body> tag to end the body of HTML content.*/
	void				BodyEnd()
		{ EndTag(VHTML_BODY); }

	/** Stream a <b> tag and optional attributes to turn bolding of text on.*/
	void				BoldBegin(VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(VHTML_BOLD, pszAttributes); }

	/** Stream a </b> tag to turn bolding of text off.*/
	void				BoldEnd()
		{ EndTag(VHTML_BOLD); }

	/** Stream a <br> tag and optional attributes to cause a soft break.*/
	void				Break(VSTRING_CONST pszAttributes = NULL)
		{ SingletonTag(VHTML_BREAK, pszAttributes); }

	/** Stream a <center> tag and optional attributes.*/
	void				CenterBegin(VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(VHTML_CENTER, pszAttributes); }

	/** Stream a </center> tag.*/
	void				CenterEnd()
		{ EndTag(VHTML_CENTER); }

	/** Write a <code> tag and optional attributes to turn code style
	text on.*/
	void				CodeBegin(VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(VHTML_CODE, pszAttributes); }

	/** Write a </code> tag to turn code style text off.*/
	void				CodeEnd()
		{ EndTag(VHTML_CODE); }

	/** Stream a <div> tag and optional attributes.*/
	void				DivBegin(VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(VHTML_DIV, pszAttributes); }

	/** Stream a </div> tag.*/
	void				DivEnd()
		{ EndTag(VHTML_DIV); }

	/** Write a <em> tag and optional attributes to turn emphasis style
	text on.*/
	void				EmphasisBegin(VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(VHTML_EMPHASIS, pszAttributes); }

	/** Write a </em> tag to turn emphasis style text off.*/
	void				EmphasisEnd()
		{ EndTag(VHTML_EMPHASIS); }

	/** End a tag by streaming its end tag. This function prefaces the write
	with <, followed, by the slash, the tag name, and then a trailing >
	character.*/
	void				EndTag(VUINT nID)
	{
		Leader();
		Slash();
		Stream(nID);
		Trailer();

		/* Set flag to indicate we should not write the </html> end tag?*/
		if ( nID == VHTML_HTML )
			m_bNeedsHTMLEndTag = VFALSE;
	}

	/** Write the equal (=) character to the stream.*/
	void				Equal()
		{ Char(VTEXT('=')); }

	/** Write a <font> tag and optional attributes to the stream.*/
	void				FontBegin(VSTRING_CONST pszAttributes)
		{ BeginTag(VHTML_FONT, pszAttributes); }

	/** Write a </font> ending tag to the stream.*/
	void				FontEnd()
		{ EndTag(VHTML_FONT); }

	/** Write a <form> tag and optional attributes to the stream.*/
	void				FormBegin(VSTRING_CONST pszAttributes)
		{ BeginTag(VHTML_FORM, pszAttributes); }

	/** Write a </form> ending tag to the stream.*/
	void				FormEnd()
		{ EndTag(VHTML_FORM); }

	/** Stream a <head> begin tag and optional attributes to the stream.*/
	void				HeadBegin(VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(VHTML_HEAD, pszAttributes); }

	/** Stream a </head> end tag.*/
	void				HeadEnd()
		{ EndTag(VHTML_HEAD); }

	/** Write a HREF (link) tag to the HTML stream. This function writes the
	entire tag. pszURL is the URL to link to an is required. pszDisplayName
	is the text the user will see to click on, and pszTargetFrame is the
	frame to use to display the new link in. The display name and target
	frame can be NULL. This function can also be used for creating non-http
	links. The protocol can be specified in pszProtocol. If not given, it
	is assumed by this function that you have alredy specified it in the
	pszURL parameter.*/
	void				HREF(	VSTRING_CONST pszURL,
								VSTRING_CONST pszDisplayName = NULL,
								VSTRING_CONST pszTargetFrame = NULL,
								VSTRING_CONST pszProtocol = NULL)
	{
		VASSERT_HTML(pszURL, "VHTMLStream::HREF() requires a URL and pszURL is NULL")

		/* Build the tag parameters like this one.*/
		/* <a href="http://www.link.com/" target="DYNAMIC">Link</a>.*/
		BeginTag(VHTML_ADDRESS, NULL, VFALSE);
		Space();
		Stream(VHTML_HREF);
		Equal();
		Quote();

		if ( pszProtocol )
			Stream(pszProtocol);

		Stream(pszURL);
		Quote();

		/* Target frame?*/
		if ( pszTargetFrame )
		{
			Space();
			Stream(VHTML_TARGET);
			Equal();
			Quote();
			Stream(pszTargetFrame);
			Quote();
		}

		Trailer();

		/* Display name?*/
		if ( pszDisplayName )
			Stream(pszDisplayName);

		/* End it.*/
		EndTag(VHTML_ADDRESS);
	}

	/** Write a <html> tag and optional attributes to signal start of
	HTML content.*/
	void				HTMLBegin(VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(VHTML_HTML, pszAttributes); }

	/** Write a </html> tag to signal end of HTML content.*/
	void				HTMLEnd()
		{ EndTag(VHTML_HTML); }

	/** Stream a <img> tag and optional attributes to the stream.*/
	void				Image(VSTRING_CONST pszAttributes = NULL)
		{ SingletonTag(VHTML_IMAGE, pszAttributes); }

	/** Write the leader (<) character to the HTML stream.*/
	void				Leader()
		{ Char(VTEXT('<')); }

	/** Insert a mailto URL to create an email link. If pszDisplayName
	is NULL, pszEmailAddress will be used.*/
	void				MailTo(	VSTRING_CONST pszEmailAddress,
								VSTRING_CONST pszDisplayName = NULL)
	{
		VASSERT_HTML(pszEmailAddress, "VHTMLStream::MailTo() requires an E-Mail address and pszEmailAddress is NULL.")

		HREF(	pszEmailAddress,
				(pszDisplayName) ? pszDisplayName : pszEmailAddress,
				NULL,
				VHTML_STRING(VHTML_MAILTO));
	}

	/** Write the <meta> tag. Only valid when first placed between the HTML
	<head> and </head> tags. This function writes the entire meta tag, so
	pszMetaData is required.*/
	void				Meta(VSTRING_CONST pszMetaData)
	{
		VASSERT_HTML(pszMetaData, "VHTMLStream::Meta() requires meta-data and pszMetaData is NULL.")

		BeginTag(VHTML_META, pszMetaData);
	}

	/** Write a new line (carriage return) to the stream.*/
	void				NewLine()
		{ Char(VTEXT('\n')); }

	/** Stream a <option> tag and optional attributes to the stream.*/
	void				OptionBegin(VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(VHTML_OPTION, pszAttributes); }

	/** Stream a </option> tag to the stream.*/
	void				OptionEnd()
		{ EndTag(VHTML_OPTION); }

	/** Start a new paragraph <p> tag and optional attributes to the stream.*/
	void				ParagraphBegin(VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(VHTML_PARAGRAPH, pszAttributes); }

	/** End a paragraph </p> tag.*/
	void				ParagraphEnd()
		{ EndTag(VHTML_PARAGRAPH); }

	/** Write the period (.) character to the stream.*/
	void				Period()
		{ Char(VTEXT('.')); }

	/** Write the qoute (") character to the stream.*/
	void				Quote()
		{ Char(VTEXT('"')); }

	/** For derived classes that can clear their current stream, override
	this function to empty contents. This base class function sets the
	internal variable m_bNeedsHTMLEndTag to VTRUE if bSendHTMLStartTag is
	also VTRUE. It will NOT set it to VFALSE otherwise, so derived code
	should modify this variable based on what it does.*/
	virtual void		Reset(VBOOL bSendHTMLStartTag = VTRUE)
	{
		if ( bSendHTMLStartTag )
		{
			HTMLBegin();
			m_bNeedsHTMLEndTag = VTRUE;
		}
	}

	/** Write a <select> tag and optional attributes to the stream.*/
	void				SelectBegin(VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(VHTML_SELECT, pszAttributes); }

	/** Write a </select> tag to the stream.*/
	void				SelectEnd()
		{ EndTag(VHTML_SELECT); }

	/** Write a singleton tag. A singleton tag is like <br> where there is
	no ending </br>, and usually no attributes.*/
	void				SingletonTag(	VUINT nID,
										VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(nID, pszAttributes, VTRUE); }

	/** Write the slash (/) character to the HTML stream.*/
	void				Slash()
		{ Char(VTEXT('/')); }

	/** Write a space ( ) character to the HTML stream.*/
	void				Space()
		{ Char(VTEXT(' ')); }

	/** Stream pszText to the stream.*/
	void				Stream(VSTRING_CONST pszText)
		{ OnStream(pszText); }

	/** Stream the tag identified by nID to the stream. Only the tag name is
	sent, not the surrounding < and > characters. See BeginTag(), EndTag()
	and SingletonTag() to send the leader, tag, and trailer at once.*/
	void				Stream(VUINT nID)
	{
		VASSERT(nID < VARRAY_SIZE(VHTML_STRINGS))
		Stream(VHTML_STRING(nID));
	}

	/** Stream an integer, as a string, to the stream.*/
	void				StreamInt(VINT nValue)
	{
		VTCHAR sz[64];
		Stream(VITOA(nValue, sz, 10));
	}

	/** Stream an unsigned integer, as a string, to the stream.*/
	void				StreamUINT(VUINT nValue)
	{
		VTCHAR sz[64];
		Stream(VUITOA(nValue, sz, 10));
	}

	/** Stream a long, as a string, to the stream.*/
	void				StreamLong(VLONG nValue)
	{
		VTCHAR sz[64];
		Stream(VLTOA(nValue, sz, 10));
	}

	/** Stream an unsigned long, as a string, to the stream.*/
	void				StreamULong(VULONG nValue)
	{
		VTCHAR sz[64];
		Stream(VULTOA(nValue, sz, 10));
	}

	/** Write a <strong> tag and optional attributes to turn strong text on.*/
	void				StrongBegin(VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(VHTML_STRONG, pszAttributes); }

	/** Write a </strong> tag to turn strong text off.*/
	void				StrongEnd()
		{ EndTag(VHTML_STRONG); }

	/** Write a <table> tag and optional attributes to the stream.*/
	void				TableBegin(VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(VHTML_TABLE, pszAttributes); }

	/** Write a </table> end tag to the stream.*/
	void				TableEnd()
		{ EndTag(VHTML_TABLE); }

	/** Write a <td> (table data, or table cell) tag and optional attributes
	to the stream.*/
	void				TableCellBegin(VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(VHTML_TABLE_DATA, pszAttributes); }

	/** Write a </td> (table data, or table cell) end tag to the stream.*/
	void				TableCellEnd()
		{ EndTag(VHTML_TABLE_DATA); }

	/** Write a <td> (table data, or table cell) tag and optional attributes
	to the stream.*/
	void				TableDataBegin(VSTRING_CONST pszAttributes = NULL)
		{ TableCellBegin(pszAttributes); }

	/** Write a </td> (table data, or table cell) end tag to the stream.*/
	void				TableDataEnd()
		{ TableCellEnd(); }

	/** Write a <tr> (table row) tag and optional attributes to the stream.*/
	void				TableRowBegin(VSTRING_CONST pszAttributes = NULL)
		{ BeginTag(VHTML_TABLE_ROW, pszAttributes); }

	/** Write a </tr> (table row) end tag to the stream.*/
	void				TableRowEnd()
		{ EndTag(VHTML_TABLE_ROW); }

	/** Write the <title> tag. Only valid when first placed between the
	HTML <head> and </head> tags.*/
	void				Title(VSTRING_CONST pszTitle)
	{
		BeginTag(VHTML_TITLE);
		Stream(pszTitle);
		EndTag(VHTML_TITLE);
	}

	/** Write the trailer (>) character to the HTML stream.*/
	void				Trailer()
		{ Char(VTEXT('>')); }

	/** Return a pointer to the VWebServerRequest object held in this object.*/
	VWebServerRequest*	WebServerRequest() const
		{ return (VWebServerRequest*)m_pWebServerRequest; }

	/** Set a pointer to a VWebServerRequest object to be used by this
	object. If not NULL, all content streamed through this object will be
	sent to VWebServerRequest::SendContent(). If NULL, content will be sent
	to stdout.*/
	void				WebServerRequest(VWebServerRequest* pWebServerRequest)
		{ m_pWebServerRequest = pWebServerRequest; }

protected:
	/** Override in derived class to perform custom output processing. The
	default implementation sends output to VWebServerRequest::SendContent()
	if m_pWebServerRequest is not NULL, stdout otherwise. Overrides should
	still be a NOOP if pszText is NULL. nLength does not have to be
	specified as it will be determined by the string length if 0. If the
	string length is known, a slight performance increase can be seen by
	specifying this value.*/
	virtual void		OnStream(	VSTRING_CONST	pszText,
									VULONG			nLength = 0)
	{
		if ( pszText )
		{
			if ( m_pWebServerRequest )
				m_pWebServerRequest->SendContent(pszText, nLength);
			else
				fwrite(	pszText,
						1,
						(nLength) ? nLength : VSTRLEN(pszText),
						stdout);
		}
	}

	/** Embedded Members.*/
	VWebServerRequest*	m_pWebServerRequest;
	VBOOL				m_bNeedsHTMLEndTag;
};

#endif /* VHTMLSTREAM*/
