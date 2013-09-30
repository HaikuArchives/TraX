
/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : BarView.cpp
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: View that displays a simple separator bar
 * Licence: AUL
 *******************************************************/

#include "BarView.h"

#ifndef   COLOURS_H
#include "Colours.h"
#endif

BarView :: BarView( BRect r, const char * n, orientation o,
				 uint32 rs , uint32 f )
:	BView( r, n, rs, f )
,	fOrientation(o)
{
	SetViewColor( Colours::Grey15 ) ;	
}

void BarView :: Draw( BRect r )
{
	r = Bounds() ;
	BeginLineArray( 4 ) ;

	AddLine( r.LeftTop() ,
	         ( fOrientation == B_VERTICAL ? r.LeftBottom() : r.RightTop() ) ,
	         Colours::Grey40 ) ;
	AddLine( r.RightBottom() ,
	         ( fOrientation == B_VERTICAL ? r.RightTop() : r.LeftBottom() ) ,
	         Colours::Grey40 ) ;

	if( fOrientation == B_VERTICAL )
		r.InsetBy(1,0) ;
	else
		r.InsetBy(0,1) ;
	
	AddLine( r.LeftTop() ,
	         ( fOrientation == B_VERTICAL ? r.LeftBottom() : r.RightTop() ) ,
	         Colours::White ) ;
	AddLine( r.RightBottom() ,
	         ( fOrientation == B_VERTICAL ? r.RightTop() : r.LeftBottom() ) ,
	         Colours::Grey25 ) ;
	
	EndLineArray() ;
}
