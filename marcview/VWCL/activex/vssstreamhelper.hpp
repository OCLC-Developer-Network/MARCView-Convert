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

#ifndef VSSSTREAMHELPER
#define VSSSTREAMHELPER

/** Structured Storage helpers with enhancements including ANSI/ASCII/UNICODE
based string functions, boolean return values, and extended functionality.
When using these classes it is assumed that the class takes ownership of the
structured storage interface pointer. As such, it will call Release() when
appropriate. If calling code wishes to keep an interface pointer longer than
the scope of this object, you will need to AddRef() the interface before this
object goes out of scope. Doing this, however, may prevent the rename element
caching from working since it can only rename elements after they have been
released for the final time. Care must be taken here.*/

#include "../vstandard.h"
#include "vunknown.hpp"
#include "vssobjecthelper.hpp"

/* Forward declaration(s).*/
class VSSStorageHelper;

/** VSSStreamHelper is a wrapper for IStream.*/
class VSSStreamHelper : public VSSObjectHelper
{
friend class VSSStorageHelper;

public:
	/** Default constructor initializes by taking ownership of an existing
	IStream object, or none.*/
	VSSStreamHelper(IStream* pStream = NULL)
		: VSSObjectHelper(VSS_RTTI_STREAM, (IUnknown*)pStream, NULL)
		{;}

	/** Creates a new stream object with its own seek pointer that references
	the same bytes as the original stream. There is no management of the
	cloned stream and it must be released when done, or wrapped in an
	additional VSSStreamHelper object.*/
	VBOOL			Clone(IStream** ppStream) const
	{
		VASSERT(ppStream && GetStream())
		return (SUCCEEDED(GetStream()->Clone(ppStream))) ? VTRUE : VFALSE;
	}

	/** Copies a specified number of bytes from the current seek pointer in the
	stream to the current seek pointer in another stream. pDestStream points
	to the destination stream. The stream pointed to by pDestStream can be a
	new stream or a clone of the source stream. nNumBytes specifies the number
	of bytes to copy from the source stream. pnRead is a p ointer to the
	location where this method writes the actual number of bytes read from the
	source. You can set this pointer to NULL to indicate that you are not
	interested in this value. In this case, this method does not provide the
	actual number of bytes read. pnWritten is a pointer to the location where
	this method writes the actual number of bytes written to the destination.
	You can set this pointer to NULL to indicate that you are not interested
	in this value. In this case, this method does not provide the actual number
	of bytes written. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			CopyTo(	IStream*		pDestStream,
							ULARGE_INTEGER	nNumBytes,
							ULARGE_INTEGER*	pnRead = NULL,
							ULARGE_INTEGER*	pnWritten = NULL) const
	{
		VASSERT(pDestStream && GetStream())

		return SUCCEEDED(GetStream()->CopyTo(	pDestStream,
												nNumBytes,
												pnRead,
												pnWritten));
	}

	/** Get locks supported. See LOCKTYPES for more information. Returns 0
	on failure.*/
	VDWORD			GetLocksSupported() const
	{
		STATSTG stg;
		return (Stat(stg)) ? stg.grfLocksSupported : 0;
	}

	/** Get the size of this stream into nSize. Returns VTRUE on success,
	VFALSE on failure.*/
	VBOOL			GetSize(ULARGE_INTEGER& nSize) const
	{
		STATSTG stg;
		return (Stat(stg)) ? VTRUE : VFALSE;
	}

	/** Restricts access to a specified range of bytes in the stream. Supporting
	this functionality is optional since some file systems do not provide it.
	nOffSet specifies the byte offset for the beginning of the range. nRange
	specifies the length of the range, in bytes, to be restricted. nLockType
	specifies the restrictions being requested on accessing the range. The
	byte range can extend past the current end of the stream. Locking beyond
	the end of a stream is useful as a method of communication between
	different instances of the stream without changing data that is actually
	part of the stream. Three types of locking can be supported: locking to
	exclude other writers, locking to exclude other readers or writers, and
	locking that allows only one requestor to obtain a lock on the given range,
	which is usually an alias for one of the other two lock types. A given
	stream instance might support either of the first two types, or both. The
	lock type is specified by nLockType, using a value from the LOCKTYPE
	enumeration. Any region locked with IStream::LockRegion must later be
	explicitly unlocked by calling IStream::UnlockRegion with exactly the
	same values for the nOffset, nRange, and nLockType parameters. The region
	must be unlocked before the stream is released. Two adjacent regions cannot
	be locked separately and then unlocked with a single unlock call. Notes to
	Callers: Since the type of locking supported is optional and can vary in
	different implementations of IStream, you must provide code to deal with
	the STG_E_INVALIDFUNCTION error. This method has no effect in the compound
	file implementation, because the implementation does not support range
	locking. Notes to Implementers: Support for this method is optional for
	implementations of stream objects since it may not be supported by the
	underlying file system. The type of locking supported is also optional.
	The STG_E_INVALIDFUNCTION error is returned if the requested type of
	locking is not supported.*/
	VBOOL			LockRegion(	ULARGE_INTEGER	nOffSet,
								ULARGE_INTEGER	nRange,
								VDWORD			nLockType)
	{
		VASSERT(GetStream())

		return (SUCCEEDED(GetStream()->LockRegion(	nOffSet,
													nRange,
													nLockType)))
													? VTRUE : VFALSE;
	}

