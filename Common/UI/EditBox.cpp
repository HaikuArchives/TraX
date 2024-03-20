#include "EditBox.h"

/*
 *       Class: EditBox
 * Description: A simple TextControl view
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: LGPL
 */

#ifndef   COLOURS_H
#include "Colours.h"
#endif

#ifndef   UIDEFS_H
#include "UIDefs.h"
#endif

#include <Looper.h>
#include <Window.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <parsedate.h>
#include <ctype.h>

const float SubViewInsetH = 2 ;
const float SubViewInsetV = 2 ;

class EditTextView : public BTextView
{
	typedef BTextView inherited ;
	public:
		EditTextView( BRect r )
		 :	BTextView( r, "textview", BRect( 1,1,1000,0 ) ,
		               B_FOLLOW_ALL_SIDES ,
		               B_WILL_DRAW | B_NAVIGABLE )
		{
		}
		
		void MakeFocus( bool b )
		{
			inherited::MakeFocus(b) ;
			
			BMessage msg( UI::Messages::ChildHasFocus ) ;
			msg.AddPointer( "source", this ) ;
			msg.AddBool( "focus", b ) ;
			Looper()->PostMessage( &msg, Parent() ) ;
		}
} ;

property_info EditBox :: sPropList[] =
{ 
 	{ "Value",
		{ B_GET_PROPERTY, B_SET_PROPERTY, 0 } ,
		{ B_DIRECT_SPECIFIER , 0 },
		"Set/Get the text in the Edit Box" } ,
 	{ "Focus",
		{ B_GET_PROPERTY, B_SET_PROPERTY, 0 } ,
		{ B_DIRECT_SPECIFIER , 0 },
		"Set/Get the focus to the Edit Box" } ,
	NULL
} ;

EditBox :: EditBox( BRect fr, const char * n, uint32 rsz , uint32 flg )
:	BView( fr, n, rsz, flg | B_WILL_DRAW | B_FRAME_EVENTS )
,	fpTextView( NULL ) 
,	fScripter( "suite/vnd.Zod-EditBox", sPropList )
{
	Create( NULL ) ;
}

EditBox :: 	EditBox( BRect fr , const char * n , BTextView * tv,
					 uint32 rsz, uint32 flg )
:	BView( fr, n, rsz, flg | B_WILL_DRAW | B_FRAME_EVENTS)
,	fpTextView( tv )
,	fScripter( "suite/vnd.Zod-EditBox", sPropList )
{
	Create( tv ) ;
}		

void EditBox :: Create( BTextView * tv )
{
	fScripter.SetOwner( this ) ;
	fScripter.SetAlias( "Value", "Text" ) ;

	Scripter::PropertyHandler_f f = Scripter::PropertyHandler_f(&EditBox::HandleProperty) ;
	fScripter.SetHandler( "*", f ) ;

	BRect r = Frame() ;
	r.OffsetTo( 0,0 ) ;
	r.InsetBy( SubViewInsetH , SubViewInsetV ) ;
	
	if( !tv )
	{
		tv = new EditTextView( r ) ;
	}
	else
	{
		tv->MoveTo( r.left, r.top ) ;
		tv->ResizeTo( r.Width() , r.Height() ) ;
		tv->SetResizingMode( B_FOLLOW_ALL_SIDES ) ;
	}
	tv->SetWordWrap( false ) ;
	fpTextView = tv ;
	AddChild( fpTextView ) ;
}

void EditBox :: SetString( const char * str )
{
	fpTextView->SetText( str ) ;
}

void EditBox :: SetFloat( float f )
{
	SetFloat( f, "%f" ) ;
}

void EditBox :: SetFloat( float f , const char * fmt )
{
	char str[64] ;
	sprintf( str, fmt, f ) ;
	fpTextView->SetText( str ) ;
}

void  EditBox :: SetInt( int32 i )
{
	char str[64] ;
	sprintf( str, "%" B_PRId32, i ) ;
	fpTextView->SetText( str ) ;
}

void  EditBox :: SetIntOct( int32 i )
{
	char str[64] ;
	sprintf( str, "%" B_PRIo32, (uint32) i ) ;
	fpTextView->SetText( str ) ;
}

