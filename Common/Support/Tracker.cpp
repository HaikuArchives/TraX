
/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : Tracker.cpp
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Functions for interfacing with Tracker
 * Licence: AUL
 *******************************************************/

#include "Tracker.h"

#include <Messenger.h>
#include <Message.h>
#include <Entry.h>

#include <stdio.h>

bool GetTrackerWindow( const entry_ref * ref, BMessenger * msgr )
{
	int32 ridx = 1 ;

	BMessenger tracker( "application/x-vnd.Be-TRAK" ) ;
	if ( !tracker.IsValid() )
	        return false ;
	
	for( ; ; ridx++  )
	{
		BMessage msg( B_GET_PROPERTY ) ;
	
		msg.AddSpecifier( "Messenger" ) ;
		BMessage specifier( B_REVERSE_INDEX_SPECIFIER ) ;
	 	specifier.AddInt32( "index", ridx ) ;
		specifier.AddString( "property", "Window" ) ;
	 	msg.AddSpecifier( &specifier ) ;

		BMessage reply ;

		if( tracker.SendMessage( &msg, &reply ) != B_OK )
		{
			fprintf( stderr, "Error sending to tracker\n" ) ; 
			continue ;
		}
				
		int32 err = -1 ;
		if( reply.FindInt32( "error", &err ) != B_OK )
		{
			fprintf( stderr, "No error field in reply\n" ) ;
			continue ;
		}
				
		// Out of range
		if( err == -2147483645 )
		{
			fprintf( stderr, "Out of range\n" ) ;
			return false ;
		}

		if( err != 0 )
		{
			reply.PrintToStream() ;
			continue ;
		}
		
		if( reply.FindMessenger( "result", msgr ) != B_OK )
		{
			fprintf( stderr, "No result in reply\n" ) ;
			continue ;
		}
		
		if( !msgr->IsValid() )
		{
			fprintf( stderr, "Messenger not valid\n" ) ;
			continue ;
		}

		BMessage msg2( B_GET_PROPERTY ) ;
		msg2.AddSpecifier( "Path" ) ;
		msg2.AddSpecifier( "Poses" ) ;
		
		if( msgr->SendMessage( &msg2, &reply ) != B_OK )
		{
			fprintf( stderr, "Can't Send message (2)\n" ) ;
			continue ;
		}
				
		if( reply.FindInt32( "error", &err ) == B_OK )
		{
			if( err != 0 )
			{
				reply.PrintToStream() ;
				continue ;
			}
		}

		entry_ref windowRef ;
		if( reply.FindRef( "result", &windowRef ) != B_OK )
		{
			fprintf( stderr, "No result in reply\n" ) ;
			continue ;
		}

		if( windowRef == *ref )
			return true ;
	}
	return false ;
}	
