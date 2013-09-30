/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : StringPopUp.h
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Class to provide a popup menu of simple strings
 * Licence: LGPL
 *******************************************************/

#include "StringPopUp.h"

#include <SupportDefs.h>
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <Message.h>
#include <Looper.h>
 
#include <stdio.h>

const uint32 kMSG_StringItem = 'StrI' ;

property_info StringPopUp :: sPropList[] =
{ 
 	{ "Index",
		{ B_GET_PROPERTY, B_SET_PROPERTY, 0 } ,
		{ B_DIRECT_SPECIFIER , 0 },
		"Set/Get the selected item by index" } ,
 	{ "Selection",
		{ B_GET_PROPERTY, B_SET_PROPERTY, 0 } ,
		{ B_DIRECT_SPECIFIER , 0 },
		"Set/Get the selected item by text" } ,
 	{ "String",
		{ B_GET_PROPERTY, 0 } ,
		{ B_INDEX_SPECIFIER , 0 },
		"Get the string for a specified index" } ,
	NULL
} ;

StringPopUp :: StringPopUp( BRect r , const char * name , const char * label, uint32 rsz, uint32 flgs )
:	BMenuField( r, name, label,
		static_cast<BMenu *>( new BPopUpMenu( "string_popup" ) ),
		rsz, flgs )
,	fpTarget( NULL )
,	fpMessage( NULL )
,	fPopUpWidth(0.0)
,	fScripter( "suite/vnd.Zod-StringPopUp", sPropList )
{
	if( label )
		SetDivider( StringWidth( label ) + 1.0 ) ;
	else
		SetDivider( 0.0 ) ;

	SetEnabled( true ) ;

	fScripter.SetOwner( this ) ;

	Scripter::PropertyHandler_f f = Scripter::PropertyHandler_f(&StringPopUp::HandleProperty) ;
	fScripter.SetHandler( "*", f ) ;
}		
	
StringPopUp :: ~StringPopUp ( void )
{
	delete fpMessage ;
}
	
void StringPopUp :: AttachedToWindow( void )
{	
	BView * parent = Parent() ;
	if( parent )
		SetViewColor( parent->ViewColor() ) ;
	
	int32 count = Menu()->CountItems();
	for( int32 i = 0 ; i < count ; i++ )
	{
		Menu()->ItemAt(i)->SetTarget( this ) ;
		if( i == 0 )
			Menu()->ItemAt(i)->SetMarked(true) ;
	}
}

bool StringPopUp ::	SetSelection( const char * str )
{
	for( int32 i=0 ; i< Menu()->CountItems() ; i++ )
	{
		const BMenuItem * mi = Menu()->ItemAt(i) ;
		if( mi == NULL )
			continue ;

		if( strcmp( mi->Label() , str ) != 0 )
			continue ;
		
		return SetIndex( i ) ;
	}
	return false ;
}

bool StringPopUp :: SetIndex( int32 i )
{
	BMenuItem * mi ;
	
	mi = Menu()->FindMarked() ;
	if( mi )
		mi->SetMarked( false ) ;

	mi = Menu()->ItemAt(i) ;
	if( mi )
	{
		mi->SetMarked( true ) ;
		InvokeItem( mi ) ;
		return true ;
	}

	return false ;
}	

const char * StringPopUp :: GetSelection( void ) const
{
	const BMenuItem * mi = Menu()->FindMarked() ;
	if( mi == NULL )
		return NULL ;

	return mi->Label() ;
}

int32 StringPopUp:: GetIndex( void ) const
{
	const BMenuItem * mi = Menu()->FindMarked() ;
	if( mi == NULL )
		return -1 ;
	return Menu()->IndexOf( const_cast<BMenuItem*>(mi) ) ;
}
	
const char * StringPopUp:: GetString( int32 index ) const
{
	const BMenuItem * mi = Menu()->ItemAt(index) ;
	if( mi == NULL )
		return NULL ;

	return mi->Label() ;
}

void StringPopUp :: AddStrings( int32 count , const char * const * strs )
{
	for( int32 i = 0; i<count ; i++ )
	{
		if( strs[i] == NULL )
			AddSeparator() ;
		else
			AddString( strs[i] ) ;
	}
	
	BMenuItem * mi = Menu()->FindMarked() ;
	if( !mi )
	{
		mi = Menu()->ItemAt( (int32)0 ) ;
		if( mi ) mi->SetMarked( true ) ;
	}
}

void StringPopUp :: AddString( const char * str )
{
	BMenuItem * mi = new BMenuItem( str, new BMessage(kMSG_StringItem) ) ;
	Menu()->AddItem( mi ) ;
	mi->SetTarget( this ) ;
}

void StringPopUp :: AddSeparator( void )
{
	Menu()->AddSeparatorItem() ;
}

void StringPopUp :: Empty( void )
{
	BMenu * menu = Menu() ;
	BMenuItem * item ;
	while( (item = menu->RemoveItem( (int32)0 ) ) != NULL )
		delete item ;
}

