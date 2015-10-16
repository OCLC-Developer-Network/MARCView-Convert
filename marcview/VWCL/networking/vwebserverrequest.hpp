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

#ifndef VWEBSERVERREQUEST
#define VWEBSERVERREQUEST

#include "../vstandard.h"
#include "../parsers/vcommandparsemap.hpp"
#include "../strings/vstring.hpp"

/** HTTP Response Header Strings.*/
static VSTRING_CONST VHTTP_RESPONSE_HEADER_STRINGS[] =
		{	"Content-type",
			"Pragma",
			"Location"
		};

/** Enumeration of above strings.*/
enum	{	VHTTP_RESPONSE_HEADER_CONTENT_TYPE,
			VHTTP_RESPONSE_HEADER_PRAGMA,
			VHTTP_RESPONSE_HEADER_LOCATION
		};

/** HTTP Response Header Paramter Strings.*/
static VSTRING_CONST VHTTP_RESPONSE_HEADER_PARAMETER_STRINGS[] =
		{	"text/html",
			"text/plain",
			"no-cache"
		};

/** Enumeration of above strings.*/
enum	{	VHTTP_RESPONSE_HEADER_PARAMETER_TEXT_HTML,
			VHTTP_RESPONSE_HEADER_PARAMETER_TEXT_PLAIN,
			VHTTP_RESPONSE_HEADER_PARAMETER_NO_CACHE
		};

/** HTTP Enivironment Strings.*/
static VSTRING_CONST VHTTP_ENV_STRINGS[] =
		{	"AUTH_TYPE",
			"CONTENT_LENGTH",
			"CONTENT_TYPE",
			"GATEWAY_INTERFACE",
			"HTTP_ACCEPT",
			"HTTP_ACCEPT_CHARSET",
			"HTTP_ACCEPT_ENCODING",
			"HTTP_ACCEPT_LANGUAGE",
			"HTTP_CONNECTION",
			"HTTP_HOST",
			"HTTP_REFERER",
			"HTTP_USER_AGENT",
			"PATH_INFO",
			"PATH_TRANSLATED",
			"QUERY_STRING",
			"REDIRECT_REQUEST",
			"REDIRECT_STATUS",
			"REDIRECT_URL",
			"REMOTE_ADDR",
			"REMOTE_HOST",
			"REMOTE_IDENT",
			"REMOTE_USER",
			"REQUEST_METHOD",
			"SCRIPT_NAME",
			"SERVER_NAME",
			"SERVER_PORT",
			"SERVER_PORT_SECURE",
			"SERVER_PROTOCOL",
			"SERVER_SOFTWARE"
		};

/** Enumeration of above strings.*/
enum	{	VHTTP_AUTH_TYPE,
			VHTTP_CONTENT_LENGTH,
			VHTTP_CONTENT_TYPE,
			VHTTP_GATEWAY_INTERFACE,
			VHTTP_HTTP_ACCEPT,
			VHTTP_HTTP_ACCEPT_CHARSET,
			VHTTP_HTTP_ACCEPT_ENCODING,
			VHTTP_HTTP_ACCEPT_LANGUAGE,
			VHTTP_HTTP_CONNECTION,
			VHTTP_HTTP_HOST,
			VHTTP_HTTP_REFERER,
			VHTTP_HTTP_USER_AGENT,
			VHTTP_PATH_INFO,
			VHTTP_PATH_TRANSLATED,
			VHTTP_QUERY_STRING,
			VHTTP_REDIRECT_REQUEST,
			VHTTP_REDIRECT_STATUS,
			VHTTP_REDIRECT_URL,
			VHTTP_REMOTE_ADDR,
			VHTTP_REMOTE_HOST,
			VHTTP_REMOTE_IDENT,
			VHTTP_REMOTE_USER,
			VHTTP_REQUEST_METHOD,
			VHTTP_SCRIPT_NAME,
			VHTTP_SERVER_NAME,
			VHTTP_SERVER_PORT,
			VHTTP_SERVER_PORT_SECURE,
			VHTTP_SERVER_PROTOCOL,
			VHTTP_SERVER_SOFTWARE
		};

/** Macros to return strings from VHTTP_RESPONSE_HEADER_STRINGS,
VHTTP_RESPONSE_HEADER_PARAMETER_STRINGS, and VHTTP_ENV_STRINGS arrays. In
debug builds these are functions that perform bounds checking. In release
builds, they simply return the string at a given index.*/
#ifdef VWCL_DEBUG
	VSTRING_CONST VHTTP_RESPONSE_HEADER_PARAMETER_STRING(VUINT nIndex)
	{
		VASSERT(nIndex < VARRAY_SIZE(VHTTP_RESPONSE_HEADER_PARAMETER_STRINGS))
		return VHTTP_RESPONSE_HEADER_PARAMETER_STRINGS[nIndex];
	}
	VSTRING_CONST VHTTP_RESPONSE_HEADER_STRING(VUINT nIndex)
	{
		VASSERT(nIndex < VARRAY_SIZE(VHTTP_RESPONSE_HEADER_STRINGS))
		return VHTTP_RESPONSE_HEADER_STRINGS[nIndex];
	}
	VSTRING_CONST VHTTP_ENV_STRING(VUINT nIndex)
	{
		VASSERT(nIndex < VARRAY_SIZE(VHTTP_ENV_STRINGS))
		return VHTTP_ENV_STRINGS[nIndex];
	}
