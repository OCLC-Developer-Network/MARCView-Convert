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

#ifndef VIMAGELIST
#define VIMAGELIST

#include "../gdi/vbitmap.hpp"
#include "../numbers/vbitmask.hpp"
#include "../structures/vpoint.hpp"
#ifndef VWCL_NO_VWINDOW
	#include "../windows/vwindow.hpp"
#endif

/** VImageList is a simple wrapping of the Win32 ImageList common control.*/
class VImageList
{
public:
	/** Options that can be applied to this class. This can be set during
	construction or by a call to GetOptions().*/
	enum	{	OPTION_AUTO_DESTROY =	0x0001,
				OPTION_DEFAULT =		OPTION_AUTO_DESTROY,
			};

	/** Default constructor initializes members to default values.*/
	VImageList(	HIMAGELIST	hImageList = NULL,
				VUINT		nOptions = OPTION_DEFAULT)
	{
		m_hImageList = hImageList;
		m_Options =		nOptions;
	}

	/** Virtual destructor automatically destroys the internal image list if
	the OPTION_AUTO_DESTROY bit is set.*/
	virtual ~VImageList()
	{
		if ( m_Options.IsSet(OPTION_AUTO_DESTROY) )
			Destroy();
	}

	/** Adds an image or images to the image list. bitmap is a VBitmap
	object that contains the image or images to add. The number of images
	is inferred from the width of the bitmap. pBitmapMask is a pointer to
	the bitmap that contains the mask. If no mask is used with the image
	list, this parameter is ignored. Returns the index of the first new
	image if successful, or -1 otherwise.*/
	VINT				Add(	VBitmap const& bitmap,
								VBitmap const* pBitmapMask = NULL) const
	{
		VASSERT(bitmap.GetHandle() && m_hImageList)

		return ImageList_Add(	m_hImageList,
								bitmap.GetHandle(),
								(HBITMAP)((pBitmapMask)
								? pBitmapMask->GetHandle()
								: NULL));
	}

	/** Add an icon to the end of the image list. Returns the index of the
	first new image if successful, or -1 otherwise.*/
	VINT				AddIcon(HICON hIcon) const
	{
		VASSERT(hIcon)
		return Icon(-1, hIcon);
	}

	/** Adds an image or images to an image list, generating a mask from the
	specified bitmap. bitmap is a VBitmap object that contains the image or
	images to add. The number of images is inferred from the width of the
	bitmap. crMask is the color used to generate the mask. Each pixel of
	this color in the specified bitmap is changed to black, and the
	corresponding bit in the mask is set to 1. The function copies the
	bitmap to an internal data structure.*/
	VINT				AddMasked(	VBitmap const&	bitmap,
									COLORREF		crMask) const
	{
		VASSERT(bitmap.GetHandle())
		VASSERT(m_hImageList)

		return ImageList_AddMasked(m_hImageList, bitmap.GetHandle(), crMask);
	}

	/** Returns the current background color for the image list.*/
	COLORREF			BackColor() const
	{
		VASSERT(m_hImageList)
		return ImageList_GetBkColor(m_hImageList);
	}

	/** Set the background color for the image list, returning the previous
	color, or CLR_NONE otherwise.*/
	COLORREF			BackColor(COLORREF crBackColor) const
	{
		VASSERT(m_hImageList)
		return ImageList_SetBkColor(m_hImageList, crBackColor);
	}

