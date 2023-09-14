#include "FastTraxWindow.h"

/*
 *       Class: FastTraxWindow
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

#ifndef   COLOUREDVIEW_H
#include "ColouredView.h"
#endif

#ifndef   BARVIEW_H
#include "BarView.h"
#endif

#ifndef   RIDGEVIEW_H
#include "RidgeView.h"
#endif

#ifndef   STATICSTRING_H
#include "StaticString.h"
#endif

#ifndef   STRINGPOPUP_H
#include "StringPopUp.h"
#endif

#ifndef   FINDLIBTHREAD_H
#include "FindLibThread.h"
#endif

#ifndef   FINDLIB_H
#include "findlib.h"
#endif

#ifndef   TRAXFOLDERPOPUP_H
#include "TraxFolderPopUp.h"
#endif

#ifndef   SETTINGS_H
#include "Settings.h"
#endif

#ifndef   SETTINGSWINDOW_H
#include "SettingsWindow.h"
#endif

#ifndef   SLIMBUTTON_H
#include "SlimButton.h"
#endif

#ifndef   COLOURS_H
#include "Colours.h"
#endif

#ifndef   MESSAGES_H
#include "Messages.h"
#endif

#ifndef   SUPPORT_H
#include "Support.h"
#endif

#include <Application.h>
#include <Entry.h>
#include <Button.h>
#include <TextControl.h>
#include <CheckBox.h>
#include <Beep.h>
#include <Alert.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <parsedate.h>

const char * kKindStr[] = { "Any", "File", "Folder", "Link" } ;
int32 kKindType[] = { 0, S_IFREG, S_IFDIR, S_IFLNK } ;

FastTraxWindow :: FastTraxWindow( entry_ref * ref  )
:	BWindow( BRect( 20, 50, 40, 100 ) , "FastTraX",
		B_MODAL_WINDOW_LOOK , B_NORMAL_WINDOW_FEEL , B_NOT_RESIZABLE )
,	fNameStr(NULL)
,	fContainsStr(NULL)
,	fSettingsWindow(NULL)
{
	// Load the settings
	fSettingsWindow = new SettingsWindow ;
	fSettingsWindow->Hide();
	fSettingsWindow->Show();
	
	BView	* topView ,
	     	* rightView ;
	BRect 	r ;
	
	float	mainWidth = 0;
	float	mainHeight = 0;
	
	topView = new ColouredView( BRect(0,0,50,20) , "top",
	                	B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW ) ;

	rightView = new ColouredView( BRect(50,0,70,100) , "right",
	                	B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW ) ;

	fNameView = new ColouredView( BRect(0,20,50,100) , "name",
	                	B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW ) ;
	fFileView = new ColouredView( BRect(0,20,50,100) , "file",
	                	B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW ) ;
	fDateView = new ColouredView( BRect(0,20,50,100) , "date",
	                	B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW ) ;

	AddChild( topView ) ;
	AddChild( fNameView ) ;
	AddChild( fFileView ) ;
	AddChild( fDateView ) ;
	AddChild( rightView ) ;

	topView->SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) ) ;
	rightView->SetViewColor( ui_color( B_MENU_BACKGROUND_COLOR ) ) ;
	fNameView->SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) ) ;
	fFileView->SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) ) ;
	fDateView->SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) ) ;
	
	//=================================================================
	// Right View
	//=================================================================
	BButton * button ;
	button = new BButton( BRect( 7,5,20,15 ) , "exit", "Close",  
		new BMessage( B_QUIT_REQUESTED )  ) ;
	rightView->AddChild( button ) ;
	button->ResizeToPreferred() ;
	r = button->Frame() ;
	r.OffsetTo( r.left, r.bottom + 7 ) ;

	button = new BButton( r , "about", "About",  
		new BMessage( B_ABOUT_REQUESTED ) ) ;
	rightView->AddChild( button ) ;
	button->ResizeToPreferred() ;
	r = button->Frame() ;
	r.OffsetTo( r.left, r.bottom + 7 ) ;

	button = new BButton( r , "settings", "Settings",  
		new BMessage( Messages::SettingsRequested ) ) ;
	rightView->AddChild( button ) ;
	button->ResizeToPreferred() ;
	r = button->Frame() ;
	r.OffsetTo( r.left, r.bottom + 7 ) ;
	
	rightView->ResizeTo( r.right + 7 , r.bottom + 7 ) ;
	fFindButton = new BButton( r , "find", "Find",  
		new BMessage( Messages::StartFind ) , B_FOLLOW_LEFT | B_FOLLOW_BOTTOM ) ;
	rightView->AddChild( fFindButton ) ;
	fFindButton->ResizeToPreferred() ;
	fFindButton->MakeDefault( true ) ;

	//=================================================================
	// Top View
	//=================================================================
	fNameButton = new SlimButton( BRect( 7, 2, 25, 13 ) , "name", "Name",
		new BMessage( Messages::SetTab | 'N' ) ) ;
	topView->AddChild( fNameButton ) ;
	fNameButton->ResizeToPreferred() ;
	r = fNameButton->Frame() ;	
	r.OffsetTo( r.right + 7 , r.top ) ;

	fFileButton = new SlimButton( r , "file", "File",
		new BMessage( Messages::SetTab | 'F' ) ) ;
	topView->AddChild( fFileButton ) ;
	fFileButton->ResizeToPreferred() ;
	r = fFileButton->Frame() ;
	r.OffsetTo( r.right + 7 , r.top ) ;
	
	fDateButton = new SlimButton( r , "date", "Date",
		new BMessage( Messages::SetTab | 'D' ) ) ;
	topView->AddChild( fDateButton ) ;
	fDateButton->ResizeToPreferred() ;
	r = fDateButton->Frame() ;
	
	topView->ResizeTo( r.right + 7, r.bottom + 3 ) ;
	
	float width , top ;

	//=================================================================
	// Name Main View
	//=================================================================
	fpFolder = new TraxFolderPopUp( BRect( 5, 7, 15, 10 ), "location", "Find in:" ) ;
	fpFolder->InitFolders( ref ) ;
	fNameView->AddChild( fpFolder ) ;
	fpFolder->ResizeToPreferred() ;
	fpFolder->SetViewColor( fNameView->ViewColor() ) ;
	fpFolder->SetLowColor( fNameView->ViewColor() ) ;
	fpFolder->SetChoice( gSettings.fDefaultFolder ) ;
	r = fpFolder->Frame() ;
	width = r.right ;
	
	top = r.bottom + 10 ;
	fpName = new BTextControl( BRect( 5, top, r.right * 2, top + 10 ) ,
				"name", "Named:", "", NULL ) ;
	fNameView->AddChild( fpName ) ;
	fpName->ResizeToPreferred() ;
	fpName->SetDivider( fpName->StringWidth( "Named: " ) ) ;
	fpName->MakeFocus( true ) ;
	r = fpName->Frame() ;
	width = MathUtils::max( width, r.right ) ;

	top = r.bottom + 10 ;
	fpSubDirs = new BCheckBox( BRect( 5, top, 15, top + 10 ) ,
				"subdirs", "Include Sub-Directories", NULL ) ;
	fNameView->AddChild( fpSubDirs ) ;
	fpSubDirs->ResizeToPreferred() ;
	fpSubDirs->SetValue(1) ;
	r = fpSubDirs->Frame() ;
	width = MathUtils::max( width, r.right ) ;
	fNameView->ResizeTo( width + 10, r.bottom + 10 ) ;
	r = fNameView->Frame() ;
	
	mainWidth  = MathUtils::max( mainWidth, r.right ) ;
	mainHeight = MathUtils::max( mainHeight, r.bottom ) ;
	
	//=================================================================
	// File Main View
	//=================================================================
	fpContaining = new BTextControl( BRect( 3, 3, 10, 10 ) ,
				"containing", "Containing Text:", "", NULL ) ;
	fFileView->AddChild( fpContaining ) ;
	width = fpContaining->StringWidth( "Containing Text:" ) ;
	width *= 2.5 ;
	fpContaining->ResizeTo( width , 10 ) ;
	fpContaining->ResizeToPreferred() ;
	r = fpContaining->Frame() ;
	
	top = r.bottom + 7 ;
	StaticString * sv = new StaticString( BRect( 3, 3, 50, 10 ) , "kb",
							"kB" ) ;
	sv -> ResizeToPreferred() ;
	r = sv->Frame() ;
	width -= (r.Width() + 3) ;

	fpSizeGT = new BTextControl(
				BRect( 3, top, width - 3, top + 10 ) ,
				"size", "Size at least: ", "", NULL ) ;
	fFileView->AddChild( fpSizeGT ) ;
	fpSizeGT->ResizeToPreferred() ;
	fpSizeGT->SetDivider( fpContaining->Divider() ) ;
	r = fpSizeGT->Frame() ;
	fpSizeGT->ResizeTo( width - 6, r.Height() ) ;
	r = fpSizeGT->Frame() ;

	top = r.bottom - sv->Frame().Height() ;
	sv->MoveTo( width , top ) ;
	fFileView->AddChild( sv ) ;
	
	top = r.bottom + 4 ;
	sv = new StaticString( BRect( 3, 3, 50, 10 ) , "kb",
							"kB" ) ;
	sv -> ResizeToPreferred() ;
	r = sv->Frame() ;

	fpSizeLT = new BTextControl(
				BRect( 3, top, width - 3, top + 10 ) ,
				"size", "Size less than: ", "", NULL ) ;
	fFileView->AddChild( fpSizeLT ) ;
	fpSizeLT->ResizeToPreferred() ;
	fpSizeLT->SetDivider( fpContaining->Divider() ) ;
	r = fpSizeLT->Frame() ;
	fpSizeLT->ResizeTo( width - 6, r.Height() ) ;
	r = fpSizeLT->Frame() ;

	top = r.bottom - sv->Frame().Height() ;
	sv->MoveTo( width , top ) ;
	fFileView->AddChild( sv ) ;

	width = sv->Frame().right ;
	top = r.bottom + 7 ;
	fpKind = new StringPopUp( BRect( 3, top, width, top + 10 ) ,
				"kind", "Kind:  ") ;
	fpKind->AddStrings( sizeof(kKindStr)/sizeof(kKindStr[0]), kKindStr) ; 
	fFileView->AddChild( fpKind ) ;
	fpKind->ResizeToPreferred() ;
	r = fpKind->Frame() ;

	width = MathUtils::max( r.right, fpContaining->Frame().right ) ;
	fFileView->ResizeTo( width + 10, r.bottom + 10 ) ;

	r = fFileView->Frame() ;
	mainWidth  = MathUtils::max( mainWidth, r.right ) ;
	mainHeight = MathUtils::max( mainHeight, r.bottom ) ;
	
	//=================================================================
	// Date Main View
	//=================================================================
	sv = new StaticString( BRect( 3, 3, 50, 10 ) , "mod_string",
							"Modified " ) ;
	sv -> ResizeToPreferred() ;
	fDateView->AddChild( sv ) ;
	sv->SetViewColor( fDateView->ViewColor() ) ;
	sv->SetLowColor( fDateView->ViewColor() ) ;
	r = sv->Frame() ;
	width = r.right ;
	
	fpModAfter  = new BTextControl( BRect( width, 3, r.right * 2, 10 ) ,
				"mod_after", "after:", "", NULL ) ;
	fDateView->AddChild( fpModAfter ) ;
	fpModAfter->ResizeToPreferred() ;
	r = fpModAfter->Frame() ;

	fpModBefore = new BTextControl( BRect( width, r.bottom + 3, r.right * 2, r.bottom + 10 ) ,
				"mod_before", "before:", "", NULL ) ;
	fDateView->AddChild( fpModBefore ) ;
	fpModBefore->ResizeToPreferred() ;

	width = fpModBefore->StringWidth( "before: " ) ;
	fpModAfter ->ResizeTo( width * 4, r.Height() ) ;
	fpModBefore->ResizeTo( width * 4, r.Height() ) ;

	fpModAfter ->SetDivider( width ) ;
	fpModBefore->SetDivider( width ) ;

	r = fpModAfter->Frame() ;
	sv->MoveTo( 3, r.bottom - sv->Frame().Height()/3 ) ;

	r = fpModBefore->Frame() ;
	top = r.bottom + 5 ;
	r = sv->Frame() ;
	width = r.right ;
	sv = new StaticString( BRect( 3, top, 50, top + 5 ) , "cre_string",
							"Created " ) ;
	sv -> ResizeToPreferred() ;
	fDateView->AddChild( sv ) ;
	sv->SetViewColor( fDateView->ViewColor() ) ;
	sv->SetLowColor( fDateView->ViewColor() ) ;
	
	fpCreAfter  = new BTextControl( BRect( width, top, r.right * 2, top + 5 ) ,
				"cre_after", "after:", "", NULL ) ;
	fDateView->AddChild( fpCreAfter ) ;
	fpCreAfter->ResizeToPreferred() ;
	r = fpCreAfter->Frame() ;

	fpCreBefore = new BTextControl( BRect( width, r.bottom + 3, r.right * 2, r.bottom + 10 ) ,
				"cre_before", "before:", "", NULL ) ;
	fDateView->AddChild( fpCreBefore ) ;
	fpCreBefore->ResizeToPreferred() ;

	width = fpCreBefore->StringWidth( "before: " ) ;
	fpCreAfter ->ResizeTo( width * 4, r.Height() ) ;
	fpCreBefore->ResizeTo( width * 4, r.Height() ) ;

	fpCreAfter ->SetDivider( width ) ;
	fpCreBefore->SetDivider( width ) ;

	r = fpCreAfter->Frame() ;
	sv->MoveTo( 3, r.bottom - sv->Frame().Height()/3 ) ;

	r = fpCreBefore->Frame() ;
	fDateView->ResizeTo( r.right + 10, r.bottom + 10 ) ;

	r = fDateView->Frame() ;
	mainWidth  = MathUtils::max( mainWidth, r.right ) ;
	mainHeight = MathUtils::max( mainHeight, r.bottom ) ;

	//=================================================================
	// Fit Together
	//=================================================================
	float middle = MathUtils::max( topView->Frame().right , mainWidth ) ;
	float bottom = MathUtils::max( rightView->Frame().bottom , mainHeight ) ;

	width = middle - 10 ;
	fpFolder->ResizeTo( width, fpFolder->Frame().Height() ) ;	
	fpName->ResizeTo(   width, fpName  ->Frame().Height() ) ;	
	fNameView->ResizeTo( width + 10, r.bottom + 10 ) ;

	float height = bottom - topView->Frame().bottom ;

	topView->ResizeTo( middle, topView->Frame().Height() ) ;	
	rightView->MoveTo( middle + 5, 0 ) ;
	rightView->ResizeTo( rightView->Frame().Width(), bottom ) ;
	fNameView->MoveTo( 0, topView->Frame().bottom + 3) ;
	fNameView->ResizeTo( middle, height ) ;
	fFileView->MoveTo( 0, topView->Frame().bottom + 3) ;
	fFileView->ResizeTo( middle, height ) ;
	fDateView->MoveTo( 0, topView->Frame().bottom + 3) ;
	fDateView->ResizeTo( middle, height ) ;

	//=================================================================
	// Resize Window
	//=================================================================
	ResizeTo( rightView->Frame().right , rightView->Frame().bottom ) ;
	BView * bar ;
	bar = new BarView( BRect( middle,0,middle+4,bottom), "bar1" ) ;
	AddChild( bar ) ;
	bar = new RidgeView( BRect( 0, topView->Frame().bottom, middle-1, topView->Frame().bottom + 2 ), "bar2", B_HORIZONTAL ) ;
	AddChild( bar ) ;
	
	fCurrentView = fNameView ;
	fDisabledButton = fNameButton ;
	fNameButton->SetEnabled( false ) ;
	fFileView->Hide() ;
	fDateView->Hide() ;
}

FastTraxWindow::~FastTraxWindow( void )
{
	delete[] fNameStr ;
	delete[] fContainsStr ;
}

bool FastTraxWindow::QuitRequested( void )
{
	// Response to Close Message, not to Application Quiting
	if( CurrentMessage() )
	{		
		BMessenger msgr( be_app ) ;
		BMessage reply ;
		if( msgr.IsValid() )
		{
			BMessage msg( Messages::WindowClosed ) ;
			msg.AddPointer( "Window", this ) ;
			msgr.SendMessage( &msg , &reply ) ;
		}
	}
	return  BWindow::QuitRequested( ) ;
}

// -1 Kill
//  0 Ignore
//  1 Ok.
signed char FastTraxWindow :: GetTime( const char * str, time_t now, time_t *t  )
{
	*t = parsedate( str, now ) ;
	if( *t != -1 )
		return 1;
	
	char buffer[512] ;
	sprintf( buffer, "Cannot parse date \"%s\"", str ) ;
	BAlert * alert = new BAlert(
		"Error" , buffer, "Ignore Error" , "Cancel Find",
		NULL, B_WIDTH_FROM_WIDEST, B_STOP_ALERT ) ;
	int32 button = alert->Go() ;
	if( button != 0 )
	{
		FindLibThread::UninitFind() ;
		return -1 ;
	}
	return 0 ;	
}


void FastTraxWindow :: MessageReceived( BMessage * msg )
{
	switch( msg->what )
	{
		case B_ABOUT_REQUESTED:
		{
			BMessenger msgr( be_app ) ;
			if( msgr.IsValid() )
				msgr.SendMessage( msg ) ;
			break ;
		}
		case ( Messages::SetTab | 'N' ) :
		{
			fCurrentView->Hide() ;
			fCurrentView = fNameView ;
			fCurrentView->Show() ;

			fDisabledButton->SetEnabled( true ) ;
			fDisabledButton = fNameButton ;
			fDisabledButton->SetEnabled( false ) ;
			break ;
		}
		case ( Messages::SetTab | 'F' ) :
		{
			fCurrentView->Hide() ;
			fCurrentView = fFileView ;
			fCurrentView->Show() ;

			fDisabledButton->SetEnabled( true ) ;
			fDisabledButton = fFileButton ;
			fDisabledButton->SetEnabled( false ) ;
			break ;
		}
		case ( Messages::SetTab | 'D' ) :
		{
			fCurrentView->Hide() ;
			fCurrentView = fDateView ;
			fCurrentView->Show() ;

			fDisabledButton->SetEnabled( true ) ;
			fDisabledButton = fDateButton ;
			fDisabledButton->SetEnabled( false ) ;
			break ;
		}
		case Messages::LibInUse:
		{
			fFindButton->SetEnabled( false ) ;
			break ;
		}
		case Messages::LibNotInUse:
		{
			fFindButton->SetEnabled( true ) ;
			delete[] fNameStr ;
			delete[] fContainsStr ;

			fNameStr = NULL ;
			fContainsStr = NULL ;
			break ;
		}
		case Messages::StartFind :
		{
			if( !FindLibThread::InitFind( "FastTraX" ) )
			{
				beep() ;
				break ;
			}
			
			findlib_start_predicates() ;
			const char * str ;

			time_t now = time( NULL ) ;
			
			str = fpCreAfter->Text() ;
			if( str[0] )
			{
				time_t t ;
				signed char ok = GetTime( str, now, &t ) ;
				if( ok < 0 )
					break ;
				if( ok > 0 )
					findlib_insert_crtime( COMP_GT, t ) ;
			}
			str = fpCreBefore->Text() ;
			if( str[0] )
			{
				time_t t ;
				signed char ok = GetTime( str, now, &t ) ;
				if( ok < 0 )
					break ;
				if( ok > 0 )
				{
					findlib_insert_crtime( COMP_LT, t ) ;
				}
			}

			str = fpModAfter->Text() ;
			if( str[0] )
			{
				time_t t ;
				signed char ok = GetTime( str, now, &t ) ;
				if( ok < 0 )
					break ;
				if( ok > 0 )
				{
					findlib_insert_mtime( COMP_GT, t ) ;
				}
			}

			str = fpModBefore->Text() ;
			if( str[0] )
			{
				time_t t ;
				signed char ok = GetTime( str, now, &t ) ;
				if( ok < 0 )
					break ;
				if( ok > 0 )
				{
					findlib_insert_mtime( COMP_LT, t ) ;
				}
			}


			fNameStr = strdup( fpName->Text() ) ;
			if( fNameStr[0] )
				findlib_insert_iname( fNameStr ) ;
			
			if( fpSubDirs->Value() == 0 )
				findlib_insert_maxdepth( 1Ul ) ;
	
			fContainsStr = strdup( fpContaining->Text() ) ;
			if( fContainsStr[0] )
			{
				findlib_insert_open_paren() ;
				findlib_insert_type( S_IFREG ) ;
				findlib_insert_and() ;
				findlib_insert_contains_istr( fContainsStr ) ;
				findlib_insert_close_paren() ;
			}

			str = fpSizeLT->Text() ;
			if( str[0] )
			{
				float f = atof(str) ;
				findlib_insert_fsize( COMP_LT, f, 1024 ) ;
			}

			str = fpSizeGT->Text() ;
			if( str[0] )
			{
				float f = atof(str) ;
				findlib_insert_fsize( COMP_GT, f, 1024 ) ;
			}
			
			int32 i = fpKind->GetIndex() ;
			if( i )
				findlib_insert_type( kKindType[i] ) ;

			findlib_end_predicates() ;
			FindLibThread * thread = new FindLibThread ;
			
			entry_ref ref ;
			fpFolder->GetRef( &ref ) ;
			BEntry ent( &ref ) ;
			thread->Path.SetTo( &ent ) ;
			thread->StartThread() ;

			break ;
		}
		case Messages::SettingsRequested:
		{
			fSettingsWindow->Lock() ;
			if( fSettingsWindow->IsHidden() )
				fSettingsWindow->Show();
			fSettingsWindow->Unlock() ;
			fSettingsWindow->Activate( true ) ;
			break ;
		}		
		default:
		{
			BWindow::MessageReceived( msg ) ;
		}
	}
}