#else
	#define	VHTTP_RESPONSE_HEADER_PARAMETER_STRING(nIndex)	\
		VHTTP_RESPONSE_HEADER_PARAMETER_STRINGS[nIndex]
	#define	VHTTP_RESPONSE_HEADER_STRING(nIndex)			\
		VHTTP_RESPONSE_HEADER_STRINGS[nIndex]
	#define	VHTTP_ENV_STRING(nIndex)						\
		VHTTP_ENV_STRINGS[nIndex]
#endif

/** VWebServerRequest encapsulates the development of Common Gateway
Interface (CGI) based, and Microsoft Internet Information Server (IIS), web
server applications. It would be futile to explain all the concepts of CGI
and ISAPI here, as there are many fine references and tutorials available
on the Internet, as well as several books dedicated to the issues. VWCL
sample applications that use this class, and others related to it, are
PRINTENV and VChat. Please see the VWCL web site for more information and
to download these applications.

In short, a HTTP server that supports CGI applications sets environment
variables that refer to a request and which contain information about the
server, the client, and the script (your program). It then spawns a CGI
process (your program) which is responsible for sending content back to
the client through the server. CGI applications are stand alone programs
that send data to the web server using stdout streams.

In contrast, IIS extensions are compiled as Windows DLL modules. Unlike CGI
applications, they are loaded into the address space of the Microsoft web
server. This means that each incoming request does not have to spawn a new
CGI process, but simply can make a call to a function (HttpExtensionProc())
that resides in your extension DLL, which is already loaded into memory.
This is much more efficient than the CGI model, but is supported on fewer
web servers. Because your application is loaded into IIS (or IIS compatible
web server), and because web servers are multithreaded, your application
MUST also be tread-safe. CGI applications do not usually have to worry
about this, since they are there own, independently running module, often
times single-threaded.

CGI applications can be made to perform as well as ISAPI extensions, and
also incur the above mentioned threading problems. This of course is a
trade-off, but a FastCGI specification and development code does exist
freely on the Internet, and many web servers are beginning to support it.
While not tested with FastCGI at this time, this class should support it in
full, and if not, this will be corrected should a problem be found.

Often times these applications (CGI and IIS Extensions) dynamically
generate HTML. This first thing a web server application should send is the
HTTP response header and parameters. The most common is the string
Content-type: text/html, followed by 2 newline characters. These newline
characters signify the end of the header and parameters. After this, the
web server application generates and streams the content back to the
server, which sends it to the client. You can send more than one HTTP
header. For CGI applications, the headers are separated by a single newline
character (\n). For IIS extensions this is not needed. When you are done
sending HTTP headers, a CGI application sends two newline characters (\n\n),
whereas IIS extensions use \r\n. CGI applications send these through stdout,
whereas IIS extensions use an IIS provided function called
ServerSupportFunction(). The ugliness of these response codes is handled
cleanly by this class through the SendHTTPResponseHeader() function(s). This
function should be called before sending content to the client using the
SendContent() function. If not, this class will assume you want uncached
HTML content, and send the appropriate headers for you, before sending the
real content.

One of the most popular reasons for developing web servers applications is
to process forms. This class' ProcessForm() function makes it almost
trivial to handle form data. In addition, this class caches environment
variables after you make the first request for them, thus saving overhead
of getting them each time from the operating system or web server. Some
variables, including the path info, translated path, and script name,
are retrieved from the system, cached, and then modified before being
returned to you. The query string, when first read from the system, is in
a format that is referred to a HTTP encoded. This means that spaces are
replaced with + signs and the string contains ASCII characters encoded in
hex. This class has a static function called ParseEncodedURL(), that
accepts an encoded string and converts it to look like a normal string.
When the query string is requested, a local copy of the string is made, and
this local copy should likely be ran through the ParseEncodedURL()
function, resulting in an un-parsed string, which is then returned to you.
This class does NOT automatically do that, since you may be interesed in
the unparsed query string. All file paths used by this class are assumed to
be UNIX style, that is, they use forward slashes (/) to separate directories,
and normally have all lowercase names. Windows NT can use these paths as
well, so this is a common ground, which is why this class assumes paths
will be delimited with forward slashes. When a request is made for the path
info, translated path or script name, a local copy of the environment
variable string is made, converted to UNIX style paths, and returned to you.
Unless otherwise noted all other cached strings are simply pointers to
internal VString objects. This is because it is not valid to modify
environment variables. Normally, you would have to copy each environment
variable yourself when you make a request for a named CGI variable. This
class hides this fact from you, and you can use this class almost
identically (except during construction), when developing CGI or IIS
applications. This caching is important for performance, as most web server
requests should execute and exit as quickly as possible.

This class does not implement a "least common denominator" approach to web
server applications. It properly supports IIS and CGI applications to their
fullest, using the highest performance approach for each platform. It does
this by using conditional compilation and abstraction, primarily through
the GetEnvironmentVariable() function.

All functions that request environment type variables have wrapper
functions that call GetEnvironmentVariable(). This is to deliberately
prevent you from requesting a variable that cannot properly be cached by
this class. If you need to support a HTTP variable that this class does not
have direct support for, you will have to override GetEnvironmentVariable()
and implement the correct caching yourself. If the HTTP variable is a
standard (or common) variable that you feel this class should support,
please let the author of this class know so that it can be added. Or, add
support yourself, and E-Mail a copy of your modified source to the author
so that it can be properly checked into VWCL master sources.

VERY IMPORTANT: The VWCL_TARGET_IIS_EXTENSION preprocessor must be defined
by your application for VWebServerRequest to compile with support for IIS.*/
class VWebServerRequest
{
public:
	#ifdef VWCL_TARGET_IIS_EXTENSION
		/** Required constructor (IIS version only) MUST be initialized with
		the EXTENSION_CONTROL_BLOCK structure pointer that IIS sends to your
		application in the HttpExtensionProc() function, which you are
		required to export. This constructor initializes the class and the
		members of the EXTENSION_CONTROL_BLOCK structure.*/
		VWebServerRequest(EXTENSION_CONTROL_BLOCK* pECB)
			{ Init(pECB); }
	#else
		/** Default constructor (for CGI version only) initializes the class.*/
		VWebServerRequest()
			{ Init(); }
	#endif