	/** Begins dragging an image. nIndex in the image index to use for
	dragging. nxHotspot and nyHotspot is the location of the drag position
	relative to the upper-left corner of the image. This function creates
	a temporary image list that is used for dragging. In response to
	subsequent WM_MOUSEMOVE messages, you can move the drag image by using
	the DragMove function. To end the drag operation, you can use the
	EndDrag() function. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL				BeginDrag(	VINT nIndex,
									VINT nxHotspot,
									VINT nyHotspot) const
	{
		VASSERT(m_hImageList)

		return (ImageList_BeginDrag(	m_hImageList,
										nIndex,
										nxHotspot,
										nyHotspot))
										? VTRUE
										: VFALSE;
	}

	/** Creates a new image list. Returns VTRUE on success, VFALSE on failure.
	nImageWidth is the width, in pixels, of each image in the list.
	nImageHeight is the height, in pixels, of each image in the list. nFlags
	is a set of bits that specify the type of image list to create. This
	parameter can be a combination of the following values, but it can
	include only one of the ILC_COLOR values.

	ILC_COLOR - Use the default behavior if none of the other ILC_COLOR
	flags is specified. Typically, the default is ILC_COLOR4, but for
	older display drivers, the default is ILC_COLORDDB.

	ILC_COLOR4 - Use a 4-bit (16-color) device-independent bitmap (DIB)
	section as the bitmap for the image list.

	ILC_COLOR8 - Use an 8-bit DIB section. The colors used for the color
	table are the same colors as the halftone palette.

	ILC_COLOR16 - Use a 16-bit (32/64k-color) DIB section.

	ILC_COLOR24 - Use a 24-bit DIB section.

	ILC_COLOR32 - Use a 32-bit DIB section.

	ILC_COLORDDB - Use a device-dependent bitmap.

	ILC_MASK - Use a mask. The image list contains two bitmaps, one of
	which is a monochrome bitmap used as a mask. If this value is not
	included, the image list contains only one bitmap.

	nNumImages is the number of images that the image list initially
	contains. nGrow  is the number of images by which the image list can
	grow when the system needs to make room for new images. This parameter
	represents the number of new images that the resized image list can
	contain.*/
	VBOOL			Create(	VINT	nImageWidth,
							VINT	nImageHeight,
							VINT	nNumImages,
							VINT	nGrow = 0,
							VUINT	nFlags = ILC_COLOR4)
	{
		if ( m_Options.IsSet(OPTION_AUTO_DESTROY) )
			Destroy();

		m_hImageList =
			ImageList_Create(	nImageWidth,
								nImageHeight,
								nFlags,
								nNumImages,
								nGrow);

		return (m_hImageList) ? VTRUE : VFALSE;
	}

	/** Destroy the internal image list.*/
	VBOOL				Destroy()
	{
		if ( m_hImageList && ImageList_Destroy(m_hImageList) )
		{
			m_hImageList = NULL;
			return VTRUE;
		}

		return VFALSE;
	}

	#ifndef VWCL_NO_VWINDOW
		/** Locks updates to the specified window during a drag operation
		and displays the drag image at the specified position within the
		window. nx  is the X-coordinate at which to display the drag image.
		The coordinate is relative to the upper-left corner of the window,
		not the client area. ny is the Y-coordinate at which to display the
		drag image. The coordinate is relative to the upper-left corner of
		the window, not the client area. Returns VTRUE on success, VFALSE
		on failure.*/
		static VBOOL	DragEnter(	VWindow const&	wndLock,
									VINT			nx,
									VINT			ny)
		{
			VASSERT(wndLock.GetSafeWindow())

			return (ImageList_DragEnter(	wndLock.GetHandle(),
											nx,
											ny))
											? VTRUE
											: VFALSE;
		}

		/** Unlocks the specified window and hides the drag image, allowing
		the window to be updated. Returns VTRUE on success, VFALSE on
		failure.*/
		static VBOOL	DragLeave(VWindow const& wndLock)
		{
			VASSERT(wndLock.GetSafeWindow())

			if ( ImageList_DragLeave(wndLock.GetHandle()) )
				return VTRUE;

			return VFALSE;
		}
	#endif

	/** Moves the image that is being dragged during a drag-and-drop
	operation. This function is typically called in response to a
	WM_MOUSEMOVE message. nx is the X-coordinate at which to display the
	drag image. The coordinate is relative to the upper-left corner of the
	window, not the client area. ny is the Y-coordinate at which to display
	the drag image. The coordinate is relative to the upper-left corner of
	the window, not the client area. Returns VTRUE on success, VFALSE on
	failure.*/
	static VBOOL		DragMove(	VINT nx,
									VINT ny)
		{ return (ImageList_DragMove(nx, ny)) ? VTRUE : VFALSE; }

