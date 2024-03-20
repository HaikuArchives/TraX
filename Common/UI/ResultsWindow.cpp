
/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : ResultsWindow.cpp
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Window that displays results of a Trax search
 * Licence: GPL
 *******************************************************/

#include "ResultsWindow.h"

#ifndef   RESULTSITEM_H
#include "ResultsItem.h"
#endif

#ifndef   WINMSG_H
#include "WinMsg.h"
#endif

#ifndef   TRACKER_H
#include "Tracker.h"
#endif

#ifndef   COLOURS_H
#include "Colours.h"
#endif

#ifndef   DRAGNDROP_H
#include "DragNDrop.h"
#endif

#include <ListView.h>
#include <ScrollBar.h>
#include <ScrollView.h>
#include <Application.h>
#include <Path.h>
#include <Entry.h>

#include <stdio.h>
#include <string.h>

namespace Messages
{
	const uint32 ItemInvoked = 'IInv' ;
}

class ResultsList : public BListView
{
	public:
		ResultsList( BRect ) ;
		bool InitiateDrag( BPoint, int32, bool ) ;
} ;

///////////////////// WINDOW

ResultsWindow * ResultsWindow::spCurrentWindow ;

ResultsWindow :: ResultsWindow( void )
:	BWindow( BRect( 150, 100, 400, 350 ) , "Results",
		B_DOCUMENT_WINDOW , 0 )
{
	BRect r = Bounds() ;
	r.top = 15 ;
	r.right -= B_V_SCROLL_BAR_WIDTH ;
	
	fpList = new ResultsList( r ) ;

	BScrollView * sv = new BScrollView( "scroll", fpList, B_FOLLOW_ALL_SIDES,
									0, false, true ) ;
	sv->SetViewColor( Colours::Grey15 ) ;
	AddChild( sv ) ;
	
	r.top = 0 ;
	r.bottom = 14 ;
	r.right += B_V_SCROLL_BAR_WIDTH ;
	
	BView * v = new BView( r, "panel",
		B_FOLLOW_TOP | B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW ) ;
	v->SetViewColor( Colours::Grey15 ) ;
	AddChild( v ) ;
	
	spCurrentWindow = this ;
	fpList->SetInvocationMessage( new BMessage(Messages::ItemInvoked) ) ;
}

ResultsWindow :: ~ResultsWindow( void )
{
	BListItem * bli ;
	while ((bli = fpList->RemoveItem(static_cast<int32>(0L))) != NULL)
		delete bli ;
}

int ResultsWindow :: AddResult( const char * file )
{
	if( !spCurrentWindow ) return 0 ;
	
	spCurrentWindow->AddFile( file ) ;
	return 1 ;
}

void ResultsWindow :: AddFile( const char * file )
{
	BListItem * bli = new ResultsItem( file ) ;
	if( Lock() )
	{
		fpList->AddItem( bli ) ;
		Unlock() ;
	}
}
				
bool ResultsWindow :: QuitRequested( void )
{
	if( spCurrentWindow == this )
	{
		Lock() ;
		spCurrentWindow = NULL ;
		Unlock() ;
	}
	
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
	
	return BWindow::QuitRequested() ;
}

void ResultsWindow :: MessageReceived( BMessage * msg )
{
	switch( msg->what )
	{
		case Messages::ItemInvoked :
		{
			int32 i ;
			int32 c = 0 ;
			while( msg->FindInt32( "index", c, &i ) == B_OK )
			{
				ItemInvoked( i ) ;
				c++ ;
			}
			break ;
		}

		default:
		{
			BWindow::MessageReceived( msg ) ;
			break ;
		}
	}
}

void ResultsWindow :: ItemInvoked( int32 i )
{
	uint32 mod = modifiers() ;

	ResultsItem * ri = reinterpret_cast< ResultsItem * >( fpList->ItemAt(i) ) ;
	if( !ri )
		return ;

	bool display ;
	if( mod & B_COMMAND_KEY )
		display = !(BEntry( ri->Ref() ).IsDirectory()) ;
	else if( mod & B_CONTROL_KEY )
		display = true ;
	else
		display = false ;
	
	if( display )
		DisplayFile( ri ) ;
	else
		LaunchFile( ri ) ;
}

void ResultsWindow :: DisplayFile( ResultsItem * ri )
{
	BMessenger tracker( "application/x-vnd.Be-TRAK" ) ;
	if ( !tracker.IsValid() )
	        return ;
	
	BEntry ent( ri->Ref() ) ;
	BEntry parent ;
	ent.GetParent( &parent ) ;

	entry_ref ref ;
	parent.GetRef( &ref ) ;

	BMessage send( B_REFS_RECEIVED ) ;
	send.AddRef( "refs", &ref );
	BMessage reply ;
	if( tracker.SendMessage( &send, &reply) == B_OK )
	{
		// Wait for Tracker to get a chance to respond to the message
		snooze( 250 * 1000 ) ;
		
		/*
		 * Find the window for the ref. We need to look at the ref
		 *  bcs Tracker doesn't distinguish between multiple directories
		 *  with the same leaf name
		 */
		BMessenger window ;
		if( GetTrackerWindow( &ref, &window ) )
		{
			// If we have the window, change the selection in it
			BMessage selector( B_SET_PROPERTY ) ;
			selector.AddRef( "data", ri->Ref() ) ;
			selector.AddSpecifier( "Selection" ) ;
			selector.AddSpecifier( "Poses" ) ;
			window.SendMessage( &selector, &reply ) ;
		}
	}
}

void ResultsWindow :: LaunchFile( ResultsItem * ri )
{
	BMessenger tracker( "application/x-vnd.Be-TRAK" ) ;
	if ( !tracker.IsValid() )
	        return ;
	
	BMessage send( B_REFS_RECEIVED ) ;
	send.AddRef( "refs", ri->Ref() );
	tracker.SendMessage(&send) ;
}

///////////////////// LIST

ResultsList :: ResultsList( BRect frame )
:	BListView( frame, "list", B_MULTIPLE_SELECTION_LIST , B_FOLLOW_ALL_SIDES )
{
}

bool ResultsList :: InitiateDrag(BPoint, int32, bool wasSelected )
{
	if( !wasSelected )
		return false ;
	
	RefDragMessage dragger ;
	BMessage * message = Window()->CurrentMessage() ; 

	int32 buttons = 0; 
 	message->FindInt32("buttons", &buttons);
	dragger.SetButtons( buttons ) ;	

	int32 sel = -1 ;
	int32 i ;
	for (	i = 0 ;
			( sel = CurrentSelection(i) ) != -1 ;
			i++ )
	{
		ResultsItem * item = dynamic_cast<ResultsItem*>( ItemAt(sel) ) ;
		if( item )
		{
			dragger.AddRect( ItemFrame( sel ) ) ;
			dragger.AddEntryRef( item->Ref() ) ;
		}
	}
	if( i )
		dragger.Post( this ) ;
	
	return true ;
}