void  EditBox :: SetIntHex( int32 i )
{
	char str[64] ;
	sprintf( str, "%" B_PRIx32, (uint32) i ) ;
	fpTextView->SetText( str ) ;
}

void  EditBox :: SetIntHEX( int32 i )
{
	char str[64] ;
	sprintf( str, "%" B_PRIX32, (uint32) i ) ;
	fpTextView->SetText( str ) ;
}

void  EditBox :: SetDate( time_t tt )
{
	char str[128] ;

	struct tm * t = localtime( &tt ) ;
	strftime( str , sizeof(str) , "%d %b %Y, %H:%M" , t ) ;
	fpTextView->SetText( str ) ;
}

const char *  EditBox :: GetString( void ) const
{
	return fpTextView->Text() ;
}

float EditBox :: GetFloat( void ) const
{
	float f = atof( fpTextView->Text() ) ;
//	printf( "EditBox (Float): %f\n", f ) ;
	return f ;
}

int32 EditBox :: GetInt( void ) const
{
	int32 i = atoi( fpTextView->Text() ) ;
//	printf( "EditBox (Int): %ld\n", i ) ;
	return i ;
}

int32 EditBox :: GetIntC( void ) const
{
	char * e ;
	return strtol( fpTextView->Text() , &e, 0 ) ;
}

int32 EditBox :: GetIntInBase( int radix ) const
{
	char * e ;
	return strtol( fpTextView->Text() , &e , radix ) ;
}

time_t EditBox :: GetDate( void ) const
{
	time_t t = time( NULL ) ;
	return parsedate( fpTextView->Text(), t ) ;
}

void EditBox :: SetFilter( EditFilter filter )
{
	uint16 i ;
	for( i = 0 ; i < 256 ; i++ )
	{
		char c = char(i) ;
		 
		if( !isprint(c) || c == B_TAB )
		{
			fpTextView->DisallowChar( c ) ;
			continue ;
		}
		
		if( filter == eString )
		{
			fpTextView->AllowChar( c ) ;
			continue ;
		}
		
		if( filter == eIntOct && (c == '8' || c == '9') )
		{
			fpTextView->DisallowChar(c) ;
			continue ;
		}

		if( isdigit(c) )
		{
			fpTextView->AllowChar( c ) ;
			continue ;
		}
		
		if( filter == eInteger || filter == eIntOct )
		{
			fpTextView->DisallowChar(c) ;
			continue ;
		}
		
		if( filter == eFloat )
		{
			if( c == '.' ) fpTextView->AllowChar( c ) ;
			else fpTextView->DisallowChar(c) ;
			continue ;
		}
		
		// IntC or IntHex
		if( isalpha(c) && tolower(c) >= 'a' && tolower(c) <= 'f' )
		{
			fpTextView->AllowChar( c ) ;
			continue ;
		}
			
		if( c == 'x' &&	filter == eIntC )
		{
			fpTextView->AllowChar( c ) ;
			continue ;
		}
		
		fpTextView->DisallowChar(c) ;
		continue ;
	}
}

void EditBox :: DrawFrame( bool focus )
{
//	printf( "EB: DrawFrame\n" ) ;
	BRect r = Frame() ;
	r.OffsetTo( 0, 0 ) ;

	BeginLineArray( 8 ) ;
	AddLine( r.LeftBottom()  , r.RightBottom() , Colours::White ) ;
	AddLine( r.RightBottom() , r.RightTop()    , Colours::White ) ;
	AddLine( r.RightTop()    , r.LeftTop()     , Colours::Grey25 ) ;
	AddLine( r.LeftTop()     , r.LeftBottom()  , Colours::Grey25 ) ;

	r.InsetBy( 1,1 ) ;	

	if( focus )
	{
		rgb_color nav = ui_color(B_KEYBOARD_NAVIGATION_COLOR) ;
		AddLine( r.LeftBottom()  , r.RightBottom() , nav ) ;
		AddLine( r.RightBottom() , r.RightTop()    , nav ) ;
		AddLine( r.RightTop()    , r.LeftTop()     , nav ) ;
		AddLine( r.LeftTop()     , r.LeftBottom()  , nav ) ;
	}
	else
	{
		AddLine( r.LeftBottom()  , r.RightBottom() , Colours::Grey15 ) ;
		AddLine( r.RightBottom() , r.RightTop()    , Colours::Grey15 ) ;
		AddLine( r.RightTop()    , r.LeftTop()     , Colours::Grey60 ) ;
		AddLine( r.LeftTop()     , r.LeftBottom()  , Colours::Grey60 ) ;
	}

	EndLineArray() ;
}