	/** Shows or hides the image being dragged. Returns VTRUE on success,
	VFALSE on failure.*/
	static VBOOL		DragShowNoLock(VBOOL bShow = VTRUE)
		{ return (ImageList_DragShowNolock(bShow)) ? VTRUE : VFALSE; }

	/** Draws an image list item in the specified device context. nIndex is
	the index into the imagelist to draw. dc is the device context to draw
	onto. nx is the X-coordinate at which to draw within the specified
	device context. ny is the Y-coordinate at which to draw within the
	specified device context. nStyle is the drawing style and, optionally,
	the overlay image. This parameter can be a combination of an overlay
	image index and one or more of the following values:

	ILD_BLEND25 or ILD_FOCUS - Draws the image, blending 25 percent with
	the system highlight color. This value has no effect if the image list
	does not contain a mask.

	ILD_BLEND50 or ILD_SELECTED - Draws the image, blending 50 percent with
	the system highlight color. This value has no effect if the image list
	does not contain a mask.

	ILD_MASK - Draws the mask.

	ILD_NORMAL - Draws the image using the background color for the image
	list. If the background color is the CLR_NONE value, the image is drawn
	transparently using the mask.

	ILD_TRANSPARENT - Draws the image transparently using the mask,
	regardless of the background color. This value has no effect if the
	image list does not contain a mask.

	An overlay image is drawn transparently over the primary image
	specified in the nIndex parameter. To specify an overlay image in the
	nStyle parameter, use the INDEXTOOVERLAYMASK macro to shift the
	one-based index of the overlay image. Use the OR operator to logically
	combine the return value of the macro with the drawing style flags
	specified in the nStyle parameter.
	You must first specify this image as an overlay image by using the
	SetOverlayImage function. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL				Draw(	VINT	nIndex,
								VDC&	dc,
								VINT	nx,
								VINT	ny,
								VUINT	nStyle = ILD_NORMAL) const
	{
		VASSERT(m_hImageList && dc.GetHandle())

		return (ImageList_Draw(	m_hImageList,
								nIndex,
								dc.GetHandle(),
								nx,
								ny,
								nStyle))
								? VTRUE
								: VFALSE;
	}

	/** Draws an image list item in the specified device context. The
	function uses the specified drawing style and blends the image with the
	specified color. nIndex is the index of the image to draw. dc is the
	device context to draw onto. nx is the X-coordinate at which to draw
	within the specified device context. ny is the Y-coordinate at which to
	draw within the specified device context. nWidth is the width of the
	portion of the image to draw relative to the upper-left corner of the
	image. If nWidth and nHeight are zero, the function draws the entire
	image. The function does not ensure that the parameters are valid.
	nHeight is the height of the portion of the image to draw, relative to
	the upper-left corner of the image. If nWidth and nHeight are zero, the
	function draws the entire image. The function does not ensure that the
	parameters are valid. crBackColor is the background color of the image.
	This parameter can be an application-defined RGB value or one of the
	following values:

	CLR_NONE - No background color. The image is drawn transparently.

	CLR_DEFAULT - Default background color. The image is drawn using the
	background color of the image list. This parameter is used only if the
	image list was created with the ILC_MASK value. crForeColor is the
	foreground color of the image. This parameter can be an
	application-defined RGB value or one of the following values:

	CLR_NONE - No blend color. The image is blended with the color of the
	destination device context.

	CLR_DEFAULT - Default foreground color. The image is drawn using the
	system highlight color as the foreground color. This parameter is used
	only if nStyle includes the ILD_BLEND25 or ILD_BLEND50 value. nStyle is
	the drawing style and, optionally, the overlay image. This parameter can
	be a combination of an overlay image index and one or more of the
	following values:

	ILD_BLEND25 or ILD_FOCUS - Draws the image, blending 25 percent with
	the blend color specified by crForeColor. This value has no effect if
	the image list does not contain a mask.

	ILD_BLEND50, ILD_SELECTED, or ILD_BLEND - Draws the image, blending 50
	percent with the blend color specified by crForeColor. This value has
	no effect if the image list does not contain a mask.

	ILD_MASK - Draws the mask.

	ILD_NORMAL - Draws the image using the background color for the image
	list. If the background color is the CLR_NONE value, the image is drawn
	transparently using the mask.

	ILD_TRANSPARENT - Draws the image transparently using the mask,
	regardless of the background color. This value has no effect if
	the image list does not contain a mask.

	An overlay image is drawn transparently over the primary image specified
	in the nIndex parameter. To specify an overlay image in the nStyle
	parameter, use the INDEXTOOVERLAYMASK macro to shift the one-based
	index of the overlay image. Use the OR operator to logically combine
	the return value of the macro with the drawing style flags specified
	in the nStyle parameter. You must first specify this image as an
	overlay image by using the SetOverlayImage function. Returns VTRUE on
	success, VFALSE on failure.*/
	VBOOL				DrawEx(	VINT		nIndex,
								VDC&		dc,
								VINT		nx,
								VINT		ny,
								VINT		nWidth,
								VINT		nHeight,
								COLORREF	crBackColor,
								COLORREF	crForeColor,
								VUINT		nStyle = ILD_NORMAL) const
	{
		VASSERT(m_hImageList && dc.GetHandle())

		return ImageList_DrawEx(	m_hImageList,
									nIndex,
									dc.GetHandle(),
									nx,
									ny,
									nWidth,
									nHeight,
									crBackColor,
									crForeColor,
									nStyle);
	}

