#ifndef   TRAXWINDOW_H
#include "TraxWindow.h"
#endif

/*
 *              Trax.cpp
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

#ifndef   MESSAGES_H
#include "Messages.h"
#endif

#ifndef   SUPPORT_H
#include "Support.h"
#endif

#ifndef   WINDOWLIST_H
#include "WindowList.h"
#endif

#ifndef   INIT_H
#include "Init.h"
#endif

#include <Application.h>
#include <Entry.h>
#include <Alert.h>

#include <stdio.h>
#include <string.h>

class TraxApp : public BApplication
{
	typedef BApplication inherited ;
	public:
				TraxApp( void ) ;

		void	ReadyToRun( void ) ;
		void 	RefsReceived( BMessage * ) ;
		void 	ArgvReceived( int32, char ** ) ;
		void 	MessageReceived( BMessage * ) ;
		bool	QuitRequested( void ) ;
		void	AboutRequested( void ) ;

	private:
		void	RefReceived( entry_ref * ) ;
		void	NewWindow( entry_ref * ) ;
		void	HandleCommandSwitch( const char * ) ;

		bool		fbQuiting ;
		WindowList	fWindows ;

		enum
		{
			F_Hide = 0x1 ,
		} ;
		uint32		fOptions ;
} ;

TraxApp:: TraxApp( void )
:	BApplication( "application/x-vnd.ZOD-TraX" )
,	fbQuiting( false )
,	fOptions(0)
{
	InitLibs() ;
}
		
void TraxApp:: ReadyToRun( void )
{
	if( fWindows.CountWindows() == 0 )
	{
		BWindow * win = new TraxWindow ;
		fWindows.AddWindow( win ) ;
		if( (fOptions & F_Hide) )
			win->Hide() ;
		win->Show() ;
	}
}
		
void TraxApp:: RefsReceived( BMessage * msg )
{
	uint32 type; 
 	int32 count; 

 	msg->GetInfo("refs", &type, &count); 
	if ( type != B_REF_TYPE ) 
		return ;
		
 	entry_ref ref; 
	while( count -- )
	{
		if ( msg->FindRef("refs", count, &ref) == B_OK )
			RefReceived( &ref ) ;
	}
}					

void TraxApp :: ArgvReceived( int32 c, char ** v )
{
	if( c <= 1 )
		return ;
	
	int32 i ;
	for( i=1 ; i<c ; i++ )
	{
		if( v[i][0] == '-' )
			HandleCommandSwitch( v[i] ) ;
		else
		{
			entry_ref ref ;
			status_t s ;
			s = get_ref_for_path( v[i], &ref ) ;
			if( s == B_OK )
			{
				RefReceived( &ref ) ;
			}
			else
			{
				char buffer[64] ;
				fprintf( stderr, "%s: %s: %s\n", v[0] , v[i], StringUtils::ErrorString(s, buffer, sizeof(buffer) ) ) ;
			}
		}
	}			
}

void TraxApp:: HandleCommandSwitch( const char * str )
{
	if( str[1] == '-' )
	{
		if( strcmp( str+2, "hide" ) == 0 )
			fOptions |= F_Hide ;
		else
			fprintf( stderr, "Warning: Option \"%s\" not recognised\n", str ) ; 
		return ;
	}
	for( int32 i = 1 ; str[i] ; i++ )
	{
		switch( str[i] )
		{
			case 'h':
				fOptions |= F_Hide ;
				break ;
			default:
				fprintf( stderr, "Warning: Option '%c' not recognised\n", str[i] ) ; 
				break ;
		}
	}
}

void TraxApp:: RefReceived( entry_ref * ref )
{
	BEntry ent( ref ) ;
	if( ent.IsDirectory() )
	{
		NewWindow(ref) ;
	}
}	

void TraxApp:: NewWindow( entry_ref * ref )
{
	BWindow * win = new TraxWindow( ref, fWindows.CountWindows() ) ;
	fWindows.AddWindow( win ) ;
	if( (fOptions & F_Hide) )
		win->Hide() ;
	win->Show() ;
} 

void TraxApp:: MessageReceived( BMessage * msg )
{
	switch( msg->what )
	{
		case Messages::WindowClosed:
		{
			BWindow * win ;
			msg->FindPointer( "Window", reinterpret_cast<void**>(&win) ) ;
			fWindows.RemoveWindow( win ) ;
			if( !fbQuiting && fWindows.CountWindows() == 0 )
				Quit() ;
			msg->SendReply( B_REPLY ) ;
			break ;
		}
		
		case Messages::WindowOpened:
		{
			BWindow * win ;
			msg->FindPointer( "Window", reinterpret_cast<void**>(&win) ) ;
			fWindows.AddWindow( win ) ;
			msg->SendReply( B_REPLY ) ;
			break ;
		}
				
		case Messages::LibInUse:
		case Messages::LibNotInUse:
		{
			fWindows.Broadcast( msg ) ;
			break ;
		}
		
		case Messages::Broadcast:
		{
			BMessage submsg ;
			if( msg->FindMessage( "message", &submsg ) == B_OK )
				fWindows.Broadcast( &submsg ) ;
			break ;
		}
		
		case Messages::NewWindow:
		{
			NewWindow( NULL ) ;
			break ;
		}
		default:
			BApplication::MessageReceived( msg ) ;
	}
}

bool TraxApp:: QuitRequested( void )
{
	fbQuiting = true ;
	if( !inherited::QuitRequested() )
	{
		fbQuiting = false ;
		return false;
	}
	else
		return true ;
}

void TraxApp:: AboutRequested( void )
{
	BAlert * alert ;
	alert = new BAlert( "About" ,
"TraX v1.1\n"
"\n"
"Copyright (C) 1999-2000\n"
"Tim Vernum <zodsoft@kagi.com>\n"
"\n"
"Distributed under the GPL.\n"
" See Doc/Licence/GPL\n"
,
						"_ABG" ) ;
	alert -> Go() ;
}
	
int main( void )
{
	TraxApp app ;
	app.Run() ;
	return 0 ;
}
