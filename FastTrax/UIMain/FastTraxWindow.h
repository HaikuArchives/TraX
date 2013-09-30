#ifndef FASTTRAXWINDOW_H
#define FASTTRAXWINDOW_H

/*
 *       Class: FastTraxWindow
 *   Copyright: Tim Vernum 1999, All rights reserved
 *     Licence: GPL
 */

#include <Window.h>

#ifndef   TRAXFOLDERPOPUP_FWD
#include "TraxFolderPopUp.fwd"
#endif

class BButton ;
class BView ;
class BTextControl ;
class BCheckBox ;
class StringPopUp ;
class SettingsWindow ;

class FastTraxWindow : public BWindow
{
	public:
		 FastTraxWindow( entry_ref * = NULL ) ;
		~FastTraxWindow( void ) ;

		bool QuitRequested( void ) ;
		void MessageReceived( BMessage * ) ;

	private:
	
		signed char	GetTime( const char *, time_t, time_t *  ) ;
	
		BButton			* fFindButton ;
	
		BView			* fNameView ;
		BView			* fFileView ;
		BView			* fDateView ;
			
		BButton     	* fNameButton ;
		BButton     	* fFileButton ;
		BButton     	* fDateButton ;

		BView       	* fCurrentView ;
		BButton     	* fDisabledButton ;

		TraxFolderPopUp	* fpFolder ;
		BTextControl	* fpName ;
		BCheckBox		* fpSubDirs ;

		BTextControl	* fpContaining ;
		BTextControl	* fpSizeGT ;
		BTextControl	* fpSizeLT ;
		StringPopUp		* fpKind ;

		BTextControl	* fpModAfter ;
		BTextControl	* fpModBefore ;
		BTextControl	* fpCreAfter ;
		BTextControl	* fpCreBefore ;
		
		char 			* fNameStr ;
		char 			* fContainsStr ;
		
		SettingsWindow	* fSettingsWindow ;
} ;

#endif