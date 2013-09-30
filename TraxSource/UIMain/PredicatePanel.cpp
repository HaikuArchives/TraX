#include "PredicatePanel.h"
#include "Predicates.h"

/*
 *       Class: PredicatePanel
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

#ifndef   STRINGPOPUP_H
#include "StringPopUp.h"
#endif

#ifndef   EDITBOX_H
#include "EditBox.h"
#endif

#ifndef  COLOURS_H
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

#include <Window.h>

#include <float.h>
#include <string.h>

const uint32 kActionChanged = 'ActC' ;

#define ARRAY_SIZE( _x ) (sizeof(_x)/sizeof((_x)[0]))

PredicatePanel :: PredicatePanel( float top )
:	inherited(	BRect( 0,top, 25, 25 ) , "PredicatePanel",
	      	B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW )
,	fpAction( NULL )
,	fpTest( NULL )
,	fpEdit( NULL )
,	fpOptions( NULL )
,	fStr( NULL ) 
{
	SetViewColor( ui_color(B_PANEL_BACKGROUND_COLOR) ) ;
	
	BRect r( 10, 10, 200, 200 ) ;

	fpAction = new StringPopUp( r, "action", NULL, B_FOLLOW_LEFT | B_FOLLOW_V_CENTER ) ;
	fpAction->AddStrings( ARRAY_SIZE(kActionStrings), kActionStrings ) ;

	fpTest = new StringPopUp( r, "test", NULL, B_FOLLOW_LEFT | B_FOLLOW_V_CENTER ) ;
	fpOptions = new StringPopUp( r, "options", NULL, B_FOLLOW_RIGHT | B_FOLLOW_V_CENTER ) ;

	fpEdit = new EditBox( r, "edit", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_V_CENTER, 0 ) ;
}

PredicatePanel :: ~PredicatePanel( void )
{
	delete[] fStr ;
}

void PredicatePanel :: SetupItems( void )
{
	fpTest    -> Empty() ;
	fpOptions -> Empty() ;

	EActionType type  = kActionTypes[ fpAction->GetIndex() ] ;

	fpTest->AddStrings( kNumberTests[type] , kTests[ type ] ) ;

	if( kNumberOptions[ type ] )
	{
		ShowItem( fpOptions ) ;
		fpOptions->AddStrings( kNumberOptions[type] , kOptions[ type ] ) ;
	}
	else
	{
		HideItem( fpOptions ) ;
	}

	switch( type )
	{
		case eTypeString:
		case eTypeSimpleString:
		case eTypeSubString:
		case eTypeDate:
			fpEdit->SetFilter( EditBox::eString ) ;
			ShowItem( fpEdit ) ;
			break ;			

		case eTypeSize:
			fpEdit->SetFilter( EditBox::eFloat ) ;
			ShowItem( fpEdit ) ;
			break ;			

		case eTypeKind:
			HideItem( fpEdit ) ;
			break ;
	}
}

void PredicatePanel :: AttachedToWindow( void )
{
	/* Resize Panel */
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
	fr.bottom = fr.top + 4 * (fh.ascent + fh.descent + fh.leading ) ;
	
	MoveTo( fr.left , fr.top ) ;
	ResizeTo( fr.Width(), fr.Height() ) ;
		
	/* Calculate max String Widths */
	float actionWidth = 0 ;
	for( uint32 i = 0 ; i < ARRAY_SIZE(kActionStrings) ; i++ )
	{
		float sw = StringWidth( kActionStrings[i] ) ;
		if( sw > actionWidth )
			actionWidth = sw ;
	}

	float testWidth = 0 ;
	for( uint32 i = 0 ; i < ARRAY_SIZE(kNumberTests) ; i++ )
	{
		for( uint32 j = 0 ; j < kNumberTests[i] ; j++ )
		{
			float sw = StringWidth( kTests[i][j] ) ;
			if( sw > testWidth )
				testWidth = sw ;
		}
	}

	float optionsWidth = 0 ;
	for( uint32 i = 0 ; i < ARRAY_SIZE(kNumberOptions) ; i++ )
	{
		for( uint32 j = 0 ; j < kNumberOptions[i] ; j++ )
		{
			float sw = StringWidth( kOptions[i][j] ) ;
			if( sw > optionsWidth )
				optionsWidth = sw ;
		}
	}

	BRect r = fr ;
	
	r.top     = fr.Height()/2 - ( fh.ascent + fh.descent + fh.leading ) * 0.85 ;
	r.bottom  = fr.Height()/2 + ( fh.ascent + fh.descent + fh.leading ) * 0.85 ;

	r.left  = 10 ;
	r.right = r.left + actionWidth + 15 ;
	
	fpAction->MoveTo( r.left , r.top ) ;
	fpAction->ResizeTo( r.Width() , r.Height() ) ;

	fpAction->SetViewColor( ViewColor() ) ;
	fpAction->SetLowColor( ViewColor() ) ;

	r.left  = r.right + 5 ;
	r.right = r.left + testWidth + 15 ;

	fpTest->MoveTo( r.left , r.top ) ;
	fpTest->ResizeTo( r.Width() , r.Height() ) ;

	fpTest->SetViewColor( ViewColor() ) ;
	fpTest->SetLowColor( ViewColor() ) ;

	float editLeft = r.right + 10 ;

	r.right = fr.Width() - 10;
	r.left  = r.right - optionsWidth - 15 ;

	float editRight = r.left - 10 ;

	fpOptions->MoveTo( r.left , r.top ) ;
	fpOptions->ResizeTo( r.Width() , r.Height() ) ;

	fpOptions->SetViewColor( ViewColor() ) ;
	fpOptions->SetLowColor( ViewColor() ) ;
	
	r.left    = editLeft ;
	r.right   = editRight ;
	
	fpEdit->MoveTo( r.left , r.top ) ;
	fpEdit->ResizeTo( r.Width() , r.Height() ) ;
		
	AddChild( fpAction ) ;
	AddChild( fpTest ) ;
	AddChild( fpEdit ) ;
	AddChild( fpOptions ) ;

	BMessage * msg = new BMessage( kActionChanged ) ;  
	fpAction->SetTarget( this ) ;
	fpAction->SetMessage( msg ) ;

	SetupItems() ;
}