	/** Ends a drag operation.*/
	static void			EndDrag()
		{ ImageList_EndDrag(); }

	/** Retrieves the temporary image list that is used for the drag image.
	The function also retrieves the current drag position and the offset of
	the drag image relative to the drag position. Returns the handle to the
	imagelist if successful, or NULL otherwise. pPoint is the address of a
	VPoint object that receives the current drag position. It can be NULL.
	pPointHotspot is the address of a VPoint object that receives the offset
	of the drag image relative to the drag position. It can be NULL. The
	temporary image list is destroyed when the EndDrag() function is called.
	To begin a drag operation, use the BeginDrag function.*/
	static HIMAGELIST	GetDragImage(	VPoint* pPoint,
										VPoint* pPointHotspot = NULL)
	{
		return ImageList_GetDragImage(	(pPoint)
										? pPoint->GetPointer()
										: NULL,
										(pPointHotspot)
										? pPointHotspot->GetPointer()
										: NULL);
	}

	/** Retrieves the dimensions of images in an image list. All images in
	an image list have the same dimensions. nWidth receives the width, in
	pixels, of each image. nHeight receives the height, in pixels, of each
	image. Returns VTRUE on success, VFALSE on failure.*/
	VBOOL				GetIconSize(	VINT& nWidth,
										VINT& nHeight) const
	{
		VASSERT(m_hImageList)

		return (ImageList_GetIconSize(	m_hImageList,
										&nWidth,
										&nHeight))
										? VTRUE
										: VFALSE;
	}

	/** Returns the number of images in an image list.*/
	VINT				GetImageCount() const
	{
		VASSERT(m_hImageList)
		return (m_hImageList) ? ImageList_GetImageCount(m_hImageList) : 0;
	}

	/** Retrieves information about an image. nIndex is the index of the
	image to get information about. imageInfo is a structure that receives
	information about the image. The information in this structure can be
	used to directly manipulate the bitmaps for the image. Returns VTRUE on
	success, VFALSE on failure.*/
	VBOOL				GetImageInfo(	VINT		nIndex,
										IMAGEINFO&	imageInfo) const
	{
		VASSERT(m_hImageList)

		return (ImageList_GetImageInfo(	m_hImageList,
										nIndex,
										&imageInfo))
										? VTRUE
										: VFALSE;
	}

	/** Returns a reference to the internal imagelist.*/
	HIMAGELIST&			GetHandle() const
		{ return (HIMAGELIST&)m_hImageList; }

	/** Return a reference to the options object for this class.*/
	VBitmask&			GetOptions() const
		{ return (VBitmask&)m_Options; }

	/** Creates an icon from an image and mask in an image list. Returns the
	handle to the icon if successful, or NULL otherwise. nIndex is the
	index of the image in the imagelist. nFlags is a combination of flags
	that specify the drawing style. For a list of values, see the description
	of the Draw() function. It is the responsibility of the calling
	application to destroy the icon returned from this function using
	the DestroyIcon() function.*/
	HICON				Icon(	VINT	nIndex,
								VUINT	nFlags = ILD_NORMAL) const
	{
		VASSERT(m_hImageList)
		return ImageList_GetIcon(m_hImageList, nIndex, nFlags);
	}

