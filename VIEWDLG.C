#pragma	title("Icon/Pointer Viewer  --  Version 1  --  (ViewDlg.C)")
#pragma	subtitle("   Module Purpose - Interface Definitions")

#define	INCL_GPI		   /* Include OS/2 PM GPI Interface	*/
#define	INCL_WIN		   /* Include OS/2 PM Windows Interface	*/

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "appdefs.h"
#include "makeptr.h"

/* This	module contains	routine	used to	create the defined pointer	*/
/* and test its	appearance.						*/

/* Filename:   ViewDlg.C						*/

/*  Version:   1							*/
/*  Created:   1995-11-06						*/
/*  Revised:   1995-11-24						*/

/* Routines:   VOID CreateMask(LONG cx,	LONG cy);			*/
/*	       VOID CreateBackgroundBitmap(VOID);			*/
/*	       VOID DestroyBackgroundBitmap(VOID);			*/
/*	       VOID SaveBackground(HWND	hWnd);				*/
/*	       VOID RestoreBackground(VOID);				*/
/*	       VOID BuildMaskImage(VOID);				*/
/*	       MRESULT EXPENTRY	ViewDlgProc(HWND hWnd, ULONG msg,	*/
/*					    MPARAM mp1,	MPARAM mp2);	*/


/************************************************************************/
/************************************************************************/
/************************************************************************/
/* DISCLAIMER OF WARRANTIES:						*/
/* -------------------------						*/
/* The following [enclosed] code is sample code	created	by IBM		*/
/* Corporation and Prominare Inc.  This	sample code is not part	of any	*/
/* standard IBM	product	and is provided	to you solely for the purpose	*/
/* of assisting	you in the development of your applications.  The code	*/
/* is provided "AS IS",	without	warranty of any	kind.  Neither IBM nor	*/
/* Prominare shall be liable for any damages arising out of your	*/
/* use of the sample code, even	if they	have been advised of the	*/
/* possibility of such damages.						*/
/************************************************************************/
/************************************************************************/
/************************************************************************/
/*		       D I S C L A I M E R				*/
/* This	code is	provided on an as is basis with	no implied support.	*/
/* It should be	considered freeware that cannot	be rebundled as		*/
/* part	of a larger "*ware" offering without our consent.		*/
/************************************************************************/
/************************************************************************/
/************************************************************************/

/* Copyright ¸ International Business Machines Corp., 1995.		*/
/* Copyright ¸ 1995  Prominare Inc.  All Rights	Reserved.		*/

/* --------------------------------------------------------------------	*/


POINTL		 ptlMask;
POINTL		 ptlImage;
POINTL		 ptlMouse;
POINTL		 ptlBackground;

HBITMAP		 hbmXOR;
HBITMAP		 hbmImage;
HBITMAP		 hbmMask;
HBITMAP		 hbmBackground;

HDC hDC;			   /* Screen Device Context Handle	*/
HPS hpsMem;			   /* Memory Presentation Space	Handle	*/
HPS hpsDesktop;			   /* Desktop Presentation Space Handle	*/

POINTL		 aptlBackground[4];/* Background Display Points		*/
POINTL		 aptlImage[4];	   /* Image Pointer Display Points	*/
POINTL		 aptlSource[3];	   /* Bitmap Region Points Array	*/
BITMAPINFOHEADER2 bmInfo;	   /* Bitmap Information Header		*/
LONG		  alBmpFormats[2]; /* Bitmap Formats Array		*/

BOOL		 fTest = FALSE;

/* --- Module Prototype	Definitions -----------------------------------	*/

VOID CreateMask(LONG cx, LONG cy);
VOID CreateBackgroundBitmap(VOID);
VOID DestroyBackgroundBitmap(VOID);
VOID SaveBackground(HWND hWnd);
VOID RestoreBackground(VOID);
VOID BuildMaskImage(VOID);

#pragma	subtitle("   View Images - Mask Creation Function")
#pragma	page( )

/* --- CreateMask -------------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	create the necessary black and white	*/
/*     AND and XOR mask.						*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     LONG cx;	= Image	Width						*/
/*     LONG cy;	= Image	Height						*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID CreateMask(LONG cx, LONG cy)

{
SIZEL  sizl;			   /* Size Holder			*/
POINTL aptl[4];			   /* Bitmap Region Points Array	*/

		       /* Set the width	and height of the mask bitmap	*/
sizl.cx	= cx;
sizl.cy	= cy * 2;
		       /* Open the device context and create a memory	*/
		       /* DC for the bitmap				*/

