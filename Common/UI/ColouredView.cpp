#ifndef   COLOUREDVIEW_H
#include "ColouredView.h"
#endif

/*
 *       Class: ColouredView
 * Description: View that allows DragNDrop/Scripting of bg Colour
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: AUL
 */

#include <string.h>
#include <stdio.h>

property_info ColouredView :: sPropList[] =
{ 
 	{ "Colour",
		{ B_GET_PROPERTY, B_SET_PROPERTY, 0 } ,
		{ B_DIRECT_SPECIFIER , 0 },
		"" } ,
	NULL
} ;


ColouredView :: ColouredView( BRect r, const char * n,
						uint32 rsz ,
						uint32 flg )
:	BView( r,n,rsz,flg | B_WILL_DRAW)
,	fScripter( "suite/vnd.Zod-ColouredView", sPropList )
{
	fScripter.SetOwner( this ) ;
	fScripter.SetAlias( "Colour", "Color" ) ;
	Scripter::PropertyHandler_f f = Scripter::PropertyHandler_f(&ColouredView::HandleProperty) ;
	fScripter.SetHandler( "*", f ) ;
} 

void ColouredView :: SetViewColor( rgb_color col )
{
	inherited::SetViewColor( col ) ;
	inherited::SetLowColor( col ) ;

	uint32 children = CountChildren() ;
	for( uint32 i=0; i< children ; i++)
	{
		BView * view = ChildAt(i) ;
		if( view )
		{
			view->SetViewColor( col ) ;
			view->SetLowColor( col ) ;
			view->Invalidate() ;
		}
	}
	Invalidate() ;
}
	
void ColouredView :: MessageReceived( BMessage * msg )
{
	if( fScripter.HandleMessage( msg ) )
		return ;

	switch( msg->what )
	{
		// Handle Colour Paste
		case B_PASTE :
		{
			const void * v ;
			ssize_t sz ;
			if( msg->FindData( "RGBColor", B_RGB_COLOR_TYPE, &v, &sz ) == B_OK )
			{
				const rgb_color * pCol ;
				pCol = (const rgb_color *)( v ) ;
				SetViewColor( *pCol ) ;
				break ;
			}
			else
			{
				inherited::MessageReceived( msg ) ;
				break ;
			}
		}
		default:
		{
			inherited::MessageReceived( msg ) ;
		}
	}
}

status_t ColouredView :: GetSupportedSuites( BMessage * msg )
{
	status_t s = fScripter.GetSupportedSuites( msg ) ;

	if( s == B_OK )
		return inherited ::GetSupportedSuites( msg );
	else
		return s ;
}

BHandler * ColouredView :: ResolveSpecifier(
	BMessage * msg , int32 idx, BMessage * spec, int32 what, const char * prop )
{
	BHandler * h = fScripter.ResolveSpecifier( msg, idx, spec, what, prop ) ;

	if( h )
		return h ;
	else
		return inherited::ResolveSpecifier( msg, idx, spec, what, prop ) ;
}
	
bool ColouredView :: HandleProperty( const char * prop, BMessage * msg )
{
	if( strcmp( prop, "Colour" ) == 0 )
	{
		switch( msg->what )
		{
			case B_SET_PROPERTY:
			{
				const void * v ;
				ssize_t sz ;
				if( msg->FindData( "data", B_RGB_COLOR_TYPE, &v, &sz ) != B_OK )
				{
					msg->SendReply( B_MESSAGE_NOT_UNDERSTOOD ) ;
				}
				else
				{
					const rgb_color * pCol ;
					pCol = (const rgb_color *)( v ) ;
					SetViewColor( *pCol ) ;

					BMessage reply( B_REPLY ) ;
					reply.AddInt32( "error", 0 ) ; 
					msg->SendReply( &reply ) ;
				}
				break ;
			}

			case B_GET_PROPERTY:
			{
				rgb_color col = ViewColor() ;
				BMessage reply( B_REPLY ) ;
				reply.AddData( "result", B_RGB_COLOR_TYPE, &col, sizeof(col) ) ;
				msg->SendReply( &reply ) ;
				break ;
			}
			
			default:
			{
				msg->SendReply( B_MESSAGE_NOT_UNDERSTOOD ) ;
				break ;
			}
		}

		return true ;
	}

	return false ;
}