	/** Replaces an image with an icon or cursor. Returns the index of the
	image if successful, or -1 otherwise. nIndex is the index of the image
	in the imagelist to replace. If i is -1, the function appends the image
	to the end of the list, but AddIcon() is better for this. hIcon is a
	handle to the icon or cursor that contains the bitmap and mask for the
	new image. Because the system does not save hIcon, you can destroy it
	after the function returns if the icon or cursor was created by the
	CreateIcon() function. You do not need to destroy hIcon if it was loaded
	by the LoadIcon() function; the system automatically frees an icon
	resource when it is no longer needed.*/
	VINT				Icon(	VINT	nIndex,
								HICON	hIcon) const
	{
		VASSERT(m_hImageList && hIcon)
		return ImageList_ReplaceIcon(m_hImageList, nIndex, hIcon);
	}

	/** Load a bitmap from resources and set as the imagelist. nBitmapID is
	the bitmap resource ID. nImageWidth is is the width of each bitmap in
	the the imagelist. nGrow is the number of images by which the image
	list can grow when the system needs to make room for new images. crMask
	if the mask color. If hResource is not given, VGetResourceHandle() is
	assumed. Returns VTRUE on success, VFALSE on failure. See LoadImage()
	for more information.*/
	VBOOL				LoadBitmap(	VUINT		nBitmapID,
									VINT		nImageWidth,
									VINT		nGrow = 0,
									COLORREF	crMask = CLR_NONE,
									HINSTANCE	hResource = NULL)
	{
		return LoadImage(	MAKEINTRESOURCE(nBitmapID),
							nImageWidth,
							nGrow,
							crMask,
							IMAGE_BITMAP,
							0,
							(hResource) ? hResource : VGetResourceHandle());
	}

	/** Load a cursor from resources and set as the imagelist. nCursorID is
	the cursor resource ID. nImageWidth is is the width of each cursor in
	the the imagelist. nGrow is the number of images by which the image
	list can grow when the system needs to make room for new images. crMask
	if the mask color. If hResource is not given, VGetResourceHandle() is
	assumed. Returns VTRUE on success, VFALSE on failure. See LoadImage()
	for more information.*/
	VBOOL				LoadCursor(	VUINT		nCursorID,
									VINT		nImageWidth = 32,
									VINT		nGrow = 0,
									COLORREF	crMask = CLR_NONE,
									HINSTANCE	hResource = NULL)
	{
		return LoadImage(	MAKEINTRESOURCE(nCursorID),
							nImageWidth,
							nGrow,
							crMask,
							IMAGE_CURSOR,
							0,
							(hResource) ? hResource : VGetResourceHandle());
	}

	/** Load an icon from resources and set as the imagelist. nIconID is the
	icon resource ID. nImageWidth is is the width of each icon in the the
	imagelist. nGrow is the number of images by which the image list can
	grow when the system needs to make room for new images. crMask if the
	mask color. If hResource is not given, VGetResourceHandle() is assumed.
	Returns VTRUE on success, VFALSE on failure. See LoadImage() for
	more information.*/
	VBOOL				LoadIcon(	VUINT		nIconID,
									VINT		nImageWidth = 32,
									VINT		nGrow = 0,
									COLORREF	crMask = CLR_NONE,
									HINSTANCE	hResource = NULL)
	{
		return LoadImage(	MAKEINTRESOURCE(nIconID),
							nImageWidth,
							nGrow,
							crMask,
							IMAGE_ICON,
							0,
							(hResource) ? hResource : VGetResourceHandle());
	}

