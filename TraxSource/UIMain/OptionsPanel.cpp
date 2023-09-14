#include "OptionsPanel.h"

/*
 *       Class: OptionsPanel
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

#ifndef   EDITBOX_H
#include "EditBox.h"
#endif

#ifndef   COLOURS_H
#include "Colours.h"
#endif

#ifndef   MESSAGES_H
#include "Messages.h"
#endif

#ifndef   FINDLIB_H
#include "findlib.h"
#endif

#ifndef   UIDEFS_H
#include "UIDefs.h"
#endif

#ifndef   BARBERPOLE_H
#include "BarberPole.h"
#endif

#include <Window.h>
#include <CheckBox.h>
#include <Button.h>
#include <Font.h>

const char kMaxDepthString[] = "Maximum subfolder depth :" ;

const rgb_color & WaitBarDisabledHighColor = Colours::Grey15 ;
const rgb_color & WaitBarDisabledLowColor  = Colours::Grey25 ;

const rgb_color & WaitBarEnabledHighColor = Colours::SlateBlue ;
const rgb_color & WaitBarEnabledLowColor  = Colours::Fuchsia ;

OptionsPanel :: OptionsPanel( float top )
:	Panel(	BRect( 0,top, 25, 25 ) , "OptionsPanel",
	      	B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW )
,	fpMaxDepthEdit( NULL )
,	fpMaxDepthCheck( NULL )
{
}

OptionsPanel :: ~OptionsPanel( void )
{
}

void OptionsPanel :: AttachedToWindow( void )
{
	SetViewColor( ui_color(B_PANEL_BACKGROUND_COLOR) ) ;
	
	BRect fr ;
	if( Parent() )
		fr = Parent()->Frame() ;
	else
		fr = Window()->Frame() ;
	
	fr.top = Frame().top ;
	fr.right = fr.Width() ;
	fr.left = 0 ;  
	fr.bottom = fr.top + 5 ; // Set Later
			
	MoveTo( fr.left, fr.top ) ;
	ResizeTo( fr.Width(), fr.Height() ) ;

	font_height fh ;
	GetFontHeight( &fh ) ;
	
	BRect r ;
	
	r.top = 5 ;
	r.left = fr.Width() * 2/3 ;

	r.right   = r.left + ( r.Height() * 2 ) + 1  ; 
	r.bottom  = r.top + 10 ;
	
	BButton * addButton = new
					BButton( r, "Add", "+",
							  new BMessage( Messages::AddPanel ) ,
							  B_FOLLOW_RIGHT | B_FOLLOW_TOP ) ;
	AddChild( addButton ) ;

	float h , w ;
	addButton->GetPreferredSize( &h, &w ) ;
	if( h > w )
		h = w ;
	else
		w = h ;
	addButton->ResizeTo( w, h ) ;
	
	r.left += ( w + 4 ) ;
	r.right += ( w + 4 ) ; 
	BButton * rmButton = new BButton( r, "Remove", "-",
							  new BMessage( Messages::RemovePanel ) ,
							  B_FOLLOW_RIGHT | B_FOLLOW_TOP ) ;
	AddChild( rmButton ) ;
	rmButton->ResizeTo( w, h ) ;

	r.left = 10 ;
	r.top = addButton->Frame().bottom + 2 ;

	r.right   = r.left + 10 ; 
	r.bottom  = r.top + 10 ;
	
	fpMaxDepthCheck = new BCheckBox( r, "depth_check", kMaxDepthString ,
							NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP ) ;
	AddChild( fpMaxDepthCheck ) ;
	fpMaxDepthCheck->ResizeToPreferred() ;

	r = fpMaxDepthCheck->Frame() ;
 	float centre = r.top + r.Height()/2 ;

	r.left   = r.right + 5 ;
	r.top    = centre - (fh.ascent + fh.descent + fh.leading) * 3/4 ;
	r.bottom = centre + (fh.ascent + fh.descent + fh.leading) * 3/4 ;
	r.right  = r.left + StringWidth( "xx37xx" ) ;

	fpMaxDepthEdit = new EditBox( r, "depth_edit", B_FOLLOW_LEFT | B_FOLLOW_TOP ) ;
	AddChild( fpMaxDepthEdit ) ;

	r.top    = r.bottom + 7 ;
	r.bottom = r.top + 20 ;
	r.left   = 10 ;
	r.right  = r.left + 50 ;
	
	BButton * settingsButton = new BButton( r, "settings", "Settings" B_UTF8_ELLIPSIS,
								new BMessage( Messages::Settings ) ,
								B_FOLLOW_LEFT | B_FOLLOW_TOP ) ;
	AddChild( settingsButton ) ;
	settingsButton->ResizeToPreferred() ;
	settingsButton->SetTarget( Window() ) ;
		
	r.right = fr.Width() - 25 ;
	r.left  = r.right - 50 ;

	fpFindButton = new BButton( r, "go", "Find",
							  new BMessage( Messages::StartFind ) ,
							  B_FOLLOW_RIGHT | B_FOLLOW_TOP ) ;
	AddChild( fpFindButton ) ;
	fpFindButton->ResizeToPreferred() ;
	r = fpFindButton->Frame() ;
	fpFindButton->MoveTo( fr.Width() - 20 - r.Width(), r.top ) ;
	fpFindButton->SetTarget( Window() ) ;
	fpFindButton->MakeDefault(true) ;

	r = fpFindButton->Frame() ;
	r.bottom = r.top - 3 ;
	r.top = r.bottom - 8 ;
	fpPoleFrame = new ColouredView( r, "pole_frame" ) ;
	fpPoleFrame->SetViewColor( Colours::Grey60 ) ;
	AddChild( fpPoleFrame ) ;

	r.OffsetTo( 1 , 1 ) ;
	fpBarberPole = new BarberPole( r, "pole", B_FOLLOW_RIGHT | B_FOLLOW_TOP ) ;
	fpBarberPole->SetHighColor( WaitBarDisabledHighColor  ) ;
	fpBarberPole->SetLowColor ( WaitBarDisabledLowColor ) ;
	fpPoleFrame->AddChild( fpBarberPole ) ;
	fpPoleFrame->Hide() ;

	r = fpFindButton->Frame() ;

	fr.bottom = fr.top + r.bottom + 8 ;
	ResizeTo( fr.Width(), fr.Height() ) ;

	fr = addButton->Frame() ;
	addButton->MoveTo( r.left + 10, fr.top ) ;
	rmButton ->MoveTo( r.left + 17 + fr.Width(), fr.top ) ;
}

void OptionsPanel :: Draw( BRect r )
{
	inherited::Draw( r ) ;
	
	r = Bounds() ;
	BeginLineArray( 1 ) ;
	AddLine( r.LeftTop()    , r.RightTop()    , Colours::Grey85 ) ;
	EndLineArray( ) ;
}

void OptionsPanel :: MessageReceived( BMessage * msg )
{
	switch( msg->what )
	{
		case UI::Messages::ChildHasFocus:
		{
			void * view = NULL ;
			msg->FindPointer( "parent", &view ) ;
			bool b = msg->FindBool( "focus" ) ;
			if( b && view == fpMaxDepthEdit )
			{
				fpMaxDepthCheck->SetValue( B_CONTROL_ON ) ;
			}
			break ;
		}
		case Messages::StartFind:
		{
			StartFind() ;
			break ;
		}		
		case Messages::LibInUse:
		{
			fpFindButton->SetEnabled( false ) ;
			fpPoleFrame->Show() ;
			fpBarberPole->Start() ;
			break ;
		}
		case Messages::LibNotInUse:
		{
			fpFindButton->SetEnabled( true ) ;
			fpBarberPole->Stop() ;
			// Set the colours back to grey, in case this was the primary window
			fpBarberPole->SetHighColor( WaitBarDisabledHighColor ) ;
			fpBarberPole->SetLowColor ( WaitBarDisabledLowColor  ) ;
			fpPoleFrame->Hide() ;
			break ;
		}
		case Messages::SettingsChanged :
		{
			break ;
		}
		default:
		{
			inherited::MessageReceived( msg ) ;
			break ;
		}
	}
}

void OptionsPanel :: StartFind( void )
{
	fpBarberPole->SetHighColor( WaitBarEnabledHighColor  ) ;
	fpBarberPole->SetLowColor ( WaitBarEnabledLowColor ) ;

	findlib_start_predicates() ;
	if( fpMaxDepthCheck->Value() )
	{
		int32 i = fpMaxDepthEdit->GetInt() ;
		printf("MaxDepth: %" B_PRId32 "\n", i ) ;

		/* Add 1. Because maxdepth 0 => folder only
		 * We want 0 => folder contents only
		 *         1 => search 1 subfolder depth
		 */
		findlib_insert_maxdepth( static_cast<uint32>(i+1) ) ;
	}
}
