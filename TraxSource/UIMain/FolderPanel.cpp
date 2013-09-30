#include "FolderPanel.h"

/*
 *       Class: FolderPanel
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

#ifndef   TRAXFOLDERPOPUP_H
#include "TraxFolderPopUp.h"
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

#ifndef   FINDLIBTHREAD_H
#include "FindLibThread.h"
#endif

#ifndef   SETTINGS_H
#include "Settings.h"
#endif

#include <Window.h>
#include <Entry.h>
#include <StringView.h>
#include <stdio.h>

const char kSelectString[] = "Find in Folder:" ;

FolderPanel :: FolderPanel( void )
:	Panel(	BRect( 0,0, 25, 25 ) , "FolderPanel",
	      	B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW )
,	fpFolderPU( NULL )
{
	Create( NULL ) ;
}

FolderPanel :: FolderPanel( const entry_ref * ref )
:	Panel(	BRect( 0,0, 25, 25 ) , "FolderPanel",
	      	B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW )
,	fpFolderPU( NULL )
{
	Create( ref ) ;
}

void FolderPanel :: Create( const entry_ref * pRef )
{
	SetViewColor( ui_color(B_PANEL_BACKGROUND_COLOR) ) ;
	
	fpFolderPU = new TraxFolderPopUp( BRect( 0,0,10,10 ) , "PopUp", kSelectString ) ;
	fpFolderPU->InitFolders( pRef ) ;
	if( !pRef )
	{
		entry_ref ref ;
		if( get_ref_for_path( gSettings.fDefaultFolder, &ref ) == B_OK )
			fpFolderPU->SetChoice( &ref ) ;
	}	
	
/*
	fpFolderPU->AddFolder( "/" ) ;

	BVolumeRoster volRoster ;
	BVolume vol ;
	BDirectory dir ;
	BPath path ;
	BEntry ent ;

	while( volRoster.GetNextVolume( &vol ) == B_OK )
	{
		if( vol.IsPersistent() )
		{
			vol.GetRootDirectory( &dir ) ;
			dir.GetEntry( &ent ) ;		
			path.SetTo( &ent ) ;
			fpFolderPU->AddFolder( &path ) ;
		}
	}

	fpFolderPU->AddSeparator( ) ;
	
	if( find_directory( B_USER_DIRECTORY, &path ) == B_OK )
		fpFolderPU->AddFolder( &path ) ;
	if( find_directory( B_DESKTOP_DIRECTORY, &path ) == B_OK )
		fpFolderPU->AddFolder( &path ) ;
	if( find_directory( B_BEOS_DIRECTORY, &path ) == B_OK )
		fpFolderPU->AddFolder( &path ) ;

	entry_ref opt_ref ;
	if( get_ref_for_path( "/boot/optional/", &opt_ref ) == B_OK )
		fpFolderPU->AddFolder( "/boot/optional" ) ;

	fpFolderPU->AddSeparator( ) ;
	fpFolderPU->AddChoice( "other" B_UTF8_ELLIPSIS ) ;
	
	if( ref )
		fpFolderPU->SetChoice( ref ) ;
*/
}

bool FolderPanel :: GetRef( entry_ref * ref )
{
	return ( fpFolderPU && fpFolderPU->GetRef( ref ) ) ;
} 

bool FolderPanel :: GetPath( BPath * path )
{
	entry_ref ref ;

	if( !GetRef( &ref ) )
		return false ;
	
	BEntry ent( &ref ) ;
	path->SetTo( &ent ) ;

	return ( path->InitCheck() == B_OK ) ;
} 

void FolderPanel :: AttachedToWindow( void )
{
	BRect r ;
	if( Parent() )
		r = Parent()->Frame() ;
	else
		r = Window()->Frame() ;
	
	font_height fh ;
	GetFontHeight( &fh ) ;
	
	r.top = 0 ;
	r.right = r.Width() ;
	r.left = 0 ;  
	r.bottom = 3 * (fh.ascent + fh.descent + fh.leading ) ;
	
	MoveTo( 0,0 ) ;
	ResizeTo( r.right, r.bottom ) ;
		
	r.left    = 25 ;
	r.right   = r.right - r.left ;
	r.top     = r.Height()/5 ;
	r.bottom  = r.bottom - r.top ;
	
	fpFolderPU->MoveTo( r.left , r.top ) ;
	fpFolderPU->ResizeTo( r.Width() , r.Height() ) ;

	fpFolderPU->SetViewColor( ViewColor() ) ;
	fpFolderPU->SetLowColor( ViewColor() ) ;

	AddChild( fpFolderPU ) ;
}

void FolderPanel :: Draw( BRect r )
{
	inherited::Draw( r ) ;
	
	r = Bounds() ;
	BeginLineArray( 1 ) ;
	AddLine( r.LeftBottom() , r.RightBottom() , Colours::White ) ;
	EndLineArray( ) ;
}

void FolderPanel :: MessageReceived( BMessage * msg )
{
	switch( msg->what )
	{
		case Messages::StartFind:
		{
			StartFind() ;
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

void FolderPanel :: StartFind( void )
{
	findlib_end_predicates() ;
	FindLibThread * thread = new FindLibThread ;
	GetPath( &thread->Path ) ;
	thread->StartThread() ;
}