	/** Virtual destructor free' memory allocated internally by this class.*/
	virtual ~VWebServerRequest()
	{
		for ( VUINT i = 0; i < VARRAY_SIZE(VHTTP_ENV_STRINGS); i++ )
			VDELETE_NULL(m_pstrEnv[i])
	}

	/** "Fix" a path to be UNIX style with forward slashes, or Windows-DOS
	style with backslashes, and convert case.*/
	static VSTRING			FixPath(	VSTRING	pszPath,
										VBOOL	bUNIXStyle = VTRUE,
										VBOOL	bMakeLowerCase = VTRUE)
	{
		VASSERT(VSTRLEN_CHECK(pszPath))

		if ( pszPath )
		{
			VSTRING pszOffSet = pszPath;

			while ( *pszOffSet != '\0' )
			{
				if (	(*pszOffSet == '\\' && bUNIXStyle) ||
						(*pszOffSet == '/' && !bUNIXStyle) )
					*pszOffSet = (bUNIXStyle) ? '/' : '\\';

				pszOffSet++;
			}

			if ( bMakeLowerCase )
				VSTRLWR(pszPath);
		}

		return pszPath;
	}

	/** Get the AUTH_TYPE variable.*/
	VSTRING_CONST			GetAuthType()
		{ return GetEnvironmentVariable(VHTTP_AUTH_TYPE); }

	/** Get the CONTENT_LENGTH variable.*/
	VSTRING_CONST			GetContentLength()
		{ return GetEnvironmentVariable(VHTTP_CONTENT_LENGTH); }

	/** Get the CONTENT_LENGTH variable as a integer.*/
	VUINT					GetContentLengthValue()
	{
		#ifdef VWCL_TARGET_IIS_EXTENSION
			VASSERT(m_pECB)
			return m_pECB->cbTotalBytes;
		#else
			return (GetContentLength())
				? atoi(m_pstrEnv[VHTTP_CONTENT_LENGTH]->String())
				: 0;
		#endif
	}

	/** Get the CONTENT_TYPE variable.*/
	VSTRING_CONST			GetContentType()
		{ return GetEnvironmentVariable(VHTTP_CONTENT_TYPE); }