if ( !(hDC = DevOpenDC(hAB, OD_MEMORY, "*", 0L,	(PDEVOPENDATA)NULL, (HDC)NULL))	)
   {
   WinMessageBox(HWND_DESKTOP, hwndMakePtr, "Cannot open memory DC",
		 "Pointer Creation Tool", 0UL, MB_OK | MB_ICONHAND | MB_MOVEABLE);
   return;
   }
else
   if (	!(hpsMem = GpiCreatePS(hAB, hDC, &sizl,	PU_PELS	| GPIT_MICRO | GPIA_ASSOC)) )
       {
       DevCloseDC(hDC);
       WinMessageBox(HWND_DESKTOP, hwndMakePtr,	"Cannot open memory PS",
		     "Pointer Creation Tool", 0UL, MB_OK | MB_ICONHAND | MB_MOVEABLE);
       return;
       }
		       /* Get the bitmap formats for the current	*/
		       /* display.  This will allow the	code to	work	*/
		       /* on any display				*/

GpiQueryDeviceBitmapFormats(hpsMem, 2L,	alBmpFormats);

		       /* Initialize the bitmap	header information for	*/
		       /* the window/dialogue being copied to the	*/
		       /* clipboard					*/

memset(&bmInfo,	0, sizeof(BITMAPINFOHEADER2));
bmInfo.cbFix	 = sizeof(BITMAPINFOHEADER2);
bmInfo.cx	 = (ULONG)cx;
bmInfo.cy	 = (ULONG)cy * 2;
bmInfo.cPlanes	 = (USHORT)alBmpFormats[0];
bmInfo.cBitCount = (USHORT)1;

		       /* Create the bitmap for	the XOR/AND mask and	*/
		       /* set the bitmap created as the	current	bitmap	*/

GpiSetBitmap(hpsMem, hbmMask = GpiCreateBitmap(hpsMem, &bmInfo,	0L, NULL, NULL));

memset(aptl, 0,	sizeof(POINTL) * 4);

aptl[3].x = (aptl[1].x = cx) + 1L;
aptl[3].y = (aptl[1].y = cy) + 1L;

GpiSetColor(hpsMem, CLR_WHITE);
GpiSetBackColor(hpsMem,	CLR_BLACK);

GpiWCBitBlt(hpsMem, hbmXOR, 4L,	aptl, ROP_ZERO,	BBO_OR);

aptl[0].y = cy;
aptl[1].y = cy << 1UL;

GpiSetBackColor(hpsMem,	CLR_WHITE);
GpiSetColor(hpsMem, CLR_BLACK);

GpiWCBitBlt(hpsMem, hbmXOR, 4L,	aptl, ROP_SRCCOPY, BBO_OR);

		       /* Set the memory bitmap	as the current bitmap	*/

GpiSetBitmap(hpsMem, (HBITMAP)NULL);

		       /* Destroy the memory device context		*/

GpiAssociate(hpsMem, (HDC)NULL);
DevCloseDC(hDC);
		       /* Destroy the presentation spaces used		*/
GpiDestroyPS(hpsMem);

}
#pragma	subtitle("   View Images - Background Bitmap Create Function")
#pragma	page( )

/* --- CreateBackgroundBitmap -------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	create the bitmap that will contain	*/
/*     the background bitmap image.					*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     Nothing								*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID CreateBackgroundBitmap(VOID)

{
SIZEL  sizl;			   /* Size Holder			*/

		       /* Initialize the background bitmap display	*/
		       /* points					*/

memset(aptlBackground, 0, sizeof(POINTL) * 4UL);
aptlBackground[3].x = abm[iBitmap].cx +	2;
aptlBackground[3].y = abm[iBitmap].cy +	2;

		       /* Initialize the size of the bitmap		*/

sizl.cx	= abm[iBitmap].cx + 1L;
sizl.cy	= abm[iBitmap].cy + 1L;

		       /* Initialize the bitmap	header information for	*/
		       /* the background bitmap	image			*/

memset(&bmInfo,	0, sizeof(BITMAPINFOHEADER2));
bmInfo.cbFix	 = sizeof(BITMAPINFOHEADER2);
bmInfo.cx	 = (ULONG)abm[iBitmap].cx;
bmInfo.cy	 = (ULONG)abm[iBitmap].cy;
bmInfo.cPlanes	 = (USHORT)alBmpFormats[0];
bmInfo.cBitCount = (USHORT)alBmpFormats[1];

		       /* Open the device context and create a memory	*/
		       /* DC for the bitmap				*/

