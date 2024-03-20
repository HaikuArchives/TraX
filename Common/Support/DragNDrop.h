#ifndef DRAGNDROP_H
#define DRAGNDROP_H

#include <Rect.h>
#include <Message.h>
#include <View.h>

/*
 *       Class: RefDragMessage
 * Description: Class to managing BeOS file DragNDrop
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: LGPL
 */

class RefDragMessage : public BMessage
{
	public:
		RefDragMessage( const entry_ref * = NULL ) ;
	
		void SetButtons( int32 );
		void SetRect( const BRect& ) ;
		void AddRect( const BRect& ) ;
		void AddEntryRef( const entry_ref *  ) ;
		
		void Post( BView * ) ;
	private:
		BRect rect ;
} ;

#endif