	/** Get and cache the specified environment variable. Returns pointer to
	internal string on success, NULL on failure. The nStringIndex parameter
	is one of the VHTTP_xxx enumeration constants above, which is an index
	into the VHTTP_ENV_STRINGS string array.*/
	virtual VSTRING_CONST	GetEnvironmentVariable(VUINT nStringIndex)
	{
		VASSERT(nStringIndex < VARRAY_SIZE(VHTTP_ENV_STRINGS))

		/* Have we already retrieved this value before? If yes, skip this.*/
		if ( !m_pstrEnv[nStringIndex] )
		{
			/* Allocate a VString object.*/
			m_pstrEnv[nStringIndex] = VNEW VString;

			if ( m_pstrEnv[nStringIndex] )
			{
				#ifdef VWCL_TARGET_IIS_EXTENSION
					VASSERT(m_pECB)

					/* IIS provides some higher performance ways to get
					some information. If available, use it.*/
					switch ( nStringIndex )
					{
						case VHTTP_CONTENT_LENGTH:
							m_pstrEnv[nStringIndex]->
								String(m_pECB->cbTotalBytes);
							break;

						case VHTTP_CONTENT_TYPE:
							m_pstrEnv[nStringIndex]->
								String(m_pECB->lpszContentType);
							break;

						case VHTTP_QUERY_STRING:
							m_pstrEnv[nStringIndex]->
								String(m_pECB->lpszQueryString);
							break;

						case VHTTP_REQUEST_METHOD:
							m_pstrEnv[nStringIndex]->
								String(m_pECB->lpszMethod);
							break;

						case VHTTP_PATH_INFO:
							m_pstrEnv[nStringIndex]->
								String(m_pECB->lpszPathInfo);
							break;

						case VHTTP_PATH_TRANSLATED:
							m_pstrEnv[nStringIndex]->
								String(m_pECB->lpszPathTranslated);
							break;

						/* We need to ask IIS for the value.*/
						default:
						{
							VDWORD nBytes = 0;

							/* First ask IIS for the length of the string.*/
							m_pECB->GetServerVariable(
								m_pECB->ConnID,
								(VSTRING)VHTTP_ENV_STRING(nStringIndex),
								NULL,
								&nBytes);

							/* Now allocate memory for the string and
							tell IIS to give us the string.*/
							if ( nBytes && m_pstrEnv[nStringIndex]->
									String(NULL, nBytes + 1) )
							{
								m_pECB->GetServerVariable(
									m_pECB->ConnID,
									(VSTRING)VHTTP_ENV_STRING(nStringIndex),
									m_pstrEnv[nStringIndex]->String(),
									&nBytes);

								m_pstrEnv[nStringIndex]->UpdateLength(nBytes);
							}

							break;
						}
					}

					/* Check for empty string. IIS has been known to give
					back "" instead of NULL.*/
					if (	m_pstrEnv[nStringIndex]->IsNotEmpty() &&
							m_pstrEnv[nStringIndex]->CharAt(0) == '\0' )
						m_pstrEnv[nStringIndex]->Empty();
				#else
					m_pstrEnv[nStringIndex]->
						String(getenv(VHTTP_ENV_STRING(nStringIndex)));
				#endif

				if ( m_pstrEnv[nStringIndex]->IsNotEmpty() )
				{
					/* See if we need to modify the string in any way.*/
					switch ( nStringIndex )
					{
						/* Convert these to UNIX style paths.*/
						case VHTTP_PATH_INFO:
						case VHTTP_PATH_TRANSLATED:
						case VHTTP_SCRIPT_NAME:
							FixPath(m_pstrEnv[nStringIndex]->String());
							break;
					}
				}
				else
					m_pstrEnv[nStringIndex]->Empty();
			}
		}

		/* Return value now or NULL result.*/
		return	(m_pstrEnv[nStringIndex])
				? m_pstrEnv[nStringIndex]->String()
				: NULL;
	}

	/** Get the GATEWAY_INTERFACE variable.*/
	VSTRING_CONST			GetGatewayInterface()
		{ return GetEnvironmentVariable(VHTTP_GATEWAY_INTERFACE); }

	/** Get the HTTP_ACCEPT variable.*/
	VSTRING_CONST			GetHTTPAccept()
		{ return GetEnvironmentVariable(VHTTP_HTTP_ACCEPT); }

	/** Get the HTTP_ACCEPT_CHARSET variable.*/
	VSTRING_CONST			GetHTTPAcceptCharset()
		{ return GetEnvironmentVariable(VHTTP_HTTP_ACCEPT_CHARSET); }

	/** Get the HTTP_ACCEPT_ENCODING variable.*/
	VSTRING_CONST			GetHTTPAcceptEncoding()
		{ return GetEnvironmentVariable(VHTTP_HTTP_ACCEPT_ENCODING); }

	/** Get the HTTP_ACCEPT_LANGUAGE variable.*/
	VSTRING_CONST			GetHTTPAcceptLanguage()
		{ return GetEnvironmentVariable(VHTTP_HTTP_ACCEPT_LANGUAGE); }

	/** Get the HTTP_CONNECTION variable.*/
	VSTRING_CONST			GetHTTPConnection()
		{ return GetEnvironmentVariable(VHTTP_HTTP_CONNECTION); }

	/** Get the HTTP_HOST variable.*/
	VSTRING_CONST			GetHTTPHost()
		{ return GetEnvironmentVariable(VHTTP_HTTP_HOST); }

	/** Get the HTTP_REFERER variable.*/
	VSTRING_CONST			GetHTTPReferer()
		{ return GetEnvironmentVariable(VHTTP_HTTP_REFERER); }

	/** Get the HTTP_USER_AGENT variable.*/
	VSTRING_CONST			GetHTTPUserAgent()
		{ return GetEnvironmentVariable(VHTTP_HTTP_USER_AGENT); }