if ( !(hDC = DevOpenDC(hAB, OD_MEMORY, "*", 0L,	(PDEVOPENDATA)NULL, (HDC)NULL))	)
   {
   WinMessageBox(HWND_DESKTOP, hwndMakePtr, "Cannot open memory DC",
		 "Pointer Creation Tool", 0UL, MB_OK | MB_ICONHAND | MB_MOVEABLE);
   return;
   }
else
   if (	!(hpsMem = GpiCreatePS(hAB, hDC, &sizl,	PU_PELS	| GPIT_MICRO | GPIA_ASSOC)) )
       {
       DevCloseDC(hDC);
       WinMessageBox(HWND_DESKTOP, hwndMakePtr,	"Cannot open memory PS",
		     "Pointer Creation Tool", 0UL, MB_OK | MB_ICONHAND | MB_MOVEABLE);
       return;
       }
		       /* Create the bitmap for	the background and	*/
		       /* set the bitmap created as the	current	bitmap	*/

hbmBackground =	GpiCreateBitmap(hpsMem,	&bmInfo, 0L, NULL, NULL);

		       /* Initialize the source	points			*/
aptlSource[0].x	=
aptlSource[0].y	= 0L;
aptlSource[1].x	= abm[iBitmap].cx;
aptlSource[1].y	= abm[iBitmap].cy;

		       /* Get the PS for the desktop since this	is	*/
		       /* used to capture and restore the background	*/
		       /* bitmap area					*/

hpsDesktop = WinGetScreenPS(HWND_DESKTOP);

}
#pragma	subtitle("   View Images - Background Bitmap Destroy Function")
#pragma	page( )

/* --- DestroyBackgroundBitmap ------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	destroy	the bitmap that	contains	*/
/*     the background bitmap image.					*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     Nothing								*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID DestroyBackgroundBitmap(VOID)

{
		       /* Restore the background area			*/
RestoreBackground( );
		       /* Destroy the memory device context		*/

GpiAssociate(hpsMem, (HDC)NULL);
DevCloseDC(hDC);
		       /* Destroy the presentation spaces used		*/
GpiDestroyPS(hpsMem);
WinReleasePS(hpsDesktop);
		       /* Delete the bitmap				*/

GpiDeleteBitmap(hbmBackground);
}
#pragma	subtitle("   View Images - Save Background Function")
#pragma	page( )

/* --- SaveBackground ---------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	copy the background where the pointer	*/
/*     will be displayed.  When	the pointer moves to a new location,	*/
/*     the old background will be refreshed using the captured image.	*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND hWnd; = Dialogue Window Handle				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID SaveBackground(HWND hWnd)

{
		       /* Map the target window/dialogue points	to the	*/
		       /* screen desktop				*/

WinMapWindowPoints(hWnd, HWND_DESKTOP, &ptlBackground, 1L);
aptlBackground[1].x = (aptlBackground[0].x = aptlSource[2].x = ptlBackground.x)	+ (aptlBackground[3].x - 1L);
aptlBackground[1].y = (aptlBackground[0].y = aptlSource[2].y = ptlBackground.y)	+ (aptlBackground[3].y - 1L);

		       /* Set the background bitmap as the current	*/
		       /* bitmap					*/

GpiSetBitmap(hpsMem, hbmBackground);

		       /* Copy the background area to the bitmap	*/

GpiBitBlt(hpsMem, hpsDesktop, 3L, aptlSource, ROP_SRCCOPY, BBO_IGNORE);

		       /* Set the memory bitmap	as the current bitmap	*/

GpiSetBitmap(hpsMem, (HBITMAP)NULL);

}
#pragma	subtitle("   View Images - Restore Background Function")
#pragma	page( )

/* --- RestoreBackground ------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	copy the background where the pointer	*/
/*     will be displayed.  When	the pointer moves to a new location,	*/
/*     the old background will be refreshed using the captured image.	*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     Nothing								*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID RestoreBackground(VOID)

{

GpiWCBitBlt(hpsDesktop,	hbmBackground, 4L, aptlBackground, ROP_SRCCOPY,	BBO_IGNORE);

}
#pragma	subtitle("   View Images - Bitmap Mask Build Function")
#pragma	page( )

/* --- hbmBuildMask ------------------------------------ [ Public ] ---	*/
/*									*/
/*     This function is	used to	build the AND/XOR mask along with the	*/
/*     final colour image bitmap.					*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     Nothing								*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     Nothing								*/
/*									*/
/* --------------------------------------------------------------------	*/

