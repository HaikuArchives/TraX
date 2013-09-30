#ifndef TRAXWINDOW_H
#define TRAXWINDOW_H

/*
 *       Class: TraxWindow
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

#include <Window.h>

#ifndef   FOLDERPANEL_FWD
#include "FolderPanel.fwd"
#endif

#ifndef   OPTIONSPANEL_FWD
#include "OptionsPanel.fwd"
#endif

#ifndef   SETTINGS_H
#include "Settings.h"
#endif

class TraxWindow : public BWindow
{
	typedef BWindow inherited ;
	public:
		TraxWindow( entry_ref * = NULL , const uint32 n = 0 ) ;

		bool QuitRequested( void ) ;
		void MessageReceived( BMessage * ) ;

		virtual void Show( void ) ;
		
	private:
	
		void ReshufflePanels( void ) ;
		void ResizeHeight( void ) ;

		BList fPanels ;
		FolderPanel * fpFolderPanel ;
		OptionsPanel * fpOptionsPanel ;
		
		static BWindow * spSettingsWindow ;
		static TraxSettings sSettings ;
} ;

#endif