	/** Get the PATH_INFO variable.*/
	VSTRING_CONST			GetPathInfo()
		{ return GetEnvironmentVariable(VHTTP_PATH_INFO); }

	/** Get the PATH_TRANSLATED variable.*/
	VSTRING_CONST			GetPathTranslated()
		{ return GetEnvironmentVariable(VHTTP_PATH_TRANSLATED); }

	/** Get the QUERY_STRING variable.*/
	VSTRING_CONST			GetQueryString()
		{ return GetEnvironmentVariable(VHTTP_QUERY_STRING); }

	/** Get the REDIRECT_REQUEST variable.*/
	VSTRING_CONST			GetRedirectRequest()
		{ return GetEnvironmentVariable(VHTTP_REDIRECT_REQUEST); }

	/** Get the REDIRECT_STATUS variable.*/
	VSTRING_CONST			GetRedirectStatus()
		{ return GetEnvironmentVariable(VHTTP_REDIRECT_STATUS); }

	/** Get the REDIRECT_URL variable.*/
	VSTRING_CONST			GetRedirectURL()
		{ return GetEnvironmentVariable(VHTTP_REDIRECT_URL); }

	/** Get the REMOTE_ADDR variable.*/
	VSTRING_CONST			GetRemoteAddress()
		{ return GetEnvironmentVariable(VHTTP_REMOTE_ADDR); }

	/** Get the REMOTE_HOST variable.*/
	VSTRING_CONST			GetRemoteHost()
		{ return GetEnvironmentVariable(VHTTP_REMOTE_HOST); }

	/** Get the REMOTE_IDENT variable.*/
	VSTRING_CONST			GetRemoteIdent()
		{ return GetEnvironmentVariable(VHTTP_REMOTE_IDENT); }

	/** Get the REMOTE_USER variable.*/
	VSTRING_CONST			GetRemoteUser()
		{ return GetEnvironmentVariable(VHTTP_REMOTE_USER); }

	/** Get the REQUEST_METHOD variable.*/
	VSTRING_CONST			GetRequestMethod()
		{ return GetEnvironmentVariable(VHTTP_REQUEST_METHOD); }

	/** Get the SCRIPT_NAME variable.*/
	VSTRING_CONST			GetScriptName()
		{ return GetEnvironmentVariable(VHTTP_SCRIPT_NAME); }

	/** Get the SERVER_NAME variable.*/
	VSTRING_CONST			GetServerName()
		{ return GetEnvironmentVariable(VHTTP_SERVER_NAME); }

	/** Get the SERVER_PORT variable.*/
	VSTRING_CONST			GetServerPort()
		{ return GetEnvironmentVariable(VHTTP_SERVER_PORT); }

	/** Get the SERVER_PORT variable as a VUSHORT value.*/
	VUSHORT					GetServerPortValue()
	{
		return	(GetServerPort())
				? (VUSHORT)atoi(m_pstrEnv[VHTTP_SERVER_PORT]->String())
				: 0;
	}

	/** Get the SERVER_PORT_SECURE variable.*/
	VSTRING_CONST			GetServerPortSecure()
		{ return GetEnvironmentVariable(VHTTP_SERVER_PORT_SECURE); }

	/** Get the SERVER_PORT_SECURE variable as a VUSHORT value.*/
	VUSHORT					GetServerPortSecureValue()
	{
		return	(GetServerPortSecure())
				? (VUSHORT)atoi(m_pstrEnv[VHTTP_SERVER_PORT_SECURE]->String())
				: 0;
	}

	/** Get the SERVER_PROTOCOL variable.*/
	VSTRING_CONST			GetServerProtocol()
		{ return GetEnvironmentVariable(VHTTP_SERVER_PROTOCOL); }

	/** Get the SERVER_SOFTWARE variable.*/
	VSTRING_CONST			GetServerSoftware()
		{ return GetEnvironmentVariable(VHTTP_SERVER_SOFTWARE); }

	/** Determine if the request method is GET.*/
	VBOOL					IsRequestMethodGET()
	{
		return
			(GetRequestMethod())
			? (VSTRCMP_NOCASE(m_pstrEnv[VHTTP_REQUEST_METHOD]->
				String(), "GET") == 0)
			? VTRUE
			: VFALSE
			: VFALSE;
	}

	/** Determine if the request method is HEAD.*/
	VBOOL					IsRequestMethodHEAD()
	{
		return	(GetRequestMethod())
				? (VSTRCMP_NOCASE(	m_pstrEnv[VHTTP_REQUEST_METHOD]->String(),
									"HEAD") == 0)
				? VTRUE
				: VFALSE
				: VFALSE;
	}

	/** Determine if the request method is POST.*/
	VBOOL					IsRequestMethodPOST()
	{
		return	(GetRequestMethod())
				? (VSTRCMP_NOCASE(	m_pstrEnv[VHTTP_REQUEST_METHOD]->String(),
									"POST") == 0)
				? VTRUE
				: VFALSE
				: VFALSE;
	}

