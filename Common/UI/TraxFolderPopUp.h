#ifndef TRAXFOLDERPOPUP_H
#define TRAXFOLDERPOPUP_H

#ifndef TRAXFOLDERPOPUP_FWD
#define TRAXFOLDERPOPUP_FWD
#endif

/*
 *       Class: TraxFolderPopUp
 * Description: An extension of the FolderPopUp class, to handle TraX specific requirements
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

#ifndef   FOLDERPOPUP_H
#include "FolderPopUp.h"
#endif

#include <VolumeRoster.h>

class TraxFolderPopUp : public FolderPopUp
{
	typedef FolderPopUp inherited ;
	public:
				TraxFolderPopUp( BRect, const char*, const char* ) ;
				TraxFolderPopUp( BRect, const char* ) ;
	
		virtual	void	InitFolders( const entry_ref * = NULL ) ;
		virtual	void	MessageReceived( BMessage * ) ;
		virtual	void	AttachedToWindow( void ) ;
		
	private:
		virtual bool	HandleNodeMonitor( uint32, BMessage * ) ;
				void	LoadVolumes( ) ;
				void	ClearVolumes( ) ;
		
		BVolumeRoster	fVolumeRoster ;
		uint32			fnVolumes ;
} ;

#endif