void StringPopUp :: SetTarget( BHandler * handler )
{
	fpTarget = handler ;
}

void StringPopUp :: SetMessage( BMessage * msg )
{
	delete fpMessage ;
	fpMessage = msg ;
}

void StringPopUp :: SetPopUpWidth( float w )
{
	fPopUpWidth = w ;
}

void StringPopUp :: GetPreferredSize( float * w , float * h )
{
	/** Width **/
	float lw = StringWidth( inherited::Label() ) ;

	float pw ;
	if( fPopUpWidth > 0.0 )
	{
		pw = fPopUpWidth ;
	}
	else
	{
		pw = StringWidth( "12345678" ) ;
	}
		
	*w = lw + 5 + pw ;
	
	/** Height **/
	font_height fh;
	GetFontHeight( &fh ) ;
	*h = fh.ascent + fh.leading + fh.descent + 10 ; 
}

void StringPopUp :: MessageReceived( BMessage * msg )
{
	if( fScripter.HandleMessage( msg ) )
		return ;

	switch( msg->what )
	{
		case kMSG_StringItem:
		{
			BMenuItem * sel = NULL ;
			msg->FindPointer( "source", reinterpret_cast<void**>(&sel) ) ;

			InvokeItem( sel ) ;
			break ;
		}

		default:
			inherited :: MessageReceived( msg ) ;
			break ;
	}	
}		

void StringPopUp :: InvokeItem( BMenuItem * mi )
{
	if( !mi ) return ;

	if( fpTarget && fpMessage )
	{
		BMessage post( *fpMessage ) ;
		post.AddString( "selection" , mi->Label() ) ;
		Looper()->PostMessage( &post, fpTarget, Looper() ) ;
	}
}

void StringPopUp :: SetViewColor( rgb_color col )
{
	inherited::SetViewColor( col ) ;
	SetLowColor( col ) ;
	Invalidate() ;
}

status_t StringPopUp :: GetSupportedSuites( BMessage * msg )
{
	status_t s = fScripter.GetSupportedSuites( msg ) ;

	if( s == B_OK )
		return inherited ::GetSupportedSuites( msg );
	else
		return s ;
}

BHandler * StringPopUp :: ResolveSpecifier(
	BMessage * msg , int32 idx, BMessage * spec, int32 what, const char * prop )
{
	BHandler * h = fScripter.ResolveSpecifier( msg, idx, spec, what, prop ) ;

	if( h )
		return h ;
	else
		return inherited::ResolveSpecifier( msg, idx, spec, what, prop ) ;
}
	
bool StringPopUp :: HandleProperty( const char * prop, BMessage * msg )
{
	if( strcmp( prop, "Index" ) == 0 )
	{
		switch( msg->what )
		{
			case B_SET_PROPERTY:
			{
				int32 i ;
				if( msg->FindInt32( "data", &i ) != B_OK )
				{
					msg->SendReply( B_MESSAGE_NOT_UNDERSTOOD ) ;
				}
				else
				{
					BMessage reply( B_REPLY ) ;
					if( SetIndex( i ) )
						reply.AddInt32( "error", 0 ) ;
					else
					{
					    reply.AddString( "message" , "Index out of range" ) ;
						reply.AddInt32( "error", -1 ) ;
					}
					msg->SendReply( &reply ) ;
				}
				break ;
			}

			case B_GET_PROPERTY:
			{
				int32 i = GetIndex() ;
				BMessage reply( B_REPLY ) ;
				reply.AddInt32( "result", i ) ;
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

	if( strcmp( prop, "Selection" ) == 0 )
	{
		switch( msg->what )
		{
			case B_SET_PROPERTY:
			{
				const char * str ;
				if( msg->FindString( "data", &str ) != B_OK )
				{
					msg->SendReply( B_MESSAGE_NOT_UNDERSTOOD ) ;
				}
				else
				{
					BMessage reply( B_REPLY ) ;
					if( SetSelection( str ) )
						reply.AddInt32( "error", 0 ) ;
					else
					{
					    reply.AddString( "message" , "String not found" ) ;
						reply.AddInt32( "error", -1 ) ;
					}
					msg->SendReply( &reply ) ;
				}
				break ;
			}

			case B_GET_PROPERTY:
			{
				const char * str = GetSelection() ;
				BMessage reply( B_REPLY ) ;
				reply.AddString( "result", str ) ;
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

	if( strcmp( prop, "String" ) == 0 )
	{
		switch( msg->what )
		{
			case B_GET_PROPERTY:
			{
				int32 i ;
				if( msg->FindInt32( "index", &i ) != B_OK )
				{
					msg->SendReply( B_MESSAGE_NOT_UNDERSTOOD ) ;
				}

				const char * str = GetString( i ) ;
				BMessage reply( B_REPLY ) ;
				reply.AddString( "result", str ) ;
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

