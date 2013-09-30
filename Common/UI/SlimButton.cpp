#include "SlimButton.h"

/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : SlimButton.cpp
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Class to handle thinner than normal, BButton
 * Licence: AUL
 *******************************************************/

SlimButton :: SlimButton( BRect r, const char * n, const char * t, BMessage * m,
					uint32 rsz , uint32 f )
: BButton( r, n, t, m, rsz, f )
{		
}

void SlimButton :: GetPreferredSize( float * w , float * h )
{	
	inherited::GetPreferredSize( w, h ) ;
	*w -= 16 ;
	*h -= 7 ; 
}

void SlimButton :: MakeFocus( bool b )
{
	inherited::MakeFocus(b) ;
	if( !b )
	{
		Invalidate() ;
	}
} 