void EditBox :: Draw( BRect r )
{
	inherited::Draw(r) ;
	DrawFrame( fpTextView->IsFocus() && Window()->IsActive()) ;
}

void EditBox:: MakeFocus( bool b )
{
	inherited::MakeFocus(b) ;
	DrawFrame(b) ;

	BMessage msg( UI::Messages::ChildHasFocus ) ;
	msg.AddBool( "focus", b ) ;
	msg.AddPointer( "source", this ) ;

	BHandler * h = Parent() ;
	if( !h ) h = Looper() ;	
	Looper()->PostMessage( &msg, h ) ;
}

void EditBox::WindowActivated( bool b )
{
	inherited::WindowActivated(b) ;
	DrawFrame( b && fpTextView->IsFocus() ) ;
}

void EditBox::FrameResized( float w, float h )
{
	inherited::FrameResized( w,h ) ;
	ResetTextViewRect(w,h) ;
}

void EditBox::AttachedToWindow( void )
{
	inherited::AttachedToWindow() ;
	ResetTextViewRect( Frame().Width(), Frame().Height() ) ;
}	

void EditBox::ResetTextViewRect( float w, float h )
{
	font_height fh ;
	GetFontHeight( &fh ) ;

	h -= (2*SubViewInsetV);
	w -= (2*SubViewInsetH) ;

	float th = fh.ascent + fh.descent + 2 * fh.leading ;
	BRect tr( 1.5, h/2 - th/2 , 1000 , 0 ) ;
	fpTextView->SetTextRect( tr ) ;
	fpTextView->ResizeTo( w , h ) ;
	Invalidate() ;
}

void EditBox :: MessageReceived( BMessage * msg )
{
	if( fScripter.HandleMessage( msg ) )
		return ;

	if( msg->what == UI::Messages::ChildHasFocus )
	{
		DrawFrame( fpTextView->IsFocus() ) ;
		msg->AddPointer( "parent", this ) ;
	}
	// Fall Thru
	inherited::MessageReceived( msg ) ;
}

// Scripting
status_t EditBox :: GetSupportedSuites( BMessage * msg )
{
	status_t s = fScripter.GetSupportedSuites( msg ) ;

	if( s == B_OK )
		return inherited ::GetSupportedSuites( msg );
	else
		return s ;
}

BHandler * EditBox :: ResolveSpecifier(
	BMessage * msg , int32 idx, BMessage * spec, int32 what, const char * prop )
{
	BHandler * h = fScripter.ResolveSpecifier( msg, idx, spec, what, prop ) ;

	if( h )
		return h ;
	else
		return inherited::ResolveSpecifier( msg, idx, spec, what, prop ) ;
}
	
bool EditBox :: HandleProperty( const char * prop, BMessage * msg )
{
	if( strcmp( prop, "Value" ) == 0 )
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
					SetString( str ) ;
					reply.AddInt32( "error", 0 ) ;
					msg->SendReply( &reply ) ;
				}
				break ;
			}

			case B_GET_PROPERTY:
			{
				const char * str = GetString() ;
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

	if( strcmp( prop, "Focus" ) == 0 )
	{
		switch( msg->what )
		{
			case B_SET_PROPERTY:
			{
				bool b ;
				if( msg->FindBool( "data", &b ) != B_OK )
				{
					msg->SendReply( B_MESSAGE_NOT_UNDERSTOOD ) ;
				}
				else
				{
					BMessage reply( B_REPLY ) ;
					fpTextView->MakeFocus( b ) ;
					reply.AddInt32( "error", 0 ) ;
					msg->SendReply( &reply ) ;
				}
				break ;
			}

			case B_GET_PROPERTY:
			{
				bool b = fpTextView->IsFocus() ;
				BMessage reply( B_REPLY ) ;
				reply.AddBool( "result", b ) ;
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