	/** Reads a specified number of bytes from the stream object into memory
	starting at the current seek pointer. pBuffer points to the buffer into
	which the stream data is read. If an error occurs, this value is NULL.
	nBufferSize specifies the number of bytes of data to attempt to read from
	the stream object. pnRead is a pointer to a VULONG variable that receives
	the actual number of bytes read from the stream object. You can set this
	pointer to NULL to indicate that you are not interested in this value. In
	this case, this method does not provide the actual number of bytes read.
	Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			Read(	VVOIDPTR	pBuffer,
							VULONG		nBufferSize,
							VULONG*		pnRead = NULL)
	{
		VASSERT(pBuffer && nBufferSize && GetStream())

		return (SUCCEEDED(GetStream()->Read(	pBuffer,
												nBufferSize,
												pnRead)))
												? VTRUE : VFALSE;
	}

	/** Changes the seek pointer to a new location relative to the beginning of
	the stream, to the end of the stream, or to the current seek pointer.
	nOffSet is the displacement to be added to the location indicated by
	nOrigin. If nOrigin is STREAM_SEEK_SET, this is interpreted as an unsigned
	value rather than signed. nOrigin specifies the origin for the displacement
	specified in nOffset. The origin can be the beginning of the file, the
	current seek pointer, or the end of the file. See the STREAM_SEEK
	enumeration for the values. pnNewPosition is a pointer to the location
	where this method writes the value of the new seek pointer from the
	beginning of the stream. You can set this pointer to NULL to indicate that
	you are not interested in this value. In this case, this method does not
	provide the new seek pointer. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			Seek(	LARGE_INTEGER	nOffSet,
							VDWORD			nOrigin = STREAM_SEEK_SET,
							ULARGE_INTEGER*	pnNewPosition = NULL)
	{
		VASSERT(GetStream())

		return (SUCCEEDED(GetStream()->Seek(	nOffSet,
												nOrigin,
												pnNewPosition)))
												? VTRUE : VFALSE;
	}

	/** Changes the size of the stream object. nNewSize specifies the new size
	of the stream as a number of bytes. Call this method to preallocate space
	for the stream. If the nNewSize parameter is larger than the current stream
	size, the stream is extended to the indicated size by filling the
	intervening space with bytes of undefined value. This operation is similar
	to the ISequentialStream::Write method if the seek pointer is past the
	current end-of-stream. If the nNewSize parameter is smaller than the
	current stream, then the stream is truncated to the indicated size. The
	seek pointer is not affected by the change in stream size. Calling
	SetSize() can be an effective way of trying to obtain a large chunk of
	contiguous space. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			SetSize(ULARGE_INTEGER nNewSize)
	{
		VASSERT(GetStream())
		return (SUCCEEDED(GetStream()->SetSize(nNewSize))) ? VTRUE : VFALSE;
	}

	/** Removes the access restriction on a range of bytes previously
	restricted with LockRegion. See this function for more information.
	Returns VTRUE on success, VFALSE on failure.*/
	VBOOL			UnlockRegion(	ULARGE_INTEGER	nOffSet,
									ULARGE_INTEGER	nRange,
									VDWORD			nLockType)
	{
		VASSERT(GetStream())

		return (SUCCEEDED(GetStream()->UnlockRegion(	nOffSet,
														nRange,
														nLockType)))
														? VTRUE : VFALSE;
	}

	/** Writes a specified number of bytes into the stream object starting at
	the current seek pointer. pBuffer is a buffer containing the data that is
	to be written to the stream. A valid pointer must be provided for this
	parameter even when nBufferSize is zero. nBufferSize specifies the number
	of bytes of data to attempt to write into the stream and can be zero.
	pnWritten is a pointer to a VULONG variable where this method writes the
	actual number of bytes written to the stream object. The caller can set
	this pointer to NULL, in which case, this method does not provide the
	actual number of bytes written. Returns VTRUE on success, VFALSE on
	failure.*/
	VBOOL			Write(	VVOIDPTR_CONST	pBuffer,
							VULONG			nBufferSize,
							VULONG*			pnWritten = NULL)
	{
		VASSERT(pBuffer && GetStream())

		return (SUCCEEDED(GetStream()->Write(	pBuffer,
												nBufferSize,
												pnWritten)))
												? VTRUE : VFALSE;
	}

protected:
	/** Protected constructor initializes VSSStorageHelper object.*/
	VSSStreamHelper(	IStream*			pStream,
						VSSStorageHelper*	pParentStorage)
		: VSSObjectHelper(VSS_RTTI_STREAM, (IUnknown*)pStream, pParentStorage)
		{ VASSERT(pStream && pParentStorage) }
};

#endif	/* VSSSTREAMHELPER*/
