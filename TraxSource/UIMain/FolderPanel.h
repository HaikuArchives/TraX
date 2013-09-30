#ifndef FOLDERPANEL_H
#define FOLDERPANEL_H

/*
 *       Class: FolderPanel
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

#ifndef   TRAXFOLDERPOPUP_FWD
#include "TraxFolderPopUp.fwd"
#endif

#include <View.h>
#include "Panel.h"

class FolderPanel : public Panel
{
	typedef Panel inherited ;
	public:
		FolderPanel( void ) ;
		FolderPanel( const entry_ref * ) ;

		bool	GetRef( entry_ref * ) ;
		bool	GetPath( BPath * path ) ;

		void	AttachedToWindow( void ) ;
		void	Draw( BRect ) ;
		void	MessageReceived( BMessage * ) ;
		
	private:
		void	Create( const entry_ref * ) ;
		void	StartFind( void ) ;
		
		TraxFolderPopUp	*	fpFolderPU ;
} ;

#endif
