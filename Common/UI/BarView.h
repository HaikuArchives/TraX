#ifndef BARVIEW_H
#define BARVIEW_H


/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : BarView.h
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: View that displays a simple separator bar
 * Licence: AUL
 *******************************************************/

#include <View.h>

class BarView : public BView
{
	public:
		BarView( BRect, const char *, orientation = B_VERTICAL ,
		         uint32 = B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM ,
		         uint32 = B_WILL_DRAW ) ;
		
		void Draw( BRect ) ;

	private :
		orientation fOrientation ;
} ;

#endif