VOID BuildMaskImage(VOID)

{
HPS			hPS;	   /* Presentation Space Handle		*/
PBITMAPARRAYFILEHEADER	pbafh;	   /* Bitmap Array File	Header Pointer	*/
PBITMAPARRAYFILEHEADER2	pbafh2;	   /* Bitmap Array File	Header Pointer	*/
PBITMAPFILEHEADER	pbfh;	   /* Bitmap Array File	Header Pointer	*/
PBITMAPFILEHEADER2	pbfh2;	   /* Bitmap Array File	Header Pointer	*/
PBITMAPINFO		pbmi;	   /* Bitmap Info Pointer		*/
PBITMAPINFO2		pbmi2;	   /* Bitmap Info Pointer		*/
RGB			rgbBlack;  /* Black RGB				*/
RGB2			rgb2Black; /* Black RGB				*/
RGB2			rgb2White; /* White RGB				*/
register INT i;			   /* Loop Counter			*/

		       /* Check	to make	sure that the bitmap is	using	*/
		       /* a colour table since bitmaps with more than	*/
		       /* 256 colours use the actual bits of the image	*/
		       /* to describe the colour			*/

if ( abm[iBitmap].cColours <= 256L )
   {
		       /* Check	to see if the image is a OS/2 2.x	*/
		       /* format					*/
   if (	f20Bitmap )
       {
		       /* Check	to see if the bitmap is	an array and	*/
		       /* depending on type, use appropriate		*/
		       /* structures to	locate the RGB table		*/

       if ( (cBitmaps == 1) && !fBitmapArray )
	   {
	   pbfh2 = (PBITMAPFILEHEADER2)abm[iBitmap].pb;
	   memcpy(pbmi2	= (PBITMAPINFO2)malloc(sizeof(BITMAPINFOHEADER2) + abm[iBitmap].cColours * sizeof(RGB2)),
		  &pbfh2->bmp2,	sizeof(BITMAPINFOHEADER2) + abm[iBitmap].cColours * sizeof(RGB2));
	   }
       else
	   {
	   pbafh2 = (PBITMAPARRAYFILEHEADER2)abm[iBitmap].pb;
	   memcpy(pbmi2	= (PBITMAPINFO2)malloc(sizeof(BITMAPINFOHEADER2) + abm[iBitmap].cColours * sizeof(RGB2)),
		  &pbafh2->bfh2.bmp2, sizeof(BITMAPINFOHEADER2)	+ abm[iBitmap].cColours	* sizeof(RGB2));
	   }
       rgb2White.bRed =	rgb2White.bBlue	= rgb2White.bGreen = (BYTE)0xff;
       rgb2Black.bRed =	rgb2Black.bBlue	= rgb2Black.bGreen = (BYTE)0x00;
       rgb2Black.fcOptions = rgb2White.fcOptions = (BYTE)0x00;

       pbmi2->argbColor[iClr] =	rgb2Black;

       hbmImage	= GpiCreateBitmap(hPS =	WinGetPS(HWND_DESKTOP),	(PBITMAPINFOHEADER2)pbmi2, CBM_INIT, abm[iBitmap].pbImage, pbmi2);
       for ( i = 0; i <	(INT)abm[iBitmap].cColours; i++	)
	   pbmi2->argbColor[i] = rgb2White;
       pbmi2->argbColor[iClr] =	rgb2Black;
       hbmXOR =	GpiCreateBitmap(hPS, (PBITMAPINFOHEADER2)pbmi2,	CBM_INIT, abm[iBitmap].pbImage,	pbmi2);

       WinReleasePS(hPS);
       free(pbmi2);
       }
   else
		       /* Image	a OS/2 1.x format, use the appropriate	*/
		       /* structures					*/
       {
		       /* Check	to see if the bitmap is	an array and	*/
		       /* depending on type, use appropriate		*/
		       /* structures to	locate the RGB table		*/

       if ( (cBitmaps == 1) && !fBitmapArray )
	   {
	   pbfh	= (PBITMAPFILEHEADER)abm[iBitmap].pb;
	   memcpy(pbmi = (PBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER) +	abm[iBitmap].cColours *	sizeof(RGB)),
		  &pbfh->bmp, sizeof(BITMAPINFOHEADER) + abm[iBitmap].cColours * sizeof(RGB));
	   }
       else
	   {
	   pbafh = (PBITMAPARRAYFILEHEADER)abm[iBitmap].pb;
	   memcpy(pbmi = (PBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER) +	abm[iBitmap].cColours *	sizeof(RGB)),
		  &pbafh->bfh.bmp, sizeof(BITMAPINFOHEADER) + abm[iBitmap].cColours * sizeof(RGB));
	   }

       rgbBlack.bRed = rgbBlack.bBlue =	rgbBlack.bGreen	= (BYTE)0x00;

       pbmi->argbColor[iClr] = rgbBlack;

       hbmImage	= GpiCreateBitmap(hPS =	WinGetPS(HWND_DESKTOP),	(PBITMAPINFOHEADER2)pbmi, CBM_INIT, abm[iBitmap].pbImage,
				  (PBITMAPINFO2)pbmi);

       memset(pbmi->argbColor, 0xff, abm[iBitmap].cColours * sizeof(RGB));
       pbmi->argbColor[iClr] = rgbBlack;
       hbmXOR =	GpiCreateBitmap(hPS, (PBITMAPINFOHEADER2)pbmi, CBM_INIT, abm[iBitmap].pbImage, (PBITMAPINFO2)pbmi);

       WinReleasePS(hPS);
       free(pbmi);
       }
   CreateMask(abm[iBitmap].cx, abm[iBitmap].cy);
   GpiDeleteBitmap(hbmXOR);
   }
}
#pragma	subtitle("   View Images - View Pointer Dialogue Procedure")
#pragma	page( )