	/** Determine if the application is running as a CGI application.
	Returns VFALSE if user invoked it from command line.*/
	static VBOOL			IsRunningAsCGI()
	{
		#ifdef VWCL_TARGET_IIS_EXTENSION
			return VTRUE;
		#else
			if ( getenv(VHTTP_ENV_STRING(VHTTP_REQUEST_METHOD)) )
				return VTRUE;

			return VFALSE;
		#endif
	}

	/** Given a HTTP parsed URL, convert + signs into spaces and %xx escape
	sequences into single ASCII character. The return value is the same
	pointer passed to function.*/
	static VSTRING			ParseEncodedURL(VSTRING pszURL)
	{
		VASSERT(pszURL)

		if ( pszURL )
		{
			register VINT	i, j;
			register VCHAR	digit;
			VSTRING			pszThisChar;

			for ( i = 0, j = 0; pszURL[j]; ++i, ++j )
			{
				pszURL[i] = pszURL[j];

				if ( pszURL[i] == '%' )
				{
					pszThisChar = &pszURL[j + 1];

					digit =
						(pszThisChar[0] >= 'A'
						? ((pszThisChar[0] & 0xdf) - 'A') + 10
						: (pszThisChar[0] - '0'));

					digit *= 16;

					digit +=
						(pszThisChar[1] >= 'A'
						? ((pszThisChar[1] & 0xdf) - 'A') + 10
						: (pszThisChar[1] - '0'));
				
					pszURL[i] = digit;

					j += 2;
				}
				else if ( pszURL[i] == '+' )
					pszURL[i] = ' ';
			}

			pszURL[i] = '\0';
		}

		return pszURL;
	}

	/** Process a form submittal. Return value is number of items parsed
	into the command parse map object, or 0 on error. Upon return, the
	command parse map object will contain the name-value pairs posted from
	the form data. It is then a simple matter of walking the map extracting
	what you are interested in. The value part of each entry in the command
	parse map will already be URL un-encoded so it is safe to assume the
	data is parsed, as you expect it to be. This function works with POST
	and GET method form submittals.*/
	VUINT					ProcessForm(VCommandParseMap& cpm)
	{
		VUINT nCountOfCommands = 0;

		/* Determine form posting method.*/
		VBOOL bPOST =					IsRequestMethodPOST();
		VUINT nContentLength =			(bPOST) ? GetContentLengthValue() : 0;
		VSTRING_CONST pszQueryString =	NULL;

		/* If a get, we need to get the query string. We don't use the
		possibly cached version since it could be modified.*/
		if ( !bPOST )
		{
			#ifdef VWCL_TARGET_IIS_EXTENSION
			{
				VASSERT(m_pECB)
				pszQueryString = m_pECB->lpszQueryString;
			}
			#else
				pszQueryString = getenv(VHTTP_ENV_STRING(VHTTP_QUERY_STRING));
			#endif
		}

		/* If no content length, the form was probably a GET method, not
		a POST.*/
		if (	pszQueryString &&
				!nContentLength &&
				IsRequestMethodGET() )
		{
			bPOST =				VFALSE;
			nContentLength =	VSTRLEN(pszQueryString);
		}

		/* Allocate buffer space to read into. We will actually allocate
		one more byte than we need to place a trailing & so that
		VCommandParseMap can deal with it.*/
		if (	nContentLength &&
				m_strFormData.String(NULL, nContentLength + 2) )
		{
			/* Read data or copy from query string.*/
			if ( bPOST )
			{
				#ifdef VWCL_TARGET_IIS_EXTENSION
					VSTRNCPY(	m_strFormData.String(),
								(VSTRING_CONST)m_pECB->lpbData,
								nContentLength);
				#else
					fread(m_strFormData.String(), 1, nContentLength, stdin);
				#endif
			}
			else
				VSTRCPY(m_strFormData.String(), pszQueryString);

			/* Update string length now.*/
			m_strFormData.UpdateLength(nContentLength + 1);

			/* Count the number of = signs to determine the number of
			name-value pairs.*/
			VUINT nCommandCount = m_strFormData.GetCharCount('=');

			if ( nCommandCount )
			{
				/* Append trailing & so VCommandParseMap find it.*/
				*(m_strFormData.String() + nContentLength) = '&';

				/* Set command parse map delimiters.*/
				cpm.SetDelimiters(	VCommandParseMap::MARKER_COMMAND_ANYTHING,
									'=',
									'&');

				/* Parse it now into name-value (command-parameter) pairs.*/
				nCountOfCommands =
					cpm.Parse(	m_strFormData,
								nContentLength + 1,
								nCommandCount);

				if ( nCountOfCommands )
				{
					/* Un-parse URL encoded data (values only) into
					normal strings.*/
					for ( VUINT i = 0; i < nCountOfCommands; i++ )
						ParseEncodedURL(cpm.GetParameterAt(i));
				}
			}
		}

		if ( !nCountOfCommands )
			m_strFormData.Empty();

		return nCountOfCommands;
	}

