#ifndef STATICSTRING_H
#define STATICSTRING_H

/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : StaticString.h
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: BStringView that will GetPreferredSize() properly
 * Licence: AUL
 *******************************************************/

#include <StringView.h>

class StaticString : public BStringView
{
	typedef BStringView inherited ;
	public:
		StaticString( BRect , const char * n, const char * str,
					  uint32 = B_FOLLOW_LEFT  | B_FOLLOW_RIGHT ,
					  uint32 = B_WILL_DRAW ) ;
		
		void GetPreferredSize( float * ,float * ) ;
		void SetViewColor( rgb_color ) ;
} ;

#endif