void PredicatePanel :: Draw( BRect r )
{
	inherited::Draw( r ) ;
	
	r = Bounds() ;
	BeginLineArray( 2 ) ;
	AddLine( r.LeftBottom() , r.RightBottom() , Colours::White ) ;
	AddLine( r.LeftTop()    , r.RightTop()    , Colours::Grey85 ) ;
	EndLineArray( ) ;
}

void PredicatePanel :: ProcessFindAction( void )
{
	int32 iAct = fpAction->GetIndex() ;
	int32 iTst = fpTest->GetIndex() ;
	int32 iOpt = fpOptions->GetIndex() ;
	
	printf( "*\t" ) ;
	if( !fpAction->IsHidden() )
		printf( "%s ", fpAction->GetSelection() ) ;
	if( !fpTest->IsHidden() )
		printf( "%s ", fpTest->GetSelection() ) ;
	if( !fpEdit->IsHidden() )
		printf( "%s ", fpEdit->GetString() ) ;
	if( !fpOptions->IsHidden() )
		printf( "%s ", fpOptions->GetSelection() ) ;

	printf( "\n" ) ;

	switch( iAct )
	{
		case eName:
		{
			fStr = NULL ;

			const char * editStr = fpEdit->GetString() ;
			int sl = strlen( editStr ) ;

			switch( iTst )
			{
				case eStrContains:
				{
					fStr = new char[ sl + 3 ] ;
					fStr[0] = '*' ;
					strcpy( fStr+1, editStr ) ;
					fStr[sl+1] = '*' ;
					fStr[sl+2] = 0 ;
					break ;
				}
				case eStrEquals:
				{
					fStr = strdup(editStr) ;
					break ;
				}
				case eStrBegins:
				{
					fStr = new char[ sl + 2 ] ;
					strcpy( fStr, editStr ) ;
					fStr[sl  ] = '*' ;
					fStr[sl+1] = 0 ;
					break ;
				}
				case eStrEnds:
				{
					fStr = new char[ sl + 2 ] ;
					fStr[0] = '*' ;
					strcpy( fStr+1, editStr ) ;
					break ;
				}
				case eStrRegex:
				{
					fStr = strdup(editStr) ;
					if( kStrCmpValues[iOpt] )
						findlib_insert_regex( fStr ) ;
					else
						findlib_insert_iregex( fStr ) ;
					return ;
				}
			}
			if( kStrCmpValues[iOpt] )
				findlib_insert_name( fStr ) ;
			else
				findlib_insert_iname( fStr ) ;
			
			break ; // iAct
		}
		
		case eExtension:
		{
			const char * editStr = fpEdit->GetString() ;
			int sl = strlen( editStr ) ;

			fStr = new char[ sl + 3 ];
			fStr[0] = '*' ;
			fStr[1] = '.' ;
			
			if( editStr[0] == '.' )
				strcpy( fStr+2, editStr+1 ) ;
			else
				strcpy( fStr+2, editStr ) ;
			
			if( !kSimpleCmpValues[iTst] )
				findlib_insert_negate( ) ;

			if( kStrCmpValues[iOpt] )
				findlib_insert_name( fStr ) ;
			else
				findlib_insert_iname( fStr ) ;
			
			break ; // iAct
		}
		
		case eSize:
		{
			enum comparison_type ctype = kSizeCmpValues[iTst] ;
			uint32 bs = kSizeOptValues[iOpt] ;
			if( strchr(fpEdit->GetString(), '.' ) != NULL )
			{
				float f = fpEdit->GetFloat() ;
				findlib_insert_fsize( ctype, f, bs ) ;
			}
			else
			{
				int32 i = fpEdit->GetInt() ;
				findlib_insert_size( ctype, i, bs ) ;
			}
							
			break ;
		}
		
		case eCreated:
		{
			time_t t = fpEdit->GetDate() ;
			enum comparison_type ctype = kDateCmpValues[iTst] ;
			findlib_insert_crtime( ctype, t ) ;
			break ;
		}
		case eModified:
		{
			time_t t = fpEdit->GetDate() ;
			enum comparison_type ctype = kDateCmpValues[iTst] ;
			findlib_insert_mtime( ctype, t ) ;
			break ;
		}
			
		case eKind:
		{
			uint32 kind = kKindCmpValues[iOpt] ;

			if( !kSimpleCmpValues[iTst] )
				findlib_insert_negate( ) ;
			
			findlib_insert_type( kind ) ;
			break ;
		}
		
		case eContains:
		{
			findlib_insert_open_paren() ;
			findlib_insert_type( S_IFREG ) ;
			findlib_insert_and() ;
			const char * editStr = fpEdit->GetString() ;
			fStr = strdup( editStr ) ;
			
			if( ESubStringCompare(iTst) == eSubString )
			{
				if( kStrCmpValues[iOpt] )
					findlib_insert_contains_str( fStr ) ;
				else
					findlib_insert_contains_istr( fStr ) ;
			}
			else
			{
				if( kStrCmpValues[iOpt] )
					findlib_insert_contains_re( fStr ) ;
				else
					findlib_insert_contains_ire( fStr ) ;
			}
			findlib_insert_close_paren() ;
		}
	}
	
	return ;
}

void PredicatePanel :: MessageReceived( BMessage * msg )
{
	switch ( msg->what )
	{
		case kActionChanged:
		{
			SetupItems() ;
			break ;
		}
		case Messages::StartFind:
		{
			ProcessFindAction() ;
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