	/** Send a string to server. This can be any string except HTTP header
	responses on IIS server extensions. If the length of the string is
	known, it can be specified in nLength, otherwise the length of the
	string will be determined by this function if nLength is 0. For HTTP
	response headers, see the SendHTTPResponseHeader() functions.*/
	void					Send(	VSTRING_CONST	pszString,
									VULONG			nLength = 0)
	{
		if ( pszString )
		{
			if ( !nLength )
				nLength = VSTRLEN_CHECK(pszString);

			if ( nLength )
			{
				#ifdef VWCL_TARGET_IIS_EXTENSION
					VASSERT(m_pECB)
					m_pECB->WriteClient(	m_pECB->ConnID,
											(VVOIDPTR)pszString,
											&nLength,
											0);
				#else
					fwrite(pszString, 1, nLength, stdout);
				#endif
			}
		}
	}

	/** This function sends content (normally HTML) to the web server to be
	sent to the client. If the length of the string in pszContent is known,
	it can be specified, otherwise it will be assumed to be a 0 terminated
	string and the length will be retrieved. For CGI applications, the
	entire string is sent regardless of the value of nLength. It should be
	noted that while the HTTP response header and parameters are required to
	be sent to the server before content, with a call to
	SendHTTPResponseHeader(), if this has not been done, this function will
	send "Content-type: text/html" and "Pragma: no-cache" as the default
	before sending the content.*/
	void					SendContent(	VSTRING_CONST	pszContent,
											VULONG			nLength = 0)
	{
		/* Check this.*/
		if ( !pszContent )
			return;

		/* If response headers have not been sent, assume default.*/
		if ( !m_bHTTPResponseHeaderSent )
		{
			SendHTTPResponseHeader(	VHTTP_RESPONSE_HEADER_CONTENT_TYPE,
									VHTTP_RESPONSE_HEADER_PARAMETER_TEXT_HTML);

			SendHTTPResponseHeader(	VHTTP_RESPONSE_HEADER_PRAGMA,
									VHTTP_RESPONSE_HEADER_PARAMETER_NO_CACHE);
		}

		/* If the end of the HTTP response headers has not been sent, we
		should do that now.*/
		SendHTTPResponseHeaderEndIfNeeded();

		/* Send the content now.*/
		Send(pszContent, nLength);
	}

	/** Before content can be sent to the web server, and then back to the
	client, HTTP header information must be inserted to tell the web server
	and client what type of content is being sent back, and any additional
	information about it. The HTTP protocol defines many of these headers,
	of which VWCL encapsulates only a few of the most common. By far the
	most common is "Content-type: text/html". As an option to this, many
	applications that generate HTML also use "Pragma: no-cache". These are
	the defaults assumed by this class unless you use this function to send
	other HTTP header responses. Many common responses and parameters to
	them are contained in the VHTTP_RESPONSE_HEADER_STRINGS and
	VHTTP_RESPONSE_HEADER_PARAMETER_STRINGS, which are defined above, and
	for which a numeric enumeration exists. If these responses do not fit
	your needs, you can pass any valid response header and parameter to
	this function. An example of the pszHTTPResponseHeader would be the
	"Content-type" string. The pszParameter to this value could be
	"text/html", which also happens to be a MIME type. Internally, this
	function separates the header from the parameter using a colon and
	space, as required by the specification. If this is the last header
	that is to be sent, you can pass VTRUE for bSendHTTPResponseHeaderEnd
	to tell the web server you are done sending HTTP headers. This is done
	automatically for you as needed inside the SendContent() function, but
	if you are not using it, or simply need to terminate the sending of
	HTTP headers, you can pass VTRUE as this parameter. If the header
	terminator has already been sent, this is a NOOP.*/
	void					SendHTTPResponseHeader(
		VSTRING_CONST	pszHTTPResponseHeader,
		VSTRING_CONST	pszParameter,
		VBOOL			bSendHTTPResponseHeaderEnd = VFALSE)
	{
		/* The pszHTTPResponseHeader and pszParameter MUST always be known.*/
		VASSERT(	VSTRLEN_CHECK(pszHTTPResponseHeader) &&
					VSTRLEN_CHECK(pszParameter))

		/* It is not valid to send headers once the web server has been
		told you are done!*/
		VASSERT(!m_bHTTPResponseHeaderClosed)

		/* Set flag to indicate header was sent.*/
		m_bHTTPResponseHeaderSent = VTRUE;

		/* Build single string.*/
		VString s(pszHTTPResponseHeader);
		s += ": ";
		s += pszParameter;

		#ifdef VWCL_TARGET_IIS_EXTENSION
			/* IIS needs CRLF pair.*/
			s.AppendCRLF();

			/* Send header to IIS now.*/
			VULONG nLength = s.GetLength();
			m_pECB->ServerSupportFunction(	m_pECB->ConnID,
											HSE_REQ_SEND_RESPONSE_HEADER,
											NULL,
											&nLength,
											(VULONG*)s.String());
		#else
			/* CGI needs newline.*/
			s += "\n";

			/* Send now.*/
			Send(s, s.GetLength());
		#endif

		/* Send HTTP response end?*/
		if ( bSendHTTPResponseHeaderEnd )
			SendHTTPResponseHeaderEndIfNeeded();
	}

