#ifndef SLIMBUTTON_H
#define SLIMBUTTON_H

#include <Button.h>

/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : SlimButton.h
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Class to handle thinner than normal, BButton
 * Licence: AUL
 *******************************************************/

class SlimButton : public BButton
{
	typedef BButton inherited ; 
	public:
		SlimButton( BRect r, const char * n, const char * t, BMessage *,
					uint32 = B_FOLLOW_LEFT | B_FOLLOW_TOP ,
					uint32 = B_WILL_DRAW | B_NAVIGABLE ) ;
		
		void GetPreferredSize( float * , float * ) ;
		void MakeFocus( bool ) ;		
} ;

#endif
