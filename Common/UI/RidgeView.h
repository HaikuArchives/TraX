#ifndef RIDGEVIEW_H
#define RIDGEVIEW_H

/*
 *       Class: RidgeView
 * Description: A simple view like a bar, but slightly different visually
 *     Licence: Public Domain
 */

#include <View.h>

class RidgeView : public BView
{
	public:
		RidgeView( BRect, const char *, orientation = B_VERTICAL ,
		         uint32 = B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM ,
		         uint32 = B_WILL_DRAW ) ;
		
		void Draw( BRect ) ;

	private :
		orientation fOrientation ;
} ;

#endif