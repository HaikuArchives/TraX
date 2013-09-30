#include "TraxFolderPopUp.h"

/*
 *       Class: TraxFolderPopUp
 * Description: An extension of the FolderPopUp class, to handle TraX specific requirements
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

#include <StorageKit.h>

#include <stdio.h>
#include <string.h>

TraxFolderPopUp :: TraxFolderPopUp( BRect r , const char* n , const char * l )
: 	inherited( r, n, l )
,	fnVolumes(0)
{
}

TraxFolderPopUp :: TraxFolderPopUp( BRect r , const char* n )
: inherited( r, n )
,	fnVolumes(0)
{
}
	
void TraxFolderPopUp :: InitFolders( const entry_ref * ref )
{
	this->AddFolder( "/" ) ;

	BPath path ;

	LoadVolumes() ;

	this->AddSeparator( ) ;
	
	if( find_directory( B_USER_DIRECTORY, &path ) == B_OK )
		this->AddFolder( &path ) ;
	if( find_directory( B_DESKTOP_DIRECTORY, &path ) == B_OK )
		this->AddFolder( &path ) ;
	if( find_directory( B_BEOS_DIRECTORY, &path ) == B_OK )
		this->AddFolder( &path ) ;

	entry_ref opt_ref ;
	if( get_ref_for_path( "/boot/optional/", &opt_ref ) == B_OK )
		this->AddFolder( "/boot/optional" ) ;

	this->AddSeparator( ) ;
	this->AddChoice( "other" B_UTF8_ELLIPSIS ) ;
	
	if( ref )
		this->SetChoice( ref ) ;

}

void TraxFolderPopUp :: AttachedToWindow( void )
{
	inherited::AttachedToWindow() ;
	SetViewColor( Parent()->ViewColor() ) ;

	BMessenger msgr( this, Looper() ) ;
	fVolumeRoster.StartWatching( msgr ) ;
}

void TraxFolderPopUp :: MessageReceived( BMessage * msg )
{
	switch( msg->what )
	{
		case B_NODE_MONITOR:
		{
			int32 op ;
			if( msg->FindInt32( "opcode", &op ) != B_OK )
				break ;
			
			if( !HandleNodeMonitor( op, msg ) )
				inherited::MessageReceived( msg ) ;
			break ;
		}
		default:
		{
			inherited::MessageReceived( msg ) ;
			break ;
		}
	}
	return ;
}

bool TraxFolderPopUp :: HandleNodeMonitor( uint32 opcode, BMessage * msg )
{
	switch( opcode )
	{
		case B_DEVICE_UNMOUNTED:
		case B_DEVICE_MOUNTED:
			ClearVolumes() ;
			LoadVolumes() ;
			return true ;


/* This logic fails. The easily solution is to just re-read all volumes each time.
   Since anyone with over 10 volumes is going to despise the popup anyway, this
   isn't going to hurt too badly */

/*		{
			node_ref node ;
			msg->FindInt32("new device", &node.device) ;
			msg->FindInt64("directory", &node.node) ;
			
			BDirectory dir ;
			status_t err = dir.SetTo( &node ) ;
			if( err != B_NO_ERROR )
			{
				printf( "Cannot SetTo dir\n" ) ;
				return true ;
			}
				
			
			BEntry ent ;
			err = dir.GetEntry( &ent ) ;
			if( err != B_NO_ERROR )
			{
				printf( "Cannot GetEntry of dir: %s\n", strerror(err) ) ;
				return true ;
			}
			
			BPath path ;
			err = path.SetTo( &ent ) ;
			if( err != B_NO_ERROR )
			{
				printf( "Cannot SetTo path\n" ) ;
				return true ;
			}

			printf( "Adding %s\n", path.Path() ) ;

			// Increment before adding. You do the math... (Don't forget the "/")
			fnVolumes++ ;
			this->AddFolder( &path, fnVolumes ) ;
			return true ;
		}
		case B_DEVICE_UNMOUNTED:
		{
			entry_ref ref ;
			for( uint32 i = 1 ; i <= fnVolumes ; i++ )
			{
				if( !RefAt( i, &ref ) )
					continue ;
				BEntry ent( &ref ) ;
				if( ent.InitCheck() != B_OK )
				{
					RemoveFolder(i);
					fnVolumes -- ;
					i-- ;
					continue ;
				}
			}
			return true ;
		}*/
		
		default:
		{
			return false ;
		}
	}
	return false ;
}

void TraxFolderPopUp :: LoadVolumes( void )
{
	BVolume vol ;
	BDirectory dir ;
	BEntry ent ;
	BPath path ;

	fVolumeRoster.Rewind() ;
	while( fVolumeRoster.GetNextVolume( &vol ) == B_OK )
	{
		if( vol.IsPersistent() )
		{
			vol.GetRootDirectory( &dir ) ;
			dir.GetEntry( &ent ) ;		
			path.SetTo( &ent ) ;

			// Increment before adding. You do the math... (Don't forget the "/")
			fnVolumes++ ;
			this->AddFolder( &path, fnVolumes ) ;
		}
	}
}

void TraxFolderPopUp :: ClearVolumes( void )
{
	entry_ref ref ;
	while( fnVolumes )
	{
		RemoveFolder(1);
		fnVolumes -- ;
	}
}
