#ifndef   DRAGNDROP_H
#include "DragNDrop.h"
#endif

/*
 *       Class: RefDragMessage
 * Description: Class to managing BeOS file DragNDrop
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: LGPL
 */

#include <Path.h>
#include <Entry.h>
#include <View.h>

RefDragMessage :: RefDragMessage( const entry_ref * ref )
: BMessage( B_SIMPLE_DATA ) ,
  rect( -1 , -1 , -1 , -1 )
{
	if( ref )
		AddEntryRef( ref ) ;
	
	BMessage :: AddString( "OriginatingApp", "TraX" ) ;
}

void RefDragMessage :: AddEntryRef( const entry_ref * ref )
{
	BMessage::AddRef( "refs", ref ) ;

	BEntry entry( ref ) ;
	BPath path ;
	entry.GetPath( &path ) ;
 	BMessage :: AddData( "text/plain", B_MIME_TYPE, path.Path() ,
					strlen(path.Path())+1 ) ;
}

void RefDragMessage :: SetButtons( int32 bt )
{
	BMessage :: AddInt32( "buttons", bt ) ;
}

void RefDragMessage :: SetRect( const BRect& r )
{
	rect = r ;
}

void RefDragMessage :: AddRect( const BRect& r )
{
	if( ( rect.left < 0 ) || ( r.left < rect.left ) )
		rect.left = r.left ;
	if( ( rect.right < 0 ) || ( r.right > rect.right ) )
		rect.right = r.right ;
	if( ( rect.bottom < 0 ) || ( r.bottom > rect.bottom ) )
		rect.bottom = r.bottom ;
	if( ( rect.top < 0 ) || ( r.top < rect.top ) )
		rect.top = r.top ;
}
			
void RefDragMessage :: Post( BView * v )
{
	rect.PrintToStream() ;
//	rect.OffsetTo( 50, 50 ) ;
	v -> DragMessage( this, rect ) ;
}
