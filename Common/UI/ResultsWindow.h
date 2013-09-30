#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

/*******************************************************
 * (c) 1999, Tim Vernum <zodsoft@kagi.com>
 * All rights reserved
 *******************************************************
 * File   : ResultsWindow.h
 * Author : Tim Vernum
 * Project: Trax Application
 * Purpose: Window that displays results of a Trax search
 * Licence: GPL
 *******************************************************/

#ifndef   RESULTSITEM_FWD
#include "ResultsItem.fwd"
#endif

#include <Window.h>

class ResultsList ;
class BListItem ;

class ResultsWindow : public BWindow
{
	public:
						 ResultsWindow( void ) ;
						~ResultsWindow( void ) ;
		
		static	int 	AddResult( const char * ) ;
				void 	AddFile( const char * ) ;
				
				bool	QuitRequested( void ) ;
				void	MessageReceived( BMessage * ) ;
	
	private:
				void	ItemInvoked( int32 ) ;
				void	DisplayFile( ResultsItem * ) ;
				void	LaunchFile( ResultsItem * ) ;

		ResultsList	*	fpList ;
		static	ResultsWindow *	spCurrentWindow ; 
} ;


#endif