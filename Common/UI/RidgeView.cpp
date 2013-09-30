#include "RidgeView.h"
#include "Colours.h"

/*
 *       Class: RidgeView
 * Description: A simple view like a bar, but slightly different visually
 *     Licence: Public Domain
 */

RidgeView :: RidgeView( BRect r, const char * n, orientation o,
				 uint32 rs , uint32 f )
:	BView( r, n, rs, f )
,	fOrientation(o)
{
	SetViewColor( Colours::Grey15 ) ;	
}

void RidgeView :: Draw( BRect r )
{
	r = Bounds() ;
	BeginLineArray( 2 ) ;

	AddLine( r.LeftTop() ,
	         ( fOrientation == B_VERTICAL ? r.LeftBottom() : r.RightTop() ) ,
	         Colours::Grey40 ) ;
	AddLine( r.RightBottom() ,
	         ( fOrientation == B_VERTICAL ? r.RightTop() : r.LeftBottom() ) ,
	         Colours::Grey08 ) ;

	EndLineArray() ;
}
