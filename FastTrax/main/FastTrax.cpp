#include "FastTraxWindow.h"
#include "Messages.h"
#include "Support.h"
#include "WindowList.h"

#include <Application.h>
#include <Alert.h>
#include <Entry.h>

#include <stdio.h>
#include <string.h>

class FastTraxApp : public BApplication
{
	typedef BApplication inherited ;
	public:
				FastTraxApp( void ) ;

		void	ReadyToRun( void ) ;
		void 	RefsReceived( BMessage * ) ;
		void 	ArgvReceived( int32, char ** ) ;
		void 	MessageReceived( BMessage * ) ;
		bool	QuitRequested( void ) ;
		void	AboutRequested( void ) ;

	private:
		void	RefReceived( entry_ref * ) ;
		void	HandleCommandSwitch( const char * ) ;

		bool		fbQuiting ;
		WindowList	fWindows ;

		enum
		{
			F_Hide = 0x1 ,
		} ;
		uint32		fOptions ;
} ;

FastTraxApp:: FastTraxApp( void )
:	BApplication( "application/x-vnd.ZOD-FastTrax" )
,	fbQuiting( false )
,	fOptions(0)
{
}
		
void FastTraxApp:: ReadyToRun( void )
{
	if( fWindows.CountWindows() == 0 )
	{
		BWindow * win = new FastTraxWindow ;
		fWindows.AddWindow( win ) ;
		if( (fOptions & F_Hide) )
			win->Hide() ;
		win->Show() ;
	}
}
		
void FastTraxApp:: RefsReceived( BMessage * msg )
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

void FastTraxApp :: ArgvReceived( int32 c, char ** v )
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

void FastTraxApp:: HandleCommandSwitch( const char * str )
{
	if( str[1] == '-' )
	{
		if( strcmp( &str[2], "hide" ) == 0 )
			fOptions |= F_Hide ;
		else if( strcmp( &str[2], "show" ) == 0 )
			fOptions &= (~F_Hide) ;
		else
		{
			fprintf( stderr, "Warning: Option \"%s\" not recognised\n", str ) ; 
			return ;
		}
	}
	for( int32 i = 1 ; str[i] ; i++ )
	{
		switch( str[i] )
		{
			case 'h':
				fOptions |= F_Hide ;
				break ;
			case 's':
				fOptions &= (~F_Hide) ;
				break ;
			default:
				fprintf( stderr, "Warning: Option '%c' not recognised\n", str[i] ) ; 
				break ;
		}
	}
}

void FastTraxApp:: RefReceived( entry_ref * ref )
{
	BEntry ent( ref ) ;
	if( ent.IsDirectory() )
	{
		BWindow * win = new FastTraxWindow( ref ) ;
		fWindows.AddWindow( win ) ;
		if( (fOptions & F_Hide) )
			win->Hide() ;
		win->Show() ;
	}
}	

void FastTraxApp:: MessageReceived( BMessage * msg )
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
			
		default:
			BApplication::MessageReceived( msg ) ;
	}
}

bool FastTraxApp:: QuitRequested( void )
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

void FastTraxApp:: AboutRequested( void )
{
	BAlert * alert ;
	alert = new BAlert( "About" ,
"FastTraX v1.1\n"
"\n"
"Copyright (C) 1999-2000\n"
"Tim Vernum <zodsoft@kagi.com>\n"
"\n"
"Distributed under the GPL.\n"
" See Doc/Licence/GPL\n"
,
						"POP!" ) ;
	alert -> Go() ;
}

int main( void )
{
	FastTraxApp app ;
	app.Run() ;
	return 0 ;
}