	/** Creates an image list from the specified bitmap, cursor, or icon
	resource of from a file. pszImage identifies the image to load. If the
	nFlags parameter includes LR_LOADFROMFILE, pszImage is the address of a
	null-terminated string that names the file containing the image to load.
	If the hResource parameter is non-NULL and LR_LOADFROMFILE is not
	specified, pszImage is the address of a null-terminated string that
	contains the name of the image resource in the hResource module. If
	hResource is NULL and LR_LOADFROMFILE is not specified, the low-order
	word of this parameter must be the identifier of an OEM image to load.
	To create this value, use the MAKEINTRESOURCE macro with one of the OEM
	image identifiers defined in WINUSER.H. These identifiers have the
	following prefixes:

	OBM_ - For OEM bitmaps.

	OIC_  - For OEM icons.

	OCR_  - For OEM cursors.

	The height of each image and the initial number of images are inferred
	by the dimensions of the specified resource. nGrow is the number of
	images by which the image list can grow when the system needs to make
	room for new images. This parameter represents the number of new images
	that the resized image list can contain. crMask is the color used to
	generate a mask. Each pixel of this color in the specified bitmap,
	cursor, or icon is changed to black, and the corresponding bit in the
	mask is set to 1. If this parameter is the CLR_NONE value, no mask is
	generated. If this parameter is the CLR_DEFAULT value, the color of the
	pixel at the upper-left corner of the image is treated as the mask
	color. nType is a flag that specifies the type of image to load. This
	parameter can be one of the following:

	IMAGE_BITMAP - Loads a bitmap.

	IMAGE_CURSOR - Loads a cursor.

	IMAGE_ICON - Loads an icon.

	nFlags specify how to load the image. This parameter can be a
	combination of the following values:

	LR_DEFAULTCOLOR - Uses the color format of the display.

	LR_LOADDEFAULTSIZE - Uses the width or height specified by the system
	metric values for cursors and icons if the cx parameter is set to zero.
	If this value is not specified and cx is set to zero, the function sets
	the size to the one specified in the resource. If the resource contains
	multiple images, the function sets the size to that of the first image.

	LR_LOADFROMFILE - Loads the image from the file specified by the lpbmp
	parameter.

	LR_LOADMAP3DCOLORS - Searches the color table for the image and
	replaces the following shades of gray with the corresponding
	three-dimensional color:

	Dark Gray: RGB(128, 128, 128) - COLOR_3DSHADOW

	Gray: RGB(192, 192, 192) - COLOR_3DFACE

	Light Gray: RGB(223, 223, 223) - COLOR_3DLIGHT

	LR_LOADTRANSPARENT - Retrieves the color value of the first pixel in
	the image and replaces the corresponding entry in the color table with
	the default window color (the COLOR_WINDOW display color). All pixels
	in the image that use that color become the default window value color.
	This value applies only to images that have a corresponding color table.

	LR_MONOCHROME - Loads the image in black and white.

	LR_SHARED - Shares the image handle if the image is loaded multiple
	times. Do not use this value for images that have nontraditional sizes
	that might change after loading or for images that are loaded from a
	file.

	LR_LOADTRANSPARENT - Does not load the image transparently. It creates
	an opaque image list that only appears transparent because all of the
	"background" pixels have been changed to COLOR_WINDOW. If the images
	are drawn over a background that is not the color COLOR_WINDOW, the
	image will not draw properly. Also, LR_LOADTRANSPARENT and
	LR_LOADMAP3DCOLORS use the system colors that were in effect at the
	time that ImageList_LoadImage was called. If the system colors
	subsequently change, the application must reload the image to remap the
	colors.

	Returns VTRUE on success, VFALSE on failure.*/
	VBOOL				LoadImage(	VSTRING_CONST	pszImage,
									VINT			nImageWidth,
									VINT			nGrow = 0,
									COLORREF		crMask = CLR_NONE,
									VUINT			nType = IMAGE_BITMAP,
									VUINT			nFlags = 0,
									HINSTANCE		hResource = NULL)
	{
		VASSERT(pszImage)

		m_hImageList =
			ImageList_LoadImage(	(hResource)
									? hResource
									: VGetResourceHandle(),
									pszImage,
									nImageWidth,
									nGrow,
									crMask,
									nType,
									nFlags);

		return (m_hImageList) ? VTRUE : VFALSE;
	}