	/** Same as above, but uses string array indexes from the
	VHTTP_RESPONSE_HEADERS_STRINGS and
	VHTTP_RESPONSE_HEADER_PARAMETER_STRINGS enumerations.*/
	void					SendHTTPResponseHeader(
		VUINT nResponseHeaderStringIndex = VHTTP_RESPONSE_HEADER_CONTENT_TYPE,
		VUINT nParameterStringIndex = VHTTP_RESPONSE_HEADER_PARAMETER_TEXT_HTML,
		VBOOL bSendHTTPResponseHeaderEnd = VFALSE)
	{
		SendHTTPResponseHeader(
			VHTTP_RESPONSE_HEADER_STRING(nResponseHeaderStringIndex),
			VHTTP_RESPONSE_HEADER_PARAMETER_STRING(nParameterStringIndex),
			bSendHTTPResponseHeaderEnd);
	}

	/** Set the streaming mode for STDOUT to binary (CGI version only), and
	not needed on UNIX systems.*/
	void					SetStreamModeBinary() const
	{
		#ifndef VWCL_TARGET_IIS_EXTENSION
			#ifndef VWCL_TARGET_UNIX
				setmode(fileno(stdout), O_BINARY);
			#endif
		#endif
	}

	/** Set the streaming mode for STDOUT to text (CGI version only,
	defaults to this in Init()). Not needed on UNIX systems.*/
	void					SetStreamModeText() const
	{
		#ifndef VWCL_TARGET_IIS_EXTENSION
			#ifndef VWCL_TARGET_UNIX
				setmode(fileno(stdout), O_TEXT);
			#endif
		#endif
	}

protected:
	#ifdef VWCL_TARGET_IIS_EXTENSION
		/** Initialize members to default values (including the
		EXTENSION_CONTROL_BLOCK) (IIS version only).*/
		void				Init(EXTENSION_CONTROL_BLOCK* pECB)
	#else
		/** Initialize members to default values and set
		stdout to unbuffered mode.*/
		void				Init()
	#endif
	{
		/* Initialize members.*/
		m_bHTTPResponseHeaderSent =		VFALSE;
		m_bHTTPResponseHeaderClosed =	VFALSE;

		/* Initialize internal environment variables to NULL (not allocated).*/
		for ( VUINT i = 0; i < VARRAY_SIZE(VHTTP_ENV_STRINGS); i++ )
			m_pstrEnv[i] = NULL;

		#ifdef VWCL_TARGET_IIS_EXTENSION
			/* pECB is required.*/
			VASSERT(pECB)
			m_pECB = pECB;
			m_pECB->dwHttpStatusCode = 200;
		#else
			/* Set STDOUT to unbuffered.*/
			setvbuf(stdout, NULL, _IONBF, 0);

			/* Set streaming mode to be text.*/
			SetStreamModeText();
		#endif
	}

	/** Send HTTP Response header end code, as needed.*/
	void					SendHTTPResponseHeaderEndIfNeeded()
	{
		if ( !m_bHTTPResponseHeaderClosed )
		{
			m_bHTTPResponseHeaderClosed = VTRUE;

			#ifndef VWCL_TARGET_IIS_EXTENSION
				/* Since the headers end with one \n, we only need one more.*/
				Send("\n", 1);
			#else
				VASSERT(m_pECB)
				VULONG nLength = 0;
				m_pECB->ServerSupportFunction(	m_pECB->ConnID,
												HSE_REQ_SEND_RESPONSE_HEADER,
												NULL,
												&nLength,
												NULL);
			#endif
		}
	}

	/** Embedded Members.*/

	/** Extension Control Block used only for Microsoft IIS.*/
	#ifdef VWCL_TARGET_IIS_EXTENSION
		EXTENSION_CONTROL_BLOCK*	m_pECB;
	#endif

	/** String used when reading form data.*/
	VString						m_strFormData;

	/** Environment Variables
	(Copied and cached, and modified, as requested).*/
	VString*					m_pstrEnv[VARRAY_SIZE(VHTTP_ENV_STRINGS)];

	/** Used as flag to indicate if a HTTP response header has
	already been sent.*/
	VBOOL						m_bHTTPResponseHeaderSent;

	/** Used as flag to indicate if HTTP response header
	has been closed.*/
	VBOOL						m_bHTTPResponseHeaderClosed;
};

#endif /* VWEBSERVERREQUEST*/