/* --- ViewDlgProc ------------------------------------- [ Public ] ---	*/
/*									*/
/*     This function is	used to	process	the messages for the dialog	*/
/*     procedure.							*/
/*									*/
/*     Upon Entry:							*/
/*									*/
/*     HWND   hWnd; = Dialog Window Handle				*/
/*     ULONG  msg;  = PM Message					*/
/*     MPARAM mp1;  = Message Parameter	1				*/
/*     MPARAM mp2;  = Message Parameter	2				*/
/*									*/
/*     Upon Exit:							*/
/*									*/
/*     ViewDlgProc = Message Handling Result				*/
/*									*/
/* --------------------------------------------------------------------	*/

MRESULT	EXPENTRY ViewDlgProc(HWND hWnd,	ULONG msg, MPARAM mp1, MPARAM mp2)

{
HPS	hPS;			   /* Presentation Space Handle		*/
MRESULT	mr;			   /* Message Return			*/
RECTL	rcl;			   /* Window Rectangle			*/
POINTL	aptl[4];		   /* Display Points			*/

switch ( msg )
   {
/************************************************************************/
/* Perform dialog initialization					*/
/************************************************************************/

   case	WM_INITDLG :
       fTest = FALSE;
       BuildMaskImage( );
       WinQueryWindowRect(hWnd,	&rcl);
       ptlMask.x = (rcl.xRight - rcl.xLeft) / 2L - abm[iBitmap].cx - 5L;
       ptlMask.y = ptlImage.y =	(rcl.yTop - rcl.yBottom) / 2L -	(abm[iBitmap].cy * 2L +	10L) / 2L;

       ptlImage.x = (rcl.xRight	- rcl.xLeft) / 2L + 5L;
       break;

   case	WM_MOUSEMOVE :

/************************************************************************/
/* Mouse pointer being moved						*/
/************************************************************************/

       if ( fTest )
	   {
		       /* Get the current pointer position		*/

	   aptlImage[0].x = SHORT1FROMMP(mp1);
	   aptlImage[0].y = SHORT2FROMMP(mp1);

		       /* Check	to see if the pointer position is	*/
		       /* different from the location where the	image	*/
		       /* was last drawn.  Also	check to make sure that	*/
		       /* the position is only updated every forth	*/
		       /* pixel.  This results in a smoother motion	*/
		       /* since	the drawing routines are not at	a	*/
		       /* device level.					*/

	   if (	memcmp(&ptlMouse, aptlImage, sizeof(POINTL)) &&	(!(aptlImage[0].x % 4) || !(aptlImage[0].y % 4)) )
	       {
		       /* New position,	restore	the background		*/

	       RestoreBackground( );

		       /* Save the background of the new location	*/

	       ptlBackground = aptlImage[0];
	       SaveBackground(hWnd);

		       /* Calculate the	new position based on desktop	*/
		       /* location					*/

	       ptlMouse	= aptlImage[0];
	       WinMapWindowPoints(hWnd,	HWND_DESKTOP, aptlImage, 1L);

		       /* Initialize the display points	for the	image	*/
		       /* and draw it					*/

	       aptlImage[2].y =	0L;
	       aptlImage[1].x =	aptlImage[0].x + aptlImage[3].x;
	       aptlImage[1].y =	aptlImage[0].y + (aptlImage[3].y = abm[iBitmap].cy);

	       GpiSetBackColor(hpsDesktop, CLR_WHITE);
	       GpiWCBitBlt(hpsDesktop, hbmMask,	 4L, aptlImage,	ROP_SRCAND, BBO_OR);
	       GpiWCBitBlt(hpsDesktop, hbmImage, 4L, aptlImage,	ROP_SRCPAINT, BBO_OR);
	       GpiSetBackColor(hpsDesktop, CLR_BLACK);
	       aptlImage[3].y =	((aptlImage[2].y = abm[iBitmap].cy) << 1) + 1;
	       GpiWCBitBlt(hpsDesktop, hbmMask,	 4L, aptlImage,	ROP_SRCINVERT, BBO_OR);
	       }
	   }
       else
	   return(WinDefDlgProc(hWnd, msg, mp1,	mp2));
       break;

   case	WM_BUTTON1DOWN :
   case	WM_BUTTON2DOWN :
   case	WM_BUTTON3DOWN :

/************************************************************************/
/* Button 1 being depressed						*/
/************************************************************************/

       if ( fTest )
	   {
	   fTest = FALSE;
	   DestroyBackgroundBitmap( );
	   }
       break;

/************************************************************************/
/* Process push	button selections					*/
/************************************************************************/

   case	WM_COMMAND :
       switch (	SHORT1FROMMP(mp1) )
	   {
	   case	DID_TEST :
	       if ( fTest )
		   {
		   fTest = FALSE;
		   DestroyBackgroundBitmap( );
		   }
	       else
		   {
		   fTest = TRUE;
		   CreateBackgroundBitmap( );
		   aptlImage[2].x = 0L;
		   aptlImage[3].x = abm[iBitmap].cx;

		   WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);
		   ptlBackground = ptlMouse;
		   SaveBackground(hWnd);
		   }
	       break;

	   case	DID_OK :
	       if ( fTest )
		   {
		   fTest = FALSE;
		   DestroyBackgroundBitmap( );
		   }
	       GpiDeleteBitmap(hbmImage);
	       GpiDeleteBitmap(hbmMask);
	       WinDismissDlg(hWnd, TRUE);
	       break;
	   }
       break;

   case	WM_PAINT :
       mr = WinDefDlgProc(hWnd,	msg, mp1, mp2);

       aptl[2].x = aptl[2].y = 0L;
       aptl[1].x = (aptl[0].x =	ptlMask.x) + (aptl[3].x	= abm[iBitmap].cx);
       aptl[1].y = (aptl[0].y =	ptlMask.y) + (aptl[3].y	= abm[iBitmap].cy << 1UL);

       GpiSetBackColor(hPS = WinGetPS(hWnd), CLR_WHITE);
       GpiWCBitBlt(hPS,	hbmMask, 4L, aptl, ROP_SRCCOPY,	BBO_IGNORE);

       aptl[1].x = (aptl[0].x =	ptlImage.x) + abm[iBitmap].cx;
       aptl[1].y = aptl[0].y + (aptl[3].y = abm[iBitmap].cy);

       GpiWCBitBlt(hPS,	hbmMask,  4L, aptl, ROP_SRCAND,	BBO_IGNORE);
       GpiWCBitBlt(hPS,	hbmImage, 4L, aptl, ROP_SRCPAINT, BBO_IGNORE);
       GpiSetBackColor(hPS, CLR_BLACK);
       aptl[2].y = abm[iBitmap].cy + 1;
       aptl[3].y = (abm[iBitmap].cy << 1) + 1;
       GpiWCBitBlt(hPS,	hbmMask,  4L, aptl, ROP_SRCINVERT, BBO_IGNORE);

       WinReleasePS(hPS);
       return(mr);

/************************************************************************/
/* Close requested, exit dialogue					*/
/************************************************************************/

   case	WM_CLOSE :
       if ( fTest )
	   {
	   fTest = FALSE;
	   DestroyBackgroundBitmap( );
	   }
       GpiDeleteBitmap(hbmMask);
       GpiDeleteBitmap(hbmImage);
       WinDismissDlg(hWnd, FALSE);
       break;
			/* Pass	through	unhandled messages		*/
   default :
       return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
return(0L);
}
