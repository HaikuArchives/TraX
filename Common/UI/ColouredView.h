#ifndef COLOUREDVIEW_H
#define COLOUREDVIEW_H

/*
 *       Class: ColouredView
 * Description: View that allows DragNDrop/Scripting of bg Colour
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: AUL
 */

#include <View.h>

#include "Scripter.h"

class ColouredView : public BView
{
	typedef BView inherited ;
	public:
		ColouredView(	BRect r, const char * n,
		             	uint32 = B_FOLLOW_LEFT | B_FOLLOW_TOP,
		             	uint32 = B_WILL_DRAW ) ;
		
		void		MessageReceived( BMessage * ) ;
		void		SetViewColor( rgb_color col ) ;
		
		status_t	GetSupportedSuites( BMessage * ) ;
		BHandler *	ResolveSpecifier( BMessage * , int32 , BMessage *,
		          	                  int32 , const char * ) ;

	private:
		bool		HandleProperty( const char *, BMessage * ) ;

		static	property_info	sPropList[] ;
				Scripter		fScripter ;
} ;

#endif