	/** Creates a new image by combining this imagelist with another
	existing images. The function also creates a new imagelist in which to
	store the image. Returns the handle to the new image list if successful,
	or NULL otherwise. nIndex is the index of the first existing image.
	imageList2 is the second imagelist to combine with this one to create a
	new one. nIndex2 is the index of the second existing image. nx and ny
	is the offset of the second image relative to the first image. The new
	imagelist consists of the second existing image drawn transparently
	over the first. The mask for the new image is the result of performing
	a logical OR operation on the masks of the two existing images.*/
	HIMAGELIST			Merge(	VINT				nIndex,
								VImageList const&	imageList2,
								VINT				nIndex2,
								VINT				nx,
								VINT				ny) const
	{
		VASSERT(m_hImageList)
		VASSERT(imageList2.GetHandle())

		return ImageList_Merge(	m_hImageList,
								nIndex,
								imageList2.GetHandle(),
								nIndex2,
								nx,
								ny);
	}

	/** Removes the image at nIndex from an image list. nIndex is the index
	of the image to remove. If this parameter is -1, the function removes
	all images. When an image is removed, the indexes of the remaining
	images are adjusted so that the image indexes always range from zero to
	one less than the number of images in the imagelist. For example, if
	you remove the image at index 0, then image 1 becomes image 0, image 2
	becomes image 1, and so on.*/
	VBOOL				Remove(VINT nIndex) const
	{
		VASSERT(m_hImageList)
		return (ImageList_Remove(m_hImageList, nIndex)) ? VTRUE : VFALSE;
	}

	/** Remove all images from the imagelist. The imagelist itself remains
	valid, although empty.*/
	VBOOL				RemoveAll() const
		{ return Remove(-1); }

	/** Replaces an image in an image list with a new image. nIndex is the
	index of the image to replace. bitmap is an existing bitmap object that
	contains the image. pBitmapMask is an existing object that contains the
	mask. If no mask is used with the image list, this parameter is ignored.
	Returns VTRUE on success, VFALSE on failure.*/
	VBOOL				Replace(	VINT			nIndex,
									VBitmap const&	bitmap,
									VBitmap*		pBitmapMask = NULL) const
	{
		VASSERT(m_hImageList && (HBITMAP)bitmap.GetHandle())

		return ImageList_Replace(	m_hImageList,
									nIndex,
									(HBITMAP)bitmap.GetHandle(),
									(HBITMAP)((pBitmapMask)
									? pBitmapMask->GetHandle()
									: NULL));
	}

	/** Sets the dimensions of images in an image list and removes all
	images from the list. nWidth is the width, in pixels, of the images in
	the image list. All images in an image list have the same dimensions.
	nHeight is the height, in pixels, of the images in the image list. All
	images in an image list have the same dimensions. Returns VTRUE on
	success, VFALSE on failure.*/
	VBOOL				SetIconSize(	VINT nWidth,
										VINT nHeight) const
	{
		VASSERT(m_hImageList)

		return (ImageList_SetIconSize(	m_hImageList,
										nWidth,
										nHeight))
										? VTRUE
										: VFALSE;
	}

	/** Adds a specified image to the list of images to be used as overlay
	masks. An imagelist can have up to four overlay masks in version 4.70
	and earlier and up to 15 in version 4.71. The function assigns an
	overlay mask index to the specified image. nImage is the zero-based
	index of an image in the imagelist. This index identifies the image to
	use as an overlay mask. nOverlayIndex is the one-based index of the
	overlay mask. An overlay mask is an image drawn transparently over
	another image. To draw an overlay mask over an image, call the Draw()
	or DrawEx() function. The nStyle parameter of these functions can use
	the INDEXTOOVERLAYMASK macro to specify an overlay mask index. Returns
	VTRUE on success, VFALSE on failure.*/
	VBOOL				SetOverlayImage(	VINT nImage,
											VINT nOverlayIndex) const
	{
		VASSERT(m_hImageList)

		return (ImageList_SetOverlayImage(	m_hImageList,
											nImage,
											nOverlayIndex))
											? VTRUE
											: VFALSE;
	}

protected:
	/** Embedded Members(s).*/
	VBitmask			m_Options;
	HIMAGELIST			m_hImageList;
};

#endif /* VIMAGELIST*/
