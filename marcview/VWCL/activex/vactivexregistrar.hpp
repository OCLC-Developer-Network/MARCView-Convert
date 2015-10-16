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

#ifndef VACTIVEXREGISTRAR
#define VACTIVEXREGISTRAR

#include "../vstandard.h"
#include "../strings/vstring.hpp"
#include "../utility/vregistry.hpp"

/** VActiveXRegistrar is used when installing or un-installing
ActiveX componenet into or from the system registry.*/
class VActiveXRegistrar
{
public:
	/** Register an ActiveX Server. Only pszCLSID and pszClassName are
	required, others are optional.*/
	static VBOOL	RegisterServer(	VSTRING_CONST	pszCLSID,
									VSTRING_CONST	pszClassName,
									VSTRING_CONST	pszProgID = NULL,
									VSTRING_CONST	pszProgIndependantID =
													NULL,
									VBOOL			bInProcess = VTRUE,
									VSTRING_CONST	pszThreadingModel = NULL,
									HINSTANCE		hInstance = NULL)
	{
		VASSERT(VSTRLEN_CHECK(pszCLSID) && VSTRLEN_CHECK(pszClassName))

		VBOOL bResult = VTRUE;

		/* Default threading model?*/
		if ( !pszThreadingModel )
			pszThreadingModel = VTEXT("Apartment");

		/* Create programmatic ID's first if specified.*/
		if ( pszProgID && pszProgIndependantID )
		{
			VSTRING_CONST pszKeys[] = {pszProgID, pszProgIndependantID};

			for ( VUINT i = 0; i < VARRAY_SIZE(pszKeys); i++ )
			{
				VRegistry reg;

				if ( reg.CreateKey(pszKeys[i], HKEY_CLASSES_ROOT, KEY_WRITE) )
				{
					/* Save the class name, CLSID key, and current
					version values.*/
					if (	!reg.WriteString(	(VSTRING_CONST)NULL,
												pszClassName) ||
							!reg.WriteSubKey(VTEXT("CLSID"), pszCLSID) ||
							!reg.WriteSubKey(VTEXT("CurVer"), pszProgID) )
					{
						bResult = VFALSE;
					}
				}
				else
					bResult = VFALSE;

				/* No use in continuing now?*/
				if ( !bResult )
					break;
			}
		}

		if ( bResult )
		{
			VRegistry reg;

			/* Build relative path to HKEY_CLASSES_ROOT\CLSID.*/
			VString s(VTEXT("CLSID\\"));
			s += pszCLSID;

			/* Create CLSID key.*/
			if (	!s.GetErrorCount() &&
					reg.CreateKey(s, HKEY_CLASSES_ROOT, KEY_WRITE) )
			{
				/* Save the class name.*/
				reg.WriteString((VSTRING_CONST)NULL, pszClassName);

				/* Get the path to this module.*/
				s.GetModulePath(VFALSE, VFALSE, hInstance);

				VRegistry regSub;

				if ( bInProcess && !s.GetErrorCount() )
				{
					/* Create the InprocServer32 key.*/
					if ( regSub.CreateKey(	VTEXT("InprocServer32"),
											reg.GetKey(), KEY_WRITE) )
					{
						/* Save the path and threading model.*/
						if (	!regSub.WriteString((VSTRING_CONST)NULL, s) ||
								!regSub.WriteString(VTEXT("ThreadingModel"),
								pszThreadingModel) )
							bResult = VFALSE;

						regSub.Close();
					}
				}

				if ( bResult )
				{
					/* Create the ProgID key.*/
					if ( pszProgIndependantID || pszProgID )
					{
						if ( regSub.CreateKey(	VTEXT("ProgID"),
												reg.GetKey(),
												KEY_WRITE) )
						{
							if ( !regSub.WriteString(
								(VSTRING_CONST)NULL,
								(pszProgID)
								? pszProgID
								: pszProgIndependantID) )
							{
								bResult = VFALSE;
							}

							regSub.Close();
						}
						else
							bResult = VFALSE;
					}
				}

				if ( bResult )
				{
					/* Create the Programmable key.*/
					if ( regSub.CreateKey(	VTEXT("Programmable"),
											reg.GetKey(),
											KEY_WRITE) )
						regSub.Close();
					else
						bResult = VFALSE;
				}

				if ( bResult )
				{
					/* Create the version independant prog ID.*/
					if ( pszProgIndependantID )
					{
						if ( regSub.CreateKey(
							VTEXT("VersionIndependentProgID"),
							reg.GetKey(),
							KEY_WRITE) )
						{
							if ( !regSub.WriteString(	(VSTRING_CONST)NULL,
														pszProgIndependantID) )
								bResult = VFALSE;

							regSub.Close();
						}
						else
							bResult = VFALSE;
					}
				}
			}
			else
				bResult = VFALSE;
		}

		return bResult;
	}

	/** Same as above, but loads all strings from string resources.
	Only nStringIDCLSID and nStringIDClassName are required, others
	are optional.*/
	static VBOOL	RegisterServer(	VUINT		nStringIDCLSID,
									VUINT		nStringIDClassName,
									VUINT		nStringIDProgID,
									VUINT		nStringIDProgIndependantID,
									VBOOL		bInProcess,
									VUINT		nStringIDThreadingModel = 0,
									HINSTANCE	hInstance = NULL,
									HINSTANCE	hResource = NULL)
	{
		VASSERT(nStringIDCLSID && nStringIDClassName)

		return RegisterServer(
			(VSTRING_CONST)VString(nStringIDCLSID, hResource),
			(VSTRING_CONST)VString(nStringIDClassName, hResource),
			(nStringIDProgID)
			? (VSTRING_CONST)VString(nStringIDProgID, hResource)
			: NULL,
			(nStringIDProgIndependantID)
			? (VSTRING_CONST)VString(nStringIDProgIndependantID, hResource)
			: NULL,
			bInProcess,
			(nStringIDThreadingModel)
			? (VSTRING_CONST)VString(nStringIDThreadingModel, hResource)
			: NULL,
			hInstance);
	}
	/** Un-Register an ActiveX Server.*/
	static void		UnRegisterServer(	VSTRING_CONST pszCLSID,
										VSTRING_CONST pszProgID,
										VSTRING_CONST pszProgIndependantID)
	{
		/* Delete the CLSID?*/
		if ( pszCLSID )
		{
			VString s(VTEXT("CLSID\\"));
			s += pszCLSID;

			if ( !s.GetErrorCount() )
				VRegistry::DeleteKey(HKEY_CLASSES_ROOT, s);
		}

		if ( pszProgIndependantID )
			VRegistry::DeleteKey(HKEY_CLASSES_ROOT, pszProgIndependantID);

		if ( pszProgID )
			VRegistry::DeleteKey(HKEY_CLASSES_ROOT, pszProgID);
	}

	/** Same as above, but loads strings from string resources.*/
	static void		UnRegisterServer(	VUINT		nStringIDCLSID,
										VUINT		nStringIDProgID,
										VUINT		nStringIDProgIndependantID,
										HINSTANCE	hResource = NULL)
	{
		UnRegisterServer(	VString(nStringIDCLSID, hResource),
							VString(nStringIDProgID, hResource),
							VString(nStringIDProgIndependantID, hResource));
	}
};

#endif /* VACTIVEXREGISTRAR*/
