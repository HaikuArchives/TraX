#include "JoinPanel.h"

/*
 *       Class: JoinPanel
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

#ifndef   STRINGPOPUP_H
#include "StringPopUp.h"
#endif

#ifndef   COLOURS_H
#include "Colours.h"
#endif

#ifndef   FINDLIB_H
#include "findlib.h"
#endif

#ifndef   HIDEITEM_H
#include "HideItem.h"
#endif

#ifndef   MESSAGES_H
#include "Messages.h"
#endif

#ifndef   JOINS_H
#include "Joins.h"
#endif

#ifndef   SETTINGS_H
#include "Settings.h"
#endif

#include <Window.h>
#include <stdio.h>

JoinPanel :: JoinPanel( float top )
:	inherited(	BRect( 0,top, 25, 25 ) , "JoinPanel",
	      	B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW )
,	fpNot( NULL ) 
,	fpCloseBracket( NULL )
,	fpConjunction( NULL )
,	fpOpenBracket( NULL )
,	fPosition( ePosBottom )
{
	SetViewColor( ui_color(B_PANEL_BACKGROUND_COLOR) ) ;
	
	BRect r( 10, 10, 20, 20 ) ;

	fpCloseBracket = new StringPopUp( r, "close", NULL, B_FOLLOW_LEFT | B_FOLLOW_V_CENTER ) ;
	fpCloseBracket->AddStrings( 2, kCloseStrings ) ;

	fpConjunction = new StringPopUp( r, "conjunction", NULL, B_FOLLOW_H_CENTER | B_FOLLOW_V_CENTER ) ;
	fpConjunction->AddStrings( 2, kConjunctionStrings ) ;

	fpNot = new StringPopUp( r, "not", NULL, B_FOLLOW_H_CENTER | B_FOLLOW_V_CENTER ) ;
	fpNot->AddStrings( 2, kNotStrings ) ;

	fpOpenBracket = new StringPopUp( r, "open", NULL, B_FOLLOW_RIGHT | B_FOLLOW_V_CENTER ) ;
	fpOpenBracket->AddStrings( 2, kOpenStrings ) ;
}

void JoinPanel :: SetPosition( EPosition pos )
{
	fPosition = pos ;
	ShowHidePopUps( ) ;
}
	
void JoinPanel :: ShowHidePopUps( void )
{
	switch( fPosition )
	{
		case ePosTop:
		{
			HideItem( fpCloseBracket ) ;
			HideItem( fpConjunction ) ;

			if( gSettings.fShow & TraxSettings::eShowNot )
				ShowItem( fpNot ) ;
			else
				HideItem( fpNot ) ;

			if( gSettings.fShow & TraxSettings::eShowBrackets )
				ShowItem( fpOpenBracket ) ;
			else
				HideItem( fpOpenBracket ) ;

			break ;
		}
		case ePosMiddle:
		{
			if( gSettings.fShow & TraxSettings::eShowBrackets )
			{
				ShowItem( fpCloseBracket ) ;
				ShowItem( fpOpenBracket ) ;
			}
			else
			{
				HideItem( fpCloseBracket ) ;
				HideItem( fpOpenBracket ) ;
			}

			ShowItem( fpConjunction ) ;

			if( gSettings.fShow & TraxSettings::eShowNot )
				ShowItem( fpNot ) ;
			else
				HideItem( fpNot ) ;

			break ;
		}
		default:
		case ePosBottom:
		{
			if( gSettings.fShow & TraxSettings::eShowBrackets )
				ShowItem( fpCloseBracket ) ;
			else
				HideItem( fpCloseBracket ) ;				

			HideItem( fpConjunction ) ;
			HideItem( fpNot ) ;
			HideItem( fpOpenBracket ) ;
			break ;
		}
	}
}

void JoinPanel :: AttachedToWindow( void )
{
	BRect fr ;
	if( Parent() )
		fr = Parent()->Frame() ;
	else
		fr = Window()->Frame() ;
	
	font_height fh ;
	GetFontHeight( &fh ) ;
	
	fr.top = Frame().top ;
	fr.right = fr.Width() ;
	fr.left = 0 ;  
	fr.bottom = fr.top + 3 * (fh.ascent + fh.descent + fh.leading ) ;
	
	MoveTo( fr.left , fr.top ) ;
	ResizeTo( fr.Width(), fr.Height() ) ;
		
	BRect r = fr ;
	
	r.top     = r.Height() * 1/5;
	r.bottom  = r.Height() * 3/5 ;

	r.left    = 25 ;
	r.right   = r.left + 25 ;
	
	fpCloseBracket->MoveTo( r.left , r.top ) ;
	fpCloseBracket->ResizeTo( r.Width() , r.Height() ) ;

	fpCloseBracket->SetViewColor( ViewColor() ) ;
	fpCloseBracket->SetLowColor( ViewColor() ) ;

	r.left    = fr.Width() - StringWidth( kOpenStrings[1] ) - 45 ;
	r.right   = r.left + 25 ;

	fpOpenBracket->MoveTo( r.left , r.top ) ;
	fpOpenBracket->ResizeTo( r.Width() , r.Height() ) ;

	fpOpenBracket->SetViewColor( ViewColor() ) ;
	fpOpenBracket->SetLowColor( ViewColor() ) ;

	r.left    = 2 * fr.Width()/5 - StringWidth( kConjunctionStrings[0] ) * 2/3 ;
	r.right   = r.left + 25 ;
	
	fpConjunction->MoveTo( r.left , r.top ) ;
	fpConjunction->ResizeTo( r.Width() , r.Height() ) ;

	fpConjunction->SetViewColor( ViewColor() ) ;
	fpConjunction->SetLowColor( ViewColor() ) ;
	
	r.left    = 3 * fr.Width()/5 - StringWidth( kNotStrings[1] ) * 2/3;
	r.right   = r.left + 25 ;
	
	fpNot->MoveTo( r.left , r.top ) ;
	fpNot->ResizeTo( r.Width() , r.Height() ) ;

	fpNot->SetViewColor( ViewColor() ) ;
	fpNot->SetLowColor( ViewColor() ) ;

	AddChild( fpCloseBracket ) ;
	AddChild( fpConjunction ) ;
	AddChild( fpNot ) ;
	AddChild( fpOpenBracket ) ;

	ImplementSettings() ;
	ShowHidePopUps() ;
	fpConjunction->SetIndex( gSettings.fConjunction ) ;
}

void JoinPanel :: Draw( BRect r )
{
	inherited::Draw( r ) ;
	
	r = Bounds() ;
	BeginLineArray( 2 ) ;
	AddLine( r.LeftBottom() , r.RightBottom() , Colours::White ) ;
	AddLine( r.LeftTop()    , r.RightTop()    , Colours::Grey85 ) ;
	EndLineArray( ) ;
}

void JoinPanel :: MessageReceived( BMessage * msg )
{
	switch( msg->what )
	{
		case Messages::StartFind:
		{
			ProcessFindAction() ;
			break ;
		}
		case Messages::SetPosition:
		{
			int32 i ;
			if( msg->FindInt32( "position", &i ) == B_OK )
			{
				SetPosition( static_cast<EPosition>(i) ) ;
			}
			break ;
		}
		case Messages::SettingsChanged :
		{
			ImplementSettings() ;
			break ;
		}
		default:
		{
			inherited::MessageReceived( msg ) ;
			break ;
		}
	}
}

void JoinPanel :: ProcessFindAction( void )
{
	bool b ;
	int32 i ;
	
	printf( "*\t" ) ;

	if( !fpCloseBracket->IsHidden() )
	{
		printf( "%s ", fpCloseBracket->GetSelection() ) ;
		
		i = fpCloseBracket->GetIndex() ;
		if( i >= 0 && i <= 1 )
		{
			b = kCloseValues[i] ;
			if( b )
				findlib_insert_close_paren( ) ;
		}		
	}
	
	if( fPosition == ePosMiddle ) // !fpConjunction->IsHidden() 
	{
		printf( "%s ", fpConjunction->GetSelection() ) ;

		i = fpConjunction->GetIndex() ;
		if( i >= 0 && i <= 1 )
		{
			b = kConjunctionValues[i] ;
			if( b )
				findlib_insert_and( ) ;
			else
				findlib_insert_or( ) ;
		}		
	}
			
	if( !fpNot->IsHidden() )
	{
		printf( "%s ", fpNot->GetSelection() ) ;

		i = fpNot->GetIndex() ;
		if( i >= 0 && i <= 1 )
		{
			b = kNotValues[i] ;
			if( b )
				findlib_insert_negate( ) ;
		}		
	}
	
	if ( !fpOpenBracket->IsHidden() )
	{
		printf( "%s ", fpOpenBracket->GetSelection() ) ;

		i = fpOpenBracket->GetIndex() ;
		if( i >= 0 && i <= 1 )
		{
			b = kOpenValues[i] ;
			if( b )
				findlib_insert_open_paren( ) ;
		}		
	}
	
	printf( "\n" ) ;
	return ;
}

void JoinPanel:: ImplementSettings( void )
{
	if( !(gSettings.fShow & TraxSettings::eShowJoinPanel ) )
	{
		if( !this->IsHidden() )
			this->Hide() ;
	}
	else
	{
		if( this->IsHidden() )
			this->Show() ;
		
		ShowHidePopUps() ;
	}
}
